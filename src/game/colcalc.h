#pragma once
#include <cstdint>

struct s_player;
struct s_colony;
struct s_star_data;
struct s_leader_data;
struct s_planet_data;
struct s_colony_job_production;

namespace COLCALC {
    typedef int16_t (__cdecl *ColonyBaseFunc)(s_colony* colony, int16_t owner, int32_t masked_param2, s_colony_job_production* out);

    inline ColonyBaseFunc (&_colony_empire_base_functions)[3] =
        *reinterpret_cast<ColonyBaseFunc (*)[3]>(std::uintptr_t{0x0055A858});

    inline int8_t (&_minerals_per_mine)[8] =
        *reinterpret_cast<int8_t(*)[8]>(std::uintptr_t{0x0054F160});

    inline int8_t (&_climate_maintenance_modifiers)[10] =
        *reinterpret_cast<int8_t(*)[10]>(std::uintptr_t{0x0054F168});

    inline int8_t* _morale_lookup_table = reinterpret_cast<int8_t*>(std::uintptr_t{0x0054F178});
    inline int8_t* _non_capital_morale_lookup = reinterpret_cast<int8_t*>(std::uintptr_t{0x0054F180});
    inline int8_t* _barracks_morale_lookup = reinterpret_cast<int8_t*>(std::uintptr_t{0x0054F188});

    inline int8_t (&_colony_job_production_difficulty_lookup)[8] =
        *reinterpret_cast<int8_t(*)[8]>(std::uintptr_t{0x0054F190});

