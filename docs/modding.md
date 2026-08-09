# Mod author guide

This guide explains how to create, install, enable, and combine mods for orion2re. A mod can change game configuration, replace read-only game assets through the virtual file system (VFS), or do both.

## Quick start

Create this layout in the game directory:

```text
data/
  orion2re.cfg
  mods/
    my_balance_mod/
      mod.json5
      config/
        main.cfg
      data/
```

Put this in `data/mods/my_balance_mod/mod.json5`:

```json5
{
  id: "my_balance_mod",
  name: "My Balance Mod",
  version: "1.0.0",

  requires: {
    engine: ">=1.60.0 <2.0.0",
  },

  config: "config/main.cfg",
}
```

Put an override in `data/mods/my_balance_mod/config/main.cfg`:

```text
# Statements end with semicolons.
ships guardian weapons 1 amount = 3;
```

Enable the package in `data/orion2re.cfg`:

```text
enable my_balance_mod;
```

Start the game. The loader resolves only packages named by an `enable` statement or by an enabled package's dependencies. It does not scan `data/mods/` and automatically enable what it finds.

Use a directory package while developing. Distribute the same contents as `data/mods/my_balance_mod.zip` when the mod is ready.

## Where mods go

Paths in this guide are relative to the game directory, alongside the orion2re executable and original game data.

A package may be installed in exactly one of these forms:

```text
data/mods/<id>/
data/mods/<id>.zip
```

Do not install both forms for the same ID. That is an error rather than a way to select one version.

Directory and ZIP packages have the same internal layout:

```text
<package root>/
  mod.json5             Required manifest
  config/main.cfg       Manifest-selected configuration entry point
  data/                 Public VFS asset overrides
    music/theme.dat
    INTRO.LBX
    COLBLDG0.LBX/000
```

The ZIP must contain `mod.json5`, `config/`, and `data/` at its root. Do not wrap them in another `<id>/` directory inside the archive.

Only files below the package's `data/` directory become public game assets. The manifest and CFG files remain private package files. An asset placed next to `mod.json5` will not override a game asset.

The package ID, directory or ZIP name, and manifest `id` must match. IDs must match this form:

```text
[a-z][a-z0-9_-]*
```

Use lowercase IDs and portable path names. VFS asset lookup itself is case-insensitive.

## The manifest

`mod.json5` is JSON5. Comments, trailing commas, unquoted property names, and single-quoted strings are accepted. The schema is strict: unknown and duplicate properties are errors.

### Required properties

| Property | Meaning |
| --- | --- |
| `id` | Stable package ID; must match the installed directory or ZIP name. |
| `name` | Human-readable package name. |
| `version` | Package version in `major.minor.patch` form. |
| `requires.engine` | Supported orion2re engine version range. |
| `config` | Safe package-relative path to the CFG entry point. |

Versions contain three decimal components and do not permit leading zeroes. A version range is a whitespace-separated set of comparisons, all of which must match:

```text
1.60.0
=1.60.0
>=1.60.0 <2.0.0
```

Supported comparison operators are `=`, `<`, `<=`, `>`, and `>=`. Caret, tilde, wildcard, OR, and hyphen range syntax are not supported.

Every package needs a `config` entry point, including an asset-only package. The CFG file may contain only comments when the package has no configuration overrides.

### Dependencies and conflicts

A complete manifest can also declare package relationships:

```json5
{
  id: "my_balance_mod",
  name: "My Balance Mod",
  version: "1.2.0",

  requires: {
    engine: ">=1.60.0 <2.0.0",
  },

  dependencies: [
    { id: "shared_rules", version: ">=2.0.0 <3.0.0" },
  ],

  optional_dependencies: [
    { id: "extra_portraits", version: ">=1.0.0" },
  ],

  conflicts: ["other_overhaul"],
  load_after: ["shared_rules"],
  config: "config/main.cfg",
}
```

