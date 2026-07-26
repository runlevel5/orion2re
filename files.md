# Instructions

## Goal

Normalize every internal star, ship, planet, and colony index to `int16_t`.
This is a type-only mass refactor: preserve gameplay behavior, sentinel behavior,
savegame compatibility, and the layout of save-backed state.

The legacy representations currently include:

- star indexes: commonly `int8_t`
- ship indexes: commonly `int16_t`
- colony indexes: commonly `uint8_t`, often widened to signed types; `0xFF`
  represents no colony in byte-sized contexts
- planet indexes: commonly `int16_t`

These are historical facts, not the desired runtime types. After this refactor,
all four index domains use signed `int16_t` internally.

## Non-negotiable constraints

1. Do not change the savegame format, savegame field widths, or savegame version.
2. Do not change the declaration, size, alignment, member order, or array extent
   of save-backed records listed below, including nested members.
3. Do not change save-backed global declarations merely to normalize an index.
4. Do not reinterpret an index as an ID, count, bit mask, enum, coordinate, ship
   design index, icon index, or other unrelated value based only on its name.
5. Networking packets and their encode/decode logic may change without backward
   compatibility. Embedded copies of save-backed records remain protected even
   when networking transfers them as raw bytes.
6. Preserve all existing invalid-index semantics. Internally, use the signed
   value `-1`; translate an unsigned colony index value of `0xFF` to `-1` when
   it enters internal code and translate it back only at a protected boundary.
7. Do not introduce a new public packed index type or change unrelated integer
   parameters during this work.

## Savegame boundary inventory

`src/game/savegame.cpp` is the source of truth. Its `Write_Game_State_` and
`Read_Game_State_` paths serialize fields individually, but several members are
written as byte arrays. Treat every populated record and nested record as
layout-sensitive.

Do not normalize fields inside these save-backed types:

- `s_settings`
- `s_ship_weapons`
- `s_ship_design`
- `s_colony`
- `s_planet_data`
- `s_star_data`
- `s_leader_data`
- `s_player`
- `s_ship_data`
- `s_movable_box`
- `s_nebula`
- `s_report`
- `s_event_data`
- `s_MSG_colony`
- `s_MSG` and its variant payload records
- `s_antaran`

The serialized global state is also protected. This includes:

- `MOX::_settings`, `MOX::_colony`, `MOX::_planet`, `MOX::_star`,
  `MOX::_leaders`, `MOX::_player`, `MOX::_ship`, `MOX::_moveable_box`,
  `MOX::_moveable_box_order`, `MOX::_nebula`, `MOX::_antarans`, and
  `MOX::_bill_savegame`
- the scalar counts, map state, stardate, game state, and Antaran state written
  by `Write_Game_State_`
- `REPORT::_reports` and the serialized report state
- `EVENTS::_event_data` and the serialized event state
- `MSG_::_msgs` and the serialized message-list state

Before changing any questionable struct or global, trace it through
`Write_Game_State_` and `Read_Game_State_`. If either path serializes it, or a
nested object containing it, leave its declaration unchanged. Add newly found
save-backed types to this inventory before continuing.

Known protected index-bearing fields include, but are not limited to:

- `s_planet_data::colony_index` and `s_planet_data::star_index`
- `s_star_data::wormhole_star_id`, `s_star_data::planet_index`, and other
  index-bearing star members
- `s_colony::planet`
- index-bearing fields in `s_ship_data`, `s_player`, `s_leader_data`, reports,
  events, and `s_MSG` payloads

Protection applies to the declaration and stored representation, not to values
after they have crossed into runtime logic.

## Canonical internal representation

Use `int16_t` for a value when it represents an index into any of these global
collections:

- `MOX::_star`
- `MOX::_ship`
- `MOX::_planet`
- `MOX::_colony`

Normalize all non-protected occurrences, including:

- function parameters and return values
- matching declarations, definitions, callbacks, and function-pointer types
- locals, loop indexes used as entity indexes, and temporaries
- runtime-only struct and union fields
- runtime arrays that store indexes
- test fakes, fixtures, and assertions matching changed interfaces
- gameplay network packet fields and their encode/decode temporaries

Do not blindly convert every `int`, `char`, or field containing one of the four
words. Establish that the value denotes an entity index by tracing its producer,
consumer, array access, or pointer difference.

