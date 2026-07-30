#include "pch.h"

namespace mouse {
    uint16_t _mouse_enabled;
    uint16_t _mouse_state;
    int16_t _current_mouse_list_count;
    uint32_t _num;
    int16_t _mouse_buffer_button;
    int16_t _mouse_buffer_x;
    int16_t _mouse_buffer_y;
    s_mouse_list* _current_mouse_list;
    int32_t _in_mouse_handler;
    int16_t _mouse_overide_flag;
    int16_t _mouse_overide_right_button;
    int16_t _mouse_overide_left_button;
    int16_t _mouse_buttons;
    int16_t _mouse_buffer_flag;
    uint16_t _mouse_buffer_flag2;

    int16_t __cdecl Mouse_Buffer_() {
        platform::Pump_Game_Input_Queue_();
        int16_t old_flag = _mouse_buffer_flag;
        _mouse_buffer_flag = 0;
        return old_flag;
    }

    uint16_t __cdecl Mouse_Buffer2_() {
        platform::Pump_Game_Input_Queue_();
        uint16_t original_flag = _mouse_buffer_flag2;
        _mouse_buffer_flag2 = 0;
        return original_flag;
    }

    int16_t __cdecl Mouse_Button_() {
        platform::Pump_Game_Input_Queue_();
        if (_mouse_overide_flag == 0) {
            return _mouse_buttons;
        }

        return _mouse_overide_right_button + _mouse_overide_left_button;
    }

    void __cdecl Set_Mouse_List_(s_mouse_list* mouse_list, int16_t list_count) {
        _current_mouse_list_count = list_count;
        _current_mouse_list = mouse_list;

        Check_Mouse_Shape_();
        platform::Update_Cursor_Image_();
    }

    int16_t Pointer_X_() {
        platform::Pump_Game_Input_Queue_();
        return platform::Get_Mouse_X_();
    }

    int16_t Pointer_Y_() {
        platform::Pump_Game_Input_Queue_();
        return platform::Get_Mouse_Y_();
    }

    void __cdecl Check_Mouse_Buffer_() {
        uint16_t pressed_buttons = (uint16_t)(_mouse_buttons & 0x0003);

        if (pressed_buttons != 0) {
            _mouse_buffer_button = (int16_t)pressed_buttons;

            if (_mouse_overide_flag == 1) {
                _mouse_buffer_button = (int16_t)(_mouse_overide_right_button + _mouse_overide_left_button);
            }

            _mouse_buffer_flag = 1;
            _mouse_buffer_flag2 = 1;
            _mouse_buffer_x = platform::Get_Mouse_X_();
            _mouse_buffer_y = platform::Get_Mouse_Y_();
        }

        ++_num;
    }

