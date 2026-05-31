#pragma once

#include <stdio.h>
#include <cstdint>

extern char s___0055b0ac[2];
extern const char s_tech_has_too_many_apps_0055a97c[24]; /* " tech has too many apps" */

struct s_animation_header;
struct s_lbx_stream_header;

inline s_animation_header** _anims =
    reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00565D90});

extern const char s_space_005510f0[2]; // " "
extern char s_cmbtsfx_lbx_00551228[12]; // "cmbtsfx.lbx"
extern const char s_fmt_comma_00555ff0[6]; // "%i,%i"
extern const char s_fmt_dot_00555ff8[6]; // "%i.%i"
extern char s_combat_lbx_00551244[11]; // "combat.lbx"
extern char s_monster_lbx_00551250[12]; // "monster.lbx"
extern char s_cmbtshp_lbx_0055125c[12]; // "cmbtshp.lbx"
extern char s_AI_trade_goods_00557670[15]; // "AI trade goods"
extern const char s_Human_00558020[8]; // "Human"
extern const char s_Error_Enum_Current_Games_Invalid_0055c000[40]; // "Error Enum Current Games Invalid Params"
extern const char s_Error_Enum_Current_Games_Invalid_0055c028[40]; // "Error Enum Current Games Invalid Object"

extern const char s_techname_lbx_0055a96c[13]; // "techname.lbx"
extern const char s_INFO_LBX_00556000[9]; // "INFO.LBX"
extern const char s_Remap_draw_not_support_map_00557f88[27]; // "Remap draw not support map"
extern const char s_Remap_draw_not_support_screen_00557fa4[30]; // "Remap draw not support screen"
extern const char s_Remap_draw_not_support_bitmaps_00557fc4[31]; // "Remap draw not support bitmaps"
extern const char s_racesel_lbx_00557ff4[12]; // "racesel.lbx"
extern const char s_1_2_00557ff0[4]; // "1/2"

extern const char s_SCIENCE_LBX_00557d68[12]; // "SCIENCE.LBX"
inline void* Fleet_Movement_Scroll_Callback =
    reinterpret_cast<void*>(std::uintptr_t{0x00404080});
extern const char s_TANM__03hi_LBX_005512a0[15]; /* "TANM_%03hi.LBX" */
extern const char s_SR_R_x_SC_LBX_00557d74[14]; // "SR_R%x_SC.LBX"

extern char s_a[4];
extern char s_an[4];
extern const char s_format_0055aa28[7]; // "(%s)  "

extern const char s_Too_many_colors_to_reduce_to_[39];

extern const char s_Too_many_colors_to_reduce_from_[41];

extern const char s_Receiving_Message_005609F0[23]; // "Receiving Message....\n"

inline void (__stdcall *&PTR_OutputDebugStringA_005aa3c0)(char const *) =
    *reinterpret_cast<void (__stdcall **)(char const *)>(std::uintptr_t{0x005AA3C0});

inline int (__stdcall *&PTR_GetTextExtentPoint32A_005aa3a8)(HDC hdc, const char* lpString, int c, tagSIZE* psizl) =
    *reinterpret_cast<int (__stdcall **)(HDC, const char*, int, tagSIZE*)>(std::uintptr_t{0x005AA3A8});

inline int (__stdcall *&PTR_TextOutA_005aa3ac)(HDC hdc, int x, int y, const char* lpString, int c) =
    *reinterpret_cast<int (__stdcall **)(HDC, int, int, const char*, int)>(std::uintptr_t{0x005AA3AC});

inline uint32_t (__stdcall *&PTR_SetTextColor_005aa3b0)(HDC hdc, uint32_t color) =
    *reinterpret_cast<uint32_t (__stdcall **)(HDC, uint32_t)>(std::uintptr_t{0x005AA3B0});

inline int (__stdcall *&PTR_SetBkMode_005aa3b4)(HDC hdc, int mode) =
    *reinterpret_cast<int (__stdcall **)(HDC, int)>(std::uintptr_t{0x005AA3B4});