Use `int16_t`, not plain `short`, `int`, `char`, `signed char`, `uint16_t`, or
`uint8_t`. Keep counts and container lengths in their existing appropriate type;
only an iterator that is itself passed or stored as an entity index must be
normalized.

## Conversion and narrowing-cast rules

Once a value enters internal code, convert it to `int16_t` and keep it there.
Do not repeatedly widen, narrow, or switch signedness through the call chain.

Do not add explicit widening casts. In particular, do not cast signed `int8_t`,
`int16_t`, or other narrower signed values merely to assign, pass, compare, or
return them as `int16_t` or a wider type. Use the canonical type throughout and
allow ordinary value-preserving conversion where a protected field remains
narrower.

The exception is a semantically meaningful unsigned-colony conversion where
`UINT8_MAX` means no colony. The conversion must explicitly translate that
sentinel to internal `-1`; it must not rely on a signedness cast to produce `-1`.

Every explicit narrowing cast of a star, ship, planet, or colony index must be
necessary for a protected externally facing representation. Put this exact
stable marker on every such narrowing cast so all narrowing sites can be found
later:

```cpp
/* INDEX_NARROWING */
```

Keep the marker on the same line as the cast. Reading a narrower signed field
does not require a cast; writing one does:

```cpp
const int16_t star_idx = planet.star_index;
planet.star_index = static_cast<int8_t>(star_idx); /* INDEX_NARROWING */
```

For an unsigned byte with `0xFF` as the invalid value, preserve the sentinel
explicitly. The widening cast is allowed here because the unsigned-to-signed
colony conversion is semantic rather than cosmetic:

```cpp
const int16_t colony_idx = stored_colony_idx == UINT8_MAX
    ? int16_t{-1}
    : static_cast<int16_t>(stored_colony_idx);
stored_colony_idx = colony_idx < 0
    ? UINT8_MAX
    : static_cast<uint8_t>(colony_idx); /* INDEX_NARROWING */
```

If a protected field is already `int16_t`, assignment to or from the canonical
runtime type needs no cast or marker. Never add a cast merely to silence a
conversion that is already value-preserving.

Every remaining cast directly between an entity index and `int8_t`, `uint8_t`,
`uint16_t`, `int32_t`, `uint32_t`, `int`, or `char` must either:

- be removed because both sides can use `int16_t`; or
- be a semantically necessary unsigned-colony conversion; or
- be a necessary narrowing conversion and carry `INDEX_NARROWING`.

Pointer subtraction may be explicitly converted once to `int16_t` after the
result is known to fit. This is an index construction, not a save boundary, so
do not attach the narrowing marker.

## Networking

Networking is not a compatibility boundary for this task. Normalize star, ship,
planet, and colony index fields in packet structs, packet-local records, encoder
and decoder parameters, and packet validation to `int16_t` together.

Update packet size checks and tests when wire widths change. Do not widen raw
embedded save-backed records such as `s_player`, `s_colony`, `s_ship_data`, or
`s_leader_data`; those records remain protected even when sent over the network.
Any unavoidable narrowing cast in networking code still requires
`INDEX_NARROWING`, although packet fields should normally be changed to
`int16_t` so no such cast remains.

## Per-file workflow

The workflow provides one claimed file name at a time. Treat that file as the
entry point for the current iteration. Use the provided queue as-is and do not
create a replacement file list.

1. Read and validate every line of the claimed file as required below.
2. Trace only the declarations, definitions, callers, callbacks, data
   definitions, packet code, and tests needed to make that file coherent.
3. Normalize interfaces.
   - Change the header declaration and implementation together.
   - Propagate `int16_t` through affected callers and callees.
4. Normalize storage and expressions.
   - Change runtime-only fields, arrays, locals, returns, and loop variables.
   - Replace unsigned invalid values with internal `-1` semantics.
5. Clean casts.
   - Remove cosmetic widening casts and casts made redundant by the canonical
     type.
   - Mark every necessary narrowing cast with `INDEX_NARROWING`.
6. Update affected networking code and tests.
7. Search for stale declarations and call sites specifically related to the
   claimed file and the interfaces changed in this iteration.
8. Build and run focused tests before the iteration is complete.

## Mandatory line-by-line source validation

Every single line of the claimed source file must be read and validated; search
results and symbol matches are not a substitute for reading the complete file.
This requirement also applies when the claimed file appears unrelated to the
four index domains.

