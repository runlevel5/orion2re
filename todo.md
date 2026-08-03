# TODO

## Replace game-domain magic numbers with consistent enums and masks

Prepare authoritative definitions before using `.vocode/workflows/mass.yaml` for file-level replacements. Preserve numeric behavior and serialized layouts; this work names existing domains rather than renumbering them.

### 1. Define and verify domains

- [ ] Inventory finite game domains used by configuration and restored gameplay, recording storage width, signedness, valid values, sentinels, aliases, and whether each domain is an enum, bit mask, index, or numeric quantity.
- [ ] Reconcile existing definitions in `src/game/orion2_consts.h`, including names, count sentinels, spelling, zero values, and distinctions between IDs and bit masks.
- [x] Add verified definitions for weapon firing arcs, natural weapon properties, game setup, governments, stock-race table indexes, crew quality, strategic special categories, technology application types, map environmental groups, map-generation satellite choices, leader types, AI special-choice item kinds, and AI ship themes.
- [x] Preserve the distinct tactical all-sector and design-screen 360-degree firing-arc encodings, and retain weapon-modification bit 0 as reserved.
- [x] Keep wire/save/native numeric values stable and add compile-time checks for important counts, bit positions, and table extents.

#### Deferred domains and unresolved names

- [ ] Recover `s_leader_data::status` values -2 through 6; existing comments conflict for statuses 4 and 5.
- [ ] Separate generic race-like values into stock-race table indexes, player ownership, packed-population race values, Android/native values, and custom-race selection domains.
- [ ] Recover building category names 0 through 6; only category 7 (satellite/system defense) is semantically proven.
- [ ] Recover technology application `ai_group` names for values 0 through 39.
- [ ] Treat technology field `ai_group` values 0 through 22 as ordered research levels unless further semantics are recovered.
- [ ] Recover `unk_class` and other unknown technology fields before naming them.
- [ ] Keep AI design allocation fields as percentages with disabled sentinel -1 rather than enums.
- [ ] Resolve the AI design table class mismatch: native source has seven tables, while 1.50 declares eight names and native class 7 aliases table 0.
- [ ] Convert special exclusion entries to `SPECIAL` IDs without treating the entries as a mask.
- [ ] Model orbit and random-roll dimensions as schema aliases or indexes rather than native gameplay enums.

### 2. Prepare the mass-update queue

- [x] Create `files.md` only after the domain definitions and naming rules are reviewed. Put global replacement rules under `# Instructions` and one source entry point per `[ ] <filename>` line under `# Files`.
- [x] Instruct the mass workflow to replace raw literals only when the field's semantic domain is proven, to leave quantities and unrelated bit fields unchanged, and to report ambiguous values instead of guessing.
- [x] Process definitions and central data tables before behavioral call sites, then search globally for stale literals and partially migrated domains.
- [x] Keep each claimed-file change focused; do not combine enum migration with unrelated cleanup or behavior changes.

### 3. Define JSON5 schema types independently

- [x] Recreate the verified domains explicitly under `types` in `specs/config/main.json5`; the configuration tool must not parse C/C++ headers.
- [x] Use separate schema types for semantic IDs, table indexes, masks, and similarly named domains with different encodings, including weapon modification IDs, available-modification flags, natural weapon properties, and firing arcs.
- [ ] Use enum-backed `index_type` declarations for configurable arrays such as weapons, specials, buildings, technologies, and technology fields.
- [x] Prefer stable lowercase configuration names while retaining exact native numeric values; reject unknown numeric values where the domain is complete.
- [x] Add validation tests that pin representative and boundary values in both the JSON5 declarations and native enums so duplicated definitions cannot drift silently.

### 4. Verify each migration wave

- [x] Build after central definitions and after each domain wave.
- [ ] Run focused tests for configuration schema/generation, save serialization, networking, ship design and combat, technology data, map generation, leaders, and new-game setup as applicable.
- [ ] Verify generated configuration output, save/network numeric compatibility, and all configured table indexes against their native counts.
- [ ] Finish with global searches for raw domain literals, `git diff --check`, the full test suite, and a review of any intentionally retained numeric values.