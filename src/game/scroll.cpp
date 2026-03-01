#include "pch.h"

namespace SCROLL {
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
            uint16_t step = (uint16_t)sb->row_step;
            sb->first_visible_row += step;

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
}