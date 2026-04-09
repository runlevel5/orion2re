#pragma once

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

    inline void __cdecl Get_Main_Palette_Base_Colors_(int16_t player_num, uint8_t* color_a, uint8_t* color_b, uint8_t* color_c) {
        using Fn = void(__cdecl*)(int16_t, uint8_t*, uint8_t*, uint8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00503920});
        fn(player_num, color_a, color_b, color_c);
    }

    inline void __cdecl Draw_Net_Next_Turn_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00502210})();
    }

    void __cdecl Add_Net_Next_Turn_Fields_();

    void __cdecl Chat_Box_Input_Loop_();

    int16_t __cdecl Hotseat_Player_Status_(int16_t player_idx);

    void __cdecl Reload_Initializing_Net_Info_();

    inline void __cdecl Reload_Generic_Net_Info_(int32_t screen_id) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00507EA0});
        fn(screen_id);
    }

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

    inline void __cdecl Draw_Choose_Multi_Net_Game_Screen_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00502F20});
        fn();
    }

    inline void __cdecl Load_Choose_Multi_Net_Game_Screen_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00504B40});
        fn();
    }

    inline void __cdecl Add_Choose_Multi_Net_Game_Fields_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00501B30});
        fn();
    }

    void __cdecl Choose_Multi_Network_Game_Screen_();

    inline int16_t __cdecl Choose_Network_Plyrs_Screen_(int16_t* selection, bool* connected_flags, bool flag1, bool flag2) {
        using Fn = int16_t(__cdecl*)(int16_t*, bool*, bool, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00501c80});
        return fn(selection, connected_flags, flag1, flag2);
    }
}