# Historical patch bug TODOs

This file tracks bugs documented by historical Master of Orion II patches.

The goal here is to keep the bug reports, not to blindly copy original patch behavior. If a historical patch handled a problem by disabling or removing a feature, record the bug but do not treat that workaround as an accepted solution in this project.

## Patch 1.40

Sources:

- FAQ: https://lordbrazen.blogspot.com/2005/01/moo2v140-patch-faq.html
- Changelog: https://moo2mod.com/patch/CHANGELOG_140.TXT

### TODO

### Stability and rendering

- [x] Display redraw overwrite bug #1: a partial display redraw path could write outside the DOS VESA display window, causing random crashes and lockups. Fixed in source by hardening the shared scan-list pipeline instead of reproducing the DOS-side VESA workaround: `video::Add_Line_To_Scan_List_()` now rejects fully off-screen lines and normalizes clipped dword spans, `video::Add_Dot_To_Scan_List_()` now ignores off-screen dots instead of indexing the scan list out of bounds, and `video::Stencil_Out_Background_()` now validates and clamps each stencil-row span before copying from the back page into the off page. This preserves the intended partial-redraw behavior while preventing malformed scan-list entries from propagating into memory overwrites. Historical patch 1.40 handled the bug with a DOS binary helper around `video::Partial_Screen_Dump_Hi_Res_()` that clamped the final VESA-window copy; source now fixes both the scan-entry producers and the shared consumer directly in the restored rendering path.
- [x] Display redraw overwrite bug #2: another display redraw path can write outside the video frame buffers, causing save corruption, crashes, and other undefined behavior. Fixed in source by hardening the remaining rectangle-based scan-list producer in `setvesa::Add_Square_To_Scan_List_()`: it now accepts clipped updates that land on row or column `0`, rejects rectangles that become inverted after clipping, and normalizes the final dword span with the shared scan-bounds helper before touching `_refresh_scan_list` or `_refresh_stencil_list`. This complements the earlier source fixes in `video::Add_Line_To_Scan_List_()`, `video::Add_Dot_To_Scan_List_()`, and `video::Stencil_Out_Background_()` so malformed partial-redraw rectangles cannot propagate into out-of-bounds frame-buffer copies. Historical patch 1.40 handled the problem with DOS-side clamping helpers around the partial screen-dump copy path; source now fixes the remaining bad producer directly in the shared redraw pipeline instead of relying on a late binary copy guard.

### UI and race selection

- [x] Ship leader crash: viewing ship leaders with no ships can crash the game. Source inspection indicates the restored code already preserves and handles the no-fleet case by leaving `_small_ship_stack_ptr == -1` when no valid player ship stack exists, and by guarding the ship-leader screen's fleet-specific draw/palette/selection paths on that state in `OFFICER::Officers_Screen_()`, `OFFICER::Draw_Officer_Screen()`, `OFFICER::Fade_Into_Leaders_Screen_()`, and `MAINSCR::Cycle_Ship_Icons_()`. This avoids the original crash path where ship-leader UI logic could follow an invalid fleet stack when the player had no ships. Historical patch 1.40 handled the problem with a small DOS-side control-flow patch in the shared ship-leader path; source already fixes it more directly by preserving the explicit no-stack state and checking it before fleet-only operations, so no further source change was required.
- [x] ESC from Custom Race screen bug: after creating a custom race, pressing ESC can cause the Select Race screen to show `Last Race` instead of `Custom`. Fixed in source by restoring the real `Custom` button when canceling out of the custom-race editor. Historical patch 1.40 handled this with a fixup-pointer retarget to the correct button data.

### Economy and production

- [ ] Credit cheat: under some conditions a colony can both generate Trade Goods income and still keep production toward a build item in the same turn. Historical patch 1.40 handled the bug with a DOS-side next-turn call-order change. A previous source-level workaround cleared `production_spent` for Trade Goods, Housing, and no selected product, but that changed original save-state semantics and caused oracle mismatches, so it was removed pending a restoration-compatible implementation.
- [x] Repeat Build cancel exploit: a bought repeat-build item can be canceled to convert money into stored production. Fixed in source by clearing `bought_outright` whenever build-queue edits change `producing[0]` as the popup commits back to the colony, so purchased production cannot transfer to a different front item. Historical patch 1.40 handled this at the UI level by blocking cancellation or movement of the first queue item after an outright purchase; source now enforces the accounting invariant directly instead of relying only on popup restrictions.

### Colony and galaxy state

- [x] Home system quality issue: starting home systems can generate without any colonizable planet besides the homeworld. Source inspection shows the restored `HOMEGEN::Enforce_3_Planet_Minimum_()` already matches the historical 1.40 behavior: each home system is forced to contain at least three true `PLANET_TYPE_PLANET` bodies before homeworld selection, which leaves at least two additional planets after one is chosen as the homeworld. As in the original 1.40 patch, this guarantees additional real planets, not specifically additional habitable planets. No further source change was required for this bug after comparison against the original binaries.
- [ ] Arti planet bug: in LAN play, an artifacts world can lose its artifacts special when another player ends the turn first. Fixed in source by narrowing `COMBFIND::Reset_One_Shot_System_Special_()` so only temporary discovered special markers in the 50..89 range clear mirrored `planet_special` data, while the 90..99 range used for discovered Ancient Artifacts notifications only clears the transient `system_special` marker. Historical patch 1.40 handled this with a DOS-side range check injected into the shared one-shot-special reset path; source now expresses the actual rule directly instead of relying on a code-cave branch around the cleanup.
- [ ] Cloned colony ship / outpost ship bug: colony ships and outpost ships can sometimes remain after founding a colony or outpost. Fixed in source by clearing the transient auto-colonization assignment whenever a ship is retired in `SHIPS::Kill_Ship_()`, and by hardening `PLNTSUM::Colony_Ship_Auto_Colonization_()` and `PLNTSUM::Update_Ship_Etas_()` so consumed, invalid, or already-resolved colony/outpost assignments cannot be reused by summary UI state. Historical patch 1.40 handled this with DOS-side hooks in the ship-summary/report pipeline that patched both colony and outpost cleanup behavior; source now fixes the underlying invariant directly by retiring the assignment state with the ship and refusing to rebuild ETAs from stale references.

