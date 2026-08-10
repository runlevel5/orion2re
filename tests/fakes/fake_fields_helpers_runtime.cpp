#include "pch.h"

namespace animate {
    int16_t _clipping_flag = 0;
}

namespace fields {
    int16_t _help_list_active = 0;
    int16_t _help_list_count = 0;
    s_help_box* _help_struct_pointer = nullptr;
    uint16_t _mouse_cancel_disabled = 0;
    uint16_t _field_box_mode = 0;
    uint16_t _help_box_mode = 0;
    uint16_t _draw_fields_flag = 1;
    uint16_t _dynamic_field_refresh = 1;
    int16_t _mouse_auto_exit = 0;
    uint32_t _draw_override_flag = 0;
    int16_t _selected_count = 0;
    uint16_t _list_field_on = 0;
    uint16_t _list_field_scan_flag = 1;
    int16_t _pressed_button_flag = 0;
    int16_t _last_button_number = 0;
    int16_t _last_button_x = 0;
    int16_t _last_button_y = 0;

    void __cdecl Clear_Fields_() {
    }
}

namespace platform {
    void __cdecl Request_Exit_(const char*) {
    }

    void __cdecl Request_Fatal_Exit_(const char*) {
    }

    void __cdecl Check_Exit_() {
    }
}