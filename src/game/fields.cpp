#include "deps.h"

namespace fields {
    void __cdecl Assign_Update_Function_(void* update_func) {
        _update_function = update_func;
    }

    void __cdecl Deactivate_Auto_Function_() {
        _auto_active_flag = 0;
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