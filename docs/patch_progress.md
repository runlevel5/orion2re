# Patch 1.40 bug TODOs

This file tracks bugs documented by the historical Master of Orion II 1.40 patch.

The goal here is to keep the bug reports, not to blindly copy the original patch behavior. If 1.40 handled a problem by disabling or removing a feature, record the bug but do not treat that workaround as an accepted solution in this project.

Sources:

- FAQ: https://lordbrazen.blogspot.com/2005/01/moo2v140-patch-faq.html
- Changelog: https://moo2mod.com/patch/CHANGELOG_140.TXT

## TODO

### Stability and rendering

- [ ] Display redraw overwrite bug #1: a display redraw path can write outside the VESA display memory window, causing random crashes and lockups.
- [ ] Display redraw overwrite bug #2: another display redraw path can write outside the video frame buffers, causing save corruption, crashes, and other undefined behavior.

### UI and race selection

- [ ] Ship leader crash: viewing ship leaders with no ships can crash the game.
- [x] ESC from Custom Race screen bug: after creating a custom race, pressing ESC can cause the Select Race screen to show `Last Race` instead of `Custom`. Fixed in source by restoring the real `Custom` button when canceling out of the custom-race editor. Historical patch 1.40 handled this with a fixup-pointer retarget to the correct button data.

### Economy and production

- [ ] Credit cheat: under some conditions a colony can both generate Trade Goods income and still keep production toward a build item in the same turn.
- [x] Repeat Build cancel exploit: a bought repeat-build item can be canceled to convert money into stored production. Fixed in source by clearing `bought_outright` whenever build-queue edits change `producing[0]` as the popup commits back to the colony, so purchased production cannot transfer to a different front item. Historical patch 1.40 handled this at the UI level by blocking cancellation or movement of the first queue item after an outright purchase; source now enforces the accounting invariant directly instead of relying only on popup restrictions.

### Colony and galaxy state

- [x] Home system quality issue: starting home systems can generate without any colonizable planet besides the homeworld. Source inspection shows the restored `HOMEGEN::Enforce_3_Planet_Minimum_()` already matches the historical 1.40 behavior: each home system is forced to contain at least three true `PLANET_TYPE_PLANET` bodies before homeworld selection, which leaves at least two additional planets after one is chosen as the homeworld. As in the original 1.40 patch, this guarantees additional real planets, not specifically additional habitable planets. No further source change was required for this bug after comparison against the original binaries.
- [ ] Arti planet bug: in LAN play, an artifacts world can lose its artifacts special when another player ends the turn first.
- [ ] Cloned colony ship / outpost ship bug: colony ships and outpost ships can sometimes remain after founding a colony or outpost.

## Separate source-level fixes for original bugs not covered by patch 1.40

- [x] `HOMEGEN::Usable_Orbit_()` random-probe bug: the original code checked `_planet[_planet_count].planet_type` instead of the candidate orbit occupant, so source now uses the actual `p_idx` planet when deciding whether an occupied orbit is reusable.

### Detection, cloaking, and combat

- [ ] Stealth Field bug: ships with Stealth Field can still appear on long-range galaxy map scans.
- [ ] Cloak techs bug: ships with Cloaking Device or Phasing Cloak can still appear on long-range galaxy map scans.
- [ ] Phasing Cloak bug: after 10 combat rounds, phasing cloak reversion does not apply correctly to all players.
- [ ] Tractor Beam crash: active tractor beams can crash the game while ships are moving, especially when one of the ships is off-screen.
- [ ] Black Hole Generator with initiative bug: when ship initiative is enabled, a Black Hole Generator can immobilize a target instead of destroying it.

### Networking

- [ ] LAN packet send bug: the original game can lock up or crash during native Windows XP LAN play because of packet-send handling in `RKERNEL.COM`.

## Historical 1.40 workaround that should not be copied as-is

- [ ] Last Race multiplayer bugs: pressing `Last Race` can crash LAN games, and multiplayer can also allow duplicate race portraits. Patch 1.40 solved this by removing the `Last Race` button, but feature removal is not an accepted fix here.
- [ ] Modem Comm Info / TEN regression: patch 1.40 disabled the modem `Comm Info` button and the `TEN` button to make room for patch code. If this project needs to preserve those features, the underlying space or architecture problem should be solved instead of copying the disablement.

## Notes

- The FAQ's short fixed-bug summary is older and smaller than the later 1.40 changelog. This list uses the later changelog as the fuller source and keeps the FAQ wording where it adds useful bug detail.