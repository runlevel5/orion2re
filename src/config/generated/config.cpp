// Generated from specs/config/main.json5. Do not edit.
#include "config/config_runtime.h"
#include <cstring>
#include "game/boarding.h"
#include "game/build_queue.h"
#include "game/cmbtfire.h"
#include "game/enhanced_ux.h"
#include "game/mox.h"
#include "game/ship_config.h"
#include "game/shipstak.h"

namespace config {

static const s_symbol_descriptor generated_values_0[] = {
    { "end", 0 },
    { "append", 1 },
    { "prepend", 2 },
    { "merge", 3 },
    { "buy", 4 },
    { "buy10", 5 },
    { "buy50", 6 },
    { "buy75", 7 },
    { "unbuy", 8 },
    { "clear", 9 },
    { "clear_refits", 10 },
    { "stop", 11 },
    { "housing", 12 },
    { "trade_goods", 13 },
    { "freighter_fleet", 14 },
    { "spy", 15 },
    { "android_farmer", 16 },
    { "android_worker", 17 },
    { "android_scientist", 18 },
    { "repeat", 19 },
    { "colony_ship", 20 },
    { "outpost_ship", 21 },
    { "transport", 22 },
    { "design1", 23 },
    { "design2", 24 },
    { "design3", 25 },
    { "design4", 26 },
    { "design5", 27 },
    { "design6", 28 },
    { "clear_continuous", 29 },
    { "alien_control_center", 32 },
    { "armor_barracks", 33 },
    { "artemis_system_net", 34 },
    { "astro_university", 35 },
    { "atmosphere_renewer", 36 },
    { "autolab", 37 },
    { "automated_factory", 38 },
    { "battlestation", 39 },
    { "capitol", 40 },
    { "cloning_center", 41 },
    { "colony_base", 42 },
    { "deep_core_mine", 43 },
    { "core_waste_dump", 44 },
    { "dimensional_portal", 45 },
    { "biospheres", 46 },
    { "food_replicators", 47 },
    { "gaia_transformation", 48 },
    { "currency_exchange", 49 },
    { "galactic_cybernet", 50 },
    { "holo_simulator", 51 },
    { "hydroponic_farm", 52 },
    { "marine_barracks", 53 },
    { "barrier_shield", 54 },
    { "flux_shield", 55 },
    { "gravity_generator", 56 },
    { "missile_base", 57 },
    { "ground_batteries", 58 },
    { "radiation_shield", 59 },
    { "stock_exchange", 60 },
    { "supercomputer", 61 },
    { "pleasure_dome", 62 },
    { "pollution_processor", 63 },
    { "recyclotron", 64 },
    { "robotic_factory", 65 },
    { "research_lab", 66 },
    { "robo_miner_plant", 67 },
    { "soil_enrichment", 68 },
    { "space_academy", 69 },
    { "spaceport", 70 },
    { "star_base", 71 },
    { "star_fortress", 72 },
    { "stellar_converter", 73 },
    { "subterranean_farms", 74 },
    { "terraforming", 75 },
    { "warp_interdictor", 76 },
    { "weather_controller", 77 },
    { "fighter_garrison", 78 },
    { "artificial_planet", 79 },
};

static const s_symbol_descriptor generated_values_1[] = {
    { "frigate", 0 },
    { "destroyer", 1 },
    { "cruiser", 2 },
    { "battleship", 3 },
    { "titan", 4 },
};

static const s_symbol_descriptor generated_values_2[] = {
    { "none", 0 },
    { "nuclear", 1 },
    { "fusion", 2 },
    { "ion", 3 },
    { "anti_matter", 4 },
    { "hyper", 5 },
    { "interphased", 6 },
};

static const s_symbol_descriptor generated_values_3[] = {
    { "none", 0 },
    { "titanium", 1 },
    { "tritanium", 2 },
    { "zortrium", 3 },
    { "neutronium", 4 },
    { "adamantium", 5 },
    { "xentronium", 6 },
};

static const s_symbol_descriptor generated_values_4[] = {
    { "none", 0 },
    { "class_i", 1 },
    { "class_iii", 2 },
    { "class_v", 3 },
    { "class_vii", 4 },
    { "class_x", 5 },
};

static const s_symbol_descriptor generated_values_5[] = {
    { "none", 0 },
    { "electronic", 1 },
    { "optronic", 2 },
    { "positronic", 3 },
    { "cybertronic", 4 },
    { "moleculartronic", 5 },
};

static const s_symbol_descriptor generated_values_6[] = {
    { "none", 0 },
    { "mass_driver", 1 },
    { "gauss_cannon", 2 },
    { "laser_cannon", 3 },
    { "particle_beam", 4 },
    { "fusion_beam", 5 },
    { "ion_pulse_cannon", 6 },
    { "graviton_beam", 7 },
    { "neutron_blaster", 8 },
    { "phasor", 9 },
    { "disrupter", 10 },
    { "death_ray", 11 },
    { "plasma_cannon", 12 },
    { "spatial_compressor", 13 },
    { "nuclear_missile", 14 },
    { "merculite_missile", 15 },
    { "pulson_missile", 16 },
    { "zeon_missile", 17 },
    { "anti_matter_torpedo", 18 },
    { "proton_torpedo", 19 },
    { "plasma_torpedo", 20 },
    { "nuclear_bomb", 21 },
    { "fusion_bomb", 22 },
    { "anti_matter_bomb", 23 },
    { "neutronium_bomb", 24 },
    { "death_spore", 25 },
    { "bio_terminator", 26 },
    { "mauler_device", 27 },
    { "assault_shuttle", 28 },
    { "heavy_fighter", 29 },
    { "bomber", 30 },
    { "interceptor", 31 },
    { "stasis_field", 32 },
    { "anti_missile_rocket", 33 },
    { "gyro_destabilizer", 34 },
    { "plasma_web", 35 },
    { "pulsar", 36 },
    { "black_hole_generator", 37 },
    { "stellar_converter", 38 },
    { "tractor_beam", 39 },
    { "dragon_breath", 40 },
    { "phasor_eye", 41 },
    { "crystal_ray", 42 },
    { "plasma_breath", 43 },
    { "plasma_flux", 44 },
    { "caustic_slime", 45 },
};

static const s_symbol_descriptor generated_values_7[] = {
    { "none", 0 },
    { "forward", 1 },
    { "forward_extended", 2 },
    { "back_extended", 4 },
    { "rear", 8 },
    { "all_sectors", 15 },
    { "design_360", 16 },
};

static const s_symbol_descriptor generated_values_8[] = {
    { "none", 0 },
};

static const s_symbol_descriptor generated_values_9[] = {
    { "none", 0 },
    { "race", 1 },
    { "race_tech", 2 },
};

static const s_symbol_descriptor generated_flags_0[] = {
    { "reserved_0", 1 },
    { "hv", 2 },
    { "pd", 4 },
    { "ap", 8 },
    { "co", 16 },
    { "nr", 32 },
    { "sp", 64 },
    { "af", 128 },
    { "env", 256 },
    { "mv", 512 },
    { "eccm", 1024 },
    { "arm", 2048 },
    { "fst", 4096 },
    { "emg", 8192 },
    { "ovr", 16384 },
};

static const uint8_t generated_bitmap_value_0_symbol_0[] = { 0, 0, 0, 0, 0 };
static const s_bitmap_symbol_descriptor generated_bitmap_values_0[] = {
    { "none", generated_bitmap_value_0_symbol_0 },
};

static const uint8_t generated_bitmap_flag_0_symbol_0[] = { 2, 0, 0, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_1[] = { 4, 0, 0, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_2[] = { 8, 0, 0, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_3[] = { 16, 0, 0, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_4[] = { 32, 0, 0, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_5[] = { 64, 0, 0, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_6[] = { 128, 0, 0, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_7[] = { 0, 1, 0, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_8[] = { 0, 2, 0, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_9[] = { 0, 4, 0, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_10[] = { 0, 8, 0, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_11[] = { 0, 16, 0, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_12[] = { 0, 32, 0, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_13[] = { 0, 64, 0, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_14[] = { 0, 128, 0, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_15[] = { 0, 0, 1, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_16[] = { 0, 0, 2, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_17[] = { 0, 0, 4, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_18[] = { 0, 0, 8, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_19[] = { 0, 0, 16, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_20[] = { 0, 0, 32, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_21[] = { 0, 0, 64, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_22[] = { 0, 0, 128, 0, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_23[] = { 0, 0, 0, 1, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_24[] = { 0, 0, 0, 2, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_25[] = { 0, 0, 0, 4, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_26[] = { 0, 0, 0, 8, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_27[] = { 0, 0, 0, 16, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_28[] = { 0, 0, 0, 32, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_29[] = { 0, 0, 0, 64, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_30[] = { 0, 0, 0, 128, 0 };
static const uint8_t generated_bitmap_flag_0_symbol_31[] = { 0, 0, 0, 0, 1 };
static const uint8_t generated_bitmap_flag_0_symbol_32[] = { 0, 0, 0, 0, 2 };
static const uint8_t generated_bitmap_flag_0_symbol_33[] = { 0, 0, 0, 0, 4 };
static const uint8_t generated_bitmap_flag_0_symbol_34[] = { 0, 0, 0, 0, 8 };
static const uint8_t generated_bitmap_flag_0_symbol_35[] = { 0, 0, 0, 0, 16 };
static const uint8_t generated_bitmap_flag_0_symbol_36[] = { 0, 0, 0, 0, 32 };
static const uint8_t generated_bitmap_flag_0_symbol_37[] = { 0, 0, 0, 0, 64 };
static const uint8_t generated_bitmap_flag_0_symbol_38[] = { 0, 0, 0, 0, 128 };
static const s_bitmap_symbol_descriptor generated_bitmap_flags_0[] = {
    { "achilles_targeting_unit", generated_bitmap_flag_0_symbol_0 },
    { "augmented_engines", generated_bitmap_flag_0_symbol_1 },
    { "automated_repair_unit", generated_bitmap_flag_0_symbol_2 },
    { "battle_pods", generated_bitmap_flag_0_symbol_3 },
    { "battle_scanner", generated_bitmap_flag_0_symbol_4 },
    { "cloaking_device", generated_bitmap_flag_0_symbol_5 },
    { "damper_field", generated_bitmap_flag_0_symbol_6 },
    { "displacement_device", generated_bitmap_flag_0_symbol_7 },
    { "ecm_jammer", generated_bitmap_flag_0_symbol_8 },
    { "energy_absorber", generated_bitmap_flag_0_symbol_9 },
    { "extended_fuel_tanks", generated_bitmap_flag_0_symbol_10 },
    { "fast_missile_racks", generated_bitmap_flag_0_symbol_11 },
    { "hard_shields", generated_bitmap_flag_0_symbol_12 },
    { "heavy_armor", generated_bitmap_flag_0_symbol_13 },
    { "high_energy_focus", generated_bitmap_flag_0_symbol_14 },
    { "hyper_x_capacitors", generated_bitmap_flag_0_symbol_15 },
    { "inertial_nullifier", generated_bitmap_flag_0_symbol_16 },
    { "inertial_stabilizer", generated_bitmap_flag_0_symbol_17 },
    { "lightning_field", generated_bitmap_flag_0_symbol_18 },
    { "multi_phased_shields", generated_bitmap_flag_0_symbol_19 },
    { "multi_wave_ecm_jammer", generated_bitmap_flag_0_symbol_20 },
    { "phase_shifter", generated_bitmap_flag_0_symbol_21 },
    { "phasing_cloak", generated_bitmap_flag_0_symbol_22 },
    { "quantum_detonator", generated_bitmap_flag_0_symbol_23 },
    { "rangemaster_unit", generated_bitmap_flag_0_symbol_24 },
    { "reflection_field", generated_bitmap_flag_0_symbol_25 },
    { "reinforced_hull", generated_bitmap_flag_0_symbol_26 },
    { "scout_lab", generated_bitmap_flag_0_symbol_27 },
    { "security_stations", generated_bitmap_flag_0_symbol_28 },
    { "shield_capacitor", generated_bitmap_flag_0_symbol_29 },
    { "stealth_field", generated_bitmap_flag_0_symbol_30 },
    { "structural_analyzer", generated_bitmap_flag_0_symbol_31 },
    { "sub_space_teleporter", generated_bitmap_flag_0_symbol_32 },
    { "time_warp_facilitator", generated_bitmap_flag_0_symbol_33 },
    { "transporters", generated_bitmap_flag_0_symbol_34 },
    { "troop_pods", generated_bitmap_flag_0_symbol_35 },
    { "warp_dissipator", generated_bitmap_flag_0_symbol_36 },
    { "wide_area_jammer", generated_bitmap_flag_0_symbol_37 },
    { "regeneration", generated_bitmap_flag_0_symbol_38 },
};

static const uint8_t generated_bitmap_known_bits_0[] = { 254, 255, 255, 255, 255 };

static const s_type_descriptor generated_type_0 = { CONFIG_VALUE_BOOL, false, false, 0, 0, CONFIG_SCALAR_PRIMITIVE, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_1 = { CONFIG_VALUE_INT8, true, true, 0, 100, CONFIG_SCALAR_PRIMITIVE, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_2 = { CONFIG_VALUE_UINT8, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_0, 78, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_3 = { CONFIG_VALUE_INT8, false, false, 0, 0, CONFIG_SCALAR_PRIMITIVE, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_4 = { CONFIG_VALUE_UINT8, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_1, 5, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_5 = { CONFIG_VALUE_UINT8, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_2, 7, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_6 = { CONFIG_VALUE_UINT8, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_3, 7, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_7 = { CONFIG_VALUE_UINT8, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_4, 6, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_8 = { CONFIG_VALUE_UINT8, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_5, 6, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_9 = { CONFIG_VALUE_UINT16, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_6, 46, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_10 = { CONFIG_VALUE_UINT8, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_7, 7, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_11 = { CONFIG_VALUE_INT16, false, false, 0, 0, CONFIG_SCALAR_MASK, generated_values_8, 1, generated_flags_0, 15, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 32767, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_12 = { CONFIG_VALUE_INT8, true, false, 0, 0, CONFIG_SCALAR_PRIMITIVE, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_13 = { CONFIG_VALUE_BITMAP, false, false, 0, 0, CONFIG_SCALAR_BITMAP, nullptr, 0, nullptr, 0, generated_bitmap_values_0, 1, generated_bitmap_flags_0, 39, generated_bitmap_known_bits_0, 40, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_14 = { CONFIG_VALUE_UINT8, true, true, 0, 10, CONFIG_SCALAR_PRIMITIVE, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_15 = { CONFIG_VALUE_UINT16, false, true, 0, 32767, CONFIG_SCALAR_PRIMITIVE, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_16 = { CONFIG_VALUE_UINT8, false, true, 0, 127, CONFIG_SCALAR_PRIMITIVE, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_17 = { CONFIG_VALUE_UINT16, false, false, 0, 0, CONFIG_SCALAR_PRIMITIVE, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_18 = { CONFIG_VALUE_INT16, false, false, 0, 0, CONFIG_SCALAR_PRIMITIVE, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_19 = { CONFIG_VALUE_UINT8, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_9, 3, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };

static const uint8_t generated_array_defaults_0[] = { 29, 3, 32, 53, 38, 67, 63, 43, 56, 65, 36, 46, 52, 74, 51, 62, 66, 61, 37, 50, 64, 35, 44, 70, 60, 68, 77, 75, 48, 73, 57, 58, 78, 71, 39, 72, 59, 55, 54, 34, 33, 42, 42, 42, 42, 79, 69, 0 };
static const uint8_t generated_array_defaults_1[] = { 2, 23, 0 };
static const uint8_t generated_array_defaults_2[] = { 2, 24, 0 };
static const uint8_t generated_array_defaults_3[] = { 2, 25, 0 };
static const uint8_t generated_array_defaults_4[] = { 2, 26, 0 };
static const uint8_t generated_array_defaults_5[] = { 2, 27, 0 };
static const uint8_t generated_array_defaults_6[] = { 2, 28, 0 };
static const uint8_t generated_array_defaults_7[] = { 2, 14, 0 };
static const uint8_t generated_array_defaults_8[] = { 2, 15, 0 };
static const uint8_t generated_array_defaults_9[] = { 12, 0 };
static const uint8_t generated_array_defaults_10[] = { 9, 0 };

const size_t variable_count = 3964;

static_assert(CONFIG_MAX_VARIABLE_TARGETS >= 2);

static const uint16_t generated_compact_defaults_0_0[] = {
    11,
    4,
    20,
    20,
    13,
    0,
    0,
    0,
};
static const uint8_t generated_compact_defaults_0_1[] = {
    15,
    15,
    15,
    15,
    15,
    0,
    0,
    0,
};
static const int16_t generated_compact_defaults_0_2[] = {
    2,
    4,
    17664,
    17664,
    0,
    0,
    0,
    0,
};
static const int8_t generated_compact_defaults_0_3[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
static const int8_t generated_compact_defaults_0_4[] = {
    2,
    10,
    1,
    1,
    2,
    0,
    0,
    0,
};
static const uint16_t generated_compact_defaults_1_0[] = {
    4,
    11,
    4,
    13,
    4,
    24,
    0,
    0,
};
static const uint8_t generated_compact_defaults_1_1[] = {
    1,
    1,
    1,
    15,
    1,
    1,
    0,
    0,
};
static const int16_t generated_compact_defaults_1_2[] = {
    2,
    2,
    0,
    0,
    4,
    0,
    0,
    0,
};
static const int8_t generated_compact_defaults_1_3[] = {
    0,
    0,
    0,
    0,
    0,
    10,
    0,
    0,
};
static const int8_t generated_compact_defaults_1_4[] = {
    4,
    2,
    10,
    2,
    10,
    15,
    0,
    0,
};
static const int8_t generated_compact_defaults_2_0[] = {
    20,
    8,
};
static const uint8_t generated_compact_defaults_2_1[] = {
    2,
    3,
};
static const uint8_t generated_compact_defaults_2_2[] = {
    0,
    2,
};
static const uint8_t generated_compact_defaults_2_3[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_2_4[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_2_5[] = {
    2,
    2,
};
static const uint16_t generated_compact_defaults_2_6[] = {
    45,
    45,
};
static const uint8_t generated_compact_defaults_2_7[] = {
    15,
    15,
};
static const int16_t generated_compact_defaults_2_8[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_2_9[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_2_10[] = {
    1,
    2,
};
static const uint16_t generated_compact_defaults_2_11[] = {
    0,
    23,
};
static const uint8_t generated_compact_defaults_2_12[] = {
    0,
    15,
};
static const int16_t generated_compact_defaults_2_13[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_2_14[] = {
    0,
    10,
};
static const int8_t generated_compact_defaults_2_15[] = {
    0,
    5,
};
static const uint16_t generated_compact_defaults_2_16[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_2_17[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_2_18[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_2_19[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_2_20[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_2_21[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_2_22[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_2_23[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_2_24[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_2_25[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_2_26[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_2_27[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_2_28[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_2_29[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_2_30[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_2_31[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_2_32[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_2_33[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_2_34[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_2_35[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_2_36[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_2_37[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_2_38[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_2_39[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_2_40[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_2_41[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_2_42[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_2_43[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_2_44[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_2_45[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_2_46[][5] = {
    { 0, 0, 0, 0, 128 },
    { 0, 0, 0, 0, 128 },
};
static const int8_t generated_compact_defaults_3_0[] = {
    21,
    9,
};
static const uint8_t generated_compact_defaults_3_1[] = {
    2,
    4,
};
static const uint8_t generated_compact_defaults_3_2[] = {
    0,
    4,
};
static const uint8_t generated_compact_defaults_3_3[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_3_4[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_3_5[] = {
    5,
    5,
};
static const uint16_t generated_compact_defaults_3_6[] = {
    42,
    42,
};
static const uint8_t generated_compact_defaults_3_7[] = {
    15,
    15,
};
static const int16_t generated_compact_defaults_3_8[] = {
    0,
    2,
};
static const int8_t generated_compact_defaults_3_9[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_3_10[] = {
    1,
    1,
};
static const uint16_t generated_compact_defaults_3_11[] = {
    25,
    26,
};
static const uint8_t generated_compact_defaults_3_12[] = {
    15,
    15,
};
static const int16_t generated_compact_defaults_3_13[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_3_14[] = {
    10,
    10,
};
static const int8_t generated_compact_defaults_3_15[] = {
    5,
    5,
};
static const uint16_t generated_compact_defaults_3_16[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_3_17[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_3_18[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_3_19[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_3_20[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_3_21[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_3_22[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_3_23[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_3_24[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_3_25[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_3_26[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_3_27[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_3_28[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_3_29[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_3_30[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_3_31[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_3_32[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_3_33[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_3_34[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_3_35[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_3_36[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_3_37[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_3_38[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_3_39[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_3_40[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_3_41[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_3_42[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_3_43[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_3_44[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_3_45[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_3_46[][5] = {
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
};
static const int8_t generated_compact_defaults_4_0[] = {
    22,
    10,
};
static const uint8_t generated_compact_defaults_4_1[] = {
    2,
    4,
};
static const uint8_t generated_compact_defaults_4_2[] = {
    0,
    6,
};
static const uint8_t generated_compact_defaults_4_3[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_4_4[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_4_5[] = {
    5,
    5,
};
static const uint16_t generated_compact_defaults_4_6[] = {
    41,
    41,
};
static const uint8_t generated_compact_defaults_4_7[] = {
    15,
    15,
};
static const int16_t generated_compact_defaults_4_8[] = {
    4,
    4,
};
static const int8_t generated_compact_defaults_4_9[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_4_10[] = {
    8,
    20,
};
static const uint16_t generated_compact_defaults_4_11[] = {
    40,
    40,
};
static const uint8_t generated_compact_defaults_4_12[] = {
    15,
    15,
};
static const int16_t generated_compact_defaults_4_13[] = {
    0,
    16384,
};
static const int8_t generated_compact_defaults_4_14[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_4_15[] = {
    1,
    1,
};
static const uint16_t generated_compact_defaults_4_16[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_4_17[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_4_18[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_4_19[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_4_20[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_4_21[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_4_22[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_4_23[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_4_24[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_4_25[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_4_26[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_4_27[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_4_28[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_4_29[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_4_30[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_4_31[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_4_32[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_4_33[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_4_34[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_4_35[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_4_36[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_4_37[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_4_38[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_4_39[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_4_40[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_4_41[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_4_42[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_4_43[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_4_44[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_4_45[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_4_46[][5] = {
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
};
static const int8_t generated_compact_defaults_5_0[] = {
    23,
    11,
};
static const uint8_t generated_compact_defaults_5_1[] = {
    2,
    3,
};
static const uint8_t generated_compact_defaults_5_2[] = {
    0,
    6,
};
static const uint8_t generated_compact_defaults_5_3[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_5_4[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_5_5[] = {
    4,
    4,
};
static const uint16_t generated_compact_defaults_5_6[] = {
    44,
    44,
};
static const uint8_t generated_compact_defaults_5_7[] = {
    15,
    15,
};
static const int16_t generated_compact_defaults_5_8[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_9[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_10[] = {
    1,
    2,
};
static const uint16_t generated_compact_defaults_5_11[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_5_12[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_5_13[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_14[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_15[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_5_16[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_5_17[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_5_18[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_19[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_20[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_5_21[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_5_22[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_5_23[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_24[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_25[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_5_26[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_5_27[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_5_28[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_29[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_30[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_5_31[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_5_32[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_5_33[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_34[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_35[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_5_36[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_5_37[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_5_38[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_39[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_40[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_5_41[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_5_42[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_5_43[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_44[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_5_45[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_5_46[][5] = {
    { 0, 0, 8, 0, 0 },
    { 0, 0, 8, 0, 0 },
};
static const int8_t generated_compact_defaults_6_0[] = {
    24,
    12,
};
static const uint8_t generated_compact_defaults_6_1[] = {
    2,
    4,
};
static const uint8_t generated_compact_defaults_6_2[] = {
    0,
    2,
};
static const uint8_t generated_compact_defaults_6_3[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_6_4[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_6_5[] = {
    2,
    2,
};
static const uint16_t generated_compact_defaults_6_6[] = {
    43,
    43,
};
static const uint8_t generated_compact_defaults_6_7[] = {
    15,
    15,
};
static const int16_t generated_compact_defaults_6_8[] = {
    0,
    2,
};
static const int8_t generated_compact_defaults_6_9[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_6_10[] = {
    3,
    5,
};
static const uint16_t generated_compact_defaults_6_11[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_6_12[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_6_13[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_6_14[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_6_15[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_6_16[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_6_17[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_6_18[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_6_19[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_6_20[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_6_21[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_6_22[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_6_23[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_6_24[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_6_25[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_6_26[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_6_27[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_6_28[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_6_29[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_6_30[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_6_31[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_6_32[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_6_33[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_6_34[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_6_35[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_6_36[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_6_37[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_6_38[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_6_39[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_6_40[] = {
    0,
    0,
};
static const uint16_t generated_compact_defaults_6_41[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_6_42[] = {
    0,
    0,
};
static const int16_t generated_compact_defaults_6_43[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_6_44[] = {
    0,
    0,
};
static const int8_t generated_compact_defaults_6_45[] = {
    0,
    0,
};
static const uint8_t generated_compact_defaults_6_46[][5] = {
    { 0, 4, 0, 0, 0 },
    { 0, 4, 0, 0, 0 },
};
static const int8_t generated_compact_defaults_7_0[] = {
    0,
    1,
    2,
    3,
    4,
    0,
    1,
    2,
    3,
    4,
    0,
    1,
    2,
    3,
    4,
    0,
    1,
    2,
    3,
    4,
    0,
    1,
    2,
    3,
    4,
};
static const uint8_t generated_compact_defaults_7_1[] = {
    0,
    1,
    2,
    3,
    4,
    0,
    1,
    2,
    3,
    4,
    0,
    1,
    2,
    3,
    4,
    0,
    1,
    2,
    3,
    4,
    0,
    1,
    2,
    3,
    4,
};
static const uint8_t generated_compact_defaults_7_2[] = {
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
};
static const uint8_t generated_compact_defaults_7_3[] = {
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
    6,
};
static const uint8_t generated_compact_defaults_7_4[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
static const uint8_t generated_compact_defaults_7_5[] = {
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
    5,
};
static const uint16_t generated_compact_defaults_7_6[] = {
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
    4,
};
static const uint8_t generated_compact_defaults_7_7[] = {
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
    1,
};
static const int16_t generated_compact_defaults_7_8[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
static const int8_t generated_compact_defaults_7_9[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
static const int8_t generated_compact_defaults_7_10[] = {
    2,
    3,
    4,
    10,
    20,
    2,
    3,
    4,
    10,
    20,
    2,
    3,
    4,
    10,
    20,
    2,
    3,
    4,
    10,
    20,
    2,
    3,
    4,
    10,
    20,
};
static const uint16_t generated_compact_defaults_7_11[] = {
    24,
    4,
    4,
    4,
    4,
    24,
    4,
    4,
    4,
    4,
    24,
    4,
    4,
    4,
    4,
    24,
    4,
    4,
    4,
    4,
    24,
    4,
    4,
    4,
    4,
};
static const uint8_t generated_compact_defaults_7_12[] = {
    15,
    1,
    1,
    1,
    1,
    15,
    1,
    1,
    1,
    1,
    15,
    1,
    1,
    1,
    1,
    15,
    1,
    1,
    1,
    1,
    15,
    1,
    1,
    1,
    1,
};
static const int16_t generated_compact_defaults_7_13[] = {
    0,
    2,
    2,
    2,
    2,
    0,
    2,
    2,
    2,
    2,
    0,
    2,
    2,
    2,
    2,
    0,
    2,
    2,
    2,
    2,
    0,
    2,
    2,
    2,
    2,
};
static const int8_t generated_compact_defaults_7_14[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
static const int8_t generated_compact_defaults_7_15[] = {
    1,
    1,
    2,
    4,
    5,
    1,
    1,
    2,
    4,
    5,
    1,
    1,
    2,
    4,
    5,
    1,
    1,
    2,
    4,
    5,
    1,
    1,
    2,
    4,
    5,
};
static const uint16_t generated_compact_defaults_7_16[] = {
    0,
    24,
    24,
    24,
    24,
    0,
    24,
    24,
    24,
    24,
    0,
    24,
    24,
    24,
    24,
    0,
    24,
    24,
    24,
    24,
    0,
    24,
    24,
    24,
    24,
};
static const uint8_t generated_compact_defaults_7_17[] = {
    0,
    15,
    15,
    1,
    1,
    0,
    15,
    15,
    1,
    1,
    0,
    15,
    15,
    1,
    1,
    0,
    15,
    15,
    1,
    1,
    0,
    15,
    15,
    1,
    1,
};
static const int16_t generated_compact_defaults_7_18[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
static const int8_t generated_compact_defaults_7_19[] = {
    0,
    10,
    10,
    10,
    10,
    0,
    10,
    10,
    10,
    10,
    0,
    10,
    10,
    10,
    10,
    0,
    10,
    10,
    10,
    10,
    0,
    10,
    10,
    10,
    10,
};
static const int8_t generated_compact_defaults_7_20[] = {
    0,
    1,
    5,
    15,
    30,
    0,
    1,
    5,
    15,
    30,
    0,
    1,
    5,
    15,
    30,
    0,
    1,
    5,
    15,
    30,
    0,
    1,
    5,
    15,
    30,
};
static const uint16_t generated_compact_defaults_7_21[] = {
    0,
    0,
    13,
    13,
    13,
    0,
    0,
    13,
    13,
    13,
    0,
    0,
    13,
    13,
    13,
    0,
    0,
    13,
    13,
    13,
    0,
    0,
    13,
    13,
    13,
};
static const uint8_t generated_compact_defaults_7_22[] = {
    0,
    0,
    15,
    15,
    15,
    0,
    0,
    15,
    15,
    15,
    0,
    0,
    15,
    15,
    15,
    0,
    0,
    15,
    15,
    15,
    0,
    0,
    15,
    15,
    15,
};
static const int16_t generated_compact_defaults_7_23[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
static const int8_t generated_compact_defaults_7_24[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
static const int8_t generated_compact_defaults_7_25[] = {
    0,
    0,
    2,
    2,
    3,
    0,
    0,
    2,
    2,
    3,
    0,
    0,
    2,
    2,
    3,
    0,
    0,
    2,
    2,
    3,
    0,
    0,
    2,
    2,
    3,
};
static const uint16_t generated_compact_defaults_7_26[] = {
    0,
    0,
    31,
    4,
    4,
    0,
    0,
    31,
    4,
    4,
    0,
    0,
    31,
    4,
    4,
    0,
    0,
    31,
    4,
    4,
    0,
    0,
    31,
    4,
    4,
};
static const uint8_t generated_compact_defaults_7_27[] = {
    0,
    0,
    15,
    1,
    1,
    0,
    0,
    15,
    1,
    1,
    0,
    0,
    15,
    1,
    1,
    0,
    0,
    15,
    1,
    1,
    0,
    0,
    15,
    1,
    1,
};
static const int16_t generated_compact_defaults_7_28[] = {
    0,
    0,
    0,
    4,
    4,
    0,
    0,
    0,
    4,
    4,
    0,
    0,
    0,
    4,
    4,
    0,
    0,
    0,
    4,
    4,
    0,
    0,
    0,
    4,
    4,
};
static const int8_t generated_compact_defaults_7_29[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
static const int8_t generated_compact_defaults_7_30[] = {
    0,
    0,
    3,
    10,
    20,
    0,
    0,
    3,
    10,
    20,
    0,
    0,
    3,
    10,
    20,
    0,
    0,
    3,
    10,
    20,
    0,
    0,
    3,
    10,
    20,
};
static const uint16_t generated_compact_defaults_7_31[] = {
    0,
    0,
    0,
    11,
    11,
    0,
    0,
    0,
    11,
    11,
    0,
    0,
    0,
    11,
    11,
    0,
    0,
    0,
    11,
    11,
    0,
    0,
    0,
    11,
    11,
};
static const uint8_t generated_compact_defaults_7_32[] = {
    0,
    0,
    0,
    1,
    1,
    0,
    0,
    0,
    1,
    1,
    0,
    0,
    0,
    1,
    1,
    0,
    0,
    0,
    1,
    1,
    0,
    0,
    0,
    1,
    1,
};
static const int16_t generated_compact_defaults_7_33[] = {
    0,
    0,
    0,
    0,
    2,
    0,
    0,
    0,
    0,
    2,
    0,
    0,
    0,
    0,
    2,
    0,
    0,
    0,
    0,
    2,
    0,
    0,
    0,
    0,
    2,
};
static const int8_t generated_compact_defaults_7_34[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
static const int8_t generated_compact_defaults_7_35[] = {
    0,
    0,
    0,
    4,
    5,
    0,
    0,
    0,
    4,
    5,
    0,
    0,
    0,
    4,
    5,
    0,
    0,
    0,
    4,
    5,
    0,
    0,
    0,
    4,
    5,
};
static const uint16_t generated_compact_defaults_7_36[] = {
    0,
    0,
    0,
    0,
    37,
    0,
    0,
    0,
    0,
    37,
    0,
    0,
    0,
    0,
    37,
    0,
    0,
    0,
    0,
    37,
    0,
    0,
    0,
    0,
    37,
};
static const uint8_t generated_compact_defaults_7_37[] = {
    0,
    0,
    0,
    0,
    15,
    0,
    0,
    0,
    0,
    15,
    0,
    0,
    0,
    0,
    15,
    0,
    0,
    0,
    0,
    15,
    0,
    0,
    0,
    0,
    15,
};
static const int16_t generated_compact_defaults_7_38[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
static const int8_t generated_compact_defaults_7_39[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
static const int8_t generated_compact_defaults_7_40[] = {
    0,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    0,
    1,
};
static const uint16_t generated_compact_defaults_7_41[] = {
    0,
    0,
    0,
    0,
    37,
    0,
    0,
    0,
    0,
    37,
    0,
    0,
    0,
    0,
    37,
    0,
    0,
    0,
    0,
    37,
    0,
    0,
    0,
    0,
    37,
};
static const uint8_t generated_compact_defaults_7_42[] = {
    0,
    0,
    0,
    0,
    15,
    0,
    0,
    0,
    0,
    15,
    0,
    0,
    0,
    0,
    15,
    0,
    0,
    0,
    0,
    15,
    0,
    0,
    0,
    0,
    15,
};
static const int16_t generated_compact_defaults_7_43[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
static const int8_t generated_compact_defaults_7_44[] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};
static const int8_t generated_compact_defaults_7_45[] = {
    0,
    0,
    0,
    0,
    12,
    0,
    0,
    0,
    0,
    12,
    0,
    0,
    0,
    0,
    12,
    0,
    0,
    0,
    0,
    12,
    0,
    0,
    0,
    0,
    12,
};
static const uint8_t generated_compact_defaults_7_46[][5] = {
    { 128, 0, 0, 1, 0 },
    { 128, 0, 0, 1, 0 },
    { 128, 0, 0, 1, 0 },
    { 128, 0, 0, 5, 2 },
    { 128, 2, 0, 5, 2 },
    { 128, 0, 0, 1, 0 },
    { 128, 0, 0, 1, 0 },
    { 128, 0, 0, 1, 0 },
    { 128, 2, 0, 5, 2 },
    { 128, 0, 32, 5, 2 },
    { 128, 0, 0, 1, 0 },
    { 128, 0, 0, 1, 0 },
    { 128, 2, 0, 1, 0 },
    { 128, 0, 32, 5, 2 },
    { 128, 0, 0, 5, 66 },
    { 128, 2, 0, 1, 0 },
    { 128, 2, 0, 1, 0 },
    { 128, 0, 32, 1, 0 },
    { 128, 0, 0, 5, 66 },
    { 128, 0, 0, 5, 66 },
    { 128, 2, 0, 1, 0 },
    { 128, 0, 32, 1, 0 },
    { 128, 0, 0, 1, 64 },
    { 128, 0, 0, 5, 66 },
    { 128, 0, 0, 5, 66 },
};

static_assert(sizeof(SHIP_CONFIG::guardian_template.special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::avenger_template.special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::amoeba_templates[0].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::amoeba_templates[1].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::crystal_templates[0].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::crystal_templates[1].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::dragon_templates[0].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::dragon_templates[1].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::eel_templates[0].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::eel_templates[1].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::hydra_templates[0].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::hydra_templates[1].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[0][0].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[0][1].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[0][2].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[0][3].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[0][4].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[1][0].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[1][1].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[1][2].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[1][3].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[1][4].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[2][0].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[2][1].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[2][2].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[2][3].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[2][4].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[3][0].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[3][1].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[3][2].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[3][3].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[3][4].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[4][0].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[4][1].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[4][2].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[4][3].special_device_flags) == 5);
static_assert(sizeof(SHIP_CONFIG::antaran_templates[4][4].special_device_flags) == 5);

static const s_array_index_alias_descriptor generated_array_index_aliases[] = {
    { 40, "system", 0 },
    { 40, "event", 1 },
    { 56, "system", 0 },
    { 56, "event", 1 },
    { 72, "system", 0 },
    { 72, "event", 1 },
    { 88, "system", 0 },
    { 88, "event", 1 },
    { 104, "system", 0 },
    { 104, "event", 1 },
    { 120, "tutor", 0 },
    { 120, "easy", 1 },
    { 120, "average", 2 },
    { 120, "hard", 3 },
    { 120, "impossible", 4 },
    { 121, "frigate", 0 },
    { 121, "destroyer", 1 },
    { 121, "cruiser", 2 },
    { 121, "battleship", 3 },
    { 121, "titan", 4 },
};

const s_array_index_alias_descriptor* const array_index_aliases = generated_array_index_aliases;

const size_t array_index_alias_count = 20;

const s_schema_node_descriptor schema_nodes[] = {
    { "music_enabled", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, 0, 0, 0, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_0, 0, 0, 0 },
    { "music_volume", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, 1, 1, 1, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_1, 0, 0, 0 },
    { "sound_effects_enabled", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, 2, 2, 2, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_0, 0, 0, 0 },
    { "sound_effects_volume", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, 3, 3, 3, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_1, 0, 0, 0 },
    { "enhanced_ux", CONFIG_NODE_RECORD, CONFIG_NO_NODE, 0, 4, 4, CONFIG_NO_VARIABLE, false, 0, 5, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "enabled", CONFIG_NODE_SCALAR, 4, 0, 4, 4, 4, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_0, 0, 0, 0 },
    { "build_queue", CONFIG_NODE_ARRAY, CONFIG_NO_NODE, 0, 5, 2204, CONFIG_NO_VARIABLE, false, 0, 7, CONFIG_NO_NODE, nullptr, 0, 11, 200 },
    { "", CONFIG_NODE_ARRAY, 6, 0, 5, 204, CONFIG_NO_VARIABLE, true, 0, 8, CONFIG_NO_NODE, nullptr, 0, 200, 1 },
    { "", CONFIG_NODE_SCALAR, 7, 0, 5, 5, 5, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_2, 0, 0, 0 },
    { "ships", CONFIG_NODE_NAMESPACE, CONFIG_NO_NODE, 0, 2205, 3943, CONFIG_NO_VARIABLE, false, 0, 10, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "guardian", CONFIG_NODE_RECORD, 9, 0, 2205, 2251, CONFIG_NO_VARIABLE, false, 0, 11, 25, nullptr, 0, 0, 0 },
    { "appearance", CONFIG_NODE_SCALAR, 10, 0, 2205, 2205, 2205, false, 0, CONFIG_NO_NODE, 12, &generated_type_3, 0, 0, 0 },
    { "hull_size", CONFIG_NODE_SCALAR, 10, 0, 2206, 2206, 2206, false, 0, CONFIG_NO_NODE, 13, &generated_type_4, 0, 0, 0 },
    { "drive", CONFIG_NODE_SCALAR, 10, 0, 2207, 2207, 2207, false, 0, CONFIG_NO_NODE, 14, &generated_type_5, 0, 0, 0 },
    { "armor", CONFIG_NODE_SCALAR, 10, 0, 2208, 2208, 2208, false, 0, CONFIG_NO_NODE, 15, &generated_type_6, 0, 0, 0 },
    { "shield", CONFIG_NODE_SCALAR, 10, 0, 2209, 2209, 2209, false, 0, CONFIG_NO_NODE, 16, &generated_type_7, 0, 0, 0 },
    { "computer", CONFIG_NODE_SCALAR, 10, 0, 2210, 2210, 2210, false, 0, CONFIG_NO_NODE, 17, &generated_type_8, 0, 0, 0 },
    { "weapons", CONFIG_NODE_ARRAY, 10, 0, 2211, 2250, CONFIG_NO_VARIABLE, false, 0, 18, 24, nullptr, 1, 8, 5 },
    { "", CONFIG_NODE_RECORD, 17, 1, 2211, 2215, CONFIG_NO_VARIABLE, false, 0, 19, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "type", CONFIG_NODE_SCALAR, 18, 0, 2211, 2211, 2211, false, 0, CONFIG_NO_NODE, 20, &generated_type_9, 0, 0, 0 },
    { "arc", CONFIG_NODE_SCALAR, 18, 0, 2212, 2212, 2212, false, 0, CONFIG_NO_NODE, 21, &generated_type_10, 0, 0, 0 },
    { "mods", CONFIG_NODE_SCALAR, 18, 0, 2213, 2213, 2213, false, 0, CONFIG_NO_NODE, 22, &generated_type_11, 0, 0, 0 },
    { "ammo", CONFIG_NODE_SCALAR, 18, 0, 2214, 2214, 2214, false, 0, CONFIG_NO_NODE, 23, &generated_type_3, 0, 0, 0 },
    { "amount", CONFIG_NODE_SCALAR, 18, 0, 2215, 2215, 2215, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_12, 0, 0, 0 },
    { "specials", CONFIG_NODE_SCALAR, 10, 0, 2251, 2251, 2251, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_13, 0, 0, 0 },
    { "avenger", CONFIG_NODE_RECORD, 9, 0, 2252, 2298, CONFIG_NO_VARIABLE, false, 0, 26, 40, nullptr, 0, 0, 0 },
    { "appearance", CONFIG_NODE_SCALAR, 25, 0, 2252, 2252, 2252, false, 0, CONFIG_NO_NODE, 27, &generated_type_3, 0, 0, 0 },
    { "hull_size", CONFIG_NODE_SCALAR, 25, 0, 2253, 2253, 2253, false, 0, CONFIG_NO_NODE, 28, &generated_type_4, 0, 0, 0 },
    { "drive", CONFIG_NODE_SCALAR, 25, 0, 2254, 2254, 2254, false, 0, CONFIG_NO_NODE, 29, &generated_type_5, 0, 0, 0 },
    { "armor", CONFIG_NODE_SCALAR, 25, 0, 2255, 2255, 2255, false, 0, CONFIG_NO_NODE, 30, &generated_type_6, 0, 0, 0 },
    { "shield", CONFIG_NODE_SCALAR, 25, 0, 2256, 2256, 2256, false, 0, CONFIG_NO_NODE, 31, &generated_type_7, 0, 0, 0 },
    { "computer", CONFIG_NODE_SCALAR, 25, 0, 2257, 2257, 2257, false, 0, CONFIG_NO_NODE, 32, &generated_type_8, 0, 0, 0 },
    { "weapons", CONFIG_NODE_ARRAY, 25, 0, 2258, 2297, CONFIG_NO_VARIABLE, false, 0, 33, 39, nullptr, 1, 8, 5 },
    { "", CONFIG_NODE_RECORD, 32, 1, 2258, 2262, CONFIG_NO_VARIABLE, false, 0, 34, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "type", CONFIG_NODE_SCALAR, 33, 0, 2258, 2258, 2258, false, 0, CONFIG_NO_NODE, 35, &generated_type_9, 0, 0, 0 },
    { "arc", CONFIG_NODE_SCALAR, 33, 0, 2259, 2259, 2259, false, 0, CONFIG_NO_NODE, 36, &generated_type_10, 0, 0, 0 },
    { "mods", CONFIG_NODE_SCALAR, 33, 0, 2260, 2260, 2260, false, 0, CONFIG_NO_NODE, 37, &generated_type_11, 0, 0, 0 },
    { "ammo", CONFIG_NODE_SCALAR, 33, 0, 2261, 2261, 2261, false, 0, CONFIG_NO_NODE, 38, &generated_type_3, 0, 0, 0 },
    { "amount", CONFIG_NODE_SCALAR, 33, 0, 2262, 2262, 2262, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_12, 0, 0, 0 },
    { "specials", CONFIG_NODE_SCALAR, 25, 0, 2298, 2298, 2298, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_13, 0, 0, 0 },
    { "amoeba", CONFIG_NODE_ARRAY, 9, 0, 2299, 2392, CONFIG_NO_VARIABLE, false, 0, 41, 56, nullptr, 0, 2, 47 },
    { "", CONFIG_NODE_RECORD, 40, 0, 2299, 2345, CONFIG_NO_VARIABLE, false, 0, 42, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "appearance", CONFIG_NODE_SCALAR, 41, 0, 2299, 2299, 2299, false, 0, CONFIG_NO_NODE, 43, &generated_type_3, 0, 0, 0 },
    { "hull_size", CONFIG_NODE_SCALAR, 41, 0, 2300, 2300, 2300, false, 0, CONFIG_NO_NODE, 44, &generated_type_4, 0, 0, 0 },
    { "drive", CONFIG_NODE_SCALAR, 41, 0, 2301, 2301, 2301, false, 0, CONFIG_NO_NODE, 45, &generated_type_5, 0, 0, 0 },
    { "armor", CONFIG_NODE_SCALAR, 41, 0, 2302, 2302, 2302, false, 0, CONFIG_NO_NODE, 46, &generated_type_6, 0, 0, 0 },
    { "shield", CONFIG_NODE_SCALAR, 41, 0, 2303, 2303, 2303, false, 0, CONFIG_NO_NODE, 47, &generated_type_7, 0, 0, 0 },
    { "computer", CONFIG_NODE_SCALAR, 41, 0, 2304, 2304, 2304, false, 0, CONFIG_NO_NODE, 48, &generated_type_8, 0, 0, 0 },
    { "weapons", CONFIG_NODE_ARRAY, 41, 0, 2305, 2344, CONFIG_NO_VARIABLE, false, 0, 49, 55, nullptr, 1, 8, 5 },
    { "", CONFIG_NODE_RECORD, 48, 1, 2305, 2309, CONFIG_NO_VARIABLE, false, 0, 50, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "type", CONFIG_NODE_SCALAR, 49, 0, 2305, 2305, 2305, false, 0, CONFIG_NO_NODE, 51, &generated_type_9, 0, 0, 0 },
    { "arc", CONFIG_NODE_SCALAR, 49, 0, 2306, 2306, 2306, false, 0, CONFIG_NO_NODE, 52, &generated_type_10, 0, 0, 0 },
    { "mods", CONFIG_NODE_SCALAR, 49, 0, 2307, 2307, 2307, false, 0, CONFIG_NO_NODE, 53, &generated_type_11, 0, 0, 0 },
    { "ammo", CONFIG_NODE_SCALAR, 49, 0, 2308, 2308, 2308, false, 0, CONFIG_NO_NODE, 54, &generated_type_3, 0, 0, 0 },
    { "amount", CONFIG_NODE_SCALAR, 49, 0, 2309, 2309, 2309, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_12, 0, 0, 0 },
    { "specials", CONFIG_NODE_SCALAR, 41, 0, 2345, 2345, 2345, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_13, 0, 0, 0 },
    { "crystal", CONFIG_NODE_ARRAY, 9, 0, 2393, 2486, CONFIG_NO_VARIABLE, false, 0, 57, 72, nullptr, 0, 2, 47 },
    { "", CONFIG_NODE_RECORD, 56, 0, 2393, 2439, CONFIG_NO_VARIABLE, false, 0, 58, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "appearance", CONFIG_NODE_SCALAR, 57, 0, 2393, 2393, 2393, false, 0, CONFIG_NO_NODE, 59, &generated_type_3, 0, 0, 0 },
    { "hull_size", CONFIG_NODE_SCALAR, 57, 0, 2394, 2394, 2394, false, 0, CONFIG_NO_NODE, 60, &generated_type_4, 0, 0, 0 },
    { "drive", CONFIG_NODE_SCALAR, 57, 0, 2395, 2395, 2395, false, 0, CONFIG_NO_NODE, 61, &generated_type_5, 0, 0, 0 },
    { "armor", CONFIG_NODE_SCALAR, 57, 0, 2396, 2396, 2396, false, 0, CONFIG_NO_NODE, 62, &generated_type_6, 0, 0, 0 },
    { "shield", CONFIG_NODE_SCALAR, 57, 0, 2397, 2397, 2397, false, 0, CONFIG_NO_NODE, 63, &generated_type_7, 0, 0, 0 },
    { "computer", CONFIG_NODE_SCALAR, 57, 0, 2398, 2398, 2398, false, 0, CONFIG_NO_NODE, 64, &generated_type_8, 0, 0, 0 },
    { "weapons", CONFIG_NODE_ARRAY, 57, 0, 2399, 2438, CONFIG_NO_VARIABLE, false, 0, 65, 71, nullptr, 1, 8, 5 },
    { "", CONFIG_NODE_RECORD, 64, 1, 2399, 2403, CONFIG_NO_VARIABLE, false, 0, 66, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "type", CONFIG_NODE_SCALAR, 65, 0, 2399, 2399, 2399, false, 0, CONFIG_NO_NODE, 67, &generated_type_9, 0, 0, 0 },
    { "arc", CONFIG_NODE_SCALAR, 65, 0, 2400, 2400, 2400, false, 0, CONFIG_NO_NODE, 68, &generated_type_10, 0, 0, 0 },
    { "mods", CONFIG_NODE_SCALAR, 65, 0, 2401, 2401, 2401, false, 0, CONFIG_NO_NODE, 69, &generated_type_11, 0, 0, 0 },
    { "ammo", CONFIG_NODE_SCALAR, 65, 0, 2402, 2402, 2402, false, 0, CONFIG_NO_NODE, 70, &generated_type_3, 0, 0, 0 },
    { "amount", CONFIG_NODE_SCALAR, 65, 0, 2403, 2403, 2403, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_12, 0, 0, 0 },
    { "specials", CONFIG_NODE_SCALAR, 57, 0, 2439, 2439, 2439, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_13, 0, 0, 0 },
    { "dragon", CONFIG_NODE_ARRAY, 9, 0, 2487, 2580, CONFIG_NO_VARIABLE, false, 0, 73, 88, nullptr, 0, 2, 47 },
    { "", CONFIG_NODE_RECORD, 72, 0, 2487, 2533, CONFIG_NO_VARIABLE, false, 0, 74, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "appearance", CONFIG_NODE_SCALAR, 73, 0, 2487, 2487, 2487, false, 0, CONFIG_NO_NODE, 75, &generated_type_3, 0, 0, 0 },
    { "hull_size", CONFIG_NODE_SCALAR, 73, 0, 2488, 2488, 2488, false, 0, CONFIG_NO_NODE, 76, &generated_type_4, 0, 0, 0 },
    { "drive", CONFIG_NODE_SCALAR, 73, 0, 2489, 2489, 2489, false, 0, CONFIG_NO_NODE, 77, &generated_type_5, 0, 0, 0 },
    { "armor", CONFIG_NODE_SCALAR, 73, 0, 2490, 2490, 2490, false, 0, CONFIG_NO_NODE, 78, &generated_type_6, 0, 0, 0 },
    { "shield", CONFIG_NODE_SCALAR, 73, 0, 2491, 2491, 2491, false, 0, CONFIG_NO_NODE, 79, &generated_type_7, 0, 0, 0 },
    { "computer", CONFIG_NODE_SCALAR, 73, 0, 2492, 2492, 2492, false, 0, CONFIG_NO_NODE, 80, &generated_type_8, 0, 0, 0 },
    { "weapons", CONFIG_NODE_ARRAY, 73, 0, 2493, 2532, CONFIG_NO_VARIABLE, false, 0, 81, 87, nullptr, 1, 8, 5 },
    { "", CONFIG_NODE_RECORD, 80, 1, 2493, 2497, CONFIG_NO_VARIABLE, false, 0, 82, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "type", CONFIG_NODE_SCALAR, 81, 0, 2493, 2493, 2493, false, 0, CONFIG_NO_NODE, 83, &generated_type_9, 0, 0, 0 },
    { "arc", CONFIG_NODE_SCALAR, 81, 0, 2494, 2494, 2494, false, 0, CONFIG_NO_NODE, 84, &generated_type_10, 0, 0, 0 },
    { "mods", CONFIG_NODE_SCALAR, 81, 0, 2495, 2495, 2495, false, 0, CONFIG_NO_NODE, 85, &generated_type_11, 0, 0, 0 },
    { "ammo", CONFIG_NODE_SCALAR, 81, 0, 2496, 2496, 2496, false, 0, CONFIG_NO_NODE, 86, &generated_type_3, 0, 0, 0 },
    { "amount", CONFIG_NODE_SCALAR, 81, 0, 2497, 2497, 2497, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_12, 0, 0, 0 },
    { "specials", CONFIG_NODE_SCALAR, 73, 0, 2533, 2533, 2533, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_13, 0, 0, 0 },
    { "eel", CONFIG_NODE_ARRAY, 9, 0, 2581, 2674, CONFIG_NO_VARIABLE, false, 0, 89, 104, nullptr, 0, 2, 47 },
    { "", CONFIG_NODE_RECORD, 88, 0, 2581, 2627, CONFIG_NO_VARIABLE, false, 0, 90, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "appearance", CONFIG_NODE_SCALAR, 89, 0, 2581, 2581, 2581, false, 0, CONFIG_NO_NODE, 91, &generated_type_3, 0, 0, 0 },
    { "hull_size", CONFIG_NODE_SCALAR, 89, 0, 2582, 2582, 2582, false, 0, CONFIG_NO_NODE, 92, &generated_type_4, 0, 0, 0 },
    { "drive", CONFIG_NODE_SCALAR, 89, 0, 2583, 2583, 2583, false, 0, CONFIG_NO_NODE, 93, &generated_type_5, 0, 0, 0 },
    { "armor", CONFIG_NODE_SCALAR, 89, 0, 2584, 2584, 2584, false, 0, CONFIG_NO_NODE, 94, &generated_type_6, 0, 0, 0 },
    { "shield", CONFIG_NODE_SCALAR, 89, 0, 2585, 2585, 2585, false, 0, CONFIG_NO_NODE, 95, &generated_type_7, 0, 0, 0 },
    { "computer", CONFIG_NODE_SCALAR, 89, 0, 2586, 2586, 2586, false, 0, CONFIG_NO_NODE, 96, &generated_type_8, 0, 0, 0 },
    { "weapons", CONFIG_NODE_ARRAY, 89, 0, 2587, 2626, CONFIG_NO_VARIABLE, false, 0, 97, 103, nullptr, 1, 8, 5 },
    { "", CONFIG_NODE_RECORD, 96, 1, 2587, 2591, CONFIG_NO_VARIABLE, false, 0, 98, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "type", CONFIG_NODE_SCALAR, 97, 0, 2587, 2587, 2587, false, 0, CONFIG_NO_NODE, 99, &generated_type_9, 0, 0, 0 },
    { "arc", CONFIG_NODE_SCALAR, 97, 0, 2588, 2588, 2588, false, 0, CONFIG_NO_NODE, 100, &generated_type_10, 0, 0, 0 },
    { "mods", CONFIG_NODE_SCALAR, 97, 0, 2589, 2589, 2589, false, 0, CONFIG_NO_NODE, 101, &generated_type_11, 0, 0, 0 },
    { "ammo", CONFIG_NODE_SCALAR, 97, 0, 2590, 2590, 2590, false, 0, CONFIG_NO_NODE, 102, &generated_type_3, 0, 0, 0 },
    { "amount", CONFIG_NODE_SCALAR, 97, 0, 2591, 2591, 2591, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_12, 0, 0, 0 },
    { "specials", CONFIG_NODE_SCALAR, 89, 0, 2627, 2627, 2627, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_13, 0, 0, 0 },
    { "hydra", CONFIG_NODE_ARRAY, 9, 0, 2675, 2768, CONFIG_NO_VARIABLE, false, 0, 105, 120, nullptr, 0, 2, 47 },
    { "", CONFIG_NODE_RECORD, 104, 0, 2675, 2721, CONFIG_NO_VARIABLE, false, 0, 106, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "appearance", CONFIG_NODE_SCALAR, 105, 0, 2675, 2675, 2675, false, 0, CONFIG_NO_NODE, 107, &generated_type_3, 0, 0, 0 },
    { "hull_size", CONFIG_NODE_SCALAR, 105, 0, 2676, 2676, 2676, false, 0, CONFIG_NO_NODE, 108, &generated_type_4, 0, 0, 0 },
    { "drive", CONFIG_NODE_SCALAR, 105, 0, 2677, 2677, 2677, false, 0, CONFIG_NO_NODE, 109, &generated_type_5, 0, 0, 0 },
    { "armor", CONFIG_NODE_SCALAR, 105, 0, 2678, 2678, 2678, false, 0, CONFIG_NO_NODE, 110, &generated_type_6, 0, 0, 0 },
    { "shield", CONFIG_NODE_SCALAR, 105, 0, 2679, 2679, 2679, false, 0, CONFIG_NO_NODE, 111, &generated_type_7, 0, 0, 0 },
    { "computer", CONFIG_NODE_SCALAR, 105, 0, 2680, 2680, 2680, false, 0, CONFIG_NO_NODE, 112, &generated_type_8, 0, 0, 0 },
    { "weapons", CONFIG_NODE_ARRAY, 105, 0, 2681, 2720, CONFIG_NO_VARIABLE, false, 0, 113, 119, nullptr, 1, 8, 5 },
    { "", CONFIG_NODE_RECORD, 112, 1, 2681, 2685, CONFIG_NO_VARIABLE, false, 0, 114, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "type", CONFIG_NODE_SCALAR, 113, 0, 2681, 2681, 2681, false, 0, CONFIG_NO_NODE, 115, &generated_type_9, 0, 0, 0 },
    { "arc", CONFIG_NODE_SCALAR, 113, 0, 2682, 2682, 2682, false, 0, CONFIG_NO_NODE, 116, &generated_type_10, 0, 0, 0 },
    { "mods", CONFIG_NODE_SCALAR, 113, 0, 2683, 2683, 2683, false, 0, CONFIG_NO_NODE, 117, &generated_type_11, 0, 0, 0 },
    { "ammo", CONFIG_NODE_SCALAR, 113, 0, 2684, 2684, 2684, false, 0, CONFIG_NO_NODE, 118, &generated_type_3, 0, 0, 0 },
    { "amount", CONFIG_NODE_SCALAR, 113, 0, 2685, 2685, 2685, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_12, 0, 0, 0 },
    { "specials", CONFIG_NODE_SCALAR, 105, 0, 2721, 2721, 2721, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_13, 0, 0, 0 },
    { "antaran", CONFIG_NODE_ARRAY, 9, 0, 2769, 3943, CONFIG_NO_VARIABLE, false, 0, 121, CONFIG_NO_NODE, nullptr, 0, 5, 235 },
    { "", CONFIG_NODE_ARRAY, 120, 0, 2769, 3003, CONFIG_NO_VARIABLE, false, 0, 122, CONFIG_NO_NODE, nullptr, 0, 5, 47 },
    { "", CONFIG_NODE_RECORD, 121, 0, 2769, 2815, CONFIG_NO_VARIABLE, false, 0, 123, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "appearance", CONFIG_NODE_SCALAR, 122, 0, 2769, 2769, 2769, false, 0, CONFIG_NO_NODE, 124, &generated_type_3, 0, 0, 0 },
    { "hull_size", CONFIG_NODE_SCALAR, 122, 0, 2770, 2770, 2770, false, 0, CONFIG_NO_NODE, 125, &generated_type_4, 0, 0, 0 },
    { "drive", CONFIG_NODE_SCALAR, 122, 0, 2771, 2771, 2771, false, 0, CONFIG_NO_NODE, 126, &generated_type_5, 0, 0, 0 },
    { "armor", CONFIG_NODE_SCALAR, 122, 0, 2772, 2772, 2772, false, 0, CONFIG_NO_NODE, 127, &generated_type_6, 0, 0, 0 },
    { "shield", CONFIG_NODE_SCALAR, 122, 0, 2773, 2773, 2773, false, 0, CONFIG_NO_NODE, 128, &generated_type_7, 0, 0, 0 },
    { "computer", CONFIG_NODE_SCALAR, 122, 0, 2774, 2774, 2774, false, 0, CONFIG_NO_NODE, 129, &generated_type_8, 0, 0, 0 },
    { "weapons", CONFIG_NODE_ARRAY, 122, 0, 2775, 2814, CONFIG_NO_VARIABLE, false, 0, 130, 136, nullptr, 1, 8, 5 },
    { "", CONFIG_NODE_RECORD, 129, 1, 2775, 2779, CONFIG_NO_VARIABLE, false, 0, 131, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "type", CONFIG_NODE_SCALAR, 130, 0, 2775, 2775, 2775, false, 0, CONFIG_NO_NODE, 132, &generated_type_9, 0, 0, 0 },
    { "arc", CONFIG_NODE_SCALAR, 130, 0, 2776, 2776, 2776, false, 0, CONFIG_NO_NODE, 133, &generated_type_10, 0, 0, 0 },
    { "mods", CONFIG_NODE_SCALAR, 130, 0, 2777, 2777, 2777, false, 0, CONFIG_NO_NODE, 134, &generated_type_11, 0, 0, 0 },
    { "ammo", CONFIG_NODE_SCALAR, 130, 0, 2778, 2778, 2778, false, 0, CONFIG_NO_NODE, 135, &generated_type_3, 0, 0, 0 },
    { "amount", CONFIG_NODE_SCALAR, 130, 0, 2779, 2779, 2779, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_12, 0, 0, 0 },
    { "specials", CONFIG_NODE_SCALAR, 122, 0, 2815, 2815, 2815, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_13, 0, 0, 0 },
    { "experiment", CONFIG_NODE_NAMESPACE, CONFIG_NO_NODE, 0, 3944, 3945, CONFIG_NO_VARIABLE, false, 0, 138, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "autobuild", CONFIG_NODE_RECORD, 137, 0, 3944, 3945, CONFIG_NO_VARIABLE, false, 0, 139, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "enabled", CONFIG_NODE_SCALAR, 138, 0, 3944, 3944, 3944, false, 0, CONFIG_NO_NODE, 140, &generated_type_0, 0, 0, 0 },
    { "default", CONFIG_NODE_SCALAR, 138, 0, 3945, 3945, 3945, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_14, 0, 0, 0 },
    { "detection", CONFIG_NODE_NAMESPACE, CONFIG_NO_NODE, 0, 3946, 3961, CONFIG_NO_VARIABLE, false, 0, 142, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "scan_bonus", CONFIG_NODE_RECORD, 141, 0, 3946, 3956, CONFIG_NO_VARIABLE, false, 0, 143, 154, nullptr, 0, 0, 0 },
    { "battle_scanner", CONFIG_NODE_SCALAR, 142, 0, 3946, 3946, 3946, false, 0, CONFIG_NO_NODE, 144, &generated_type_15, 0, 0, 0 },
    { "scout_lab", CONFIG_NODE_SCALAR, 142, 0, 3947, 3947, 3947, false, 0, CONFIG_NO_NODE, 145, &generated_type_15, 0, 0, 0 },
    { "star_base", CONFIG_NODE_SCALAR, 142, 0, 3948, 3948, 3948, false, 0, CONFIG_NO_NODE, 146, &generated_type_15, 0, 0, 0 },
    { "battlestation", CONFIG_NODE_SCALAR, 142, 0, 3949, 3949, 3949, false, 0, CONFIG_NO_NODE, 147, &generated_type_16, 0, 0, 0 },
    { "star_fortress", CONFIG_NODE_SCALAR, 142, 0, 3950, 3950, 3950, false, 0, CONFIG_NO_NODE, 148, &generated_type_16, 0, 0, 0 },
    { "no_scanner", CONFIG_NODE_SCALAR, 142, 0, 3951, 3951, 3951, false, 0, CONFIG_NO_NODE, 149, &generated_type_16, 0, 0, 0 },
    { "tachyon_scanner", CONFIG_NODE_SCALAR, 142, 0, 3952, 3952, 3952, false, 0, CONFIG_NO_NODE, 150, &generated_type_16, 0, 0, 0 },
    { "neutron_scanner", CONFIG_NODE_SCALAR, 142, 0, 3953, 3953, 3953, false, 0, CONFIG_NO_NODE, 151, &generated_type_16, 0, 0, 0 },
    { "sensors", CONFIG_NODE_SCALAR, 142, 0, 3954, 3954, 3954, false, 0, CONFIG_NO_NODE, 152, &generated_type_16, 0, 0, 0 },
    { "planet", CONFIG_NODE_SCALAR, 142, 0, 3955, 3955, 3955, false, 0, CONFIG_NO_NODE, 153, &generated_type_17, 0, 0, 0 },
    { "fleet", CONFIG_NODE_SCALAR, 142, 0, 3956, 3956, 3956, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_17, 0, 0, 0 },
    { "stealth_bonus", CONFIG_NODE_RECORD, 141, 0, 3957, 3960, CONFIG_NO_VARIABLE, false, 0, 155, 159, nullptr, 0, 0, 0 },
    { "stealthy_ships", CONFIG_NODE_SCALAR, 154, 0, 3957, 3957, 3957, false, 0, CONFIG_NO_NODE, 156, &generated_type_18, 0, 0, 0 },
    { "stealth_field", CONFIG_NODE_SCALAR, 154, 0, 3958, 3958, 3958, false, 0, CONFIG_NO_NODE, 157, &generated_type_18, 0, 0, 0 },
    { "cloaking_device", CONFIG_NODE_SCALAR, 154, 0, 3959, 3959, 3959, false, 0, CONFIG_NO_NODE, 158, &generated_type_18, 0, 0, 0 },
    { "phasing_cloak", CONFIG_NODE_SCALAR, 154, 0, 3960, 3960, 3960, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_18, 0, 0, 0 },
    { "stealth_size_penalty", CONFIG_NODE_SCALAR, 141, 0, 3961, 3961, 3961, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_19, 0, 0, 0 },
    { "combat", CONFIG_NODE_NAMESPACE, CONFIG_NO_NODE, 0, 3962, 3963, CONFIG_NO_VARIABLE, false, 0, 161, CONFIG_NO_NODE, nullptr, 0, 0, 0 },
    { "board_in_black_hole", CONFIG_NODE_SCALAR, 160, 0, 3962, 3962, 3962, false, 0, CONFIG_NO_NODE, 162, &generated_type_0, 0, 0, 0 },
    { "fighters_use_point_defense", CONFIG_NODE_SCALAR, 160, 0, 3963, 3963, 3963, false, 0, CONFIG_NO_NODE, CONFIG_NO_NODE, &generated_type_0, 0, 0, 0 },
};

const size_t schema_node_count = sizeof(schema_nodes) / sizeof(schema_nodes[0]);

namespace generated {

bool Get_Default_Value(t_variable_id variable_id, s_typed_value* value) {
    if (value == nullptr) return false;
    if (variable_id >= 5 && variable_id < 205) {
        const size_t offset = variable_id - 5;
        if (offset < 48) {
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = generated_array_defaults_0[offset];
        return true;
        }
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 0;
        return true;
    }
    if (variable_id >= 205 && variable_id < 405) {
        const size_t offset = variable_id - 205;
        if (offset < 3) {
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = generated_array_defaults_1[offset];
        return true;
        }
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 0;
        return true;
    }
    if (variable_id >= 405 && variable_id < 605) {
        const size_t offset = variable_id - 405;
        if (offset < 3) {
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = generated_array_defaults_2[offset];
        return true;
        }
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 0;
        return true;
    }
    if (variable_id >= 605 && variable_id < 805) {
        const size_t offset = variable_id - 605;
        if (offset < 3) {
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = generated_array_defaults_3[offset];
        return true;
        }
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 0;
        return true;
    }
    if (variable_id >= 805 && variable_id < 1005) {
        const size_t offset = variable_id - 805;
        if (offset < 3) {
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = generated_array_defaults_4[offset];
        return true;
        }
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 0;
        return true;
    }
    if (variable_id >= 1005 && variable_id < 1205) {
        const size_t offset = variable_id - 1005;
        if (offset < 3) {
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = generated_array_defaults_5[offset];
        return true;
        }
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 0;
        return true;
    }
    if (variable_id >= 1205 && variable_id < 1405) {
        const size_t offset = variable_id - 1205;
        if (offset < 3) {
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = generated_array_defaults_6[offset];
        return true;
        }
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 0;
        return true;
    }
    if (variable_id >= 1405 && variable_id < 1605) {
        const size_t offset = variable_id - 1405;
        if (offset < 3) {
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = generated_array_defaults_7[offset];
        return true;
        }
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 0;
        return true;
    }
    if (variable_id >= 1605 && variable_id < 1805) {
        const size_t offset = variable_id - 1605;
        if (offset < 3) {
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = generated_array_defaults_8[offset];
        return true;
        }
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 0;
        return true;
    }
    if (variable_id >= 1805 && variable_id < 2005) {
        const size_t offset = variable_id - 1805;
        if (offset < 2) {
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = generated_array_defaults_9[offset];
        return true;
        }
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 0;
        return true;
    }
    if (variable_id >= 2005 && variable_id < 2205) {
        const size_t offset = variable_id - 2005;
        if (offset < 2) {
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = generated_array_defaults_10[offset];
        return true;
        }
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 0;
        return true;
    }
    if (variable_id >= 2211 && variable_id <= 2250) {
        const size_t instance = (variable_id - 2211) / 5;
        const size_t leaf = (variable_id - 2211) % 5;
        *value = {};
        switch (leaf) {
        case 0:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_0_0[instance];
            return true;
        case 1:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_0_1[instance];
            return true;
        case 2:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_0_2[instance];
            return true;
        case 3:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_0_3[instance];
            return true;
        case 4:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_0_4[instance];
            return true;
        default: return false;
        }
    }
    if (variable_id >= 2258 && variable_id <= 2297) {
        const size_t instance = (variable_id - 2258) / 5;
        const size_t leaf = (variable_id - 2258) % 5;
        *value = {};
        switch (leaf) {
        case 0:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_1_0[instance];
            return true;
        case 1:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_1_1[instance];
            return true;
        case 2:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_1_2[instance];
            return true;
        case 3:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_1_3[instance];
            return true;
        case 4:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_1_4[instance];
            return true;
        default: return false;
        }
    }
    if (variable_id >= 2299 && variable_id <= 2392) {
        const size_t instance = (variable_id - 2299) / 47;
        const size_t leaf = (variable_id - 2299) % 47;
        *value = {};
        switch (leaf) {
        case 0:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_0[instance];
            return true;
        case 1:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_2_1[instance];
            return true;
        case 2:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_2_2[instance];
            return true;
        case 3:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_2_3[instance];
            return true;
        case 4:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_2_4[instance];
            return true;
        case 5:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_2_5[instance];
            return true;
        case 6:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_2_6[instance];
            return true;
        case 7:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_2_7[instance];
            return true;
        case 8:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_2_8[instance];
            return true;
        case 9:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_9[instance];
            return true;
        case 10:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_10[instance];
            return true;
        case 11:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_2_11[instance];
            return true;
        case 12:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_2_12[instance];
            return true;
        case 13:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_2_13[instance];
            return true;
        case 14:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_14[instance];
            return true;
        case 15:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_15[instance];
            return true;
        case 16:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_2_16[instance];
            return true;
        case 17:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_2_17[instance];
            return true;
        case 18:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_2_18[instance];
            return true;
        case 19:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_19[instance];
            return true;
        case 20:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_20[instance];
            return true;
        case 21:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_2_21[instance];
            return true;
        case 22:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_2_22[instance];
            return true;
        case 23:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_2_23[instance];
            return true;
        case 24:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_24[instance];
            return true;
        case 25:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_25[instance];
            return true;
        case 26:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_2_26[instance];
            return true;
        case 27:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_2_27[instance];
            return true;
        case 28:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_2_28[instance];
            return true;
        case 29:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_29[instance];
            return true;
        case 30:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_30[instance];
            return true;
        case 31:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_2_31[instance];
            return true;
        case 32:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_2_32[instance];
            return true;
        case 33:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_2_33[instance];
            return true;
        case 34:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_34[instance];
            return true;
        case 35:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_35[instance];
            return true;
        case 36:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_2_36[instance];
            return true;
        case 37:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_2_37[instance];
            return true;
        case 38:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_2_38[instance];
            return true;
        case 39:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_39[instance];
            return true;
        case 40:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_40[instance];
            return true;
        case 41:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_2_41[instance];
            return true;
        case 42:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_2_42[instance];
            return true;
        case 43:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_2_43[instance];
            return true;
        case 44:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_44[instance];
            return true;
        case 45:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_2_45[instance];
            return true;
        case 46:
            value->type = CONFIG_VALUE_BITMAP;
            value->bitmap_value.assign(generated_compact_defaults_2_46[instance], generated_compact_defaults_2_46[instance] + sizeof(generated_compact_defaults_2_46[instance]));
            return true;
        default: return false;
        }
    }
    if (variable_id >= 2393 && variable_id <= 2486) {
        const size_t instance = (variable_id - 2393) / 47;
        const size_t leaf = (variable_id - 2393) % 47;
        *value = {};
        switch (leaf) {
        case 0:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_0[instance];
            return true;
        case 1:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_3_1[instance];
            return true;
        case 2:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_3_2[instance];
            return true;
        case 3:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_3_3[instance];
            return true;
        case 4:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_3_4[instance];
            return true;
        case 5:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_3_5[instance];
            return true;
        case 6:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_3_6[instance];
            return true;
        case 7:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_3_7[instance];
            return true;
        case 8:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_3_8[instance];
            return true;
        case 9:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_9[instance];
            return true;
        case 10:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_10[instance];
            return true;
        case 11:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_3_11[instance];
            return true;
        case 12:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_3_12[instance];
            return true;
        case 13:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_3_13[instance];
            return true;
        case 14:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_14[instance];
            return true;
        case 15:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_15[instance];
            return true;
        case 16:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_3_16[instance];
            return true;
        case 17:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_3_17[instance];
            return true;
        case 18:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_3_18[instance];
            return true;
        case 19:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_19[instance];
            return true;
        case 20:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_20[instance];
            return true;
        case 21:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_3_21[instance];
            return true;
        case 22:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_3_22[instance];
            return true;
        case 23:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_3_23[instance];
            return true;
        case 24:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_24[instance];
            return true;
        case 25:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_25[instance];
            return true;
        case 26:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_3_26[instance];
            return true;
        case 27:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_3_27[instance];
            return true;
        case 28:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_3_28[instance];
            return true;
        case 29:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_29[instance];
            return true;
        case 30:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_30[instance];
            return true;
        case 31:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_3_31[instance];
            return true;
        case 32:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_3_32[instance];
            return true;
        case 33:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_3_33[instance];
            return true;
        case 34:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_34[instance];
            return true;
        case 35:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_35[instance];
            return true;
        case 36:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_3_36[instance];
            return true;
        case 37:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_3_37[instance];
            return true;
        case 38:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_3_38[instance];
            return true;
        case 39:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_39[instance];
            return true;
        case 40:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_40[instance];
            return true;
        case 41:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_3_41[instance];
            return true;
        case 42:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_3_42[instance];
            return true;
        case 43:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_3_43[instance];
            return true;
        case 44:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_44[instance];
            return true;
        case 45:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_3_45[instance];
            return true;
        case 46:
            value->type = CONFIG_VALUE_BITMAP;
            value->bitmap_value.assign(generated_compact_defaults_3_46[instance], generated_compact_defaults_3_46[instance] + sizeof(generated_compact_defaults_3_46[instance]));
            return true;
        default: return false;
        }
    }
    if (variable_id >= 2487 && variable_id <= 2580) {
        const size_t instance = (variable_id - 2487) / 47;
        const size_t leaf = (variable_id - 2487) % 47;
        *value = {};
        switch (leaf) {
        case 0:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_0[instance];
            return true;
        case 1:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_4_1[instance];
            return true;
        case 2:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_4_2[instance];
            return true;
        case 3:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_4_3[instance];
            return true;
        case 4:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_4_4[instance];
            return true;
        case 5:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_4_5[instance];
            return true;
        case 6:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_4_6[instance];
            return true;
        case 7:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_4_7[instance];
            return true;
        case 8:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_4_8[instance];
            return true;
        case 9:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_9[instance];
            return true;
        case 10:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_10[instance];
            return true;
        case 11:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_4_11[instance];
            return true;
        case 12:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_4_12[instance];
            return true;
        case 13:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_4_13[instance];
            return true;
        case 14:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_14[instance];
            return true;
        case 15:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_15[instance];
            return true;
        case 16:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_4_16[instance];
            return true;
        case 17:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_4_17[instance];
            return true;
        case 18:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_4_18[instance];
            return true;
        case 19:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_19[instance];
            return true;
        case 20:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_20[instance];
            return true;
        case 21:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_4_21[instance];
            return true;
        case 22:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_4_22[instance];
            return true;
        case 23:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_4_23[instance];
            return true;
        case 24:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_24[instance];
            return true;
        case 25:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_25[instance];
            return true;
        case 26:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_4_26[instance];
            return true;
        case 27:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_4_27[instance];
            return true;
        case 28:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_4_28[instance];
            return true;
        case 29:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_29[instance];
            return true;
        case 30:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_30[instance];
            return true;
        case 31:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_4_31[instance];
            return true;
        case 32:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_4_32[instance];
            return true;
        case 33:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_4_33[instance];
            return true;
        case 34:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_34[instance];
            return true;
        case 35:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_35[instance];
            return true;
        case 36:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_4_36[instance];
            return true;
        case 37:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_4_37[instance];
            return true;
        case 38:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_4_38[instance];
            return true;
        case 39:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_39[instance];
            return true;
        case 40:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_40[instance];
            return true;
        case 41:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_4_41[instance];
            return true;
        case 42:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_4_42[instance];
            return true;
        case 43:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_4_43[instance];
            return true;
        case 44:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_44[instance];
            return true;
        case 45:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_4_45[instance];
            return true;
        case 46:
            value->type = CONFIG_VALUE_BITMAP;
            value->bitmap_value.assign(generated_compact_defaults_4_46[instance], generated_compact_defaults_4_46[instance] + sizeof(generated_compact_defaults_4_46[instance]));
            return true;
        default: return false;
        }
    }
    if (variable_id >= 2581 && variable_id <= 2674) {
        const size_t instance = (variable_id - 2581) / 47;
        const size_t leaf = (variable_id - 2581) % 47;
        *value = {};
        switch (leaf) {
        case 0:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_0[instance];
            return true;
        case 1:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_5_1[instance];
            return true;
        case 2:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_5_2[instance];
            return true;
        case 3:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_5_3[instance];
            return true;
        case 4:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_5_4[instance];
            return true;
        case 5:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_5_5[instance];
            return true;
        case 6:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_5_6[instance];
            return true;
        case 7:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_5_7[instance];
            return true;
        case 8:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_5_8[instance];
            return true;
        case 9:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_9[instance];
            return true;
        case 10:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_10[instance];
            return true;
        case 11:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_5_11[instance];
            return true;
        case 12:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_5_12[instance];
            return true;
        case 13:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_5_13[instance];
            return true;
        case 14:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_14[instance];
            return true;
        case 15:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_15[instance];
            return true;
        case 16:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_5_16[instance];
            return true;
        case 17:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_5_17[instance];
            return true;
        case 18:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_5_18[instance];
            return true;
        case 19:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_19[instance];
            return true;
        case 20:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_20[instance];
            return true;
        case 21:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_5_21[instance];
            return true;
        case 22:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_5_22[instance];
            return true;
        case 23:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_5_23[instance];
            return true;
        case 24:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_24[instance];
            return true;
        case 25:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_25[instance];
            return true;
        case 26:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_5_26[instance];
            return true;
        case 27:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_5_27[instance];
            return true;
        case 28:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_5_28[instance];
            return true;
        case 29:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_29[instance];
            return true;
        case 30:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_30[instance];
            return true;
        case 31:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_5_31[instance];
            return true;
        case 32:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_5_32[instance];
            return true;
        case 33:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_5_33[instance];
            return true;
        case 34:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_34[instance];
            return true;
        case 35:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_35[instance];
            return true;
        case 36:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_5_36[instance];
            return true;
        case 37:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_5_37[instance];
            return true;
        case 38:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_5_38[instance];
            return true;
        case 39:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_39[instance];
            return true;
        case 40:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_40[instance];
            return true;
        case 41:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_5_41[instance];
            return true;
        case 42:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_5_42[instance];
            return true;
        case 43:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_5_43[instance];
            return true;
        case 44:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_44[instance];
            return true;
        case 45:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_5_45[instance];
            return true;
        case 46:
            value->type = CONFIG_VALUE_BITMAP;
            value->bitmap_value.assign(generated_compact_defaults_5_46[instance], generated_compact_defaults_5_46[instance] + sizeof(generated_compact_defaults_5_46[instance]));
            return true;
        default: return false;
        }
    }
    if (variable_id >= 2675 && variable_id <= 2768) {
        const size_t instance = (variable_id - 2675) / 47;
        const size_t leaf = (variable_id - 2675) % 47;
        *value = {};
        switch (leaf) {
        case 0:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_0[instance];
            return true;
        case 1:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_6_1[instance];
            return true;
        case 2:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_6_2[instance];
            return true;
        case 3:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_6_3[instance];
            return true;
        case 4:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_6_4[instance];
            return true;
        case 5:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_6_5[instance];
            return true;
        case 6:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_6_6[instance];
            return true;
        case 7:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_6_7[instance];
            return true;
        case 8:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_6_8[instance];
            return true;
        case 9:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_9[instance];
            return true;
        case 10:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_10[instance];
            return true;
        case 11:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_6_11[instance];
            return true;
        case 12:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_6_12[instance];
            return true;
        case 13:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_6_13[instance];
            return true;
        case 14:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_14[instance];
            return true;
        case 15:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_15[instance];
            return true;
        case 16:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_6_16[instance];
            return true;
        case 17:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_6_17[instance];
            return true;
        case 18:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_6_18[instance];
            return true;
        case 19:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_19[instance];
            return true;
        case 20:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_20[instance];
            return true;
        case 21:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_6_21[instance];
            return true;
        case 22:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_6_22[instance];
            return true;
        case 23:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_6_23[instance];
            return true;
        case 24:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_24[instance];
            return true;
        case 25:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_25[instance];
            return true;
        case 26:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_6_26[instance];
            return true;
        case 27:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_6_27[instance];
            return true;
        case 28:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_6_28[instance];
            return true;
        case 29:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_29[instance];
            return true;
        case 30:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_30[instance];
            return true;
        case 31:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_6_31[instance];
            return true;
        case 32:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_6_32[instance];
            return true;
        case 33:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_6_33[instance];
            return true;
        case 34:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_34[instance];
            return true;
        case 35:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_35[instance];
            return true;
        case 36:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_6_36[instance];
            return true;
        case 37:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_6_37[instance];
            return true;
        case 38:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_6_38[instance];
            return true;
        case 39:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_39[instance];
            return true;
        case 40:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_40[instance];
            return true;
        case 41:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_6_41[instance];
            return true;
        case 42:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_6_42[instance];
            return true;
        case 43:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_6_43[instance];
            return true;
        case 44:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_44[instance];
            return true;
        case 45:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_6_45[instance];
            return true;
        case 46:
            value->type = CONFIG_VALUE_BITMAP;
            value->bitmap_value.assign(generated_compact_defaults_6_46[instance], generated_compact_defaults_6_46[instance] + sizeof(generated_compact_defaults_6_46[instance]));
            return true;
        default: return false;
        }
    }
    if (variable_id >= 2769 && variable_id <= 3943) {
        const size_t instance = (variable_id - 2769) / 47;
        const size_t leaf = (variable_id - 2769) % 47;
        *value = {};
        switch (leaf) {
        case 0:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_0[instance];
            return true;
        case 1:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_7_1[instance];
            return true;
        case 2:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_7_2[instance];
            return true;
        case 3:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_7_3[instance];
            return true;
        case 4:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_7_4[instance];
            return true;
        case 5:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_7_5[instance];
            return true;
        case 6:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_7_6[instance];
            return true;
        case 7:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_7_7[instance];
            return true;
        case 8:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_7_8[instance];
            return true;
        case 9:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_9[instance];
            return true;
        case 10:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_10[instance];
            return true;
        case 11:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_7_11[instance];
            return true;
        case 12:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_7_12[instance];
            return true;
        case 13:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_7_13[instance];
            return true;
        case 14:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_14[instance];
            return true;
        case 15:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_15[instance];
            return true;
        case 16:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_7_16[instance];
            return true;
        case 17:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_7_17[instance];
            return true;
        case 18:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_7_18[instance];
            return true;
        case 19:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_19[instance];
            return true;
        case 20:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_20[instance];
            return true;
        case 21:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_7_21[instance];
            return true;
        case 22:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_7_22[instance];
            return true;
        case 23:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_7_23[instance];
            return true;
        case 24:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_24[instance];
            return true;
        case 25:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_25[instance];
            return true;
        case 26:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_7_26[instance];
            return true;
        case 27:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_7_27[instance];
            return true;
        case 28:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_7_28[instance];
            return true;
        case 29:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_29[instance];
            return true;
        case 30:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_30[instance];
            return true;
        case 31:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_7_31[instance];
            return true;
        case 32:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_7_32[instance];
            return true;
        case 33:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_7_33[instance];
            return true;
        case 34:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_34[instance];
            return true;
        case 35:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_35[instance];
            return true;
        case 36:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_7_36[instance];
            return true;
        case 37:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_7_37[instance];
            return true;
        case 38:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_7_38[instance];
            return true;
        case 39:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_39[instance];
            return true;
        case 40:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_40[instance];
            return true;
        case 41:
            value->type = CONFIG_VALUE_UINT16;
            value->integer_value = generated_compact_defaults_7_41[instance];
            return true;
        case 42:
            value->type = CONFIG_VALUE_UINT8;
            value->integer_value = generated_compact_defaults_7_42[instance];
            return true;
        case 43:
            value->type = CONFIG_VALUE_INT16;
            value->integer_value = generated_compact_defaults_7_43[instance];
            return true;
        case 44:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_44[instance];
            return true;
        case 45:
            value->type = CONFIG_VALUE_INT8;
            value->integer_value = generated_compact_defaults_7_45[instance];
            return true;
        case 46:
            value->type = CONFIG_VALUE_BITMAP;
            value->bitmap_value.assign(generated_compact_defaults_7_46[instance], generated_compact_defaults_7_46[instance] + sizeof(generated_compact_defaults_7_46[instance]));
            return true;
        default: return false;
        }
    }
    switch (variable_id) {
    case 0:
        *value = {};
        value->type = CONFIG_VALUE_BOOL;
        value->bool_value = true;
        return true;
    case 1:
        *value = {};
        value->type = CONFIG_VALUE_INT8;
        value->integer_value = 50;
        return true;
    case 2:
        *value = {};
        value->type = CONFIG_VALUE_BOOL;
        value->bool_value = true;
        return true;
    case 3:
        *value = {};
        value->type = CONFIG_VALUE_INT8;
        value->integer_value = 50;
        return true;
    case 4:
        *value = {};
        value->type = CONFIG_VALUE_BOOL;
        value->bool_value = false;
        return true;
    case 2205:
        *value = {};
        value->type = CONFIG_VALUE_INT8;
        value->integer_value = 7;
        return true;
    case 2206:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 4;
        return true;
    case 2207:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 0;
        return true;
    case 2208:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 6;
        return true;
    case 2209:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 3;
        return true;
    case 2210:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 5;
        return true;
    case 2251:
        *value = {};
        value->type = CONFIG_VALUE_BITMAP;
        value->bitmap_value = { 42, 32, 44, 2, 0 };
        return true;
    case 2252:
        *value = {};
        value->type = CONFIG_VALUE_INT8;
        value->integer_value = 44;
        return true;
    case 2253:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 3;
        return true;
    case 2254:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 6;
        return true;
    case 2255:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 6;
        return true;
    case 2256:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 0;
        return true;
    case 2257:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 5;
        return true;
    case 2298:
        *value = {};
        value->type = CONFIG_VALUE_BITMAP;
        value->bitmap_value = { 128, 0, 0, 1, 2 };
        return true;
    case 3944:
        *value = {};
        value->type = CONFIG_VALUE_BOOL;
        value->bool_value = false;
        return true;
    case 3945:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 0;
        return true;
    case 3946:
        *value = {};
        value->type = CONFIG_VALUE_UINT16;
        value->integer_value = 2;
        return true;
    case 3947:
        *value = {};
        value->type = CONFIG_VALUE_UINT16;
        value->integer_value = 0;
        return true;
    case 3948:
        *value = {};
        value->type = CONFIG_VALUE_UINT16;
        value->integer_value = 2;
        return true;
    case 3949:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 4;
        return true;
    case 3950:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 6;
        return true;
    case 3951:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 2;
        return true;
    case 3952:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 4;
        return true;
    case 3953:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 6;
        return true;
    case 3954:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 9;
        return true;
    case 3955:
        *value = {};
        value->type = CONFIG_VALUE_UINT16;
        value->integer_value = 32000;
        return true;
    case 3956:
        *value = {};
        value->type = CONFIG_VALUE_UINT16;
        value->integer_value = 32000;
        return true;
    case 3957:
        *value = {};
        value->type = CONFIG_VALUE_INT16;
        value->integer_value = 500;
        return true;
    case 3958:
        *value = {};
        value->type = CONFIG_VALUE_INT16;
        value->integer_value = 500;
        return true;
    case 3959:
        *value = {};
        value->type = CONFIG_VALUE_INT16;
        value->integer_value = 500;
        return true;
    case 3960:
        *value = {};
        value->type = CONFIG_VALUE_INT16;
        value->integer_value = 500;
        return true;
    case 3961:
        *value = {};
        value->type = CONFIG_VALUE_UINT8;
        value->integer_value = 2;
        return true;
    case 3962:
        *value = {};
        value->type = CONFIG_VALUE_BOOL;
        value->bool_value = true;
        return true;
    case 3963:
        *value = {};
        value->type = CONFIG_VALUE_BOOL;
        value->bool_value = false;
        return true;
    default: return false;
    }
}

bool Resolve_Variable(t_variable_id variable_id, s_resolved_variable* variable) {
    if (variable == nullptr) return false;
    *variable = {};
    if (variable_id >= 5 && variable_id <= 2204) {
        const t_variable_id leaf = (variable_id - 5) % 1;
        switch (leaf) {
        case 0: {
            variable->type = &generated_type_2;
            uint32_t generated_index_6 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 6, &generated_index_6)) return false;
            uint32_t generated_index_7 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 7, &generated_index_7)) return false;
            variable->targets[0] = { (void*)&BUILD_QUEUE::presets[generated_index_6][generated_index_7], sizeof(BUILD_QUEUE::presets[generated_index_6][generated_index_7]) };
            variable->target_count = 1;
            return true;
        }
        default: return false;
        }
    }
    if (variable_id >= 2211 && variable_id <= 2250) {
        const t_variable_id leaf = (variable_id - 2211) % 5;
        switch (leaf) {
        case 0: {
            variable->type = &generated_type_9;
            uint32_t generated_index_17 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 17, &generated_index_17)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[generated_index_17].type, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[generated_index_17].type) };
            variable->target_count = 1;
            return true;
        }
        case 1: {
            variable->type = &generated_type_10;
            uint32_t generated_index_17 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 17, &generated_index_17)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[generated_index_17].firing_arc, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[generated_index_17].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 2: {
            variable->type = &generated_type_11;
            uint32_t generated_index_17 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 17, &generated_index_17)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[generated_index_17].specials, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[generated_index_17].specials) };
            variable->target_count = 1;
            return true;
        }
        case 3: {
            variable->type = &generated_type_3;
            uint32_t generated_index_17 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 17, &generated_index_17)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[generated_index_17].ammo, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[generated_index_17].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 4: {
            variable->type = &generated_type_12;
            uint32_t generated_index_17 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 17, &generated_index_17)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[generated_index_17].count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[generated_index_17].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[generated_index_17].current_count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[generated_index_17].current_count) };
            variable->target_count = 2;
            return true;
        }
        default: return false;
        }
    }
    if (variable_id >= 2258 && variable_id <= 2297) {
        const t_variable_id leaf = (variable_id - 2258) % 5;
        switch (leaf) {
        case 0: {
            variable->type = &generated_type_9;
            uint32_t generated_index_32 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 32, &generated_index_32)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[generated_index_32].type, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[generated_index_32].type) };
            variable->target_count = 1;
            return true;
        }
        case 1: {
            variable->type = &generated_type_10;
            uint32_t generated_index_32 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 32, &generated_index_32)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[generated_index_32].firing_arc, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[generated_index_32].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 2: {
            variable->type = &generated_type_11;
            uint32_t generated_index_32 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 32, &generated_index_32)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[generated_index_32].specials, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[generated_index_32].specials) };
            variable->target_count = 1;
            return true;
        }
        case 3: {
            variable->type = &generated_type_3;
            uint32_t generated_index_32 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 32, &generated_index_32)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[generated_index_32].ammo, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[generated_index_32].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 4: {
            variable->type = &generated_type_12;
            uint32_t generated_index_32 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 32, &generated_index_32)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[generated_index_32].count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[generated_index_32].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[generated_index_32].current_count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[generated_index_32].current_count) };
            variable->target_count = 2;
            return true;
        }
        default: return false;
        }
    }
    if (variable_id >= 2299 && variable_id <= 2392) {
        const t_variable_id leaf = (variable_id - 2299) % 47;
        switch (leaf) {
        case 0: {
            variable->type = &generated_type_3;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].picture_num, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].picture_num) };
            variable->target_count = 1;
            return true;
        }
        case 1: {
            variable->type = &generated_type_4;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].size, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].size) };
            variable->target_count = 1;
            return true;
        }
        case 2: {
            variable->type = &generated_type_5;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ftl_type, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ftl_type) };
            variable->target_count = 1;
            return true;
        }
        case 3: {
            variable->type = &generated_type_6;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].armor_type, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].armor_type) };
            variable->target_count = 1;
            return true;
        }
        case 4: {
            variable->type = &generated_type_7;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].shield_type, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].shield_type) };
            variable->target_count = 1;
            return true;
        }
        case 5: {
            variable->type = &generated_type_8;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].computer_type, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].computer_type) };
            variable->target_count = 1;
            return true;
        }
        case 6: {
            variable->type = &generated_type_9;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type) };
            variable->target_count = 1;
            return true;
        }
        case 7: {
            variable->type = &generated_type_10;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 8: {
            variable->type = &generated_type_11;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials) };
            variable->target_count = 1;
            return true;
        }
        case 9: {
            variable->type = &generated_type_3;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 10: {
            variable->type = &generated_type_12;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 11: {
            variable->type = &generated_type_9;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type) };
            variable->target_count = 1;
            return true;
        }
        case 12: {
            variable->type = &generated_type_10;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 13: {
            variable->type = &generated_type_11;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials) };
            variable->target_count = 1;
            return true;
        }
        case 14: {
            variable->type = &generated_type_3;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 15: {
            variable->type = &generated_type_12;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 16: {
            variable->type = &generated_type_9;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type) };
            variable->target_count = 1;
            return true;
        }
        case 17: {
            variable->type = &generated_type_10;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 18: {
            variable->type = &generated_type_11;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials) };
            variable->target_count = 1;
            return true;
        }
        case 19: {
            variable->type = &generated_type_3;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 20: {
            variable->type = &generated_type_12;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 21: {
            variable->type = &generated_type_9;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type) };
            variable->target_count = 1;
            return true;
        }
        case 22: {
            variable->type = &generated_type_10;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 23: {
            variable->type = &generated_type_11;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials) };
            variable->target_count = 1;
            return true;
        }
        case 24: {
            variable->type = &generated_type_3;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 25: {
            variable->type = &generated_type_12;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 26: {
            variable->type = &generated_type_9;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type) };
            variable->target_count = 1;
            return true;
        }
        case 27: {
            variable->type = &generated_type_10;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 28: {
            variable->type = &generated_type_11;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials) };
            variable->target_count = 1;
            return true;
        }
        case 29: {
            variable->type = &generated_type_3;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 30: {
            variable->type = &generated_type_12;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 31: {
            variable->type = &generated_type_9;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type) };
            variable->target_count = 1;
            return true;
        }
        case 32: {
            variable->type = &generated_type_10;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 33: {
            variable->type = &generated_type_11;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials) };
            variable->target_count = 1;
            return true;
        }
        case 34: {
            variable->type = &generated_type_3;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 35: {
            variable->type = &generated_type_12;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 36: {
            variable->type = &generated_type_9;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type) };
            variable->target_count = 1;
            return true;
        }
        case 37: {
            variable->type = &generated_type_10;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 38: {
            variable->type = &generated_type_11;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials) };
            variable->target_count = 1;
            return true;
        }
        case 39: {
            variable->type = &generated_type_3;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 40: {
            variable->type = &generated_type_12;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 41: {
            variable->type = &generated_type_9;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].type) };
            variable->target_count = 1;
            return true;
        }
        case 42: {
            variable->type = &generated_type_10;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 43: {
            variable->type = &generated_type_11;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].specials) };
            variable->target_count = 1;
            return true;
        }
        case 44: {
            variable->type = &generated_type_3;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 45: {
            variable->type = &generated_type_12;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            uint32_t generated_index_48 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 48, &generated_index_48)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].ship_weapon[generated_index_48].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 46: {
            variable->type = &generated_type_13;
            uint32_t generated_index_40 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 40, &generated_index_40)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::amoeba_templates[generated_index_40].special_device_flags, sizeof(SHIP_CONFIG::amoeba_templates[generated_index_40].special_device_flags) };
            variable->target_count = 1;
            return true;
        }
        default: return false;
        }
    }
    if (variable_id >= 2393 && variable_id <= 2486) {
        const t_variable_id leaf = (variable_id - 2393) % 47;
        switch (leaf) {
        case 0: {
            variable->type = &generated_type_3;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].picture_num, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].picture_num) };
            variable->target_count = 1;
            return true;
        }
        case 1: {
            variable->type = &generated_type_4;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].size, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].size) };
            variable->target_count = 1;
            return true;
        }
        case 2: {
            variable->type = &generated_type_5;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ftl_type, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ftl_type) };
            variable->target_count = 1;
            return true;
        }
        case 3: {
            variable->type = &generated_type_6;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].armor_type, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].armor_type) };
            variable->target_count = 1;
            return true;
        }
        case 4: {
            variable->type = &generated_type_7;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].shield_type, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].shield_type) };
            variable->target_count = 1;
            return true;
        }
        case 5: {
            variable->type = &generated_type_8;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].computer_type, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].computer_type) };
            variable->target_count = 1;
            return true;
        }
        case 6: {
            variable->type = &generated_type_9;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type) };
            variable->target_count = 1;
            return true;
        }
        case 7: {
            variable->type = &generated_type_10;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 8: {
            variable->type = &generated_type_11;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials) };
            variable->target_count = 1;
            return true;
        }
        case 9: {
            variable->type = &generated_type_3;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 10: {
            variable->type = &generated_type_12;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 11: {
            variable->type = &generated_type_9;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type) };
            variable->target_count = 1;
            return true;
        }
        case 12: {
            variable->type = &generated_type_10;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 13: {
            variable->type = &generated_type_11;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials) };
            variable->target_count = 1;
            return true;
        }
        case 14: {
            variable->type = &generated_type_3;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 15: {
            variable->type = &generated_type_12;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 16: {
            variable->type = &generated_type_9;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type) };
            variable->target_count = 1;
            return true;
        }
        case 17: {
            variable->type = &generated_type_10;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 18: {
            variable->type = &generated_type_11;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials) };
            variable->target_count = 1;
            return true;
        }
        case 19: {
            variable->type = &generated_type_3;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 20: {
            variable->type = &generated_type_12;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 21: {
            variable->type = &generated_type_9;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type) };
            variable->target_count = 1;
            return true;
        }
        case 22: {
            variable->type = &generated_type_10;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 23: {
            variable->type = &generated_type_11;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials) };
            variable->target_count = 1;
            return true;
        }
        case 24: {
            variable->type = &generated_type_3;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 25: {
            variable->type = &generated_type_12;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 26: {
            variable->type = &generated_type_9;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type) };
            variable->target_count = 1;
            return true;
        }
        case 27: {
            variable->type = &generated_type_10;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 28: {
            variable->type = &generated_type_11;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials) };
            variable->target_count = 1;
            return true;
        }
        case 29: {
            variable->type = &generated_type_3;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 30: {
            variable->type = &generated_type_12;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 31: {
            variable->type = &generated_type_9;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type) };
            variable->target_count = 1;
            return true;
        }
        case 32: {
            variable->type = &generated_type_10;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 33: {
            variable->type = &generated_type_11;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials) };
            variable->target_count = 1;
            return true;
        }
        case 34: {
            variable->type = &generated_type_3;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 35: {
            variable->type = &generated_type_12;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 36: {
            variable->type = &generated_type_9;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type) };
            variable->target_count = 1;
            return true;
        }
        case 37: {
            variable->type = &generated_type_10;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 38: {
            variable->type = &generated_type_11;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials) };
            variable->target_count = 1;
            return true;
        }
        case 39: {
            variable->type = &generated_type_3;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 40: {
            variable->type = &generated_type_12;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 41: {
            variable->type = &generated_type_9;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].type) };
            variable->target_count = 1;
            return true;
        }
        case 42: {
            variable->type = &generated_type_10;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 43: {
            variable->type = &generated_type_11;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].specials) };
            variable->target_count = 1;
            return true;
        }
        case 44: {
            variable->type = &generated_type_3;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 45: {
            variable->type = &generated_type_12;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            uint32_t generated_index_64 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 64, &generated_index_64)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].ship_weapon[generated_index_64].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 46: {
            variable->type = &generated_type_13;
            uint32_t generated_index_56 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 56, &generated_index_56)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::crystal_templates[generated_index_56].special_device_flags, sizeof(SHIP_CONFIG::crystal_templates[generated_index_56].special_device_flags) };
            variable->target_count = 1;
            return true;
        }
        default: return false;
        }
    }
    if (variable_id >= 2487 && variable_id <= 2580) {
        const t_variable_id leaf = (variable_id - 2487) % 47;
        switch (leaf) {
        case 0: {
            variable->type = &generated_type_3;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].picture_num, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].picture_num) };
            variable->target_count = 1;
            return true;
        }
        case 1: {
            variable->type = &generated_type_4;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].size, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].size) };
            variable->target_count = 1;
            return true;
        }
        case 2: {
            variable->type = &generated_type_5;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ftl_type, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ftl_type) };
            variable->target_count = 1;
            return true;
        }
        case 3: {
            variable->type = &generated_type_6;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].armor_type, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].armor_type) };
            variable->target_count = 1;
            return true;
        }
        case 4: {
            variable->type = &generated_type_7;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].shield_type, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].shield_type) };
            variable->target_count = 1;
            return true;
        }
        case 5: {
            variable->type = &generated_type_8;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].computer_type, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].computer_type) };
            variable->target_count = 1;
            return true;
        }
        case 6: {
            variable->type = &generated_type_9;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type) };
            variable->target_count = 1;
            return true;
        }
        case 7: {
            variable->type = &generated_type_10;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 8: {
            variable->type = &generated_type_11;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials) };
            variable->target_count = 1;
            return true;
        }
        case 9: {
            variable->type = &generated_type_3;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 10: {
            variable->type = &generated_type_12;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 11: {
            variable->type = &generated_type_9;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type) };
            variable->target_count = 1;
            return true;
        }
        case 12: {
            variable->type = &generated_type_10;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 13: {
            variable->type = &generated_type_11;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials) };
            variable->target_count = 1;
            return true;
        }
        case 14: {
            variable->type = &generated_type_3;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 15: {
            variable->type = &generated_type_12;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 16: {
            variable->type = &generated_type_9;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type) };
            variable->target_count = 1;
            return true;
        }
        case 17: {
            variable->type = &generated_type_10;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 18: {
            variable->type = &generated_type_11;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials) };
            variable->target_count = 1;
            return true;
        }
        case 19: {
            variable->type = &generated_type_3;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 20: {
            variable->type = &generated_type_12;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 21: {
            variable->type = &generated_type_9;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type) };
            variable->target_count = 1;
            return true;
        }
        case 22: {
            variable->type = &generated_type_10;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 23: {
            variable->type = &generated_type_11;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials) };
            variable->target_count = 1;
            return true;
        }
        case 24: {
            variable->type = &generated_type_3;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 25: {
            variable->type = &generated_type_12;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 26: {
            variable->type = &generated_type_9;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type) };
            variable->target_count = 1;
            return true;
        }
        case 27: {
            variable->type = &generated_type_10;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 28: {
            variable->type = &generated_type_11;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials) };
            variable->target_count = 1;
            return true;
        }
        case 29: {
            variable->type = &generated_type_3;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 30: {
            variable->type = &generated_type_12;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 31: {
            variable->type = &generated_type_9;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type) };
            variable->target_count = 1;
            return true;
        }
        case 32: {
            variable->type = &generated_type_10;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 33: {
            variable->type = &generated_type_11;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials) };
            variable->target_count = 1;
            return true;
        }
        case 34: {
            variable->type = &generated_type_3;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 35: {
            variable->type = &generated_type_12;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 36: {
            variable->type = &generated_type_9;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type) };
            variable->target_count = 1;
            return true;
        }
        case 37: {
            variable->type = &generated_type_10;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 38: {
            variable->type = &generated_type_11;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials) };
            variable->target_count = 1;
            return true;
        }
        case 39: {
            variable->type = &generated_type_3;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 40: {
            variable->type = &generated_type_12;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 41: {
            variable->type = &generated_type_9;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].type) };
            variable->target_count = 1;
            return true;
        }
        case 42: {
            variable->type = &generated_type_10;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 43: {
            variable->type = &generated_type_11;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].specials) };
            variable->target_count = 1;
            return true;
        }
        case 44: {
            variable->type = &generated_type_3;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 45: {
            variable->type = &generated_type_12;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            uint32_t generated_index_80 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 80, &generated_index_80)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].ship_weapon[generated_index_80].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 46: {
            variable->type = &generated_type_13;
            uint32_t generated_index_72 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 72, &generated_index_72)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::dragon_templates[generated_index_72].special_device_flags, sizeof(SHIP_CONFIG::dragon_templates[generated_index_72].special_device_flags) };
            variable->target_count = 1;
            return true;
        }
        default: return false;
        }
    }
    if (variable_id >= 2581 && variable_id <= 2674) {
        const t_variable_id leaf = (variable_id - 2581) % 47;
        switch (leaf) {
        case 0: {
            variable->type = &generated_type_3;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].picture_num, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].picture_num) };
            variable->target_count = 1;
            return true;
        }
        case 1: {
            variable->type = &generated_type_4;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].size, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].size) };
            variable->target_count = 1;
            return true;
        }
        case 2: {
            variable->type = &generated_type_5;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ftl_type, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ftl_type) };
            variable->target_count = 1;
            return true;
        }
        case 3: {
            variable->type = &generated_type_6;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].armor_type, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].armor_type) };
            variable->target_count = 1;
            return true;
        }
        case 4: {
            variable->type = &generated_type_7;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].shield_type, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].shield_type) };
            variable->target_count = 1;
            return true;
        }
        case 5: {
            variable->type = &generated_type_8;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].computer_type, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].computer_type) };
            variable->target_count = 1;
            return true;
        }
        case 6: {
            variable->type = &generated_type_9;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type) };
            variable->target_count = 1;
            return true;
        }
        case 7: {
            variable->type = &generated_type_10;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 8: {
            variable->type = &generated_type_11;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials) };
            variable->target_count = 1;
            return true;
        }
        case 9: {
            variable->type = &generated_type_3;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 10: {
            variable->type = &generated_type_12;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 11: {
            variable->type = &generated_type_9;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type) };
            variable->target_count = 1;
            return true;
        }
        case 12: {
            variable->type = &generated_type_10;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 13: {
            variable->type = &generated_type_11;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials) };
            variable->target_count = 1;
            return true;
        }
        case 14: {
            variable->type = &generated_type_3;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 15: {
            variable->type = &generated_type_12;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 16: {
            variable->type = &generated_type_9;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type) };
            variable->target_count = 1;
            return true;
        }
        case 17: {
            variable->type = &generated_type_10;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 18: {
            variable->type = &generated_type_11;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials) };
            variable->target_count = 1;
            return true;
        }
        case 19: {
            variable->type = &generated_type_3;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 20: {
            variable->type = &generated_type_12;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 21: {
            variable->type = &generated_type_9;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type) };
            variable->target_count = 1;
            return true;
        }
        case 22: {
            variable->type = &generated_type_10;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 23: {
            variable->type = &generated_type_11;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials) };
            variable->target_count = 1;
            return true;
        }
        case 24: {
            variable->type = &generated_type_3;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 25: {
            variable->type = &generated_type_12;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 26: {
            variable->type = &generated_type_9;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type) };
            variable->target_count = 1;
            return true;
        }
        case 27: {
            variable->type = &generated_type_10;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 28: {
            variable->type = &generated_type_11;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials) };
            variable->target_count = 1;
            return true;
        }
        case 29: {
            variable->type = &generated_type_3;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 30: {
            variable->type = &generated_type_12;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 31: {
            variable->type = &generated_type_9;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type) };
            variable->target_count = 1;
            return true;
        }
        case 32: {
            variable->type = &generated_type_10;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 33: {
            variable->type = &generated_type_11;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials) };
            variable->target_count = 1;
            return true;
        }
        case 34: {
            variable->type = &generated_type_3;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 35: {
            variable->type = &generated_type_12;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 36: {
            variable->type = &generated_type_9;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type) };
            variable->target_count = 1;
            return true;
        }
        case 37: {
            variable->type = &generated_type_10;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 38: {
            variable->type = &generated_type_11;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials) };
            variable->target_count = 1;
            return true;
        }
        case 39: {
            variable->type = &generated_type_3;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 40: {
            variable->type = &generated_type_12;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 41: {
            variable->type = &generated_type_9;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].type) };
            variable->target_count = 1;
            return true;
        }
        case 42: {
            variable->type = &generated_type_10;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 43: {
            variable->type = &generated_type_11;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].specials) };
            variable->target_count = 1;
            return true;
        }
        case 44: {
            variable->type = &generated_type_3;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 45: {
            variable->type = &generated_type_12;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            uint32_t generated_index_96 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 96, &generated_index_96)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].ship_weapon[generated_index_96].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 46: {
            variable->type = &generated_type_13;
            uint32_t generated_index_88 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 88, &generated_index_88)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::eel_templates[generated_index_88].special_device_flags, sizeof(SHIP_CONFIG::eel_templates[generated_index_88].special_device_flags) };
            variable->target_count = 1;
            return true;
        }
        default: return false;
        }
    }
    if (variable_id >= 2675 && variable_id <= 2768) {
        const t_variable_id leaf = (variable_id - 2675) % 47;
        switch (leaf) {
        case 0: {
            variable->type = &generated_type_3;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].picture_num, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].picture_num) };
            variable->target_count = 1;
            return true;
        }
        case 1: {
            variable->type = &generated_type_4;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].size, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].size) };
            variable->target_count = 1;
            return true;
        }
        case 2: {
            variable->type = &generated_type_5;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ftl_type, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ftl_type) };
            variable->target_count = 1;
            return true;
        }
        case 3: {
            variable->type = &generated_type_6;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].armor_type, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].armor_type) };
            variable->target_count = 1;
            return true;
        }
        case 4: {
            variable->type = &generated_type_7;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].shield_type, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].shield_type) };
            variable->target_count = 1;
            return true;
        }
        case 5: {
            variable->type = &generated_type_8;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].computer_type, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].computer_type) };
            variable->target_count = 1;
            return true;
        }
        case 6: {
            variable->type = &generated_type_9;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type) };
            variable->target_count = 1;
            return true;
        }
        case 7: {
            variable->type = &generated_type_10;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 8: {
            variable->type = &generated_type_11;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials) };
            variable->target_count = 1;
            return true;
        }
        case 9: {
            variable->type = &generated_type_3;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 10: {
            variable->type = &generated_type_12;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 11: {
            variable->type = &generated_type_9;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type) };
            variable->target_count = 1;
            return true;
        }
        case 12: {
            variable->type = &generated_type_10;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 13: {
            variable->type = &generated_type_11;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials) };
            variable->target_count = 1;
            return true;
        }
        case 14: {
            variable->type = &generated_type_3;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 15: {
            variable->type = &generated_type_12;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 16: {
            variable->type = &generated_type_9;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type) };
            variable->target_count = 1;
            return true;
        }
        case 17: {
            variable->type = &generated_type_10;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 18: {
            variable->type = &generated_type_11;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials) };
            variable->target_count = 1;
            return true;
        }
        case 19: {
            variable->type = &generated_type_3;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 20: {
            variable->type = &generated_type_12;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 21: {
            variable->type = &generated_type_9;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type) };
            variable->target_count = 1;
            return true;
        }
        case 22: {
            variable->type = &generated_type_10;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 23: {
            variable->type = &generated_type_11;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials) };
            variable->target_count = 1;
            return true;
        }
        case 24: {
            variable->type = &generated_type_3;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 25: {
            variable->type = &generated_type_12;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 26: {
            variable->type = &generated_type_9;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type) };
            variable->target_count = 1;
            return true;
        }
        case 27: {
            variable->type = &generated_type_10;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 28: {
            variable->type = &generated_type_11;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials) };
            variable->target_count = 1;
            return true;
        }
        case 29: {
            variable->type = &generated_type_3;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 30: {
            variable->type = &generated_type_12;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 31: {
            variable->type = &generated_type_9;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type) };
            variable->target_count = 1;
            return true;
        }
        case 32: {
            variable->type = &generated_type_10;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 33: {
            variable->type = &generated_type_11;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials) };
            variable->target_count = 1;
            return true;
        }
        case 34: {
            variable->type = &generated_type_3;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 35: {
            variable->type = &generated_type_12;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 36: {
            variable->type = &generated_type_9;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type) };
            variable->target_count = 1;
            return true;
        }
        case 37: {
            variable->type = &generated_type_10;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 38: {
            variable->type = &generated_type_11;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials) };
            variable->target_count = 1;
            return true;
        }
        case 39: {
            variable->type = &generated_type_3;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 40: {
            variable->type = &generated_type_12;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 41: {
            variable->type = &generated_type_9;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].type) };
            variable->target_count = 1;
            return true;
        }
        case 42: {
            variable->type = &generated_type_10;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 43: {
            variable->type = &generated_type_11;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].specials) };
            variable->target_count = 1;
            return true;
        }
        case 44: {
            variable->type = &generated_type_3;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 45: {
            variable->type = &generated_type_12;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            uint32_t generated_index_112 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 112, &generated_index_112)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].ship_weapon[generated_index_112].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 46: {
            variable->type = &generated_type_13;
            uint32_t generated_index_104 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 104, &generated_index_104)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::hydra_templates[generated_index_104].special_device_flags, sizeof(SHIP_CONFIG::hydra_templates[generated_index_104].special_device_flags) };
            variable->target_count = 1;
            return true;
        }
        default: return false;
        }
    }
    if (variable_id >= 2769 && variable_id <= 3943) {
        const t_variable_id leaf = (variable_id - 2769) % 47;
        switch (leaf) {
        case 0: {
            variable->type = &generated_type_3;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].picture_num, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].picture_num) };
            variable->target_count = 1;
            return true;
        }
        case 1: {
            variable->type = &generated_type_4;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].size, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].size) };
            variable->target_count = 1;
            return true;
        }
        case 2: {
            variable->type = &generated_type_5;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ftl_type) };
            variable->target_count = 1;
            return true;
        }
        case 3: {
            variable->type = &generated_type_6;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].armor_type, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].armor_type) };
            variable->target_count = 1;
            return true;
        }
        case 4: {
            variable->type = &generated_type_7;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].shield_type, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].shield_type) };
            variable->target_count = 1;
            return true;
        }
        case 5: {
            variable->type = &generated_type_8;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].computer_type, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].computer_type) };
            variable->target_count = 1;
            return true;
        }
        case 6: {
            variable->type = &generated_type_9;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type) };
            variable->target_count = 1;
            return true;
        }
        case 7: {
            variable->type = &generated_type_10;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 8: {
            variable->type = &generated_type_11;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials) };
            variable->target_count = 1;
            return true;
        }
        case 9: {
            variable->type = &generated_type_3;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 10: {
            variable->type = &generated_type_12;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 11: {
            variable->type = &generated_type_9;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type) };
            variable->target_count = 1;
            return true;
        }
        case 12: {
            variable->type = &generated_type_10;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 13: {
            variable->type = &generated_type_11;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials) };
            variable->target_count = 1;
            return true;
        }
        case 14: {
            variable->type = &generated_type_3;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 15: {
            variable->type = &generated_type_12;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 16: {
            variable->type = &generated_type_9;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type) };
            variable->target_count = 1;
            return true;
        }
        case 17: {
            variable->type = &generated_type_10;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 18: {
            variable->type = &generated_type_11;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials) };
            variable->target_count = 1;
            return true;
        }
        case 19: {
            variable->type = &generated_type_3;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 20: {
            variable->type = &generated_type_12;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 21: {
            variable->type = &generated_type_9;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type) };
            variable->target_count = 1;
            return true;
        }
        case 22: {
            variable->type = &generated_type_10;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 23: {
            variable->type = &generated_type_11;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials) };
            variable->target_count = 1;
            return true;
        }
        case 24: {
            variable->type = &generated_type_3;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 25: {
            variable->type = &generated_type_12;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 26: {
            variable->type = &generated_type_9;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type) };
            variable->target_count = 1;
            return true;
        }
        case 27: {
            variable->type = &generated_type_10;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 28: {
            variable->type = &generated_type_11;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials) };
            variable->target_count = 1;
            return true;
        }
        case 29: {
            variable->type = &generated_type_3;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 30: {
            variable->type = &generated_type_12;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 31: {
            variable->type = &generated_type_9;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type) };
            variable->target_count = 1;
            return true;
        }
        case 32: {
            variable->type = &generated_type_10;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 33: {
            variable->type = &generated_type_11;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials) };
            variable->target_count = 1;
            return true;
        }
        case 34: {
            variable->type = &generated_type_3;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 35: {
            variable->type = &generated_type_12;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 36: {
            variable->type = &generated_type_9;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type) };
            variable->target_count = 1;
            return true;
        }
        case 37: {
            variable->type = &generated_type_10;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 38: {
            variable->type = &generated_type_11;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials) };
            variable->target_count = 1;
            return true;
        }
        case 39: {
            variable->type = &generated_type_3;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 40: {
            variable->type = &generated_type_12;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 41: {
            variable->type = &generated_type_9;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].type) };
            variable->target_count = 1;
            return true;
        }
        case 42: {
            variable->type = &generated_type_10;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].firing_arc) };
            variable->target_count = 1;
            return true;
        }
        case 43: {
            variable->type = &generated_type_11;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].specials) };
            variable->target_count = 1;
            return true;
        }
        case 44: {
            variable->type = &generated_type_3;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].ammo) };
            variable->target_count = 1;
            return true;
        }
        case 45: {
            variable->type = &generated_type_12;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            uint32_t generated_index_129 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 129, &generated_index_129)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].count) };
            variable->targets[1] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].ship_weapon[generated_index_129].current_count) };
            variable->target_count = 2;
            return true;
        }
        case 46: {
            variable->type = &generated_type_13;
            uint32_t generated_index_120 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 120, &generated_index_120)) return false;
            uint32_t generated_index_121 = 0;
            if (!Get_Array_Element_Offset(Get_Runtime_Schema(), variable_id, 121, &generated_index_121)) return false;
            variable->targets[0] = { (void*)&SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[generated_index_120][generated_index_121].special_device_flags) };
            variable->target_count = 1;
            return true;
        }
        default: return false;
        }
    }
    switch (variable_id) {
    case 0:
        variable->type = &generated_type_0;
        variable->targets[0] = { (void*)&MOX::_settings.music_on, sizeof(MOX::_settings.music_on) };
        variable->target_count = 1;
        return true;
    case 1:
        variable->type = &generated_type_1;
        variable->targets[0] = { (void*)&MOX::_settings.music_level, sizeof(MOX::_settings.music_level) };
        variable->target_count = 1;
        return true;
    case 2:
        variable->type = &generated_type_0;
        variable->targets[0] = { (void*)&MOX::_settings.sound_fx_on, sizeof(MOX::_settings.sound_fx_on) };
        variable->target_count = 1;
        return true;
    case 3:
        variable->type = &generated_type_1;
        variable->targets[0] = { (void*)&MOX::_settings.sound_fx_level, sizeof(MOX::_settings.sound_fx_level) };
        variable->target_count = 1;
        return true;
    case 4:
        variable->type = &generated_type_0;
        variable->targets[0] = { (void*)&ENHANCED_UX::enhanced_ux_settings.enabled, sizeof(ENHANCED_UX::enhanced_ux_settings.enabled) };
        variable->target_count = 1;
        return true;
    case 2205:
        variable->type = &generated_type_3;
        variable->targets[0] = { (void*)&SHIP_CONFIG::guardian_template.picture_num, sizeof(SHIP_CONFIG::guardian_template.picture_num) };
        variable->target_count = 1;
        return true;
    case 2206:
        variable->type = &generated_type_4;
        variable->targets[0] = { (void*)&SHIP_CONFIG::guardian_template.size, sizeof(SHIP_CONFIG::guardian_template.size) };
        variable->target_count = 1;
        return true;
    case 2207:
        variable->type = &generated_type_5;
        variable->targets[0] = { (void*)&SHIP_CONFIG::guardian_template.ftl_type, sizeof(SHIP_CONFIG::guardian_template.ftl_type) };
        variable->target_count = 1;
        return true;
    case 2208:
        variable->type = &generated_type_6;
        variable->targets[0] = { (void*)&SHIP_CONFIG::guardian_template.armor_type, sizeof(SHIP_CONFIG::guardian_template.armor_type) };
        variable->target_count = 1;
        return true;
    case 2209:
        variable->type = &generated_type_7;
        variable->targets[0] = { (void*)&SHIP_CONFIG::guardian_template.shield_type, sizeof(SHIP_CONFIG::guardian_template.shield_type) };
        variable->target_count = 1;
        return true;
    case 2210:
        variable->type = &generated_type_8;
        variable->targets[0] = { (void*)&SHIP_CONFIG::guardian_template.computer_type, sizeof(SHIP_CONFIG::guardian_template.computer_type) };
        variable->target_count = 1;
        return true;
    case 2251:
        variable->type = &generated_type_13;
        variable->targets[0] = { (void*)&SHIP_CONFIG::guardian_template.special_device_flags, sizeof(SHIP_CONFIG::guardian_template.special_device_flags) };
        variable->target_count = 1;
        return true;
    case 2252:
        variable->type = &generated_type_3;
        variable->targets[0] = { (void*)&SHIP_CONFIG::avenger_template.picture_num, sizeof(SHIP_CONFIG::avenger_template.picture_num) };
        variable->target_count = 1;
        return true;
    case 2253:
        variable->type = &generated_type_4;
        variable->targets[0] = { (void*)&SHIP_CONFIG::avenger_template.size, sizeof(SHIP_CONFIG::avenger_template.size) };
        variable->target_count = 1;
        return true;
    case 2254:
        variable->type = &generated_type_5;
        variable->targets[0] = { (void*)&SHIP_CONFIG::avenger_template.ftl_type, sizeof(SHIP_CONFIG::avenger_template.ftl_type) };
        variable->target_count = 1;
        return true;
    case 2255:
        variable->type = &generated_type_6;
        variable->targets[0] = { (void*)&SHIP_CONFIG::avenger_template.armor_type, sizeof(SHIP_CONFIG::avenger_template.armor_type) };
        variable->target_count = 1;
        return true;
    case 2256:
        variable->type = &generated_type_7;
        variable->targets[0] = { (void*)&SHIP_CONFIG::avenger_template.shield_type, sizeof(SHIP_CONFIG::avenger_template.shield_type) };
        variable->target_count = 1;
        return true;
    case 2257:
        variable->type = &generated_type_8;
        variable->targets[0] = { (void*)&SHIP_CONFIG::avenger_template.computer_type, sizeof(SHIP_CONFIG::avenger_template.computer_type) };
        variable->target_count = 1;
        return true;
    case 2298:
        variable->type = &generated_type_13;
        variable->targets[0] = { (void*)&SHIP_CONFIG::avenger_template.special_device_flags, sizeof(SHIP_CONFIG::avenger_template.special_device_flags) };
        variable->target_count = 1;
        return true;
    case 3944:
        variable->type = &generated_type_0;
        variable->targets[0] = { (void*)&BUILD_QUEUE::autobuild_settings.enabled, sizeof(BUILD_QUEUE::autobuild_settings.enabled) };
        variable->target_count = 1;
        return true;
    case 3945:
        variable->type = &generated_type_14;
        variable->targets[0] = { (void*)&BUILD_QUEUE::autobuild_settings.default_preset, sizeof(BUILD_QUEUE::autobuild_settings.default_preset) };
        variable->target_count = 1;
        return true;
    case 3946:
        variable->type = &generated_type_15;
        variable->targets[0] = { (void*)&SHIPSTAK::scan_bonus.battle_scanner, sizeof(SHIPSTAK::scan_bonus.battle_scanner) };
        variable->target_count = 1;
        return true;
    case 3947:
        variable->type = &generated_type_15;
        variable->targets[0] = { (void*)&SHIPSTAK::scan_bonus.scout_lab, sizeof(SHIPSTAK::scan_bonus.scout_lab) };
        variable->target_count = 1;
        return true;
    case 3948:
        variable->type = &generated_type_15;
        variable->targets[0] = { (void*)&SHIPSTAK::scan_bonus.star_base, sizeof(SHIPSTAK::scan_bonus.star_base) };
        variable->target_count = 1;
        return true;
    case 3949:
        variable->type = &generated_type_16;
        variable->targets[0] = { (void*)&SHIPSTAK::scan_bonus.battlestation, sizeof(SHIPSTAK::scan_bonus.battlestation) };
        variable->target_count = 1;
        return true;
    case 3950:
        variable->type = &generated_type_16;
        variable->targets[0] = { (void*)&SHIPSTAK::scan_bonus.star_fortress, sizeof(SHIPSTAK::scan_bonus.star_fortress) };
        variable->target_count = 1;
        return true;
    case 3951:
        variable->type = &generated_type_16;
        variable->targets[0] = { (void*)&SHIPSTAK::scan_bonus.no_scanner, sizeof(SHIPSTAK::scan_bonus.no_scanner) };
        variable->target_count = 1;
        return true;
    case 3952:
        variable->type = &generated_type_16;
        variable->targets[0] = { (void*)&SHIPSTAK::scan_bonus.tachyon_scanner, sizeof(SHIPSTAK::scan_bonus.tachyon_scanner) };
        variable->target_count = 1;
        return true;
    case 3953:
        variable->type = &generated_type_16;
        variable->targets[0] = { (void*)&SHIPSTAK::scan_bonus.neutron_scanner, sizeof(SHIPSTAK::scan_bonus.neutron_scanner) };
        variable->target_count = 1;
        return true;
    case 3954:
        variable->type = &generated_type_16;
        variable->targets[0] = { (void*)&SHIPSTAK::scan_bonus.sensors, sizeof(SHIPSTAK::scan_bonus.sensors) };
        variable->target_count = 1;
        return true;
    case 3955:
        variable->type = &generated_type_17;
        variable->targets[0] = { (void*)&SHIPSTAK::scan_bonus.planet, sizeof(SHIPSTAK::scan_bonus.planet) };
        variable->target_count = 1;
        return true;
    case 3956:
        variable->type = &generated_type_17;
        variable->targets[0] = { (void*)&SHIPSTAK::scan_bonus.fleet, sizeof(SHIPSTAK::scan_bonus.fleet) };
        variable->target_count = 1;
        return true;
    case 3957:
        variable->type = &generated_type_18;
        variable->targets[0] = { (void*)&SHIPSTAK::stealth_bonus.stealthy_ships, sizeof(SHIPSTAK::stealth_bonus.stealthy_ships) };
        variable->target_count = 1;
        return true;
    case 3958:
        variable->type = &generated_type_18;
        variable->targets[0] = { (void*)&SHIPSTAK::stealth_bonus.stealth_field, sizeof(SHIPSTAK::stealth_bonus.stealth_field) };
        variable->target_count = 1;
        return true;
    case 3959:
        variable->type = &generated_type_18;
        variable->targets[0] = { (void*)&SHIPSTAK::stealth_bonus.cloaking_device, sizeof(SHIPSTAK::stealth_bonus.cloaking_device) };
        variable->target_count = 1;
        return true;
    case 3960:
        variable->type = &generated_type_18;
        variable->targets[0] = { (void*)&SHIPSTAK::stealth_bonus.phasing_cloak, sizeof(SHIPSTAK::stealth_bonus.phasing_cloak) };
        variable->target_count = 1;
        return true;
    case 3961:
        variable->type = &generated_type_19;
        variable->targets[0] = { (void*)&SHIPSTAK::stealth_size_penalty, sizeof(SHIPSTAK::stealth_size_penalty) };
        variable->target_count = 1;
        return true;
    case 3962:
        variable->type = &generated_type_0;
        variable->targets[0] = { (void*)&BOARDING::_board_in_black_hole, sizeof(BOARDING::_board_in_black_hole) };
        variable->target_count = 1;
        return true;
    case 3963:
        variable->type = &generated_type_0;
        variable->targets[0] = { (void*)&CMBTFIRE::_fighters_use_point_defense, sizeof(CMBTFIRE::_fighters_use_point_defense) };
        variable->target_count = 1;
        return true;
    default: return false;
    }
}

}

}
