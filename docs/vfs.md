# Virtual file system design

## Goal

Add a read-only virtual file system layer for game assets.

The VFS should:

- stop direct resource reads from going straight to raw files
- provide one common asset resolution path for loose overrides, zip mods, and raw game data
- support overriding whole files
- support overriding individual LBX entries without replacing the whole LBX
- preserve a C-style API shape that fits the existing codebase
- keep save games, config writes, and other write paths as normal local file I/O

This document records the current proposed design.

## Core decisions

### 1. The VFS is read-only

The VFS only handles asset reads.

It does not replace write paths such as:

- save games
- `MOX.SET`
- `language.ini`
- temporary local user files

Those can keep using existing local file APIs.

### 2. Resolution order is fixed

Asset lookup order should be:

1. `data/overrides/`
2. `data/*.zip`
3. raw game data

This means a loose override in `data/overrides/` always wins over a zip mod, and zip mods always win over the original raw data.

Zip ordering should be deterministic. The simplest rule is lexicographic filename order inside `data/`.

That makes mod priority easy to reason about with filename prefixes such as:

```text
data/
  overrides/
  10-base-fix.zip
  20-balance-mod.zip
  30-total-conversion.zip
```

### 3. The VFS should expose a C-style API

The public shape should match the style of the existing codebase.

The VFS should expose an opaque handle type and stdio-like functions.

Proposed public API:

```cpp
struct s_vfs_file;

namespace vfs {
    int8_t __cdecl Init(const char* base_path);
    void __cdecl Shutdown();

    s_vfs_file* __cdecl fopen(const char* path, const char* mode);
    s_vfs_file* __cdecl fopen_lbx(const char* lbx_name, int32_t entry_idx);

    size_t __cdecl fread(void* ptr, size_t size, size_t count, s_vfs_file* file);
    int32_t __cdecl fseek(s_vfs_file* file, int32_t offset, int32_t origin);
    int32_t __cdecl ftell(s_vfs_file* file);
    int32_t __cdecl fclose(s_vfs_file* file);

    int8_t __cdecl File_Exists(const char* path);
}
```

Notes:

- `vfs::fopen` is for generic asset files.
- `vfs::fopen_lbx` is the preferred API for LBX-backed resources.
- The handle hides whether the backing source is a raw file, a loose override file, or a zip-backed source.
- Initial implementation should remain read-only. Any non-read mode should fail.

### 4. The VFS should know about LBX

The VFS should not stay completely format-agnostic.

LBX is the dominant asset container in the game, and the modding requirement depends on LBX entry addressing.

So the design should include explicit LBX-aware behavior in the VFS through `vfs::fopen_lbx`.

This is preferable to forcing every existing loader to:

- open a whole LBX file
- parse the header itself
- seek to an entry offset itself
- add override logic itself

Instead:

- VFS resolves the requested LBX entry
- VFS returns a stream already scoped to the requested entry
- existing loaders become thin wrappers around `vfs::fopen_lbx`, `vfs::fread`, `vfs::fseek`, and `vfs::fclose`

## LBX override model

### 1. Per-entry virtual path mapping

LBX entry overrides should be addressable through a filename-like virtual path:

```text
COLBLDG0.LBX/000
COLBLDG0.LBX/001
COLBLDG0.LBX/002
```

Rules:

- the LBX filename remains the directory-like prefix
- entry indices are zero-padded decimal
- `000` means LBX entry 0
- `001` means LBX entry 1
- and so on

This path form should be used consistently in both `data/overrides/` and zip contents.

### 2. LBX lookup cascade

For `vfs::fopen_lbx("COLBLDG0.LBX", 12)`, the VFS should resolve in this order:

1. `data/overrides/COLBLDG0.LBX/012`
2. first matching zip entry `COLBLDG0.LBX/012` in zip priority order
3. `data/overrides/COLBLDG0.LBX`
4. first matching zip entry `COLBLDG0.LBX` in zip priority order
5. raw `COLBLDG0.LBX`

This gives two useful modding behaviors:

- replace just one entry
- replace the whole LBX

### 3. Returned LBX handle should be entry-relative

`vfs::fopen_lbx` should return a handle that behaves like a file containing only the selected entry.

That means:

- initial read position is 0 relative to the entry
- `vfs::fread` reads entry payload bytes only
- `vfs::fseek(file, 0, SEEK_SET)` goes to the start of the entry, not the LBX file start
- callers do not need to know the original LBX byte offset

This keeps the old loader logic simple and removes most direct LBX header parsing from call sites.

## Why keep both `fopen` and `fopen_lbx`

Not all asset reads are LBX entry reads.

The codebase also reads things like:

- whole LBX files in some paths
- Smacker files
- INI/config files
- loose copied support assets

So the VFS should provide:

- a generic asset stream API through `vfs::fopen`
- a higher-level LBX-aware API through `vfs::fopen_lbx`

The generic API remains useful, but most game resource loaders should migrate to `vfs::fopen_lbx` where possible.

## Proposed backend model

The public handle should be opaque.

Internally, a VFS handle can represent one of several backing kinds.

Conceptually:

```cpp
enum e_vfs_backend_kind {
    VFS_BACKEND_RAW_FILE,
    VFS_BACKEND_LOOSE_OVERRIDE_FILE,
    VFS_BACKEND_ZIP_ENTRY,
    VFS_BACKEND_LBX_ENTRY_FROM_FILE,
    VFS_BACKEND_LBX_ENTRY_FROM_ZIP_ENTRY,
};
```

