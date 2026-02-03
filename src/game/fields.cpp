#include "deps.h"

namespace fields {
    void __cdecl Assign_Update_Function_(void* update_func) {
        _update_function = update_func;
    }

    void __cdecl Assign_Auto_Function_(void* function_ptr, int16_t delay) {
        _auto_active_flag = 1;
        _function = function_ptr;

        if (delay > 0 && delay < 10) {
            _auto_function_delay = delay;
        } else {
            _auto_function_delay = 2;
        }
    }

    void __cdecl Deactivate_Auto_Function_() {
        _auto_active_flag = 0;
    }

    void __cdecl Set_Help_List_(void* help_data, int16_t count) {
        _help_list_count = count;
        _help_struct_pointer = help_data;
        _help_list_active = 1;
    }

    void __cdecl Disable_Draw_All_Fields_() {
        _draw_fields_flag = 0;
    }

    void __cdecl Enable_Draw_All_Fields_() {
        _draw_fields_flag = 1;
    }

    void __cdecl Deactivate_Help_List_() {
        _help_list_active = 0;
        _help_struct_pointer = nullptr;
        _help_list_count = 0;
    }

    void __cdecl Invoke_Auto_Function_() {
        if (_auto_active_flag != 0) {
            if (_function != nullptr) {
                auto fn = reinterpret_cast<void(__cdecl*)()>(_function);
                fn();
            }
        }
    }

    void __cdecl Clear_Fields_() {
        _fields_count = 1;
        _input_delay = 0;
        _down_mouse_button = -1;
        _auto_input_variable = 0;
        _input_field_active = 0;
        _active_input_field_number = -1;
        _multi_hotkey_active_field = 0;

        mouse::Mouse_Buffer_();
        mouse::Mouse_Buffer2_();
    }
}