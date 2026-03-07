typedef unsigned char undefined;
typedef unsigned char   undefined;

typedef unsigned char    byte;
typedef unsigned int    dword;

typedef long long    int16;
typedef long long    longlong;
typedef unsigned char    uchar;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned long long    ulonglong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined5;
typedef unsigned long long    undefined6;
typedef unsigned long long    undefined7;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
typedef unsigned short    word;
typedef struct s_adv_civ_player s_adv_civ_player, *Ps_adv_civ_player;


typedef struct s_adv_civ_player_star s_adv_civ_player_star, *Ps_adv_civ_player_star;

typedef struct s_adv_civ_player_planet s_adv_civ_player_planet, *Ps_adv_civ_player_planet;

typedef int int32_t;

struct s_adv_civ_player_planet {
    int8_t is_homeworld;
    short proximity_bonus;
    int32_t worth;
    short planet_idx;
    int8_t is_settled;
};

struct s_adv_civ_player_star {
    short star_idx;
    short min_dist_to_colony;
};

struct s_adv_civ_player {
    short candidate_star_count;
    short candidate_planet_count;
    short settled_planet_count;
    short furthest_star_dist_from_home;
    int8_t has_planet[360];
    struct s_adv_civ_player_star stars[72];
    struct s_adv_civ_player_planet planets[360];
};

typedef struct s_adv_planet_worth s_adv_planet_worth, *Ps_adv_planet_worth;

typedef struct s_adv_planet_worth_s1 s_adv_planet_worth_s1, *Ps_adv_planet_worth_s1;

struct s_adv_planet_worth_s1 {
    short planet_idx;
    int worth;
};

struct s_adv_planet_worth {
    int8_t num_planets;
    struct s_adv_planet_worth_s1 planets[20];
};

typedef struct s_ai_colonizable_planet s_ai_colonizable_planet, *Ps_ai_colonizable_planet;

struct s_ai_colonizable_planet {
    short planet_idx;
    int contextual_value;
};

typedef struct s_ai_colony_info s_ai_colony_info, *Ps_ai_colony_info;

struct s_ai_colony_info {
    int8_t race;
    int8_t pop_limit;
    int8_t trait_eats_minerals;
    int8_t trait_tolerant;
    int8_t trait_environment_immune;
    int8_t threatened;
    int8_t n_flagged_pops;
};

typedef struct s_ai_player_info s_ai_player_info, *Ps_ai_player_info;

struct s_ai_player_info {
    short colony_count;
    short colony_deficit;
    short colony_blockaded_count;
    short n_stars_with_colony;
    short n_blockaded_stars;
    short n_contacts;
    short n_reachable_players;
    int8_t reachable_players[8];
    undefined field8_0x16;
    undefined field9_0x17;
    short prev_n_contacts;
    int32_t total_ship_strength[8];
};

typedef struct s_ai_refit_entry s_ai_refit_entry, *Ps_ai_refit_entry;

struct s_ai_refit_entry {
    short ship_idx;
    int desirability;
};

typedef struct s_ai_refit_player s_ai_refit_player, *Ps_ai_refit_player;

struct s_ai_refit_player {
    short n_entries;
    struct s_ai_refit_entry entry[500];
};

typedef struct s_ai_star_info s_ai_star_info, *Ps_ai_star_info;

typedef struct s_ai_star_player_block s_ai_star_player_block, *Ps_ai_star_player_block;

struct s_ai_star_player_block {
    int8_t enemy_count;
    int8_t neutral_count;
    int8_t alliance_count;
    int8_t out_of_range_count;
    undefined1 nearest_eta;
    int8_t enemy_enroute_or_threat;
};

struct s_ai_star_info {
    int8_t n_planets_present;
    struct s_ai_star_player_block per_player[8];
};

typedef struct s_ai_tech_info s_ai_tech_info, *Ps_ai_tech_info;

typedef struct s_ai_tech_info_player s_ai_tech_info_player, *Ps_ai_tech_info_player;

struct s_ai_tech_info_player {
    int8_t best_shield;
    int8_t best_shield_delta;
    int8_t best_armor;
    int8_t armor_delta;
    int8_t best_computer;
    int8_t best_computer_delta;
    int8_t best_fuel;
    int8_t best_planet_shield;
    int8_t planet_shield_delta;
    int8_t personality_mod;
    int8_t contact_count;
    int8_t best_drive;
    int8_t best_weapon1;
    int8_t best_weapon1_delta;
    int8_t best_weapon2;
    int8_t best_weapon2_delta;
    int8_t groun_tech_bonus;
    short ground_bonus;
    int8_t ground_bonus_delta;
    int8_t colonization_idle;
};

struct s_ai_tech_info {
    struct s_ai_tech_info_player players[8];
};

typedef struct s_aidesign_ship_design s_aidesign_ship_design, *Ps_aidesign_ship_design;

struct s_aidesign_ship_design {
    short theme_special;
    short beam_special;
    short missile_special;
    short defense_special;
    short special_weapon;
    short special_device;
    short fighters;
    short missile_weapon;
    short heavy_beams;
    short beams;
    short point_defense_beams;
    short bombs;
};

typedef struct s_aidudes_colony_info s_aidudes_colony_info, *Ps_aidudes_colony_info;

struct s_aidudes_colony_info {
    int8_t colony_idx;
    int8_t n_farmers;
    int8_t n_workers;
    int8_t first_assignable_pop_idx;
    int8_t cur_pop_idx;
    int8_t pop_end_idx;
    int8_t pop_tail_idx;
    int8_t pollution_at_start;
    short best_worker_to_farmer_score;
    short worst_worker_to_farmer_score;
    int8_t eats_minerals;
};

typedef struct s_alloc_header s_alloc_header, *Ps_alloc_header;

typedef uint uint32_t;

struct s_alloc_header {
    uint32_t current;
    uint32_t size;
    uint32_t reserved;
};

typedef struct s_animation_header s_animation_header, *Ps_animation_header;

typedef short int16_t;

typedef uchar uint8_t;

struct s_animation_header {
    int16_t width;
    int16_t height;
    int16_t current_frame;
    int16_t frame_count;
    uint8_t loop_frame;
    short field5_0x9;
    undefined1 flags;
    uint32_t frame_offsets[0]; /* Dynamic length, varies by file */
};

typedef struct s_antaran s_antaran, *Ps_antaran;

struct s_antaran {
    short attack_pressure;
    short resource_offensive;
    short resource_defensive;
    short defensive_ships[9];
    short offensive_ships[9];
    short offensive_deployed[9];
    undefined1 field6_0x3c;
    int invasion_stardate;
    undefined1 field8_0x41;
};

typedef struct s_big_icon s_big_icon, *Ps_big_icon;

struct s_big_icon {
    short grid_x;
    short grid_y;
    int8_t selected;
    short screen_x;
    short screen_y;
    short button_id;
    short ship_idx;
};

typedef struct s_bomb s_bomb, *Ps_bomb;

typedef char sbyte;

struct s_bomb {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    sbyte field10_0xa;
    short field11_0xb;
    undefined field12_0xd;
    undefined field13_0xe;
};

typedef struct s_build_ship_icon_item s_build_ship_icon_item, *Ps_build_ship_icon_item;

struct s_build_ship_icon_item {
    int8_t player_idx;
    short ship_idx;
    short icon_idx;
};

typedef struct s_class_group s_class_group, *Ps_class_group;

struct s_class_group {
    int8_t planet_group_by_orbit[5];
};

typedef struct s_colony s_colony, *Ps_colony;


/* WARNING! conflicting data type names: /rpcndr.h/byte - /byte */

struct s_colony {
    char owner;
    char allocated_to;
    short planet;
    short officer;
    byte outpost_flag;
    char morale;
    short pollution;
    int8_t n_pops;
    char specialty;
    uint32_t pop[42];
    short pop_roundoff[10];
    short pop_growth[10];
    char n_turns_existed;
    char food2_per_farmer;
    char industry_per_worker;
    char research_per_scientist;
    char max_farms;
    char max_population;
    char climate;
    short ground_strength;
    short space_strength;
    short production[4];
    char maintenance[4];
    short imports[4];
    char n_industry_recyclers;
    char food2_needed_for_our_empire;
    char food2_needed_for_assimilated;
    char food2_needed_for_conquered;
    char food2_needed_for_natives;
    char industry2_needed_for_our_empire;
    char industry2_needed_for_androids;
    char industry2_needed_for_assimilated;
    char industry2_needed_for_conquered;
    char food2_needed_for_empire[8];
    char industry2_needed_for_empire[8];
    char n_food_replicated;
    short producing[7];
    short just_produced;
    short production_spent;
    short n_industry_taxed;
    char auto_building;
    short production_surplus;
    short bought_outright;
    char occupation_points;
    char occupation_policy;
    short military[2];
    char tank_roundoff;
    char infantry_roundoff;
    byte buildings[49];
    short last_turn_building_destroyed;
};

typedef struct s_colony_bldg s_colony_bldg, *Ps_colony_bldg;

struct s_colony_bldg {
    int32_t buildings[6];
};

typedef struct s_colony_job_production s_colony_job_production, *Ps_colony_job_production;

struct s_colony_job_production {
    int base_prod;
    short racial_bonuses[8];
    short android_bonuses[2];
    short tech_apps[212];
    short buildings_prod[49];
    int food_bonus;
    int artifacts_bonus;
    int intelligence_bonus;
    int base_prod_scaled;
    int gravity_penalty;
    int conquered_penalty;
    int pollution_penalty;
    int morale_bonus;
    int govt_bonus;
    int blockaded_penalty;
    int officer_bonus;
    int difficulty_bonus;
    int final_prod;
};

typedef struct s_colors s_colors, *Ps_colors;

struct s_colors {
    uint8_t colors[8];
};

typedef struct s_colsum_col_pop_ref s_colsum_col_pop_ref, *Ps_colsum_col_pop_ref;

struct s_colsum_col_pop_ref {
    short refs[42];
};

typedef struct s_combat_data s_combat_data, *Ps_combat_data;

typedef struct s_combat_weapon_slot s_combat_weapon_slot, *Ps_combat_weapon_slot;

typedef ushort uint16_t;

struct s_combat_weapon_slot {
    uint16_t weapon_id;
    uint8_t count;
    int8_t arc;
    uint16_t specials;
    int16_t ammo;
    int8_t active;
    int8_t count_max;
    uint8_t damage_flag;
};

struct s_combat_data {
    char name[16];
    undefined field1_0x10;
    undefined field2_0x11;
    undefined field3_0x12;
    undefined field4_0x13;
    undefined field5_0x14;
    undefined field6_0x15;
    undefined field7_0x16;
    undefined field8_0x17;
    undefined field9_0x18;
    undefined field10_0x19;
    undefined field11_0x1a;
    undefined field12_0x1b;
    undefined field13_0x1c;
    undefined field14_0x1d;
    int16_t ship_idx;
    int8_t owner;
    int8_t x;
    int8_t y;
    int8_t facing_dir;
    int8_t unit_status;
    int8_t size_class;
    int8_t shield_type;
    int16_t shield_arc_max;
    int16_t shield_arc_current[4];
    int8_t drive_type;
    int8_t computer_type;
    int8_t armor_type;
    int16_t tac_val_attack;
    int16_t tac_val_defense;
    int16_t tac_val_evade;
    int8_t combat_speed_base;
    int16_t current_speed;
    undefined field33_0x3d;
    undefined field34_0x3e;
    undefined field35_0x3f;
    int8_t special_status_flag;
    int8_t special_status_timer;
    int8_t special_cooldown;
    undefined field39_0x43;
    undefined field40_0x44;
    undefined field41_0x45;
    undefined field42_0x46;
    undefined field43_0x47;
    undefined field44_0x48;
    undefined field45_0x49;
    int8_t in_stasis;
    undefined field47_0x4b;
    uint8_t special_device_flags[5];
    undefined field49_0x51;
    struct s_combat_weapon_slot weapons[8];
    int16_t structure_max;
    int8_t crew_quality;
    int16_t officer_idx;
    int8_t armor_multiplier;
    undefined field55_0xb0;
    undefined field56_0xb1;
    uint8_t special_device_damage_flags[5];
    undefined field58_0xb7;
    uint8_t weapon_ready_flags[8];
    int16_t structure_damage;
    int16_t armor_remaining;
    int16_t picture_num;
    struct s_animation_header *ship_image_seg;
    int16_t drive_max_hits;
    int16_t computer_max_hits;
    undefined field66_0xce;
    undefined field67_0xcf;
    undefined field68_0xd0;
    undefined field69_0xd1;
    undefined field70_0xd2;
    undefined field71_0xd3;
    undefined field72_0xd4;
    undefined field73_0xd5;
    undefined field74_0xd6;
    undefined field75_0xd7;
    undefined field76_0xd8;
    undefined field77_0xd9;
    undefined field78_0xda;
    undefined field79_0xdb;
    undefined field80_0xdc;
    undefined field81_0xdd;
    undefined field82_0xde;
    undefined field83_0xdf;
    undefined field84_0xe0;
    undefined field85_0xe1;
    undefined field86_0xe2;
    undefined field87_0xe3;
    undefined field88_0xe4;
    undefined field89_0xe5;
    undefined field90_0xe6;
    undefined field91_0xe7;
    undefined field92_0xe8;
    undefined field93_0xe9;
    undefined field94_0xea;
    undefined field95_0xeb;
    undefined field96_0xec;
    undefined field97_0xed;
    undefined field98_0xee;
    undefined field99_0xef;
    undefined field100_0xf0;
    undefined field101_0xf1;
    undefined field102_0xf2;
    undefined field103_0xf3;
    undefined field104_0xf4;
    undefined field105_0xf5;
    undefined field106_0xf6;
    undefined field107_0xf7;
    undefined field108_0xf8;
    undefined field109_0xf9;
    undefined field110_0xfa;
    undefined field111_0xfb;
    undefined field112_0xfc;
    undefined field113_0xfd;
    int16_t shield_max_hits;
    int16_t drive_current_hits;
    int16_t computer_current_hits;
    undefined field117_0x104;
    undefined field118_0x105;
    undefined field119_0x106;
    undefined field120_0x107;
    undefined field121_0x108;
    undefined field122_0x109;
    undefined field123_0x10a;
    undefined field124_0x10b;
    undefined field125_0x10c;
    undefined field126_0x10d;
    undefined field127_0x10e;
    undefined field128_0x10f;
    undefined field129_0x110;
    undefined field130_0x111;
    undefined field131_0x112;
    undefined field132_0x113;
    undefined field133_0x114;
    undefined field134_0x115;
    undefined field135_0x116;
    undefined field136_0x117;
    undefined field137_0x118;
    undefined field138_0x119;
    undefined field139_0x11a;
    undefined field140_0x11b;
    undefined field141_0x11c;
    undefined field142_0x11d;
    undefined field143_0x11e;
    undefined field144_0x11f;
    undefined field145_0x120;
    undefined field146_0x121;
    undefined field147_0x122;
    undefined field148_0x123;
    undefined field149_0x124;
    undefined field150_0x125;
    undefined field151_0x126;
    undefined field152_0x127;
    undefined field153_0x128;
    undefined field154_0x129;
    undefined field155_0x12a;
    undefined field156_0x12b;
    undefined field157_0x12c;
    undefined field158_0x12d;
    undefined field159_0x12e;
    undefined field160_0x12f;
    undefined field161_0x130;
    undefined field162_0x131;
    undefined field163_0x132;
    undefined field164_0x133;
    short shield_current_hits;
    undefined field166_0x136;
    undefined field167_0x137;
    int8_t previous_owner;
};

