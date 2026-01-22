#pragma once

namespace mouse {
    inline uint16_t& _current_mouse_list_count =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x00578A90 });

    inline uint8_t*& _current_mouse_list =
        *reinterpret_cast<uint8_t**>(std::uintptr_t{ 0x00578A98 });

    // Suggested name: mouse::Check_Mouse_Shape_
    inline void __cdecl Check_Mouse_Shape_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00527510 });
        fn();
    }

    void Set_Mouse_List_(uint8_t* mouse_data, uint16_t count);
}