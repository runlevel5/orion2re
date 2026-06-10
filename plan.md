1. Stabilize the compatibility layer
   - Expand `src/game/compat.h` into the single home for platform portability macros and wrappers.
   - Add no-op calling convention macros for non-MSVC builds such as `__cdecl` and `__stdcall`.
   - Add wrappers or aliases for Win32 string helpers already used in active code, such as `_strlwr`, `_strcmpi`, and related case-insensitive helpers.
   - Add SDL-backed timing, logging, and process-exit helpers so active code can stop calling Win32 APIs directly.

2. Remove global Windows header dependency from `deps.h`
   - Audit every symbol currently obtained from `Windows.h`, `conio.h`, `io.h`, `process.h`, and `direct.h` through `src/game/deps.h`.
   - Replace broad system includes in `deps.h` with standard C/C++ headers plus `compat.h` declarations.
   - Keep any unavoidable Windows-only declarations behind `#if defined(_WIN32)` so non-Windows builds can parse the header set.

3. Convert the SDL platform layer to stop exposing Win32 types
   - Change `platform::g_main_window` in `src/game/platform.h` from `HWND` to an SDL-native or opaque project-defined type.
   - Replace `ExitProcess` in `src/game/platform.cpp` with a compat helper that uses `SDL_Quit`, `abort`, or `exit` as appropriate.
   - Replace `GlobalMemoryStatus` usage in `src/game/platform.cpp` with a cross-platform fallback or SDL-based memory query.
   - Replace `_strlwr` use in command-line parsing with a compat helper.

4. Replace remaining active Win32 API calls in gameplay code
   - Update `src/game/timer.cpp` and `src/game/transprt.cpp` to use a compat tick-count helper backed by SDL ticks.
   - Update `src/game/comcore.cpp`, `src/game/netcode.cpp`, and `src/game/winnet.cpp` to use a compat logging helper instead of `OutputDebugStringA`.
   - Update `src/game/fields.cpp` to replace `GlobalMemoryStatus` and `MessageBoxA` with portable abstractions.
   - Re-scan after each file group to verify no direct Win32 API references remain in active non-networking code.

5. Untangle DOS and CRT-specific filesystem helpers
   - Refactor `src/game/dos.cpp` and `src/game/dos.h` away from `_findfirst32`, `_findnext32`, `_finddata32_t`, `_getdiskfree`, and `LPCSTR`.
   - Introduce cross-platform directory iteration and disk-free helpers using standard library or SDL filesystem APIs.
   - Preserve current game behavior and signatures where practical by routing old function names through portable implementations.

6. Reduce Win32 type leakage from active headers
   - Identify active code paths that still rely on `GUID`, `HRESULT`, `DWORD`, `WORD`, `LPCSTR`, `tagPALETTEENTRY`, and `MEMORYSTATUS` outside the DirectPlay-gated code.
   - Move portable replacements into `compat.h` where they are purely type aliases.
   - For palette-related structures such as `tagPALETTEENTRY`, define a project-owned equivalent so rendering code no longer depends on Windows declarations.

7. Keep legacy DirectPlay isolated
   - Ensure `src/game/winnet.*`, `src/dplay.h`, and import-lib generation remain fully gated by `ORION2RE_ENABLE_NETWORKING`.
   - Remove any DirectPlay-related type exposure from always-included headers where possible.
   - Confirm non-networking builds do not require `ole2.h`, `guiddef.h`, or other Windows SDK headers.

8. Make calling convention portability mechanical
   - Add compat definitions so the current large surface of `__cdecl` declarations compiles cleanly on non-MSVC compilers.
   - If needed, centralize any other compiler-specific keywords in `compat.h` instead of touching hundreds of declarations at once.

9. De-Windows the build configuration
   - Relax the hard Windows-only checks in `CMakeLists.txt` so non-Windows configure passes can begin.
   - Keep networking and any remaining unsupported pieces behind feature flags rather than global platform rejection.
   - Add non-Windows presets once the codebase can configure and compile far enough to validate incremental fixes.

10. Validate incrementally after each stage
   - After each step, run the narrowest possible configure or build command to catch portability regressions early.
   - Re-run repository scans for Win32 headers, APIs, and types in active code after each completed stage.
   - Only move on once the previous stage compiles cleanly or all remaining failures are understood and isolated.