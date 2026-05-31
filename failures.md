
src/game/dip_scrn.h

Failures:
- `_continuous_string` @ `0x00572C18`: missing from this recovered header. The pre-variables header exposed it under `DIP_SCRN`, but current ownership in Ghidra is `fields::_continuous_string`, so the DIP_SCRN recovery for this header is not preserved and the old stub would also alias a different namespace owner.
- `_propose_treaty_help_entry` @ `0x005522D0`: wrong array length. Old stub points at contiguous storage whose bytes are `576,577,578,579,580,613,0`; Ghidra defines `short[7]`. Current recovery is `int16_t[6]`, truncating owned storage.
- `_break_treaty_help_entry` @ `0x005522E0`: wrong array length. Ghidra defines `short[7]` at the stub address; current recovery is `int16_t[6]`.
- `_demand_help_entry` @ `0x005522F0`: wrong array length. Binary bytes cover `588,589,590,591,592,593,594,595,596,613,0`; current recovery is `int16_t[10]`, but the owned span is 11 shorts.
- `_offer_help_entry` @ `0x00552308`: wrong array length. Binary bytes cover `598,599,600,601,602,613,-1`; current recovery is `int16_t[6]`, but the owned span is 7 shorts.
- `_main_repulsive_diplomacy_help_entry` @ `0x005522B0`: wrong array length. Ghidra defines `short[7]`; current recovery is `int16_t[4]`.
- `_main_net_repulsive_diplomacy_help_entry` @ `0x005522C0`: wrong array length. Ghidra defines `short[7]`; current recovery is `int16_t[5]`.
- `_main_repulsive_diplomacy_list` @ `0x005A8230`: wrong array length / alias-risk against larger owner. Old stub was recovered as 5 pointers, but Ghidra’s defined owner at that address is `char *[7]` spanning `0x005A8230–0x005A824B`. Current recovery truncates the owned storage.
- `_diplomacy_system_planet_list` @ `0x005A85A0`: wrong array length and storage shape. Old stub was a raw pointer to inline storage, and Ghidra shows only inline data starting at that address, not a pointer variable. Current recovery as `uint8_t[5]` conflicts with code usage too: `Diplomacy_System_Scan_` can fill up to `MOX::_NUM_STARS` planets, and max `_NUM_STARS` in project code is 71. A 5-byte array is not credible for the binary storage.
- `_diplomacy_system_fields` @ `0x005A850E`: wrong array length. Old stub comment and Ghidra defined data both show `short[72]` (144 bytes). Current recovery is `int16_t[73]`, overrunning the proven owned span.

Result: validation failed.
---------------------------------------
src/game/diplomac.h

- `_unprovoked_message_mapping` @ `0x00453734`: current recovery is wrong storage kind and alias candidate. The old stub was a `uint8_t*`, but the current project restores it as `uint8_t[20]` in `diplomac.cpp`. In Ghidra, `0x00453734` is inside .text, labeled `switchdataD_00453734` under namespace `switchD_00453182`, not owned by `DIPLOMAC`. Disassembly at `0x0045317c` shows direct indexed reads from inline code storage (`MOV CL, byte ptr [EAX + 0x453734]`), proving this is embedded read-only switch/table data, not a DIPLOMAC-owned variable. By the alias rules, restoring it as a `diplomac::_unprovoked_message_mapping` object fails ownership/namespace validation.

- `_personality_break_peace_treaty_chance` @ `0x00556CE0`: alias/namespace mismatch. Current header restores it as `diplomac::_personality_break_peace_treaty_chance[8]`, but in Ghidra the symbol at that exact address is `_personality_break_peace_treaty_chance` in namespace `Global`, not `DIPLOMAC`. Since the old stub address resolves to a different owning namespace than the recovered variable, this must fail.

- `_personality_sneak_attack_modifiers` @ `0x00556CF0`: alias/namespace mismatch. Current header restores it as `diplomac::_personality_sneak_attack_modifiers[8]`, but in Ghidra the symbol at that exact address is `_personality_sneak_attack_modifiers` in namespace `Global`, not `DIPLOMAC`. Per the alias rules, this is a failure.

fail
---------------------------------------
src/game/eric.h

- `s_2_00551108` @ `0x00551108`: alias failure. In the old header this was a raw pointer stub (`inline char* const s_2_00551108 = reinterpret_cast<char*>(0x00551108);`), but the current recovered variable is `ERIC::s_2_00551108` as a new owned array `const char[3]`. Live Ghidra shows the storage owner at `0x00551108` is `Global::s_2_00551108`, a 3-byte string object, not an `ERIC` namespace variable. Recovering it as `ERIC::s_2_00551108` assigns the storage to the wrong namespace and creates a conflicting owner.
---------------------------------------
src/game/ericnet.h

- ERICNET::_start_civ_level, 0x0054F000 — current recovery declares/defines `int8_t[3] = {3, 5, 9}`, but Ghidra shows the next symbol at `0x0054F008`, leaving 8 bytes of contiguous storage from the stub address, with bytes `[3, 5, 9, 0, 0, 0, 0, 0]`. I could confirm only the first 3 indexed values are used by current code (`MOV AL,byte ptr [ECX + 0x54f000]`), but I could not prove the true owning object extent is exactly 3 bytes rather than a larger object with trailing initialized/owned zero bytes. Under the required no-uncertainty rule, this cannot be accepted confidently.

- ERICNET::_buildlist, 0x00551198 — current recovery declares/defines `int16_t[43]`, but Ghidra defines the storage at that address as `short[44]` and the full 88-byte span before the next symbol contains `u16 [41, 8, 40, 21, 22, 15, 7, 20, 37, 4, 31, 33, 34, 39, 2, 12, 13, 32, 35, 10, 43, 16, 28, 25, 18, 19, 24, 26, 47, 27, 6, 5, 23, 36, 29, 30, 46, 14, 45, 3, 42, 38, 0, 0]`. The recovered definition accounts for only 43 elements, leaving one additional zero `int16_t` in the binary extent. That is a shape/length mismatch, so validation fails.

