#pragma once

namespace MAINMENU {
 #pragma pack(push, 1)
    struct s_credit_y_slot {
        union {
            int32_t dword;
            struct {
                int16_t y;
                int16_t upper_word;
            };
        };
    };
 #pragma pack(pop)

    extern s_main_menu* _mainmenu;

    extern s_animation_header* _main_menu_background;

    extern s_animation_header* _mainmenu_first_seg;
    extern s_animation_header* _mainmenu_last_seg;
    extern void* __gametype;
    extern int16_t _max_credits;
    extern int32_t _credit_count;
    extern int16_t _credits_font_style;
    extern uint8_t _credit_num;
    extern uint8_t _credits_font_spacing;
    extern s_animation_header* _bitmap;
    extern s_credit_y_slot _credit_y_slot;
    extern int32_t _credit_x;
    extern int16_t _delay_count;
    extern int16_t _fade_in_count;
    extern int16_t _fade_out_count;
    extern char (*_credits)[2][127];

    extern const char s_MAINMENU_LBX_0055a630[13]; // "MAINMENU.LBX"
    extern const char s_C_00552420[2]; // "C"
    extern const char s_S_005569CC[2]; // "S"
    extern const char s_L_005560E8[2]; // "L"
    extern const char s_M_00557ED8[2]; // "M"
    extern const char s_H_005569D4[2]; // "H"
    extern const char s_Q_005598F0[2]; // "Q"
    extern const char s_empty_00551120[2]; // "\x1b"
    extern const char s_N_0055765C[2]; // "N"
    extern const char s_save_question_gam[10]; // "save?.gam"
    extern const char s_save_percent_d_gam[11]; // "save%d.gam"

    void __cdecl Main_Menu_Fade_Function_();
    void __cdecl Main_Menu_Fade_Function2_();
    void __cdecl Add_Main_Menu_Fields_();
    void __cdecl Reload_Main_Menu_Screen_();

    int16_t __cdecl Do_Main_Menu_File_Animation_();

    void __cdecl Fade_Credit_Colors_(int8_t amount, s_colors* colors);

    extern uint8_t _credits_column_one;

    void __cdecl Copy_Credit_String_To_Bitmap_(int16_t index, s_animation_header* bitmap);

    void __cdecl Draw_Credits_();

    void __cdecl Draw_Main_Menu_Screen_();

    void __cdecl Set_Main_Menu_Screen_Help_List_Aux_(s_help_box* help_data, int16_t count);
    void __cdecl Set_Main_Menu_Screen_Help_List_();

    int8_t __cdecl Do_Mainmenu_Load_Screen_();

    void __cdecl Load_Mainmenu_Load_Game_Popup_();

    int8_t __cdecl Mainmenu_Load_Game_Popup_(int16_t* save_game_list);

    void __cdecl Main_Menu_Screen_();

    int32_t __cdecl Get_File_Animation_Block_Space_();

    void __cdecl Init_Credits_(char* filename);

    void __cdecl Load_Credits_();
}