    inline const char*& _government_morale_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571AE0});
    inline const char*& _mixed_races_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571AE4});
    inline const char*& _unification_has_no_morale_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B00});
    inline const char*& _no_capital_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B10});
    inline const char*& _no_barracks_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B38});
    inline const char*& _total_morale_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571BC8});
    inline int8_t (&_colony_calc_food_cache)[10] =
        *reinterpret_cast<int8_t(*)[10]>(std::uintptr_t{0x00571BA0});
    inline const char*& _ship_maintenance_costs_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571ADC});
    inline const char*& _d_from_1_s_ship_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571AEC});
    inline const char*& _d_from_d_battlestations_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571AF0});
    inline const char*& _from_warlord_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571AF8});
    inline const char*& _d_from_1_star_base_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B18});
    inline const char*& _d_from_1_noncombat_ship_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B1C});
    inline const char*& _d_from_d_fortresses_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B34});
    inline const char*& _d_from_1_fortress_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B3C});
    inline const char*& _d_from_d_star_bases_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B44});
    inline const char*& _total_command_point_used_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B58});
    inline const char*& _d_from_d_s_ships_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B60});
    inline const char*& _d_from_d_noncombat_ships_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B64});
    inline const char*& _total_command_points_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B68});
    inline const char*& _total_command_points_used_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B74});
    inline const char*& _from_imperium_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B84});
    inline const char*& _d_from_1_battlestation_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B94});
    inline const char*& _d_from_1_fortress_msg_alt = *reinterpret_cast<const char**>(std::uintptr_t{0x00571BC4});

    inline s_colony*& _colony_calc_food_colony =
        *reinterpret_cast<s_colony**>(std::uintptr_t{0x0055A864});

    inline uint8_t& _colony_calc_food_type =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055A868});

    inline s_colony*& _colony_calc_industry_colony =
        *reinterpret_cast<s_colony**>(std::uintptr_t{0x0055A86C});

    inline uint8_t& _colony_calc_industry_type =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055A870});

    inline int8_t (&_colony_calc_industry_cache)[10] =
        *reinterpret_cast<int8_t (*)[10]>(std::uintptr_t{0x00571B88});

    inline const char* s_msg_bc_calc = reinterpret_cast<const char*>(std::uintptr_t{0x0055A908});
    inline const char* s_msg_bc_zero = reinterpret_cast<const char*>(std::uintptr_t{0x0055A928});
    inline const char* s_separator = reinterpret_cast<const char*>(std::uintptr_t{0x0055A934});
    inline const char* s_fmt_val_name = reinterpret_cast<const char*>(std::uintptr_t{0x0055A94C});
    inline const char* s_fmt_val_msg = reinterpret_cast<const char*>(std::uintptr_t{0x0055A958});

    inline bool __cdecl Colony_Can_Build_Product_(s_colony* colony, int16_t building_id) {
        using Fn = bool(__cdecl*)(s_colony*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004F6410});
        return fn(colony, building_id);
    }

    int32_t __cdecl Colony_Cost_To_Buy_Given_Product_(s_colony* colony, int32_t product, int32_t production_spent);

    int32_t __cdecl Colony_Cost_To_Buy_Producing_Product_(s_colony* colony);

    bool __cdecl Colony_Can_Afford_To_Buy_Product_(int16_t colony_idx);

    int __cdecl Colony_Product_Cost_(s_colony* colony, int16_t product_id);

    int32_t __cdecl Colony_Producing_Product_Cost_(s_colony* colony);

    int32_t __cdecl Ship_Type_Cost_For_Player_(int32_t ship_type, int16_t player_idx);

    void __cdecl Colony_Buy_Outright_(int16_t colony_idx);

    int16_t __cdecl Colony_N_Turns_To_Produce_(s_colony* colony);

    void __cdecl Apply_Colony_Changes_(s_colony* colony);
    void __cdecl Apply_All_Colony_Changes_();
    void __cdecl Do_Surrenders_();
    void __cdecl All_Colony_Calculations_();
    void __cdecl Apply_All_Player_Changes_();
    void __cdecl Compute_Blockades_();

    int32_t __cdecl Mixed_Race_Morale_Penalty_(s_colony* colony);

    int __cdecl Colony_Officer_(s_colony* colony);

    short __cdecl Colony_Race_Pop_Limit_(s_colony* colony, int16_t player_idx);

    inline int16_t __cdecl Owned_Officer_Level_(int officer_idx) {
        using Fn = int16_t(__cdecl*)(int);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0043e780})(officer_idx);
    }

    inline int16_t __cdecl Officer_Maintenance_(int16_t player_idx, int16_t leader_idx) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0043E8C0})(player_idx, leader_idx);
    }

    int16_t __cdecl Get_Player_Max_Population_(int16_t player_idx);
    int __cdecl Best_Leader_Diplomat_Bonus_(int player_idx);

    inline int8_t* _difficulty_bc_lookup_table = reinterpret_cast<int8_t*>(std::uintptr_t{0x0054F1A8});
    inline int8_t* _size_climate_max_pop_lookup = reinterpret_cast<int8_t*>(std::uintptr_t{0x0054F1B0});
    inline char*& _government_bonus_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571AD4});
    inline char*& _produced_by_workers_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571AD8});
    inline char*& _produced_by_workers_industry_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571AE8});
    inline char*& _government_penalty_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571AF4});
    inline char*& DAT_00571afc = *reinterpret_cast<char**>(std::uintptr_t{0x00571AFC});
    inline char*& _conquered_penalty_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B0C});
    inline char*& _produced_by_scientists_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B14});
    inline char*& _colony_debug_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B40});
    inline char*& _anomaly_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B40});
    inline char*& _bonus_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B48});
    inline char*& _blockaded_penalty_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B50});
    inline char*& _morale_penalty_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B54});
    inline char*& _total_research_produced_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B5C});
    inline char*& _artifacts_bonus_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B78});
    inline char*& DAT_00571b80 = *reinterpret_cast<char**>(std::uintptr_t{0x00571B80});
    inline char*& _gravity_penalty_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B98});
    inline char*& DAT_00571bc0 = *reinterpret_cast<char**>(std::uintptr_t{0x00571BC0});
    inline char*& _leader_bonus_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571BE0});
    inline char*& _android_bonus_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571BE4});
    inline char*& _intelligence_bonus_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571BE8});
    inline char*& _morale_bonus_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571BEC});
    inline char*& _special_income_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B08});
    inline char*& _taxed_from_industry_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B30});
    inline char*& _used_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B20});
    inline char*& _pollution_penalty_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571BD8});
    inline char*& _replicated_to_d_food_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B28});
    inline char*& _taxes_collected_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B7C});
    inline char*& _trade_goods_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571BD4});
    inline char*& _total_industry_produced_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571B70});
    inline char*& _total_income_msg = *reinterpret_cast<char**>(std::uintptr_t{0x00571BDC});
    inline const char*& _industry_summary_msg = *reinterpret_cast<const char**>(std::uintptr_t{0x00571B24});

    inline int8_t* _planet_robotic_factory_lookup = reinterpret_cast<int8_t*>(std::uintptr_t{0x0054F198});
    inline int8_t* _industry_planet_size_lookup = reinterpret_cast<int8_t*>(std::uintptr_t{0x0054F1A0});

    inline const char* const s_T_15_65 = reinterpret_cast<const char*>(std::uintptr_t{0x0055A8A4}); // "T+15,+65."
    inline const char* const s_fmt_s_00551130 = reinterpret_cast<const char*>(std::uintptr_t{0x00551130}); // "%s"
    inline const char* const s_0 = reinterpret_cast<const char*>(std::uintptr_t{0x0055110C}); // "0"
    inline const char* const s_fmt_plus_d_s = reinterpret_cast<const char*>(std::uintptr_t{0x0055A8DC}); // "%+d %s"
    inline const char* const s_fmt_plus_d_s_2 = reinterpret_cast<const char*>(std::uintptr_t{0x0055A8B8}); // "%+d %s"
    inline const char* const s_fmt_plus_d_s_s = reinterpret_cast<const char*>(std::uintptr_t{0x0055A8D0}); // "%+d %s %s"
    inline const char* const s_fmt_s_s_s = reinterpret_cast<const char*>(std::uintptr_t{0x0055A8C4}); // "%s %s %s"
    inline const char* const s_fmt_s_s = reinterpret_cast<const char*>(std::uintptr_t{0x0055A8B0}); // "%s %s"
    inline const char* const s_fmt_d_s = reinterpret_cast<const char*>(std::uintptr_t{0x0055A8F4}); // "%d %s"
    inline const char* const s_fmt_minus_d_s = reinterpret_cast<const char*>(std::uintptr_t{0x0055A8E8}); // "-%d %s"
    inline const char* const s_fmt_plus_d_s_3 = reinterpret_cast<const char*>(std::uintptr_t{0x0055A8FC}); // "+%d %s"
    inline const char* const s_newline_0055136c = reinterpret_cast<const char*>(std::uintptr_t{0x0055136C}); // ""

    inline char* __cdecl Load_Colcalc_Strings_(void* data) {
        using Fn = char*(__cdecl*)(void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004F28E0});
        return fn(data);
    }

    int16_t __cdecl Colony_Industry_Per_Worker_(s_colony* colony, uint16_t player_idx, s_colony_job_production* out);
    int16_t __cdecl Colony_Food2_Per_Farmer_(s_colony* colony, int16_t player_idx, void* out);
    inline s_colony*& _colony_calc_research_colony =
        *reinterpret_cast<s_colony**>(std::uintptr_t{0x0055A874});

    inline uint8_t& _colony_calc_research_type =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055A878});

    inline int8_t (&_colony_calc_research_cache)[10] =
        *reinterpret_cast<int8_t(*)[10]>(std::uintptr_t{0x00571BB0});

    int16_t __cdecl Colony_Research_Per_Scientist_(s_colony* colony, int16_t player_idx, s_colony_job_production* out);

    void __cdecl Twiddle_Initial_Homeworlds_();
    void __cdecl Twiddle_Initial_Homeworld_(s_colony* colony);

    void __cdecl Pre_Import_Computing_(s_colony* colony);

    void __cdecl Pass_Out_Imports_(int16_t owner, s_colony* colony);

    void __cdecl Recalculate_Colony_(s_colony* colony);

    void __cdecl Compute_All_Star_Colony_Stuff_();
    void __cdecl Compute_Star_Colony_Stuff_(s_star_data* star);

    void __cdecl Star_Colony_Calculation_(short star_idx, short player_idx);

    void __cdecl Init_Surrenders_();

    inline void __cdecl Surrender_Empire_(int16_t actor_player, int16_t target_player) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004fafb0});
        fn(actor_player, target_player);
    }

    bool __cdecl May_Surrender_Star_(int16_t star_idx, int16_t player_idx, int16_t recipient_idx);

    inline void __cdecl Surrender_Star_(int16_t star_idx, int16_t player_idx, int16_t to_player_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004faeb0});
        fn(star_idx, player_idx, to_player_idx);
    }

    inline void __cdecl Do_Surrender_Colony_(s_colony* colony, int16_t new_owner) {
        using Fn = void(__cdecl*)(s_colony*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004FB230});
        fn(colony, new_owner);
    }

    void __cdecl Do_Surrender_Star_(int16_t star_idx, int16_t old_owner, int16_t new_owner);

    void __cdecl Do_Surrender_Empire_(int16_t player_idx, int16_t target_idx);

    inline void __cdecl Eliminate_Player_(s_player* player) {
        using Fn = void(__cdecl*)(s_player*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004FB520});
        fn(player);
    }

    bool __cdecl May_Surrender_Empire_(int16_t target_idx, int16_t player_idx);

    void __cdecl No_Op1_();

    int __cdecl Player_Research_Cost_(s_player* plr, int16_t tech_field);

    int32_t __cdecl Chance_For_Research_Breakthrough_Aux_(int32_t accumulated_total, int32_t produced, int32_t cost);

    int16_t __cdecl Player_N_Turns_Until_Research_Complete_(s_player* plr);

    int32_t __cdecl Chance_For_Research_Breakthrough_(s_player* plr);

    short __cdecl Get_First_Player_Colony_Idx(int player_idx);

    inline bool __cdecl Monster_At_Star_(int star_idx);

    uint8_t __cdecl Leader_At_Anomaly_(s_leader_data* leader);

    void __cdecl All_Colony_Calculations_();

    void __cdecl Colony_Calculation_(int16_t colony_idx);

    void __cdecl Colony_Pop_Grows_(s_colony* colony);

    void __cdecl Colony_Specialty_(s_colony* colony);

    void __cdecl Update_Colony_Pop_Specialty_(s_colony* colony);

    void __cdecl Update_Player_Stats_(s_player* player);

    void __cdecl Colony_Environmental_Stuff_(s_colony* colony);

    void __cdecl Colony_Morale_(s_colony* colony, char* debug);

    void __cdecl Colony_Industry_Maintenance_(s_colony* colony);

    void __cdecl Colony_Industry_Production_(s_colony* colony, char* debug_out);

    void __cdecl Show_Industry_Production_(short colony_idx);

    int __cdecl Colony_Job_Production_(s_colony* colony, short econ_type, s_colony_job_production* out);

    int16_t __cdecl Colony_Pop_Base_Prod_Produced_(s_colony* colony, uint32_t pop, int16_t job_type, s_colony_job_production* out);

    inline char* __cdecl Format_Quotient_(int value, int divisor) {
        using Fn = char*(__cdecl*)(int, int);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004f2e40})(value, divisor);
    }

    void __cdecl Colony_Food_Production_(s_colony* colony, char* debug_out);

    int16_t __cdecl Colony_Empire_Base_Food2_Produced_(s_colony* colony, int16_t owner_idx, int16_t player_idx, s_colony_job_production* out);

    int16_t __cdecl Colony_Empire_Base_Industry_Produced_(s_colony* colony, uint16_t type, int16_t player_idx, s_colony_job_production* out);

    int16_t __cdecl Colony_Empire_Base_Research_Produced_(s_colony* colony, int16_t research_type, int16_t player_idx, s_colony_job_production* out);

    void __cdecl Colony_Food_Maintenance_(s_colony* colony);

    void __cdecl Colony_Replicators_(s_colony* colony);

    void __cdecl Colony_Research_Production_(s_colony* colony, char* debug_out);

    inline const char* s_str = reinterpret_cast<const char*>(std::uintptr_t{0x00551130}); // "%s"
    inline const char* s_newline = reinterpret_cast<const char*>(std::uintptr_t{0x0055136C}); // "\r"
    inline const char* s_tab_s_s_r = reinterpret_cast<const char*>(std::uintptr_t{0x0055A8B0}); // "\t%s\t%s\r"
    inline const char* s_tab_d_s_s_r = reinterpret_cast<const char*>(std::uintptr_t{0x0055A8D0}); // "\t%+d\t%s %s\r"
    inline const char* s_tab_d_s_r = reinterpret_cast<const char*>(std::uintptr_t{0x0055A8DC}); // "\t%+d\t%s\r"
    inline const char* s_tab_plus_d_s_r = reinterpret_cast<const char*>(std::uintptr_t{0x0055A8FC}); // "\t+%d\t%s\r"

    void __cdecl Colony_Industry_To_Tax_(s_colony* colony);

    void __cdecl Colony_BC_Production_(s_colony* colony, char* debug_buf);

    void __cdecl Colony_BC_Maintenance_(s_colony* colony);

    bool __cdecl Colony_Is_Blockaded(s_colony* colony);

    int __cdecl Calc_Player_Climate_(int player_idx, int climate);

    int __cdecl Size_And_Climate_Max_Population_(int8_t size, int climate, bool is_immune);

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

    void __cdecl Compute_Player_Maintenance_(s_player* plr, char* debug_out);

    int16_t __cdecl Gravity_Player_Production_Factor_(s_colony* colony, int16_t player_idx);

    int16_t __cdecl Get_Govt_Type_Tech_App_(int16_t govt_type);

    void __cdecl Compute_Potential_Colony_Info_(s_player* plr, s_planet_data* planet, s_potential_colony_info* results);

    int16_t __cdecl Planet_Max_Population_For_Player_(s_planet_data* planet, int16_t player_idx);

    bool __cdecl Colony_Is_At_Max_Population_(s_colony* colony);

    int16_t __cdecl N_Colonies_And_Outposts_At_Star_(int16_t star_index);

    int16_t N_Living_Human_Players_();

    int16_t N_Living_Players_();

    void __cdecl Check_For_Eliminated_Players_();

    void __cdecl Apply_All_Player_Changes_();

    void __cdecl Check_For_Research_Breakthrough_(s_player* player);

    void __cdecl Give_Player_Field_(s_player* player, int field_id, char get_all_flag);

    void __cdecl Compute_Player_Total_Known_Tech_Cost_(s_player* player);
}