---------------------------------------
src/game/estrings.h

- `ESTRINGS::s_0_0055110c`, old `0x0055110C`: recovered as `const char[4]`, but live Ghidra shows a 3-byte string object `"0"` at `0x0055110C` and the next distinct owner begins at `0x00551110`. The current declaration/definition overstate the object extent and overlap neighboring storage.
- `ESTRINGS::s_1_00551110`, old `0x00551110`: recovered as `const char[4]`, but live Ghidra shows a 3-byte string object `"1"` at `0x00551110` and the next distinct owner begins at `0x00551114`. The current declaration/definition overstate the object extent and overlap neighboring storage.
- `ESTRINGS::_string_data`, old `0x00566580`: recovered as `char[21000]`, but that claimed span ends at `0x0056B788` and contains a different live Ghidra owner `draw::_glass_lookup_table` at `0x0056A998`, which is actively indexed by multiple glassed-sprite routines (`... + 0x56a998`). This is an alias/overlap failure, so the restored `_string_data` extent cannot be accepted as claimed.

Fail outcome selected.
---------------------------------------
src/game/events.h

- s__00551100, 0x00551100: alias/owner mismatch. In Ghidra the address is owned by global symbol `s__00551100` in the Global namespace, and the project already defines the same storage in `src/game/strings.h` / `src/game/strings.cpp` as `strings::s__00551100[1]`. Recovering it again as `EVENTS::s__00551100[]` gives a second project-side owner for the same non-local symbol.
- s_0_0055110c, 0x0055110C: alias/owner mismatch. Ghidra owns this as global `s_0_0055110c`; the project already defines it in `strings::s_0_0055110c[3]`. Duplicating it in `EVENTS` is a conflicting recovered owner.
- s_s_005510f4, 0x005510F4: alias/owner mismatch. Ghidra owner is global `s_s_005510f4`, not `EVENTS`.
- s_jimtext2_lbx_00552340, 0x00552340: alias/owner mismatch. Ghidra owner is global `s_jimtext2.lbx_00552340`, not `EVENTS`.
- s_EVENTS_LBX_0055753c, 0x0055753C: alias/owner mismatch. Ghidra owner is global `s_EVENTS.LBX_0055753c`, not `EVENTS`.
- s_X_d__d___s_00557548, 0x00557548: alias/owner mismatch. Ghidra owner is global `s_X%d.%d._%s_00557548`, not `EVENTS`.
- s_EVENTMSP_LBX_00557554, 0x00557554: alias/owner mismatch. Ghidra owner is global `s_EVENTMSP.LBX_00557554`, not `EVENTS`.
- s_EVENTMSI_LBX_00557564, 0x00557564: alias/owner mismatch. Ghidra owner is global `s_EVENTMSI.LBX_00557564`, not `EVENTS`.
- s_EVENTMSS_LBX_00557574, 0x00557574: alias/owner mismatch. Ghidra owner is global `s_EVENTMSS.LBX_00557574`, not `EVENTS`.
- s_EVENTMSF_LBX_00557584, 0x00557584: alias/owner mismatch. Ghidra owner is global `s_EVENTMSF.LBX_00557584`, not `EVENTS`.
- s_EVENTMSG_LBX_00557594, 0x00557594: alias/owner mismatch. Ghidra owner is global `s_EVENTMSG.LBX_00557594`, not `EVENTS`.
- s_EVENTMSE_LBX_005575a4, 0x005575A4: alias/owner mismatch. Ghidra owner is global `s_EVENTMSE.LBX_005575a4`, not `EVENTS`.

---------------------------------------
src/game/exitm.h

- Missing recovered variable in claimed header: `g_is_exiting_with_error` / old raw address `0x00578E68`. The pre-variables header contains `inline uint8_t& DAT_00578e68 = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00578E68 });`, but the current `src/game/exitm.h` contains no variable declaration at all.
- Wrong namespace/owner for old raw address `0x00578E68`: Ghidra resolves the symbol at that address as `platform::_need_quit`, not an `exitm` variable. The symbol namespace owner is `platform`, so any recovery of this stub as an `exitm` variable would be an alias failure under the stated rules.
- Concrete Ghidra evidence for `0x00578E68`: symbol `platform::_need_quit`; byte-sized writes only:
  - `platform::Create_Direct_Draw`: `MOV byte ptr [0x00578e68], BL`
  - `exitm::Exit_With_Message`: `MOV byte ptr [0x00578e68], 0x1`
  - `fields::F11_Key_`: `MOV byte ptr [0x00578e68], 0x1`
  This confirms current project owner/type `platform::_need_quit` as `uint8_t`, with default byte value `0`.
- Therefore the claimed header `src/game/exitm.h` does not fully validate, because its historical variable stub was not correctly recoverable as an `exitm` variable from that header; the storage belongs to `platform::_need_quit`.
---------------------------------------
src/game/fields.h