For each line, determine whether it declares, receives, returns, stores,
converts, compares, performs arithmetic on, indexes with, serializes, or sends a
star, ship, planet, or colony index. Inspect unnamed temporaries, loop variables,
plain `char` values, aliases, union members, macros, callbacks, and pointer
subtractions as carefully as explicitly named `*_idx` variables.

Validate every function body and every declaration in context. Follow a
dependency outside the claimed file when necessary to establish the value's
meaning or keep an interface consistent, but do not use that as a reason to skip
any line in the claimed file. Before finishing, make a second complete pass over
the claimed file and confirm that no relevant line was missed.

## Verification

After each cluster:

```sh
cmake --build build-tests -j
ctest --test-dir build-tests --output-on-failure
```

Run the focused savegame test whenever code touches a protected record, a
save-boundary conversion, or save/load code:

```sh
ctest --test-dir build-tests --output-on-failure -R savegame
```

Run networking-focused tests after packet changes:

```sh
ctest --test-dir build-tests --output-on-failure -R 'net|router'
```

Before completion, perform repository-wide audits for mixed-width declarations,
casts involving the four index domains, unmarked protected conversions, and
unexpected changes to `src/game/savegame.cpp` or protected declarations.

## Completion criteria

The refactor is complete only when all of the following are true:

- all confirmed internal star, ship, planet, and colony indexes are `int16_t`
- all related signatures agree across headers, implementations, callbacks, and
  tests
- all runtime-only index fields and networking index fields are normalized
- internal invalid indexes consistently use `-1`
- cosmetic widening casts and redundant narrowing or signedness casts have been
  removed
- every necessary narrowing cast has the exact `INDEX_NARROWING` marker
- do not use C++ style casts, follow existing project style
- every line of each claimed file has been validated and reviewed a second time
- protected save-backed declarations and serialized widths are unchanged
- existing versioned and legacy savegame tests pass without a version bump
- focused networking tests and the full test suite pass
- every remaining mixed-width occurrence has been manually classified as a
  protected boundary or as unrelated to an entity index

Do not mark work complete merely because the project compiles. The final cast
audit, protected-layout review, savegame tests, and network tests are required.

# Files

