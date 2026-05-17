#include "pch.h"

namespace SCROLL {
    int8_t __cdecl Set_Vertical_Scroll_Bar_Maxima_(s_scroll_bar* scroll_bar) {
        int16_t half_height = scroll_bar->thumb_height_px / 2;

        if (scroll_bar->thumb_center_y + half_height > scroll_bar->track_len_px) {
            scroll_bar->thumb_center_y = scroll_bar->track_len_px - half_height;
            return 1;
        }

        if (scroll_bar->thumb_center_y - half_height < 0) {
            scroll_bar->thumb_center_y = half_height;
            return 1;
        }

        return 0;
    }

    int16_t __cdecl Get_Vertical_Scroll_Bar_Height_(s_scroll_bar* sb) {
        sb->thumb_y_snapshot = sb->thumb_center_y;

        if (sb->total_rows < sb->visible_rows) {
            return sb->track_len_px;
        }

        int32_t track_len = (int32_t)sb->track_len_px;
        int32_t numerator = (track_len + 1) * (int32_t)sb->visible_rows - 1;

        return (int16_t)(numerator / (int32_t)sb->total_rows);
    }

    void __cdecl Scroll_Down_One_Item_(s_scroll_bar* sb) {
        if (sb->total_rows > sb->visible_rows) {
            uint8_t step = sb->row_step;
            sb->first_visible_row = (int16_t)((uint16_t)sb->first_visible_row + step);

            if ((sb->total_rows - sb->visible_rows) < sb->first_visible_row) {
                // Clamp to bottom
                sb->first_visible_row = sb->total_rows - sb->visible_rows;
                sb->thumb_center_y = sb->thumb_height_px / 2;
            }
            else {
                // Move thumb position
                sb->thumb_center_y -= (sb->track_len_px / sb->total_rows) * step;
            }

            sb->last_thumb_center_y = sb->thumb_center_y;
            sb->thumb_center_y_copy = sb->thumb_center_y;
        }
    }

    void __cdecl Scroll_Up_One_Item_(s_scroll_bar* bar) {
        if (bar->visible_rows < bar->total_rows) {
            bar->first_visible_row -= (int16_t)bar->row_step;

            if (bar->first_visible_row <= 0) {
                bar->first_visible_row = 0;
                bar->thumb_center_y = bar->track_len_px + (bar->thumb_height_px / 2);
            } else {
                bar->thumb_center_y += (bar->track_len_px / bar->total_rows) * (int16_t)bar->row_step;
            }

            bar->last_thumb_center_y = bar->thumb_center_y;
            bar->thumb_center_y_copy = bar->thumb_center_y;
        }
    }

    int16_t __cdecl Get_Vertical_Scroll_Bar_First_Item_(s_scroll_bar* sb) {
        int16_t result = 0;
        int16_t total_rows = sb->total_rows;

        if (total_rows > 0) {
            int32_t track_len_px = sb->track_len_px;

            int32_t diff = track_len_px - (int16_t)(sb->thumb_height_px / 2 + sb->thumb_center_y);
            int32_t step = ((uint8_t)sb->row_step * track_len_px * 10) / total_rows;

            result = (diff * 10) / (int16_t)step;

            if (sb->visible_rows + result > total_rows) {
                result = total_rows - sb->visible_rows;
            } else if (total_rows > sb->visible_rows) {
                int16_t mox_bp = MOX::_scroll_bar->track_len_px -
                                 MOX::_scroll_bar->thumb_height_px / 2 -
                                 MOX::_scroll_bar->thumb_center_y;

                if (mox_bp + sb->thumb_height_px >= track_len_px) {
                    result = total_rows - sb->visible_rows;
                }
            }

            if (result <= 0) {
                result = 0;
            }
        }

        return result;
    }

    void __cdecl Update_Vertical_Scroll_Bar_State_(s_scroll_bar* scroll) {
        scroll->thumb_height_px = Get_Vertical_Scroll_Bar_Height_(scroll);
        Set_Vertical_Scroll_Bar_Maxima_(scroll);
        scroll->first_visible_row = Get_Vertical_Scroll_Bar_First_Item_(scroll);
    }

    bool __cdecl Draw_Generic_Vertical_Scroll_Bar_(s_scroll_bar* scroll_bar, void(__cdecl* callback)()) {
        int16_t mouse_btn = mouse::Mouse_Button_();
        int16_t orig_first_visible_row = scroll_bar->first_visible_row;

        scroll_bar->mouse_button_down = 0;
        scroll_bar->thumb_height_px = Get_Vertical_Scroll_Bar_Height_(scroll_bar);

        if (scroll_bar->last_thumb_center_y == -1) {
            scroll_bar->last_thumb_center_y = scroll_bar->thumb_center_y;
            scroll_bar->thumb_center_y_copy = scroll_bar->thumb_center_y;
        } else if (scroll_bar->last_thumb_center_y != scroll_bar->thumb_center_y) {
            int32_t diff = abs(scroll_bar->thumb_center_y - scroll_bar->thumb_center_y_copy);
            if (diff > (scroll_bar->thumb_height_px / 2)) {
                Update_Vertical_Scroll_Bar_State_(scroll_bar);
                scroll_bar->last_thumb_center_y = scroll_bar->thumb_center_y;
                scroll_bar->thumb_center_y_copy = scroll_bar->thumb_center_y;
            }
        }

        if (Set_Vertical_Scroll_Bar_Maxima_(scroll_bar)) {
            scroll_bar->first_visible_row = Get_Vertical_Scroll_Bar_First_Item_(scroll_bar);
            scroll_bar->last_thumb_center_y = scroll_bar->thumb_center_y;
            scroll_bar->thumb_center_y_copy = scroll_bar->thumb_center_y;
        }

        callback();

        if (mouse_btn == 1) {
            scroll_bar->mouse_button_down++;
        }

        return scroll_bar->first_visible_row != orig_first_visible_row;
    }
}