- `s_You_exceeded_the_max_number_of_fields` @ `0x0055B080` — alias/namespace failure. Legacy stub points to Ghidra owner `Global::s_You_exceeded_the_max_number_of_f_0055b080`, but current recovery declares it as `fields::s_You_exceeded_the_max_number_of_fields`.
- `s_Maximum_number_of_button_fields_e_0055b0b0` @ `0x0055B0B0` — alias/namespace failure. Legacy stub points to Ghidra owner `Global::s_Maximum_number_of_button_fields_e_0055b0b0`, but current recovery declares it as `fields::s_Maximum_number_of_button_fields_e_0055b0b0`.
- `mouse_saved_center_x` @ `0x0057630C` — alias/owner failure. Legacy stub address resolves in Ghidra to `Global::DAT_0057630c`, not a `fields`-namespace owner; current recovery claims `fields::mouse_saved_center_x`.
- `s_Avail_Mem_0055b390` — recovered variable cannot be validated from the legacy stub set because `pre-variables:src/game/fields.h` contains no stub for this symbol; header includes an extra recovered variable not backed by an enumerated legacy absolute-address stub from this header.
- `src/game/fields.cpp:566` uses `s_You_exceeded_the_max_number_of_f_0055b080` — unresolved symbol/name mismatch in the current implementation, so the recovered definition set is internally inconsistent and cannot be validated confidently.

fail
---------------------------------------
src/game/filedef.h

Failures:
- FILEDEF::s_no_description_00556920 @ 0x00556920: current recovery is the wrong array length. `src/game/filedef.h` declares `extern const char ...[21]` and `src/game/filedef.cpp` defines `[21]`, but Ghidra defines a 24-byte string object at that address and the full bytes are `<< no description >>    `. The same address is also recovered elsewhere in the current tree as `s_no_description_00556920[24]` in `src/game/funcs.h` / `src/game/funcs.cpp`. This makes the FILEDEF recovery undersized and inconsistent with the proven binary extent.

---------------------------------------
src/game/fleetpop.h

Failures:
- `_fleet_box_width`, old raw address `0x005977C6`: current project still exposes this as `inline int16_t& _fleet_box_width = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005977C6});` in `src/game/funcs.h`. Ghidra shows `0x005977C6` is not an owning standalone variable; it is offset `+6` inside the larger owning object `MOX::_fleet_box` at `0x005977C0` (`s_fleet_box`, len 33), with an interior label `_fleet_box.width`. This violates both alias rules and the “still depends on a raw absolute address” fail condition.

Other old stubs checked:
- `_fleet_box` at `0x005977C0`: Ghidra owner is `MOX::_fleet_box` (`s_fleet_box`), zero-initialized in binary; current ownership exists as a normal project-side variable.
- `s_BUFFER0_LBX` at `0x00551124`: Ghidra bytes are "BUFFER0.LBX\0"; current recovered `const char s_BUFFER0_LBX[12]` matches.
- `_fleet_box_name_seg` at `0x005977D2`, `_fleet_box_bottom_seg` at `0x005977D6`, `_fleet_box_is_small_frame` at `0x005977DA`: all resolve in Ghidra to interior labels inside owning `MOX::_fleet_box`, not standalone owners; current code correctly uses struct members instead of separate recovered globals.
- `_n_added_buttons` at `0x00564734`: Ghidra primary symbol is `FLEETPOP::_n_added_buttons`; current `extern` + definition as `uint8_t` matches zero-initialized storage.

Outcome: fail
---------------------------------------
src/game/fmtpara.h

Failures:
- `_saved_superscript` @ `0x005650D8`: recovered as `s_fmtpara_style`, but Ghidra proves the binary object at this address is a 13-byte array (`uint8_t[13]`), matching the old stub, not a full `s_fmtpara_style` object. In this TU `orion2.h` is packed via `deps.h`, so `s_fmtpara_style` is 15 bytes, causing wrong type/size and wrong `memcpy(sizeof(_saved_superscript))` behavior.
- `_insert_item_kind` @ `0x005562F8`: current header declares a recovered variable, but this variable was not one of the old fmtpara header stubs. The old stub list instead included `_legal_variable_types` at `0x005562A8`, which is now missing from the recovered header, so the claimed header does not fully validate against the authoritative pre-variables stub set.
- `_tabs_3_plus_1` @ `0x005562B7`: old stub points at an interior address labeled by Ghidra as `_tabs[3]+1`, inside `_tabs` at `0x005562B0`. That is an alias candidate by rule. The current recovery changes it to `_insert_item_kind - 'A'`, but the authoritative old raw-address-backed variable for this header still aliases interior storage and therefore fails validation as a recovered variable.
- `_para_global_item_ptr` @ `0x005A7B18`: old stub is an interior address inside owning object `FMTPARA::_para_global` at `0x005A7B10`, so it is an alias candidate and fails validation.
- `_para_global_item_type` @ `0x005A7B58`: old stub is an interior address inside owning object `FMTPARA::_para_global` at `0x005A7B10`, so it is an alias candidate and fails validation.
---------------------------------------
src/game/fonts.h

Failures:
- `fonts::_font_name`, old raw `0x005787C0`: current recovery is `char _font_name[80]`, but the legacy stub was `char* const` and Ghidra only defines a 4-byte object at `0x005787C0` (`undefined4 00000000h`). I cannot validate an 80-byte owning object there with confidence; current array shape/extent is not proven from the binary listing.
- `fonts::_outline_color`, old raw `0x0055B148`: current recovery widened this to `uint16_t`, but the old stub was `uint8_t&`. Ghidra use sites are inconsistent/overlapping: writes use `MOV [0x0055b148],AX`, but reads in font code use `MOVSX ... byte ptr [0x0055b148]`, indicating byte semantics within a wider overlapped region. Current width is not validated confidently.
- `fonts::_alias_outline_color`, old raw `0x0055B14C`: current recovery widened this to `uint16_t`, but the old stub was `uint8_t&` and Ghidra defines only `undefined1` at that address. Reads use `MOV AL,[0x0055b14c]` while the setter writes `MOV word ptr [0x0055b14c],CX`, so the current 16-bit declaration is not validated confidently.
- `fonts::_font_header_current_height`, old raw `0x0057646C`: missing as a recovered project variable from this header lineage. Ghidra shows this address is an interior label `_font_header.current_height` inside the owning `fonts::_font_header` object, so the old stub was not kept as a standalone variable; current header no longer contains any corresponding recovered variable.
- `fonts::_current_font_style_2`, old raw `0x00578E1C`: missing as a recovered project variable from this header lineage. Ghidra shows no symbol or defined data at `0x00578E1C`, so I cannot validate that the omission is correct rather than an incomplete recovery.
- `fonts::_palette_block__unk`, old raw `0x00576447`: missing as a recovered project variable from this header lineage. Ghidra still has a symbol at that exact address with data refs (`MOV DL,byte ptr [EBX + 0x576447]`), so this previously recovered stub was dropped from the current header without a validated replacement owner.

