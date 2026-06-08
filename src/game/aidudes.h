#pragma once

struct s_colony;
struct s_aidudes_colony_info;

namespace aidudes {
    extern int16_t _n_our_blockaded;
    extern int16_t _n_our_unblockaded;
    extern s_aidudes_colony_info* _our_blockaded;

    extern s_player* _p_ai;

    extern s_aidudes_colony_info* _our_unblockaded;
    extern int16_t _n_our_colonies;
    extern s_aidudes_colony_info* _Sort_Pops_colony_info;

    extern s_colony* _Sort_Pops_colony_data;

    extern int16_t _i_ai;

    extern s_aidudes_colony_info (*_our_colonies)[MAX_COLONIES];

    /**
     * Sorts colony-info entries so blockaded colonies are processed first.
     * @param a First colony-info entry.
     * @param b Second colony-info entry.
     */
    int __cdecl Sort_Colonies_Blockaded_(const void* a, const void* b);

    /**
     * Collects the current AI player's owned, non-outpost colonies into the working lists.
     */
    void __cdecl Collect_AI_Colonies_();

    /**
     * Clears build queues after a tech change when the colony is on a rebuild-eligible project.
     * @param force_clear Nonzero to force reevaluation immediately.
     */
    void __cdecl Clear_Build_Queues_If_New_Tech_(bool force_clear);

    /**
     * Refreshes ships under construction so they use the current design template.
     */
    void __cdecl Keep_Ship_Designs_Up_To_Date_();

    /**
     * Grants emergency freighters to an AI empire that is short on food logistics.
     * @param player_idx Player receiving the freighter check.
     */
    void __cdecl Freighter_Cheat_(int32_t player_idx);

    /**
     * Opportunistically upgrades eligible AI ships using the current refit tables.
     */
    void __cdecl Do_Ship_Upgrade_Cheats_();

    /**
     * Runs the colony-allocation passes after tax policy has already been chosen.
     */
    void __cdecl Colony_AI_Given_Tax_();

    int __cdecl Sort_Pops_FI_(const void* a, const void* b);

    int __cdecl Sort_Pops_F_IR_(const void* a, const void* b);

    int16_t __cdecl FI_Worthiness_(const s_colony* colony, const uint32_t* pop);

    int __cdecl F_IR_Worthiness_(s_aidudes_colony_info* info, uint32_t* flags);

    /**
     * Processes a blockaded colony's farmer and worker assignments.
     * @param colony_info Working colony-info record for the colony.
     */
    void __cdecl Do_Blockaded_Colony_(s_aidudes_colony_info* colony_info);

    /**
     * Processes an unblockaded colony's worker and researcher assignments.
     * @param info Working colony-info record for the colony.
     */
    void __cdecl Do_Unblockaded_Colony_(s_aidudes_colony_info* info);

    /**
     * Assigns required fixed colonists before the colony-specific AI pass runs.
     * @param colony_info Working colony-info record to populate.
     */
    void __cdecl Assign_Required_Colonists_(s_aidudes_colony_info* colony_info);

    void __cdecl Make_First_Unassigned_Into_Farmer_(s_aidudes_colony_info* info);

    int __cdecl Sort_Pops_Fixed_(const void* a, const void* b);

    void __cdecl Compute_Max_Min_R_I_Worthiness_(s_aidudes_colony_info* info);

    int __cdecl _Sort_Pops_colony(const void* a, const void* b);

    int __cdecl Sort_Pops_FR_I_(const void* a, const void* b);

    /**
     * Balances empire-wide worker and scientist assignments across unblockaded colonies.
     */
    void __cdecl Assign_Additional_Research_Industry_();

    void __cdecl Grab_Scientist_(s_aidudes_colony_info* colony);

    void __cdecl Grab_Worker_(s_aidudes_colony_info* info);

    /**
     * Selects a shared emergency build project for colonies with open queues.
     * @param player_idx Player whose colonies are being updated.
     */
    void __cdecl Assign_Empire_Building_(int16_t player_idx);

    /**
     * Revalidates researching colonies after assignment changes.
     */
    void __cdecl Industry_Sanity_Check_();

    /**
     * Adds extra farmers until the empire's food and freighter shortages are resolved.
     */
    void __cdecl Assign_Additional_Unblockaded_Farmers_();

    /**
     * Runs the full colony-AI pass for one player.
     * @param i_ai AI player index to process.
     */
    void __cdecl Colony_AI_(int16_t i_ai);

    /**
     * Runs colony AI for every active player.
     */
    void __cdecl All_Colony_AI_();

    /**
     * Lets the building AI finalize per-colony build choices after empire-wide decisions.
     */
    void __cdecl Assign_Buildings_();
    int __cdecl colony_less_industry_cmp_(const void* p1, const void* p2);

    int __cdecl Sort_Pops_R_I_(const void* p1, const void* p2);
}