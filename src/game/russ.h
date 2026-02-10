#pragma once
#include <cstdint>

namespace RUSS {
    inline auto PFMT_ = reinterpret_cast<void(__cdecl*)(int32_t, int32_t, const char*, ...)>(std::uintptr_t{0x004D7850});

    int16_t __cdecl Get_Weighted_Choice_Char_(int8_t* weights, int16_t count);

    int16_t __cdecl Get_Weighted_Choice_Short_(const int16_t* weights, int16_t count);

    inline void Mox_Sync_Update_() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00483C80 });
        fn();
    }

    inline void __cdecl Mox_Update_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004d7820 });
        fn();
    }

    inline void __cdecl Assert_Settings_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004d7810 });
        fn();
    }

    void __cdecl Release_All_Blocks_(void* seg);

    void __cdecl Make_All_AI_Game_();

    inline void __cdecl Make_Player_Into_AI_(int32_t player_idx) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004D7DC0});
        fn(player_idx);
    }

    void __cdecl Load_Russ_Strings_();

    inline uint8_t& _russ_strings = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056CF08});

    inline const char* s_Seed_ld = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575BC }); // "Seed: %ld"
    inline const char* s_Map_d = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575C8 });   // "Map: %d"

    inline uint8_t& _all_ai_game =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0059533F});

    inline const char* s_This_isnt_a_single_player_game =
        reinterpret_cast<const char*>(std::uintptr_t{0x00559A08}); // "This isn't a single player game, cheeky monkey!"
}