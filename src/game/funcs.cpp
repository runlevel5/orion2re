#include "pch.h"

const char* PTR_s_OFFICER_LBX_00556990 = "OFFICER.LBX";

char s__00551100[1] = "";

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
        static_cast<s_animation_header*>(buffer::Buffer_Reload_("BUFFER0.LBX", 0x39, MOX::_global_cache_seg));
    animate::Draw_(
        static_cast<int16_t>(MOX::_fleet_box.x + MOX::_moveable_box[2].content_offset_x + 0xBD),
        static_cast<int16_t>(MOX::_fleet_box.y + MOX::_moveable_box[2].content_offset_y + 0x42),
        scroll_anim);
}

const char s__00551120[2] = {'\x1B', '\0'};
const char s_SAVE_005568fc[5] = "SAVE";
const char s__GAM_005568f4[5] = ".GAM";
const char s_0_0055110c[3] = "\x1A" "0";
const char s_1_00551110[3] = "\x1A" "1";
const char s_Unable_to_open_mox_set_005575f4[23] = "Unable to open mox.set";
const char s_Evan_Axelrad_0055AA48[16] = "Evan Axelrad";
const char s_Refresh_Mask_can_t_be_drawn_0055b10c[28] = "Refresh Mask can't be drawn";
const char s_warning_lbx_00557650[12] = "warning.lbx";