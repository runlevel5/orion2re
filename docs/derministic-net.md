# Deterministic multiplayer state and consistency plan

## Purpose

This document describes the multiplayer state machine as it exists, the ways it can diverge or stall, and a practical path to explicit state consistency guarantees. It also proposes a transparent state oracle that compares each client with the gameplay host at deterministic checkpoints.

The analysis covers gameplay synchronization, not the router protocol itself. Transport and session architecture are described in `docs/networking.md`.

The current implementation should be treated as a single-threaded program with logical concurrency. Router I/O is queued, but packet callbacks run synchronously on the game thread when game code pumps the network. A "race" in this document therefore usually means callback ordering, re-entrant dispatch, or observing partially updated state rather than two CPU threads writing the same object. This assumption should be asserted in code; the mutable networking globals are not thread-safe.

## Source map

The important layers are:

| Area | Primary code | Responsibility |
| --- | --- | --- |
| Transport adapter | `netadapter.cpp`, `netcode.cpp` | Router connection, packet queues, send, poll, disconnect events |
| Gameplay dispatcher | `netmox.cpp` | Message dispatch, periodic message scheduler, snapshots, orders, dropout handling |
| Startup | `netstart.cpp`, `mainscr.cpp` | Join/create, slot/race selection, initial state transfer |
| Strategic turn | `netturn.cpp`, `mainscr.cpp` | Host election, order collection, host simulation, state broadcast |
| Combat | `combfind.cpp`, `cmbtmov1.cpp`, `mainscr.cpp` | Combat target selection, tactical and strategic barriers, results |
| Diplomacy | `netmox.cpp`, `dip_scrn.cpp`, `mainscr.cpp` | AI and human diplomacy handshakes and UI synchronization |
| Shared state | `orion2.h`, `orion2_consts.h` | `s_network`, `NET_SYNC_STATE`, `NET_PLAYER_STATE` |

There are two overlapping state machines:

- `s_network::sync_state` is the local process's coarse phase.
- `s_network::player_net_info[i].player_state` is a local view of each player's combat or diplomacy phase.

They are supplemented by counters, bit masks, flags, global pointers, and subsystem-specific latches. Those values are part of the effective state machine even though transitions are not centralized.

## Executive summary

The existing model is host-coordinated state replication:

1. Human players submit strategic orders.
2. The elected gameplay host merges those orders and runs `NEXTTURN::Next_Turn_()`.
3. Combat is coordinated through additional peer messages and barriers.
4. The host serializes authoritative world state and broadcasts either a full snapshot or a differential.
5. Clients overwrite their local world state from that payload.

This can work reliably, but the current code does not enforce the protocol assumptions on which it depends. In particular:

- Receiving a packet immediately mutates gameplay state inside the network pump.
- Sending may pump the network and may synchronously dispatch a host-local packet.
- Phase changes are assignments spread across UI, packet handlers, combat, diplomacy, and turn code.
- Most packets have no epoch, turn, transaction, sequence, expected sender, or expected phase.
- Shared single-slot storage such as `network_data_ptr` has no message identity or ownership object.
- Counters and flags generally are not duplicate-safe.
- Wait loops are unbounded and often rely on incidental pumping by UI or sound update calls.
- Differential snapshots use one implicit global baseline.
- The existing snapshot is useful state replication, but raw structures, omitted state, and local fields make it unsuitable as a rigorous oracle format.

The most important architectural change is not "allow" versus "forbid" recursive calls. Transport polling, packet dispatch, sending, waiting, and UI progress must be separated. A packet handler must run to completion without another packet handler entering it. Calls made during a handler should only enqueue work for the outer dispatcher. Any handler that waits for another packet must become an explicit continuation/state transition rather than a blocking loop.

## Current execution model

### Pump and callback chain

The normal chain is:

```text
UI/input/wait loop
  -> RUSS::Mox_Update_()
    -> NETMOX::Mox_Net_Update_()
      -> netcode::Net_Update_()
        -> Pump_Router_Packets_()
          -> registered receive callback
            -> NETMOX::Receive_Message_()
              -> strategic, combat, or diplomacy handler
```

`netcode::Net_Update_()` has `_inComms`, and `NETMOX::Mox_Net_Update_()` has `_is_updating` and `_in_receive_message`. Snapshot and order serialization also use `_suspended_net_update_depth`. These guards reduce recursion but do not define a complete execution contract:

- Some `netcode` queries and send/wait paths call the router pump directly.
- `NETMOX::Mox_Send_Message_()` calls `Update_()` before sending.
- A send to the local gameplay host calls `Receive_Message_()` synchronously instead of following the same queued path as a remote send.
- `_in_receive_message` is a Boolean-like global, not a nesting depth or scoped dispatcher state. A nested local receive can clear an outer receive's guard.
- `_sending_msg`, `_is_updating`, `_in_receive_message`, `_inComms`, and `_in_send_message` describe call-stack accidents rather than protocol state.

### Why making update non-reentrant changed behavior

Commit `3291064b` added scoped update suspension around snapshot/order encode and decode. It fixed a real race: calls to `RUSS::Mox_Update_()` inside serializers could dispatch packets and mutate the same world state while it was being copied. It also moved the client's `network_data_ptr` reset to before `TURN_END_REQUEST`, preserving a final game-data payload that can arrive early during combat.

