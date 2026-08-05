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

- [ ] Credit cheat: under some conditions a colony can both generate Trade Goods income and still keep production toward a build item in the same turn. Historical patch 1.40 handled the bug with a DOS-side next-turn call-order change. A previous source-level workaround cleared `production_spent` for Trade Goods, Housing, and no selected product, but that changed original save-state semantics, so it was removed pending a correct implementation.
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

### Scope and source of truth

This catalog was generated from `C:\Projects\MOO2150\src\asm.pl`. The working copy inspected contains 344 active `topic(...)` declarations, from `asm.pl:29` through `asm.pl:19821`. The ordered `line:name` inventory has SHA-256 `0a744664c0abfc4f0bc601d4e4e8fc516d46f03895c37c8bea918927c625706f`.

`asm.pl` is authoritative even when a hook delegates its implementation to `src/c/patch.c`. Non-Lua C files were also inspected. Game-level C fixes are concentrated in `src/c/patch.c`; they normally extend an `asm.pl` topic and are cataloged under that parent rather than as duplicate patches. `src/c/lua`, `src/c/script.c`, Lua APIs, scripting support, generic runtime wrappers, build tooling, LBX substitution, and unrelated new features are out of backport scope.

The MOO2150 working tree had unrelated local modifications when inspected. Line numbers therefore identify that working copy, while repository HEAD was `e5d3857f1d30456b5e4b4abd734dc81e4ba1ff8f`.

### Status and disposition

Each topic appears exactly once in one of the four inventories below:

- **Backport audit** (140): likely original-game correctness, stability, desync, overflow, bad-state, or incorrect-display fixes. Determine whether current source already contains an equivalent root fix, then mark complete.
- **Mixed topic** (19): combines fixes with optional behavior, map generation, compatibility, or broader redesign. Audit individual patch blocks rather than importing the whole topic.
- **Behavior review** (79): balance, rules, AI policy, table corrections, or ambiguous intended behavior. Do not backport automatically.
- **Follow up** (106): infrastructure, configuration/modding, scripting, UI features, format compatibility specific to 1.50, or other new functionality. These are not immediate backport candidates, but remain cataloged for later review and to prove completeness.

Checkboxes record migration review, not whether MOO2150 implemented the topic.

### Backport audit

