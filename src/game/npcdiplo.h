#pragma once

#include <cstdint>

namespace npcdiplo {
    void __cdecl Clear_Last_Proposal_Turn_(int16_t player_id_1, int16_t player_id_2);

    void __cdecl NPC_Diplomacy_();

    void __cdecl NPC_Diplomacy_Buildup_Penalties_();

    void __cdecl NPC_Threats_();

    void __cdecl NPC_To_NPC_Treaty_Negotiations_();

    void __cdecl Honor_Alliances_(int16_t player_idx, int16_t ally_idx);

    void __cdecl Eliminate_NPC_Treaty_Conflicts_();

    void __cdecl NoOp3();

    void __cdecl NPC_Declarations_Of_War_();

    void __cdecl NPC_Peace_Negotiations_();

    int16_t __cdecl NPC_Surrender_Check_(int16_t player_idx);

    void __cdecl NPC_Tech_Exchange_();

    void __cdecl NPC_Treaty_Hatred_();
}