The immediate follow-up `a685c4f0` restored client startup decoding that had been skipped, demonstrating the underlying coupling: legacy code often uses "update" both as harmless UI progress and as the operation that advances a required protocol handshake. Suppressing update at one call site can prevent a needed packet transition; allowing it can mutate data in the middle of an operation.

The correct long-term contract is:

- UI, audio, rendering, and transport polling may continue independently.
- Received bytes are queued by polling.
- Gameplay packets are dispatched only from an outer, non-reentrant dispatch point.
- A serializer observes an immutable captured state, not live globals that callbacks can mutate.
- A send only queues/sends bytes; it never dispatches inbound gameplay callbacks.
- A handler never waits synchronously for a packet that can only be processed after that handler returns.

## Reconstructed state machine

### Coarse lifecycle

```text
IDLE
  | create                         | join
  v                                v
HOST_STARTUP                    CLIENT_JOINING
  v                                |
HOST_ASSIGN_PLAYERS                +-> CLIENT_NO_AVAILABLE_SLOT -> IDLE
  v                                +-> CLIENT_SELECT_RACE
HOST_WAITING_FOR_PLAYER_SETUP      +-> CLIENT_PICK_POSITION
  |                                v
  +---------- initial full state -> WAITING_FOR_GAME_DATA
                                   v
                                 READY
```

Host setup generates the game and broadcasts a full state. A joining client stores it in `network_data_ptr`, decodes it, and enters `READY`. Load-game startup follows a parallel `LOAD_GAME_HOST_SETUP` path.

The startup protocol has no explicit startup transaction ID. Delayed or duplicate startup packets can overwrite selections or move a process into a startup phase after it has advanced.

### Strategic turn

At the end of a turn, every peer enters `NEXT_TURN_READY`, marks its local `players_hit_next_turn` entry, and exchanges `NET_PACKET_HOST_ELECTION_RANK`.

`Determine_Lowest_Active_Host_Player_()` actually chooses the lowest non-eliminated human player slot with a valid network ID. The exchanged rank does not participate in that selection. Legacy names are confusing: `net_host_id` identifies this process's network ID, while `local_net_id` is set to the elected gameplay host ID for the turn. A process acts as host when those IDs are equal.

The intended turn flow is:

```text
All peers: NEXT_TURN_READY
  |
  +-> elected host: HOST_WAITING_FOR_ORDERS
  |       |
  |       +-> for each ready remote human:
  |       |     HOST_RECEIVING_PLAYER_DATA
  |       |       -> request orders
  |       |       -> receive one payload
  |       |       -> decode payload
  |       |
  |       +-> HOST_BEGIN_TURN
  |       +-> COMBAT_READY
  |       +-> NEXTTURN::Next_Turn_()
  |       +-> combat/diplomacy sub-protocols
  |       +-> broadcast authoritative state
  |       +-> READY
  |
  +-> clients:
          send TURN_END_REQUEST
          wait until host requests orders
          send GAME_ORDERS_PAYLOAD
          run Client_Combat_Loop_()
          wait for GAME_DATA_PAYLOAD
          decode authoritative state
          READY
```

The host's readiness is represented by all of the following:

- `players_hit_next_turn[]`
- `player_is_active_flags[]`
- `host_active_player_id`
- `host_sync_counter`
- `players_ready_mask[]`
- `sync_state`

There is no single authoritative collection of expected participants or accepted order submissions.

### Combat

At turn resolution, all `player_net_info[].player_state` entries are reset to `COMBAT_READY`, and coarse state becomes `COMBAT_READY`. The host repeatedly chooses combat work and moves participants through states such as:

```text
COMBAT_READY
  -> PICK_TARGET_INFO / PICKING_COMBAT_TARGET
  -> COMBAT_TARGET_CHOSEN / COMBAT_TARGET_SENT
  -> COMBAT_RESOLUTION_DISPATCH
  -> tactical attacker/defender or strategic attacker/defender
  -> result received/display/ack
  -> COMBAT_READY
  -> COMBAT_COMPLETE
```

Combat readiness is a distributed barrier inferred from each process's local `player_net_info[]`. Tactical combat adds `NET_TACTICAL_COMBAT_SYNC_STATE` and `_combat_done_waiting`. Action reception uses single global pending-message storage. `Client_Combat_Loop_()` and target-search code publish pointers to stack-owned scratch data in globals while repeatedly pumping the network.

The protocol assumes every peer sees target selection, tactical completion, result application, and acknowledgements in the expected order. Packet handlers generally do not verify that order.

### Diplomacy

Human diplomacy overlays the strategic state machine:

```text
READY
  -> HUMAN_DIPLOMACY_REQUESTED
  -> HUMAN_DIPLOMACY_CONNECTING
  -> HUMAN_DIPLOMACY_READY
  -> READY

REQUESTED -> REJECTED -> READY
REQUESTED/CONNECTING -> BLOCKED or cancellation paths
```

The requester and receiver exchange request/accept/ready data and then the diplomacy UI runs its own handshake using global message and latch variables. AI diplomacy has a separate requested/cancel-pending/done/ready sequence.

These are not transaction-scoped. A delayed handshake, goodbye, response, or ready packet from one negotiation can satisfy or overwrite a later negotiation.

### Disconnect and authority recovery

The router emits `EVENT_MEMBER_LEFT`. Game code maps the departed network ID to a player and calls `Handle_Network_Player_Disconnected_()`.