- [x] `asm.pl:307` - `artemis strategic combat fix`: fixed in `COMBAT::Strategic_Combat_()` by keeping the loaded attacker-slot count separate from the live post-Artemis survivor count when deriving defenders; this replaces the 1.50 save/restore and ghost-balance assembly workaround with direct source-level accounting.
- [x] `asm.pl:397` - `plasma torpedo`: fixed in `CMBTMIS::Move_Missile_()` and `CMBTMIS::Resolve_Missile_()` by preserving the signed initiative-launch counter and including completed flight turns in plasma torpedo dissipation.
- [ ] `asm.pl:475` - `outpost ship prohibits colonizing bug`
- [ ] `asm.pl:511` - `artifacts click bonus`
- [ ] `asm.pl:520` - `ship cost exchange aka feudal bug`
- [ ] `asm.pl:539` - `planet +10 offense bonus bug`
- [ ] `asm.pl:837` - `fighters point blank bug`
- [ ] `asm.pl:846` - `combat repair`
- [ ] `asm.pl:913` - `combat raid`
- [ ] `asm.pl:978` - `fighters use pd`
- [ ] `asm.pl:1023` - `EM desync + calc correction`
- [ ] `asm.pl:1137` - `raid and capture`
- [ ] `asm.pl:1263` - `mutation unclickable picks`
- [ ] `asm.pl:1281` - `tolerant races maintenance bug`
- [ ] `asm.pl:1290` - `telepath skill level is 1 too low bug`
- [ ] `asm.pl:1306` - `ordinary assasin skill works`
- [ ] `asm.pl:1315` - `navigator skill display bug`
- [ ] `asm.pl:1368` - `AI fighters size`
- [ ] `asm.pl:1377` - `filter boarding for stasis, bhg`
- [ ] `asm.pl:1492` - `tech field desync`
- [ ] `asm.pl:1548` - `clip fighter damage`
- [ ] `asm.pl:1577` - `bombardment hits fix`
- [ ] `asm.pl:1654` - `stealth`
- [ ] `asm.pl:1964` - `mapedge retreat crash`
- [ ] `asm.pl:2160` - `diplomatic event`
- [ ] `asm.pl:2219` - `relax end of turn`
- [ ] `asm.pl:2261` - `pop limit bug`
- [ ] `asm.pl:2364` - `recalc prod`
- [ ] `asm.pl:2885` - `displacer, cloak missile`
- [ ] `asm.pl:3474` - `teleporter + tractor fix`
- [ ] `asm.pl:4421` - `AI missile targeting`
- [ ] `asm.pl:5530` - `speed calc speed driveless no div zero`
- [ ] `asm.pl:5877` - `other player's portal fix`
- [ ] `asm.pl:6090` - `BHG slot multiple shots`
- [ ] `asm.pl:6295` - `lightning field`
- [ ] `asm.pl:6433` - `woes of stasis`
- [ ] `asm.pl:6573` - `tractor cuts`
- [ ] `asm.pl:6787` - `specials repair`
- [ ] `asm.pl:6812` - `ICS cloak`
- [ ] `asm.pl:6866` - `starbase capture`
- [ ] `asm.pl:6994` - `retreat fix`
- [ ] `asm.pl:7084` - `flyer targeting`
- [ ] `asm.pl:7224` - `stolen prod`
- [ ] `asm.pl:7839` - `turn 1 research`
- [ ] `asm.pl:7864` - `unload ship damage storage`
- [ ] `asm.pl:7887` - `destroying combat cloaks`
- [ ] `asm.pl:8101` - `miniaturization`
- [ ] `asm.pl:8114` - `mark stack overflow`
- [ ] `asm.pl:8145` - `turn order ini overflow`
- [ ] `asm.pl:8169` - `damper planet`
- [ ] `asm.pl:8263` - `satellites plasma darkening`
- [ ] `asm.pl:8272` - `shuttle hijack`
- [ ] `asm.pl:8367` - `marines overload`
- [ ] `asm.pl:8662` - `cloak initialize`
- [ ] `asm.pl:8685` - `stasis cut`
- [ ] `asm.pl:8703` - `infantry and armor growth and death bug fixes`
- [ ] `asm.pl:8762` - `nullifier missiles`
- [ ] `asm.pl:8814` - `cybernetic advanced damage control`
- [ ] `asm.pl:8838` - `stasis area effects`
- [ ] `asm.pl:8897` - `absorber cleanup`
- [ ] `asm.pl:9091` - `monster bug`
- [ ] `asm.pl:9871` - `send leader to pool on capture`
- [ ] `asm.pl:9900` - `system popup freeze`
- [ ] `asm.pl:9995` - `stumble breakthrough research storage`
- [ ] `asm.pl:10195` - `curship visuals`
- [ ] `asm.pl:10260` - `fighter retargeting`
- [ ] `asm.pl:10315` - `pirate overflow`
- [ ] `asm.pl:10339` - `skan cleanup`
- [ ] `asm.pl:10633` - `beam sfx crash`
- [ ] `asm.pl:10675` - `getchar die`
- [ ] `asm.pl:11151` - `warlord woes`
- [ ] `asm.pl:11345` - `raid message`
- [ ] `asm.pl:11591` - `get all technology from surrender`
- [ ] `asm.pl:12180` - `fighters return`
- [ ] `asm.pl:12231` - `no computer hp`
- [ ] `asm.pl:12253` - `destroy building trigger engage`
- [ ] `asm.pl:12319` - `lander ships no best armor minus 1`
- [ ] `asm.pl:12342` - `event conditions`
- [ ] `asm.pl:12386` - `leaders desynch`
- [ ] `asm.pl:12470` - `fighters helmsman`
- [ ] `asm.pl:12492` - `crystal fleet`
- [ ] `asm.pl:13063` - `ensure officer_star_displayed is set`
- [ ] `asm.pl:13295` - `wrong missile bearing`
- [ ] `asm.pl:13430` - `no rifle no sound on invasion crash`
- [ ] `asm.pl:14094` - `wandering monster search range`
- [ ] `asm.pl:14103` - `AI selfdestruct`
- [ ] `asm.pl:14203` - `Loknar message`
- [ ] `asm.pl:14312` - `brown stars blank`
- [ ] `asm.pl:14445` - `comet event message`
- [ ] `asm.pl:14479` - `negative thrust`
- [ ] `asm.pl:14529` - `scrap value cap`
- [ ] `asm.pl:14561` - `AI fleet strength assessment`
- [ ] `asm.pl:15387` - `sound of crash`
- [ ] `asm.pl:15470` - `msgbox overflow`
- [ ] `asm.pl:15618` - `fast mod removal from anti-matter torpedo`
- [ ] `asm.pl:16269` - `hypertech bug`
- [ ] `asm.pl:16341` - `dead on graphs`
- [ ] `asm.pl:16430` - `usable orbit fix`
- [ ] `asm.pl:16439` - `1 turn display for 1 turn breakthrough`
- [ ] `asm.pl:16567` - `true omni`
- [ ] `asm.pl:16692` - `bombardment ammo fix`
- [ ] `asm.pl:16729` - `bombardment fair armor`
- [ ] `asm.pl:16814` - `wormhole to Orion fix`
- [ ] `asm.pl:16823` - `planet screen arti`
- [ ] `asm.pl:16869` - `empty bombardment`
- [ ] `asm.pl:17018` - `blockaded freeze`
- [ ] `asm.pl:17027` - `no assault shuttles shots`
- [ ] `asm.pl:17089` - `del_ships`
- [ ] `asm.pl:17144` - `no green system popup`
- [ ] `asm.pl:17265` - `planets screen: high-g race has -25% prod penalty on low-g planet`
- [ ] `asm.pl:17281` - `new pop not assigned to farming on housing colony`
- [ ] `asm.pl:17307` - `amoeba regeneration`
- [ ] `asm.pl:17417` - `new leader window2 shown on tutor only`
- [ ] `asm.pl:17828` - `AI zero field research`
- [ ] `asm.pl:17871` - `food replicator no negative production`
- [ ] `asm.pl:17981` - `artificial planet on outposts climate fix`
- [ ] `asm.pl:18002` - `commando bonus`
- [ ] `asm.pl:18437` - `net nested combat`
- [ ] `asm.pl:18649` - `no homeworld`
- [ ] `asm.pl:18679` - `outpost false button`
- [ ] `asm.pl:18757` - `nebula check`
- [ ] `asm.pl:18774` - `zero colony loss`
- [ ] `asm.pl:18883` - `unload troops button wrong message`
- [ ] `asm.pl:18925` - `telepatic capture`
- [ ] `asm.pl:18959` - `autodesign considers x3 shot`
- [ ] `asm.pl:18989` - `ship explosion leader says nooo`
- [ ] `asm.pl:19157` - `dimensional portal icon in system display box displays race name instead of ruler name`
- [ ] `asm.pl:19225` - `energy absorber 21 squares range bug`
- [ ] `asm.pl:19256` - `artemis desync`
- [ ] `asm.pl:19295` - `food shortage anims also shown when colony has 0 food production and 0 food import`
- [ ] `asm.pl:19304` - `scrap freighters wrong message`
- [ ] `asm.pl:19313` - `captured ship's missile's armor's owner`
- [ ] `asm.pl:19362` - `non-zero missile hp`
- [ ] `asm.pl:19409` - `assassin message race_name instead of ruler_name`
- [ ] `asm.pl:19594` - `large fleet net freeze`
- [ ] `asm.pl:19640` - `grain and barrels not shown during anomaly`
- [ ] `asm.pl:19659` - `Loknar cannot give tech_0`
- [ ] `asm.pl:19675` - `Leader #0, Ruola, shown in ship refit popup`
- [ ] `asm.pl:19684` - `galactic lore does not see planet defenses`
- [ ] `asm.pl:19702` - `destroying multi-phased shields`

