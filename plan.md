1. Stabilize the compatibility layer
   - Expand `src/game/compat.h` into the single home for platform portability macros and Unix-first libc forwarding.
   - Add no-op calling convention macros for non-MSVC builds such as `__cdecl` and `__stdcall`.
   - Prefer POSIX/C runtime names such as `strcasecmp`, `strncasecmp`, and local case-conversion helpers; define Win32 spellings only as compatibility forwards when the non-Windows libc spelling is the canonical implementation.
   - Replace active Win32 API call sites directly with SDL or standard-library equivalents instead of adding new Win32-shaped wrapper functions.
   - Clean up transitional helpers such as `cstrcmpi` once call sites have moved to the canonical portable names.
   - Status: completed for current active call sites; `cstrcmpi`, `GetTickCount`, `OutputDebugStringA`, and `ExitProcess` were removed from active `src/game` usage, and MSVC calling convention compatibility was corrected for SDL entrypoints.

2. Move primitive integer type definitions out of `orion2.h`
   - Inventory all primitive aliases currently defined in `src/game/orion2.h`, including `int8_t`, `uint8_t`, `uint16_t`, `uint32_t`, `byte`, `word`, `dword`, `ulong`, and related legacy names.
   - Create a small project-owned types header for canonical primitive and legacy-width aliases, backed by `<cstdint>` or `<stdint.h>`.
   - Include that header before `orion2.h` so generated structures consume stable shared definitions instead of redeclaring them locally.
   - Remove or suppress duplicated primitive typedefs from `orion2.h` once all active code builds against the shared type header.
   - Reconcile any name conflicts with system headers or generated declarations so the project has one authoritative definition per primitive alias.
   - Status: completed for the shared primitive alias set. Primitive aliases were moved into `src/game/types.h`, `deps.h` now pulls `types.h` in first, and duplicate primitive typedef blocks were removed from `orion2.h`, `orion2_consts.h`, and `orion2_str.h`.
   - Follow-up cleanup completed: size/layout assertions were split out of `types.h` into `src/game/sizes.h`, `sizes.h` is included once at the end of `deps.h`, and local packet/layout checks were moved into shared headers where practical such as `s_load_net_screen_packet` in `src/game/netstart.h`.

3. Remove global Windows header dependency from `deps.h`
   - Audit every symbol currently obtained from `Windows.h`, `conio.h`, `io.h`, `process.h`, and `direct.h` through `src/game/deps.h`.
   - Replace broad system includes in `deps.h` with standard C/C++ headers plus `compat.h` declarations.
   - Keep any unavoidable Windows-only declarations behind `#if defined(_WIN32)` so non-Windows builds can parse the header set.
   - Status: completed for the active dependency surface. `deps.h` no longer pulls `Windows.h`, `conio.h`, `io.h`, `process.h`, or `direct.h`; shared portability helpers now live in `src/game/compat.h` and `src/game/compat.cpp`, and recent filesystem/message-box/memory-status call sites were moved to compat or SDL-backed helpers.

4. Convert the SDL platform layer to stop exposing Win32 types
   - Change `platform::g_main_window` in `src/game/platform.h` from `HWND` to an SDL-native or opaque project-defined type.
   - Replace `ExitProcess` in `src/game/platform.cpp` with a compat helper that uses `SDL_Quit`, `abort`, or `exit` as appropriate.
   - Replace `GlobalMemoryStatus` usage in `src/game/platform.cpp` with a cross-platform fallback or SDL-based memory query.
   - Replace `_strlwr` use in command-line parsing with a compat helper.
   - Status: completed for the active SDL platform layer. `platform::g_main_window` is now an opaque `void*` backed by `SDL_Window*`, memory-status queries route through SDL-backed compat helpers, and command-line lowercasing stays on the compat `strlwr` path.

5. Replace remaining active Win32 API calls in gameplay code
   - Update `src/game/timer.cpp` and `src/game/transprt.cpp` to use a compat tick-count helper backed by SDL ticks.
   - Update `src/game/comcore.cpp`, `src/game/netcode.cpp`, and `src/game/winnet.cpp` to use a compat logging helper instead of `OutputDebugStringA`.
   - Update `src/game/fields.cpp` to replace `GlobalMemoryStatus` and `MessageBoxA` with portable abstractions.
   - Re-scan after each file group to verify no direct Win32 API references remain in active non-networking code.