The intended behavior is:

1. Remove the departed human from the connected candidate set.
2. Deterministically choose the lowest remaining connected human player slot as recovery coordinator.
3. The coordinator broadcasts game-level dropout state.
4. Every peer converts the same empire to AI and updates host/election bookkeeping.

This is compatible with the router design in `docs/networking.md`, but the operation is not currently a transaction. Duplicate local events or dropout packets can repeat counters and state changes. Session-closed/disconnected router events also do not consistently transition the gameplay state to an explicit terminal/recovery state.

## Current consistency boundaries

### Full and differential state

`Encode_Broadcast_Game_Data_Snapshot_()` currently serializes:

- gameplay host election ID and stardate
- selected game settings
- main RNG seed
- players, stars, planets, colonies, ships, and leaders
- reports, events, and player messages
- council state
- Antaran state
- nebulas and event timing
- `_bill_savegame`

Recent fixes show why field coverage matters:

- `6cd496cc` fixed missing technology state.
- `bfd53f9f` fixed leader deassignment synchronization.
- `1f7b3689` addressed the separate Artemis random stream.

The snapshot is both replication data and the implicit differential baseline. The baseline is one global cache with no epoch, checkpoint sequence, source, schema ID, base hash, or explicit ownership.

### What the snapshot does not guarantee

Applying the host snapshot makes many strategic structures equal, but it is not proof of deterministic execution:

- Some deterministic globals and subsystem state are not serialized.
- Combat-in-progress state is intentionally outside the strategic checkpoint.
- Raw structures include padding, legacy undefined fields, local network IDs, and inactive tails.
- Counts and copied extents are inconsistent: some arrays copy live entries and others copy full capacity.
- A client can diverge before the broadcast and then be silently corrected.
- A shared omission can exist on host and client without appearing in snapshot bytes.
- No acknowledgement proves that every client decoded the same checkpoint.

The existing snapshot encoder is a useful source list, but an oracle needs a canonical semantic representation independent of the replication encoding.

## Findings and failure scenarios

### Critical: re-entrant mutation and call-order dependence

1. `Mox_Send_Message_()` pumps before send. A caller preparing a transition can receive and apply an unrelated packet before its outbound packet is sent.
2. A host-local send dispatches synchronously, while a remote send is queued. Identical gameplay actions therefore have different call-stack and ordering semantics depending on which participant is host.
3. A nested `Receive_Message_()` can clear the outer `_in_receive_message` flag because it is not scoped as a depth. The outer handler can then unexpectedly pump.
4. `RUSS::Mox_Update_()` calls inside encode/decode, combat builders, and gameplay loops look harmless but can dispatch state-changing callbacks unless a separate guard happens to be active.
5. Shared screen-arena allocations and globally published stack pointers remain live across update calls. Re-entry can overwrite scratch state, invalidate LIFO allocation assumptions, or observe a half-built packet.

The scoped update suspension is a necessary containment fix, not a complete model. Extending suppression to more functions risks more handshake regressions because progress and dispatch are still coupled.

### Critical: packets are not bound to protocol transactions

Most gameplay packets do not carry or validate:

- session epoch
- turn number
- phase or transaction kind
- transaction ID
- sender player slot and network identity binding
- per-sender sequence number
- expected prior state
- differential base checkpoint

Examples:

- Any accepted `GAME_DATA_PAYLOAD` replaces the single receive pointer and resets election bookkeeping.
- `GAME_ORDERS_PAYLOAD` increments `host_sync_counter` without proving it came from `host_active_player_id` or that this player has not already submitted.
- `COMBAT_PHASE_COMPLETE` unconditionally changes the coarse phase.
- Combat result and acknowledgement packets can overwrite state outside the combat they describe.
- Diplomacy handshake flags can be satisfied by a stale packet.
- Dropout packets are not idempotent operations tied to a membership epoch.

Reliable ordered TCP delivery does not fix phase confusion. Packets from different peers can interleave, local loopback takes a different path, disconnect events race logical phase changes, and old queued packets can still be valid transport messages but invalid gameplay messages.

### Critical: validation occurs after trust, or not at all

Several handlers trust lengths, indices, counts, and senders before mutation. Representative risks include:

- `PLAYER_SETUP` indexes arrays with `from_player_id` and copies `payload_len` bytes into one `s_player`.
- Startup and pick-position payloads trust player counts and target indices.
- Combat handlers trust attacker, defender, star, planet, colony, ship, and player indices in multiple paths.
- Tactical receive ignores or incompletely uses message size.
- Disconnect/dropout handling validates little beyond outer packet shape and player range.

Malformed packets are a memory-safety concern, but ordinary stale data can trigger the same invalid transitions. Validation must happen before allocation, counter changes, pointer replacement, or world mutation.

### High: non-idempotent barriers and counters

- Duplicate order payloads increment `host_sync_counter` more than once and can start a turn without all players.
- `network_data_ptr` is a single mailbox. A second payload can overwrite ownership of the first allocation.
- Disconnect/dropout paths decrement connected-player counts and adjust host counters without an explicit "already applied for this epoch" record.
- Periodic scheduler flags are often cleared before delivery is known to be valid. If a target has disconnected or a send fails, the state transition is lost.
- Readiness masks mix persistent participant state with one-shot work scheduling.

