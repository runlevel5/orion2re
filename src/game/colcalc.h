#pragma once

#include "strbuild.h"

namespace COLCALC {
    struct s_lander_ship_cost {
        int32_t colony_ship;
        int32_t outpost_ship;
        int32_t transport_ship;
    };

    extern s_lander_ship_cost lander_ship_cost;

    struct s_growth_rate_bonus {
        uint8_t general_modifier;
        uint8_t microbiotics_tech;
        uint8_t antidote_tech;
        uint8_t boom_event;
        int16_t plague_event;
    };

    extern s_growth_rate_bonus growth_rate_bonus;

    extern uint8_t _housing_creates_no_farmers;
    extern uint8_t _planet_extinction;
    extern uint8_t _zero_colony_lost;
    extern uint8_t _keep_research_on_breakthrough;
    extern uint8_t _keep_research_on_finds;
    extern uint8_t _replicator_food_pp;
    extern uint8_t _replicator_food_bc;
    extern uint8_t _freighters_cash_bonus;

    typedef int16_t (__cdecl *ColonyBaseFunc)(const s_colony* colony, int16_t owner, int16_t player_idx, s_colony_job_production* out);

    extern ColonyBaseFunc _colony_empire_base_functions[3];

    extern int8_t _minerals_per_mine[8];

    extern int8_t _climate_maintenance_modifiers[PLANET_CLIMATE_COUNT];

    extern int8_t _morale_lookup_table[GOVERNMENT_COUNT];
    extern int8_t _non_capital_morale_lookup[GOVERNMENT_COUNT];
    extern int8_t _barracks_morale_lookup[GOVERNMENT_COUNT];

    extern int8_t _colony_job_production_difficulty_lookup[8];

    extern char* _government_morale_msg;
    extern char* _mixed_races_msg;
    extern char* _unification_has_no_morale_msg;
    extern char* _research_summary_msg;
    extern char* _no_capital_msg;
    extern char* _no_barracks_msg;
    extern char* _morale_summary_msg;
    extern char* _total_morale_msg;
    extern char* _command_summary_msg;
    extern char* _ship_maintenance_costs_msg;
    extern char* _d_from_1_s_ship_msg;
    extern char* _d_from_d_battlestations_msg;
    extern char* _from_warlord_msg;
    extern char* _d_from_1_star_base_msg;
    extern char* _d_from_1_noncombat_ship_msg;
    extern char* _d_from_d_fortresses_msg;
    extern char* _starting_command_points_msg;
    extern char* _d_from_d_star_bases_msg;
    extern char* _total_command_point_used_msg;
    extern char* _d_from_d_s_ships_msg;
    extern char* _d_from_d_noncombat_ships_msg;
    extern char* _total_command_points_msg;
    extern char* _bc_summary_msg;
    extern char* _total_command_points_used_msg;
    extern char* _from_imperium_msg;
    extern char* _d_from_1_battlestation_msg;
    extern char* _d_from_1_fortress_msg;

    uint8_t __cdecl Colony_Can_Build_Product_(s_colony* colony, int16_t product);

    bool __cdecl Room_For_Another_Dude_(s_colony* colony, int race_idx);

    int32_t __cdecl Production_Cost_To_Buy_(int32_t product_cost, int32_t production_spent);

    int32_t __cdecl Colony_Cost_To_Buy_Given_Product_(s_colony* colony, int32_t product, int32_t production_spent);

    int32_t __cdecl Colony_Cost_To_Buy_Producing_Product_(s_colony* colony);

    bool __cdecl Colony_Can_Afford_To_Buy_Product_(int16_t colony_idx);

    int32_t __cdecl Colony_Product_Cost_(s_colony* colony, int16_t product_id);

    int32_t __cdecl Colony_Producing_Product_Cost_(s_colony* colony);

    int32_t __cdecl Ship_Type_Cost_For_Player_(int32_t ship_type, int16_t player_idx);

    void __cdecl Colony_Buy_Outright_(int16_t colony_idx);

    int16_t __cdecl Colony_N_Turns_To_Produce_(s_colony* colony);

    void __cdecl Apply_Colony_Changes_(s_colony* colony);

    void __cdecl Apply_Colony_Pop_Growth_(s_colony* colony);

    extern int8_t _assimilation_rate_by_government[GOVERNMENT_COUNT];

    void __cdecl Apply_Assimilation_(s_colony* colony);

    void __cdecl Produce_Ground_Military_(s_colony* colony);

