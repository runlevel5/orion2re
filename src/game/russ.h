#pragma once
#include <cstdint>

namespace RUSS {
    inline auto PFMT_ = reinterpret_cast<void(__cdecl*)(int32_t, int32_t, const char*, ...)>(std::uintptr_t{0x004D7850});
    inline int8_t& _all_ai_game = *reinterpret_cast<int8_t*>(std::uintptr_t{ 0x005960E9 });

    int16_t __cdecl Get_Weighted_Choice_Char_(uint8_t* weights, int16_t count);

    int16_t __cdecl Get_Weighted_Choice_Short_(const uint16_t* weights, int16_t count);

    int16_t __cdecl Get_Weighted_Choice_Int_(uint32_t* weights, int16_t count);

    char* __cdecl Star_Name_(int32_t star_idx);

    void __cdecl Mox_Sync_Update_();

    void __cdecl Mox_Update_();

    void __cdecl Assert_Settings_();

    void __cdecl Shuffle_Char_(int8_t* buffer, int16_t length);

    void __cdecl Shuffle_Sint_(int16_t* data, int16_t count);

    void __cdecl Shuffle_Int_(int32_t* array, int16_t count);

    void __cdecl Shuffle_(void* buffer, int16_t element_size, int16_t num_elements);

    void __cdecl Release_All_Blocks_(void* seg);

    void __cdecl Make_All_AI_Game_();

    inline void __cdecl Make_Player_Into_AI_(int32_t player_idx) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004D7DC0});
        fn(player_idx);
    }

    int __cdecl Compare_Ratios_(uint32_t first_numerator, uint32_t first_denominator, uint32_t second_numerator, uint32_t second_denominator);

    void __cdecl Load_Russ_Strings_();

    inline uint8_t& _russ_strings = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056CF08});

    inline const char* s_Seed_ld = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575BC }); // "Seed: %ld"
    inline const char* s_Map_d = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575C8 });   // "Map: %d"

    inline const char* s_This_isnt_a_single_player_game =
        reinterpret_cast<const char*>(std::uintptr_t{0x00559A08}); // "This isn't a single player game, cheeky monkey!"
}