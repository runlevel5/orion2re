#pragma once
#include <cstdint>

#pragma pack(push, 1)
struct s_net_election_msg {
    int32_t rank;
    int8_t hit_next_turn;
};
#pragma pack(pop)

namespace NETTURN {
    /** Advances the networked end-of-turn flow for the local player. */
    void __cdecl Net_Next_Turn_();

    /** Runs the host-side order collection, sync, and broadcast sequence. */
    void __cdecl Host_Next_Turn_();

    /** Runs the client-side order submission and waits for host results. */
    void __cdecl Client_Next_Turn_();

    /**
     * Loads packed net-turn UI strings from a contiguous string block.
     * @param string_block Pointer to the first packed string.
     * @return Pointer to the first unread byte after the consumed strings.
     */
    char* __cdecl Load_Netturn_Strings_(char* string_block);

    extern char* _waiting_message_fmt;

    extern char* _chunk_progress_fmt;

    /** Returns whether any AI-player sync mask still requires host broadcast data. */
    int8_t __cdecl Must_Broadcast_AI_Player_Data_();

    /** Resets host-election and per-turn network sync state for the next turn. */
    void __cdecl Net_Init_Host_Election_();
}