---------------------------------------
src/game/funcs.h

Failures:
- `_anims`, old `0x00565D90`: still depends on a raw absolute address in the current header (`inline ... = reinterpret_cast<...>(0x00565D90)`), with no recovered project-side definition.
- `g_main_screen_unknown_flag`, old `0x00596196`: still raw-address-backed in the current header; no recovered definition.
- `g_map_unknown_word`, old `0x0059B5B4`: still raw-address-backed in the current header; no recovered definition.
- `g_ui_unknown_word`, old `0x0059C02A`: still raw-address-backed in the current header; no recovered definition.
- `g_map_unknown_flag`, old `0x0059701A`: still raw-address-backed in the current header; no recovered definition.
- `DAT_0056178c`, old `0x0056178C`: still raw-address-backed in the current header; no recovered definition.
- `PTR_DAT_00561580`, old `0x00561580`: still raw-address-backed in the current header; no recovered definition.
- `_mouse_XY_star_idx`, old `0x0059B5C4`: still raw-address-backed in the current header; no recovered definition.
- `DAT_00590000`, old `0x00590000`: still raw-address-backed in the current header; no recovered definition.
- `s__GEGGGGG_s___s__005569b8`, old `0x005569B8`: still raw-address-backed in the current header; no recovered definition.
- `_fleet_box_width`, old `0x005977C6`: still raw-address-backed in the current header; no recovered definition.

Missing recovered variables that existed as old raw-address stubs but are absent from the current header:
- `s_Avail_Mem_0055b390`, old `0x0055B390`: missing from current header.
- `s_Outpost_Ship_00558f48`, old `0x00558F48`: missing from current header.
- `s_inbox_lbx_00551370`, old `0x00551370`: missing from current header.
- `s_cmbtsfx_lbx`, old `0x00551228`: missing from current header.
- `s__00559998`, old `0x00559998`: missing from current header.
- `s_craziness_00557680`, old `0x00557680`: missing from current header.
- `s_SCIENCE_LBX_00551290`, old `0x00551290`: missing from current header.
- `s_app_pics_lbx_00556028`, old `0x00556028`: missing from current header.
- `s_APP_PICS_LBX_00556018`, old `0x00556018`: missing from current header.
- `s_Lost_Player_Connection_0055bf3c`, old `0x0055BF3C`: missing from current header.
- `s_REENTERING_Net_Send_Message_0055bf58`, old `0x0055BF58`: missing from current header.
- `s_Sending_Message_To_Individual`, old `0x005609B0`: missing from current header.
- `s_Sending_Message_To_All`, old `0x005609D4`: missing from current header.
- `DAT_00578820`, old `0x00578820`: missing from current header.
- `DAT_00578dc8`, old `0x00578DC8`: missing from current header.
- `DAT_00578dcc`, old `0x00578DCC`: missing from current header.
- `g_cached_mouse_y`, old `0x00578AB8`: missing from current header.
- `g_cached_mouse_x`, old `0x00578ABC`: missing from current header.
- `g_window_focus_state`, old `0x00579328`: missing from current header.
- `g_last_scan_code`, old `0x00589A1C`: missing from current header.
- `s_modem_005607dc`, old `0x005607DC`: missing from current header.
- `s_ipx_005607e4`, old `0x005607E4`: missing from current header.
- `s_tcp_005607e8`, old `0x005607E8`: missing from current header.
- `s_fmt_to_00557e00`, old `0x00557E00`: missing from current header.
- `s_0_00556108`, old `0x00556108`: missing from current header.
- `s_comma_lo_00556a4c`, old `0x00556A4C`: missing from current header.
- `s___0055632c`, old `0x0055632C`: missing from current header.
- `s_racestuf_lbx_00556310`, old `0x00556310`: missing from current header.
- `s_STREAMHD_LBX_00555908`, old `0x00555908`: missing from current header.
- `g_race_names_ptrs`, old `0x0059D660`: missing from current header.
- `DAT_0059d660`, old `0x0059D660`: missing from current header.
- `s__00557e60`, old `0x00557E60`: missing from current header.
- `s___s_00551130`, old `0x00551130`: missing from current header.
- `s___005569ec`, old `0x005569EC`: missing from current header.
- `g_trait_modifiers_table`, old `0x0055903A`: missing from current header.
- `s_a_00552414`, old `0x00552414`: missing from current header.
- `s_Auto_Save_00556938`, old `0x00556938`: missing from current header.
- `s_cmbtplnt_lbx`, old `0x00551278`: missing from current header.
- `s_0_0055110d`, old `0x0055110D`: missing from current header.
- `s_BUFFER0_LBX_00551124`, old `0x00551124`: missing from current header.
- `s_DIPLOMAT_LBX_00552350`, old `0x00552350`: missing from current header.
- `s_jimtext_lbx_00552360`, old `0x00552360`: missing from current header.
- `s_jimtext2_lbx_00552340`, old `0x00552340`: missing from current header.
- `s_jimtext2_lbx`, old `0x00552340`: missing from current header.
- `s_percent_s_0055236c`, old `0x0055236C`: missing from current header.
- `s_empty_00551100`, old `0x00551100`: missing from current header.
- `s__005510f0`, old `0x005510F0`: missing from current header.

