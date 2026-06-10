# SDL migration plan

## Goal

Remove the remaining DirectSound and winmm dependencies, continue the SDL-first migration for audio, and eliminate Windows-only dependencies that are directly part of the sound path.

This document is intentionally implementation-oriented so each item can be completed and validated one at a time.

Out of scope for this plan:
- DirectPlay and networking cleanup
- `winnet.*`, `dplayx.def`, and related multiplayer code
- broader non-audio Windows portability work unless it directly blocks sound migration

## Current findings

### 1. Build system still carries audio-specific Windows dependencies

Current state:
- `CMakeLists.txt` links `winmm`, `dsound`, `SDL3::SDL3-static`, and generated import libraries for `smackw32.lib` and `dplayx.lib`.

Impact:
- DirectSound and winmm cannot be removed cleanly without updating the build wiring.
- `dplayx.lib` may remain if DirectPlay stays in scope elsewhere, but it is not part of the sound cleanup.

### 2. Audio backend is fully tied to DirectSound and winmm

Primary files:
- `src/game/deps.h`
- `src/game/sound.h`
- `src/game/sound.cpp`

Current state:
- `deps.h` includes `Windows.h`, `dsound.h`, and `mmsystem.h` globally.
- `sound.h` exposes DirectSound-specific types and declares `DirectSoundCreate`.
- `sound.cpp` owns the full sound path around:
  - `IDirectSound`
  - `LPDIRECTSOUNDBUFFER`
  - `DSBUFFERDESC` and `DSBUFFERDESC1`
  - `WAVEFORMATEX`
  - `timeBeginPeriod`, `timeSetEvent`, `timeKillEvent`, `timeEndPeriod`
  - `mmioSeek`, `mmioRead`, `mmioGetInfo`, `mmioAdvance`, `mmioSetInfo`
- The current implementation relies on a periodic multimedia timer for both SFX and music stream maintenance.

Impact:
- DirectSound removal is the main technical blocker.
- winmm removal is not just timer cleanup; it also includes stream file I/O currently done through mmio.

### 3. SDL main loop exists, but platform layer still reaches into Win32

Primary files:
- `src/game/platform.h`
- `src/game/platform.cpp`
- `src/game/timer.cpp`
- `src/game/time.cpp`

Current state:
- SDL window, renderer, texture, event pump, cursor warp, threads, and delays are already in use.
- `platform.h` still exports `HWND g_main_window`.
- `platform.cpp` extracts `SDL_PROP_WINDOW_WIN32_HWND_POINTER` from the SDL window and stores it in `g_main_window`.
- `platform.cpp` still uses Windows-only helpers such as `ExitProcess` and `GlobalMemoryStatus`.
- `timer.cpp` uses `GetTickCount`.

Impact:
- SDL is already the right runtime direction.
- There is still unnecessary Win32 coupling that should be removed once audio no longer needs a window handle.

### 4. Smacker playback is coupled to DirectSound through the legacy DLL interface

Primary files:
- `src/game/smckplay.h`
- `src/game/smckplay.cpp`
- `smackw32.def`
- `CMakeLists.txt`

Current state:
- `smckplay.h` imports `_SmackSoundUseDirectSound(IDirectSound*)` from `SMACKW32.DLL`.
- `smckplay.cpp` forwards `sound::_dsound` into `_SmackSoundUseDirectSound` before opening a cinematic.
- `smackw32.def` exports `_SmackSoundUseDirectSound@4` and is used to generate `smackw32.lib`.

Impact:
- DirectSound removal affects not only gameplay audio but also cinematic audio.
- We need a concrete decision for Smacker audio:
  - keep video playback and disable Smacker-managed audio temporarily, or
  - replace the Smacker audio path with an SDL-compatible path.

### 5. winmm usage is split across two concerns

Current state:
- winmm timer usage drives periodic audio work.
- winmm mmio usage reads stream data from LBX-backed wave content.