### Mixed topics requiring block-level review

- [ ] `asm.pl:1158` - `mapgen`
- [ ] `asm.pl:1973` - `refit`
- [ ] `asm.pl:2387` - `town Zero`
- [ ] `asm.pl:3545` - `ICS + weapon breakages`
- [ ] `asm.pl:5757` - `antaran fixup for starbasi`
- [ ] `asm.pl:6957` - `free maintenance cash`
- [ ] `asm.pl:9343` - `unbuy and queue`
- [ ] `asm.pl:10360` - `show arti tech`
- [ ] `asm.pl:11479` - `scrap ships`
- [ ] `asm.pl:13245` - `fleet summary giveaways`
- [ ] `asm.pl:13578` - `overmap speed recalc`
- [ ] `asm.pl:14135` - `scrap stuff maintenance`
- [ ] `asm.pl:15487` - `keep autocolonization`
- [ ] `asm.pl:16349` - `design button behaviour`
- [ ] `asm.pl:16395` - `runaway homeworlds`
- [ ] `asm.pl:16409` - `home systems min 3 planets default`
- [ ] `asm.pl:16448` - `BH to real star planet generation`
- [ ] `asm.pl:16621` - `orphans fix`
- [ ] `asm.pl:17094` - `surrender maps`

### Behavior and balance review

