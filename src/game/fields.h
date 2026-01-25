#pragma once

namespace fields {
    inline void*& _update_function = *reinterpret_cast<void**>(std::uintptr_t{ 0x0055AFD8 });
    inline uint16_t& _auto_active_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0055AFDC });
    inline uint16_t& _down_mouse_button = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055AFE4});
    inline uint16_t& _auto_input_variable = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B004});
    inline uint16_t& _input_field_active = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B040});
    inline uint16_t& _active_input_field_number = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B04C});
    inline uint16_t& _multi_hotkey_active_field = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x0055B064});
    inline uint16_t& _input_delay = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00572C00});
    inline uint16_t& _fields_count = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00572D18});

    void __cdecl Assign_Update_Function_(void* update_func);
    void __cdecl Deactivate_Auto_Function_();
    void __cdecl Init_Fields_State_();
}