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

    inline void __cdecl Mox_Send_Message_(int16_t target, int16_t type, void* data, int32_t len) {
        using Fn = void(__cdecl*)(int16_t, int16_t, void*, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00515560});
        fn(target, type, data, len);
    }

    inline char* net_field_1be = reinterpret_cast<char*>(std::uintptr_t{0x0059532E});
}