typedef struct s_combfind_star_has_player s_combfind_star_has_player, *Ps_combfind_star_has_player;

struct s_combfind_star_has_player {
    int8_t ship_sizes[5];
    int8_t has_transport; /* Created by Rename Structure Field action */
    int8_t has_player;
    int8_t colony_count; /* Created by Rename Structure Field action */
    short transport_count;
    int8_t transport_unk; /* Created by Rename Structure Field action */
};

typedef struct s_crew_data s_crew_data, *Ps_crew_data;

struct s_crew_data {
    ushort experience;
    short attack;
    short defense;
    short ground;
};

typedef struct s_current_design s_current_design, *Ps_current_design;

struct s_current_design {
    char name[16];
    undefined field1_0x10;
    short shield;
    short shield_cost;
    short shield_space;
    undefined field5_0x17;
    undefined field6_0x18;
    undefined field7_0x19;
    undefined field8_0x1a;
    short ftl_type;
    short drive_speed;
    short drive_cost;
    short base_space;
    undefined field13_0x23;
    undefined field14_0x24;
    short base_combat_speed;
    undefined field16_0x27;
    undefined field17_0x28;
    short field18_0x29;
    short computer_type;
    short computer_cost;
    short fuel_type;
    short armor_type;
    short armor_cost;
    short weapon_type[8];
    short weapon_count[8];
    char weapon_firing_arc[8];
    short weapon_specials[8];
    int weapon_cost[8];
    int weapon_space[8];
    char weapon_ammo[8];
    short special_devices[8];
    short special_device_cost[8];
    short special_device_space[8];
    short ship_size;
    short picture_type;
    int hull_space;
    int space_used;
    int total_cost;
    int8_t show_help;
    short num_weapon_slots;
    short num_special_slots;
    int8_t item_unlocked[41];
    short date_of_design;
};

typedef struct s_event_data s_event_data, *Ps_event_data;

struct s_event_data {
    sbyte field0_0x0;
    sbyte field1_0x1;
    sbyte field2_0x2;
    short event_type;
    short field4_0x5;
    short field5_0x7;
};

typedef struct s_field s_field, *Ps_field;

typedef union s_field_detail s_field_detail, *Ps_field_detail;

typedef struct s_field_button s_field_button, *Ps_field_button;

typedef struct s_field_grid s_field_grid, *Ps_field_grid;

typedef struct s_field_radio s_field_radio, *Ps_field_radio;

typedef struct s_field_scroll s_field_scroll, *Ps_field_scroll;

typedef struct s_field_cont_input s_field_cont_input, *Ps_field_cont_input;

typedef struct s_field_multi_button s_field_multi_button, *Ps_field_multi_button;

struct s_field_grid {
    undefined field0_0x0;
    undefined field1_0x1;
    short field2_0x2;
    struct s_colors *highlight_color;
    char *help;
    short step_x;
    undefined field6_0xe;
    undefined field7_0xf;
    short step_y;
    undefined field9_0x12;
    undefined field10_0x13;
    short *grid_x_ptr;
    short *grid_y_ptr;
    struct s_animation_header *pic;
    short field14_0x20;
    undefined field15_0x22;
    undefined field16_0x23;
};

struct s_field_scroll {
    undefined field0_0x0;
    undefined field1_0x1;
    short field2_0x2;
    struct s_colors *highlight_color;
    short field4_0x8;
    undefined field5_0xa;
    undefined field6_0xb;
    short field7_0xc;
    undefined field8_0xe;
    undefined field9_0xf;
    short *value;
    short field11_0x14;
    undefined field12_0x16;
    undefined field13_0x17;
    short field14_0x18;
    undefined field15_0x1a;
    undefined field16_0x1b;
    short field17_0x1c;
    undefined field18_0x1e;
    undefined field19_0x1f;
    short field20_0x20;
    undefined field21_0x22;
    undefined field22_0x23;
};

struct s_field_multi_button {
    undefined field0_0x0;
    undefined field1_0x1;
    short field2_0x2;
    struct s_colors *highlight_color;
    short field4_0x8;
    undefined field5_0xa;
    undefined field6_0xb;
    short count;
    undefined field8_0xe;
    undefined field9_0xf;
    short *value;
    short field11_0x14;
    undefined field12_0x16;
    undefined field13_0x17;
    short field14_0x18;
    undefined field15_0x1a;
    undefined field16_0x1b;
    short field17_0x1c;
    undefined field18_0x1e;
    undefined field19_0x1f;
    short field20_0x20;
    undefined field21_0x22;
    undefined field22_0x23;
};

struct s_field_button {
    undefined field0_0x0;
    undefined field1_0x1;
    short field2_0x2;
    struct s_colors *highlight_color;
    char *help;
    short help_start;
    undefined field6_0xe;
    undefined field7_0xf;
    short help_end;
    undefined field9_0x12;
    undefined field10_0x13;
    short full_store_flag;
    undefined field12_0x16;
    undefined field13_0x17;
    int field14_0x18;
    struct s_animation_header *pic;
    short field16_0x20;
    undefined field17_0x22;
    undefined field18_0x23;
};

struct s_field_cont_input {
    undefined field0_0x0;
    undefined field1_0x1;
    short field2_0x2;
    struct s_colors *highlight_color;
    char *help;
    int field5_0xc;
    short help_end;
    undefined field7_0x12;
    undefined field8_0x13;
    short full_store_flag;
    undefined field10_0x16;
    undefined field11_0x17;
    int field12_0x18;
    short field13_0x1c;
    undefined field14_0x1e;
    undefined field15_0x1f;
    short field16_0x20;
    undefined field17_0x22;
    undefined field18_0x23;
};

struct s_field_radio {
    undefined field0_0x0;
    undefined field1_0x1;
    short field2_0x2;
    struct s_colors *highlight_color;
    char *help;
    short help_start;
    undefined field6_0xe;
    undefined field7_0xf;
    char *selection;
    short field9_0x14;
    undefined field10_0x16;
    undefined field11_0x17;
    int field12_0x18;
    struct s_animation_header *pic;
    short field14_0x20;
    undefined field15_0x22;
    undefined field16_0x23;
};

union s_field_detail {
    struct s_field_button button;
    struct s_field_grid grid;
    struct s_field_radio radio;
    struct s_field_scroll scroll;
    struct s_field_cont_input cont_input;
    struct s_field_multi_button multibtn;
};

struct s_field {
    short x;
    short y;
    short x_end;
    short y_end;
    short field_type;
    short font_style;
    struct s_colors *font_color;
    union s_field_detail detail;
    char hotkey;
    short sound;
};

typedef struct s_first_field_in_group s_first_field_in_group, *Ps_first_field_in_group;

struct s_first_field_in_group {
    uint8_t biology_field1;
    uint8_t power_field1;
    uint8_t physics_field1;
    uint8_t construction_field1;
    uint8_t force_fields_field1;
    uint8_t chemistry_field1;
    uint8_t computers_field1;
    uint8_t sociology_field1;
    uint8_t xenon_field1;
};

typedef struct s_fleet_box s_fleet_box, *Ps_fleet_box;

struct s_fleet_box {
    short x;
    short y;
    short height;
    short width;
    short map_height;
    undefined field5_0xa;
    undefined field6_0xb;
    short count;
    short field8_0xe;
    short field9_0x10;
    struct s_animation_header *name_sm_seg;
    struct s_animation_header *name_sm_seg2;
    int8_t is_small_frame;
    int8_t mouse_pressed;
    int8_t field14_0x1c;
    int8_t field15_0x1d;
    int8_t field16_0x1e;
    int8_t field17_0x1f;
    int8_t field18_0x20;
};

typedef struct s_fleet_movement_icon s_fleet_movement_icon, *Ps_fleet_movement_icon;

struct s_fleet_movement_icon {
    int8_t field0_0x0;
    short x;
    short y;
    short field;
    short ship_node_idx;
};

typedef struct s_fmtpara_break s_fmtpara_break, *Ps_fmtpara_break;

struct s_fmtpara_break {
    char *break_str_next;
    short break_x;
    short break_x_candidate;
    short insert_case;
    short line_y_offset;
    short font_spacing;
    short nest_depth;
    short char_count;
    short space_count;
    bool break_is_backspace;
    bool break_valid;
    short x;
    short left_limit_x;
    short right_limit_x;
    short exclusion_left_x;
    short exclusion_right_x;
    short x_candidate;
    int8_t line_char_count;
};

typedef struct s_fmtpara_char s_fmtpara_char, *Ps_fmtpara_char;

struct s_fmtpara_char {
    short x_advance;
    short y;
    uint8_t font_id;
    uint8_t fg_and_attr;
    uint8_t bg_color;
    char ch;
};

typedef struct s_fmtpara_global s_fmtpara_global, *Ps_fmtpara_global;

struct s_fmtpara_global {
    short min_x;
    short min_y;
    short max_x;
    short max_y;
    void *item_ptr[16];
    int8_t item_type[16];
    void *user_fn;
    bool cond_flag;
    bool use_plural;
};

typedef struct s_fmtpara_line s_fmtpara_line, *Ps_fmtpara_line;

struct s_fmtpara_line {
    short x;
    short left_limit_x;
    short right_limit_x;
    short exclusion_left_x;
    short exclusion_right_x;
    short x_candidate;
    short char_count;
    short space_count;
    short line_y_offset;
    short prev_x;
    short left_offset_abs;
    short font_spacing;
    bool line_done;
    int8_t line_started;
};

typedef struct s_fmtpara_para s_fmtpara_para, *Ps_fmtpara_para;

struct s_fmtpara_para {
    short para_y;
    char *str;
    short x1;
    short x2;
    short y;
    short y_origin;
    short width;
    struct s_animation_header *seg;
    bool do_render;
    short line_start_x;
    short line_right_limit_x;
    short y2_limit;
    undefined field12_0x1b;
    undefined field13_0x1c;
    short line_advance_y;
    short para_advance_y;
    short first_line_indent;
    short right_indent;
    short left_indent;
    int8_t justify_mode;
    int8_t paragraph_break;
    int16_t nest_depth;
    char *nest_stack[9];
    struct s_fmtpara_line *line;
};

typedef struct s_fmtpara_style s_fmtpara_style, *Ps_fmtpara_style;

struct s_fmtpara_style {
    int8_t font_id;
    undefined1 highlight;
    uint8_t fg_color;
    uint8_t bg_color;
    uint8_t color_attr;
    short font_height_px;
    int8_t *char_widths;
    short space_width;
};

typedef struct s_font s_font, *Ps_font;

struct s_font {
    byte selected_colors[8];
    short font_style_height;
    short font_style;
    byte normal_colors[8];
    byte highlight_colors[8];
    byte special_colors[8];
    uint16_t current_height;
    uint16_t line_height;
    uint16_t leading_offset;
    short font_style_spacing;
    char font_style_widths[256];
    uint32_t font_style_offsets[256];
    short font_heights[8];
    short font_spacing[8];
    short leading_extra[8];
    byte font_glyph_widths[6][256];
    uint32_t font_glyph_offsets[6][256];
};

typedef struct s_font_color s_font_color, *Ps_font_color;

struct s_font_color {
    uint8_t field1;
    uint32_t field2;
    uint16_t field6;
    uint8_t field7;
};

typedef struct s_game_anims s_game_anims, *Ps_game_anims;

struct s_game_anims {
    struct s_animation_header *pictures[25];
    int8_t animating;
};

typedef struct s_game_popup_fields s_game_popup_fields, *Ps_game_popup_fields;