- [ ] `asm.pl:339` - `missile initiative`
- [ ] `asm.pl:343` - `HEF/mount`
- [ ] `asm.pl:1230` - `torpedoes for satellites`
- [ ] `asm.pl:1244` - `tractor power table`
- [ ] `asm.pl:1272` - `pre-warp refit requires just nuclear fission instead of cold fusion`
- [ ] `asm.pl:1331` - `strategic tech tree corrections`
- [ ] `asm.pl:1425` - `crew experience affects capture`
- [ ] `asm.pl:1593` - `missile speed table`
- [ ] `asm.pl:1613` - `TD ftl bonus`
- [ ] `asm.pl:1748` - `enforce pop limit`
- [ ] `asm.pl:1780` - `command points fix OP&CS`
- [ ] `asm.pl:1828` - `heavy fighter scramble`
- [ ] `asm.pl:1901` - `detach 1.40 growth modifier from /nohousing switch`
- [ ] `asm.pl:1917` - `unglue CS&TR`
- [ ] `asm.pl:2579` - `planet non-enveloping`
- [ ] `asm.pl:2632` - `cannon zero`
- [ ] `asm.pl:2710` - `autodesign`
- [ ] `asm.pl:2948` - `sfx in flics`
- [ ] `asm.pl:3286` - `teleporters and boarding`
- [ ] `asm.pl:3935` - `stealth landers`
- [ ] `asm.pl:4444` - `android cost`
- [ ] `asm.pl:4766` - `moving farmer`
- [ ] `asm.pl:4813` - `absorber, damper`
- [ ] `asm.pl:4912` - `phasing cloak shwifty`
- [ ] `asm.pl:4939` - `thousands of millions colony ships`
- [ ] `asm.pl:5286` - `starbases`
- [ ] `asm.pl:5553` - `bombing range`
- [ ] `asm.pl:5616` - `planet visuals`
- [ ] `asm.pl:5634` - `strategic visuals`
- [ ] `asm.pl:5917` - `report cheaters`
- [ ] `asm.pl:5940` - `biospheres`
- [ ] `asm.pl:5968` - `correct food output`
- [ ] `asm.pl:6017` - `shield table`
- [ ] `asm.pl:6317` - `side end turn`
- [ ] `asm.pl:6632` - `scout lab bonus`
- [ ] `asm.pl:6645` - `command points`
- [ ] `asm.pl:6772` - `feudal cost reduction`
- [ ] `asm.pl:8411` - `specials repair percent`
- [ ] `asm.pl:8433` - `trade goods`
- [ ] `asm.pl:8468` - `rotate warning`
- [ ] `asm.pl:8604` - `postwarp techtable length`
- [ ] `asm.pl:9036` - `no engine explosion`
- [ ] `asm.pl:9321` - `spatial size class`
- [ ] `asm.pl:9486` - `range to xy`
- [ ] `asm.pl:9559` - `red the weapon`
- [ ] `asm.pl:9579` - `colony bases and outposts`
- [ ] `asm.pl:9614` - `fighter ordnance`
- [ ] `asm.pl:9685` - `shwifty range`
- [ ] `asm.pl:10496` - `mixed race penalty`
- [ ] `asm.pl:10523` - `battle pods cap`
- [ ] `asm.pl:10617` - `extended fuel range rounding`
- [ ] `asm.pl:11600` - `colony enviromental stuff stuff`
- [ ] `asm.pl:11617` - `required climate to build weather controller`
- [ ] `asm.pl:12616` - `warlord leaders +1 level`
- [ ] `asm.pl:13504` - `colony commando`
- [ ] `asm.pl:14321` - `strategic weapon damage`
- [ ] `asm.pl:14601` - `strategic weapon damage value corrections`
- [ ] `asm.pl:14701` - `telepath skill blocks mind control`
- [ ] `asm.pl:15672` - `2x penalty`
- [ ] `asm.pl:15717` - `expected damage`
- [ ] `asm.pl:16106` - `fair beams`
- [ ] `asm.pl:16844` - `subterranean bonuses`
- [ ] `asm.pl:16912` - `stealth mutual exclusive with cloaks`
- [ ] `asm.pl:16935` - `double range accuracy penalty inherent mod removals`
- [ ] `asm.pl:16951` - `starting relations symmetry meklar-bulrathi`
- [ ] `asm.pl:16960` - `leader hire tech`
- [ ] `asm.pl:17108` - `star base fleet beam offense`
- [ ] `asm.pl:17131` - `fair leader hire`
- [ ] `asm.pl:17157` - `aquatic treats tundra as swamp`
- [ ] `asm.pl:17339` - `transport ship extended fuel tanks`
- [ ] `asm.pl:17365` - `skip outpostization prompt`
- [ ] `asm.pl:17499` - `freighter cost`
- [ ] `asm.pl:17531` - `satellite special 6 and 14`
- [ ] `asm.pl:18379` - `ground defense armor multiplier`
- [ ] `asm.pl:18892` - `fighter garrison strength for bomber bays`
- [ ] `asm.pl:18901` - `xeno psychology spy bonus`
- [ ] `asm.pl:19202` - `mixed race penalty building`
- [ ] `asm.pl:19382` - `is_ship_out_of_ammo checks`
- [ ] `asm.pl:19812` - `F9 displays star distance if black hole is in between`