Outcome: fail

---------------------------------------
src/game/gendraw.h

- `GENDRAW::_direction` @ `0x00566074` — recovered as `int16_t`, but Ghidra xrefs show 16-bit word accesses while the defined object at that address is only an undefined1 byte and the surrounding recovered layout leaves `0x00566072-0x00566073` unowned. I cannot validate the recovered storage extent confidently from the current project/Ghidra evidence, so this is a blocker.
- `GENDRAW::_gendraw_af` @ `0x00566044` — recovered as `MOX2::action_func_t`, but the surrounding recovered layout leaves `0x00566045-0x00566047` unowned even though this is used as a pointer-sized callback slot via `PUSH 0x566044` into `Get_Auto_Function_`, `Execute_Auto_Function_`, and `Put_Auto_Function_`. Ghidra currently defines only an undefined1 at the address, so I cannot validate the recovered full 4-byte extent confidently.
- `GENDRAW::_warning_box` @ `0x00566068` — recovered as `uint8_t`, but the surrounding recovered layout leaves `0x00566069-0x0056606b` unowned before `_animation_frame` at `0x0056606c`. Ghidra only defines an undefined1 here; ownership and exact binary container layout are underspecified enough that I cannot rule out a larger owning object confidently.

Validation failed.

---------------------------------------
src/game/geo.h

- GEO::_orbit_consts, old raw address 0x0054F008: alias/blocker. The pre-variables header enumerates three recovered stubs from this storage region: `_orbit_radiuses` at 0x0054F008, `_orbit_consts_end` at 0x0054F024, and `_dads` at 0x00564840. In the current header only `_orbit_consts` remains. Ghidra shows user-defined labels `_orbit_consts` at 0x0054F008, `_orbit_radiuses` at 0x0054F00c, and `_orbit_consts_end` at 0x0054F024, all in namespace GEO, with the 28-byte owner at 0x0054F008 typed as `short[7][2]`. Because one old stub (`_orbit_radiuses`) points at interior address 0x0054F00c inside that larger owner, the previous recovery set includes an alias candidate for the same storage span. Per the mandatory conservative alias rules, this header does not fully validate.
- GEO::_orbit_consts, old raw address 0x0054F008: current recovery does not account for every old variable stub from the authoritative pre-variables header. The omitted `_orbit_consts_end` stub at 0x0054F024 is still an old raw-address-backed variable from this header. Although disassembly shows it is just the loop end pointer used by `Get_Orbit_From_X_Y_` (`CMP EDI,0x54F024`) and not an independent owning object, the required workflow says every previously recovered variable from the old header must be validated. The current header has no corresponding recovered project-side declaration/definition for `_orbit_consts_end`, so the header is incomplete for validation.

Fail.
src/game/haccess.h

- `_fleet_movement_icon`, old raw address `0x0059C470`: validation failure. The pre-variables header exposed this storage under `HACCESS` as an inline absolute-address-backed array, but the current recovered header removed it entirely instead of preserving a validated project-side variable declaration/definition. Ghidra shows the owner at `0x0059C470` is `MOX::_fleet_movement_icon`, symbol type label in namespace `MOX`, with defined type `s_fleet_movement_icon[500]` and length `4500`. That means the old `HACCESS` stub was an alias to storage owned by `MOX`, so the claimed header does not fully validate every previously recovered variable from that header.

The two string recoveries do validate:
- `s_format_star_roman`, `0x00558014`: binary string storage is `"%s %s"` with length 6, matching `const char[6]`.
- `s_I`, `0x0055801C`: binary string storage is `"I"` with length 2, matching `const char[2]`.

fail
---------------------------------------
src/game/harold.h

Failures:
- `_saved_mouse_list_offset_4`, old raw address `0x00571A44`: missing from the current recovered header and implementation. The `pre-variables` header contains this stub-backed variable explicitly, but the current project has no recovered declaration/definition for it (`rg` found nothing). Ghidra still shows a distinct label at `0x00571A44` (`DAT_00571a44`, Global namespace), so I cannot accept it as silently absorbed without an explicit recovered variable.
- `_saved_mouse_list_offset_4`, old raw address `0x00571A44`: alias/blocker. The old stub resolves in Ghidra to a different storage owner/label in the `Global` namespace, not `HAROLD`; under the alias rules, a recovered `HAROLD` variable at that address would fail namespace ownership, and omitting it also fails completeness.
fail

---------------------------------------
src/game/info.h

Failures:
- `PTR__orange_text_colors`, old raw address `0x00555C40`: alias/namespace-owner mismatch. In `pre-variables` this header claimed the storage at `0x00555C40`, but Ghidra’s primary owner at that address is the global symbol `PTR__orange_text_colors_00555c40`, not `INFO::PTR__orange_text_colors`. Per the alias rules, a recovered variable whose old stub address resolves to a different storage owner/namespace must fail validation. The bytes do match an 8-entry pointer table, but the binary owner is not `INFO`.

Blockers:
- None beyond the above definite failure. Other recovered variables checked here matched their binary storage shape and default bytes closely enough, but the claimed header does not fully validate because of the namespace-owner conflict above.
---------------------------------------
src/game/initgame.h