typedef struct s_save_game_description s_save_game_description, *Ps_save_game_description;

struct s_save_game_description {
    char description[37];
};

struct s_game_popup_fields {
    int32_t slot_occupied_flags[10];
    short btn_main_load_id;
    short btn_main_save_id;
    short btn_main_quit_id;
    short btn_main_new_id;
    short btn_main_options_id;
    short btn_main_close_id;
    undefined field7_0x34;
    undefined field8_0x35;
    undefined field9_0x36;
    undefined field10_0x37;
    short saved_game_fields[10];
    short debug_hotspot_id;
    undefined field13_0x4e;
    undefined field14_0x4f;
    undefined field15_0x50;
    undefined field16_0x51;
    short option_names[13];
    short btn_settings_accept_id;
    short btn_popup_close_id;
    short screen_cancel_field_id;
    short field_space;
    short music_level;
    short sound_fx_level;
    short music_on;
    ushort sound_fx_on;
    short save_slot_fields[10];
    ushort options[13];
    int8_t slot_status[10];
    struct s_save_game_description descriptions[10];
    int8_t saved_game_types[10];
    short btn_popup_load_id;
    short btn_popup_save_id;
    struct s_colors color_normal;
    struct s_colors color_active;
    short option_toggles[13];
    short language;
    undefined field37_0x260;
    undefined field38_0x261;
    undefined field39_0x262;
    undefined field40_0x263;
    undefined field41_0x264;
    undefined field42_0x265;
    undefined field43_0x266;
    undefined field44_0x267;
    undefined field45_0x268;
    undefined field46_0x269;
    undefined field47_0x26a;
    undefined field48_0x26b;
    undefined field49_0x26c;
    undefined field50_0x26d;
    undefined field51_0x26e;
    undefined field52_0x26f;
    undefined field53_0x270;
    undefined field54_0x271;
    undefined field55_0x272;
    undefined field56_0x273;
    undefined field57_0x274;
    undefined field58_0x275;
    undefined field59_0x276;
    undefined field60_0x277;
};

typedef struct s_global_combat_data s_global_combat_data, *Ps_global_combat_data;

struct s_global_combat_data {
    char name[30];
    int16_t design_idx;
    int8_t player_idx;
    undefined field3_0x21;
    undefined field4_0x22;
    undefined field5_0x23;
    undefined field6_0x24;
    int8_t hull_size;
    int8_t shield_type;
    int16_t shield_points_base;
    int16_t shield_quadrants[4];
    int8_t ftl_type;
    int8_t computer_type;
    int8_t armor_type;
    short ship_attack;
    short ship_defense;
    short ship_missle_defense;
    undefined field17_0x3a;
    int16_t tactical_speed;
    undefined field19_0x3d;
    undefined field20_0x3e;
    undefined field21_0x3f;
    undefined field22_0x40;
    undefined field23_0x41;
    undefined field24_0x42;
    undefined field25_0x43;
    undefined field26_0x44;
    undefined field27_0x45;
    undefined field28_0x46;
    undefined field29_0x47;
    undefined field30_0x48;
    undefined field31_0x49;
    undefined field32_0x4a;
    undefined field33_0x4b;
    uint8_t special_device_flags[5];
    undefined field35_0x51;
    struct s_combat_weapon_slot weapons[8];
    int16_t structure_max;
    int8_t crew_quality;
    int16_t officer_idx;
    undefined field40_0xaf;
    undefined field41_0xb0;
    undefined field42_0xb1;
    int8_t special_device_damage[5];
    undefined field44_0xb7;
    uint8_t weapon_slot_active[8];
    int16_t structure_damage;
    int16_t armor_hits_current;
    int16_t picture_num;
    undefined field49_0xc6;
    undefined field50_0xc7;
    undefined field51_0xc8;
    undefined field52_0xc9;
    undefined field53_0xca;
    undefined field54_0xcb;
    undefined field55_0xcc;
    undefined field56_0xcd;
    undefined field57_0xce;
    undefined field58_0xcf;
    undefined field59_0xd0;
    undefined field60_0xd1;
    undefined field61_0xd2;
    undefined field62_0xd3;
    undefined field63_0xd4;
    undefined field64_0xd5;
    undefined field65_0xd6;
    undefined field66_0xd7;
    undefined field67_0xd8;
    undefined field68_0xd9;
    undefined field69_0xda;
    undefined field70_0xdb;
    undefined field71_0xdc;
    undefined field72_0xdd;
    undefined field73_0xde;
    undefined field74_0xdf;
    undefined field75_0xe0;
    undefined field76_0xe1;
    undefined field77_0xe2;
    undefined field78_0xe3;
    undefined field79_0xe4;
    undefined field80_0xe5;
    undefined field81_0xe6;
    undefined field82_0xe7;
    undefined field83_0xe8;
    undefined field84_0xe9;
    undefined field85_0xea;
    undefined field86_0xeb;
    undefined field87_0xec;
    undefined field88_0xed;
    undefined field89_0xee;
    undefined field90_0xef;
    undefined field91_0xf0;
    undefined field92_0xf1;
    undefined field93_0xf2;
    undefined field94_0xf3;
    undefined field95_0xf4;
    undefined field96_0xf5;
    undefined field97_0xf6;
    undefined field98_0xf7;
    undefined field99_0xf8;
    undefined field100_0xf9;
    undefined field101_0xfa;
    undefined field102_0xfb;
    undefined field103_0xfc;
    undefined field104_0xfd;
    undefined field105_0xfe;
    undefined field106_0xff;
    int16_t drive_effectiveness_pct;
    int16_t computer_effectiveness_pct;
    undefined field109_0x104;
    undefined field110_0x105;
    undefined field111_0x106;
    undefined field112_0x107;
    undefined field113_0x108;
    undefined field114_0x109;
    undefined field115_0x10a;
    undefined field116_0x10b;
    undefined field117_0x10c;
    undefined field118_0x10d;
    undefined field119_0x10e;
    undefined field120_0x10f;
    undefined field121_0x110;
    undefined field122_0x111;
    undefined field123_0x112;
    undefined field124_0x113;
    undefined field125_0x114;
    undefined field126_0x115;
    undefined field127_0x116;
    undefined field128_0x117;
    undefined field129_0x118;
    undefined field130_0x119;
    undefined field131_0x11a;
    undefined field132_0x11b;
    undefined field133_0x11c;
    undefined field134_0x11d;
    undefined field135_0x11e;
    undefined field136_0x11f;
    undefined field137_0x120;
    undefined field138_0x121;
    undefined field139_0x122;
    undefined field140_0x123;
    undefined field141_0x124;
    undefined field142_0x125;
    undefined field143_0x126;
    undefined field144_0x127;
    undefined field145_0x128;
    undefined field146_0x129;
    undefined field147_0x12a;
    undefined field148_0x12b;
    undefined field149_0x12c;
    undefined field150_0x12d;
    undefined field151_0x12e;
    undefined field152_0x12f;
    undefined field153_0x130;
    undefined field154_0x131;
    undefined field155_0x132;
    undefined field156_0x133;
    undefined field157_0x134;
    undefined field158_0x135;
    undefined field159_0x136;
    undefined field160_0x137;
    int8_t previous_owner;
};

typedef struct s_good_star s_good_star, *Ps_good_star;

struct s_good_star {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    uint32_t star_distances[72];
    uint16_t star_count;
};

typedef struct s_invasion_player_bonuses s_invasion_player_bonuses, *Ps_invasion_player_bonuses;

struct s_invasion_player_bonuses {
    int8_t antigrav_bonus;
    int8_t battleoids_bonus;
    int8_t has_battleoids;
    int8_t powered_armor_bonus;
    int8_t has_powered_armor;
    int8_t best_armor_tech_app_id;
    int8_t troop_bonus;
    int8_t personal_shield_tech_app_id;
    undefined1 personal_shield_bonus;
    int8_t rifle_tech_app_id;
    int8_t rifle_bonus;
    int8_t ground_combat_bonus;
    undefined1 heavy_g_bonus;
    int8_t low_g_penalty;
    undefined1 subterranean_bonus;
    int8_t difficulty_bonus;
    short field16_0x10;
    int8_t field17_0x12;
};

typedef struct s_lbx_header s_lbx_header, *Ps_lbx_header;

struct s_lbx_header {
    uint16_t entry_count;
    uint16_t magic;
    uint32_t unknown4;
    uint32_t offsets[510];
};

typedef struct s_leader_data s_leader_data, *Ps_leader_data;

struct s_leader_data {
    char name[15];
    char title[20];
    char type;
    short xp;
    uint32_t general_skills;
    uint32_t special_skills;
    byte tech_application[3];
    char pict_num;
    short skill_value;
    char level;
    short location;
    sbyte eta;
    char display_level_popup;
    sbyte status;
    char player_index;
};

typedef struct s_main_menu s_main_menu, *Ps_main_menu;

typedef struct s_main_menu_button s_main_menu_button, *Ps_main_menu_button;

struct s_main_menu_button {
    struct s_animation_header *field0_0x0;
    struct s_animation_header *field1_0x4;
    struct s_animation_header *field2_0x8;
    short field;
};

struct s_main_menu {
    struct s_main_menu_button fields[6];
    undefined field1_0x54;
    undefined field2_0x55;
    undefined field3_0x56;
    undefined field4_0x57;
    undefined field5_0x58;
    undefined field6_0x59;
    undefined field7_0x5a;
    undefined field8_0x5b;
    undefined field9_0x5c;
    undefined field10_0x5d;
    undefined field11_0x5e;
    undefined field12_0x5f;
    undefined field13_0x60;
    undefined field14_0x61;
    undefined field15_0x62;
    undefined field16_0x63;
    undefined field17_0x64;
    undefined field18_0x65;
    undefined field19_0x66;
    undefined field20_0x67;
    undefined field21_0x68;
    undefined field22_0x69;
    undefined field23_0x6a;
    undefined field24_0x6b;
    undefined field25_0x6c;
    undefined field26_0x6d;
    undefined field27_0x6e;
    undefined field28_0x6f;
    undefined field29_0x70;
    undefined field30_0x71;
    undefined field31_0x72;
    undefined field32_0x73;
    undefined field33_0x74;
    undefined field34_0x75;
    undefined field35_0x76;
    undefined field36_0x77;
    undefined field37_0x78;
    undefined field38_0x79;
    undefined field39_0x7a;
    undefined field40_0x7b;
    undefined field41_0x7c;
    undefined field42_0x7d;
    undefined field43_0x7e;
    undefined field44_0x7f;
    undefined field45_0x80;
    undefined field46_0x81;
    undefined field47_0x82;
    undefined field48_0x83;
    undefined field49_0x84;
    undefined field50_0x85;
    undefined field51_0x86;
    undefined field52_0x87;
    undefined field53_0x88;
    undefined field54_0x89;
    undefined field55_0x8a;
    undefined field56_0x8b;
    undefined field57_0x8c;
    undefined field58_0x8d;
    undefined field59_0x8e;
    undefined field60_0x8f;
    undefined field61_0x90;
    undefined field62_0x91;
    undefined field63_0x92;
    undefined field64_0x93;
    undefined field65_0x94;
    undefined field66_0x95;
    undefined field67_0x96;
    undefined field68_0x97;
    undefined field69_0x98;
    undefined field70_0x99;
    undefined field71_0x9a;
    undefined field72_0x9b;
    undefined field73_0x9c;
    undefined field74_0x9d;
    undefined field75_0x9e;
    undefined field76_0x9f;
    undefined field77_0xa0;
    undefined field78_0xa1;
    undefined field79_0xa2;
    undefined field80_0xa3;
    undefined field81_0xa4;
    undefined field82_0xa5;
    undefined field83_0xa6;
    undefined field84_0xa7;
    undefined field85_0xa8;
    undefined field86_0xa9;
    undefined field87_0xaa;
    undefined field88_0xab;
    undefined field89_0xac;
    undefined field90_0xad;
    undefined field91_0xae;
    undefined field92_0xaf;
    undefined field93_0xb0;
    undefined field94_0xb1;
    undefined field95_0xb2;
    undefined field96_0xb3;
    undefined field97_0xb4;
    undefined field98_0xb5;
    undefined field99_0xb6;
    undefined field100_0xb7;
    undefined field101_0xb8;
    undefined field102_0xb9;
    undefined field103_0xba;
    undefined field104_0xbb;
    undefined field105_0xbc;
    undefined field106_0xbd;
    undefined field107_0xbe;
    undefined field108_0xbf;
    undefined field109_0xc0;
    undefined field110_0xc1;
    undefined field111_0xc2;
    undefined field112_0xc3;
    undefined field113_0xc4;
    undefined field114_0xc5;
    undefined field115_0xc6;
    undefined field116_0xc7;
    undefined field117_0xc8;
    undefined field118_0xc9;
    undefined field119_0xca;
    undefined field120_0xcb;
    undefined field121_0xcc;
    undefined field122_0xcd;
    undefined field123_0xce;
    undefined field124_0xcf;
    undefined field125_0xd0;
    undefined field126_0xd1;
};

typedef struct s_missle s_missle, *Ps_missle;

struct s_missle {
    short type;
    short owner;
    short field2_0x4;
    short target_missle_idx;
    sbyte is_anti_missile_rocket;
    undefined field5_0x9;
    undefined field6_0xa;
    undefined field7_0xb;
    undefined field8_0xc;
    undefined field9_0xd;
    undefined field10_0xe;
    undefined field11_0xf;
    undefined field12_0x10;
    undefined field13_0x11;
    undefined field14_0x12;
    undefined field15_0x13;
    undefined field16_0x14;
    undefined field17_0x15;
    undefined field18_0x16;
    undefined field19_0x17;
    undefined field20_0x18;
    undefined field21_0x19;
};