Impact:
- The timer can move to SDL timing or an SDL audio callback driven model.
- The stream I/O should move to existing file/LBX reads or SDL RWops style abstractions, not a 1:1 timer-only replacement.

### 6. DirectPlay and networking are explicitly out of scope

Current state:
- `winnet.*`, `dplayx.def`, and `src/dplay.h` remain Windows-specific.
- `CMakeLists.txt` still generates and links `dplayx.lib`.

Impact:
- These files should remain unchanged during the sound cleanup.
- Any audio-related CMake cleanup must avoid disturbing the DirectPlay link path.

### 7. Generated type headers still preserve Windows ABI types

Primary files:
- `src/game/orion2.h`

Current state:
- `orion2.h` contains imported Windows and DirectSound type definitions such as `IDirectSound`, `IDirectSoundBuffer`, and `HWND`.

Impact:
- Some of these may become dead once the handwritten code is cleaned up.
- We should not edit this header early unless required, because it is broad and risky. Prefer isolating it behind wrappers first.

## Recommended approach

### Guiding principles

- Prefer SDL for runtime services: audio, timing, threads, windowing, events.
- Remove Windows-only dependencies at the handwritten boundary first.
- Keep changes incremental and buildable.
- Keep networking and DirectPlay code untouched.
- Do not touch large generated headers until their usage has been eliminated from first-party code.

### Proposed migration order

1. Introduce an SDL-backed audio abstraction layer without changing gameplay call sites.
2. Migrate SFX playback from DirectSound buffers to SDL audio-managed mixing.
3. Migrate music/stream playback and remove mmio-based streaming.
4. Remove winmm timer usage and move periodic work into SDL-friendly update or callback flow.
5. Decouple Smacker playback from DirectSound.
6. Remove Win32 handles and helper calls that were only kept for DirectSound.
7. Simplify build files to stop linking `dsound` and `winmm`.

## Step-by-step implementation plan

| ID | Step | Status | Notes |
| --- | --- | --- | --- |
| P0 | Repository analysis and migration planning | completed | This document covers current findings and recommended sequence. |
| P1 | Define SDL audio backend shape | pending | Create a minimal internal audio backend boundary so `sound.cpp` can be ported in stages instead of rewritten all at once. |
| P2 | Replace DirectSound channel representation | pending | Replace `IDirectSound` and `LPDIRECTSOUNDBUFFER` usage in `sound.h` and `sound.cpp` with SDL-owned channel state and software mixing buffers. |
| P3 | Replace winmm timer-driven updates | pending | Remove `timeBeginPeriod`, `timeSetEvent`, `timeKillEvent`, and `timeEndPeriod`. Use SDL audio callback and/or explicit SDL tick-based pumping. |
| P4 | Replace mmio stream reading | pending | Remove `mmio*` calls from music streaming and read raw data through project file/LBX helpers or SDL RW APIs. |
| P5 | Preserve existing sound behavior on SDL | pending | Re-implement current semantics: 16 channels, volume table behavior, looping, pitch adjustments if still needed, stream fade in/out, and cache logic. |
| P6 | Decouple Smacker from DirectSound | pending | Remove `_SmackSoundUseDirectSound` usage. Decide whether cinematic audio is temporarily disabled or replaced with SDL-backed playback. |
| P7 | Remove sound-related Win32 dependency from platform layer | pending | Delete `platform::g_main_window`, stop extracting Win32 HWND from SDL window properties, and remove only the native window coupling required by DirectSound. |
| P8 | Replace remaining Win32 utility calls inside the sound path | pending | Migrate `OutputDebugStringA`, `_strcmpi`, `MAX_PATH`, and similar helpers used by the audio code to portable equivalents. |
| P9 | Remove DirectSound and winmm from build | pending | Drop `dsound` and `winmm` link dependencies and any now-unused include/import setup in `CMakeLists.txt` without changing DirectPlay-related link setup. |

