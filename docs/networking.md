# Networking redesign notes

## Goals

This document describes the intended networking redesign for the current project.

The original game used IPX in DOS and DirectPlay in Windows. The current restoration still carries a large amount of legacy networking structure, including DirectPlay integration and a custom reliable transport layer built on top of it. That model no longer fits the project goals well.

The new goals are:

- support cross-platform multiplayer
- remove dependency on DirectPlay and legacy Windows-only networking APIs
- remove legacy modem, null-modem, and serial-cable assumptions
- simplify the transport layer substantially
- preserve the game's existing high-level packet model where practical
- support both LAN and online play through one core networking stack
- support clean recovery by rehosting from a synchronized save rather than transparent host migration

The redesign intentionally favors maintainability and correctness over strict transport-level faithfulness to the original implementation.

## Current implementation summary

### High-level structure

The current Windows multiplayer path is layered roughly like this:

- `WINNET` wraps DirectPlay session management and message send/receive
- `comcore` adds packet headers and polling/dispatch glue
- `transprt` implements a custom reliable large-message transport on top of DirectPlay
- `netcode` chooses the backend and exposes send/update/status functions
- `NETSTART`, `NETTURN`, `NETMOX`, `MAINSCR`, combat, diplomacy, and chat use the abstracted networking layer

### What DirectPlay currently owns

DirectPlay currently owns:

- provider selection
- session discovery via `EnumSessions`
- session creation and joining via `Open`
- player registration via `CreatePlayer`
- message send/receive via `Send` and `Receive`
- allow/disallow joining via `EnableNewPlayers`

In the current Windows path, one DirectPlay session maps directly to one visible pending game. Session browsing is the game browser.

### What the custom transport currently does

The current restoration includes a custom transport in `transprt.cpp` that does all of the following:

- splits larger logical messages into transport packets
- sends packets in bursts of four
- waits for transport-level ACK messages
- tracks duplicates and old messages
- serializes competing senders with a queued-sender mechanism
- wakes the next sender with a control packet
- reassembles and optionally decompresses the payload before handing it to gameplay code

This exists because the original Windows game used DirectPlay without requesting guaranteed delivery for normal sends. The game therefore built its own reliability model above DirectPlay.

### Current gameplay topology

The game is not pure client/server in transport terms.

It is logically host-coordinated but transport-wise peer-to-peer. Players frequently send directly to other players by `network_player_id`. The host coordinates turn progression, collects data, runs authoritative progression logic, and broadcasts world-state updates, but many messages still travel directly between peers.

### Current disconnect behavior

The current game does not support reconnecting to a running multiplayer session.

If a player drops:

- that player is converted into AI
- their `network_player_id` is cleared
- the session continues without them

If the original human wants back in, a new session must be created, typically by loading a save and starting multiplayer again.

This is important because it means the game already tolerates "disconnect means that human is gone for this session." That aligns naturally with ordinary TCP connection loss.

## Key design decisions

## Decision 1: remove DirectPlay entirely

We will remove DirectPlay as an implementation dependency.

### Why

- DirectPlay is Windows-specific and obsolete.
- It does not help with cross-platform support.
- It duplicates responsibilities that are better owned by a simpler modern networking layer.
- The restoration currently carries DirectPlay-specific session logic that can be represented more directly in project code.

### Expected result

- `WINNET` will be removed.
- session listing, joining, player identity, send/receive, and routing will be implemented by project-owned code

### Tradeoffs

Pros:

- removes legacy dependency
- simplifies portability
- makes behavior explicit and testable

Cons:

- we must replace DirectPlay discovery and session management ourselves
- we lose any hidden fragmentation/session behavior DirectPlay previously provided

## Decision 2: remove the legacy custom reliable transport layer

We will remove the `transprt.cpp` model rather than preserve it as the primary transport.

### Why

- it exists mainly to build reliability over a non-guaranteed transport path
- it adds substantial complexity: fragmentation, ACKs, bursts, duplicate tracking, sender queueing
- its maintenance cost is high relative to its value in the new architecture
- the quality of the restored implementation is questionable enough that we already found at least one packetization bug

### Expected result

- large-message fragmentation at the old transport level goes away
- ACK/control packets such as transport ACK and wake-up packets go away
- transport reliability is delegated to TCP

### Tradeoffs

Pros:

- much simpler code
- much easier debugging
- no need to maintain a custom reliability protocol

Cons:

- we lose exact transport behavior parity with original Windows networking
- some game code paths that assume the old transport state flags may need cleanup or replacement

## Decision 3: use TCP for gameplay traffic

Gameplay traffic will use TCP, with explicit message framing.

### Why