typedef struct s_modem_settings s_modem_settings, *Ps_modem_settings;

struct s_modem_settings {
    uint32_t com_port_index;
    uint32_t baud_rate;
    uint8_t parity;
    uint32_t data_bits;
    uint32_t serial_cfg_ext;
    char cmd_reset[30];
    char cmd_init[30];
    char cmd_hangup[30];
    char dial_number[24];
};

typedef struct s_mouse_xy s_mouse_xy, *Ps_mouse_xy;

struct s_mouse_xy {
    short ship_icon_idx;
    short field1_0x2;
    short star_idx;
    short field3_0x6;
    int8_t selected;
};

typedef struct s_movable_box s_movable_box, *Ps_movable_box;

struct s_movable_box {
    int16_t index;
    int16_t off_flag;
    int16_t height;
    int16_t width;
    int16_t x;
    int16_t y;
    int16_t active;
    int16_t field;
    int16_t field8_0x10;
    int16_t field9_0x12;
    int16_t field10_0x14;
    int16_t field11_0x16;
    int16_t content_offset_x;
    int16_t content_offset_y;
};

typedef struct s_mp_screen s_mp_screen, *Ps_mp_screen;

struct s_mp_screen {
    char signature[13];
    undefined field1_0xd;
    undefined field2_0xe;
    struct s_animation_header *pic;
    undefined field4_0x13;
    undefined field5_0x14;
    undefined field6_0x15;
    undefined field7_0x16;
    undefined field8_0x17;
    undefined field9_0x18;
    undefined field10_0x19;
    undefined field11_0x1a;
    undefined field12_0x1b;
    undefined field13_0x1c;
    undefined field14_0x1d;
    undefined field15_0x1e;
    undefined field16_0x1f;
    undefined field17_0x20;
    undefined field18_0x21;
    undefined field19_0x22;
    undefined field20_0x23;
    undefined field21_0x24;
    undefined field22_0x25;
    undefined field23_0x26;
    undefined field24_0x27;
    undefined field25_0x28;
    undefined field26_0x29;
    undefined field27_0x2a;
    undefined field28_0x2b;
    undefined field29_0x2c;
    undefined field30_0x2d;
    undefined field31_0x2e;
    struct s_animation_header *bg_seg;
    undefined field33_0x33;
    undefined field34_0x34;
    undefined field35_0x35;
    undefined field36_0x36;
    undefined field37_0x37;
    undefined field38_0x38;
    undefined field39_0x39;
    undefined field40_0x3a;
    undefined field41_0x3b;
    undefined field42_0x3c;
    undefined field43_0x3d;
    undefined field44_0x3e;
    undefined field45_0x3f;
    undefined field46_0x40;
    undefined field47_0x41;
    undefined field48_0x42;
    undefined field49_0x43;
    undefined field50_0x44;
    undefined field51_0x45;
    undefined field52_0x46;
    undefined field53_0x47;
    undefined field54_0x48;
    undefined field55_0x49;
    undefined field56_0x4a;
    undefined field57_0x4b;
    undefined field58_0x4c;
    undefined field59_0x4d;
    undefined field60_0x4e;
    undefined field61_0x4f;
    undefined field62_0x50;
    undefined field63_0x51;
    undefined field64_0x52;
    struct s_animation_header *btn_accept_off;
    struct s_animation_header *btn_accept_on;
    undefined field67_0x5b;
    undefined field68_0x5c;
    undefined field69_0x5d;
    undefined field70_0x5e;
    undefined field71_0x5f;
    undefined field72_0x60;
    undefined field73_0x61;
    undefined field74_0x62;
    undefined field75_0x63;
    undefined field76_0x64;
    undefined field77_0x65;
    undefined field78_0x66;
    undefined field79_0x67;
    undefined field80_0x68;
    undefined field81_0x69;
    undefined field82_0x6a;
    undefined field83_0x6b;
    undefined field84_0x6c;
    undefined field85_0x6d;
    undefined field86_0x6e;
    undefined field87_0x6f;
    undefined field88_0x70;
    undefined field89_0x71;
    undefined field90_0x72;
    undefined field91_0x73;
    undefined field92_0x74;
    undefined field93_0x75;
    undefined field94_0x76;
    undefined field95_0x77;
    undefined field96_0x78;
    undefined field97_0x79;
    undefined field98_0x7a;
    undefined field99_0x7b;
    undefined field100_0x7c;
    undefined field101_0x7d;
    undefined field102_0x7e;
    undefined field103_0x7f;
    undefined field104_0x80;
    undefined field105_0x81;
    undefined field106_0x82;
    undefined field107_0x83;
    undefined field108_0x84;
    undefined field109_0x85;
    undefined field110_0x86;
    undefined field111_0x87;
    undefined field112_0x88;
    undefined field113_0x89;
    undefined field114_0x8a;
    undefined field115_0x8b;
    undefined field116_0x8c;
    undefined field117_0x8d;
    undefined field118_0x8e;
    undefined field119_0x8f;
    undefined field120_0x90;
    undefined field121_0x91;
    undefined field122_0x92;
    undefined field123_0x93;
    undefined field124_0x94;
    undefined field125_0x95;
    undefined field126_0x96;
    undefined field127_0x97;
    undefined field128_0x98;
    undefined field129_0x99;
    undefined field130_0x9a;
    undefined field131_0x9b;
    undefined field132_0x9c;
    undefined field133_0x9d;
    undefined field134_0x9e;
    undefined field135_0x9f;
    undefined field136_0xa0;
    undefined field137_0xa1;
    undefined field138_0xa2;
    undefined field139_0xa3;
    undefined field140_0xa4;
    undefined field141_0xa5;
    undefined field142_0xa6;
    int16_t game_list_field_ids[10];
    undefined field144_0xbb;
    undefined field145_0xbc;
    undefined field146_0xbd;
    undefined field147_0xbe;
    undefined field148_0xbf;
    undefined field149_0xc0;
    undefined field150_0xc1;
    undefined field151_0xc2;
    undefined field152_0xc3;
    undefined field153_0xc4;
    undefined field154_0xc5;
    undefined field155_0xc6;
    undefined field156_0xc7;
    undefined field157_0xc8;
    undefined field158_0xc9;
    undefined field159_0xca;
    undefined field160_0xcb;
    undefined field161_0xcc;
    undefined field162_0xcd;
    undefined field163_0xce;
    undefined field164_0xcf;
    undefined field165_0xd0;
    undefined field166_0xd1;
    undefined field167_0xd2;
    undefined field168_0xd3;
    undefined field169_0xd4;
    int16_t field_id_close_dropdown;
    int16_t field_id_ten;
    int16_t field_id_modem;
    int16_t field_id_serial;
    int16_t field_id_network;
    int16_t field_id_cancel;
    int16_t field_id_ok;
    int16_t field_id_next;
    int16_t field_id_accept;
    int16_t field_id_back;
    int16_t field_id_setup;
    int16_t field_id_help;
    undefined field182_0xed;
    undefined field183_0xee;
    undefined field184_0xef;
    undefined field185_0xf0;
    int16_t lbx_entry_num;
    int16_t screen_x;
    int16_t screen_y;
    int16_t flag_ten_disabled;
    int16_t flag_modem_disabled;
    int16_t flag_serial_type;
    int16_t flag_network_disabled;
    undefined field193_0xff;
    undefined field194_0x100;
    int16_t last_scan_id;
    undefined field196_0x103;
    undefined field197_0x104;
    undefined field198_0x105;
    undefined field199_0x106;
    undefined field200_0x107;
    undefined field201_0x108;
    undefined field202_0x109;
    undefined field203_0x10a;
    undefined field204_0x10b;
    int16_t bg_height;
    int8_t subscreen_mode;
    undefined field207_0x10f;
    undefined field208_0x110;
    undefined field209_0x111;
    undefined field210_0x112;
    undefined field211_0x113;
    undefined field212_0x114;
    undefined field213_0x115;
    undefined field214_0x116;
    undefined field215_0x117;
    undefined field216_0x118;
    undefined field217_0x119;
    undefined field218_0x11a;
    undefined field219_0x11b;
    undefined field220_0x11c;
    undefined field221_0x11d;
    undefined field222_0x11e;
    undefined field223_0x11f;
    undefined field224_0x120;
    undefined field225_0x121;
    undefined field226_0x122;
    undefined field227_0x123;
    undefined field228_0x124;
    undefined field229_0x125;
    undefined field230_0x126;
    undefined field231_0x127;
    undefined field232_0x128;
    undefined field233_0x129;
    undefined field234_0x12a;
    undefined field235_0x12b;
    undefined field236_0x12c;
    undefined field237_0x12d;
    undefined field238_0x12e;
    undefined field239_0x12f;
    undefined field240_0x130;
    undefined field241_0x131;
    undefined field242_0x132;
    undefined field243_0x133;
    undefined field244_0x134;
    undefined field245_0x135;
    undefined field246_0x136;
    undefined field247_0x137;
    undefined field248_0x138;
    undefined field249_0x139;
    undefined field250_0x13a;
    undefined field251_0x13b;
    undefined field252_0x13c;
    undefined field253_0x13d;
    undefined field254_0x13e;
    undefined field255_0x13f;
    undefined field256_0x140;
    undefined field257_0x141;
    undefined field258_0x142;
    undefined field259_0x143;
    undefined field260_0x144;
    undefined field261_0x145;
    undefined field262_0x146;
    undefined field263_0x147;
    undefined field264_0x148;
    undefined field265_0x149;
    undefined field266_0x14a;
    undefined field267_0x14b;
    undefined field268_0x14c;
    undefined field269_0x14d;
    undefined field270_0x14e;
    undefined field271_0x14f;
    undefined field272_0x150;
    undefined field273_0x151;
    undefined field274_0x152;
    undefined field275_0x153;
    undefined field276_0x154;
    undefined field277_0x155;
    undefined field278_0x156;
    undefined field279_0x157;
    undefined field280_0x158;
    undefined field281_0x159;
    undefined field282_0x15a;
    undefined field283_0x15b;
    undefined field284_0x15c;
    undefined field285_0x15d;
    undefined field286_0x15e;
    undefined field287_0x15f;
    undefined field288_0x160;
    undefined field289_0x161;
    undefined field290_0x162;
    undefined field291_0x163;
    undefined field292_0x164;
    undefined field293_0x165;
    undefined field294_0x166;
    undefined field295_0x167;
    undefined field296_0x168;
    undefined field297_0x169;
    undefined field298_0x16a;
    undefined field299_0x16b;
    undefined field300_0x16c;
    undefined field301_0x16d;
    undefined field302_0x16e;
    undefined field303_0x16f;
    undefined field304_0x170;
    undefined field305_0x171;
    undefined field306_0x172;
    undefined field307_0x173;
    undefined field308_0x174;
    undefined field309_0x175;
    undefined field310_0x176;
    undefined field311_0x177;
    undefined field312_0x178;
    undefined field313_0x179;
    undefined field314_0x17a;
    undefined field315_0x17b;
    undefined field316_0x17c;
    undefined field317_0x17d;
    undefined field318_0x17e;
    undefined field319_0x17f;
    undefined field320_0x180;
    undefined field321_0x181;
    undefined field322_0x182;
    undefined field323_0x183;
    undefined field324_0x184;
    undefined field325_0x185;
    undefined field326_0x186;
    undefined field327_0x187;
    undefined field328_0x188;
    undefined field329_0x189;
    undefined field330_0x18a;
    undefined field331_0x18b;
    undefined field332_0x18c;
    undefined field333_0x18d;
    undefined field334_0x18e;
    undefined field335_0x18f;
    undefined field336_0x190;
    undefined field337_0x191;
    undefined field338_0x192;
    undefined field339_0x193;
    undefined field340_0x194;
    undefined field341_0x195;
    undefined field342_0x196;
    undefined field343_0x197;
    undefined field344_0x198;
    undefined field345_0x199;
    undefined field346_0x19a;
    undefined field347_0x19b;
    undefined field348_0x19c;
    undefined field349_0x19d;
    undefined field350_0x19e;
    undefined field351_0x19f;
    undefined field352_0x1a0;
    undefined field353_0x1a1;
    undefined field354_0x1a2;
    undefined field355_0x1a3;
    undefined field356_0x1a4;
    undefined field357_0x1a5;
    undefined field358_0x1a6;
    undefined field359_0x1a7;
    undefined field360_0x1a8;
    undefined field361_0x1a9;
    undefined field362_0x1aa;
    undefined field363_0x1ab;
    undefined field364_0x1ac;
    undefined field365_0x1ad;
    undefined field366_0x1ae;
    undefined field367_0x1af;
    undefined field368_0x1b0;
    undefined field369_0x1b1;
    undefined field370_0x1b2;
    undefined field371_0x1b3;
    undefined field372_0x1b4;
    undefined field373_0x1b5;
    undefined field374_0x1b6;
    undefined field375_0x1b7;
    undefined field376_0x1b8;
    undefined field377_0x1b9;
    undefined field378_0x1ba;
    undefined field379_0x1bb;
    undefined field380_0x1bc;
    undefined field381_0x1bd;
    undefined field382_0x1be;
    undefined field383_0x1bf;
    undefined field384_0x1c0;
    undefined field385_0x1c1;
    undefined field386_0x1c2;
    undefined field387_0x1c3;
    undefined field388_0x1c4;
    undefined field389_0x1c5;
    undefined field390_0x1c6;
    undefined field391_0x1c7;
    undefined field392_0x1c8;
    undefined field393_0x1c9;
    undefined field394_0x1ca;
    undefined field395_0x1cb;
    undefined field396_0x1cc;
    undefined field397_0x1cd;
    undefined field398_0x1ce;
    undefined field399_0x1cf;
    undefined field400_0x1d0;
    undefined field401_0x1d1;
    undefined field402_0x1d2;
    undefined field403_0x1d3;
    undefined field404_0x1d4;
    undefined field405_0x1d5;
    undefined field406_0x1d6;
    undefined field407_0x1d7;
    undefined field408_0x1d8;
    undefined field409_0x1d9;
    undefined field410_0x1da;
    undefined field411_0x1db;
    undefined field412_0x1dc;
    undefined field413_0x1dd;
    undefined field414_0x1de;
    undefined field415_0x1df;
    int8_t is_active;
    undefined field417_0x1e1;
    undefined field418_0x1e2;
    undefined field419_0x1e3;
    undefined field420_0x1e4;
    undefined field421_0x1e5;
    undefined field422_0x1e6;
    undefined field423_0x1e7;
    undefined field424_0x1e8;
    undefined field425_0x1e9;
    uint8_t anim_frame_timer;
};

