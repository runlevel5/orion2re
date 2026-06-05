#include "pch.h"

char s___0055b0ac[2] = "_";
const char s_tech_has_too_many_apps_0055a97c[24] = " tech has too many apps";
const char* PTR_s_FLEET_LBX_005584a8 = "FLEET.LBX";
static char s_GAME_LBX_005576e8[9] = "GAME.LBX";
char* PTR_s_GAME_LBX_0055ac4c = s_GAME_LBX_005576e8;
const char* PTR_s_OFFICER_LBX_00556990 = "OFFICER.LBX";

char s__00551100[1] = "";
const char s_Monster_ship_id_cannot_mov_00552cd8[46] = "Monster (ship id: %d) cannot move to %s (%d)!";
char s_Not_Enough_Space_For_Buffer_Spac_0055b310[34] = "Not Enough Space For Buffer Space";
const char s_TEMP_TMP_0055becc[9] = "TEMP.TMP";
const char s_max_file_buffer_count_went_over__0055b334[34] = "max file buffer count went over..";
const char s_max_buffer_segs_went_over___0055b358[28] = "max buffer segs went over..";
const char s_Cache_Seg_Corrupted____0055b374[23] = "Cache Seg Corrupted...";
const char s_File_Animation_Frame_Too_Large_0055b804[31] = "File Animation Frame Too Large";
char s_You_exceeded_the_max_number_of_f_0055b080[44] = "You exceeded the max number of fields (150)";
const char s_Human_00558020[8] = "Human";

const char s_Remap_draw_not_support_map_00557f88[27] = "Remap draw not support map";
const char s_Remap_draw_not_support_screen_00557fa4[30] = "Remap draw not support screen";
const char s_Remap_draw_not_support_bitmaps_00557fc4[31] = "Remap draw not support bitmaps";

const char s_Too_many_colors_to_reduce_to_[39] = "Too many colors to reduce to (Max 256)";
const char s_Too_many_colors_to_reduce_from_[41] = "Too many colors to reduce from (Max 512)";
const char s_Receiving_Message_005609F0[23] = "Receiving Message....\n";

void __cdecl Fleet_Movement_Scroll_Callback() {
    int16_t thumb_height = MOX::_fltpop_scroll_bar.thumb_height_px;
    int16_t track_len = MOX::_fltpop_scroll_bar.track_len_px;
    int16_t thumb_y = MOX::_fltpop_scroll_bar.thumb_center_y;

    int16_t start_y = static_cast<int16_t>(
        MOX::_fleet_box.y +
        MOX::_moveable_box[2].content_offset_y +
        track_len -
        (thumb_height / 2) -
        thumb_y +
        0x43);

    int16_t end_y = static_cast<int16_t>(start_y + track_len);
    int16_t x = static_cast<int16_t>(MOX::_fleet_box.x + MOX::_moveable_box[2].content_offset_x + 0xC0);
    uint8_t color = 0x74;

    for (int16_t i = 0; i < 9; ++i) {
        line::Line_(x, start_y, x, end_y, color);

        if ((i % 2) == 0) {
            --color;
        }

        ++x;
    }

    s_animation_header* scroll_anim =
        static_cast<s_animation_header*>(buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x39, MOX::_global_cache_seg));
    animate::Draw_(
        static_cast<int16_t>(MOX::_fleet_box.x + MOX::_moveable_box[2].content_offset_x + 0xBD),
        static_cast<int16_t>(MOX::_fleet_box.y + MOX::_moveable_box[2].content_offset_y + 0x42),
        scroll_anim);
}

const char s_Add_Square_to_scan_list_failed__0055b270[33] = "Add Square to scan list failed..";

const char s_STREAM_LBX_005558fc[11] = "STREAM.LBX";

char s_a[4] = "a";
char s_an[4] = "an";
s_colors s__GEGGGGG_s___s__005569b8 = {{0x3E, 0x47, 0x45, 0x47, 0x47, 0x47, 0x47, 0x47}};

namespace GLOBALS {
const char s_spaces_percent_s_0055236c[5] = "  %s";
const char s_percent_s_percent_s_percent_s_005574f4[9] = "%s %s %s";
}

const char s__d_BC_00552384[8] = "  %d BC";
const char s_plus_00551118[2] = "+";
const char s_minus_0055111c[2] = "-";
const char s__00551120[2] = {'\x1B', '\0'};
const char s_o_00556af4[2] = "o";
const char s_oc_00556af8[3] = "oc";
const char s_S_005569cc[2] = "S";
const char s_O_00556afc[2] = "O";
const char s_A_00557ed4[2] = "A";
const char s_M_00557ed8[2] = "M";
const char DAT_00558540[2] = "U";
const char DAT_0055aa90[2] = "J";
const char s_no_description_00556920[24] = "<< no description >>";
const char s_0_00551104[4] = "\x1B" "0";
const char s_2_00551108[4] = "\x1B" "2";
const char s_1_00551114[4] = "\x1B" "1";
const char s_SAVE_005568fc[5] = "SAVE";
const char s__GAM_005568f4[5] = ".GAM";
const char s_cmbtplnt_lbx_00551278[13] = "cmbtplnt.lbx";
const char s_0_0055110c[3] = "\x1A" "0";
const char s_1_00551110[3] = "\x1A" "1";
const char s_BUFFER0_LBX[12] = "BUFFER0.LBX";
const char s_billtext_lbx_0055135c[13] = "billtext.lbx";
char s_cmbtsfx_lbx_00551228[12] = "cmbtsfx.lbx";
char s_combat_lbx_00551244[11] = "combat.lbx";
char s_monster_lbx_00551250[12] = "monster.lbx";
char s_cmbtshp_lbx_0055125c[12] = "cmbtshp.lbx";
const char s_mox_set_0055760c[8] = "mox.set";
const char s_Unable_to_open_mox_set_005575f4[23] = "Unable to open mox.set";
char s_SPA_HELP_LBX_00557450[16] = "SPA_HELP.LBX";
char s_ITA_HELP_LBX_00557460[16] = "ITA_HELP.LBX";
char s_FRE_HELP_LBX_00557470[16] = "FRE_HELP.LBX";
char s_GER_HELP_LBX_00557480[16] = "GER_HELP.LBX";
char s_HELP_LBX_00557490[9] = "HELP.LBX";
char s_MULTIGM_LBX_00558D04[12] = "MULTIGM.LBX";
const char s_Evan_Axelrad_0055AA48[16] = "Evan Axelrad";
char s_Moo_IIAlreadyRunning[27] = "MOO II is already running.";
char s_X_005569d0[2] = "X";
char s_Y_00557660[2] = "Y";
char s_confirm_lbx_00557664[12] = "confirm.lbx";
char s___INVALID___0055ac58[12] = "* INVALID *";
const char s_Too_many_frames_for_multi_file_a_0055b83c[41] = "Too many frames for multi file animation";
const char s_Refresh_Mask_can_t_be_drawn_0055b10c[28] = "Refresh Mask can't be drawn";
const char s_warning_lbx_00557650[12] = "warning.lbx";