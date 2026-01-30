#pragma once
#include <cstdint>

namespace animate {
#pragma pack(push, 1)
    struct s_animation_header {
        int16_t width;
        int16_t height;
        int16_t current_frame;
        int16_t frame_count;
        uint8_t loop_frame;
        uint8_t field5_0x9;
        uint8_t field6_0xa;
        uint8_t flags;
        uint32_t frame_offsets[1]; // Dynamic length array
    };
#pragma pack(pop)

    inline s_animation_header*& _animation_header =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{ 0x0057632C });

    inline int16_t& _clipping_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0055B0E0 });
    inline int16_t& _dot_stencil_flag = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0055B0E4 });

    inline const char* s_Screen_Background_cant_be_clipp = reinterpret_cast<const char*>(0x0055B0E8);
    inline const char* s_Refresh_Mask_cant_be_drawn = reinterpret_cast<const char*>(0x0055B10C);

    void __cdecl Set_Animation_Frame_(s_animation_header* anim, int32_t frame);
    void __cdecl Reset_Animation_Frame_(s_animation_header* anim);
    int16_t __cdecl Get_Animation_Frame_Count_(s_animation_header* seg);
    int16_t __cdecl Get_Current_Frame_(s_animation_header* animation);

    inline int16_t __cdecl Get_Width_(s_animation_header* anim) {
        using Fn = int16_t(__cdecl*)(s_animation_header*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00524d20})(anim);
    }

    inline int16_t __cdecl Get_Height_(s_animation_header* anim) {
        using Fn = int16_t(__cdecl*)(s_animation_header*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00524d30})(anim);
    }

    void __cdecl Draw_(int16_t x, int16_t y, s_animation_header* resource);

    inline void __cdecl Set_Animation_Palette_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x00524930)();
    }

    inline void __cdecl Draw_Glassed_Animated_Sprite_(int16_t x, int16_t y, uint8_t* data) {
        using Fn = void(__cdecl*)(int16_t, int16_t, uint8_t*);
        reinterpret_cast<Fn>(0x00524830)(x, y, data);
    }

    inline void __cdecl Draw_Glassed_Bitmap_Sprite_(int16_t x, int16_t y, uint8_t* data) {
        using Fn = void(__cdecl*)(int16_t, int16_t, uint8_t*);
        reinterpret_cast<Fn>(0x00524990)(x, y, data);
    }

    inline void __cdecl Draw_Screen_Background_On_(uint8_t* data) {
        using Fn = void(__cdecl*)(uint8_t*);
        reinterpret_cast<Fn>(0x00524AE0)(data);
    }

    inline void __cdecl Draw_Glassed_Screen_Background_On_(uint8_t* data) {
        using Fn = void(__cdecl*)(uint8_t*);
        reinterpret_cast<Fn>(0x00524C30)(data);
    }

    inline void __cdecl Draw_Glassed_Screen_Background_Off_(uint8_t* data) {
        using Fn = void(__cdecl*)(uint8_t*);
        reinterpret_cast<Fn>(0x00524BB0)(data);
    }

    inline void __cdecl Draw_Screen_Background_Off_(uint8_t* data) {
        using Fn = void(__cdecl*)(uint8_t*);
        reinterpret_cast<Fn>(0x00524A50)(data);
    }
}