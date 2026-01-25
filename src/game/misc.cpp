#include "deps.h"

namespace MISC {
    void __cdecl Increment_Mouse_Clock_Image_() {
        if (_mouse_clock_image_num >= 16) {
            _mouse_clock_image_num = 0;
        }

        uint32_t clock_index = _mouse_clock_image_num >> 2;
        uint8_t* cursor_entry = &_mouse_list_clock[clock_index * 12];

        mouse::Set_Mouse_List_(cursor_entry, 1);

        _mouse_clock_image_num++;
    }
}