typedef struct s_MSG s_MSG, *Ps_MSG;

typedef union s_MSG_union s_MSG_union, *Ps_MSG_union;

typedef struct s_MSG_colony s_MSG_colony, *Ps_MSG_colony;

typedef struct s_MSG_players s_MSG_players, *Ps_MSG_players;

typedef struct s_MSG_financial s_MSG_financial, *Ps_MSG_financial;

typedef struct s_MSG_lost_ship s_MSG_lost_ship, *Ps_MSG_lost_ship;

typedef struct s_MSG_scrap_ship s_MSG_scrap_ship, *Ps_MSG_scrap_ship;

typedef struct s_MSG_lost_colony s_MSG_lost_colony, *Ps_MSG_lost_colony;

typedef struct s_MSG_hyperspace_beast s_MSG_hyperspace_beast, *Ps_MSG_hyperspace_beast;

typedef struct s_MSG_lost_attacking_spy s_MSG_lost_attacking_spy, *Ps_MSG_lost_attacking_spy;

typedef struct s_MSG_lost_defending_spy s_MSG_lost_defending_spy, *Ps_MSG_lost_defending_spy;

typedef struct s_MSG_artemis s_MSG_artemis, *Ps_MSG_artemis;

typedef struct s_MSG_artemis_leader s_MSG_artemis_leader, *Ps_MSG_artemis_leader;

typedef struct s_MSG_failed_retreat s_MSG_failed_retreat, *Ps_MSG_failed_retreat;

typedef struct s_MSG_spy_assasination s_MSG_spy_assasination, *Ps_MSG_spy_assasination;

typedef struct s_MSG_anomaly s_MSG_anomaly, *Ps_MSG_anomaly;

typedef struct s_MSG_diplomacy s_MSG_diplomacy, *Ps_MSG_diplomacy;

typedef struct s_MSG_eel s_MSG_eel, *Ps_MSG_eel;

typedef struct s_MSG_artifacts s_MSG_artifacts, *Ps_MSG_artifacts;

typedef struct s_MSG_scrap_leader s_MSG_scrap_leader, *Ps_MSG_scrap_leader;

typedef union s_MSG_colony_union s_MSG_colony_union, *Ps_MSG_colony_union;

typedef struct s_MSG_colony_scrap_building s_MSG_colony_scrap_building, *Ps_MSG_colony_scrap_building;

typedef struct s_MSG_colony_build_info s_MSG_colony_build_info, *Ps_MSG_colony_build_info;

typedef struct s_MSG_colony_assimilation s_MSG_colony_assimilation, *Ps_MSG_colony_assimilation;

typedef struct s_MSG_colony_starvation s_MSG_colony_starvation, *Ps_MSG_colony_starvation;

typedef struct s_MSG_colony_settlers s_MSG_colony_settlers, *Ps_MSG_colony_settlers;

typedef struct s_MSG_colony_rebellion s_MSG_colony_rebellion, *Ps_MSG_colony_rebellion;

typedef struct s_MSG_colony_android_killed s_MSG_colony_android_killed, *Ps_MSG_colony_android_killed;

struct s_MSG_financial {
    int16_t a;
    int16_t b;
};

struct s_MSG_colony_scrap_building {
    int16_t building_id;
    int16_t bc;
};

struct s_MSG_lost_attacking_spy {
    int16_t player_idx;
};

struct s_MSG_colony_settlers {
    int8_t settlers;
    int8_t over_limit;
    bool blockaded;
};

struct s_MSG_players {
    int16_t player_a_idx;
    int16_t player_b_idx;
};

struct s_MSG_artemis {
    int16_t star_idx;
};

struct s_MSG_artifacts {
    uint8_t star_idx;
    int8_t tech1;
    int8_t tech2;
};

struct s_MSG_scrap_leader {
    int8_t leader_idx;
};

struct s_MSG_failed_retreat {
    int8_t star_idx;
    int8_t reason;
    int16_t ships_lost;
    int8_t leader_count;
};

struct s_MSG_artemis_leader {
    int8_t star_idx;
    int8_t leader_idx;
    int8_t fate;
};

struct s_MSG_eel {
    uint8_t star_idx;
};

struct s_MSG_scrap_ship {
    int16_t where;
    int16_t bc;
    int16_t ship_idx;
};

struct s_MSG_hyperspace_beast {
    int16_t ship_idx;
    int8_t star_idx;
    int8_t field2_0x3;
    int8_t leader_idx;
    int8_t fate;
    int8_t your_ship;
};

struct s_MSG_lost_ship {
    int16_t star_idx;
    int16_t ship_idx;
    int8_t leader_idx;
    int8_t fate;
};

struct s_MSG_colony_android_killed {
    int8_t race_idx;
};

struct s_MSG_colony_starvation {
    int8_t a;
    int8_t b;
};

struct s_MSG_colony_assimilation {
    uint8_t val;
};

struct s_MSG_colony_rebellion {
    int8_t owner_player;
    int8_t rebels;
    int8_t lost;
};

struct s_MSG_colony_build_info {
    int16_t build_id;
    int16_t product_id;
    bool is_refit;
    int8_t append_fmt;
};

union s_MSG_colony_union {
    struct s_MSG_colony_scrap_building scrap;
    struct s_MSG_colony_build_info build;
    struct s_MSG_colony_assimilation assimilation;
    struct s_MSG_colony_starvation starvation;
    struct s_MSG_colony_settlers settlers;
    struct s_MSG_colony_rebellion rebels;
    struct s_MSG_colony_android_killed android_killed;
};

struct s_MSG_colony {
    uint8_t planet_idx;
    int8_t subtype;
    union s_MSG_colony_union c;
};

struct s_MSG_lost_colony {
    int16_t planet_idx;
    int16_t reason;
    int16_t leader_idx;
    int8_t colony_or_outpost;
};

struct s_MSG_diplomacy {
    int8_t diplomacy_idx;
    int8_t plr_a;
    int8_t plr_b;
};

struct s_MSG_lost_defending_spy {
    int16_t count;
};

struct s_MSG_spy_assasination {
    int8_t plr_a;
    int8_t plr_b;
    int8_t leader_id;
    int8_t victim_id;
};

struct s_MSG_anomaly {
    uint8_t star_idx;
};

union s_MSG_union {
    struct s_MSG_colony colony;
    struct s_MSG_players players;
    struct s_MSG_financial financial;
    struct s_MSG_lost_ship lost_ship;
    struct s_MSG_scrap_ship scrap_ship;
    struct s_MSG_lost_colony lost_colony;
    struct s_MSG_hyperspace_beast hyperspace_beast;
    struct s_MSG_lost_attacking_spy lost_attacking_spy;
    struct s_MSG_lost_defending_spy lost_defending_spy;
    struct s_MSG_artemis artemis;
    struct s_MSG_artemis_leader artemis_leader;
    struct s_MSG_failed_retreat retreat;
    struct s_MSG_spy_assasination spy_assasination;
    struct s_MSG_anomaly anomaly;
    struct s_MSG_diplomacy diplomacy;
    struct s_MSG_eel eel;
    struct s_MSG_artifacts artifacts;
    struct s_MSG_scrap_leader scrap_leader;
};

struct s_MSG {
    int8_t msg_type;
    uint8_t priority;
    union s_MSG_union u;
    undefined field3_0xa;
    undefined field4_0xb;
    undefined field5_0xc;
    undefined field6_0xd;
    undefined field7_0xe;
    undefined field8_0xf;
    undefined field9_0x10;
    undefined field10_0x11;
};

typedef struct s_nebula s_nebula, *Ps_nebula;

struct s_nebula {
    short x;
    short y; /* Created by Rename Structure Field action */
    int8_t type;
};

typedef struct s_network s_network, *Ps_network;

