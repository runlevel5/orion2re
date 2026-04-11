#pragma once

struct s_animation_header;
struct s_mp_screen;
struct s_player;
struct s_colors;

namespace MULTPLAY {
    void __cdecl Set_Default_Modem_Settings_();
    void __cdecl Copy_Modem_Defaults_To_Net_Struct_();

    void __cdecl Multi_Player_Screen_();
    void __cdecl Hotseat_Select_Player_();

    int16_t __cdecl Warning_Box_Has_Opponent_Pressed_();

    int8_t __cdecl Get_Valid_MP_Save_Games_(int8_t* valid_types_out);

    void __cdecl Draw_MP_Setup_Screen_();

    void __cdecl Load_MP_Setup_Pictures_();

    void __cdecl Add_MP_Setup_Screen_Fields_();
    void __cdecl Add_Modem_Setup_Fields2_();
    void __cdecl Load_Modem_Setup_Screen_();

    void __cdecl NullModem_Setup_Screen_();

    void __cdecl Load_NullModem_Setup_Screen_();

    void __cdecl Draw_NullModem_Setup_Screen_();

    void __cdecl Add_NullModem_Setup_Fields_();

    void __cdecl Modem_Setup_Screen_();

    void __cdecl Add_Modem_Setup_Fields_();

    void __cdecl Draw_Modem_Setup_Screen_();

    void __cdecl Print_Modem_Strings_();

    void __cdecl Change_MP_Game_Name_();

    int8_t __cdecl Add_Load_MP_Game_Screen_Fields_();

    bool __cdecl Check_Modem_Init_Help_Fields_(int16_t field_id, int8_t language);

    int16_t __cdecl Load_Multi_Player_Game_Screen_();
    void __cdecl Load_Load_MP_Game_Screen_();
    void __cdecl Draw_Load_MP_Game_Screen_();

    inline char (&_global_chat_string)[60] =
        *reinterpret_cast<char(*)[60]>(std::uintptr_t{0x00571BF8});

    inline s_mp_screen& _g_mp_screen = 
        *reinterpret_cast<s_mp_screen*>(std::uintptr_t{0x00571C58});

    inline char* const s_Evan_Axelrad = reinterpret_cast<char*>(std::uintptr_t{0x0055AA48});
    inline const char* s_Evan_Axelrad_0055aa48 = 
        reinterpret_cast<const char*>(std::uintptr_t{0x0055AA48}); // "Evan Axelrad"
    inline const char* s_signature_axelrad = s_Evan_Axelrad_0055aa48;

    inline char* const s_MULTIGM_LBX = reinterpret_cast<char*>(std::uintptr_t{0x00558D04});
    inline const char* s_MULTIGM_LBX_00558d04 = 
        reinterpret_cast<const char*>(std::uintptr_t{0x00558D04}); // "MULTIGM.LBX"
    inline const char* lbx_filename = s_MULTIGM_LBX_00558d04;

    inline const char** PTR_00558D3C =
        reinterpret_cast<const char**>(std::uintptr_t{0x00558D3C});

    inline const char* s_Moo_II_0055aa88 = 
        reinterpret_cast<const char*>(std::uintptr_t{0x0055AA88}); // "Moo II "

    inline uint32_t (&_green_pal_colors)[16] =
        *reinterpret_cast<uint32_t (*)[16]>(std::uintptr_t{0x0055A9A0});

    inline uint32_t (&_cur_pal_colors)[16] =
        *reinterpret_cast<uint32_t (*)[16]>(std::uintptr_t{0x00579590});

    inline char (&_baud_rate_strings)[8][6] =
        *reinterpret_cast<char (*)[8][6]>(std::uintptr_t{0x0055A9E0});