Every barrier should derive from a set keyed by stable participant ID, not an increment-only counter. Receiving the same acknowledgement twice must have the same result as receiving it once.

### High: implicit differential baseline

The global snapshot cache assumes exactly one ordered stream of checkpoints. There is no way to reject:

- a differential whose base full snapshot was missed
- a duplicate differential
- an old differential after host recovery
- a differential generated by another gameplay host
- a payload from another schema/build

Decoding against the wrong baseline can produce valid-looking but incorrect state. A differential must name its base checkpoint and base hash, and decoding must create a new checkpoint atomically rather than mutate the sole cache before validation completes.

### High: unbounded waits and incomplete disconnect transitions

Startup, order collection, combat, diplomacy, tactical synchronization, send completion, and final state waits can run forever. Many loops only test one state variable and assume another callback will eventually change it.

Failure scenarios include:

- The expected sender disconnects while the host is receiving that player's orders.
- A diplomacy accept path waits while dispatch is suppressed by an outer receive callback.
- A tactical action is overwritten in the single pending-message slot.
- Router/session loss is logged but does not move the gameplay protocol to terminal recovery.
- One peer rejects an invalid packet while other peers continue, leaving different barriers.

Every wait needs a deadline and an explicit outcome: retry an idempotent request, apply deterministic dropout, request a full checkpoint, abort the transaction, or end the session.

### High: distributed combat state and borrowed storage

Combat uses coarse state, per-player state, tactical state, global latches, and borrowed scratch pointers. A late packet can mutate a newer combat because combat messages do not identify a combat transaction. Single-slot action/result storage can lose messages. Pointers to stack objects published globally are safe only if dispatch order exactly matches current assumptions.

Combat needs a stable `combat_id`, owned transaction storage, participant roles, expected sequence, and an explicit completion barrier.

### Medium: host election semantics are implicit

Election rank packets are exchanged, but host selection uses lowest active player slot. That rule is deterministic if all peers have identical connected-player sets, but the implementation should state it directly and include a membership epoch. Otherwise two peers that process disconnect and election messages in different orders can choose different hosts.

Authority should be represented as `(membership_epoch, authority_player, authority_term)`. A payload from an old authority term must be rejected even if its network ID is otherwise valid.

### Medium: raw-byte state and uninitialized values

Raw structure serialization and comparison can include padding and stale inactive entries. Diplomacy handshake construction also leaves a field uninitialized. Such bytes can differ across runs, builds, compilers, or allocation histories without a semantic game difference, and can poison differential efficiency or hashes.

Canonical serialization must write named fields in fixed-width form, normalize local-only values, zero reserved fields, and define whether inactive slots are excluded or represented as zero.

### Medium: authoritative overwrite hides divergence

Clients receive an authoritative strategic state each turn. This repairs many differences but makes defects hard to locate: the first visible symptom can occur turns after the original divergent decision, and successful overwrite provides no record that client simulation differed beforehand.

The oracle should compare both:

- pre-apply client state where peers are expected to have independently reached the same result; and
- post-apply state, which verifies replication and decoding.

Pre-apply comparison may initially be diagnostic because some current phases intentionally differ. Post-apply comparison can become a hard guarantee at strategic checkpoints.

## Required consistency guarantees

The refactored system should make these contracts enforceable:

1. **Single writer:** Gameplay and protocol state are mutated only on the game thread.
2. **Non-reentrant dispatch:** At most one gameplay packet handler is on the stack. Nested pump requests defer work to the outer dispatcher.
3. **No implicit dispatch from send:** Sending never runs inbound gameplay callbacks.
4. **Validate before mutation:** Envelope, length, sender, indices, epoch, phase, sequence, and payload are validated before changing state or allocating protocol-owned storage.
5. **Explicit transactions:** Turn, combat, diplomacy, startup, and dropout messages name their transaction.
6. **Idempotence:** Duplicate messages and duplicate disconnect events do not advance counters or apply world changes twice.
7. **Monotonic authority:** Authority terms and checkpoint sequences only advance. Old-authority packets are rejected.
8. **Atomic checkpoints:** Snapshot capture and apply either complete successfully or leave the prior state and baseline unchanged.
9. **Bounded progress:** Every wait has timeout, retry, recovery, and terminal behavior.
10. **Checkpoint agreement:** Every connected client reports the same canonical post-apply state hash as the host before a checkpoint is committed.
11. **Diagnosable failure:** A mismatch report identifies checkpoint, component, first differing field when available, recent protocol events, build/schema, and RNG state.

## Minimal first version

The first version should enforce one execution boundary and add enough packet identity to reject old-turn work. It does not require a general transaction framework or an application-level outgoing queue.

### One network dispatch boundary

The adapter already queues packets, but the gameplay layer is not polling-only. `netadapter::Pump()` fills adapter queues; `netcode::Pump_Router_Packets_()` immediately drains them and invokes the gameplay receive callback in the same call. `netcode::Net_Send_Message_()` also calls that combined pump after sending.

Keep `RUSS::Mox_Update_()` and `NETMOX::Mox_Net_Update_()` as the public progress points used by the main screen, startup, combat, diplomacy, and explicit wait loops. Internally split progress into:

```text
Poll_Transport()       move bytes/events into queues; never call gameplay
Dispatch_Game_Events() invoke gameplay handlers from one outer dispatch loop
Send_Message()         submit bytes; never poll or dispatch inbound events
```

