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

typedef struct s_battle_results s_battle_results, *Ps_battle_results;

typedef struct s_combat_bomb_result s_combat_bomb_result, *Ps_combat_bomb_result;

typedef struct s_invasion_side_info s_invasion_side_info, *Ps_invasion_side_info;

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
    int8_t personal_shield_bonus;
    int8_t rifle_tech_app_id;
    int8_t rifle_bonus;
    int8_t ground_combat_bonus;
    int8_t heavy_g_bonus;
    int8_t low_g_penalty;
    int8_t subterranean_bonus;
    int8_t difficulty_bonus;
    short additional_modifier;
    int8_t field17_0x12;
};

struct s_invasion_side_info {
    int16_t player_id;
    int16_t unit_strengths[4];
    int16_t unit_counts[4];
    int8_t unit_hps[4];
    int8_t current_troop_type_idx;
    int8_t current_unit_damage;
    int8_t round_hit_unit_type;
    int8_t round_killed_unit_type;
    int16_t colony_id;
    int16_t *ship_list;
    int16_t n_ships;
    int16_t opponent_player_id;
    struct s_invasion_player_bonuses combat_modifiers;
};

struct s_combat_bomb_result {
    int8_t attacker_idx;
    uint8_t colony_idx;
    int8_t is_planetary_hit;
    int16_t bomb_hits_remaining;
    int8_t is_bio_weapon;
    int16_t bio_hits_remaining;
    int16_t infantry_killed;
    int16_t tanks_killed;
    int16_t population_killed;
    int8_t buildings_destroyed[49];
    int16_t bio_pop_killed;
    int8_t damage_occurred;
    int8_t colony_wiped_out;
    int16_t production_lost;
    int32_t total_hits_processed;
};