struct s_network {
    struct s_modem_settings modem_settings;
    undefined field1_0x83;
    undefined field2_0x84;
    undefined field3_0x85;
    undefined field4_0x86;
    undefined field5_0x87;
    undefined field6_0x88;
    undefined field7_0x89;
    undefined field8_0x8a;
    undefined field9_0x8b;
    undefined field10_0x8c;
    undefined field11_0x8d;
    undefined field12_0x8e;
    undefined field13_0x8f;
    undefined field14_0x90;
    undefined field15_0x91;
    undefined field16_0x92;
    undefined field17_0x93;
    uint8_t mode; /* 3=ten 1/2=modem/ip? 0=offline */
    int field_95;
    int8_t field_99;
    short net_host_id;
    undefined1 field_9c; /* Created by Rename Structure Field action */
    undefined field23_0x9d;
    undefined field24_0x9e;
    undefined field25_0x9f;
    undefined field26_0xa0;
    undefined field27_0xa1;
    undefined field28_0xa2;
    undefined field29_0xa3;
    short field_a4;
    undefined field31_0xa6;
    undefined field32_0xa7;
    undefined field33_0xa8;
    undefined field34_0xa9;
    undefined field35_0xaa;
    undefined field36_0xab;
    undefined field37_0xac;
    undefined field38_0xad;
    undefined field39_0xae;
    undefined field40_0xaf;
    undefined field41_0xb0;
    undefined field42_0xb1;
    undefined field43_0xb2;
    undefined field44_0xb3;
    undefined field45_0xb4;
    undefined field46_0xb5;
    undefined field47_0xb6;
    undefined field48_0xb7;
    undefined field49_0xb8;
    undefined field50_0xb9;
    undefined field51_0xba;
    undefined field52_0xbb;
    undefined field53_0xbc;
    undefined field54_0xbd;
    undefined field55_0xbe;
    undefined field56_0xbf;
    undefined field57_0xc0;
    undefined field58_0xc1;
    undefined field59_0xc2;
    undefined field60_0xc3;
    undefined field61_0xc4;
    undefined field62_0xc5;
    undefined field63_0xc6;
    byte field_c7;
    undefined field65_0xc8;
    undefined field66_0xc9;
    undefined field67_0xca;
    undefined field68_0xcb;
    undefined field69_0xcc;
    undefined field70_0xcd;
    undefined field71_0xce;
    undefined field72_0xcf;
    undefined field73_0xd0;
    undefined field74_0xd1;
    undefined field75_0xd2;
    undefined field76_0xd3;
    undefined field77_0xd4;
    undefined field78_0xd5;
    undefined field79_0xd6;
    undefined field80_0xd7;
    undefined field81_0xd8;
    undefined field82_0xd9;
    undefined field83_0xda;
    undefined field84_0xdb;
    undefined field85_0xdc;
    undefined field86_0xdd;
    undefined field87_0xde;
    undefined field88_0xdf;
    undefined field89_0xe0;
    undefined field90_0xe1;
    undefined field91_0xe2;
    undefined field92_0xe3;
    undefined field93_0xe4;
    undefined field94_0xe5;
    undefined field95_0xe6;
    undefined field96_0xe7;
    undefined field97_0xe8;
    undefined field98_0xe9;
    undefined field99_0xea;
    undefined field100_0xeb;
    undefined field101_0xec;
    undefined field102_0xed;
    undefined field103_0xee;
    undefined field104_0xef;
    undefined field105_0xf0;
    undefined field106_0xf1;
    undefined field107_0xf2;
    undefined field108_0xf3;
    undefined field109_0xf4;
    undefined field110_0xf5;
    undefined field111_0xf6;
    undefined field112_0xf7;
    undefined field113_0xf8;
    undefined field114_0xf9;
    undefined field115_0xfa;
    undefined field116_0xfb;
    undefined field117_0xfc;
    undefined field118_0xfd;
    undefined field119_0xfe;
    undefined field120_0xff;
    undefined field121_0x100;
    undefined field122_0x101;
    undefined field123_0x102;
    undefined field124_0x103;
    undefined field125_0x104;
    undefined field126_0x105;
    undefined field127_0x106;
    undefined field128_0x107;
    undefined field129_0x108;
    undefined field130_0x109;
    undefined field131_0x10a;
    undefined field132_0x10b;
    undefined field133_0x10c;
    undefined field134_0x10d;
    undefined field135_0x10e;
    undefined field136_0x10f;
    undefined field137_0x110;
    undefined field138_0x111;
    undefined field139_0x112;
    undefined field140_0x113;
    undefined field141_0x114;
    undefined field142_0x115;
    undefined field143_0x116;
    undefined field144_0x117;
    undefined field145_0x118;
    undefined field146_0x119;
    undefined field147_0x11a;
    undefined field148_0x11b;
    undefined field149_0x11c;
    undefined field150_0x11d;
    undefined field151_0x11e;
    undefined field152_0x11f;
    undefined field153_0x120;
    undefined field154_0x121;
    undefined field155_0x122;
    undefined field156_0x123;
    undefined field157_0x124;
    undefined field158_0x125;
    undefined field159_0x126;
    undefined field160_0x127;
    undefined field161_0x128;
    undefined field162_0x129;
    undefined field163_0x12a;
    undefined field164_0x12b;
    undefined field165_0x12c;
    undefined field166_0x12d;
    undefined field167_0x12e;
    undefined field168_0x12f;
    undefined field169_0x130;
    undefined field170_0x131;
    undefined field171_0x132;
    undefined field172_0x133;
    undefined field173_0x134;
    undefined field174_0x135;
    undefined field175_0x136;
    undefined field176_0x137;
    undefined field177_0x138;
    undefined field178_0x139;
    undefined field179_0x13a;
    undefined field180_0x13b;
    undefined field181_0x13c;
    undefined field182_0x13d;
    undefined field183_0x13e;
    undefined field184_0x13f;
    undefined field185_0x140;
    undefined field186_0x141;
    undefined field187_0x142;
    undefined field188_0x143;
    undefined field189_0x144;
    undefined field190_0x145;
    undefined field191_0x146;
    undefined field192_0x147;
    undefined field193_0x148;
    undefined field194_0x149;
    undefined field195_0x14a;
    undefined field196_0x14b;
    undefined field197_0x14c;
    undefined field198_0x14d;
    undefined field199_0x14e;
    undefined field200_0x14f;
    undefined field201_0x150;
    undefined field202_0x151;
    undefined field203_0x152;
    undefined field204_0x153;
    undefined field205_0x154;
    undefined field206_0x155;
    undefined field207_0x156;
    undefined field208_0x157;
    undefined field209_0x158;
    undefined field210_0x159;
    undefined field211_0x15a;
    undefined field212_0x15b;
    undefined field213_0x15c;
    undefined field214_0x15d;
    undefined field215_0x15e;
    undefined field216_0x15f;
    undefined field217_0x160;
    undefined field218_0x161;
    undefined field219_0x162;
    undefined field220_0x163;
    undefined field221_0x164;
    undefined field222_0x165;
    undefined field223_0x166;
    undefined field224_0x167;
    undefined field225_0x168;
    undefined field226_0x169;
    undefined field227_0x16a;
    undefined field228_0x16b;
    undefined field229_0x16c;
    undefined field230_0x16d;
    undefined field231_0x16e;
    undefined field232_0x16f;
    undefined field233_0x170;
    undefined field234_0x171;
    undefined field235_0x172;
    undefined field236_0x173;
    undefined field237_0x174;
    undefined field238_0x175;
    undefined field239_0x176;
    undefined field240_0x177;
    undefined field241_0x178;
    undefined field242_0x179;
    undefined field243_0x17a;
    byte state;
    undefined field245_0x17c;
    undefined field246_0x17d;
    undefined field247_0x17e;
    undefined field248_0x17f;
    undefined field249_0x180;
    undefined field250_0x181;
    undefined field251_0x182;
    undefined field252_0x183;
    undefined field253_0x184;
    undefined field254_0x185;
    undefined field255_0x186;
    undefined field256_0x187;
    undefined field257_0x188;
    undefined field258_0x189;
    undefined field259_0x18a;
    undefined field260_0x18b;
    undefined field261_0x18c;
    undefined field262_0x18d;
    undefined field263_0x18e;
    undefined field264_0x18f;
    int8_t field265_0x190;
    undefined field266_0x191;
    undefined field267_0x192;
    undefined field268_0x193;
    undefined field269_0x194;
    undefined field270_0x195;
    undefined field271_0x196;
    undefined field272_0x197;
    undefined field273_0x198;
    undefined field274_0x199;
    undefined field275_0x19a;
    undefined field276_0x19b;
    undefined field277_0x19c;
    undefined field278_0x19d;
    undefined field279_0x19e;
    undefined field280_0x19f;
    undefined field281_0x1a0;
    undefined field282_0x1a1;
    undefined field283_0x1a2;
    undefined field284_0x1a3;
    undefined field285_0x1a4;
    int8_t field286_0x1a5;
    undefined field287_0x1a6;
    undefined field288_0x1a7;
    undefined field289_0x1a8;
    undefined field290_0x1a9;
    undefined field291_0x1aa;
    undefined field292_0x1ab;
    undefined field293_0x1ac;
    undefined field294_0x1ad;
    undefined field295_0x1ae;
    undefined field296_0x1af;
    undefined field297_0x1b0;
    undefined field298_0x1b1;
    undefined field299_0x1b2;
    undefined field300_0x1b3;
    undefined field301_0x1b4;
    undefined field302_0x1b5;
    undefined field303_0x1b6;
    undefined field304_0x1b7;
    undefined field305_0x1b8;
    undefined field306_0x1b9;
    undefined field307_0x1ba;
    undefined field308_0x1bb;
    undefined field309_0x1bc;
    undefined field310_0x1bd;
    undefined1 field_1be; /* Created by Rename Structure Field action */
    undefined field312_0x1bf;
    undefined field313_0x1c0;
    undefined field314_0x1c1;
    undefined field315_0x1c2;
    undefined field316_0x1c3;
    undefined field317_0x1c4;
    undefined field318_0x1c5;
    undefined field319_0x1c6;
    undefined field320_0x1c7;
    undefined field321_0x1c8;
    undefined field322_0x1c9;
    undefined field323_0x1ca;
    undefined field324_0x1cb;
    undefined field325_0x1cc;
    undefined field326_0x1cd;
    undefined field327_0x1ce;
    undefined field328_0x1cf;
    char field_1d0[8]; /* Created by Rename Structure Field action */
    short field_1d8;
    char field_1da;
    int field_1db;
    int field_1df;
    undefined field334_0x1e3;
    undefined field335_0x1e4;
    undefined field336_0x1e5;
    undefined field337_0x1e6;
    undefined field338_0x1e7;
    undefined field339_0x1e8;
    undefined field340_0x1e9;
    undefined field341_0x1ea;
    undefined field342_0x1eb;
    undefined field343_0x1ec;
    undefined field344_0x1ed;
    undefined field345_0x1ee;
    undefined field346_0x1ef;
    undefined field347_0x1f0;
    undefined field348_0x1f1;
    undefined field349_0x1f2;
    undefined field350_0x1f3;
    undefined field351_0x1f4;
    undefined field352_0x1f5;
    undefined field353_0x1f6;
    undefined field354_0x1f7;
    undefined field355_0x1f8;
    undefined field356_0x1f9;
    undefined field357_0x1fa;
    undefined field358_0x1fb;
    undefined field359_0x1fc;
    byte field_1fd;
    undefined field361_0x1fe;
    undefined field362_0x1ff;
    undefined field363_0x200;
    undefined field364_0x201;
    undefined field365_0x202;
    undefined field366_0x203;
    undefined field367_0x204;
    undefined field368_0x205;
    undefined field369_0x206;
    undefined field370_0x207;
    undefined field371_0x208;
    undefined field372_0x209;
};

typedef struct s_orion_candidate s_orion_candidate, *Ps_orion_candidate;

struct s_orion_candidate {
    int dist;
    short star_idx;
};

typedef struct s_parsec s_parsec, *Ps_parsec;

struct s_parsec {
    int32_t dist[72];
};

typedef struct s_planet_data s_planet_data, *Ps_planet_data;

struct s_planet_data {
    short colony_index;
    char star_index;
    char orbit;
    int8_t planet_type;
    int8_t size;
    char gravity_class;
    char group;
    char climate;
    char climate_bg_type;
    char mineral_class;
    uint8_t food_per_farmer;
    char n_terraforms;
    char max_farms;
    char max_population;
    int8_t planet_special;
    char environmental_alterations;
};

typedef struct s_planet_max_farms s_planet_max_farms, *Ps_planet_max_farms;

struct s_planet_max_farms {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
};

typedef struct s_player s_player, *Ps_player;

typedef struct s_settler_info s_settler_info, *Ps_settler_info;

typedef struct s_ship_design s_ship_design, *Ps_ship_design;


/* WARNING! conflicting data type names: /types.h/ushort - /ushort */

typedef struct s_ship_weapons s_ship_weapons, *Ps_ship_weapons;

struct s_ship_weapons {
    short type;
    char count;
    char current_count;
    char firing_arc;
    short specials;
    char ammo;
};

struct s_ship_design {
    char name[16];
    char size;
    byte ship_type;
    char shield_type;
    char ftl_type;
    char speed;
    char computer_type;
    char armor_type;
    char special_device_flags[5];
    struct s_ship_weapons ship_weapon[8];
    sbyte picture_num;
    char previous_owner;
    short cost;
    char combat_speed;
    ushort date_of_design;
};

struct s_settler_info {
    byte source_colony:8;
    byte dest_planet:8;
    byte eta:4;
    byte player:4;
    byte job:2;
};

struct s_player {
    char picture;
    char name[20];
    char race_name[15];
    byte eliminated;
    char race;
    char color;
    char personality;
    byte objectives;
    short home_planet_id;
    short network_player_id;
    char player_done_flags;
    short dead_field;
    char research_breakthrough;
    char tax_rate;
    int bc;
    short n_freighters;
    short surplus_freighters;
    short command_points;
    short command_points_used;
    short food_freighted;
    short settlers_freighted;
    struct s_settler_info settlers[25];
    short total_pop;
    short food_produced;
    short industry_produced;
    short research_produced;
    short bc_produced;
    short surplus_food;
    short surplus_bc;
    int total_maintenance;
    short maintenance[6];
    byte tech_fields[83];
    byte tech_applications[212];
    int research_accumulated;
    char captured_orion;
    char captured_antares;
    char won_council_vote;
    short player_defeated[8];
    short captured_population;
    int8_t last_attacker;
    int8_t ship_design_theme;
    char ship_special_theme;
    char tech_apps_learned[4];
    char tech_modifiers[17];
    char hyper_advanced_tech[8];
    int total_known_tech_cost;
    char knows_racial_stats;
    int last_tech_report_date[8];
    char player_tech_applications[8][27];
    char current_research_field;
    char current_research_application;
    char got_new_app;
    short ship_range;
    struct s_ship_design ship_designs[6];
    short ships_built[6];
    sbyte contact[8];
    char just_established_contact;
    char n_times_established_contact[8];
    sbyte within_range[8];
    char do_not_research;
    char offensive_stagepoint;
    char best_ftl_drive;
    char ship_speed;
    char officer_for_hire;
    short n_trade_pops;
    short current_trade_agreement_level[8];
    short trade_agreement_goal[8];
    short n_research_pops;
    short current_research_agreement_level[8];
    short research_agreement_goal[8];
    char fighter_beam_type;
    char fighter_bomb_type;
    short total_research;
    short total_ships;
    int32_t total_ship_strength[8]; /* Type 'dword.conflict[8]' was deleted */
    short total_colonies;
    sbyte failed_to_expand;
    char first_contact[8];
    sbyte relations[8];
    char base_relations[8];
    sbyte treaty[8];
    char trade_treaty[8];
    char research_treaty[8];
    short tribute_treaty[8];
    char diplomacy_incident[8];
    char diplomacy_message[8];
    short diplomacy_severity[8];
    short diplomacy_value[8];
    short diplomacy_system[8];
    short treaty_modifier[8];
    short trade_modifier[8];
    short tech_exchange_modifier[8];
    short peace_modifier[8];
    sbyte last_bad_diplomatic_incident[8];
    char broken_treaties_modifier[8];
    char last_broken_treaty[8];
    char last_gift[8];
    short ship_war_losses[8];
    short colony_war_losses[8];
    char biological_weapons_flag[8];
    char time_since_last_attack[8];
    char threats[8];
    char dishonored_flag[8];
    char peace_duration[8];
    short last_turn_diplomacy_severity[8];
    char last_broken_treaty_type[8];
    sbyte last_turn_treaty[8];
    char diplomacy_proposal_request[8];
    char diplomacy_proposal_tribute_bribe[8];
    char diplomacy_proposal_tech_bribe1[8];
    char diplomacy_proposal_tech_bribe2[8];
    short diplomacy_proposal_gold_bribe[8];
    char diplomacy_proposal_war_player[8];
    int diplomacy_proposal_exchange_max_value[8];
    char diplomacy_proposal_exchange_tech[8];
    short diplomacy_proposal_system_bribe[8];
    short sneak_attack_planet;
    char sneak_attack_message;
    char sneak_attack_player;
    char council_threat;
    short reward_amount[8];
    char reward_tech[8];
    char reward_attack_player[8];
    short food_shortage_duration;
    char trust_worthiness[8];
    char trust_breaker_player[8];
    char trust_breaker_treaty[8];
    char stop_spying_duration[8];
    char stop_blockading_duration[8];
    char war_buildup_duration;
    short last_diplomacy_proposal_turn[8];
    short diplomacy_proposal_rejection[8];
    short opportunity_planet_num[8];
    short opportunity_planet_value[8];
    int opportunity_actual_attacker_ratio[8];
    short temp_diplomacy_dummy1[8];
    char diplomacy_demand_rejection_message[8];
    sbyte last_diplomacy_threat_turn[8];
    char delayed_diplomacy_orders[8];
    int8_t traits[31];
    int8_t evolutionary_upgrade_traits[31];
    char picked_evolutionary_upgrade;
    char applied_evolutionary_upgrade;
    char fleet_history[350];
    char tech_history[350];
    char population_history[350];
    char production_history[350];
    int8_t spies[8];
    char history_btns;
    char ignoring;
    char peace_flags[8];
    int8_t cheated;
    ushort stardate_last_offered_hero;
    char current_zoom_level;
    short cur_map_x;
    short cur_map_y;
    char council_voted_for;
    char surrender_to;
    short time_since_last_lucky_event;
    char has_galactic_lore;
    char warned_about_cheating_player;
    char cheated_this_turn;
    int tribute_income;
    char filler[45];
};

