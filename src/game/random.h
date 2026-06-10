#pragma once
#include <cstdint>

namespace game_random {
    extern uint32_t _random_seed;
    extern uint32_t _saved_random_seed;

    /** Returns the current random seed value.
     *  @param None.
     */
    uint32_t __cdecl Get_Random_Seed_();

    /** Saves the current random seed so it can be restored later.
     *  @param None.
     */
    void __cdecl Save_Random_Seed_();

    /** Sets the current random seed.
     *  @param seed New seed value to store.
     */
    uint32_t __cdecl Set_Random_Seed_(uint32_t seed);

    /** Seeds the generator from the current time value.
     *  @param None.
     */
    uint32_t __cdecl Randomize_();

    /** Returns a random value in the range 1 through the supplied maximum.
     *  @param max_val Inclusive upper bound for the generated value.
     */
    uint32_t __cdecl Random_(uint32_t max_val);

    /** Restores the previously saved random seed.
     *  @param None.
     */
    void __cdecl Restore_Random_Seed_();
}