struct s_battle_results {
    int8_t packet_type;
    uint8_t combat_star_id;
    int16_t combat_defender_id;
    int16_t combat_planet_id;
    int16_t combat_target_player_id;
    undefined field5_0x8;
    int8_t combat_attacker_id;
    int8_t combat_result;
    undefined field8_0xb;
    int8_t outpost_destroyed_flag;
    undefined field10_0xd;
    undefined field11_0xe;
    struct s_combat_bomb_result bomb_results;
    struct s_invasion_side_info attacker_invasion;
    struct s_invasion_side_info defender_invasion;
    int32_t ground_defense_strength;
    uint8_t captured_techs_data[424];
    int16_t captured_tech_id;
    int16_t captured_tech_planet_id;
    int8_t planet_destroyed_flag;
    int16_t orion_techs[4];
    short rescued_hero_id;
    char summary[100];
    char details[500];
    undefined field24_0x4d9;
    undefined field25_0x4da;
    undefined field26_0x4db;
    undefined field27_0x4dc;
    undefined field28_0x4dd;
    undefined field29_0x4de;
    undefined field30_0x4df;
    undefined field31_0x4e0;
    undefined field32_0x4e1;
    undefined field33_0x4e2;
    undefined field34_0x4e3;
    undefined field35_0x4e4;
    undefined field36_0x4e5;
    undefined field37_0x4e6;
    undefined field38_0x4e7;
    undefined field39_0x4e8;
    undefined field40_0x4e9;
    undefined field41_0x4ea;
    undefined field42_0x4eb;
    undefined field43_0x4ec;
    undefined field44_0x4ed;
    undefined field45_0x4ee;
    undefined field46_0x4ef;
    undefined field47_0x4f0;
    undefined field48_0x4f1;
    undefined field49_0x4f2;
    undefined field50_0x4f3;
    undefined field51_0x4f4;
    undefined field52_0x4f5;
    undefined field53_0x4f6;
    undefined field54_0x4f7;
    undefined field55_0x4f8;
    undefined field56_0x4f9;
    undefined field57_0x4fa;
    undefined field58_0x4fb;
    undefined field59_0x4fc;
    undefined field60_0x4fd;
    undefined field61_0x4fe;
    undefined field62_0x4ff;
    undefined field63_0x500;
    undefined field64_0x501;
    undefined field65_0x502;
    undefined field66_0x503;
    undefined field67_0x504;
    undefined field68_0x505;
    undefined field69_0x506;
    undefined field70_0x507;
    undefined field71_0x508;
    undefined field72_0x509;
    undefined field73_0x50a;
    undefined field74_0x50b;
    undefined field75_0x50c;
    undefined field76_0x50d;
    undefined field77_0x50e;
    undefined field78_0x50f;
    undefined field79_0x510;
    undefined field80_0x511;
    undefined field81_0x512;
    undefined field82_0x513;
    undefined field83_0x514;
    undefined field84_0x515;
    undefined field85_0x516;
    undefined field86_0x517;
    undefined field87_0x518;
    undefined field88_0x519;
    undefined field89_0x51a;
    undefined field90_0x51b;
    undefined field91_0x51c;
    undefined field92_0x51d;
    undefined field93_0x51e;
    undefined field94_0x51f;
    undefined field95_0x520;
    undefined field96_0x521;
    undefined field97_0x522;
    undefined field98_0x523;
    undefined field99_0x524;
    undefined field100_0x525;
    undefined field101_0x526;
    undefined field102_0x527;
    undefined field103_0x528;
    undefined field104_0x529;
    undefined field105_0x52a;
    undefined field106_0x52b;
    undefined field107_0x52c;
    undefined field108_0x52d;
    undefined field109_0x52e;
    undefined field110_0x52f;
    undefined field111_0x530;
    undefined field112_0x531;
    undefined field113_0x532;
    undefined field114_0x533;
    undefined field115_0x534;
    undefined field116_0x535;
    undefined field117_0x536;
    undefined field118_0x537;
    undefined field119_0x538;
    undefined field120_0x539;
    undefined field121_0x53a;
    undefined field122_0x53b;
    undefined field123_0x53c;
    undefined field124_0x53d;
    undefined field125_0x53e;
    undefined field126_0x53f;
    undefined field127_0x540;
    undefined field128_0x541;
    undefined field129_0x542;
    undefined field130_0x543;
    undefined field131_0x544;
    undefined field132_0x545;
    undefined field133_0x546;
    undefined field134_0x547;
    undefined field135_0x548;
    undefined field136_0x549;
    undefined field137_0x54a;
    undefined field138_0x54b;
    undefined field139_0x54c;
    undefined field140_0x54d;
    undefined field141_0x54e;
    undefined field142_0x54f;
    undefined field143_0x550;
    undefined field144_0x551;
    undefined field145_0x552;
    undefined field146_0x553;
    undefined field147_0x554;
    undefined field148_0x555;
    undefined field149_0x556;
    undefined field150_0x557;
    undefined field151_0x558;
    undefined field152_0x559;
    undefined field153_0x55a;
    undefined field154_0x55b;
    undefined field155_0x55c;
    undefined field156_0x55d;
    undefined field157_0x55e;
    undefined field158_0x55f;
    undefined field159_0x560;
    undefined field160_0x561;
    undefined field161_0x562;
    undefined field162_0x563;
    undefined field163_0x564;
    undefined field164_0x565;
    undefined field165_0x566;
    undefined field166_0x567;
    undefined field167_0x568;
    undefined field168_0x569;
    undefined field169_0x56a;
    undefined field170_0x56b;
    undefined field171_0x56c;
    undefined field172_0x56d;
    undefined field173_0x56e;
    undefined field174_0x56f;
    undefined field175_0x570;
    undefined field176_0x571;
    undefined field177_0x572;
    undefined field178_0x573;
    undefined field179_0x574;
    undefined field180_0x575;
    undefined field181_0x576;
    undefined field182_0x577;
    undefined field183_0x578;
    undefined field184_0x579;
    undefined field185_0x57a;
    undefined field186_0x57b;
    undefined field187_0x57c;
    undefined field188_0x57d;
    undefined field189_0x57e;
    undefined field190_0x57f;
    undefined field191_0x580;
    undefined field192_0x581;
    undefined field193_0x582;
    undefined field194_0x583;
    undefined field195_0x584;
    undefined field196_0x585;
    undefined field197_0x586;
    undefined field198_0x587;
    undefined field199_0x588;
    undefined field200_0x589;
    undefined field201_0x58a;
    undefined field202_0x58b;
    undefined field203_0x58c;
    undefined field204_0x58d;
    undefined field205_0x58e;
    undefined field206_0x58f;
    undefined field207_0x590;
    undefined field208_0x591;
    undefined field209_0x592;
    undefined field210_0x593;
    undefined field211_0x594;
    undefined field212_0x595;
    undefined field213_0x596;
    undefined field214_0x597;
    undefined field215_0x598;
    undefined field216_0x599;
    undefined field217_0x59a;
    undefined field218_0x59b;
    undefined field219_0x59c;
    undefined field220_0x59d;
    undefined field221_0x59e;
    undefined field222_0x59f;
    undefined field223_0x5a0;
    undefined field224_0x5a1;
    undefined field225_0x5a2;
    undefined field226_0x5a3;
    undefined field227_0x5a4;
    undefined field228_0x5a5;
    undefined field229_0x5a6;
    undefined field230_0x5a7;
    undefined field231_0x5a8;
    undefined field232_0x5a9;
    undefined field233_0x5aa;
    undefined field234_0x5ab;
    undefined field235_0x5ac;
    undefined field236_0x5ad;
    undefined field237_0x5ae;
    undefined field238_0x5af;
    undefined field239_0x5b0;
    undefined field240_0x5b1;
    undefined field241_0x5b2;
    undefined field242_0x5b3;
    undefined field243_0x5b4;
    undefined field244_0x5b5;
    undefined field245_0x5b6;
    undefined field246_0x5b7;
    undefined field247_0x5b8;
    undefined field248_0x5b9;
    undefined field249_0x5ba;
    undefined field250_0x5bb;
    undefined field251_0x5bc;
    undefined field252_0x5bd;
    undefined field253_0x5be;
    undefined field254_0x5bf;
    undefined field255_0x5c0;
    undefined field256_0x5c1;
    undefined field257_0x5c2;
    undefined field258_0x5c3;
    undefined field259_0x5c4;
    undefined field260_0x5c5;
    undefined field261_0x5c6;
    undefined field262_0x5c7;
    undefined field263_0x5c8;
    undefined field264_0x5c9;
    undefined field265_0x5ca;
    undefined field266_0x5cb;
    undefined field267_0x5cc;
    undefined field268_0x5cd;
    undefined field269_0x5ce;
    undefined field270_0x5cf;
    undefined field271_0x5d0;
    undefined field272_0x5d1;
    undefined field273_0x5d2;
    undefined field274_0x5d3;
    undefined field275_0x5d4;
    undefined field276_0x5d5;
    undefined field277_0x5d6;
    undefined field278_0x5d7;
    undefined field279_0x5d8;
    undefined field280_0x5d9;
    undefined field281_0x5da;
    undefined field282_0x5db;
    undefined field283_0x5dc;
    undefined field284_0x5dd;
    undefined field285_0x5de;
    undefined field286_0x5df;
    undefined field287_0x5e0;
    undefined field288_0x5e1;
    undefined field289_0x5e2;
    undefined field290_0x5e3;
    undefined field291_0x5e4;
    undefined field292_0x5e5;
    undefined field293_0x5e6;
    undefined field294_0x5e7;
    undefined field295_0x5e8;
    undefined field296_0x5e9;
    undefined field297_0x5ea;
    undefined field298_0x5eb;
    undefined field299_0x5ec;
    undefined field300_0x5ed;
    undefined field301_0x5ee;
    undefined field302_0x5ef;
    undefined field303_0x5f0;
    undefined field304_0x5f1;
    undefined field305_0x5f2;
    undefined field306_0x5f3;
    undefined field307_0x5f4;
    undefined field308_0x5f5;
    undefined field309_0x5f6;
    undefined field310_0x5f7;
    undefined field311_0x5f8;
    undefined field312_0x5f9;
    undefined field313_0x5fa;
    undefined field314_0x5fb;
    undefined field315_0x5fc;
    undefined field316_0x5fd;
    undefined field317_0x5fe;
    undefined field318_0x5ff;
    undefined field319_0x600;
    undefined field320_0x601;
    undefined field321_0x602;
    undefined field322_0x603;
    undefined field323_0x604;
    undefined field324_0x605;
    undefined field325_0x606;
    undefined field326_0x607;
    undefined field327_0x608;
    undefined field328_0x609;
    undefined field329_0x60a;
    undefined field330_0x60b;
    undefined field331_0x60c;
    undefined field332_0x60d;
    undefined field333_0x60e;
    undefined field334_0x60f;
    undefined field335_0x610;
    undefined field336_0x611;
    undefined field337_0x612;
    undefined field338_0x613;
    undefined field339_0x614;
    undefined field340_0x615;
    undefined field341_0x616;
    undefined field342_0x617;
    undefined field343_0x618;
    undefined field344_0x619;
    undefined field345_0x61a;
    undefined field346_0x61b;
    undefined field347_0x61c;
    undefined field348_0x61d;
    undefined field349_0x61e;
    undefined field350_0x61f;
    undefined field351_0x620;
    undefined field352_0x621;
    undefined field353_0x622;
    undefined field354_0x623;
    undefined field355_0x624;
    undefined field356_0x625;
    undefined field357_0x626;
    undefined field358_0x627;
    undefined field359_0x628;
    undefined field360_0x629;
    undefined field361_0x62a;
    undefined field362_0x62b;
    undefined field363_0x62c;
    undefined field364_0x62d;
    undefined field365_0x62e;
    undefined field366_0x62f;
    undefined field367_0x630;
    undefined field368_0x631;
    undefined field369_0x632;
    undefined field370_0x633;
    undefined field371_0x634;
    undefined field372_0x635;
    undefined field373_0x636;
    undefined field374_0x637;
    undefined field375_0x638;
    undefined field376_0x639;
    undefined field377_0x63a;
    undefined field378_0x63b;
    undefined field379_0x63c;
    undefined field380_0x63d;
    undefined field381_0x63e;
    undefined field382_0x63f;
    undefined field383_0x640;
    undefined field384_0x641;
    undefined field385_0x642;
    undefined field386_0x643;
    undefined field387_0x644;
    undefined field388_0x645;
    undefined field389_0x646;
    undefined field390_0x647;
    undefined field391_0x648;
    undefined field392_0x649;
    undefined field393_0x64a;
    undefined field394_0x64b;
    undefined field395_0x64c;
    undefined field396_0x64d;
    undefined field397_0x64e;
    undefined field398_0x64f;
    undefined field399_0x650;
    undefined field400_0x651;
    undefined field401_0x652;
    undefined field402_0x653;
    undefined field403_0x654;
    undefined field404_0x655;
    undefined field405_0x656;
    undefined field406_0x657;
    undefined field407_0x658;
    undefined field408_0x659;
    undefined field409_0x65a;
    undefined field410_0x65b;
    undefined field411_0x65c;
    undefined field412_0x65d;
    undefined field413_0x65e;
    undefined field414_0x65f;
    undefined field415_0x660;
    undefined field416_0x661;
    undefined field417_0x662;
    undefined field418_0x663;
    undefined field419_0x664;
    undefined field420_0x665;
    undefined field421_0x666;
    undefined field422_0x667;
    undefined field423_0x668;
    undefined field424_0x669;
    undefined field425_0x66a;
    undefined field426_0x66b;
    undefined field427_0x66c;
    undefined field428_0x66d;
    undefined field429_0x66e;
    undefined field430_0x66f;
    undefined field431_0x670;
    undefined field432_0x671;
    undefined field433_0x672;
    undefined field434_0x673;
    undefined field435_0x674;
    int8_t is_mind_controlled;
    undefined field437_0x676;
    undefined field438_0x677;
    undefined field439_0x678;
    undefined field440_0x679;
    undefined field441_0x67a;
    undefined field442_0x67b;
    undefined field443_0x67c;
    undefined field444_0x67d;
    undefined field445_0x67e;
    undefined field446_0x67f;
    undefined field447_0x680;
    undefined field448_0x681;
    undefined field449_0x682;
    undefined field450_0x683;
    undefined field451_0x684;
    undefined field452_0x685;
    undefined field453_0x686;
    undefined field454_0x687;
    undefined field455_0x688;
    undefined field456_0x689;
    undefined field457_0x68a;
    undefined field458_0x68b;
    undefined field459_0x68c;
    undefined field460_0x68d;
    undefined field461_0x68e;
    undefined field462_0x68f;
    undefined field463_0x690;
    undefined field464_0x691;
    undefined field465_0x692;
    undefined field466_0x693;
    undefined field467_0x694;
    undefined field468_0x695;
    undefined field469_0x696;
    undefined field470_0x697;
    undefined field471_0x698;
    undefined field472_0x699;
    undefined field473_0x69a;
    undefined field474_0x69b;
    undefined field475_0x69c;
    undefined field476_0x69d;
    undefined field477_0x69e;
    undefined field478_0x69f;
    undefined field479_0x6a0;
    undefined field480_0x6a1;
    undefined field481_0x6a2;
    undefined field482_0x6a3;
    undefined field483_0x6a4;
    undefined field484_0x6a5;
    undefined field485_0x6a6;
    undefined field486_0x6a7;
    undefined field487_0x6a8;
    undefined field488_0x6a9;
    undefined field489_0x6aa;
    undefined field490_0x6ab;
    undefined field491_0x6ac;
    undefined field492_0x6ad;
    undefined field493_0x6ae;
    undefined field494_0x6af;
    undefined field495_0x6b0;
    undefined field496_0x6b1;
    undefined field497_0x6b2;
    undefined field498_0x6b3;
    undefined field499_0x6b4;
    undefined field500_0x6b5;
    undefined field501_0x6b6;
    undefined field502_0x6b7;
    undefined field503_0x6b8;
    undefined field504_0x6b9;
    undefined field505_0x6ba;
    undefined field506_0x6bb;
    undefined field507_0x6bc;
    undefined field508_0x6bd;
    undefined field509_0x6be;
    undefined field510_0x6bf;
    undefined field511_0x6c0;
    undefined field512_0x6c1;
    undefined field513_0x6c2;
    undefined field514_0x6c3;
    undefined field515_0x6c4;
    undefined field516_0x6c5;
    undefined field517_0x6c6;
    undefined field518_0x6c7;
    undefined field519_0x6c8;
    undefined field520_0x6c9;
    undefined field521_0x6ca;
    int8_t colony_captured_flag;
    undefined field523_0x6cc;
    undefined field524_0x6cd;
};

