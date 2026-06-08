# orion2re

This project is a restoration effort for the Master of Orion II game engine.

All rights to Master of Orion II, its assets, trademarks, and related intellectual property belong to their corresponding owners.

This repository does not include the original game data. To play, you must own a legitimate copy of Master of Orion II and provide the original game files from a supported release such as GOG or Steam.

## Requirements

- Windows build environment
- CMake 3.20 or newer
- MSVC toolchain with 32-bit/x86 support
- Ninja
- Original Master of Orion II game files from GOG or Steam

The current CMake setup only supports 32-bit MSVC Windows builds.

## Build

Configure a preset:

```sh
cmake --preset win32-debug
```

Build it:

```sh
cmake --build --preset win32-debug
```

Available presets:

- `win32-debug`
- `win32-release`
- `win32-release-pdb`
- `win32-debug-asan`

Example release build:

```sh
cmake --preset win32-release
cmake --build --preset win32-release
```

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