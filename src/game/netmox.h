#pragma once
#include <cstdint>

struct s_network;

namespace NETMOX {
    inline s_network& _net = *reinterpret_cast<s_network*>(std::uintptr_t{0x00595170});

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

    // Suggested name: NETMOX::Humans_Requesting_Diplomacy
    inline uint8_t __cdecl Humans_Requesting_Diplomacy_() {
        using Fn = uint8_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x00519c40})();
    }

    inline void __cdecl Russ_Change_Relations_(int32_t change, int16_t attacker, int16_t defender, int32_t reason, int16_t star_idx, int32_t param6) {
        using Fn = void(__cdecl*)(int32_t, int16_t, int16_t, int32_t, int16_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00518820});
        fn(change, attacker, defender, reason, star_idx, param6);
    }

    inline void __cdecl Human_Abdicates_(int32_t player_num) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00519D50});
        fn(player_num);
    }

    inline void __cdecl Russ_Event_Monster_Update_(int16_t owner, int16_t p2, int16_t p3) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(0x005187C0)(owner, p2, p3);
    }

    inline char* __cdecl Load_Netmox_Strings_(char* data) {
        using Fn = char*(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00513E80});
        return fn(data);
    }

    inline int __cdecl Mox_Send_Message_(int16_t target, int32_t type, void* data, int32_t len) {
        using Fn = int(__cdecl*)(int16_t, int32_t, void*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00515560});
        return fn(target, type, data, len);
    }

    inline char* net_field_1be = reinterpret_cast<char*>(std::uintptr_t{0x0059532E});

    inline void __cdecl Net_Request_AI_Diplomacy_(int16_t player_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00519560})(player_idx);
    }

    int8_t __cdecl Net_May_Begin_AI_Diplomacy_();

    inline int8_t __cdecl No_AI_Diplomacy_Allowed_() {
        using Fn = int8_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x00519730})();
    }

    inline void __cdecl Net_Cancel_AI_Diplomacy_Request_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x005196D0})();
    }

    inline int8_t __cdecl Net_Cancel_AI_Diplomacy_Confirmed_() {
        using Fn = int8_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x00519720})();
    }

    inline void __cdecl Net_End_AI_Diplomacy_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x005195D0})();
    }

    inline void __cdecl Net_Request_Human_Diplomacy_(int16_t player_idx, int16_t dummy) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00519750})(player_idx, dummy);
    }

    inline int8_t __cdecl Net_May_Begin_Human_Diplomacy_() {
        using Fn = int8_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x005198D0})();
    }

    inline int8_t __cdecl Human_Hit_Next_Turn_(int16_t player_idx) {
        using Fn = int8_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x005199C0})(player_idx);
    }

    inline int8_t __cdecl Allowed_To_Cancel_Human_Diplomacy_() {
        using Fn = int8_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x00519950})();
    }

    inline int8_t __cdecl Human_Diplomat_Says_Wait_() {
        using Fn = int8_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x005199E0})();
    }

    inline int8_t __cdecl Net_Human_Rejected_Diplomacy_() {
        using Fn = int8_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x00519C50})();
    }

    inline void __cdecl Net_End_Human_Diplomacy_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x005199D0})();
    }

    inline void __cdecl Net_Cancel_Human_Diplomacy_Request_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00519960})();
    }
}