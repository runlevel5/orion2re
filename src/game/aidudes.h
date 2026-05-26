#pragma once

struct s_colony;
struct s_aidudes_colony_info;

namespace aidudes {
    inline int16_t& _n_our_blockaded = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005657C0});
    inline int16_t& _n_our_unblockaded = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005657C4});
    inline s_aidudes_colony_info*& _our_blockaded = *reinterpret_cast<s_aidudes_colony_info**>(std::uintptr_t{0x005657C8});

    inline s_player*& _p_ai =
        *reinterpret_cast<s_player**>(std::uintptr_t{0x005657CC});

    inline s_aidudes_colony_info*& _our_unblockaded = *reinterpret_cast<s_aidudes_colony_info**>(std::uintptr_t{0x005657D0});
    inline int16_t& _n_our_colonies = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005657D4});
    inline s_aidudes_colony_info*& _Sort_Pops_colony_info =
        *reinterpret_cast<s_aidudes_colony_info**>(std::uintptr_t{0x005657BC});

    inline s_colony*& _Sort_Pops_colony_data =
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

    int __cdecl Sort_Pops_FI_(const void* a, const void* b);

    int __cdecl Sort_Pops_F_IR_(const void* a, const void* b);

    int16_t __cdecl FI_Worthiness_(const s_colony* colony, const uint32_t* pop);

    int __cdecl F_IR_Worthiness_(s_aidudes_colony_info* info, uint32_t* flags);

    void __cdecl Do_Blockaded_Colony_(s_aidudes_colony_info* colony_info);

    void __cdecl Do_Unblockaded_Colony_(s_aidudes_colony_info* info);

    void __cdecl Assign_Required_Colonists_(s_aidudes_colony_info* colony_info);

    void __cdecl Make_First_Unassigned_Into_Farmer_(s_aidudes_colony_info* info);

    int __cdecl Sort_Pops_Fixed_(const void* a, const void* b);

    void __cdecl Compute_Max_Min_R_I_Worthiness_(s_aidudes_colony_info* info);

    int __cdecl _Sort_Pops_colony(const void* a, const void* b);

    int __cdecl Sort_Pops_FR_I_(const void* a, const void* b);

    void __cdecl Assign_Additional_Research_Industry_();

    void __cdecl Grab_Scientist_(s_aidudes_colony_info* colony);

    void __cdecl Grab_Worker_(s_aidudes_colony_info* info);

    void __cdecl Assign_Empire_Building_(int16_t player_idx);

    void __cdecl Industry_Sanity_Check_();

    void __cdecl Assign_Additional_Unblockaded_Farmers_();

    void __cdecl Colony_AI_(int16_t i_ai);
    void __cdecl All_Colony_AI_();

    void __cdecl Assign_Buildings_();
    int __cdecl colony_less_industry_cmp_(const void* p1, const void* p2);

    int __cdecl Sort_Pops_R_I_(const void* p1, const void* p2);
}