- the game is turn-based and latency-sensitive behavior is less demanding than in action games
- reliable ordered delivery fits most of the current message model
- TCP eliminates the need for the old custom reliability layer
- TCP greatly simplifies cross-platform transport implementation

### Wire framing

Use a length-prefixed framed protocol.

Recommended framing:

- 4-byte little-endian message length
- fixed routing header
- opaque game payload

Use 4 bytes instead of 2 bytes to avoid arbitrary limits and to simplify large state transfer support.

### TCP options

Consider enabling `TCP_NODELAY` to reduce delay for small chat/control packets.

### Tradeoffs

Pros:

- simplest reliable transport option
- no custom fragmentation needed
- easy to reason about

Cons:

- stream semantics require explicit framing
- head-of-line blocking can delay later messages if earlier ones stall
- not ideal for mixed reliable/unreliable traffic, though that is acceptable here

## Decision 4: use a TCP routing service instead of direct peer connections

The core new architecture is router-based rather than peer mesh.

Each game client will maintain one TCP connection to a routing service.

### Why

- one unified networking path for LAN and online play
- avoids N-1 peer connections per client
- avoids peer address exchange and connection orchestration
- keeps session management in one place
- matches the project's desire for simpler cross-platform support

### Important consequence

The router is responsible for:

- assigning network-level IDs
- managing sessions
- delivering direct and broadcast packets
- exposing session lists

The router is not required to understand gameplay logic deeply.

### Tradeoffs

Pros:

- one TCP connection per client
- simpler client implementation
- easier NAT/firewall story later
- unified LAN and online model

Cons:

- host migration is no longer transport-transparent
- router becomes a required infrastructure component
- if gameplay authority is still in the host game process, router and host are separate concepts that must be coordinated carefully

## Decision 5: do not preserve automatic host migration as a first-class feature

The redesign assumes explicit recovery rather than transparent migration.

This decision originally assumed that host migration would be dropped entirely. After further review, that is too strong.

With a router-based virtual networking layer, the old meaning of "host" splits into two roles:

- router or service host: owns routing, session membership, and network-level identity
- gameplay host: owns turn progression and authoritative world-state generation

As long as the router remains available, clients can stay connected to the same virtual network and session. If the gameplay host exits but the router remains up, the game can still migrate gameplay authority to another connected node without rebuilding transport connectivity.

Therefore:

- network-level host election is no longer required for routing
- gameplay-level host election or promotion can still be useful and should remain in the game if we want live authority migration
- if the router itself goes away, the session is considered lost and recovery falls back to rehost-from-save

### Why

- current game already cannot support true rejoin of a dropped player into an in-progress session
- synchronized save/rehost is still the simplest recovery path when routing itself is lost
- gameplay host migration becomes easier to isolate once routing and authority are separate concepts
- keeping authority migration in the game avoids pushing gameplay rules into the router

### Recovery model

If host authority is lost:

- if the router is still up, the session can remain alive and gameplay authority may be promoted to another client by game logic
- if the router is gone, the session is considered lost
- when the session is lost, players use a synchronized autosave or latest manual save
- one player becomes the new host explicitly
- a new session is created

### Tradeoffs

Pros:

- keeps routing simple while preserving gameplay-host failover as an option
- authority migration remains game-owned, where turn/state logic already lives
- router loss still has a simple and robust fallback path

Cons:

- gameplay host migration still needs protocol and state-transition design in game code
- router loss still ends the session

## Decision 6: keep gameplay packets mostly intact where practical

The router should mostly forward opaque game payloads.

### Why

- current gameplay code already knows how to interpret these packets
- a full gameplay-protocol rewrite would be expensive and risky
- many current packet handlers can remain usable if sender, target, and packet type are preserved

### Important nuance

We should not preserve old transport packets as-is.

We should preserve game-level messages where possible.

That means:

- do not preserve `transprt.cpp` packet formats, ACKs, burst control, or DirectPlay header assumptions
- do preserve the higher-level game packet identifiers and payload layouts where it reduces rewrite cost

### Resulting split

The router protocol needs:

- a routing/control envelope owned by the new implementation
- opaque game payload bytes that are forwarded to clients or the host as needed

## Router responsibilities

The router should remain deliberately shallow.

## Control plane responsibilities

The router owns:

- connection acceptance
- client registration / hello handshake
- router-assigned network IDs
- session registration
- session listing
- join / leave
- per-session membership
- direct-send and broadcast fanout
- optional LAN discovery response support

## Data plane responsibilities

The router forwards:

- direct game packets from one network ID to another
- broadcast packets to all session members except or including the sender according to protocol flags

The router does not need to understand:

- turn progression
- diplomacy rules
- combat rules
- savegame structure
- authority validation of gameplay decisions in the first version

## Recommended router protocol shape

Exact field names can change, but the logical operations should look like this:

- `HELLO`
- `REGISTER_SESSION`
- `UNREGISTER_SESSION`
- `LIST_SESSIONS`
- `JOIN_SESSION`
- `LEAVE_SESSION`
- `SEND_TO`
- `SEND_BROADCAST`
- `DISCONNECT_NOTICE`

Potential envelope fields:

- message kind
- session id
- sender network id
- target network id
- payload size
- flags

## Session model

## Session = game

In the current DirectPlay implementation, one pending DirectPlay session corresponds to one visible game.

The new design should preserve that model:

- one router session record corresponds to one pending or active game

This keeps the mental model close to the current UI and reduces rewrite pressure.

## Router vs session

Do not make clients think in terms of "joining a router."

Clients connect to a router endpoint, then select a session exposed by that router.

This matters because a LAN may contain:

- multiple routers
- multiple sessions hosted by different routers
- potentially multiple sessions on the same router

The UI should list sessions, not routers.

## Avoiding ambiguity

Session listing records should include enough information to disambiguate sessions:

- `session_id`
- `router_id`
- session name
- host name
- player counts
- game version / build string
- optional save fingerprint or campaign fingerprint

## LAN discovery design

## Why LAN discovery still matters

We still want LAN play to feel easy and discoverable.

DirectPlay previously handled this through session enumeration. Without DirectPlay, we need a replacement.

## Chosen approach

Use UDP broadcast only to discover active routers/services on the local subnet.

Then query those routers over TCP for session lists.

### Flow

1. client sends UDP discovery probe on LAN
2. routers reply with router identity and TCP endpoint
3. client connects to router(s) over TCP
4. client requests session list
5. UI merges all returned sessions into one list
6. user picks a session and joins it over TCP

### Why not direct game discovery over gameplay transport

Because that would tend to force one of two bad outcomes:

- a second direct-IP gameplay stack alongside router-based routing
- fake packet broadcast semantics for session discovery where a simple registry query is better

The chosen approach keeps one gameplay transport stack.

## Online discovery design

LAN and online discovery should share the same session-listing and join protocol.

The only thing that differs should be bootstrap:

- LAN bootstrap: UDP broadcast finds routers
- online bootstrap: known directory server, invite code, favorite endpoint, or manual address entry finds routers

After bootstrap, clients use the same TCP API:

- list sessions
- join session
- send and receive routed traffic

This gives one unified browsing model while accepting that LAN and Internet bootstrap are inherently different.

## Authority model options

Several authority models were considered.

## Option A: fully authoritative standalone server

### Description

Move all game authority into a standalone headless server process. Clients become thin or semi-thin participants.

### Pros

- strongest cheat resistance potential
- cleanest server-side ownership model long-term
- clean host/process separation

### Cons

- huge rewrite
- requires reimplementing large portions of gameplay flow and validation on the server
- not necessary for the current project goals

### Status

Skipped for the initial redesign.

## Option B: host-authoritative game client plus shallow router

### Description

The host game process remains gameplay-authoritative. The router handles only session management and packet routing.

### Pros

- closest to current game logic
- minimal intrusion into gameplay code
- fastest path to a working replacement

### Cons

- host and router are separate roles
- gameplay host migration must be handled explicitly by game logic if we want live continuation
- not a strong anti-cheat model

### Status

Chosen as the preferred first implementation model.

Important consequence: if the gameplay host exits while the router stays alive, clients may remain connected to the router and the game may promote a new authoritative gameplay host. If the router exits, the session is lost regardless of gameplay-host logic.

## Option C: router-based gameplay authority over time

### Description

Start with Option B and later move selected authoritative checks or turn-resolution logic into the router/service.

### Pros

- staged path toward stronger authority
- lets us ship a simpler implementation first

### Cons

- transitional complexity
- requires careful protocol evolution

### Status

Plausible future evolution, not required initially.

## Transport/library alternatives considered

## Alternative 1: preserve custom transport, replace DirectPlay only

### Description

Keep `transprt.cpp` and run it over raw sockets.

### Pros

- closer behavioral match to original Windows code
- less gameplay-facing refactoring initially

### Cons

- preserves a complicated and fragile layer that no longer solves the right problem
- duplicates reliability already available from TCP
- increases long-term maintenance burden

### Status

Skipped.

## Alternative 2: raw UDP plus new reliability layer

### Description

Replace DirectPlay with UDP and build a new fragmentation/ACK/reliability model.

### Pros

