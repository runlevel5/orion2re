# Alias inventory

This file captures source-level aliases verified against the current Ghidra project for ORION95.EXE.

Scope:
- fixed-address source bindings found under `src/`
- exact same-address aliases
- interior aliases where a source symbol points inside a larger Ghidra object
- suspicious aliases that are likely misnamed or should be normalized

Notes:
- Ghidra data/layout was treated as source of truth
- many exact aliases are harmless duplicate names for the same string/resource
- `funcs.h` contains many duplicate declarations that overlap with more specific headers

## Exact same-address aliases - we need to only keep one name per address

Format:
- `address | ghidra label | ghidra type | source names`

```text
0055135C | s_billtext.lbx_0055135c | string | lbx_billtext, s_billtext_lbx, s_billtext_lbx_0055135c
0055136C | s__0055136c | string | s__0055136c, s_carriage_return, s_newline, s_newline_0055136c
00552340 | s_jimtext2.lbx_00552340 | string | s_jimtext2_lbx, s_jimtext2_lbx_00552340
00552350 | s_DIPLOMAT.LBX_00552350 | string | s_DIPLOMAT_LBX, s_DIPLOMAT_LBX_00552350
0055236C | s_%s_0055236c | string | s__s_0055236c, s_spaces_percent_s_0055236c
0055238C | s_%s,_%s_0055238c | string | s__s___s_0055238c, s_format_comma_space
00555FF0 | s_%i,%i_00555ff0 | string | s_fmt_comma, s_fmt_comma_00555ff0
00555FF8 | s_%i.%i_00555ff8 | string | s_fmt_dot, s_fmt_dot_00555ff8
00556070 | s_^_00556070 | string | s___00556070, s_caret_space_00556070
005560E4 | s_B_005560e4 | string | s_B, s_B_005560e4
00556168 | s_,_00556168 | string | s___00556168, s_comma_space_00556168
0055632C | s_._0055632c | string | _companion_star, s___0055632c, s_dot_0055632c
005569C0 | - | - | s_GEGGGGG_format, s_geggggg_format, s_s_s_005569c0
005569CC | s_S_005569cc | string | s_S, s_S_005569cc
005569DC | s_D_005569dc | string | s_D, s_D_005569dc
005569F8 | s__005569f8 | string | s_Newline, s__005569f8
00556AFC | s_O_00556afc | string | s_O, s_O_00556afc
00556B10 | s_1_00556b10 | string | s_1, s_1_00556B10
00556D98 | s_%s:_%d_00556d98 | string | s_percent_s_percent_d_00556d98, s_str_fmt_00556d98
00557444 | s_textbox.lbx_00557444 | string | s_textbox_lbx, s_textbox_lbx_00557444
00557E60 | s__00557e60 | string | s_Newline2, s__00557e60
00557ED4 | s_A_00557ed4 | string | s_A, s_A_00557ed4
00557ED8 | s_M_00557ed8 | TerminatedCString | s_M, s_M_00557ed8
0055801C | s_I_0055801c | string | s_I, s_I_0055801c
00558238 | s_PR_00558238 | string | s_PR_00558238, s_pr_str
0055823C | s_MC_0055823c | string | s_MC_0055823c, s_mc_str
00558240 | s_FP_00558240 | char[4] | s_FP_00558240, s_fp_str
00558244 | s_BC_00558244 | char[3] | s_BC_00558244, s_bc_str
00558248 | s_RP_00558248 | string | s_RP_00558248, s_rp_str
0055824C | s_R_0055824c | string | s_R, s_R_0055824c
00558D04 | s_MULTIGM.LBX_00558d04 | string | s_MULTIGM_LBX, s_MULTIGM_LBX_00558D04
00559A74 | s_council.LBX_00559a74 | string | LBX_00559a74, s_council_lbx
0055A8B0 | s_%s_%s_0055a8b0 | string | s_fmt_s_s, s_tab_s_s_r
0055A8D0 | s_%+d_%s_%s_0055a8d0 | string | s_fmt_plus_d_s_s, s_tab_d_s_s_r
0055A8DC | s_%+d_%s_0055a8dc | string | s_fmt_plus_d_s, s_tab_d_s_r
0055A8FC | s_+%d_%s_0055a8fc | string | s_fmt_plus_d_s_3, s_tab_plus_d_s_r
0055AA48 | s_Evan_Axelrad_0055aa48 | string | s_Evan_Axelrad, s_Evan_Axelrad_0055AA48
0055B10C | s_Refresh_Mask_can't_be_drawn_0055b10c | string | s_Refresh_Mask_can_t_be_drawn, s_Refresh_Mask_can_t_be_drawn_0055b10c
0055B83C | s_Too_many_frames_for_multi_file_a_0055b83c | string | s_Too_many_frames_for_multi_file_a, s_Too_many_frames_for_multi_file_a_0055b83c
```

## Suspicious aliases to normalize

These are the most obvious cases where one name is ambiguous, duplicate, or likely wrong.

0055632C | s_._0055632c | string | _companion_star, s___0055632c, s_dot_0055632c
  note: Ghidra shows this is the string "."; _companion_star is likely a bad semantic alias

0056CE24 | _noncombat_ships_destroyed_msg | undefined4 | _combfind_strings_base, _noncombat_ships_destroyed_msg
  note: same pointer slot, likely one generic base pointer aliasing a specific string pointer name

005977CC | inside _fleet_box | s_fleet_box.count | _box_off_flag_, _fleet_box_count
  note: should be treated as _fleet_box.count, not two independent globals

0059799C | _design_screen_replacement_slot | undefined2 | _design_screen_replacement_slot, _selected_weapon_index
  note: same 16-bit object with conflicting semantics; likely keep _design_screen_replacement_slot

005A7AB6 | DAT_005a7ab6 | undefined2 | DAT_005a7ab6, raceopt_flag_005a7ab6
  note: semantic alias is preferable to raw DAT_ name

00596D60 | _weapons_arc_pict_seg | s_animation_header *[5] | _weapons_arc_pict_seg, weapons_arc_pict_seg
  note: exact duplicate declaration

00597030 | _weapons_arc_text_seg | s_animation_header *[5] | _weapons_arc_text_seg, weapons_arc_text_seg
  note: exact duplicate declaration
```

## Practical cleanup guidance

Short list of aliases that are safest to normalize first:

- use `_game_guid` instead of `GUID_0054F150`
- use `_noncombat_ships_destroyed_msg` instead of `_combfind_strings_base`
- use `_design_screen_replacement_slot` instead of `_selected_weapon_index`
- use `raceopt_flag_005a7ab6` instead of `DAT_005a7ab6`
- use `_fleet_box_count` instead of `_box_off_flag_`
- remove `funcs.h` duplicates where a more specific header already defines the same address

## Status

Saved during paused refactor work. No additional code changes were applied as part of writing this file.