`Dispatch_Game_Events()` is the only function allowed to invoke `Receive_Message_()`, disconnect handling, or another gameplay network handler. It should have a scoped `dispatch_active` guard:

- An outer `Mox_Net_Update_()` polls and dispatches a bounded batch.
- A nested `Mox_Net_Update_()` while a handler is active does not dispatch. It may set `dispatch_requested` and return.
- After the current handler returns, the outer dispatcher may process the next queued event.
- Sound, rendering, and UI updates remain independent of gameplay dispatch.

This boundary does not prevent diplomacy. Incoming diplomacy packets currently update request masks/state and return; the main-screen tick subsequently opens the request UI. If the player accepts, the acceptance wait calls `Mox_Update_()` from ordinary UI/game code, outside a receive handler, so matching responses can be dispatched normally. Any path that actually waits while still inside a receive handler must instead record pending state and return.

The dispatcher should be called from the existing outer update points rather than added independently throughout gameplay:

- normal main-screen update ticks
- `Chat_Box_Input_Loop_()` ticks used by turn waits
- startup wait ticks
- combat and diplomacy wait/UI ticks

Calls from all of those places are safe because the dispatcher itself, rather than every caller, enforces non-reentrancy.

### Sends remain immediate

Gameplay sends do not need to be gated behind a new application output queue. The router client already owns any transport buffering needed by its send API. The minimal rule is only that sending cannot cause inbound gameplay work.

Therefore:

- remove the pre-send `Update_()` behavior from `Mox_Send_Message_()`
- remove the post-send `Pump_Router_Packets_()` call from `Net_Send_Message_()`
- replace synchronous local-host `Receive_Message_()` with insertion into the same inbound event queue used for remote packets
- return an immediate send result so failed sends do not silently consume one-shot scheduler flags

Local and remote delivery must have the same ordering semantics. An outbound sequence or turn stamp is assigned when the message is constructed, before the immediate send.

### Minimal turn stamp

Use a logical `turn_id` in the gameplay packet envelope. It names the turn whose activity the packet belongs to; it is not necessarily the stardate contained in the resulting snapshot.

For example, orders close turn `T`. The host resolves them, advances the world stardate, and sends a state payload stamped `resolved_turn=T` whose body contains the resulting stardate. This avoids interpreting a state payload as a future packet merely because simulation already advanced its stardate.

Initially classify packets as follows:

| Packet class | Turn rule |
| --- | --- |
| Strategic turn control and orders | Must equal the active turn being closed |
| Authoritative end-turn state | `resolved_turn` must equal the client's pending turn |
| Combat and state-changing diplomacy | Must equal the current active turn and be legal in the current phase |
| Startup/load/session negotiation | Not turn-stamped; legal only in startup/load states |
| Chat | Not turn-gated; sender must still be a current session member |
| Disconnect/dropout | Membership/session-scoped, not turn-gated, and idempotent |

On receipt and again immediately before processing, record and check:

- packet turn
- local turn when received
- local turn when processed
- current coarse phase
- authenticated network sender and mapped player slot
- packet disposition: accepted, duplicate, stale, future, wrong phase, wrong sender, or malformed

A packet can arrive before a turn advances but remain queued until afterward. Processing-time validation must reject and log it as stale. Do not retain arbitrary future packets; log and reject them because they usually indicate authority or state divergence. The authoritative end-turn payload is the explicit exception described above.

### Checks in the minimal packet matrix

Validation should be a table keyed by message kind rather than scattered conditionals. Each entry defines only concrete checks that the current protocol can support:

1. Minimum, maximum, or exact payload length before any cast or allocation.
2. Allowed coarse state or small set of states.
3. Turn policy from the table above.
4. Sender role: gameplay host, requested player, diplomacy counterpart, combat participant, or any session member.
5. Sender network ID to player-slot binding.
6. Payload indices and counts against live and capacity bounds.
7. Duplicate rule and the state used to recognize an already applied message.

Packets lacking a clearly defined rule should initially be logged in audit mode, then moved to enforced mode once their real call paths are covered by tests. Invalid packets must be rejected before pointer replacement, allocation ownership changes, counter updates, or world mutation.

### Duplicate-safe turn barrier

For host order collection, replace the free-running meaning of `host_sync_counter` with masks for the active turn:

- `expected_orders_mask`: connected remote humans whose orders are required
- `requested_orders_mask`: players from whom the host has requested orders
- `applied_orders_mask`: players whose one valid payload has been accepted and decoded

An order payload is accepted only when:

- its turn equals the active closing turn
- its authenticated sender maps to the player encoded by the payload
- that player is in both the expected and requested masks
- that player's applied bit is not set

Set the applied bit as part of accepting the owned payload, before any nested progress is possible. Completion is `(applied_orders_mask & expected_orders_mask) == expected_orders_mask`; AI, local, and already eliminated players are not pre-counted as synthetic responses.

A duplicate `TURN_END_REQUEST` only keeps a readiness bit set. It cannot re-arm a player whose order is already applied. If a player disconnects, the idempotent dropout path removes that player from `expected_orders_mask` and reevaluates completion.

For disconnect handling, maintain session-lifetime masks keyed by player slot:

- membership departure observed
- dropout/AI conversion applied
- coordinator announcement sent

