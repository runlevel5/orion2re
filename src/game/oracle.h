#pragma once

#include <cstdint>

namespace oracle {
    // Parses oracle replay options and enables replay mode.
    bool Configure_(int argc, char** argv);

    // Verifies one locally generated random value against the active oracle turn.
    void Verify_Random_(uint32_t max_value, uint32_t value, uint32_t random_seed,
        const uintptr_t* return_addresses, uint32_t return_address_count,
        uintptr_t image_base);

    // Replaces an explicit local RNG seed with the seed captured from the original game.
    uint32_t Replay_Random_Seed_(uint32_t local_seed,
        const uintptr_t* return_addresses, uint32_t return_address_count,
        uintptr_t image_base);

    // Starts one tactical-combat checkpoint sequence.
    void Begin_Tactical_Combat_();

    // Verifies the first Do_Combat_Turn_ call for each tactical turn.
    void Verify_Tactical_Turn_(int16_t active_player_id,
        int16_t time_warp_only);

    // Verifies tactical state immediately before End_Of_Combat_ changes it.
    void Verify_Tactical_Combat_End_();

    // Loads the requested turn and prepares replay after normal game initialization.
    bool Start_();

    // Injects the next due logical input event without compressing overdue event spacing.
    void Pump_Input_();

    // Saves and verifies the state produced by the completed turn.
    void Complete_Turn_();

    // Physical keyboard and mouse input is ignored while oracle mode is enabled.
    bool Physical_Input_Enabled_();

    // Completes a recorded stationary click that has already delivered its press.
    bool Complete_Replay_Click_();
}