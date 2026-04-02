#pragma once
#include <cstdint>

namespace RUSS {
    inline auto PFMT_ = reinterpret_cast<void(__cdecl*)(int32_t, int32_t, const char*, ...)>(std::uintptr_t{0x004D7850});
    inline int8_t& _all_ai_game = *reinterpret_cast<int8_t*>(std::uintptr_t{ 0x005960E9 });

    int16_t __cdecl Get_Weighted_Choice_Char_(int8_t* weights, int16_t count);

    int16_t __cdecl Get_Weighted_Choice_Short_(const int16_t* weights, int16_t count);

    inline int16_t __cdecl Get_Weighted_Choice_Int_(uint32_t* weights, int16_t num_weights) {
        using Fn = int16_t(__cdecl*)(uint32_t*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004d7950});
        return fn(weights, num_weights);
    }

    inline char* __cdecl Star_Name_(int32_t star_idx) {
        using Fn = char*(__cdecl*)(int32_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004d7c30})(star_idx);
    }

    inline void __cdecl Mox_Sync_Update_() {
        using Fn = void(__cdecl*)();
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

    inline void __cdecl Shuffle_Char_(int8_t* array, int16_t count) {
        using Fn = void(__cdecl*)(int8_t*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004d79b0});
        fn(array, count);
    }

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

    inline bool __cdecl Test_Bit_Field_(uint8_t* bit_field, int32_t bit_idx) {
        using Fn = bool(__cdecl*)(uint8_t*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052CFB5});
        return fn(bit_field, bit_idx);
    }

    inline int32_t __cdecl Compare_Ratios_(uint32_t a1, uint32_t a2, uint32_t b1, uint32_t b2) {
        using Fn = int32_t(__cdecl*)(uint32_t, uint32_t, uint32_t, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004D7C60});
        return fn(a1, a2, b1, b2);
    }

    void __cdecl Load_Russ_Strings_();

    inline uint8_t& _russ_strings = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056CF08});

    inline const char* s_Seed_ld = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575BC }); // "Seed: %ld"
    inline const char* s_Map_d = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575C8 });   // "Map: %d"

    inline const char* s_This_isnt_a_single_player_game =
        reinterpret_cast<const char*>(std::uintptr_t{0x00559A08}); // "This isn't a single player game, cheeky monkey!"
}