- `DAT_005974f0` @ `0x005974F0`: old stub was not recovered in this header, but Ghidra shows this address is the owning symbol `MOX::_save_game_description`, an `s_save_game_description[10]` object of 370 bytes. The current `INITGAME` header dropped the stub entirely instead of recovering/removing it via the correct owning namespace. This header therefore does not fully validate on its own.
- `DAT_00597662` @ `0x00597662`: current header is missing this recovered variable entirely. Ghidra shows concrete 16-bit reads/writes at this exact address (`CMP SI, word ptr [0x00597662]`, `MOV [0x00597662], AX`), so it is not safe to dismiss as only an end label. No current project declaration/definition exists at this address under `INITGAME` or elsewhere, so this is a missing recovered variable/blocker.
- `DAT_00597662` @ `0x00597662`: alias/ownership is unresolved against nearby `MOX` globals. The address sits immediately after `MOX::_save_game_description` and before `MOX::_ship_icon_count`; current tree provides no validated owner for the 2-byte storage at `0x00597662`, so validation cannot be completed confidently.

---------------------------------------
src/game/initship.h

Failures:
- `s_Colony_Ship_Localized`, old raw address `0x00558E28`: missing from the current recovered header/definition set. The old stub pointed at `Global::_ship_type_names[1][0][0]`, and Ghidra shows `0x00558E28` is not an owning standalone variable but an interior label inside the larger owning object `char _ship_type_names[5][6][16]` spanning `0x00558DC8..0x00558FA7`. This header’s previous recovered variable is absent, so the header does not fully validate.
- `s_Transport_00558e88`, old raw address `0x00558E88`: missing from the current recovered header/definition set. The old stub pointed at `Global::_ship_type_names[2][0][0]`, and Ghidra shows `0x00558E88` is likewise an interior label inside the same owning object `MOX::_ship_type_names[5][6][16]`, not standalone storage. This header does not retain a directly corresponding recovered variable, so full validation of every previously recovered variable in the old header fails.

Validated:
- `s_Avenger_005576c4`, old raw address `0x005576C4`: current `extern const char s_Avenger_005576c4[8];` with definition `const char s_Avenger_005576c4[8] = "Avenger";` matches Ghidra exactly. Ghidra shows a standalone 8-byte string object at `0x005576C4` with bytes `41 76 65 6E 67 65 72 00` and primary symbol `s_Avenger_005576c4`.

Outcome: fail
---------------------------------------
src/game/mainpups.h

Failures:
- `_anim_10`, old raw address `0x00565DB8`: missing recovered variable. The old header enumerates this as a separate stub, but the current header/implementation omit it entirely. Ghidra shows `0x00565DB8` is an interior label `_anims[10]` inside owning object `MAINPUPS::_anims` at `0x00565D90-0x00565E27`, so this stub was not validated as a recovered project variable.
- `_anim_11`, old raw address `0x00565DBC`: missing recovered variable. The old header enumerates this as a separate stub, but the current header/implementation omit it entirely. Ghidra shows `0x00565DBC` is an interior label `_anims[11]` inside owning object `MAINPUPS::_anims` at `0x00565D90-0x00565E27`, so this stub was not validated as a recovered project variable.
- `_main_scr_font_colors`, old raw address `0x00556D20`: wrong initializer. Current definition initializes `s_colors` as 8 bytes `{0xA0,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9}`, but Ghidra helper reads at `0x00556D20` show bytes `A0 A3 A4 A5 A6 A7 A8 A9 AA ...`. The binary default storage immediately following the claimed 8-byte object contains a real non-zero `0xAA` byte, and Ghidra does not define a separate adjacent owner there. Under the required conservative rules, I cannot confidently accept the recovered extent/initializer.

Result: fail
---------------------------------------
src/game/mainscr.h

Failures:
- _twinkling_stars @ 0x00565460 — old stub-backed variable is missing from the current header/source, so not every recovered variable from this header was restored.
- _fleet_box @ 0x005977C0 — alias failure: Ghidra ownership is `MOX::_fleet_box`, but the current recovered variable is declared/defined in `MAINSCR`. Wrong namespace and conflicting owner.
- _whole_screen_field @ 0x00596134 — alias/namespace inconsistency in Ghidra: primary symbol is `MAINSCR::MAINSCR::_whole_screen_field`, not a clean `MAINSCR::_whole_screen_field`, so ownership does not validate confidently.
- s_EINSTEIN_005597e8 @ 0x005597E8 — alias failure: Ghidra owner at this storage is `MAINSCR::_ALL_TECH_CHEAT`, not the claimed recovered string symbol.
- s_MOOLA_005597f8 @ 0x005597F8 — alias failure: Ghidra owner at this storage is `MAINSCR::_MORE_MONEY_CHEAT`, not the claimed recovered string symbol.
- s_MENLO_00559800 @ 0x00559800 — alias failure: Ghidra owner at this storage is `MAINSCR::_GET_TECH_CHEAT`, not the claimed recovered string symbol.
- s_ISEEALL_00559808 @ 0x00559808 — alias failure: Ghidra owner at this storage is `MAINSCR::_OMNISCIENCE_CHEAT`, not the claimed recovered string symbol.
- s_SCORE_00559820 @ 0x00559820 — alias failure: Ghidra owner at this storage is `MAINSCR::_SCORE_CHEAT`, not the claimed recovered string symbol.

---------------------------------------
src/game/mapgen.h