    void __cdecl Apply_Production_(s_colony* colony);

    void __cdecl Ghost_Colony_Stuff_(s_colony* colony);

    void __cdecl Apply_All_Colony_Changes_();
    void __cdecl Do_Surrenders_();
    void __cdecl All_Colony_Calculations_();
    void __cdecl Apply_All_Player_Changes_();
    void __cdecl Compute_Blockades_();

    void __cdecl Do_Colony_Owner_Change_Star_Colony_Stuff_(s_colony* colony);

    void __cdecl Colony_Environmental_Stuff_(s_colony* colony);

    void __cdecl Compute_Star_Colony_Stuff_(s_star_data* star);

    int32_t __cdecl Mixed_Race_Morale_Penalty_(s_colony* colony);

    char* __cdecl Player_Name_(int32_t player_idx);

    int __cdecl Colony_Officer_(s_colony* colony);

    short __cdecl Colony_Race_Pop_Limit_(s_colony* colony, int16_t player_idx);

    int16_t __cdecl Get_Player_Max_Population_(int16_t player_idx);
    int __cdecl Best_Leader_Diplomat_Bonus_(int player_idx);

    bool __cdecl Star_Has_Asteroid_Or_Gas_Giant_(int16_t star_index);

    /**
     * Sums building construction costs for each player's completed colonies.
     * @param player_costs Output array indexed by player.
     */
    void __cdecl Compute_Total_Building_Costs_(int32_t* player_costs);

    extern int8_t _difficulty_bc_lookup_table[8];
    extern int8_t _size_climate_max_pop_lookup[PLANET_CLIMATE_COUNT];
    extern char* _government_bonus_msg;
    extern char* _produced_by_farmers_msg;
    extern char* _produced_by_workers_msg;
    extern char* _government_penalty_msg;
    extern char* _native_bonus_msg;
    extern char* _conquered_penalty_msg;
    extern char* _produced_by_scientists_msg;
    extern char* _anomaly_msg;
    extern char* _bonus_msg;
    extern char* _food_summary_msg;
    extern char* _blockaded_penalty_msg;
    extern char* _morale_penalty_msg;
    extern char* _total_research_produced_msg;
    extern char* _artifacts_bonus_msg;
    extern char* _total_food_produced_msg;
    extern char* _gravity_penalty_msg;
    extern char* _aquatic_bonus_msg;
    extern char* _leader_bonus_msg;
    extern char* _android_bonus_msg;
    extern char* _intelligence_bonus_msg;
    extern char* _morale_bonus_msg;
    extern char* _special_income_msg;
    extern char* _taxed_from_industry_msg;
    extern char* _used_msg;
    extern char* _pollution_penalty_msg;
    extern char* _replicated_to_d_food_msg;
    extern char* _taxes_collected_msg;
    extern char* _trade_goods_msg;
    extern char* _total_industry_produced_msg;
    extern char* _total_income_msg;
    extern char* _industry_summary_msg;

    extern int8_t _planet_robotic_factory_lookup[8];
    extern int8_t _industry_planet_size_lookup[8];

    extern char* _taxed_msg;
    extern char* _base_msg;

    char* __cdecl Load_Colcalc_Strings_(char* strings);

    int16_t __cdecl Colony_Industry_Per_Worker_(const s_colony* colony, int16_t player_idx, s_colony_job_production* out);
    int16_t __cdecl Colony_Food2_Per_Farmer_(const s_colony* colony, int16_t player_idx, s_colony_job_production* out);
    int16_t __cdecl Colony_Research_Per_Scientist_(const s_colony* colony, int16_t player_idx, s_colony_job_production* out);

    void __cdecl Twiddle_Initial_Homeworlds_();
    void __cdecl Twiddle_Initial_Homeworld_(s_colony* colony);

    /**
     * Recomputes a colony's local production and maintenance state before empire-wide import distribution.
     * @param colony Colony to prepare.
     */
    void __cdecl Pre_Import_Computing_(s_colony* colony);

    /**
     * Distributes empire food imports among a player's colonies and optionally finalizes one target colony.
     * @param player_idx Player whose colonies are being balanced.
     * @param colony Optional colony to ensure receives post-import recomputation.
     */
    void __cdecl Pass_Out_Imports_(int16_t player_idx, s_colony* colony);

    void __cdecl Recalculate_Colony_(s_colony* colony);

    bool __cdecl Colony_Can_Build_Large_Ships_(s_colony* colony);