The session does not support slot reuse, so a per-session mask is sufficient. Both a router member-left event and a game dropout packet may describe the same departure; state conversion and counters occur once. A new network session clears the masks. This is the minimal meaning of making barriers and dropout handling idempotent.

### What "transaction" means and what can be deferred

A transaction ID only distinguishes repeated instances of a multi-message activity. It is not required as a general abstraction in the first version.

- There is one strategic resolution per turn, so `turn_id` is sufficient for orders and the resulting checkpoint.
- There can be multiple combats in one turn. A future monotonically increasing `combat_id` prevents a late result or tactical action from an earlier combat in the same turn from affecting a later one.
- There can be multiple or crossed diplomacy attempts in one turn. A future `diplomacy_id`, together with initiator and target, prevents an old accept, cancel, ready, or handshake packet from satisfying a later negotiation.

For the minimal version, combat and diplomacy use turn, expected participants, current phase, and detailed logging. Add their IDs when those packet families are hardened. Do not introduce a generic transaction manager first.

## Later protocol hardening

### Common gameplay envelope

After the minimal turn-stamped protocol is stable, new or migrated gameplay messages may carry:

```text
protocol_version
session_id
membership_epoch
authority_term
turn_number
transaction_kind
transaction_id
sender_player
sender_sequence
message_kind
payload_length
payload_checksum
```

The router-provided network sender must map to `sender_player`; payload bytes must not be allowed to claim another player. `sender_sequence` is tracked per sender and transaction. Message-specific validation defines whether duplicates are ignored, acknowledged again, or treated as a protocol error.

### Explicit transaction objects

Replace scattered flags gradually with owned contexts:

- `Startup_Transaction`: expected members, selected slots, initial checkpoint.
- `Turn_Transaction`: authority, expected order owners, accepted-order set, resolution status, output checkpoint.
- `Combat_Transaction`: combat ID, star, participants, roles, action sequence, result, acknowledgements.
- `Diplomacy_Transaction`: initiator, target, negotiation ID, request/accept/ready/closed state.
- `Dropout_Transaction`: membership epoch, departed player, coordinator, applied set.

The first implementation can wrap legacy fields rather than replace them all. All assignments should go through a transition function that checks `(current state, event) -> next state`, records the transition, and updates compatibility fields.

### Queue and dispatcher

Use three distinct operations:

```text
Poll_Transport()       reads framed bytes and appends validated raw events
Dispatch_Game_Events() applies a bounded batch at a safe outer point
Flush_Outgoing()       submits queued sends without receiving callbacks
```

If `Dispatch_Game_Events()` is called while dispatching, set a `dispatch_requested` latch and return. The outer dispatcher can run another bounded pass after the current handler returns. This preserves prompt progress without recursive mutation.

Handlers that currently block while waiting for network state must return a pending result. The UI/main loop resumes them when a matching event advances their transaction. Sound and rendering updates remain callable without dispatching gameplay packets.

### Differential checkpoints

Each state payload should identify:

```text
checkpoint_sequence
full_or_differential
base_checkpoint_sequence
base_state_hash
result_state_hash
state_schema_id
```

A client accepts a differential only when it has the named base with the expected hash. Decode into temporary owned storage, validate the result hash, then atomically install both game state and the new baseline. On failure, request a full checkpoint; never attempt to decode repeatedly against a mutated cache.

## Transparent host/client state oracle

### Goals

The oracle should:

- require no player action during normal games
- detect divergence at the first stable boundary
- distinguish simulation divergence from snapshot transport/decode failure
- avoid false positives from pointers, padding, UI state, or local player identity
- provide enough detail to reproduce the failure
- be inexpensive enough to enable by default after a diagnostic rollout

It is a correctness oracle, not an anti-cheat system. A modified host remains authoritative, and a modified client can lie about its result unless stronger attestation is added.

### Canonical state format

Implement a separate `Canonical_Game_State_Writer`, even if it initially shares field-list helpers with snapshot serialization. It must serialize semantic fields explicitly in fixed order and fixed widths.

Rules:

- Never hash raw pointers or allocator metadata.
- Never hash structure padding or reserved bytes.
- Normalize transport-local `network_player_id` values to stable player slots or exclude them.
- Exclude UI, animation, audio, screen, chat, input, local selection, and packet-pump state.
- Include all state that can affect future authoritative decisions, including every RNG stream.
- Include active counts and active entries; either omit inactive tails or canonicalize them to zero.
- Encode enums and Booleans canonically.
- Include a `state_schema_id` derived from the ordered field schema, not `sizeof` values.

Produce a root hash plus component hashes, for example:

```text
meta/settings
rng
players
stars/planets
colonies
ships
leaders
diplomacy/council
events/reports/messages
antarans
combat (only at defined combat checkpoints)
```

A fast non-cryptographic hash is sufficient for continuous checks if collisions are acceptable diagnostically, but BLAKE3 or another portable 128/256-bit hash avoids ambiguity at modest checkpoint frequency. The wire protocol should name the algorithm.

### Checkpoint handshake

For checkpoint `N`:

1. The host captures immutable canonical state and replication state at the same safe boundary.
2. The host sends the full/differential game payload with checkpoint metadata and host component hashes.
3. A client decodes into temporary state and verifies the replication result hash before install.
4. The client atomically installs the checkpoint.
5. The client canonicalizes its installed live state and compares root/component hashes with the host.
6. The client sends `CHECKPOINT_RESULT(N, match, component_hashes)` to the host.
7. The host commits the barrier after every expected client reports a match or is deterministically removed by a dropout transaction.