[X] src/game/advciv.cpp
[X] src/game/advciv.h
[ ] src/game/aibomb.cpp
[ ] src/game/aibomb.h
[ ] src/game/aibuild.cpp
[ ] src/game/aibuild.h
[ ] src/game/aidata.cpp
[ ] src/game/aidata.h
[ ] src/game/aidesign.cpp
[ ] src/game/aidesign.h
[ ] src/game/aidudes.cpp
[ ] src/game/aidudes.h
[ ] src/game/aileader.cpp
[ ] src/game/aileader.h
[ ] src/game/aimove.cpp
[ ] src/game/aimove.h
[ ] src/game/aipower.cpp
[ ] src/game/aipower.h
[ ] src/game/aistage.cpp
[ ] src/game/aistage.h
[ ] src/game/aitech.cpp
[ ] src/game/aitech.h
[ ] src/game/allocate.cpp
[ ] src/game/allocate.h
[ ] src/game/angle.cpp
[ ] src/game/angle.h
[ ] src/game/animate.cpp
[ ] src/game/animate.h
[ ] src/game/antarans.cpp
[ ] src/game/antarans.h
[ ] src/game/antaroom.cpp
[ ] src/game/antaroom.h
[ ] src/game/beams.cpp
[ ] src/game/beams.h
[ ] src/game/bill.cpp
[ ] src/game/bill.h
[ ] src/game/billhelp.cpp
[ ] src/game/billhelp.h
[ ] src/game/bitmap.cpp
[ ] src/game/bitmap.h
[ ] src/game/bolt.cpp
[ ] src/game/bolt.h
[ ] src/game/bomb.cpp
[ ] src/game/bomb.h
[ ] src/game/buffer.cpp
[ ] src/game/buffer.h
[ ] src/game/capture.cpp
[ ] src/game/capture.h
[ ] src/game/chat.cpp
[ ] src/game/chat.h
[ ] src/game/clipped.cpp
[ ] src/game/clipped.h
[ ] src/game/cmbtai.cpp
[ ] src/game/cmbtai.h
[ ] src/game/cmbtdrw1.cpp
[ ] src/game/cmbtdrw1.h
[ ] src/game/cmbtfir2.cpp
[ ] src/game/cmbtfir2.h
[ ] src/game/cmbtfire.cpp
[ ] src/game/cmbtfire.h
[ ] src/game/cmbtmis.cpp
[ ] src/game/cmbtmis.h
[ ] src/game/cmbtmov1.cpp
[ ] src/game/cmbtmov1.h
[ ] src/game/cmbtspec.cpp
[ ] src/game/cmbtspec.h
[ ] src/game/colbcach.cpp
[ ] src/game/colbcach.h
[ ] src/game/colbldg.cpp
[ ] src/game/colbldg.h
[ ] src/game/colbomb.cpp
[ ] src/game/colbomb.h
[ ] src/game/colcalc.cpp
[ ] src/game/colcalc.h
[ ] src/game/colcalc_main.cpp
[ ] src/game/coldraw.cpp
[ ] src/game/coldraw.h
[ ] src/game/colgcbt.cpp
[ ] src/game/colgcbt.h
[ ] src/game/colland.cpp
[ ] src/game/colland.h
[ ] src/game/colmove.cpp
[ ] src/game/colmove.h
[ ] src/game/colonize.cpp
[ ] src/game/colonize.h
[ ] src/game/colony.cpp
[ ] src/game/colony.h
[ ] src/game/colony_main.cpp
[ ] src/game/colrefit.cpp
[ ] src/game/colrefit.h
[ ] src/game/colsum.cpp
[ ] src/game/colsum.h
[ ] src/game/colsysdi.cpp
[ ] src/game/colsysdi.h
[ ] src/game/colxport.cpp
[ ] src/game/colxport.h
[ ] src/game/combat.cpp
[ ] src/game/combat.h
[ ] src/game/combat1.cpp
[ ] src/game/combat1.h
[ ] src/game/combfind.cpp
[ ] src/game/combfind.h
[ ] src/game/combinit.cpp
[ ] src/game/combinit.h
[ ] src/game/compat.cpp
[ ] src/game/compat.h
[ ] src/game/consts.h
[ ] src/game/contact.cpp
[ ] src/game/contact.h
[ ] src/game/council.cpp
[ ] src/game/council.h
[ ] src/game/deps.h
[ ] src/game/desbox.cpp
[ ] src/game/desbox.h
[ ] src/game/design.cpp
[ ] src/game/design.h
[ ] src/game/design_main.cpp
[ ] src/game/dip_scrn.cpp
[ ] src/game/dip_scrn.h
[ ] src/game/dip_scrn_main.cpp
[ ] src/game/diplodef.cpp
[ ] src/game/diplodef.h
[ ] src/game/diplomac.cpp
[ ] src/game/diplomac.h
[ ] src/game/dos.cpp
[ ] src/game/dos.h
[ ] src/game/draw.cpp
[ ] src/game/draw.h
[ ] src/game/eric.cpp
[ ] src/game/eric.h
[ ] src/game/erichelp.cpp
[ ] src/game/erichelp.h
[ ] src/game/ericnet.cpp
[ ] src/game/ericnet.h
[ ] src/game/estrings.cpp
[ ] src/game/estrings.h
[ ] src/game/evanhelp.cpp
[ ] src/game/evanhelp.h
[ ] src/game/events.cpp
[ ] src/game/events.h
[ ] src/game/exitm.cpp
[ ] src/game/exitm.h
[ ] src/game/fade.cpp
[ ] src/game/fade.h
[ ] src/game/fakebuts.cpp
[ ] src/game/fakebuts.h
[ ] src/game/farload.cpp
[ ] src/game/farload.h
[ ] src/game/fields.cpp
[ ] src/game/fields.h
[ ] src/game/fieldsav.cpp
[ ] src/game/fieldsav.h
[ ] src/game/file_ani.cpp
[ ] src/game/file_ani.h
[ ] src/game/filedef.cpp
[ ] src/game/filedef.h
[ ] src/game/firepts.cpp
[ ] src/game/firepts.h
[ ] src/game/flcqueue.cpp
[ ] src/game/flcqueue.h
[ ] src/game/fleetpop.cpp
[ ] src/game/fleetpop.h
[ ] src/game/flt.cpp
[ ] src/game/flt.h
[ ] src/game/flt1.cpp
[ ] src/game/flt1.h
[ ] src/game/flt2.cpp
[ ] src/game/flt2.h
[ ] src/game/fmtpara.cpp
[ ] src/game/fmtpara.h
[ ] src/game/fontdraw.cpp
[ ] src/game/fontdraw.h
[ ] src/game/fonts.cpp
[ ] src/game/fonts.h
[ ] src/game/gendraw.cpp
[ ] src/game/gendraw.h
[ ] src/game/geo.cpp
[ ] src/game/geo.h
[ ] src/game/global.cpp
[ ] src/game/global.h
[ ] src/game/graphics.cpp
[ ] src/game/graphics.h
[ ] src/game/haccess.cpp
[ ] src/game/haccess.h
[ ] src/game/harold.cpp
[ ] src/game/harold.h
[ ] src/game/homegen.cpp
[ ] src/game/homegen.h
[ ] src/game/hotpop.cpp
[ ] src/game/hotpop.h
[ ] src/game/info.cpp
[ ] src/game/info.h
[ ] src/game/init.cpp
[ ] src/game/init.h
[ ] src/game/initgame.cpp
[ ] src/game/initgame.h
[ ] src/game/initship.cpp
[ ] src/game/initship.h
[ ] src/game/invasion.cpp
[ ] src/game/invasion.h
[ ] src/game/jim.cpp
[ ] src/game/jim.h
[ ] src/game/ken.cpp
[ ] src/game/ken.h
[ ] src/game/key.cpp
[ ] src/game/key.h
[ ] src/game/lbx_types.h
[ ] src/game/line.cpp
[ ] src/game/line.h
[ ] src/game/list.cpp
[ ] src/game/list.h
[ ] src/game/loader.cpp
[ ] src/game/loader.h
[ ] src/game/loadsave.cpp
[ ] src/game/loadsave.h
[ ] src/game/lostship.cpp
[ ] src/game/lostship.h
[ ] src/game/mainmenu.cpp
[ ] src/game/mainmenu.h
[ ] src/game/mainpups.cpp
[ ] src/game/mainpups.h
[ ] src/game/mainscr.cpp
[ ] src/game/mainscr.h
[ ] src/game/mainscr2.cpp
[ ] src/game/mainscr2.h
[ ] src/game/mainscr_main.cpp
[ ] src/game/maintain.cpp
[ ] src/game/maintain.h
[ ] src/game/mapgen.cpp
[ ] src/game/mapgen.h
[ ] src/game/misc.cpp
[ ] src/game/misc.h
[ ] src/game/mouse.cpp
[ ] src/game/mouse.h
[ ] src/game/movebox.cpp
[ ] src/game/movebox.h
[ ] src/game/mox.cpp
[ ] src/game/mox.h
[ ] src/game/mox2.cpp
[ ] src/game/mox2.h
[ ] src/game/msg.cpp
[ ] src/game/msg.h
[ ] src/game/multplay.cpp
[ ] src/game/multplay.h
[ ] src/game/namestar.cpp
[ ] src/game/namestar.h
[ ] src/game/netadapter.cpp
[ ] src/game/netadapter.h
[ ] src/game/netcode.cpp
[ ] src/game/netcode.h
[ ] src/game/netmox.cpp
[ ] src/game/netmox.h
[ ] src/game/netproto.cpp
[ ] src/game/netproto.h
[ ] src/game/netstart.cpp
[ ] src/game/netstart.h
[ ] src/game/netstrat.cpp
[ ] src/game/netstrat.h
[ ] src/game/netturn.cpp
[ ] src/game/netturn.h
[ ] src/game/newgame.cpp
[ ] src/game/newgame.h
[ ] src/game/newpup.cpp
[ ] src/game/newpup.h
[ ] src/game/newtech.cpp
[ ] src/game/newtech.h
[ ] src/game/nextturn.cpp
[ ] src/game/nextturn.h
[ ] src/game/npcdiplo.cpp
[ ] src/game/npcdiplo.h
[ ] src/game/ocpolpup.cpp
[ ] src/game/ocpolpup.h
[ ] src/game/officer.cpp
[ ] src/game/officer.h
[ ] src/game/oracle.cpp
[ ] src/game/oracle.h
[ ] src/game/orion2.h
[ ] src/game/orion2_consts.h
[ ] src/game/orion2_str.h
[ ] src/game/palette.cpp
[ ] src/game/palette.h
[ ] src/game/palstore.cpp
[ ] src/game/palstore.h
[ ] src/game/paragrph.cpp
[ ] src/game/paragrph.h
[ ] src/game/platform.cpp
[ ] src/game/platform.h
[ ] src/game/plntsum.cpp
[ ] src/game/plntsum.h
[ ] src/game/pop.h
[ ] src/game/racecust.cpp
[ ] src/game/racecust.h
[ ] src/game/raceopt.cpp
[ ] src/game/raceopt.h
[ ] src/game/racerprt.cpp
[ ] src/game/racerprt.h
[ ] src/game/racescrn.cpp
[ ] src/game/racescrn.h
[ ] src/game/racesel.cpp
[ ] src/game/racesel.h
[ ] src/game/random.cpp
[ ] src/game/random.h
[ ] src/game/reduce.cpp
[ ] src/game/reduce.h
[ ] src/game/regions.cpp
[ ] src/game/regions.h
[ ] src/game/remap.cpp
[ ] src/game/remap.h
[ ] src/game/replace.cpp
[ ] src/game/replace.h
[ ] src/game/report.cpp
[ ] src/game/report.h
[ ] src/game/russ.cpp
[ ] src/game/russ.h
[ ] src/game/science.cpp
[ ] src/game/science.h
[ ] src/game/score.cpp
[ ] src/game/score.h
[ ] src/game/scroll.cpp
[ ] src/game/scroll.h
[ ] src/game/settings.h
[ ] src/game/settler.cpp
[ ] src/game/settler.h
[ ] src/game/setvesa.cpp
[ ] src/game/setvesa.h
[ ] src/game/shear.cpp
[ ] src/game/shear.h
[ ] src/game/shipmove.cpp
[ ] src/game/shipmove.h
[ ] src/game/ships.cpp
[ ] src/game/ships.h
[ ] src/game/shipstak.cpp
[ ] src/game/shipstak.h
[ ] src/game/sizes.h
[ ] src/game/smack.cpp
[ ] src/game/smack.h
[ ] src/game/smckplay.cpp
[ ] src/game/smckplay.h
[ ] src/game/sound.cpp
[ ] src/game/sound.h
[ ] src/game/sound_types.h
[ ] src/game/special.cpp
[ ] src/game/special.h
[ ] src/game/spy.cpp
[ ] src/game/spy.h
[ ] src/game/string.cpp
[ ] src/game/string.h
[ ] src/game/strings.cpp
[ ] src/game/strings.h
[ ] src/game/struct.cpp
[ ] src/game/struct.h
[ ] src/game/swap.cpp
[ ] src/game/swap.h
[ ] src/game/sys.cpp
[ ] src/game/sys.h
[ ] src/game/sysfont.cpp
[ ] src/game/sysfont.h
[ ] src/game/tech.cpp
[ ] src/game/tech.h
[ ] src/game/techdata.cpp
[ ] src/game/techdata.h
[ ] src/game/techinit.cpp
[ ] src/game/techinit.h
[ ] src/game/textbox.cpp
[ ] src/game/textbox.h
[ ] src/game/time.cpp
[ ] src/game/time.h
[ ] src/game/timer.cpp
[ ] src/game/timer.h
[ ] src/game/trade.cpp
[ ] src/game/trade.h
[ ] src/game/turnsum.cpp
[ ] src/game/turnsum.h
[ ] src/game/types.h
[ ] src/game/vfs.cpp
[ ] src/game/vfs.h
[ ] src/game/vfs_detail.h
[ ] src/game/vfs_file.cpp
[ ] src/game/vfs_lbx.cpp
[ ] src/game/vfs_path.cpp
[ ] src/game/vfs_zip.cpp
[ ] src/game/video.cpp
[ ] src/game/video.h
[ ] src/game/video_scan_bounds.h
[ ] src/main.cpp
[ ] src/network/router_cli.cpp
[ ] src/network/router_cli.h
[ ] src/network/router_client.cpp
[ ] src/network/router_client.h
[ ] src/network/router_core.cpp
[ ] src/network/router_core.h
[ ] src/network/router_protocol.cpp
[ ] src/network/router_protocol.h
[ ] src/network/router_service.cpp
[ ] src/network/router_service.h
[ ] src/network/router_session_browser.cpp
[ ] src/network/router_session_browser.h
[ ] src/pch.cpp
[ ] src/pch.h
[ ] src/strbuild.cpp
[ ] src/strbuild.h