6. Untangle DOS and CRT-specific filesystem helpers
   - Refactor `src/game/dos.cpp` and `src/game/dos.h` away from `_findfirst32`, `_findnext32`, `_finddata32_t`, `_getdiskfree`, and `LPCSTR`.
   - Introduce cross-platform directory iteration and disk-free helpers using standard library or SDL filesystem APIs.
   - Preserve current game behavior and signatures where practical by routing old function names through portable implementations.
   - Status: completed for active helper usage. `dos.h` no longer exposes `_finddata32_t` or `LPCSTR`, and `DIR_`, `Disk_Free_`, and `Time_Stamp_` now use SDL filesystem/storage helpers on active paths instead of Win32 CRT search and disk APIs.

7. Reduce Win32 type leakage from active headers
   - Identify active code paths that still rely on `GUID`, `HRESULT`, `DWORD`, `WORD`, `LPCSTR`, `tagPALETTEENTRY`, and `MEMORYSTATUS` outside the DirectPlay-gated code.
   - Move portable replacements into `compat.h` where they are purely type aliases.
   - For palette-related structures such as `tagPALETTEENTRY`, define a project-owned equivalent so rendering code no longer depends on Windows declarations.
   - Progress update: active palette and window-handle leakage was reduced. `tagPALETTEENTRY` was replaced with project-owned `video::s_palette_color`, and `platform.h` no longer exports `HWND`.

8. Keep legacy DirectPlay isolated
   - Ensure `src/game/winnet.*`, `src/dplay.h`, and import-lib generation remain fully gated by `ORION2RE_ENABLE_NETWORKING`.
   - Remove any DirectPlay-related type exposure from always-included headers where possible.
   - Confirm non-networking builds do not require `ole2.h`, `guiddef.h`, or other Windows SDK headers.
   - Progress update: GUID/session identifiers were removed from active public networking entry points in `netcode.h`, `netstart.h`, `ten.h`, and `winnet.h`; the remaining GUID usage is now contained to generated declarations and the DirectPlay implementation in `winnet.cpp`.

9. Make calling convention portability mechanical
   - Add compat definitions so the current large surface of `__cdecl` declarations compiles cleanly on non-MSVC compilers.
   - If needed, centralize any other compiler-specific keywords in `compat.h` instead of touching hundreds of declarations at once.

10. De-Windows the build configuration
   - Relax the hard Windows-only checks in `CMakeLists.txt` so non-Windows configure passes can begin.
   - Keep networking and any remaining unsupported pieces behind feature flags rather than global platform rejection.
   - Add non-Windows presets once the codebase can configure and compile far enough to validate incremental fixes.
   - Progress update: shared target setup no longer defines `_WINDOWS` unconditionally; that Win32 define now stays limited to the MSVC/Win32-specific branch instead of leaking into non-Windows builds.

11. Validate incrementally after each stage
   - After each step, run the narrowest possible configure or build command to catch portability regressions early.
   - Re-run repository scans for Win32 headers, APIs, and types in active code after each completed stage.
   - Only move on once the previous stage compiles cleanly or all remaining failures are understood and isolated.
   - Latest validation: repository scans are clean for the recently removed global Windows headers, active palette Win32 type usage, and recent compat-layer C++ stdlib usage; current configure still advances until the broader build-system portability limits take over.
   - Follow-up validation: non-networking configure still advances on Linux after the `_WINDOWS` define cleanup, but the configure run remains slow in vendored SDL detection and timed out before reaching the first project compile failure.

