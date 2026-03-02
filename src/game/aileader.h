#pragma once

#include <cstdint>

struct s_leader_data;

namespace aileader {
    void __cdecl Do_AI_Leaders_();

    void __cdecl Assign_Fleet_Officer_(int player_index, int leader_index);

    void __cdecl Assign_Star_Officer_(int player_index, int leader_index);

    void __cdecl Reassign_Fleet_Officer_(int player_index, int leader_index);

    void __cdecl Assign_Unassigned_Leaders_(int32_t player_idx);
}