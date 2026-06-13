# orion2re

This project is a restoration effort for the Master of Orion II game engine.

All rights to Master of Orion II, its assets, trademarks, and related intellectual property belong to their corresponding owners.

This repository does not include the original game data. To play, you must own a legitimate copy of Master of Orion II and provide the original game files from a supported release such as GOG or Steam.

## Requirements

- CMake 3.20 or newer
- Ninja
- Either:
  - Windows build environment with an MSVC toolchain
  - Linux build environment with MinGW-w64 cross-compilers
- Original Master of Orion II game files from GOG or Steam

Supported build paths:

- Native Linux builds
- Native Windows builds with MSVC for x86 and x64
- Linux-hosted MinGW-w64 cross-builds for Win32 and Win64 with networking disabled

## SDL3 submodule

The project requires the SDL3 submodule at `vendors/SDL`.

Clone the repository with submodules:

```sh
git clone --recurse-submodules <repo-url>
```

If you already cloned the repository without submodules, initialize it with:

```sh
git submodule update --init --recursive vendors/SDL
```

Bootstrap dependencies and verify required tools first:

Unix-like shells:

```sh
tools/bootstrap_deps.sh
```

Windows `cmd.exe`:

```bat
tools\bootstrap_deps.bat
```

The bootstrap scripts verify that `git`, `cmake`, and `ninja` are available, then initialize the SDL3 submodule and verify that it is pinned correctly.

The repository is expected to be pinned to SDL commit `8e37db5e797b6167f3a00d697d816a684bd259c7` (`release-3.4.10`).

## Build

Configure a preset:

```sh
cmake --preset win32-debug
```

Build it:

```sh
cmake --build --preset win32-debug
```

Tests are opt-in and are not built by default. To configure, build, and run the current unit tests:

```sh
tools/run-tests.sh
```

Available presets:

- `win32-debug`
- `win32-release`
- `win32-release-pdb`
- `win32-debug-asan`
- `win64-debug`
- `win64-release`
- `win64-release-pdb`
- `win64-debug-asan`
- `mingw-win32-debug`
- `mingw-win32-release`
- `mingw-win64-debug`
- `mingw-win64-release`
- `linux-debug`
- `linux-release`
- `linux-debug-asan`
- `linux-x86-debug`
- `linux-x86-release`
- `linux-x86-debug-asan`

Example release build:

```sh
cmake --preset win32-release
cmake --build --preset win32-release
```

## Cross-compiling Windows builds from Linux

Install MinGW-w64 cross-compilers and Ninja first. On Debian or Ubuntu, the required packages are typically:

```sh
sudo apt install gcc-mingw-w64 g++-mingw-w64 binutils-mingw-w64 ninja-build
```

Then configure and build one of the MinGW presets:

```sh
cmake --preset mingw-win32-debug
cmake --build --preset mingw-win32-debug
```

or:

```sh
cmake --preset mingw-win64-release
cmake --build --preset mingw-win64-release
```

The MinGW presets target Windows with vendored SDL and keep legacy DirectPlay networking disabled.

The MinGW toolchain files also request static GCC and C++ runtime linkage so the generated Windows executables do not depend on the MinGW runtime DLLs.

## Customization script

The repository includes `tools/create_user_preset.sh` to create a local CMake override file at `cmake/local-user.cmake`.

The script sets `ORION2RE_GAME_DIR`, which is used as the runtime output directory and debugger working directory for supported Win32 MSVC builds.

Usage:

```sh
tools/create_user_preset.sh /path/to/your/master-of-orion-2
```

On WSL, `/mnt/...` paths are automatically converted to Windows-style paths when possible.

After running the script, configure and build as usual:

```sh
cmake --preset win32-debug
cmake --build --preset win32-debug
```

If `ORION2RE_GAME_DIR` points to your installed game directory, the built executable and debug output will be placed there for supported configurations.