# Virtual file system

The game uses a read-only virtual file system for asset loading.

This lets the engine read assets from loose overrides, zip mods, and the original game files through one common path.

## What the VFS covers

The VFS is used for read-only game assets such as:

- LBX resources
- streamed LBX-backed audio
- animation assets loaded through the normal resource loaders
- Smacker video playback assets
- generic asset file reads that go through the VFS API

The VFS does not replace normal local file I/O for:

- save games
- `MOX.SET`
- `language.ini`
- `orioncd.ini`
- temporary files
- other write paths

## Resolution order

Assets are resolved in this order:

1. `data/overrides/`
2. `data/*.zip`
3. raw asset files under the game base path
4. raw asset files under the configured fallback install path, if one is set

Loose overrides always win over zip mods, and zip mods always win over raw assets.

Zip priority is deterministic: zip files are scanned in lexicographic filename order inside `data/`.

Matching is case-insensitive.

## Mod layout

Recommended layout:

```text
data/
  overrides/
    COLBLDG0.LBX/000
    intro.lbx
  10-base-fixes.zip
  20-balance.zip
```

Zip contents use the same virtual paths as loose overrides.

## LBX support

LBX entry overrides are supported directly.

Per-entry override paths use this format:

```text
NAME.LBX/000
NAME.LBX/001
NAME.LBX/002
```

For `NAME.LBX` entry `12`, lookup order is:

1. `data/overrides/NAME.LBX/012`
2. first matching zip entry `NAME.LBX/012`
3. `data/overrides/NAME.LBX`
4. first matching zip entry `NAME.LBX`
5. raw `NAME.LBX`

When an LBX entry is opened through the VFS, the returned stream is entry-relative:

- position `0` is the start of the selected entry
- reads only return that entry's payload
- seeks are relative to the entry, not the full LBX file

This allows per-entry overrides without forcing callers to parse whole LBX files themselves.

## Zip behavior

Zip assets are read directly from the archive.

Compressed zip entries support backward seeks by reopening the zip stream and skipping forward to the requested position. This keeps the implementation simple and avoids temp-file extraction.

## Alternate install path

If the game configures an alternate raw asset path through `orioncd.ini`, the VFS uses it as a raw fallback location.

This fallback applies to raw asset lookup only. It does not currently scan `data/overrides/` or `data/*.zip` under the alternate install path.

## Current limitations

- The VFS is read-only.
- Write/config/save paths remain normal local file I/O.

## Summary

The current VFS behavior is:

- loose overrides, then zip mods, then raw assets
- case-insensitive asset matching
- direct support for whole-file and per-entry LBX overrides
- one common read path for migrated asset loaders
- Smacker videos can be loaded from loose overrides, zip mods, or raw assets
- no VFS handling for writes