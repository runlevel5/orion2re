#include "pch.h"

#include "fakes/fake_test_support.h"

namespace key {
    int32_t _last_key_pressed[10]{};
    uint8_t _key_pressed = 0;
    uint8_t _key_write = 0;
    int8_t _key_read = 0;

    uint8_t Keyboard_Status_() {
        return test::fakes::g_keyboard_status;
    }

    int32_t Read_Key_() {
        return test::fakes::g_read_key_value;
    }

    void Push_Key_Code_(uint32_t key_code, uint32_t scan_code) {
        (void)key_code;
        (void)scan_code;
    }

    int32_t Wait_For_Key_() {
        return test::fakes::g_read_key_value;
    }

    int16_t Scan_Key_() {
        if (!test::fakes::g_scan_key_sequence.empty()) {
            int16_t value = test::fakes::g_scan_key_sequence.front();
            test::fakes::g_scan_key_sequence.erase(test::fakes::g_scan_key_sequence.begin());
            test::fakes::g_scan_key_value = value;
            return value;
        }

        return test::fakes::g_scan_key_value;
    }
}

namespace mouse {
    uint16_t _mouse_enabled = 0;
    uint16_t _mouse_state = 0;
    int16_t _current_mouse_list_count = 0;
    uint32_t _num = 0;
    int16_t _mouse_buffer_button = 0;
    int16_t _mouse_buffer_x = 0;
    int16_t _mouse_buffer_y = 0;
    s_mouse_list* _current_mouse_list = nullptr;
    int32_t _in_mouse_handler = 0;
    int16_t _mouse_overide_flag = 0;
    int16_t _mouse_overide_right_button = 0;
    int16_t _mouse_overide_left_button = 0;
    int16_t _mouse_buttons = 0;
    int16_t _mouse_buffer_flag = 0;
    uint16_t _mouse_buffer_flag2 = 0;

    void __cdecl Check_Mouse_Shape_() {
    }

    void __cdecl Check_Mouse_Buffer_() {
    }

    void __cdecl User_Mouse_Handler_(int16_t mouse_x, int16_t mouse_y, uint16_t mouse_buttons) {
        (void)mouse_x;
        (void)mouse_y;
        (void)mouse_buttons;
    }

    int16_t __cdecl Mouse_Buffer_() {
        int16_t value = test::fakes::g_mouse_buffer_flag;
        test::fakes::g_mouse_buffer_flag = 0;
        return value;
    }

    uint16_t __cdecl Mouse_Buffer2_() {
        uint16_t value = test::fakes::g_mouse_buffer_flag2;
        test::fakes::g_mouse_buffer_flag2 = 0;
        return value;
    }

    int16_t __cdecl Mouse_Button_() {
        if (!test::fakes::g_mouse_button_sequence.empty()) {
            int16_t value = test::fakes::g_mouse_button_sequence.front();
            test::fakes::g_mouse_button_sequence.erase(test::fakes::g_mouse_button_sequence.begin());
            test::fakes::g_mouse_button = value;
            return value;
        }

        return test::fakes::g_mouse_button;
    }

    void __cdecl Set_Mouse_List_(s_mouse_list* mouse_list, int16_t list_count) {
        (void)mouse_list;
        (void)list_count;
    }

    int16_t Pointer_X_() {
        return test::fakes::g_mouse_pointer_x;
    }

    int16_t Pointer_Y_() {
        return test::fakes::g_mouse_pointer_y;
    }

    void __cdecl Clear_Mouse_XY_() {
    }

    int __cdecl Get_Pointer_Offset_() {
        return test::fakes::g_mouse_pointer_offset;
    }

    int __cdecl Get_Pointer_Image_Number_() {
        return 0;
    }

    void __cdecl Save_Mouse_State_() {
    }

    void __cdecl Restore_Mouse_State_() {
    }

    int16_t __cdecl Mouse_Buffer_Button_() {
        return test::fakes::g_mouse_buffer_button;
    }

    int16_t __cdecl Mouse_Buffer_X_() {
        return test::fakes::g_mouse_buffer_x;
    }

    int16_t __cdecl Mouse_Buffer_Y_() {
        return test::fakes::g_mouse_buffer_y;
    }

    void __cdecl Disable_Mouse_() {
    }

    void __cdecl Enable_Mouse_() {
    }
}