Failures:
- `_companion_star_flag`, old raw address `0x005A4B18`: alias/owner mismatch. In live Ghidra, `0x005A4B18` is labeled in namespace `MAPGEN` as `_companion_star`, not `_companion_star_flag`. The current project split this into `_companion_star_flag` at `0x005A4B18` plus a separate local `const char _companion_star[2] = "."` backed by different storage at `0x0055632C`. That means the recovered variable at the original stub address no longer matches the binary-side storage owner.
- `_companion_star`, old raw address `0x0055632C`: alias/owner mismatch versus prior recovered stub set. The pre-variables header also had `_is_companion_star` aliased to `_companion_star_flag` at `0x005A4B18`, while `_companion_star` pointed at `0x0055632C`. The current header removed `_is_companion_star` entirely and repurposed the `0x005A4B18` owner into `_companion_star_flag`; this leaves the original recovered owner set inconsistent with Ghidra ownership at `0x005A4B18`.
- `DAT_0056CDC8`, old raw address `0x0056CDC8`: missing restored variable. The old header contains `inline uint32_t& DAT_0056CDC8 = *reinterpret_cast<uint32_t*>(0x0056CDC8);` but there is no corresponding declaration/definition in the current header or implementation. Ghidra still shows an interior default label at `0x0056CDC8`, so the old stub-enumerated variable set was not fully restored or intentionally resolved in-source.
- `__evan_ptr`, old raw address `0x0056CDA8`: name mismatch/uncertain validation. In live Ghidra the symbol at that address is `_evan_ptr` in namespace `MAPGEN`, but the current project declares and defines `__evan_ptr`. Since symbol ownership/name evidence does not match exactly and no stronger binary evidence establishes that the double-underscore form is correct, this cannot be validated confidently.

Outcome: fail
---------------------------------------
src/game/movebox.h

Failures:
- `MOVEBOX::_moveable_box`, old raw address `0x00597860` — alias/namespace failure. In Ghidra the owning symbol at `0x00597860` is `MOX::_moveable_box` with defined type `s_movable_box[5]` length `0x8c`. The current recovery adds a second owner `MOVEBOX::_moveable_box` and even defines another separate project object in `src/game/movebox.cpp`, while `src/game/mox.h`/`src/game/mox.cpp` already recover the same object as `MOX::_moveable_box`.
- `MOVEBOX::_fleet_box`, old raw address `0x005977C0` — alias/namespace failure. In Ghidra the owning symbol at `0x005977C0` is `MOX::_fleet_box` with defined type `s_fleet_box` length `0x21`. The current recovery adds a second owner `MOVEBOX::_fleet_box` and another separate project definition in `src/game/movebox.cpp`, while `src/game/mox.h`/`src/game/mox.cpp` already recover the same object as `MOX::_fleet_box`.
- `MOVEBOX::s__00551100`, old raw address `0x00551100` — alias/namespace failure. In Ghidra the symbol at `0x00551100` is global `s__00551100`, a 1-byte empty string object. The current recovery invents a MOVEBOX-owned array in `src/game/movebox.h`/`src/game/movebox.cpp`, but the same binary string is also separately recovered in `src/game/mox.h`/`src/game/mox.cpp` as `MOX::s__00551100`. Cross-module duplication is allowed for immutable string backing storage, but this specific recovered variable still fails because the old stub in this header did not resolve to namespace `MOVEBOX`; it resolved to the global string object at `0x00551100`.

Result: fail.
---------------------------------------
src/game/mox.h

Failures:
- _moving_a_stack — 0x0056CEB4 — alias/namespace failure. Old mox.h claimed `MOX::_moving_a_stack`, but Ghidra resolves 0x0056CEB4 to symbol `_moving_a_stack` in namespace `MAINSCR`, and the current project also defines it in `src/game/mainscr.h/.cpp`, not in `mox.h`.
- _track_time_flag — 0x0056CEC0 — alias/namespace failure. Old mox.h claimed `MOX::_track_time_flag`, but Ghidra resolves 0x0056CEC0 to symbol `MAINSCR::_track_time_flag`; current project likewise owns it under `MAINSCR`, not `MOX`.
- _temp_bitmap — 0x00596130 — alias/namespace failure. Old mox.h claimed `MOX::_temp_bitmap`, but Ghidra resolves 0x00596130 to `MAINSCR::_temp_bitmap`; current project defines it in `mainscr.h/.cpp`, not `mox.h`.
- _special_scroll_up_button — 0x0059775E — wrong shape. Old stub was scalar `int16_t&`, but Ghidra defines `int16_t[2]` at 0x0059775E, and current project declares/defines `int16_t _special_scroll_up_button[2]`.
- _fltscrn_flashing_monster_ship_icon — 0x005A2C00 — wrong array length. Old stub was `s_animation_header *[8]`, but current project declares/defines `s_animation_header* _fltscrn_flashing_monster_ship_icon[7]`.
- gate_portal_icons — 0x0059BCD0 — missing recovered variable. Old stub exposed a separate variable at 0x0059BCD0, but current `mox.h/.cpp` do not recover it as a project-side symbol. Ghidra shows this address is owned by `MOX::_gate_icon_seg` as a 16-entry array, so the old extra name is an interior/alias candidate and cannot be accepted as a separate recovered variable.
- gate_artemis_icons — 0x0059BD00 — missing recovered variable and alias candidate. Old stub exposed a separate variable at 0x0059BD00, but current `mox.h/.cpp` do not recover it. Ghidra shows 0x0059BD00 as `_gate_icon_seg[12]`, i.e. an interior label inside `MOX::_gate_icon_seg`, not a separate owning object.
- DAT_0059bd22 — 0x0059BD22 — missing recovered variable and alias candidate. Old stub exposed a separate scalar at 0x0059BD22, but current `mox.h/.cpp` do not recover it. Ghidra shows 0x0059BD22 as `_moveable_box_order[1]`, an interior array element inside `MOX::_moveable_box_order`, not a separate owning object.

fail

---------------------------------------
src/game/multplay.h