inline int (__stdcall *&PTR_lstrlenA_005aa3c4)(const char* lpString) =
    *reinterpret_cast<int (__stdcall **)(const char*)>(std::uintptr_t{0x005AA3C4});

extern const char s_Add_Square_to_scan_list_failed__0055b270[33]; /* "Add Square to scan list failed.." */
extern const char s__d___d___d_0055b2d0[11]; // "%d, %d, %d"
extern const char s_Get_Picture_Color_Picture_Must_H_0055b2dc[45]; // "Get_Picture_Color  Picture Must Have Palette"

inline uint8_t& g_main_screen_unknown_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596196});
inline int16_t& g_map_unknown_word = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B5B4});
inline int16_t& g_ui_unknown_word = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059C02A});
inline uint8_t& g_map_unknown_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059701A});

inline int32_t& DAT_0056178c = *reinterpret_cast<int32_t*>(std::uintptr_t{0x0056178C});
inline uint16_t*& PTR_DAT_00561580 = *reinterpret_cast<uint16_t**>(std::uintptr_t{0x00561580});

extern const char s_percent_0055633c[2]; // "%"
extern const char s_slash_005599a4[2]; // "/"
extern const char s_lparen_005599a8[2]; // "("

inline MMRESULT (__stdcall*& PTR_timeEndPeriod_005aa5c4)(uint32_t) =
    *reinterpret_cast<MMRESULT (__stdcall**)(uint32_t)>(std::uintptr_t{0x005AA5C4});

inline MMRESULT (__stdcall*& PTR_timeKillEvent_005aa5cc)(uint32_t) =
    *reinterpret_cast<MMRESULT (__stdcall**)(uint32_t)>(std::uintptr_t{0x005AA5CC});


inline int16_t& _mouse_XY_star_idx =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x0059B5C4});

extern const char s_fmt_modifier_full_005523a4[6]; // "%d.%d"
extern const char s_fmt_score_simple_00556104[3]; // "%d"
extern const char s_dot_0055632c[2]; // "."
extern const char s_scor_0055a510[5]; // "scor"
extern const char s_RACEOPT_LBX_00556320[12]; // "RACEOPT.LBX"
extern char s_patch13_lbx_00556330[12]; // "patch13.lbx"
extern const char s_fmt_score_complex_0055a518[13]; // "(%d x %s) %d"
extern const char s_percent_s_percent_d_00556d98[7]; /* "%s: %d" */
extern const char s_RACESTUF_lbx_00558550[13]; // "RACESTUF.lbx"

extern const char s_STREAM_LBX_005558fc[11]; /* "STREAM.LBX" */

inline void __cdecl No_Op1_() {
    using Fn = void(__cdecl*)();
    auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004fbef0 });
    fn();
}

namespace GLOBALS {
    extern const char s_spaces_percent_s_0055236c[5];
    extern const char s_percent_s_percent_s_percent_s_005574f4[9];
}

inline const char** g_race_names_ptrs = reinterpret_cast<const char**>(std::uintptr_t{ 0x0059D660 });
inline char**& DAT_0059d660 = *reinterpret_cast<char***>(std::uintptr_t{ 0x0059D660 });
extern char s_360_00556164[4]; // "360"
extern const char s_comma_space_00556168[3]; // ", "

inline uint8_t& DAT_00590000 = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00590000});

extern const char s_Raider_0055768c[7]; // "Raider"
extern char s_rparen_00557f08[2]; // ")"
extern const char s_Marauder_00557694[9]; // "Marauder"
extern char s_space_lparen_0055999c[3]; // " ("
extern char s_space_x_005599a0[3]; // " x"
extern const char s_Interdictor_005576ac[12]; // "Interdictor"
extern const char s_Harbinger_005576b8[10]; // "Harbinger"
extern const char s_RACENAME_LBX[13];
extern const char s_RACESTUF_LBX[13];
extern const char s_WWWWWWW_0055137c[8]; // "WWWWWWW"
extern const char s__s_00551130[3]; // "%s"
struct s_colors;
inline s_colors& s__GEGGGGG_s___s__005569b8 = *reinterpret_cast<s_colors*>(std::uintptr_t{0x005569B8});

