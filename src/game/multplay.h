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

    inline s_mp_screen& _g_mp_screen = 
        *reinterpret_cast<s_mp_screen*>(std::uintptr_t{0x00571C58});

    inline char* const s_Evan_Axelrad = reinterpret_cast<char*>(std::uintptr_t{0x0055AA48});
    inline const char* s_Evan_Axelrad_0055aa48 = 
        reinterpret_cast<const char*>(std::uintptr_t{0x0055AA48}); // "Evan Axelrad"

    inline char* const s_MULTIGM_LBX = reinterpret_cast<char*>(std::uintptr_t{0x00558D04});
    inline const char* s_MULTIGM_LBX_00558d04 = 
        reinterpret_cast<const char*>(std::uintptr_t{0x00558D04}); // "MULTIGM.LBX"

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

    inline void __cdecl Load_Net_Next_Turn_Screen_(int8_t param) {
        using Fn = void(__cdecl*)(int8_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00504640})(param);
    }

    inline void __cdecl Chat_Box_Input_Loop_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x005080f0})();
    }

    inline uint32_t __cdecl Hotseat_Player_Status_(int16_t player_idx) {
        using Fn = uint32_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005045f0});
        return fn(player_idx);
    }
}