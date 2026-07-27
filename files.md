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
[X] src/game/aibomb.cpp
[X] src/game/aibomb.h
[X] src/game/aibuild.cpp
[X] src/game/aibuild.h
[X] src/game/aidata.cpp
[X] src/game/aidata.h
[X] src/game/aidesign.cpp
[X] src/game/aidesign.h
[X] src/game/aidudes.cpp
[X] src/game/aidudes.h
[X] src/game/aileader.cpp
[X] src/game/aileader.h
[X] src/game/aimove.cpp
[X] src/game/aimove.h
[X] src/game/aipower.cpp
[X] src/game/aipower.h
[X] src/game/aistage.cpp
[X] src/game/aistage.h
[X] src/game/aitech.cpp
[X] src/game/aitech.h
[X] src/game/allocate.cpp
[X] src/game/allocate.h
[X] src/game/angle.cpp
[X] src/game/angle.h
[X] src/game/animate.cpp
[X] src/game/animate.h
[X] src/game/antarans.cpp
[X] src/game/antarans.h
[X] src/game/antaroom.cpp
[X] src/game/antaroom.h
[X] src/game/beams.cpp
[X] src/game/beams.h
[X] src/game/bill.cpp
[X] src/game/bill.h
[X] src/game/billhelp.cpp
[X] src/game/billhelp.h
[X] src/game/bitmap.cpp
[X] src/game/bitmap.h
[X] src/game/bolt.cpp
[X] src/game/bolt.h
[X] src/game/bomb.cpp
[X] src/game/bomb.h
[X] src/game/buffer.cpp
[X] src/game/buffer.h
[X] src/game/capture.cpp
[X] src/game/capture.h
[X] src/game/chat.cpp
[X] src/game/chat.h
[X] src/game/clipped.cpp
[X] src/game/clipped.h
[X] src/game/cmbtai.cpp
[X] src/game/cmbtai.h
[X] src/game/cmbtdrw1.cpp
[X] src/game/cmbtdrw1.h
[X] src/game/cmbtfir2.cpp
[X] src/game/cmbtfir2.h
[X] src/game/cmbtfire.cpp
[X] src/game/cmbtfire.h
[X] src/game/cmbtmis.cpp
[X] src/game/cmbtmis.h
[X] src/game/cmbtmov1.cpp
[X] src/game/cmbtmov1.h
[X] src/game/cmbtspec.cpp
[X] src/game/cmbtspec.h
[X] src/game/colbcach.cpp
[X] src/game/colbcach.h
[X] src/game/colbldg.cpp
[X] src/game/colbldg.h
[X] src/game/colbomb.cpp
[X] src/game/colbomb.h
[X] src/game/colcalc.cpp
[X] src/game/colcalc.h
[X] src/game/colcalc_main.cpp
[X] src/game/coldraw.cpp
[X] src/game/coldraw.h
[X] src/game/colgcbt.cpp
[X] src/game/colgcbt.h
[X] src/game/colland.cpp
[X] src/game/colland.h
[X] src/game/colmove.cpp
[X] src/game/colmove.h
[X] src/game/colonize.cpp
[X] src/game/colonize.h
[X] src/game/colony.cpp
[X] src/game/colony.h
[X] src/game/colony_main.cpp
[X] src/game/colrefit.cpp
[X] src/game/colrefit.h
[X] src/game/colsum.cpp
[X] src/game/colsum.h
[X] src/game/colsysdi.cpp
[X] src/game/colsysdi.h
[X] src/game/colxport.cpp
[X] src/game/colxport.h
[X] src/game/combat.cpp
[X] src/game/combat.h
[X] src/game/combat1.cpp
[X] src/game/combat1.h
[X] src/game/combfind.cpp
[X] src/game/combfind.h
[X] src/game/combinit.cpp
[X] src/game/combinit.h
[X] src/game/compat.cpp
[X] src/game/compat.h
[X] src/game/consts.h
[X] src/game/contact.cpp
[X] src/game/contact.h
[X] src/game/council.cpp
[X] src/game/council.h
[X] src/game/deps.h
[X] src/game/desbox.cpp
[X] src/game/desbox.h
[X] src/game/design.cpp
[X] src/game/design.h
[X] src/game/design_main.cpp
[X] src/game/dip_scrn.cpp
[X] src/game/dip_scrn.h
[X] src/game/dip_scrn_main.cpp
[X] src/game/diplodef.cpp
[X] src/game/diplodef.h
[X] src/game/diplomac.cpp
[X] src/game/diplomac.h
[X] src/game/dos.cpp
[X] src/game/dos.h
[X] src/game/draw.cpp
[X] src/game/draw.h
[X] src/game/eric.cpp
[X] src/game/eric.h
[X] src/game/erichelp.cpp
[X] src/game/erichelp.h
[X] src/game/ericnet.cpp
[X] src/game/ericnet.h
[X] src/game/estrings.cpp
[X] src/game/estrings.h
[X] src/game/evanhelp.cpp
[X] src/game/evanhelp.h
[X] src/game/events.cpp
[X] src/game/events.h
[X] src/game/exitm.cpp
[X] src/game/exitm.h
[X] src/game/fade.cpp
[X] src/game/fade.h
[X] src/game/fakebuts.cpp
[X] src/game/fakebuts.h
[X] src/game/farload.cpp
[X] src/game/farload.h
[X] src/game/fields.cpp
[X] src/game/fields.h
[X] src/game/fieldsav.cpp
[X] src/game/fieldsav.h
[X] src/game/file_ani.cpp
[X] src/game/file_ani.h
[X] src/game/filedef.cpp
[X] src/game/filedef.h
[X] src/game/firepts.cpp
[X] src/game/firepts.h
[X] src/game/flcqueue.cpp
[X] src/game/flcqueue.h
[X] src/game/fleetpop.cpp
[X] src/game/fleetpop.h
[X] src/game/flt.cpp
[X] src/game/flt.h
[X] src/game/flt1.cpp
[X] src/game/flt1.h
[X] src/game/flt2.cpp
[X] src/game/flt2.h
[X] src/game/fmtpara.cpp
[X] src/game/fmtpara.h
[X] src/game/fontdraw.cpp
[X] src/game/fontdraw.h
[X] src/game/fonts.cpp
[X] src/game/fonts.h
[X] src/game/gendraw.cpp
[X] src/game/gendraw.h
[X] src/game/geo.cpp
[X] src/game/geo.h
[X] src/game/global.cpp
[X] src/game/global.h
[X] src/game/graphics.cpp
[X] src/game/graphics.h
[X] src/game/haccess.cpp
[X] src/game/haccess.h
[X] src/game/harold.cpp
[X] src/game/harold.h
[X] src/game/homegen.cpp
[X] src/game/homegen.h
[X] src/game/hotpop.cpp
[X] src/game/hotpop.h
[X] src/game/info.cpp
[X] src/game/info.h
[X] src/game/init.cpp
[X] src/game/init.h
[X] src/game/initgame.cpp
[X] src/game/initgame.h
[X] src/game/initship.cpp
[X] src/game/initship.h
[X] src/game/invasion.cpp
[X] src/game/invasion.h
[X] src/game/jim.cpp
[X] src/game/jim.h
[X] src/game/ken.cpp
[X] src/game/ken.h
[X] src/game/key.cpp
[X] src/game/key.h
[X] src/game/lbx_types.h
[X] src/game/line.cpp
[X] src/game/line.h
[X] src/game/list.cpp
[X] src/game/list.h
[X] src/game/loader.cpp
[X] src/game/loader.h
[X] src/game/loadsave.cpp
[X] src/game/loadsave.h
[X] src/game/lostship.cpp
[X] src/game/lostship.h
[X] src/game/mainmenu.cpp
[X] src/game/mainmenu.h
[X] src/game/mainpups.cpp
[X] src/game/mainpups.h
[X] src/game/mainscr.cpp
[X] src/game/mainscr.h
[X] src/game/mainscr2.cpp
[X] src/game/mainscr2.h
[X] src/game/mainscr_main.cpp
[X] src/game/maintain.cpp
[X] src/game/maintain.h
[X] src/game/mapgen.cpp
[X] src/game/mapgen.h
[X] src/game/misc.cpp
[X] src/game/misc.h
[X] src/game/mouse.cpp
[X] src/game/mouse.h
[X] src/game/movebox.cpp
[X] src/game/movebox.h
[X] src/game/mox.cpp
[X] src/game/mox.h
[X] src/game/mox2.cpp
[X] src/game/mox2.h
[X] src/game/msg.cpp
[X] src/game/msg.h
[X] src/game/multplay.cpp
[X] src/game/multplay.h
[X] src/game/namestar.cpp
[X] src/game/namestar.h
[X] src/game/netadapter.cpp
[X] src/game/netadapter.h
[X] src/game/netcode.cpp
[X] src/game/netcode.h
[X] src/game/netmox.cpp
[X] src/game/netmox.h
[X] src/game/netproto.cpp
[X] src/game/netproto.h
[X] src/game/netstart.cpp
[X] src/game/netstart.h
[X] src/game/netstrat.cpp
[X] src/game/netstrat.h
[X] src/game/netturn.cpp
[X] src/game/netturn.h
[X] src/game/newgame.cpp
[X] src/game/newgame.h
[X] src/game/newpup.cpp
[X] src/game/newpup.h
[X] src/game/newtech.cpp
[X] src/game/newtech.h
[X] src/game/nextturn.cpp
[X] src/game/nextturn.h
[X] src/game/npcdiplo.cpp
[X] src/game/npcdiplo.h
[X] src/game/ocpolpup.cpp
[X] src/game/ocpolpup.h
[X] src/game/officer.cpp
[X] src/game/officer.h
[X] src/game/oracle.cpp
[X] src/game/oracle.h
[X] src/game/orion2.h
[X] src/game/orion2_consts.h
[X] src/game/orion2_str.h
[X] src/game/palette.cpp
[X] src/game/palette.h
[X] src/game/palstore.cpp
[X] src/game/palstore.h
[X] src/game/paragrph.cpp
[X] src/game/paragrph.h
[X] src/game/platform.cpp
[X] src/game/platform.h
[X] src/game/plntsum.cpp
[X] src/game/plntsum.h
[X] src/game/pop.h
[X] src/game/racecust.cpp
[X] src/game/racecust.h
[X] src/game/raceopt.cpp
[X] src/game/raceopt.h
[X] src/game/racerprt.cpp
[X] src/game/racerprt.h
[X] src/game/racescrn.cpp
[X] src/game/racescrn.h
[X] src/game/racesel.cpp
[X] src/game/racesel.h
[X] src/game/random.cpp
[X] src/game/random.h
[X] src/game/reduce.cpp
[X] src/game/reduce.h
[X] src/game/regions.cpp
[X] src/game/regions.h
[X] src/game/remap.cpp
[X] src/game/remap.h
[X] src/game/replace.cpp
[X] src/game/replace.h
[X] src/game/report.cpp
[X] src/game/report.h
[X] src/game/russ.cpp
[X] src/game/russ.h
[X] src/game/science.cpp
[X] src/game/science.h
[X] src/game/score.cpp
[X] src/game/score.h
[X] src/game/scroll.cpp
[X] src/game/scroll.h
[X] src/game/settings.h
[X] src/game/settler.cpp
[X] src/game/settler.h
[X] src/game/setvesa.cpp
[X] src/game/setvesa.h
[X] src/game/shear.cpp
[X] src/game/shear.h
[X] src/game/shipmove.cpp
[X] src/game/shipmove.h
[X] src/game/ships.cpp
[X] src/game/ships.h
[X] src/game/shipstak.cpp
[X] src/game/shipstak.h
[X] src/game/sizes.h
[X] src/game/smack.cpp
[X] src/game/smack.h
[X] src/game/smckplay.cpp
[X] src/game/smckplay.h
[X] src/game/sound.cpp
[X] src/game/sound.h
[X] src/game/sound_types.h
[X] src/game/special.cpp
[X] src/game/special.h
[X] src/game/spy.cpp
[X] src/game/spy.h
[X] src/game/string.cpp
[X] src/game/string.h
[X] src/game/strings.cpp
[X] src/game/strings.h
[X] src/game/struct.cpp
[X] src/game/struct.h
[X] src/game/swap.cpp
[X] src/game/swap.h
[X] src/game/sys.cpp
[X] src/game/sys.h
[X] src/game/sysfont.cpp
[X] src/game/sysfont.h
[X] src/game/tech.cpp
[X] src/game/tech.h
[X] src/game/techdata.cpp
[X] src/game/techdata.h
[X] src/game/techinit.cpp
[X] src/game/techinit.h
[X] src/game/textbox.cpp
[X] src/game/textbox.h
[X] src/game/time.cpp
[X] src/game/time.h
[X] src/game/timer.cpp
[X] src/game/timer.h
[X] src/game/trade.cpp
[X] src/game/trade.h
[X] src/game/turnsum.cpp
[X] src/game/turnsum.h
[X] src/game/types.h
[X] src/game/vfs.cpp
[X] src/game/vfs.h
[X] src/game/vfs_detail.h
[X] src/game/vfs_file.cpp
[X] src/game/vfs_lbx.cpp
[X] src/game/vfs_path.cpp
[X] src/game/vfs_zip.cpp
[X] src/game/video.cpp
[X] src/game/video.h
[X] src/game/video_scan_bounds.h
[X] src/main.cpp
[X] src/network/router_cli.cpp
[X] src/network/router_cli.h
[X] src/network/router_client.cpp
[X] src/network/router_client.h
[X] src/network/router_core.cpp
[X] src/network/router_core.h
[X] src/network/router_protocol.cpp
[X] src/network/router_protocol.h
[X] src/network/router_service.cpp
[X] src/network/router_service.h
[X] src/network/router_session_browser.cpp
[X] src/network/router_session_browser.h
[X] src/pch.cpp
[X] src/pch.h
[X] src/strbuild.cpp
[X] src/strbuild.h