typedef struct s_player_info s_player_info, *Ps_player_info;

struct s_player_info {
    char current_government;
    char pop_growth_bonus;
    char food_bonus;
    char production_bonus;
    char research_bonus;
    char tax_bonus;
    char ship_defense;
    char ship_attack;
    char ground_combat_bonus;
    char spying_bonus;
    char low_g_world;
    char heavy_g_world;
    sbyte aquatic;
    sbyte subterranean;
    char large_home_world;
    char rich_home_world;
    char ancient_home_world;
    sbyte cybernetic;
    char eats_minerals;
    char repulsive;
    char charismatic;
    char uncreative;
    char creative;
    sbyte environment_immune;
    char fantastic_traders;
    sbyte telepathic;
    sbyte lucky;
    char omniscience;
    char stealthy_ships;
    sbyte trans_dimensional;
    char warlord;
};

typedef struct s_plntsum_planet s_plntsum_planet, *Ps_plntsum_planet;

struct s_plntsum_planet {
    short max_pop;
    short food_per_farmer;
    short minerals;
    short mineral_class;
    short climate;
    short explored;
    short best_colony_eta;
    short best_outpost_eta;
    short best_colony_ship_idx;
    short best_outpost_ship_idx;
    int8_t blocked;
    short ship_summary_idx;
    short scan_highlight_pulse;
    int8_t scan_highlight_pulse_step;
    int8_t colony_eta;
    int8_t outpost_eta;
};

typedef struct s_plntsum_screen s_plntsum_screen, *Ps_plntsum_screen;

struct s_plntsum_screen {
    struct s_animation_header *pics[14];
    short buttons[10];
    short fields[8];
    short colony_icons[8];
    short output_icons[8];
    short up_button;
    short dn_button;
    short esc_button;
    short planet_icons[72];
    bool refresh_fields;
    short scanned_input;
};

typedef struct s_plntsum_ship s_plntsum_ship, *Ps_plntsum_ship;

struct s_plntsum_ship {
    short colony_ship_idx;
    short outpost_ship_idx;
    short ship_icon_id;
    bool colony_available; /* Created by Rename Structure Field action */
    bool output_available; /* Created by Rename Structure Field action */
};

typedef struct s_pop_info s_pop_info, *Ps_pop_info;

struct s_pop_info {
    uint race:4;
    uint loyalty:3;
    uint job:2;
    uint conquered:1;
    uint unknown1:4;
    uint starve_priority:2;
    uint unknown2:15;
    uint employed:1;
};

typedef struct s_potential_colony_info s_potential_colony_info, *Ps_potential_colony_info;

struct s_potential_colony_info {
    short pop_growth_bonus;
    short maintenance;
    short field2_0x4;
    short worker_bonus;
    short food;
    short industry;
    short research;
};

typedef struct s_prev_leader_data s_prev_leader_data, *Ps_prev_leader_data;

struct s_prev_leader_data {
    int8_t status;
    short location;
    short eta;
};

typedef struct s_scroll_bar s_scroll_bar, *Ps_scroll_bar;

struct s_scroll_bar {
    short thumb_center_y;
    short last_thumb_center_y;
    short thumb_center_y_copy;
    short thumb_height_px;
    short total_rows;
    short visible_rows;
    short first_visible_row;
    short track_len_px;
    short up_btn;
    short dn_btn;
    undefined field10_0x14;
    undefined field11_0x15;
    undefined field12_0x16;
    undefined field13_0x17;
    int8_t field14_0x18;
    undefined field15_0x19;
    int8_t row_step;
    short thumb_y_snapshot;
};

typedef struct s_settings s_settings, *Ps_settings;

struct s_settings {
    sbyte end_of_turn_summary;
    sbyte end_of_turn_wait;
    sbyte random_events;
    sbyte enemy_moves;
    sbyte expanding_help;
    sbyte auto_select_ships;
    sbyte animations_on;
    sbyte auto_select_colony;
    sbyte show_relocation_lines;
    sbyte show_gnn_report;
    sbyte auto_delete_tg_housing;
    sbyte auto_saves;
    sbyte only_show_serious_turn_summary_reports;
    sbyte ship_initiative;
    int8_t tactical_combat_speed;
    int8_t message_delay;
    undefined field16_0x10;
    sbyte sound_fx_on;
    sbyte sound_fx_level;
    sbyte music_on;
    int8_t music_level;
    char active_save_slot;
    byte multi_player_game_type;
    char net_game_name[9];
    struct s_modem_settings modem_settings;
    byte modem_init_string[11];
    undefined field26_0xae;
    undefined field27_0xaf;
    undefined field28_0xb0;
    undefined field29_0xb1;
    undefined field30_0xb2;
    undefined field31_0xb3;
    char combat_speedx2_flag;
    char combat_legal_moves_flag;
    char combat_missile_impact_flag;
    char combat_shields_arc_flag;
    char combat_grid_flag;
    undefined field37_0xb9;
    undefined field38_0xba;
    undefined field39_0xbb;
    undefined field40_0xbc;
    undefined field41_0xbd;
    undefined field42_0xbe;
    undefined field43_0xbf;
    undefined field44_0xc0;
    undefined field45_0xc1;
    undefined field46_0xc2;
    undefined field47_0xc3;
    undefined field48_0xc4;
    undefined field49_0xc5;
    undefined field50_0xc6;
    undefined field51_0xc7;
    undefined field52_0xc8;
    undefined field53_0xc9;
    undefined field54_0xca;
    undefined field55_0xcb;
    undefined field56_0xcc;
    undefined field57_0xcd;
    undefined field58_0xce;
    undefined field59_0xcf;
    undefined field60_0xd0;
    undefined field61_0xd1;
    int8_t language;
    byte xenons_exist;
    int8_t game_difficulty;
    byte number_of_players;
    byte galaxy_size;
    byte galaxy_age;
    byte strategic_combat_flag;
    byte starting_civilization_level;
    uint32_t start_of_game_seed;
    short sound_driver_id;
    short music_driver_id;
    uint16_t version;
    undefined field74_0xe4;
    undefined field75_0xe5;
    undefined field76_0xe6;
    undefined field77_0xe7;
    undefined field78_0xe8;
    undefined field79_0xe9;
    undefined field80_0xea;
    undefined field81_0xeb;
    undefined field82_0xec;
    undefined field83_0xed;
    undefined field84_0xee;
    undefined field85_0xef;
    undefined field86_0xf0;
    undefined field87_0xf1;
    undefined field88_0xf2;
    undefined field89_0xf3;
    undefined field90_0xf4;
    undefined field91_0xf5;
    undefined field92_0xf6;
    undefined field93_0xf7;
    undefined field94_0xf8;
    undefined field95_0xf9;
    undefined field96_0xfa;
    undefined field97_0xfb;
    undefined field98_0xfc;
    undefined field99_0xfd;
    undefined field100_0xfe;
    undefined field101_0xff;
    undefined field102_0x100;
    undefined field103_0x101;
    undefined field104_0x102;
    undefined field105_0x103;
    undefined field106_0x104;
    undefined field107_0x105;
    undefined field108_0x106;
    undefined field109_0x107;
    undefined field110_0x108;
    undefined field111_0x109;
    undefined field112_0x10a;
    undefined field113_0x10b;
    undefined field114_0x10c;
    undefined field115_0x10d;
    undefined field116_0x10e;
    undefined field117_0x10f;
    undefined field118_0x110;
    undefined field119_0x111;
    undefined field120_0x112;
    undefined field121_0x113;
    undefined field122_0x114;
    undefined field123_0x115;
    undefined field124_0x116;
    undefined field125_0x117;
    undefined field126_0x118;
    undefined field127_0x119;
    undefined field128_0x11a;
    undefined field129_0x11b;
    undefined field130_0x11c;
    undefined field131_0x11d;
    undefined field132_0x11e;
    undefined field133_0x11f;
    undefined field134_0x120;
    undefined field135_0x121;
    undefined field136_0x122;
    undefined field137_0x123;
    undefined field138_0x124;
    undefined field139_0x125;
    undefined field140_0x126;
    undefined field141_0x127;
    undefined field142_0x128;
    undefined field143_0x129;
    undefined field144_0x12a;
    undefined field145_0x12b;
    undefined field146_0x12c;
    undefined field147_0x12d;
    undefined field148_0x12e;
    undefined field149_0x12f;
    undefined field150_0x130;
    undefined field151_0x131;
    undefined field152_0x132;
    undefined field153_0x133;
    undefined field154_0x134;
    undefined field155_0x135;
    undefined field156_0x136;
    undefined field157_0x137;
    undefined field158_0x138;
    undefined field159_0x139;
    undefined field160_0x13a;
    undefined field161_0x13b;
    undefined field162_0x13c;
    undefined field163_0x13d;
    undefined field164_0x13e;
    undefined field165_0x13f;
    undefined field166_0x140;
    undefined field167_0x141;
    undefined field168_0x142;
    undefined field169_0x143;
    undefined field170_0x144;
    undefined field171_0x145;
    undefined field172_0x146;
    undefined field173_0x147;
    undefined field174_0x148;
    undefined field175_0x149;
    undefined field176_0x14a;
    undefined field177_0x14b;
    undefined field178_0x14c;
    undefined field179_0x14d;
    undefined field180_0x14e;
    undefined field181_0x14f;
    undefined field182_0x150;
    undefined field183_0x151;
    undefined field184_0x152;
    undefined field185_0x153;
    undefined field186_0x154;
    undefined field187_0x155;
    undefined field188_0x156;
    undefined field189_0x157;
    undefined field190_0x158;
    undefined field191_0x159;
    undefined field192_0x15a;
    undefined field193_0x15b;
    undefined field194_0x15c;
    undefined field195_0x15d;
    undefined field196_0x15e;
    undefined field197_0x15f;
    undefined field198_0x160;
    undefined field199_0x161;
    undefined field200_0x162;
    undefined field201_0x163;
    undefined field202_0x164;
    undefined field203_0x165;
    undefined field204_0x166;
    undefined field205_0x167;
    undefined field206_0x168;
    undefined field207_0x169;
    undefined field208_0x16a;
    undefined field209_0x16b;
    undefined field210_0x16c;
    undefined field211_0x16d;
    undefined field212_0x16e;
    undefined field213_0x16f;
    undefined field214_0x170;
    undefined field215_0x171;
    undefined field216_0x172;
    undefined field217_0x173;
    undefined field218_0x174;
    undefined field219_0x175;
    undefined field220_0x176;
    undefined field221_0x177;
    undefined field222_0x178;
    undefined field223_0x179;
    undefined field224_0x17a;
    undefined field225_0x17b;
    undefined field226_0x17c;
    undefined field227_0x17d;
    undefined field228_0x17e;
    undefined field229_0x17f;
    undefined field230_0x180;
    undefined field231_0x181;
    undefined field232_0x182;
    undefined field233_0x183;
    undefined field234_0x184;
    undefined field235_0x185;
    undefined field236_0x186;
    undefined field237_0x187;
    undefined field238_0x188;
    undefined field239_0x189;
    undefined field240_0x18a;
    undefined field241_0x18b;
    undefined field242_0x18c;
    undefined field243_0x18d;
    undefined field244_0x18e;
    undefined field245_0x18f;
    undefined field246_0x190;
    undefined field247_0x191;
    undefined field248_0x192;
    undefined field249_0x193;
    undefined field250_0x194;
    undefined field251_0x195;
    undefined field252_0x196;
    undefined field253_0x197;
    undefined field254_0x198;
    undefined field255_0x199;
    undefined field256_0x19a;
    undefined field257_0x19b;
    undefined field258_0x19c;
    undefined field259_0x19d;
    undefined field260_0x19e;
    undefined field261_0x19f;
    undefined field262_0x1a0;
    undefined field263_0x1a1;
    undefined field264_0x1a2;
    undefined field265_0x1a3;
    undefined field266_0x1a4;
    undefined field267_0x1a5;
    undefined field268_0x1a6;
    undefined field269_0x1a7;
    undefined field270_0x1a8;
    undefined field271_0x1a9;
    undefined field272_0x1aa;
    undefined field273_0x1ab;
    undefined field274_0x1ac;
    undefined field275_0x1ad;
    undefined field276_0x1ae;
    undefined field277_0x1af;
    undefined field278_0x1b0;
    undefined field279_0x1b1;
    undefined field280_0x1b2;
    undefined field281_0x1b3;
    undefined field282_0x1b4;
    undefined field283_0x1b5;
    undefined field284_0x1b6;
    undefined field285_0x1b7;
    undefined field286_0x1b8;
    undefined field287_0x1b9;
    undefined field288_0x1ba;
    undefined field289_0x1bb;
    undefined field290_0x1bc;
    undefined field291_0x1bd;
    undefined field292_0x1be;
    undefined field293_0x1bf;
    undefined field294_0x1c0;
    undefined field295_0x1c1;
    undefined field296_0x1c2;
    undefined field297_0x1c3;
    undefined field298_0x1c4;
    undefined field299_0x1c5;
    undefined field300_0x1c6;
    undefined field301_0x1c7;
    undefined field302_0x1c8;
    undefined field303_0x1c9;
    undefined field304_0x1ca;
    undefined field305_0x1cb;
    undefined field306_0x1cc;
    undefined field307_0x1cd;
    undefined field308_0x1ce;
    undefined field309_0x1cf;
    undefined field310_0x1d0;
    undefined field311_0x1d1;
    undefined field312_0x1d2;
    undefined field313_0x1d3;
    undefined field314_0x1d4;
    undefined field315_0x1d5;
    undefined field316_0x1d6;
    undefined field317_0x1d7;
    undefined field318_0x1d8;
    undefined field319_0x1d9;
    undefined field320_0x1da;
    undefined field321_0x1db;
    undefined field322_0x1dc;
    undefined field323_0x1dd;
    undefined field324_0x1de;
    undefined field325_0x1df;
    undefined field326_0x1e0;
    undefined field327_0x1e1;
    undefined field328_0x1e2;
    undefined field329_0x1e3;
    undefined field330_0x1e4;
    undefined field331_0x1e5;
    undefined field332_0x1e6;
    undefined field333_0x1e7;
    undefined field334_0x1e8;
    undefined field335_0x1e9;
    undefined field336_0x1ea;
    undefined field337_0x1eb;
    undefined field338_0x1ec;
    undefined field339_0x1ed;
    undefined field340_0x1ee;
    undefined field341_0x1ef;
    undefined field342_0x1f0;
    undefined field343_0x1f1;
    undefined field344_0x1f2;
    undefined field345_0x1f3;
    undefined field346_0x1f4;
    undefined field347_0x1f5;
    undefined field348_0x1f6;
    undefined field349_0x1f7;
    undefined field350_0x1f8;
    undefined field351_0x1f9;
    undefined field352_0x1fa;
    undefined field353_0x1fb;
    undefined field354_0x1fc;
    undefined field355_0x1fd;
    undefined field356_0x1fe;
    undefined field357_0x1ff;
    undefined field358_0x200;
    undefined field359_0x201;
    undefined field360_0x202;
    undefined field361_0x203;
    undefined field362_0x204;
    undefined field363_0x205;
    undefined field364_0x206;
    undefined field365_0x207;
    undefined field366_0x208;
    undefined field367_0x209;
    undefined field368_0x20a;
    undefined field369_0x20b;
    undefined field370_0x20c;
    undefined field371_0x20d;
    undefined field372_0x20e;
    undefined field373_0x20f;
    undefined field374_0x210;
    undefined field375_0x211;
    undefined field376_0x212;
    undefined field377_0x213;
    undefined field378_0x214;
    undefined field379_0x215;
    undefined field380_0x216;
    undefined field381_0x217;
    undefined field382_0x218;
    undefined field383_0x219;
    undefined field384_0x21a;
    undefined field385_0x21b;
    undefined field386_0x21c;
    undefined field387_0x21d;
    undefined field388_0x21e;
    undefined field389_0x21f;
    undefined field390_0x220;
    undefined field391_0x221;
    undefined field392_0x222;
    undefined field393_0x223;
    undefined field394_0x224;
    undefined field395_0x225;
    undefined field396_0x226;
    undefined field397_0x227;
    undefined field398_0x228;
};