    void __cdecl Check_Mouse_Shape_() {
        video::Set_Current_Mouse_List_Ptr_(-1);

        for (int i = 0; i < (int)mouse::_current_mouse_list_count; ++i) {
            s_mouse_list* entry = &mouse::_current_mouse_list[i];

            // Note: frame field is used as a coordinate offset for the mouse position check
            int adjusted_x = (int)platform::Get_Mouse_X_() + (int)entry->frame;
            int adjusted_y = (int)platform::Get_Mouse_Y_() + (int)entry->frame;

            if (entry->x1 <= adjusted_x && adjusted_x <= entry->x2 &&
                entry->y1 <= adjusted_y && adjusted_y <= entry->y2) {
                video::Set_Current_Mouse_List_Ptr_((int16_t)i);
            }
        }

        if (video::Get_Current_Mouse_List_Ptr_() == -1) {
            video::Set_Current_Mouse_Shape_(0);
        } else {
            video::Set_Current_Mouse_Shape_(mouse::_current_mouse_list[video::Get_Current_Mouse_List_Ptr_()].icon);
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
     * Returns the frame offset from the active mouse-list entry.
     * Caller must ensure a valid mouse-list entry is currently selected.
     */
    int __cdecl Get_Pointer_Offset_() {
        const int16_t current_mouse_list_ptr = video::Get_Current_Mouse_List_Ptr_();

        if (_current_mouse_list == nullptr ||
            current_mouse_list_ptr < 0 ||
            current_mouse_list_ptr >= _current_mouse_list_count) {
            return 0;
        }

        return (int)mouse::_current_mouse_list[current_mouse_list_ptr].frame;
    }

    /**
     * Returns the icon index from the active mouse-list entry.
     * Caller must ensure a valid mouse-list entry is currently selected.
     */
    int __cdecl Get_Pointer_Image_Number_() {
        const int16_t current_mouse_list_ptr = video::Get_Current_Mouse_List_Ptr_();

        if (_current_mouse_list == nullptr ||
            current_mouse_list_ptr < 0 ||
            current_mouse_list_ptr >= _current_mouse_list_count) {
            return 0;
        }

        return (int)mouse::_current_mouse_list[current_mouse_list_ptr].icon;
    }

    void __cdecl Reset_Mouse_Runtime_() {
        _mouse_enabled = 0;
        _mouse_state = 0;
        _current_mouse_list_count = 0;
        _num = 0;
        _mouse_buffer_button = 0;
        _mouse_buffer_x = 0;
        _mouse_buffer_y = 0;
        _current_mouse_list = nullptr;
        _in_mouse_handler = 0;
        _mouse_overide_flag = 0;
        _mouse_overide_right_button = 0;
        _mouse_overide_left_button = 0;
        _mouse_buttons = 0;
        _mouse_buffer_flag = 0;
        _mouse_buffer_flag2 = 0;
        video::Set_Current_Mouse_List_Ptr_(-1);
        video::Set_Current_Mouse_Shape_(0);
    }

    uint16_t __cdecl Mouse_Enabled_() {
        return _mouse_enabled;
    }

    int16_t __cdecl Copy_Current_Mouse_List_(s_mouse_list* out_mouse_list, int16_t max_count) {
        if (out_mouse_list == nullptr || max_count <= 0 || _current_mouse_list == nullptr || _current_mouse_list_count <= 0) {
            return 0;
        }

        int16_t copy_count = _current_mouse_list_count;
        if (copy_count > max_count) {
            copy_count = max_count;
        }

        memcpy(out_mouse_list, _current_mouse_list, (size_t)copy_count * sizeof(out_mouse_list[0]));
        return copy_count;
    }

    void __cdecl Save_Mouse_State_() {
        _mouse_state = _mouse_enabled;
        _mouse_enabled = 0;
        platform::Update_Cursor_Image_();
    }

    void __cdecl Restore_Mouse_State_() {
        _mouse_enabled = _mouse_state;
        platform::Update_Cursor_Image_();
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
        platform::Update_Cursor_Image_();
    }

    void __cdecl Enable_Mouse_() {
        _mouse_enabled = 1;
        platform::Update_Cursor_Image_();
    }

    void __cdecl User_Mouse_Handler_(int16_t mouse_x, int16_t mouse_y, uint16_t mouse_buttons) {
        if (mouse::_in_mouse_handler == 1) {
            return;
        }

        if (mouse_x < 0) {
            mouse_x = 0;
        } else if (mouse_x >= video::g_screen_res_x) {
            mouse_x = (int16_t)(video::g_screen_res_x - 1);
        }

        if (mouse_y < 0) {
            mouse_y = 0;
        } else if (mouse_y >= video::g_screen_res_y) {
            mouse_y = (int16_t)(video::g_screen_res_y - 1);
        }

        mouse::_in_mouse_handler = 1;

        if (platform::g_pending_toggle_pages) {
            const int16_t previous_mouse_x = platform::Get_Mouse_X_();
            const int16_t previous_mouse_y = platform::Get_Mouse_Y_();

            platform::Set_Logical_Mouse_Position_(mouse_x, mouse_y);
            mouse::_mouse_buttons = (int16_t)mouse_buttons;

            mouse::Check_Mouse_Buffer_();

            mouse::_in_mouse_handler = 0;

            // In the original fullscreen flow, pending-toggle mouse moves are typically
            // synthetic SetCursorPos echoes to the same logical position. In windowed mode,
            // real user movement can also arrive here, so only discard same-coordinate echoes.
            if (mouse_x == previous_mouse_x && mouse_y == previous_mouse_y) {
                platform::Set_Logical_Mouse_Position_(previous_mouse_x, previous_mouse_y);
            }

            return;
        }

        if (mouse::_mouse_overide_flag == 0) {
            platform::Set_Logical_Mouse_Position_(mouse_x, mouse_y);
            mouse::_mouse_buttons = (int16_t)mouse_buttons;
        } else {
            mouse::_mouse_buttons =
                (int16_t)(mouse::_mouse_overide_right_button + mouse::_mouse_overide_left_button);
        }

        mouse::Check_Mouse_Buffer_();

        if ((mouse::_mouse_enabled == 1) && !platform::g_pending_toggle_pages) {
            mouse::Check_Mouse_Shape_();
            platform::Update_Cursor_Image_();
        }

        mouse::_in_mouse_handler = 0;
    }
}