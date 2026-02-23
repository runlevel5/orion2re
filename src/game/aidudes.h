#pragma once

struct s_colony;
struct s_aidudes_colony_info;

namespace aidudes {
    inline uint16_t& _n_our_blockaded = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005657C0});
    inline uint16_t& _n_our_unblockaded = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005657C4});
    inline s_aidudes_colony_info*& _our_blockaded = *reinterpret_cast<s_aidudes_colony_info**>(std::uintptr_t{0x005657C8});

    inline s_player*& _p_ai =
        *reinterpret_cast<s_player**>(std::uintptr_t{0x005657CC});

    inline s_aidudes_colony_info*& _our_unblockaded = *reinterpret_cast<s_aidudes_colony_info**>(std::uintptr_t{0x005657D0});
    inline uint16_t& _n_our_colonies = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005657D4});
    inline s_colony*& _Sort_Pops_colony =
        *reinterpret_cast<s_colony**>(std::uintptr_t{0x005657D8});

    inline int16_t& _i_ai =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005657DC});

    inline s_aidudes_colony_info (*&_our_colonies)[250] =
        *reinterpret_cast<s_aidudes_colony_info (**)[250]>(std::uintptr_t{0x005657E0});

    int __cdecl Sort_Colonies_Blockaded_(const void* a, const void* b);

    void __cdecl Collect_AI_Colonies_();

    void __cdecl Clear_Build_Queues_If_New_Tech_(bool force_clear);

    void __cdecl Keep_Ship_Designs_Up_To_Date_();

    void __cdecl Freighter_Cheat_(int32_t player_idx);

    void __cdecl Do_Ship_Upgrade_Cheats_();

    void __cdecl Colony_AI_Given_Tax_();

    inline int __cdecl Sort_Pops_FI_(const void* a, const void* b) {
        using Fn = int(__cdecl*)(const void*, const void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00445320});
        return fn(a, b);
    }

    void __cdecl Do_Blockaded_Colony_(s_aidudes_colony_info* colony_info);

    void __cdecl Do_Unblockaded_Colony_(s_aidudes_colony_info* info);

    inline void __cdecl Assign_Required_Colonists_(s_aidudes_colony_info* info) {
        using Fn = void(__cdecl*)(s_aidudes_colony_info*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00445130});
        fn(info);
    }

    inline void __cdecl Compute_Max_Min_R_I_Worthiness_(s_aidudes_colony_info* info) {
        using Fn = void(__cdecl*)(s_aidudes_colony_info*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00445710});
        fn(info);
    }

    inline int __cdecl Sort_Pops_Compare_2(const void* a, const void* b) {
        using Fn = int(__cdecl*)(const void*, const void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00445520});
        return fn(a, b);
    }

    inline int __cdecl Sort_Pops_Compare_1(const void* a, const void* b) {
        using Fn = int(__cdecl*)(const void*, const void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004455D0});
        return fn(a, b);
    }

    void __cdecl Assign_Additional_Research_Industry_();

    void __cdecl Grab_Scientist_(s_aidudes_colony_info* colony);

    void __cdecl Grab_Worker_(s_aidudes_colony_info* info);

    inline void __cdecl Assign_Empire_Building_(int16_t i_ai) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0047be10});
        fn(i_ai);
    }

    void __cdecl Industry_Sanity_Check_();

    inline void __cdecl Assign_Additional_Unblockaded_Farmers_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004459d0});
        fn();
    }

    void __cdecl Colony_AI_(int16_t i_ai);
    void All_Colony_AI_();

    void __cdecl Assign_Buildings_();
}