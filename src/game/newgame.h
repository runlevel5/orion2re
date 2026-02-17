#pragma once
#include <cstdint>

namespace NEWGAME {
    inline int16_t& _g_x = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00557DA8});
    inline int16_t& _g_y = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00557DAC});
    inline s_colors& _color_ken = *reinterpret_cast<s_colors*>(std::uintptr_t{0x005A6260});
    inline uint16_t& _opp_button_variable = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00566300});
    inline int16_t& _diff_button_variable = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566368});
    inline uint8_t& _xenon_attacks_button_variable = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056636C});
    inline int16_t& _civ_button_variable = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566384});
    inline uint16_t& _combat_button_variable = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005663A4});
    inline uint16_t& _gal_size_button_variable = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005663A8});
    inline uint8_t& _random_events_button_variable = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005663C4});
    inline uint16_t& _gal_age_button_variable = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005663C8});
    inline int16_t* _fields = reinterpret_cast<int16_t*>(std::uintptr_t{0x005663B0});
    inline char* DAT_0059532e = reinterpret_cast<char*>(std::uintptr_t{0x0059532E});
    inline s_animation_header** _anims = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00566330});
    inline s_animation_header** _tech_level_anims = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A61F0});
    inline s_animation_header** _difficulty_anims = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A6200});
    inline s_animation_header** _num_players_anims = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A6220});
    inline s_animation_header** _gal_age_anims = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A6240});
    inline s_animation_header** _gal_size_anims = reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005A6250});
    inline void*& _background_seg = *reinterpret_cast<void**>(std::uintptr_t{0x005A6268});
    inline char* (&_gal_size_button_strings)[4] = *reinterpret_cast<char*(*)[4]>(std::uintptr_t{0x005662F0});
    inline char* (&_civ_button_strings)[3] = *reinterpret_cast<char*(*)[3]>(std::uintptr_t{0x00566308});
    inline char* (&_diff_button_strings)[5] = *reinterpret_cast<char*(*)[5]>(std::uintptr_t{0x00566318});
    inline char* (&_gal_age_button_strings)[3] = *reinterpret_cast<char*(*)[3]>(std::uintptr_t{0x00566350});
    inline char* (&_combat_button_strings)[2] = *reinterpret_cast<char*(*)[2]>(std::uintptr_t{0x00566360});
    inline char& _hack_msg = *reinterpret_cast<char*>(std::uintptr_t{0x00566370});
    inline char* (&_opp_button_strings)[7] = *reinterpret_cast<char*(*)[7]>(std::uintptr_t{0x00566388});

    inline const char* s_Media_00557db4 = reinterpret_cast<const char*>(std::uintptr_t{0x00557DB4}); // "Media"
    inline const char* s_sen_00557dbc = reinterpret_cast<const char*>(std::uintptr_t{0x00557DBC}); // "%sen"
    inline const char* s_Piccola_00557dc4 = reinterpret_cast<const char*>(std::uintptr_t{0x00557DC4}); // "Piccola"

    void __cdecl Load_New_Game_Strings_();

    void __cdecl Reload_Newgame_Screen_();

    void __cdecl Globals_To_Temps_();

    void __cdecl Update_Fields_();

    void __cdecl Draw_Pictures_();

    void __cdecl Draw_Newgame_Screen_();

    void __cdecl Temps_To_Globals_();

    inline int16_t __cdecl Race_Selection_Screen_(s_player* player) {
        using Fn = int16_t(__cdecl*)(s_player*);
        return reinterpret_cast<Fn>(0x00498B40)(player);
    }

    int16_t __cdecl Newgame_Screen_();
}