## Separate source-level fixes for original bugs not covered by patch 1.40

- [x] `HOMEGEN::Usable_Orbit_()` random-probe bug: the original code checked `_planet[_planet_count].planet_type` instead of the candidate orbit occupant, so source now uses the actual `p_idx` planet when deciding whether an occupied orbit is reusable.

### Detection, cloaking, and combat

- [ ] Stealth Field bug: ships with Stealth Field can still appear on long-range galaxy map scans. Fixed in source by correcting `SHIPSTAK::Remove_Non_Detected_Ships_()` to test for working stealth/cloak specials via `Ship_Has_Special_()` instead of testing `special_device_damage_flags` directly. The original logic only added the concealment penalty when the device was damaged, so healthy stealth systems stayed visible. Historical patch 1.40 handled this with injected DOS-side detection code; source now fixes the root condition directly in the shared visibility path.
- [ ] Cloak techs bug: ships with Cloaking Device or Phasing Cloak can still appear on long-range galaxy map scans. Fixed by the same source-level visibility correction as the Stealth Field bug, since all three concealment systems were affected by the same reversed condition. Historical patch 1.40 patched the relevant scan logic in binary; source now expresses the intended rule directly by checking whether the special is present and not damaged.
- [ ] Phasing Cloak bug: after 10 combat rounds, phasing cloak reversion does not apply correctly to all players. Fixed in source by moving the phasing-cloak timer countdown out of the per-side end-of-turn pass in `COMBAT1::Do_Combat_Turn()` and into shared round-end bookkeeping via `COMBAT1::Update_Phasing_Cloak_Timers_()` called from `COMBINIT::End_Of_Turn_Bookeeping_()`. This makes phasing-cloak expiration advance once per combat round for every active phased ship in both classic side-order and initiative combat, instead of depending on which side's cleanup block ran. Historical patch 1.40 handled the problem with a tiny DOS-side overwrite suppression near the shared phasing-cloak path; source now fixes the actual timing rule directly rather than preserving the binary workaround.
- [ ] Tractor Beam crash: active tractor beams can crash the game while ships are moving, especially when one of the ships is off-screen. Fixed in source by hardening the live tractor-link state and clipping every tractor-beam draw to the active combat window before calling `bitmap::Draw_Bitmap_Line_()`. `CMBTSPEC::Try_Get_Active_Tractor_Target_()` now clears stale `special_cooldown` links before the draw and movement code uses them, `CMBTSPEC::Draw_Continuing_Tractor_()`, `CMBTSPEC::Draw_Moving_Tractor_()`, and `CMBTSPEC::Tractor_Beam_()` now reject fully off-screen beam segments instead of feeding inverted bounds into the bitmap-line renderer, and `CMBTMOV1::Move_Ship_()` no longer stores moving tractor participants in a tiny fixed array that could overflow during heavily linked combats. This keeps working tractor beams visible when they should be, while preventing both stale-link dereferences and off-screen beam rasterization from turning into combat crashes. Historical patch 1.40 fixed the issue in the DOS binary; source now fixes the restored tractor state and rendering paths directly instead of reproducing a binary-only workaround.
- [ ] Black Hole Generator with initiative bug: when ship initiative is enabled, a Black Hole Generator can immobilize a target instead of destroying it. Fixed in source by moving `Increment_Bhg_()` into shared end-of-round combat bookkeeping, so black-hole victims advance once per round in both classic side-order and initiative combat. Historical patch 1.40 handled this with DOS-side control-flow hooks that restored the countdown from an alternate initiative path; source now reuses the existing countdown logic directly from the common round boundary instead of relying on mode-specific binary detours.

## Historical 1.40 workaround that should not be copied as-is

- [x] Last Race multiplayer bugs: pressing `Last Race` could crash LAN games, and multiplayer could also allow duplicate race portraits because the original flow reserved pseudo-slot `13` over the network while the loaded `lastrace.rac` data switched the player to a real base race `0..12`. Fixed in source by keeping `Last Race` as a local UI entry only and using the saved base race itself for multiplayer race reservation, release, and downstream banner/portrait identity. The source path also rejects invalid saved base-race ids defensively. Historical patch 1.40 handled this by removing the `Last Race` button from multiplayer entirely; source keeps the feature and fixes the actual race-identity mismatch instead.

### Notes

- The FAQ's short fixed-bug summary is older and smaller than the later 1.40 changelog. This list uses the later changelog as the fuller source and keeps the FAQ wording where it adds useful bug detail.

## Patch 1.50

Sources:

- TBD

### TODO

- No 1.50 items recorded yet.