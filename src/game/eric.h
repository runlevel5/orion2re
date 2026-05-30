#pragma once
#include <cstdint>

struct s_animation_header;

namespace ERIC {
    extern uint8_t _fade_percent;

    extern uint8_t _fade_speed_in_percent;

    extern uint8_t _hacky_need_to_fade_to_colony_palette_flag;

    extern uint16_t _saved_sfx_l;

    extern uint16_t _saved_music_l;

    extern MOX2::action_func_t _cinematic_saved_autofunction;

    void __cdecl Eric_Init_();

    void __cdecl Eric_Defaults_();

    int16_t __cdecl Get_Sint_Index_From_List_(int16_t value, const int16_t* list, uint16_t count);

    uint16_t __cdecl Get_Fade_Percent_();

    void __cdecl Fast_Fade_In_();

    void __cdecl Fast_Fade_Out_();

    bool __cdecl Fast_Fade_Out_Needed_For_Colony_Palette_();

    void __cdecl Set_Fade_Speed_Percent_(uint8_t percentage);

    void __cdecl Set_Fade_Percent_(int16_t percent);

    void __cdecl Print_Paragraph_Centered_(int16_t x, int16_t y, int16_t width, int16_t height, const char* text);

    void __cdecl Print_Paragraph_Centered_Vertically_(int16_t x, int16_t y, int16_t width, uint16_t box_height, char* text, int16_t color);

    int16_t __cdecl Global_Cycler_(int16_t divisor, int16_t modulus);

    void __cdecl Draw_Centered_(int16_t x, int16_t y, s_animation_header* anim);

    void __cdecl Draw_Bottom_Centered_(int16_t x, int16_t y, s_animation_header* animation);

    void __cdecl Draw_Bottom_Centered_No_Glass_(int16_t x, int16_t y, s_animation_header* anim);

    void __cdecl String_Builder2_(char* dest, int16_t mode, const char* format, ...);

    extern const char s_2_00551108[3];

    const char* __cdecl Red_If_Negative_Fmt_String_(int16_t value);

    void __cdecl Pulsing_Box_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t param_5, int32_t param_6, int16_t param_7);

    void __cdecl Play_Loose_Flic_();

    void __cdecl Play_Normal_Win_Flic_();

    void __cdecl Play_All_Flics_();

    void __cdecl Play_Take_Orion_Flic_();

    void __cdecl Cinematic_Autofunction_();

    void __cdecl Do_Cinematic_Popup_Setup_();

    void __cdecl Do_Cinematic_Popup_Cleanup_();

    void __cdecl Set_Refresh_Mode_(int16_t mode);

    void __cdecl Play_Dimensional_Portal_Flic_();

    void __cdecl Play_Planetary_Destruction_Flic_();

    void __cdecl Play_Antarans_Attack_Flic_();

    bool __cdecl Sint_Is_In_List_(int16_t value, int16_t* list, uint16_t count);

    void __cdecl Play_Diplomatic_Win_Flic_();

    void __cdecl Play_Amoeba_Flic_();

    int8_t __cdecl Player_Has_Ship_Size_Or_Larger_At_Star_(int16_t player_idx, int16_t size, int16_t star_idx);

    int8_t __cdecl Player_Has_Leader_With_General_Skill_At_Star_(short player_idx, short skill_idx, short star_idx);

    int8_t __cdecl Player_Can_Mind_Control_Colony_(int16_t player_idx, int16_t colony_idx);

    int8_t __cdecl Player_Can_Mind_Control_Colony_With_Help_(int16_t player_idx, int16_t colony_idx);

    void __cdecl Force_Fade_To_Colony_Palette_(uint8_t flag);

    int16_t __cdecl Player_Capitol_World_(int16_t player_idx);

    int16_t __cdecl Pick_Random_Colony_(int16_t owner);
}