### Follow-up topics

- [ ] `asm.pl:29` - `meta`
- [ ] `asm.pl:158` - `library`
- [ ] `asm.pl:567` - `shortcuts`
- [ ] `asm.pl:774` - `case sensitive shortcuts`
- [ ] `asm.pl:954` - `missiles gluing option`
- [ ] `asm.pl:1090` - `no autodone`
- [ ] `asm.pl:1107` - `stored prod display`
- [ ] `asm.pl:1452` - `reports reordered`
- [ ] `asm.pl:1463` - `tech review items reordered`
- [ ] `asm.pl:1926` - `AI extra races`
- [ ] `asm.pl:2078` - `names in config`
- [ ] `asm.pl:2107` - `subst LBX`
- [ ] `asm.pl:2234` - `scroll timer`
- [ ] `asm.pl:2294` - `config net sync`
- [ ] `asm.pl:2344` - `command line config sync`
- [ ] `asm.pl:2964` - `default game settings`
- [ ] `asm.pl:2987` - `combat boni visuals`
- [ ] `asm.pl:4029` - `race picks`
- [ ] `asm.pl:4064` - `sixth slot`
- [ ] `asm.pl:4468` - `cluster`
- [ ] `asm.pl:4518` - `pollution conf`
- [ ] `asm.pl:4570` - `multi-phased shields conf`
- [ ] `asm.pl:4625` - `analyzer conf`
- [ ] `asm.pl:4660` - `ESD conf`
- [ ] `asm.pl:4957` - `stock races`
- [ ] `asm.pl:4983` - `race info visuals`
- [ ] `asm.pl:5079` - `sync pics and races`
- [ ] `asm.pl:5137` - `antaran delay`
- [ ] `asm.pl:5158` - `megafluxers`
- [ ] `asm.pl:6124` - `newgame screen extensions`
- [ ] `asm.pl:6339` - `race boni`
- [ ] `asm.pl:6479` - `strict combat sequencing`
- [ ] `asm.pl:6936` - `starting outpost`
- [ ] `asm.pl:7058` - `autosave`
- [ ] `asm.pl:7242` - `turn structure`
- [ ] `asm.pl:8025` - `save/load game hooks`
- [ ] `asm.pl:8080` - `no blockers dialog`
- [ ] `asm.pl:8635` - `Zfast messages`
- [ ] `asm.pl:8995` - `timeout for battle message`
- [ ] `asm.pl:9027` - `newgame help`
- [ ] `asm.pl:9143` - `defence fire`
- [ ] `asm.pl:9286` - `final fire`
- [ ] `asm.pl:9840` - `grayboxing`
- [ ] `asm.pl:9923` - `i18n`
- [ ] `asm.pl:10282` - `known techs modding`
- [ ] `asm.pl:10727` - `bc bonuses`
- [ ] `asm.pl:10781` - `food buildings`
- [ ] `asm.pl:11112` - `research buildings`
- [ ] `asm.pl:11287` - `production buildings`
- [ ] `asm.pl:11654` - `events`
- [ ] `asm.pl:12122` - `5+ leaders`
- [ ] `asm.pl:12673` - `placement`
- [ ] `asm.pl:12838` - `current fleet summary`
- [ ] `asm.pl:12993` - `fleet screen`
- [ ] `asm.pl:13037` - `officer screen scrollbars`
- [ ] `asm.pl:13083` - `fleet scrollbars`
- [ ] `asm.pl:13439` - `alternate diplomacy`
- [ ] `asm.pl:13541` - `AI ground combat bonus visuals`
- [ ] `asm.pl:14577` - `MOX.SET >> 150.SET`
- [ ] `asm.pl:14592` - `lastrace.rac >> lastr150.rac`
- [ ] `asm.pl:14771` - `custom defenses missiles mods`
- [ ] `asm.pl:14822` - `custom defenses heavy mods`
- [ ] `asm.pl:14850` - `custom defenses normal mods`
- [ ] `asm.pl:14897` - `custom defenses pd mods`
- [ ] `asm.pl:14944` - `planet defenses mods`
- [ ] `asm.pl:15014` - `satellite space allocation`
- [ ] `asm.pl:15135` - `proper defenses arcs`
- [ ] `asm.pl:15224` - `mods aware pd selection`
- [ ] `asm.pl:15243` - `satellite missile distrubution`
- [ ] `asm.pl:15334` - `satellite missile distribution limit`
- [ ] `asm.pl:15364` - `satellite +1/3 space adjustment for 360 arcs and missile mods`
- [ ] `asm.pl:15428` - `satellite heavy beams`
- [ ] `asm.pl:15523` - `autodesign beam modifications`
- [ ] `asm.pl:15627` - `tunable animation speed`
- [ ] `asm.pl:16093` - `tunable antaran star fortress space`
- [ ] `asm.pl:16129` - `AI Hypertech`
- [ ] `asm.pl:16173` - `rebalanced HA techfield chances`
- [ ] `asm.pl:16189` - `convert toxic to barren`
- [ ] `asm.pl:16217` - `human techtrade`
- [ ] `asm.pl:16879` - `antaran extra state`
- [ ] `asm.pl:17049` - `multibuild`
- [ ] `asm.pl:17389` - `show enemy moves`
- [ ] `asm.pl:17433` - `planets screen scrollbar`
- [ ] `asm.pl:17457` - `build screen cost and time info`
- [ ] `asm.pl:17625` - `draw race_extra info`
- [ ] `asm.pl:17749` - `planet extra specials`
- [ ] `asm.pl:17840` - `independent space scanner`
- [ ] `asm.pl:17912` - `food replicator pp and bc cost`
- [ ] `asm.pl:18087` - `antaran ship designs`
- [ ] `asm.pl:18178` - `system monster weapons`
- [ ] `asm.pl:18326` - `build screen colors and ordering`
- [ ] `asm.pl:18480` - `leader xp`
- [ ] `asm.pl:18504` - `freight limit`
- [ ] `asm.pl:18586` - `orders hooks`
- [ ] `asm.pl:18688` - `pregame script support`
- [ ] `asm.pl:18703` - `cleaner tech init`
- [ ] `asm.pl:18732` - `main screen delayed messages`
- [ ] `asm.pl:18796` - `noreport`
- [ ] `asm.pl:19064` - `display hyperspace beast presence`
- [ ] `asm.pl:19166` - `max one dimensional portal per system using existing artemis check`
- [ ] `asm.pl:19234` - `reimplement builtin random`
- [ ] `asm.pl:19469` - `type research value printed in Empire Info Window`
- [ ] `asm.pl:19540` - `freighted population value printed in Empire Info Window`
- [ ] `asm.pl:19770` - `population history includes freighted pop`
- [ ] `asm.pl:19791` - `all wait hotkey`
- [ ] `asm.pl:19821` - `Doom Star name`