// Suggested name: s_Null_Pointer
extern const char s__d_c_d_0055996c[7]; // "%d%c%d"
extern const char s__s_s__s_00559964[8]; // "%s%s %s"
extern const char s__s__s_00558014[6]; // "%s %s"
extern const char s__s_d___d__00559958[10]; // "%s%d (%d)"
extern const char s__s_d_00556134[5]; // "%s%d"
extern const char s___d__00559950[5]; // "(%d)"
extern const char s__s_s_00556aec[5]; // "%s%s"
extern const char s__d___0055612c[5]; // "%d%%"
extern const char s___s_0055994c[4]; // "@%s"
extern const char s___00559948[2]; // "@"
extern const char s_0__s_00559940[5]; // "0 %s"
extern const char s__d__s_005569f0[6]; // "%d %s"
extern char s__00551100[1];   // ""
extern const char s__d_BC_00552384[8];
extern const char s_plus_00551118[2]; // "+"
extern const char s_minus_0055111c[2]; // "-"
extern const char s__00551120[2]; // "\x1b"
extern const char s_c_00556350[2]; // "c"
extern const char s_o_00556af4[2]; // "o"
extern const char s_oc_00556af8[3]; // "oc"
extern const char s_C_00552420[2]; // "C"
extern const char s_H_005569d4[2]; // "H"
extern const char s_S_005569cc[2]; // "S"
extern const char s_D_005569dc[2]; // "D"
extern const char s_L_005560e8[2]; // "L"
extern const char s_O_00556afc[2]; // "O"
extern const char s_A_00557ed4[2]; // "A"
extern const char s_M_00557ed8[2]; // "M"
extern const char s_I_0055801c[2]; // "I"
extern const char s_R_0055824c[2]; // "R"
extern const char DAT_00558540[2];
extern const char DAT_0055aa90[2];

extern const char s_Monster_ship_id_cannot_mov_00552cd8[46]; // "Monster (ship id: %d) cannot move to %s (%d)!"

extern const char s_no_description_00556920[24]; // "<< no description >>"

extern const char s_0_00551104[4];
extern const char s_2_00551108[4];
extern const char s_1_00551114[4];
extern const char s_1_00556B10[2]; // "1"

extern const char s_e_0055ac48[2]; // "e"

extern const char s_Null_Pointer_0055b658[13]; // "Null Pointer"

extern char s_Not_Enough_Space_For_Buffer_Spac_0055b310[34]; // "Not Enough Space For Buffer Space"

// Suggested name: s_allocation_error_fmt
extern const char s_Couldn_t_Allocate_0055B668[38]; // "Couldn't Allocate %d Increase Virtual"

extern const char s_Unable_to_Allocate_bytes[73]; // "Unable to Allocate %d bytes (short by %d bytes) in a block size of %d!\n\n"

extern const char s_fmt_00557F18[7]; // "%d> %s"

extern const char s__2d__s_00557f10[7]; // "%2d %s"

inline const char*& s_format_string = *reinterpret_cast<const char**>(std::uintptr_t{0x00558BD8});

inline const char*& PTR_s_FLEET_LBX_005584a8 = *reinterpret_cast<const char**>(std::uintptr_t{0x005584A8});