The exact internal structure does not need to be public.

The main requirement is that the same external API works regardless of backing source.

## Zip library choice

The zip support should use a vendored, cross-platform library.

Recommended choice:

- `miniz`

Reasons:

- single-header and widely used
- cross-platform
- no external system dependency required
- suitable for straightforward zip-reader integration

The project already contains a copy inside the SDL vendor tree, but the VFS should use a project-owned vendored copy instead of depending on SDL internal source layout.

Suggested location:

```text
vendors/miniz/miniz.h
```

## Seek behavior and compressed zip entries

### 1. Do not start with temp-file materialization

The current preferred plan is to avoid temp-file extraction in the first implementation.

Reasons:

- more moving parts
- more lifecycle management
- unnecessary complexity for the first version
- not obviously required if most access patterns are forward-only or entry-relative

### 2. Entry-relative LBX streams reduce the seek problem

Once `vfs::fopen_lbx` returns an entry-relative handle, many LBX users no longer need large arbitrary seeks inside whole compressed LBX files.

That removes most of the original problem.

### 3. Compressed zip seek fallback

For zip-backed streams that do not support direct random access, the VFS should support repositioning by reopening and re-reading from the start of the underlying entry when needed.

This is acceptable for the first version.

Practical expectation:

- many asset loads are one-shot forward reads
- many LBX users can read from the beginning of a scoped entry handle
- some looping users may seek back to zero

If a zip-backed compressed stream needs to seek backwards:

- reopen the zip entry stream
- discard bytes until the requested offset is reached
- continue serving reads from that point

This is simpler than temp-file materialization and stays fully cross-platform.

### 4. If performance later requires it, temp materialization remains a fallback

If profiling later shows that some workloads are too slow, the next fallback can be:

- extract selected large zip-backed assets to memory
- or extract selected large zip-backed assets to a temporary file

But that should be a later optimization, not the starting design.

## Expected migration strategy

The VFS should be introduced in stages.

### Stage 1: generic VFS streams

Add:

- `vfs::Init`
- `vfs::Shutdown`
- `vfs::fopen`
- `vfs::fread`
- `vfs::fseek`
- `vfs::ftell`
- `vfs::fclose`
- `vfs::File_Exists`

At this stage, the VFS resolves:

- `data/overrides/`
- `data/*.zip`
- raw files

### Stage 2: LBX-aware open path

Add `vfs::fopen_lbx` and make it handle:

- per-entry overrides via `NAME.LBX/NNN`
- whole-LBX replacement via `NAME.LBX`
- raw LBX fallback

### Stage 3: wrap existing LBX loaders

Update these resource-focused paths first:

- `farload.cpp`
- `buffer.cpp`
- `lbx.cpp`
- `file_ani.cpp`
- LBX-backed parts of `sound.cpp`

These are the main places currently doing direct LBX file parsing and seeking.

### Stage 4: update read-only asset existence checks

Update read-only asset checks to use VFS-aware existence rules where appropriate.

Likely candidates:

- `dos::DIR_` when used for asset presence checks
- `File_Exists` for asset read paths
- intro and cinematic asset discovery

This should be done carefully so save/config behavior is not unintentionally changed.

## Data directory layout

Recommended external directory layout:

```text
data/
  overrides/
    COLBLDG0.LBX/000
    COLBLDG0.LBX/001
    intro.lbx
  10-base-fixes.zip
  20-balance.zip
  30-total-conversion.zip
```

Rules:

- `data/overrides/` contains loose highest-priority overrides
- zip files live directly under `data/`
- zip contents use the same virtual paths as loose overrides

Example zip contents:

```text
COLBLDG0.LBX/000
COLBLDG0.LBX/001
COLBLDG0.LBX
STREAMHD.LBX/005
intro.lbx
```

## Non-goals for the first version

The first VFS version should not try to solve everything.

Non-goals:

- replacing write paths
- building a generic archive abstraction for every future format
- implementing aggressive asset caching before correctness exists
- building a temp-file extraction system first
- rewriting unrelated file I/O outside resource loading

## Open questions

The main remaining design questions are implementation details, not API shape.

### 1. Zip enumeration and caching

Should the VFS scan zip central directories at startup and cache filenames, or should it open and search lazily?

Current expectation:

- scan once during `vfs::Init`
- cache entry names and lookup metadata
- open compressed payload streams lazily

### 2. Case handling

The current codebase already contains case-insensitive path handling helpers.

The VFS should define one canonical internal comparison policy so that:

- loose overrides
- zip entries
- raw file fallback

all follow the same asset name matching rules.

### 3. LBX metadata caching

It will likely be useful to cache parsed LBX offset tables for whole-LBX replacements, especially zip-backed ones.

That is compatible with this design and should be considered a practical implementation detail.

## Summary

The current recommended VFS direction is:

- read-only VFS for assets only
- resolution order: `data/overrides/`, then zip mods, then raw game data
- C-style API with an opaque `s_vfs_file` handle
- generic `vfs::fopen/fread/fseek/fclose`
- explicit LBX-aware `vfs::fopen_lbx`
- LBX entry override path format `NAME.LBX/NNN`
- no temp-file extraction in the first version
- backward seek for compressed zip-backed streams handled by reopen-and-skip

This keeps the public API simple, fits the existing code style, and directly supports the required modding model.