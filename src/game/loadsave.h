#pragma once
#include <cstdint>

namespace LOADSAVE {
    extern const char s_INVALID_STR[12];

    extern char _months[12][6];

    void __cdecl Load_Month_Names_();

    void __cdecl Allocate_Data_Space_For_Game_Popup_();

    void __cdecl Restore_Main_Screen_();

    void __cdecl Restore_Game_Popup_();

    int16_t __cdecl Game_Popup_User_Box_(char* message, int16_t type);

    void __cdecl Reload_Language_Strings_();

    extern uint8_t _draw_game_popup_to_back;
    extern uint8_t _dont_draw_darkened_buttons;
    extern uint8_t _draw_down;
    extern char _empty_save_slot_message[20];

    extern int16_t _music_level_field;
    extern int16_t _sfx_level_field;
    extern uint8_t _print_data_to_bitmap;
    extern struct s_animation_header* _bitmap;

    extern s_game_anims* _game_anims;

    extern s_game_popup_fields* _game_popup_fields;
    extern uint8_t _loaded_new_game;
    extern uint8_t _loaded_debug_save;

    int32_t __cdecl Check_Magic_Number_(const char* filename);

    bool __cdecl Get_Game_Type_From_Saved_Game_(const char* filename);

    bool __cdecl Is_Valid_Game_Type_(uint32_t game_type);

    void __cdecl Check_For_Saved_Games_(int8_t* list);

    void __cdecl Get_Game_Popup_Colors_(int16_t font_idx, s_colors* out_colors_primary, s_colors* out_colors_secondary);

    void __cdecl Add_Game_Popup_Fields_(int16_t *out_x, int16_t *out_y);

    bool __cdecl Active_Save_Slot_Invalid_();

    void __cdecl Set_Up_Load_Save_Popup_();

    int8_t __cdecl Do_Load_Game_Popup_(int16_t* exit_flag);

    void __cdecl Do_Main_Game_Popup_(int16_t* out_val);

    void __cdecl Do_Options_Game_Popup_(int16_t* out_val);

    void __cdecl Do_Save_Game_Popup_(int16_t* out_val);

    void __cdecl _Draw_Load_Save_Game_Popup_();

    void __cdecl _Draw_Main_Game_Popup_();

    void __cdecl _Draw_Main_Game_Popup_To_Bitmap_(struct s_animation_header* bitmap);

    void __cdecl _Draw_Load_Save_Game_Popup_To_Bitmap_(struct s_animation_header* bitmap);

    void __cdecl _Draw_Options_Game_Popup_To_Bitmap_(s_animation_header* bitmap);

    void __cdecl Animate_Game_Popup_(s_animation_header* header, int16_t y_offset, int16_t direction, uint16_t type);

    void __cdecl Animate_Game_Popup_Pic_Height_(s_animation_header* animation, uint16_t type);

    void __cdecl Save_Session_Related_Settings_();

    void __cdecl Bad_Game_File_Message_(uint8_t type, int16_t file_index);

    void __cdecl _Draw_Options_Game_Popup_();

    void __cdecl _Game_Popup_();

    void __cdecl Get_Star_Date_Strings_(int32_t* fields, char* output_buf);

    extern const char s_dot_GAM_005568f4[5];
    extern const char s_SAVE_005568fc[5];
    extern const char s_format_0055ad30[14];

    void __cdecl Auto_Save_Last_Saved_Game_(int16_t* flag);

    void __cdecl Auto_Load_Last_Saved_Game_(int16_t* flag);

    extern const char s__ALT_F1__0055ad24[9];
    extern const char s__ALT_F2__0055ad18[9];
    extern const char s__ALT_F3__0055ad0c[9];
    extern const char s__ALT_F5__0055ad00[9];
    extern const char s__ALT_F6__0055acf4[9];
    extern const char s__ALT_F7__0055ace8[9];
    extern const char s__ALT_F8__0055acdc[9];

    extern const char s__ALT_F1__0055acd0[9];
    extern const char s__ALT_F2__0055acc4[9];
    extern const char s__ALT_F3__0055acb8[9];
    extern const char s__ALT_F4__0055acac[9];
    extern const char s__ALT_F5__0055aca0[9];
    extern const char s__ALT_F6__0055ac94[9];
    extern const char s__ALT_F7__0055ac88[9];
    extern const char s__ALT_F8__0055ac7c[9];

    void __cdecl Get_Save_Game_Date_Strings_(char* output_buffer);

    void __cdecl Get_File_Date_String_(const char* file_path, char* out_string);

    void __cdecl Embed_Special_Color_Codes_(char* str, int16_t max_len);

    void __cdecl Remove_Embedded_Special_Codes_(char* text);

    void __cdecl Draw_Popup_Pic1_();
    void __cdecl Draw_Popup_Pic2_();

    void __cdecl Set_Music_For_Game_Popup_();

    void __cdecl Set_Sound_For_Game_Popup_(char play_sound_effect);

    void __cdecl Load_Game_Popup_Pictures_();

    void __cdecl Set_Current_Game_Option_Flags_();

    void __cdecl Animate_Game_Popup_Pic_(s_animation_header* animation_header);

    void __cdecl Update_Game_Settings_();
}