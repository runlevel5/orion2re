#include "pch.h"

namespace mouse {
    int16_t __cdecl Mouse_Buffer_() {
        int16_t old_flag = _mouse_buffer_flag;
        _mouse_buffer_flag = 0;
        return old_flag;
    }

    uint16_t __cdecl Mouse_Button_() {
        if (_mouse_overide_flag == 0) {
            return g_mouse_buttons_state;
        }

        return _mouse_overide_right_button + _mouse_overide_left_button;
    }

    void __cdecl Set_Mouse_List_(uint8_t* mouse_data, uint16_t count) {
        _current_mouse_list_count = count;
        _current_mouse_list = mouse_data;

        Check_Mouse_Shape_();

        // Send a custom message (WM_USER + 2) to the main window
        SendMessageA(platform::g_main_window, 0x402, 0, 0);
    }

    int16_t Pointer_X_() {
        return platform::g_mouse_x;
    }

    int16_t Pointer_Y_() {
        return platform::g_mouse_y;
    }
}