typedef struct s_beam_fx_entry s_beam_fx_entry, *Ps_beam_fx_entry;

typedef ushort uint16_t;

struct s_beam_fx_entry {
    int16_t weapon_id;
    int16_t shot_count;
    uint16_t result_flags;
    uint16_t special_flag;
    int16_t accum_hit_x;
    int16_t accum_hit_y;
    uint16_t specials;
    int8_t is_damage_entry;
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

typedef struct s_click_through_field s_click_through_field, *Ps_click_through_field;

struct s_click_through_field {
    short f0;
    short f2;
    short f4;
    short f6;
    short f8;
    short fa;
    int fc;
    short f10;
    int f12;
    byte f16;
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

struct s_combat_weapon_slot {
    int16_t weapon_id;
    uint8_t count;
    int8_t arc;
    uint16_t specials;
    int16_t ammo;
    int8_t active;
    int8_t shots_left;
    uint8_t status;
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
    int8_t ftl_type;
    int8_t computer_type;
    int8_t armor_type;
    int16_t tac_val_attack;
    int16_t tac_val_defense;
    int16_t tac_val_evade;
    int8_t combat_speed_base;
    int16_t current_speed;
    int16_t movement_left;
    int8_t target_type_flag; /* 0 = ship, 1 = missle */
    int8_t special_status_flag;
    int8_t special_status_timer;
    int8_t special_cooldown;
    int16_t plasma_web_damage;
    short reflected_damage_pool;
    int8_t field40_0x47;
    uint8_t combat_status_flags; /* bit 0: teleporter used; bit 1: being boarded; bit 2: turn finished */
    undefined field42_0x49;
    int8_t stasis_source_idx;
    int8_t is_retreating;
    uint8_t special_device_flags[5];
    undefined field46_0x51;
    struct s_combat_weapon_slot weapons[8];
    int16_t structure_max;
    int8_t crew_quality;
    int16_t officer_idx;
    int8_t marine_count;
    int8_t is_captured;
    int8_t ai_range_tactic;
    uint8_t special_device_damage_flags[5];
    undefined field55_0xb7;
    uint8_t weapon_ready_flags[8];
    int16_t structure_damage;
    int16_t armor_remaining;
    int16_t picture_num;
    struct s_animation_header *ship_image_seg;
    int16_t drive_max_hits;
    int16_t computer_max_hits;
    int16_t weapon_total_hits[8];
    int16_t weapon_unit_hits[8];
    int16_t special_device_hits[8];
    int16_t shield_max_hits;
    int16_t drive_current_hits;
    int16_t computer_current_hits;
    int16_t weapon_hits[8];
    int16_t weapon_hits_per_unit[8];
    int16_t special_hits[8];
    short shield_current_hits;
    int8_t black_hole_flag;
    int8_t black_hole_source_idx;
    int8_t previous_owner;
};

typedef struct s_combfind_star_has_player s_combfind_star_has_player, *Ps_combfind_star_has_player;

struct s_combfind_star_has_player {
    int8_t ship_sizes[5];
    int8_t has_transport;
    int8_t has_player;
    int8_t colony_count;
    short transport_count;
    int8_t transport_unk;
};

typedef struct s_combfind_target s_combfind_target, *Ps_combfind_target;

struct s_combfind_target {
    int16_t colony_targets[5];
    int16_t num_colony_targets;
    int16_t player_targets[15];
    int16_t num_player_targets;
};

typedef struct s_crew_data s_crew_data, *Ps_crew_data;

struct s_crew_data {
    uint16_t experience;
    int16_t attack;
    int16_t defense;
    int16_t ground;
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
    int8_t player_idx;
    int8_t status; /* 0:Inactive, 1:Triggered, 2:Active, 3:Expired, 5:Finishing, 6:Just Started */
    int8_t turn_count;
    short target_id;
    short amount;
    short max_amount;
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
    int8_t field6_0x24;
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
    int8_t marine_count;
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
    int16_t drive_hp_max;
    int16_t computer_hp_max;
    undefined field55_0xce;
    undefined field56_0xcf;
    undefined field57_0xd0;
    undefined field58_0xd1;
    undefined field59_0xd2;
    undefined field60_0xd3;
    undefined field61_0xd4;
    undefined field62_0xd5;
    undefined field63_0xd6;
    undefined field64_0xd7;
    undefined field65_0xd8;
    undefined field66_0xd9;
    undefined field67_0xda;
    undefined field68_0xdb;
    undefined field69_0xdc;
    undefined field70_0xdd;
    undefined field71_0xde;
    undefined field72_0xdf;
    undefined field73_0xe0;
    undefined field74_0xe1;
    undefined field75_0xe2;
    undefined field76_0xe3;
    undefined field77_0xe4;
    undefined field78_0xe5;
    undefined field79_0xe6;
    undefined field80_0xe7;
    undefined field81_0xe8;
    undefined field82_0xe9;
    undefined field83_0xea;
    undefined field84_0xeb;
    undefined field85_0xec;
    undefined field86_0xed;
    undefined field87_0xee;
    undefined field88_0xef;
    undefined field89_0xf0;
    undefined field90_0xf1;
    undefined field91_0xf2;
    undefined field92_0xf3;
    undefined field93_0xf4;
    undefined field94_0xf5;
    undefined field95_0xf6;
    undefined field96_0xf7;
    undefined field97_0xf8;
    undefined field98_0xf9;
    undefined field99_0xfa;
    undefined field100_0xfb;
    undefined field101_0xfc;
    undefined field102_0xfd;
    int16_t shield_hp_max;
    int16_t drive_effectiveness_pct;
    int16_t computer_effectiveness_pct;
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
    int16_t shield_hp_current;
    undefined field155_0x136;
    undefined field156_0x137;
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

typedef struct s_missile s_missile, *Ps_missile;

struct s_missile {
    short type;
    short owner;
    short source_unit_idx;
    short target_unit_idx;
    sbyte is_anti_missile_rocket;
    short x;
    short y;
    short hits_taken;
    int16_t quantity;
    uint16_t specials;
    int16_t travel_dist;
    int8_t facing_dir;
    int8_t speed;
    int8_t fighter_count;
    undefined field14_0x18;
    int8_t is_active;
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

typedef struct s_mouse_list s_mouse_list, *Ps_mouse_list;

struct s_mouse_list {
    int16_t icon;
    int16_t frame;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
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
    struct s_animation_header *res_setup_bg;
    undefined field5_0x17;
    undefined field6_0x18;
    undefined field7_0x19;
    undefined field8_0x1a;
    struct s_animation_header *anim_btn_unk_4;
    undefined field10_0x1f;
    undefined field11_0x20;
    undefined field12_0x21;
    undefined field13_0x22;
    undefined field14_0x23;
    undefined field15_0x24;
    undefined field16_0x25;
    undefined field17_0x26;
    undefined field18_0x27;
    undefined field19_0x28;
    undefined field20_0x29;
    undefined field21_0x2a;
    undefined field22_0x2b;
    undefined field23_0x2c;
    undefined field24_0x2d;
    undefined field25_0x2e;
    struct s_animation_header *bg_seg;
    undefined field27_0x33;
    undefined field28_0x34;
    undefined field29_0x35;
    undefined field30_0x36;
    undefined field31_0x37;
    undefined field32_0x38;
    undefined field33_0x39;
    undefined field34_0x3a;
    undefined field35_0x3b;
    undefined field36_0x3c;
    undefined field37_0x3d;
    undefined field38_0x3e;
    undefined field39_0x3f;
    undefined field40_0x40;
    undefined field41_0x41;
    undefined field42_0x42;
    undefined field43_0x43;
    undefined field44_0x44;
    undefined field45_0x45;
    undefined field46_0x46;
    undefined field47_0x47;
    undefined field48_0x48;
    undefined field49_0x49;
    undefined field50_0x4a;
    undefined field51_0x4b;
    undefined field52_0x4c;
    undefined field53_0x4d;
    undefined field54_0x4e;
    undefined field55_0x4f;
    undefined field56_0x50;
    undefined field57_0x51;
    undefined field58_0x52;
    struct s_animation_header *btn_accept_off;
    struct s_animation_header *btn_accept_on;
    undefined field61_0x5b;
    undefined field62_0x5c;
    undefined field63_0x5d;
    undefined field64_0x5e;
    struct s_animation_header *anim_ten;
    struct s_animation_header *anim_modem;
    struct s_animation_header *anim_serial;
    struct s_animation_header *anim_network;
    struct s_animation_header *anim_btn_unk_3;
    struct s_animation_header *anim_btn_unk_2;
    undefined field71_0x77;
    undefined field72_0x78;
    undefined field73_0x79;
    undefined field74_0x7a;
    struct s_animation_header *anim_btn_next;
    struct s_animation_header *anim_btn_accept;
    struct s_animation_header *anim_btn_back;
    struct s_animation_header *anim_btn_setup;
    struct s_animation_header *anim_btn_unk_1;
    undefined field80_0x8f;
    undefined field81_0x90;
    undefined field82_0x91;
    undefined field83_0x92;
    undefined field84_0x93;
    undefined field85_0x94;
    undefined field86_0x95;
    undefined field87_0x96;
    undefined field88_0x97;
    undefined field89_0x98;
    undefined field90_0x99;
    undefined field91_0x9a;
    undefined field92_0x9b;
    undefined field93_0x9c;
    undefined field94_0x9d;
    undefined field95_0x9e;
    undefined field96_0x9f;
    undefined field97_0xa0;
    undefined field98_0xa1;
    undefined field99_0xa2;
    undefined field100_0xa3;
    undefined field101_0xa4;
    undefined field102_0xa5;
    undefined field103_0xa6;
    int16_t game_list_field_ids[10];
    int16_t player_field_ids[8];
    int8_t save_slot_valid[10];
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
    undefined field119_0xed;
    undefined field120_0xee;
    undefined field121_0xef;
    undefined field122_0xf0;
    int16_t lbx_entry_num;
    int16_t screen_x;
    int16_t screen_y;
    int16_t flag_ten_disabled;
    int16_t flag_modem_disabled;
    int16_t flag_serial_type;
    int16_t flag_network_disabled;
    short field130_0xff;
    int16_t last_scan_id;
    undefined field132_0x103;
    undefined field133_0x104;
    undefined field134_0x105;
    undefined field135_0x106;
    undefined field136_0x107;
    undefined field137_0x108;
    undefined field138_0x109;
    undefined field139_0x10a;
    undefined field140_0x10b;
    int16_t bg_height;
    int8_t subscreen_mode;
    undefined field143_0x10f;
    int8_t player_button_status[10];
    undefined field145_0x11a;
    undefined field146_0x11b;
    undefined field147_0x11c;
    undefined field148_0x11d;
    undefined field149_0x11e;
    undefined field150_0x11f;
    undefined field151_0x120;
    undefined field152_0x121;
    undefined field153_0x122;
    undefined field154_0x123;
    undefined field155_0x124;
    undefined field156_0x125;
    undefined field157_0x126;
    undefined field158_0x127;
    undefined field159_0x128;
    undefined field160_0x129;
    undefined field161_0x12a;
    undefined field162_0x12b;
    undefined field163_0x12c;
    undefined field164_0x12d;
    undefined field165_0x12e;
    undefined field166_0x12f;
    undefined field167_0x130;
    undefined field168_0x131;
    undefined field169_0x132;
    undefined field170_0x133;
    undefined field171_0x134;
    undefined field172_0x135;
    undefined field173_0x136;
    undefined field174_0x137;
    undefined field175_0x138;
    undefined field176_0x139;
    undefined field177_0x13a;
    undefined field178_0x13b;
    undefined field179_0x13c;
    undefined field180_0x13d;
    undefined field181_0x13e;
    undefined field182_0x13f;
    undefined field183_0x140;
    undefined field184_0x141;
    undefined field185_0x142;
    undefined field186_0x143;
    undefined field187_0x144;
    undefined field188_0x145;
    undefined field189_0x146;
    undefined field190_0x147;
    undefined field191_0x148;
    undefined field192_0x149;
    undefined field193_0x14a;
    undefined field194_0x14b;
    undefined field195_0x14c;
    undefined field196_0x14d;
    undefined field197_0x14e;
    undefined field198_0x14f;
    undefined field199_0x150;
    undefined field200_0x151;
    undefined field201_0x152;
    undefined field202_0x153;
    undefined field203_0x154;
    undefined field204_0x155;
    undefined field205_0x156;
    undefined field206_0x157;
    undefined field207_0x158;
    undefined field208_0x159;
    undefined field209_0x15a;
    undefined field210_0x15b;
    undefined field211_0x15c;
    undefined field212_0x15d;
    undefined field213_0x15e;
    undefined field214_0x15f;
    undefined field215_0x160;
    undefined field216_0x161;
    undefined field217_0x162;
    undefined field218_0x163;
    undefined field219_0x164;
    undefined field220_0x165;
    undefined field221_0x166;
    undefined field222_0x167;
    undefined field223_0x168;
    undefined field224_0x169;
    undefined field225_0x16a;
    undefined field226_0x16b;
    undefined field227_0x16c;
    undefined field228_0x16d;
    undefined field229_0x16e;
    undefined field230_0x16f;
    undefined field231_0x170;
    undefined field232_0x171;
    undefined field233_0x172;
    undefined field234_0x173;
    undefined field235_0x174;
    undefined field236_0x175;
    undefined field237_0x176;
    undefined field238_0x177;
    undefined field239_0x178;
    undefined field240_0x179;
    undefined field241_0x17a;
    undefined field242_0x17b;
    undefined field243_0x17c;
    undefined field244_0x17d;
    undefined field245_0x17e;
    undefined field246_0x17f;
    undefined field247_0x180;
    undefined field248_0x181;
    undefined field249_0x182;
    undefined field250_0x183;
    undefined field251_0x184;
    undefined field252_0x185;
    undefined field253_0x186;
    undefined field254_0x187;
    undefined field255_0x188;
    undefined field256_0x189;
    undefined field257_0x18a;
    undefined field258_0x18b;
    undefined field259_0x18c;
    undefined field260_0x18d;
    undefined field261_0x18e;
    undefined field262_0x18f;
    undefined field263_0x190;
    undefined field264_0x191;
    undefined field265_0x192;
    undefined field266_0x193;
    undefined field267_0x194;
    undefined field268_0x195;
    undefined field269_0x196;
    undefined field270_0x197;
    undefined field271_0x198;
    undefined field272_0x199;
    undefined field273_0x19a;
    undefined field274_0x19b;
    undefined field275_0x19c;
    undefined field276_0x19d;
    undefined field277_0x19e;
    undefined field278_0x19f;
    undefined field279_0x1a0;
    undefined field280_0x1a1;
    undefined field281_0x1a2;
    undefined field282_0x1a3;
    undefined field283_0x1a4;
    undefined field284_0x1a5;
    undefined field285_0x1a6;
    undefined field286_0x1a7;
    undefined field287_0x1a8;
    undefined field288_0x1a9;
    undefined field289_0x1aa;
    undefined field290_0x1ab;
    undefined field291_0x1ac;
    undefined field292_0x1ad;
    undefined field293_0x1ae;
    undefined field294_0x1af;
    undefined field295_0x1b0;
    undefined field296_0x1b1;
    undefined field297_0x1b2;
    undefined field298_0x1b3;
    undefined field299_0x1b4;
    undefined field300_0x1b5;
    undefined field301_0x1b6;
    undefined field302_0x1b7;
    undefined field303_0x1b8;
    undefined field304_0x1b9;
    undefined field305_0x1ba;
    undefined field306_0x1bb;
    undefined field307_0x1bc;
    undefined field308_0x1bd;
    undefined field309_0x1be;
    undefined field310_0x1bf;
    undefined field311_0x1c0;
    undefined field312_0x1c1;
    undefined field313_0x1c2;
    undefined field314_0x1c3;
    undefined field315_0x1c4;
    undefined field316_0x1c5;
    undefined field317_0x1c6;
    undefined field318_0x1c7;
    undefined field319_0x1c8;
    undefined field320_0x1c9;
    undefined field321_0x1ca;
    undefined field322_0x1cb;
    undefined field323_0x1cc;
    undefined field324_0x1cd;
    undefined field325_0x1ce;
    undefined field326_0x1cf;
    undefined field327_0x1d0;
    undefined field328_0x1d1;
    undefined field329_0x1d2;
    undefined field330_0x1d3;
    undefined field331_0x1d4;
    undefined field332_0x1d5;
    undefined field333_0x1d6;
    undefined field334_0x1d7;
    undefined field335_0x1d8;
    undefined field336_0x1d9;
    undefined field337_0x1da;
    undefined field338_0x1db;
    undefined field339_0x1dc;
    undefined field340_0x1dd;
    undefined field341_0x1de;
    undefined field342_0x1df;
    int8_t is_active;
    undefined field344_0x1e1;
    undefined field345_0x1e2;
    short force_player_selection;
    short forced_player_idx;
    int8_t exit_screen_flag;
    int8_t sel_anim_frame;
    int8_t sel_anim_active;
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
    short y;
    int8_t type;
};

typedef struct s_net_combat_message s_net_combat_message, *Ps_net_combat_message;

struct s_net_combat_message {
    short x;
    short y;
    int f4;
    short f8;
    short f10;
    short f12;
    short f14;
};

typedef struct s_net_player_info s_net_player_info, *Ps_net_player_info;

struct s_net_player_info {
    uint8_t packet_sync_status;
    undefined field1_0x1;
    int16_t sync_star_id;
    int16_t sync_planet_id;
    int16_t sync_colony_id;
    undefined field5_0x8;
    undefined field6_0x9;
    int8_t combat_status;
    int8_t remote_player_id;
};

typedef struct s_network s_network, *Ps_network;

struct s_network {
    struct s_modem_settings modem_settings;
    char modem_init_str[11];
    undefined field2_0x8e;
    undefined field3_0x8f;
    undefined field4_0x90;
    undefined field5_0x91;
    undefined field6_0x92;
    int8_t modem_initialized;
    uint8_t mode; /* 3=ten 1/2=modem/ip? 0=offline */
    int field9_0x95;
    int8_t num_connected_players;
    short net_host_id;
    int8_t player_connected_flags[8];
    short host_election_id;
    uint32_t total_bytes_received;
    int16_t received_chunk_count;
    int16_t total_chunk_count;
    void *network_data_ptr;
    int8_t host_election_player_rank[8];
    int8_t dropped_players[8];
    short local_net_id;
    int8_t host_active_player_id;
    int8_t host_sync_counter;
    undefined field23_0xc6;
    struct s_net_player_info player_net_info[8];
    undefined field25_0x127;
    undefined field26_0x128;
    undefined field27_0x129;
    undefined field28_0x12a;
    undefined field29_0x12b;
    undefined field30_0x12c;
    undefined field31_0x12d;
    undefined field32_0x12e;
    undefined field33_0x12f;
    undefined field34_0x130;
    undefined field35_0x131;
    undefined field36_0x132;
    undefined field37_0x133;
    undefined field38_0x134;
    undefined field39_0x135;
    undefined field40_0x136;
    undefined field41_0x137;
    undefined field42_0x138;
    undefined field43_0x139;
    undefined field44_0x13a;
    undefined field45_0x13b;
    undefined field46_0x13c;
    undefined field47_0x13d;
    undefined field48_0x13e;
    undefined field49_0x13f;
    undefined field50_0x140;
    undefined field51_0x141;
    undefined field52_0x142;
    undefined field53_0x143;
    undefined field54_0x144;
    undefined field55_0x145;
    undefined field56_0x146;
    undefined field57_0x147;
    undefined field58_0x148;
    undefined field59_0x149;
    undefined field60_0x14a;
    undefined field61_0x14b;
    undefined field62_0x14c;
    undefined field63_0x14d;
    undefined field64_0x14e;
    undefined field65_0x14f;
    undefined field66_0x150;
    undefined field67_0x151;
    undefined field68_0x152;
    undefined field69_0x153;
    undefined field70_0x154;
    undefined field71_0x155;
    undefined field72_0x156;
    undefined field73_0x157;
    undefined field74_0x158;
    undefined field75_0x159;
    undefined field76_0x15a;
    undefined field77_0x15b;
    undefined field78_0x15c;
    undefined field79_0x15d;
    undefined field80_0x15e;
    undefined field81_0x15f;
    undefined field82_0x160;
    undefined field83_0x161;
    undefined field84_0x162;
    undefined field85_0x163;
    undefined field86_0x164;
    undefined field87_0x165;
    undefined field88_0x166;
    undefined field89_0x167;
    undefined field90_0x168;
    undefined field91_0x169;
    undefined field92_0x16a;
    undefined field93_0x16b;
    undefined field94_0x16c;
    undefined field95_0x16d;
    undefined field96_0x16e;
    undefined field97_0x16f;
    undefined field98_0x170;
    undefined field99_0x171;
    undefined field100_0x172;
    undefined field101_0x173;
    undefined field102_0x174;
    undefined field103_0x175;
    undefined field104_0x176;
    undefined field105_0x177;
    undefined field106_0x178;
    undefined field107_0x179;
    undefined field108_0x17a;
    byte packet_type;
    int8_t combat_star_id;
    int16_t combat_defender_id;
    int16_t combat_planet_id;
    int16_t combat_target_player_id;
    undefined field114_0x183;
    int8_t combat_attacker_id;
    undefined field116_0x185;
    int8_t diplomacy_target_id;
    int8_t sync_phase_active_flag;
    struct s_battle_results *combat_results_data;
    undefined field120_0x18c;
    undefined field121_0x18d;
    undefined field122_0x18e;
    undefined field123_0x18f;
    uint8_t players_active_mask;
    uint8_t players_sync_mask;
    uint8_t players_disconnected_mask;
    uint8_t players_ignoring_mask;
    undefined field128_0x194;
    uint8_t sync_ack_count;
    uint8_t client_priority_flag;
    undefined field131_0x197;
    undefined field132_0x198;
    undefined field133_0x199;
    undefined field134_0x19a;
    undefined field135_0x19b;
    undefined field136_0x19c;
    undefined field137_0x19d;
    undefined field138_0x19e;
    undefined field139_0x19f;
    undefined field140_0x1a0;
    undefined field141_0x1a1;
    int8_t is_diplomacy_initiator;
    undefined field143_0x1a3;
    undefined field144_0x1a4;
    uint8_t players_ready_mask;
    undefined field146_0x1a6;
    undefined field147_0x1a7;
    undefined field148_0x1a8;
    undefined field149_0x1a9;
    undefined field150_0x1aa;
    undefined field151_0x1ab;
    undefined field152_0x1ac;
    undefined field153_0x1ad;
    int8_t player_is_active_flags[8];
    undefined field155_0x1b6;
    undefined field156_0x1b7;
    undefined field157_0x1b8;
    undefined field158_0x1b9;
    undefined field159_0x1ba;
    undefined field160_0x1bb;
    undefined field161_0x1bc;
    undefined field162_0x1bd;
    char cur_game_name[9];
    int8_t player_color_selected_flags[8];
    undefined field165_0x1cf;
    int8_t player_race_selected_flags[8];
    short host_election_winner_id;
    char field_1da;
    int field_1db;
    int field_1df;
    undefined field171_0x1e3;
    undefined field172_0x1e4;
    undefined field173_0x1e5;
    undefined field174_0x1e6;
    undefined field175_0x1e7;
    undefined field176_0x1e8;
    undefined field177_0x1e9;
    undefined field178_0x1ea;
    undefined field179_0x1eb;
    undefined field180_0x1ec;
    undefined field181_0x1ed;
    undefined field182_0x1ee;
    undefined field183_0x1ef;
    undefined field184_0x1f0;
    undefined field185_0x1f1;
    undefined field186_0x1f2;
    undefined field187_0x1f3;
    undefined field188_0x1f4;
    undefined field189_0x1f5;
    undefined field190_0x1f6;
    undefined field191_0x1f7;
    undefined field192_0x1f8;
    undefined field193_0x1f9;
    undefined field194_0x1fa;
    undefined field195_0x1fb;
    undefined field196_0x1fc;
    uint8_t network_sync_complete_flag;
    undefined field198_0x1fe;
    int8_t connection_timeout_flag;
    int8_t connection_state;
    undefined field201_0x201;
    undefined field202_0x202;
    int8_t diplomacy_partner_id;
    int8_t diplomacy_handshake_state;
    undefined field205_0x205;
    undefined field206_0x206;
    int8_t diplomacy_initiator_id;
    int8_t diplomacy_active_flag;
    undefined field209_0x209;
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
    bool colony_available;
    bool output_available;
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

typedef struct s_qship s_qship, *Ps_qship;

struct s_qship { /* Strategic (quick) combat ships, used in strategic combat. The structure is smaller than for tactical ships. Named after global MOX::_qcombat_data. */
    int8_t player;
    byte size;
    short ship_design_idx;
    short field3_0x4;
    int8_t status; /* 0: Active, 1: Destroyed, 2: Inactive */
    int16_t officer_idx;
    int16_t shield_id;
    int16_t missile_weapon_id;
    short beam_weapon_id;
    short bomb_weapon_id;
    int16_t special_weapon_id;
    short missile_defense_bonus;
    short beam_defense_bonus;
    short base_ocv;
    short beam_damage_modifier;
    int16_t missile_damage_modifier;
    int16_t max_hits;
    int16_t structural_damage;
};

typedef struct s_report s_report, *Ps_report;

typedef char __int8_t;

struct s_report {
    int field0_0x0;
    int field1_0x4;
    int field2_0x8;
    int field3_0xc;
    short field4_0x10;
    __int8_t field5_0x12;
};

typedef struct s_saved_box s_saved_box, *Ps_saved_box;

struct s_saved_box {
    short f0;
    __int8_t f2;
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
    sbyte music_level;
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
    int8_t modem_initialized;
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
    uint8_t skill_id;
    uint skill_mask;
    uint8_t skill_type;
    uint16_t strength;
    uint8_t level_up;
    uint8_t cost;
    char *name;
    char *format_str;
};

typedef struct s_sound_index s_sound_index, *Ps_sound_index;

struct s_sound_index {
    int field_0;
    int field_4;
    int field_8;
    byte field_c;
    undefined field4_0xd;
    undefined field5_0xe;
    undefined field6_0xf;
    undefined field7_0x10;
    undefined field8_0x11;
    undefined field9_0x12;
    undefined field10_0x13;
    undefined field11_0x14;
    undefined field12_0x15;
    undefined field13_0x16;
    undefined field14_0x17;
    undefined field15_0x18;
    undefined field16_0x19;
    undefined field17_0x1a;
    undefined field18_0x1b;
    int field_1c;
    int field_20;
    int field_24;
};

typedef struct s_spcl s_spcl, *Ps_spcl;

struct s_spcl {
    uint8_t id;
    int8_t special_or_weapon_id;
    uint8_t type;
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
    uint16_t tech_app_id;
    int16_t tech_field_id;
    uint8_t unk_class;
    uint8_t ai_group;
    byte unknown1;
    uint8_t in_strategic;
    uint8_t type;
};

typedef struct s_tech_armor_data s_tech_armor_data, *Ps_tech_armor_data;

struct s_tech_armor_data {
    char *name;
    short armor_id;
    short tech_app_id; /* Type 'TECH_APP' was deleted */
    undefined field3_0x8;
    uint8_t troops_bonus;
    uint16_t ships_bonus;
    uint16_t cost_percent;
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
    uint16_t computer_id;
    uint16_t tech_app_id;
    uint16_t cost_by_ship_type[6];
    uint16_t bonus;
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
    uint16_t drive_id;
    uint16_t tech_app_id;
    uint16_t space_per_ship_type[6];
    uint16_t cost_per_ship_type[6];
    uint8_t combat_speed_min[6];
    uint8_t combat_speed_max[6];
    uint16_t warp_speed;
};

typedef struct s_tech_field_data s_tech_field_data, *Ps_tech_field_data;

struct s_tech_field_data {
    char *name;
    short tech_field_id;
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
    uint16_t fuel_id;
    uint16_t tech_app_id;
    uint16_t ship_range;
};

typedef struct s_tech_hull_data s_tech_hull_data, *Ps_tech_hull_data;

struct s_tech_hull_data {
    short hull_space;
    short hull_size;
    undefined1 marine_count; /* Created by Rename Structure Field action */
    undefined field3_0x5;
    short armor_absorption;
    short structure;
    short computer_hits;
    short drive_hits;
    short shield_hits;
    short beam_weapon_count;
    short missle_weapon_count;
    short special_weapon_count;
    short bomb_weapon_count;
    short base_beam_defense;
    short base_structure;
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
    int8_t category;
    int16_t bonus_value;
};

typedef struct s_tech_weapon_data s_tech_weapon_data, *Ps_tech_weapon_data;

struct s_tech_weapon_data {
    char *name;
    char *name_plural;
    undefined field2_0x8;
    undefined field3_0x9;
    short tech_application_id;
    sbyte rack;
    int8_t base_ammo;
    short base_size;
    short cost;
    ushort flags;
    short min_damage;
    short damage;
    undefined field12_0x18;
    undefined1 ai_priority; /* Created by Rename Structure Field action */
    uint16_t specials;
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

typedef struct s_techdata_planetary_shield s_techdata_planetary_shield, *Ps_techdata_planetary_shield;

struct s_techdata_planetary_shield {
    undefined field0_0x0;
    undefined field1_0x1;
    int8_t absorption;
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

