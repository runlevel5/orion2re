#include "pch.h"

namespace mouse {
    int16_t __cdecl Mouse_Buffer_() {
        int16_t old_flag = _mouse_buffer_flag;
        _mouse_buffer_flag = 0;
        return old_flag;
    }

    uint16_t __cdecl Mouse_Buffer2_() {
        uint16_t original_flag = _mouse_buffer_flag2;
        _mouse_buffer_flag2 = 0;
        return original_flag;
    }

    int16_t __cdecl Mouse_Button_() {
        if (_mouse_overide_flag == 0) {
            return _mouse_buttons;
        }

        return _mouse_overide_right_button + _mouse_overide_left_button;
    }

    void __cdecl Set_Mouse_List_(s_mouse_list* list, int16_t count) {
        _current_mouse_list_count = count;
        _current_mouse_list = list;

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

    void __cdecl Check_Mouse_Shape_() {
        video::_current_mouse_list_ptr = -1;

        for (int i = 0; i < (int)mouse::_current_mouse_list_count; ++i) {
            s_mouse_list* entry = &mouse::_current_mouse_list[i];

            // Note: frame field is used as a coordinate offset for the mouse position check
            int adjusted_x = (int)platform::g_mouse_x + (int)entry->frame;
            int adjusted_y = (int)platform::g_mouse_y + (int)entry->frame;

            if (entry->x1 <= adjusted_x && adjusted_x <= entry->x2 &&
                entry->y1 <= adjusted_y && adjusted_y <= entry->y2) {
                video::_current_mouse_list_ptr = (int16_t)i;
            }
        }

        if (video::_current_mouse_list_ptr == -1) {
            video::_current_mouse_shape = 0;
        } else {
            video::_current_mouse_shape = mouse::_current_mouse_list[video::_current_mouse_list_ptr].icon;
        }
    }

    void __cdecl Clear_Mouse_XY_() {
        MOX::_mouse_XY.ship_icon_idx = -1;
        MOX::_mouse_XY.hovering_ship_icon_idx = -1;
        MOX::_mouse_XY.star_idx = -1;
        MOX::_mouse_XY.hovering_star_idx = -1;
        MOX::_mouse_XY.selected = -1;
    }

    /**
     * @brief Retrieves the frame offset for the current mouse cursor from the active mouse list.
     * @return The frame index of the current mouse pointer.
     */
    int __cdecl Get_Pointer_Offset_() {
        // The instruction sequence:
        // MOVSX EAX, word ptr [video::_current_mouse_list_ptr]
        // MOV ECX, [mouse::_current_mouse_list]
        // MOVSX EAX, word ptr [ECX + EAX*12 + 2]
        // maps to indexing into the array of s_mouse_list (size 0xC) and returning the 'frame' field (offset 0x2).
        return (int)mouse::_current_mouse_list[video::_current_mouse_list_ptr].frame;
    }

    /**
     * @brief Retrieves the icon index for the currently active mouse pointer from the current mouse list.
     * @return The icon index of the current mouse pointer.
     */
    int16_t __cdecl Get_Pointer_Image_Number_() {
        return mouse::_current_mouse_list[video::_current_mouse_list_ptr].icon;
    }

    void __cdecl Save_Mouse_State_() {
        _mouse_state = _mouse_enabled;
        _mouse_enabled = 0;
    }

    void __cdecl Restore_Mouse_State_() {
        _mouse_enabled = _mouse_state;
    }

    int16_t __cdecl Mouse_Buffer_Button_() {
        return _mouse_buffer_button;
    }

    int16_t __cdecl Mouse_Buffer_X_() {
        return _mouse_buffer_x;
    }

    int16_t __cdecl Mouse_Buffer_Y_() {
        return _mouse_buffer_y;
    }

    void __cdecl Disable_Mouse_() {
        _mouse_enabled = 0;
    }

    void __cdecl Enable_Mouse_() {
        _mouse_enabled = 1;
    }

    void __cdecl User_Mouse_Handler_(int16_t mouse_x, int16_t mouse_y, uint16_t mouse_buttons) {
        if (mouse::_in_mouse_handler == 1) {
            return;
        }

        mouse::_in_mouse_handler = 1;

        if (platform::g_pending_toggle_pages) {
            const int16_t previous_mouse_x = platform::g_mouse_x;
            const int16_t previous_mouse_y = platform::g_mouse_y;

            platform::g_mouse_x = mouse_x;
            platform::g_mouse_y = mouse_y;
            mouse::_mouse_buttons = (int16_t)mouse_buttons;

            mouse::Check_Mouse_Buffer_();

            mouse::_in_mouse_handler = 0;
            platform::g_mouse_x = previous_mouse_x;
            platform::g_mouse_y = previous_mouse_y;
            return;
        }

        if (mouse::_mouse_overide_flag == 0) {
            platform::g_mouse_x = mouse_x;
            platform::g_mouse_y = mouse_y;
            mouse::_mouse_buttons = (int16_t)mouse_buttons;
        } else {
            mouse::_mouse_buttons =
                static_cast<int16_t>(mouse::_mouse_overide_right_button + mouse::_mouse_overide_left_button);
        }

        mouse::Check_Mouse_Buffer_();

        if ((mouse::_mouse_enabled == 1) && !platform::g_pending_toggle_pages) {
            mouse::Check_Mouse_Shape_();
            platform::Draw_Auto_Cursor();
            platform::Save_Mouse_(platform::g_mouse_x, platform::g_mouse_y);
            platform::Draw_Cursor_XY(
                (int16_t)platform::g_mouse_x,
                (int16_t)platform::g_mouse_y);
        }

        mouse::_in_mouse_handler = 0;
    }
}