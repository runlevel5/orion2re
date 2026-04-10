#pragma once
#include <cstdint>

struct s_network;

namespace NETMOX {
#pragma push(pack, 1)
    struct s_net_change_relation {
        int16_t change;
        int16_t attacker;
        int16_t defender;
        int16_t reason;
        int16_t star_idx;
        int16_t param6;
    };
#pragma pop()

    inline s_network& _net = *reinterpret_cast<s_network*>(std::uintptr_t{0x00595170});

    inline uint8_t& _sending_msg =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055AD40});

    inline char*& _lost_net_user_error =
        *reinterpret_cast<char**>(std::uintptr_t{0x00572358});

    inline char*& _only_1_host_allowed_error =
        *reinterpret_cast<char**>(std::uintptr_t{0x00572374});

    inline char*& _modem_diff_buffer_error =
        *reinterpret_cast<char**>(std::uintptr_t{0x00572378});

    inline char*& _modem_disk_error =
        *reinterpret_cast<char**>(std::uintptr_t{0x0057237C});

    inline char*& _sending_chunk_d_of_d_fmt =
        *reinterpret_cast<char**>(std::uintptr_t{0x00572380});

    inline char*& _incompatible_versions_error =
        *reinterpret_cast<char**>(std::uintptr_t{0x00572384});

    inline char*& _ambassador_left_before_you_arrived_fmt =
        *reinterpret_cast<char**>(std::uintptr_t{0x00572388});

    inline char*& _net_send_error =
        *reinterpret_cast<char**>(std::uintptr_t{0x0057238C});

    inline uint8_t& _in_receive_message =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0057235C});

    inline uint8_t& _net_players_active_mask =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00595300});

    inline int16_t __cdecl Human_Requesting_Diplomacy_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00519a00});
        return fn();
    }

    inline void __cdecl Accept_Diplomacy_Request_(int32_t requester_id) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00519a80});
        fn(requester_id);
    }

    inline void __cdecl Refuse_Diplomacy_Request_(int32_t requester_id) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00519b80});
        fn(requester_id);
    }

    inline void __cdecl Tell_Diplomacy_Requester_To_Wait_(int32_t requester_id) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00519c10});
        fn(requester_id);
    }

    uint8_t Humans_Requesting_Diplomacy_();

    void __cdecl Russ_Change_Relations_(int32_t change, int16_t attacker, int16_t defender, int32_t reason, int16_t star_idx, int32_t param6);

    inline void __cdecl Send_Drop_Out_Info_(int player_idx, int reason) {
        using Fn = void(__cdecl*)(int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00519d60});
        fn(player_idx, reason);
    }

    inline void __cdecl Russ_Event_Monster_Update_(int16_t owner, int16_t p2, int16_t p3) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(0x005187C0)(owner, p2, p3);
    }

    int32_t __cdecl Net_Id_To_Player_Id_(int32_t player_idx);

    inline void __cdecl Send_Pick_Your_Position_(uint32_t net_id) {
        using Fn = void(__cdecl*)(uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005192b0});
        fn(net_id);
    }

    char* __cdecl Load_Netmox_Strings_(char* strings);

    inline void __cdecl Update_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005156A0});
        fn();
    }

    int32_t __cdecl Mox_Send_Message_(int16_t param_1, uint16_t param_2, void* packet, uint16_t len);

    inline void __cdecl Decode_Game_Orders_(void* data, uint32_t len) {
        using Fn = void(__cdecl*)(void*, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005171C0});
        fn(data, len);
    }

    inline void __cdecl Broadcast_Game_Data_Differential_(uint8_t unknown) {
        using Fn = void(__cdecl*)(uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00516220});
        fn(unknown);
    }

    inline bool __cdecl Decode_Broadcast_Game_Data_Differential_(void* data, uint32_t size, int unknown) {
        using Fn = bool(__cdecl*)(void*, uint32_t, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005166a0});
        return fn(data, size, unknown);
    }

    inline void __cdecl Send_Game_Orders_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00516c80});
        fn();
    }

    void __cdecl Receive_Message_(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet);

    inline char* net_field_1be = reinterpret_cast<char*>(std::uintptr_t{0x0059532E});

    void __cdecl Net_Request_AI_Diplomacy_(int16_t player_idx);

    int8_t __cdecl Net_May_Begin_AI_Diplomacy_();

    bool __cdecl No_AI_Diplomacy_Allowed_();

    void __cdecl Net_Cancel_AI_Diplomacy_Request_();

    bool __cdecl Net_Cancel_AI_Diplomacy_Confirmed_();

    inline void __cdecl Net_End_AI_Diplomacy_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x005195D0})();
    }

    inline void __cdecl Net_Request_Human_Diplomacy_(int16_t player_idx, int16_t dummy) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00519750})(player_idx, dummy);
    }

    int8_t __cdecl Net_May_Begin_Human_Diplomacy_();

    int8_t __cdecl Human_Hit_Next_Turn_(int16_t player_idx);

    bool __cdecl Allowed_To_Cancel_Human_Diplomacy_();

    int8_t __cdecl Human_Diplomat_Says_Wait_();

    bool __cdecl Net_Human_Rejected_Diplomacy_();

    void __cdecl Net_End_Human_Diplomacy_();

    void __cdecl Net_Cancel_Human_Diplomacy_Request_();

    int32_t __cdecl Net_Diplomacy_With_(int16_t limit);

    void __cdecl Human_Abdicates_(int player_idx);

    void __cdecl Human_Surrenders_(int player_idx);
}