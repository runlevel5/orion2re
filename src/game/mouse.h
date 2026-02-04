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

    inline uint16_t& _mouse_overide_flag =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0055B180 });

    inline uint16_t& _mouse_overide_right_button =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0055B184 });

    inline uint16_t& _mouse_overide_left_button =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0055B188 });

    inline int16_t& _mouse_buffer_flag =
        *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00578DD8 });

    inline uint8_t* _mouse_list_none = reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00558D70 });
    inline uint8_t* _mouse_list_arrow = reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00558D10 });

    int16_t __cdecl Mouse_Buffer_();

    inline void __cdecl Mouse_Buffer2_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00527640 });
        fn();
    }

    /**
     * Returns the current mouse button state, allowing for button overrides.
     * Usually, the return value represents a bitmask or sum of button states:
     * 1 = Left, 2 = Right, 3 = Both.
     */
    uint16_t __cdecl Mouse_Button_();

    void __cdecl Set_Mouse_List_(uint8_t* mouse_data, uint16_t count);

    int32_t Pointer_X_();
    int32_t Pointer_Y_();

    inline int16_t __cdecl Get_Pointer_Offset_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{ 0x00526F30 })();
    }
}