12. Remove `WINNET` and integrate router service connectivity
   - Goal: replace the active Windows/DirectPlay multiplayer path with the project-owned TCP router service while preserving the existing high-level gameplay packet model.
   - Existing router status:
     - `src/network/router_protocol.*`, `router_core.*`, `router_service.*`, and `router_cli.*` already implement the service-side protocol, session management, TCP routing, UDP LAN discovery replies, and standalone `orion2re-router` target.
     - Focused router tests cover protocol framing, session lifecycle, direct/broadcast routing, member events, service startup/shutdown, malformed frames, backpressure, and UDP discovery.
   - Add a game-facing router client layer:
     - Create `src/network/router_client.h` and `src/network/router_client.cpp`.
     - Own one TCP connection to a selected router endpoint.
     - Send and receive length-prefixed `router_protocol` frames.
     - Maintain the router-assigned local `network_id` and current `session_id`.
     - Cache session-list results returned by `LIST_SESSIONS`.
     - Cache current session members from `MEMBER_LIST`, `MEMBER_JOINED`, and `MEMBER_LEFT`.
     - Queue received opaque `GAME_DATA` payloads for the game layer.
     - Queue membership/session lifecycle events for the game layer.
     - Expose a nonblocking pump/update API suitable for `netcode::Net_Update_()`.
     - Keep this layer gameplay-opaque; it must not convert players to AI, elect gameplay hosts, or interpret MOX/NETMOX packet IDs.
     - Status: implemented and covered by `test_router_client`.
   - Add the legacy-compatible game adapter:
     - Create `src/game/netadapter.h` and `src/game/netadapter.cpp`.
     - This is the compatibility seam between old gameplay code and the new router client.
     - Mirror only the behavior still needed by `netcode`, `comcore`, `NETSTART`, and `NETMOX`; do not recreate DirectPlay concepts that are no longer useful.
     - Initial adapter responsibilities:
       - initialize/shutdown the router client
       - connect to a LAN or online router endpoint
       - refresh and expose session lists
       - create/register a session
       - join a session by cached session record
       - enable/disable new joins through `SET_SESSION_OPEN`
       - return local network ID
       - return connected network IDs for `Net_Get_Users_()` compatibility
       - send direct game payloads with `SEND_TO`
       - send broadcast game payloads with `SEND_BROADCAST`
       - poll queued received game payloads
       - expose member-left/session-closed events to gameplay recovery code
     - Status: implemented and covered by `test_netadapter`.
   - Network ID allocation rules:
     - Router-assigned IDs must never overlap legacy game broadcast sentinels `0x7FFE` and `0x7FFF`.
     - Router-assigned IDs must remain representable by existing signed 16-bit game fields such as `network_player_id`, `net_host_id`, `host_election_id`, and `local_net_id`.
     - Router-assigned IDs are therefore constrained to `1..0x7FFD`.
     - Allocation wraps to the first free active-safe ID and never collides with a currently connected client.
     - If all IDs are active, the router returns `ROUTER_ERROR_ROUTER_FULL` instead of assigning a duplicate or sentinel ID.
     - Status: implemented and covered by `test_router_core` allocator cases.
   - Widen legacy user-list APIs:
     - `netcode::Net_Get_Users_()` and current `NETSTART` buffers must carry at least 16-bit IDs, not `uint8_t` IDs.
     - Status: `Net_Get_Users_()` now accepts `int16_t*`; active `NETSTART` user-list buffers and the TEN stub signature were widened.
   - Update `netcode.cpp` mode 0 to use `netadapter`:
     - Replace `WINNET::Init_Connection_(2)` with adapter initialization and router endpoint selection.
     - Replace `WINNET::Refresh_Game_List_()` with router session listing.
     - Replace `WINNET::Create_Game_()` with adapter session registration.
     - Replace `WINNET::Join_Game_()` with adapter session join.
     - Keep modem/TEN branches untouched until the router path is stable.
     - Preserve the public `netcode` API so higher gameplay code does not need a large first-pass rewrite.
     - Keep `Net_Get_Game_Count_()`, `Net_Get_Game_Name_()`, `Net_Create_Game_()`, `Net_Join_Game_()`, `Net_Get_Users_()`, `Net_Disable_New_Players_()`, `Net_Send_Message_()`, `Net_Update_()`, and `Net_Status_()` behavior-compatible from callers' point of view.
     - Status: initial mode 0 wiring is implemented and the main `orion2re` target builds.
   - Bypass `transprt.cpp` for router-backed gameplay traffic:
     - TCP router frames already provide reliable ordered delivery, so the legacy ACK/burst/fragmentation transport should not be used on the router path.
     - Encode each gameplay send as one router game envelope containing:
       - game message type
       - payload byte count
       - payload bytes
     - Send the game envelope as router `SEND_TO` or `SEND_BROADCAST` payload.
     - On receive, decode the small game envelope and invoke the existing registered gameplay receive callback, currently `NETMOX::Receive_Message_()` through `netcode::_recv_fn`.
     - Keep `transprt.cpp` compiled but unused initially, then delete it after router mode is proven.
     - For compatibility, make router-mode `Net_Status_()` report send success once the client layer accepts/queues the frame; no long-running `_tpSending` loop should remain on the router path.
     - Status: initial router-mode send/receive bypass is implemented through `netadapter`; broader `comcore` cleanup remains.
   - Simplify `comcore` on the router path:
     - Short term: keep `comcore::ComInit_()` only as callback/buffer setup if that minimizes gameplay churn.
     - Route mode 0 sends and polls through `netadapter` instead of `WINNET::Send_Message_()` and `WINNET::Poll_Message_()`.
     - Remove router-mode handling for DirectPlay session packets such as create-player, destroy-player, and keepalive packets.
     - Treat router member events as the source of truth for membership changes.
     - Medium term: split `comcore` into a deleted legacy transport path and a simple router dispatch path.
     - Status: pending cleanup; mode 0 no longer relies on comcore for active send/update, but `ComInit_()` is still used for legacy initialization side effects.
   - Wire router membership loss into `NETMOX` recovery:
     - Add an explicit adapter/game bridge for abrupt disconnects, for example `NETMOX::Handle_Network_Player_Disconnected_(int player_idx)`.
     - Map router network IDs to game player slots using existing game-owned state such as `MOX::_player[i].network_player_id` and `NETMOX::Net_Id_To_Player_Id_()`.
     - The router/client layer only reports `MEMBER_LEFT`; gameplay owns the dropout-to-AI policy.
     - For abrupt disconnect, remaining clients should deterministically elect the recovery coordinator from shared game state, initially lowest remaining connected game player index.
     - Only the elected coordinator should synthesize and broadcast the existing game-level dropout/AI takeover packet.
     - Reuse `NETMOX::Decode_Drop_Out_Info_()` and `RUSS::Make_Player_Into_AI_()` paths where practical.
     - Status: pending; current netcode logs member-left/session events but does not yet invoke gameplay recovery.
   - Update multiplayer UI session flow:
     - Keep the first pass minimally invasive by preserving current session-list display behavior.
     - Internally track router `session_id` instead of joining solely by session name because names are ambiguous.
     - Session records should carry router ID, session ID, session name, host name, player counts, build string, and open/closed state.
     - Later UI polish can expose the extra router/session metadata directly.
     - Status: adapter caches router session IDs, but existing UI still passes names through the legacy netcode API.
   - Bootstrap behavior and UI mode mapping:
     - Make the LAN button act as the LAN router mode toggle.
     - LAN mode should use local router discovery and/or an embedded local router service for hosting.
     - For LAN hosting, start the existing blocking `Router_Service_Run()` on an SDL-managed worker thread, then connect the local client to it.
     - Stop and join the embedded router thread during multiplayer shutdown.
     - Repurpose the modem path as the online/direct-IP adapter path.
     - Online/direct-IP mode should connect to a manually entered router host/IP and TCP port, defaulting to port `47800`.
     - Keep LAN and online modes using the same router protocol after bootstrap; only endpoint discovery differs.
     - Status: direct endpoint configuration currently uses the existing modem reset text field as a temporary host string source when present; proper UI mapping remains pending.
   - Remove `WINNET` after adapter parity:
     - First remove mode 0 call sites of `WINNET::*` from `netcode.cpp`, `comcore.cpp`, `transprt.cpp`, and related gameplay code.
     - Remove public includes of `winnet.h` from active code.
     - Delete `src/game/winnet.cpp` and `src/game/winnet.h` once no active code references them.
     - Remove DirectPlay import-library generation and `dplayx.def` from the active build if they become unused.
     - Retire or rename `ORION2RE_ENABLE_NETWORKING` so it no longer means legacy DirectPlay networking.
     - Status: pending; `WINNET` still exists and is still compiled, but active mode 0 `netcode` path no longer initializes/sends/receives through it.
   - Testing checkpoints:
     - Add unit tests for `router_client` connect/hello/list/create/join/send/receive/event handling.
     - Add unit tests for `netadapter` session-list caching, local ID propagation, membership tracking, direct send, broadcast send, and member-left exposure.
     - Add an integration-style test using an in-process `s_router_service` plus two clients.
     - Keep existing `test_router_protocol`, `test_router_core`, and `test_router_service` passing.
     - Build with legacy DirectPlay disabled and confirm router networking does not require Windows SDK or DirectPlay headers.
     - Latest validation: `test_router_protocol`, `test_router_core`, `test_router_service`, `test_router_client`, and `test_netadapter` pass; `orion2re` builds in `build-router-tests`.