    /**
     * Recomputes colony-derived control and infrastructure flags for every star system.
     */
    void __cdecl Compute_All_Star_Colony_Stuff_();
    void __cdecl Compute_Star_Colony_Stuff_(s_star_data* star);

    void __cdecl Star_Colony_Calculation_(int16_t star_idx, int16_t player_idx);

    void __cdecl Init_Surrenders_();

    void __cdecl Surrender_Empire_(int16_t player_id, int16_t target_id);

    bool __cdecl May_Surrender_Star_(int16_t star_idx, int16_t player_idx, int16_t recipient_idx);

    void __cdecl Surrender_Star_(int16_t star_idx, int16_t player_idx, int16_t to_player_idx);

    void __cdecl Do_Surrender_Colony_(s_colony* colony, int16_t new_owner);

    void __cdecl Do_Colony_Owner_Change_Star_Colony_Stuff_(s_colony* colony);

    void __cdecl Do_Surrender_Star_(int16_t star_idx, int16_t old_owner, int16_t new_owner);

    void __cdecl Do_Surrender_Empire_(int16_t player_idx, int16_t target_idx);

    void __cdecl Eliminate_Player_(s_player* player);

    bool __cdecl May_Surrender_Empire_(int16_t player_idx, int16_t target_idx);

    int32_t __cdecl Sort_star_weights_(const void* a, const void* b);

    int16_t __cdecl Get_Surrender_System_(int16_t player_idx, int16_t target_player_idx, int16_t selection_mode);

    void __cdecl No_Op1_();

    int __cdecl Player_Research_Cost_(s_player* plr, int16_t tech_field);

    int32_t __cdecl Chance_For_Research_Breakthrough_Aux_(int32_t accumulated_total, int32_t produced, int32_t cost);

    int16_t __cdecl Player_N_Turns_Until_Research_Complete_(s_player* plr);

    int32_t __cdecl Chance_For_Research_Breakthrough_(s_player* plr);

    int16_t __cdecl Get_First_Player_Colony_Idx(int32_t player_idx);

    inline bool __cdecl Monster_At_Star_(int16_t star_idx);

    void __cdecl Delete_Obsolete_Artifical_Planets_Under_Construction_(int16_t star_idx);

    int32_t __cdecl Colony_N_Pops_Of_Empire_(s_colony* colony, uint32_t empire_id);

    uint8_t __cdecl Leader_At_Anomaly_(s_leader_data* leader);

    void __cdecl All_Colony_Calculations_();

    void __cdecl Colony_Calculation_(int16_t colony_idx);

    void __cdecl Colony_Pop_Grows_(s_colony* colony);

    void __cdecl Colony_Specialty_(s_colony* colony);

    void __cdecl Post_Import_Computing_(s_colony* colony);

    void __cdecl Update_Player_Stats_(s_player* player);

    void __cdecl Colony_Environmental_Stuff_(s_colony* colony);

    /**
     * Computes a colony's morale and optionally writes a short breakdown string.
     * @param colony Colony to evaluate.
     * @param debug_out Optional output buffer for the morale breakdown.
     */
    void __cdecl Colony_Morale_(s_colony* colony, STRBUILD::s_builder* debug_out);

    void __cdecl Colony_Industry_Maintenance_(s_colony* colony);

    /**
     * Computes colony industry output, pollution, and optional production breakdown text.
     * @param colony Colony to evaluate.
     * @param debug_out Optional output buffer for the industry summary.
     */
    void __cdecl Colony_Industry_Production_(s_colony* colony, STRBUILD::s_builder* debug_out);

    void __cdecl Show_Industry_Production_(int16_t colony_idx);

    void __cdecl Show_Research_Production_(int16_t colony_idx);

    int __cdecl Colony_Job_Production_(s_colony* colony, short econ_type, s_colony_job_production* out);

    int16_t __cdecl Colony_Pop_Base_Prod_Produced_(const s_colony* colony, const uint32_t* pop, int16_t job_type, s_colony_job_production* out);

    int16_t __cdecl Colony_Pop_Prod_Produced_(const s_colony* colony, const uint32_t* pop_unit, int16_t mode);

    extern char _tmp_quotient[20];
    char* __cdecl Format_Quotient_(int32_t value, int32_t divisor);

    /**
     * Computes colony food output and optionally writes a production breakdown text.
     * @param colony Colony to evaluate.
     * @param debug_out Optional output buffer for the food summary.
     */
    void __cdecl Colony_Food_Production_(s_colony* colony, STRBUILD::s_builder* debug_out);

