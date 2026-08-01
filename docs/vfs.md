# Virtual file system

The game uses a read-only virtual file system for asset loading.

This lets the engine read assets from loose overrides, enabled package layers, one base archive, the
shared data directory, and original game files through one common path.

## What the VFS covers

The VFS is used for read-only game assets such as:

- LBX resources
- streamed LBX-backed audio
- animation assets loaded through the normal resource loaders
- Smacker video playback assets
- generic asset file reads that go through the VFS API
- configuration files and enabled-package manifests

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
2. enabled package `data/` projections in explicit `enable` priority
3. `data/base.zip`, if present
4. loose files under `data/`
5. raw asset files under the game base path
6. raw asset files under the configured fallback install path, if one is set

Loose overrides always win. The first enabled package wins over later packages, packages win over
`base.zip` and loose shared data, and all shared data wins over original raw assets.

`base.zip` is the only ZIP mounted implicitly from `data/`. Other ZIP files in that directory are ignored;
mods must use `data/mods/<package>.zip` and be activated with `enable`.

Matching is case-insensitive.

## Shared data and package layout

Recommended layout:

```text
data/
  orion2re.cfg
  overrides/
    COLBLDG0.LBX/000
    intro.lbx
  base.zip
  mods/
    example/
      mod.json5
      config/main.cfg
      data/music/theme.dat
    packaged-example.zip
```

Base archive contents use the same virtual paths as loose data. Package directories and package archives
instead have a private package root and an implicit public resource root:

- `data/mods/example/mod.json5` is `mods/example/mod.json5`
- `data/mods/example/config/main.cfg` is `mods/example/config/main.cfg`
- `data/mods/example/data/music/theme.dat` is `music/theme.dat`
- entry `data/music/theme.dat` in `data/mods/example.zip` is also `music/theme.dat`

Only the package's `data/` subtree is projected onto the public VFS root. Manifest and configuration files
remain under `mods/<package>/`. Package sources are prepared as staged layers so their configuration can be
validated before the new lookup order is committed.

The private `mods/<package>/` namespace can only be supplied by that package source. Loose overrides,
the base archive, loose shared data, and original install files cannot impersonate a package manifest or
configuration file.

## LBX support

LBX entry overrides are supported directly.

Per-entry override paths use this format:

```text
NAME.LBX/000
NAME.LBX/001
NAME.LBX/002
```

For `NAME.LBX` entry `12`, the VFS first searches `NAME.LBX/012` through the normal source order:

1. `data/overrides/`
2. enabled package layers
3. `data/base.zip`
4. loose `data/`
5. original and fallback raw files

If no entry override exists, the VFS searches for the whole `NAME.LBX` through the same source order and
returns a slice containing entry `12`.

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

This fallback applies to raw asset lookup only. It does not scan shared data or packages under the alternate
install path.

## Current limitations

- The VFS is read-only.
- Write/save paths remain normal local file I/O. Configuration input is read through the VFS.

## Summary

The current VFS behavior is:

- loose overrides, enabled packages, `base.zip` and loose data, then raw assets
- case-insensitive asset matching
- direct support for whole-file and per-entry LBX overrides
- one common read path for migrated asset loaders
- Smacker videos can be loaded from any mounted VFS source
- no VFS handling for writes