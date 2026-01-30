#pragma once

namespace MISC {
    inline uint8_t& _mouse_clock_image_num =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00566578});

    inline uint8_t* _mouse_list_clock =
        reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558250});

    void __cdecl Increment_Mouse_Clock_Image_();

#pragma pack(push, 1)
    struct s_colors {
        uint8_t colors[8];
    };
#pragma pack(pop)

    inline s_colors* __cdecl Font_Colors2_(uint16_t a1, uint8_t a2, uint8_t a3) {
        using Fn = s_colors*(__cdecl*)(int16_t, uint8_t, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049ddb0});
        return fn(a1, a2, a3);
    }

    inline void __cdecl Draw_Advancing_Fill_Message_(int8_t a1, void* ctr, uint16_t max, s_colors* c1, s_colors* c2, int16_t x, int16_t y, const char* msg, void* bitmap, uint8_t flag) {
        using Fn = void(__cdecl*)(int8_t, void*, uint16_t, s_colors*, s_colors*, int32_t, int32_t, const char*, void*, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049e3f0});
        fn(a1, ctr, max, c1, c2, x, y, msg, bitmap, flag);
    }

    inline void __cdecl Save_Window_Status_(int16_t* status) {
        using Fn = void(__cdecl*)(int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049EDC0});
        fn(status);
    }

    inline void __cdecl Restore_Window_Status_(int16_t status) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049EDA0});
        fn(status);
    }
}