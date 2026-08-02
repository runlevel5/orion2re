# Mass workflow

## Instructions

Migrate raw game-domain literals in the claimed file to the verified enum constants in `src/game/orion2_consts.h`. This is a naming-only refactor: preserve behavior, storage widths, signedness, serialized layouts, control flow, and table order.

### Approved domains

Replace a literal only when the surrounding field, parameter, array, switch, comparison, or bit operation proves that it belongs to one of these domains:

- `WEAPON_FIRING_ARC`: `NONE`, `FORWARD`, `FORWARD_EXTENDED`, `BACK_EXTENDED`, `REAR`, `ALL_SECTORS`, and `360`.
- `WEAPON_NATURAL_FLAG`: all natural weapon-property bits, including the two `AI_` metadata bits.
- `GAME_DIFFICULTY`, `GALAXY_AGE`, `GALAXY_SIZE`, and their count sentinels.
- `GOVERNMENT` and `STOCK_RACE`.
- `CREW_QUALITY` and `LEADER_TYPE`.
- `SPECIAL_CATEGORY`, `TECH_APPLICATION_TYPE`, and their count sentinels.
- `PLANET_GROUP`, `MAPGEN_SATELLITE`, and their count sentinels.
- `AI_SPECIAL_ITEM`, `AI_SHIP_DESIGN_THEME`, and `AI_SHIP_SPECIAL_THEME`.
- Newly added fixed-range sentinels: `ECON_COUNT`, `MINERAL_RESOURCE_COUNT`, `PLANET_CLIMATE_COUNT`, `PLANET_GRAVITY_COUNT`, `PLANET_SIZE_COUNT`, `SHIP_TYPE_COUNT`, `STAR_CLASS_COUNT`, and `SYSTEM_SPECIAL_COUNT`.
- `TECH_FIELD_INVALID` where `-1` specifically means no technology field.

Use the exact constant names and values already declared in `src/game/orion2_consts.h`; do not add, rename, renumber, or alias enum members during this workflow.

### Context rules

- Treat numeric equality as insufficient evidence. Verify the semantic domain from the accessed field, called function, declaration, table, or nearby control flow before replacing a literal.
- Preserve both 360-degree firing-arc encodings. Use `WEAPON_FIRING_ARC_ALL_SECTORS` for the tactical `0x0F` all-sector mask and `WEAPON_FIRING_ARC_360` for the design-screen `0x10` choice. Never collapse or interchange them.
- Use `WEAPON_NATURAL_FLAG_*` only for natural weapon-property masks such as `natural_mods`; do not substitute them for installed or available `WEAPON_MOD_FLAG_*` values merely because bit positions match.
- Use `STOCK_RACE_*` only for indexes into the 13-entry stock-race tables. Do not apply it to player indexes, ownership, packed population race values, Android/native values, or custom-race selection values.
- Use `MAPGEN_SATELLITE_*` only for map-generation satellite choices 0 through 4, with `MAPGEN_SATELLITE_COUNT` as the exclusive bound 5. Do not replace companion-star spectral encodings 5 through 9, `PLANET_TYPE` values, or generic orbit indexes.
- Use `TECH_APPLICATION_TYPE_COUNT` only as the exclusive bound 10 for technology application types 0 through 9; do not apply it to `ai_group` or other technology dimensions.
- Use `AI_SPECIAL_ITEM_*` only for `s_spcl::type` and values directly derived from that field; `SPECIAL` and `WEAPON` IDs remain separate domains.
- Use the AI ship theme constants only for `ship_design_theme` and `ship_special_theme` values, not AI design table classes or allocation percentages.
- Use count sentinels only as complete-domain bounds or extents. Do not replace unrelated quantities that happen to equal a count.
- Prefer the named zero member when zero is an actual domain value. Do not replace zero used for initialization, false, arithmetic, a null terminator, or an unrelated sentinel.
- Preserve casts required by the destination storage type. Do not change struct field types, function signatures, save/network read-write widths, or binary layouts.
- Existing correct enum usages may be left unchanged. Replace every proven raw code occurrence in the claimed file, including decimal and hexadecimal spellings. Do not edit comments solely to replace numeric text.
- Remove unnecessary, misleading and incorrect comments next to raw numeric values.

### Explicit exclusions

Do not enumize or guess any of the following:

- `s_leader_data::status`.
- Generic race, player, owner, packed-population, Android/native, or custom-race values.
- Building categories 0 through 7.
- Technology application `ai_group`, technology field `ai_group`, `unk_class`, or other unknown technology fields.
- AI design allocation percentages, disabled `-1` allocation sentinels, or AI design table classes.
- Special exclusion lists as masks.
- Orbit indexes, random-roll dimensions, array dimensions that are not complete approved domains, or other numeric quantities.

Do not edit JSON5 schemas, generated configuration code, tests, documentation, `todo.md`, `plan.md`, or `files.md`. Do not perform spelling cleanup, type redesign, formatting-only changes, comment rewrites, or unrelated magic-number replacement.

### Per-file verification

- Search the claimed file for all approved field names and raw decimal or hexadecimal values in their immediate semantic context.
- Review each edit against `src/game/orion2_consts.h` and confirm the numeric value is unchanged.
- Search the claimed file again for stale raw literals in proven approved domains; leave ambiguous cases unchanged and mention them in the workflow result.
- Run `git diff --check` after edits. Do not run the full build or full test suite for each claimed file; final integration verification will be performed after the queue is complete.

## Files

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
[X] src/game/map_scale.h
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
[X] src/game/msg_queue.cpp
[X] src/game/msg_queue.h
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
[X] src/game/savegame.cpp
[X] src/game/savegame.h
[X] src/game/science.cpp
[X] src/game/science.h
[X] src/game/score.cpp
[X] src/game/score.h
[X] src/game/scroll.cpp
[X] src/game/scroll.h
[X] src/game/serialization.cpp
[X] src/game/serialization.h
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