extern const char s_RACEOPT_LBX[12]; // "RACEOPT.LBX"
extern const char s_SAVE_005568fc[5]; // "SAVE"
extern const char s__GAM_005568f4[5]; // ".GAM"
extern const char s_rb_0055117c[3]; // "rb"
extern const char s_wb_00551178[3]; // "wb"
extern const char s_Error_in_swap_file_0055beb8[19]; // "Error in swap file"
extern const char s_TEMP_TMP_0055becc[9]; // "TEMP.TMP"
extern const char s_Insufficient_disk_space_for_swap_0055bed8[33]; // "Insufficient disk space for swap"
extern const char s_Drive_swap_reqd_too_large_for_bl_0055befc[36]; // "Drive swap reqd too large for block"
extern const char s_mox_set_0055760c[8]; // "mox.set"
extern const char s_Unable_to_open_mox_set_005575f4[23]; // "Unable to open mox.set"
extern const char s_MOX_SET_00556918[8]; // "MOX.SET"
extern const char s_Settings_file_error_00556950[46]; // "Settings file (MOX.SET) is missing or invalid"

inline int16_t (&_weapon_replacement_screen_help_list)[12][10] =
    *reinterpret_cast<int16_t(*)[12][10]>(std::uintptr_t{0x005648D8});

extern const char s_Antares_00551288[8];
extern const char s_cmbtplnt_lbx_00551278[13]; // "cmbtplnt.lbx"
extern const char s_0_0055110c[3];
extern const char s_1_00551110[3];
extern const char s_caret_space_00556070[3]; /* "^ " */
extern const char s_BUFFER0_LBX[12]; // "BUFFER0.LBX"
inline int16_t& _fleet_box_width =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005977C6});
extern const char s__s___s_0055238c[7]; // "%s, %s"
extern const char s__s_005569e8[4]; // "%s "
extern const char s___s_00557f0c[4]; // "(%s"
extern const char s_rstring_d_lbx[14]; // "rstring%d.lbx"
extern const char s_STARBG_LBX[11]; // "STARBG.LBX"
extern const char s_councmsg_lbx[13]; // "councmsg.lbx"
extern const char s_ships_lbx_005597b4[10]; // "ships.lbx"
extern const char s_billtext_lbx_0055135c[13]; // "billtext.lbx"
extern char s_racerprt_lbx_005574d8[13];
extern const char s_gstar_lbx_00558544[10]; // "gstar.lbx"
extern char s_SPA_HELP_LBX_00557450[16]; // "SPA_HELP.LBX"
extern char s_ITA_HELP_LBX_00557460[16]; // "ITA_HELP.LBX"
extern char s_FRE_HELP_LBX_00557470[16]; // "FRE_HELP.LBX"
extern char s_GER_HELP_LBX_00557480[16]; // "GER_HELP.LBX"
extern char s_HELP_LBX_00557490[9]; // "HELP.LBX"
extern const char s_DIPLOMAT_LBX[13]; /* "DIPLOMAT.LBX" */
extern const char s_dipstars_lbx_00552374[13]; // "dipstars.lbx"

extern const char s_______CLIMATE_modifier_is__d_Pla_00558a58[54]; // "------CLIMATE modifier is %d  Planet: %d  Mineral: %d"

extern char s_MULTIGM_LBX_00558D04[12]; // "MULTIGM.LBX"
extern const char s_Evan_Axelrad_0055AA48[16]; // "Evan Axelrad"
extern char s_Moo_IIAlreadyRunning[27]; // "MOO II is already running."
extern char s_Microprose[11]; // "Microprose"
extern const char s_Mar_26_1997_005578e0[12]; // "Mar 26 1997"

extern const char s_Memory_Corruption_val_00558028[73]; // "Memory Corruption! val == %d, ship_id == %d, owner == %d stardate = %ld\n"

extern const char s_max_file_buffer_count_went_over__0055b334[34]; /* "max file buffer count went over.." */

extern const char s_max_buffer_segs_went_over___0055b358[28]; /* "max buffer segs went over.." */

extern const char s_Cache_Seg_Corrupted____0055b374[23]; /* "Cache Seg Corrupted..." */

extern const char s_File_Animation_Frame_Too_Large_0055b804[31]; // "File Animation Frame Too Large"

extern const char s_Too_many_frames_for_multi_file_a_0055b83c[41]; /* "Too many frames for multi file animation" */

extern const char s_Refresh_Mask_can_t_be_drawn_0055b10c[28]; // "Refresh Mask can't be drawn"

