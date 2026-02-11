#include "deps.h"

namespace FLT1 {
    /**
     * @brief Initializes a scroll bar structure with default settings.
     * 
     * @param bar Pointer to the scroll bar structure to initialize.
     */
    void __cdecl Initialize_Scroll_Bar_(s_scroll_bar* bar) {
        bar->visible_rows = 3;
        bar->track_len_px = 121 /* 0x79 */;
        
        // Calculate the physical height of the scroll bar thumb
        int16_t thumb_h = SCROLL::Get_Vertical_Scroll_Bar_Height_(bar);
        bar->thumb_height_px = thumb_h;
        
        bar->last_thumb_center_y = -1;
        bar->field14_0x18 = 2;
        bar->row_step = 1;
        
        // Position the thumb at the bottom of the track (subtracting half its height)
        bar->thumb_center_y = bar->track_len_px - (thumb_h / 2);
    }
}