typedef struct s_ship_data s_ship_data, *Ps_ship_data;

struct s_ship_data {
    struct s_ship_design d;
    char owner;
    sbyte status;
    short location;
    short x;
    short y;
    char group_has_navigator;
    char travelling_speed;
    char turns_left;
    char shield_damage_percent;
    char drive_damage_percent;
    char computer_damage;
    char crew_quality;
    short crew_experience;
    short officer_index;
    byte special_device_damage_flags[5];
    short armor_damage;
    short structural_damage;
    char mission;
    char just_built;
};

typedef struct s_ship_drive_data s_ship_drive_data, *Ps_ship_drive_data;

struct s_ship_drive_data {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    short tech_application_idx;
    undefined field7_0x8;
    undefined field8_0x9;
    undefined field9_0xa;
    undefined field10_0xb;
    undefined field11_0xc;
    undefined field12_0xd;
    undefined field13_0xe;
    undefined field14_0xf;
    undefined field15_0x10;
    undefined field16_0x11;
    undefined field17_0x12;
    undefined field18_0x13;
    undefined field19_0x14;
    undefined field20_0x15;
    undefined field21_0x16;
    undefined field22_0x17;
    undefined field23_0x18;
    undefined field24_0x19;
    undefined field25_0x1a;
    undefined field26_0x1b;
    undefined field27_0x1c;
    undefined field28_0x1d;
    undefined field29_0x1e;
    undefined field30_0x1f;
    uint8_t combat_speed_modifier[12];
    short ftl_speed;
};

typedef struct s_ship_icon s_ship_icon, *Ps_ship_icon;

struct s_ship_icon {
    short field0_0x0;
    short node_idx;
    short star_idx;
    short stack_slot;
    short x;
    short y;
};

typedef struct s_ship_move_info s_ship_move_info, *Ps_ship_move_info;

struct s_ship_move_info {
    int8_t blackhole_blocks;
    int8_t insufficient_fuel;
    int8_t immobile;
    int8_t pad;
    int8_t turns_left;
    int8_t moving;
    int8_t out_of_range;
    int8_t has_navigator;
    int8_t traveling_speed;
    int8_t parsecs;
    short uses_wormhole;
    int8_t has_stargate_route;
    int8_t hyperspace_flux;
};

typedef struct s_ship_node s_ship_node, *Ps_ship_node;

struct s_ship_node {
    short ship_idx;
    short next_node;
    char selected;
};

typedef struct s_skill_data s_skill_data, *Ps_skill_data;

struct s_skill_data {
    byte num;
    uint skill_mask;
    byte unknown1;
    int16_t skill_bonus;
    byte unknown2;
    byte unknown3;
    char *name;
    char *format_str;
};

typedef struct s_spcl s_spcl, *Ps_spcl;

struct s_spcl {
    int8_t id;
    int8_t special_or_weapon_id;
    int8_t type;
    int8_t category;
};

typedef struct s_star_data s_star_data, *Ps_star_data;

struct s_star_data {
    char name[15];
    short x;
    short y;
    char size;
    int8_t owner;
    char pict_type;
    int8_t spectral_class;
    int8_t last_planet_selected[8];
    char black_hole_blocks[9];
    int8_t system_special;
    int8_t wormhole_star_id;
    byte blockaded;
    char blockaded_by[8];
    int8_t visited;
    byte just_visited;
    byte ignore_colony_ships;
    byte ignore_combat_ships;
    int8_t colonize_player;
    byte has_colony;
    byte has_warp_field_interdictor;
    byte next_wfi_in_list;
    byte has_tachyon;
    byte has_subspace;
    byte has_stargate;
    byte has_jumpgate;
    byte has_artemis_net;
    byte has_dimensional_portal;
    byte is_stagepoint;
    char officer_index[8];
    short planet_index[5];
    short relocate_ship_to[8];
    int8_t twinkling1;
    int8_t twinkling2;
    byte not_used3;
    char surrender_to[8];
    byte in_nebula;
    byte artifacts_gave_app;
};

typedef struct s_sys_disp s_sys_disp, *Ps_sys_disp;

struct s_sys_disp {
    short orbit;
    short x;
    short y;
    int8_t oribit_pos;
    int8_t orbit_idx;
    short current_frame;
    struct s_animation_header *pic;
};

typedef struct s_system_display_ships s_system_display_ships, *Ps_system_display_ships;

struct s_system_display_ships {
    short field0_0x0;
    short field1_0x2;
    short button;
    short ship_idx;
    int field4_0x8;
};

typedef struct s_tech_app_data s_tech_app_data, *Ps_tech_app_data;

struct s_tech_app_data {
    char *name;
    ushort id;
    short tech_field_id;
    byte unk_class;
    byte ai_group;
    byte unknown1;
    byte in_strategic;
    byte type;
};

typedef struct s_tech_armor_data s_tech_armor_data, *Ps_tech_armor_data;

struct s_tech_armor_data {
    char *name;
    short armor_id;
    short tech_app_id; /* Type 'TECH_APP' was deleted */
    undefined field3_0x8;
    uint8_t troops_bonus;
    short armor_absorption_percent;
    short cost;
    uint8_t strategic_hp;
};

typedef struct s_tech_building_data s_tech_building_data, *Ps_tech_building_data;

struct s_tech_building_data {
    char *name;
    undefined field1_0x4;
    undefined field2_0x5;
    short tech_app;
    int cost;
    short maintenance;
    int8_t type;
    undefined field7_0xf;
    undefined field8_0x10;
    undefined field9_0x11;
    undefined field10_0x12;
};

typedef struct s_tech_computer_data s_tech_computer_data, *Ps_tech_computer_data;

struct s_tech_computer_data {
    char *name;
    short computer_id;
    short tech_app;
    short cost_by_ship_type[6];
    short bonus;
};

typedef struct s_tech_data_personal_shield s_tech_data_personal_shield, *Ps_tech_data_personal_shield;

struct s_tech_data_personal_shield {
    int16_t tech_app_id;
    int8_t bonus;
};

typedef struct s_tech_data_rifle s_tech_data_rifle, *Ps_tech_data_rifle;

struct s_tech_data_rifle {
    int16_t tech_app_id;
    int8_t bonus;
};

typedef struct s_tech_drive_data s_tech_drive_data, *Ps_tech_drive_data;

struct s_tech_drive_data {
    char *name;
    short drive_id;
    short tech_app;
    short space_per_ship_type[6];
    short cost_per_ship_type[6];
    int8_t combat_speed_modifier[6];
    int8_t speed_modifier[6];
    short speed;
};

typedef struct s_tech_field_data s_tech_field_data, *Ps_tech_field_data;

struct s_tech_field_data {
    char *name;
    short id;
    short prev_field_id;
    short next_field_id;
    ushort tech1;
    ushort tech2;
    ushort tech3;
    ushort tech4;
    uint cost;
    byte ai_group;
};

typedef struct s_tech_fuel_data s_tech_fuel_data, *Ps_tech_fuel_data;

struct s_tech_fuel_data {
    char *name;
    short fuel_id;
    short tech_app;
    short ship_range;
};

typedef struct s_tech_hull_data s_tech_hull_data, *Ps_tech_hull_data;

struct s_tech_hull_data {
    short hull_space;
    short hull_size;
    undefined field2_0x4;
    undefined field3_0x5;
    short armor_absorption;
    short structure;
    undefined field6_0xa;
    undefined field7_0xb;
    undefined field8_0xc;
    undefined field9_0xd;
    undefined field10_0xe;
    undefined field11_0xf;
    short beam_weapon_count;
    short missle_weapon_count;
    short special_weapon_count;
    short bomb_weapon_count;
    undefined field16_0x18;
    undefined field17_0x19;
    short field18_0x1a;
    char *name;
    char *size_name;
};

typedef struct s_tech_shield_data s_tech_shield_data, *Ps_tech_shield_data;

struct s_tech_shield_data {
    char *name;
    ushort shield_id;
    short tech_app_id;
    short space_per_ship_type[6];
    short cost_per_ship_type[6];
    short points_blocked;
    undefined field6_0x22;
    undefined field7_0x23;
    undefined field8_0x24;
    undefined field9_0x25;
    undefined field10_0x26;
    undefined field11_0x27;
    undefined field12_0x28;
    undefined field13_0x29;
    undefined field14_0x2a;
    undefined field15_0x2b;
    undefined field16_0x2c;
    undefined field17_0x2d;
    undefined field18_0x2e;
    undefined field19_0x2f;
    undefined field20_0x30;
    undefined field21_0x31;
    undefined field22_0x32;
    undefined field23_0x33;
    undefined field24_0x34;
    undefined field25_0x35;
    undefined field26_0x36;
    undefined field27_0x37;
    undefined field28_0x38;
    undefined field29_0x39;
    byte unk_strat_value;
};

typedef struct s_tech_specials_data s_tech_specials_data, *Ps_tech_specials_data;

struct s_tech_specials_data {
    char *name;
    undefined field1_0x4;
    undefined field2_0x5;
    short tech_application_id;
    short space_per_ship_type[6];
    short cost_per_ship_type[6];
    short field6_0x20[6];
    undefined field7_0x2c;
    undefined field8_0x2d;
    undefined field9_0x2e;
};

typedef struct s_tech_weapon_data s_tech_weapon_data, *Ps_tech_weapon_data;

struct s_tech_weapon_data {
    char *name;
    char *name_plural;
    undefined field2_0x8;
    undefined field3_0x9;
    short tech_application_id;
    sbyte rack;
    undefined field6_0xd;
    short base_size;
    short cost;
    short flags;
    short field10_0x14;
    short damage;
    undefined field12_0x18;
    undefined field13_0x19;
    undefined field14_0x1a;
    undefined field15_0x1b;
};

typedef struct s_tech_weapon_mod_data s_tech_weapon_mod_data, *Ps_tech_weapon_mod_data;

struct s_tech_weapon_mod_data {
    char *name0;
    char *name;
    uint8_t mod_id;
    uint8_t level;
    short bonus1;
    short bonus2;
    undefined field6_0xe;
};

typedef struct s_unit_data s_unit_data, *Ps_unit_data;

struct s_unit_data {
    char *name;
    undefined field1_0x4;
    undefined field2_0x5;
    undefined field3_0x6;
    undefined field4_0x7;
    undefined field5_0x8;
    undefined field6_0x9;
    undefined field7_0xa;
    undefined field8_0xb;
};