For transparent operation, matching results need no UI. A mismatch should write a diagnostic bundle on both peers and display a concise session error rather than allowing silent continuation.

Recommended diagnostic bundle:

- session, membership epoch, authority term, turn, phase, checkpoint
- build ID and state schema ID
- host and client root/component hashes
- canonical field/value dump for differing components, generated locally
- last 256 protocol events and validated transitions
- current and recent RNG states/call counts
- differential base/result metadata
- optional synchronized save or canonical snapshot

Do not send an unrestricted memory dump; it is non-portable and may contain local data.

### Replay and direct discrepancy analysis

A hash reports disagreement but cannot by itself identify a field. The oracle should write a replayable checkpoint record using the same canonical field schema:

- checkpoint metadata and root/component hashes
- canonical field records identified by stable field path or numeric schema ID
- previous matching canonical checkpoint, or a reference to it
- every accepted gameplay packet after that checkpoint in dispatch order
- rejected-packet metadata and reason
- authoritative order payloads
- all deterministic RNG stream states and, where practical, call counts
- state-transition log with before/after phase and turn

On mismatch, both host and client preserve their canonical records. An offline comparator first finds the differing component, then the first differing canonical field and value. Replaying starts from the previous agreed checkpoint and feeds accepted events through the same non-reentrant dispatcher in recorded order. Per-event or selected-stage component hashes narrow the first divergent transition.

Canonical field records, component hashing, replication serialization, and replay must share one field inventory so a field cannot silently appear in only one diagnostic path. Full canonical records may be retained only around mismatches; normal checkpoints can keep hashes and a bounded event ring to control disk usage.

### Checkpoint locations

Start with boundaries where equality is already intended:

1. After initial new-game or load-game full state has been installed.
2. Immediately after each client applies the host's end-of-turn state and before returning to `READY`.
3. After deterministic dropout/AI takeover is committed.
4. After a full-state recovery/resynchronization.

Add finer checkpoints only after their equality contract is defined:

- before host state application, to expose independent simulation divergence
- after each combat transaction
- after diplomacy commit
- around AI processing stages and known random-sensitive systems

The strategic post-apply checkpoint should be the first hard guarantee. Pre-apply checks should initially log only because the current host/client paths intentionally execute different code in places.

### Mismatch policy and recovery

Phase 1 policy:

- log and preserve diagnostics
- notify all peers
- stop advancing the next authoritative transaction

Optional recovery after the cause is understood:

1. Enter an explicit `RESYNC_REQUIRED` state.
2. Discard pending old-epoch gameplay events.
3. Host sends a full canonical replication snapshot with a new checkpoint sequence.
4. Clients apply and verify it atomically.
5. Differential baselines reset to the verified full checkpoint.

Automatic resync must not hide repeated divergence. Limit attempts and terminate the session after a repeated mismatch at the same checkpoint or after any schema/build mismatch.

## Refactoring plan

### Phase 0: observability without behavior change

- Add one structured transition/event ring buffer.
- Record pump entry/exit depth, packet enqueue/dispatch, sender, message kind, current phase, turn, and key counters.
- Add assertions for game-thread ownership, balanced update suspension, pointer ownership, valid player counts, and legal array indices.
- Assign diagnostic transaction IDs locally even before they are on the wire.
- Preserve and expand the current network debug logging.

### Phase 1: centralize dispatch and ownership

- Make transport polling enqueue-only.
- Remove receive dispatch from `Mox_Send_Message_()` and local-host fast paths.
- Introduce an outer non-reentrant dispatcher with a bounded event budget.
- Replace `network_data_ptr`/`total_bytes_received` with an owned message object or queue.
- Separate audio/UI progress from gameplay network dispatch.
- Convert waits in receive-sensitive paths to resumable state.

This phase must preserve the early-arriving final payload behavior tested in `test_netturn_host_selection.cpp` and the serialization suspension behavior tested in `test_netmox_packet_limits.cpp`.

### Phase 2: checked transitions and transaction identity

- Add session epoch, authority term, turn, transaction ID, sender slot, and sequence to strategic packets.
- Centralize allowed state transitions.
- Replace readiness counters with participant sets/bitsets.
- Validate sender ownership and phase before decoding payloads.
- Make disconnect/dropout and order submission idempotent.
- Add deadlines and explicit failure/recovery states.

### Phase 3: checkpoint protocol and oracle

- Build canonical field-based state serialization and component hashes.
- Add full checkpoint metadata and client result acknowledgements.
- Require all expected post-apply hashes to match before beginning the next turn.
- Add diagnostic bundles and deterministic full-state recovery.
- Version the canonical schema independently from packet structs and savegames.

### Phase 4: combat and diplomacy transactions

- Give every combat and negotiation a stable ID.
- Replace global handshake latches and single pending-message slots with owned queues/context.
- Sequence tactical actions and reject gaps, duplicates, and wrong-combat packets.
- Define and oracle-check combat/diplomacy commit boundaries.

## Test plan

### Unit tests for dispatch semantics

