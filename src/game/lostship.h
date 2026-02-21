#pragma once

namespace lostship {
    void __cdecl Lose_Out_Of_Range_Ships_();

    void __cdecl Compute_Stars_In_Range_Of_Player_(int8_t* reserve_range_array, int8_t* fuel_range_array, int16_t player_idx);

    void __cdecl Player_Lose_Out_Of_Range_Ships_(int8_t* reserve_range_array, int8_t* fuel_range_array, int16_t player_id);
}