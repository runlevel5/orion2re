#pragma once

namespace CMBTDRW1 {
    inline const char* s_combat_lbx = reinterpret_cast<const char*>(0x00551244);
    inline void*& _k_save = *reinterpret_cast<void**>(std::uintptr_t{0x005960FC});

    inline const char* s_B_005560e4 = reinterpret_cast<const char*>(std::uintptr_t{0x005560E4}); // "B"
    inline const char* s_L_005560e8 = reinterpret_cast<const char*>(std::uintptr_t{0x005560E8}); // "L"
    inline const char* s_S_005569cc = reinterpret_cast<const char*>(std::uintptr_t{0x005569CC}); // "S"
    inline const char* s_D_005569dc = reinterpret_cast<const char*>(std::uintptr_t{0x005569DC}); // "D"
    inline const char* s_A_00557ed4 = reinterpret_cast<const char*>(std::uintptr_t{0x00557ED4}); // "A"
    inline const char* s_I_0055801c = reinterpret_cast<const char*>(std::uintptr_t{0x0055801C}); // "I"
    inline const char* s_R_0055824c = reinterpret_cast<const char*>(std::uintptr_t{0x0055824C}); // "R"
    inline const char* s_F_005598f4 = reinterpret_cast<const char*>(std::uintptr_t{0x005598F4}); // "F"

    void __cdecl Detailed_View_Ship_(int16_t ship_idx);

    void __cdecl Draw_Internals_Display_(int16_t x_off, int16_t y_off);
    void __cdecl Full_Draw_Combat_Screen_();
    void __cdecl Draw_Legal_Boxes_();
    inline void __cdecl Draw_Ship_Status_(int16_t y, int16_t x, int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004ce740});
        fn(y, x, ship_idx);
    }
    void __cdecl Draw_Grey_Damage_Bars_(int16_t x, int16_t y, int16_t ship_idx);
    void __cdecl Draw_Crew_Level_Stars_(int16_t ship_idx, int16_t x, int16_t y);
    void __cdecl Draw_Reduced_Combat_Map_();

    void __cdecl Draw_Reduced_Map_Box_();

    void __cdecl Draw_Fake_Combat_Buttons_();

    void __cdecl Draw_Option_Panel_Lights_();

    void __cdecl Draw_Blip2_(int16_t x, int16_t y, int16_t intensity_a, int16_t intensity_b);

    void __cdecl Draw_Blip3_(int16_t x, int16_t y, int16_t p3, int16_t p4);

    void __cdecl Draw_Blip4_(int16_t x, int16_t y, int16_t base_color, int16_t color_offset);

    void __cdecl Draw_Blip5_(int16_t x, int16_t y, int16_t color_base_a, int16_t color_base_b);

    void __cdecl Draw_Shield_Arcs_();

    inline void __cdecl Draw_Weapon_Status_Display_(int16_t param_1, int16_t param_2) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(0x004cf810)(param_1, param_2);
    }

    void __cdecl Draw_Special_Status_Display_();

    void __cdecl Draw_Main_Combat_Screen_();

    void __cdecl Refresh_Combat_Screen_Full_();

    void __cdecl Draw_Super_Fast_Screen_();

    void __cdecl Move_Options_Background_();

    void __cdecl Display_Combat_View_Ship_(int16_t ship_idx);

    void __cdecl Scroll_Draw_Combat_Screen_();

    void __cdecl Create_Ship_Darken_Map_();

    int16_t __cdecl Ship_Completely_On_Screen_(int16_t ship_idx);

    int16_t __cdecl Relative_Bearing_XY_(int16_t ship_idx, int16_t target_x, int16_t target_y);

    int16_t __cdecl Relative_Bearing_(int16_t ship_a_idx, int16_t ship_b_idx);

    int16_t __cdecl Ship_On_Screen_(int16_t ship_idx);

    void __cdecl Draw_Ship_(int16_t param_1, int16_t param_2, int16_t param_3);

    void __cdecl Get_Draw_Ship_Offsets_(int16_t ship_idx, int16_t* out_x, int16_t* out_y);

    void __cdecl Draw_Ship_To_Bitmap_(int16_t ship_idx, int16_t param_2, int16_t param_3, uint16_t param_4);

    void __cdecl Draw_Retreating_Ship_(int16_t ship_idx);

    void __cdecl Get_Ship_Center_Draw_XY_(int16_t combat_idx, int16_t* x_out, int16_t* y_out);

    void __cdecl Get_Ship_Actual_Size_(int16_t ship_id, int16_t* width, int16_t* height);

    uint8_t __cdecl Get_Weapon_Arc_(int16_t bearing);

    inline int16_t __cdecl Board_Popup_(int16_t unknown_0, int16_t boarding_strength, char* message, int16_t* param_4) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, char*, int16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004d7120});
        return fn(unknown_0, boarding_strength, message, param_4);
    }

    inline void __cdecl Plasma_Darken_(s_animation_header* anim, int16_t val, int16_t color) {
        using Fn = void(__cdecl*)(s_animation_header*, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004d5da0 });
        fn(anim, val, color);
    }
}