- `dependencies` are required. Missing packages and version mismatches are fatal.
- `optional_dependencies` are ignored only when they are not installed. If an optional package exists, its manifest, version, dependencies, and configuration must be valid.
- `conflicts` rejects the load when the named package is also active.
- `load_after` is an ordering assertion, not an automatic sorter or dependency. If both packages are active, put the declaring package earlier in `orion2re.cfg` so it has higher priority.

Dependency entries may contain only `id` and `version`. Dependency cycles are rejected, and a shared dependency is loaded once.

## Enabling and ordering mods

Enable mods in `data/orion2re.cfg`:

```text
# Highest priority first.
enable my_compatibility_patch;
enable total_conversion;
enable shared_rules;
```

The first enabled package has the highest priority. This one ordering controls configuration and VFS assets:

- For an asset collision, the first enabled package supplies the file.
- Package CFG files are applied from lowest to highest priority, so the first enabled package writes last and wins a configuration collision.
- Package configuration overrides assignments in the base `data/orion2re.cfg`.
- Explicitly enabled packages have priority over packages pulled in only as dependencies.
- A dependency used by several packages is loaded once.

In the example, `my_compatibility_patch` can override both `total_conversion` and `shared_rules`. Package priority is intentionally "first wins," not "last wins."

An `enable` statement may also appear in a package CFG, but normal requirements should use `dependencies` in the manifest. Manifest dependencies are visible to tools and can be version-checked before configuration is applied.

## Authoring CFG overrides

CFG is the human-authored language for changing fields exposed by the engine. Basic statements are:

```text
# A comment continues to the end of the line.
path to field = value;
include more/settings.cfg;
enable another_mod;
stop;
```

Every statement ends with `;`. Paths use whitespace between components, not dots:

```text
ships guardian weapons 1 amount = 3;
ships antaran impossible titan weapons 8 amount = 12;
```

Array indexes follow the field definition and may be zero-based or one-based. Some arrays accept a symbolic index such as `impossible` instead of its number. Consult the configuration reference instead of assuming an index base.

Values may include:

- decimal, hexadecimal, and binary integers: `18`, `0x12`, `0b10010`
- symbolic values and booleans: `death_ray`, `true`, `false`
- comma-separated masks: `hv,eccm`
- ratios: `1/3`
- quoted strings with JSON-style escapes
- `-` to skip one value in a sequential assignment

### Records and sequential assignments

An assignment can change one field:

```text
ships guardian weapons 1 amount = 3;
```

It can also start at a record and supply fields in declaration order:

```text
ships guardian weapons 1 = death_ray all_sectors hv 0 2;
```

Use `-` to preserve a position:

```text
# Keep type and arc, replace mods, keep ammo, replace amount.
ships guardian weapons 1 = - - hv,eccm - 3;
```

Sequential writes are contained by the selected structure. A weapon-record assignment cannot accidentally continue into the next weapon slot. Too many values, an assignment containing only skips, unknown names, and out-of-range values are errors. A statement is validated before any of its values are written.

Prefer explicit field assignments when maintainability matters. Record and table assignments are concise, but their value order follows the configuration schema.

### Includes

Use includes to split a package into logical files:

```text
include balance/weapons.cfg;
include "interface options.cfg";
```

Include paths resolve relative to the CFG file containing the statement. Included statements appear at the include location, so later assignments can override earlier ones. Missing files and include cycles are errors. `stop;` ends only the current file and returns to its includer.

### Finding available fields and values

Open `docs/config-reference.html` from your orion2re installation. It lists public paths, types, valid enum and mask names, constraints, defaults, and examples. You do not need the game's source code to author CFG overrides.

For the complete CFG traversal model and schema design, see [Configuration and mod system design](config.md).

## VFS asset overrides

The VFS handles read-only assets loaded through the engine. To replace a complete asset, reproduce its virtual path below the package's `data/` directory. For example:

```text
data/mods/my_visual_mod/data/INTRO.LBX
```

provides the virtual asset `INTRO.LBX`. In a ZIP, the equivalent entry is:

```text
data/INTRO.LBX
```

