#pragma once
#include <cstdint>

struct s_colors;

namespace GENDRAW {
    void __cdecl Message_Box_(const char* text);

    void __cdecl Help_(const char* message);

    int16_t __cdecl Confirmation_Box_(char* text);

    void __cdecl Warning_Box_(const char* message);

    inline s_colors& _message_text_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00557640});
    inline uint8_t& _warning_box = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00566068});
    inline int16_t& _message_box_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056603C});
    inline uint8_t*& _box_bitmap = *reinterpret_cast<uint8_t**>(std::uintptr_t{0x00566040});
    inline int16_t& _animation_frame = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056606C});
    inline int16_t& _direction = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566074});
    inline int16_t& _first_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566064});
    inline uint8_t* (&_image_seg)[3] = *reinterpret_cast<uint8_t*(*)[3]>(std::uintptr_t{0x00566078});

    inline void __cdecl Draw_Confirm_Box_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00479c30})();
    }

    inline void __cdecl Message_Box_Startup_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00479cc0})();
    }

    inline void __cdecl Do_Print_1_Or_More_Formatted_Lines_Centered_(int x, int y, int width, char* str) {
        using Fn = void(__cdecl*)(int, int, int, char*);
        reinterpret_cast<Fn>(std::uintptr_t{0x00479e60})(x, y, width, str);
    }

    void __cdecl Draw_Animating_Message_Box_();

    inline void __cdecl Message_Box_Cleanup_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00479d70})();
    }

    void __cdecl Message_Box_Exploding_(char* text);

    void __cdecl Message_Box_Exploding_Star_(const char* text, int16_t star_idx);
}