### Non-Lua C-level follow-up

The following correctness changes were found in `src/c/patch.c` or its history. Most are revisions of an existing `asm.pl` topic, so the parent topic remains the migration unit.

| Parent catalog topic | C implementation or later correction to inspect |
|---|---|
| `strict combat sequencing` | `next_ship_strict()`, `iterate_tships()`, and `SHIP_DONE_FLAG`: fixes duplicate AI/AUTO boarding caused by confusing the done and boarding flags. Also inspect the later last-ship shuttle-capture multiplayer stall correction. |
| `ICS + weapon breakages` / `tractor cuts` | `moo_tship.tractor_target`, `proc_break_weapon()`, and `proc_recalc_ship_id()`: use an unsigned 0..254 target with 255 as invalid, fixing tractor crashes in battles with ship ids above 127. |
| `expected damage` | `need_to_shoot_at_missile()`: later fixes select the requested missile instead of element zero, use the launched missile's overloaded modifier, and make predicted damage match actual anti-missile damage. |
| `mapgen` | `n_food_planets_in_system()` and `remove_monsters_from_food_worlds()`: multiple repairs to ensure at least one food world, including counter reset and monster removal. |
| `orphans fix` / `del_ships` / `zero colony loss` | `planet_is_orphan_ptr()`, `del_planets()`, and `decide_zero_loss()`: clear deleted records and unlink destroyed colonies from their planets. |
| `pop limit bug` / `freight limit` | `proc_n_room_for_pop_wf()`, `realloc_freights_maybe()`, and freight load fallback: enforce the actual freight cap and correctly include inbound population. |
| `nebula check` | `point_is_in_nebula_n()`: corrected image-bound and point-in-nebula checks. |
| `overmap speed recalc` | Jump-gate redirection helpers: clear stuck jump-gate state and avoid double redirect or incorrectly reduced travel speed. |
| `bombing range` | `proc_can_bomb()`: compare untruncated point range against the weapon range instead of dividing before comparison. |
| `fleet summary giveaways` | `proc_stack_ship_per_type_counts()` and fleet-screen hook revisions: close three indirect detection leaks involving undetected ships. |
| `large fleet net freeze` | Network combat count/packet revisions: fix tactical network freeze above 99 ships and retain full-width ship ids/counts. |
| `cluster` | `size_and_civ_to_lb()`, `size_and_civ_from_lb()`, and `wrap_init_new_game()`: fixes the 1.50 cluster mode generating 54 rather than 71 stars. This is a patch-feature regression and remains a follow-up unless equivalent modes are adopted. |
| `case sensitive shortcuts` | `proc_lowercase_shortcut_maybe()`: accepts a null diplomacy shortcut pointer instead of dereferencing it. This is a real crash fix embedded in an otherwise follow-up UI topic. |
| `names in config` | `proc_init_tech()`: fixes broken custom technology names and network/config reload synchronization. Follow up if configurable names are adopted. |
| `save/load game hooks` | `load_old_header()` and `proc_load_game_hook()`: repairs corrupt historical 1.50/VDC save variants and old freight storage. This is 1.50 compatibility, not an original-game fix. |
| `config net sync` / `orders hooks` | `unpack_gdd()`, `unpack_net_pdus()`, and PDU validation: handle `max_pop_freighted == 0`, unknown PDUs, and stream signatures. Follow up if the 1.50 protocol is adopted. |

Potential standalone C correctness item with no dedicated `topic(...)` declaration:

- [ ] `src/c/patch.c:wrap_init_new_game()` - random-player starts must inspect the player's stored type directly; calling the normal human-player predicate during incomplete initialization caused a crash. Backport only if the corresponding current new-game path can make the same premature query.

### Completeness check

- Active `topic(...)` declarations in inspected `asm.pl`: **344**.
- Cataloged as backport audit: **140**.
- Cataloged as mixed: **19**.
- Cataloged as behavior review: **79**.
- Cataloged for follow-up: **106**.
- Total: **344**.
- No commented-out `topic(...)` declarations were found; all 344 declarations are active Perl statements.
- `src/c/patch.c` is an implementation source, not a second definition registry. Its Lua-gated sections and `src/c/script.c` were left for follow-up as requested.