    int16_t __cdecl Colony_Empire_Base_Food2_Produced_(const s_colony* colony, int16_t owner_idx, int16_t player_idx, s_colony_job_production* out);

    int16_t __cdecl Colony_Empire_Base_Industry_Produced_(const s_colony* colony, int16_t type, int16_t player_idx, s_colony_job_production* out);

    int16_t __cdecl Colony_Empire_Base_Research_Produced_(const s_colony* colony, int16_t research_type, int16_t player_idx, s_colony_job_production* out);

    void __cdecl Colony_Food_Maintenance_(s_colony* colony);

    void __cdecl Colony_Replicators_(s_colony* colony);

    void __cdecl Colony_Research_Production_(s_colony* colony, STRBUILD::s_builder* debug_out);

    void __cdecl Colony_Industry_To_Tax_(s_colony* colony);

    void __cdecl Colony_BC_Production_(s_colony* colony, STRBUILD::s_builder* debug_buf);

    void __cdecl Colony_BC_Maintenance_(s_colony* colony);

    bool __cdecl Colony_Is_Blockaded(s_colony* colony);

    int __cdecl Calc_Player_Climate_(int player_idx, int climate);

    int __cdecl Size_And_Climate_Max_Population_(int8_t size, int climate, uint8_t is_immune);

    short __cdecl Double_Add_Two_(short value);

    int16_t __cdecl Size_And_Climate_Race_Pop_Limit_(int8_t size, int climate, int16_t player_idx);

    int32_t __cdecl Tribute_Received_From_(int32_t receiver_idx, int32_t payer_idx);

    bool __cdecl Player_May_Get_Tech_App_(s_player* plr, int tech_app);

    void __cdecl Player_Gets_Tech_App_(s_player* plr, int16_t tech_app_id);

    bool __cdecl Field_Is_Gray_Box_(int32_t field_id);

    void __cdecl Advance_Player_Field_(s_player* plr, int32_t field_id, bool skip_first_advance);

    void __cdecl Ensure_Uncreative_Field_OK_(s_player* player, int field_idx);

    void __cdecl Do_Player_Colony_Calculations_(int16_t player_idx);

    void __cdecl Do_Player_Colony_Post_Production_(int16_t player_idx);

    bool __cdecl Player_Owns_Officer_(s_player* plr, s_leader_data* leader);

    /**
     * Recalculates player maintenance totals, command points, and optional maintenance breakdown text.
     * @param plr Player to update.
     * @param debug_out Optional output buffer for the maintenance summary.
     */
    void __cdecl Compute_Player_Maintenance_(s_player* plr, STRBUILD::s_builder* debug_out);

    int16_t __cdecl Gravity_Player_Production_Factor_(const s_colony* colony, int16_t player_idx);

    int16_t __cdecl Get_Govt_Type_Tech_App_(int16_t govt_type);

    void __cdecl Compute_Potential_Colony_Info_(s_player* plr, s_planet_data* planet, s_potential_colony_info* results);

    int16_t __cdecl Planet_Max_Population_For_Player_(s_planet_data* planet, int16_t player_idx);

    bool __cdecl Housing_Is_Useless_(s_colony* colony);

    int16_t __cdecl N_Colonies_And_Outposts_At_Star_(int16_t star_index);

    int16_t N_Living_Human_Players_();

    int16_t N_Living_Players_();

    void __cdecl Check_For_Eliminated_Players_();

    void __cdecl Apply_All_Player_Changes_();

    void __cdecl Check_For_Research_Breakthrough_(s_player* player);

    void __cdecl Resolve_Research_Completion_(s_player* player, bool keep_points, int32_t points_spent);

    void __cdecl Give_Player_Field_(s_player* player, int field_id, char get_all_flag);

    void __cdecl Compute_Player_Total_Known_Tech_Cost_(s_player* player);

    short __cdecl N_Prods_In_Colony_Queue_(int16_t colony_idx, short item_id);

    int16_t __cdecl Colony_Planetary_Shield_(s_colony* colony);

    void __cdecl Show_Command_Points_(int player_idx);
    int16_t __cdecl Get_System_Population_(int16_t player_idx, int16_t star_idx);

    int16_t __cdecl Get_Possible_Surrender_Systems_(int16_t player_idx, int16_t target_player_idx, int16_t *star_list);
}