Failures:
- `MULTPLAY::_cur_pal_colors`, old raw address `0x00579590` — wrong namespace/alias candidate. In Ghidra the primary storage owner at `00579590` is global `_cur_pal_colors`, not `MULTPLAY::_cur_pal_colors`. Per alias/namespace rules this must fail.
- `MULTPLAY::PTR_00558D3C`, old raw address `0x00558D3C` — wrong namespace/alias candidate. In Ghidra the primary owner is global `PTR_s_MULTIGM.LBX_00558d3c`, not a `MULTPLAY` symbol. The recovered variable is also project-defined as a new project-side pointer object rather than matching the live owner namespace.
- `MULTPLAY::_allow_chat_mode`, old raw address `0x0059538C` — cannot be validated confidently. Ghidra has the symbol `MULTPLAY::_allow_chat_mode`, but no defined data type at the address; the project restores it as `uint8_t` zero-initialized, and I do not have code/memory evidence strong enough here to prove byte width versus larger storage from this header alone under the required fail-on-uncertainty rule.
fail

---------------------------------------
src/game/netcode.h

Failures:
- `_tpSending`, old raw address `0x0055BF30`: recovered in `netcode.h/netcode.cpp` as `netcode::_tpSending` with type `int8_t`, but the old stub names `0x0055BF30` and Ghidra both identify this storage as `transprt::_tpSending`. This is a namespace/owner mismatch and alias failure. Current project usage also shows the real transport variable defined separately in `src/game/transprt.cpp` as `transprt::_tpSending` with type `int32_t`.
- `_tpSending`, old raw address `0x0055BF30`: recovered type is also wrong. Current `netcode` declaration/definition use `int8_t`, while the corresponding recovered owner in `transprt.h/transprt.cpp` is `int32_t`, and usage performs full integer compares/assignments. The claimed netcode variable is therefore wrong width/storage interpretation in addition to being an alias.
- `s_Lost_Player_Connection_0055bf3c`, old raw address `0x0055BF3C`: current declaration/definition place this string in namespace `netcode`, but Ghidra resolves the backing storage as a global string symbol, not a `netcode`-owned object. Per the alias/namespace rules, this recovered owner does not match the binary storage owner.
- `s_REENTERING_Net_Send_Message_0055bf58`, old raw address `0x0055BF58`: current declaration/definition place this string in namespace `netcode`, but Ghidra resolves the backing storage as a global string symbol, not a `netcode`-owned object. This is likewise a namespace/owner mismatch.

Other validated items from old stubs were consistent enough with current recovery:
- `_netInitialized` at `0x0055BF24`
- `_inComms` at `0x0055BF34`
- `_dplay` at `0x0055BF28`
- `_recv_fn` at `0x00589990`
- `_connection_type` at `0x00589A0C`
- `_in_send_message` at `0x0055BF38`

fail
src/game/netmox.h

Failures:
- `_net_players_active_mask`, old raw address `0x00595300`: missing from the current recovered header/definition set. The old stub enumerates this variable, but current `src/game/netmox.h` / `src/game/netmox.cpp` do not define it at all.
- `_net_players_active_mask`, old raw address `0x00595300`: alias candidate / wrong ownership. Live Ghidra resolves `0x00595300` inside `NETMOX::_net` (`s_network`, base `0x00595170`, offset `0x190`), and the symbol at that address is `_net.players_active_mask`, i.e. an interior field of the larger owning object, not an independent top-level variable. Any recovered standalone variable there would fail alias rules; omitting it also leaves the old stub unvalidated as a recovered variable.

All other old `netmox.h` variable stubs checked cleanly against current declarations/definitions and Ghidra defaults:
- zero-initialized globals/pointers at `0x00595170`, `0x0055AD40`, `0x00572358`, `0x0057235C`, `0x00572360`, `0x00572374`, `0x00572378`, `0x0057237C`, `0x00572380`, `0x00572384`, `0x00572388`, `0x0057238C`, `0x0055AD44`, `0x0055AD48` are fully zero in binary storage
- string objects at `0x0055AD4C`, `0x00551178`, `0x0055117C`, `0x0055AD58`, `0x0055AD74`, `0x0055AD88`, `0x0055ADA0`, `0x0055ADB4` match their recovered array contents exactly

fail

---------------------------------------
src/game/netstart.h

Failures:
- `NETSTART::s__00551100`, old raw address `0x00551100`: alias candidate and wrong storage owner. Ghidra names the storage at `0x00551100` as global `s__00551100`, and the project already has other owners for the same backing string data (`fields`, `strings`, `sys`, etc.). This NETSTART-local recovered object is not the owning object for that address.
- `NETSTART::s_myname_005597dc`, old raw address `0x005597DC`: wrong array length. Ghidra shows defined string data "myname" of length 7 at that address; the recovered definition is `char[12]`, which extends past the defined object into following data.
- `NETSTART::s__s_00551130`, old raw address `0x00551130`: alias candidate and wrong array length. Ghidra owns `0x00551130` as global `s_%s_00551130` with defined string length 3 (`"%s"` including terminator). The project already has another recovered owner in `funcs.cpp` as `const char s__s_00551130[3]`; NETSTART’s separate `const char[8]` is both a competing owner and the wrong extent.
- `NETSTART::s_Mar_26_1997_005578e0`, old raw address `0x005578E0`: alias candidate. Ghidra owns the storage globally at that address, and the project already defines the same recovered object in `funcs.cpp` as `const char s_Mar_26_1997_005578e0[12]`. NETSTART’s separate definition is a competing owner for the same binary storage.

Validated without failure:
- `_dialing_failed_error` `0x0056CE78`
- `_receiving_chunk_d_of_d` `0x0056CE7C`
- `_really_begin_game_with_unused_slots_msg` `0x0056CE80`
- `_really_start_with_only_1_msg` `0x0056CE84`
- `_no_more_player_slots_error` `0x0056CE8C`
- `_game_full_string` `0x0056CE90`
- `_answering_failed_error` `0x0056CE94`
- `_windows_string` `0x0056CE98`

fail

---------------------------------------