The VFS covers assets such as LBX resources, streamed LBX audio, animations, Smacker videos, and generic VFS asset reads. It does not redirect saves, `MOX.SET`, `language.ini`, `orioncd.ini`, temporary files, or other write paths.

### VFS lookup order

For a virtual asset path, the game searches:

1. loose `data/overrides/`
2. enabled package `data/` projections, highest-priority package first
3. optional `data/base.zip`
4. loose files under `data/`
5. original files in the game directory
6. original files in the configured fallback install path, if present

The first matching source wins. This means:

- A loose file in `data/overrides/` always hides every mod version.
- The first enabled mod wins an asset collision with later enabled mods.
- Every enabled mod wins over `data/base.zip`, loose shared `data/`, and original assets.
- `data/base.zip` wins over loose shared `data/` and original assets.
- Loose shared `data/` wins over original assets.

`data/overrides/` is useful for local experiments, but it is not a managed package and does not require `enable`. Avoid using it for distributable mods because it bypasses package identity, dependencies, and ordering.

`data/base.zip` is the only archive mounted automatically from `data/`. Other ZIP files directly under `data/` are ignored. Mod archives belong under `data/mods/` and still require an `enable` statement.

Asset matching is case-insensitive. Keep only one spelling of each path; ZIP entries that normalize to the same virtual path are invalid. Unsafe archive paths, encrypted entries, unsupported entries, and malformed archives cause package loading to fail.

### Replacing one LBX entry

An individual LBX entry can be replaced without distributing the complete archive. Use the LBX name as a directory and a zero-padded three-digit entry number:

```text
data/mods/my_visual_mod/data/COLBLDG0.LBX/000
data/mods/my_visual_mod/data/COLBLDG0.LBX/012
```

When entry 12 is requested, the VFS checks `COLBLDG0.LBX/012` through the normal source order before falling back to entry 12 in a complete `COLBLDG0.LBX` file. The replacement file contains only that entry's payload, not an LBX header or table.

If both an entry override and a complete LBX replacement are visible, the entry override wins for that entry. Other entries continue to come from the highest-priority complete LBX file.

See [Virtual file system](vfs.md) for the lower-level asset-loading reference.

## Packaging a mod

Test the directory form first. When it is ready, use any ZIP application to archive the contents of the package directory. Select `mod.json5`, `config/`, and `data/` themselves, not the directory that contains them. This keeps `mod.json5` at the root of the archive.

Install the result as:

```text
data/mods/my_balance_mod.zip
```

Before distributing it, verify that:

- the archive is named exactly `<id>.zip`
- `mod.json5` is at the archive root
- the manifest ID matches the archive name
- the configured CFG entry point exists
- public assets are under `data/`
- archive paths are relative, normalized, and unique
- the ZIP is not encrypted
- the directory form has been removed from `data/mods/`

Open the completed archive before distributing it. Its top level should look like this:

```text
my_balance_mod.zip
  mod.json5
  config/
    main.cfg
  data/
```

## Failure behavior and troubleshooting

Mod loading is strict. Invalid syntax, an incompatible engine range, a missing required dependency, a conflict, a dependency or include cycle, an invalid archive, or a missing CFG file causes the load to fail rather than partially enabling a package.

Check these common mistakes first:

1. The package was copied to `data/mods/` but not enabled in `data/orion2re.cfg`.
2. A ZIP contains `my_mod/mod.json5` instead of root-level `mod.json5`.
3. Both `data/mods/my_mod/` and `data/mods/my_mod.zip` exist.
4. The directory, ZIP, manifest, and `enable` IDs do not match.
5. The manifest contains an unsupported property or a non-`major.minor.patch` version.
6. An asset was placed at the package root instead of under the package's `data/` directory.
7. Enabled mods were listed in the opposite order from the intended "first wins" priority.
8. A loose file in `data/overrides/` is hiding the package version of an asset.

When testing compatibility, enable the compatibility package first, then the package it is intended to override. Test configuration and asset collisions together because both use the same package priority.