    inline const char* s_COM_PORT_FMT =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055AA9C}); // "COM:%d"

    inline const char* s_COM_PORT_FMT_v2 =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055AA94}); // "COM%d"

    inline const char* s_BAUD_RATE_FMT =
        reinterpret_cast<const char*>(std::uintptr_t{0x00558280}); // "%ld"

    int8_t __cdecl Do_Get_Valid_MP_Save_Games_(int8_t* valid_types_out);

    void __cdecl Load_Net_Next_Turn_Screen_(char fade_in);

    void __cdecl Get_Net_Next_Turn_Player_Colors_();

    void __cdecl Get_Main_Palette_Base_Colors_(int16_t player_idx, uint8_t* color_a, uint8_t* color_b, uint8_t* color_c);

    void __cdecl Draw_Fill_Seg_(int16_t idx);

    void __cdecl Draw_Multi_Player_Banner_(int16_t player_idx, int16_t row_idx, int8_t is_alt_layout);

    inline const char* s_name_fmt = reinterpret_cast<const char*>(0x0055AA28);
    inline const char* s_gnn_fmt = reinterpret_cast<const char*>(0x0055AA30);
    inline const char* s_chat_msg_fmt = reinterpret_cast<const char*>(0x0055AA3C);
    inline const char* s_fmt_player_race = reinterpret_cast<const char*>(0x005569C0);

    // Suggested name: MULTPLAY::Get_Player_Colors
    inline void __cdecl Get_Player_Colors_(uint8_t color, s_colors* c1, s_colors* c2, s_colors* c3) {
        using Fn = void(__cdecl*)(uint8_t, s_colors*, s_colors*, s_colors*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005039A0});
        fn(color, c1, c2, c3);
    }

    void __cdecl Draw_Net_Next_Turn_Screen_();

    void __cdecl Add_Net_Next_Turn_Fields_();

    void __cdecl Chat_Box_Input_Loop_();

    int16_t __cdecl Hotseat_Player_Status_(int16_t player_idx);

    void __cdecl Reload_Initializing_Net_Info_();

    void __cdecl Reload_Generic_Net_Info_(int16_t lbx_entry_num);

    inline uint8_t& _allow_chat_mode =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059538C});

    void __cdecl Reload_Generating_Map_Info_();

    void __cdecl Reload_Getting_Data_Info_();

    void __cdecl Draw_SendGet_Net_Info_Screen_();

    void __cdecl Draw_Generic_Net_Info_Screen_();

    inline void __cdecl thunk_Draw_Generic_Net_Info_Screen_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00502F10});
        fn();
    }

    void __cdecl Reload_Join_Net_Screen_();

    inline void __cdecl Reload_Waiting_For_Joiners_Screen_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005080a0});
        fn();
    }

    inline void __cdecl Add_Waiting_For_Joiners_Field_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00501900});
        fn();
    }

    inline void __cdecl Draw_Wait_For_New_Joiners_Screen_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00503560});
        fn();
    }

    inline void __cdecl Reload_Sending_Data_Info_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00508040});
        fn();
    }

    void __cdecl Draw_Choose_Multi_Net_Game_Screen_();

    void __cdecl Load_Choose_Multi_Net_Game_Screen_();

    inline const char* s__00551100 = reinterpret_cast<const char*>(std::uintptr_t{0x00551100});
    inline const char* s__00551120 = reinterpret_cast<const char*>(std::uintptr_t{0x00551120});
    inline const char* s_S_005569cc = reinterpret_cast<const char*>(0x005569CC);

    inline void __cdecl Add_Banner_Field_(int16_t player_idx, int16_t banner_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00502150});
        fn(player_idx, banner_idx);
    }

    void __cdecl Add_Choose_Multi_Net_Game_Fields_();

    int16_t __cdecl Choose_Multi_Network_Game_Screen_();

    void __cdecl Load_Choose_Net_Plyrs_Screen_();

    void __cdecl Add_Choose_Net_Plyrs_Fields_();

    void __cdecl Draw_Choose_Net_Plyrs_Screen_();

    int16_t __cdecl Choose_Network_Plyrs_Screen_(int16_t* out_selected_player, int8_t* connected_flags, int8_t is_host, int8_t should_load);

    inline void __cdecl Reload_Wait_For_Race_Info_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00508080});
        fn();
    }

    inline void __cdecl Draw_Wait_For_Race_Info_Screen_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00503740});
        fn();
    }
}