- closer in spirit to old datagram-oriented architecture
- supports mixed reliable/unreliable semantics if needed later

### Cons

- unnecessary complexity for a turn-based game
- reintroduces most of the transport problems we want to delete
- requires more testing and protocol design

### Status

Skipped.

## Alternative 3: ENet

### Description

Use ENet as a UDP-based reliable networking library.

### Pros

- mature library for reliable UDP
- supports channels and partial reliability patterns

### Cons

- solves a class of problems we no longer need urgently
- still more complex than TCP for this use case
- not necessary for a turn-based synchronization-heavy game

### Status

Skipped for now.

## Alternative 4: GameNetworkingSockets

### Description

Use Valve's GameNetworkingSockets for modern P2P/networking support.

### Pros

- powerful feature set
- strong support for P2P, NAT traversal, relay, and modern connection management

### Cons

- heavier dependency than needed for the initial goals
- complexity does not obviously pay off if we use a simple router-based TCP model

### Status

Skipped for now.

## Alternative 5: full peer-to-peer TCP mesh

### Description

Replace DirectPlay with plain TCP but keep peer mesh connectivity, one connection per pair.

### Pros

- preserves host migration possibility more naturally
- closer to current transport topology

### Cons

- more complex client networking model
- complicates LAN and online traversal/bootstrap
- less aligned with the desire for one unified session-routing model

### Status

Not chosen for the current preferred plan.

## Large state transfer handling

The current game has several large payload flows, including game-state broadcast and combat/turn-order packet groups. These are currently chunked because the legacy transport stack could not safely treat them as single messages.

With TCP framing, we have options:

## Option A: preserve old chunking at the game level

### Pros

- minimal logic changes in the short term
- preserves current UI progress behavior for send/get screens

### Cons

- carries old constraints into the new design unnecessarily

## Option B: send one framed message for the whole logical payload

### Pros

- much simpler transport behavior
- no artificial `0x7800` application chunks needed for reliability reasons

### Cons

- some progress-screen logic may need to be redesigned if user feedback during transfer is desired

### Preferred direction

Initially, either approach is acceptable.

If it reduces integration risk, keep current high-level chunk messages first and remove them later. Long-term, a single framed message per logical payload is preferable.

## Rehost and autosave strategy

Because router failure still ends the session, explicit rehost from synchronized save remains an important strategic fit even if gameplay-host migration is preserved.

## Assumption

Each participating client maintains a full local game state in sync.

This assumption already appears to hold in the current design.

## Implication

Every client can create autosaves locally at safe synchronization points.

## Safe sync points

Autosaves should happen only when:

- all pending network messages for the current phase are fully applied
- no partial chunk transfer is in progress
- authoritative turn resolution is complete

The ideal time is after the host-authoritative world-state update has been received and applied by all participants.

## Suggested implementation phases

## Phase 1: infrastructure replacement

- remove DirectPlay dependency from the active multiplayer path
- introduce TCP router service and TCP client implementation
- implement UDP LAN router discovery
- implement router session listing and joining
- keep most gameplay packets intact

## Phase 2: transport simplification

- bypass or delete `transprt.cpp`
- route gameplay payloads over framed TCP messages
- keep or temporarily emulate the status flags needed by higher layers

## Phase 3: gameplay integration cleanup

- simplify `netcode` to one main backend for network play
- remove modem/null-modem/serial assumptions from active code paths
- remove or stub host-election logic if no longer needed
- replace any DirectPlay-specific lifecycle assumptions in UI code

## Phase 4: recovery and polish

- add synchronized autosave policy
- support explicit rehost workflow
- add direct IP and optional online directory bootstrap
- revisit whether legacy chunking can be removed entirely

## Affected areas in the codebase

Likely primary impact points:

- `src/game/winnet.cpp`
- `src/game/netcode.cpp`
- `src/game/comcore.cpp`
- `src/game/transprt.cpp`
- `src/game/netstart.cpp`
- `src/game/netturn.cpp`
- `src/game/netmox.cpp`
- UI code that assumes DirectPlay-style session browsing

## Summary of the preferred architecture

The preferred architecture is:

- router-based TCP gameplay transport
- UDP LAN discovery for routers only
- TCP session listing and joining
- one session maps to one game
- host game process remains gameplay-authoritative initially
- router remains shallow and mostly packet-opaque
- no network-level host election is required for routing
- gameplay-host election or promotion may remain in game code while the router is alive
- router loss falls back to explicit rehost from synchronized save

This approach removes the deepest legacy dependencies while preserving the current gameplay packet model as much as practical. It is a good balance between cleanup, portability, implementation cost, and operational simplicity.