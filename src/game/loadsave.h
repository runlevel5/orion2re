#pragma once
#include <cstdint>

namespace LOADSAVE {
    inline const char* s_INVALID_STR = reinterpret_cast<const char*>(0x0055AC58);

    inline char (&_months)[12][6] = *reinterpret_cast<char(*)[12][6]>(std::uintptr_t{0x005722F8});

    void __cdecl Load_Month_Names_();

    void __cdecl Allocate_Data_Space_For_Game_Popup_();

    void __cdecl Restore_Main_Screen_();

    void __cdecl Restore_Game_Popup_();

    int16_t __cdecl Game_Popup_User_Box_(char* message, int16_t type);

    inline uint8_t& _draw_game_popup_to_back =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005722F4});
    inline uint8_t& _dont_draw_darkened_buttons = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005722C4});
    inline uint8_t& _draw_down = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055AC50});
    inline char (&_empty_save_slot_message)[20] = *reinterpret_cast<char(*)[20]>(std::uintptr_t{0x005722D8});

    inline int16_t& _music_level_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00572344});
    inline int16_t& _sfx_level_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005722B8});
    inline uint8_t& _print_data_to_bitmap = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005722EC});
    inline struct s_animation_header*& _bitmap = *reinterpret_cast<struct s_animation_header**>(std::uintptr_t{0x005722F0});

    inline s_game_anims*& _game_anims = *reinterpret_cast<s_game_anims**>(std::uintptr_t{0x00572354});

    inline s_game_popup_fields*& _game_popup_fields = *reinterpret_cast<s_game_popup_fields**>(std::uintptr_t{0x0059537c});
    inline uint8_t& _loaded_new_game = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00572340});
    inline uint8_t& _loaded_debug_save = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00572350});

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

    void __cdecl Bad_Game_File_Message_(int16_t type, int16_t file_index);

    void __cdecl _Draw_Options_Game_Popup_();

    void __cdecl _Game_Popup_();

    void __cdecl Get_Star_Date_Strings_(int32_t* fields, char* output_buf);

    inline const char* const s_dot_GAM_005568f4 = reinterpret_cast<const char*>(0x005568F4);
    inline const char* const s_SAVE_005568fc = reinterpret_cast<const char*>(0x005568FC);
    inline const char* s_format_0055ad30 = reinterpret_cast<const char*>(0x0055AD30); // "%s, %s, %d %s"

    void __cdecl Auto_Save_Last_Saved_Game_(int16_t* flag);

    inline void __cdecl Auto_Load_Last_Saved_Game_(int16_t* flag) {
        using Fn = void(__cdecl*)(int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x00512320})(flag);
    }

    inline const char* s__ALT_F1__0055ad24 = reinterpret_cast<const char*>(0x0055AD24); // "(ALT F1)"
    inline const char* s__ALT_F2__0055ad18 = reinterpret_cast<const char*>(0x0055AD18); // "(ALT F2)"
    inline const char* s__ALT_F3__0055ad0c = reinterpret_cast<const char*>(0x0055AD0C); // "(ALT F3)"
    inline const char* s__ALT_F5__0055ad00 = reinterpret_cast<const char*>(0x0055AD00); // "(ALT F5)"
    inline const char* s__ALT_F6__0055acf4 = reinterpret_cast<const char*>(0x0055ACF4); // "(ALT F6)"
    inline const char* s__ALT_F7__0055ace8 = reinterpret_cast<const char*>(0x0055ACE8); // "(ALT F7)"
    inline const char* s__ALT_F8__0055addc = reinterpret_cast<const char*>(0x0055ACDC); // "(ALT F8)"

    inline const char* s__ALT_F1__0055acd0 = reinterpret_cast<const char*>(0x0055ACD0); // "(ALT-F1)"
    inline const char* s__ALT_F2__0055acc4 = reinterpret_cast<const char*>(0x0055ACC4); // "(ALT-F2)"
    inline const char* s__ALT_F3__0055acb8 = reinterpret_cast<const char*>(0x0055ACB8); // "(ALT-F3)"
    inline const char* s__ALT_F4__0055acac = reinterpret_cast<const char*>(0x0055ACAC); // "(ALT-F4)"
    inline const char* s__ALT_F5__0055aca0 = reinterpret_cast<const char*>(0x0055ACA0); // "(ALT-F5)"
    inline const char* s__ALT_F6__0055ac94 = reinterpret_cast<const char*>(0x0055AC94); // "(ALT-F6)"
    inline const char* s__ALT_F7__0055ac88 = reinterpret_cast<const char*>(0x0055AC88); // "(ALT-F7)"
    inline const char* s__ALT_F8__0055ac7c = reinterpret_cast<const char*>(0x0055AC7C); // "(ALT-F8)"

    void __cdecl Get_Save_Game_Date_Strings_(char* output_buffer);

    void __cdecl Embed_Special_Color_Codes_(char* str, int16_t max_len);

    void __cdecl Remove_Embedded_Special_Codes_(char* text, int max_len = 37);

    void __cdecl Draw_Popup_Pic1_();
    void __cdecl Draw_Popup_Pic2_();

    void __cdecl Set_Music_For_Game_Popup_();

    void __cdecl Set_Sound_For_Game_Popup_(char play_sound_effect);

    void __cdecl Load_Game_Popup_Pictures_();

    void __cdecl Set_Current_Game_Option_Flags_();

    void __cdecl Animate_Game_Popup_Pic_(s_animation_header* animation_header);

    void __cdecl Update_Game_Settings_();
}