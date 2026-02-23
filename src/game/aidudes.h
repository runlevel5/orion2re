#pragma once

namespace aidudes {
    struct s_aidudes_colony_info {
        int8_t colony_idx;
        uint8_t field1_0x1;
        uint8_t field2_0x2;
        uint8_t field3_0x3;
        uint8_t field4_0x4;
        uint8_t field5_0x5;
        uint8_t field6_0x6;
        uint8_t field7_0x7;
        uint8_t field8_0x8;
        uint8_t field9_0x9;
        uint8_t field10_0xa;
        uint8_t field11_0xb;
        uint8_t field12_0xc;
    };

    inline uint16_t& _n_our_blockaded = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005657C0});
    inline uint16_t& _n_our_unblockaded = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005657C4});
    inline s_aidudes_colony_info*& _our_blockaded = *reinterpret_cast<s_aidudes_colony_info**>(std::uintptr_t{0x005657C8});

    inline s_player*& _p_ai =
        *reinterpret_cast<s_player**>(std::uintptr_t{0x005657CC});

    inline s_aidudes_colony_info*& _our_unblockaded = *reinterpret_cast<s_aidudes_colony_info**>(std::uintptr_t{0x005657D0});
    inline uint16_t& _n_our_colonies = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005657D4});

    inline int16_t& _i_ai =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005657DC});

    inline s_aidudes_colony_info (*&_our_colonies)[250] =
        *reinterpret_cast<s_aidudes_colony_info (**)[250]>(std::uintptr_t{0x005657E0});

    int __cdecl Sort_Colonies_Blockaded_(const void* a, const void* b);

    void __cdecl Collect_AI_Colonies_();

    void __cdecl Clear_Build_Queues_If_New_Tech_(bool force_clear);

    void __cdecl Keep_Ship_Designs_Up_To_Date_();

    void __cdecl Freighter_Cheat_(int32_t player_idx);

    inline void __cdecl Do_Ship_Upgrade_Cheats_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00446950});
        fn();
    }

    inline void __cdecl Colony_AI_Given_Tax_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00444E80});
        fn();
    }

    void __cdecl Colony_AI_(int16_t i_ai);
    void All_Colony_AI_();
}