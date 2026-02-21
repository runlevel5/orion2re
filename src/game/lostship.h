#pragma once

namespace lostship {
    void __cdecl Lose_Out_Of_Range_Ships_();

    inline void __cdecl Compute_Stars_In_Range_Of_Player_(int8_t* reserve_range_array, int8_t* fuel_range_array, int16_t player_idx) {
        using Fn = void(__cdecl*)(int8_t*, int8_t*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00470960});
        fn(reserve_range_array, fuel_range_array, player_idx);
    }

    void __cdecl Player_Lose_Out_Of_Range_Ships_(int8_t* reserve_range_array, int8_t* fuel_range_array, int16_t player_id);
}