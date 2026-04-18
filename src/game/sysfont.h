#pragma once

#include <cstdint>

struct s_mouse_list;

namespace sysfont {
    inline uint32_t* _thin_column_lookup =
        reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C288});
    inline uint32_t* _font_pixel_data =
        reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C2E8});
    inline uint8_t* _default_mouse_palette =
        reinterpret_cast<uint8_t*>(std::uintptr_t{0x0058AA70});

    inline uint32_t& _mouse_cursor_color = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055C230});
    inline s_mouse_list& _default_system_mouse_list = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x0055C2D8});
    inline uint16_t* _default_mouse_image = reinterpret_cast<uint16_t*>(std::uintptr_t{0x005602E8});
    inline uint8_t* _default_palette_colors = reinterpret_cast<uint8_t*>(std::uintptr_t{0x00560768});

    void __cdecl Load_System_Mouse_();
    void __cdecl Load_System_Font_();
}