## Detailed execution notes per step

### P1. Define SDL audio backend shape

Target files:
- `src/game/sound.h`
- `src/game/sound.cpp`
- possibly a new internal helper file if separation helps

Expected outcome:
- Gameplay-facing function names stay the same.
- Backend details move behind internal structs and helper functions.
- No DirectSound types remain in the public handwritten header surface.

Suggested validation:
- Project still compiles after introducing the abstraction shell.

### P2. Replace DirectSound channel representation

Expected outcome:
- Each logical channel tracks playback state in plain C++ data.
- SDL audio device owns final audio output.
- Channel refill/update logic works on software buffers rather than DirectSound lock/unlock APIs.

Key risk:
- The current implementation depends on DirectSound cursor semantics and circular buffer behavior. Those mechanics need to be translated carefully, not approximated blindly.

Suggested validation:
- Sound effects still play.
- Multiple overlapping effects still work.
- Stop, loop, and fadeout behavior still matches current gameplay expectations.

### P3. Replace winmm timer-driven updates

Expected outcome:
- No multimedia timer functions remain.
- Audio work is driven either by:
  - SDL audio callback mixing, or
  - a dedicated SDL thread plus locking, or
  - main-loop pumping if latency stays acceptable.

Preferred direction:
- Prefer SDL audio callback ownership, because it naturally replaces both playback timing and fill pressure.

### P4. Replace mmio stream reading

Expected outcome:
- No `mmioSeek`, `mmioRead`, `mmioGetInfo`, `mmioAdvance`, or `mmioSetInfo` remain.
- Stream parsing reads from normal bytes already owned by the project or from a portable file abstraction.

Suggested validation:
- Background music starts, loops, fades, and stops correctly.
- Stream read errors remain visible in logs or user-facing messages.

### P5. Preserve existing sound behavior on SDL

Behavior to keep:
- 16 effect channels
- master volume and per-channel volume handling
- existing volume lookup table behavior
- loop counts
- music stream fade up/down
- audio cache allocation behavior from the existing sound buffer pool

Note:
- This step is about compatibility, not redesign.

### P6. Decouple Smacker from DirectSound

Decision required:
- If `SMACKW32.DLL` can play video without `_SmackSoundUseDirectSound`, remove the call first and test.
- If not, we need a fallback plan:
  - accept silent cinematics temporarily, or
  - replace the cinematic playback stack later.

Expected outcome:
- No first-party code passes DirectSound objects into Smacker.
- `smackw32.def` and import setup can be re-evaluated based on what exports are still required.

### P7. Remove Win32 handle dependency from platform layer

Expected outcome:
- `platform::g_main_window` disappears.
- `SDL_PROP_WINDOW_WIN32_HWND_POINTER` usage disappears.
- SDL stays the only window/event owner.

### P8. Replace remaining Win32 utility calls in active SDL path

Portable replacements to consider:
- `OutputDebugStringA` -> `SDL_Log` or project logging helper
- `_strlwr` -> local ASCII lowercase helper
- `_strcmpi` -> local case-insensitive compare helper
- `MAX_PATH` -> fixed local constant or `std::array`

### P9. Remove DirectSound and winmm from build

Expected outcome:
- `CMakeLists.txt` no longer links `dsound` or `winmm`.
- `deps.h` no longer includes `dsound.h` or `mmsystem.h`.
- Build breaks from removed headers should now be localized and fixable.

## Recommended next implementation step

Start with `P1` and `P2` together:
- define the SDL audio backend boundary
- remove DirectSound types from `sound.h`
- keep existing gameplay entry points untouched

That gives the project a controlled seam for the rest of the migration and prevents DirectSound details from spreading further.

## Status summary

- Planning and repository analysis: done
- DirectSound removal: not started
- winmm removal: not started
- DirectPlay/networking cleanup: intentionally out of scope and should remain unchanged