extern char s_Cannot_color_replace_refresh_mas_0055b7a0[35]; // "Cannot color replace refresh mask."

extern char s_PR_00558238[3]; // "PR"
extern char s_FP_00558240[4]; // "FP"
extern char s_RP_00558248[3]; // "RP"
extern char s_PI_00559994[3]; // "PI"
extern char s_G_0055241c[2]; // "G"
extern char s_P_005569d8[2]; // "P"
extern char s_W_0055992c[2]; // "W"
extern char s_r_00556db0[2]; // "r"
extern char s_Q_005598f0[2]; // "Q"
extern const char s_F_005598f4[2]; // "F"
extern char s_T_005598f8[2]; // "T"
extern const char s_combat_lbx_005599c8[11]; // "combat.LBX"
extern const char s_warning_lbx_00557650[12]; // "warning.lbx"

extern char s_You_exceeded_the_max_number_of_f_0055b080[44];

extern char s_X_005569d0[2]; // "X"
extern char s_comma_005569c8[2]; // ","
extern char s_N_0055765c[2]; // "N"
extern char s_Y_00557660[2]; // "Y"
extern char s_confirm_lbx_00557664[12]; // "confirm.lbx"
extern char s_dot_d_0055a528[4]; // ".%d"
inline char*& PTR_s_GAME_LBX_0055ac4c =
    *reinterpret_cast<char**>(std::uintptr_t{0x0055AC4C});
inline void*& PTR_s_OFFICER_LBX_00556990 =
    *reinterpret_cast<void**>(std::uintptr_t{0x00556990});
extern char s___INVALID___0055ac58[12]; // "* INVALID *"
extern char s__3s__2d____4d__2d__2d_0055ac64[23]; // "%3s %2d, %-4d  %2d:%2d"

extern char s_fmt_d_s_s_00557e0c[11]; /* "%d %s (%s)" */
extern char s_fmt_s_d_EP_00557e18[11]; /* "%s (%d EP)" */

extern char s_Too_many_ships_in_list_00557de8[23]; /* "Too many ships in list" */

extern char s_i_PR_00557d84[6]; // "%i PR"
extern char s_i_FP_00557d8c[6]; // "%i FP"
extern char s_i_RP_00557d94[6]; // "%i RP"
extern const char s_s_sez__s_00552334[11]; // "%s sez: %s"
extern const char s_PR_00557d9c[4];
extern const char s_FP_00557da0[4];
extern const char s_RP_00557da4[4];

struct s_fleet_movement_icon;

struct IDirectSound;
struct s_smack_video;

extern const char s___s__String___d_Too_Long__check_t_0055A740[56]; // "(%s) String #%d Too Long, check terminator (strlen: %i)"
extern const char s_HISTRNGS_LBX_0055A778[16]; // "HISTRNGS.LBX"
extern const char s_HSSTRNGS_LBX_0055A788[16]; // "HSSTRNGS.LBX"
extern const char s_HFSTRNGS_LBX_0055A798[16]; // "HFSTRNGS.LBX"
extern const char s_HGSTRNGS_LBX_0055A7A8[16]; // "HGSTRNGS.LBX"
extern const char s_HESTRNGS_LBX_0055A7B8[16]; // "HESTRNGS.LBX"

inline void __stdcall SmackSoundUseDirectSound(IDirectSound* direct_sound) {
    using Fn = void(__stdcall*)(IDirectSound*);
    auto fn = *reinterpret_cast<Fn*>(std::uintptr_t{ 0x005AA62C });
    fn(direct_sound);
}

inline s_smack_video* __stdcall SmackOpen(char* path, uint32_t flags, int32_t priority) {
    using Fn = s_smack_video*(__stdcall*)(char*, uint32_t, int32_t);
    auto fn = *reinterpret_cast<Fn*>(std::uintptr_t{ 0x005AA648 });
    return fn(path, flags, priority);
}

extern const char k_backslash[2]; // "\\"