- A handler that requests another update never causes nested handler entry.
- A handler that sends to the local host observes the same queued ordering as a remote send.
- Multiple nested update requests set a deferred-work latch and are drained once by the outer dispatcher.
- Serializers can update sound/UI without dispatching a mutating packet.
- A pending event received during snapshot capture is applied only after capture completes.
- Dispatch budget exhaustion resumes on the next outer tick without dropping events.

### State transition tests

Table-drive every `(state, event)` pair:

- legal transition reaches exactly the documented next state
- wrong phase, sender, epoch, authority term, or transaction is rejected without mutation
- duplicate event is idempotent
- stale event cannot satisfy a current barrier
- invalid payload cannot allocate storage, increment counters, or partially alter world state

Cover startup, turn orders, combat, diplomacy, dropout, and terminal session loss.

### Regression tests for known failure modes

Keep the current tests and add:

- final game data arriving before the client's final wait is retained and consumed once
- initial client state is decoded before its receive storage is released
- nested update during snapshot and order encode/decode cannot mutate source/destination state
- duplicate `GAME_ORDERS_PAYLOAD` does not advance readiness twice
- an order payload from a player other than the requested player is rejected
- duplicate member-left and dropout packets perform one AI conversion and one count change
- disconnect while waiting for orders resolves the expected participant set
- stale diplomacy handshake cannot release a new negotiation wait
- tactical actions are queued in order rather than overwriting a single slot
- a differential with a missing/wrong base is rejected and triggers full-state request

### Oracle tests

- Identical semantic state with different padding, pointer values, local player IDs, and inactive tail bytes hashes identically.
- A one-field mutation in every canonical component changes that component and the root hash.
- Every RNG stream changes the RNG component hash.
- Snapshot encode/decode followed by canonicalization matches the original canonical state.
- Differential apply followed by canonicalization matches the host result hash.
- A deliberately omitted replication field is detected by post-apply oracle comparison.
- Schema mismatch is rejected before state apply.
- Mismatch diagnostics name the correct component and checkpoint.
- Full resync resets the differential baseline and produces agreement.

A field-coverage test should compare the canonical schema inventory against the authoritative state inventory. This makes adding a new deterministic global an explicit decision: include it, document it as derived, or document why it is local/transient.

### Deterministic multi-peer simulator

Build a test harness with two or more in-process peers and a fake router. The harness should control delivery one event at a time and support:

- delay and reordering across senders
- duplicates
- disconnect at every transition
- early delivery before a wait is installed
- send failure
- differential loss and stale baseline
- bounded dispatch batches

For each generated schedule, assert that peers either:

- reach the same committed checkpoint and oracle hash; or
- enter the same documented recovery/terminal outcome without partial commit.

Persist failing schedules as short replay traces. Run a deterministic corpus in normal CI and a larger randomized/property corpus separately.

### Integration tests

- New game with host plus one and several clients through the real router client.
- Load game and first full checkpoint agreement.
- Multiple turns including AI, tactical combat, strategic combat, and diplomacy.
- Gameplay-host disconnect during `READY`, order collection, combat, and checkpoint acknowledgement.
- Client disconnect during the same phases.
- Router loss produces a consistent terminal state and synchronized recovery-save policy.
- Cross-build/schema mismatch fails cleanly before gameplay starts.

Run focused networking tests under AddressSanitizer and UndefinedBehaviorSanitizer. ThreadSanitizer is still useful for adapter integration, but it will not detect the main single-threaded re-entrancy failures; the controlled scheduler tests are required for those.

## Immediate concrete changes recommended

The minimal implementation order is:

1. Add structured receive/process/send/transition tracing and assert game-thread ownership.
2. Split transport polling from gameplay dispatch and enforce one guarded dispatcher at existing `Mox_Net_Update_()` progress points.
3. Make sends immediate but non-pumping; route local-host delivery through the inbound queue.
4. Replace `network_data_ptr` with owned typed inbound payloads.
5. Add `turn_id` to strategic, combat, and state-changing diplomacy packets, with receive-time and process-time stale/future logging.
6. Introduce the per-message validation matrix for length, state, turn, sender role, sender mapping, indices, and duplicate policy.
7. Replace host order counting with expected/requested/applied masks and make dropout application session-idempotent.
8. Add explicit checkpoint/base sequence and hashes to full/differential state transfer.
9. Implement the canonical post-apply oracle, field-addressable mismatch records, and accepted-event replay log.
10. Add `combat_id` and `diplomacy_id` only when hardening those same-turn repeated protocols.

Steps 2 and 3 preserve diplomacy progress because normal UI and wait ticks remain dispatch points. The forbidden operation is dispatching another gameplay handler while one is already active, not sending a response or handling an asynchronous diplomacy request. Any true handler-time wait discovered by tests must be changed to set pending state and return.

## Definition of done

The networking state refactor is complete when:

- packet callbacks cannot be re-entered
- send and UI/audio updates cannot unexpectedly dispatch gameplay state
- every gameplay packet is attributable to a valid sender, authority term, turn, and transaction
- duplicate/stale packets and disconnect events are harmless
- every barrier has bounded recovery behavior
- state checkpoints and differential baselines are explicit and atomic
- all clients automatically prove canonical post-apply equality with the host at committed checkpoints
- mismatches stop progression and produce actionable deterministic replay data
- the multi-peer fault-injection suite reaches agreement or a defined common failure outcome for every tested schedule

This gives the project a measurable consistency contract rather than relying on timing, incidental update calls, and periodic authoritative overwrite.