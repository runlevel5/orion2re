// Generated from specs/config/main.json5. Do not edit.
#include "config/config_runtime.h"
#include <cstring>
#include "game/mox.h"
#include "game/ship_config.h"

namespace config {

static const s_symbol_descriptor generated_values_0[] = {
    { "frigate", 0 },
    { "destroyer", 1 },
    { "cruiser", 2 },
    { "battleship", 3 },
    { "titan", 4 },
};

static const s_symbol_descriptor generated_values_1[] = {
    { "none", 0 },
    { "nuclear", 1 },
    { "fusion", 2 },
    { "ion", 3 },
    { "anti_matter", 4 },
    { "hyper", 5 },
    { "interphased", 6 },
};

static const s_symbol_descriptor generated_values_2[] = {
    { "none", 0 },
    { "titanium", 1 },
    { "tritanium", 2 },
    { "zortrium", 3 },
    { "neutronium", 4 },
    { "adamantium", 5 },
    { "xentronium", 6 },
};

static const s_symbol_descriptor generated_values_3[] = {
    { "none", 0 },
    { "class_i", 1 },
    { "class_iii", 2 },
    { "class_v", 3 },
    { "class_vii", 4 },
    { "class_x", 5 },
};

static const s_symbol_descriptor generated_values_4[] = {
    { "none", 0 },
    { "electronic", 1 },
    { "optronic", 2 },
    { "positronic", 3 },
    { "cybertronic", 4 },
    { "moleculartronic", 5 },
};

static const s_symbol_descriptor generated_values_5[] = {
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

static const s_symbol_descriptor generated_values_6[] = {
    { "none", 0 },
    { "forward", 1 },
    { "forward_extended", 2 },
    { "back_extended", 4 },
    { "rear", 8 },
    { "all_sectors", 15 },
    { "design_360", 16 },
};

static const s_symbol_descriptor generated_values_7[] = {
    { "none", 0 },
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
static const s_type_descriptor generated_type_2 = { CONFIG_VALUE_INT8, false, false, 0, 0, CONFIG_SCALAR_PRIMITIVE, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_3 = { CONFIG_VALUE_UINT8, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_0, 5, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_4 = { CONFIG_VALUE_UINT8, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_1, 7, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_5 = { CONFIG_VALUE_UINT8, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_2, 7, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_6 = { CONFIG_VALUE_UINT8, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_3, 6, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_7 = { CONFIG_VALUE_UINT8, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_4, 6, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_8 = { CONFIG_VALUE_UINT16, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_5, 46, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_9 = { CONFIG_VALUE_UINT8, false, false, 0, 0, CONFIG_SCALAR_ENUM, generated_values_6, 7, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_10 = { CONFIG_VALUE_INT16, false, false, 0, 0, CONFIG_SCALAR_MASK, generated_values_7, 1, generated_flags_0, 15, nullptr, 0, nullptr, 0, nullptr, 0, true, true, 32767, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_11 = { CONFIG_VALUE_INT8, true, false, 0, 0, CONFIG_SCALAR_PRIMITIVE, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };
static const s_type_descriptor generated_type_12 = { CONFIG_VALUE_BITMAP, false, false, 0, 0, CONFIG_SCALAR_BITMAP, nullptr, 0, nullptr, 0, generated_bitmap_values_0, 1, generated_bitmap_flags_0, 39, generated_bitmap_known_bits_0, 40, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 };

static const uint8_t generated_default_0 = true;
static const s_variable_target generated_targets_0[] = {
    { (void*)&MOX::_settings.music_on, sizeof(MOX::_settings.music_on) },
};

static const int8_t generated_default_1 = 50;
static const s_variable_target generated_targets_1[] = {
    { (void*)&MOX::_settings.music_level, sizeof(MOX::_settings.music_level) },
};

static const uint8_t generated_default_2 = true;
static const s_variable_target generated_targets_2[] = {
    { (void*)&MOX::_settings.sound_fx_on, sizeof(MOX::_settings.sound_fx_on) },
};

static const int8_t generated_default_3 = 50;
static const s_variable_target generated_targets_3[] = {
    { (void*)&MOX::_settings.sound_fx_level, sizeof(MOX::_settings.sound_fx_level) },
};

static const int8_t generated_default_4 = 7;
static const s_variable_target generated_targets_4[] = {
    { (void*)&SHIP_CONFIG::guardian_template.picture_num, sizeof(SHIP_CONFIG::guardian_template.picture_num) },
};

static const uint8_t generated_default_5 = 4;
static const s_variable_target generated_targets_5[] = {
    { (void*)&SHIP_CONFIG::guardian_template.size, sizeof(SHIP_CONFIG::guardian_template.size) },
};

static const uint8_t generated_default_6 = 0;
static const s_variable_target generated_targets_6[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ftl_type, sizeof(SHIP_CONFIG::guardian_template.ftl_type) },
};

static const uint8_t generated_default_7 = 6;
static const s_variable_target generated_targets_7[] = {
    { (void*)&SHIP_CONFIG::guardian_template.armor_type, sizeof(SHIP_CONFIG::guardian_template.armor_type) },
};

static const uint8_t generated_default_8 = 3;
static const s_variable_target generated_targets_8[] = {
    { (void*)&SHIP_CONFIG::guardian_template.shield_type, sizeof(SHIP_CONFIG::guardian_template.shield_type) },
};

static const uint8_t generated_default_9 = 5;
static const s_variable_target generated_targets_9[] = {
    { (void*)&SHIP_CONFIG::guardian_template.computer_type, sizeof(SHIP_CONFIG::guardian_template.computer_type) },
};

static const uint16_t generated_default_10 = 11;
static const s_variable_target generated_targets_10[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[0].type, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[0].type) },
};

static const uint8_t generated_default_11 = 15;
static const s_variable_target generated_targets_11[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_12 = 2;
static const s_variable_target generated_targets_12[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[0].specials, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[0].specials) },
};

static const int8_t generated_default_13 = 0;
static const s_variable_target generated_targets_13[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[0].ammo, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[0].ammo) },
};

static const int8_t generated_default_14 = 2;
static const s_variable_target generated_targets_14[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[0].count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[0].current_count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[0].current_count) },
};

static const uint16_t generated_default_15 = 4;
static const s_variable_target generated_targets_15[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[1].type, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[1].type) },
};

static const uint8_t generated_default_16 = 15;
static const s_variable_target generated_targets_16[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_17 = 4;
static const s_variable_target generated_targets_17[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[1].specials, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[1].specials) },
};

static const int8_t generated_default_18 = 0;
static const s_variable_target generated_targets_18[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[1].ammo, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[1].ammo) },
};

static const int8_t generated_default_19 = 10;
static const s_variable_target generated_targets_19[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[1].count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[1].current_count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[1].current_count) },
};

static const uint16_t generated_default_20 = 20;
static const s_variable_target generated_targets_20[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[2].type, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[2].type) },
};

static const uint8_t generated_default_21 = 15;
static const s_variable_target generated_targets_21[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_22 = 17664;
static const s_variable_target generated_targets_22[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[2].specials, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[2].specials) },
};

static const int8_t generated_default_23 = 0;
static const s_variable_target generated_targets_23[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[2].ammo, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[2].ammo) },
};

static const int8_t generated_default_24 = 1;
static const s_variable_target generated_targets_24[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[2].count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[2].current_count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[2].current_count) },
};

static const uint16_t generated_default_25 = 20;
static const s_variable_target generated_targets_25[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[3].type, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[3].type) },
};

static const uint8_t generated_default_26 = 15;
static const s_variable_target generated_targets_26[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_27 = 17664;
static const s_variable_target generated_targets_27[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[3].specials, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[3].specials) },
};

static const int8_t generated_default_28 = 0;
static const s_variable_target generated_targets_28[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[3].ammo, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[3].ammo) },
};

static const int8_t generated_default_29 = 1;
static const s_variable_target generated_targets_29[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[3].count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[3].current_count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[3].current_count) },
};

static const uint16_t generated_default_30 = 13;
static const s_variable_target generated_targets_30[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[4].type, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[4].type) },
};

static const uint8_t generated_default_31 = 15;
static const s_variable_target generated_targets_31[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_32 = 0;
static const s_variable_target generated_targets_32[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[4].specials, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[4].specials) },
};

static const int8_t generated_default_33 = 0;
static const s_variable_target generated_targets_33[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[4].ammo, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[4].ammo) },
};

static const int8_t generated_default_34 = 2;
static const s_variable_target generated_targets_34[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[4].count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[4].current_count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[4].current_count) },
};

static const uint16_t generated_default_35 = 0;
static const s_variable_target generated_targets_35[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[5].type, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[5].type) },
};

static const uint8_t generated_default_36 = 0;
static const s_variable_target generated_targets_36[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_37 = 0;
static const s_variable_target generated_targets_37[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[5].specials, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[5].specials) },
};

static const int8_t generated_default_38 = 0;
static const s_variable_target generated_targets_38[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[5].ammo, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[5].ammo) },
};

static const int8_t generated_default_39 = 0;
static const s_variable_target generated_targets_39[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[5].count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[5].current_count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[5].current_count) },
};

static const uint16_t generated_default_40 = 0;
static const s_variable_target generated_targets_40[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[6].type, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[6].type) },
};

static const uint8_t generated_default_41 = 0;
static const s_variable_target generated_targets_41[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_42 = 0;
static const s_variable_target generated_targets_42[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[6].specials, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[6].specials) },
};

static const int8_t generated_default_43 = 0;
static const s_variable_target generated_targets_43[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[6].ammo, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[6].ammo) },
};

static const int8_t generated_default_44 = 0;
static const s_variable_target generated_targets_44[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[6].count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[6].current_count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[6].current_count) },
};

static const uint16_t generated_default_45 = 0;
static const s_variable_target generated_targets_45[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[7].type, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[7].type) },
};

static const uint8_t generated_default_46 = 0;
static const s_variable_target generated_targets_46[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_47 = 0;
static const s_variable_target generated_targets_47[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[7].specials, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[7].specials) },
};

static const int8_t generated_default_48 = 0;
static const s_variable_target generated_targets_48[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[7].ammo, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[7].ammo) },
};

static const int8_t generated_default_49 = 0;
static const s_variable_target generated_targets_49[] = {
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[7].count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::guardian_template.ship_weapon[7].current_count, sizeof(SHIP_CONFIG::guardian_template.ship_weapon[7].current_count) },
};

static const uint8_t generated_default_50[] = { 42, 32, 44, 2, 0 };
static const s_variable_target generated_targets_50[] = {
    { (void*)&SHIP_CONFIG::guardian_template.special_device_flags, sizeof(SHIP_CONFIG::guardian_template.special_device_flags) },
};

static const int8_t generated_default_51 = 44;
static const s_variable_target generated_targets_51[] = {
    { (void*)&SHIP_CONFIG::avenger_template.picture_num, sizeof(SHIP_CONFIG::avenger_template.picture_num) },
};

static const uint8_t generated_default_52 = 3;
static const s_variable_target generated_targets_52[] = {
    { (void*)&SHIP_CONFIG::avenger_template.size, sizeof(SHIP_CONFIG::avenger_template.size) },
};

static const uint8_t generated_default_53 = 6;
static const s_variable_target generated_targets_53[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ftl_type, sizeof(SHIP_CONFIG::avenger_template.ftl_type) },
};

static const uint8_t generated_default_54 = 6;
static const s_variable_target generated_targets_54[] = {
    { (void*)&SHIP_CONFIG::avenger_template.armor_type, sizeof(SHIP_CONFIG::avenger_template.armor_type) },
};

static const uint8_t generated_default_55 = 0;
static const s_variable_target generated_targets_55[] = {
    { (void*)&SHIP_CONFIG::avenger_template.shield_type, sizeof(SHIP_CONFIG::avenger_template.shield_type) },
};

static const uint8_t generated_default_56 = 5;
static const s_variable_target generated_targets_56[] = {
    { (void*)&SHIP_CONFIG::avenger_template.computer_type, sizeof(SHIP_CONFIG::avenger_template.computer_type) },
};

static const uint16_t generated_default_57 = 4;
static const s_variable_target generated_targets_57[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[0].type, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[0].type) },
};

static const uint8_t generated_default_58 = 1;
static const s_variable_target generated_targets_58[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_59 = 2;
static const s_variable_target generated_targets_59[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[0].specials, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[0].specials) },
};

static const int8_t generated_default_60 = 0;
static const s_variable_target generated_targets_60[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[0].ammo, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[0].ammo) },
};

static const int8_t generated_default_61 = 4;
static const s_variable_target generated_targets_61[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[0].count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[0].current_count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[0].current_count) },
};

static const uint16_t generated_default_62 = 11;
static const s_variable_target generated_targets_62[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[1].type, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[1].type) },
};

static const uint8_t generated_default_63 = 1;
static const s_variable_target generated_targets_63[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_64 = 2;
static const s_variable_target generated_targets_64[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[1].specials, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[1].specials) },
};

static const int8_t generated_default_65 = 0;
static const s_variable_target generated_targets_65[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[1].ammo, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[1].ammo) },
};

static const int8_t generated_default_66 = 2;
static const s_variable_target generated_targets_66[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[1].count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[1].current_count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[1].current_count) },
};

static const uint16_t generated_default_67 = 4;
static const s_variable_target generated_targets_67[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[2].type, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[2].type) },
};

static const uint8_t generated_default_68 = 1;
static const s_variable_target generated_targets_68[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_69 = 0;
static const s_variable_target generated_targets_69[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[2].specials, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[2].specials) },
};

static const int8_t generated_default_70 = 0;
static const s_variable_target generated_targets_70[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[2].ammo, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[2].ammo) },
};

static const int8_t generated_default_71 = 10;
static const s_variable_target generated_targets_71[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[2].count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[2].current_count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[2].current_count) },
};

static const uint16_t generated_default_72 = 13;
static const s_variable_target generated_targets_72[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[3].type, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[3].type) },
};

static const uint8_t generated_default_73 = 15;
static const s_variable_target generated_targets_73[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_74 = 0;
static const s_variable_target generated_targets_74[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[3].specials, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[3].specials) },
};

static const int8_t generated_default_75 = 0;
static const s_variable_target generated_targets_75[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[3].ammo, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[3].ammo) },
};

static const int8_t generated_default_76 = 2;
static const s_variable_target generated_targets_76[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[3].count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[3].current_count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[3].current_count) },
};

static const uint16_t generated_default_77 = 4;
static const s_variable_target generated_targets_77[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[4].type, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[4].type) },
};

static const uint8_t generated_default_78 = 1;
static const s_variable_target generated_targets_78[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_79 = 4;
static const s_variable_target generated_targets_79[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[4].specials, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[4].specials) },
};

static const int8_t generated_default_80 = 0;
static const s_variable_target generated_targets_80[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[4].ammo, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[4].ammo) },
};

static const int8_t generated_default_81 = 10;
static const s_variable_target generated_targets_81[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[4].count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[4].current_count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[4].current_count) },
};

static const uint16_t generated_default_82 = 24;
static const s_variable_target generated_targets_82[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[5].type, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[5].type) },
};

static const uint8_t generated_default_83 = 1;
static const s_variable_target generated_targets_83[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_84 = 0;
static const s_variable_target generated_targets_84[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[5].specials, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[5].specials) },
};

static const int8_t generated_default_85 = 10;
static const s_variable_target generated_targets_85[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[5].ammo, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[5].ammo) },
};

static const int8_t generated_default_86 = 15;
static const s_variable_target generated_targets_86[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[5].count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[5].current_count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[5].current_count) },
};

static const uint16_t generated_default_87 = 0;
static const s_variable_target generated_targets_87[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[6].type, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[6].type) },
};

static const uint8_t generated_default_88 = 0;
static const s_variable_target generated_targets_88[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_89 = 0;
static const s_variable_target generated_targets_89[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[6].specials, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[6].specials) },
};

static const int8_t generated_default_90 = 0;
static const s_variable_target generated_targets_90[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[6].ammo, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[6].ammo) },
};

static const int8_t generated_default_91 = 0;
static const s_variable_target generated_targets_91[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[6].count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[6].current_count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[6].current_count) },
};

static const uint16_t generated_default_92 = 0;
static const s_variable_target generated_targets_92[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[7].type, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[7].type) },
};

static const uint8_t generated_default_93 = 0;
static const s_variable_target generated_targets_93[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_94 = 0;
static const s_variable_target generated_targets_94[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[7].specials, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[7].specials) },
};

static const int8_t generated_default_95 = 0;
static const s_variable_target generated_targets_95[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[7].ammo, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[7].ammo) },
};

static const int8_t generated_default_96 = 0;
static const s_variable_target generated_targets_96[] = {
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[7].count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::avenger_template.ship_weapon[7].current_count, sizeof(SHIP_CONFIG::avenger_template.ship_weapon[7].current_count) },
};

static const uint8_t generated_default_97[] = { 128, 0, 0, 1, 2 };
static const s_variable_target generated_targets_97[] = {
    { (void*)&SHIP_CONFIG::avenger_template.special_device_flags, sizeof(SHIP_CONFIG::avenger_template.special_device_flags) },
};

static const int8_t generated_default_98 = 20;
static const s_variable_target generated_targets_98[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].picture_num, sizeof(SHIP_CONFIG::amoeba_templates[0].picture_num) },
};

static const uint8_t generated_default_99 = 2;
static const s_variable_target generated_targets_99[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].size, sizeof(SHIP_CONFIG::amoeba_templates[0].size) },
};

static const uint8_t generated_default_100 = 0;
static const s_variable_target generated_targets_100[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ftl_type, sizeof(SHIP_CONFIG::amoeba_templates[0].ftl_type) },
};

static const uint8_t generated_default_101 = 0;
static const s_variable_target generated_targets_101[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].armor_type, sizeof(SHIP_CONFIG::amoeba_templates[0].armor_type) },
};

static const uint8_t generated_default_102 = 0;
static const s_variable_target generated_targets_102[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].shield_type, sizeof(SHIP_CONFIG::amoeba_templates[0].shield_type) },
};

static const uint8_t generated_default_103 = 2;
static const s_variable_target generated_targets_103[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].computer_type, sizeof(SHIP_CONFIG::amoeba_templates[0].computer_type) },
};

static const uint16_t generated_default_104 = 45;
static const s_variable_target generated_targets_104[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[0].type, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[0].type) },
};

static const uint8_t generated_default_105 = 15;
static const s_variable_target generated_targets_105[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_106 = 0;
static const s_variable_target generated_targets_106[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[0].specials, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[0].specials) },
};

static const int8_t generated_default_107 = 0;
static const s_variable_target generated_targets_107[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[0].ammo) },
};

static const int8_t generated_default_108 = 1;
static const s_variable_target generated_targets_108[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[0].count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_109 = 0;
static const s_variable_target generated_targets_109[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[1].type, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[1].type) },
};

static const uint8_t generated_default_110 = 0;
static const s_variable_target generated_targets_110[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_111 = 0;
static const s_variable_target generated_targets_111[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[1].specials, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[1].specials) },
};

static const int8_t generated_default_112 = 0;
static const s_variable_target generated_targets_112[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[1].ammo) },
};

static const int8_t generated_default_113 = 0;
static const s_variable_target generated_targets_113[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[1].count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_114 = 0;
static const s_variable_target generated_targets_114[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[2].type, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[2].type) },
};

static const uint8_t generated_default_115 = 0;
static const s_variable_target generated_targets_115[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_116 = 0;
static const s_variable_target generated_targets_116[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[2].specials, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[2].specials) },
};

static const int8_t generated_default_117 = 0;
static const s_variable_target generated_targets_117[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[2].ammo) },
};

static const int8_t generated_default_118 = 0;
static const s_variable_target generated_targets_118[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[2].count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_119 = 0;
static const s_variable_target generated_targets_119[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[3].type, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[3].type) },
};

static const uint8_t generated_default_120 = 0;
static const s_variable_target generated_targets_120[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_121 = 0;
static const s_variable_target generated_targets_121[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[3].specials, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[3].specials) },
};

static const int8_t generated_default_122 = 0;
static const s_variable_target generated_targets_122[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[3].ammo) },
};

static const int8_t generated_default_123 = 0;
static const s_variable_target generated_targets_123[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[3].count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_124 = 0;
static const s_variable_target generated_targets_124[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[4].type, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[4].type) },
};

static const uint8_t generated_default_125 = 0;
static const s_variable_target generated_targets_125[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_126 = 0;
static const s_variable_target generated_targets_126[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[4].specials, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[4].specials) },
};

static const int8_t generated_default_127 = 0;
static const s_variable_target generated_targets_127[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[4].ammo) },
};

static const int8_t generated_default_128 = 0;
static const s_variable_target generated_targets_128[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[4].count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_129 = 0;
static const s_variable_target generated_targets_129[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[5].type, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[5].type) },
};

static const uint8_t generated_default_130 = 0;
static const s_variable_target generated_targets_130[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_131 = 0;
static const s_variable_target generated_targets_131[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[5].specials, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[5].specials) },
};

static const int8_t generated_default_132 = 0;
static const s_variable_target generated_targets_132[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[5].ammo) },
};

static const int8_t generated_default_133 = 0;
static const s_variable_target generated_targets_133[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[5].count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_134 = 0;
static const s_variable_target generated_targets_134[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[6].type, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[6].type) },
};

static const uint8_t generated_default_135 = 0;
static const s_variable_target generated_targets_135[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_136 = 0;
static const s_variable_target generated_targets_136[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[6].specials, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[6].specials) },
};

static const int8_t generated_default_137 = 0;
static const s_variable_target generated_targets_137[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[6].ammo) },
};

static const int8_t generated_default_138 = 0;
static const s_variable_target generated_targets_138[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[6].count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_139 = 0;
static const s_variable_target generated_targets_139[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[7].type, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[7].type) },
};

static const uint8_t generated_default_140 = 0;
static const s_variable_target generated_targets_140[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_141 = 0;
static const s_variable_target generated_targets_141[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[7].specials, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[7].specials) },
};

static const int8_t generated_default_142 = 0;
static const s_variable_target generated_targets_142[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[7].ammo) },
};

static const int8_t generated_default_143 = 0;
static const s_variable_target generated_targets_143[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[7].count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[0].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::amoeba_templates[0].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_144[] = { 0, 0, 0, 0, 128 };
static const s_variable_target generated_targets_144[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[0].special_device_flags, sizeof(SHIP_CONFIG::amoeba_templates[0].special_device_flags) },
};

static const int8_t generated_default_145 = 8;
static const s_variable_target generated_targets_145[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].picture_num, sizeof(SHIP_CONFIG::amoeba_templates[1].picture_num) },
};

static const uint8_t generated_default_146 = 3;
static const s_variable_target generated_targets_146[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].size, sizeof(SHIP_CONFIG::amoeba_templates[1].size) },
};

static const uint8_t generated_default_147 = 2;
static const s_variable_target generated_targets_147[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ftl_type, sizeof(SHIP_CONFIG::amoeba_templates[1].ftl_type) },
};

static const uint8_t generated_default_148 = 0;
static const s_variable_target generated_targets_148[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].armor_type, sizeof(SHIP_CONFIG::amoeba_templates[1].armor_type) },
};

static const uint8_t generated_default_149 = 0;
static const s_variable_target generated_targets_149[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].shield_type, sizeof(SHIP_CONFIG::amoeba_templates[1].shield_type) },
};

static const uint8_t generated_default_150 = 2;
static const s_variable_target generated_targets_150[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].computer_type, sizeof(SHIP_CONFIG::amoeba_templates[1].computer_type) },
};

static const uint16_t generated_default_151 = 45;
static const s_variable_target generated_targets_151[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[0].type, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[0].type) },
};

static const uint8_t generated_default_152 = 15;
static const s_variable_target generated_targets_152[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_153 = 0;
static const s_variable_target generated_targets_153[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[0].specials, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[0].specials) },
};

static const int8_t generated_default_154 = 0;
static const s_variable_target generated_targets_154[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[0].ammo) },
};

static const int8_t generated_default_155 = 2;
static const s_variable_target generated_targets_155[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[0].count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_156 = 23;
static const s_variable_target generated_targets_156[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[1].type, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[1].type) },
};

static const uint8_t generated_default_157 = 15;
static const s_variable_target generated_targets_157[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_158 = 0;
static const s_variable_target generated_targets_158[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[1].specials, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[1].specials) },
};

static const int8_t generated_default_159 = 10;
static const s_variable_target generated_targets_159[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[1].ammo) },
};

static const int8_t generated_default_160 = 5;
static const s_variable_target generated_targets_160[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[1].count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_161 = 0;
static const s_variable_target generated_targets_161[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[2].type, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[2].type) },
};

static const uint8_t generated_default_162 = 0;
static const s_variable_target generated_targets_162[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_163 = 0;
static const s_variable_target generated_targets_163[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[2].specials, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[2].specials) },
};

static const int8_t generated_default_164 = 0;
static const s_variable_target generated_targets_164[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[2].ammo) },
};

static const int8_t generated_default_165 = 0;
static const s_variable_target generated_targets_165[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[2].count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_166 = 0;
static const s_variable_target generated_targets_166[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[3].type, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[3].type) },
};

static const uint8_t generated_default_167 = 0;
static const s_variable_target generated_targets_167[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_168 = 0;
static const s_variable_target generated_targets_168[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[3].specials, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[3].specials) },
};

static const int8_t generated_default_169 = 0;
static const s_variable_target generated_targets_169[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[3].ammo) },
};

static const int8_t generated_default_170 = 0;
static const s_variable_target generated_targets_170[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[3].count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_171 = 0;
static const s_variable_target generated_targets_171[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[4].type, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[4].type) },
};

static const uint8_t generated_default_172 = 0;
static const s_variable_target generated_targets_172[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_173 = 0;
static const s_variable_target generated_targets_173[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[4].specials, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[4].specials) },
};

static const int8_t generated_default_174 = 0;
static const s_variable_target generated_targets_174[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[4].ammo) },
};

static const int8_t generated_default_175 = 0;
static const s_variable_target generated_targets_175[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[4].count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_176 = 0;
static const s_variable_target generated_targets_176[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[5].type, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[5].type) },
};

static const uint8_t generated_default_177 = 0;
static const s_variable_target generated_targets_177[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_178 = 0;
static const s_variable_target generated_targets_178[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[5].specials, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[5].specials) },
};

static const int8_t generated_default_179 = 0;
static const s_variable_target generated_targets_179[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[5].ammo) },
};

static const int8_t generated_default_180 = 0;
static const s_variable_target generated_targets_180[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[5].count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_181 = 0;
static const s_variable_target generated_targets_181[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[6].type, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[6].type) },
};

static const uint8_t generated_default_182 = 0;
static const s_variable_target generated_targets_182[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_183 = 0;
static const s_variable_target generated_targets_183[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[6].specials, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[6].specials) },
};

static const int8_t generated_default_184 = 0;
static const s_variable_target generated_targets_184[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[6].ammo) },
};

static const int8_t generated_default_185 = 0;
static const s_variable_target generated_targets_185[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[6].count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_186 = 0;
static const s_variable_target generated_targets_186[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[7].type, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[7].type) },
};

static const uint8_t generated_default_187 = 0;
static const s_variable_target generated_targets_187[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_188 = 0;
static const s_variable_target generated_targets_188[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[7].specials, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[7].specials) },
};

static const int8_t generated_default_189 = 0;
static const s_variable_target generated_targets_189[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[7].ammo) },
};

static const int8_t generated_default_190 = 0;
static const s_variable_target generated_targets_190[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[7].count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::amoeba_templates[1].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::amoeba_templates[1].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_191[] = { 0, 0, 0, 0, 128 };
static const s_variable_target generated_targets_191[] = {
    { (void*)&SHIP_CONFIG::amoeba_templates[1].special_device_flags, sizeof(SHIP_CONFIG::amoeba_templates[1].special_device_flags) },
};

static const int8_t generated_default_192 = 21;
static const s_variable_target generated_targets_192[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].picture_num, sizeof(SHIP_CONFIG::crystal_templates[0].picture_num) },
};

static const uint8_t generated_default_193 = 2;
static const s_variable_target generated_targets_193[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].size, sizeof(SHIP_CONFIG::crystal_templates[0].size) },
};

static const uint8_t generated_default_194 = 0;
static const s_variable_target generated_targets_194[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ftl_type, sizeof(SHIP_CONFIG::crystal_templates[0].ftl_type) },
};

static const uint8_t generated_default_195 = 0;
static const s_variable_target generated_targets_195[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].armor_type, sizeof(SHIP_CONFIG::crystal_templates[0].armor_type) },
};

static const uint8_t generated_default_196 = 0;
static const s_variable_target generated_targets_196[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].shield_type, sizeof(SHIP_CONFIG::crystal_templates[0].shield_type) },
};

static const uint8_t generated_default_197 = 5;
static const s_variable_target generated_targets_197[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].computer_type, sizeof(SHIP_CONFIG::crystal_templates[0].computer_type) },
};

static const uint16_t generated_default_198 = 42;
static const s_variable_target generated_targets_198[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[0].type, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[0].type) },
};

static const uint8_t generated_default_199 = 15;
static const s_variable_target generated_targets_199[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_200 = 0;
static const s_variable_target generated_targets_200[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[0].specials, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[0].specials) },
};

static const int8_t generated_default_201 = 0;
static const s_variable_target generated_targets_201[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[0].ammo) },
};

static const int8_t generated_default_202 = 1;
static const s_variable_target generated_targets_202[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[0].count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_203 = 25;
static const s_variable_target generated_targets_203[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[1].type, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[1].type) },
};

static const uint8_t generated_default_204 = 15;
static const s_variable_target generated_targets_204[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_205 = 0;
static const s_variable_target generated_targets_205[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[1].specials, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[1].specials) },
};

static const int8_t generated_default_206 = 10;
static const s_variable_target generated_targets_206[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[1].ammo) },
};

static const int8_t generated_default_207 = 5;
static const s_variable_target generated_targets_207[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[1].count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_208 = 0;
static const s_variable_target generated_targets_208[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[2].type, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[2].type) },
};

static const uint8_t generated_default_209 = 0;
static const s_variable_target generated_targets_209[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_210 = 0;
static const s_variable_target generated_targets_210[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[2].specials, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[2].specials) },
};

static const int8_t generated_default_211 = 0;
static const s_variable_target generated_targets_211[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[2].ammo) },
};

static const int8_t generated_default_212 = 0;
static const s_variable_target generated_targets_212[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[2].count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_213 = 0;
static const s_variable_target generated_targets_213[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[3].type, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[3].type) },
};

static const uint8_t generated_default_214 = 0;
static const s_variable_target generated_targets_214[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_215 = 0;
static const s_variable_target generated_targets_215[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[3].specials, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[3].specials) },
};

static const int8_t generated_default_216 = 0;
static const s_variable_target generated_targets_216[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[3].ammo) },
};

static const int8_t generated_default_217 = 0;
static const s_variable_target generated_targets_217[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[3].count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_218 = 0;
static const s_variable_target generated_targets_218[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[4].type, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[4].type) },
};

static const uint8_t generated_default_219 = 0;
static const s_variable_target generated_targets_219[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_220 = 0;
static const s_variable_target generated_targets_220[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[4].specials, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[4].specials) },
};

static const int8_t generated_default_221 = 0;
static const s_variable_target generated_targets_221[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[4].ammo) },
};

static const int8_t generated_default_222 = 0;
static const s_variable_target generated_targets_222[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[4].count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_223 = 0;
static const s_variable_target generated_targets_223[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[5].type, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[5].type) },
};

static const uint8_t generated_default_224 = 0;
static const s_variable_target generated_targets_224[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_225 = 0;
static const s_variable_target generated_targets_225[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[5].specials, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[5].specials) },
};

static const int8_t generated_default_226 = 0;
static const s_variable_target generated_targets_226[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[5].ammo) },
};

static const int8_t generated_default_227 = 0;
static const s_variable_target generated_targets_227[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[5].count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_228 = 0;
static const s_variable_target generated_targets_228[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[6].type, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[6].type) },
};

static const uint8_t generated_default_229 = 0;
static const s_variable_target generated_targets_229[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_230 = 0;
static const s_variable_target generated_targets_230[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[6].specials, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[6].specials) },
};

static const int8_t generated_default_231 = 0;
static const s_variable_target generated_targets_231[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[6].ammo) },
};

static const int8_t generated_default_232 = 0;
static const s_variable_target generated_targets_232[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[6].count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_233 = 0;
static const s_variable_target generated_targets_233[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[7].type, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[7].type) },
};

static const uint8_t generated_default_234 = 0;
static const s_variable_target generated_targets_234[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_235 = 0;
static const s_variable_target generated_targets_235[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[7].specials, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[7].specials) },
};

static const int8_t generated_default_236 = 0;
static const s_variable_target generated_targets_236[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[7].ammo) },
};

static const int8_t generated_default_237 = 0;
static const s_variable_target generated_targets_237[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[7].count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[0].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::crystal_templates[0].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_238[] = { 0, 0, 0, 0, 0 };
static const s_variable_target generated_targets_238[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[0].special_device_flags, sizeof(SHIP_CONFIG::crystal_templates[0].special_device_flags) },
};

static const int8_t generated_default_239 = 9;
static const s_variable_target generated_targets_239[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].picture_num, sizeof(SHIP_CONFIG::crystal_templates[1].picture_num) },
};

static const uint8_t generated_default_240 = 4;
static const s_variable_target generated_targets_240[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].size, sizeof(SHIP_CONFIG::crystal_templates[1].size) },
};

static const uint8_t generated_default_241 = 4;
static const s_variable_target generated_targets_241[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ftl_type, sizeof(SHIP_CONFIG::crystal_templates[1].ftl_type) },
};

static const uint8_t generated_default_242 = 0;
static const s_variable_target generated_targets_242[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].armor_type, sizeof(SHIP_CONFIG::crystal_templates[1].armor_type) },
};

static const uint8_t generated_default_243 = 0;
static const s_variable_target generated_targets_243[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].shield_type, sizeof(SHIP_CONFIG::crystal_templates[1].shield_type) },
};

static const uint8_t generated_default_244 = 5;
static const s_variable_target generated_targets_244[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].computer_type, sizeof(SHIP_CONFIG::crystal_templates[1].computer_type) },
};

static const uint16_t generated_default_245 = 42;
static const s_variable_target generated_targets_245[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[0].type, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[0].type) },
};

static const uint8_t generated_default_246 = 15;
static const s_variable_target generated_targets_246[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_247 = 2;
static const s_variable_target generated_targets_247[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[0].specials, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[0].specials) },
};

static const int8_t generated_default_248 = 0;
static const s_variable_target generated_targets_248[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[0].ammo) },
};

static const int8_t generated_default_249 = 1;
static const s_variable_target generated_targets_249[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[0].count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_250 = 26;
static const s_variable_target generated_targets_250[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[1].type, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[1].type) },
};

static const uint8_t generated_default_251 = 15;
static const s_variable_target generated_targets_251[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_252 = 0;
static const s_variable_target generated_targets_252[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[1].specials, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[1].specials) },
};

static const int8_t generated_default_253 = 10;
static const s_variable_target generated_targets_253[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[1].ammo) },
};

static const int8_t generated_default_254 = 5;
static const s_variable_target generated_targets_254[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[1].count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_255 = 0;
static const s_variable_target generated_targets_255[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[2].type, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[2].type) },
};

static const uint8_t generated_default_256 = 0;
static const s_variable_target generated_targets_256[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_257 = 0;
static const s_variable_target generated_targets_257[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[2].specials, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[2].specials) },
};

static const int8_t generated_default_258 = 0;
static const s_variable_target generated_targets_258[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[2].ammo) },
};

static const int8_t generated_default_259 = 0;
static const s_variable_target generated_targets_259[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[2].count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_260 = 0;
static const s_variable_target generated_targets_260[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[3].type, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[3].type) },
};

static const uint8_t generated_default_261 = 0;
static const s_variable_target generated_targets_261[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_262 = 0;
static const s_variable_target generated_targets_262[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[3].specials, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[3].specials) },
};

static const int8_t generated_default_263 = 0;
static const s_variable_target generated_targets_263[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[3].ammo) },
};

static const int8_t generated_default_264 = 0;
static const s_variable_target generated_targets_264[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[3].count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_265 = 0;
static const s_variable_target generated_targets_265[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[4].type, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[4].type) },
};

static const uint8_t generated_default_266 = 0;
static const s_variable_target generated_targets_266[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_267 = 0;
static const s_variable_target generated_targets_267[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[4].specials, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[4].specials) },
};

static const int8_t generated_default_268 = 0;
static const s_variable_target generated_targets_268[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[4].ammo) },
};

static const int8_t generated_default_269 = 0;
static const s_variable_target generated_targets_269[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[4].count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_270 = 0;
static const s_variable_target generated_targets_270[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[5].type, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[5].type) },
};

static const uint8_t generated_default_271 = 0;
static const s_variable_target generated_targets_271[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_272 = 0;
static const s_variable_target generated_targets_272[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[5].specials, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[5].specials) },
};

static const int8_t generated_default_273 = 0;
static const s_variable_target generated_targets_273[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[5].ammo) },
};

static const int8_t generated_default_274 = 0;
static const s_variable_target generated_targets_274[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[5].count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_275 = 0;
static const s_variable_target generated_targets_275[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[6].type, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[6].type) },
};

static const uint8_t generated_default_276 = 0;
static const s_variable_target generated_targets_276[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_277 = 0;
static const s_variable_target generated_targets_277[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[6].specials, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[6].specials) },
};

static const int8_t generated_default_278 = 0;
static const s_variable_target generated_targets_278[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[6].ammo) },
};

static const int8_t generated_default_279 = 0;
static const s_variable_target generated_targets_279[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[6].count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_280 = 0;
static const s_variable_target generated_targets_280[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[7].type, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[7].type) },
};

static const uint8_t generated_default_281 = 0;
static const s_variable_target generated_targets_281[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_282 = 0;
static const s_variable_target generated_targets_282[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[7].specials, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[7].specials) },
};

static const int8_t generated_default_283 = 0;
static const s_variable_target generated_targets_283[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[7].ammo) },
};

static const int8_t generated_default_284 = 0;
static const s_variable_target generated_targets_284[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[7].count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::crystal_templates[1].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::crystal_templates[1].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_285[] = { 0, 0, 0, 0, 0 };
static const s_variable_target generated_targets_285[] = {
    { (void*)&SHIP_CONFIG::crystal_templates[1].special_device_flags, sizeof(SHIP_CONFIG::crystal_templates[1].special_device_flags) },
};

static const int8_t generated_default_286 = 22;
static const s_variable_target generated_targets_286[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].picture_num, sizeof(SHIP_CONFIG::dragon_templates[0].picture_num) },
};

static const uint8_t generated_default_287 = 2;
static const s_variable_target generated_targets_287[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].size, sizeof(SHIP_CONFIG::dragon_templates[0].size) },
};

static const uint8_t generated_default_288 = 0;
static const s_variable_target generated_targets_288[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ftl_type, sizeof(SHIP_CONFIG::dragon_templates[0].ftl_type) },
};

static const uint8_t generated_default_289 = 0;
static const s_variable_target generated_targets_289[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].armor_type, sizeof(SHIP_CONFIG::dragon_templates[0].armor_type) },
};

static const uint8_t generated_default_290 = 0;
static const s_variable_target generated_targets_290[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].shield_type, sizeof(SHIP_CONFIG::dragon_templates[0].shield_type) },
};

static const uint8_t generated_default_291 = 5;
static const s_variable_target generated_targets_291[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].computer_type, sizeof(SHIP_CONFIG::dragon_templates[0].computer_type) },
};

static const uint16_t generated_default_292 = 41;
static const s_variable_target generated_targets_292[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[0].type, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[0].type) },
};

static const uint8_t generated_default_293 = 15;
static const s_variable_target generated_targets_293[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_294 = 4;
static const s_variable_target generated_targets_294[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[0].specials, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[0].specials) },
};

static const int8_t generated_default_295 = 0;
static const s_variable_target generated_targets_295[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[0].ammo) },
};

static const int8_t generated_default_296 = 8;
static const s_variable_target generated_targets_296[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[0].count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_297 = 40;
static const s_variable_target generated_targets_297[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[1].type, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[1].type) },
};

static const uint8_t generated_default_298 = 15;
static const s_variable_target generated_targets_298[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_299 = 0;
static const s_variable_target generated_targets_299[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[1].specials, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[1].specials) },
};

static const int8_t generated_default_300 = 0;
static const s_variable_target generated_targets_300[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[1].ammo) },
};

static const int8_t generated_default_301 = 1;
static const s_variable_target generated_targets_301[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[1].count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_302 = 0;
static const s_variable_target generated_targets_302[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[2].type, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[2].type) },
};

static const uint8_t generated_default_303 = 0;
static const s_variable_target generated_targets_303[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_304 = 0;
static const s_variable_target generated_targets_304[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[2].specials, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[2].specials) },
};

static const int8_t generated_default_305 = 0;
static const s_variable_target generated_targets_305[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[2].ammo) },
};

static const int8_t generated_default_306 = 0;
static const s_variable_target generated_targets_306[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[2].count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_307 = 0;
static const s_variable_target generated_targets_307[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[3].type, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[3].type) },
};

static const uint8_t generated_default_308 = 0;
static const s_variable_target generated_targets_308[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_309 = 0;
static const s_variable_target generated_targets_309[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[3].specials, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[3].specials) },
};

static const int8_t generated_default_310 = 0;
static const s_variable_target generated_targets_310[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[3].ammo) },
};

static const int8_t generated_default_311 = 0;
static const s_variable_target generated_targets_311[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[3].count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_312 = 0;
static const s_variable_target generated_targets_312[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[4].type, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[4].type) },
};

static const uint8_t generated_default_313 = 0;
static const s_variable_target generated_targets_313[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_314 = 0;
static const s_variable_target generated_targets_314[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[4].specials, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[4].specials) },
};

static const int8_t generated_default_315 = 0;
static const s_variable_target generated_targets_315[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[4].ammo) },
};

static const int8_t generated_default_316 = 0;
static const s_variable_target generated_targets_316[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[4].count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_317 = 0;
static const s_variable_target generated_targets_317[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[5].type, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[5].type) },
};

static const uint8_t generated_default_318 = 0;
static const s_variable_target generated_targets_318[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_319 = 0;
static const s_variable_target generated_targets_319[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[5].specials, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[5].specials) },
};

static const int8_t generated_default_320 = 0;
static const s_variable_target generated_targets_320[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[5].ammo) },
};

static const int8_t generated_default_321 = 0;
static const s_variable_target generated_targets_321[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[5].count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_322 = 0;
static const s_variable_target generated_targets_322[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[6].type, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[6].type) },
};

static const uint8_t generated_default_323 = 0;
static const s_variable_target generated_targets_323[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_324 = 0;
static const s_variable_target generated_targets_324[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[6].specials, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[6].specials) },
};

static const int8_t generated_default_325 = 0;
static const s_variable_target generated_targets_325[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[6].ammo) },
};

static const int8_t generated_default_326 = 0;
static const s_variable_target generated_targets_326[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[6].count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_327 = 0;
static const s_variable_target generated_targets_327[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[7].type, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[7].type) },
};

static const uint8_t generated_default_328 = 0;
static const s_variable_target generated_targets_328[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_329 = 0;
static const s_variable_target generated_targets_329[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[7].specials, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[7].specials) },
};

static const int8_t generated_default_330 = 0;
static const s_variable_target generated_targets_330[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[7].ammo) },
};

static const int8_t generated_default_331 = 0;
static const s_variable_target generated_targets_331[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[7].count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[0].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::dragon_templates[0].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_332[] = { 0, 0, 0, 0, 0 };
static const s_variable_target generated_targets_332[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[0].special_device_flags, sizeof(SHIP_CONFIG::dragon_templates[0].special_device_flags) },
};

static const int8_t generated_default_333 = 10;
static const s_variable_target generated_targets_333[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].picture_num, sizeof(SHIP_CONFIG::dragon_templates[1].picture_num) },
};

static const uint8_t generated_default_334 = 4;
static const s_variable_target generated_targets_334[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].size, sizeof(SHIP_CONFIG::dragon_templates[1].size) },
};

static const uint8_t generated_default_335 = 6;
static const s_variable_target generated_targets_335[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ftl_type, sizeof(SHIP_CONFIG::dragon_templates[1].ftl_type) },
};

static const uint8_t generated_default_336 = 0;
static const s_variable_target generated_targets_336[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].armor_type, sizeof(SHIP_CONFIG::dragon_templates[1].armor_type) },
};

static const uint8_t generated_default_337 = 0;
static const s_variable_target generated_targets_337[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].shield_type, sizeof(SHIP_CONFIG::dragon_templates[1].shield_type) },
};

static const uint8_t generated_default_338 = 5;
static const s_variable_target generated_targets_338[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].computer_type, sizeof(SHIP_CONFIG::dragon_templates[1].computer_type) },
};

static const uint16_t generated_default_339 = 41;
static const s_variable_target generated_targets_339[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[0].type, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[0].type) },
};

static const uint8_t generated_default_340 = 15;
static const s_variable_target generated_targets_340[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_341 = 4;
static const s_variable_target generated_targets_341[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[0].specials, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[0].specials) },
};

static const int8_t generated_default_342 = 0;
static const s_variable_target generated_targets_342[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[0].ammo) },
};

static const int8_t generated_default_343 = 20;
static const s_variable_target generated_targets_343[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[0].count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_344 = 40;
static const s_variable_target generated_targets_344[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[1].type, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[1].type) },
};

static const uint8_t generated_default_345 = 15;
static const s_variable_target generated_targets_345[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_346 = 16384;
static const s_variable_target generated_targets_346[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[1].specials, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[1].specials) },
};

static const int8_t generated_default_347 = 0;
static const s_variable_target generated_targets_347[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[1].ammo) },
};

static const int8_t generated_default_348 = 1;
static const s_variable_target generated_targets_348[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[1].count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_349 = 0;
static const s_variable_target generated_targets_349[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[2].type, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[2].type) },
};

static const uint8_t generated_default_350 = 0;
static const s_variable_target generated_targets_350[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_351 = 0;
static const s_variable_target generated_targets_351[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[2].specials, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[2].specials) },
};

static const int8_t generated_default_352 = 0;
static const s_variable_target generated_targets_352[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[2].ammo) },
};

static const int8_t generated_default_353 = 0;
static const s_variable_target generated_targets_353[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[2].count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_354 = 0;
static const s_variable_target generated_targets_354[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[3].type, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[3].type) },
};

static const uint8_t generated_default_355 = 0;
static const s_variable_target generated_targets_355[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_356 = 0;
static const s_variable_target generated_targets_356[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[3].specials, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[3].specials) },
};

static const int8_t generated_default_357 = 0;
static const s_variable_target generated_targets_357[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[3].ammo) },
};

static const int8_t generated_default_358 = 0;
static const s_variable_target generated_targets_358[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[3].count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_359 = 0;
static const s_variable_target generated_targets_359[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[4].type, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[4].type) },
};

static const uint8_t generated_default_360 = 0;
static const s_variable_target generated_targets_360[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_361 = 0;
static const s_variable_target generated_targets_361[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[4].specials, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[4].specials) },
};

static const int8_t generated_default_362 = 0;
static const s_variable_target generated_targets_362[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[4].ammo) },
};

static const int8_t generated_default_363 = 0;
static const s_variable_target generated_targets_363[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[4].count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_364 = 0;
static const s_variable_target generated_targets_364[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[5].type, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[5].type) },
};

static const uint8_t generated_default_365 = 0;
static const s_variable_target generated_targets_365[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_366 = 0;
static const s_variable_target generated_targets_366[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[5].specials, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[5].specials) },
};

static const int8_t generated_default_367 = 0;
static const s_variable_target generated_targets_367[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[5].ammo) },
};

static const int8_t generated_default_368 = 0;
static const s_variable_target generated_targets_368[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[5].count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_369 = 0;
static const s_variable_target generated_targets_369[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[6].type, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[6].type) },
};

static const uint8_t generated_default_370 = 0;
static const s_variable_target generated_targets_370[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_371 = 0;
static const s_variable_target generated_targets_371[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[6].specials, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[6].specials) },
};

static const int8_t generated_default_372 = 0;
static const s_variable_target generated_targets_372[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[6].ammo) },
};

static const int8_t generated_default_373 = 0;
static const s_variable_target generated_targets_373[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[6].count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_374 = 0;
static const s_variable_target generated_targets_374[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[7].type, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[7].type) },
};

static const uint8_t generated_default_375 = 0;
static const s_variable_target generated_targets_375[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_376 = 0;
static const s_variable_target generated_targets_376[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[7].specials, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[7].specials) },
};

static const int8_t generated_default_377 = 0;
static const s_variable_target generated_targets_377[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[7].ammo) },
};

static const int8_t generated_default_378 = 0;
static const s_variable_target generated_targets_378[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[7].count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::dragon_templates[1].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::dragon_templates[1].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_379[] = { 0, 0, 0, 0, 0 };
static const s_variable_target generated_targets_379[] = {
    { (void*)&SHIP_CONFIG::dragon_templates[1].special_device_flags, sizeof(SHIP_CONFIG::dragon_templates[1].special_device_flags) },
};

static const int8_t generated_default_380 = 23;
static const s_variable_target generated_targets_380[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].picture_num, sizeof(SHIP_CONFIG::eel_templates[0].picture_num) },
};

static const uint8_t generated_default_381 = 2;
static const s_variable_target generated_targets_381[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].size, sizeof(SHIP_CONFIG::eel_templates[0].size) },
};

static const uint8_t generated_default_382 = 0;
static const s_variable_target generated_targets_382[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ftl_type, sizeof(SHIP_CONFIG::eel_templates[0].ftl_type) },
};

static const uint8_t generated_default_383 = 0;
static const s_variable_target generated_targets_383[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].armor_type, sizeof(SHIP_CONFIG::eel_templates[0].armor_type) },
};

static const uint8_t generated_default_384 = 0;
static const s_variable_target generated_targets_384[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].shield_type, sizeof(SHIP_CONFIG::eel_templates[0].shield_type) },
};

static const uint8_t generated_default_385 = 4;
static const s_variable_target generated_targets_385[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].computer_type, sizeof(SHIP_CONFIG::eel_templates[0].computer_type) },
};

static const uint16_t generated_default_386 = 44;
static const s_variable_target generated_targets_386[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[0].type, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[0].type) },
};

static const uint8_t generated_default_387 = 15;
static const s_variable_target generated_targets_387[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_388 = 0;
static const s_variable_target generated_targets_388[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[0].specials, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[0].specials) },
};

static const int8_t generated_default_389 = 0;
static const s_variable_target generated_targets_389[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[0].ammo) },
};

static const int8_t generated_default_390 = 1;
static const s_variable_target generated_targets_390[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[0].count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_391 = 0;
static const s_variable_target generated_targets_391[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[1].type, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[1].type) },
};

static const uint8_t generated_default_392 = 0;
static const s_variable_target generated_targets_392[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_393 = 0;
static const s_variable_target generated_targets_393[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[1].specials, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[1].specials) },
};

static const int8_t generated_default_394 = 0;
static const s_variable_target generated_targets_394[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[1].ammo) },
};

static const int8_t generated_default_395 = 0;
static const s_variable_target generated_targets_395[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[1].count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_396 = 0;
static const s_variable_target generated_targets_396[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[2].type, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[2].type) },
};

static const uint8_t generated_default_397 = 0;
static const s_variable_target generated_targets_397[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_398 = 0;
static const s_variable_target generated_targets_398[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[2].specials, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[2].specials) },
};

static const int8_t generated_default_399 = 0;
static const s_variable_target generated_targets_399[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[2].ammo) },
};

static const int8_t generated_default_400 = 0;
static const s_variable_target generated_targets_400[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[2].count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_401 = 0;
static const s_variable_target generated_targets_401[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[3].type, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[3].type) },
};

static const uint8_t generated_default_402 = 0;
static const s_variable_target generated_targets_402[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_403 = 0;
static const s_variable_target generated_targets_403[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[3].specials, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[3].specials) },
};

static const int8_t generated_default_404 = 0;
static const s_variable_target generated_targets_404[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[3].ammo) },
};

static const int8_t generated_default_405 = 0;
static const s_variable_target generated_targets_405[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[3].count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_406 = 0;
static const s_variable_target generated_targets_406[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[4].type, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[4].type) },
};

static const uint8_t generated_default_407 = 0;
static const s_variable_target generated_targets_407[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_408 = 0;
static const s_variable_target generated_targets_408[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[4].specials, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[4].specials) },
};

static const int8_t generated_default_409 = 0;
static const s_variable_target generated_targets_409[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[4].ammo) },
};

static const int8_t generated_default_410 = 0;
static const s_variable_target generated_targets_410[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[4].count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_411 = 0;
static const s_variable_target generated_targets_411[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[5].type, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[5].type) },
};

static const uint8_t generated_default_412 = 0;
static const s_variable_target generated_targets_412[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_413 = 0;
static const s_variable_target generated_targets_413[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[5].specials, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[5].specials) },
};

static const int8_t generated_default_414 = 0;
static const s_variable_target generated_targets_414[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[5].ammo) },
};

static const int8_t generated_default_415 = 0;
static const s_variable_target generated_targets_415[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[5].count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_416 = 0;
static const s_variable_target generated_targets_416[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[6].type, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[6].type) },
};

static const uint8_t generated_default_417 = 0;
static const s_variable_target generated_targets_417[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_418 = 0;
static const s_variable_target generated_targets_418[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[6].specials, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[6].specials) },
};

static const int8_t generated_default_419 = 0;
static const s_variable_target generated_targets_419[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[6].ammo) },
};

static const int8_t generated_default_420 = 0;
static const s_variable_target generated_targets_420[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[6].count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_421 = 0;
static const s_variable_target generated_targets_421[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[7].type, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[7].type) },
};

static const uint8_t generated_default_422 = 0;
static const s_variable_target generated_targets_422[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_423 = 0;
static const s_variable_target generated_targets_423[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[7].specials, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[7].specials) },
};

static const int8_t generated_default_424 = 0;
static const s_variable_target generated_targets_424[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[7].ammo) },
};

static const int8_t generated_default_425 = 0;
static const s_variable_target generated_targets_425[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[7].count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::eel_templates[0].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::eel_templates[0].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_426[] = { 0, 0, 8, 0, 0 };
static const s_variable_target generated_targets_426[] = {
    { (void*)&SHIP_CONFIG::eel_templates[0].special_device_flags, sizeof(SHIP_CONFIG::eel_templates[0].special_device_flags) },
};

static const int8_t generated_default_427 = 11;
static const s_variable_target generated_targets_427[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].picture_num, sizeof(SHIP_CONFIG::eel_templates[1].picture_num) },
};

static const uint8_t generated_default_428 = 3;
static const s_variable_target generated_targets_428[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].size, sizeof(SHIP_CONFIG::eel_templates[1].size) },
};

static const uint8_t generated_default_429 = 6;
static const s_variable_target generated_targets_429[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ftl_type, sizeof(SHIP_CONFIG::eel_templates[1].ftl_type) },
};

static const uint8_t generated_default_430 = 0;
static const s_variable_target generated_targets_430[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].armor_type, sizeof(SHIP_CONFIG::eel_templates[1].armor_type) },
};

static const uint8_t generated_default_431 = 0;
static const s_variable_target generated_targets_431[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].shield_type, sizeof(SHIP_CONFIG::eel_templates[1].shield_type) },
};

static const uint8_t generated_default_432 = 4;
static const s_variable_target generated_targets_432[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].computer_type, sizeof(SHIP_CONFIG::eel_templates[1].computer_type) },
};

static const uint16_t generated_default_433 = 44;
static const s_variable_target generated_targets_433[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[0].type, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[0].type) },
};

static const uint8_t generated_default_434 = 15;
static const s_variable_target generated_targets_434[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_435 = 0;
static const s_variable_target generated_targets_435[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[0].specials, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[0].specials) },
};

static const int8_t generated_default_436 = 0;
static const s_variable_target generated_targets_436[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[0].ammo) },
};

static const int8_t generated_default_437 = 2;
static const s_variable_target generated_targets_437[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[0].count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_438 = 0;
static const s_variable_target generated_targets_438[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[1].type, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[1].type) },
};

static const uint8_t generated_default_439 = 0;
static const s_variable_target generated_targets_439[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_440 = 0;
static const s_variable_target generated_targets_440[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[1].specials, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[1].specials) },
};

static const int8_t generated_default_441 = 0;
static const s_variable_target generated_targets_441[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[1].ammo) },
};

static const int8_t generated_default_442 = 0;
static const s_variable_target generated_targets_442[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[1].count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_443 = 0;
static const s_variable_target generated_targets_443[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[2].type, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[2].type) },
};

static const uint8_t generated_default_444 = 0;
static const s_variable_target generated_targets_444[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_445 = 0;
static const s_variable_target generated_targets_445[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[2].specials, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[2].specials) },
};

static const int8_t generated_default_446 = 0;
static const s_variable_target generated_targets_446[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[2].ammo) },
};

static const int8_t generated_default_447 = 0;
static const s_variable_target generated_targets_447[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[2].count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_448 = 0;
static const s_variable_target generated_targets_448[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[3].type, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[3].type) },
};

static const uint8_t generated_default_449 = 0;
static const s_variable_target generated_targets_449[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_450 = 0;
static const s_variable_target generated_targets_450[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[3].specials, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[3].specials) },
};

static const int8_t generated_default_451 = 0;
static const s_variable_target generated_targets_451[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[3].ammo) },
};

static const int8_t generated_default_452 = 0;
static const s_variable_target generated_targets_452[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[3].count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_453 = 0;
static const s_variable_target generated_targets_453[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[4].type, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[4].type) },
};

static const uint8_t generated_default_454 = 0;
static const s_variable_target generated_targets_454[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_455 = 0;
static const s_variable_target generated_targets_455[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[4].specials, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[4].specials) },
};

static const int8_t generated_default_456 = 0;
static const s_variable_target generated_targets_456[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[4].ammo) },
};

static const int8_t generated_default_457 = 0;
static const s_variable_target generated_targets_457[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[4].count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_458 = 0;
static const s_variable_target generated_targets_458[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[5].type, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[5].type) },
};

static const uint8_t generated_default_459 = 0;
static const s_variable_target generated_targets_459[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_460 = 0;
static const s_variable_target generated_targets_460[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[5].specials, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[5].specials) },
};

static const int8_t generated_default_461 = 0;
static const s_variable_target generated_targets_461[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[5].ammo) },
};

static const int8_t generated_default_462 = 0;
static const s_variable_target generated_targets_462[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[5].count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_463 = 0;
static const s_variable_target generated_targets_463[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[6].type, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[6].type) },
};

static const uint8_t generated_default_464 = 0;
static const s_variable_target generated_targets_464[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_465 = 0;
static const s_variable_target generated_targets_465[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[6].specials, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[6].specials) },
};

static const int8_t generated_default_466 = 0;
static const s_variable_target generated_targets_466[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[6].ammo) },
};

static const int8_t generated_default_467 = 0;
static const s_variable_target generated_targets_467[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[6].count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_468 = 0;
static const s_variable_target generated_targets_468[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[7].type, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[7].type) },
};

static const uint8_t generated_default_469 = 0;
static const s_variable_target generated_targets_469[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_470 = 0;
static const s_variable_target generated_targets_470[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[7].specials, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[7].specials) },
};

static const int8_t generated_default_471 = 0;
static const s_variable_target generated_targets_471[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[7].ammo) },
};

static const int8_t generated_default_472 = 0;
static const s_variable_target generated_targets_472[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[7].count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::eel_templates[1].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::eel_templates[1].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_473[] = { 0, 0, 8, 0, 0 };
static const s_variable_target generated_targets_473[] = {
    { (void*)&SHIP_CONFIG::eel_templates[1].special_device_flags, sizeof(SHIP_CONFIG::eel_templates[1].special_device_flags) },
};

static const int8_t generated_default_474 = 24;
static const s_variable_target generated_targets_474[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].picture_num, sizeof(SHIP_CONFIG::hydra_templates[0].picture_num) },
};

static const uint8_t generated_default_475 = 2;
static const s_variable_target generated_targets_475[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].size, sizeof(SHIP_CONFIG::hydra_templates[0].size) },
};

static const uint8_t generated_default_476 = 0;
static const s_variable_target generated_targets_476[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ftl_type, sizeof(SHIP_CONFIG::hydra_templates[0].ftl_type) },
};

static const uint8_t generated_default_477 = 0;
static const s_variable_target generated_targets_477[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].armor_type, sizeof(SHIP_CONFIG::hydra_templates[0].armor_type) },
};

static const uint8_t generated_default_478 = 0;
static const s_variable_target generated_targets_478[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].shield_type, sizeof(SHIP_CONFIG::hydra_templates[0].shield_type) },
};

static const uint8_t generated_default_479 = 2;
static const s_variable_target generated_targets_479[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].computer_type, sizeof(SHIP_CONFIG::hydra_templates[0].computer_type) },
};

static const uint16_t generated_default_480 = 43;
static const s_variable_target generated_targets_480[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[0].type, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[0].type) },
};

static const uint8_t generated_default_481 = 15;
static const s_variable_target generated_targets_481[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_482 = 0;
static const s_variable_target generated_targets_482[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[0].specials, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[0].specials) },
};

static const int8_t generated_default_483 = 0;
static const s_variable_target generated_targets_483[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[0].ammo) },
};

static const int8_t generated_default_484 = 3;
static const s_variable_target generated_targets_484[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[0].count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_485 = 0;
static const s_variable_target generated_targets_485[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[1].type, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[1].type) },
};

static const uint8_t generated_default_486 = 0;
static const s_variable_target generated_targets_486[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_487 = 0;
static const s_variable_target generated_targets_487[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[1].specials, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[1].specials) },
};

static const int8_t generated_default_488 = 0;
static const s_variable_target generated_targets_488[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[1].ammo) },
};

static const int8_t generated_default_489 = 0;
static const s_variable_target generated_targets_489[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[1].count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_490 = 0;
static const s_variable_target generated_targets_490[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[2].type, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[2].type) },
};

static const uint8_t generated_default_491 = 0;
static const s_variable_target generated_targets_491[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_492 = 0;
static const s_variable_target generated_targets_492[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[2].specials, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[2].specials) },
};

static const int8_t generated_default_493 = 0;
static const s_variable_target generated_targets_493[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[2].ammo) },
};

static const int8_t generated_default_494 = 0;
static const s_variable_target generated_targets_494[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[2].count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_495 = 0;
static const s_variable_target generated_targets_495[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[3].type, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[3].type) },
};

static const uint8_t generated_default_496 = 0;
static const s_variable_target generated_targets_496[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_497 = 0;
static const s_variable_target generated_targets_497[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[3].specials, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[3].specials) },
};

static const int8_t generated_default_498 = 0;
static const s_variable_target generated_targets_498[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[3].ammo) },
};

static const int8_t generated_default_499 = 0;
static const s_variable_target generated_targets_499[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[3].count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_500 = 0;
static const s_variable_target generated_targets_500[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[4].type, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[4].type) },
};

static const uint8_t generated_default_501 = 0;
static const s_variable_target generated_targets_501[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_502 = 0;
static const s_variable_target generated_targets_502[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[4].specials, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[4].specials) },
};

static const int8_t generated_default_503 = 0;
static const s_variable_target generated_targets_503[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[4].ammo) },
};

static const int8_t generated_default_504 = 0;
static const s_variable_target generated_targets_504[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[4].count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_505 = 0;
static const s_variable_target generated_targets_505[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[5].type, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[5].type) },
};

static const uint8_t generated_default_506 = 0;
static const s_variable_target generated_targets_506[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_507 = 0;
static const s_variable_target generated_targets_507[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[5].specials, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[5].specials) },
};

static const int8_t generated_default_508 = 0;
static const s_variable_target generated_targets_508[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[5].ammo) },
};

static const int8_t generated_default_509 = 0;
static const s_variable_target generated_targets_509[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[5].count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_510 = 0;
static const s_variable_target generated_targets_510[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[6].type, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[6].type) },
};

static const uint8_t generated_default_511 = 0;
static const s_variable_target generated_targets_511[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_512 = 0;
static const s_variable_target generated_targets_512[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[6].specials, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[6].specials) },
};

static const int8_t generated_default_513 = 0;
static const s_variable_target generated_targets_513[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[6].ammo) },
};

static const int8_t generated_default_514 = 0;
static const s_variable_target generated_targets_514[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[6].count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_515 = 0;
static const s_variable_target generated_targets_515[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[7].type, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[7].type) },
};

static const uint8_t generated_default_516 = 0;
static const s_variable_target generated_targets_516[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_517 = 0;
static const s_variable_target generated_targets_517[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[7].specials, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[7].specials) },
};

static const int8_t generated_default_518 = 0;
static const s_variable_target generated_targets_518[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[7].ammo) },
};

static const int8_t generated_default_519 = 0;
static const s_variable_target generated_targets_519[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[7].count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[0].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::hydra_templates[0].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_520[] = { 0, 4, 0, 0, 0 };
static const s_variable_target generated_targets_520[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[0].special_device_flags, sizeof(SHIP_CONFIG::hydra_templates[0].special_device_flags) },
};

static const int8_t generated_default_521 = 12;
static const s_variable_target generated_targets_521[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].picture_num, sizeof(SHIP_CONFIG::hydra_templates[1].picture_num) },
};

static const uint8_t generated_default_522 = 4;
static const s_variable_target generated_targets_522[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].size, sizeof(SHIP_CONFIG::hydra_templates[1].size) },
};

static const uint8_t generated_default_523 = 2;
static const s_variable_target generated_targets_523[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ftl_type, sizeof(SHIP_CONFIG::hydra_templates[1].ftl_type) },
};

static const uint8_t generated_default_524 = 0;
static const s_variable_target generated_targets_524[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].armor_type, sizeof(SHIP_CONFIG::hydra_templates[1].armor_type) },
};

static const uint8_t generated_default_525 = 0;
static const s_variable_target generated_targets_525[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].shield_type, sizeof(SHIP_CONFIG::hydra_templates[1].shield_type) },
};

static const uint8_t generated_default_526 = 2;
static const s_variable_target generated_targets_526[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].computer_type, sizeof(SHIP_CONFIG::hydra_templates[1].computer_type) },
};

static const uint16_t generated_default_527 = 43;
static const s_variable_target generated_targets_527[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[0].type, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[0].type) },
};

static const uint8_t generated_default_528 = 15;
static const s_variable_target generated_targets_528[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_529 = 2;
static const s_variable_target generated_targets_529[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[0].specials, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[0].specials) },
};

static const int8_t generated_default_530 = 0;
static const s_variable_target generated_targets_530[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[0].ammo) },
};

static const int8_t generated_default_531 = 5;
static const s_variable_target generated_targets_531[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[0].count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_532 = 0;
static const s_variable_target generated_targets_532[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[1].type, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[1].type) },
};

static const uint8_t generated_default_533 = 0;
static const s_variable_target generated_targets_533[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_534 = 0;
static const s_variable_target generated_targets_534[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[1].specials, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[1].specials) },
};

static const int8_t generated_default_535 = 0;
static const s_variable_target generated_targets_535[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[1].ammo) },
};

static const int8_t generated_default_536 = 0;
static const s_variable_target generated_targets_536[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[1].count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_537 = 0;
static const s_variable_target generated_targets_537[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[2].type, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[2].type) },
};

static const uint8_t generated_default_538 = 0;
static const s_variable_target generated_targets_538[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_539 = 0;
static const s_variable_target generated_targets_539[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[2].specials, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[2].specials) },
};

static const int8_t generated_default_540 = 0;
static const s_variable_target generated_targets_540[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[2].ammo) },
};

static const int8_t generated_default_541 = 0;
static const s_variable_target generated_targets_541[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[2].count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_542 = 0;
static const s_variable_target generated_targets_542[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[3].type, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[3].type) },
};

static const uint8_t generated_default_543 = 0;
static const s_variable_target generated_targets_543[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_544 = 0;
static const s_variable_target generated_targets_544[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[3].specials, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[3].specials) },
};

static const int8_t generated_default_545 = 0;
static const s_variable_target generated_targets_545[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[3].ammo) },
};

static const int8_t generated_default_546 = 0;
static const s_variable_target generated_targets_546[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[3].count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_547 = 0;
static const s_variable_target generated_targets_547[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[4].type, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[4].type) },
};

static const uint8_t generated_default_548 = 0;
static const s_variable_target generated_targets_548[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_549 = 0;
static const s_variable_target generated_targets_549[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[4].specials, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[4].specials) },
};

static const int8_t generated_default_550 = 0;
static const s_variable_target generated_targets_550[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[4].ammo) },
};

static const int8_t generated_default_551 = 0;
static const s_variable_target generated_targets_551[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[4].count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_552 = 0;
static const s_variable_target generated_targets_552[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[5].type, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[5].type) },
};

static const uint8_t generated_default_553 = 0;
static const s_variable_target generated_targets_553[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_554 = 0;
static const s_variable_target generated_targets_554[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[5].specials, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[5].specials) },
};

static const int8_t generated_default_555 = 0;
static const s_variable_target generated_targets_555[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[5].ammo) },
};

static const int8_t generated_default_556 = 0;
static const s_variable_target generated_targets_556[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[5].count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_557 = 0;
static const s_variable_target generated_targets_557[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[6].type, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[6].type) },
};

static const uint8_t generated_default_558 = 0;
static const s_variable_target generated_targets_558[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_559 = 0;
static const s_variable_target generated_targets_559[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[6].specials, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[6].specials) },
};

static const int8_t generated_default_560 = 0;
static const s_variable_target generated_targets_560[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[6].ammo) },
};

static const int8_t generated_default_561 = 0;
static const s_variable_target generated_targets_561[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[6].count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_562 = 0;
static const s_variable_target generated_targets_562[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[7].type, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[7].type) },
};

static const uint8_t generated_default_563 = 0;
static const s_variable_target generated_targets_563[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_564 = 0;
static const s_variable_target generated_targets_564[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[7].specials, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[7].specials) },
};

static const int8_t generated_default_565 = 0;
static const s_variable_target generated_targets_565[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[7].ammo) },
};

static const int8_t generated_default_566 = 0;
static const s_variable_target generated_targets_566[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[7].count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::hydra_templates[1].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::hydra_templates[1].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_567[] = { 0, 4, 0, 0, 0 };
static const s_variable_target generated_targets_567[] = {
    { (void*)&SHIP_CONFIG::hydra_templates[1].special_device_flags, sizeof(SHIP_CONFIG::hydra_templates[1].special_device_flags) },
};

static const int8_t generated_default_568 = 0;
static const s_variable_target generated_targets_568[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].picture_num, sizeof(SHIP_CONFIG::antaran_templates[0][0].picture_num) },
};

static const uint8_t generated_default_569 = 0;
static const s_variable_target generated_targets_569[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].size, sizeof(SHIP_CONFIG::antaran_templates[0][0].size) },
};

static const uint8_t generated_default_570 = 5;
static const s_variable_target generated_targets_570[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[0][0].ftl_type) },
};

static const uint8_t generated_default_571 = 6;
static const s_variable_target generated_targets_571[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].armor_type, sizeof(SHIP_CONFIG::antaran_templates[0][0].armor_type) },
};

static const uint8_t generated_default_572 = 0;
static const s_variable_target generated_targets_572[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].shield_type, sizeof(SHIP_CONFIG::antaran_templates[0][0].shield_type) },
};

static const uint8_t generated_default_573 = 5;
static const s_variable_target generated_targets_573[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].computer_type, sizeof(SHIP_CONFIG::antaran_templates[0][0].computer_type) },
};

static const uint16_t generated_default_574 = 4;
static const s_variable_target generated_targets_574[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[0].type) },
};

static const uint8_t generated_default_575 = 1;
static const s_variable_target generated_targets_575[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_576 = 0;
static const s_variable_target generated_targets_576[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[0].specials) },
};

static const int8_t generated_default_577 = 0;
static const s_variable_target generated_targets_577[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[0].ammo) },
};

static const int8_t generated_default_578 = 2;
static const s_variable_target generated_targets_578[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_579 = 24;
static const s_variable_target generated_targets_579[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[1].type) },
};

static const uint8_t generated_default_580 = 15;
static const s_variable_target generated_targets_580[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_581 = 0;
static const s_variable_target generated_targets_581[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[1].specials) },
};

static const int8_t generated_default_582 = 0;
static const s_variable_target generated_targets_582[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[1].ammo) },
};

static const int8_t generated_default_583 = 1;
static const s_variable_target generated_targets_583[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_584 = 0;
static const s_variable_target generated_targets_584[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[2].type) },
};

static const uint8_t generated_default_585 = 0;
static const s_variable_target generated_targets_585[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_586 = 0;
static const s_variable_target generated_targets_586[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[2].specials) },
};

static const int8_t generated_default_587 = 0;
static const s_variable_target generated_targets_587[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[2].ammo) },
};

static const int8_t generated_default_588 = 0;
static const s_variable_target generated_targets_588[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_589 = 0;
static const s_variable_target generated_targets_589[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[3].type) },
};

static const uint8_t generated_default_590 = 0;
static const s_variable_target generated_targets_590[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_591 = 0;
static const s_variable_target generated_targets_591[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[3].specials) },
};

static const int8_t generated_default_592 = 0;
static const s_variable_target generated_targets_592[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[3].ammo) },
};

static const int8_t generated_default_593 = 0;
static const s_variable_target generated_targets_593[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_594 = 0;
static const s_variable_target generated_targets_594[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[4].type) },
};

static const uint8_t generated_default_595 = 0;
static const s_variable_target generated_targets_595[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_596 = 0;
static const s_variable_target generated_targets_596[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[4].specials) },
};

static const int8_t generated_default_597 = 0;
static const s_variable_target generated_targets_597[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[4].ammo) },
};

static const int8_t generated_default_598 = 0;
static const s_variable_target generated_targets_598[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_599 = 0;
static const s_variable_target generated_targets_599[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[5].type) },
};

static const uint8_t generated_default_600 = 0;
static const s_variable_target generated_targets_600[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_601 = 0;
static const s_variable_target generated_targets_601[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[5].specials) },
};

static const int8_t generated_default_602 = 0;
static const s_variable_target generated_targets_602[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[5].ammo) },
};

static const int8_t generated_default_603 = 0;
static const s_variable_target generated_targets_603[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_604 = 0;
static const s_variable_target generated_targets_604[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[6].type) },
};

static const uint8_t generated_default_605 = 0;
static const s_variable_target generated_targets_605[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_606 = 0;
static const s_variable_target generated_targets_606[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[6].specials) },
};

static const int8_t generated_default_607 = 0;
static const s_variable_target generated_targets_607[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[6].ammo) },
};

static const int8_t generated_default_608 = 0;
static const s_variable_target generated_targets_608[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_609 = 0;
static const s_variable_target generated_targets_609[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[7].type) },
};

static const uint8_t generated_default_610 = 0;
static const s_variable_target generated_targets_610[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_611 = 0;
static const s_variable_target generated_targets_611[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[7].specials) },
};

static const int8_t generated_default_612 = 0;
static const s_variable_target generated_targets_612[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[7].ammo) },
};

static const int8_t generated_default_613 = 0;
static const s_variable_target generated_targets_613[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][0].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_614[] = { 128, 0, 0, 1, 0 };
static const s_variable_target generated_targets_614[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][0].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[0][0].special_device_flags) },
};

static const int8_t generated_default_615 = 1;
static const s_variable_target generated_targets_615[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].picture_num, sizeof(SHIP_CONFIG::antaran_templates[0][1].picture_num) },
};

static const uint8_t generated_default_616 = 1;
static const s_variable_target generated_targets_616[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].size, sizeof(SHIP_CONFIG::antaran_templates[0][1].size) },
};

static const uint8_t generated_default_617 = 5;
static const s_variable_target generated_targets_617[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[0][1].ftl_type) },
};

static const uint8_t generated_default_618 = 6;
static const s_variable_target generated_targets_618[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].armor_type, sizeof(SHIP_CONFIG::antaran_templates[0][1].armor_type) },
};

static const uint8_t generated_default_619 = 0;
static const s_variable_target generated_targets_619[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].shield_type, sizeof(SHIP_CONFIG::antaran_templates[0][1].shield_type) },
};

static const uint8_t generated_default_620 = 5;
static const s_variable_target generated_targets_620[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].computer_type, sizeof(SHIP_CONFIG::antaran_templates[0][1].computer_type) },
};

static const uint16_t generated_default_621 = 4;
static const s_variable_target generated_targets_621[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[0].type) },
};

static const uint8_t generated_default_622 = 1;
static const s_variable_target generated_targets_622[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_623 = 0;
static const s_variable_target generated_targets_623[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[0].specials) },
};

static const int8_t generated_default_624 = 0;
static const s_variable_target generated_targets_624[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[0].ammo) },
};

static const int8_t generated_default_625 = 3;
static const s_variable_target generated_targets_625[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_626 = 4;
static const s_variable_target generated_targets_626[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[1].type) },
};

static const uint8_t generated_default_627 = 1;
static const s_variable_target generated_targets_627[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_628 = 2;
static const s_variable_target generated_targets_628[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[1].specials) },
};

static const int8_t generated_default_629 = 0;
static const s_variable_target generated_targets_629[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[1].ammo) },
};

static const int8_t generated_default_630 = 1;
static const s_variable_target generated_targets_630[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_631 = 24;
static const s_variable_target generated_targets_631[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[2].type) },
};

static const uint8_t generated_default_632 = 15;
static const s_variable_target generated_targets_632[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_633 = 0;
static const s_variable_target generated_targets_633[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[2].specials) },
};

static const int8_t generated_default_634 = 10;
static const s_variable_target generated_targets_634[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[2].ammo) },
};

static const int8_t generated_default_635 = 1;
static const s_variable_target generated_targets_635[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_636 = 0;
static const s_variable_target generated_targets_636[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[3].type) },
};

static const uint8_t generated_default_637 = 0;
static const s_variable_target generated_targets_637[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_638 = 0;
static const s_variable_target generated_targets_638[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[3].specials) },
};

static const int8_t generated_default_639 = 0;
static const s_variable_target generated_targets_639[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[3].ammo) },
};

static const int8_t generated_default_640 = 0;
static const s_variable_target generated_targets_640[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_641 = 0;
static const s_variable_target generated_targets_641[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[4].type) },
};

static const uint8_t generated_default_642 = 0;
static const s_variable_target generated_targets_642[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_643 = 0;
static const s_variable_target generated_targets_643[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[4].specials) },
};

static const int8_t generated_default_644 = 0;
static const s_variable_target generated_targets_644[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[4].ammo) },
};

static const int8_t generated_default_645 = 0;
static const s_variable_target generated_targets_645[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_646 = 0;
static const s_variable_target generated_targets_646[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[5].type) },
};

static const uint8_t generated_default_647 = 0;
static const s_variable_target generated_targets_647[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_648 = 0;
static const s_variable_target generated_targets_648[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[5].specials) },
};

static const int8_t generated_default_649 = 0;
static const s_variable_target generated_targets_649[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[5].ammo) },
};

static const int8_t generated_default_650 = 0;
static const s_variable_target generated_targets_650[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_651 = 0;
static const s_variable_target generated_targets_651[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[6].type) },
};

static const uint8_t generated_default_652 = 0;
static const s_variable_target generated_targets_652[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_653 = 0;
static const s_variable_target generated_targets_653[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[6].specials) },
};

static const int8_t generated_default_654 = 0;
static const s_variable_target generated_targets_654[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[6].ammo) },
};

static const int8_t generated_default_655 = 0;
static const s_variable_target generated_targets_655[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_656 = 0;
static const s_variable_target generated_targets_656[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[7].type) },
};

static const uint8_t generated_default_657 = 0;
static const s_variable_target generated_targets_657[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_658 = 0;
static const s_variable_target generated_targets_658[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[7].specials) },
};

static const int8_t generated_default_659 = 0;
static const s_variable_target generated_targets_659[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[7].ammo) },
};

static const int8_t generated_default_660 = 0;
static const s_variable_target generated_targets_660[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][1].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_661[] = { 128, 0, 0, 1, 0 };
static const s_variable_target generated_targets_661[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][1].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[0][1].special_device_flags) },
};

static const int8_t generated_default_662 = 2;
static const s_variable_target generated_targets_662[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].picture_num, sizeof(SHIP_CONFIG::antaran_templates[0][2].picture_num) },
};

static const uint8_t generated_default_663 = 2;
static const s_variable_target generated_targets_663[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].size, sizeof(SHIP_CONFIG::antaran_templates[0][2].size) },
};

static const uint8_t generated_default_664 = 5;
static const s_variable_target generated_targets_664[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[0][2].ftl_type) },
};

static const uint8_t generated_default_665 = 6;
static const s_variable_target generated_targets_665[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].armor_type, sizeof(SHIP_CONFIG::antaran_templates[0][2].armor_type) },
};

static const uint8_t generated_default_666 = 0;
static const s_variable_target generated_targets_666[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].shield_type, sizeof(SHIP_CONFIG::antaran_templates[0][2].shield_type) },
};

static const uint8_t generated_default_667 = 5;
static const s_variable_target generated_targets_667[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].computer_type, sizeof(SHIP_CONFIG::antaran_templates[0][2].computer_type) },
};

static const uint16_t generated_default_668 = 4;
static const s_variable_target generated_targets_668[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[0].type) },
};

static const uint8_t generated_default_669 = 1;
static const s_variable_target generated_targets_669[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_670 = 0;
static const s_variable_target generated_targets_670[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[0].specials) },
};

static const int8_t generated_default_671 = 0;
static const s_variable_target generated_targets_671[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[0].ammo) },
};

static const int8_t generated_default_672 = 4;
static const s_variable_target generated_targets_672[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_673 = 4;
static const s_variable_target generated_targets_673[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[1].type) },
};

static const uint8_t generated_default_674 = 1;
static const s_variable_target generated_targets_674[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_675 = 2;
static const s_variable_target generated_targets_675[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[1].specials) },
};

static const int8_t generated_default_676 = 0;
static const s_variable_target generated_targets_676[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[1].ammo) },
};

static const int8_t generated_default_677 = 2;
static const s_variable_target generated_targets_677[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_678 = 24;
static const s_variable_target generated_targets_678[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[2].type) },
};

static const uint8_t generated_default_679 = 15;
static const s_variable_target generated_targets_679[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_680 = 0;
static const s_variable_target generated_targets_680[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[2].specials) },
};

static const int8_t generated_default_681 = 10;
static const s_variable_target generated_targets_681[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[2].ammo) },
};

static const int8_t generated_default_682 = 5;
static const s_variable_target generated_targets_682[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_683 = 13;
static const s_variable_target generated_targets_683[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[3].type) },
};

static const uint8_t generated_default_684 = 15;
static const s_variable_target generated_targets_684[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_685 = 0;
static const s_variable_target generated_targets_685[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[3].specials) },
};

static const int8_t generated_default_686 = 0;
static const s_variable_target generated_targets_686[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[3].ammo) },
};

static const int8_t generated_default_687 = 2;
static const s_variable_target generated_targets_687[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_688 = 31;
static const s_variable_target generated_targets_688[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[4].type) },
};

static const uint8_t generated_default_689 = 15;
static const s_variable_target generated_targets_689[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_690 = 0;
static const s_variable_target generated_targets_690[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[4].specials) },
};

static const int8_t generated_default_691 = 0;
static const s_variable_target generated_targets_691[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[4].ammo) },
};

static const int8_t generated_default_692 = 3;
static const s_variable_target generated_targets_692[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_693 = 0;
static const s_variable_target generated_targets_693[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[5].type) },
};

static const uint8_t generated_default_694 = 0;
static const s_variable_target generated_targets_694[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_695 = 0;
static const s_variable_target generated_targets_695[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[5].specials) },
};

static const int8_t generated_default_696 = 0;
static const s_variable_target generated_targets_696[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[5].ammo) },
};

static const int8_t generated_default_697 = 0;
static const s_variable_target generated_targets_697[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_698 = 0;
static const s_variable_target generated_targets_698[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[6].type) },
};

static const uint8_t generated_default_699 = 0;
static const s_variable_target generated_targets_699[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_700 = 0;
static const s_variable_target generated_targets_700[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[6].specials) },
};

static const int8_t generated_default_701 = 0;
static const s_variable_target generated_targets_701[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[6].ammo) },
};

static const int8_t generated_default_702 = 0;
static const s_variable_target generated_targets_702[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_703 = 0;
static const s_variable_target generated_targets_703[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[7].type) },
};

static const uint8_t generated_default_704 = 0;
static const s_variable_target generated_targets_704[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_705 = 0;
static const s_variable_target generated_targets_705[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[7].specials) },
};

static const int8_t generated_default_706 = 0;
static const s_variable_target generated_targets_706[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[7].ammo) },
};

static const int8_t generated_default_707 = 0;
static const s_variable_target generated_targets_707[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][2].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_708[] = { 128, 0, 0, 1, 0 };
static const s_variable_target generated_targets_708[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][2].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[0][2].special_device_flags) },
};

static const int8_t generated_default_709 = 3;
static const s_variable_target generated_targets_709[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].picture_num, sizeof(SHIP_CONFIG::antaran_templates[0][3].picture_num) },
};

static const uint8_t generated_default_710 = 3;
static const s_variable_target generated_targets_710[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].size, sizeof(SHIP_CONFIG::antaran_templates[0][3].size) },
};

static const uint8_t generated_default_711 = 5;
static const s_variable_target generated_targets_711[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[0][3].ftl_type) },
};

static const uint8_t generated_default_712 = 6;
static const s_variable_target generated_targets_712[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].armor_type, sizeof(SHIP_CONFIG::antaran_templates[0][3].armor_type) },
};

static const uint8_t generated_default_713 = 0;
static const s_variable_target generated_targets_713[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].shield_type, sizeof(SHIP_CONFIG::antaran_templates[0][3].shield_type) },
};

static const uint8_t generated_default_714 = 5;
static const s_variable_target generated_targets_714[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].computer_type, sizeof(SHIP_CONFIG::antaran_templates[0][3].computer_type) },
};

static const uint16_t generated_default_715 = 4;
static const s_variable_target generated_targets_715[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[0].type) },
};

static const uint8_t generated_default_716 = 1;
static const s_variable_target generated_targets_716[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_717 = 0;
static const s_variable_target generated_targets_717[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[0].specials) },
};

static const int8_t generated_default_718 = 0;
static const s_variable_target generated_targets_718[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[0].ammo) },
};

static const int8_t generated_default_719 = 10;
static const s_variable_target generated_targets_719[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_720 = 4;
static const s_variable_target generated_targets_720[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[1].type) },
};

static const uint8_t generated_default_721 = 1;
static const s_variable_target generated_targets_721[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_722 = 2;
static const s_variable_target generated_targets_722[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[1].specials) },
};

static const int8_t generated_default_723 = 0;
static const s_variable_target generated_targets_723[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[1].ammo) },
};

static const int8_t generated_default_724 = 4;
static const s_variable_target generated_targets_724[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_725 = 24;
static const s_variable_target generated_targets_725[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[2].type) },
};

static const uint8_t generated_default_726 = 1;
static const s_variable_target generated_targets_726[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_727 = 0;
static const s_variable_target generated_targets_727[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[2].specials) },
};

static const int8_t generated_default_728 = 10;
static const s_variable_target generated_targets_728[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[2].ammo) },
};

static const int8_t generated_default_729 = 15;
static const s_variable_target generated_targets_729[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_730 = 13;
static const s_variable_target generated_targets_730[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[3].type) },
};

static const uint8_t generated_default_731 = 15;
static const s_variable_target generated_targets_731[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_732 = 0;
static const s_variable_target generated_targets_732[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[3].specials) },
};

static const int8_t generated_default_733 = 0;
static const s_variable_target generated_targets_733[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[3].ammo) },
};

static const int8_t generated_default_734 = 2;
static const s_variable_target generated_targets_734[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_735 = 4;
static const s_variable_target generated_targets_735[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[4].type) },
};

static const uint8_t generated_default_736 = 1;
static const s_variable_target generated_targets_736[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_737 = 4;
static const s_variable_target generated_targets_737[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[4].specials) },
};

static const int8_t generated_default_738 = 0;
static const s_variable_target generated_targets_738[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[4].ammo) },
};

static const int8_t generated_default_739 = 10;
static const s_variable_target generated_targets_739[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_740 = 11;
static const s_variable_target generated_targets_740[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[5].type) },
};

static const uint8_t generated_default_741 = 1;
static const s_variable_target generated_targets_741[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_742 = 0;
static const s_variable_target generated_targets_742[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[5].specials) },
};

static const int8_t generated_default_743 = 0;
static const s_variable_target generated_targets_743[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[5].ammo) },
};

static const int8_t generated_default_744 = 4;
static const s_variable_target generated_targets_744[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_745 = 0;
static const s_variable_target generated_targets_745[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[6].type) },
};

static const uint8_t generated_default_746 = 0;
static const s_variable_target generated_targets_746[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_747 = 0;
static const s_variable_target generated_targets_747[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[6].specials) },
};

static const int8_t generated_default_748 = 0;
static const s_variable_target generated_targets_748[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[6].ammo) },
};

static const int8_t generated_default_749 = 0;
static const s_variable_target generated_targets_749[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_750 = 0;
static const s_variable_target generated_targets_750[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[7].type) },
};

static const uint8_t generated_default_751 = 0;
static const s_variable_target generated_targets_751[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_752 = 0;
static const s_variable_target generated_targets_752[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[7].specials) },
};

static const int8_t generated_default_753 = 0;
static const s_variable_target generated_targets_753[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[7].ammo) },
};

static const int8_t generated_default_754 = 0;
static const s_variable_target generated_targets_754[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][3].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_755[] = { 128, 0, 0, 5, 2 };
static const s_variable_target generated_targets_755[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][3].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[0][3].special_device_flags) },
};

static const int8_t generated_default_756 = 4;
static const s_variable_target generated_targets_756[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].picture_num, sizeof(SHIP_CONFIG::antaran_templates[0][4].picture_num) },
};

static const uint8_t generated_default_757 = 4;
static const s_variable_target generated_targets_757[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].size, sizeof(SHIP_CONFIG::antaran_templates[0][4].size) },
};

static const uint8_t generated_default_758 = 5;
static const s_variable_target generated_targets_758[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[0][4].ftl_type) },
};

static const uint8_t generated_default_759 = 6;
static const s_variable_target generated_targets_759[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].armor_type, sizeof(SHIP_CONFIG::antaran_templates[0][4].armor_type) },
};

static const uint8_t generated_default_760 = 0;
static const s_variable_target generated_targets_760[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].shield_type, sizeof(SHIP_CONFIG::antaran_templates[0][4].shield_type) },
};

static const uint8_t generated_default_761 = 5;
static const s_variable_target generated_targets_761[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].computer_type, sizeof(SHIP_CONFIG::antaran_templates[0][4].computer_type) },
};

static const uint16_t generated_default_762 = 4;
static const s_variable_target generated_targets_762[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[0].type) },
};

static const uint8_t generated_default_763 = 1;
static const s_variable_target generated_targets_763[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_764 = 0;
static const s_variable_target generated_targets_764[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[0].specials) },
};

static const int8_t generated_default_765 = 0;
static const s_variable_target generated_targets_765[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[0].ammo) },
};

static const int8_t generated_default_766 = 20;
static const s_variable_target generated_targets_766[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_767 = 4;
static const s_variable_target generated_targets_767[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[1].type) },
};

static const uint8_t generated_default_768 = 1;
static const s_variable_target generated_targets_768[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_769 = 2;
static const s_variable_target generated_targets_769[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[1].specials) },
};

static const int8_t generated_default_770 = 0;
static const s_variable_target generated_targets_770[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[1].ammo) },
};

static const int8_t generated_default_771 = 5;
static const s_variable_target generated_targets_771[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_772 = 24;
static const s_variable_target generated_targets_772[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[2].type) },
};

static const uint8_t generated_default_773 = 1;
static const s_variable_target generated_targets_773[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_774 = 0;
static const s_variable_target generated_targets_774[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[2].specials) },
};

static const int8_t generated_default_775 = 10;
static const s_variable_target generated_targets_775[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[2].ammo) },
};

static const int8_t generated_default_776 = 30;
static const s_variable_target generated_targets_776[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_777 = 13;
static const s_variable_target generated_targets_777[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[3].type) },
};

static const uint8_t generated_default_778 = 15;
static const s_variable_target generated_targets_778[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_779 = 0;
static const s_variable_target generated_targets_779[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[3].specials) },
};

static const int8_t generated_default_780 = 0;
static const s_variable_target generated_targets_780[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[3].ammo) },
};

static const int8_t generated_default_781 = 3;
static const s_variable_target generated_targets_781[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_782 = 4;
static const s_variable_target generated_targets_782[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[4].type) },
};

static const uint8_t generated_default_783 = 1;
static const s_variable_target generated_targets_783[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_784 = 4;
static const s_variable_target generated_targets_784[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[4].specials) },
};

static const int8_t generated_default_785 = 0;
static const s_variable_target generated_targets_785[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[4].ammo) },
};

static const int8_t generated_default_786 = 20;
static const s_variable_target generated_targets_786[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_787 = 11;
static const s_variable_target generated_targets_787[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[5].type) },
};

static const uint8_t generated_default_788 = 1;
static const s_variable_target generated_targets_788[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_789 = 2;
static const s_variable_target generated_targets_789[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[5].specials) },
};

static const int8_t generated_default_790 = 0;
static const s_variable_target generated_targets_790[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[5].ammo) },
};

static const int8_t generated_default_791 = 5;
static const s_variable_target generated_targets_791[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_792 = 37;
static const s_variable_target generated_targets_792[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[6].type) },
};

static const uint8_t generated_default_793 = 15;
static const s_variable_target generated_targets_793[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_794 = 0;
static const s_variable_target generated_targets_794[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[6].specials) },
};

static const int8_t generated_default_795 = 0;
static const s_variable_target generated_targets_795[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[6].ammo) },
};

static const int8_t generated_default_796 = 1;
static const s_variable_target generated_targets_796[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_797 = 37;
static const s_variable_target generated_targets_797[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[7].type) },
};

static const uint8_t generated_default_798 = 15;
static const s_variable_target generated_targets_798[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_799 = 0;
static const s_variable_target generated_targets_799[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[7].specials) },
};

static const int8_t generated_default_800 = 0;
static const s_variable_target generated_targets_800[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[7].ammo) },
};

static const int8_t generated_default_801 = 12;
static const s_variable_target generated_targets_801[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[0][4].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_802[] = { 128, 2, 0, 5, 2 };
static const s_variable_target generated_targets_802[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[0][4].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[0][4].special_device_flags) },
};

static const int8_t generated_default_803 = 0;
static const s_variable_target generated_targets_803[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].picture_num, sizeof(SHIP_CONFIG::antaran_templates[1][0].picture_num) },
};

static const uint8_t generated_default_804 = 0;
static const s_variable_target generated_targets_804[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].size, sizeof(SHIP_CONFIG::antaran_templates[1][0].size) },
};

static const uint8_t generated_default_805 = 5;
static const s_variable_target generated_targets_805[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[1][0].ftl_type) },
};

static const uint8_t generated_default_806 = 6;
static const s_variable_target generated_targets_806[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].armor_type, sizeof(SHIP_CONFIG::antaran_templates[1][0].armor_type) },
};

static const uint8_t generated_default_807 = 0;
static const s_variable_target generated_targets_807[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].shield_type, sizeof(SHIP_CONFIG::antaran_templates[1][0].shield_type) },
};

static const uint8_t generated_default_808 = 5;
static const s_variable_target generated_targets_808[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].computer_type, sizeof(SHIP_CONFIG::antaran_templates[1][0].computer_type) },
};

static const uint16_t generated_default_809 = 4;
static const s_variable_target generated_targets_809[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[0].type) },
};

static const uint8_t generated_default_810 = 1;
static const s_variable_target generated_targets_810[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_811 = 0;
static const s_variable_target generated_targets_811[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[0].specials) },
};

static const int8_t generated_default_812 = 0;
static const s_variable_target generated_targets_812[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[0].ammo) },
};

static const int8_t generated_default_813 = 2;
static const s_variable_target generated_targets_813[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_814 = 24;
static const s_variable_target generated_targets_814[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[1].type) },
};

static const uint8_t generated_default_815 = 15;
static const s_variable_target generated_targets_815[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_816 = 0;
static const s_variable_target generated_targets_816[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[1].specials) },
};

static const int8_t generated_default_817 = 0;
static const s_variable_target generated_targets_817[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[1].ammo) },
};

static const int8_t generated_default_818 = 1;
static const s_variable_target generated_targets_818[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_819 = 0;
static const s_variable_target generated_targets_819[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[2].type) },
};

static const uint8_t generated_default_820 = 0;
static const s_variable_target generated_targets_820[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_821 = 0;
static const s_variable_target generated_targets_821[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[2].specials) },
};

static const int8_t generated_default_822 = 0;
static const s_variable_target generated_targets_822[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[2].ammo) },
};

static const int8_t generated_default_823 = 0;
static const s_variable_target generated_targets_823[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_824 = 0;
static const s_variable_target generated_targets_824[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[3].type) },
};

static const uint8_t generated_default_825 = 0;
static const s_variable_target generated_targets_825[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_826 = 0;
static const s_variable_target generated_targets_826[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[3].specials) },
};

static const int8_t generated_default_827 = 0;
static const s_variable_target generated_targets_827[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[3].ammo) },
};

static const int8_t generated_default_828 = 0;
static const s_variable_target generated_targets_828[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_829 = 0;
static const s_variable_target generated_targets_829[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[4].type) },
};

static const uint8_t generated_default_830 = 0;
static const s_variable_target generated_targets_830[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_831 = 0;
static const s_variable_target generated_targets_831[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[4].specials) },
};

static const int8_t generated_default_832 = 0;
static const s_variable_target generated_targets_832[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[4].ammo) },
};

static const int8_t generated_default_833 = 0;
static const s_variable_target generated_targets_833[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_834 = 0;
static const s_variable_target generated_targets_834[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[5].type) },
};

static const uint8_t generated_default_835 = 0;
static const s_variable_target generated_targets_835[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_836 = 0;
static const s_variable_target generated_targets_836[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[5].specials) },
};

static const int8_t generated_default_837 = 0;
static const s_variable_target generated_targets_837[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[5].ammo) },
};

static const int8_t generated_default_838 = 0;
static const s_variable_target generated_targets_838[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_839 = 0;
static const s_variable_target generated_targets_839[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[6].type) },
};

static const uint8_t generated_default_840 = 0;
static const s_variable_target generated_targets_840[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_841 = 0;
static const s_variable_target generated_targets_841[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[6].specials) },
};

static const int8_t generated_default_842 = 0;
static const s_variable_target generated_targets_842[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[6].ammo) },
};

static const int8_t generated_default_843 = 0;
static const s_variable_target generated_targets_843[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_844 = 0;
static const s_variable_target generated_targets_844[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[7].type) },
};

static const uint8_t generated_default_845 = 0;
static const s_variable_target generated_targets_845[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_846 = 0;
static const s_variable_target generated_targets_846[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[7].specials) },
};

static const int8_t generated_default_847 = 0;
static const s_variable_target generated_targets_847[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[7].ammo) },
};

static const int8_t generated_default_848 = 0;
static const s_variable_target generated_targets_848[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][0].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_849[] = { 128, 0, 0, 1, 0 };
static const s_variable_target generated_targets_849[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][0].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[1][0].special_device_flags) },
};

static const int8_t generated_default_850 = 1;
static const s_variable_target generated_targets_850[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].picture_num, sizeof(SHIP_CONFIG::antaran_templates[1][1].picture_num) },
};

static const uint8_t generated_default_851 = 1;
static const s_variable_target generated_targets_851[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].size, sizeof(SHIP_CONFIG::antaran_templates[1][1].size) },
};

static const uint8_t generated_default_852 = 5;
static const s_variable_target generated_targets_852[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[1][1].ftl_type) },
};

static const uint8_t generated_default_853 = 6;
static const s_variable_target generated_targets_853[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].armor_type, sizeof(SHIP_CONFIG::antaran_templates[1][1].armor_type) },
};

static const uint8_t generated_default_854 = 0;
static const s_variable_target generated_targets_854[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].shield_type, sizeof(SHIP_CONFIG::antaran_templates[1][1].shield_type) },
};

static const uint8_t generated_default_855 = 5;
static const s_variable_target generated_targets_855[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].computer_type, sizeof(SHIP_CONFIG::antaran_templates[1][1].computer_type) },
};

static const uint16_t generated_default_856 = 4;
static const s_variable_target generated_targets_856[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[0].type) },
};

static const uint8_t generated_default_857 = 1;
static const s_variable_target generated_targets_857[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_858 = 0;
static const s_variable_target generated_targets_858[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[0].specials) },
};

static const int8_t generated_default_859 = 0;
static const s_variable_target generated_targets_859[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[0].ammo) },
};

static const int8_t generated_default_860 = 3;
static const s_variable_target generated_targets_860[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_861 = 4;
static const s_variable_target generated_targets_861[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[1].type) },
};

static const uint8_t generated_default_862 = 1;
static const s_variable_target generated_targets_862[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_863 = 2;
static const s_variable_target generated_targets_863[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[1].specials) },
};

static const int8_t generated_default_864 = 0;
static const s_variable_target generated_targets_864[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[1].ammo) },
};

static const int8_t generated_default_865 = 1;
static const s_variable_target generated_targets_865[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_866 = 24;
static const s_variable_target generated_targets_866[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[2].type) },
};

static const uint8_t generated_default_867 = 15;
static const s_variable_target generated_targets_867[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_868 = 0;
static const s_variable_target generated_targets_868[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[2].specials) },
};

static const int8_t generated_default_869 = 10;
static const s_variable_target generated_targets_869[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[2].ammo) },
};

static const int8_t generated_default_870 = 1;
static const s_variable_target generated_targets_870[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_871 = 0;
static const s_variable_target generated_targets_871[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[3].type) },
};

static const uint8_t generated_default_872 = 0;
static const s_variable_target generated_targets_872[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_873 = 0;
static const s_variable_target generated_targets_873[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[3].specials) },
};

static const int8_t generated_default_874 = 0;
static const s_variable_target generated_targets_874[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[3].ammo) },
};

static const int8_t generated_default_875 = 0;
static const s_variable_target generated_targets_875[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_876 = 0;
static const s_variable_target generated_targets_876[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[4].type) },
};

static const uint8_t generated_default_877 = 0;
static const s_variable_target generated_targets_877[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_878 = 0;
static const s_variable_target generated_targets_878[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[4].specials) },
};

static const int8_t generated_default_879 = 0;
static const s_variable_target generated_targets_879[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[4].ammo) },
};

static const int8_t generated_default_880 = 0;
static const s_variable_target generated_targets_880[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_881 = 0;
static const s_variable_target generated_targets_881[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[5].type) },
};

static const uint8_t generated_default_882 = 0;
static const s_variable_target generated_targets_882[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_883 = 0;
static const s_variable_target generated_targets_883[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[5].specials) },
};

static const int8_t generated_default_884 = 0;
static const s_variable_target generated_targets_884[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[5].ammo) },
};

static const int8_t generated_default_885 = 0;
static const s_variable_target generated_targets_885[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_886 = 0;
static const s_variable_target generated_targets_886[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[6].type) },
};

static const uint8_t generated_default_887 = 0;
static const s_variable_target generated_targets_887[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_888 = 0;
static const s_variable_target generated_targets_888[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[6].specials) },
};

static const int8_t generated_default_889 = 0;
static const s_variable_target generated_targets_889[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[6].ammo) },
};

static const int8_t generated_default_890 = 0;
static const s_variable_target generated_targets_890[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_891 = 0;
static const s_variable_target generated_targets_891[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[7].type) },
};

static const uint8_t generated_default_892 = 0;
static const s_variable_target generated_targets_892[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_893 = 0;
static const s_variable_target generated_targets_893[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[7].specials) },
};

static const int8_t generated_default_894 = 0;
static const s_variable_target generated_targets_894[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[7].ammo) },
};

static const int8_t generated_default_895 = 0;
static const s_variable_target generated_targets_895[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][1].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_896[] = { 128, 0, 0, 1, 0 };
static const s_variable_target generated_targets_896[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][1].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[1][1].special_device_flags) },
};

static const int8_t generated_default_897 = 2;
static const s_variable_target generated_targets_897[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].picture_num, sizeof(SHIP_CONFIG::antaran_templates[1][2].picture_num) },
};

static const uint8_t generated_default_898 = 2;
static const s_variable_target generated_targets_898[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].size, sizeof(SHIP_CONFIG::antaran_templates[1][2].size) },
};

static const uint8_t generated_default_899 = 5;
static const s_variable_target generated_targets_899[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[1][2].ftl_type) },
};

static const uint8_t generated_default_900 = 6;
static const s_variable_target generated_targets_900[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].armor_type, sizeof(SHIP_CONFIG::antaran_templates[1][2].armor_type) },
};

static const uint8_t generated_default_901 = 0;
static const s_variable_target generated_targets_901[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].shield_type, sizeof(SHIP_CONFIG::antaran_templates[1][2].shield_type) },
};

static const uint8_t generated_default_902 = 5;
static const s_variable_target generated_targets_902[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].computer_type, sizeof(SHIP_CONFIG::antaran_templates[1][2].computer_type) },
};

static const uint16_t generated_default_903 = 4;
static const s_variable_target generated_targets_903[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[0].type) },
};

static const uint8_t generated_default_904 = 1;
static const s_variable_target generated_targets_904[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_905 = 0;
static const s_variable_target generated_targets_905[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[0].specials) },
};

static const int8_t generated_default_906 = 0;
static const s_variable_target generated_targets_906[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[0].ammo) },
};

static const int8_t generated_default_907 = 4;
static const s_variable_target generated_targets_907[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_908 = 4;
static const s_variable_target generated_targets_908[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[1].type) },
};

static const uint8_t generated_default_909 = 1;
static const s_variable_target generated_targets_909[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_910 = 2;
static const s_variable_target generated_targets_910[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[1].specials) },
};

static const int8_t generated_default_911 = 0;
static const s_variable_target generated_targets_911[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[1].ammo) },
};

static const int8_t generated_default_912 = 2;
static const s_variable_target generated_targets_912[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_913 = 24;
static const s_variable_target generated_targets_913[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[2].type) },
};

static const uint8_t generated_default_914 = 15;
static const s_variable_target generated_targets_914[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_915 = 0;
static const s_variable_target generated_targets_915[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[2].specials) },
};

static const int8_t generated_default_916 = 10;
static const s_variable_target generated_targets_916[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[2].ammo) },
};

static const int8_t generated_default_917 = 5;
static const s_variable_target generated_targets_917[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_918 = 13;
static const s_variable_target generated_targets_918[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[3].type) },
};

static const uint8_t generated_default_919 = 15;
static const s_variable_target generated_targets_919[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_920 = 0;
static const s_variable_target generated_targets_920[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[3].specials) },
};

static const int8_t generated_default_921 = 0;
static const s_variable_target generated_targets_921[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[3].ammo) },
};

static const int8_t generated_default_922 = 2;
static const s_variable_target generated_targets_922[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_923 = 31;
static const s_variable_target generated_targets_923[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[4].type) },
};

static const uint8_t generated_default_924 = 15;
static const s_variable_target generated_targets_924[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_925 = 0;
static const s_variable_target generated_targets_925[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[4].specials) },
};

static const int8_t generated_default_926 = 0;
static const s_variable_target generated_targets_926[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[4].ammo) },
};

static const int8_t generated_default_927 = 3;
static const s_variable_target generated_targets_927[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_928 = 0;
static const s_variable_target generated_targets_928[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[5].type) },
};

static const uint8_t generated_default_929 = 0;
static const s_variable_target generated_targets_929[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_930 = 0;
static const s_variable_target generated_targets_930[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[5].specials) },
};

static const int8_t generated_default_931 = 0;
static const s_variable_target generated_targets_931[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[5].ammo) },
};

static const int8_t generated_default_932 = 0;
static const s_variable_target generated_targets_932[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_933 = 0;
static const s_variable_target generated_targets_933[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[6].type) },
};

static const uint8_t generated_default_934 = 0;
static const s_variable_target generated_targets_934[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_935 = 0;
static const s_variable_target generated_targets_935[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[6].specials) },
};

static const int8_t generated_default_936 = 0;
static const s_variable_target generated_targets_936[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[6].ammo) },
};

static const int8_t generated_default_937 = 0;
static const s_variable_target generated_targets_937[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_938 = 0;
static const s_variable_target generated_targets_938[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[7].type) },
};

static const uint8_t generated_default_939 = 0;
static const s_variable_target generated_targets_939[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_940 = 0;
static const s_variable_target generated_targets_940[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[7].specials) },
};

static const int8_t generated_default_941 = 0;
static const s_variable_target generated_targets_941[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[7].ammo) },
};

static const int8_t generated_default_942 = 0;
static const s_variable_target generated_targets_942[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][2].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_943[] = { 128, 0, 0, 1, 0 };
static const s_variable_target generated_targets_943[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][2].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[1][2].special_device_flags) },
};

static const int8_t generated_default_944 = 3;
static const s_variable_target generated_targets_944[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].picture_num, sizeof(SHIP_CONFIG::antaran_templates[1][3].picture_num) },
};

static const uint8_t generated_default_945 = 3;
static const s_variable_target generated_targets_945[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].size, sizeof(SHIP_CONFIG::antaran_templates[1][3].size) },
};

static const uint8_t generated_default_946 = 5;
static const s_variable_target generated_targets_946[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[1][3].ftl_type) },
};

static const uint8_t generated_default_947 = 6;
static const s_variable_target generated_targets_947[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].armor_type, sizeof(SHIP_CONFIG::antaran_templates[1][3].armor_type) },
};

static const uint8_t generated_default_948 = 0;
static const s_variable_target generated_targets_948[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].shield_type, sizeof(SHIP_CONFIG::antaran_templates[1][3].shield_type) },
};

static const uint8_t generated_default_949 = 5;
static const s_variable_target generated_targets_949[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].computer_type, sizeof(SHIP_CONFIG::antaran_templates[1][3].computer_type) },
};

static const uint16_t generated_default_950 = 4;
static const s_variable_target generated_targets_950[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[0].type) },
};

static const uint8_t generated_default_951 = 1;
static const s_variable_target generated_targets_951[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_952 = 0;
static const s_variable_target generated_targets_952[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[0].specials) },
};

static const int8_t generated_default_953 = 0;
static const s_variable_target generated_targets_953[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[0].ammo) },
};

static const int8_t generated_default_954 = 10;
static const s_variable_target generated_targets_954[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_955 = 4;
static const s_variable_target generated_targets_955[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[1].type) },
};

static const uint8_t generated_default_956 = 1;
static const s_variable_target generated_targets_956[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_957 = 2;
static const s_variable_target generated_targets_957[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[1].specials) },
};

static const int8_t generated_default_958 = 0;
static const s_variable_target generated_targets_958[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[1].ammo) },
};

static const int8_t generated_default_959 = 4;
static const s_variable_target generated_targets_959[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_960 = 24;
static const s_variable_target generated_targets_960[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[2].type) },
};

static const uint8_t generated_default_961 = 1;
static const s_variable_target generated_targets_961[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_962 = 0;
static const s_variable_target generated_targets_962[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[2].specials) },
};

static const int8_t generated_default_963 = 10;
static const s_variable_target generated_targets_963[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[2].ammo) },
};

static const int8_t generated_default_964 = 15;
static const s_variable_target generated_targets_964[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_965 = 13;
static const s_variable_target generated_targets_965[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[3].type) },
};

static const uint8_t generated_default_966 = 15;
static const s_variable_target generated_targets_966[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_967 = 0;
static const s_variable_target generated_targets_967[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[3].specials) },
};

static const int8_t generated_default_968 = 0;
static const s_variable_target generated_targets_968[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[3].ammo) },
};

static const int8_t generated_default_969 = 2;
static const s_variable_target generated_targets_969[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_970 = 4;
static const s_variable_target generated_targets_970[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[4].type) },
};

static const uint8_t generated_default_971 = 1;
static const s_variable_target generated_targets_971[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_972 = 4;
static const s_variable_target generated_targets_972[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[4].specials) },
};

static const int8_t generated_default_973 = 0;
static const s_variable_target generated_targets_973[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[4].ammo) },
};

static const int8_t generated_default_974 = 10;
static const s_variable_target generated_targets_974[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_975 = 11;
static const s_variable_target generated_targets_975[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[5].type) },
};

static const uint8_t generated_default_976 = 1;
static const s_variable_target generated_targets_976[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_977 = 0;
static const s_variable_target generated_targets_977[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[5].specials) },
};

static const int8_t generated_default_978 = 0;
static const s_variable_target generated_targets_978[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[5].ammo) },
};

static const int8_t generated_default_979 = 4;
static const s_variable_target generated_targets_979[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_980 = 0;
static const s_variable_target generated_targets_980[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[6].type) },
};

static const uint8_t generated_default_981 = 0;
static const s_variable_target generated_targets_981[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_982 = 0;
static const s_variable_target generated_targets_982[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[6].specials) },
};

static const int8_t generated_default_983 = 0;
static const s_variable_target generated_targets_983[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[6].ammo) },
};

static const int8_t generated_default_984 = 0;
static const s_variable_target generated_targets_984[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_985 = 0;
static const s_variable_target generated_targets_985[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[7].type) },
};

static const uint8_t generated_default_986 = 0;
static const s_variable_target generated_targets_986[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_987 = 0;
static const s_variable_target generated_targets_987[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[7].specials) },
};

static const int8_t generated_default_988 = 0;
static const s_variable_target generated_targets_988[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[7].ammo) },
};

static const int8_t generated_default_989 = 0;
static const s_variable_target generated_targets_989[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][3].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_990[] = { 128, 2, 0, 5, 2 };
static const s_variable_target generated_targets_990[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][3].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[1][3].special_device_flags) },
};

static const int8_t generated_default_991 = 4;
static const s_variable_target generated_targets_991[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].picture_num, sizeof(SHIP_CONFIG::antaran_templates[1][4].picture_num) },
};

static const uint8_t generated_default_992 = 4;
static const s_variable_target generated_targets_992[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].size, sizeof(SHIP_CONFIG::antaran_templates[1][4].size) },
};

static const uint8_t generated_default_993 = 5;
static const s_variable_target generated_targets_993[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[1][4].ftl_type) },
};

static const uint8_t generated_default_994 = 6;
static const s_variable_target generated_targets_994[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].armor_type, sizeof(SHIP_CONFIG::antaran_templates[1][4].armor_type) },
};

static const uint8_t generated_default_995 = 0;
static const s_variable_target generated_targets_995[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].shield_type, sizeof(SHIP_CONFIG::antaran_templates[1][4].shield_type) },
};

static const uint8_t generated_default_996 = 5;
static const s_variable_target generated_targets_996[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].computer_type, sizeof(SHIP_CONFIG::antaran_templates[1][4].computer_type) },
};

static const uint16_t generated_default_997 = 4;
static const s_variable_target generated_targets_997[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[0].type) },
};

static const uint8_t generated_default_998 = 1;
static const s_variable_target generated_targets_998[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_999 = 0;
static const s_variable_target generated_targets_999[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[0].specials) },
};

static const int8_t generated_default_1000 = 0;
static const s_variable_target generated_targets_1000[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1001 = 20;
static const s_variable_target generated_targets_1001[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1002 = 4;
static const s_variable_target generated_targets_1002[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[1].type) },
};

static const uint8_t generated_default_1003 = 1;
static const s_variable_target generated_targets_1003[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1004 = 2;
static const s_variable_target generated_targets_1004[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[1].specials) },
};

static const int8_t generated_default_1005 = 0;
static const s_variable_target generated_targets_1005[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1006 = 5;
static const s_variable_target generated_targets_1006[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1007 = 24;
static const s_variable_target generated_targets_1007[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[2].type) },
};

static const uint8_t generated_default_1008 = 1;
static const s_variable_target generated_targets_1008[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1009 = 0;
static const s_variable_target generated_targets_1009[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[2].specials) },
};

static const int8_t generated_default_1010 = 10;
static const s_variable_target generated_targets_1010[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1011 = 30;
static const s_variable_target generated_targets_1011[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1012 = 13;
static const s_variable_target generated_targets_1012[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[3].type) },
};

static const uint8_t generated_default_1013 = 15;
static const s_variable_target generated_targets_1013[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1014 = 0;
static const s_variable_target generated_targets_1014[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[3].specials) },
};

static const int8_t generated_default_1015 = 0;
static const s_variable_target generated_targets_1015[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1016 = 3;
static const s_variable_target generated_targets_1016[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1017 = 4;
static const s_variable_target generated_targets_1017[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[4].type) },
};

static const uint8_t generated_default_1018 = 1;
static const s_variable_target generated_targets_1018[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1019 = 4;
static const s_variable_target generated_targets_1019[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[4].specials) },
};

static const int8_t generated_default_1020 = 0;
static const s_variable_target generated_targets_1020[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1021 = 20;
static const s_variable_target generated_targets_1021[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1022 = 11;
static const s_variable_target generated_targets_1022[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[5].type) },
};

static const uint8_t generated_default_1023 = 1;
static const s_variable_target generated_targets_1023[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1024 = 2;
static const s_variable_target generated_targets_1024[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[5].specials) },
};

static const int8_t generated_default_1025 = 0;
static const s_variable_target generated_targets_1025[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1026 = 5;
static const s_variable_target generated_targets_1026[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1027 = 37;
static const s_variable_target generated_targets_1027[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[6].type) },
};

static const uint8_t generated_default_1028 = 15;
static const s_variable_target generated_targets_1028[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1029 = 0;
static const s_variable_target generated_targets_1029[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[6].specials) },
};

static const int8_t generated_default_1030 = 0;
static const s_variable_target generated_targets_1030[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1031 = 1;
static const s_variable_target generated_targets_1031[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1032 = 37;
static const s_variable_target generated_targets_1032[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[7].type) },
};

static const uint8_t generated_default_1033 = 15;
static const s_variable_target generated_targets_1033[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1034 = 0;
static const s_variable_target generated_targets_1034[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[7].specials) },
};

static const int8_t generated_default_1035 = 0;
static const s_variable_target generated_targets_1035[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1036 = 12;
static const s_variable_target generated_targets_1036[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[1][4].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1037[] = { 128, 0, 32, 5, 2 };
static const s_variable_target generated_targets_1037[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[1][4].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[1][4].special_device_flags) },
};

static const int8_t generated_default_1038 = 0;
static const s_variable_target generated_targets_1038[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].picture_num, sizeof(SHIP_CONFIG::antaran_templates[2][0].picture_num) },
};

static const uint8_t generated_default_1039 = 0;
static const s_variable_target generated_targets_1039[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].size, sizeof(SHIP_CONFIG::antaran_templates[2][0].size) },
};

static const uint8_t generated_default_1040 = 5;
static const s_variable_target generated_targets_1040[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[2][0].ftl_type) },
};

static const uint8_t generated_default_1041 = 6;
static const s_variable_target generated_targets_1041[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].armor_type, sizeof(SHIP_CONFIG::antaran_templates[2][0].armor_type) },
};

static const uint8_t generated_default_1042 = 0;
static const s_variable_target generated_targets_1042[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].shield_type, sizeof(SHIP_CONFIG::antaran_templates[2][0].shield_type) },
};

static const uint8_t generated_default_1043 = 5;
static const s_variable_target generated_targets_1043[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].computer_type, sizeof(SHIP_CONFIG::antaran_templates[2][0].computer_type) },
};

static const uint16_t generated_default_1044 = 4;
static const s_variable_target generated_targets_1044[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[0].type) },
};

static const uint8_t generated_default_1045 = 1;
static const s_variable_target generated_targets_1045[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_1046 = 0;
static const s_variable_target generated_targets_1046[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[0].specials) },
};

static const int8_t generated_default_1047 = 0;
static const s_variable_target generated_targets_1047[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1048 = 2;
static const s_variable_target generated_targets_1048[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1049 = 24;
static const s_variable_target generated_targets_1049[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[1].type) },
};

static const uint8_t generated_default_1050 = 15;
static const s_variable_target generated_targets_1050[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1051 = 0;
static const s_variable_target generated_targets_1051[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[1].specials) },
};

static const int8_t generated_default_1052 = 0;
static const s_variable_target generated_targets_1052[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1053 = 1;
static const s_variable_target generated_targets_1053[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1054 = 0;
static const s_variable_target generated_targets_1054[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[2].type) },
};

static const uint8_t generated_default_1055 = 0;
static const s_variable_target generated_targets_1055[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1056 = 0;
static const s_variable_target generated_targets_1056[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[2].specials) },
};

static const int8_t generated_default_1057 = 0;
static const s_variable_target generated_targets_1057[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1058 = 0;
static const s_variable_target generated_targets_1058[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1059 = 0;
static const s_variable_target generated_targets_1059[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[3].type) },
};

static const uint8_t generated_default_1060 = 0;
static const s_variable_target generated_targets_1060[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1061 = 0;
static const s_variable_target generated_targets_1061[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[3].specials) },
};

static const int8_t generated_default_1062 = 0;
static const s_variable_target generated_targets_1062[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1063 = 0;
static const s_variable_target generated_targets_1063[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1064 = 0;
static const s_variable_target generated_targets_1064[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[4].type) },
};

static const uint8_t generated_default_1065 = 0;
static const s_variable_target generated_targets_1065[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1066 = 0;
static const s_variable_target generated_targets_1066[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[4].specials) },
};

static const int8_t generated_default_1067 = 0;
static const s_variable_target generated_targets_1067[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1068 = 0;
static const s_variable_target generated_targets_1068[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1069 = 0;
static const s_variable_target generated_targets_1069[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[5].type) },
};

static const uint8_t generated_default_1070 = 0;
static const s_variable_target generated_targets_1070[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1071 = 0;
static const s_variable_target generated_targets_1071[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[5].specials) },
};

static const int8_t generated_default_1072 = 0;
static const s_variable_target generated_targets_1072[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1073 = 0;
static const s_variable_target generated_targets_1073[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1074 = 0;
static const s_variable_target generated_targets_1074[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[6].type) },
};

static const uint8_t generated_default_1075 = 0;
static const s_variable_target generated_targets_1075[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1076 = 0;
static const s_variable_target generated_targets_1076[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[6].specials) },
};

static const int8_t generated_default_1077 = 0;
static const s_variable_target generated_targets_1077[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1078 = 0;
static const s_variable_target generated_targets_1078[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1079 = 0;
static const s_variable_target generated_targets_1079[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[7].type) },
};

static const uint8_t generated_default_1080 = 0;
static const s_variable_target generated_targets_1080[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1081 = 0;
static const s_variable_target generated_targets_1081[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[7].specials) },
};

static const int8_t generated_default_1082 = 0;
static const s_variable_target generated_targets_1082[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1083 = 0;
static const s_variable_target generated_targets_1083[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][0].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1084[] = { 128, 0, 0, 1, 0 };
static const s_variable_target generated_targets_1084[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][0].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[2][0].special_device_flags) },
};

static const int8_t generated_default_1085 = 1;
static const s_variable_target generated_targets_1085[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].picture_num, sizeof(SHIP_CONFIG::antaran_templates[2][1].picture_num) },
};

static const uint8_t generated_default_1086 = 1;
static const s_variable_target generated_targets_1086[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].size, sizeof(SHIP_CONFIG::antaran_templates[2][1].size) },
};

static const uint8_t generated_default_1087 = 5;
static const s_variable_target generated_targets_1087[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[2][1].ftl_type) },
};

static const uint8_t generated_default_1088 = 6;
static const s_variable_target generated_targets_1088[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].armor_type, sizeof(SHIP_CONFIG::antaran_templates[2][1].armor_type) },
};

static const uint8_t generated_default_1089 = 0;
static const s_variable_target generated_targets_1089[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].shield_type, sizeof(SHIP_CONFIG::antaran_templates[2][1].shield_type) },
};

static const uint8_t generated_default_1090 = 5;
static const s_variable_target generated_targets_1090[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].computer_type, sizeof(SHIP_CONFIG::antaran_templates[2][1].computer_type) },
};

static const uint16_t generated_default_1091 = 4;
static const s_variable_target generated_targets_1091[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[0].type) },
};

static const uint8_t generated_default_1092 = 1;
static const s_variable_target generated_targets_1092[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_1093 = 0;
static const s_variable_target generated_targets_1093[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[0].specials) },
};

static const int8_t generated_default_1094 = 0;
static const s_variable_target generated_targets_1094[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1095 = 3;
static const s_variable_target generated_targets_1095[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1096 = 4;
static const s_variable_target generated_targets_1096[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[1].type) },
};

static const uint8_t generated_default_1097 = 1;
static const s_variable_target generated_targets_1097[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1098 = 2;
static const s_variable_target generated_targets_1098[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[1].specials) },
};

static const int8_t generated_default_1099 = 0;
static const s_variable_target generated_targets_1099[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1100 = 1;
static const s_variable_target generated_targets_1100[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1101 = 24;
static const s_variable_target generated_targets_1101[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[2].type) },
};

static const uint8_t generated_default_1102 = 15;
static const s_variable_target generated_targets_1102[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1103 = 0;
static const s_variable_target generated_targets_1103[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[2].specials) },
};

static const int8_t generated_default_1104 = 10;
static const s_variable_target generated_targets_1104[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1105 = 1;
static const s_variable_target generated_targets_1105[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1106 = 0;
static const s_variable_target generated_targets_1106[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[3].type) },
};

static const uint8_t generated_default_1107 = 0;
static const s_variable_target generated_targets_1107[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1108 = 0;
static const s_variable_target generated_targets_1108[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[3].specials) },
};

static const int8_t generated_default_1109 = 0;
static const s_variable_target generated_targets_1109[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1110 = 0;
static const s_variable_target generated_targets_1110[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1111 = 0;
static const s_variable_target generated_targets_1111[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[4].type) },
};

static const uint8_t generated_default_1112 = 0;
static const s_variable_target generated_targets_1112[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1113 = 0;
static const s_variable_target generated_targets_1113[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[4].specials) },
};

static const int8_t generated_default_1114 = 0;
static const s_variable_target generated_targets_1114[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1115 = 0;
static const s_variable_target generated_targets_1115[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1116 = 0;
static const s_variable_target generated_targets_1116[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[5].type) },
};

static const uint8_t generated_default_1117 = 0;
static const s_variable_target generated_targets_1117[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1118 = 0;
static const s_variable_target generated_targets_1118[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[5].specials) },
};

static const int8_t generated_default_1119 = 0;
static const s_variable_target generated_targets_1119[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1120 = 0;
static const s_variable_target generated_targets_1120[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1121 = 0;
static const s_variable_target generated_targets_1121[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[6].type) },
};

static const uint8_t generated_default_1122 = 0;
static const s_variable_target generated_targets_1122[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1123 = 0;
static const s_variable_target generated_targets_1123[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[6].specials) },
};

static const int8_t generated_default_1124 = 0;
static const s_variable_target generated_targets_1124[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1125 = 0;
static const s_variable_target generated_targets_1125[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1126 = 0;
static const s_variable_target generated_targets_1126[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[7].type) },
};

static const uint8_t generated_default_1127 = 0;
static const s_variable_target generated_targets_1127[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1128 = 0;
static const s_variable_target generated_targets_1128[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[7].specials) },
};

static const int8_t generated_default_1129 = 0;
static const s_variable_target generated_targets_1129[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1130 = 0;
static const s_variable_target generated_targets_1130[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][1].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1131[] = { 128, 0, 0, 1, 0 };
static const s_variable_target generated_targets_1131[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][1].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[2][1].special_device_flags) },
};

static const int8_t generated_default_1132 = 2;
static const s_variable_target generated_targets_1132[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].picture_num, sizeof(SHIP_CONFIG::antaran_templates[2][2].picture_num) },
};

static const uint8_t generated_default_1133 = 2;
static const s_variable_target generated_targets_1133[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].size, sizeof(SHIP_CONFIG::antaran_templates[2][2].size) },
};

static const uint8_t generated_default_1134 = 5;
static const s_variable_target generated_targets_1134[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[2][2].ftl_type) },
};

static const uint8_t generated_default_1135 = 6;
static const s_variable_target generated_targets_1135[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].armor_type, sizeof(SHIP_CONFIG::antaran_templates[2][2].armor_type) },
};

static const uint8_t generated_default_1136 = 0;
static const s_variable_target generated_targets_1136[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].shield_type, sizeof(SHIP_CONFIG::antaran_templates[2][2].shield_type) },
};

static const uint8_t generated_default_1137 = 5;
static const s_variable_target generated_targets_1137[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].computer_type, sizeof(SHIP_CONFIG::antaran_templates[2][2].computer_type) },
};

static const uint16_t generated_default_1138 = 4;
static const s_variable_target generated_targets_1138[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[0].type) },
};

static const uint8_t generated_default_1139 = 1;
static const s_variable_target generated_targets_1139[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_1140 = 0;
static const s_variable_target generated_targets_1140[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[0].specials) },
};

static const int8_t generated_default_1141 = 0;
static const s_variable_target generated_targets_1141[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1142 = 4;
static const s_variable_target generated_targets_1142[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1143 = 4;
static const s_variable_target generated_targets_1143[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[1].type) },
};

static const uint8_t generated_default_1144 = 1;
static const s_variable_target generated_targets_1144[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1145 = 2;
static const s_variable_target generated_targets_1145[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[1].specials) },
};

static const int8_t generated_default_1146 = 0;
static const s_variable_target generated_targets_1146[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1147 = 2;
static const s_variable_target generated_targets_1147[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1148 = 24;
static const s_variable_target generated_targets_1148[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[2].type) },
};

static const uint8_t generated_default_1149 = 15;
static const s_variable_target generated_targets_1149[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1150 = 0;
static const s_variable_target generated_targets_1150[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[2].specials) },
};

static const int8_t generated_default_1151 = 10;
static const s_variable_target generated_targets_1151[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1152 = 5;
static const s_variable_target generated_targets_1152[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1153 = 13;
static const s_variable_target generated_targets_1153[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[3].type) },
};

static const uint8_t generated_default_1154 = 15;
static const s_variable_target generated_targets_1154[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1155 = 0;
static const s_variable_target generated_targets_1155[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[3].specials) },
};

static const int8_t generated_default_1156 = 0;
static const s_variable_target generated_targets_1156[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1157 = 2;
static const s_variable_target generated_targets_1157[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1158 = 31;
static const s_variable_target generated_targets_1158[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[4].type) },
};

static const uint8_t generated_default_1159 = 15;
static const s_variable_target generated_targets_1159[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1160 = 0;
static const s_variable_target generated_targets_1160[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[4].specials) },
};

static const int8_t generated_default_1161 = 0;
static const s_variable_target generated_targets_1161[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1162 = 3;
static const s_variable_target generated_targets_1162[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1163 = 0;
static const s_variable_target generated_targets_1163[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[5].type) },
};

static const uint8_t generated_default_1164 = 0;
static const s_variable_target generated_targets_1164[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1165 = 0;
static const s_variable_target generated_targets_1165[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[5].specials) },
};

static const int8_t generated_default_1166 = 0;
static const s_variable_target generated_targets_1166[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1167 = 0;
static const s_variable_target generated_targets_1167[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1168 = 0;
static const s_variable_target generated_targets_1168[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[6].type) },
};

static const uint8_t generated_default_1169 = 0;
static const s_variable_target generated_targets_1169[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1170 = 0;
static const s_variable_target generated_targets_1170[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[6].specials) },
};

static const int8_t generated_default_1171 = 0;
static const s_variable_target generated_targets_1171[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1172 = 0;
static const s_variable_target generated_targets_1172[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1173 = 0;
static const s_variable_target generated_targets_1173[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[7].type) },
};

static const uint8_t generated_default_1174 = 0;
static const s_variable_target generated_targets_1174[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1175 = 0;
static const s_variable_target generated_targets_1175[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[7].specials) },
};

static const int8_t generated_default_1176 = 0;
static const s_variable_target generated_targets_1176[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1177 = 0;
static const s_variable_target generated_targets_1177[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][2].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1178[] = { 128, 2, 0, 1, 0 };
static const s_variable_target generated_targets_1178[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][2].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[2][2].special_device_flags) },
};

static const int8_t generated_default_1179 = 3;
static const s_variable_target generated_targets_1179[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].picture_num, sizeof(SHIP_CONFIG::antaran_templates[2][3].picture_num) },
};

static const uint8_t generated_default_1180 = 3;
static const s_variable_target generated_targets_1180[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].size, sizeof(SHIP_CONFIG::antaran_templates[2][3].size) },
};

static const uint8_t generated_default_1181 = 5;
static const s_variable_target generated_targets_1181[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[2][3].ftl_type) },
};

static const uint8_t generated_default_1182 = 6;
static const s_variable_target generated_targets_1182[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].armor_type, sizeof(SHIP_CONFIG::antaran_templates[2][3].armor_type) },
};

static const uint8_t generated_default_1183 = 0;
static const s_variable_target generated_targets_1183[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].shield_type, sizeof(SHIP_CONFIG::antaran_templates[2][3].shield_type) },
};

static const uint8_t generated_default_1184 = 5;
static const s_variable_target generated_targets_1184[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].computer_type, sizeof(SHIP_CONFIG::antaran_templates[2][3].computer_type) },
};

static const uint16_t generated_default_1185 = 4;
static const s_variable_target generated_targets_1185[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[0].type) },
};

static const uint8_t generated_default_1186 = 1;
static const s_variable_target generated_targets_1186[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_1187 = 0;
static const s_variable_target generated_targets_1187[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[0].specials) },
};

static const int8_t generated_default_1188 = 0;
static const s_variable_target generated_targets_1188[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1189 = 10;
static const s_variable_target generated_targets_1189[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1190 = 4;
static const s_variable_target generated_targets_1190[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[1].type) },
};

static const uint8_t generated_default_1191 = 1;
static const s_variable_target generated_targets_1191[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1192 = 2;
static const s_variable_target generated_targets_1192[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[1].specials) },
};

static const int8_t generated_default_1193 = 0;
static const s_variable_target generated_targets_1193[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1194 = 4;
static const s_variable_target generated_targets_1194[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1195 = 24;
static const s_variable_target generated_targets_1195[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[2].type) },
};

static const uint8_t generated_default_1196 = 1;
static const s_variable_target generated_targets_1196[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1197 = 0;
static const s_variable_target generated_targets_1197[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[2].specials) },
};

static const int8_t generated_default_1198 = 10;
static const s_variable_target generated_targets_1198[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1199 = 15;
static const s_variable_target generated_targets_1199[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1200 = 13;
static const s_variable_target generated_targets_1200[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[3].type) },
};

static const uint8_t generated_default_1201 = 15;
static const s_variable_target generated_targets_1201[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1202 = 0;
static const s_variable_target generated_targets_1202[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[3].specials) },
};

static const int8_t generated_default_1203 = 0;
static const s_variable_target generated_targets_1203[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1204 = 2;
static const s_variable_target generated_targets_1204[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1205 = 4;
static const s_variable_target generated_targets_1205[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[4].type) },
};

static const uint8_t generated_default_1206 = 1;
static const s_variable_target generated_targets_1206[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1207 = 4;
static const s_variable_target generated_targets_1207[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[4].specials) },
};

static const int8_t generated_default_1208 = 0;
static const s_variable_target generated_targets_1208[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1209 = 10;
static const s_variable_target generated_targets_1209[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1210 = 11;
static const s_variable_target generated_targets_1210[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[5].type) },
};

static const uint8_t generated_default_1211 = 1;
static const s_variable_target generated_targets_1211[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1212 = 0;
static const s_variable_target generated_targets_1212[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[5].specials) },
};

static const int8_t generated_default_1213 = 0;
static const s_variable_target generated_targets_1213[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1214 = 4;
static const s_variable_target generated_targets_1214[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1215 = 0;
static const s_variable_target generated_targets_1215[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[6].type) },
};

static const uint8_t generated_default_1216 = 0;
static const s_variable_target generated_targets_1216[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1217 = 0;
static const s_variable_target generated_targets_1217[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[6].specials) },
};

static const int8_t generated_default_1218 = 0;
static const s_variable_target generated_targets_1218[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1219 = 0;
static const s_variable_target generated_targets_1219[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1220 = 0;
static const s_variable_target generated_targets_1220[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[7].type) },
};

static const uint8_t generated_default_1221 = 0;
static const s_variable_target generated_targets_1221[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1222 = 0;
static const s_variable_target generated_targets_1222[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[7].specials) },
};

static const int8_t generated_default_1223 = 0;
static const s_variable_target generated_targets_1223[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1224 = 0;
static const s_variable_target generated_targets_1224[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][3].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1225[] = { 128, 0, 32, 5, 2 };
static const s_variable_target generated_targets_1225[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][3].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[2][3].special_device_flags) },
};

static const int8_t generated_default_1226 = 4;
static const s_variable_target generated_targets_1226[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].picture_num, sizeof(SHIP_CONFIG::antaran_templates[2][4].picture_num) },
};

static const uint8_t generated_default_1227 = 4;
static const s_variable_target generated_targets_1227[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].size, sizeof(SHIP_CONFIG::antaran_templates[2][4].size) },
};

static const uint8_t generated_default_1228 = 5;
static const s_variable_target generated_targets_1228[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[2][4].ftl_type) },
};

static const uint8_t generated_default_1229 = 6;
static const s_variable_target generated_targets_1229[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].armor_type, sizeof(SHIP_CONFIG::antaran_templates[2][4].armor_type) },
};

static const uint8_t generated_default_1230 = 0;
static const s_variable_target generated_targets_1230[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].shield_type, sizeof(SHIP_CONFIG::antaran_templates[2][4].shield_type) },
};

static const uint8_t generated_default_1231 = 5;
static const s_variable_target generated_targets_1231[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].computer_type, sizeof(SHIP_CONFIG::antaran_templates[2][4].computer_type) },
};

static const uint16_t generated_default_1232 = 4;
static const s_variable_target generated_targets_1232[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[0].type) },
};

static const uint8_t generated_default_1233 = 1;
static const s_variable_target generated_targets_1233[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_1234 = 0;
static const s_variable_target generated_targets_1234[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[0].specials) },
};

static const int8_t generated_default_1235 = 0;
static const s_variable_target generated_targets_1235[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1236 = 20;
static const s_variable_target generated_targets_1236[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1237 = 4;
static const s_variable_target generated_targets_1237[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[1].type) },
};

static const uint8_t generated_default_1238 = 1;
static const s_variable_target generated_targets_1238[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1239 = 2;
static const s_variable_target generated_targets_1239[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[1].specials) },
};

static const int8_t generated_default_1240 = 0;
static const s_variable_target generated_targets_1240[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1241 = 5;
static const s_variable_target generated_targets_1241[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1242 = 24;
static const s_variable_target generated_targets_1242[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[2].type) },
};

static const uint8_t generated_default_1243 = 1;
static const s_variable_target generated_targets_1243[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1244 = 0;
static const s_variable_target generated_targets_1244[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[2].specials) },
};

static const int8_t generated_default_1245 = 10;
static const s_variable_target generated_targets_1245[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1246 = 30;
static const s_variable_target generated_targets_1246[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1247 = 13;
static const s_variable_target generated_targets_1247[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[3].type) },
};

static const uint8_t generated_default_1248 = 15;
static const s_variable_target generated_targets_1248[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1249 = 0;
static const s_variable_target generated_targets_1249[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[3].specials) },
};

static const int8_t generated_default_1250 = 0;
static const s_variable_target generated_targets_1250[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1251 = 3;
static const s_variable_target generated_targets_1251[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1252 = 4;
static const s_variable_target generated_targets_1252[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[4].type) },
};

static const uint8_t generated_default_1253 = 1;
static const s_variable_target generated_targets_1253[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1254 = 4;
static const s_variable_target generated_targets_1254[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[4].specials) },
};

static const int8_t generated_default_1255 = 0;
static const s_variable_target generated_targets_1255[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1256 = 20;
static const s_variable_target generated_targets_1256[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1257 = 11;
static const s_variable_target generated_targets_1257[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[5].type) },
};

static const uint8_t generated_default_1258 = 1;
static const s_variable_target generated_targets_1258[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1259 = 2;
static const s_variable_target generated_targets_1259[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[5].specials) },
};

static const int8_t generated_default_1260 = 0;
static const s_variable_target generated_targets_1260[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1261 = 5;
static const s_variable_target generated_targets_1261[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1262 = 37;
static const s_variable_target generated_targets_1262[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[6].type) },
};

static const uint8_t generated_default_1263 = 15;
static const s_variable_target generated_targets_1263[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1264 = 0;
static const s_variable_target generated_targets_1264[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[6].specials) },
};

static const int8_t generated_default_1265 = 0;
static const s_variable_target generated_targets_1265[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1266 = 1;
static const s_variable_target generated_targets_1266[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1267 = 37;
static const s_variable_target generated_targets_1267[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[7].type) },
};

static const uint8_t generated_default_1268 = 15;
static const s_variable_target generated_targets_1268[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1269 = 0;
static const s_variable_target generated_targets_1269[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[7].specials) },
};

static const int8_t generated_default_1270 = 0;
static const s_variable_target generated_targets_1270[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1271 = 12;
static const s_variable_target generated_targets_1271[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[2][4].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1272[] = { 128, 0, 0, 5, 66 };
static const s_variable_target generated_targets_1272[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[2][4].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[2][4].special_device_flags) },
};

static const int8_t generated_default_1273 = 0;
static const s_variable_target generated_targets_1273[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].picture_num, sizeof(SHIP_CONFIG::antaran_templates[3][0].picture_num) },
};

static const uint8_t generated_default_1274 = 0;
static const s_variable_target generated_targets_1274[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].size, sizeof(SHIP_CONFIG::antaran_templates[3][0].size) },
};

static const uint8_t generated_default_1275 = 5;
static const s_variable_target generated_targets_1275[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[3][0].ftl_type) },
};

static const uint8_t generated_default_1276 = 6;
static const s_variable_target generated_targets_1276[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].armor_type, sizeof(SHIP_CONFIG::antaran_templates[3][0].armor_type) },
};

static const uint8_t generated_default_1277 = 0;
static const s_variable_target generated_targets_1277[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].shield_type, sizeof(SHIP_CONFIG::antaran_templates[3][0].shield_type) },
};

static const uint8_t generated_default_1278 = 5;
static const s_variable_target generated_targets_1278[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].computer_type, sizeof(SHIP_CONFIG::antaran_templates[3][0].computer_type) },
};

static const uint16_t generated_default_1279 = 4;
static const s_variable_target generated_targets_1279[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[0].type) },
};

static const uint8_t generated_default_1280 = 1;
static const s_variable_target generated_targets_1280[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_1281 = 0;
static const s_variable_target generated_targets_1281[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[0].specials) },
};

static const int8_t generated_default_1282 = 0;
static const s_variable_target generated_targets_1282[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1283 = 2;
static const s_variable_target generated_targets_1283[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1284 = 24;
static const s_variable_target generated_targets_1284[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[1].type) },
};

static const uint8_t generated_default_1285 = 15;
static const s_variable_target generated_targets_1285[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1286 = 0;
static const s_variable_target generated_targets_1286[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[1].specials) },
};

static const int8_t generated_default_1287 = 0;
static const s_variable_target generated_targets_1287[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1288 = 1;
static const s_variable_target generated_targets_1288[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1289 = 0;
static const s_variable_target generated_targets_1289[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[2].type) },
};

static const uint8_t generated_default_1290 = 0;
static const s_variable_target generated_targets_1290[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1291 = 0;
static const s_variable_target generated_targets_1291[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[2].specials) },
};

static const int8_t generated_default_1292 = 0;
static const s_variable_target generated_targets_1292[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1293 = 0;
static const s_variable_target generated_targets_1293[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1294 = 0;
static const s_variable_target generated_targets_1294[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[3].type) },
};

static const uint8_t generated_default_1295 = 0;
static const s_variable_target generated_targets_1295[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1296 = 0;
static const s_variable_target generated_targets_1296[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[3].specials) },
};

static const int8_t generated_default_1297 = 0;
static const s_variable_target generated_targets_1297[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1298 = 0;
static const s_variable_target generated_targets_1298[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1299 = 0;
static const s_variable_target generated_targets_1299[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[4].type) },
};

static const uint8_t generated_default_1300 = 0;
static const s_variable_target generated_targets_1300[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1301 = 0;
static const s_variable_target generated_targets_1301[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[4].specials) },
};

static const int8_t generated_default_1302 = 0;
static const s_variable_target generated_targets_1302[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1303 = 0;
static const s_variable_target generated_targets_1303[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1304 = 0;
static const s_variable_target generated_targets_1304[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[5].type) },
};

static const uint8_t generated_default_1305 = 0;
static const s_variable_target generated_targets_1305[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1306 = 0;
static const s_variable_target generated_targets_1306[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[5].specials) },
};

static const int8_t generated_default_1307 = 0;
static const s_variable_target generated_targets_1307[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1308 = 0;
static const s_variable_target generated_targets_1308[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1309 = 0;
static const s_variable_target generated_targets_1309[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[6].type) },
};

static const uint8_t generated_default_1310 = 0;
static const s_variable_target generated_targets_1310[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1311 = 0;
static const s_variable_target generated_targets_1311[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[6].specials) },
};

static const int8_t generated_default_1312 = 0;
static const s_variable_target generated_targets_1312[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1313 = 0;
static const s_variable_target generated_targets_1313[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1314 = 0;
static const s_variable_target generated_targets_1314[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[7].type) },
};

static const uint8_t generated_default_1315 = 0;
static const s_variable_target generated_targets_1315[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1316 = 0;
static const s_variable_target generated_targets_1316[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[7].specials) },
};

static const int8_t generated_default_1317 = 0;
static const s_variable_target generated_targets_1317[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1318 = 0;
static const s_variable_target generated_targets_1318[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][0].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1319[] = { 128, 2, 0, 1, 0 };
static const s_variable_target generated_targets_1319[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][0].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[3][0].special_device_flags) },
};

static const int8_t generated_default_1320 = 1;
static const s_variable_target generated_targets_1320[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].picture_num, sizeof(SHIP_CONFIG::antaran_templates[3][1].picture_num) },
};

static const uint8_t generated_default_1321 = 1;
static const s_variable_target generated_targets_1321[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].size, sizeof(SHIP_CONFIG::antaran_templates[3][1].size) },
};

static const uint8_t generated_default_1322 = 5;
static const s_variable_target generated_targets_1322[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[3][1].ftl_type) },
};

static const uint8_t generated_default_1323 = 6;
static const s_variable_target generated_targets_1323[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].armor_type, sizeof(SHIP_CONFIG::antaran_templates[3][1].armor_type) },
};

static const uint8_t generated_default_1324 = 0;
static const s_variable_target generated_targets_1324[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].shield_type, sizeof(SHIP_CONFIG::antaran_templates[3][1].shield_type) },
};

static const uint8_t generated_default_1325 = 5;
static const s_variable_target generated_targets_1325[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].computer_type, sizeof(SHIP_CONFIG::antaran_templates[3][1].computer_type) },
};

static const uint16_t generated_default_1326 = 4;
static const s_variable_target generated_targets_1326[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[0].type) },
};

static const uint8_t generated_default_1327 = 1;
static const s_variable_target generated_targets_1327[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_1328 = 0;
static const s_variable_target generated_targets_1328[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[0].specials) },
};

static const int8_t generated_default_1329 = 0;
static const s_variable_target generated_targets_1329[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1330 = 3;
static const s_variable_target generated_targets_1330[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1331 = 4;
static const s_variable_target generated_targets_1331[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[1].type) },
};

static const uint8_t generated_default_1332 = 1;
static const s_variable_target generated_targets_1332[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1333 = 2;
static const s_variable_target generated_targets_1333[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[1].specials) },
};

static const int8_t generated_default_1334 = 0;
static const s_variable_target generated_targets_1334[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1335 = 1;
static const s_variable_target generated_targets_1335[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1336 = 24;
static const s_variable_target generated_targets_1336[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[2].type) },
};

static const uint8_t generated_default_1337 = 15;
static const s_variable_target generated_targets_1337[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1338 = 0;
static const s_variable_target generated_targets_1338[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[2].specials) },
};

static const int8_t generated_default_1339 = 10;
static const s_variable_target generated_targets_1339[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1340 = 1;
static const s_variable_target generated_targets_1340[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1341 = 0;
static const s_variable_target generated_targets_1341[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[3].type) },
};

static const uint8_t generated_default_1342 = 0;
static const s_variable_target generated_targets_1342[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1343 = 0;
static const s_variable_target generated_targets_1343[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[3].specials) },
};

static const int8_t generated_default_1344 = 0;
static const s_variable_target generated_targets_1344[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1345 = 0;
static const s_variable_target generated_targets_1345[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1346 = 0;
static const s_variable_target generated_targets_1346[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[4].type) },
};

static const uint8_t generated_default_1347 = 0;
static const s_variable_target generated_targets_1347[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1348 = 0;
static const s_variable_target generated_targets_1348[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[4].specials) },
};

static const int8_t generated_default_1349 = 0;
static const s_variable_target generated_targets_1349[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1350 = 0;
static const s_variable_target generated_targets_1350[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1351 = 0;
static const s_variable_target generated_targets_1351[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[5].type) },
};

static const uint8_t generated_default_1352 = 0;
static const s_variable_target generated_targets_1352[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1353 = 0;
static const s_variable_target generated_targets_1353[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[5].specials) },
};

static const int8_t generated_default_1354 = 0;
static const s_variable_target generated_targets_1354[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1355 = 0;
static const s_variable_target generated_targets_1355[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1356 = 0;
static const s_variable_target generated_targets_1356[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[6].type) },
};

static const uint8_t generated_default_1357 = 0;
static const s_variable_target generated_targets_1357[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1358 = 0;
static const s_variable_target generated_targets_1358[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[6].specials) },
};

static const int8_t generated_default_1359 = 0;
static const s_variable_target generated_targets_1359[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1360 = 0;
static const s_variable_target generated_targets_1360[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1361 = 0;
static const s_variable_target generated_targets_1361[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[7].type) },
};

static const uint8_t generated_default_1362 = 0;
static const s_variable_target generated_targets_1362[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1363 = 0;
static const s_variable_target generated_targets_1363[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[7].specials) },
};

static const int8_t generated_default_1364 = 0;
static const s_variable_target generated_targets_1364[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1365 = 0;
static const s_variable_target generated_targets_1365[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][1].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1366[] = { 128, 2, 0, 1, 0 };
static const s_variable_target generated_targets_1366[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][1].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[3][1].special_device_flags) },
};

static const int8_t generated_default_1367 = 2;
static const s_variable_target generated_targets_1367[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].picture_num, sizeof(SHIP_CONFIG::antaran_templates[3][2].picture_num) },
};

static const uint8_t generated_default_1368 = 2;
static const s_variable_target generated_targets_1368[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].size, sizeof(SHIP_CONFIG::antaran_templates[3][2].size) },
};

static const uint8_t generated_default_1369 = 5;
static const s_variable_target generated_targets_1369[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[3][2].ftl_type) },
};

static const uint8_t generated_default_1370 = 6;
static const s_variable_target generated_targets_1370[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].armor_type, sizeof(SHIP_CONFIG::antaran_templates[3][2].armor_type) },
};

static const uint8_t generated_default_1371 = 0;
static const s_variable_target generated_targets_1371[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].shield_type, sizeof(SHIP_CONFIG::antaran_templates[3][2].shield_type) },
};

static const uint8_t generated_default_1372 = 5;
static const s_variable_target generated_targets_1372[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].computer_type, sizeof(SHIP_CONFIG::antaran_templates[3][2].computer_type) },
};

static const uint16_t generated_default_1373 = 4;
static const s_variable_target generated_targets_1373[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[0].type) },
};

static const uint8_t generated_default_1374 = 1;
static const s_variable_target generated_targets_1374[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_1375 = 0;
static const s_variable_target generated_targets_1375[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[0].specials) },
};

static const int8_t generated_default_1376 = 0;
static const s_variable_target generated_targets_1376[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1377 = 4;
static const s_variable_target generated_targets_1377[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1378 = 4;
static const s_variable_target generated_targets_1378[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[1].type) },
};

static const uint8_t generated_default_1379 = 1;
static const s_variable_target generated_targets_1379[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1380 = 2;
static const s_variable_target generated_targets_1380[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[1].specials) },
};

static const int8_t generated_default_1381 = 0;
static const s_variable_target generated_targets_1381[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1382 = 2;
static const s_variable_target generated_targets_1382[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1383 = 24;
static const s_variable_target generated_targets_1383[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[2].type) },
};

static const uint8_t generated_default_1384 = 15;
static const s_variable_target generated_targets_1384[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1385 = 0;
static const s_variable_target generated_targets_1385[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[2].specials) },
};

static const int8_t generated_default_1386 = 10;
static const s_variable_target generated_targets_1386[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1387 = 5;
static const s_variable_target generated_targets_1387[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1388 = 13;
static const s_variable_target generated_targets_1388[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[3].type) },
};

static const uint8_t generated_default_1389 = 15;
static const s_variable_target generated_targets_1389[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1390 = 0;
static const s_variable_target generated_targets_1390[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[3].specials) },
};

static const int8_t generated_default_1391 = 0;
static const s_variable_target generated_targets_1391[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1392 = 2;
static const s_variable_target generated_targets_1392[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1393 = 31;
static const s_variable_target generated_targets_1393[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[4].type) },
};

static const uint8_t generated_default_1394 = 15;
static const s_variable_target generated_targets_1394[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1395 = 0;
static const s_variable_target generated_targets_1395[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[4].specials) },
};

static const int8_t generated_default_1396 = 0;
static const s_variable_target generated_targets_1396[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1397 = 3;
static const s_variable_target generated_targets_1397[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1398 = 0;
static const s_variable_target generated_targets_1398[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[5].type) },
};

static const uint8_t generated_default_1399 = 0;
static const s_variable_target generated_targets_1399[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1400 = 0;
static const s_variable_target generated_targets_1400[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[5].specials) },
};

static const int8_t generated_default_1401 = 0;
static const s_variable_target generated_targets_1401[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1402 = 0;
static const s_variable_target generated_targets_1402[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1403 = 0;
static const s_variable_target generated_targets_1403[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[6].type) },
};

static const uint8_t generated_default_1404 = 0;
static const s_variable_target generated_targets_1404[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1405 = 0;
static const s_variable_target generated_targets_1405[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[6].specials) },
};

static const int8_t generated_default_1406 = 0;
static const s_variable_target generated_targets_1406[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1407 = 0;
static const s_variable_target generated_targets_1407[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1408 = 0;
static const s_variable_target generated_targets_1408[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[7].type) },
};

static const uint8_t generated_default_1409 = 0;
static const s_variable_target generated_targets_1409[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1410 = 0;
static const s_variable_target generated_targets_1410[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[7].specials) },
};

static const int8_t generated_default_1411 = 0;
static const s_variable_target generated_targets_1411[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1412 = 0;
static const s_variable_target generated_targets_1412[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][2].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1413[] = { 128, 0, 32, 1, 0 };
static const s_variable_target generated_targets_1413[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][2].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[3][2].special_device_flags) },
};

static const int8_t generated_default_1414 = 3;
static const s_variable_target generated_targets_1414[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].picture_num, sizeof(SHIP_CONFIG::antaran_templates[3][3].picture_num) },
};

static const uint8_t generated_default_1415 = 3;
static const s_variable_target generated_targets_1415[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].size, sizeof(SHIP_CONFIG::antaran_templates[3][3].size) },
};

static const uint8_t generated_default_1416 = 5;
static const s_variable_target generated_targets_1416[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[3][3].ftl_type) },
};

static const uint8_t generated_default_1417 = 6;
static const s_variable_target generated_targets_1417[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].armor_type, sizeof(SHIP_CONFIG::antaran_templates[3][3].armor_type) },
};

static const uint8_t generated_default_1418 = 0;
static const s_variable_target generated_targets_1418[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].shield_type, sizeof(SHIP_CONFIG::antaran_templates[3][3].shield_type) },
};

static const uint8_t generated_default_1419 = 5;
static const s_variable_target generated_targets_1419[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].computer_type, sizeof(SHIP_CONFIG::antaran_templates[3][3].computer_type) },
};

static const uint16_t generated_default_1420 = 4;
static const s_variable_target generated_targets_1420[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[0].type) },
};

static const uint8_t generated_default_1421 = 1;
static const s_variable_target generated_targets_1421[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_1422 = 0;
static const s_variable_target generated_targets_1422[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[0].specials) },
};

static const int8_t generated_default_1423 = 0;
static const s_variable_target generated_targets_1423[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1424 = 10;
static const s_variable_target generated_targets_1424[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1425 = 4;
static const s_variable_target generated_targets_1425[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[1].type) },
};

static const uint8_t generated_default_1426 = 1;
static const s_variable_target generated_targets_1426[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1427 = 2;
static const s_variable_target generated_targets_1427[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[1].specials) },
};

static const int8_t generated_default_1428 = 0;
static const s_variable_target generated_targets_1428[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1429 = 4;
static const s_variable_target generated_targets_1429[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1430 = 24;
static const s_variable_target generated_targets_1430[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[2].type) },
};

static const uint8_t generated_default_1431 = 1;
static const s_variable_target generated_targets_1431[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1432 = 0;
static const s_variable_target generated_targets_1432[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[2].specials) },
};

static const int8_t generated_default_1433 = 10;
static const s_variable_target generated_targets_1433[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1434 = 15;
static const s_variable_target generated_targets_1434[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1435 = 13;
static const s_variable_target generated_targets_1435[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[3].type) },
};

static const uint8_t generated_default_1436 = 15;
static const s_variable_target generated_targets_1436[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1437 = 0;
static const s_variable_target generated_targets_1437[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[3].specials) },
};

static const int8_t generated_default_1438 = 0;
static const s_variable_target generated_targets_1438[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1439 = 2;
static const s_variable_target generated_targets_1439[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1440 = 4;
static const s_variable_target generated_targets_1440[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[4].type) },
};

static const uint8_t generated_default_1441 = 1;
static const s_variable_target generated_targets_1441[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1442 = 4;
static const s_variable_target generated_targets_1442[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[4].specials) },
};

static const int8_t generated_default_1443 = 0;
static const s_variable_target generated_targets_1443[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1444 = 10;
static const s_variable_target generated_targets_1444[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1445 = 11;
static const s_variable_target generated_targets_1445[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[5].type) },
};

static const uint8_t generated_default_1446 = 1;
static const s_variable_target generated_targets_1446[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1447 = 0;
static const s_variable_target generated_targets_1447[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[5].specials) },
};

static const int8_t generated_default_1448 = 0;
static const s_variable_target generated_targets_1448[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1449 = 4;
static const s_variable_target generated_targets_1449[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1450 = 0;
static const s_variable_target generated_targets_1450[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[6].type) },
};

static const uint8_t generated_default_1451 = 0;
static const s_variable_target generated_targets_1451[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1452 = 0;
static const s_variable_target generated_targets_1452[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[6].specials) },
};

static const int8_t generated_default_1453 = 0;
static const s_variable_target generated_targets_1453[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1454 = 0;
static const s_variable_target generated_targets_1454[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1455 = 0;
static const s_variable_target generated_targets_1455[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[7].type) },
};

static const uint8_t generated_default_1456 = 0;
static const s_variable_target generated_targets_1456[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1457 = 0;
static const s_variable_target generated_targets_1457[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[7].specials) },
};

static const int8_t generated_default_1458 = 0;
static const s_variable_target generated_targets_1458[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1459 = 0;
static const s_variable_target generated_targets_1459[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][3].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1460[] = { 128, 0, 0, 5, 66 };
static const s_variable_target generated_targets_1460[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][3].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[3][3].special_device_flags) },
};

static const int8_t generated_default_1461 = 4;
static const s_variable_target generated_targets_1461[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].picture_num, sizeof(SHIP_CONFIG::antaran_templates[3][4].picture_num) },
};

static const uint8_t generated_default_1462 = 4;
static const s_variable_target generated_targets_1462[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].size, sizeof(SHIP_CONFIG::antaran_templates[3][4].size) },
};

static const uint8_t generated_default_1463 = 5;
static const s_variable_target generated_targets_1463[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[3][4].ftl_type) },
};

static const uint8_t generated_default_1464 = 6;
static const s_variable_target generated_targets_1464[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].armor_type, sizeof(SHIP_CONFIG::antaran_templates[3][4].armor_type) },
};

static const uint8_t generated_default_1465 = 0;
static const s_variable_target generated_targets_1465[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].shield_type, sizeof(SHIP_CONFIG::antaran_templates[3][4].shield_type) },
};

static const uint8_t generated_default_1466 = 5;
static const s_variable_target generated_targets_1466[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].computer_type, sizeof(SHIP_CONFIG::antaran_templates[3][4].computer_type) },
};

static const uint16_t generated_default_1467 = 4;
static const s_variable_target generated_targets_1467[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[0].type) },
};

static const uint8_t generated_default_1468 = 1;
static const s_variable_target generated_targets_1468[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_1469 = 0;
static const s_variable_target generated_targets_1469[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[0].specials) },
};

static const int8_t generated_default_1470 = 0;
static const s_variable_target generated_targets_1470[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1471 = 20;
static const s_variable_target generated_targets_1471[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1472 = 4;
static const s_variable_target generated_targets_1472[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[1].type) },
};

static const uint8_t generated_default_1473 = 1;
static const s_variable_target generated_targets_1473[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1474 = 2;
static const s_variable_target generated_targets_1474[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[1].specials) },
};

static const int8_t generated_default_1475 = 0;
static const s_variable_target generated_targets_1475[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1476 = 5;
static const s_variable_target generated_targets_1476[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1477 = 24;
static const s_variable_target generated_targets_1477[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[2].type) },
};

static const uint8_t generated_default_1478 = 1;
static const s_variable_target generated_targets_1478[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1479 = 0;
static const s_variable_target generated_targets_1479[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[2].specials) },
};

static const int8_t generated_default_1480 = 10;
static const s_variable_target generated_targets_1480[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1481 = 30;
static const s_variable_target generated_targets_1481[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1482 = 13;
static const s_variable_target generated_targets_1482[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[3].type) },
};

static const uint8_t generated_default_1483 = 15;
static const s_variable_target generated_targets_1483[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1484 = 0;
static const s_variable_target generated_targets_1484[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[3].specials) },
};

static const int8_t generated_default_1485 = 0;
static const s_variable_target generated_targets_1485[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1486 = 3;
static const s_variable_target generated_targets_1486[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1487 = 4;
static const s_variable_target generated_targets_1487[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[4].type) },
};

static const uint8_t generated_default_1488 = 1;
static const s_variable_target generated_targets_1488[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1489 = 4;
static const s_variable_target generated_targets_1489[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[4].specials) },
};

static const int8_t generated_default_1490 = 0;
static const s_variable_target generated_targets_1490[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1491 = 20;
static const s_variable_target generated_targets_1491[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1492 = 11;
static const s_variable_target generated_targets_1492[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[5].type) },
};

static const uint8_t generated_default_1493 = 1;
static const s_variable_target generated_targets_1493[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1494 = 2;
static const s_variable_target generated_targets_1494[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[5].specials) },
};

static const int8_t generated_default_1495 = 0;
static const s_variable_target generated_targets_1495[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1496 = 5;
static const s_variable_target generated_targets_1496[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1497 = 37;
static const s_variable_target generated_targets_1497[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[6].type) },
};

static const uint8_t generated_default_1498 = 15;
static const s_variable_target generated_targets_1498[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1499 = 0;
static const s_variable_target generated_targets_1499[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[6].specials) },
};

static const int8_t generated_default_1500 = 0;
static const s_variable_target generated_targets_1500[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1501 = 1;
static const s_variable_target generated_targets_1501[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1502 = 37;
static const s_variable_target generated_targets_1502[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[7].type) },
};

static const uint8_t generated_default_1503 = 15;
static const s_variable_target generated_targets_1503[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1504 = 0;
static const s_variable_target generated_targets_1504[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[7].specials) },
};

static const int8_t generated_default_1505 = 0;
static const s_variable_target generated_targets_1505[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1506 = 12;
static const s_variable_target generated_targets_1506[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[3][4].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1507[] = { 128, 0, 0, 5, 66 };
static const s_variable_target generated_targets_1507[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[3][4].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[3][4].special_device_flags) },
};

static const int8_t generated_default_1508 = 0;
static const s_variable_target generated_targets_1508[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].picture_num, sizeof(SHIP_CONFIG::antaran_templates[4][0].picture_num) },
};

static const uint8_t generated_default_1509 = 0;
static const s_variable_target generated_targets_1509[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].size, sizeof(SHIP_CONFIG::antaran_templates[4][0].size) },
};

static const uint8_t generated_default_1510 = 5;
static const s_variable_target generated_targets_1510[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[4][0].ftl_type) },
};

static const uint8_t generated_default_1511 = 6;
static const s_variable_target generated_targets_1511[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].armor_type, sizeof(SHIP_CONFIG::antaran_templates[4][0].armor_type) },
};

static const uint8_t generated_default_1512 = 0;
static const s_variable_target generated_targets_1512[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].shield_type, sizeof(SHIP_CONFIG::antaran_templates[4][0].shield_type) },
};

static const uint8_t generated_default_1513 = 5;
static const s_variable_target generated_targets_1513[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].computer_type, sizeof(SHIP_CONFIG::antaran_templates[4][0].computer_type) },
};

static const uint16_t generated_default_1514 = 4;
static const s_variable_target generated_targets_1514[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[0].type) },
};

static const uint8_t generated_default_1515 = 1;
static const s_variable_target generated_targets_1515[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_1516 = 0;
static const s_variable_target generated_targets_1516[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[0].specials) },
};

static const int8_t generated_default_1517 = 0;
static const s_variable_target generated_targets_1517[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1518 = 2;
static const s_variable_target generated_targets_1518[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1519 = 24;
static const s_variable_target generated_targets_1519[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[1].type) },
};

static const uint8_t generated_default_1520 = 15;
static const s_variable_target generated_targets_1520[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1521 = 0;
static const s_variable_target generated_targets_1521[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[1].specials) },
};

static const int8_t generated_default_1522 = 0;
static const s_variable_target generated_targets_1522[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1523 = 1;
static const s_variable_target generated_targets_1523[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1524 = 0;
static const s_variable_target generated_targets_1524[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[2].type) },
};

static const uint8_t generated_default_1525 = 0;
static const s_variable_target generated_targets_1525[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1526 = 0;
static const s_variable_target generated_targets_1526[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[2].specials) },
};

static const int8_t generated_default_1527 = 0;
static const s_variable_target generated_targets_1527[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1528 = 0;
static const s_variable_target generated_targets_1528[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1529 = 0;
static const s_variable_target generated_targets_1529[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[3].type) },
};

static const uint8_t generated_default_1530 = 0;
static const s_variable_target generated_targets_1530[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1531 = 0;
static const s_variable_target generated_targets_1531[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[3].specials) },
};

static const int8_t generated_default_1532 = 0;
static const s_variable_target generated_targets_1532[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1533 = 0;
static const s_variable_target generated_targets_1533[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1534 = 0;
static const s_variable_target generated_targets_1534[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[4].type) },
};

static const uint8_t generated_default_1535 = 0;
static const s_variable_target generated_targets_1535[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1536 = 0;
static const s_variable_target generated_targets_1536[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[4].specials) },
};

static const int8_t generated_default_1537 = 0;
static const s_variable_target generated_targets_1537[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1538 = 0;
static const s_variable_target generated_targets_1538[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1539 = 0;
static const s_variable_target generated_targets_1539[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[5].type) },
};

static const uint8_t generated_default_1540 = 0;
static const s_variable_target generated_targets_1540[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1541 = 0;
static const s_variable_target generated_targets_1541[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[5].specials) },
};

static const int8_t generated_default_1542 = 0;
static const s_variable_target generated_targets_1542[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1543 = 0;
static const s_variable_target generated_targets_1543[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1544 = 0;
static const s_variable_target generated_targets_1544[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[6].type) },
};

static const uint8_t generated_default_1545 = 0;
static const s_variable_target generated_targets_1545[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1546 = 0;
static const s_variable_target generated_targets_1546[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[6].specials) },
};

static const int8_t generated_default_1547 = 0;
static const s_variable_target generated_targets_1547[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1548 = 0;
static const s_variable_target generated_targets_1548[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1549 = 0;
static const s_variable_target generated_targets_1549[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[7].type) },
};

static const uint8_t generated_default_1550 = 0;
static const s_variable_target generated_targets_1550[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1551 = 0;
static const s_variable_target generated_targets_1551[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[7].specials) },
};

static const int8_t generated_default_1552 = 0;
static const s_variable_target generated_targets_1552[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1553 = 0;
static const s_variable_target generated_targets_1553[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][0].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1554[] = { 128, 2, 0, 1, 0 };
static const s_variable_target generated_targets_1554[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][0].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[4][0].special_device_flags) },
};

static const int8_t generated_default_1555 = 1;
static const s_variable_target generated_targets_1555[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].picture_num, sizeof(SHIP_CONFIG::antaran_templates[4][1].picture_num) },
};

static const uint8_t generated_default_1556 = 1;
static const s_variable_target generated_targets_1556[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].size, sizeof(SHIP_CONFIG::antaran_templates[4][1].size) },
};

static const uint8_t generated_default_1557 = 5;
static const s_variable_target generated_targets_1557[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[4][1].ftl_type) },
};

static const uint8_t generated_default_1558 = 6;
static const s_variable_target generated_targets_1558[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].armor_type, sizeof(SHIP_CONFIG::antaran_templates[4][1].armor_type) },
};

static const uint8_t generated_default_1559 = 0;
static const s_variable_target generated_targets_1559[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].shield_type, sizeof(SHIP_CONFIG::antaran_templates[4][1].shield_type) },
};

static const uint8_t generated_default_1560 = 5;
static const s_variable_target generated_targets_1560[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].computer_type, sizeof(SHIP_CONFIG::antaran_templates[4][1].computer_type) },
};

static const uint16_t generated_default_1561 = 4;
static const s_variable_target generated_targets_1561[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[0].type) },
};

static const uint8_t generated_default_1562 = 1;
static const s_variable_target generated_targets_1562[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_1563 = 0;
static const s_variable_target generated_targets_1563[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[0].specials) },
};

static const int8_t generated_default_1564 = 0;
static const s_variable_target generated_targets_1564[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1565 = 3;
static const s_variable_target generated_targets_1565[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1566 = 4;
static const s_variable_target generated_targets_1566[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[1].type) },
};

static const uint8_t generated_default_1567 = 1;
static const s_variable_target generated_targets_1567[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1568 = 2;
static const s_variable_target generated_targets_1568[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[1].specials) },
};

static const int8_t generated_default_1569 = 0;
static const s_variable_target generated_targets_1569[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1570 = 1;
static const s_variable_target generated_targets_1570[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1571 = 24;
static const s_variable_target generated_targets_1571[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[2].type) },
};

static const uint8_t generated_default_1572 = 15;
static const s_variable_target generated_targets_1572[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1573 = 0;
static const s_variable_target generated_targets_1573[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[2].specials) },
};

static const int8_t generated_default_1574 = 10;
static const s_variable_target generated_targets_1574[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1575 = 1;
static const s_variable_target generated_targets_1575[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1576 = 0;
static const s_variable_target generated_targets_1576[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[3].type) },
};

static const uint8_t generated_default_1577 = 0;
static const s_variable_target generated_targets_1577[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1578 = 0;
static const s_variable_target generated_targets_1578[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[3].specials) },
};

static const int8_t generated_default_1579 = 0;
static const s_variable_target generated_targets_1579[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1580 = 0;
static const s_variable_target generated_targets_1580[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1581 = 0;
static const s_variable_target generated_targets_1581[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[4].type) },
};

static const uint8_t generated_default_1582 = 0;
static const s_variable_target generated_targets_1582[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1583 = 0;
static const s_variable_target generated_targets_1583[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[4].specials) },
};

static const int8_t generated_default_1584 = 0;
static const s_variable_target generated_targets_1584[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1585 = 0;
static const s_variable_target generated_targets_1585[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1586 = 0;
static const s_variable_target generated_targets_1586[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[5].type) },
};

static const uint8_t generated_default_1587 = 0;
static const s_variable_target generated_targets_1587[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1588 = 0;
static const s_variable_target generated_targets_1588[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[5].specials) },
};

static const int8_t generated_default_1589 = 0;
static const s_variable_target generated_targets_1589[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1590 = 0;
static const s_variable_target generated_targets_1590[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1591 = 0;
static const s_variable_target generated_targets_1591[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[6].type) },
};

static const uint8_t generated_default_1592 = 0;
static const s_variable_target generated_targets_1592[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1593 = 0;
static const s_variable_target generated_targets_1593[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[6].specials) },
};

static const int8_t generated_default_1594 = 0;
static const s_variable_target generated_targets_1594[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1595 = 0;
static const s_variable_target generated_targets_1595[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1596 = 0;
static const s_variable_target generated_targets_1596[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[7].type) },
};

static const uint8_t generated_default_1597 = 0;
static const s_variable_target generated_targets_1597[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1598 = 0;
static const s_variable_target generated_targets_1598[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[7].specials) },
};

static const int8_t generated_default_1599 = 0;
static const s_variable_target generated_targets_1599[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1600 = 0;
static const s_variable_target generated_targets_1600[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][1].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1601[] = { 128, 0, 32, 1, 0 };
static const s_variable_target generated_targets_1601[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][1].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[4][1].special_device_flags) },
};

static const int8_t generated_default_1602 = 2;
static const s_variable_target generated_targets_1602[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].picture_num, sizeof(SHIP_CONFIG::antaran_templates[4][2].picture_num) },
};

static const uint8_t generated_default_1603 = 2;
static const s_variable_target generated_targets_1603[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].size, sizeof(SHIP_CONFIG::antaran_templates[4][2].size) },
};

static const uint8_t generated_default_1604 = 5;
static const s_variable_target generated_targets_1604[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[4][2].ftl_type) },
};

static const uint8_t generated_default_1605 = 6;
static const s_variable_target generated_targets_1605[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].armor_type, sizeof(SHIP_CONFIG::antaran_templates[4][2].armor_type) },
};

static const uint8_t generated_default_1606 = 0;
static const s_variable_target generated_targets_1606[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].shield_type, sizeof(SHIP_CONFIG::antaran_templates[4][2].shield_type) },
};

static const uint8_t generated_default_1607 = 5;
static const s_variable_target generated_targets_1607[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].computer_type, sizeof(SHIP_CONFIG::antaran_templates[4][2].computer_type) },
};

static const uint16_t generated_default_1608 = 4;
static const s_variable_target generated_targets_1608[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[0].type) },
};

static const uint8_t generated_default_1609 = 1;
static const s_variable_target generated_targets_1609[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_1610 = 0;
static const s_variable_target generated_targets_1610[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[0].specials) },
};

static const int8_t generated_default_1611 = 0;
static const s_variable_target generated_targets_1611[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1612 = 4;
static const s_variable_target generated_targets_1612[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1613 = 4;
static const s_variable_target generated_targets_1613[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[1].type) },
};

static const uint8_t generated_default_1614 = 1;
static const s_variable_target generated_targets_1614[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1615 = 2;
static const s_variable_target generated_targets_1615[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[1].specials) },
};

static const int8_t generated_default_1616 = 0;
static const s_variable_target generated_targets_1616[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1617 = 2;
static const s_variable_target generated_targets_1617[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1618 = 24;
static const s_variable_target generated_targets_1618[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[2].type) },
};

static const uint8_t generated_default_1619 = 15;
static const s_variable_target generated_targets_1619[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1620 = 0;
static const s_variable_target generated_targets_1620[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[2].specials) },
};

static const int8_t generated_default_1621 = 10;
static const s_variable_target generated_targets_1621[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1622 = 5;
static const s_variable_target generated_targets_1622[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1623 = 13;
static const s_variable_target generated_targets_1623[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[3].type) },
};

static const uint8_t generated_default_1624 = 15;
static const s_variable_target generated_targets_1624[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1625 = 0;
static const s_variable_target generated_targets_1625[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[3].specials) },
};

static const int8_t generated_default_1626 = 0;
static const s_variable_target generated_targets_1626[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1627 = 2;
static const s_variable_target generated_targets_1627[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1628 = 31;
static const s_variable_target generated_targets_1628[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[4].type) },
};

static const uint8_t generated_default_1629 = 15;
static const s_variable_target generated_targets_1629[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1630 = 0;
static const s_variable_target generated_targets_1630[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[4].specials) },
};

static const int8_t generated_default_1631 = 0;
static const s_variable_target generated_targets_1631[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1632 = 3;
static const s_variable_target generated_targets_1632[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1633 = 0;
static const s_variable_target generated_targets_1633[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[5].type) },
};

static const uint8_t generated_default_1634 = 0;
static const s_variable_target generated_targets_1634[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1635 = 0;
static const s_variable_target generated_targets_1635[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[5].specials) },
};

static const int8_t generated_default_1636 = 0;
static const s_variable_target generated_targets_1636[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1637 = 0;
static const s_variable_target generated_targets_1637[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1638 = 0;
static const s_variable_target generated_targets_1638[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[6].type) },
};

static const uint8_t generated_default_1639 = 0;
static const s_variable_target generated_targets_1639[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1640 = 0;
static const s_variable_target generated_targets_1640[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[6].specials) },
};

static const int8_t generated_default_1641 = 0;
static const s_variable_target generated_targets_1641[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1642 = 0;
static const s_variable_target generated_targets_1642[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1643 = 0;
static const s_variable_target generated_targets_1643[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[7].type) },
};

static const uint8_t generated_default_1644 = 0;
static const s_variable_target generated_targets_1644[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1645 = 0;
static const s_variable_target generated_targets_1645[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[7].specials) },
};

static const int8_t generated_default_1646 = 0;
static const s_variable_target generated_targets_1646[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1647 = 0;
static const s_variable_target generated_targets_1647[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][2].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1648[] = { 128, 0, 0, 1, 64 };
static const s_variable_target generated_targets_1648[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][2].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[4][2].special_device_flags) },
};

static const int8_t generated_default_1649 = 3;
static const s_variable_target generated_targets_1649[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].picture_num, sizeof(SHIP_CONFIG::antaran_templates[4][3].picture_num) },
};

static const uint8_t generated_default_1650 = 3;
static const s_variable_target generated_targets_1650[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].size, sizeof(SHIP_CONFIG::antaran_templates[4][3].size) },
};

static const uint8_t generated_default_1651 = 5;
static const s_variable_target generated_targets_1651[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[4][3].ftl_type) },
};

static const uint8_t generated_default_1652 = 6;
static const s_variable_target generated_targets_1652[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].armor_type, sizeof(SHIP_CONFIG::antaran_templates[4][3].armor_type) },
};

static const uint8_t generated_default_1653 = 0;
static const s_variable_target generated_targets_1653[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].shield_type, sizeof(SHIP_CONFIG::antaran_templates[4][3].shield_type) },
};

static const uint8_t generated_default_1654 = 5;
static const s_variable_target generated_targets_1654[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].computer_type, sizeof(SHIP_CONFIG::antaran_templates[4][3].computer_type) },
};

static const uint16_t generated_default_1655 = 4;
static const s_variable_target generated_targets_1655[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[0].type) },
};

static const uint8_t generated_default_1656 = 1;
static const s_variable_target generated_targets_1656[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_1657 = 0;
static const s_variable_target generated_targets_1657[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[0].specials) },
};

static const int8_t generated_default_1658 = 0;
static const s_variable_target generated_targets_1658[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1659 = 10;
static const s_variable_target generated_targets_1659[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1660 = 4;
static const s_variable_target generated_targets_1660[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[1].type) },
};

static const uint8_t generated_default_1661 = 1;
static const s_variable_target generated_targets_1661[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1662 = 2;
static const s_variable_target generated_targets_1662[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[1].specials) },
};

static const int8_t generated_default_1663 = 0;
static const s_variable_target generated_targets_1663[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1664 = 4;
static const s_variable_target generated_targets_1664[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1665 = 24;
static const s_variable_target generated_targets_1665[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[2].type) },
};

static const uint8_t generated_default_1666 = 1;
static const s_variable_target generated_targets_1666[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1667 = 0;
static const s_variable_target generated_targets_1667[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[2].specials) },
};

static const int8_t generated_default_1668 = 10;
static const s_variable_target generated_targets_1668[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1669 = 15;
static const s_variable_target generated_targets_1669[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1670 = 13;
static const s_variable_target generated_targets_1670[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[3].type) },
};

static const uint8_t generated_default_1671 = 15;
static const s_variable_target generated_targets_1671[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1672 = 0;
static const s_variable_target generated_targets_1672[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[3].specials) },
};

static const int8_t generated_default_1673 = 0;
static const s_variable_target generated_targets_1673[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1674 = 2;
static const s_variable_target generated_targets_1674[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1675 = 4;
static const s_variable_target generated_targets_1675[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[4].type) },
};

static const uint8_t generated_default_1676 = 1;
static const s_variable_target generated_targets_1676[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1677 = 4;
static const s_variable_target generated_targets_1677[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[4].specials) },
};

static const int8_t generated_default_1678 = 0;
static const s_variable_target generated_targets_1678[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1679 = 10;
static const s_variable_target generated_targets_1679[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1680 = 11;
static const s_variable_target generated_targets_1680[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[5].type) },
};

static const uint8_t generated_default_1681 = 1;
static const s_variable_target generated_targets_1681[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1682 = 0;
static const s_variable_target generated_targets_1682[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[5].specials) },
};

static const int8_t generated_default_1683 = 0;
static const s_variable_target generated_targets_1683[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1684 = 4;
static const s_variable_target generated_targets_1684[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1685 = 0;
static const s_variable_target generated_targets_1685[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[6].type) },
};

static const uint8_t generated_default_1686 = 0;
static const s_variable_target generated_targets_1686[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1687 = 0;
static const s_variable_target generated_targets_1687[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[6].specials) },
};

static const int8_t generated_default_1688 = 0;
static const s_variable_target generated_targets_1688[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1689 = 0;
static const s_variable_target generated_targets_1689[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1690 = 0;
static const s_variable_target generated_targets_1690[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[7].type) },
};

static const uint8_t generated_default_1691 = 0;
static const s_variable_target generated_targets_1691[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1692 = 0;
static const s_variable_target generated_targets_1692[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[7].specials) },
};

static const int8_t generated_default_1693 = 0;
static const s_variable_target generated_targets_1693[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1694 = 0;
static const s_variable_target generated_targets_1694[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][3].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1695[] = { 128, 0, 0, 5, 66 };
static const s_variable_target generated_targets_1695[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][3].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[4][3].special_device_flags) },
};

static const int8_t generated_default_1696 = 4;
static const s_variable_target generated_targets_1696[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].picture_num, sizeof(SHIP_CONFIG::antaran_templates[4][4].picture_num) },
};

static const uint8_t generated_default_1697 = 4;
static const s_variable_target generated_targets_1697[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].size, sizeof(SHIP_CONFIG::antaran_templates[4][4].size) },
};

static const uint8_t generated_default_1698 = 5;
static const s_variable_target generated_targets_1698[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ftl_type, sizeof(SHIP_CONFIG::antaran_templates[4][4].ftl_type) },
};

static const uint8_t generated_default_1699 = 6;
static const s_variable_target generated_targets_1699[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].armor_type, sizeof(SHIP_CONFIG::antaran_templates[4][4].armor_type) },
};

static const uint8_t generated_default_1700 = 0;
static const s_variable_target generated_targets_1700[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].shield_type, sizeof(SHIP_CONFIG::antaran_templates[4][4].shield_type) },
};

static const uint8_t generated_default_1701 = 5;
static const s_variable_target generated_targets_1701[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].computer_type, sizeof(SHIP_CONFIG::antaran_templates[4][4].computer_type) },
};

static const uint16_t generated_default_1702 = 4;
static const s_variable_target generated_targets_1702[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[0].type, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[0].type) },
};

static const uint8_t generated_default_1703 = 1;
static const s_variable_target generated_targets_1703[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[0].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[0].firing_arc) },
};

static const int16_t generated_default_1704 = 0;
static const s_variable_target generated_targets_1704[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[0].specials, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[0].specials) },
};

static const int8_t generated_default_1705 = 0;
static const s_variable_target generated_targets_1705[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[0].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[0].ammo) },
};

static const int8_t generated_default_1706 = 20;
static const s_variable_target generated_targets_1706[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[0].count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[0].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[0].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[0].current_count) },
};

static const uint16_t generated_default_1707 = 4;
static const s_variable_target generated_targets_1707[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[1].type, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[1].type) },
};

static const uint8_t generated_default_1708 = 1;
static const s_variable_target generated_targets_1708[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[1].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[1].firing_arc) },
};

static const int16_t generated_default_1709 = 2;
static const s_variable_target generated_targets_1709[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[1].specials, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[1].specials) },
};

static const int8_t generated_default_1710 = 0;
static const s_variable_target generated_targets_1710[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[1].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[1].ammo) },
};

static const int8_t generated_default_1711 = 5;
static const s_variable_target generated_targets_1711[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[1].count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[1].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[1].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[1].current_count) },
};

static const uint16_t generated_default_1712 = 24;
static const s_variable_target generated_targets_1712[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[2].type, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[2].type) },
};

static const uint8_t generated_default_1713 = 1;
static const s_variable_target generated_targets_1713[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[2].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[2].firing_arc) },
};

static const int16_t generated_default_1714 = 0;
static const s_variable_target generated_targets_1714[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[2].specials, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[2].specials) },
};

static const int8_t generated_default_1715 = 10;
static const s_variable_target generated_targets_1715[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[2].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[2].ammo) },
};

static const int8_t generated_default_1716 = 30;
static const s_variable_target generated_targets_1716[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[2].count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[2].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[2].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[2].current_count) },
};

static const uint16_t generated_default_1717 = 13;
static const s_variable_target generated_targets_1717[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[3].type, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[3].type) },
};

static const uint8_t generated_default_1718 = 15;
static const s_variable_target generated_targets_1718[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[3].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[3].firing_arc) },
};

static const int16_t generated_default_1719 = 0;
static const s_variable_target generated_targets_1719[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[3].specials, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[3].specials) },
};

static const int8_t generated_default_1720 = 0;
static const s_variable_target generated_targets_1720[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[3].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[3].ammo) },
};

static const int8_t generated_default_1721 = 3;
static const s_variable_target generated_targets_1721[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[3].count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[3].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[3].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[3].current_count) },
};

static const uint16_t generated_default_1722 = 4;
static const s_variable_target generated_targets_1722[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[4].type, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[4].type) },
};

static const uint8_t generated_default_1723 = 1;
static const s_variable_target generated_targets_1723[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[4].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[4].firing_arc) },
};

static const int16_t generated_default_1724 = 4;
static const s_variable_target generated_targets_1724[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[4].specials, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[4].specials) },
};

static const int8_t generated_default_1725 = 0;
static const s_variable_target generated_targets_1725[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[4].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[4].ammo) },
};

static const int8_t generated_default_1726 = 20;
static const s_variable_target generated_targets_1726[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[4].count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[4].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[4].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[4].current_count) },
};

static const uint16_t generated_default_1727 = 11;
static const s_variable_target generated_targets_1727[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[5].type, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[5].type) },
};

static const uint8_t generated_default_1728 = 1;
static const s_variable_target generated_targets_1728[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[5].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[5].firing_arc) },
};

static const int16_t generated_default_1729 = 2;
static const s_variable_target generated_targets_1729[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[5].specials, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[5].specials) },
};

static const int8_t generated_default_1730 = 0;
static const s_variable_target generated_targets_1730[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[5].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[5].ammo) },
};

static const int8_t generated_default_1731 = 5;
static const s_variable_target generated_targets_1731[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[5].count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[5].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[5].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[5].current_count) },
};

static const uint16_t generated_default_1732 = 37;
static const s_variable_target generated_targets_1732[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[6].type, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[6].type) },
};

static const uint8_t generated_default_1733 = 15;
static const s_variable_target generated_targets_1733[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[6].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[6].firing_arc) },
};

static const int16_t generated_default_1734 = 0;
static const s_variable_target generated_targets_1734[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[6].specials, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[6].specials) },
};

static const int8_t generated_default_1735 = 0;
static const s_variable_target generated_targets_1735[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[6].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[6].ammo) },
};

static const int8_t generated_default_1736 = 1;
static const s_variable_target generated_targets_1736[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[6].count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[6].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[6].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[6].current_count) },
};

static const uint16_t generated_default_1737 = 37;
static const s_variable_target generated_targets_1737[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[7].type, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[7].type) },
};

static const uint8_t generated_default_1738 = 15;
static const s_variable_target generated_targets_1738[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[7].firing_arc, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[7].firing_arc) },
};

static const int16_t generated_default_1739 = 0;
static const s_variable_target generated_targets_1739[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[7].specials, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[7].specials) },
};

static const int8_t generated_default_1740 = 0;
static const s_variable_target generated_targets_1740[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[7].ammo, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[7].ammo) },
};

static const int8_t generated_default_1741 = 12;
static const s_variable_target generated_targets_1741[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[7].count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[7].count) },
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].ship_weapon[7].current_count, sizeof(SHIP_CONFIG::antaran_templates[4][4].ship_weapon[7].current_count) },
};

static const uint8_t generated_default_1742[] = { 128, 0, 0, 5, 66 };
static const s_variable_target generated_targets_1742[] = {
    { (void*)&SHIP_CONFIG::antaran_templates[4][4].special_device_flags, sizeof(SHIP_CONFIG::antaran_templates[4][4].special_device_flags) },
};

const s_variable_descriptor variables[] = {
    { "music_enabled", &generated_type_0, &generated_default_0, generated_targets_0, 1 },
    { "music_volume", &generated_type_1, &generated_default_1, generated_targets_1, 1 },
    { "sound_effects_enabled", &generated_type_0, &generated_default_2, generated_targets_2, 1 },
    { "sound_effects_volume", &generated_type_1, &generated_default_3, generated_targets_3, 1 },
    { "ships.guardian.appearance", &generated_type_2, &generated_default_4, generated_targets_4, 1 },
    { "ships.guardian.hull_size", &generated_type_3, &generated_default_5, generated_targets_5, 1 },
    { "ships.guardian.drive", &generated_type_4, &generated_default_6, generated_targets_6, 1 },
    { "ships.guardian.armor", &generated_type_5, &generated_default_7, generated_targets_7, 1 },
    { "ships.guardian.shield", &generated_type_6, &generated_default_8, generated_targets_8, 1 },
    { "ships.guardian.computer", &generated_type_7, &generated_default_9, generated_targets_9, 1 },
    { "ships.guardian.weapons.1.type", &generated_type_8, &generated_default_10, generated_targets_10, 1 },
    { "ships.guardian.weapons.1.arc", &generated_type_9, &generated_default_11, generated_targets_11, 1 },
    { "ships.guardian.weapons.1.mods", &generated_type_10, &generated_default_12, generated_targets_12, 1 },
    { "ships.guardian.weapons.1.ammo", &generated_type_2, &generated_default_13, generated_targets_13, 1 },
    { "ships.guardian.weapons.1.amount", &generated_type_11, &generated_default_14, generated_targets_14, 2 },
    { "ships.guardian.weapons.2.type", &generated_type_8, &generated_default_15, generated_targets_15, 1 },
    { "ships.guardian.weapons.2.arc", &generated_type_9, &generated_default_16, generated_targets_16, 1 },
    { "ships.guardian.weapons.2.mods", &generated_type_10, &generated_default_17, generated_targets_17, 1 },
    { "ships.guardian.weapons.2.ammo", &generated_type_2, &generated_default_18, generated_targets_18, 1 },
    { "ships.guardian.weapons.2.amount", &generated_type_11, &generated_default_19, generated_targets_19, 2 },
    { "ships.guardian.weapons.3.type", &generated_type_8, &generated_default_20, generated_targets_20, 1 },
    { "ships.guardian.weapons.3.arc", &generated_type_9, &generated_default_21, generated_targets_21, 1 },
    { "ships.guardian.weapons.3.mods", &generated_type_10, &generated_default_22, generated_targets_22, 1 },
    { "ships.guardian.weapons.3.ammo", &generated_type_2, &generated_default_23, generated_targets_23, 1 },
    { "ships.guardian.weapons.3.amount", &generated_type_11, &generated_default_24, generated_targets_24, 2 },
    { "ships.guardian.weapons.4.type", &generated_type_8, &generated_default_25, generated_targets_25, 1 },
    { "ships.guardian.weapons.4.arc", &generated_type_9, &generated_default_26, generated_targets_26, 1 },
    { "ships.guardian.weapons.4.mods", &generated_type_10, &generated_default_27, generated_targets_27, 1 },
    { "ships.guardian.weapons.4.ammo", &generated_type_2, &generated_default_28, generated_targets_28, 1 },
    { "ships.guardian.weapons.4.amount", &generated_type_11, &generated_default_29, generated_targets_29, 2 },
    { "ships.guardian.weapons.5.type", &generated_type_8, &generated_default_30, generated_targets_30, 1 },
    { "ships.guardian.weapons.5.arc", &generated_type_9, &generated_default_31, generated_targets_31, 1 },
    { "ships.guardian.weapons.5.mods", &generated_type_10, &generated_default_32, generated_targets_32, 1 },
    { "ships.guardian.weapons.5.ammo", &generated_type_2, &generated_default_33, generated_targets_33, 1 },
    { "ships.guardian.weapons.5.amount", &generated_type_11, &generated_default_34, generated_targets_34, 2 },
    { "ships.guardian.weapons.6.type", &generated_type_8, &generated_default_35, generated_targets_35, 1 },
    { "ships.guardian.weapons.6.arc", &generated_type_9, &generated_default_36, generated_targets_36, 1 },
    { "ships.guardian.weapons.6.mods", &generated_type_10, &generated_default_37, generated_targets_37, 1 },
    { "ships.guardian.weapons.6.ammo", &generated_type_2, &generated_default_38, generated_targets_38, 1 },
    { "ships.guardian.weapons.6.amount", &generated_type_11, &generated_default_39, generated_targets_39, 2 },
    { "ships.guardian.weapons.7.type", &generated_type_8, &generated_default_40, generated_targets_40, 1 },
    { "ships.guardian.weapons.7.arc", &generated_type_9, &generated_default_41, generated_targets_41, 1 },
    { "ships.guardian.weapons.7.mods", &generated_type_10, &generated_default_42, generated_targets_42, 1 },
    { "ships.guardian.weapons.7.ammo", &generated_type_2, &generated_default_43, generated_targets_43, 1 },
    { "ships.guardian.weapons.7.amount", &generated_type_11, &generated_default_44, generated_targets_44, 2 },
    { "ships.guardian.weapons.8.type", &generated_type_8, &generated_default_45, generated_targets_45, 1 },
    { "ships.guardian.weapons.8.arc", &generated_type_9, &generated_default_46, generated_targets_46, 1 },
    { "ships.guardian.weapons.8.mods", &generated_type_10, &generated_default_47, generated_targets_47, 1 },
    { "ships.guardian.weapons.8.ammo", &generated_type_2, &generated_default_48, generated_targets_48, 1 },
    { "ships.guardian.weapons.8.amount", &generated_type_11, &generated_default_49, generated_targets_49, 2 },
    { "ships.guardian.specials", &generated_type_12, &generated_default_50, generated_targets_50, 1 },
    { "ships.avenger.appearance", &generated_type_2, &generated_default_51, generated_targets_51, 1 },
    { "ships.avenger.hull_size", &generated_type_3, &generated_default_52, generated_targets_52, 1 },
    { "ships.avenger.drive", &generated_type_4, &generated_default_53, generated_targets_53, 1 },
    { "ships.avenger.armor", &generated_type_5, &generated_default_54, generated_targets_54, 1 },
    { "ships.avenger.shield", &generated_type_6, &generated_default_55, generated_targets_55, 1 },
    { "ships.avenger.computer", &generated_type_7, &generated_default_56, generated_targets_56, 1 },
    { "ships.avenger.weapons.1.type", &generated_type_8, &generated_default_57, generated_targets_57, 1 },
    { "ships.avenger.weapons.1.arc", &generated_type_9, &generated_default_58, generated_targets_58, 1 },
    { "ships.avenger.weapons.1.mods", &generated_type_10, &generated_default_59, generated_targets_59, 1 },
    { "ships.avenger.weapons.1.ammo", &generated_type_2, &generated_default_60, generated_targets_60, 1 },
    { "ships.avenger.weapons.1.amount", &generated_type_11, &generated_default_61, generated_targets_61, 2 },
    { "ships.avenger.weapons.2.type", &generated_type_8, &generated_default_62, generated_targets_62, 1 },
    { "ships.avenger.weapons.2.arc", &generated_type_9, &generated_default_63, generated_targets_63, 1 },
    { "ships.avenger.weapons.2.mods", &generated_type_10, &generated_default_64, generated_targets_64, 1 },
    { "ships.avenger.weapons.2.ammo", &generated_type_2, &generated_default_65, generated_targets_65, 1 },
    { "ships.avenger.weapons.2.amount", &generated_type_11, &generated_default_66, generated_targets_66, 2 },
    { "ships.avenger.weapons.3.type", &generated_type_8, &generated_default_67, generated_targets_67, 1 },
    { "ships.avenger.weapons.3.arc", &generated_type_9, &generated_default_68, generated_targets_68, 1 },
    { "ships.avenger.weapons.3.mods", &generated_type_10, &generated_default_69, generated_targets_69, 1 },
    { "ships.avenger.weapons.3.ammo", &generated_type_2, &generated_default_70, generated_targets_70, 1 },
    { "ships.avenger.weapons.3.amount", &generated_type_11, &generated_default_71, generated_targets_71, 2 },
    { "ships.avenger.weapons.4.type", &generated_type_8, &generated_default_72, generated_targets_72, 1 },
    { "ships.avenger.weapons.4.arc", &generated_type_9, &generated_default_73, generated_targets_73, 1 },
    { "ships.avenger.weapons.4.mods", &generated_type_10, &generated_default_74, generated_targets_74, 1 },
    { "ships.avenger.weapons.4.ammo", &generated_type_2, &generated_default_75, generated_targets_75, 1 },
    { "ships.avenger.weapons.4.amount", &generated_type_11, &generated_default_76, generated_targets_76, 2 },
    { "ships.avenger.weapons.5.type", &generated_type_8, &generated_default_77, generated_targets_77, 1 },
    { "ships.avenger.weapons.5.arc", &generated_type_9, &generated_default_78, generated_targets_78, 1 },
    { "ships.avenger.weapons.5.mods", &generated_type_10, &generated_default_79, generated_targets_79, 1 },
    { "ships.avenger.weapons.5.ammo", &generated_type_2, &generated_default_80, generated_targets_80, 1 },
    { "ships.avenger.weapons.5.amount", &generated_type_11, &generated_default_81, generated_targets_81, 2 },
    { "ships.avenger.weapons.6.type", &generated_type_8, &generated_default_82, generated_targets_82, 1 },
    { "ships.avenger.weapons.6.arc", &generated_type_9, &generated_default_83, generated_targets_83, 1 },
    { "ships.avenger.weapons.6.mods", &generated_type_10, &generated_default_84, generated_targets_84, 1 },
    { "ships.avenger.weapons.6.ammo", &generated_type_2, &generated_default_85, generated_targets_85, 1 },
    { "ships.avenger.weapons.6.amount", &generated_type_11, &generated_default_86, generated_targets_86, 2 },
    { "ships.avenger.weapons.7.type", &generated_type_8, &generated_default_87, generated_targets_87, 1 },
    { "ships.avenger.weapons.7.arc", &generated_type_9, &generated_default_88, generated_targets_88, 1 },
    { "ships.avenger.weapons.7.mods", &generated_type_10, &generated_default_89, generated_targets_89, 1 },
    { "ships.avenger.weapons.7.ammo", &generated_type_2, &generated_default_90, generated_targets_90, 1 },
    { "ships.avenger.weapons.7.amount", &generated_type_11, &generated_default_91, generated_targets_91, 2 },
    { "ships.avenger.weapons.8.type", &generated_type_8, &generated_default_92, generated_targets_92, 1 },
    { "ships.avenger.weapons.8.arc", &generated_type_9, &generated_default_93, generated_targets_93, 1 },
    { "ships.avenger.weapons.8.mods", &generated_type_10, &generated_default_94, generated_targets_94, 1 },
    { "ships.avenger.weapons.8.ammo", &generated_type_2, &generated_default_95, generated_targets_95, 1 },
    { "ships.avenger.weapons.8.amount", &generated_type_11, &generated_default_96, generated_targets_96, 2 },
    { "ships.avenger.specials", &generated_type_12, &generated_default_97, generated_targets_97, 1 },
    { "ships.amoeba.0.appearance", &generated_type_2, &generated_default_98, generated_targets_98, 1 },
    { "ships.amoeba.0.hull_size", &generated_type_3, &generated_default_99, generated_targets_99, 1 },
    { "ships.amoeba.0.drive", &generated_type_4, &generated_default_100, generated_targets_100, 1 },
    { "ships.amoeba.0.armor", &generated_type_5, &generated_default_101, generated_targets_101, 1 },
    { "ships.amoeba.0.shield", &generated_type_6, &generated_default_102, generated_targets_102, 1 },
    { "ships.amoeba.0.computer", &generated_type_7, &generated_default_103, generated_targets_103, 1 },
    { "ships.amoeba.0.weapons.1.type", &generated_type_8, &generated_default_104, generated_targets_104, 1 },
    { "ships.amoeba.0.weapons.1.arc", &generated_type_9, &generated_default_105, generated_targets_105, 1 },
    { "ships.amoeba.0.weapons.1.mods", &generated_type_10, &generated_default_106, generated_targets_106, 1 },
    { "ships.amoeba.0.weapons.1.ammo", &generated_type_2, &generated_default_107, generated_targets_107, 1 },
    { "ships.amoeba.0.weapons.1.amount", &generated_type_11, &generated_default_108, generated_targets_108, 2 },
    { "ships.amoeba.0.weapons.2.type", &generated_type_8, &generated_default_109, generated_targets_109, 1 },
    { "ships.amoeba.0.weapons.2.arc", &generated_type_9, &generated_default_110, generated_targets_110, 1 },
    { "ships.amoeba.0.weapons.2.mods", &generated_type_10, &generated_default_111, generated_targets_111, 1 },
    { "ships.amoeba.0.weapons.2.ammo", &generated_type_2, &generated_default_112, generated_targets_112, 1 },
    { "ships.amoeba.0.weapons.2.amount", &generated_type_11, &generated_default_113, generated_targets_113, 2 },
    { "ships.amoeba.0.weapons.3.type", &generated_type_8, &generated_default_114, generated_targets_114, 1 },
    { "ships.amoeba.0.weapons.3.arc", &generated_type_9, &generated_default_115, generated_targets_115, 1 },
    { "ships.amoeba.0.weapons.3.mods", &generated_type_10, &generated_default_116, generated_targets_116, 1 },
    { "ships.amoeba.0.weapons.3.ammo", &generated_type_2, &generated_default_117, generated_targets_117, 1 },
    { "ships.amoeba.0.weapons.3.amount", &generated_type_11, &generated_default_118, generated_targets_118, 2 },
    { "ships.amoeba.0.weapons.4.type", &generated_type_8, &generated_default_119, generated_targets_119, 1 },
    { "ships.amoeba.0.weapons.4.arc", &generated_type_9, &generated_default_120, generated_targets_120, 1 },
    { "ships.amoeba.0.weapons.4.mods", &generated_type_10, &generated_default_121, generated_targets_121, 1 },
    { "ships.amoeba.0.weapons.4.ammo", &generated_type_2, &generated_default_122, generated_targets_122, 1 },
    { "ships.amoeba.0.weapons.4.amount", &generated_type_11, &generated_default_123, generated_targets_123, 2 },
    { "ships.amoeba.0.weapons.5.type", &generated_type_8, &generated_default_124, generated_targets_124, 1 },
    { "ships.amoeba.0.weapons.5.arc", &generated_type_9, &generated_default_125, generated_targets_125, 1 },
    { "ships.amoeba.0.weapons.5.mods", &generated_type_10, &generated_default_126, generated_targets_126, 1 },
    { "ships.amoeba.0.weapons.5.ammo", &generated_type_2, &generated_default_127, generated_targets_127, 1 },
    { "ships.amoeba.0.weapons.5.amount", &generated_type_11, &generated_default_128, generated_targets_128, 2 },
    { "ships.amoeba.0.weapons.6.type", &generated_type_8, &generated_default_129, generated_targets_129, 1 },
    { "ships.amoeba.0.weapons.6.arc", &generated_type_9, &generated_default_130, generated_targets_130, 1 },
    { "ships.amoeba.0.weapons.6.mods", &generated_type_10, &generated_default_131, generated_targets_131, 1 },
    { "ships.amoeba.0.weapons.6.ammo", &generated_type_2, &generated_default_132, generated_targets_132, 1 },
    { "ships.amoeba.0.weapons.6.amount", &generated_type_11, &generated_default_133, generated_targets_133, 2 },
    { "ships.amoeba.0.weapons.7.type", &generated_type_8, &generated_default_134, generated_targets_134, 1 },
    { "ships.amoeba.0.weapons.7.arc", &generated_type_9, &generated_default_135, generated_targets_135, 1 },
    { "ships.amoeba.0.weapons.7.mods", &generated_type_10, &generated_default_136, generated_targets_136, 1 },
    { "ships.amoeba.0.weapons.7.ammo", &generated_type_2, &generated_default_137, generated_targets_137, 1 },
    { "ships.amoeba.0.weapons.7.amount", &generated_type_11, &generated_default_138, generated_targets_138, 2 },
    { "ships.amoeba.0.weapons.8.type", &generated_type_8, &generated_default_139, generated_targets_139, 1 },
    { "ships.amoeba.0.weapons.8.arc", &generated_type_9, &generated_default_140, generated_targets_140, 1 },
    { "ships.amoeba.0.weapons.8.mods", &generated_type_10, &generated_default_141, generated_targets_141, 1 },
    { "ships.amoeba.0.weapons.8.ammo", &generated_type_2, &generated_default_142, generated_targets_142, 1 },
    { "ships.amoeba.0.weapons.8.amount", &generated_type_11, &generated_default_143, generated_targets_143, 2 },
    { "ships.amoeba.0.specials", &generated_type_12, &generated_default_144, generated_targets_144, 1 },
    { "ships.amoeba.1.appearance", &generated_type_2, &generated_default_145, generated_targets_145, 1 },
    { "ships.amoeba.1.hull_size", &generated_type_3, &generated_default_146, generated_targets_146, 1 },
    { "ships.amoeba.1.drive", &generated_type_4, &generated_default_147, generated_targets_147, 1 },
    { "ships.amoeba.1.armor", &generated_type_5, &generated_default_148, generated_targets_148, 1 },
    { "ships.amoeba.1.shield", &generated_type_6, &generated_default_149, generated_targets_149, 1 },
    { "ships.amoeba.1.computer", &generated_type_7, &generated_default_150, generated_targets_150, 1 },
    { "ships.amoeba.1.weapons.1.type", &generated_type_8, &generated_default_151, generated_targets_151, 1 },
    { "ships.amoeba.1.weapons.1.arc", &generated_type_9, &generated_default_152, generated_targets_152, 1 },
    { "ships.amoeba.1.weapons.1.mods", &generated_type_10, &generated_default_153, generated_targets_153, 1 },
    { "ships.amoeba.1.weapons.1.ammo", &generated_type_2, &generated_default_154, generated_targets_154, 1 },
    { "ships.amoeba.1.weapons.1.amount", &generated_type_11, &generated_default_155, generated_targets_155, 2 },
    { "ships.amoeba.1.weapons.2.type", &generated_type_8, &generated_default_156, generated_targets_156, 1 },
    { "ships.amoeba.1.weapons.2.arc", &generated_type_9, &generated_default_157, generated_targets_157, 1 },
    { "ships.amoeba.1.weapons.2.mods", &generated_type_10, &generated_default_158, generated_targets_158, 1 },
    { "ships.amoeba.1.weapons.2.ammo", &generated_type_2, &generated_default_159, generated_targets_159, 1 },
    { "ships.amoeba.1.weapons.2.amount", &generated_type_11, &generated_default_160, generated_targets_160, 2 },
    { "ships.amoeba.1.weapons.3.type", &generated_type_8, &generated_default_161, generated_targets_161, 1 },
    { "ships.amoeba.1.weapons.3.arc", &generated_type_9, &generated_default_162, generated_targets_162, 1 },
    { "ships.amoeba.1.weapons.3.mods", &generated_type_10, &generated_default_163, generated_targets_163, 1 },
    { "ships.amoeba.1.weapons.3.ammo", &generated_type_2, &generated_default_164, generated_targets_164, 1 },
    { "ships.amoeba.1.weapons.3.amount", &generated_type_11, &generated_default_165, generated_targets_165, 2 },
    { "ships.amoeba.1.weapons.4.type", &generated_type_8, &generated_default_166, generated_targets_166, 1 },
    { "ships.amoeba.1.weapons.4.arc", &generated_type_9, &generated_default_167, generated_targets_167, 1 },
    { "ships.amoeba.1.weapons.4.mods", &generated_type_10, &generated_default_168, generated_targets_168, 1 },
    { "ships.amoeba.1.weapons.4.ammo", &generated_type_2, &generated_default_169, generated_targets_169, 1 },
    { "ships.amoeba.1.weapons.4.amount", &generated_type_11, &generated_default_170, generated_targets_170, 2 },
    { "ships.amoeba.1.weapons.5.type", &generated_type_8, &generated_default_171, generated_targets_171, 1 },
    { "ships.amoeba.1.weapons.5.arc", &generated_type_9, &generated_default_172, generated_targets_172, 1 },
    { "ships.amoeba.1.weapons.5.mods", &generated_type_10, &generated_default_173, generated_targets_173, 1 },
    { "ships.amoeba.1.weapons.5.ammo", &generated_type_2, &generated_default_174, generated_targets_174, 1 },
    { "ships.amoeba.1.weapons.5.amount", &generated_type_11, &generated_default_175, generated_targets_175, 2 },
    { "ships.amoeba.1.weapons.6.type", &generated_type_8, &generated_default_176, generated_targets_176, 1 },
    { "ships.amoeba.1.weapons.6.arc", &generated_type_9, &generated_default_177, generated_targets_177, 1 },
    { "ships.amoeba.1.weapons.6.mods", &generated_type_10, &generated_default_178, generated_targets_178, 1 },
    { "ships.amoeba.1.weapons.6.ammo", &generated_type_2, &generated_default_179, generated_targets_179, 1 },
    { "ships.amoeba.1.weapons.6.amount", &generated_type_11, &generated_default_180, generated_targets_180, 2 },
    { "ships.amoeba.1.weapons.7.type", &generated_type_8, &generated_default_181, generated_targets_181, 1 },
    { "ships.amoeba.1.weapons.7.arc", &generated_type_9, &generated_default_182, generated_targets_182, 1 },
    { "ships.amoeba.1.weapons.7.mods", &generated_type_10, &generated_default_183, generated_targets_183, 1 },
    { "ships.amoeba.1.weapons.7.ammo", &generated_type_2, &generated_default_184, generated_targets_184, 1 },
    { "ships.amoeba.1.weapons.7.amount", &generated_type_11, &generated_default_185, generated_targets_185, 2 },
    { "ships.amoeba.1.weapons.8.type", &generated_type_8, &generated_default_186, generated_targets_186, 1 },
    { "ships.amoeba.1.weapons.8.arc", &generated_type_9, &generated_default_187, generated_targets_187, 1 },
    { "ships.amoeba.1.weapons.8.mods", &generated_type_10, &generated_default_188, generated_targets_188, 1 },
    { "ships.amoeba.1.weapons.8.ammo", &generated_type_2, &generated_default_189, generated_targets_189, 1 },
    { "ships.amoeba.1.weapons.8.amount", &generated_type_11, &generated_default_190, generated_targets_190, 2 },
    { "ships.amoeba.1.specials", &generated_type_12, &generated_default_191, generated_targets_191, 1 },
    { "ships.crystal.0.appearance", &generated_type_2, &generated_default_192, generated_targets_192, 1 },
    { "ships.crystal.0.hull_size", &generated_type_3, &generated_default_193, generated_targets_193, 1 },
    { "ships.crystal.0.drive", &generated_type_4, &generated_default_194, generated_targets_194, 1 },
    { "ships.crystal.0.armor", &generated_type_5, &generated_default_195, generated_targets_195, 1 },
    { "ships.crystal.0.shield", &generated_type_6, &generated_default_196, generated_targets_196, 1 },
    { "ships.crystal.0.computer", &generated_type_7, &generated_default_197, generated_targets_197, 1 },
    { "ships.crystal.0.weapons.1.type", &generated_type_8, &generated_default_198, generated_targets_198, 1 },
    { "ships.crystal.0.weapons.1.arc", &generated_type_9, &generated_default_199, generated_targets_199, 1 },
    { "ships.crystal.0.weapons.1.mods", &generated_type_10, &generated_default_200, generated_targets_200, 1 },
    { "ships.crystal.0.weapons.1.ammo", &generated_type_2, &generated_default_201, generated_targets_201, 1 },
    { "ships.crystal.0.weapons.1.amount", &generated_type_11, &generated_default_202, generated_targets_202, 2 },
    { "ships.crystal.0.weapons.2.type", &generated_type_8, &generated_default_203, generated_targets_203, 1 },
    { "ships.crystal.0.weapons.2.arc", &generated_type_9, &generated_default_204, generated_targets_204, 1 },
    { "ships.crystal.0.weapons.2.mods", &generated_type_10, &generated_default_205, generated_targets_205, 1 },
    { "ships.crystal.0.weapons.2.ammo", &generated_type_2, &generated_default_206, generated_targets_206, 1 },
    { "ships.crystal.0.weapons.2.amount", &generated_type_11, &generated_default_207, generated_targets_207, 2 },
    { "ships.crystal.0.weapons.3.type", &generated_type_8, &generated_default_208, generated_targets_208, 1 },
    { "ships.crystal.0.weapons.3.arc", &generated_type_9, &generated_default_209, generated_targets_209, 1 },
    { "ships.crystal.0.weapons.3.mods", &generated_type_10, &generated_default_210, generated_targets_210, 1 },
    { "ships.crystal.0.weapons.3.ammo", &generated_type_2, &generated_default_211, generated_targets_211, 1 },
    { "ships.crystal.0.weapons.3.amount", &generated_type_11, &generated_default_212, generated_targets_212, 2 },
    { "ships.crystal.0.weapons.4.type", &generated_type_8, &generated_default_213, generated_targets_213, 1 },
    { "ships.crystal.0.weapons.4.arc", &generated_type_9, &generated_default_214, generated_targets_214, 1 },
    { "ships.crystal.0.weapons.4.mods", &generated_type_10, &generated_default_215, generated_targets_215, 1 },
    { "ships.crystal.0.weapons.4.ammo", &generated_type_2, &generated_default_216, generated_targets_216, 1 },
    { "ships.crystal.0.weapons.4.amount", &generated_type_11, &generated_default_217, generated_targets_217, 2 },
    { "ships.crystal.0.weapons.5.type", &generated_type_8, &generated_default_218, generated_targets_218, 1 },
    { "ships.crystal.0.weapons.5.arc", &generated_type_9, &generated_default_219, generated_targets_219, 1 },
    { "ships.crystal.0.weapons.5.mods", &generated_type_10, &generated_default_220, generated_targets_220, 1 },
    { "ships.crystal.0.weapons.5.ammo", &generated_type_2, &generated_default_221, generated_targets_221, 1 },
    { "ships.crystal.0.weapons.5.amount", &generated_type_11, &generated_default_222, generated_targets_222, 2 },
    { "ships.crystal.0.weapons.6.type", &generated_type_8, &generated_default_223, generated_targets_223, 1 },
    { "ships.crystal.0.weapons.6.arc", &generated_type_9, &generated_default_224, generated_targets_224, 1 },
    { "ships.crystal.0.weapons.6.mods", &generated_type_10, &generated_default_225, generated_targets_225, 1 },
    { "ships.crystal.0.weapons.6.ammo", &generated_type_2, &generated_default_226, generated_targets_226, 1 },
    { "ships.crystal.0.weapons.6.amount", &generated_type_11, &generated_default_227, generated_targets_227, 2 },
    { "ships.crystal.0.weapons.7.type", &generated_type_8, &generated_default_228, generated_targets_228, 1 },
    { "ships.crystal.0.weapons.7.arc", &generated_type_9, &generated_default_229, generated_targets_229, 1 },
    { "ships.crystal.0.weapons.7.mods", &generated_type_10, &generated_default_230, generated_targets_230, 1 },
    { "ships.crystal.0.weapons.7.ammo", &generated_type_2, &generated_default_231, generated_targets_231, 1 },
    { "ships.crystal.0.weapons.7.amount", &generated_type_11, &generated_default_232, generated_targets_232, 2 },
    { "ships.crystal.0.weapons.8.type", &generated_type_8, &generated_default_233, generated_targets_233, 1 },
    { "ships.crystal.0.weapons.8.arc", &generated_type_9, &generated_default_234, generated_targets_234, 1 },
    { "ships.crystal.0.weapons.8.mods", &generated_type_10, &generated_default_235, generated_targets_235, 1 },
    { "ships.crystal.0.weapons.8.ammo", &generated_type_2, &generated_default_236, generated_targets_236, 1 },
    { "ships.crystal.0.weapons.8.amount", &generated_type_11, &generated_default_237, generated_targets_237, 2 },
    { "ships.crystal.0.specials", &generated_type_12, &generated_default_238, generated_targets_238, 1 },
    { "ships.crystal.1.appearance", &generated_type_2, &generated_default_239, generated_targets_239, 1 },
    { "ships.crystal.1.hull_size", &generated_type_3, &generated_default_240, generated_targets_240, 1 },
    { "ships.crystal.1.drive", &generated_type_4, &generated_default_241, generated_targets_241, 1 },
    { "ships.crystal.1.armor", &generated_type_5, &generated_default_242, generated_targets_242, 1 },
    { "ships.crystal.1.shield", &generated_type_6, &generated_default_243, generated_targets_243, 1 },
    { "ships.crystal.1.computer", &generated_type_7, &generated_default_244, generated_targets_244, 1 },
    { "ships.crystal.1.weapons.1.type", &generated_type_8, &generated_default_245, generated_targets_245, 1 },
    { "ships.crystal.1.weapons.1.arc", &generated_type_9, &generated_default_246, generated_targets_246, 1 },
    { "ships.crystal.1.weapons.1.mods", &generated_type_10, &generated_default_247, generated_targets_247, 1 },
    { "ships.crystal.1.weapons.1.ammo", &generated_type_2, &generated_default_248, generated_targets_248, 1 },
    { "ships.crystal.1.weapons.1.amount", &generated_type_11, &generated_default_249, generated_targets_249, 2 },
    { "ships.crystal.1.weapons.2.type", &generated_type_8, &generated_default_250, generated_targets_250, 1 },
    { "ships.crystal.1.weapons.2.arc", &generated_type_9, &generated_default_251, generated_targets_251, 1 },
    { "ships.crystal.1.weapons.2.mods", &generated_type_10, &generated_default_252, generated_targets_252, 1 },
    { "ships.crystal.1.weapons.2.ammo", &generated_type_2, &generated_default_253, generated_targets_253, 1 },
    { "ships.crystal.1.weapons.2.amount", &generated_type_11, &generated_default_254, generated_targets_254, 2 },
    { "ships.crystal.1.weapons.3.type", &generated_type_8, &generated_default_255, generated_targets_255, 1 },
    { "ships.crystal.1.weapons.3.arc", &generated_type_9, &generated_default_256, generated_targets_256, 1 },
    { "ships.crystal.1.weapons.3.mods", &generated_type_10, &generated_default_257, generated_targets_257, 1 },
    { "ships.crystal.1.weapons.3.ammo", &generated_type_2, &generated_default_258, generated_targets_258, 1 },
    { "ships.crystal.1.weapons.3.amount", &generated_type_11, &generated_default_259, generated_targets_259, 2 },
    { "ships.crystal.1.weapons.4.type", &generated_type_8, &generated_default_260, generated_targets_260, 1 },
    { "ships.crystal.1.weapons.4.arc", &generated_type_9, &generated_default_261, generated_targets_261, 1 },
    { "ships.crystal.1.weapons.4.mods", &generated_type_10, &generated_default_262, generated_targets_262, 1 },
    { "ships.crystal.1.weapons.4.ammo", &generated_type_2, &generated_default_263, generated_targets_263, 1 },
    { "ships.crystal.1.weapons.4.amount", &generated_type_11, &generated_default_264, generated_targets_264, 2 },
    { "ships.crystal.1.weapons.5.type", &generated_type_8, &generated_default_265, generated_targets_265, 1 },
    { "ships.crystal.1.weapons.5.arc", &generated_type_9, &generated_default_266, generated_targets_266, 1 },
    { "ships.crystal.1.weapons.5.mods", &generated_type_10, &generated_default_267, generated_targets_267, 1 },
    { "ships.crystal.1.weapons.5.ammo", &generated_type_2, &generated_default_268, generated_targets_268, 1 },
    { "ships.crystal.1.weapons.5.amount", &generated_type_11, &generated_default_269, generated_targets_269, 2 },
    { "ships.crystal.1.weapons.6.type", &generated_type_8, &generated_default_270, generated_targets_270, 1 },
    { "ships.crystal.1.weapons.6.arc", &generated_type_9, &generated_default_271, generated_targets_271, 1 },
    { "ships.crystal.1.weapons.6.mods", &generated_type_10, &generated_default_272, generated_targets_272, 1 },
    { "ships.crystal.1.weapons.6.ammo", &generated_type_2, &generated_default_273, generated_targets_273, 1 },
    { "ships.crystal.1.weapons.6.amount", &generated_type_11, &generated_default_274, generated_targets_274, 2 },
    { "ships.crystal.1.weapons.7.type", &generated_type_8, &generated_default_275, generated_targets_275, 1 },
    { "ships.crystal.1.weapons.7.arc", &generated_type_9, &generated_default_276, generated_targets_276, 1 },
    { "ships.crystal.1.weapons.7.mods", &generated_type_10, &generated_default_277, generated_targets_277, 1 },
    { "ships.crystal.1.weapons.7.ammo", &generated_type_2, &generated_default_278, generated_targets_278, 1 },
    { "ships.crystal.1.weapons.7.amount", &generated_type_11, &generated_default_279, generated_targets_279, 2 },
    { "ships.crystal.1.weapons.8.type", &generated_type_8, &generated_default_280, generated_targets_280, 1 },
    { "ships.crystal.1.weapons.8.arc", &generated_type_9, &generated_default_281, generated_targets_281, 1 },
    { "ships.crystal.1.weapons.8.mods", &generated_type_10, &generated_default_282, generated_targets_282, 1 },
    { "ships.crystal.1.weapons.8.ammo", &generated_type_2, &generated_default_283, generated_targets_283, 1 },
    { "ships.crystal.1.weapons.8.amount", &generated_type_11, &generated_default_284, generated_targets_284, 2 },
    { "ships.crystal.1.specials", &generated_type_12, &generated_default_285, generated_targets_285, 1 },
    { "ships.dragon.0.appearance", &generated_type_2, &generated_default_286, generated_targets_286, 1 },
    { "ships.dragon.0.hull_size", &generated_type_3, &generated_default_287, generated_targets_287, 1 },
    { "ships.dragon.0.drive", &generated_type_4, &generated_default_288, generated_targets_288, 1 },
    { "ships.dragon.0.armor", &generated_type_5, &generated_default_289, generated_targets_289, 1 },
    { "ships.dragon.0.shield", &generated_type_6, &generated_default_290, generated_targets_290, 1 },
    { "ships.dragon.0.computer", &generated_type_7, &generated_default_291, generated_targets_291, 1 },
    { "ships.dragon.0.weapons.1.type", &generated_type_8, &generated_default_292, generated_targets_292, 1 },
    { "ships.dragon.0.weapons.1.arc", &generated_type_9, &generated_default_293, generated_targets_293, 1 },
    { "ships.dragon.0.weapons.1.mods", &generated_type_10, &generated_default_294, generated_targets_294, 1 },
    { "ships.dragon.0.weapons.1.ammo", &generated_type_2, &generated_default_295, generated_targets_295, 1 },
    { "ships.dragon.0.weapons.1.amount", &generated_type_11, &generated_default_296, generated_targets_296, 2 },
    { "ships.dragon.0.weapons.2.type", &generated_type_8, &generated_default_297, generated_targets_297, 1 },
    { "ships.dragon.0.weapons.2.arc", &generated_type_9, &generated_default_298, generated_targets_298, 1 },
    { "ships.dragon.0.weapons.2.mods", &generated_type_10, &generated_default_299, generated_targets_299, 1 },
    { "ships.dragon.0.weapons.2.ammo", &generated_type_2, &generated_default_300, generated_targets_300, 1 },
    { "ships.dragon.0.weapons.2.amount", &generated_type_11, &generated_default_301, generated_targets_301, 2 },
    { "ships.dragon.0.weapons.3.type", &generated_type_8, &generated_default_302, generated_targets_302, 1 },
    { "ships.dragon.0.weapons.3.arc", &generated_type_9, &generated_default_303, generated_targets_303, 1 },
    { "ships.dragon.0.weapons.3.mods", &generated_type_10, &generated_default_304, generated_targets_304, 1 },
    { "ships.dragon.0.weapons.3.ammo", &generated_type_2, &generated_default_305, generated_targets_305, 1 },
    { "ships.dragon.0.weapons.3.amount", &generated_type_11, &generated_default_306, generated_targets_306, 2 },
    { "ships.dragon.0.weapons.4.type", &generated_type_8, &generated_default_307, generated_targets_307, 1 },
    { "ships.dragon.0.weapons.4.arc", &generated_type_9, &generated_default_308, generated_targets_308, 1 },
    { "ships.dragon.0.weapons.4.mods", &generated_type_10, &generated_default_309, generated_targets_309, 1 },
    { "ships.dragon.0.weapons.4.ammo", &generated_type_2, &generated_default_310, generated_targets_310, 1 },
    { "ships.dragon.0.weapons.4.amount", &generated_type_11, &generated_default_311, generated_targets_311, 2 },
    { "ships.dragon.0.weapons.5.type", &generated_type_8, &generated_default_312, generated_targets_312, 1 },
    { "ships.dragon.0.weapons.5.arc", &generated_type_9, &generated_default_313, generated_targets_313, 1 },
    { "ships.dragon.0.weapons.5.mods", &generated_type_10, &generated_default_314, generated_targets_314, 1 },
    { "ships.dragon.0.weapons.5.ammo", &generated_type_2, &generated_default_315, generated_targets_315, 1 },
    { "ships.dragon.0.weapons.5.amount", &generated_type_11, &generated_default_316, generated_targets_316, 2 },
    { "ships.dragon.0.weapons.6.type", &generated_type_8, &generated_default_317, generated_targets_317, 1 },
    { "ships.dragon.0.weapons.6.arc", &generated_type_9, &generated_default_318, generated_targets_318, 1 },
    { "ships.dragon.0.weapons.6.mods", &generated_type_10, &generated_default_319, generated_targets_319, 1 },
    { "ships.dragon.0.weapons.6.ammo", &generated_type_2, &generated_default_320, generated_targets_320, 1 },
    { "ships.dragon.0.weapons.6.amount", &generated_type_11, &generated_default_321, generated_targets_321, 2 },
    { "ships.dragon.0.weapons.7.type", &generated_type_8, &generated_default_322, generated_targets_322, 1 },
    { "ships.dragon.0.weapons.7.arc", &generated_type_9, &generated_default_323, generated_targets_323, 1 },
    { "ships.dragon.0.weapons.7.mods", &generated_type_10, &generated_default_324, generated_targets_324, 1 },
    { "ships.dragon.0.weapons.7.ammo", &generated_type_2, &generated_default_325, generated_targets_325, 1 },
    { "ships.dragon.0.weapons.7.amount", &generated_type_11, &generated_default_326, generated_targets_326, 2 },
    { "ships.dragon.0.weapons.8.type", &generated_type_8, &generated_default_327, generated_targets_327, 1 },
    { "ships.dragon.0.weapons.8.arc", &generated_type_9, &generated_default_328, generated_targets_328, 1 },
    { "ships.dragon.0.weapons.8.mods", &generated_type_10, &generated_default_329, generated_targets_329, 1 },
    { "ships.dragon.0.weapons.8.ammo", &generated_type_2, &generated_default_330, generated_targets_330, 1 },
    { "ships.dragon.0.weapons.8.amount", &generated_type_11, &generated_default_331, generated_targets_331, 2 },
    { "ships.dragon.0.specials", &generated_type_12, &generated_default_332, generated_targets_332, 1 },
    { "ships.dragon.1.appearance", &generated_type_2, &generated_default_333, generated_targets_333, 1 },
    { "ships.dragon.1.hull_size", &generated_type_3, &generated_default_334, generated_targets_334, 1 },
    { "ships.dragon.1.drive", &generated_type_4, &generated_default_335, generated_targets_335, 1 },
    { "ships.dragon.1.armor", &generated_type_5, &generated_default_336, generated_targets_336, 1 },
    { "ships.dragon.1.shield", &generated_type_6, &generated_default_337, generated_targets_337, 1 },
    { "ships.dragon.1.computer", &generated_type_7, &generated_default_338, generated_targets_338, 1 },
    { "ships.dragon.1.weapons.1.type", &generated_type_8, &generated_default_339, generated_targets_339, 1 },
    { "ships.dragon.1.weapons.1.arc", &generated_type_9, &generated_default_340, generated_targets_340, 1 },
    { "ships.dragon.1.weapons.1.mods", &generated_type_10, &generated_default_341, generated_targets_341, 1 },
    { "ships.dragon.1.weapons.1.ammo", &generated_type_2, &generated_default_342, generated_targets_342, 1 },
    { "ships.dragon.1.weapons.1.amount", &generated_type_11, &generated_default_343, generated_targets_343, 2 },
    { "ships.dragon.1.weapons.2.type", &generated_type_8, &generated_default_344, generated_targets_344, 1 },
    { "ships.dragon.1.weapons.2.arc", &generated_type_9, &generated_default_345, generated_targets_345, 1 },
    { "ships.dragon.1.weapons.2.mods", &generated_type_10, &generated_default_346, generated_targets_346, 1 },
    { "ships.dragon.1.weapons.2.ammo", &generated_type_2, &generated_default_347, generated_targets_347, 1 },
    { "ships.dragon.1.weapons.2.amount", &generated_type_11, &generated_default_348, generated_targets_348, 2 },
    { "ships.dragon.1.weapons.3.type", &generated_type_8, &generated_default_349, generated_targets_349, 1 },
    { "ships.dragon.1.weapons.3.arc", &generated_type_9, &generated_default_350, generated_targets_350, 1 },
    { "ships.dragon.1.weapons.3.mods", &generated_type_10, &generated_default_351, generated_targets_351, 1 },
    { "ships.dragon.1.weapons.3.ammo", &generated_type_2, &generated_default_352, generated_targets_352, 1 },
    { "ships.dragon.1.weapons.3.amount", &generated_type_11, &generated_default_353, generated_targets_353, 2 },
    { "ships.dragon.1.weapons.4.type", &generated_type_8, &generated_default_354, generated_targets_354, 1 },
    { "ships.dragon.1.weapons.4.arc", &generated_type_9, &generated_default_355, generated_targets_355, 1 },
    { "ships.dragon.1.weapons.4.mods", &generated_type_10, &generated_default_356, generated_targets_356, 1 },
    { "ships.dragon.1.weapons.4.ammo", &generated_type_2, &generated_default_357, generated_targets_357, 1 },
    { "ships.dragon.1.weapons.4.amount", &generated_type_11, &generated_default_358, generated_targets_358, 2 },
    { "ships.dragon.1.weapons.5.type", &generated_type_8, &generated_default_359, generated_targets_359, 1 },
    { "ships.dragon.1.weapons.5.arc", &generated_type_9, &generated_default_360, generated_targets_360, 1 },
    { "ships.dragon.1.weapons.5.mods", &generated_type_10, &generated_default_361, generated_targets_361, 1 },
    { "ships.dragon.1.weapons.5.ammo", &generated_type_2, &generated_default_362, generated_targets_362, 1 },
    { "ships.dragon.1.weapons.5.amount", &generated_type_11, &generated_default_363, generated_targets_363, 2 },
    { "ships.dragon.1.weapons.6.type", &generated_type_8, &generated_default_364, generated_targets_364, 1 },
    { "ships.dragon.1.weapons.6.arc", &generated_type_9, &generated_default_365, generated_targets_365, 1 },
    { "ships.dragon.1.weapons.6.mods", &generated_type_10, &generated_default_366, generated_targets_366, 1 },
    { "ships.dragon.1.weapons.6.ammo", &generated_type_2, &generated_default_367, generated_targets_367, 1 },
    { "ships.dragon.1.weapons.6.amount", &generated_type_11, &generated_default_368, generated_targets_368, 2 },
    { "ships.dragon.1.weapons.7.type", &generated_type_8, &generated_default_369, generated_targets_369, 1 },
    { "ships.dragon.1.weapons.7.arc", &generated_type_9, &generated_default_370, generated_targets_370, 1 },
    { "ships.dragon.1.weapons.7.mods", &generated_type_10, &generated_default_371, generated_targets_371, 1 },
    { "ships.dragon.1.weapons.7.ammo", &generated_type_2, &generated_default_372, generated_targets_372, 1 },
    { "ships.dragon.1.weapons.7.amount", &generated_type_11, &generated_default_373, generated_targets_373, 2 },
    { "ships.dragon.1.weapons.8.type", &generated_type_8, &generated_default_374, generated_targets_374, 1 },
    { "ships.dragon.1.weapons.8.arc", &generated_type_9, &generated_default_375, generated_targets_375, 1 },
    { "ships.dragon.1.weapons.8.mods", &generated_type_10, &generated_default_376, generated_targets_376, 1 },
    { "ships.dragon.1.weapons.8.ammo", &generated_type_2, &generated_default_377, generated_targets_377, 1 },
    { "ships.dragon.1.weapons.8.amount", &generated_type_11, &generated_default_378, generated_targets_378, 2 },
    { "ships.dragon.1.specials", &generated_type_12, &generated_default_379, generated_targets_379, 1 },
    { "ships.eel.0.appearance", &generated_type_2, &generated_default_380, generated_targets_380, 1 },
    { "ships.eel.0.hull_size", &generated_type_3, &generated_default_381, generated_targets_381, 1 },
    { "ships.eel.0.drive", &generated_type_4, &generated_default_382, generated_targets_382, 1 },
    { "ships.eel.0.armor", &generated_type_5, &generated_default_383, generated_targets_383, 1 },
    { "ships.eel.0.shield", &generated_type_6, &generated_default_384, generated_targets_384, 1 },
    { "ships.eel.0.computer", &generated_type_7, &generated_default_385, generated_targets_385, 1 },
    { "ships.eel.0.weapons.1.type", &generated_type_8, &generated_default_386, generated_targets_386, 1 },
    { "ships.eel.0.weapons.1.arc", &generated_type_9, &generated_default_387, generated_targets_387, 1 },
    { "ships.eel.0.weapons.1.mods", &generated_type_10, &generated_default_388, generated_targets_388, 1 },
    { "ships.eel.0.weapons.1.ammo", &generated_type_2, &generated_default_389, generated_targets_389, 1 },
    { "ships.eel.0.weapons.1.amount", &generated_type_11, &generated_default_390, generated_targets_390, 2 },
    { "ships.eel.0.weapons.2.type", &generated_type_8, &generated_default_391, generated_targets_391, 1 },
    { "ships.eel.0.weapons.2.arc", &generated_type_9, &generated_default_392, generated_targets_392, 1 },
    { "ships.eel.0.weapons.2.mods", &generated_type_10, &generated_default_393, generated_targets_393, 1 },
    { "ships.eel.0.weapons.2.ammo", &generated_type_2, &generated_default_394, generated_targets_394, 1 },
    { "ships.eel.0.weapons.2.amount", &generated_type_11, &generated_default_395, generated_targets_395, 2 },
    { "ships.eel.0.weapons.3.type", &generated_type_8, &generated_default_396, generated_targets_396, 1 },
    { "ships.eel.0.weapons.3.arc", &generated_type_9, &generated_default_397, generated_targets_397, 1 },
    { "ships.eel.0.weapons.3.mods", &generated_type_10, &generated_default_398, generated_targets_398, 1 },
    { "ships.eel.0.weapons.3.ammo", &generated_type_2, &generated_default_399, generated_targets_399, 1 },
    { "ships.eel.0.weapons.3.amount", &generated_type_11, &generated_default_400, generated_targets_400, 2 },
    { "ships.eel.0.weapons.4.type", &generated_type_8, &generated_default_401, generated_targets_401, 1 },
    { "ships.eel.0.weapons.4.arc", &generated_type_9, &generated_default_402, generated_targets_402, 1 },
    { "ships.eel.0.weapons.4.mods", &generated_type_10, &generated_default_403, generated_targets_403, 1 },
    { "ships.eel.0.weapons.4.ammo", &generated_type_2, &generated_default_404, generated_targets_404, 1 },
    { "ships.eel.0.weapons.4.amount", &generated_type_11, &generated_default_405, generated_targets_405, 2 },
    { "ships.eel.0.weapons.5.type", &generated_type_8, &generated_default_406, generated_targets_406, 1 },
    { "ships.eel.0.weapons.5.arc", &generated_type_9, &generated_default_407, generated_targets_407, 1 },
    { "ships.eel.0.weapons.5.mods", &generated_type_10, &generated_default_408, generated_targets_408, 1 },
    { "ships.eel.0.weapons.5.ammo", &generated_type_2, &generated_default_409, generated_targets_409, 1 },
    { "ships.eel.0.weapons.5.amount", &generated_type_11, &generated_default_410, generated_targets_410, 2 },
    { "ships.eel.0.weapons.6.type", &generated_type_8, &generated_default_411, generated_targets_411, 1 },
    { "ships.eel.0.weapons.6.arc", &generated_type_9, &generated_default_412, generated_targets_412, 1 },
    { "ships.eel.0.weapons.6.mods", &generated_type_10, &generated_default_413, generated_targets_413, 1 },
    { "ships.eel.0.weapons.6.ammo", &generated_type_2, &generated_default_414, generated_targets_414, 1 },
    { "ships.eel.0.weapons.6.amount", &generated_type_11, &generated_default_415, generated_targets_415, 2 },
    { "ships.eel.0.weapons.7.type", &generated_type_8, &generated_default_416, generated_targets_416, 1 },
    { "ships.eel.0.weapons.7.arc", &generated_type_9, &generated_default_417, generated_targets_417, 1 },
    { "ships.eel.0.weapons.7.mods", &generated_type_10, &generated_default_418, generated_targets_418, 1 },
    { "ships.eel.0.weapons.7.ammo", &generated_type_2, &generated_default_419, generated_targets_419, 1 },
    { "ships.eel.0.weapons.7.amount", &generated_type_11, &generated_default_420, generated_targets_420, 2 },
    { "ships.eel.0.weapons.8.type", &generated_type_8, &generated_default_421, generated_targets_421, 1 },
    { "ships.eel.0.weapons.8.arc", &generated_type_9, &generated_default_422, generated_targets_422, 1 },
    { "ships.eel.0.weapons.8.mods", &generated_type_10, &generated_default_423, generated_targets_423, 1 },
    { "ships.eel.0.weapons.8.ammo", &generated_type_2, &generated_default_424, generated_targets_424, 1 },
    { "ships.eel.0.weapons.8.amount", &generated_type_11, &generated_default_425, generated_targets_425, 2 },
    { "ships.eel.0.specials", &generated_type_12, &generated_default_426, generated_targets_426, 1 },
    { "ships.eel.1.appearance", &generated_type_2, &generated_default_427, generated_targets_427, 1 },
    { "ships.eel.1.hull_size", &generated_type_3, &generated_default_428, generated_targets_428, 1 },
    { "ships.eel.1.drive", &generated_type_4, &generated_default_429, generated_targets_429, 1 },
    { "ships.eel.1.armor", &generated_type_5, &generated_default_430, generated_targets_430, 1 },
    { "ships.eel.1.shield", &generated_type_6, &generated_default_431, generated_targets_431, 1 },
    { "ships.eel.1.computer", &generated_type_7, &generated_default_432, generated_targets_432, 1 },
    { "ships.eel.1.weapons.1.type", &generated_type_8, &generated_default_433, generated_targets_433, 1 },
    { "ships.eel.1.weapons.1.arc", &generated_type_9, &generated_default_434, generated_targets_434, 1 },
    { "ships.eel.1.weapons.1.mods", &generated_type_10, &generated_default_435, generated_targets_435, 1 },
    { "ships.eel.1.weapons.1.ammo", &generated_type_2, &generated_default_436, generated_targets_436, 1 },
    { "ships.eel.1.weapons.1.amount", &generated_type_11, &generated_default_437, generated_targets_437, 2 },
    { "ships.eel.1.weapons.2.type", &generated_type_8, &generated_default_438, generated_targets_438, 1 },
    { "ships.eel.1.weapons.2.arc", &generated_type_9, &generated_default_439, generated_targets_439, 1 },
    { "ships.eel.1.weapons.2.mods", &generated_type_10, &generated_default_440, generated_targets_440, 1 },
    { "ships.eel.1.weapons.2.ammo", &generated_type_2, &generated_default_441, generated_targets_441, 1 },
    { "ships.eel.1.weapons.2.amount", &generated_type_11, &generated_default_442, generated_targets_442, 2 },
    { "ships.eel.1.weapons.3.type", &generated_type_8, &generated_default_443, generated_targets_443, 1 },
    { "ships.eel.1.weapons.3.arc", &generated_type_9, &generated_default_444, generated_targets_444, 1 },
    { "ships.eel.1.weapons.3.mods", &generated_type_10, &generated_default_445, generated_targets_445, 1 },
    { "ships.eel.1.weapons.3.ammo", &generated_type_2, &generated_default_446, generated_targets_446, 1 },
    { "ships.eel.1.weapons.3.amount", &generated_type_11, &generated_default_447, generated_targets_447, 2 },
    { "ships.eel.1.weapons.4.type", &generated_type_8, &generated_default_448, generated_targets_448, 1 },
    { "ships.eel.1.weapons.4.arc", &generated_type_9, &generated_default_449, generated_targets_449, 1 },
    { "ships.eel.1.weapons.4.mods", &generated_type_10, &generated_default_450, generated_targets_450, 1 },
    { "ships.eel.1.weapons.4.ammo", &generated_type_2, &generated_default_451, generated_targets_451, 1 },
    { "ships.eel.1.weapons.4.amount", &generated_type_11, &generated_default_452, generated_targets_452, 2 },
    { "ships.eel.1.weapons.5.type", &generated_type_8, &generated_default_453, generated_targets_453, 1 },
    { "ships.eel.1.weapons.5.arc", &generated_type_9, &generated_default_454, generated_targets_454, 1 },
    { "ships.eel.1.weapons.5.mods", &generated_type_10, &generated_default_455, generated_targets_455, 1 },
    { "ships.eel.1.weapons.5.ammo", &generated_type_2, &generated_default_456, generated_targets_456, 1 },
    { "ships.eel.1.weapons.5.amount", &generated_type_11, &generated_default_457, generated_targets_457, 2 },
    { "ships.eel.1.weapons.6.type", &generated_type_8, &generated_default_458, generated_targets_458, 1 },
    { "ships.eel.1.weapons.6.arc", &generated_type_9, &generated_default_459, generated_targets_459, 1 },
    { "ships.eel.1.weapons.6.mods", &generated_type_10, &generated_default_460, generated_targets_460, 1 },
    { "ships.eel.1.weapons.6.ammo", &generated_type_2, &generated_default_461, generated_targets_461, 1 },
    { "ships.eel.1.weapons.6.amount", &generated_type_11, &generated_default_462, generated_targets_462, 2 },
    { "ships.eel.1.weapons.7.type", &generated_type_8, &generated_default_463, generated_targets_463, 1 },
    { "ships.eel.1.weapons.7.arc", &generated_type_9, &generated_default_464, generated_targets_464, 1 },
    { "ships.eel.1.weapons.7.mods", &generated_type_10, &generated_default_465, generated_targets_465, 1 },
    { "ships.eel.1.weapons.7.ammo", &generated_type_2, &generated_default_466, generated_targets_466, 1 },
    { "ships.eel.1.weapons.7.amount", &generated_type_11, &generated_default_467, generated_targets_467, 2 },
    { "ships.eel.1.weapons.8.type", &generated_type_8, &generated_default_468, generated_targets_468, 1 },
    { "ships.eel.1.weapons.8.arc", &generated_type_9, &generated_default_469, generated_targets_469, 1 },
    { "ships.eel.1.weapons.8.mods", &generated_type_10, &generated_default_470, generated_targets_470, 1 },
    { "ships.eel.1.weapons.8.ammo", &generated_type_2, &generated_default_471, generated_targets_471, 1 },
    { "ships.eel.1.weapons.8.amount", &generated_type_11, &generated_default_472, generated_targets_472, 2 },
    { "ships.eel.1.specials", &generated_type_12, &generated_default_473, generated_targets_473, 1 },
    { "ships.hydra.0.appearance", &generated_type_2, &generated_default_474, generated_targets_474, 1 },
    { "ships.hydra.0.hull_size", &generated_type_3, &generated_default_475, generated_targets_475, 1 },
    { "ships.hydra.0.drive", &generated_type_4, &generated_default_476, generated_targets_476, 1 },
    { "ships.hydra.0.armor", &generated_type_5, &generated_default_477, generated_targets_477, 1 },
    { "ships.hydra.0.shield", &generated_type_6, &generated_default_478, generated_targets_478, 1 },
    { "ships.hydra.0.computer", &generated_type_7, &generated_default_479, generated_targets_479, 1 },
    { "ships.hydra.0.weapons.1.type", &generated_type_8, &generated_default_480, generated_targets_480, 1 },
    { "ships.hydra.0.weapons.1.arc", &generated_type_9, &generated_default_481, generated_targets_481, 1 },
    { "ships.hydra.0.weapons.1.mods", &generated_type_10, &generated_default_482, generated_targets_482, 1 },
    { "ships.hydra.0.weapons.1.ammo", &generated_type_2, &generated_default_483, generated_targets_483, 1 },
    { "ships.hydra.0.weapons.1.amount", &generated_type_11, &generated_default_484, generated_targets_484, 2 },
    { "ships.hydra.0.weapons.2.type", &generated_type_8, &generated_default_485, generated_targets_485, 1 },
    { "ships.hydra.0.weapons.2.arc", &generated_type_9, &generated_default_486, generated_targets_486, 1 },
    { "ships.hydra.0.weapons.2.mods", &generated_type_10, &generated_default_487, generated_targets_487, 1 },
    { "ships.hydra.0.weapons.2.ammo", &generated_type_2, &generated_default_488, generated_targets_488, 1 },
    { "ships.hydra.0.weapons.2.amount", &generated_type_11, &generated_default_489, generated_targets_489, 2 },
    { "ships.hydra.0.weapons.3.type", &generated_type_8, &generated_default_490, generated_targets_490, 1 },
    { "ships.hydra.0.weapons.3.arc", &generated_type_9, &generated_default_491, generated_targets_491, 1 },
    { "ships.hydra.0.weapons.3.mods", &generated_type_10, &generated_default_492, generated_targets_492, 1 },
    { "ships.hydra.0.weapons.3.ammo", &generated_type_2, &generated_default_493, generated_targets_493, 1 },
    { "ships.hydra.0.weapons.3.amount", &generated_type_11, &generated_default_494, generated_targets_494, 2 },
    { "ships.hydra.0.weapons.4.type", &generated_type_8, &generated_default_495, generated_targets_495, 1 },
    { "ships.hydra.0.weapons.4.arc", &generated_type_9, &generated_default_496, generated_targets_496, 1 },
    { "ships.hydra.0.weapons.4.mods", &generated_type_10, &generated_default_497, generated_targets_497, 1 },
    { "ships.hydra.0.weapons.4.ammo", &generated_type_2, &generated_default_498, generated_targets_498, 1 },
    { "ships.hydra.0.weapons.4.amount", &generated_type_11, &generated_default_499, generated_targets_499, 2 },
    { "ships.hydra.0.weapons.5.type", &generated_type_8, &generated_default_500, generated_targets_500, 1 },
    { "ships.hydra.0.weapons.5.arc", &generated_type_9, &generated_default_501, generated_targets_501, 1 },
    { "ships.hydra.0.weapons.5.mods", &generated_type_10, &generated_default_502, generated_targets_502, 1 },
    { "ships.hydra.0.weapons.5.ammo", &generated_type_2, &generated_default_503, generated_targets_503, 1 },
    { "ships.hydra.0.weapons.5.amount", &generated_type_11, &generated_default_504, generated_targets_504, 2 },
    { "ships.hydra.0.weapons.6.type", &generated_type_8, &generated_default_505, generated_targets_505, 1 },
    { "ships.hydra.0.weapons.6.arc", &generated_type_9, &generated_default_506, generated_targets_506, 1 },
    { "ships.hydra.0.weapons.6.mods", &generated_type_10, &generated_default_507, generated_targets_507, 1 },
    { "ships.hydra.0.weapons.6.ammo", &generated_type_2, &generated_default_508, generated_targets_508, 1 },
    { "ships.hydra.0.weapons.6.amount", &generated_type_11, &generated_default_509, generated_targets_509, 2 },
    { "ships.hydra.0.weapons.7.type", &generated_type_8, &generated_default_510, generated_targets_510, 1 },
    { "ships.hydra.0.weapons.7.arc", &generated_type_9, &generated_default_511, generated_targets_511, 1 },
    { "ships.hydra.0.weapons.7.mods", &generated_type_10, &generated_default_512, generated_targets_512, 1 },
    { "ships.hydra.0.weapons.7.ammo", &generated_type_2, &generated_default_513, generated_targets_513, 1 },
    { "ships.hydra.0.weapons.7.amount", &generated_type_11, &generated_default_514, generated_targets_514, 2 },
    { "ships.hydra.0.weapons.8.type", &generated_type_8, &generated_default_515, generated_targets_515, 1 },
    { "ships.hydra.0.weapons.8.arc", &generated_type_9, &generated_default_516, generated_targets_516, 1 },
    { "ships.hydra.0.weapons.8.mods", &generated_type_10, &generated_default_517, generated_targets_517, 1 },
    { "ships.hydra.0.weapons.8.ammo", &generated_type_2, &generated_default_518, generated_targets_518, 1 },
    { "ships.hydra.0.weapons.8.amount", &generated_type_11, &generated_default_519, generated_targets_519, 2 },
    { "ships.hydra.0.specials", &generated_type_12, &generated_default_520, generated_targets_520, 1 },
    { "ships.hydra.1.appearance", &generated_type_2, &generated_default_521, generated_targets_521, 1 },
    { "ships.hydra.1.hull_size", &generated_type_3, &generated_default_522, generated_targets_522, 1 },
    { "ships.hydra.1.drive", &generated_type_4, &generated_default_523, generated_targets_523, 1 },
    { "ships.hydra.1.armor", &generated_type_5, &generated_default_524, generated_targets_524, 1 },
    { "ships.hydra.1.shield", &generated_type_6, &generated_default_525, generated_targets_525, 1 },
    { "ships.hydra.1.computer", &generated_type_7, &generated_default_526, generated_targets_526, 1 },
    { "ships.hydra.1.weapons.1.type", &generated_type_8, &generated_default_527, generated_targets_527, 1 },
    { "ships.hydra.1.weapons.1.arc", &generated_type_9, &generated_default_528, generated_targets_528, 1 },
    { "ships.hydra.1.weapons.1.mods", &generated_type_10, &generated_default_529, generated_targets_529, 1 },
    { "ships.hydra.1.weapons.1.ammo", &generated_type_2, &generated_default_530, generated_targets_530, 1 },
    { "ships.hydra.1.weapons.1.amount", &generated_type_11, &generated_default_531, generated_targets_531, 2 },
    { "ships.hydra.1.weapons.2.type", &generated_type_8, &generated_default_532, generated_targets_532, 1 },
    { "ships.hydra.1.weapons.2.arc", &generated_type_9, &generated_default_533, generated_targets_533, 1 },
    { "ships.hydra.1.weapons.2.mods", &generated_type_10, &generated_default_534, generated_targets_534, 1 },
    { "ships.hydra.1.weapons.2.ammo", &generated_type_2, &generated_default_535, generated_targets_535, 1 },
    { "ships.hydra.1.weapons.2.amount", &generated_type_11, &generated_default_536, generated_targets_536, 2 },
    { "ships.hydra.1.weapons.3.type", &generated_type_8, &generated_default_537, generated_targets_537, 1 },
    { "ships.hydra.1.weapons.3.arc", &generated_type_9, &generated_default_538, generated_targets_538, 1 },
    { "ships.hydra.1.weapons.3.mods", &generated_type_10, &generated_default_539, generated_targets_539, 1 },
    { "ships.hydra.1.weapons.3.ammo", &generated_type_2, &generated_default_540, generated_targets_540, 1 },
    { "ships.hydra.1.weapons.3.amount", &generated_type_11, &generated_default_541, generated_targets_541, 2 },
    { "ships.hydra.1.weapons.4.type", &generated_type_8, &generated_default_542, generated_targets_542, 1 },
    { "ships.hydra.1.weapons.4.arc", &generated_type_9, &generated_default_543, generated_targets_543, 1 },
    { "ships.hydra.1.weapons.4.mods", &generated_type_10, &generated_default_544, generated_targets_544, 1 },
    { "ships.hydra.1.weapons.4.ammo", &generated_type_2, &generated_default_545, generated_targets_545, 1 },
    { "ships.hydra.1.weapons.4.amount", &generated_type_11, &generated_default_546, generated_targets_546, 2 },
    { "ships.hydra.1.weapons.5.type", &generated_type_8, &generated_default_547, generated_targets_547, 1 },
    { "ships.hydra.1.weapons.5.arc", &generated_type_9, &generated_default_548, generated_targets_548, 1 },
    { "ships.hydra.1.weapons.5.mods", &generated_type_10, &generated_default_549, generated_targets_549, 1 },
    { "ships.hydra.1.weapons.5.ammo", &generated_type_2, &generated_default_550, generated_targets_550, 1 },
    { "ships.hydra.1.weapons.5.amount", &generated_type_11, &generated_default_551, generated_targets_551, 2 },
    { "ships.hydra.1.weapons.6.type", &generated_type_8, &generated_default_552, generated_targets_552, 1 },
    { "ships.hydra.1.weapons.6.arc", &generated_type_9, &generated_default_553, generated_targets_553, 1 },
    { "ships.hydra.1.weapons.6.mods", &generated_type_10, &generated_default_554, generated_targets_554, 1 },
    { "ships.hydra.1.weapons.6.ammo", &generated_type_2, &generated_default_555, generated_targets_555, 1 },
    { "ships.hydra.1.weapons.6.amount", &generated_type_11, &generated_default_556, generated_targets_556, 2 },
    { "ships.hydra.1.weapons.7.type", &generated_type_8, &generated_default_557, generated_targets_557, 1 },
    { "ships.hydra.1.weapons.7.arc", &generated_type_9, &generated_default_558, generated_targets_558, 1 },
    { "ships.hydra.1.weapons.7.mods", &generated_type_10, &generated_default_559, generated_targets_559, 1 },
    { "ships.hydra.1.weapons.7.ammo", &generated_type_2, &generated_default_560, generated_targets_560, 1 },
    { "ships.hydra.1.weapons.7.amount", &generated_type_11, &generated_default_561, generated_targets_561, 2 },
    { "ships.hydra.1.weapons.8.type", &generated_type_8, &generated_default_562, generated_targets_562, 1 },
    { "ships.hydra.1.weapons.8.arc", &generated_type_9, &generated_default_563, generated_targets_563, 1 },
    { "ships.hydra.1.weapons.8.mods", &generated_type_10, &generated_default_564, generated_targets_564, 1 },
    { "ships.hydra.1.weapons.8.ammo", &generated_type_2, &generated_default_565, generated_targets_565, 1 },
    { "ships.hydra.1.weapons.8.amount", &generated_type_11, &generated_default_566, generated_targets_566, 2 },
    { "ships.hydra.1.specials", &generated_type_12, &generated_default_567, generated_targets_567, 1 },
    { "ships.antaran.0.0.appearance", &generated_type_2, &generated_default_568, generated_targets_568, 1 },
    { "ships.antaran.0.0.hull_size", &generated_type_3, &generated_default_569, generated_targets_569, 1 },
    { "ships.antaran.0.0.drive", &generated_type_4, &generated_default_570, generated_targets_570, 1 },
    { "ships.antaran.0.0.armor", &generated_type_5, &generated_default_571, generated_targets_571, 1 },
    { "ships.antaran.0.0.shield", &generated_type_6, &generated_default_572, generated_targets_572, 1 },
    { "ships.antaran.0.0.computer", &generated_type_7, &generated_default_573, generated_targets_573, 1 },
    { "ships.antaran.0.0.weapons.1.type", &generated_type_8, &generated_default_574, generated_targets_574, 1 },
    { "ships.antaran.0.0.weapons.1.arc", &generated_type_9, &generated_default_575, generated_targets_575, 1 },
    { "ships.antaran.0.0.weapons.1.mods", &generated_type_10, &generated_default_576, generated_targets_576, 1 },
    { "ships.antaran.0.0.weapons.1.ammo", &generated_type_2, &generated_default_577, generated_targets_577, 1 },
    { "ships.antaran.0.0.weapons.1.amount", &generated_type_11, &generated_default_578, generated_targets_578, 2 },
    { "ships.antaran.0.0.weapons.2.type", &generated_type_8, &generated_default_579, generated_targets_579, 1 },
    { "ships.antaran.0.0.weapons.2.arc", &generated_type_9, &generated_default_580, generated_targets_580, 1 },
    { "ships.antaran.0.0.weapons.2.mods", &generated_type_10, &generated_default_581, generated_targets_581, 1 },
    { "ships.antaran.0.0.weapons.2.ammo", &generated_type_2, &generated_default_582, generated_targets_582, 1 },
    { "ships.antaran.0.0.weapons.2.amount", &generated_type_11, &generated_default_583, generated_targets_583, 2 },
    { "ships.antaran.0.0.weapons.3.type", &generated_type_8, &generated_default_584, generated_targets_584, 1 },
    { "ships.antaran.0.0.weapons.3.arc", &generated_type_9, &generated_default_585, generated_targets_585, 1 },
    { "ships.antaran.0.0.weapons.3.mods", &generated_type_10, &generated_default_586, generated_targets_586, 1 },
    { "ships.antaran.0.0.weapons.3.ammo", &generated_type_2, &generated_default_587, generated_targets_587, 1 },
    { "ships.antaran.0.0.weapons.3.amount", &generated_type_11, &generated_default_588, generated_targets_588, 2 },
    { "ships.antaran.0.0.weapons.4.type", &generated_type_8, &generated_default_589, generated_targets_589, 1 },
    { "ships.antaran.0.0.weapons.4.arc", &generated_type_9, &generated_default_590, generated_targets_590, 1 },
    { "ships.antaran.0.0.weapons.4.mods", &generated_type_10, &generated_default_591, generated_targets_591, 1 },
    { "ships.antaran.0.0.weapons.4.ammo", &generated_type_2, &generated_default_592, generated_targets_592, 1 },
    { "ships.antaran.0.0.weapons.4.amount", &generated_type_11, &generated_default_593, generated_targets_593, 2 },
    { "ships.antaran.0.0.weapons.5.type", &generated_type_8, &generated_default_594, generated_targets_594, 1 },
    { "ships.antaran.0.0.weapons.5.arc", &generated_type_9, &generated_default_595, generated_targets_595, 1 },
    { "ships.antaran.0.0.weapons.5.mods", &generated_type_10, &generated_default_596, generated_targets_596, 1 },
    { "ships.antaran.0.0.weapons.5.ammo", &generated_type_2, &generated_default_597, generated_targets_597, 1 },
    { "ships.antaran.0.0.weapons.5.amount", &generated_type_11, &generated_default_598, generated_targets_598, 2 },
    { "ships.antaran.0.0.weapons.6.type", &generated_type_8, &generated_default_599, generated_targets_599, 1 },
    { "ships.antaran.0.0.weapons.6.arc", &generated_type_9, &generated_default_600, generated_targets_600, 1 },
    { "ships.antaran.0.0.weapons.6.mods", &generated_type_10, &generated_default_601, generated_targets_601, 1 },
    { "ships.antaran.0.0.weapons.6.ammo", &generated_type_2, &generated_default_602, generated_targets_602, 1 },
    { "ships.antaran.0.0.weapons.6.amount", &generated_type_11, &generated_default_603, generated_targets_603, 2 },
    { "ships.antaran.0.0.weapons.7.type", &generated_type_8, &generated_default_604, generated_targets_604, 1 },
    { "ships.antaran.0.0.weapons.7.arc", &generated_type_9, &generated_default_605, generated_targets_605, 1 },
    { "ships.antaran.0.0.weapons.7.mods", &generated_type_10, &generated_default_606, generated_targets_606, 1 },
    { "ships.antaran.0.0.weapons.7.ammo", &generated_type_2, &generated_default_607, generated_targets_607, 1 },
    { "ships.antaran.0.0.weapons.7.amount", &generated_type_11, &generated_default_608, generated_targets_608, 2 },
    { "ships.antaran.0.0.weapons.8.type", &generated_type_8, &generated_default_609, generated_targets_609, 1 },
    { "ships.antaran.0.0.weapons.8.arc", &generated_type_9, &generated_default_610, generated_targets_610, 1 },
    { "ships.antaran.0.0.weapons.8.mods", &generated_type_10, &generated_default_611, generated_targets_611, 1 },
    { "ships.antaran.0.0.weapons.8.ammo", &generated_type_2, &generated_default_612, generated_targets_612, 1 },
    { "ships.antaran.0.0.weapons.8.amount", &generated_type_11, &generated_default_613, generated_targets_613, 2 },
    { "ships.antaran.0.0.specials", &generated_type_12, &generated_default_614, generated_targets_614, 1 },
    { "ships.antaran.0.1.appearance", &generated_type_2, &generated_default_615, generated_targets_615, 1 },
    { "ships.antaran.0.1.hull_size", &generated_type_3, &generated_default_616, generated_targets_616, 1 },
    { "ships.antaran.0.1.drive", &generated_type_4, &generated_default_617, generated_targets_617, 1 },
    { "ships.antaran.0.1.armor", &generated_type_5, &generated_default_618, generated_targets_618, 1 },
    { "ships.antaran.0.1.shield", &generated_type_6, &generated_default_619, generated_targets_619, 1 },
    { "ships.antaran.0.1.computer", &generated_type_7, &generated_default_620, generated_targets_620, 1 },
    { "ships.antaran.0.1.weapons.1.type", &generated_type_8, &generated_default_621, generated_targets_621, 1 },
    { "ships.antaran.0.1.weapons.1.arc", &generated_type_9, &generated_default_622, generated_targets_622, 1 },
    { "ships.antaran.0.1.weapons.1.mods", &generated_type_10, &generated_default_623, generated_targets_623, 1 },
    { "ships.antaran.0.1.weapons.1.ammo", &generated_type_2, &generated_default_624, generated_targets_624, 1 },
    { "ships.antaran.0.1.weapons.1.amount", &generated_type_11, &generated_default_625, generated_targets_625, 2 },
    { "ships.antaran.0.1.weapons.2.type", &generated_type_8, &generated_default_626, generated_targets_626, 1 },
    { "ships.antaran.0.1.weapons.2.arc", &generated_type_9, &generated_default_627, generated_targets_627, 1 },
    { "ships.antaran.0.1.weapons.2.mods", &generated_type_10, &generated_default_628, generated_targets_628, 1 },
    { "ships.antaran.0.1.weapons.2.ammo", &generated_type_2, &generated_default_629, generated_targets_629, 1 },
    { "ships.antaran.0.1.weapons.2.amount", &generated_type_11, &generated_default_630, generated_targets_630, 2 },
    { "ships.antaran.0.1.weapons.3.type", &generated_type_8, &generated_default_631, generated_targets_631, 1 },
    { "ships.antaran.0.1.weapons.3.arc", &generated_type_9, &generated_default_632, generated_targets_632, 1 },
    { "ships.antaran.0.1.weapons.3.mods", &generated_type_10, &generated_default_633, generated_targets_633, 1 },
    { "ships.antaran.0.1.weapons.3.ammo", &generated_type_2, &generated_default_634, generated_targets_634, 1 },
    { "ships.antaran.0.1.weapons.3.amount", &generated_type_11, &generated_default_635, generated_targets_635, 2 },
    { "ships.antaran.0.1.weapons.4.type", &generated_type_8, &generated_default_636, generated_targets_636, 1 },
    { "ships.antaran.0.1.weapons.4.arc", &generated_type_9, &generated_default_637, generated_targets_637, 1 },
    { "ships.antaran.0.1.weapons.4.mods", &generated_type_10, &generated_default_638, generated_targets_638, 1 },
    { "ships.antaran.0.1.weapons.4.ammo", &generated_type_2, &generated_default_639, generated_targets_639, 1 },
    { "ships.antaran.0.1.weapons.4.amount", &generated_type_11, &generated_default_640, generated_targets_640, 2 },
    { "ships.antaran.0.1.weapons.5.type", &generated_type_8, &generated_default_641, generated_targets_641, 1 },
    { "ships.antaran.0.1.weapons.5.arc", &generated_type_9, &generated_default_642, generated_targets_642, 1 },
    { "ships.antaran.0.1.weapons.5.mods", &generated_type_10, &generated_default_643, generated_targets_643, 1 },
    { "ships.antaran.0.1.weapons.5.ammo", &generated_type_2, &generated_default_644, generated_targets_644, 1 },
    { "ships.antaran.0.1.weapons.5.amount", &generated_type_11, &generated_default_645, generated_targets_645, 2 },
    { "ships.antaran.0.1.weapons.6.type", &generated_type_8, &generated_default_646, generated_targets_646, 1 },
    { "ships.antaran.0.1.weapons.6.arc", &generated_type_9, &generated_default_647, generated_targets_647, 1 },
    { "ships.antaran.0.1.weapons.6.mods", &generated_type_10, &generated_default_648, generated_targets_648, 1 },
    { "ships.antaran.0.1.weapons.6.ammo", &generated_type_2, &generated_default_649, generated_targets_649, 1 },
    { "ships.antaran.0.1.weapons.6.amount", &generated_type_11, &generated_default_650, generated_targets_650, 2 },
    { "ships.antaran.0.1.weapons.7.type", &generated_type_8, &generated_default_651, generated_targets_651, 1 },
    { "ships.antaran.0.1.weapons.7.arc", &generated_type_9, &generated_default_652, generated_targets_652, 1 },
    { "ships.antaran.0.1.weapons.7.mods", &generated_type_10, &generated_default_653, generated_targets_653, 1 },
    { "ships.antaran.0.1.weapons.7.ammo", &generated_type_2, &generated_default_654, generated_targets_654, 1 },
    { "ships.antaran.0.1.weapons.7.amount", &generated_type_11, &generated_default_655, generated_targets_655, 2 },
    { "ships.antaran.0.1.weapons.8.type", &generated_type_8, &generated_default_656, generated_targets_656, 1 },
    { "ships.antaran.0.1.weapons.8.arc", &generated_type_9, &generated_default_657, generated_targets_657, 1 },
    { "ships.antaran.0.1.weapons.8.mods", &generated_type_10, &generated_default_658, generated_targets_658, 1 },
    { "ships.antaran.0.1.weapons.8.ammo", &generated_type_2, &generated_default_659, generated_targets_659, 1 },
    { "ships.antaran.0.1.weapons.8.amount", &generated_type_11, &generated_default_660, generated_targets_660, 2 },
    { "ships.antaran.0.1.specials", &generated_type_12, &generated_default_661, generated_targets_661, 1 },
    { "ships.antaran.0.2.appearance", &generated_type_2, &generated_default_662, generated_targets_662, 1 },
    { "ships.antaran.0.2.hull_size", &generated_type_3, &generated_default_663, generated_targets_663, 1 },
    { "ships.antaran.0.2.drive", &generated_type_4, &generated_default_664, generated_targets_664, 1 },
    { "ships.antaran.0.2.armor", &generated_type_5, &generated_default_665, generated_targets_665, 1 },
    { "ships.antaran.0.2.shield", &generated_type_6, &generated_default_666, generated_targets_666, 1 },
    { "ships.antaran.0.2.computer", &generated_type_7, &generated_default_667, generated_targets_667, 1 },
    { "ships.antaran.0.2.weapons.1.type", &generated_type_8, &generated_default_668, generated_targets_668, 1 },
    { "ships.antaran.0.2.weapons.1.arc", &generated_type_9, &generated_default_669, generated_targets_669, 1 },
    { "ships.antaran.0.2.weapons.1.mods", &generated_type_10, &generated_default_670, generated_targets_670, 1 },
    { "ships.antaran.0.2.weapons.1.ammo", &generated_type_2, &generated_default_671, generated_targets_671, 1 },
    { "ships.antaran.0.2.weapons.1.amount", &generated_type_11, &generated_default_672, generated_targets_672, 2 },
    { "ships.antaran.0.2.weapons.2.type", &generated_type_8, &generated_default_673, generated_targets_673, 1 },
    { "ships.antaran.0.2.weapons.2.arc", &generated_type_9, &generated_default_674, generated_targets_674, 1 },
    { "ships.antaran.0.2.weapons.2.mods", &generated_type_10, &generated_default_675, generated_targets_675, 1 },
    { "ships.antaran.0.2.weapons.2.ammo", &generated_type_2, &generated_default_676, generated_targets_676, 1 },
    { "ships.antaran.0.2.weapons.2.amount", &generated_type_11, &generated_default_677, generated_targets_677, 2 },
    { "ships.antaran.0.2.weapons.3.type", &generated_type_8, &generated_default_678, generated_targets_678, 1 },
    { "ships.antaran.0.2.weapons.3.arc", &generated_type_9, &generated_default_679, generated_targets_679, 1 },
    { "ships.antaran.0.2.weapons.3.mods", &generated_type_10, &generated_default_680, generated_targets_680, 1 },
    { "ships.antaran.0.2.weapons.3.ammo", &generated_type_2, &generated_default_681, generated_targets_681, 1 },
    { "ships.antaran.0.2.weapons.3.amount", &generated_type_11, &generated_default_682, generated_targets_682, 2 },
    { "ships.antaran.0.2.weapons.4.type", &generated_type_8, &generated_default_683, generated_targets_683, 1 },
    { "ships.antaran.0.2.weapons.4.arc", &generated_type_9, &generated_default_684, generated_targets_684, 1 },
    { "ships.antaran.0.2.weapons.4.mods", &generated_type_10, &generated_default_685, generated_targets_685, 1 },
    { "ships.antaran.0.2.weapons.4.ammo", &generated_type_2, &generated_default_686, generated_targets_686, 1 },
    { "ships.antaran.0.2.weapons.4.amount", &generated_type_11, &generated_default_687, generated_targets_687, 2 },
    { "ships.antaran.0.2.weapons.5.type", &generated_type_8, &generated_default_688, generated_targets_688, 1 },
    { "ships.antaran.0.2.weapons.5.arc", &generated_type_9, &generated_default_689, generated_targets_689, 1 },
    { "ships.antaran.0.2.weapons.5.mods", &generated_type_10, &generated_default_690, generated_targets_690, 1 },
    { "ships.antaran.0.2.weapons.5.ammo", &generated_type_2, &generated_default_691, generated_targets_691, 1 },
    { "ships.antaran.0.2.weapons.5.amount", &generated_type_11, &generated_default_692, generated_targets_692, 2 },
    { "ships.antaran.0.2.weapons.6.type", &generated_type_8, &generated_default_693, generated_targets_693, 1 },
    { "ships.antaran.0.2.weapons.6.arc", &generated_type_9, &generated_default_694, generated_targets_694, 1 },
    { "ships.antaran.0.2.weapons.6.mods", &generated_type_10, &generated_default_695, generated_targets_695, 1 },
    { "ships.antaran.0.2.weapons.6.ammo", &generated_type_2, &generated_default_696, generated_targets_696, 1 },
    { "ships.antaran.0.2.weapons.6.amount", &generated_type_11, &generated_default_697, generated_targets_697, 2 },
    { "ships.antaran.0.2.weapons.7.type", &generated_type_8, &generated_default_698, generated_targets_698, 1 },
    { "ships.antaran.0.2.weapons.7.arc", &generated_type_9, &generated_default_699, generated_targets_699, 1 },
    { "ships.antaran.0.2.weapons.7.mods", &generated_type_10, &generated_default_700, generated_targets_700, 1 },
    { "ships.antaran.0.2.weapons.7.ammo", &generated_type_2, &generated_default_701, generated_targets_701, 1 },
    { "ships.antaran.0.2.weapons.7.amount", &generated_type_11, &generated_default_702, generated_targets_702, 2 },
    { "ships.antaran.0.2.weapons.8.type", &generated_type_8, &generated_default_703, generated_targets_703, 1 },
    { "ships.antaran.0.2.weapons.8.arc", &generated_type_9, &generated_default_704, generated_targets_704, 1 },
    { "ships.antaran.0.2.weapons.8.mods", &generated_type_10, &generated_default_705, generated_targets_705, 1 },
    { "ships.antaran.0.2.weapons.8.ammo", &generated_type_2, &generated_default_706, generated_targets_706, 1 },
    { "ships.antaran.0.2.weapons.8.amount", &generated_type_11, &generated_default_707, generated_targets_707, 2 },
    { "ships.antaran.0.2.specials", &generated_type_12, &generated_default_708, generated_targets_708, 1 },
    { "ships.antaran.0.3.appearance", &generated_type_2, &generated_default_709, generated_targets_709, 1 },
    { "ships.antaran.0.3.hull_size", &generated_type_3, &generated_default_710, generated_targets_710, 1 },
    { "ships.antaran.0.3.drive", &generated_type_4, &generated_default_711, generated_targets_711, 1 },
    { "ships.antaran.0.3.armor", &generated_type_5, &generated_default_712, generated_targets_712, 1 },
    { "ships.antaran.0.3.shield", &generated_type_6, &generated_default_713, generated_targets_713, 1 },
    { "ships.antaran.0.3.computer", &generated_type_7, &generated_default_714, generated_targets_714, 1 },
    { "ships.antaran.0.3.weapons.1.type", &generated_type_8, &generated_default_715, generated_targets_715, 1 },
    { "ships.antaran.0.3.weapons.1.arc", &generated_type_9, &generated_default_716, generated_targets_716, 1 },
    { "ships.antaran.0.3.weapons.1.mods", &generated_type_10, &generated_default_717, generated_targets_717, 1 },
    { "ships.antaran.0.3.weapons.1.ammo", &generated_type_2, &generated_default_718, generated_targets_718, 1 },
    { "ships.antaran.0.3.weapons.1.amount", &generated_type_11, &generated_default_719, generated_targets_719, 2 },
    { "ships.antaran.0.3.weapons.2.type", &generated_type_8, &generated_default_720, generated_targets_720, 1 },
    { "ships.antaran.0.3.weapons.2.arc", &generated_type_9, &generated_default_721, generated_targets_721, 1 },
    { "ships.antaran.0.3.weapons.2.mods", &generated_type_10, &generated_default_722, generated_targets_722, 1 },
    { "ships.antaran.0.3.weapons.2.ammo", &generated_type_2, &generated_default_723, generated_targets_723, 1 },
    { "ships.antaran.0.3.weapons.2.amount", &generated_type_11, &generated_default_724, generated_targets_724, 2 },
    { "ships.antaran.0.3.weapons.3.type", &generated_type_8, &generated_default_725, generated_targets_725, 1 },
    { "ships.antaran.0.3.weapons.3.arc", &generated_type_9, &generated_default_726, generated_targets_726, 1 },
    { "ships.antaran.0.3.weapons.3.mods", &generated_type_10, &generated_default_727, generated_targets_727, 1 },
    { "ships.antaran.0.3.weapons.3.ammo", &generated_type_2, &generated_default_728, generated_targets_728, 1 },
    { "ships.antaran.0.3.weapons.3.amount", &generated_type_11, &generated_default_729, generated_targets_729, 2 },
    { "ships.antaran.0.3.weapons.4.type", &generated_type_8, &generated_default_730, generated_targets_730, 1 },
    { "ships.antaran.0.3.weapons.4.arc", &generated_type_9, &generated_default_731, generated_targets_731, 1 },
    { "ships.antaran.0.3.weapons.4.mods", &generated_type_10, &generated_default_732, generated_targets_732, 1 },
    { "ships.antaran.0.3.weapons.4.ammo", &generated_type_2, &generated_default_733, generated_targets_733, 1 },
    { "ships.antaran.0.3.weapons.4.amount", &generated_type_11, &generated_default_734, generated_targets_734, 2 },
    { "ships.antaran.0.3.weapons.5.type", &generated_type_8, &generated_default_735, generated_targets_735, 1 },
    { "ships.antaran.0.3.weapons.5.arc", &generated_type_9, &generated_default_736, generated_targets_736, 1 },
    { "ships.antaran.0.3.weapons.5.mods", &generated_type_10, &generated_default_737, generated_targets_737, 1 },
    { "ships.antaran.0.3.weapons.5.ammo", &generated_type_2, &generated_default_738, generated_targets_738, 1 },
    { "ships.antaran.0.3.weapons.5.amount", &generated_type_11, &generated_default_739, generated_targets_739, 2 },
    { "ships.antaran.0.3.weapons.6.type", &generated_type_8, &generated_default_740, generated_targets_740, 1 },
    { "ships.antaran.0.3.weapons.6.arc", &generated_type_9, &generated_default_741, generated_targets_741, 1 },
    { "ships.antaran.0.3.weapons.6.mods", &generated_type_10, &generated_default_742, generated_targets_742, 1 },
    { "ships.antaran.0.3.weapons.6.ammo", &generated_type_2, &generated_default_743, generated_targets_743, 1 },
    { "ships.antaran.0.3.weapons.6.amount", &generated_type_11, &generated_default_744, generated_targets_744, 2 },
    { "ships.antaran.0.3.weapons.7.type", &generated_type_8, &generated_default_745, generated_targets_745, 1 },
    { "ships.antaran.0.3.weapons.7.arc", &generated_type_9, &generated_default_746, generated_targets_746, 1 },
    { "ships.antaran.0.3.weapons.7.mods", &generated_type_10, &generated_default_747, generated_targets_747, 1 },
    { "ships.antaran.0.3.weapons.7.ammo", &generated_type_2, &generated_default_748, generated_targets_748, 1 },
    { "ships.antaran.0.3.weapons.7.amount", &generated_type_11, &generated_default_749, generated_targets_749, 2 },
    { "ships.antaran.0.3.weapons.8.type", &generated_type_8, &generated_default_750, generated_targets_750, 1 },
    { "ships.antaran.0.3.weapons.8.arc", &generated_type_9, &generated_default_751, generated_targets_751, 1 },
    { "ships.antaran.0.3.weapons.8.mods", &generated_type_10, &generated_default_752, generated_targets_752, 1 },
    { "ships.antaran.0.3.weapons.8.ammo", &generated_type_2, &generated_default_753, generated_targets_753, 1 },
    { "ships.antaran.0.3.weapons.8.amount", &generated_type_11, &generated_default_754, generated_targets_754, 2 },
    { "ships.antaran.0.3.specials", &generated_type_12, &generated_default_755, generated_targets_755, 1 },
    { "ships.antaran.0.4.appearance", &generated_type_2, &generated_default_756, generated_targets_756, 1 },
    { "ships.antaran.0.4.hull_size", &generated_type_3, &generated_default_757, generated_targets_757, 1 },
    { "ships.antaran.0.4.drive", &generated_type_4, &generated_default_758, generated_targets_758, 1 },
    { "ships.antaran.0.4.armor", &generated_type_5, &generated_default_759, generated_targets_759, 1 },
    { "ships.antaran.0.4.shield", &generated_type_6, &generated_default_760, generated_targets_760, 1 },
    { "ships.antaran.0.4.computer", &generated_type_7, &generated_default_761, generated_targets_761, 1 },
    { "ships.antaran.0.4.weapons.1.type", &generated_type_8, &generated_default_762, generated_targets_762, 1 },
    { "ships.antaran.0.4.weapons.1.arc", &generated_type_9, &generated_default_763, generated_targets_763, 1 },
    { "ships.antaran.0.4.weapons.1.mods", &generated_type_10, &generated_default_764, generated_targets_764, 1 },
    { "ships.antaran.0.4.weapons.1.ammo", &generated_type_2, &generated_default_765, generated_targets_765, 1 },
    { "ships.antaran.0.4.weapons.1.amount", &generated_type_11, &generated_default_766, generated_targets_766, 2 },
    { "ships.antaran.0.4.weapons.2.type", &generated_type_8, &generated_default_767, generated_targets_767, 1 },
    { "ships.antaran.0.4.weapons.2.arc", &generated_type_9, &generated_default_768, generated_targets_768, 1 },
    { "ships.antaran.0.4.weapons.2.mods", &generated_type_10, &generated_default_769, generated_targets_769, 1 },
    { "ships.antaran.0.4.weapons.2.ammo", &generated_type_2, &generated_default_770, generated_targets_770, 1 },
    { "ships.antaran.0.4.weapons.2.amount", &generated_type_11, &generated_default_771, generated_targets_771, 2 },
    { "ships.antaran.0.4.weapons.3.type", &generated_type_8, &generated_default_772, generated_targets_772, 1 },
    { "ships.antaran.0.4.weapons.3.arc", &generated_type_9, &generated_default_773, generated_targets_773, 1 },
    { "ships.antaran.0.4.weapons.3.mods", &generated_type_10, &generated_default_774, generated_targets_774, 1 },
    { "ships.antaran.0.4.weapons.3.ammo", &generated_type_2, &generated_default_775, generated_targets_775, 1 },
    { "ships.antaran.0.4.weapons.3.amount", &generated_type_11, &generated_default_776, generated_targets_776, 2 },
    { "ships.antaran.0.4.weapons.4.type", &generated_type_8, &generated_default_777, generated_targets_777, 1 },
    { "ships.antaran.0.4.weapons.4.arc", &generated_type_9, &generated_default_778, generated_targets_778, 1 },
    { "ships.antaran.0.4.weapons.4.mods", &generated_type_10, &generated_default_779, generated_targets_779, 1 },
    { "ships.antaran.0.4.weapons.4.ammo", &generated_type_2, &generated_default_780, generated_targets_780, 1 },
    { "ships.antaran.0.4.weapons.4.amount", &generated_type_11, &generated_default_781, generated_targets_781, 2 },
    { "ships.antaran.0.4.weapons.5.type", &generated_type_8, &generated_default_782, generated_targets_782, 1 },
    { "ships.antaran.0.4.weapons.5.arc", &generated_type_9, &generated_default_783, generated_targets_783, 1 },
    { "ships.antaran.0.4.weapons.5.mods", &generated_type_10, &generated_default_784, generated_targets_784, 1 },
    { "ships.antaran.0.4.weapons.5.ammo", &generated_type_2, &generated_default_785, generated_targets_785, 1 },
    { "ships.antaran.0.4.weapons.5.amount", &generated_type_11, &generated_default_786, generated_targets_786, 2 },
    { "ships.antaran.0.4.weapons.6.type", &generated_type_8, &generated_default_787, generated_targets_787, 1 },
    { "ships.antaran.0.4.weapons.6.arc", &generated_type_9, &generated_default_788, generated_targets_788, 1 },
    { "ships.antaran.0.4.weapons.6.mods", &generated_type_10, &generated_default_789, generated_targets_789, 1 },
    { "ships.antaran.0.4.weapons.6.ammo", &generated_type_2, &generated_default_790, generated_targets_790, 1 },
    { "ships.antaran.0.4.weapons.6.amount", &generated_type_11, &generated_default_791, generated_targets_791, 2 },
    { "ships.antaran.0.4.weapons.7.type", &generated_type_8, &generated_default_792, generated_targets_792, 1 },
    { "ships.antaran.0.4.weapons.7.arc", &generated_type_9, &generated_default_793, generated_targets_793, 1 },
    { "ships.antaran.0.4.weapons.7.mods", &generated_type_10, &generated_default_794, generated_targets_794, 1 },
    { "ships.antaran.0.4.weapons.7.ammo", &generated_type_2, &generated_default_795, generated_targets_795, 1 },
    { "ships.antaran.0.4.weapons.7.amount", &generated_type_11, &generated_default_796, generated_targets_796, 2 },
    { "ships.antaran.0.4.weapons.8.type", &generated_type_8, &generated_default_797, generated_targets_797, 1 },
    { "ships.antaran.0.4.weapons.8.arc", &generated_type_9, &generated_default_798, generated_targets_798, 1 },
    { "ships.antaran.0.4.weapons.8.mods", &generated_type_10, &generated_default_799, generated_targets_799, 1 },
    { "ships.antaran.0.4.weapons.8.ammo", &generated_type_2, &generated_default_800, generated_targets_800, 1 },
    { "ships.antaran.0.4.weapons.8.amount", &generated_type_11, &generated_default_801, generated_targets_801, 2 },
    { "ships.antaran.0.4.specials", &generated_type_12, &generated_default_802, generated_targets_802, 1 },
    { "ships.antaran.1.0.appearance", &generated_type_2, &generated_default_803, generated_targets_803, 1 },
    { "ships.antaran.1.0.hull_size", &generated_type_3, &generated_default_804, generated_targets_804, 1 },
    { "ships.antaran.1.0.drive", &generated_type_4, &generated_default_805, generated_targets_805, 1 },
    { "ships.antaran.1.0.armor", &generated_type_5, &generated_default_806, generated_targets_806, 1 },
    { "ships.antaran.1.0.shield", &generated_type_6, &generated_default_807, generated_targets_807, 1 },
    { "ships.antaran.1.0.computer", &generated_type_7, &generated_default_808, generated_targets_808, 1 },
    { "ships.antaran.1.0.weapons.1.type", &generated_type_8, &generated_default_809, generated_targets_809, 1 },
    { "ships.antaran.1.0.weapons.1.arc", &generated_type_9, &generated_default_810, generated_targets_810, 1 },
    { "ships.antaran.1.0.weapons.1.mods", &generated_type_10, &generated_default_811, generated_targets_811, 1 },
    { "ships.antaran.1.0.weapons.1.ammo", &generated_type_2, &generated_default_812, generated_targets_812, 1 },
    { "ships.antaran.1.0.weapons.1.amount", &generated_type_11, &generated_default_813, generated_targets_813, 2 },
    { "ships.antaran.1.0.weapons.2.type", &generated_type_8, &generated_default_814, generated_targets_814, 1 },
    { "ships.antaran.1.0.weapons.2.arc", &generated_type_9, &generated_default_815, generated_targets_815, 1 },
    { "ships.antaran.1.0.weapons.2.mods", &generated_type_10, &generated_default_816, generated_targets_816, 1 },
    { "ships.antaran.1.0.weapons.2.ammo", &generated_type_2, &generated_default_817, generated_targets_817, 1 },
    { "ships.antaran.1.0.weapons.2.amount", &generated_type_11, &generated_default_818, generated_targets_818, 2 },
    { "ships.antaran.1.0.weapons.3.type", &generated_type_8, &generated_default_819, generated_targets_819, 1 },
    { "ships.antaran.1.0.weapons.3.arc", &generated_type_9, &generated_default_820, generated_targets_820, 1 },
    { "ships.antaran.1.0.weapons.3.mods", &generated_type_10, &generated_default_821, generated_targets_821, 1 },
    { "ships.antaran.1.0.weapons.3.ammo", &generated_type_2, &generated_default_822, generated_targets_822, 1 },
    { "ships.antaran.1.0.weapons.3.amount", &generated_type_11, &generated_default_823, generated_targets_823, 2 },
    { "ships.antaran.1.0.weapons.4.type", &generated_type_8, &generated_default_824, generated_targets_824, 1 },
    { "ships.antaran.1.0.weapons.4.arc", &generated_type_9, &generated_default_825, generated_targets_825, 1 },
    { "ships.antaran.1.0.weapons.4.mods", &generated_type_10, &generated_default_826, generated_targets_826, 1 },
    { "ships.antaran.1.0.weapons.4.ammo", &generated_type_2, &generated_default_827, generated_targets_827, 1 },
    { "ships.antaran.1.0.weapons.4.amount", &generated_type_11, &generated_default_828, generated_targets_828, 2 },
    { "ships.antaran.1.0.weapons.5.type", &generated_type_8, &generated_default_829, generated_targets_829, 1 },
    { "ships.antaran.1.0.weapons.5.arc", &generated_type_9, &generated_default_830, generated_targets_830, 1 },
    { "ships.antaran.1.0.weapons.5.mods", &generated_type_10, &generated_default_831, generated_targets_831, 1 },
    { "ships.antaran.1.0.weapons.5.ammo", &generated_type_2, &generated_default_832, generated_targets_832, 1 },
    { "ships.antaran.1.0.weapons.5.amount", &generated_type_11, &generated_default_833, generated_targets_833, 2 },
    { "ships.antaran.1.0.weapons.6.type", &generated_type_8, &generated_default_834, generated_targets_834, 1 },
    { "ships.antaran.1.0.weapons.6.arc", &generated_type_9, &generated_default_835, generated_targets_835, 1 },
    { "ships.antaran.1.0.weapons.6.mods", &generated_type_10, &generated_default_836, generated_targets_836, 1 },
    { "ships.antaran.1.0.weapons.6.ammo", &generated_type_2, &generated_default_837, generated_targets_837, 1 },
    { "ships.antaran.1.0.weapons.6.amount", &generated_type_11, &generated_default_838, generated_targets_838, 2 },
    { "ships.antaran.1.0.weapons.7.type", &generated_type_8, &generated_default_839, generated_targets_839, 1 },
    { "ships.antaran.1.0.weapons.7.arc", &generated_type_9, &generated_default_840, generated_targets_840, 1 },
    { "ships.antaran.1.0.weapons.7.mods", &generated_type_10, &generated_default_841, generated_targets_841, 1 },
    { "ships.antaran.1.0.weapons.7.ammo", &generated_type_2, &generated_default_842, generated_targets_842, 1 },
    { "ships.antaran.1.0.weapons.7.amount", &generated_type_11, &generated_default_843, generated_targets_843, 2 },
    { "ships.antaran.1.0.weapons.8.type", &generated_type_8, &generated_default_844, generated_targets_844, 1 },
    { "ships.antaran.1.0.weapons.8.arc", &generated_type_9, &generated_default_845, generated_targets_845, 1 },
    { "ships.antaran.1.0.weapons.8.mods", &generated_type_10, &generated_default_846, generated_targets_846, 1 },
    { "ships.antaran.1.0.weapons.8.ammo", &generated_type_2, &generated_default_847, generated_targets_847, 1 },
    { "ships.antaran.1.0.weapons.8.amount", &generated_type_11, &generated_default_848, generated_targets_848, 2 },
    { "ships.antaran.1.0.specials", &generated_type_12, &generated_default_849, generated_targets_849, 1 },
    { "ships.antaran.1.1.appearance", &generated_type_2, &generated_default_850, generated_targets_850, 1 },
    { "ships.antaran.1.1.hull_size", &generated_type_3, &generated_default_851, generated_targets_851, 1 },
    { "ships.antaran.1.1.drive", &generated_type_4, &generated_default_852, generated_targets_852, 1 },
    { "ships.antaran.1.1.armor", &generated_type_5, &generated_default_853, generated_targets_853, 1 },
    { "ships.antaran.1.1.shield", &generated_type_6, &generated_default_854, generated_targets_854, 1 },
    { "ships.antaran.1.1.computer", &generated_type_7, &generated_default_855, generated_targets_855, 1 },
    { "ships.antaran.1.1.weapons.1.type", &generated_type_8, &generated_default_856, generated_targets_856, 1 },
    { "ships.antaran.1.1.weapons.1.arc", &generated_type_9, &generated_default_857, generated_targets_857, 1 },
    { "ships.antaran.1.1.weapons.1.mods", &generated_type_10, &generated_default_858, generated_targets_858, 1 },
    { "ships.antaran.1.1.weapons.1.ammo", &generated_type_2, &generated_default_859, generated_targets_859, 1 },
    { "ships.antaran.1.1.weapons.1.amount", &generated_type_11, &generated_default_860, generated_targets_860, 2 },
    { "ships.antaran.1.1.weapons.2.type", &generated_type_8, &generated_default_861, generated_targets_861, 1 },
    { "ships.antaran.1.1.weapons.2.arc", &generated_type_9, &generated_default_862, generated_targets_862, 1 },
    { "ships.antaran.1.1.weapons.2.mods", &generated_type_10, &generated_default_863, generated_targets_863, 1 },
    { "ships.antaran.1.1.weapons.2.ammo", &generated_type_2, &generated_default_864, generated_targets_864, 1 },
    { "ships.antaran.1.1.weapons.2.amount", &generated_type_11, &generated_default_865, generated_targets_865, 2 },
    { "ships.antaran.1.1.weapons.3.type", &generated_type_8, &generated_default_866, generated_targets_866, 1 },
    { "ships.antaran.1.1.weapons.3.arc", &generated_type_9, &generated_default_867, generated_targets_867, 1 },
    { "ships.antaran.1.1.weapons.3.mods", &generated_type_10, &generated_default_868, generated_targets_868, 1 },
    { "ships.antaran.1.1.weapons.3.ammo", &generated_type_2, &generated_default_869, generated_targets_869, 1 },
    { "ships.antaran.1.1.weapons.3.amount", &generated_type_11, &generated_default_870, generated_targets_870, 2 },
    { "ships.antaran.1.1.weapons.4.type", &generated_type_8, &generated_default_871, generated_targets_871, 1 },
    { "ships.antaran.1.1.weapons.4.arc", &generated_type_9, &generated_default_872, generated_targets_872, 1 },
    { "ships.antaran.1.1.weapons.4.mods", &generated_type_10, &generated_default_873, generated_targets_873, 1 },
    { "ships.antaran.1.1.weapons.4.ammo", &generated_type_2, &generated_default_874, generated_targets_874, 1 },
    { "ships.antaran.1.1.weapons.4.amount", &generated_type_11, &generated_default_875, generated_targets_875, 2 },
    { "ships.antaran.1.1.weapons.5.type", &generated_type_8, &generated_default_876, generated_targets_876, 1 },
    { "ships.antaran.1.1.weapons.5.arc", &generated_type_9, &generated_default_877, generated_targets_877, 1 },
    { "ships.antaran.1.1.weapons.5.mods", &generated_type_10, &generated_default_878, generated_targets_878, 1 },
    { "ships.antaran.1.1.weapons.5.ammo", &generated_type_2, &generated_default_879, generated_targets_879, 1 },
    { "ships.antaran.1.1.weapons.5.amount", &generated_type_11, &generated_default_880, generated_targets_880, 2 },
    { "ships.antaran.1.1.weapons.6.type", &generated_type_8, &generated_default_881, generated_targets_881, 1 },
    { "ships.antaran.1.1.weapons.6.arc", &generated_type_9, &generated_default_882, generated_targets_882, 1 },
    { "ships.antaran.1.1.weapons.6.mods", &generated_type_10, &generated_default_883, generated_targets_883, 1 },
    { "ships.antaran.1.1.weapons.6.ammo", &generated_type_2, &generated_default_884, generated_targets_884, 1 },
    { "ships.antaran.1.1.weapons.6.amount", &generated_type_11, &generated_default_885, generated_targets_885, 2 },
    { "ships.antaran.1.1.weapons.7.type", &generated_type_8, &generated_default_886, generated_targets_886, 1 },
    { "ships.antaran.1.1.weapons.7.arc", &generated_type_9, &generated_default_887, generated_targets_887, 1 },
    { "ships.antaran.1.1.weapons.7.mods", &generated_type_10, &generated_default_888, generated_targets_888, 1 },
    { "ships.antaran.1.1.weapons.7.ammo", &generated_type_2, &generated_default_889, generated_targets_889, 1 },
    { "ships.antaran.1.1.weapons.7.amount", &generated_type_11, &generated_default_890, generated_targets_890, 2 },
    { "ships.antaran.1.1.weapons.8.type", &generated_type_8, &generated_default_891, generated_targets_891, 1 },
    { "ships.antaran.1.1.weapons.8.arc", &generated_type_9, &generated_default_892, generated_targets_892, 1 },
    { "ships.antaran.1.1.weapons.8.mods", &generated_type_10, &generated_default_893, generated_targets_893, 1 },
    { "ships.antaran.1.1.weapons.8.ammo", &generated_type_2, &generated_default_894, generated_targets_894, 1 },
    { "ships.antaran.1.1.weapons.8.amount", &generated_type_11, &generated_default_895, generated_targets_895, 2 },
    { "ships.antaran.1.1.specials", &generated_type_12, &generated_default_896, generated_targets_896, 1 },
    { "ships.antaran.1.2.appearance", &generated_type_2, &generated_default_897, generated_targets_897, 1 },
    { "ships.antaran.1.2.hull_size", &generated_type_3, &generated_default_898, generated_targets_898, 1 },
    { "ships.antaran.1.2.drive", &generated_type_4, &generated_default_899, generated_targets_899, 1 },
    { "ships.antaran.1.2.armor", &generated_type_5, &generated_default_900, generated_targets_900, 1 },
    { "ships.antaran.1.2.shield", &generated_type_6, &generated_default_901, generated_targets_901, 1 },
    { "ships.antaran.1.2.computer", &generated_type_7, &generated_default_902, generated_targets_902, 1 },
    { "ships.antaran.1.2.weapons.1.type", &generated_type_8, &generated_default_903, generated_targets_903, 1 },
    { "ships.antaran.1.2.weapons.1.arc", &generated_type_9, &generated_default_904, generated_targets_904, 1 },
    { "ships.antaran.1.2.weapons.1.mods", &generated_type_10, &generated_default_905, generated_targets_905, 1 },
    { "ships.antaran.1.2.weapons.1.ammo", &generated_type_2, &generated_default_906, generated_targets_906, 1 },
    { "ships.antaran.1.2.weapons.1.amount", &generated_type_11, &generated_default_907, generated_targets_907, 2 },
    { "ships.antaran.1.2.weapons.2.type", &generated_type_8, &generated_default_908, generated_targets_908, 1 },
    { "ships.antaran.1.2.weapons.2.arc", &generated_type_9, &generated_default_909, generated_targets_909, 1 },
    { "ships.antaran.1.2.weapons.2.mods", &generated_type_10, &generated_default_910, generated_targets_910, 1 },
    { "ships.antaran.1.2.weapons.2.ammo", &generated_type_2, &generated_default_911, generated_targets_911, 1 },
    { "ships.antaran.1.2.weapons.2.amount", &generated_type_11, &generated_default_912, generated_targets_912, 2 },
    { "ships.antaran.1.2.weapons.3.type", &generated_type_8, &generated_default_913, generated_targets_913, 1 },
    { "ships.antaran.1.2.weapons.3.arc", &generated_type_9, &generated_default_914, generated_targets_914, 1 },
    { "ships.antaran.1.2.weapons.3.mods", &generated_type_10, &generated_default_915, generated_targets_915, 1 },
    { "ships.antaran.1.2.weapons.3.ammo", &generated_type_2, &generated_default_916, generated_targets_916, 1 },
    { "ships.antaran.1.2.weapons.3.amount", &generated_type_11, &generated_default_917, generated_targets_917, 2 },
    { "ships.antaran.1.2.weapons.4.type", &generated_type_8, &generated_default_918, generated_targets_918, 1 },
    { "ships.antaran.1.2.weapons.4.arc", &generated_type_9, &generated_default_919, generated_targets_919, 1 },
    { "ships.antaran.1.2.weapons.4.mods", &generated_type_10, &generated_default_920, generated_targets_920, 1 },
    { "ships.antaran.1.2.weapons.4.ammo", &generated_type_2, &generated_default_921, generated_targets_921, 1 },
    { "ships.antaran.1.2.weapons.4.amount", &generated_type_11, &generated_default_922, generated_targets_922, 2 },
    { "ships.antaran.1.2.weapons.5.type", &generated_type_8, &generated_default_923, generated_targets_923, 1 },
    { "ships.antaran.1.2.weapons.5.arc", &generated_type_9, &generated_default_924, generated_targets_924, 1 },
    { "ships.antaran.1.2.weapons.5.mods", &generated_type_10, &generated_default_925, generated_targets_925, 1 },
    { "ships.antaran.1.2.weapons.5.ammo", &generated_type_2, &generated_default_926, generated_targets_926, 1 },
    { "ships.antaran.1.2.weapons.5.amount", &generated_type_11, &generated_default_927, generated_targets_927, 2 },
    { "ships.antaran.1.2.weapons.6.type", &generated_type_8, &generated_default_928, generated_targets_928, 1 },
    { "ships.antaran.1.2.weapons.6.arc", &generated_type_9, &generated_default_929, generated_targets_929, 1 },
    { "ships.antaran.1.2.weapons.6.mods", &generated_type_10, &generated_default_930, generated_targets_930, 1 },
    { "ships.antaran.1.2.weapons.6.ammo", &generated_type_2, &generated_default_931, generated_targets_931, 1 },
    { "ships.antaran.1.2.weapons.6.amount", &generated_type_11, &generated_default_932, generated_targets_932, 2 },
    { "ships.antaran.1.2.weapons.7.type", &generated_type_8, &generated_default_933, generated_targets_933, 1 },
    { "ships.antaran.1.2.weapons.7.arc", &generated_type_9, &generated_default_934, generated_targets_934, 1 },
    { "ships.antaran.1.2.weapons.7.mods", &generated_type_10, &generated_default_935, generated_targets_935, 1 },
    { "ships.antaran.1.2.weapons.7.ammo", &generated_type_2, &generated_default_936, generated_targets_936, 1 },
    { "ships.antaran.1.2.weapons.7.amount", &generated_type_11, &generated_default_937, generated_targets_937, 2 },
    { "ships.antaran.1.2.weapons.8.type", &generated_type_8, &generated_default_938, generated_targets_938, 1 },
    { "ships.antaran.1.2.weapons.8.arc", &generated_type_9, &generated_default_939, generated_targets_939, 1 },
    { "ships.antaran.1.2.weapons.8.mods", &generated_type_10, &generated_default_940, generated_targets_940, 1 },
    { "ships.antaran.1.2.weapons.8.ammo", &generated_type_2, &generated_default_941, generated_targets_941, 1 },
    { "ships.antaran.1.2.weapons.8.amount", &generated_type_11, &generated_default_942, generated_targets_942, 2 },
    { "ships.antaran.1.2.specials", &generated_type_12, &generated_default_943, generated_targets_943, 1 },
    { "ships.antaran.1.3.appearance", &generated_type_2, &generated_default_944, generated_targets_944, 1 },
    { "ships.antaran.1.3.hull_size", &generated_type_3, &generated_default_945, generated_targets_945, 1 },
    { "ships.antaran.1.3.drive", &generated_type_4, &generated_default_946, generated_targets_946, 1 },
    { "ships.antaran.1.3.armor", &generated_type_5, &generated_default_947, generated_targets_947, 1 },
    { "ships.antaran.1.3.shield", &generated_type_6, &generated_default_948, generated_targets_948, 1 },
    { "ships.antaran.1.3.computer", &generated_type_7, &generated_default_949, generated_targets_949, 1 },
    { "ships.antaran.1.3.weapons.1.type", &generated_type_8, &generated_default_950, generated_targets_950, 1 },
    { "ships.antaran.1.3.weapons.1.arc", &generated_type_9, &generated_default_951, generated_targets_951, 1 },
    { "ships.antaran.1.3.weapons.1.mods", &generated_type_10, &generated_default_952, generated_targets_952, 1 },
    { "ships.antaran.1.3.weapons.1.ammo", &generated_type_2, &generated_default_953, generated_targets_953, 1 },
    { "ships.antaran.1.3.weapons.1.amount", &generated_type_11, &generated_default_954, generated_targets_954, 2 },
    { "ships.antaran.1.3.weapons.2.type", &generated_type_8, &generated_default_955, generated_targets_955, 1 },
    { "ships.antaran.1.3.weapons.2.arc", &generated_type_9, &generated_default_956, generated_targets_956, 1 },
    { "ships.antaran.1.3.weapons.2.mods", &generated_type_10, &generated_default_957, generated_targets_957, 1 },
    { "ships.antaran.1.3.weapons.2.ammo", &generated_type_2, &generated_default_958, generated_targets_958, 1 },
    { "ships.antaran.1.3.weapons.2.amount", &generated_type_11, &generated_default_959, generated_targets_959, 2 },
    { "ships.antaran.1.3.weapons.3.type", &generated_type_8, &generated_default_960, generated_targets_960, 1 },
    { "ships.antaran.1.3.weapons.3.arc", &generated_type_9, &generated_default_961, generated_targets_961, 1 },
    { "ships.antaran.1.3.weapons.3.mods", &generated_type_10, &generated_default_962, generated_targets_962, 1 },
    { "ships.antaran.1.3.weapons.3.ammo", &generated_type_2, &generated_default_963, generated_targets_963, 1 },
    { "ships.antaran.1.3.weapons.3.amount", &generated_type_11, &generated_default_964, generated_targets_964, 2 },
    { "ships.antaran.1.3.weapons.4.type", &generated_type_8, &generated_default_965, generated_targets_965, 1 },
    { "ships.antaran.1.3.weapons.4.arc", &generated_type_9, &generated_default_966, generated_targets_966, 1 },
    { "ships.antaran.1.3.weapons.4.mods", &generated_type_10, &generated_default_967, generated_targets_967, 1 },
    { "ships.antaran.1.3.weapons.4.ammo", &generated_type_2, &generated_default_968, generated_targets_968, 1 },
    { "ships.antaran.1.3.weapons.4.amount", &generated_type_11, &generated_default_969, generated_targets_969, 2 },
    { "ships.antaran.1.3.weapons.5.type", &generated_type_8, &generated_default_970, generated_targets_970, 1 },
    { "ships.antaran.1.3.weapons.5.arc", &generated_type_9, &generated_default_971, generated_targets_971, 1 },
    { "ships.antaran.1.3.weapons.5.mods", &generated_type_10, &generated_default_972, generated_targets_972, 1 },
    { "ships.antaran.1.3.weapons.5.ammo", &generated_type_2, &generated_default_973, generated_targets_973, 1 },
    { "ships.antaran.1.3.weapons.5.amount", &generated_type_11, &generated_default_974, generated_targets_974, 2 },
    { "ships.antaran.1.3.weapons.6.type", &generated_type_8, &generated_default_975, generated_targets_975, 1 },
    { "ships.antaran.1.3.weapons.6.arc", &generated_type_9, &generated_default_976, generated_targets_976, 1 },
    { "ships.antaran.1.3.weapons.6.mods", &generated_type_10, &generated_default_977, generated_targets_977, 1 },
    { "ships.antaran.1.3.weapons.6.ammo", &generated_type_2, &generated_default_978, generated_targets_978, 1 },
    { "ships.antaran.1.3.weapons.6.amount", &generated_type_11, &generated_default_979, generated_targets_979, 2 },
    { "ships.antaran.1.3.weapons.7.type", &generated_type_8, &generated_default_980, generated_targets_980, 1 },
    { "ships.antaran.1.3.weapons.7.arc", &generated_type_9, &generated_default_981, generated_targets_981, 1 },
    { "ships.antaran.1.3.weapons.7.mods", &generated_type_10, &generated_default_982, generated_targets_982, 1 },
    { "ships.antaran.1.3.weapons.7.ammo", &generated_type_2, &generated_default_983, generated_targets_983, 1 },
    { "ships.antaran.1.3.weapons.7.amount", &generated_type_11, &generated_default_984, generated_targets_984, 2 },
    { "ships.antaran.1.3.weapons.8.type", &generated_type_8, &generated_default_985, generated_targets_985, 1 },
    { "ships.antaran.1.3.weapons.8.arc", &generated_type_9, &generated_default_986, generated_targets_986, 1 },
    { "ships.antaran.1.3.weapons.8.mods", &generated_type_10, &generated_default_987, generated_targets_987, 1 },
    { "ships.antaran.1.3.weapons.8.ammo", &generated_type_2, &generated_default_988, generated_targets_988, 1 },
    { "ships.antaran.1.3.weapons.8.amount", &generated_type_11, &generated_default_989, generated_targets_989, 2 },
    { "ships.antaran.1.3.specials", &generated_type_12, &generated_default_990, generated_targets_990, 1 },
    { "ships.antaran.1.4.appearance", &generated_type_2, &generated_default_991, generated_targets_991, 1 },
    { "ships.antaran.1.4.hull_size", &generated_type_3, &generated_default_992, generated_targets_992, 1 },
    { "ships.antaran.1.4.drive", &generated_type_4, &generated_default_993, generated_targets_993, 1 },
    { "ships.antaran.1.4.armor", &generated_type_5, &generated_default_994, generated_targets_994, 1 },
    { "ships.antaran.1.4.shield", &generated_type_6, &generated_default_995, generated_targets_995, 1 },
    { "ships.antaran.1.4.computer", &generated_type_7, &generated_default_996, generated_targets_996, 1 },
    { "ships.antaran.1.4.weapons.1.type", &generated_type_8, &generated_default_997, generated_targets_997, 1 },
    { "ships.antaran.1.4.weapons.1.arc", &generated_type_9, &generated_default_998, generated_targets_998, 1 },
    { "ships.antaran.1.4.weapons.1.mods", &generated_type_10, &generated_default_999, generated_targets_999, 1 },
    { "ships.antaran.1.4.weapons.1.ammo", &generated_type_2, &generated_default_1000, generated_targets_1000, 1 },
    { "ships.antaran.1.4.weapons.1.amount", &generated_type_11, &generated_default_1001, generated_targets_1001, 2 },
    { "ships.antaran.1.4.weapons.2.type", &generated_type_8, &generated_default_1002, generated_targets_1002, 1 },
    { "ships.antaran.1.4.weapons.2.arc", &generated_type_9, &generated_default_1003, generated_targets_1003, 1 },
    { "ships.antaran.1.4.weapons.2.mods", &generated_type_10, &generated_default_1004, generated_targets_1004, 1 },
    { "ships.antaran.1.4.weapons.2.ammo", &generated_type_2, &generated_default_1005, generated_targets_1005, 1 },
    { "ships.antaran.1.4.weapons.2.amount", &generated_type_11, &generated_default_1006, generated_targets_1006, 2 },
    { "ships.antaran.1.4.weapons.3.type", &generated_type_8, &generated_default_1007, generated_targets_1007, 1 },
    { "ships.antaran.1.4.weapons.3.arc", &generated_type_9, &generated_default_1008, generated_targets_1008, 1 },
    { "ships.antaran.1.4.weapons.3.mods", &generated_type_10, &generated_default_1009, generated_targets_1009, 1 },
    { "ships.antaran.1.4.weapons.3.ammo", &generated_type_2, &generated_default_1010, generated_targets_1010, 1 },
    { "ships.antaran.1.4.weapons.3.amount", &generated_type_11, &generated_default_1011, generated_targets_1011, 2 },
    { "ships.antaran.1.4.weapons.4.type", &generated_type_8, &generated_default_1012, generated_targets_1012, 1 },
    { "ships.antaran.1.4.weapons.4.arc", &generated_type_9, &generated_default_1013, generated_targets_1013, 1 },
    { "ships.antaran.1.4.weapons.4.mods", &generated_type_10, &generated_default_1014, generated_targets_1014, 1 },
    { "ships.antaran.1.4.weapons.4.ammo", &generated_type_2, &generated_default_1015, generated_targets_1015, 1 },
    { "ships.antaran.1.4.weapons.4.amount", &generated_type_11, &generated_default_1016, generated_targets_1016, 2 },
    { "ships.antaran.1.4.weapons.5.type", &generated_type_8, &generated_default_1017, generated_targets_1017, 1 },
    { "ships.antaran.1.4.weapons.5.arc", &generated_type_9, &generated_default_1018, generated_targets_1018, 1 },
    { "ships.antaran.1.4.weapons.5.mods", &generated_type_10, &generated_default_1019, generated_targets_1019, 1 },
    { "ships.antaran.1.4.weapons.5.ammo", &generated_type_2, &generated_default_1020, generated_targets_1020, 1 },
    { "ships.antaran.1.4.weapons.5.amount", &generated_type_11, &generated_default_1021, generated_targets_1021, 2 },
    { "ships.antaran.1.4.weapons.6.type", &generated_type_8, &generated_default_1022, generated_targets_1022, 1 },
    { "ships.antaran.1.4.weapons.6.arc", &generated_type_9, &generated_default_1023, generated_targets_1023, 1 },
    { "ships.antaran.1.4.weapons.6.mods", &generated_type_10, &generated_default_1024, generated_targets_1024, 1 },
    { "ships.antaran.1.4.weapons.6.ammo", &generated_type_2, &generated_default_1025, generated_targets_1025, 1 },
    { "ships.antaran.1.4.weapons.6.amount", &generated_type_11, &generated_default_1026, generated_targets_1026, 2 },
    { "ships.antaran.1.4.weapons.7.type", &generated_type_8, &generated_default_1027, generated_targets_1027, 1 },
    { "ships.antaran.1.4.weapons.7.arc", &generated_type_9, &generated_default_1028, generated_targets_1028, 1 },
    { "ships.antaran.1.4.weapons.7.mods", &generated_type_10, &generated_default_1029, generated_targets_1029, 1 },
    { "ships.antaran.1.4.weapons.7.ammo", &generated_type_2, &generated_default_1030, generated_targets_1030, 1 },
    { "ships.antaran.1.4.weapons.7.amount", &generated_type_11, &generated_default_1031, generated_targets_1031, 2 },
    { "ships.antaran.1.4.weapons.8.type", &generated_type_8, &generated_default_1032, generated_targets_1032, 1 },
    { "ships.antaran.1.4.weapons.8.arc", &generated_type_9, &generated_default_1033, generated_targets_1033, 1 },
    { "ships.antaran.1.4.weapons.8.mods", &generated_type_10, &generated_default_1034, generated_targets_1034, 1 },
    { "ships.antaran.1.4.weapons.8.ammo", &generated_type_2, &generated_default_1035, generated_targets_1035, 1 },
    { "ships.antaran.1.4.weapons.8.amount", &generated_type_11, &generated_default_1036, generated_targets_1036, 2 },
    { "ships.antaran.1.4.specials", &generated_type_12, &generated_default_1037, generated_targets_1037, 1 },
    { "ships.antaran.2.0.appearance", &generated_type_2, &generated_default_1038, generated_targets_1038, 1 },
    { "ships.antaran.2.0.hull_size", &generated_type_3, &generated_default_1039, generated_targets_1039, 1 },
    { "ships.antaran.2.0.drive", &generated_type_4, &generated_default_1040, generated_targets_1040, 1 },
    { "ships.antaran.2.0.armor", &generated_type_5, &generated_default_1041, generated_targets_1041, 1 },
    { "ships.antaran.2.0.shield", &generated_type_6, &generated_default_1042, generated_targets_1042, 1 },
    { "ships.antaran.2.0.computer", &generated_type_7, &generated_default_1043, generated_targets_1043, 1 },
    { "ships.antaran.2.0.weapons.1.type", &generated_type_8, &generated_default_1044, generated_targets_1044, 1 },
    { "ships.antaran.2.0.weapons.1.arc", &generated_type_9, &generated_default_1045, generated_targets_1045, 1 },
    { "ships.antaran.2.0.weapons.1.mods", &generated_type_10, &generated_default_1046, generated_targets_1046, 1 },
    { "ships.antaran.2.0.weapons.1.ammo", &generated_type_2, &generated_default_1047, generated_targets_1047, 1 },
    { "ships.antaran.2.0.weapons.1.amount", &generated_type_11, &generated_default_1048, generated_targets_1048, 2 },
    { "ships.antaran.2.0.weapons.2.type", &generated_type_8, &generated_default_1049, generated_targets_1049, 1 },
    { "ships.antaran.2.0.weapons.2.arc", &generated_type_9, &generated_default_1050, generated_targets_1050, 1 },
    { "ships.antaran.2.0.weapons.2.mods", &generated_type_10, &generated_default_1051, generated_targets_1051, 1 },
    { "ships.antaran.2.0.weapons.2.ammo", &generated_type_2, &generated_default_1052, generated_targets_1052, 1 },
    { "ships.antaran.2.0.weapons.2.amount", &generated_type_11, &generated_default_1053, generated_targets_1053, 2 },
    { "ships.antaran.2.0.weapons.3.type", &generated_type_8, &generated_default_1054, generated_targets_1054, 1 },
    { "ships.antaran.2.0.weapons.3.arc", &generated_type_9, &generated_default_1055, generated_targets_1055, 1 },
    { "ships.antaran.2.0.weapons.3.mods", &generated_type_10, &generated_default_1056, generated_targets_1056, 1 },
    { "ships.antaran.2.0.weapons.3.ammo", &generated_type_2, &generated_default_1057, generated_targets_1057, 1 },
    { "ships.antaran.2.0.weapons.3.amount", &generated_type_11, &generated_default_1058, generated_targets_1058, 2 },
    { "ships.antaran.2.0.weapons.4.type", &generated_type_8, &generated_default_1059, generated_targets_1059, 1 },
    { "ships.antaran.2.0.weapons.4.arc", &generated_type_9, &generated_default_1060, generated_targets_1060, 1 },
    { "ships.antaran.2.0.weapons.4.mods", &generated_type_10, &generated_default_1061, generated_targets_1061, 1 },
    { "ships.antaran.2.0.weapons.4.ammo", &generated_type_2, &generated_default_1062, generated_targets_1062, 1 },
    { "ships.antaran.2.0.weapons.4.amount", &generated_type_11, &generated_default_1063, generated_targets_1063, 2 },
    { "ships.antaran.2.0.weapons.5.type", &generated_type_8, &generated_default_1064, generated_targets_1064, 1 },
    { "ships.antaran.2.0.weapons.5.arc", &generated_type_9, &generated_default_1065, generated_targets_1065, 1 },
    { "ships.antaran.2.0.weapons.5.mods", &generated_type_10, &generated_default_1066, generated_targets_1066, 1 },
    { "ships.antaran.2.0.weapons.5.ammo", &generated_type_2, &generated_default_1067, generated_targets_1067, 1 },
    { "ships.antaran.2.0.weapons.5.amount", &generated_type_11, &generated_default_1068, generated_targets_1068, 2 },
    { "ships.antaran.2.0.weapons.6.type", &generated_type_8, &generated_default_1069, generated_targets_1069, 1 },
    { "ships.antaran.2.0.weapons.6.arc", &generated_type_9, &generated_default_1070, generated_targets_1070, 1 },
    { "ships.antaran.2.0.weapons.6.mods", &generated_type_10, &generated_default_1071, generated_targets_1071, 1 },
    { "ships.antaran.2.0.weapons.6.ammo", &generated_type_2, &generated_default_1072, generated_targets_1072, 1 },
    { "ships.antaran.2.0.weapons.6.amount", &generated_type_11, &generated_default_1073, generated_targets_1073, 2 },
    { "ships.antaran.2.0.weapons.7.type", &generated_type_8, &generated_default_1074, generated_targets_1074, 1 },
    { "ships.antaran.2.0.weapons.7.arc", &generated_type_9, &generated_default_1075, generated_targets_1075, 1 },
    { "ships.antaran.2.0.weapons.7.mods", &generated_type_10, &generated_default_1076, generated_targets_1076, 1 },
    { "ships.antaran.2.0.weapons.7.ammo", &generated_type_2, &generated_default_1077, generated_targets_1077, 1 },
    { "ships.antaran.2.0.weapons.7.amount", &generated_type_11, &generated_default_1078, generated_targets_1078, 2 },
    { "ships.antaran.2.0.weapons.8.type", &generated_type_8, &generated_default_1079, generated_targets_1079, 1 },
    { "ships.antaran.2.0.weapons.8.arc", &generated_type_9, &generated_default_1080, generated_targets_1080, 1 },
    { "ships.antaran.2.0.weapons.8.mods", &generated_type_10, &generated_default_1081, generated_targets_1081, 1 },
    { "ships.antaran.2.0.weapons.8.ammo", &generated_type_2, &generated_default_1082, generated_targets_1082, 1 },
    { "ships.antaran.2.0.weapons.8.amount", &generated_type_11, &generated_default_1083, generated_targets_1083, 2 },
    { "ships.antaran.2.0.specials", &generated_type_12, &generated_default_1084, generated_targets_1084, 1 },
    { "ships.antaran.2.1.appearance", &generated_type_2, &generated_default_1085, generated_targets_1085, 1 },
    { "ships.antaran.2.1.hull_size", &generated_type_3, &generated_default_1086, generated_targets_1086, 1 },
    { "ships.antaran.2.1.drive", &generated_type_4, &generated_default_1087, generated_targets_1087, 1 },
    { "ships.antaran.2.1.armor", &generated_type_5, &generated_default_1088, generated_targets_1088, 1 },
    { "ships.antaran.2.1.shield", &generated_type_6, &generated_default_1089, generated_targets_1089, 1 },
    { "ships.antaran.2.1.computer", &generated_type_7, &generated_default_1090, generated_targets_1090, 1 },
    { "ships.antaran.2.1.weapons.1.type", &generated_type_8, &generated_default_1091, generated_targets_1091, 1 },
    { "ships.antaran.2.1.weapons.1.arc", &generated_type_9, &generated_default_1092, generated_targets_1092, 1 },
    { "ships.antaran.2.1.weapons.1.mods", &generated_type_10, &generated_default_1093, generated_targets_1093, 1 },
    { "ships.antaran.2.1.weapons.1.ammo", &generated_type_2, &generated_default_1094, generated_targets_1094, 1 },
    { "ships.antaran.2.1.weapons.1.amount", &generated_type_11, &generated_default_1095, generated_targets_1095, 2 },
    { "ships.antaran.2.1.weapons.2.type", &generated_type_8, &generated_default_1096, generated_targets_1096, 1 },
    { "ships.antaran.2.1.weapons.2.arc", &generated_type_9, &generated_default_1097, generated_targets_1097, 1 },
    { "ships.antaran.2.1.weapons.2.mods", &generated_type_10, &generated_default_1098, generated_targets_1098, 1 },
    { "ships.antaran.2.1.weapons.2.ammo", &generated_type_2, &generated_default_1099, generated_targets_1099, 1 },
    { "ships.antaran.2.1.weapons.2.amount", &generated_type_11, &generated_default_1100, generated_targets_1100, 2 },
    { "ships.antaran.2.1.weapons.3.type", &generated_type_8, &generated_default_1101, generated_targets_1101, 1 },
    { "ships.antaran.2.1.weapons.3.arc", &generated_type_9, &generated_default_1102, generated_targets_1102, 1 },
    { "ships.antaran.2.1.weapons.3.mods", &generated_type_10, &generated_default_1103, generated_targets_1103, 1 },
    { "ships.antaran.2.1.weapons.3.ammo", &generated_type_2, &generated_default_1104, generated_targets_1104, 1 },
    { "ships.antaran.2.1.weapons.3.amount", &generated_type_11, &generated_default_1105, generated_targets_1105, 2 },
    { "ships.antaran.2.1.weapons.4.type", &generated_type_8, &generated_default_1106, generated_targets_1106, 1 },
    { "ships.antaran.2.1.weapons.4.arc", &generated_type_9, &generated_default_1107, generated_targets_1107, 1 },
    { "ships.antaran.2.1.weapons.4.mods", &generated_type_10, &generated_default_1108, generated_targets_1108, 1 },
    { "ships.antaran.2.1.weapons.4.ammo", &generated_type_2, &generated_default_1109, generated_targets_1109, 1 },
    { "ships.antaran.2.1.weapons.4.amount", &generated_type_11, &generated_default_1110, generated_targets_1110, 2 },
    { "ships.antaran.2.1.weapons.5.type", &generated_type_8, &generated_default_1111, generated_targets_1111, 1 },
    { "ships.antaran.2.1.weapons.5.arc", &generated_type_9, &generated_default_1112, generated_targets_1112, 1 },
    { "ships.antaran.2.1.weapons.5.mods", &generated_type_10, &generated_default_1113, generated_targets_1113, 1 },
    { "ships.antaran.2.1.weapons.5.ammo", &generated_type_2, &generated_default_1114, generated_targets_1114, 1 },
    { "ships.antaran.2.1.weapons.5.amount", &generated_type_11, &generated_default_1115, generated_targets_1115, 2 },
    { "ships.antaran.2.1.weapons.6.type", &generated_type_8, &generated_default_1116, generated_targets_1116, 1 },
    { "ships.antaran.2.1.weapons.6.arc", &generated_type_9, &generated_default_1117, generated_targets_1117, 1 },
    { "ships.antaran.2.1.weapons.6.mods", &generated_type_10, &generated_default_1118, generated_targets_1118, 1 },
    { "ships.antaran.2.1.weapons.6.ammo", &generated_type_2, &generated_default_1119, generated_targets_1119, 1 },
    { "ships.antaran.2.1.weapons.6.amount", &generated_type_11, &generated_default_1120, generated_targets_1120, 2 },
    { "ships.antaran.2.1.weapons.7.type", &generated_type_8, &generated_default_1121, generated_targets_1121, 1 },
    { "ships.antaran.2.1.weapons.7.arc", &generated_type_9, &generated_default_1122, generated_targets_1122, 1 },
    { "ships.antaran.2.1.weapons.7.mods", &generated_type_10, &generated_default_1123, generated_targets_1123, 1 },
    { "ships.antaran.2.1.weapons.7.ammo", &generated_type_2, &generated_default_1124, generated_targets_1124, 1 },
    { "ships.antaran.2.1.weapons.7.amount", &generated_type_11, &generated_default_1125, generated_targets_1125, 2 },
    { "ships.antaran.2.1.weapons.8.type", &generated_type_8, &generated_default_1126, generated_targets_1126, 1 },
    { "ships.antaran.2.1.weapons.8.arc", &generated_type_9, &generated_default_1127, generated_targets_1127, 1 },
    { "ships.antaran.2.1.weapons.8.mods", &generated_type_10, &generated_default_1128, generated_targets_1128, 1 },
    { "ships.antaran.2.1.weapons.8.ammo", &generated_type_2, &generated_default_1129, generated_targets_1129, 1 },
    { "ships.antaran.2.1.weapons.8.amount", &generated_type_11, &generated_default_1130, generated_targets_1130, 2 },
    { "ships.antaran.2.1.specials", &generated_type_12, &generated_default_1131, generated_targets_1131, 1 },
    { "ships.antaran.2.2.appearance", &generated_type_2, &generated_default_1132, generated_targets_1132, 1 },
    { "ships.antaran.2.2.hull_size", &generated_type_3, &generated_default_1133, generated_targets_1133, 1 },
    { "ships.antaran.2.2.drive", &generated_type_4, &generated_default_1134, generated_targets_1134, 1 },
    { "ships.antaran.2.2.armor", &generated_type_5, &generated_default_1135, generated_targets_1135, 1 },
    { "ships.antaran.2.2.shield", &generated_type_6, &generated_default_1136, generated_targets_1136, 1 },
    { "ships.antaran.2.2.computer", &generated_type_7, &generated_default_1137, generated_targets_1137, 1 },
    { "ships.antaran.2.2.weapons.1.type", &generated_type_8, &generated_default_1138, generated_targets_1138, 1 },
    { "ships.antaran.2.2.weapons.1.arc", &generated_type_9, &generated_default_1139, generated_targets_1139, 1 },
    { "ships.antaran.2.2.weapons.1.mods", &generated_type_10, &generated_default_1140, generated_targets_1140, 1 },
    { "ships.antaran.2.2.weapons.1.ammo", &generated_type_2, &generated_default_1141, generated_targets_1141, 1 },
    { "ships.antaran.2.2.weapons.1.amount", &generated_type_11, &generated_default_1142, generated_targets_1142, 2 },
    { "ships.antaran.2.2.weapons.2.type", &generated_type_8, &generated_default_1143, generated_targets_1143, 1 },
    { "ships.antaran.2.2.weapons.2.arc", &generated_type_9, &generated_default_1144, generated_targets_1144, 1 },
    { "ships.antaran.2.2.weapons.2.mods", &generated_type_10, &generated_default_1145, generated_targets_1145, 1 },
    { "ships.antaran.2.2.weapons.2.ammo", &generated_type_2, &generated_default_1146, generated_targets_1146, 1 },
    { "ships.antaran.2.2.weapons.2.amount", &generated_type_11, &generated_default_1147, generated_targets_1147, 2 },
    { "ships.antaran.2.2.weapons.3.type", &generated_type_8, &generated_default_1148, generated_targets_1148, 1 },
    { "ships.antaran.2.2.weapons.3.arc", &generated_type_9, &generated_default_1149, generated_targets_1149, 1 },
    { "ships.antaran.2.2.weapons.3.mods", &generated_type_10, &generated_default_1150, generated_targets_1150, 1 },
    { "ships.antaran.2.2.weapons.3.ammo", &generated_type_2, &generated_default_1151, generated_targets_1151, 1 },
    { "ships.antaran.2.2.weapons.3.amount", &generated_type_11, &generated_default_1152, generated_targets_1152, 2 },
    { "ships.antaran.2.2.weapons.4.type", &generated_type_8, &generated_default_1153, generated_targets_1153, 1 },
    { "ships.antaran.2.2.weapons.4.arc", &generated_type_9, &generated_default_1154, generated_targets_1154, 1 },
    { "ships.antaran.2.2.weapons.4.mods", &generated_type_10, &generated_default_1155, generated_targets_1155, 1 },
    { "ships.antaran.2.2.weapons.4.ammo", &generated_type_2, &generated_default_1156, generated_targets_1156, 1 },
    { "ships.antaran.2.2.weapons.4.amount", &generated_type_11, &generated_default_1157, generated_targets_1157, 2 },
    { "ships.antaran.2.2.weapons.5.type", &generated_type_8, &generated_default_1158, generated_targets_1158, 1 },
    { "ships.antaran.2.2.weapons.5.arc", &generated_type_9, &generated_default_1159, generated_targets_1159, 1 },
    { "ships.antaran.2.2.weapons.5.mods", &generated_type_10, &generated_default_1160, generated_targets_1160, 1 },
    { "ships.antaran.2.2.weapons.5.ammo", &generated_type_2, &generated_default_1161, generated_targets_1161, 1 },
    { "ships.antaran.2.2.weapons.5.amount", &generated_type_11, &generated_default_1162, generated_targets_1162, 2 },
    { "ships.antaran.2.2.weapons.6.type", &generated_type_8, &generated_default_1163, generated_targets_1163, 1 },
    { "ships.antaran.2.2.weapons.6.arc", &generated_type_9, &generated_default_1164, generated_targets_1164, 1 },
    { "ships.antaran.2.2.weapons.6.mods", &generated_type_10, &generated_default_1165, generated_targets_1165, 1 },
    { "ships.antaran.2.2.weapons.6.ammo", &generated_type_2, &generated_default_1166, generated_targets_1166, 1 },
    { "ships.antaran.2.2.weapons.6.amount", &generated_type_11, &generated_default_1167, generated_targets_1167, 2 },
    { "ships.antaran.2.2.weapons.7.type", &generated_type_8, &generated_default_1168, generated_targets_1168, 1 },
    { "ships.antaran.2.2.weapons.7.arc", &generated_type_9, &generated_default_1169, generated_targets_1169, 1 },
    { "ships.antaran.2.2.weapons.7.mods", &generated_type_10, &generated_default_1170, generated_targets_1170, 1 },
    { "ships.antaran.2.2.weapons.7.ammo", &generated_type_2, &generated_default_1171, generated_targets_1171, 1 },
    { "ships.antaran.2.2.weapons.7.amount", &generated_type_11, &generated_default_1172, generated_targets_1172, 2 },
    { "ships.antaran.2.2.weapons.8.type", &generated_type_8, &generated_default_1173, generated_targets_1173, 1 },
    { "ships.antaran.2.2.weapons.8.arc", &generated_type_9, &generated_default_1174, generated_targets_1174, 1 },
    { "ships.antaran.2.2.weapons.8.mods", &generated_type_10, &generated_default_1175, generated_targets_1175, 1 },
    { "ships.antaran.2.2.weapons.8.ammo", &generated_type_2, &generated_default_1176, generated_targets_1176, 1 },
    { "ships.antaran.2.2.weapons.8.amount", &generated_type_11, &generated_default_1177, generated_targets_1177, 2 },
    { "ships.antaran.2.2.specials", &generated_type_12, &generated_default_1178, generated_targets_1178, 1 },
    { "ships.antaran.2.3.appearance", &generated_type_2, &generated_default_1179, generated_targets_1179, 1 },
    { "ships.antaran.2.3.hull_size", &generated_type_3, &generated_default_1180, generated_targets_1180, 1 },
    { "ships.antaran.2.3.drive", &generated_type_4, &generated_default_1181, generated_targets_1181, 1 },
    { "ships.antaran.2.3.armor", &generated_type_5, &generated_default_1182, generated_targets_1182, 1 },
    { "ships.antaran.2.3.shield", &generated_type_6, &generated_default_1183, generated_targets_1183, 1 },
    { "ships.antaran.2.3.computer", &generated_type_7, &generated_default_1184, generated_targets_1184, 1 },
    { "ships.antaran.2.3.weapons.1.type", &generated_type_8, &generated_default_1185, generated_targets_1185, 1 },
    { "ships.antaran.2.3.weapons.1.arc", &generated_type_9, &generated_default_1186, generated_targets_1186, 1 },
    { "ships.antaran.2.3.weapons.1.mods", &generated_type_10, &generated_default_1187, generated_targets_1187, 1 },
    { "ships.antaran.2.3.weapons.1.ammo", &generated_type_2, &generated_default_1188, generated_targets_1188, 1 },
    { "ships.antaran.2.3.weapons.1.amount", &generated_type_11, &generated_default_1189, generated_targets_1189, 2 },
    { "ships.antaran.2.3.weapons.2.type", &generated_type_8, &generated_default_1190, generated_targets_1190, 1 },
    { "ships.antaran.2.3.weapons.2.arc", &generated_type_9, &generated_default_1191, generated_targets_1191, 1 },
    { "ships.antaran.2.3.weapons.2.mods", &generated_type_10, &generated_default_1192, generated_targets_1192, 1 },
    { "ships.antaran.2.3.weapons.2.ammo", &generated_type_2, &generated_default_1193, generated_targets_1193, 1 },
    { "ships.antaran.2.3.weapons.2.amount", &generated_type_11, &generated_default_1194, generated_targets_1194, 2 },
    { "ships.antaran.2.3.weapons.3.type", &generated_type_8, &generated_default_1195, generated_targets_1195, 1 },
    { "ships.antaran.2.3.weapons.3.arc", &generated_type_9, &generated_default_1196, generated_targets_1196, 1 },
    { "ships.antaran.2.3.weapons.3.mods", &generated_type_10, &generated_default_1197, generated_targets_1197, 1 },
    { "ships.antaran.2.3.weapons.3.ammo", &generated_type_2, &generated_default_1198, generated_targets_1198, 1 },
    { "ships.antaran.2.3.weapons.3.amount", &generated_type_11, &generated_default_1199, generated_targets_1199, 2 },
    { "ships.antaran.2.3.weapons.4.type", &generated_type_8, &generated_default_1200, generated_targets_1200, 1 },
    { "ships.antaran.2.3.weapons.4.arc", &generated_type_9, &generated_default_1201, generated_targets_1201, 1 },
    { "ships.antaran.2.3.weapons.4.mods", &generated_type_10, &generated_default_1202, generated_targets_1202, 1 },
    { "ships.antaran.2.3.weapons.4.ammo", &generated_type_2, &generated_default_1203, generated_targets_1203, 1 },
    { "ships.antaran.2.3.weapons.4.amount", &generated_type_11, &generated_default_1204, generated_targets_1204, 2 },
    { "ships.antaran.2.3.weapons.5.type", &generated_type_8, &generated_default_1205, generated_targets_1205, 1 },
    { "ships.antaran.2.3.weapons.5.arc", &generated_type_9, &generated_default_1206, generated_targets_1206, 1 },
    { "ships.antaran.2.3.weapons.5.mods", &generated_type_10, &generated_default_1207, generated_targets_1207, 1 },
    { "ships.antaran.2.3.weapons.5.ammo", &generated_type_2, &generated_default_1208, generated_targets_1208, 1 },
    { "ships.antaran.2.3.weapons.5.amount", &generated_type_11, &generated_default_1209, generated_targets_1209, 2 },
    { "ships.antaran.2.3.weapons.6.type", &generated_type_8, &generated_default_1210, generated_targets_1210, 1 },
    { "ships.antaran.2.3.weapons.6.arc", &generated_type_9, &generated_default_1211, generated_targets_1211, 1 },
    { "ships.antaran.2.3.weapons.6.mods", &generated_type_10, &generated_default_1212, generated_targets_1212, 1 },
    { "ships.antaran.2.3.weapons.6.ammo", &generated_type_2, &generated_default_1213, generated_targets_1213, 1 },
    { "ships.antaran.2.3.weapons.6.amount", &generated_type_11, &generated_default_1214, generated_targets_1214, 2 },
    { "ships.antaran.2.3.weapons.7.type", &generated_type_8, &generated_default_1215, generated_targets_1215, 1 },
    { "ships.antaran.2.3.weapons.7.arc", &generated_type_9, &generated_default_1216, generated_targets_1216, 1 },
    { "ships.antaran.2.3.weapons.7.mods", &generated_type_10, &generated_default_1217, generated_targets_1217, 1 },
    { "ships.antaran.2.3.weapons.7.ammo", &generated_type_2, &generated_default_1218, generated_targets_1218, 1 },
    { "ships.antaran.2.3.weapons.7.amount", &generated_type_11, &generated_default_1219, generated_targets_1219, 2 },
    { "ships.antaran.2.3.weapons.8.type", &generated_type_8, &generated_default_1220, generated_targets_1220, 1 },
    { "ships.antaran.2.3.weapons.8.arc", &generated_type_9, &generated_default_1221, generated_targets_1221, 1 },
    { "ships.antaran.2.3.weapons.8.mods", &generated_type_10, &generated_default_1222, generated_targets_1222, 1 },
    { "ships.antaran.2.3.weapons.8.ammo", &generated_type_2, &generated_default_1223, generated_targets_1223, 1 },
    { "ships.antaran.2.3.weapons.8.amount", &generated_type_11, &generated_default_1224, generated_targets_1224, 2 },
    { "ships.antaran.2.3.specials", &generated_type_12, &generated_default_1225, generated_targets_1225, 1 },
    { "ships.antaran.2.4.appearance", &generated_type_2, &generated_default_1226, generated_targets_1226, 1 },
    { "ships.antaran.2.4.hull_size", &generated_type_3, &generated_default_1227, generated_targets_1227, 1 },
    { "ships.antaran.2.4.drive", &generated_type_4, &generated_default_1228, generated_targets_1228, 1 },
    { "ships.antaran.2.4.armor", &generated_type_5, &generated_default_1229, generated_targets_1229, 1 },
    { "ships.antaran.2.4.shield", &generated_type_6, &generated_default_1230, generated_targets_1230, 1 },
    { "ships.antaran.2.4.computer", &generated_type_7, &generated_default_1231, generated_targets_1231, 1 },
    { "ships.antaran.2.4.weapons.1.type", &generated_type_8, &generated_default_1232, generated_targets_1232, 1 },
    { "ships.antaran.2.4.weapons.1.arc", &generated_type_9, &generated_default_1233, generated_targets_1233, 1 },
    { "ships.antaran.2.4.weapons.1.mods", &generated_type_10, &generated_default_1234, generated_targets_1234, 1 },
    { "ships.antaran.2.4.weapons.1.ammo", &generated_type_2, &generated_default_1235, generated_targets_1235, 1 },
    { "ships.antaran.2.4.weapons.1.amount", &generated_type_11, &generated_default_1236, generated_targets_1236, 2 },
    { "ships.antaran.2.4.weapons.2.type", &generated_type_8, &generated_default_1237, generated_targets_1237, 1 },
    { "ships.antaran.2.4.weapons.2.arc", &generated_type_9, &generated_default_1238, generated_targets_1238, 1 },
    { "ships.antaran.2.4.weapons.2.mods", &generated_type_10, &generated_default_1239, generated_targets_1239, 1 },
    { "ships.antaran.2.4.weapons.2.ammo", &generated_type_2, &generated_default_1240, generated_targets_1240, 1 },
    { "ships.antaran.2.4.weapons.2.amount", &generated_type_11, &generated_default_1241, generated_targets_1241, 2 },
    { "ships.antaran.2.4.weapons.3.type", &generated_type_8, &generated_default_1242, generated_targets_1242, 1 },
    { "ships.antaran.2.4.weapons.3.arc", &generated_type_9, &generated_default_1243, generated_targets_1243, 1 },
    { "ships.antaran.2.4.weapons.3.mods", &generated_type_10, &generated_default_1244, generated_targets_1244, 1 },
    { "ships.antaran.2.4.weapons.3.ammo", &generated_type_2, &generated_default_1245, generated_targets_1245, 1 },
    { "ships.antaran.2.4.weapons.3.amount", &generated_type_11, &generated_default_1246, generated_targets_1246, 2 },
    { "ships.antaran.2.4.weapons.4.type", &generated_type_8, &generated_default_1247, generated_targets_1247, 1 },
    { "ships.antaran.2.4.weapons.4.arc", &generated_type_9, &generated_default_1248, generated_targets_1248, 1 },
    { "ships.antaran.2.4.weapons.4.mods", &generated_type_10, &generated_default_1249, generated_targets_1249, 1 },
    { "ships.antaran.2.4.weapons.4.ammo", &generated_type_2, &generated_default_1250, generated_targets_1250, 1 },
    { "ships.antaran.2.4.weapons.4.amount", &generated_type_11, &generated_default_1251, generated_targets_1251, 2 },
    { "ships.antaran.2.4.weapons.5.type", &generated_type_8, &generated_default_1252, generated_targets_1252, 1 },
    { "ships.antaran.2.4.weapons.5.arc", &generated_type_9, &generated_default_1253, generated_targets_1253, 1 },
    { "ships.antaran.2.4.weapons.5.mods", &generated_type_10, &generated_default_1254, generated_targets_1254, 1 },
    { "ships.antaran.2.4.weapons.5.ammo", &generated_type_2, &generated_default_1255, generated_targets_1255, 1 },
    { "ships.antaran.2.4.weapons.5.amount", &generated_type_11, &generated_default_1256, generated_targets_1256, 2 },
    { "ships.antaran.2.4.weapons.6.type", &generated_type_8, &generated_default_1257, generated_targets_1257, 1 },
    { "ships.antaran.2.4.weapons.6.arc", &generated_type_9, &generated_default_1258, generated_targets_1258, 1 },
    { "ships.antaran.2.4.weapons.6.mods", &generated_type_10, &generated_default_1259, generated_targets_1259, 1 },
    { "ships.antaran.2.4.weapons.6.ammo", &generated_type_2, &generated_default_1260, generated_targets_1260, 1 },
    { "ships.antaran.2.4.weapons.6.amount", &generated_type_11, &generated_default_1261, generated_targets_1261, 2 },
    { "ships.antaran.2.4.weapons.7.type", &generated_type_8, &generated_default_1262, generated_targets_1262, 1 },
    { "ships.antaran.2.4.weapons.7.arc", &generated_type_9, &generated_default_1263, generated_targets_1263, 1 },
    { "ships.antaran.2.4.weapons.7.mods", &generated_type_10, &generated_default_1264, generated_targets_1264, 1 },
    { "ships.antaran.2.4.weapons.7.ammo", &generated_type_2, &generated_default_1265, generated_targets_1265, 1 },
    { "ships.antaran.2.4.weapons.7.amount", &generated_type_11, &generated_default_1266, generated_targets_1266, 2 },
    { "ships.antaran.2.4.weapons.8.type", &generated_type_8, &generated_default_1267, generated_targets_1267, 1 },
    { "ships.antaran.2.4.weapons.8.arc", &generated_type_9, &generated_default_1268, generated_targets_1268, 1 },
    { "ships.antaran.2.4.weapons.8.mods", &generated_type_10, &generated_default_1269, generated_targets_1269, 1 },
    { "ships.antaran.2.4.weapons.8.ammo", &generated_type_2, &generated_default_1270, generated_targets_1270, 1 },
    { "ships.antaran.2.4.weapons.8.amount", &generated_type_11, &generated_default_1271, generated_targets_1271, 2 },
    { "ships.antaran.2.4.specials", &generated_type_12, &generated_default_1272, generated_targets_1272, 1 },
    { "ships.antaran.3.0.appearance", &generated_type_2, &generated_default_1273, generated_targets_1273, 1 },
    { "ships.antaran.3.0.hull_size", &generated_type_3, &generated_default_1274, generated_targets_1274, 1 },
    { "ships.antaran.3.0.drive", &generated_type_4, &generated_default_1275, generated_targets_1275, 1 },
    { "ships.antaran.3.0.armor", &generated_type_5, &generated_default_1276, generated_targets_1276, 1 },
    { "ships.antaran.3.0.shield", &generated_type_6, &generated_default_1277, generated_targets_1277, 1 },
    { "ships.antaran.3.0.computer", &generated_type_7, &generated_default_1278, generated_targets_1278, 1 },
    { "ships.antaran.3.0.weapons.1.type", &generated_type_8, &generated_default_1279, generated_targets_1279, 1 },
    { "ships.antaran.3.0.weapons.1.arc", &generated_type_9, &generated_default_1280, generated_targets_1280, 1 },
    { "ships.antaran.3.0.weapons.1.mods", &generated_type_10, &generated_default_1281, generated_targets_1281, 1 },
    { "ships.antaran.3.0.weapons.1.ammo", &generated_type_2, &generated_default_1282, generated_targets_1282, 1 },
    { "ships.antaran.3.0.weapons.1.amount", &generated_type_11, &generated_default_1283, generated_targets_1283, 2 },
    { "ships.antaran.3.0.weapons.2.type", &generated_type_8, &generated_default_1284, generated_targets_1284, 1 },
    { "ships.antaran.3.0.weapons.2.arc", &generated_type_9, &generated_default_1285, generated_targets_1285, 1 },
    { "ships.antaran.3.0.weapons.2.mods", &generated_type_10, &generated_default_1286, generated_targets_1286, 1 },
    { "ships.antaran.3.0.weapons.2.ammo", &generated_type_2, &generated_default_1287, generated_targets_1287, 1 },
    { "ships.antaran.3.0.weapons.2.amount", &generated_type_11, &generated_default_1288, generated_targets_1288, 2 },
    { "ships.antaran.3.0.weapons.3.type", &generated_type_8, &generated_default_1289, generated_targets_1289, 1 },
    { "ships.antaran.3.0.weapons.3.arc", &generated_type_9, &generated_default_1290, generated_targets_1290, 1 },
    { "ships.antaran.3.0.weapons.3.mods", &generated_type_10, &generated_default_1291, generated_targets_1291, 1 },
    { "ships.antaran.3.0.weapons.3.ammo", &generated_type_2, &generated_default_1292, generated_targets_1292, 1 },
    { "ships.antaran.3.0.weapons.3.amount", &generated_type_11, &generated_default_1293, generated_targets_1293, 2 },
    { "ships.antaran.3.0.weapons.4.type", &generated_type_8, &generated_default_1294, generated_targets_1294, 1 },
    { "ships.antaran.3.0.weapons.4.arc", &generated_type_9, &generated_default_1295, generated_targets_1295, 1 },
    { "ships.antaran.3.0.weapons.4.mods", &generated_type_10, &generated_default_1296, generated_targets_1296, 1 },
    { "ships.antaran.3.0.weapons.4.ammo", &generated_type_2, &generated_default_1297, generated_targets_1297, 1 },
    { "ships.antaran.3.0.weapons.4.amount", &generated_type_11, &generated_default_1298, generated_targets_1298, 2 },
    { "ships.antaran.3.0.weapons.5.type", &generated_type_8, &generated_default_1299, generated_targets_1299, 1 },
    { "ships.antaran.3.0.weapons.5.arc", &generated_type_9, &generated_default_1300, generated_targets_1300, 1 },
    { "ships.antaran.3.0.weapons.5.mods", &generated_type_10, &generated_default_1301, generated_targets_1301, 1 },
    { "ships.antaran.3.0.weapons.5.ammo", &generated_type_2, &generated_default_1302, generated_targets_1302, 1 },
    { "ships.antaran.3.0.weapons.5.amount", &generated_type_11, &generated_default_1303, generated_targets_1303, 2 },
    { "ships.antaran.3.0.weapons.6.type", &generated_type_8, &generated_default_1304, generated_targets_1304, 1 },
    { "ships.antaran.3.0.weapons.6.arc", &generated_type_9, &generated_default_1305, generated_targets_1305, 1 },
    { "ships.antaran.3.0.weapons.6.mods", &generated_type_10, &generated_default_1306, generated_targets_1306, 1 },
    { "ships.antaran.3.0.weapons.6.ammo", &generated_type_2, &generated_default_1307, generated_targets_1307, 1 },
    { "ships.antaran.3.0.weapons.6.amount", &generated_type_11, &generated_default_1308, generated_targets_1308, 2 },
    { "ships.antaran.3.0.weapons.7.type", &generated_type_8, &generated_default_1309, generated_targets_1309, 1 },
    { "ships.antaran.3.0.weapons.7.arc", &generated_type_9, &generated_default_1310, generated_targets_1310, 1 },
    { "ships.antaran.3.0.weapons.7.mods", &generated_type_10, &generated_default_1311, generated_targets_1311, 1 },
    { "ships.antaran.3.0.weapons.7.ammo", &generated_type_2, &generated_default_1312, generated_targets_1312, 1 },
    { "ships.antaran.3.0.weapons.7.amount", &generated_type_11, &generated_default_1313, generated_targets_1313, 2 },
    { "ships.antaran.3.0.weapons.8.type", &generated_type_8, &generated_default_1314, generated_targets_1314, 1 },
    { "ships.antaran.3.0.weapons.8.arc", &generated_type_9, &generated_default_1315, generated_targets_1315, 1 },
    { "ships.antaran.3.0.weapons.8.mods", &generated_type_10, &generated_default_1316, generated_targets_1316, 1 },
    { "ships.antaran.3.0.weapons.8.ammo", &generated_type_2, &generated_default_1317, generated_targets_1317, 1 },
    { "ships.antaran.3.0.weapons.8.amount", &generated_type_11, &generated_default_1318, generated_targets_1318, 2 },
    { "ships.antaran.3.0.specials", &generated_type_12, &generated_default_1319, generated_targets_1319, 1 },
    { "ships.antaran.3.1.appearance", &generated_type_2, &generated_default_1320, generated_targets_1320, 1 },
    { "ships.antaran.3.1.hull_size", &generated_type_3, &generated_default_1321, generated_targets_1321, 1 },
    { "ships.antaran.3.1.drive", &generated_type_4, &generated_default_1322, generated_targets_1322, 1 },
    { "ships.antaran.3.1.armor", &generated_type_5, &generated_default_1323, generated_targets_1323, 1 },
    { "ships.antaran.3.1.shield", &generated_type_6, &generated_default_1324, generated_targets_1324, 1 },
    { "ships.antaran.3.1.computer", &generated_type_7, &generated_default_1325, generated_targets_1325, 1 },
    { "ships.antaran.3.1.weapons.1.type", &generated_type_8, &generated_default_1326, generated_targets_1326, 1 },
    { "ships.antaran.3.1.weapons.1.arc", &generated_type_9, &generated_default_1327, generated_targets_1327, 1 },
    { "ships.antaran.3.1.weapons.1.mods", &generated_type_10, &generated_default_1328, generated_targets_1328, 1 },
    { "ships.antaran.3.1.weapons.1.ammo", &generated_type_2, &generated_default_1329, generated_targets_1329, 1 },
    { "ships.antaran.3.1.weapons.1.amount", &generated_type_11, &generated_default_1330, generated_targets_1330, 2 },
    { "ships.antaran.3.1.weapons.2.type", &generated_type_8, &generated_default_1331, generated_targets_1331, 1 },
    { "ships.antaran.3.1.weapons.2.arc", &generated_type_9, &generated_default_1332, generated_targets_1332, 1 },
    { "ships.antaran.3.1.weapons.2.mods", &generated_type_10, &generated_default_1333, generated_targets_1333, 1 },
    { "ships.antaran.3.1.weapons.2.ammo", &generated_type_2, &generated_default_1334, generated_targets_1334, 1 },
    { "ships.antaran.3.1.weapons.2.amount", &generated_type_11, &generated_default_1335, generated_targets_1335, 2 },
    { "ships.antaran.3.1.weapons.3.type", &generated_type_8, &generated_default_1336, generated_targets_1336, 1 },
    { "ships.antaran.3.1.weapons.3.arc", &generated_type_9, &generated_default_1337, generated_targets_1337, 1 },
    { "ships.antaran.3.1.weapons.3.mods", &generated_type_10, &generated_default_1338, generated_targets_1338, 1 },
    { "ships.antaran.3.1.weapons.3.ammo", &generated_type_2, &generated_default_1339, generated_targets_1339, 1 },
    { "ships.antaran.3.1.weapons.3.amount", &generated_type_11, &generated_default_1340, generated_targets_1340, 2 },
    { "ships.antaran.3.1.weapons.4.type", &generated_type_8, &generated_default_1341, generated_targets_1341, 1 },
    { "ships.antaran.3.1.weapons.4.arc", &generated_type_9, &generated_default_1342, generated_targets_1342, 1 },
    { "ships.antaran.3.1.weapons.4.mods", &generated_type_10, &generated_default_1343, generated_targets_1343, 1 },
    { "ships.antaran.3.1.weapons.4.ammo", &generated_type_2, &generated_default_1344, generated_targets_1344, 1 },
    { "ships.antaran.3.1.weapons.4.amount", &generated_type_11, &generated_default_1345, generated_targets_1345, 2 },
    { "ships.antaran.3.1.weapons.5.type", &generated_type_8, &generated_default_1346, generated_targets_1346, 1 },
    { "ships.antaran.3.1.weapons.5.arc", &generated_type_9, &generated_default_1347, generated_targets_1347, 1 },
    { "ships.antaran.3.1.weapons.5.mods", &generated_type_10, &generated_default_1348, generated_targets_1348, 1 },
    { "ships.antaran.3.1.weapons.5.ammo", &generated_type_2, &generated_default_1349, generated_targets_1349, 1 },
    { "ships.antaran.3.1.weapons.5.amount", &generated_type_11, &generated_default_1350, generated_targets_1350, 2 },
    { "ships.antaran.3.1.weapons.6.type", &generated_type_8, &generated_default_1351, generated_targets_1351, 1 },
    { "ships.antaran.3.1.weapons.6.arc", &generated_type_9, &generated_default_1352, generated_targets_1352, 1 },
    { "ships.antaran.3.1.weapons.6.mods", &generated_type_10, &generated_default_1353, generated_targets_1353, 1 },
    { "ships.antaran.3.1.weapons.6.ammo", &generated_type_2, &generated_default_1354, generated_targets_1354, 1 },
    { "ships.antaran.3.1.weapons.6.amount", &generated_type_11, &generated_default_1355, generated_targets_1355, 2 },
    { "ships.antaran.3.1.weapons.7.type", &generated_type_8, &generated_default_1356, generated_targets_1356, 1 },
    { "ships.antaran.3.1.weapons.7.arc", &generated_type_9, &generated_default_1357, generated_targets_1357, 1 },
    { "ships.antaran.3.1.weapons.7.mods", &generated_type_10, &generated_default_1358, generated_targets_1358, 1 },
    { "ships.antaran.3.1.weapons.7.ammo", &generated_type_2, &generated_default_1359, generated_targets_1359, 1 },
    { "ships.antaran.3.1.weapons.7.amount", &generated_type_11, &generated_default_1360, generated_targets_1360, 2 },
    { "ships.antaran.3.1.weapons.8.type", &generated_type_8, &generated_default_1361, generated_targets_1361, 1 },
    { "ships.antaran.3.1.weapons.8.arc", &generated_type_9, &generated_default_1362, generated_targets_1362, 1 },
    { "ships.antaran.3.1.weapons.8.mods", &generated_type_10, &generated_default_1363, generated_targets_1363, 1 },
    { "ships.antaran.3.1.weapons.8.ammo", &generated_type_2, &generated_default_1364, generated_targets_1364, 1 },
    { "ships.antaran.3.1.weapons.8.amount", &generated_type_11, &generated_default_1365, generated_targets_1365, 2 },
    { "ships.antaran.3.1.specials", &generated_type_12, &generated_default_1366, generated_targets_1366, 1 },
    { "ships.antaran.3.2.appearance", &generated_type_2, &generated_default_1367, generated_targets_1367, 1 },
    { "ships.antaran.3.2.hull_size", &generated_type_3, &generated_default_1368, generated_targets_1368, 1 },
    { "ships.antaran.3.2.drive", &generated_type_4, &generated_default_1369, generated_targets_1369, 1 },
    { "ships.antaran.3.2.armor", &generated_type_5, &generated_default_1370, generated_targets_1370, 1 },
    { "ships.antaran.3.2.shield", &generated_type_6, &generated_default_1371, generated_targets_1371, 1 },
    { "ships.antaran.3.2.computer", &generated_type_7, &generated_default_1372, generated_targets_1372, 1 },
    { "ships.antaran.3.2.weapons.1.type", &generated_type_8, &generated_default_1373, generated_targets_1373, 1 },
    { "ships.antaran.3.2.weapons.1.arc", &generated_type_9, &generated_default_1374, generated_targets_1374, 1 },
    { "ships.antaran.3.2.weapons.1.mods", &generated_type_10, &generated_default_1375, generated_targets_1375, 1 },
    { "ships.antaran.3.2.weapons.1.ammo", &generated_type_2, &generated_default_1376, generated_targets_1376, 1 },
    { "ships.antaran.3.2.weapons.1.amount", &generated_type_11, &generated_default_1377, generated_targets_1377, 2 },
    { "ships.antaran.3.2.weapons.2.type", &generated_type_8, &generated_default_1378, generated_targets_1378, 1 },
    { "ships.antaran.3.2.weapons.2.arc", &generated_type_9, &generated_default_1379, generated_targets_1379, 1 },
    { "ships.antaran.3.2.weapons.2.mods", &generated_type_10, &generated_default_1380, generated_targets_1380, 1 },
    { "ships.antaran.3.2.weapons.2.ammo", &generated_type_2, &generated_default_1381, generated_targets_1381, 1 },
    { "ships.antaran.3.2.weapons.2.amount", &generated_type_11, &generated_default_1382, generated_targets_1382, 2 },
    { "ships.antaran.3.2.weapons.3.type", &generated_type_8, &generated_default_1383, generated_targets_1383, 1 },
    { "ships.antaran.3.2.weapons.3.arc", &generated_type_9, &generated_default_1384, generated_targets_1384, 1 },
    { "ships.antaran.3.2.weapons.3.mods", &generated_type_10, &generated_default_1385, generated_targets_1385, 1 },
    { "ships.antaran.3.2.weapons.3.ammo", &generated_type_2, &generated_default_1386, generated_targets_1386, 1 },
    { "ships.antaran.3.2.weapons.3.amount", &generated_type_11, &generated_default_1387, generated_targets_1387, 2 },
    { "ships.antaran.3.2.weapons.4.type", &generated_type_8, &generated_default_1388, generated_targets_1388, 1 },
    { "ships.antaran.3.2.weapons.4.arc", &generated_type_9, &generated_default_1389, generated_targets_1389, 1 },
    { "ships.antaran.3.2.weapons.4.mods", &generated_type_10, &generated_default_1390, generated_targets_1390, 1 },
    { "ships.antaran.3.2.weapons.4.ammo", &generated_type_2, &generated_default_1391, generated_targets_1391, 1 },
    { "ships.antaran.3.2.weapons.4.amount", &generated_type_11, &generated_default_1392, generated_targets_1392, 2 },
    { "ships.antaran.3.2.weapons.5.type", &generated_type_8, &generated_default_1393, generated_targets_1393, 1 },
    { "ships.antaran.3.2.weapons.5.arc", &generated_type_9, &generated_default_1394, generated_targets_1394, 1 },
    { "ships.antaran.3.2.weapons.5.mods", &generated_type_10, &generated_default_1395, generated_targets_1395, 1 },
    { "ships.antaran.3.2.weapons.5.ammo", &generated_type_2, &generated_default_1396, generated_targets_1396, 1 },
    { "ships.antaran.3.2.weapons.5.amount", &generated_type_11, &generated_default_1397, generated_targets_1397, 2 },
    { "ships.antaran.3.2.weapons.6.type", &generated_type_8, &generated_default_1398, generated_targets_1398, 1 },
    { "ships.antaran.3.2.weapons.6.arc", &generated_type_9, &generated_default_1399, generated_targets_1399, 1 },
    { "ships.antaran.3.2.weapons.6.mods", &generated_type_10, &generated_default_1400, generated_targets_1400, 1 },
    { "ships.antaran.3.2.weapons.6.ammo", &generated_type_2, &generated_default_1401, generated_targets_1401, 1 },
    { "ships.antaran.3.2.weapons.6.amount", &generated_type_11, &generated_default_1402, generated_targets_1402, 2 },
    { "ships.antaran.3.2.weapons.7.type", &generated_type_8, &generated_default_1403, generated_targets_1403, 1 },
    { "ships.antaran.3.2.weapons.7.arc", &generated_type_9, &generated_default_1404, generated_targets_1404, 1 },
    { "ships.antaran.3.2.weapons.7.mods", &generated_type_10, &generated_default_1405, generated_targets_1405, 1 },
    { "ships.antaran.3.2.weapons.7.ammo", &generated_type_2, &generated_default_1406, generated_targets_1406, 1 },
    { "ships.antaran.3.2.weapons.7.amount", &generated_type_11, &generated_default_1407, generated_targets_1407, 2 },
    { "ships.antaran.3.2.weapons.8.type", &generated_type_8, &generated_default_1408, generated_targets_1408, 1 },
    { "ships.antaran.3.2.weapons.8.arc", &generated_type_9, &generated_default_1409, generated_targets_1409, 1 },
    { "ships.antaran.3.2.weapons.8.mods", &generated_type_10, &generated_default_1410, generated_targets_1410, 1 },
    { "ships.antaran.3.2.weapons.8.ammo", &generated_type_2, &generated_default_1411, generated_targets_1411, 1 },
    { "ships.antaran.3.2.weapons.8.amount", &generated_type_11, &generated_default_1412, generated_targets_1412, 2 },
    { "ships.antaran.3.2.specials", &generated_type_12, &generated_default_1413, generated_targets_1413, 1 },
    { "ships.antaran.3.3.appearance", &generated_type_2, &generated_default_1414, generated_targets_1414, 1 },
    { "ships.antaran.3.3.hull_size", &generated_type_3, &generated_default_1415, generated_targets_1415, 1 },
    { "ships.antaran.3.3.drive", &generated_type_4, &generated_default_1416, generated_targets_1416, 1 },
    { "ships.antaran.3.3.armor", &generated_type_5, &generated_default_1417, generated_targets_1417, 1 },
    { "ships.antaran.3.3.shield", &generated_type_6, &generated_default_1418, generated_targets_1418, 1 },
    { "ships.antaran.3.3.computer", &generated_type_7, &generated_default_1419, generated_targets_1419, 1 },
    { "ships.antaran.3.3.weapons.1.type", &generated_type_8, &generated_default_1420, generated_targets_1420, 1 },
    { "ships.antaran.3.3.weapons.1.arc", &generated_type_9, &generated_default_1421, generated_targets_1421, 1 },
    { "ships.antaran.3.3.weapons.1.mods", &generated_type_10, &generated_default_1422, generated_targets_1422, 1 },
    { "ships.antaran.3.3.weapons.1.ammo", &generated_type_2, &generated_default_1423, generated_targets_1423, 1 },
    { "ships.antaran.3.3.weapons.1.amount", &generated_type_11, &generated_default_1424, generated_targets_1424, 2 },
    { "ships.antaran.3.3.weapons.2.type", &generated_type_8, &generated_default_1425, generated_targets_1425, 1 },
    { "ships.antaran.3.3.weapons.2.arc", &generated_type_9, &generated_default_1426, generated_targets_1426, 1 },
    { "ships.antaran.3.3.weapons.2.mods", &generated_type_10, &generated_default_1427, generated_targets_1427, 1 },
    { "ships.antaran.3.3.weapons.2.ammo", &generated_type_2, &generated_default_1428, generated_targets_1428, 1 },
    { "ships.antaran.3.3.weapons.2.amount", &generated_type_11, &generated_default_1429, generated_targets_1429, 2 },
    { "ships.antaran.3.3.weapons.3.type", &generated_type_8, &generated_default_1430, generated_targets_1430, 1 },
    { "ships.antaran.3.3.weapons.3.arc", &generated_type_9, &generated_default_1431, generated_targets_1431, 1 },
    { "ships.antaran.3.3.weapons.3.mods", &generated_type_10, &generated_default_1432, generated_targets_1432, 1 },
    { "ships.antaran.3.3.weapons.3.ammo", &generated_type_2, &generated_default_1433, generated_targets_1433, 1 },
    { "ships.antaran.3.3.weapons.3.amount", &generated_type_11, &generated_default_1434, generated_targets_1434, 2 },
    { "ships.antaran.3.3.weapons.4.type", &generated_type_8, &generated_default_1435, generated_targets_1435, 1 },
    { "ships.antaran.3.3.weapons.4.arc", &generated_type_9, &generated_default_1436, generated_targets_1436, 1 },
    { "ships.antaran.3.3.weapons.4.mods", &generated_type_10, &generated_default_1437, generated_targets_1437, 1 },
    { "ships.antaran.3.3.weapons.4.ammo", &generated_type_2, &generated_default_1438, generated_targets_1438, 1 },
    { "ships.antaran.3.3.weapons.4.amount", &generated_type_11, &generated_default_1439, generated_targets_1439, 2 },
    { "ships.antaran.3.3.weapons.5.type", &generated_type_8, &generated_default_1440, generated_targets_1440, 1 },
    { "ships.antaran.3.3.weapons.5.arc", &generated_type_9, &generated_default_1441, generated_targets_1441, 1 },
    { "ships.antaran.3.3.weapons.5.mods", &generated_type_10, &generated_default_1442, generated_targets_1442, 1 },
    { "ships.antaran.3.3.weapons.5.ammo", &generated_type_2, &generated_default_1443, generated_targets_1443, 1 },
    { "ships.antaran.3.3.weapons.5.amount", &generated_type_11, &generated_default_1444, generated_targets_1444, 2 },
    { "ships.antaran.3.3.weapons.6.type", &generated_type_8, &generated_default_1445, generated_targets_1445, 1 },
    { "ships.antaran.3.3.weapons.6.arc", &generated_type_9, &generated_default_1446, generated_targets_1446, 1 },
    { "ships.antaran.3.3.weapons.6.mods", &generated_type_10, &generated_default_1447, generated_targets_1447, 1 },
    { "ships.antaran.3.3.weapons.6.ammo", &generated_type_2, &generated_default_1448, generated_targets_1448, 1 },
    { "ships.antaran.3.3.weapons.6.amount", &generated_type_11, &generated_default_1449, generated_targets_1449, 2 },
    { "ships.antaran.3.3.weapons.7.type", &generated_type_8, &generated_default_1450, generated_targets_1450, 1 },
    { "ships.antaran.3.3.weapons.7.arc", &generated_type_9, &generated_default_1451, generated_targets_1451, 1 },
    { "ships.antaran.3.3.weapons.7.mods", &generated_type_10, &generated_default_1452, generated_targets_1452, 1 },
    { "ships.antaran.3.3.weapons.7.ammo", &generated_type_2, &generated_default_1453, generated_targets_1453, 1 },
    { "ships.antaran.3.3.weapons.7.amount", &generated_type_11, &generated_default_1454, generated_targets_1454, 2 },
    { "ships.antaran.3.3.weapons.8.type", &generated_type_8, &generated_default_1455, generated_targets_1455, 1 },
    { "ships.antaran.3.3.weapons.8.arc", &generated_type_9, &generated_default_1456, generated_targets_1456, 1 },
    { "ships.antaran.3.3.weapons.8.mods", &generated_type_10, &generated_default_1457, generated_targets_1457, 1 },
    { "ships.antaran.3.3.weapons.8.ammo", &generated_type_2, &generated_default_1458, generated_targets_1458, 1 },
    { "ships.antaran.3.3.weapons.8.amount", &generated_type_11, &generated_default_1459, generated_targets_1459, 2 },
    { "ships.antaran.3.3.specials", &generated_type_12, &generated_default_1460, generated_targets_1460, 1 },
    { "ships.antaran.3.4.appearance", &generated_type_2, &generated_default_1461, generated_targets_1461, 1 },
    { "ships.antaran.3.4.hull_size", &generated_type_3, &generated_default_1462, generated_targets_1462, 1 },
    { "ships.antaran.3.4.drive", &generated_type_4, &generated_default_1463, generated_targets_1463, 1 },
    { "ships.antaran.3.4.armor", &generated_type_5, &generated_default_1464, generated_targets_1464, 1 },
    { "ships.antaran.3.4.shield", &generated_type_6, &generated_default_1465, generated_targets_1465, 1 },
    { "ships.antaran.3.4.computer", &generated_type_7, &generated_default_1466, generated_targets_1466, 1 },
    { "ships.antaran.3.4.weapons.1.type", &generated_type_8, &generated_default_1467, generated_targets_1467, 1 },
    { "ships.antaran.3.4.weapons.1.arc", &generated_type_9, &generated_default_1468, generated_targets_1468, 1 },
    { "ships.antaran.3.4.weapons.1.mods", &generated_type_10, &generated_default_1469, generated_targets_1469, 1 },
    { "ships.antaran.3.4.weapons.1.ammo", &generated_type_2, &generated_default_1470, generated_targets_1470, 1 },
    { "ships.antaran.3.4.weapons.1.amount", &generated_type_11, &generated_default_1471, generated_targets_1471, 2 },
    { "ships.antaran.3.4.weapons.2.type", &generated_type_8, &generated_default_1472, generated_targets_1472, 1 },
    { "ships.antaran.3.4.weapons.2.arc", &generated_type_9, &generated_default_1473, generated_targets_1473, 1 },
    { "ships.antaran.3.4.weapons.2.mods", &generated_type_10, &generated_default_1474, generated_targets_1474, 1 },
    { "ships.antaran.3.4.weapons.2.ammo", &generated_type_2, &generated_default_1475, generated_targets_1475, 1 },
    { "ships.antaran.3.4.weapons.2.amount", &generated_type_11, &generated_default_1476, generated_targets_1476, 2 },
    { "ships.antaran.3.4.weapons.3.type", &generated_type_8, &generated_default_1477, generated_targets_1477, 1 },
    { "ships.antaran.3.4.weapons.3.arc", &generated_type_9, &generated_default_1478, generated_targets_1478, 1 },
    { "ships.antaran.3.4.weapons.3.mods", &generated_type_10, &generated_default_1479, generated_targets_1479, 1 },
    { "ships.antaran.3.4.weapons.3.ammo", &generated_type_2, &generated_default_1480, generated_targets_1480, 1 },
    { "ships.antaran.3.4.weapons.3.amount", &generated_type_11, &generated_default_1481, generated_targets_1481, 2 },
    { "ships.antaran.3.4.weapons.4.type", &generated_type_8, &generated_default_1482, generated_targets_1482, 1 },
    { "ships.antaran.3.4.weapons.4.arc", &generated_type_9, &generated_default_1483, generated_targets_1483, 1 },
    { "ships.antaran.3.4.weapons.4.mods", &generated_type_10, &generated_default_1484, generated_targets_1484, 1 },
    { "ships.antaran.3.4.weapons.4.ammo", &generated_type_2, &generated_default_1485, generated_targets_1485, 1 },
    { "ships.antaran.3.4.weapons.4.amount", &generated_type_11, &generated_default_1486, generated_targets_1486, 2 },
    { "ships.antaran.3.4.weapons.5.type", &generated_type_8, &generated_default_1487, generated_targets_1487, 1 },
    { "ships.antaran.3.4.weapons.5.arc", &generated_type_9, &generated_default_1488, generated_targets_1488, 1 },
    { "ships.antaran.3.4.weapons.5.mods", &generated_type_10, &generated_default_1489, generated_targets_1489, 1 },
    { "ships.antaran.3.4.weapons.5.ammo", &generated_type_2, &generated_default_1490, generated_targets_1490, 1 },
    { "ships.antaran.3.4.weapons.5.amount", &generated_type_11, &generated_default_1491, generated_targets_1491, 2 },
    { "ships.antaran.3.4.weapons.6.type", &generated_type_8, &generated_default_1492, generated_targets_1492, 1 },
    { "ships.antaran.3.4.weapons.6.arc", &generated_type_9, &generated_default_1493, generated_targets_1493, 1 },
    { "ships.antaran.3.4.weapons.6.mods", &generated_type_10, &generated_default_1494, generated_targets_1494, 1 },
    { "ships.antaran.3.4.weapons.6.ammo", &generated_type_2, &generated_default_1495, generated_targets_1495, 1 },
    { "ships.antaran.3.4.weapons.6.amount", &generated_type_11, &generated_default_1496, generated_targets_1496, 2 },
    { "ships.antaran.3.4.weapons.7.type", &generated_type_8, &generated_default_1497, generated_targets_1497, 1 },
    { "ships.antaran.3.4.weapons.7.arc", &generated_type_9, &generated_default_1498, generated_targets_1498, 1 },
    { "ships.antaran.3.4.weapons.7.mods", &generated_type_10, &generated_default_1499, generated_targets_1499, 1 },
    { "ships.antaran.3.4.weapons.7.ammo", &generated_type_2, &generated_default_1500, generated_targets_1500, 1 },
    { "ships.antaran.3.4.weapons.7.amount", &generated_type_11, &generated_default_1501, generated_targets_1501, 2 },
    { "ships.antaran.3.4.weapons.8.type", &generated_type_8, &generated_default_1502, generated_targets_1502, 1 },
    { "ships.antaran.3.4.weapons.8.arc", &generated_type_9, &generated_default_1503, generated_targets_1503, 1 },
    { "ships.antaran.3.4.weapons.8.mods", &generated_type_10, &generated_default_1504, generated_targets_1504, 1 },
    { "ships.antaran.3.4.weapons.8.ammo", &generated_type_2, &generated_default_1505, generated_targets_1505, 1 },
    { "ships.antaran.3.4.weapons.8.amount", &generated_type_11, &generated_default_1506, generated_targets_1506, 2 },
    { "ships.antaran.3.4.specials", &generated_type_12, &generated_default_1507, generated_targets_1507, 1 },
    { "ships.antaran.4.0.appearance", &generated_type_2, &generated_default_1508, generated_targets_1508, 1 },
    { "ships.antaran.4.0.hull_size", &generated_type_3, &generated_default_1509, generated_targets_1509, 1 },
    { "ships.antaran.4.0.drive", &generated_type_4, &generated_default_1510, generated_targets_1510, 1 },
    { "ships.antaran.4.0.armor", &generated_type_5, &generated_default_1511, generated_targets_1511, 1 },
    { "ships.antaran.4.0.shield", &generated_type_6, &generated_default_1512, generated_targets_1512, 1 },
    { "ships.antaran.4.0.computer", &generated_type_7, &generated_default_1513, generated_targets_1513, 1 },
    { "ships.antaran.4.0.weapons.1.type", &generated_type_8, &generated_default_1514, generated_targets_1514, 1 },
    { "ships.antaran.4.0.weapons.1.arc", &generated_type_9, &generated_default_1515, generated_targets_1515, 1 },
    { "ships.antaran.4.0.weapons.1.mods", &generated_type_10, &generated_default_1516, generated_targets_1516, 1 },
    { "ships.antaran.4.0.weapons.1.ammo", &generated_type_2, &generated_default_1517, generated_targets_1517, 1 },
    { "ships.antaran.4.0.weapons.1.amount", &generated_type_11, &generated_default_1518, generated_targets_1518, 2 },
    { "ships.antaran.4.0.weapons.2.type", &generated_type_8, &generated_default_1519, generated_targets_1519, 1 },
    { "ships.antaran.4.0.weapons.2.arc", &generated_type_9, &generated_default_1520, generated_targets_1520, 1 },
    { "ships.antaran.4.0.weapons.2.mods", &generated_type_10, &generated_default_1521, generated_targets_1521, 1 },
    { "ships.antaran.4.0.weapons.2.ammo", &generated_type_2, &generated_default_1522, generated_targets_1522, 1 },
    { "ships.antaran.4.0.weapons.2.amount", &generated_type_11, &generated_default_1523, generated_targets_1523, 2 },
    { "ships.antaran.4.0.weapons.3.type", &generated_type_8, &generated_default_1524, generated_targets_1524, 1 },
    { "ships.antaran.4.0.weapons.3.arc", &generated_type_9, &generated_default_1525, generated_targets_1525, 1 },
    { "ships.antaran.4.0.weapons.3.mods", &generated_type_10, &generated_default_1526, generated_targets_1526, 1 },
    { "ships.antaran.4.0.weapons.3.ammo", &generated_type_2, &generated_default_1527, generated_targets_1527, 1 },
    { "ships.antaran.4.0.weapons.3.amount", &generated_type_11, &generated_default_1528, generated_targets_1528, 2 },
    { "ships.antaran.4.0.weapons.4.type", &generated_type_8, &generated_default_1529, generated_targets_1529, 1 },
    { "ships.antaran.4.0.weapons.4.arc", &generated_type_9, &generated_default_1530, generated_targets_1530, 1 },
    { "ships.antaran.4.0.weapons.4.mods", &generated_type_10, &generated_default_1531, generated_targets_1531, 1 },
    { "ships.antaran.4.0.weapons.4.ammo", &generated_type_2, &generated_default_1532, generated_targets_1532, 1 },
    { "ships.antaran.4.0.weapons.4.amount", &generated_type_11, &generated_default_1533, generated_targets_1533, 2 },
    { "ships.antaran.4.0.weapons.5.type", &generated_type_8, &generated_default_1534, generated_targets_1534, 1 },
    { "ships.antaran.4.0.weapons.5.arc", &generated_type_9, &generated_default_1535, generated_targets_1535, 1 },
    { "ships.antaran.4.0.weapons.5.mods", &generated_type_10, &generated_default_1536, generated_targets_1536, 1 },
    { "ships.antaran.4.0.weapons.5.ammo", &generated_type_2, &generated_default_1537, generated_targets_1537, 1 },
    { "ships.antaran.4.0.weapons.5.amount", &generated_type_11, &generated_default_1538, generated_targets_1538, 2 },
    { "ships.antaran.4.0.weapons.6.type", &generated_type_8, &generated_default_1539, generated_targets_1539, 1 },
    { "ships.antaran.4.0.weapons.6.arc", &generated_type_9, &generated_default_1540, generated_targets_1540, 1 },
    { "ships.antaran.4.0.weapons.6.mods", &generated_type_10, &generated_default_1541, generated_targets_1541, 1 },
    { "ships.antaran.4.0.weapons.6.ammo", &generated_type_2, &generated_default_1542, generated_targets_1542, 1 },
    { "ships.antaran.4.0.weapons.6.amount", &generated_type_11, &generated_default_1543, generated_targets_1543, 2 },
    { "ships.antaran.4.0.weapons.7.type", &generated_type_8, &generated_default_1544, generated_targets_1544, 1 },
    { "ships.antaran.4.0.weapons.7.arc", &generated_type_9, &generated_default_1545, generated_targets_1545, 1 },
    { "ships.antaran.4.0.weapons.7.mods", &generated_type_10, &generated_default_1546, generated_targets_1546, 1 },
    { "ships.antaran.4.0.weapons.7.ammo", &generated_type_2, &generated_default_1547, generated_targets_1547, 1 },
    { "ships.antaran.4.0.weapons.7.amount", &generated_type_11, &generated_default_1548, generated_targets_1548, 2 },
    { "ships.antaran.4.0.weapons.8.type", &generated_type_8, &generated_default_1549, generated_targets_1549, 1 },
    { "ships.antaran.4.0.weapons.8.arc", &generated_type_9, &generated_default_1550, generated_targets_1550, 1 },
    { "ships.antaran.4.0.weapons.8.mods", &generated_type_10, &generated_default_1551, generated_targets_1551, 1 },
    { "ships.antaran.4.0.weapons.8.ammo", &generated_type_2, &generated_default_1552, generated_targets_1552, 1 },
    { "ships.antaran.4.0.weapons.8.amount", &generated_type_11, &generated_default_1553, generated_targets_1553, 2 },
    { "ships.antaran.4.0.specials", &generated_type_12, &generated_default_1554, generated_targets_1554, 1 },
    { "ships.antaran.4.1.appearance", &generated_type_2, &generated_default_1555, generated_targets_1555, 1 },
    { "ships.antaran.4.1.hull_size", &generated_type_3, &generated_default_1556, generated_targets_1556, 1 },
    { "ships.antaran.4.1.drive", &generated_type_4, &generated_default_1557, generated_targets_1557, 1 },
    { "ships.antaran.4.1.armor", &generated_type_5, &generated_default_1558, generated_targets_1558, 1 },
    { "ships.antaran.4.1.shield", &generated_type_6, &generated_default_1559, generated_targets_1559, 1 },
    { "ships.antaran.4.1.computer", &generated_type_7, &generated_default_1560, generated_targets_1560, 1 },
    { "ships.antaran.4.1.weapons.1.type", &generated_type_8, &generated_default_1561, generated_targets_1561, 1 },
    { "ships.antaran.4.1.weapons.1.arc", &generated_type_9, &generated_default_1562, generated_targets_1562, 1 },
    { "ships.antaran.4.1.weapons.1.mods", &generated_type_10, &generated_default_1563, generated_targets_1563, 1 },
    { "ships.antaran.4.1.weapons.1.ammo", &generated_type_2, &generated_default_1564, generated_targets_1564, 1 },
    { "ships.antaran.4.1.weapons.1.amount", &generated_type_11, &generated_default_1565, generated_targets_1565, 2 },
    { "ships.antaran.4.1.weapons.2.type", &generated_type_8, &generated_default_1566, generated_targets_1566, 1 },
    { "ships.antaran.4.1.weapons.2.arc", &generated_type_9, &generated_default_1567, generated_targets_1567, 1 },
    { "ships.antaran.4.1.weapons.2.mods", &generated_type_10, &generated_default_1568, generated_targets_1568, 1 },
    { "ships.antaran.4.1.weapons.2.ammo", &generated_type_2, &generated_default_1569, generated_targets_1569, 1 },
    { "ships.antaran.4.1.weapons.2.amount", &generated_type_11, &generated_default_1570, generated_targets_1570, 2 },
    { "ships.antaran.4.1.weapons.3.type", &generated_type_8, &generated_default_1571, generated_targets_1571, 1 },
    { "ships.antaran.4.1.weapons.3.arc", &generated_type_9, &generated_default_1572, generated_targets_1572, 1 },
    { "ships.antaran.4.1.weapons.3.mods", &generated_type_10, &generated_default_1573, generated_targets_1573, 1 },
    { "ships.antaran.4.1.weapons.3.ammo", &generated_type_2, &generated_default_1574, generated_targets_1574, 1 },
    { "ships.antaran.4.1.weapons.3.amount", &generated_type_11, &generated_default_1575, generated_targets_1575, 2 },
    { "ships.antaran.4.1.weapons.4.type", &generated_type_8, &generated_default_1576, generated_targets_1576, 1 },
    { "ships.antaran.4.1.weapons.4.arc", &generated_type_9, &generated_default_1577, generated_targets_1577, 1 },
    { "ships.antaran.4.1.weapons.4.mods", &generated_type_10, &generated_default_1578, generated_targets_1578, 1 },
    { "ships.antaran.4.1.weapons.4.ammo", &generated_type_2, &generated_default_1579, generated_targets_1579, 1 },
    { "ships.antaran.4.1.weapons.4.amount", &generated_type_11, &generated_default_1580, generated_targets_1580, 2 },
    { "ships.antaran.4.1.weapons.5.type", &generated_type_8, &generated_default_1581, generated_targets_1581, 1 },
    { "ships.antaran.4.1.weapons.5.arc", &generated_type_9, &generated_default_1582, generated_targets_1582, 1 },
    { "ships.antaran.4.1.weapons.5.mods", &generated_type_10, &generated_default_1583, generated_targets_1583, 1 },
    { "ships.antaran.4.1.weapons.5.ammo", &generated_type_2, &generated_default_1584, generated_targets_1584, 1 },
    { "ships.antaran.4.1.weapons.5.amount", &generated_type_11, &generated_default_1585, generated_targets_1585, 2 },
    { "ships.antaran.4.1.weapons.6.type", &generated_type_8, &generated_default_1586, generated_targets_1586, 1 },
    { "ships.antaran.4.1.weapons.6.arc", &generated_type_9, &generated_default_1587, generated_targets_1587, 1 },
    { "ships.antaran.4.1.weapons.6.mods", &generated_type_10, &generated_default_1588, generated_targets_1588, 1 },
    { "ships.antaran.4.1.weapons.6.ammo", &generated_type_2, &generated_default_1589, generated_targets_1589, 1 },
    { "ships.antaran.4.1.weapons.6.amount", &generated_type_11, &generated_default_1590, generated_targets_1590, 2 },
    { "ships.antaran.4.1.weapons.7.type", &generated_type_8, &generated_default_1591, generated_targets_1591, 1 },
    { "ships.antaran.4.1.weapons.7.arc", &generated_type_9, &generated_default_1592, generated_targets_1592, 1 },
    { "ships.antaran.4.1.weapons.7.mods", &generated_type_10, &generated_default_1593, generated_targets_1593, 1 },
    { "ships.antaran.4.1.weapons.7.ammo", &generated_type_2, &generated_default_1594, generated_targets_1594, 1 },
    { "ships.antaran.4.1.weapons.7.amount", &generated_type_11, &generated_default_1595, generated_targets_1595, 2 },
    { "ships.antaran.4.1.weapons.8.type", &generated_type_8, &generated_default_1596, generated_targets_1596, 1 },
    { "ships.antaran.4.1.weapons.8.arc", &generated_type_9, &generated_default_1597, generated_targets_1597, 1 },
    { "ships.antaran.4.1.weapons.8.mods", &generated_type_10, &generated_default_1598, generated_targets_1598, 1 },
    { "ships.antaran.4.1.weapons.8.ammo", &generated_type_2, &generated_default_1599, generated_targets_1599, 1 },
    { "ships.antaran.4.1.weapons.8.amount", &generated_type_11, &generated_default_1600, generated_targets_1600, 2 },
    { "ships.antaran.4.1.specials", &generated_type_12, &generated_default_1601, generated_targets_1601, 1 },
    { "ships.antaran.4.2.appearance", &generated_type_2, &generated_default_1602, generated_targets_1602, 1 },
    { "ships.antaran.4.2.hull_size", &generated_type_3, &generated_default_1603, generated_targets_1603, 1 },
    { "ships.antaran.4.2.drive", &generated_type_4, &generated_default_1604, generated_targets_1604, 1 },
    { "ships.antaran.4.2.armor", &generated_type_5, &generated_default_1605, generated_targets_1605, 1 },
    { "ships.antaran.4.2.shield", &generated_type_6, &generated_default_1606, generated_targets_1606, 1 },
    { "ships.antaran.4.2.computer", &generated_type_7, &generated_default_1607, generated_targets_1607, 1 },
    { "ships.antaran.4.2.weapons.1.type", &generated_type_8, &generated_default_1608, generated_targets_1608, 1 },
    { "ships.antaran.4.2.weapons.1.arc", &generated_type_9, &generated_default_1609, generated_targets_1609, 1 },
    { "ships.antaran.4.2.weapons.1.mods", &generated_type_10, &generated_default_1610, generated_targets_1610, 1 },
    { "ships.antaran.4.2.weapons.1.ammo", &generated_type_2, &generated_default_1611, generated_targets_1611, 1 },
    { "ships.antaran.4.2.weapons.1.amount", &generated_type_11, &generated_default_1612, generated_targets_1612, 2 },
    { "ships.antaran.4.2.weapons.2.type", &generated_type_8, &generated_default_1613, generated_targets_1613, 1 },
    { "ships.antaran.4.2.weapons.2.arc", &generated_type_9, &generated_default_1614, generated_targets_1614, 1 },
    { "ships.antaran.4.2.weapons.2.mods", &generated_type_10, &generated_default_1615, generated_targets_1615, 1 },
    { "ships.antaran.4.2.weapons.2.ammo", &generated_type_2, &generated_default_1616, generated_targets_1616, 1 },
    { "ships.antaran.4.2.weapons.2.amount", &generated_type_11, &generated_default_1617, generated_targets_1617, 2 },
    { "ships.antaran.4.2.weapons.3.type", &generated_type_8, &generated_default_1618, generated_targets_1618, 1 },
    { "ships.antaran.4.2.weapons.3.arc", &generated_type_9, &generated_default_1619, generated_targets_1619, 1 },
    { "ships.antaran.4.2.weapons.3.mods", &generated_type_10, &generated_default_1620, generated_targets_1620, 1 },
    { "ships.antaran.4.2.weapons.3.ammo", &generated_type_2, &generated_default_1621, generated_targets_1621, 1 },
    { "ships.antaran.4.2.weapons.3.amount", &generated_type_11, &generated_default_1622, generated_targets_1622, 2 },
    { "ships.antaran.4.2.weapons.4.type", &generated_type_8, &generated_default_1623, generated_targets_1623, 1 },
    { "ships.antaran.4.2.weapons.4.arc", &generated_type_9, &generated_default_1624, generated_targets_1624, 1 },
    { "ships.antaran.4.2.weapons.4.mods", &generated_type_10, &generated_default_1625, generated_targets_1625, 1 },
    { "ships.antaran.4.2.weapons.4.ammo", &generated_type_2, &generated_default_1626, generated_targets_1626, 1 },
    { "ships.antaran.4.2.weapons.4.amount", &generated_type_11, &generated_default_1627, generated_targets_1627, 2 },
    { "ships.antaran.4.2.weapons.5.type", &generated_type_8, &generated_default_1628, generated_targets_1628, 1 },
    { "ships.antaran.4.2.weapons.5.arc", &generated_type_9, &generated_default_1629, generated_targets_1629, 1 },
    { "ships.antaran.4.2.weapons.5.mods", &generated_type_10, &generated_default_1630, generated_targets_1630, 1 },
    { "ships.antaran.4.2.weapons.5.ammo", &generated_type_2, &generated_default_1631, generated_targets_1631, 1 },
    { "ships.antaran.4.2.weapons.5.amount", &generated_type_11, &generated_default_1632, generated_targets_1632, 2 },
    { "ships.antaran.4.2.weapons.6.type", &generated_type_8, &generated_default_1633, generated_targets_1633, 1 },
    { "ships.antaran.4.2.weapons.6.arc", &generated_type_9, &generated_default_1634, generated_targets_1634, 1 },
    { "ships.antaran.4.2.weapons.6.mods", &generated_type_10, &generated_default_1635, generated_targets_1635, 1 },
    { "ships.antaran.4.2.weapons.6.ammo", &generated_type_2, &generated_default_1636, generated_targets_1636, 1 },
    { "ships.antaran.4.2.weapons.6.amount", &generated_type_11, &generated_default_1637, generated_targets_1637, 2 },
    { "ships.antaran.4.2.weapons.7.type", &generated_type_8, &generated_default_1638, generated_targets_1638, 1 },
    { "ships.antaran.4.2.weapons.7.arc", &generated_type_9, &generated_default_1639, generated_targets_1639, 1 },
    { "ships.antaran.4.2.weapons.7.mods", &generated_type_10, &generated_default_1640, generated_targets_1640, 1 },
    { "ships.antaran.4.2.weapons.7.ammo", &generated_type_2, &generated_default_1641, generated_targets_1641, 1 },
    { "ships.antaran.4.2.weapons.7.amount", &generated_type_11, &generated_default_1642, generated_targets_1642, 2 },
    { "ships.antaran.4.2.weapons.8.type", &generated_type_8, &generated_default_1643, generated_targets_1643, 1 },
    { "ships.antaran.4.2.weapons.8.arc", &generated_type_9, &generated_default_1644, generated_targets_1644, 1 },
    { "ships.antaran.4.2.weapons.8.mods", &generated_type_10, &generated_default_1645, generated_targets_1645, 1 },
    { "ships.antaran.4.2.weapons.8.ammo", &generated_type_2, &generated_default_1646, generated_targets_1646, 1 },
    { "ships.antaran.4.2.weapons.8.amount", &generated_type_11, &generated_default_1647, generated_targets_1647, 2 },
    { "ships.antaran.4.2.specials", &generated_type_12, &generated_default_1648, generated_targets_1648, 1 },
    { "ships.antaran.4.3.appearance", &generated_type_2, &generated_default_1649, generated_targets_1649, 1 },
    { "ships.antaran.4.3.hull_size", &generated_type_3, &generated_default_1650, generated_targets_1650, 1 },
    { "ships.antaran.4.3.drive", &generated_type_4, &generated_default_1651, generated_targets_1651, 1 },
    { "ships.antaran.4.3.armor", &generated_type_5, &generated_default_1652, generated_targets_1652, 1 },
    { "ships.antaran.4.3.shield", &generated_type_6, &generated_default_1653, generated_targets_1653, 1 },
    { "ships.antaran.4.3.computer", &generated_type_7, &generated_default_1654, generated_targets_1654, 1 },
    { "ships.antaran.4.3.weapons.1.type", &generated_type_8, &generated_default_1655, generated_targets_1655, 1 },
    { "ships.antaran.4.3.weapons.1.arc", &generated_type_9, &generated_default_1656, generated_targets_1656, 1 },
    { "ships.antaran.4.3.weapons.1.mods", &generated_type_10, &generated_default_1657, generated_targets_1657, 1 },
    { "ships.antaran.4.3.weapons.1.ammo", &generated_type_2, &generated_default_1658, generated_targets_1658, 1 },
    { "ships.antaran.4.3.weapons.1.amount", &generated_type_11, &generated_default_1659, generated_targets_1659, 2 },
    { "ships.antaran.4.3.weapons.2.type", &generated_type_8, &generated_default_1660, generated_targets_1660, 1 },
    { "ships.antaran.4.3.weapons.2.arc", &generated_type_9, &generated_default_1661, generated_targets_1661, 1 },
    { "ships.antaran.4.3.weapons.2.mods", &generated_type_10, &generated_default_1662, generated_targets_1662, 1 },
    { "ships.antaran.4.3.weapons.2.ammo", &generated_type_2, &generated_default_1663, generated_targets_1663, 1 },
    { "ships.antaran.4.3.weapons.2.amount", &generated_type_11, &generated_default_1664, generated_targets_1664, 2 },
    { "ships.antaran.4.3.weapons.3.type", &generated_type_8, &generated_default_1665, generated_targets_1665, 1 },
    { "ships.antaran.4.3.weapons.3.arc", &generated_type_9, &generated_default_1666, generated_targets_1666, 1 },
    { "ships.antaran.4.3.weapons.3.mods", &generated_type_10, &generated_default_1667, generated_targets_1667, 1 },
    { "ships.antaran.4.3.weapons.3.ammo", &generated_type_2, &generated_default_1668, generated_targets_1668, 1 },
    { "ships.antaran.4.3.weapons.3.amount", &generated_type_11, &generated_default_1669, generated_targets_1669, 2 },
    { "ships.antaran.4.3.weapons.4.type", &generated_type_8, &generated_default_1670, generated_targets_1670, 1 },
    { "ships.antaran.4.3.weapons.4.arc", &generated_type_9, &generated_default_1671, generated_targets_1671, 1 },
    { "ships.antaran.4.3.weapons.4.mods", &generated_type_10, &generated_default_1672, generated_targets_1672, 1 },
    { "ships.antaran.4.3.weapons.4.ammo", &generated_type_2, &generated_default_1673, generated_targets_1673, 1 },
    { "ships.antaran.4.3.weapons.4.amount", &generated_type_11, &generated_default_1674, generated_targets_1674, 2 },
    { "ships.antaran.4.3.weapons.5.type", &generated_type_8, &generated_default_1675, generated_targets_1675, 1 },
    { "ships.antaran.4.3.weapons.5.arc", &generated_type_9, &generated_default_1676, generated_targets_1676, 1 },
    { "ships.antaran.4.3.weapons.5.mods", &generated_type_10, &generated_default_1677, generated_targets_1677, 1 },
    { "ships.antaran.4.3.weapons.5.ammo", &generated_type_2, &generated_default_1678, generated_targets_1678, 1 },
    { "ships.antaran.4.3.weapons.5.amount", &generated_type_11, &generated_default_1679, generated_targets_1679, 2 },
    { "ships.antaran.4.3.weapons.6.type", &generated_type_8, &generated_default_1680, generated_targets_1680, 1 },
    { "ships.antaran.4.3.weapons.6.arc", &generated_type_9, &generated_default_1681, generated_targets_1681, 1 },
    { "ships.antaran.4.3.weapons.6.mods", &generated_type_10, &generated_default_1682, generated_targets_1682, 1 },
    { "ships.antaran.4.3.weapons.6.ammo", &generated_type_2, &generated_default_1683, generated_targets_1683, 1 },
    { "ships.antaran.4.3.weapons.6.amount", &generated_type_11, &generated_default_1684, generated_targets_1684, 2 },
    { "ships.antaran.4.3.weapons.7.type", &generated_type_8, &generated_default_1685, generated_targets_1685, 1 },
    { "ships.antaran.4.3.weapons.7.arc", &generated_type_9, &generated_default_1686, generated_targets_1686, 1 },
    { "ships.antaran.4.3.weapons.7.mods", &generated_type_10, &generated_default_1687, generated_targets_1687, 1 },
    { "ships.antaran.4.3.weapons.7.ammo", &generated_type_2, &generated_default_1688, generated_targets_1688, 1 },
    { "ships.antaran.4.3.weapons.7.amount", &generated_type_11, &generated_default_1689, generated_targets_1689, 2 },
    { "ships.antaran.4.3.weapons.8.type", &generated_type_8, &generated_default_1690, generated_targets_1690, 1 },
    { "ships.antaran.4.3.weapons.8.arc", &generated_type_9, &generated_default_1691, generated_targets_1691, 1 },
    { "ships.antaran.4.3.weapons.8.mods", &generated_type_10, &generated_default_1692, generated_targets_1692, 1 },
    { "ships.antaran.4.3.weapons.8.ammo", &generated_type_2, &generated_default_1693, generated_targets_1693, 1 },
    { "ships.antaran.4.3.weapons.8.amount", &generated_type_11, &generated_default_1694, generated_targets_1694, 2 },
    { "ships.antaran.4.3.specials", &generated_type_12, &generated_default_1695, generated_targets_1695, 1 },
    { "ships.antaran.4.4.appearance", &generated_type_2, &generated_default_1696, generated_targets_1696, 1 },
    { "ships.antaran.4.4.hull_size", &generated_type_3, &generated_default_1697, generated_targets_1697, 1 },
    { "ships.antaran.4.4.drive", &generated_type_4, &generated_default_1698, generated_targets_1698, 1 },
    { "ships.antaran.4.4.armor", &generated_type_5, &generated_default_1699, generated_targets_1699, 1 },
    { "ships.antaran.4.4.shield", &generated_type_6, &generated_default_1700, generated_targets_1700, 1 },
    { "ships.antaran.4.4.computer", &generated_type_7, &generated_default_1701, generated_targets_1701, 1 },
    { "ships.antaran.4.4.weapons.1.type", &generated_type_8, &generated_default_1702, generated_targets_1702, 1 },
    { "ships.antaran.4.4.weapons.1.arc", &generated_type_9, &generated_default_1703, generated_targets_1703, 1 },
    { "ships.antaran.4.4.weapons.1.mods", &generated_type_10, &generated_default_1704, generated_targets_1704, 1 },
    { "ships.antaran.4.4.weapons.1.ammo", &generated_type_2, &generated_default_1705, generated_targets_1705, 1 },
    { "ships.antaran.4.4.weapons.1.amount", &generated_type_11, &generated_default_1706, generated_targets_1706, 2 },
    { "ships.antaran.4.4.weapons.2.type", &generated_type_8, &generated_default_1707, generated_targets_1707, 1 },
    { "ships.antaran.4.4.weapons.2.arc", &generated_type_9, &generated_default_1708, generated_targets_1708, 1 },
    { "ships.antaran.4.4.weapons.2.mods", &generated_type_10, &generated_default_1709, generated_targets_1709, 1 },
    { "ships.antaran.4.4.weapons.2.ammo", &generated_type_2, &generated_default_1710, generated_targets_1710, 1 },
    { "ships.antaran.4.4.weapons.2.amount", &generated_type_11, &generated_default_1711, generated_targets_1711, 2 },
    { "ships.antaran.4.4.weapons.3.type", &generated_type_8, &generated_default_1712, generated_targets_1712, 1 },
    { "ships.antaran.4.4.weapons.3.arc", &generated_type_9, &generated_default_1713, generated_targets_1713, 1 },
    { "ships.antaran.4.4.weapons.3.mods", &generated_type_10, &generated_default_1714, generated_targets_1714, 1 },
    { "ships.antaran.4.4.weapons.3.ammo", &generated_type_2, &generated_default_1715, generated_targets_1715, 1 },
    { "ships.antaran.4.4.weapons.3.amount", &generated_type_11, &generated_default_1716, generated_targets_1716, 2 },
    { "ships.antaran.4.4.weapons.4.type", &generated_type_8, &generated_default_1717, generated_targets_1717, 1 },
    { "ships.antaran.4.4.weapons.4.arc", &generated_type_9, &generated_default_1718, generated_targets_1718, 1 },
    { "ships.antaran.4.4.weapons.4.mods", &generated_type_10, &generated_default_1719, generated_targets_1719, 1 },
    { "ships.antaran.4.4.weapons.4.ammo", &generated_type_2, &generated_default_1720, generated_targets_1720, 1 },
    { "ships.antaran.4.4.weapons.4.amount", &generated_type_11, &generated_default_1721, generated_targets_1721, 2 },
    { "ships.antaran.4.4.weapons.5.type", &generated_type_8, &generated_default_1722, generated_targets_1722, 1 },
    { "ships.antaran.4.4.weapons.5.arc", &generated_type_9, &generated_default_1723, generated_targets_1723, 1 },
    { "ships.antaran.4.4.weapons.5.mods", &generated_type_10, &generated_default_1724, generated_targets_1724, 1 },
    { "ships.antaran.4.4.weapons.5.ammo", &generated_type_2, &generated_default_1725, generated_targets_1725, 1 },
    { "ships.antaran.4.4.weapons.5.amount", &generated_type_11, &generated_default_1726, generated_targets_1726, 2 },
    { "ships.antaran.4.4.weapons.6.type", &generated_type_8, &generated_default_1727, generated_targets_1727, 1 },
    { "ships.antaran.4.4.weapons.6.arc", &generated_type_9, &generated_default_1728, generated_targets_1728, 1 },
    { "ships.antaran.4.4.weapons.6.mods", &generated_type_10, &generated_default_1729, generated_targets_1729, 1 },
    { "ships.antaran.4.4.weapons.6.ammo", &generated_type_2, &generated_default_1730, generated_targets_1730, 1 },
    { "ships.antaran.4.4.weapons.6.amount", &generated_type_11, &generated_default_1731, generated_targets_1731, 2 },
    { "ships.antaran.4.4.weapons.7.type", &generated_type_8, &generated_default_1732, generated_targets_1732, 1 },
    { "ships.antaran.4.4.weapons.7.arc", &generated_type_9, &generated_default_1733, generated_targets_1733, 1 },
    { "ships.antaran.4.4.weapons.7.mods", &generated_type_10, &generated_default_1734, generated_targets_1734, 1 },
    { "ships.antaran.4.4.weapons.7.ammo", &generated_type_2, &generated_default_1735, generated_targets_1735, 1 },
    { "ships.antaran.4.4.weapons.7.amount", &generated_type_11, &generated_default_1736, generated_targets_1736, 2 },
    { "ships.antaran.4.4.weapons.8.type", &generated_type_8, &generated_default_1737, generated_targets_1737, 1 },
    { "ships.antaran.4.4.weapons.8.arc", &generated_type_9, &generated_default_1738, generated_targets_1738, 1 },
    { "ships.antaran.4.4.weapons.8.mods", &generated_type_10, &generated_default_1739, generated_targets_1739, 1 },
    { "ships.antaran.4.4.weapons.8.ammo", &generated_type_2, &generated_default_1740, generated_targets_1740, 1 },
    { "ships.antaran.4.4.weapons.8.amount", &generated_type_11, &generated_default_1741, generated_targets_1741, 2 },
    { "ships.antaran.4.4.specials", &generated_type_12, &generated_default_1742, generated_targets_1742, 1 },
};

const size_t variable_count = sizeof(variables) / sizeof(variables[0]);

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
    { 119, "system", 0 },
    { 119, "event", 1 },
    { 234, "system", 0 },
    { 234, "event", 1 },
    { 349, "system", 0 },
    { 349, "event", 1 },
    { 464, "system", 0 },
    { 464, "event", 1 },
    { 579, "system", 0 },
    { 579, "event", 1 },
    { 694, "tutor", 0 },
    { 694, "easy", 1 },
    { 694, "average", 2 },
    { 694, "hard", 3 },
    { 694, "impossible", 4 },
    { 695, "frigate", 0 },
    { 695, "destroyer", 1 },
    { 695, "cruiser", 2 },
    { 695, "battleship", 3 },
    { 695, "titan", 4 },
    { 981, "frigate", 0 },
    { 981, "destroyer", 1 },
    { 981, "cruiser", 2 },
    { 981, "battleship", 3 },
    { 981, "titan", 4 },
    { 1267, "frigate", 0 },
    { 1267, "destroyer", 1 },
    { 1267, "cruiser", 2 },
    { 1267, "battleship", 3 },
    { 1267, "titan", 4 },
    { 1553, "frigate", 0 },
    { 1553, "destroyer", 1 },
    { 1553, "cruiser", 2 },
    { 1553, "battleship", 3 },
    { 1553, "titan", 4 },
    { 1839, "frigate", 0 },
    { 1839, "destroyer", 1 },
    { 1839, "cruiser", 2 },
    { 1839, "battleship", 3 },
    { 1839, "titan", 4 },
};

const s_array_index_alias_descriptor* const array_index_aliases = generated_array_index_aliases;

const size_t array_index_alias_count = 40;

const s_schema_node_descriptor schema_nodes[] = {
    { "music_enabled", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, 0, 0, 0 },
    { "music_volume", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, 1, 1, 1 },
    { "sound_effects_enabled", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, 2, 2, 2 },
    { "sound_effects_volume", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, 3, 3, 3 },
    { "ships", CONFIG_NODE_NAMESPACE, CONFIG_NO_NODE, 0, 4, 1742, CONFIG_NO_VARIABLE },
    { "guardian", CONFIG_NODE_RECORD, 4, 0, 4, 50, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 5, 0, 4, 4, 4 },
    { "hull_size", CONFIG_NODE_SCALAR, 5, 0, 5, 5, 5 },
    { "drive", CONFIG_NODE_SCALAR, 5, 0, 6, 6, 6 },
    { "armor", CONFIG_NODE_SCALAR, 5, 0, 7, 7, 7 },
    { "shield", CONFIG_NODE_SCALAR, 5, 0, 8, 8, 8 },
    { "computer", CONFIG_NODE_SCALAR, 5, 0, 9, 9, 9 },
    { "weapons", CONFIG_NODE_ARRAY, 5, 0, 10, 49, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 12, 1, 10, 14, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 13, 0, 10, 10, 10 },
    { "arc", CONFIG_NODE_SCALAR, 13, 0, 11, 11, 11 },
    { "mods", CONFIG_NODE_SCALAR, 13, 0, 12, 12, 12 },
    { "ammo", CONFIG_NODE_SCALAR, 13, 0, 13, 13, 13 },
    { "amount", CONFIG_NODE_SCALAR, 13, 0, 14, 14, 14 },
    { "", CONFIG_NODE_RECORD, 12, 2, 15, 19, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 19, 0, 15, 15, 15 },
    { "arc", CONFIG_NODE_SCALAR, 19, 0, 16, 16, 16 },
    { "mods", CONFIG_NODE_SCALAR, 19, 0, 17, 17, 17 },
    { "ammo", CONFIG_NODE_SCALAR, 19, 0, 18, 18, 18 },
    { "amount", CONFIG_NODE_SCALAR, 19, 0, 19, 19, 19 },
    { "", CONFIG_NODE_RECORD, 12, 3, 20, 24, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 25, 0, 20, 20, 20 },
    { "arc", CONFIG_NODE_SCALAR, 25, 0, 21, 21, 21 },
    { "mods", CONFIG_NODE_SCALAR, 25, 0, 22, 22, 22 },
    { "ammo", CONFIG_NODE_SCALAR, 25, 0, 23, 23, 23 },
    { "amount", CONFIG_NODE_SCALAR, 25, 0, 24, 24, 24 },
    { "", CONFIG_NODE_RECORD, 12, 4, 25, 29, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 31, 0, 25, 25, 25 },
    { "arc", CONFIG_NODE_SCALAR, 31, 0, 26, 26, 26 },
    { "mods", CONFIG_NODE_SCALAR, 31, 0, 27, 27, 27 },
    { "ammo", CONFIG_NODE_SCALAR, 31, 0, 28, 28, 28 },
    { "amount", CONFIG_NODE_SCALAR, 31, 0, 29, 29, 29 },
    { "", CONFIG_NODE_RECORD, 12, 5, 30, 34, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 37, 0, 30, 30, 30 },
    { "arc", CONFIG_NODE_SCALAR, 37, 0, 31, 31, 31 },
    { "mods", CONFIG_NODE_SCALAR, 37, 0, 32, 32, 32 },
    { "ammo", CONFIG_NODE_SCALAR, 37, 0, 33, 33, 33 },
    { "amount", CONFIG_NODE_SCALAR, 37, 0, 34, 34, 34 },
    { "", CONFIG_NODE_RECORD, 12, 6, 35, 39, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 43, 0, 35, 35, 35 },
    { "arc", CONFIG_NODE_SCALAR, 43, 0, 36, 36, 36 },
    { "mods", CONFIG_NODE_SCALAR, 43, 0, 37, 37, 37 },
    { "ammo", CONFIG_NODE_SCALAR, 43, 0, 38, 38, 38 },
    { "amount", CONFIG_NODE_SCALAR, 43, 0, 39, 39, 39 },
    { "", CONFIG_NODE_RECORD, 12, 7, 40, 44, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 49, 0, 40, 40, 40 },
    { "arc", CONFIG_NODE_SCALAR, 49, 0, 41, 41, 41 },
    { "mods", CONFIG_NODE_SCALAR, 49, 0, 42, 42, 42 },
    { "ammo", CONFIG_NODE_SCALAR, 49, 0, 43, 43, 43 },
    { "amount", CONFIG_NODE_SCALAR, 49, 0, 44, 44, 44 },
    { "", CONFIG_NODE_RECORD, 12, 8, 45, 49, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 55, 0, 45, 45, 45 },
    { "arc", CONFIG_NODE_SCALAR, 55, 0, 46, 46, 46 },
    { "mods", CONFIG_NODE_SCALAR, 55, 0, 47, 47, 47 },
    { "ammo", CONFIG_NODE_SCALAR, 55, 0, 48, 48, 48 },
    { "amount", CONFIG_NODE_SCALAR, 55, 0, 49, 49, 49 },
    { "specials", CONFIG_NODE_SCALAR, 5, 0, 50, 50, 50 },
    { "avenger", CONFIG_NODE_RECORD, 4, 0, 51, 97, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 62, 0, 51, 51, 51 },
    { "hull_size", CONFIG_NODE_SCALAR, 62, 0, 52, 52, 52 },
    { "drive", CONFIG_NODE_SCALAR, 62, 0, 53, 53, 53 },
    { "armor", CONFIG_NODE_SCALAR, 62, 0, 54, 54, 54 },
    { "shield", CONFIG_NODE_SCALAR, 62, 0, 55, 55, 55 },
    { "computer", CONFIG_NODE_SCALAR, 62, 0, 56, 56, 56 },
    { "weapons", CONFIG_NODE_ARRAY, 62, 0, 57, 96, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 69, 1, 57, 61, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 70, 0, 57, 57, 57 },
    { "arc", CONFIG_NODE_SCALAR, 70, 0, 58, 58, 58 },
    { "mods", CONFIG_NODE_SCALAR, 70, 0, 59, 59, 59 },
    { "ammo", CONFIG_NODE_SCALAR, 70, 0, 60, 60, 60 },
    { "amount", CONFIG_NODE_SCALAR, 70, 0, 61, 61, 61 },
    { "", CONFIG_NODE_RECORD, 69, 2, 62, 66, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 76, 0, 62, 62, 62 },
    { "arc", CONFIG_NODE_SCALAR, 76, 0, 63, 63, 63 },
    { "mods", CONFIG_NODE_SCALAR, 76, 0, 64, 64, 64 },
    { "ammo", CONFIG_NODE_SCALAR, 76, 0, 65, 65, 65 },
    { "amount", CONFIG_NODE_SCALAR, 76, 0, 66, 66, 66 },
    { "", CONFIG_NODE_RECORD, 69, 3, 67, 71, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 82, 0, 67, 67, 67 },
    { "arc", CONFIG_NODE_SCALAR, 82, 0, 68, 68, 68 },
    { "mods", CONFIG_NODE_SCALAR, 82, 0, 69, 69, 69 },
    { "ammo", CONFIG_NODE_SCALAR, 82, 0, 70, 70, 70 },
    { "amount", CONFIG_NODE_SCALAR, 82, 0, 71, 71, 71 },
    { "", CONFIG_NODE_RECORD, 69, 4, 72, 76, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 88, 0, 72, 72, 72 },
    { "arc", CONFIG_NODE_SCALAR, 88, 0, 73, 73, 73 },
    { "mods", CONFIG_NODE_SCALAR, 88, 0, 74, 74, 74 },
    { "ammo", CONFIG_NODE_SCALAR, 88, 0, 75, 75, 75 },
    { "amount", CONFIG_NODE_SCALAR, 88, 0, 76, 76, 76 },
    { "", CONFIG_NODE_RECORD, 69, 5, 77, 81, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 94, 0, 77, 77, 77 },
    { "arc", CONFIG_NODE_SCALAR, 94, 0, 78, 78, 78 },
    { "mods", CONFIG_NODE_SCALAR, 94, 0, 79, 79, 79 },
    { "ammo", CONFIG_NODE_SCALAR, 94, 0, 80, 80, 80 },
    { "amount", CONFIG_NODE_SCALAR, 94, 0, 81, 81, 81 },
    { "", CONFIG_NODE_RECORD, 69, 6, 82, 86, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 100, 0, 82, 82, 82 },
    { "arc", CONFIG_NODE_SCALAR, 100, 0, 83, 83, 83 },
    { "mods", CONFIG_NODE_SCALAR, 100, 0, 84, 84, 84 },
    { "ammo", CONFIG_NODE_SCALAR, 100, 0, 85, 85, 85 },
    { "amount", CONFIG_NODE_SCALAR, 100, 0, 86, 86, 86 },
    { "", CONFIG_NODE_RECORD, 69, 7, 87, 91, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 106, 0, 87, 87, 87 },
    { "arc", CONFIG_NODE_SCALAR, 106, 0, 88, 88, 88 },
    { "mods", CONFIG_NODE_SCALAR, 106, 0, 89, 89, 89 },
    { "ammo", CONFIG_NODE_SCALAR, 106, 0, 90, 90, 90 },
    { "amount", CONFIG_NODE_SCALAR, 106, 0, 91, 91, 91 },
    { "", CONFIG_NODE_RECORD, 69, 8, 92, 96, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 112, 0, 92, 92, 92 },
    { "arc", CONFIG_NODE_SCALAR, 112, 0, 93, 93, 93 },
    { "mods", CONFIG_NODE_SCALAR, 112, 0, 94, 94, 94 },
    { "ammo", CONFIG_NODE_SCALAR, 112, 0, 95, 95, 95 },
    { "amount", CONFIG_NODE_SCALAR, 112, 0, 96, 96, 96 },
    { "specials", CONFIG_NODE_SCALAR, 62, 0, 97, 97, 97 },
    { "amoeba", CONFIG_NODE_ARRAY, 4, 0, 98, 191, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 119, 0, 98, 144, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 120, 0, 98, 98, 98 },
    { "hull_size", CONFIG_NODE_SCALAR, 120, 0, 99, 99, 99 },
    { "drive", CONFIG_NODE_SCALAR, 120, 0, 100, 100, 100 },
    { "armor", CONFIG_NODE_SCALAR, 120, 0, 101, 101, 101 },
    { "shield", CONFIG_NODE_SCALAR, 120, 0, 102, 102, 102 },
    { "computer", CONFIG_NODE_SCALAR, 120, 0, 103, 103, 103 },
    { "weapons", CONFIG_NODE_ARRAY, 120, 0, 104, 143, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 127, 1, 104, 108, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 128, 0, 104, 104, 104 },
    { "arc", CONFIG_NODE_SCALAR, 128, 0, 105, 105, 105 },
    { "mods", CONFIG_NODE_SCALAR, 128, 0, 106, 106, 106 },
    { "ammo", CONFIG_NODE_SCALAR, 128, 0, 107, 107, 107 },
    { "amount", CONFIG_NODE_SCALAR, 128, 0, 108, 108, 108 },
    { "", CONFIG_NODE_RECORD, 127, 2, 109, 113, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 134, 0, 109, 109, 109 },
    { "arc", CONFIG_NODE_SCALAR, 134, 0, 110, 110, 110 },
    { "mods", CONFIG_NODE_SCALAR, 134, 0, 111, 111, 111 },
    { "ammo", CONFIG_NODE_SCALAR, 134, 0, 112, 112, 112 },
    { "amount", CONFIG_NODE_SCALAR, 134, 0, 113, 113, 113 },
    { "", CONFIG_NODE_RECORD, 127, 3, 114, 118, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 140, 0, 114, 114, 114 },
    { "arc", CONFIG_NODE_SCALAR, 140, 0, 115, 115, 115 },
    { "mods", CONFIG_NODE_SCALAR, 140, 0, 116, 116, 116 },
    { "ammo", CONFIG_NODE_SCALAR, 140, 0, 117, 117, 117 },
    { "amount", CONFIG_NODE_SCALAR, 140, 0, 118, 118, 118 },
    { "", CONFIG_NODE_RECORD, 127, 4, 119, 123, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 146, 0, 119, 119, 119 },
    { "arc", CONFIG_NODE_SCALAR, 146, 0, 120, 120, 120 },
    { "mods", CONFIG_NODE_SCALAR, 146, 0, 121, 121, 121 },
    { "ammo", CONFIG_NODE_SCALAR, 146, 0, 122, 122, 122 },
    { "amount", CONFIG_NODE_SCALAR, 146, 0, 123, 123, 123 },
    { "", CONFIG_NODE_RECORD, 127, 5, 124, 128, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 152, 0, 124, 124, 124 },
    { "arc", CONFIG_NODE_SCALAR, 152, 0, 125, 125, 125 },
    { "mods", CONFIG_NODE_SCALAR, 152, 0, 126, 126, 126 },
    { "ammo", CONFIG_NODE_SCALAR, 152, 0, 127, 127, 127 },
    { "amount", CONFIG_NODE_SCALAR, 152, 0, 128, 128, 128 },
    { "", CONFIG_NODE_RECORD, 127, 6, 129, 133, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 158, 0, 129, 129, 129 },
    { "arc", CONFIG_NODE_SCALAR, 158, 0, 130, 130, 130 },
    { "mods", CONFIG_NODE_SCALAR, 158, 0, 131, 131, 131 },
    { "ammo", CONFIG_NODE_SCALAR, 158, 0, 132, 132, 132 },
    { "amount", CONFIG_NODE_SCALAR, 158, 0, 133, 133, 133 },
    { "", CONFIG_NODE_RECORD, 127, 7, 134, 138, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 164, 0, 134, 134, 134 },
    { "arc", CONFIG_NODE_SCALAR, 164, 0, 135, 135, 135 },
    { "mods", CONFIG_NODE_SCALAR, 164, 0, 136, 136, 136 },
    { "ammo", CONFIG_NODE_SCALAR, 164, 0, 137, 137, 137 },
    { "amount", CONFIG_NODE_SCALAR, 164, 0, 138, 138, 138 },
    { "", CONFIG_NODE_RECORD, 127, 8, 139, 143, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 170, 0, 139, 139, 139 },
    { "arc", CONFIG_NODE_SCALAR, 170, 0, 140, 140, 140 },
    { "mods", CONFIG_NODE_SCALAR, 170, 0, 141, 141, 141 },
    { "ammo", CONFIG_NODE_SCALAR, 170, 0, 142, 142, 142 },
    { "amount", CONFIG_NODE_SCALAR, 170, 0, 143, 143, 143 },
    { "specials", CONFIG_NODE_SCALAR, 120, 0, 144, 144, 144 },
    { "", CONFIG_NODE_RECORD, 119, 1, 145, 191, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 177, 0, 145, 145, 145 },
    { "hull_size", CONFIG_NODE_SCALAR, 177, 0, 146, 146, 146 },
    { "drive", CONFIG_NODE_SCALAR, 177, 0, 147, 147, 147 },
    { "armor", CONFIG_NODE_SCALAR, 177, 0, 148, 148, 148 },
    { "shield", CONFIG_NODE_SCALAR, 177, 0, 149, 149, 149 },
    { "computer", CONFIG_NODE_SCALAR, 177, 0, 150, 150, 150 },
    { "weapons", CONFIG_NODE_ARRAY, 177, 0, 151, 190, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 184, 1, 151, 155, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 185, 0, 151, 151, 151 },
    { "arc", CONFIG_NODE_SCALAR, 185, 0, 152, 152, 152 },
    { "mods", CONFIG_NODE_SCALAR, 185, 0, 153, 153, 153 },
    { "ammo", CONFIG_NODE_SCALAR, 185, 0, 154, 154, 154 },
    { "amount", CONFIG_NODE_SCALAR, 185, 0, 155, 155, 155 },
    { "", CONFIG_NODE_RECORD, 184, 2, 156, 160, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 191, 0, 156, 156, 156 },
    { "arc", CONFIG_NODE_SCALAR, 191, 0, 157, 157, 157 },
    { "mods", CONFIG_NODE_SCALAR, 191, 0, 158, 158, 158 },
    { "ammo", CONFIG_NODE_SCALAR, 191, 0, 159, 159, 159 },
    { "amount", CONFIG_NODE_SCALAR, 191, 0, 160, 160, 160 },
    { "", CONFIG_NODE_RECORD, 184, 3, 161, 165, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 197, 0, 161, 161, 161 },
    { "arc", CONFIG_NODE_SCALAR, 197, 0, 162, 162, 162 },
    { "mods", CONFIG_NODE_SCALAR, 197, 0, 163, 163, 163 },
    { "ammo", CONFIG_NODE_SCALAR, 197, 0, 164, 164, 164 },
    { "amount", CONFIG_NODE_SCALAR, 197, 0, 165, 165, 165 },
    { "", CONFIG_NODE_RECORD, 184, 4, 166, 170, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 203, 0, 166, 166, 166 },
    { "arc", CONFIG_NODE_SCALAR, 203, 0, 167, 167, 167 },
    { "mods", CONFIG_NODE_SCALAR, 203, 0, 168, 168, 168 },
    { "ammo", CONFIG_NODE_SCALAR, 203, 0, 169, 169, 169 },
    { "amount", CONFIG_NODE_SCALAR, 203, 0, 170, 170, 170 },
    { "", CONFIG_NODE_RECORD, 184, 5, 171, 175, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 209, 0, 171, 171, 171 },
    { "arc", CONFIG_NODE_SCALAR, 209, 0, 172, 172, 172 },
    { "mods", CONFIG_NODE_SCALAR, 209, 0, 173, 173, 173 },
    { "ammo", CONFIG_NODE_SCALAR, 209, 0, 174, 174, 174 },
    { "amount", CONFIG_NODE_SCALAR, 209, 0, 175, 175, 175 },
    { "", CONFIG_NODE_RECORD, 184, 6, 176, 180, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 215, 0, 176, 176, 176 },
    { "arc", CONFIG_NODE_SCALAR, 215, 0, 177, 177, 177 },
    { "mods", CONFIG_NODE_SCALAR, 215, 0, 178, 178, 178 },
    { "ammo", CONFIG_NODE_SCALAR, 215, 0, 179, 179, 179 },
    { "amount", CONFIG_NODE_SCALAR, 215, 0, 180, 180, 180 },
    { "", CONFIG_NODE_RECORD, 184, 7, 181, 185, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 221, 0, 181, 181, 181 },
    { "arc", CONFIG_NODE_SCALAR, 221, 0, 182, 182, 182 },
    { "mods", CONFIG_NODE_SCALAR, 221, 0, 183, 183, 183 },
    { "ammo", CONFIG_NODE_SCALAR, 221, 0, 184, 184, 184 },
    { "amount", CONFIG_NODE_SCALAR, 221, 0, 185, 185, 185 },
    { "", CONFIG_NODE_RECORD, 184, 8, 186, 190, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 227, 0, 186, 186, 186 },
    { "arc", CONFIG_NODE_SCALAR, 227, 0, 187, 187, 187 },
    { "mods", CONFIG_NODE_SCALAR, 227, 0, 188, 188, 188 },
    { "ammo", CONFIG_NODE_SCALAR, 227, 0, 189, 189, 189 },
    { "amount", CONFIG_NODE_SCALAR, 227, 0, 190, 190, 190 },
    { "specials", CONFIG_NODE_SCALAR, 177, 0, 191, 191, 191 },
    { "crystal", CONFIG_NODE_ARRAY, 4, 0, 192, 285, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 234, 0, 192, 238, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 235, 0, 192, 192, 192 },
    { "hull_size", CONFIG_NODE_SCALAR, 235, 0, 193, 193, 193 },
    { "drive", CONFIG_NODE_SCALAR, 235, 0, 194, 194, 194 },
    { "armor", CONFIG_NODE_SCALAR, 235, 0, 195, 195, 195 },
    { "shield", CONFIG_NODE_SCALAR, 235, 0, 196, 196, 196 },
    { "computer", CONFIG_NODE_SCALAR, 235, 0, 197, 197, 197 },
    { "weapons", CONFIG_NODE_ARRAY, 235, 0, 198, 237, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 242, 1, 198, 202, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 243, 0, 198, 198, 198 },
    { "arc", CONFIG_NODE_SCALAR, 243, 0, 199, 199, 199 },
    { "mods", CONFIG_NODE_SCALAR, 243, 0, 200, 200, 200 },
    { "ammo", CONFIG_NODE_SCALAR, 243, 0, 201, 201, 201 },
    { "amount", CONFIG_NODE_SCALAR, 243, 0, 202, 202, 202 },
    { "", CONFIG_NODE_RECORD, 242, 2, 203, 207, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 249, 0, 203, 203, 203 },
    { "arc", CONFIG_NODE_SCALAR, 249, 0, 204, 204, 204 },
    { "mods", CONFIG_NODE_SCALAR, 249, 0, 205, 205, 205 },
    { "ammo", CONFIG_NODE_SCALAR, 249, 0, 206, 206, 206 },
    { "amount", CONFIG_NODE_SCALAR, 249, 0, 207, 207, 207 },
    { "", CONFIG_NODE_RECORD, 242, 3, 208, 212, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 255, 0, 208, 208, 208 },
    { "arc", CONFIG_NODE_SCALAR, 255, 0, 209, 209, 209 },
    { "mods", CONFIG_NODE_SCALAR, 255, 0, 210, 210, 210 },
    { "ammo", CONFIG_NODE_SCALAR, 255, 0, 211, 211, 211 },
    { "amount", CONFIG_NODE_SCALAR, 255, 0, 212, 212, 212 },
    { "", CONFIG_NODE_RECORD, 242, 4, 213, 217, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 261, 0, 213, 213, 213 },
    { "arc", CONFIG_NODE_SCALAR, 261, 0, 214, 214, 214 },
    { "mods", CONFIG_NODE_SCALAR, 261, 0, 215, 215, 215 },
    { "ammo", CONFIG_NODE_SCALAR, 261, 0, 216, 216, 216 },
    { "amount", CONFIG_NODE_SCALAR, 261, 0, 217, 217, 217 },
    { "", CONFIG_NODE_RECORD, 242, 5, 218, 222, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 267, 0, 218, 218, 218 },
    { "arc", CONFIG_NODE_SCALAR, 267, 0, 219, 219, 219 },
    { "mods", CONFIG_NODE_SCALAR, 267, 0, 220, 220, 220 },
    { "ammo", CONFIG_NODE_SCALAR, 267, 0, 221, 221, 221 },
    { "amount", CONFIG_NODE_SCALAR, 267, 0, 222, 222, 222 },
    { "", CONFIG_NODE_RECORD, 242, 6, 223, 227, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 273, 0, 223, 223, 223 },
    { "arc", CONFIG_NODE_SCALAR, 273, 0, 224, 224, 224 },
    { "mods", CONFIG_NODE_SCALAR, 273, 0, 225, 225, 225 },
    { "ammo", CONFIG_NODE_SCALAR, 273, 0, 226, 226, 226 },
    { "amount", CONFIG_NODE_SCALAR, 273, 0, 227, 227, 227 },
    { "", CONFIG_NODE_RECORD, 242, 7, 228, 232, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 279, 0, 228, 228, 228 },
    { "arc", CONFIG_NODE_SCALAR, 279, 0, 229, 229, 229 },
    { "mods", CONFIG_NODE_SCALAR, 279, 0, 230, 230, 230 },
    { "ammo", CONFIG_NODE_SCALAR, 279, 0, 231, 231, 231 },
    { "amount", CONFIG_NODE_SCALAR, 279, 0, 232, 232, 232 },
    { "", CONFIG_NODE_RECORD, 242, 8, 233, 237, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 285, 0, 233, 233, 233 },
    { "arc", CONFIG_NODE_SCALAR, 285, 0, 234, 234, 234 },
    { "mods", CONFIG_NODE_SCALAR, 285, 0, 235, 235, 235 },
    { "ammo", CONFIG_NODE_SCALAR, 285, 0, 236, 236, 236 },
    { "amount", CONFIG_NODE_SCALAR, 285, 0, 237, 237, 237 },
    { "specials", CONFIG_NODE_SCALAR, 235, 0, 238, 238, 238 },
    { "", CONFIG_NODE_RECORD, 234, 1, 239, 285, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 292, 0, 239, 239, 239 },
    { "hull_size", CONFIG_NODE_SCALAR, 292, 0, 240, 240, 240 },
    { "drive", CONFIG_NODE_SCALAR, 292, 0, 241, 241, 241 },
    { "armor", CONFIG_NODE_SCALAR, 292, 0, 242, 242, 242 },
    { "shield", CONFIG_NODE_SCALAR, 292, 0, 243, 243, 243 },
    { "computer", CONFIG_NODE_SCALAR, 292, 0, 244, 244, 244 },
    { "weapons", CONFIG_NODE_ARRAY, 292, 0, 245, 284, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 299, 1, 245, 249, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 300, 0, 245, 245, 245 },
    { "arc", CONFIG_NODE_SCALAR, 300, 0, 246, 246, 246 },
    { "mods", CONFIG_NODE_SCALAR, 300, 0, 247, 247, 247 },
    { "ammo", CONFIG_NODE_SCALAR, 300, 0, 248, 248, 248 },
    { "amount", CONFIG_NODE_SCALAR, 300, 0, 249, 249, 249 },
    { "", CONFIG_NODE_RECORD, 299, 2, 250, 254, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 306, 0, 250, 250, 250 },
    { "arc", CONFIG_NODE_SCALAR, 306, 0, 251, 251, 251 },
    { "mods", CONFIG_NODE_SCALAR, 306, 0, 252, 252, 252 },
    { "ammo", CONFIG_NODE_SCALAR, 306, 0, 253, 253, 253 },
    { "amount", CONFIG_NODE_SCALAR, 306, 0, 254, 254, 254 },
    { "", CONFIG_NODE_RECORD, 299, 3, 255, 259, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 312, 0, 255, 255, 255 },
    { "arc", CONFIG_NODE_SCALAR, 312, 0, 256, 256, 256 },
    { "mods", CONFIG_NODE_SCALAR, 312, 0, 257, 257, 257 },
    { "ammo", CONFIG_NODE_SCALAR, 312, 0, 258, 258, 258 },
    { "amount", CONFIG_NODE_SCALAR, 312, 0, 259, 259, 259 },
    { "", CONFIG_NODE_RECORD, 299, 4, 260, 264, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 318, 0, 260, 260, 260 },
    { "arc", CONFIG_NODE_SCALAR, 318, 0, 261, 261, 261 },
    { "mods", CONFIG_NODE_SCALAR, 318, 0, 262, 262, 262 },
    { "ammo", CONFIG_NODE_SCALAR, 318, 0, 263, 263, 263 },
    { "amount", CONFIG_NODE_SCALAR, 318, 0, 264, 264, 264 },
    { "", CONFIG_NODE_RECORD, 299, 5, 265, 269, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 324, 0, 265, 265, 265 },
    { "arc", CONFIG_NODE_SCALAR, 324, 0, 266, 266, 266 },
    { "mods", CONFIG_NODE_SCALAR, 324, 0, 267, 267, 267 },
    { "ammo", CONFIG_NODE_SCALAR, 324, 0, 268, 268, 268 },
    { "amount", CONFIG_NODE_SCALAR, 324, 0, 269, 269, 269 },
    { "", CONFIG_NODE_RECORD, 299, 6, 270, 274, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 330, 0, 270, 270, 270 },
    { "arc", CONFIG_NODE_SCALAR, 330, 0, 271, 271, 271 },
    { "mods", CONFIG_NODE_SCALAR, 330, 0, 272, 272, 272 },
    { "ammo", CONFIG_NODE_SCALAR, 330, 0, 273, 273, 273 },
    { "amount", CONFIG_NODE_SCALAR, 330, 0, 274, 274, 274 },
    { "", CONFIG_NODE_RECORD, 299, 7, 275, 279, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 336, 0, 275, 275, 275 },
    { "arc", CONFIG_NODE_SCALAR, 336, 0, 276, 276, 276 },
    { "mods", CONFIG_NODE_SCALAR, 336, 0, 277, 277, 277 },
    { "ammo", CONFIG_NODE_SCALAR, 336, 0, 278, 278, 278 },
    { "amount", CONFIG_NODE_SCALAR, 336, 0, 279, 279, 279 },
    { "", CONFIG_NODE_RECORD, 299, 8, 280, 284, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 342, 0, 280, 280, 280 },
    { "arc", CONFIG_NODE_SCALAR, 342, 0, 281, 281, 281 },
    { "mods", CONFIG_NODE_SCALAR, 342, 0, 282, 282, 282 },
    { "ammo", CONFIG_NODE_SCALAR, 342, 0, 283, 283, 283 },
    { "amount", CONFIG_NODE_SCALAR, 342, 0, 284, 284, 284 },
    { "specials", CONFIG_NODE_SCALAR, 292, 0, 285, 285, 285 },
    { "dragon", CONFIG_NODE_ARRAY, 4, 0, 286, 379, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 349, 0, 286, 332, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 350, 0, 286, 286, 286 },
    { "hull_size", CONFIG_NODE_SCALAR, 350, 0, 287, 287, 287 },
    { "drive", CONFIG_NODE_SCALAR, 350, 0, 288, 288, 288 },
    { "armor", CONFIG_NODE_SCALAR, 350, 0, 289, 289, 289 },
    { "shield", CONFIG_NODE_SCALAR, 350, 0, 290, 290, 290 },
    { "computer", CONFIG_NODE_SCALAR, 350, 0, 291, 291, 291 },
    { "weapons", CONFIG_NODE_ARRAY, 350, 0, 292, 331, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 357, 1, 292, 296, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 358, 0, 292, 292, 292 },
    { "arc", CONFIG_NODE_SCALAR, 358, 0, 293, 293, 293 },
    { "mods", CONFIG_NODE_SCALAR, 358, 0, 294, 294, 294 },
    { "ammo", CONFIG_NODE_SCALAR, 358, 0, 295, 295, 295 },
    { "amount", CONFIG_NODE_SCALAR, 358, 0, 296, 296, 296 },
    { "", CONFIG_NODE_RECORD, 357, 2, 297, 301, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 364, 0, 297, 297, 297 },
    { "arc", CONFIG_NODE_SCALAR, 364, 0, 298, 298, 298 },
    { "mods", CONFIG_NODE_SCALAR, 364, 0, 299, 299, 299 },
    { "ammo", CONFIG_NODE_SCALAR, 364, 0, 300, 300, 300 },
    { "amount", CONFIG_NODE_SCALAR, 364, 0, 301, 301, 301 },
    { "", CONFIG_NODE_RECORD, 357, 3, 302, 306, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 370, 0, 302, 302, 302 },
    { "arc", CONFIG_NODE_SCALAR, 370, 0, 303, 303, 303 },
    { "mods", CONFIG_NODE_SCALAR, 370, 0, 304, 304, 304 },
    { "ammo", CONFIG_NODE_SCALAR, 370, 0, 305, 305, 305 },
    { "amount", CONFIG_NODE_SCALAR, 370, 0, 306, 306, 306 },
    { "", CONFIG_NODE_RECORD, 357, 4, 307, 311, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 376, 0, 307, 307, 307 },
    { "arc", CONFIG_NODE_SCALAR, 376, 0, 308, 308, 308 },
    { "mods", CONFIG_NODE_SCALAR, 376, 0, 309, 309, 309 },
    { "ammo", CONFIG_NODE_SCALAR, 376, 0, 310, 310, 310 },
    { "amount", CONFIG_NODE_SCALAR, 376, 0, 311, 311, 311 },
    { "", CONFIG_NODE_RECORD, 357, 5, 312, 316, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 382, 0, 312, 312, 312 },
    { "arc", CONFIG_NODE_SCALAR, 382, 0, 313, 313, 313 },
    { "mods", CONFIG_NODE_SCALAR, 382, 0, 314, 314, 314 },
    { "ammo", CONFIG_NODE_SCALAR, 382, 0, 315, 315, 315 },
    { "amount", CONFIG_NODE_SCALAR, 382, 0, 316, 316, 316 },
    { "", CONFIG_NODE_RECORD, 357, 6, 317, 321, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 388, 0, 317, 317, 317 },
    { "arc", CONFIG_NODE_SCALAR, 388, 0, 318, 318, 318 },
    { "mods", CONFIG_NODE_SCALAR, 388, 0, 319, 319, 319 },
    { "ammo", CONFIG_NODE_SCALAR, 388, 0, 320, 320, 320 },
    { "amount", CONFIG_NODE_SCALAR, 388, 0, 321, 321, 321 },
    { "", CONFIG_NODE_RECORD, 357, 7, 322, 326, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 394, 0, 322, 322, 322 },
    { "arc", CONFIG_NODE_SCALAR, 394, 0, 323, 323, 323 },
    { "mods", CONFIG_NODE_SCALAR, 394, 0, 324, 324, 324 },
    { "ammo", CONFIG_NODE_SCALAR, 394, 0, 325, 325, 325 },
    { "amount", CONFIG_NODE_SCALAR, 394, 0, 326, 326, 326 },
    { "", CONFIG_NODE_RECORD, 357, 8, 327, 331, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 400, 0, 327, 327, 327 },
    { "arc", CONFIG_NODE_SCALAR, 400, 0, 328, 328, 328 },
    { "mods", CONFIG_NODE_SCALAR, 400, 0, 329, 329, 329 },
    { "ammo", CONFIG_NODE_SCALAR, 400, 0, 330, 330, 330 },
    { "amount", CONFIG_NODE_SCALAR, 400, 0, 331, 331, 331 },
    { "specials", CONFIG_NODE_SCALAR, 350, 0, 332, 332, 332 },
    { "", CONFIG_NODE_RECORD, 349, 1, 333, 379, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 407, 0, 333, 333, 333 },
    { "hull_size", CONFIG_NODE_SCALAR, 407, 0, 334, 334, 334 },
    { "drive", CONFIG_NODE_SCALAR, 407, 0, 335, 335, 335 },
    { "armor", CONFIG_NODE_SCALAR, 407, 0, 336, 336, 336 },
    { "shield", CONFIG_NODE_SCALAR, 407, 0, 337, 337, 337 },
    { "computer", CONFIG_NODE_SCALAR, 407, 0, 338, 338, 338 },
    { "weapons", CONFIG_NODE_ARRAY, 407, 0, 339, 378, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 414, 1, 339, 343, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 415, 0, 339, 339, 339 },
    { "arc", CONFIG_NODE_SCALAR, 415, 0, 340, 340, 340 },
    { "mods", CONFIG_NODE_SCALAR, 415, 0, 341, 341, 341 },
    { "ammo", CONFIG_NODE_SCALAR, 415, 0, 342, 342, 342 },
    { "amount", CONFIG_NODE_SCALAR, 415, 0, 343, 343, 343 },
    { "", CONFIG_NODE_RECORD, 414, 2, 344, 348, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 421, 0, 344, 344, 344 },
    { "arc", CONFIG_NODE_SCALAR, 421, 0, 345, 345, 345 },
    { "mods", CONFIG_NODE_SCALAR, 421, 0, 346, 346, 346 },
    { "ammo", CONFIG_NODE_SCALAR, 421, 0, 347, 347, 347 },
    { "amount", CONFIG_NODE_SCALAR, 421, 0, 348, 348, 348 },
    { "", CONFIG_NODE_RECORD, 414, 3, 349, 353, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 427, 0, 349, 349, 349 },
    { "arc", CONFIG_NODE_SCALAR, 427, 0, 350, 350, 350 },
    { "mods", CONFIG_NODE_SCALAR, 427, 0, 351, 351, 351 },
    { "ammo", CONFIG_NODE_SCALAR, 427, 0, 352, 352, 352 },
    { "amount", CONFIG_NODE_SCALAR, 427, 0, 353, 353, 353 },
    { "", CONFIG_NODE_RECORD, 414, 4, 354, 358, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 433, 0, 354, 354, 354 },
    { "arc", CONFIG_NODE_SCALAR, 433, 0, 355, 355, 355 },
    { "mods", CONFIG_NODE_SCALAR, 433, 0, 356, 356, 356 },
    { "ammo", CONFIG_NODE_SCALAR, 433, 0, 357, 357, 357 },
    { "amount", CONFIG_NODE_SCALAR, 433, 0, 358, 358, 358 },
    { "", CONFIG_NODE_RECORD, 414, 5, 359, 363, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 439, 0, 359, 359, 359 },
    { "arc", CONFIG_NODE_SCALAR, 439, 0, 360, 360, 360 },
    { "mods", CONFIG_NODE_SCALAR, 439, 0, 361, 361, 361 },
    { "ammo", CONFIG_NODE_SCALAR, 439, 0, 362, 362, 362 },
    { "amount", CONFIG_NODE_SCALAR, 439, 0, 363, 363, 363 },
    { "", CONFIG_NODE_RECORD, 414, 6, 364, 368, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 445, 0, 364, 364, 364 },
    { "arc", CONFIG_NODE_SCALAR, 445, 0, 365, 365, 365 },
    { "mods", CONFIG_NODE_SCALAR, 445, 0, 366, 366, 366 },
    { "ammo", CONFIG_NODE_SCALAR, 445, 0, 367, 367, 367 },
    { "amount", CONFIG_NODE_SCALAR, 445, 0, 368, 368, 368 },
    { "", CONFIG_NODE_RECORD, 414, 7, 369, 373, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 451, 0, 369, 369, 369 },
    { "arc", CONFIG_NODE_SCALAR, 451, 0, 370, 370, 370 },
    { "mods", CONFIG_NODE_SCALAR, 451, 0, 371, 371, 371 },
    { "ammo", CONFIG_NODE_SCALAR, 451, 0, 372, 372, 372 },
    { "amount", CONFIG_NODE_SCALAR, 451, 0, 373, 373, 373 },
    { "", CONFIG_NODE_RECORD, 414, 8, 374, 378, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 457, 0, 374, 374, 374 },
    { "arc", CONFIG_NODE_SCALAR, 457, 0, 375, 375, 375 },
    { "mods", CONFIG_NODE_SCALAR, 457, 0, 376, 376, 376 },
    { "ammo", CONFIG_NODE_SCALAR, 457, 0, 377, 377, 377 },
    { "amount", CONFIG_NODE_SCALAR, 457, 0, 378, 378, 378 },
    { "specials", CONFIG_NODE_SCALAR, 407, 0, 379, 379, 379 },
    { "eel", CONFIG_NODE_ARRAY, 4, 0, 380, 473, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 464, 0, 380, 426, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 465, 0, 380, 380, 380 },
    { "hull_size", CONFIG_NODE_SCALAR, 465, 0, 381, 381, 381 },
    { "drive", CONFIG_NODE_SCALAR, 465, 0, 382, 382, 382 },
    { "armor", CONFIG_NODE_SCALAR, 465, 0, 383, 383, 383 },
    { "shield", CONFIG_NODE_SCALAR, 465, 0, 384, 384, 384 },
    { "computer", CONFIG_NODE_SCALAR, 465, 0, 385, 385, 385 },
    { "weapons", CONFIG_NODE_ARRAY, 465, 0, 386, 425, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 472, 1, 386, 390, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 473, 0, 386, 386, 386 },
    { "arc", CONFIG_NODE_SCALAR, 473, 0, 387, 387, 387 },
    { "mods", CONFIG_NODE_SCALAR, 473, 0, 388, 388, 388 },
    { "ammo", CONFIG_NODE_SCALAR, 473, 0, 389, 389, 389 },
    { "amount", CONFIG_NODE_SCALAR, 473, 0, 390, 390, 390 },
    { "", CONFIG_NODE_RECORD, 472, 2, 391, 395, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 479, 0, 391, 391, 391 },
    { "arc", CONFIG_NODE_SCALAR, 479, 0, 392, 392, 392 },
    { "mods", CONFIG_NODE_SCALAR, 479, 0, 393, 393, 393 },
    { "ammo", CONFIG_NODE_SCALAR, 479, 0, 394, 394, 394 },
    { "amount", CONFIG_NODE_SCALAR, 479, 0, 395, 395, 395 },
    { "", CONFIG_NODE_RECORD, 472, 3, 396, 400, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 485, 0, 396, 396, 396 },
    { "arc", CONFIG_NODE_SCALAR, 485, 0, 397, 397, 397 },
    { "mods", CONFIG_NODE_SCALAR, 485, 0, 398, 398, 398 },
    { "ammo", CONFIG_NODE_SCALAR, 485, 0, 399, 399, 399 },
    { "amount", CONFIG_NODE_SCALAR, 485, 0, 400, 400, 400 },
    { "", CONFIG_NODE_RECORD, 472, 4, 401, 405, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 491, 0, 401, 401, 401 },
    { "arc", CONFIG_NODE_SCALAR, 491, 0, 402, 402, 402 },
    { "mods", CONFIG_NODE_SCALAR, 491, 0, 403, 403, 403 },
    { "ammo", CONFIG_NODE_SCALAR, 491, 0, 404, 404, 404 },
    { "amount", CONFIG_NODE_SCALAR, 491, 0, 405, 405, 405 },
    { "", CONFIG_NODE_RECORD, 472, 5, 406, 410, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 497, 0, 406, 406, 406 },
    { "arc", CONFIG_NODE_SCALAR, 497, 0, 407, 407, 407 },
    { "mods", CONFIG_NODE_SCALAR, 497, 0, 408, 408, 408 },
    { "ammo", CONFIG_NODE_SCALAR, 497, 0, 409, 409, 409 },
    { "amount", CONFIG_NODE_SCALAR, 497, 0, 410, 410, 410 },
    { "", CONFIG_NODE_RECORD, 472, 6, 411, 415, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 503, 0, 411, 411, 411 },
    { "arc", CONFIG_NODE_SCALAR, 503, 0, 412, 412, 412 },
    { "mods", CONFIG_NODE_SCALAR, 503, 0, 413, 413, 413 },
    { "ammo", CONFIG_NODE_SCALAR, 503, 0, 414, 414, 414 },
    { "amount", CONFIG_NODE_SCALAR, 503, 0, 415, 415, 415 },
    { "", CONFIG_NODE_RECORD, 472, 7, 416, 420, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 509, 0, 416, 416, 416 },
    { "arc", CONFIG_NODE_SCALAR, 509, 0, 417, 417, 417 },
    { "mods", CONFIG_NODE_SCALAR, 509, 0, 418, 418, 418 },
    { "ammo", CONFIG_NODE_SCALAR, 509, 0, 419, 419, 419 },
    { "amount", CONFIG_NODE_SCALAR, 509, 0, 420, 420, 420 },
    { "", CONFIG_NODE_RECORD, 472, 8, 421, 425, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 515, 0, 421, 421, 421 },
    { "arc", CONFIG_NODE_SCALAR, 515, 0, 422, 422, 422 },
    { "mods", CONFIG_NODE_SCALAR, 515, 0, 423, 423, 423 },
    { "ammo", CONFIG_NODE_SCALAR, 515, 0, 424, 424, 424 },
    { "amount", CONFIG_NODE_SCALAR, 515, 0, 425, 425, 425 },
    { "specials", CONFIG_NODE_SCALAR, 465, 0, 426, 426, 426 },
    { "", CONFIG_NODE_RECORD, 464, 1, 427, 473, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 522, 0, 427, 427, 427 },
    { "hull_size", CONFIG_NODE_SCALAR, 522, 0, 428, 428, 428 },
    { "drive", CONFIG_NODE_SCALAR, 522, 0, 429, 429, 429 },
    { "armor", CONFIG_NODE_SCALAR, 522, 0, 430, 430, 430 },
    { "shield", CONFIG_NODE_SCALAR, 522, 0, 431, 431, 431 },
    { "computer", CONFIG_NODE_SCALAR, 522, 0, 432, 432, 432 },
    { "weapons", CONFIG_NODE_ARRAY, 522, 0, 433, 472, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 529, 1, 433, 437, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 530, 0, 433, 433, 433 },
    { "arc", CONFIG_NODE_SCALAR, 530, 0, 434, 434, 434 },
    { "mods", CONFIG_NODE_SCALAR, 530, 0, 435, 435, 435 },
    { "ammo", CONFIG_NODE_SCALAR, 530, 0, 436, 436, 436 },
    { "amount", CONFIG_NODE_SCALAR, 530, 0, 437, 437, 437 },
    { "", CONFIG_NODE_RECORD, 529, 2, 438, 442, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 536, 0, 438, 438, 438 },
    { "arc", CONFIG_NODE_SCALAR, 536, 0, 439, 439, 439 },
    { "mods", CONFIG_NODE_SCALAR, 536, 0, 440, 440, 440 },
    { "ammo", CONFIG_NODE_SCALAR, 536, 0, 441, 441, 441 },
    { "amount", CONFIG_NODE_SCALAR, 536, 0, 442, 442, 442 },
    { "", CONFIG_NODE_RECORD, 529, 3, 443, 447, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 542, 0, 443, 443, 443 },
    { "arc", CONFIG_NODE_SCALAR, 542, 0, 444, 444, 444 },
    { "mods", CONFIG_NODE_SCALAR, 542, 0, 445, 445, 445 },
    { "ammo", CONFIG_NODE_SCALAR, 542, 0, 446, 446, 446 },
    { "amount", CONFIG_NODE_SCALAR, 542, 0, 447, 447, 447 },
    { "", CONFIG_NODE_RECORD, 529, 4, 448, 452, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 548, 0, 448, 448, 448 },
    { "arc", CONFIG_NODE_SCALAR, 548, 0, 449, 449, 449 },
    { "mods", CONFIG_NODE_SCALAR, 548, 0, 450, 450, 450 },
    { "ammo", CONFIG_NODE_SCALAR, 548, 0, 451, 451, 451 },
    { "amount", CONFIG_NODE_SCALAR, 548, 0, 452, 452, 452 },
    { "", CONFIG_NODE_RECORD, 529, 5, 453, 457, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 554, 0, 453, 453, 453 },
    { "arc", CONFIG_NODE_SCALAR, 554, 0, 454, 454, 454 },
    { "mods", CONFIG_NODE_SCALAR, 554, 0, 455, 455, 455 },
    { "ammo", CONFIG_NODE_SCALAR, 554, 0, 456, 456, 456 },
    { "amount", CONFIG_NODE_SCALAR, 554, 0, 457, 457, 457 },
    { "", CONFIG_NODE_RECORD, 529, 6, 458, 462, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 560, 0, 458, 458, 458 },
    { "arc", CONFIG_NODE_SCALAR, 560, 0, 459, 459, 459 },
    { "mods", CONFIG_NODE_SCALAR, 560, 0, 460, 460, 460 },
    { "ammo", CONFIG_NODE_SCALAR, 560, 0, 461, 461, 461 },
    { "amount", CONFIG_NODE_SCALAR, 560, 0, 462, 462, 462 },
    { "", CONFIG_NODE_RECORD, 529, 7, 463, 467, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 566, 0, 463, 463, 463 },
    { "arc", CONFIG_NODE_SCALAR, 566, 0, 464, 464, 464 },
    { "mods", CONFIG_NODE_SCALAR, 566, 0, 465, 465, 465 },
    { "ammo", CONFIG_NODE_SCALAR, 566, 0, 466, 466, 466 },
    { "amount", CONFIG_NODE_SCALAR, 566, 0, 467, 467, 467 },
    { "", CONFIG_NODE_RECORD, 529, 8, 468, 472, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 572, 0, 468, 468, 468 },
    { "arc", CONFIG_NODE_SCALAR, 572, 0, 469, 469, 469 },
    { "mods", CONFIG_NODE_SCALAR, 572, 0, 470, 470, 470 },
    { "ammo", CONFIG_NODE_SCALAR, 572, 0, 471, 471, 471 },
    { "amount", CONFIG_NODE_SCALAR, 572, 0, 472, 472, 472 },
    { "specials", CONFIG_NODE_SCALAR, 522, 0, 473, 473, 473 },
    { "hydra", CONFIG_NODE_ARRAY, 4, 0, 474, 567, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 579, 0, 474, 520, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 580, 0, 474, 474, 474 },
    { "hull_size", CONFIG_NODE_SCALAR, 580, 0, 475, 475, 475 },
    { "drive", CONFIG_NODE_SCALAR, 580, 0, 476, 476, 476 },
    { "armor", CONFIG_NODE_SCALAR, 580, 0, 477, 477, 477 },
    { "shield", CONFIG_NODE_SCALAR, 580, 0, 478, 478, 478 },
    { "computer", CONFIG_NODE_SCALAR, 580, 0, 479, 479, 479 },
    { "weapons", CONFIG_NODE_ARRAY, 580, 0, 480, 519, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 587, 1, 480, 484, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 588, 0, 480, 480, 480 },
    { "arc", CONFIG_NODE_SCALAR, 588, 0, 481, 481, 481 },
    { "mods", CONFIG_NODE_SCALAR, 588, 0, 482, 482, 482 },
    { "ammo", CONFIG_NODE_SCALAR, 588, 0, 483, 483, 483 },
    { "amount", CONFIG_NODE_SCALAR, 588, 0, 484, 484, 484 },
    { "", CONFIG_NODE_RECORD, 587, 2, 485, 489, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 594, 0, 485, 485, 485 },
    { "arc", CONFIG_NODE_SCALAR, 594, 0, 486, 486, 486 },
    { "mods", CONFIG_NODE_SCALAR, 594, 0, 487, 487, 487 },
    { "ammo", CONFIG_NODE_SCALAR, 594, 0, 488, 488, 488 },
    { "amount", CONFIG_NODE_SCALAR, 594, 0, 489, 489, 489 },
    { "", CONFIG_NODE_RECORD, 587, 3, 490, 494, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 600, 0, 490, 490, 490 },
    { "arc", CONFIG_NODE_SCALAR, 600, 0, 491, 491, 491 },
    { "mods", CONFIG_NODE_SCALAR, 600, 0, 492, 492, 492 },
    { "ammo", CONFIG_NODE_SCALAR, 600, 0, 493, 493, 493 },
    { "amount", CONFIG_NODE_SCALAR, 600, 0, 494, 494, 494 },
    { "", CONFIG_NODE_RECORD, 587, 4, 495, 499, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 606, 0, 495, 495, 495 },
    { "arc", CONFIG_NODE_SCALAR, 606, 0, 496, 496, 496 },
    { "mods", CONFIG_NODE_SCALAR, 606, 0, 497, 497, 497 },
    { "ammo", CONFIG_NODE_SCALAR, 606, 0, 498, 498, 498 },
    { "amount", CONFIG_NODE_SCALAR, 606, 0, 499, 499, 499 },
    { "", CONFIG_NODE_RECORD, 587, 5, 500, 504, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 612, 0, 500, 500, 500 },
    { "arc", CONFIG_NODE_SCALAR, 612, 0, 501, 501, 501 },
    { "mods", CONFIG_NODE_SCALAR, 612, 0, 502, 502, 502 },
    { "ammo", CONFIG_NODE_SCALAR, 612, 0, 503, 503, 503 },
    { "amount", CONFIG_NODE_SCALAR, 612, 0, 504, 504, 504 },
    { "", CONFIG_NODE_RECORD, 587, 6, 505, 509, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 618, 0, 505, 505, 505 },
    { "arc", CONFIG_NODE_SCALAR, 618, 0, 506, 506, 506 },
    { "mods", CONFIG_NODE_SCALAR, 618, 0, 507, 507, 507 },
    { "ammo", CONFIG_NODE_SCALAR, 618, 0, 508, 508, 508 },
    { "amount", CONFIG_NODE_SCALAR, 618, 0, 509, 509, 509 },
    { "", CONFIG_NODE_RECORD, 587, 7, 510, 514, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 624, 0, 510, 510, 510 },
    { "arc", CONFIG_NODE_SCALAR, 624, 0, 511, 511, 511 },
    { "mods", CONFIG_NODE_SCALAR, 624, 0, 512, 512, 512 },
    { "ammo", CONFIG_NODE_SCALAR, 624, 0, 513, 513, 513 },
    { "amount", CONFIG_NODE_SCALAR, 624, 0, 514, 514, 514 },
    { "", CONFIG_NODE_RECORD, 587, 8, 515, 519, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 630, 0, 515, 515, 515 },
    { "arc", CONFIG_NODE_SCALAR, 630, 0, 516, 516, 516 },
    { "mods", CONFIG_NODE_SCALAR, 630, 0, 517, 517, 517 },
    { "ammo", CONFIG_NODE_SCALAR, 630, 0, 518, 518, 518 },
    { "amount", CONFIG_NODE_SCALAR, 630, 0, 519, 519, 519 },
    { "specials", CONFIG_NODE_SCALAR, 580, 0, 520, 520, 520 },
    { "", CONFIG_NODE_RECORD, 579, 1, 521, 567, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 637, 0, 521, 521, 521 },
    { "hull_size", CONFIG_NODE_SCALAR, 637, 0, 522, 522, 522 },
    { "drive", CONFIG_NODE_SCALAR, 637, 0, 523, 523, 523 },
    { "armor", CONFIG_NODE_SCALAR, 637, 0, 524, 524, 524 },
    { "shield", CONFIG_NODE_SCALAR, 637, 0, 525, 525, 525 },
    { "computer", CONFIG_NODE_SCALAR, 637, 0, 526, 526, 526 },
    { "weapons", CONFIG_NODE_ARRAY, 637, 0, 527, 566, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 644, 1, 527, 531, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 645, 0, 527, 527, 527 },
    { "arc", CONFIG_NODE_SCALAR, 645, 0, 528, 528, 528 },
    { "mods", CONFIG_NODE_SCALAR, 645, 0, 529, 529, 529 },
    { "ammo", CONFIG_NODE_SCALAR, 645, 0, 530, 530, 530 },
    { "amount", CONFIG_NODE_SCALAR, 645, 0, 531, 531, 531 },
    { "", CONFIG_NODE_RECORD, 644, 2, 532, 536, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 651, 0, 532, 532, 532 },
    { "arc", CONFIG_NODE_SCALAR, 651, 0, 533, 533, 533 },
    { "mods", CONFIG_NODE_SCALAR, 651, 0, 534, 534, 534 },
    { "ammo", CONFIG_NODE_SCALAR, 651, 0, 535, 535, 535 },
    { "amount", CONFIG_NODE_SCALAR, 651, 0, 536, 536, 536 },
    { "", CONFIG_NODE_RECORD, 644, 3, 537, 541, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 657, 0, 537, 537, 537 },
    { "arc", CONFIG_NODE_SCALAR, 657, 0, 538, 538, 538 },
    { "mods", CONFIG_NODE_SCALAR, 657, 0, 539, 539, 539 },
    { "ammo", CONFIG_NODE_SCALAR, 657, 0, 540, 540, 540 },
    { "amount", CONFIG_NODE_SCALAR, 657, 0, 541, 541, 541 },
    { "", CONFIG_NODE_RECORD, 644, 4, 542, 546, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 663, 0, 542, 542, 542 },
    { "arc", CONFIG_NODE_SCALAR, 663, 0, 543, 543, 543 },
    { "mods", CONFIG_NODE_SCALAR, 663, 0, 544, 544, 544 },
    { "ammo", CONFIG_NODE_SCALAR, 663, 0, 545, 545, 545 },
    { "amount", CONFIG_NODE_SCALAR, 663, 0, 546, 546, 546 },
    { "", CONFIG_NODE_RECORD, 644, 5, 547, 551, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 669, 0, 547, 547, 547 },
    { "arc", CONFIG_NODE_SCALAR, 669, 0, 548, 548, 548 },
    { "mods", CONFIG_NODE_SCALAR, 669, 0, 549, 549, 549 },
    { "ammo", CONFIG_NODE_SCALAR, 669, 0, 550, 550, 550 },
    { "amount", CONFIG_NODE_SCALAR, 669, 0, 551, 551, 551 },
    { "", CONFIG_NODE_RECORD, 644, 6, 552, 556, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 675, 0, 552, 552, 552 },
    { "arc", CONFIG_NODE_SCALAR, 675, 0, 553, 553, 553 },
    { "mods", CONFIG_NODE_SCALAR, 675, 0, 554, 554, 554 },
    { "ammo", CONFIG_NODE_SCALAR, 675, 0, 555, 555, 555 },
    { "amount", CONFIG_NODE_SCALAR, 675, 0, 556, 556, 556 },
    { "", CONFIG_NODE_RECORD, 644, 7, 557, 561, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 681, 0, 557, 557, 557 },
    { "arc", CONFIG_NODE_SCALAR, 681, 0, 558, 558, 558 },
    { "mods", CONFIG_NODE_SCALAR, 681, 0, 559, 559, 559 },
    { "ammo", CONFIG_NODE_SCALAR, 681, 0, 560, 560, 560 },
    { "amount", CONFIG_NODE_SCALAR, 681, 0, 561, 561, 561 },
    { "", CONFIG_NODE_RECORD, 644, 8, 562, 566, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 687, 0, 562, 562, 562 },
    { "arc", CONFIG_NODE_SCALAR, 687, 0, 563, 563, 563 },
    { "mods", CONFIG_NODE_SCALAR, 687, 0, 564, 564, 564 },
    { "ammo", CONFIG_NODE_SCALAR, 687, 0, 565, 565, 565 },
    { "amount", CONFIG_NODE_SCALAR, 687, 0, 566, 566, 566 },
    { "specials", CONFIG_NODE_SCALAR, 637, 0, 567, 567, 567 },
    { "antaran", CONFIG_NODE_ARRAY, 4, 0, 568, 1742, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_ARRAY, 694, 0, 568, 802, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 695, 0, 568, 614, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 696, 0, 568, 568, 568 },
    { "hull_size", CONFIG_NODE_SCALAR, 696, 0, 569, 569, 569 },
    { "drive", CONFIG_NODE_SCALAR, 696, 0, 570, 570, 570 },
    { "armor", CONFIG_NODE_SCALAR, 696, 0, 571, 571, 571 },
    { "shield", CONFIG_NODE_SCALAR, 696, 0, 572, 572, 572 },
    { "computer", CONFIG_NODE_SCALAR, 696, 0, 573, 573, 573 },
    { "weapons", CONFIG_NODE_ARRAY, 696, 0, 574, 613, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 703, 1, 574, 578, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 704, 0, 574, 574, 574 },
    { "arc", CONFIG_NODE_SCALAR, 704, 0, 575, 575, 575 },
    { "mods", CONFIG_NODE_SCALAR, 704, 0, 576, 576, 576 },
    { "ammo", CONFIG_NODE_SCALAR, 704, 0, 577, 577, 577 },
    { "amount", CONFIG_NODE_SCALAR, 704, 0, 578, 578, 578 },
    { "", CONFIG_NODE_RECORD, 703, 2, 579, 583, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 710, 0, 579, 579, 579 },
    { "arc", CONFIG_NODE_SCALAR, 710, 0, 580, 580, 580 },
    { "mods", CONFIG_NODE_SCALAR, 710, 0, 581, 581, 581 },
    { "ammo", CONFIG_NODE_SCALAR, 710, 0, 582, 582, 582 },
    { "amount", CONFIG_NODE_SCALAR, 710, 0, 583, 583, 583 },
    { "", CONFIG_NODE_RECORD, 703, 3, 584, 588, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 716, 0, 584, 584, 584 },
    { "arc", CONFIG_NODE_SCALAR, 716, 0, 585, 585, 585 },
    { "mods", CONFIG_NODE_SCALAR, 716, 0, 586, 586, 586 },
    { "ammo", CONFIG_NODE_SCALAR, 716, 0, 587, 587, 587 },
    { "amount", CONFIG_NODE_SCALAR, 716, 0, 588, 588, 588 },
    { "", CONFIG_NODE_RECORD, 703, 4, 589, 593, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 722, 0, 589, 589, 589 },
    { "arc", CONFIG_NODE_SCALAR, 722, 0, 590, 590, 590 },
    { "mods", CONFIG_NODE_SCALAR, 722, 0, 591, 591, 591 },
    { "ammo", CONFIG_NODE_SCALAR, 722, 0, 592, 592, 592 },
    { "amount", CONFIG_NODE_SCALAR, 722, 0, 593, 593, 593 },
    { "", CONFIG_NODE_RECORD, 703, 5, 594, 598, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 728, 0, 594, 594, 594 },
    { "arc", CONFIG_NODE_SCALAR, 728, 0, 595, 595, 595 },
    { "mods", CONFIG_NODE_SCALAR, 728, 0, 596, 596, 596 },
    { "ammo", CONFIG_NODE_SCALAR, 728, 0, 597, 597, 597 },
    { "amount", CONFIG_NODE_SCALAR, 728, 0, 598, 598, 598 },
    { "", CONFIG_NODE_RECORD, 703, 6, 599, 603, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 734, 0, 599, 599, 599 },
    { "arc", CONFIG_NODE_SCALAR, 734, 0, 600, 600, 600 },
    { "mods", CONFIG_NODE_SCALAR, 734, 0, 601, 601, 601 },
    { "ammo", CONFIG_NODE_SCALAR, 734, 0, 602, 602, 602 },
    { "amount", CONFIG_NODE_SCALAR, 734, 0, 603, 603, 603 },
    { "", CONFIG_NODE_RECORD, 703, 7, 604, 608, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 740, 0, 604, 604, 604 },
    { "arc", CONFIG_NODE_SCALAR, 740, 0, 605, 605, 605 },
    { "mods", CONFIG_NODE_SCALAR, 740, 0, 606, 606, 606 },
    { "ammo", CONFIG_NODE_SCALAR, 740, 0, 607, 607, 607 },
    { "amount", CONFIG_NODE_SCALAR, 740, 0, 608, 608, 608 },
    { "", CONFIG_NODE_RECORD, 703, 8, 609, 613, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 746, 0, 609, 609, 609 },
    { "arc", CONFIG_NODE_SCALAR, 746, 0, 610, 610, 610 },
    { "mods", CONFIG_NODE_SCALAR, 746, 0, 611, 611, 611 },
    { "ammo", CONFIG_NODE_SCALAR, 746, 0, 612, 612, 612 },
    { "amount", CONFIG_NODE_SCALAR, 746, 0, 613, 613, 613 },
    { "specials", CONFIG_NODE_SCALAR, 696, 0, 614, 614, 614 },
    { "", CONFIG_NODE_RECORD, 695, 1, 615, 661, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 753, 0, 615, 615, 615 },
    { "hull_size", CONFIG_NODE_SCALAR, 753, 0, 616, 616, 616 },
    { "drive", CONFIG_NODE_SCALAR, 753, 0, 617, 617, 617 },
    { "armor", CONFIG_NODE_SCALAR, 753, 0, 618, 618, 618 },
    { "shield", CONFIG_NODE_SCALAR, 753, 0, 619, 619, 619 },
    { "computer", CONFIG_NODE_SCALAR, 753, 0, 620, 620, 620 },
    { "weapons", CONFIG_NODE_ARRAY, 753, 0, 621, 660, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 760, 1, 621, 625, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 761, 0, 621, 621, 621 },
    { "arc", CONFIG_NODE_SCALAR, 761, 0, 622, 622, 622 },
    { "mods", CONFIG_NODE_SCALAR, 761, 0, 623, 623, 623 },
    { "ammo", CONFIG_NODE_SCALAR, 761, 0, 624, 624, 624 },
    { "amount", CONFIG_NODE_SCALAR, 761, 0, 625, 625, 625 },
    { "", CONFIG_NODE_RECORD, 760, 2, 626, 630, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 767, 0, 626, 626, 626 },
    { "arc", CONFIG_NODE_SCALAR, 767, 0, 627, 627, 627 },
    { "mods", CONFIG_NODE_SCALAR, 767, 0, 628, 628, 628 },
    { "ammo", CONFIG_NODE_SCALAR, 767, 0, 629, 629, 629 },
    { "amount", CONFIG_NODE_SCALAR, 767, 0, 630, 630, 630 },
    { "", CONFIG_NODE_RECORD, 760, 3, 631, 635, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 773, 0, 631, 631, 631 },
    { "arc", CONFIG_NODE_SCALAR, 773, 0, 632, 632, 632 },
    { "mods", CONFIG_NODE_SCALAR, 773, 0, 633, 633, 633 },
    { "ammo", CONFIG_NODE_SCALAR, 773, 0, 634, 634, 634 },
    { "amount", CONFIG_NODE_SCALAR, 773, 0, 635, 635, 635 },
    { "", CONFIG_NODE_RECORD, 760, 4, 636, 640, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 779, 0, 636, 636, 636 },
    { "arc", CONFIG_NODE_SCALAR, 779, 0, 637, 637, 637 },
    { "mods", CONFIG_NODE_SCALAR, 779, 0, 638, 638, 638 },
    { "ammo", CONFIG_NODE_SCALAR, 779, 0, 639, 639, 639 },
    { "amount", CONFIG_NODE_SCALAR, 779, 0, 640, 640, 640 },
    { "", CONFIG_NODE_RECORD, 760, 5, 641, 645, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 785, 0, 641, 641, 641 },
    { "arc", CONFIG_NODE_SCALAR, 785, 0, 642, 642, 642 },
    { "mods", CONFIG_NODE_SCALAR, 785, 0, 643, 643, 643 },
    { "ammo", CONFIG_NODE_SCALAR, 785, 0, 644, 644, 644 },
    { "amount", CONFIG_NODE_SCALAR, 785, 0, 645, 645, 645 },
    { "", CONFIG_NODE_RECORD, 760, 6, 646, 650, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 791, 0, 646, 646, 646 },
    { "arc", CONFIG_NODE_SCALAR, 791, 0, 647, 647, 647 },
    { "mods", CONFIG_NODE_SCALAR, 791, 0, 648, 648, 648 },
    { "ammo", CONFIG_NODE_SCALAR, 791, 0, 649, 649, 649 },
    { "amount", CONFIG_NODE_SCALAR, 791, 0, 650, 650, 650 },
    { "", CONFIG_NODE_RECORD, 760, 7, 651, 655, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 797, 0, 651, 651, 651 },
    { "arc", CONFIG_NODE_SCALAR, 797, 0, 652, 652, 652 },
    { "mods", CONFIG_NODE_SCALAR, 797, 0, 653, 653, 653 },
    { "ammo", CONFIG_NODE_SCALAR, 797, 0, 654, 654, 654 },
    { "amount", CONFIG_NODE_SCALAR, 797, 0, 655, 655, 655 },
    { "", CONFIG_NODE_RECORD, 760, 8, 656, 660, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 803, 0, 656, 656, 656 },
    { "arc", CONFIG_NODE_SCALAR, 803, 0, 657, 657, 657 },
    { "mods", CONFIG_NODE_SCALAR, 803, 0, 658, 658, 658 },
    { "ammo", CONFIG_NODE_SCALAR, 803, 0, 659, 659, 659 },
    { "amount", CONFIG_NODE_SCALAR, 803, 0, 660, 660, 660 },
    { "specials", CONFIG_NODE_SCALAR, 753, 0, 661, 661, 661 },
    { "", CONFIG_NODE_RECORD, 695, 2, 662, 708, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 810, 0, 662, 662, 662 },
    { "hull_size", CONFIG_NODE_SCALAR, 810, 0, 663, 663, 663 },
    { "drive", CONFIG_NODE_SCALAR, 810, 0, 664, 664, 664 },
    { "armor", CONFIG_NODE_SCALAR, 810, 0, 665, 665, 665 },
    { "shield", CONFIG_NODE_SCALAR, 810, 0, 666, 666, 666 },
    { "computer", CONFIG_NODE_SCALAR, 810, 0, 667, 667, 667 },
    { "weapons", CONFIG_NODE_ARRAY, 810, 0, 668, 707, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 817, 1, 668, 672, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 818, 0, 668, 668, 668 },
    { "arc", CONFIG_NODE_SCALAR, 818, 0, 669, 669, 669 },
    { "mods", CONFIG_NODE_SCALAR, 818, 0, 670, 670, 670 },
    { "ammo", CONFIG_NODE_SCALAR, 818, 0, 671, 671, 671 },
    { "amount", CONFIG_NODE_SCALAR, 818, 0, 672, 672, 672 },
    { "", CONFIG_NODE_RECORD, 817, 2, 673, 677, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 824, 0, 673, 673, 673 },
    { "arc", CONFIG_NODE_SCALAR, 824, 0, 674, 674, 674 },
    { "mods", CONFIG_NODE_SCALAR, 824, 0, 675, 675, 675 },
    { "ammo", CONFIG_NODE_SCALAR, 824, 0, 676, 676, 676 },
    { "amount", CONFIG_NODE_SCALAR, 824, 0, 677, 677, 677 },
    { "", CONFIG_NODE_RECORD, 817, 3, 678, 682, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 830, 0, 678, 678, 678 },
    { "arc", CONFIG_NODE_SCALAR, 830, 0, 679, 679, 679 },
    { "mods", CONFIG_NODE_SCALAR, 830, 0, 680, 680, 680 },
    { "ammo", CONFIG_NODE_SCALAR, 830, 0, 681, 681, 681 },
    { "amount", CONFIG_NODE_SCALAR, 830, 0, 682, 682, 682 },
    { "", CONFIG_NODE_RECORD, 817, 4, 683, 687, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 836, 0, 683, 683, 683 },
    { "arc", CONFIG_NODE_SCALAR, 836, 0, 684, 684, 684 },
    { "mods", CONFIG_NODE_SCALAR, 836, 0, 685, 685, 685 },
    { "ammo", CONFIG_NODE_SCALAR, 836, 0, 686, 686, 686 },
    { "amount", CONFIG_NODE_SCALAR, 836, 0, 687, 687, 687 },
    { "", CONFIG_NODE_RECORD, 817, 5, 688, 692, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 842, 0, 688, 688, 688 },
    { "arc", CONFIG_NODE_SCALAR, 842, 0, 689, 689, 689 },
    { "mods", CONFIG_NODE_SCALAR, 842, 0, 690, 690, 690 },
    { "ammo", CONFIG_NODE_SCALAR, 842, 0, 691, 691, 691 },
    { "amount", CONFIG_NODE_SCALAR, 842, 0, 692, 692, 692 },
    { "", CONFIG_NODE_RECORD, 817, 6, 693, 697, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 848, 0, 693, 693, 693 },
    { "arc", CONFIG_NODE_SCALAR, 848, 0, 694, 694, 694 },
    { "mods", CONFIG_NODE_SCALAR, 848, 0, 695, 695, 695 },
    { "ammo", CONFIG_NODE_SCALAR, 848, 0, 696, 696, 696 },
    { "amount", CONFIG_NODE_SCALAR, 848, 0, 697, 697, 697 },
    { "", CONFIG_NODE_RECORD, 817, 7, 698, 702, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 854, 0, 698, 698, 698 },
    { "arc", CONFIG_NODE_SCALAR, 854, 0, 699, 699, 699 },
    { "mods", CONFIG_NODE_SCALAR, 854, 0, 700, 700, 700 },
    { "ammo", CONFIG_NODE_SCALAR, 854, 0, 701, 701, 701 },
    { "amount", CONFIG_NODE_SCALAR, 854, 0, 702, 702, 702 },
    { "", CONFIG_NODE_RECORD, 817, 8, 703, 707, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 860, 0, 703, 703, 703 },
    { "arc", CONFIG_NODE_SCALAR, 860, 0, 704, 704, 704 },
    { "mods", CONFIG_NODE_SCALAR, 860, 0, 705, 705, 705 },
    { "ammo", CONFIG_NODE_SCALAR, 860, 0, 706, 706, 706 },
    { "amount", CONFIG_NODE_SCALAR, 860, 0, 707, 707, 707 },
    { "specials", CONFIG_NODE_SCALAR, 810, 0, 708, 708, 708 },
    { "", CONFIG_NODE_RECORD, 695, 3, 709, 755, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 867, 0, 709, 709, 709 },
    { "hull_size", CONFIG_NODE_SCALAR, 867, 0, 710, 710, 710 },
    { "drive", CONFIG_NODE_SCALAR, 867, 0, 711, 711, 711 },
    { "armor", CONFIG_NODE_SCALAR, 867, 0, 712, 712, 712 },
    { "shield", CONFIG_NODE_SCALAR, 867, 0, 713, 713, 713 },
    { "computer", CONFIG_NODE_SCALAR, 867, 0, 714, 714, 714 },
    { "weapons", CONFIG_NODE_ARRAY, 867, 0, 715, 754, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 874, 1, 715, 719, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 875, 0, 715, 715, 715 },
    { "arc", CONFIG_NODE_SCALAR, 875, 0, 716, 716, 716 },
    { "mods", CONFIG_NODE_SCALAR, 875, 0, 717, 717, 717 },
    { "ammo", CONFIG_NODE_SCALAR, 875, 0, 718, 718, 718 },
    { "amount", CONFIG_NODE_SCALAR, 875, 0, 719, 719, 719 },
    { "", CONFIG_NODE_RECORD, 874, 2, 720, 724, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 881, 0, 720, 720, 720 },
    { "arc", CONFIG_NODE_SCALAR, 881, 0, 721, 721, 721 },
    { "mods", CONFIG_NODE_SCALAR, 881, 0, 722, 722, 722 },
    { "ammo", CONFIG_NODE_SCALAR, 881, 0, 723, 723, 723 },
    { "amount", CONFIG_NODE_SCALAR, 881, 0, 724, 724, 724 },
    { "", CONFIG_NODE_RECORD, 874, 3, 725, 729, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 887, 0, 725, 725, 725 },
    { "arc", CONFIG_NODE_SCALAR, 887, 0, 726, 726, 726 },
    { "mods", CONFIG_NODE_SCALAR, 887, 0, 727, 727, 727 },
    { "ammo", CONFIG_NODE_SCALAR, 887, 0, 728, 728, 728 },
    { "amount", CONFIG_NODE_SCALAR, 887, 0, 729, 729, 729 },
    { "", CONFIG_NODE_RECORD, 874, 4, 730, 734, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 893, 0, 730, 730, 730 },
    { "arc", CONFIG_NODE_SCALAR, 893, 0, 731, 731, 731 },
    { "mods", CONFIG_NODE_SCALAR, 893, 0, 732, 732, 732 },
    { "ammo", CONFIG_NODE_SCALAR, 893, 0, 733, 733, 733 },
    { "amount", CONFIG_NODE_SCALAR, 893, 0, 734, 734, 734 },
    { "", CONFIG_NODE_RECORD, 874, 5, 735, 739, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 899, 0, 735, 735, 735 },
    { "arc", CONFIG_NODE_SCALAR, 899, 0, 736, 736, 736 },
    { "mods", CONFIG_NODE_SCALAR, 899, 0, 737, 737, 737 },
    { "ammo", CONFIG_NODE_SCALAR, 899, 0, 738, 738, 738 },
    { "amount", CONFIG_NODE_SCALAR, 899, 0, 739, 739, 739 },
    { "", CONFIG_NODE_RECORD, 874, 6, 740, 744, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 905, 0, 740, 740, 740 },
    { "arc", CONFIG_NODE_SCALAR, 905, 0, 741, 741, 741 },
    { "mods", CONFIG_NODE_SCALAR, 905, 0, 742, 742, 742 },
    { "ammo", CONFIG_NODE_SCALAR, 905, 0, 743, 743, 743 },
    { "amount", CONFIG_NODE_SCALAR, 905, 0, 744, 744, 744 },
    { "", CONFIG_NODE_RECORD, 874, 7, 745, 749, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 911, 0, 745, 745, 745 },
    { "arc", CONFIG_NODE_SCALAR, 911, 0, 746, 746, 746 },
    { "mods", CONFIG_NODE_SCALAR, 911, 0, 747, 747, 747 },
    { "ammo", CONFIG_NODE_SCALAR, 911, 0, 748, 748, 748 },
    { "amount", CONFIG_NODE_SCALAR, 911, 0, 749, 749, 749 },
    { "", CONFIG_NODE_RECORD, 874, 8, 750, 754, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 917, 0, 750, 750, 750 },
    { "arc", CONFIG_NODE_SCALAR, 917, 0, 751, 751, 751 },
    { "mods", CONFIG_NODE_SCALAR, 917, 0, 752, 752, 752 },
    { "ammo", CONFIG_NODE_SCALAR, 917, 0, 753, 753, 753 },
    { "amount", CONFIG_NODE_SCALAR, 917, 0, 754, 754, 754 },
    { "specials", CONFIG_NODE_SCALAR, 867, 0, 755, 755, 755 },
    { "", CONFIG_NODE_RECORD, 695, 4, 756, 802, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 924, 0, 756, 756, 756 },
    { "hull_size", CONFIG_NODE_SCALAR, 924, 0, 757, 757, 757 },
    { "drive", CONFIG_NODE_SCALAR, 924, 0, 758, 758, 758 },
    { "armor", CONFIG_NODE_SCALAR, 924, 0, 759, 759, 759 },
    { "shield", CONFIG_NODE_SCALAR, 924, 0, 760, 760, 760 },
    { "computer", CONFIG_NODE_SCALAR, 924, 0, 761, 761, 761 },
    { "weapons", CONFIG_NODE_ARRAY, 924, 0, 762, 801, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 931, 1, 762, 766, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 932, 0, 762, 762, 762 },
    { "arc", CONFIG_NODE_SCALAR, 932, 0, 763, 763, 763 },
    { "mods", CONFIG_NODE_SCALAR, 932, 0, 764, 764, 764 },
    { "ammo", CONFIG_NODE_SCALAR, 932, 0, 765, 765, 765 },
    { "amount", CONFIG_NODE_SCALAR, 932, 0, 766, 766, 766 },
    { "", CONFIG_NODE_RECORD, 931, 2, 767, 771, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 938, 0, 767, 767, 767 },
    { "arc", CONFIG_NODE_SCALAR, 938, 0, 768, 768, 768 },
    { "mods", CONFIG_NODE_SCALAR, 938, 0, 769, 769, 769 },
    { "ammo", CONFIG_NODE_SCALAR, 938, 0, 770, 770, 770 },
    { "amount", CONFIG_NODE_SCALAR, 938, 0, 771, 771, 771 },
    { "", CONFIG_NODE_RECORD, 931, 3, 772, 776, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 944, 0, 772, 772, 772 },
    { "arc", CONFIG_NODE_SCALAR, 944, 0, 773, 773, 773 },
    { "mods", CONFIG_NODE_SCALAR, 944, 0, 774, 774, 774 },
    { "ammo", CONFIG_NODE_SCALAR, 944, 0, 775, 775, 775 },
    { "amount", CONFIG_NODE_SCALAR, 944, 0, 776, 776, 776 },
    { "", CONFIG_NODE_RECORD, 931, 4, 777, 781, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 950, 0, 777, 777, 777 },
    { "arc", CONFIG_NODE_SCALAR, 950, 0, 778, 778, 778 },
    { "mods", CONFIG_NODE_SCALAR, 950, 0, 779, 779, 779 },
    { "ammo", CONFIG_NODE_SCALAR, 950, 0, 780, 780, 780 },
    { "amount", CONFIG_NODE_SCALAR, 950, 0, 781, 781, 781 },
    { "", CONFIG_NODE_RECORD, 931, 5, 782, 786, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 956, 0, 782, 782, 782 },
    { "arc", CONFIG_NODE_SCALAR, 956, 0, 783, 783, 783 },
    { "mods", CONFIG_NODE_SCALAR, 956, 0, 784, 784, 784 },
    { "ammo", CONFIG_NODE_SCALAR, 956, 0, 785, 785, 785 },
    { "amount", CONFIG_NODE_SCALAR, 956, 0, 786, 786, 786 },
    { "", CONFIG_NODE_RECORD, 931, 6, 787, 791, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 962, 0, 787, 787, 787 },
    { "arc", CONFIG_NODE_SCALAR, 962, 0, 788, 788, 788 },
    { "mods", CONFIG_NODE_SCALAR, 962, 0, 789, 789, 789 },
    { "ammo", CONFIG_NODE_SCALAR, 962, 0, 790, 790, 790 },
    { "amount", CONFIG_NODE_SCALAR, 962, 0, 791, 791, 791 },
    { "", CONFIG_NODE_RECORD, 931, 7, 792, 796, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 968, 0, 792, 792, 792 },
    { "arc", CONFIG_NODE_SCALAR, 968, 0, 793, 793, 793 },
    { "mods", CONFIG_NODE_SCALAR, 968, 0, 794, 794, 794 },
    { "ammo", CONFIG_NODE_SCALAR, 968, 0, 795, 795, 795 },
    { "amount", CONFIG_NODE_SCALAR, 968, 0, 796, 796, 796 },
    { "", CONFIG_NODE_RECORD, 931, 8, 797, 801, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 974, 0, 797, 797, 797 },
    { "arc", CONFIG_NODE_SCALAR, 974, 0, 798, 798, 798 },
    { "mods", CONFIG_NODE_SCALAR, 974, 0, 799, 799, 799 },
    { "ammo", CONFIG_NODE_SCALAR, 974, 0, 800, 800, 800 },
    { "amount", CONFIG_NODE_SCALAR, 974, 0, 801, 801, 801 },
    { "specials", CONFIG_NODE_SCALAR, 924, 0, 802, 802, 802 },
    { "", CONFIG_NODE_ARRAY, 694, 1, 803, 1037, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 981, 0, 803, 849, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 982, 0, 803, 803, 803 },
    { "hull_size", CONFIG_NODE_SCALAR, 982, 0, 804, 804, 804 },
    { "drive", CONFIG_NODE_SCALAR, 982, 0, 805, 805, 805 },
    { "armor", CONFIG_NODE_SCALAR, 982, 0, 806, 806, 806 },
    { "shield", CONFIG_NODE_SCALAR, 982, 0, 807, 807, 807 },
    { "computer", CONFIG_NODE_SCALAR, 982, 0, 808, 808, 808 },
    { "weapons", CONFIG_NODE_ARRAY, 982, 0, 809, 848, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 989, 1, 809, 813, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 990, 0, 809, 809, 809 },
    { "arc", CONFIG_NODE_SCALAR, 990, 0, 810, 810, 810 },
    { "mods", CONFIG_NODE_SCALAR, 990, 0, 811, 811, 811 },
    { "ammo", CONFIG_NODE_SCALAR, 990, 0, 812, 812, 812 },
    { "amount", CONFIG_NODE_SCALAR, 990, 0, 813, 813, 813 },
    { "", CONFIG_NODE_RECORD, 989, 2, 814, 818, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 996, 0, 814, 814, 814 },
    { "arc", CONFIG_NODE_SCALAR, 996, 0, 815, 815, 815 },
    { "mods", CONFIG_NODE_SCALAR, 996, 0, 816, 816, 816 },
    { "ammo", CONFIG_NODE_SCALAR, 996, 0, 817, 817, 817 },
    { "amount", CONFIG_NODE_SCALAR, 996, 0, 818, 818, 818 },
    { "", CONFIG_NODE_RECORD, 989, 3, 819, 823, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1002, 0, 819, 819, 819 },
    { "arc", CONFIG_NODE_SCALAR, 1002, 0, 820, 820, 820 },
    { "mods", CONFIG_NODE_SCALAR, 1002, 0, 821, 821, 821 },
    { "ammo", CONFIG_NODE_SCALAR, 1002, 0, 822, 822, 822 },
    { "amount", CONFIG_NODE_SCALAR, 1002, 0, 823, 823, 823 },
    { "", CONFIG_NODE_RECORD, 989, 4, 824, 828, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1008, 0, 824, 824, 824 },
    { "arc", CONFIG_NODE_SCALAR, 1008, 0, 825, 825, 825 },
    { "mods", CONFIG_NODE_SCALAR, 1008, 0, 826, 826, 826 },
    { "ammo", CONFIG_NODE_SCALAR, 1008, 0, 827, 827, 827 },
    { "amount", CONFIG_NODE_SCALAR, 1008, 0, 828, 828, 828 },
    { "", CONFIG_NODE_RECORD, 989, 5, 829, 833, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1014, 0, 829, 829, 829 },
    { "arc", CONFIG_NODE_SCALAR, 1014, 0, 830, 830, 830 },
    { "mods", CONFIG_NODE_SCALAR, 1014, 0, 831, 831, 831 },
    { "ammo", CONFIG_NODE_SCALAR, 1014, 0, 832, 832, 832 },
    { "amount", CONFIG_NODE_SCALAR, 1014, 0, 833, 833, 833 },
    { "", CONFIG_NODE_RECORD, 989, 6, 834, 838, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1020, 0, 834, 834, 834 },
    { "arc", CONFIG_NODE_SCALAR, 1020, 0, 835, 835, 835 },
    { "mods", CONFIG_NODE_SCALAR, 1020, 0, 836, 836, 836 },
    { "ammo", CONFIG_NODE_SCALAR, 1020, 0, 837, 837, 837 },
    { "amount", CONFIG_NODE_SCALAR, 1020, 0, 838, 838, 838 },
    { "", CONFIG_NODE_RECORD, 989, 7, 839, 843, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1026, 0, 839, 839, 839 },
    { "arc", CONFIG_NODE_SCALAR, 1026, 0, 840, 840, 840 },
    { "mods", CONFIG_NODE_SCALAR, 1026, 0, 841, 841, 841 },
    { "ammo", CONFIG_NODE_SCALAR, 1026, 0, 842, 842, 842 },
    { "amount", CONFIG_NODE_SCALAR, 1026, 0, 843, 843, 843 },
    { "", CONFIG_NODE_RECORD, 989, 8, 844, 848, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1032, 0, 844, 844, 844 },
    { "arc", CONFIG_NODE_SCALAR, 1032, 0, 845, 845, 845 },
    { "mods", CONFIG_NODE_SCALAR, 1032, 0, 846, 846, 846 },
    { "ammo", CONFIG_NODE_SCALAR, 1032, 0, 847, 847, 847 },
    { "amount", CONFIG_NODE_SCALAR, 1032, 0, 848, 848, 848 },
    { "specials", CONFIG_NODE_SCALAR, 982, 0, 849, 849, 849 },
    { "", CONFIG_NODE_RECORD, 981, 1, 850, 896, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1039, 0, 850, 850, 850 },
    { "hull_size", CONFIG_NODE_SCALAR, 1039, 0, 851, 851, 851 },
    { "drive", CONFIG_NODE_SCALAR, 1039, 0, 852, 852, 852 },
    { "armor", CONFIG_NODE_SCALAR, 1039, 0, 853, 853, 853 },
    { "shield", CONFIG_NODE_SCALAR, 1039, 0, 854, 854, 854 },
    { "computer", CONFIG_NODE_SCALAR, 1039, 0, 855, 855, 855 },
    { "weapons", CONFIG_NODE_ARRAY, 1039, 0, 856, 895, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1046, 1, 856, 860, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1047, 0, 856, 856, 856 },
    { "arc", CONFIG_NODE_SCALAR, 1047, 0, 857, 857, 857 },
    { "mods", CONFIG_NODE_SCALAR, 1047, 0, 858, 858, 858 },
    { "ammo", CONFIG_NODE_SCALAR, 1047, 0, 859, 859, 859 },
    { "amount", CONFIG_NODE_SCALAR, 1047, 0, 860, 860, 860 },
    { "", CONFIG_NODE_RECORD, 1046, 2, 861, 865, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1053, 0, 861, 861, 861 },
    { "arc", CONFIG_NODE_SCALAR, 1053, 0, 862, 862, 862 },
    { "mods", CONFIG_NODE_SCALAR, 1053, 0, 863, 863, 863 },
    { "ammo", CONFIG_NODE_SCALAR, 1053, 0, 864, 864, 864 },
    { "amount", CONFIG_NODE_SCALAR, 1053, 0, 865, 865, 865 },
    { "", CONFIG_NODE_RECORD, 1046, 3, 866, 870, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1059, 0, 866, 866, 866 },
    { "arc", CONFIG_NODE_SCALAR, 1059, 0, 867, 867, 867 },
    { "mods", CONFIG_NODE_SCALAR, 1059, 0, 868, 868, 868 },
    { "ammo", CONFIG_NODE_SCALAR, 1059, 0, 869, 869, 869 },
    { "amount", CONFIG_NODE_SCALAR, 1059, 0, 870, 870, 870 },
    { "", CONFIG_NODE_RECORD, 1046, 4, 871, 875, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1065, 0, 871, 871, 871 },
    { "arc", CONFIG_NODE_SCALAR, 1065, 0, 872, 872, 872 },
    { "mods", CONFIG_NODE_SCALAR, 1065, 0, 873, 873, 873 },
    { "ammo", CONFIG_NODE_SCALAR, 1065, 0, 874, 874, 874 },
    { "amount", CONFIG_NODE_SCALAR, 1065, 0, 875, 875, 875 },
    { "", CONFIG_NODE_RECORD, 1046, 5, 876, 880, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1071, 0, 876, 876, 876 },
    { "arc", CONFIG_NODE_SCALAR, 1071, 0, 877, 877, 877 },
    { "mods", CONFIG_NODE_SCALAR, 1071, 0, 878, 878, 878 },
    { "ammo", CONFIG_NODE_SCALAR, 1071, 0, 879, 879, 879 },
    { "amount", CONFIG_NODE_SCALAR, 1071, 0, 880, 880, 880 },
    { "", CONFIG_NODE_RECORD, 1046, 6, 881, 885, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1077, 0, 881, 881, 881 },
    { "arc", CONFIG_NODE_SCALAR, 1077, 0, 882, 882, 882 },
    { "mods", CONFIG_NODE_SCALAR, 1077, 0, 883, 883, 883 },
    { "ammo", CONFIG_NODE_SCALAR, 1077, 0, 884, 884, 884 },
    { "amount", CONFIG_NODE_SCALAR, 1077, 0, 885, 885, 885 },
    { "", CONFIG_NODE_RECORD, 1046, 7, 886, 890, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1083, 0, 886, 886, 886 },
    { "arc", CONFIG_NODE_SCALAR, 1083, 0, 887, 887, 887 },
    { "mods", CONFIG_NODE_SCALAR, 1083, 0, 888, 888, 888 },
    { "ammo", CONFIG_NODE_SCALAR, 1083, 0, 889, 889, 889 },
    { "amount", CONFIG_NODE_SCALAR, 1083, 0, 890, 890, 890 },
    { "", CONFIG_NODE_RECORD, 1046, 8, 891, 895, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1089, 0, 891, 891, 891 },
    { "arc", CONFIG_NODE_SCALAR, 1089, 0, 892, 892, 892 },
    { "mods", CONFIG_NODE_SCALAR, 1089, 0, 893, 893, 893 },
    { "ammo", CONFIG_NODE_SCALAR, 1089, 0, 894, 894, 894 },
    { "amount", CONFIG_NODE_SCALAR, 1089, 0, 895, 895, 895 },
    { "specials", CONFIG_NODE_SCALAR, 1039, 0, 896, 896, 896 },
    { "", CONFIG_NODE_RECORD, 981, 2, 897, 943, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1096, 0, 897, 897, 897 },
    { "hull_size", CONFIG_NODE_SCALAR, 1096, 0, 898, 898, 898 },
    { "drive", CONFIG_NODE_SCALAR, 1096, 0, 899, 899, 899 },
    { "armor", CONFIG_NODE_SCALAR, 1096, 0, 900, 900, 900 },
    { "shield", CONFIG_NODE_SCALAR, 1096, 0, 901, 901, 901 },
    { "computer", CONFIG_NODE_SCALAR, 1096, 0, 902, 902, 902 },
    { "weapons", CONFIG_NODE_ARRAY, 1096, 0, 903, 942, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1103, 1, 903, 907, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1104, 0, 903, 903, 903 },
    { "arc", CONFIG_NODE_SCALAR, 1104, 0, 904, 904, 904 },
    { "mods", CONFIG_NODE_SCALAR, 1104, 0, 905, 905, 905 },
    { "ammo", CONFIG_NODE_SCALAR, 1104, 0, 906, 906, 906 },
    { "amount", CONFIG_NODE_SCALAR, 1104, 0, 907, 907, 907 },
    { "", CONFIG_NODE_RECORD, 1103, 2, 908, 912, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1110, 0, 908, 908, 908 },
    { "arc", CONFIG_NODE_SCALAR, 1110, 0, 909, 909, 909 },
    { "mods", CONFIG_NODE_SCALAR, 1110, 0, 910, 910, 910 },
    { "ammo", CONFIG_NODE_SCALAR, 1110, 0, 911, 911, 911 },
    { "amount", CONFIG_NODE_SCALAR, 1110, 0, 912, 912, 912 },
    { "", CONFIG_NODE_RECORD, 1103, 3, 913, 917, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1116, 0, 913, 913, 913 },
    { "arc", CONFIG_NODE_SCALAR, 1116, 0, 914, 914, 914 },
    { "mods", CONFIG_NODE_SCALAR, 1116, 0, 915, 915, 915 },
    { "ammo", CONFIG_NODE_SCALAR, 1116, 0, 916, 916, 916 },
    { "amount", CONFIG_NODE_SCALAR, 1116, 0, 917, 917, 917 },
    { "", CONFIG_NODE_RECORD, 1103, 4, 918, 922, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1122, 0, 918, 918, 918 },
    { "arc", CONFIG_NODE_SCALAR, 1122, 0, 919, 919, 919 },
    { "mods", CONFIG_NODE_SCALAR, 1122, 0, 920, 920, 920 },
    { "ammo", CONFIG_NODE_SCALAR, 1122, 0, 921, 921, 921 },
    { "amount", CONFIG_NODE_SCALAR, 1122, 0, 922, 922, 922 },
    { "", CONFIG_NODE_RECORD, 1103, 5, 923, 927, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1128, 0, 923, 923, 923 },
    { "arc", CONFIG_NODE_SCALAR, 1128, 0, 924, 924, 924 },
    { "mods", CONFIG_NODE_SCALAR, 1128, 0, 925, 925, 925 },
    { "ammo", CONFIG_NODE_SCALAR, 1128, 0, 926, 926, 926 },
    { "amount", CONFIG_NODE_SCALAR, 1128, 0, 927, 927, 927 },
    { "", CONFIG_NODE_RECORD, 1103, 6, 928, 932, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1134, 0, 928, 928, 928 },
    { "arc", CONFIG_NODE_SCALAR, 1134, 0, 929, 929, 929 },
    { "mods", CONFIG_NODE_SCALAR, 1134, 0, 930, 930, 930 },
    { "ammo", CONFIG_NODE_SCALAR, 1134, 0, 931, 931, 931 },
    { "amount", CONFIG_NODE_SCALAR, 1134, 0, 932, 932, 932 },
    { "", CONFIG_NODE_RECORD, 1103, 7, 933, 937, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1140, 0, 933, 933, 933 },
    { "arc", CONFIG_NODE_SCALAR, 1140, 0, 934, 934, 934 },
    { "mods", CONFIG_NODE_SCALAR, 1140, 0, 935, 935, 935 },
    { "ammo", CONFIG_NODE_SCALAR, 1140, 0, 936, 936, 936 },
    { "amount", CONFIG_NODE_SCALAR, 1140, 0, 937, 937, 937 },
    { "", CONFIG_NODE_RECORD, 1103, 8, 938, 942, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1146, 0, 938, 938, 938 },
    { "arc", CONFIG_NODE_SCALAR, 1146, 0, 939, 939, 939 },
    { "mods", CONFIG_NODE_SCALAR, 1146, 0, 940, 940, 940 },
    { "ammo", CONFIG_NODE_SCALAR, 1146, 0, 941, 941, 941 },
    { "amount", CONFIG_NODE_SCALAR, 1146, 0, 942, 942, 942 },
    { "specials", CONFIG_NODE_SCALAR, 1096, 0, 943, 943, 943 },
    { "", CONFIG_NODE_RECORD, 981, 3, 944, 990, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1153, 0, 944, 944, 944 },
    { "hull_size", CONFIG_NODE_SCALAR, 1153, 0, 945, 945, 945 },
    { "drive", CONFIG_NODE_SCALAR, 1153, 0, 946, 946, 946 },
    { "armor", CONFIG_NODE_SCALAR, 1153, 0, 947, 947, 947 },
    { "shield", CONFIG_NODE_SCALAR, 1153, 0, 948, 948, 948 },
    { "computer", CONFIG_NODE_SCALAR, 1153, 0, 949, 949, 949 },
    { "weapons", CONFIG_NODE_ARRAY, 1153, 0, 950, 989, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1160, 1, 950, 954, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1161, 0, 950, 950, 950 },
    { "arc", CONFIG_NODE_SCALAR, 1161, 0, 951, 951, 951 },
    { "mods", CONFIG_NODE_SCALAR, 1161, 0, 952, 952, 952 },
    { "ammo", CONFIG_NODE_SCALAR, 1161, 0, 953, 953, 953 },
    { "amount", CONFIG_NODE_SCALAR, 1161, 0, 954, 954, 954 },
    { "", CONFIG_NODE_RECORD, 1160, 2, 955, 959, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1167, 0, 955, 955, 955 },
    { "arc", CONFIG_NODE_SCALAR, 1167, 0, 956, 956, 956 },
    { "mods", CONFIG_NODE_SCALAR, 1167, 0, 957, 957, 957 },
    { "ammo", CONFIG_NODE_SCALAR, 1167, 0, 958, 958, 958 },
    { "amount", CONFIG_NODE_SCALAR, 1167, 0, 959, 959, 959 },
    { "", CONFIG_NODE_RECORD, 1160, 3, 960, 964, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1173, 0, 960, 960, 960 },
    { "arc", CONFIG_NODE_SCALAR, 1173, 0, 961, 961, 961 },
    { "mods", CONFIG_NODE_SCALAR, 1173, 0, 962, 962, 962 },
    { "ammo", CONFIG_NODE_SCALAR, 1173, 0, 963, 963, 963 },
    { "amount", CONFIG_NODE_SCALAR, 1173, 0, 964, 964, 964 },
    { "", CONFIG_NODE_RECORD, 1160, 4, 965, 969, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1179, 0, 965, 965, 965 },
    { "arc", CONFIG_NODE_SCALAR, 1179, 0, 966, 966, 966 },
    { "mods", CONFIG_NODE_SCALAR, 1179, 0, 967, 967, 967 },
    { "ammo", CONFIG_NODE_SCALAR, 1179, 0, 968, 968, 968 },
    { "amount", CONFIG_NODE_SCALAR, 1179, 0, 969, 969, 969 },
    { "", CONFIG_NODE_RECORD, 1160, 5, 970, 974, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1185, 0, 970, 970, 970 },
    { "arc", CONFIG_NODE_SCALAR, 1185, 0, 971, 971, 971 },
    { "mods", CONFIG_NODE_SCALAR, 1185, 0, 972, 972, 972 },
    { "ammo", CONFIG_NODE_SCALAR, 1185, 0, 973, 973, 973 },
    { "amount", CONFIG_NODE_SCALAR, 1185, 0, 974, 974, 974 },
    { "", CONFIG_NODE_RECORD, 1160, 6, 975, 979, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1191, 0, 975, 975, 975 },
    { "arc", CONFIG_NODE_SCALAR, 1191, 0, 976, 976, 976 },
    { "mods", CONFIG_NODE_SCALAR, 1191, 0, 977, 977, 977 },
    { "ammo", CONFIG_NODE_SCALAR, 1191, 0, 978, 978, 978 },
    { "amount", CONFIG_NODE_SCALAR, 1191, 0, 979, 979, 979 },
    { "", CONFIG_NODE_RECORD, 1160, 7, 980, 984, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1197, 0, 980, 980, 980 },
    { "arc", CONFIG_NODE_SCALAR, 1197, 0, 981, 981, 981 },
    { "mods", CONFIG_NODE_SCALAR, 1197, 0, 982, 982, 982 },
    { "ammo", CONFIG_NODE_SCALAR, 1197, 0, 983, 983, 983 },
    { "amount", CONFIG_NODE_SCALAR, 1197, 0, 984, 984, 984 },
    { "", CONFIG_NODE_RECORD, 1160, 8, 985, 989, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1203, 0, 985, 985, 985 },
    { "arc", CONFIG_NODE_SCALAR, 1203, 0, 986, 986, 986 },
    { "mods", CONFIG_NODE_SCALAR, 1203, 0, 987, 987, 987 },
    { "ammo", CONFIG_NODE_SCALAR, 1203, 0, 988, 988, 988 },
    { "amount", CONFIG_NODE_SCALAR, 1203, 0, 989, 989, 989 },
    { "specials", CONFIG_NODE_SCALAR, 1153, 0, 990, 990, 990 },
    { "", CONFIG_NODE_RECORD, 981, 4, 991, 1037, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1210, 0, 991, 991, 991 },
    { "hull_size", CONFIG_NODE_SCALAR, 1210, 0, 992, 992, 992 },
    { "drive", CONFIG_NODE_SCALAR, 1210, 0, 993, 993, 993 },
    { "armor", CONFIG_NODE_SCALAR, 1210, 0, 994, 994, 994 },
    { "shield", CONFIG_NODE_SCALAR, 1210, 0, 995, 995, 995 },
    { "computer", CONFIG_NODE_SCALAR, 1210, 0, 996, 996, 996 },
    { "weapons", CONFIG_NODE_ARRAY, 1210, 0, 997, 1036, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1217, 1, 997, 1001, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1218, 0, 997, 997, 997 },
    { "arc", CONFIG_NODE_SCALAR, 1218, 0, 998, 998, 998 },
    { "mods", CONFIG_NODE_SCALAR, 1218, 0, 999, 999, 999 },
    { "ammo", CONFIG_NODE_SCALAR, 1218, 0, 1000, 1000, 1000 },
    { "amount", CONFIG_NODE_SCALAR, 1218, 0, 1001, 1001, 1001 },
    { "", CONFIG_NODE_RECORD, 1217, 2, 1002, 1006, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1224, 0, 1002, 1002, 1002 },
    { "arc", CONFIG_NODE_SCALAR, 1224, 0, 1003, 1003, 1003 },
    { "mods", CONFIG_NODE_SCALAR, 1224, 0, 1004, 1004, 1004 },
    { "ammo", CONFIG_NODE_SCALAR, 1224, 0, 1005, 1005, 1005 },
    { "amount", CONFIG_NODE_SCALAR, 1224, 0, 1006, 1006, 1006 },
    { "", CONFIG_NODE_RECORD, 1217, 3, 1007, 1011, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1230, 0, 1007, 1007, 1007 },
    { "arc", CONFIG_NODE_SCALAR, 1230, 0, 1008, 1008, 1008 },
    { "mods", CONFIG_NODE_SCALAR, 1230, 0, 1009, 1009, 1009 },
    { "ammo", CONFIG_NODE_SCALAR, 1230, 0, 1010, 1010, 1010 },
    { "amount", CONFIG_NODE_SCALAR, 1230, 0, 1011, 1011, 1011 },
    { "", CONFIG_NODE_RECORD, 1217, 4, 1012, 1016, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1236, 0, 1012, 1012, 1012 },
    { "arc", CONFIG_NODE_SCALAR, 1236, 0, 1013, 1013, 1013 },
    { "mods", CONFIG_NODE_SCALAR, 1236, 0, 1014, 1014, 1014 },
    { "ammo", CONFIG_NODE_SCALAR, 1236, 0, 1015, 1015, 1015 },
    { "amount", CONFIG_NODE_SCALAR, 1236, 0, 1016, 1016, 1016 },
    { "", CONFIG_NODE_RECORD, 1217, 5, 1017, 1021, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1242, 0, 1017, 1017, 1017 },
    { "arc", CONFIG_NODE_SCALAR, 1242, 0, 1018, 1018, 1018 },
    { "mods", CONFIG_NODE_SCALAR, 1242, 0, 1019, 1019, 1019 },
    { "ammo", CONFIG_NODE_SCALAR, 1242, 0, 1020, 1020, 1020 },
    { "amount", CONFIG_NODE_SCALAR, 1242, 0, 1021, 1021, 1021 },
    { "", CONFIG_NODE_RECORD, 1217, 6, 1022, 1026, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1248, 0, 1022, 1022, 1022 },
    { "arc", CONFIG_NODE_SCALAR, 1248, 0, 1023, 1023, 1023 },
    { "mods", CONFIG_NODE_SCALAR, 1248, 0, 1024, 1024, 1024 },
    { "ammo", CONFIG_NODE_SCALAR, 1248, 0, 1025, 1025, 1025 },
    { "amount", CONFIG_NODE_SCALAR, 1248, 0, 1026, 1026, 1026 },
    { "", CONFIG_NODE_RECORD, 1217, 7, 1027, 1031, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1254, 0, 1027, 1027, 1027 },
    { "arc", CONFIG_NODE_SCALAR, 1254, 0, 1028, 1028, 1028 },
    { "mods", CONFIG_NODE_SCALAR, 1254, 0, 1029, 1029, 1029 },
    { "ammo", CONFIG_NODE_SCALAR, 1254, 0, 1030, 1030, 1030 },
    { "amount", CONFIG_NODE_SCALAR, 1254, 0, 1031, 1031, 1031 },
    { "", CONFIG_NODE_RECORD, 1217, 8, 1032, 1036, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1260, 0, 1032, 1032, 1032 },
    { "arc", CONFIG_NODE_SCALAR, 1260, 0, 1033, 1033, 1033 },
    { "mods", CONFIG_NODE_SCALAR, 1260, 0, 1034, 1034, 1034 },
    { "ammo", CONFIG_NODE_SCALAR, 1260, 0, 1035, 1035, 1035 },
    { "amount", CONFIG_NODE_SCALAR, 1260, 0, 1036, 1036, 1036 },
    { "specials", CONFIG_NODE_SCALAR, 1210, 0, 1037, 1037, 1037 },
    { "", CONFIG_NODE_ARRAY, 694, 2, 1038, 1272, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1267, 0, 1038, 1084, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1268, 0, 1038, 1038, 1038 },
    { "hull_size", CONFIG_NODE_SCALAR, 1268, 0, 1039, 1039, 1039 },
    { "drive", CONFIG_NODE_SCALAR, 1268, 0, 1040, 1040, 1040 },
    { "armor", CONFIG_NODE_SCALAR, 1268, 0, 1041, 1041, 1041 },
    { "shield", CONFIG_NODE_SCALAR, 1268, 0, 1042, 1042, 1042 },
    { "computer", CONFIG_NODE_SCALAR, 1268, 0, 1043, 1043, 1043 },
    { "weapons", CONFIG_NODE_ARRAY, 1268, 0, 1044, 1083, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1275, 1, 1044, 1048, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1276, 0, 1044, 1044, 1044 },
    { "arc", CONFIG_NODE_SCALAR, 1276, 0, 1045, 1045, 1045 },
    { "mods", CONFIG_NODE_SCALAR, 1276, 0, 1046, 1046, 1046 },
    { "ammo", CONFIG_NODE_SCALAR, 1276, 0, 1047, 1047, 1047 },
    { "amount", CONFIG_NODE_SCALAR, 1276, 0, 1048, 1048, 1048 },
    { "", CONFIG_NODE_RECORD, 1275, 2, 1049, 1053, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1282, 0, 1049, 1049, 1049 },
    { "arc", CONFIG_NODE_SCALAR, 1282, 0, 1050, 1050, 1050 },
    { "mods", CONFIG_NODE_SCALAR, 1282, 0, 1051, 1051, 1051 },
    { "ammo", CONFIG_NODE_SCALAR, 1282, 0, 1052, 1052, 1052 },
    { "amount", CONFIG_NODE_SCALAR, 1282, 0, 1053, 1053, 1053 },
    { "", CONFIG_NODE_RECORD, 1275, 3, 1054, 1058, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1288, 0, 1054, 1054, 1054 },
    { "arc", CONFIG_NODE_SCALAR, 1288, 0, 1055, 1055, 1055 },
    { "mods", CONFIG_NODE_SCALAR, 1288, 0, 1056, 1056, 1056 },
    { "ammo", CONFIG_NODE_SCALAR, 1288, 0, 1057, 1057, 1057 },
    { "amount", CONFIG_NODE_SCALAR, 1288, 0, 1058, 1058, 1058 },
    { "", CONFIG_NODE_RECORD, 1275, 4, 1059, 1063, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1294, 0, 1059, 1059, 1059 },
    { "arc", CONFIG_NODE_SCALAR, 1294, 0, 1060, 1060, 1060 },
    { "mods", CONFIG_NODE_SCALAR, 1294, 0, 1061, 1061, 1061 },
    { "ammo", CONFIG_NODE_SCALAR, 1294, 0, 1062, 1062, 1062 },
    { "amount", CONFIG_NODE_SCALAR, 1294, 0, 1063, 1063, 1063 },
    { "", CONFIG_NODE_RECORD, 1275, 5, 1064, 1068, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1300, 0, 1064, 1064, 1064 },
    { "arc", CONFIG_NODE_SCALAR, 1300, 0, 1065, 1065, 1065 },
    { "mods", CONFIG_NODE_SCALAR, 1300, 0, 1066, 1066, 1066 },
    { "ammo", CONFIG_NODE_SCALAR, 1300, 0, 1067, 1067, 1067 },
    { "amount", CONFIG_NODE_SCALAR, 1300, 0, 1068, 1068, 1068 },
    { "", CONFIG_NODE_RECORD, 1275, 6, 1069, 1073, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1306, 0, 1069, 1069, 1069 },
    { "arc", CONFIG_NODE_SCALAR, 1306, 0, 1070, 1070, 1070 },
    { "mods", CONFIG_NODE_SCALAR, 1306, 0, 1071, 1071, 1071 },
    { "ammo", CONFIG_NODE_SCALAR, 1306, 0, 1072, 1072, 1072 },
    { "amount", CONFIG_NODE_SCALAR, 1306, 0, 1073, 1073, 1073 },
    { "", CONFIG_NODE_RECORD, 1275, 7, 1074, 1078, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1312, 0, 1074, 1074, 1074 },
    { "arc", CONFIG_NODE_SCALAR, 1312, 0, 1075, 1075, 1075 },
    { "mods", CONFIG_NODE_SCALAR, 1312, 0, 1076, 1076, 1076 },
    { "ammo", CONFIG_NODE_SCALAR, 1312, 0, 1077, 1077, 1077 },
    { "amount", CONFIG_NODE_SCALAR, 1312, 0, 1078, 1078, 1078 },
    { "", CONFIG_NODE_RECORD, 1275, 8, 1079, 1083, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1318, 0, 1079, 1079, 1079 },
    { "arc", CONFIG_NODE_SCALAR, 1318, 0, 1080, 1080, 1080 },
    { "mods", CONFIG_NODE_SCALAR, 1318, 0, 1081, 1081, 1081 },
    { "ammo", CONFIG_NODE_SCALAR, 1318, 0, 1082, 1082, 1082 },
    { "amount", CONFIG_NODE_SCALAR, 1318, 0, 1083, 1083, 1083 },
    { "specials", CONFIG_NODE_SCALAR, 1268, 0, 1084, 1084, 1084 },
    { "", CONFIG_NODE_RECORD, 1267, 1, 1085, 1131, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1325, 0, 1085, 1085, 1085 },
    { "hull_size", CONFIG_NODE_SCALAR, 1325, 0, 1086, 1086, 1086 },
    { "drive", CONFIG_NODE_SCALAR, 1325, 0, 1087, 1087, 1087 },
    { "armor", CONFIG_NODE_SCALAR, 1325, 0, 1088, 1088, 1088 },
    { "shield", CONFIG_NODE_SCALAR, 1325, 0, 1089, 1089, 1089 },
    { "computer", CONFIG_NODE_SCALAR, 1325, 0, 1090, 1090, 1090 },
    { "weapons", CONFIG_NODE_ARRAY, 1325, 0, 1091, 1130, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1332, 1, 1091, 1095, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1333, 0, 1091, 1091, 1091 },
    { "arc", CONFIG_NODE_SCALAR, 1333, 0, 1092, 1092, 1092 },
    { "mods", CONFIG_NODE_SCALAR, 1333, 0, 1093, 1093, 1093 },
    { "ammo", CONFIG_NODE_SCALAR, 1333, 0, 1094, 1094, 1094 },
    { "amount", CONFIG_NODE_SCALAR, 1333, 0, 1095, 1095, 1095 },
    { "", CONFIG_NODE_RECORD, 1332, 2, 1096, 1100, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1339, 0, 1096, 1096, 1096 },
    { "arc", CONFIG_NODE_SCALAR, 1339, 0, 1097, 1097, 1097 },
    { "mods", CONFIG_NODE_SCALAR, 1339, 0, 1098, 1098, 1098 },
    { "ammo", CONFIG_NODE_SCALAR, 1339, 0, 1099, 1099, 1099 },
    { "amount", CONFIG_NODE_SCALAR, 1339, 0, 1100, 1100, 1100 },
    { "", CONFIG_NODE_RECORD, 1332, 3, 1101, 1105, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1345, 0, 1101, 1101, 1101 },
    { "arc", CONFIG_NODE_SCALAR, 1345, 0, 1102, 1102, 1102 },
    { "mods", CONFIG_NODE_SCALAR, 1345, 0, 1103, 1103, 1103 },
    { "ammo", CONFIG_NODE_SCALAR, 1345, 0, 1104, 1104, 1104 },
    { "amount", CONFIG_NODE_SCALAR, 1345, 0, 1105, 1105, 1105 },
    { "", CONFIG_NODE_RECORD, 1332, 4, 1106, 1110, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1351, 0, 1106, 1106, 1106 },
    { "arc", CONFIG_NODE_SCALAR, 1351, 0, 1107, 1107, 1107 },
    { "mods", CONFIG_NODE_SCALAR, 1351, 0, 1108, 1108, 1108 },
    { "ammo", CONFIG_NODE_SCALAR, 1351, 0, 1109, 1109, 1109 },
    { "amount", CONFIG_NODE_SCALAR, 1351, 0, 1110, 1110, 1110 },
    { "", CONFIG_NODE_RECORD, 1332, 5, 1111, 1115, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1357, 0, 1111, 1111, 1111 },
    { "arc", CONFIG_NODE_SCALAR, 1357, 0, 1112, 1112, 1112 },
    { "mods", CONFIG_NODE_SCALAR, 1357, 0, 1113, 1113, 1113 },
    { "ammo", CONFIG_NODE_SCALAR, 1357, 0, 1114, 1114, 1114 },
    { "amount", CONFIG_NODE_SCALAR, 1357, 0, 1115, 1115, 1115 },
    { "", CONFIG_NODE_RECORD, 1332, 6, 1116, 1120, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1363, 0, 1116, 1116, 1116 },
    { "arc", CONFIG_NODE_SCALAR, 1363, 0, 1117, 1117, 1117 },
    { "mods", CONFIG_NODE_SCALAR, 1363, 0, 1118, 1118, 1118 },
    { "ammo", CONFIG_NODE_SCALAR, 1363, 0, 1119, 1119, 1119 },
    { "amount", CONFIG_NODE_SCALAR, 1363, 0, 1120, 1120, 1120 },
    { "", CONFIG_NODE_RECORD, 1332, 7, 1121, 1125, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1369, 0, 1121, 1121, 1121 },
    { "arc", CONFIG_NODE_SCALAR, 1369, 0, 1122, 1122, 1122 },
    { "mods", CONFIG_NODE_SCALAR, 1369, 0, 1123, 1123, 1123 },
    { "ammo", CONFIG_NODE_SCALAR, 1369, 0, 1124, 1124, 1124 },
    { "amount", CONFIG_NODE_SCALAR, 1369, 0, 1125, 1125, 1125 },
    { "", CONFIG_NODE_RECORD, 1332, 8, 1126, 1130, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1375, 0, 1126, 1126, 1126 },
    { "arc", CONFIG_NODE_SCALAR, 1375, 0, 1127, 1127, 1127 },
    { "mods", CONFIG_NODE_SCALAR, 1375, 0, 1128, 1128, 1128 },
    { "ammo", CONFIG_NODE_SCALAR, 1375, 0, 1129, 1129, 1129 },
    { "amount", CONFIG_NODE_SCALAR, 1375, 0, 1130, 1130, 1130 },
    { "specials", CONFIG_NODE_SCALAR, 1325, 0, 1131, 1131, 1131 },
    { "", CONFIG_NODE_RECORD, 1267, 2, 1132, 1178, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1382, 0, 1132, 1132, 1132 },
    { "hull_size", CONFIG_NODE_SCALAR, 1382, 0, 1133, 1133, 1133 },
    { "drive", CONFIG_NODE_SCALAR, 1382, 0, 1134, 1134, 1134 },
    { "armor", CONFIG_NODE_SCALAR, 1382, 0, 1135, 1135, 1135 },
    { "shield", CONFIG_NODE_SCALAR, 1382, 0, 1136, 1136, 1136 },
    { "computer", CONFIG_NODE_SCALAR, 1382, 0, 1137, 1137, 1137 },
    { "weapons", CONFIG_NODE_ARRAY, 1382, 0, 1138, 1177, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1389, 1, 1138, 1142, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1390, 0, 1138, 1138, 1138 },
    { "arc", CONFIG_NODE_SCALAR, 1390, 0, 1139, 1139, 1139 },
    { "mods", CONFIG_NODE_SCALAR, 1390, 0, 1140, 1140, 1140 },
    { "ammo", CONFIG_NODE_SCALAR, 1390, 0, 1141, 1141, 1141 },
    { "amount", CONFIG_NODE_SCALAR, 1390, 0, 1142, 1142, 1142 },
    { "", CONFIG_NODE_RECORD, 1389, 2, 1143, 1147, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1396, 0, 1143, 1143, 1143 },
    { "arc", CONFIG_NODE_SCALAR, 1396, 0, 1144, 1144, 1144 },
    { "mods", CONFIG_NODE_SCALAR, 1396, 0, 1145, 1145, 1145 },
    { "ammo", CONFIG_NODE_SCALAR, 1396, 0, 1146, 1146, 1146 },
    { "amount", CONFIG_NODE_SCALAR, 1396, 0, 1147, 1147, 1147 },
    { "", CONFIG_NODE_RECORD, 1389, 3, 1148, 1152, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1402, 0, 1148, 1148, 1148 },
    { "arc", CONFIG_NODE_SCALAR, 1402, 0, 1149, 1149, 1149 },
    { "mods", CONFIG_NODE_SCALAR, 1402, 0, 1150, 1150, 1150 },
    { "ammo", CONFIG_NODE_SCALAR, 1402, 0, 1151, 1151, 1151 },
    { "amount", CONFIG_NODE_SCALAR, 1402, 0, 1152, 1152, 1152 },
    { "", CONFIG_NODE_RECORD, 1389, 4, 1153, 1157, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1408, 0, 1153, 1153, 1153 },
    { "arc", CONFIG_NODE_SCALAR, 1408, 0, 1154, 1154, 1154 },
    { "mods", CONFIG_NODE_SCALAR, 1408, 0, 1155, 1155, 1155 },
    { "ammo", CONFIG_NODE_SCALAR, 1408, 0, 1156, 1156, 1156 },
    { "amount", CONFIG_NODE_SCALAR, 1408, 0, 1157, 1157, 1157 },
    { "", CONFIG_NODE_RECORD, 1389, 5, 1158, 1162, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1414, 0, 1158, 1158, 1158 },
    { "arc", CONFIG_NODE_SCALAR, 1414, 0, 1159, 1159, 1159 },
    { "mods", CONFIG_NODE_SCALAR, 1414, 0, 1160, 1160, 1160 },
    { "ammo", CONFIG_NODE_SCALAR, 1414, 0, 1161, 1161, 1161 },
    { "amount", CONFIG_NODE_SCALAR, 1414, 0, 1162, 1162, 1162 },
    { "", CONFIG_NODE_RECORD, 1389, 6, 1163, 1167, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1420, 0, 1163, 1163, 1163 },
    { "arc", CONFIG_NODE_SCALAR, 1420, 0, 1164, 1164, 1164 },
    { "mods", CONFIG_NODE_SCALAR, 1420, 0, 1165, 1165, 1165 },
    { "ammo", CONFIG_NODE_SCALAR, 1420, 0, 1166, 1166, 1166 },
    { "amount", CONFIG_NODE_SCALAR, 1420, 0, 1167, 1167, 1167 },
    { "", CONFIG_NODE_RECORD, 1389, 7, 1168, 1172, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1426, 0, 1168, 1168, 1168 },
    { "arc", CONFIG_NODE_SCALAR, 1426, 0, 1169, 1169, 1169 },
    { "mods", CONFIG_NODE_SCALAR, 1426, 0, 1170, 1170, 1170 },
    { "ammo", CONFIG_NODE_SCALAR, 1426, 0, 1171, 1171, 1171 },
    { "amount", CONFIG_NODE_SCALAR, 1426, 0, 1172, 1172, 1172 },
    { "", CONFIG_NODE_RECORD, 1389, 8, 1173, 1177, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1432, 0, 1173, 1173, 1173 },
    { "arc", CONFIG_NODE_SCALAR, 1432, 0, 1174, 1174, 1174 },
    { "mods", CONFIG_NODE_SCALAR, 1432, 0, 1175, 1175, 1175 },
    { "ammo", CONFIG_NODE_SCALAR, 1432, 0, 1176, 1176, 1176 },
    { "amount", CONFIG_NODE_SCALAR, 1432, 0, 1177, 1177, 1177 },
    { "specials", CONFIG_NODE_SCALAR, 1382, 0, 1178, 1178, 1178 },
    { "", CONFIG_NODE_RECORD, 1267, 3, 1179, 1225, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1439, 0, 1179, 1179, 1179 },
    { "hull_size", CONFIG_NODE_SCALAR, 1439, 0, 1180, 1180, 1180 },
    { "drive", CONFIG_NODE_SCALAR, 1439, 0, 1181, 1181, 1181 },
    { "armor", CONFIG_NODE_SCALAR, 1439, 0, 1182, 1182, 1182 },
    { "shield", CONFIG_NODE_SCALAR, 1439, 0, 1183, 1183, 1183 },
    { "computer", CONFIG_NODE_SCALAR, 1439, 0, 1184, 1184, 1184 },
    { "weapons", CONFIG_NODE_ARRAY, 1439, 0, 1185, 1224, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1446, 1, 1185, 1189, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1447, 0, 1185, 1185, 1185 },
    { "arc", CONFIG_NODE_SCALAR, 1447, 0, 1186, 1186, 1186 },
    { "mods", CONFIG_NODE_SCALAR, 1447, 0, 1187, 1187, 1187 },
    { "ammo", CONFIG_NODE_SCALAR, 1447, 0, 1188, 1188, 1188 },
    { "amount", CONFIG_NODE_SCALAR, 1447, 0, 1189, 1189, 1189 },
    { "", CONFIG_NODE_RECORD, 1446, 2, 1190, 1194, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1453, 0, 1190, 1190, 1190 },
    { "arc", CONFIG_NODE_SCALAR, 1453, 0, 1191, 1191, 1191 },
    { "mods", CONFIG_NODE_SCALAR, 1453, 0, 1192, 1192, 1192 },
    { "ammo", CONFIG_NODE_SCALAR, 1453, 0, 1193, 1193, 1193 },
    { "amount", CONFIG_NODE_SCALAR, 1453, 0, 1194, 1194, 1194 },
    { "", CONFIG_NODE_RECORD, 1446, 3, 1195, 1199, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1459, 0, 1195, 1195, 1195 },
    { "arc", CONFIG_NODE_SCALAR, 1459, 0, 1196, 1196, 1196 },
    { "mods", CONFIG_NODE_SCALAR, 1459, 0, 1197, 1197, 1197 },
    { "ammo", CONFIG_NODE_SCALAR, 1459, 0, 1198, 1198, 1198 },
    { "amount", CONFIG_NODE_SCALAR, 1459, 0, 1199, 1199, 1199 },
    { "", CONFIG_NODE_RECORD, 1446, 4, 1200, 1204, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1465, 0, 1200, 1200, 1200 },
    { "arc", CONFIG_NODE_SCALAR, 1465, 0, 1201, 1201, 1201 },
    { "mods", CONFIG_NODE_SCALAR, 1465, 0, 1202, 1202, 1202 },
    { "ammo", CONFIG_NODE_SCALAR, 1465, 0, 1203, 1203, 1203 },
    { "amount", CONFIG_NODE_SCALAR, 1465, 0, 1204, 1204, 1204 },
    { "", CONFIG_NODE_RECORD, 1446, 5, 1205, 1209, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1471, 0, 1205, 1205, 1205 },
    { "arc", CONFIG_NODE_SCALAR, 1471, 0, 1206, 1206, 1206 },
    { "mods", CONFIG_NODE_SCALAR, 1471, 0, 1207, 1207, 1207 },
    { "ammo", CONFIG_NODE_SCALAR, 1471, 0, 1208, 1208, 1208 },
    { "amount", CONFIG_NODE_SCALAR, 1471, 0, 1209, 1209, 1209 },
    { "", CONFIG_NODE_RECORD, 1446, 6, 1210, 1214, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1477, 0, 1210, 1210, 1210 },
    { "arc", CONFIG_NODE_SCALAR, 1477, 0, 1211, 1211, 1211 },
    { "mods", CONFIG_NODE_SCALAR, 1477, 0, 1212, 1212, 1212 },
    { "ammo", CONFIG_NODE_SCALAR, 1477, 0, 1213, 1213, 1213 },
    { "amount", CONFIG_NODE_SCALAR, 1477, 0, 1214, 1214, 1214 },
    { "", CONFIG_NODE_RECORD, 1446, 7, 1215, 1219, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1483, 0, 1215, 1215, 1215 },
    { "arc", CONFIG_NODE_SCALAR, 1483, 0, 1216, 1216, 1216 },
    { "mods", CONFIG_NODE_SCALAR, 1483, 0, 1217, 1217, 1217 },
    { "ammo", CONFIG_NODE_SCALAR, 1483, 0, 1218, 1218, 1218 },
    { "amount", CONFIG_NODE_SCALAR, 1483, 0, 1219, 1219, 1219 },
    { "", CONFIG_NODE_RECORD, 1446, 8, 1220, 1224, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1489, 0, 1220, 1220, 1220 },
    { "arc", CONFIG_NODE_SCALAR, 1489, 0, 1221, 1221, 1221 },
    { "mods", CONFIG_NODE_SCALAR, 1489, 0, 1222, 1222, 1222 },
    { "ammo", CONFIG_NODE_SCALAR, 1489, 0, 1223, 1223, 1223 },
    { "amount", CONFIG_NODE_SCALAR, 1489, 0, 1224, 1224, 1224 },
    { "specials", CONFIG_NODE_SCALAR, 1439, 0, 1225, 1225, 1225 },
    { "", CONFIG_NODE_RECORD, 1267, 4, 1226, 1272, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1496, 0, 1226, 1226, 1226 },
    { "hull_size", CONFIG_NODE_SCALAR, 1496, 0, 1227, 1227, 1227 },
    { "drive", CONFIG_NODE_SCALAR, 1496, 0, 1228, 1228, 1228 },
    { "armor", CONFIG_NODE_SCALAR, 1496, 0, 1229, 1229, 1229 },
    { "shield", CONFIG_NODE_SCALAR, 1496, 0, 1230, 1230, 1230 },
    { "computer", CONFIG_NODE_SCALAR, 1496, 0, 1231, 1231, 1231 },
    { "weapons", CONFIG_NODE_ARRAY, 1496, 0, 1232, 1271, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1503, 1, 1232, 1236, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1504, 0, 1232, 1232, 1232 },
    { "arc", CONFIG_NODE_SCALAR, 1504, 0, 1233, 1233, 1233 },
    { "mods", CONFIG_NODE_SCALAR, 1504, 0, 1234, 1234, 1234 },
    { "ammo", CONFIG_NODE_SCALAR, 1504, 0, 1235, 1235, 1235 },
    { "amount", CONFIG_NODE_SCALAR, 1504, 0, 1236, 1236, 1236 },
    { "", CONFIG_NODE_RECORD, 1503, 2, 1237, 1241, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1510, 0, 1237, 1237, 1237 },
    { "arc", CONFIG_NODE_SCALAR, 1510, 0, 1238, 1238, 1238 },
    { "mods", CONFIG_NODE_SCALAR, 1510, 0, 1239, 1239, 1239 },
    { "ammo", CONFIG_NODE_SCALAR, 1510, 0, 1240, 1240, 1240 },
    { "amount", CONFIG_NODE_SCALAR, 1510, 0, 1241, 1241, 1241 },
    { "", CONFIG_NODE_RECORD, 1503, 3, 1242, 1246, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1516, 0, 1242, 1242, 1242 },
    { "arc", CONFIG_NODE_SCALAR, 1516, 0, 1243, 1243, 1243 },
    { "mods", CONFIG_NODE_SCALAR, 1516, 0, 1244, 1244, 1244 },
    { "ammo", CONFIG_NODE_SCALAR, 1516, 0, 1245, 1245, 1245 },
    { "amount", CONFIG_NODE_SCALAR, 1516, 0, 1246, 1246, 1246 },
    { "", CONFIG_NODE_RECORD, 1503, 4, 1247, 1251, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1522, 0, 1247, 1247, 1247 },
    { "arc", CONFIG_NODE_SCALAR, 1522, 0, 1248, 1248, 1248 },
    { "mods", CONFIG_NODE_SCALAR, 1522, 0, 1249, 1249, 1249 },
    { "ammo", CONFIG_NODE_SCALAR, 1522, 0, 1250, 1250, 1250 },
    { "amount", CONFIG_NODE_SCALAR, 1522, 0, 1251, 1251, 1251 },
    { "", CONFIG_NODE_RECORD, 1503, 5, 1252, 1256, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1528, 0, 1252, 1252, 1252 },
    { "arc", CONFIG_NODE_SCALAR, 1528, 0, 1253, 1253, 1253 },
    { "mods", CONFIG_NODE_SCALAR, 1528, 0, 1254, 1254, 1254 },
    { "ammo", CONFIG_NODE_SCALAR, 1528, 0, 1255, 1255, 1255 },
    { "amount", CONFIG_NODE_SCALAR, 1528, 0, 1256, 1256, 1256 },
    { "", CONFIG_NODE_RECORD, 1503, 6, 1257, 1261, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1534, 0, 1257, 1257, 1257 },
    { "arc", CONFIG_NODE_SCALAR, 1534, 0, 1258, 1258, 1258 },
    { "mods", CONFIG_NODE_SCALAR, 1534, 0, 1259, 1259, 1259 },
    { "ammo", CONFIG_NODE_SCALAR, 1534, 0, 1260, 1260, 1260 },
    { "amount", CONFIG_NODE_SCALAR, 1534, 0, 1261, 1261, 1261 },
    { "", CONFIG_NODE_RECORD, 1503, 7, 1262, 1266, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1540, 0, 1262, 1262, 1262 },
    { "arc", CONFIG_NODE_SCALAR, 1540, 0, 1263, 1263, 1263 },
    { "mods", CONFIG_NODE_SCALAR, 1540, 0, 1264, 1264, 1264 },
    { "ammo", CONFIG_NODE_SCALAR, 1540, 0, 1265, 1265, 1265 },
    { "amount", CONFIG_NODE_SCALAR, 1540, 0, 1266, 1266, 1266 },
    { "", CONFIG_NODE_RECORD, 1503, 8, 1267, 1271, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1546, 0, 1267, 1267, 1267 },
    { "arc", CONFIG_NODE_SCALAR, 1546, 0, 1268, 1268, 1268 },
    { "mods", CONFIG_NODE_SCALAR, 1546, 0, 1269, 1269, 1269 },
    { "ammo", CONFIG_NODE_SCALAR, 1546, 0, 1270, 1270, 1270 },
    { "amount", CONFIG_NODE_SCALAR, 1546, 0, 1271, 1271, 1271 },
    { "specials", CONFIG_NODE_SCALAR, 1496, 0, 1272, 1272, 1272 },
    { "", CONFIG_NODE_ARRAY, 694, 3, 1273, 1507, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1553, 0, 1273, 1319, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1554, 0, 1273, 1273, 1273 },
    { "hull_size", CONFIG_NODE_SCALAR, 1554, 0, 1274, 1274, 1274 },
    { "drive", CONFIG_NODE_SCALAR, 1554, 0, 1275, 1275, 1275 },
    { "armor", CONFIG_NODE_SCALAR, 1554, 0, 1276, 1276, 1276 },
    { "shield", CONFIG_NODE_SCALAR, 1554, 0, 1277, 1277, 1277 },
    { "computer", CONFIG_NODE_SCALAR, 1554, 0, 1278, 1278, 1278 },
    { "weapons", CONFIG_NODE_ARRAY, 1554, 0, 1279, 1318, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1561, 1, 1279, 1283, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1562, 0, 1279, 1279, 1279 },
    { "arc", CONFIG_NODE_SCALAR, 1562, 0, 1280, 1280, 1280 },
    { "mods", CONFIG_NODE_SCALAR, 1562, 0, 1281, 1281, 1281 },
    { "ammo", CONFIG_NODE_SCALAR, 1562, 0, 1282, 1282, 1282 },
    { "amount", CONFIG_NODE_SCALAR, 1562, 0, 1283, 1283, 1283 },
    { "", CONFIG_NODE_RECORD, 1561, 2, 1284, 1288, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1568, 0, 1284, 1284, 1284 },
    { "arc", CONFIG_NODE_SCALAR, 1568, 0, 1285, 1285, 1285 },
    { "mods", CONFIG_NODE_SCALAR, 1568, 0, 1286, 1286, 1286 },
    { "ammo", CONFIG_NODE_SCALAR, 1568, 0, 1287, 1287, 1287 },
    { "amount", CONFIG_NODE_SCALAR, 1568, 0, 1288, 1288, 1288 },
    { "", CONFIG_NODE_RECORD, 1561, 3, 1289, 1293, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1574, 0, 1289, 1289, 1289 },
    { "arc", CONFIG_NODE_SCALAR, 1574, 0, 1290, 1290, 1290 },
    { "mods", CONFIG_NODE_SCALAR, 1574, 0, 1291, 1291, 1291 },
    { "ammo", CONFIG_NODE_SCALAR, 1574, 0, 1292, 1292, 1292 },
    { "amount", CONFIG_NODE_SCALAR, 1574, 0, 1293, 1293, 1293 },
    { "", CONFIG_NODE_RECORD, 1561, 4, 1294, 1298, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1580, 0, 1294, 1294, 1294 },
    { "arc", CONFIG_NODE_SCALAR, 1580, 0, 1295, 1295, 1295 },
    { "mods", CONFIG_NODE_SCALAR, 1580, 0, 1296, 1296, 1296 },
    { "ammo", CONFIG_NODE_SCALAR, 1580, 0, 1297, 1297, 1297 },
    { "amount", CONFIG_NODE_SCALAR, 1580, 0, 1298, 1298, 1298 },
    { "", CONFIG_NODE_RECORD, 1561, 5, 1299, 1303, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1586, 0, 1299, 1299, 1299 },
    { "arc", CONFIG_NODE_SCALAR, 1586, 0, 1300, 1300, 1300 },
    { "mods", CONFIG_NODE_SCALAR, 1586, 0, 1301, 1301, 1301 },
    { "ammo", CONFIG_NODE_SCALAR, 1586, 0, 1302, 1302, 1302 },
    { "amount", CONFIG_NODE_SCALAR, 1586, 0, 1303, 1303, 1303 },
    { "", CONFIG_NODE_RECORD, 1561, 6, 1304, 1308, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1592, 0, 1304, 1304, 1304 },
    { "arc", CONFIG_NODE_SCALAR, 1592, 0, 1305, 1305, 1305 },
    { "mods", CONFIG_NODE_SCALAR, 1592, 0, 1306, 1306, 1306 },
    { "ammo", CONFIG_NODE_SCALAR, 1592, 0, 1307, 1307, 1307 },
    { "amount", CONFIG_NODE_SCALAR, 1592, 0, 1308, 1308, 1308 },
    { "", CONFIG_NODE_RECORD, 1561, 7, 1309, 1313, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1598, 0, 1309, 1309, 1309 },
    { "arc", CONFIG_NODE_SCALAR, 1598, 0, 1310, 1310, 1310 },
    { "mods", CONFIG_NODE_SCALAR, 1598, 0, 1311, 1311, 1311 },
    { "ammo", CONFIG_NODE_SCALAR, 1598, 0, 1312, 1312, 1312 },
    { "amount", CONFIG_NODE_SCALAR, 1598, 0, 1313, 1313, 1313 },
    { "", CONFIG_NODE_RECORD, 1561, 8, 1314, 1318, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1604, 0, 1314, 1314, 1314 },
    { "arc", CONFIG_NODE_SCALAR, 1604, 0, 1315, 1315, 1315 },
    { "mods", CONFIG_NODE_SCALAR, 1604, 0, 1316, 1316, 1316 },
    { "ammo", CONFIG_NODE_SCALAR, 1604, 0, 1317, 1317, 1317 },
    { "amount", CONFIG_NODE_SCALAR, 1604, 0, 1318, 1318, 1318 },
    { "specials", CONFIG_NODE_SCALAR, 1554, 0, 1319, 1319, 1319 },
    { "", CONFIG_NODE_RECORD, 1553, 1, 1320, 1366, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1611, 0, 1320, 1320, 1320 },
    { "hull_size", CONFIG_NODE_SCALAR, 1611, 0, 1321, 1321, 1321 },
    { "drive", CONFIG_NODE_SCALAR, 1611, 0, 1322, 1322, 1322 },
    { "armor", CONFIG_NODE_SCALAR, 1611, 0, 1323, 1323, 1323 },
    { "shield", CONFIG_NODE_SCALAR, 1611, 0, 1324, 1324, 1324 },
    { "computer", CONFIG_NODE_SCALAR, 1611, 0, 1325, 1325, 1325 },
    { "weapons", CONFIG_NODE_ARRAY, 1611, 0, 1326, 1365, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1618, 1, 1326, 1330, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1619, 0, 1326, 1326, 1326 },
    { "arc", CONFIG_NODE_SCALAR, 1619, 0, 1327, 1327, 1327 },
    { "mods", CONFIG_NODE_SCALAR, 1619, 0, 1328, 1328, 1328 },
    { "ammo", CONFIG_NODE_SCALAR, 1619, 0, 1329, 1329, 1329 },
    { "amount", CONFIG_NODE_SCALAR, 1619, 0, 1330, 1330, 1330 },
    { "", CONFIG_NODE_RECORD, 1618, 2, 1331, 1335, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1625, 0, 1331, 1331, 1331 },
    { "arc", CONFIG_NODE_SCALAR, 1625, 0, 1332, 1332, 1332 },
    { "mods", CONFIG_NODE_SCALAR, 1625, 0, 1333, 1333, 1333 },
    { "ammo", CONFIG_NODE_SCALAR, 1625, 0, 1334, 1334, 1334 },
    { "amount", CONFIG_NODE_SCALAR, 1625, 0, 1335, 1335, 1335 },
    { "", CONFIG_NODE_RECORD, 1618, 3, 1336, 1340, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1631, 0, 1336, 1336, 1336 },
    { "arc", CONFIG_NODE_SCALAR, 1631, 0, 1337, 1337, 1337 },
    { "mods", CONFIG_NODE_SCALAR, 1631, 0, 1338, 1338, 1338 },
    { "ammo", CONFIG_NODE_SCALAR, 1631, 0, 1339, 1339, 1339 },
    { "amount", CONFIG_NODE_SCALAR, 1631, 0, 1340, 1340, 1340 },
    { "", CONFIG_NODE_RECORD, 1618, 4, 1341, 1345, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1637, 0, 1341, 1341, 1341 },
    { "arc", CONFIG_NODE_SCALAR, 1637, 0, 1342, 1342, 1342 },
    { "mods", CONFIG_NODE_SCALAR, 1637, 0, 1343, 1343, 1343 },
    { "ammo", CONFIG_NODE_SCALAR, 1637, 0, 1344, 1344, 1344 },
    { "amount", CONFIG_NODE_SCALAR, 1637, 0, 1345, 1345, 1345 },
    { "", CONFIG_NODE_RECORD, 1618, 5, 1346, 1350, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1643, 0, 1346, 1346, 1346 },
    { "arc", CONFIG_NODE_SCALAR, 1643, 0, 1347, 1347, 1347 },
    { "mods", CONFIG_NODE_SCALAR, 1643, 0, 1348, 1348, 1348 },
    { "ammo", CONFIG_NODE_SCALAR, 1643, 0, 1349, 1349, 1349 },
    { "amount", CONFIG_NODE_SCALAR, 1643, 0, 1350, 1350, 1350 },
    { "", CONFIG_NODE_RECORD, 1618, 6, 1351, 1355, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1649, 0, 1351, 1351, 1351 },
    { "arc", CONFIG_NODE_SCALAR, 1649, 0, 1352, 1352, 1352 },
    { "mods", CONFIG_NODE_SCALAR, 1649, 0, 1353, 1353, 1353 },
    { "ammo", CONFIG_NODE_SCALAR, 1649, 0, 1354, 1354, 1354 },
    { "amount", CONFIG_NODE_SCALAR, 1649, 0, 1355, 1355, 1355 },
    { "", CONFIG_NODE_RECORD, 1618, 7, 1356, 1360, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1655, 0, 1356, 1356, 1356 },
    { "arc", CONFIG_NODE_SCALAR, 1655, 0, 1357, 1357, 1357 },
    { "mods", CONFIG_NODE_SCALAR, 1655, 0, 1358, 1358, 1358 },
    { "ammo", CONFIG_NODE_SCALAR, 1655, 0, 1359, 1359, 1359 },
    { "amount", CONFIG_NODE_SCALAR, 1655, 0, 1360, 1360, 1360 },
    { "", CONFIG_NODE_RECORD, 1618, 8, 1361, 1365, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1661, 0, 1361, 1361, 1361 },
    { "arc", CONFIG_NODE_SCALAR, 1661, 0, 1362, 1362, 1362 },
    { "mods", CONFIG_NODE_SCALAR, 1661, 0, 1363, 1363, 1363 },
    { "ammo", CONFIG_NODE_SCALAR, 1661, 0, 1364, 1364, 1364 },
    { "amount", CONFIG_NODE_SCALAR, 1661, 0, 1365, 1365, 1365 },
    { "specials", CONFIG_NODE_SCALAR, 1611, 0, 1366, 1366, 1366 },
    { "", CONFIG_NODE_RECORD, 1553, 2, 1367, 1413, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1668, 0, 1367, 1367, 1367 },
    { "hull_size", CONFIG_NODE_SCALAR, 1668, 0, 1368, 1368, 1368 },
    { "drive", CONFIG_NODE_SCALAR, 1668, 0, 1369, 1369, 1369 },
    { "armor", CONFIG_NODE_SCALAR, 1668, 0, 1370, 1370, 1370 },
    { "shield", CONFIG_NODE_SCALAR, 1668, 0, 1371, 1371, 1371 },
    { "computer", CONFIG_NODE_SCALAR, 1668, 0, 1372, 1372, 1372 },
    { "weapons", CONFIG_NODE_ARRAY, 1668, 0, 1373, 1412, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1675, 1, 1373, 1377, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1676, 0, 1373, 1373, 1373 },
    { "arc", CONFIG_NODE_SCALAR, 1676, 0, 1374, 1374, 1374 },
    { "mods", CONFIG_NODE_SCALAR, 1676, 0, 1375, 1375, 1375 },
    { "ammo", CONFIG_NODE_SCALAR, 1676, 0, 1376, 1376, 1376 },
    { "amount", CONFIG_NODE_SCALAR, 1676, 0, 1377, 1377, 1377 },
    { "", CONFIG_NODE_RECORD, 1675, 2, 1378, 1382, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1682, 0, 1378, 1378, 1378 },
    { "arc", CONFIG_NODE_SCALAR, 1682, 0, 1379, 1379, 1379 },
    { "mods", CONFIG_NODE_SCALAR, 1682, 0, 1380, 1380, 1380 },
    { "ammo", CONFIG_NODE_SCALAR, 1682, 0, 1381, 1381, 1381 },
    { "amount", CONFIG_NODE_SCALAR, 1682, 0, 1382, 1382, 1382 },
    { "", CONFIG_NODE_RECORD, 1675, 3, 1383, 1387, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1688, 0, 1383, 1383, 1383 },
    { "arc", CONFIG_NODE_SCALAR, 1688, 0, 1384, 1384, 1384 },
    { "mods", CONFIG_NODE_SCALAR, 1688, 0, 1385, 1385, 1385 },
    { "ammo", CONFIG_NODE_SCALAR, 1688, 0, 1386, 1386, 1386 },
    { "amount", CONFIG_NODE_SCALAR, 1688, 0, 1387, 1387, 1387 },
    { "", CONFIG_NODE_RECORD, 1675, 4, 1388, 1392, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1694, 0, 1388, 1388, 1388 },
    { "arc", CONFIG_NODE_SCALAR, 1694, 0, 1389, 1389, 1389 },
    { "mods", CONFIG_NODE_SCALAR, 1694, 0, 1390, 1390, 1390 },
    { "ammo", CONFIG_NODE_SCALAR, 1694, 0, 1391, 1391, 1391 },
    { "amount", CONFIG_NODE_SCALAR, 1694, 0, 1392, 1392, 1392 },
    { "", CONFIG_NODE_RECORD, 1675, 5, 1393, 1397, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1700, 0, 1393, 1393, 1393 },
    { "arc", CONFIG_NODE_SCALAR, 1700, 0, 1394, 1394, 1394 },
    { "mods", CONFIG_NODE_SCALAR, 1700, 0, 1395, 1395, 1395 },
    { "ammo", CONFIG_NODE_SCALAR, 1700, 0, 1396, 1396, 1396 },
    { "amount", CONFIG_NODE_SCALAR, 1700, 0, 1397, 1397, 1397 },
    { "", CONFIG_NODE_RECORD, 1675, 6, 1398, 1402, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1706, 0, 1398, 1398, 1398 },
    { "arc", CONFIG_NODE_SCALAR, 1706, 0, 1399, 1399, 1399 },
    { "mods", CONFIG_NODE_SCALAR, 1706, 0, 1400, 1400, 1400 },
    { "ammo", CONFIG_NODE_SCALAR, 1706, 0, 1401, 1401, 1401 },
    { "amount", CONFIG_NODE_SCALAR, 1706, 0, 1402, 1402, 1402 },
    { "", CONFIG_NODE_RECORD, 1675, 7, 1403, 1407, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1712, 0, 1403, 1403, 1403 },
    { "arc", CONFIG_NODE_SCALAR, 1712, 0, 1404, 1404, 1404 },
    { "mods", CONFIG_NODE_SCALAR, 1712, 0, 1405, 1405, 1405 },
    { "ammo", CONFIG_NODE_SCALAR, 1712, 0, 1406, 1406, 1406 },
    { "amount", CONFIG_NODE_SCALAR, 1712, 0, 1407, 1407, 1407 },
    { "", CONFIG_NODE_RECORD, 1675, 8, 1408, 1412, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1718, 0, 1408, 1408, 1408 },
    { "arc", CONFIG_NODE_SCALAR, 1718, 0, 1409, 1409, 1409 },
    { "mods", CONFIG_NODE_SCALAR, 1718, 0, 1410, 1410, 1410 },
    { "ammo", CONFIG_NODE_SCALAR, 1718, 0, 1411, 1411, 1411 },
    { "amount", CONFIG_NODE_SCALAR, 1718, 0, 1412, 1412, 1412 },
    { "specials", CONFIG_NODE_SCALAR, 1668, 0, 1413, 1413, 1413 },
    { "", CONFIG_NODE_RECORD, 1553, 3, 1414, 1460, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1725, 0, 1414, 1414, 1414 },
    { "hull_size", CONFIG_NODE_SCALAR, 1725, 0, 1415, 1415, 1415 },
    { "drive", CONFIG_NODE_SCALAR, 1725, 0, 1416, 1416, 1416 },
    { "armor", CONFIG_NODE_SCALAR, 1725, 0, 1417, 1417, 1417 },
    { "shield", CONFIG_NODE_SCALAR, 1725, 0, 1418, 1418, 1418 },
    { "computer", CONFIG_NODE_SCALAR, 1725, 0, 1419, 1419, 1419 },
    { "weapons", CONFIG_NODE_ARRAY, 1725, 0, 1420, 1459, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1732, 1, 1420, 1424, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1733, 0, 1420, 1420, 1420 },
    { "arc", CONFIG_NODE_SCALAR, 1733, 0, 1421, 1421, 1421 },
    { "mods", CONFIG_NODE_SCALAR, 1733, 0, 1422, 1422, 1422 },
    { "ammo", CONFIG_NODE_SCALAR, 1733, 0, 1423, 1423, 1423 },
    { "amount", CONFIG_NODE_SCALAR, 1733, 0, 1424, 1424, 1424 },
    { "", CONFIG_NODE_RECORD, 1732, 2, 1425, 1429, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1739, 0, 1425, 1425, 1425 },
    { "arc", CONFIG_NODE_SCALAR, 1739, 0, 1426, 1426, 1426 },
    { "mods", CONFIG_NODE_SCALAR, 1739, 0, 1427, 1427, 1427 },
    { "ammo", CONFIG_NODE_SCALAR, 1739, 0, 1428, 1428, 1428 },
    { "amount", CONFIG_NODE_SCALAR, 1739, 0, 1429, 1429, 1429 },
    { "", CONFIG_NODE_RECORD, 1732, 3, 1430, 1434, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1745, 0, 1430, 1430, 1430 },
    { "arc", CONFIG_NODE_SCALAR, 1745, 0, 1431, 1431, 1431 },
    { "mods", CONFIG_NODE_SCALAR, 1745, 0, 1432, 1432, 1432 },
    { "ammo", CONFIG_NODE_SCALAR, 1745, 0, 1433, 1433, 1433 },
    { "amount", CONFIG_NODE_SCALAR, 1745, 0, 1434, 1434, 1434 },
    { "", CONFIG_NODE_RECORD, 1732, 4, 1435, 1439, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1751, 0, 1435, 1435, 1435 },
    { "arc", CONFIG_NODE_SCALAR, 1751, 0, 1436, 1436, 1436 },
    { "mods", CONFIG_NODE_SCALAR, 1751, 0, 1437, 1437, 1437 },
    { "ammo", CONFIG_NODE_SCALAR, 1751, 0, 1438, 1438, 1438 },
    { "amount", CONFIG_NODE_SCALAR, 1751, 0, 1439, 1439, 1439 },
    { "", CONFIG_NODE_RECORD, 1732, 5, 1440, 1444, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1757, 0, 1440, 1440, 1440 },
    { "arc", CONFIG_NODE_SCALAR, 1757, 0, 1441, 1441, 1441 },
    { "mods", CONFIG_NODE_SCALAR, 1757, 0, 1442, 1442, 1442 },
    { "ammo", CONFIG_NODE_SCALAR, 1757, 0, 1443, 1443, 1443 },
    { "amount", CONFIG_NODE_SCALAR, 1757, 0, 1444, 1444, 1444 },
    { "", CONFIG_NODE_RECORD, 1732, 6, 1445, 1449, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1763, 0, 1445, 1445, 1445 },
    { "arc", CONFIG_NODE_SCALAR, 1763, 0, 1446, 1446, 1446 },
    { "mods", CONFIG_NODE_SCALAR, 1763, 0, 1447, 1447, 1447 },
    { "ammo", CONFIG_NODE_SCALAR, 1763, 0, 1448, 1448, 1448 },
    { "amount", CONFIG_NODE_SCALAR, 1763, 0, 1449, 1449, 1449 },
    { "", CONFIG_NODE_RECORD, 1732, 7, 1450, 1454, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1769, 0, 1450, 1450, 1450 },
    { "arc", CONFIG_NODE_SCALAR, 1769, 0, 1451, 1451, 1451 },
    { "mods", CONFIG_NODE_SCALAR, 1769, 0, 1452, 1452, 1452 },
    { "ammo", CONFIG_NODE_SCALAR, 1769, 0, 1453, 1453, 1453 },
    { "amount", CONFIG_NODE_SCALAR, 1769, 0, 1454, 1454, 1454 },
    { "", CONFIG_NODE_RECORD, 1732, 8, 1455, 1459, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1775, 0, 1455, 1455, 1455 },
    { "arc", CONFIG_NODE_SCALAR, 1775, 0, 1456, 1456, 1456 },
    { "mods", CONFIG_NODE_SCALAR, 1775, 0, 1457, 1457, 1457 },
    { "ammo", CONFIG_NODE_SCALAR, 1775, 0, 1458, 1458, 1458 },
    { "amount", CONFIG_NODE_SCALAR, 1775, 0, 1459, 1459, 1459 },
    { "specials", CONFIG_NODE_SCALAR, 1725, 0, 1460, 1460, 1460 },
    { "", CONFIG_NODE_RECORD, 1553, 4, 1461, 1507, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1782, 0, 1461, 1461, 1461 },
    { "hull_size", CONFIG_NODE_SCALAR, 1782, 0, 1462, 1462, 1462 },
    { "drive", CONFIG_NODE_SCALAR, 1782, 0, 1463, 1463, 1463 },
    { "armor", CONFIG_NODE_SCALAR, 1782, 0, 1464, 1464, 1464 },
    { "shield", CONFIG_NODE_SCALAR, 1782, 0, 1465, 1465, 1465 },
    { "computer", CONFIG_NODE_SCALAR, 1782, 0, 1466, 1466, 1466 },
    { "weapons", CONFIG_NODE_ARRAY, 1782, 0, 1467, 1506, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1789, 1, 1467, 1471, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1790, 0, 1467, 1467, 1467 },
    { "arc", CONFIG_NODE_SCALAR, 1790, 0, 1468, 1468, 1468 },
    { "mods", CONFIG_NODE_SCALAR, 1790, 0, 1469, 1469, 1469 },
    { "ammo", CONFIG_NODE_SCALAR, 1790, 0, 1470, 1470, 1470 },
    { "amount", CONFIG_NODE_SCALAR, 1790, 0, 1471, 1471, 1471 },
    { "", CONFIG_NODE_RECORD, 1789, 2, 1472, 1476, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1796, 0, 1472, 1472, 1472 },
    { "arc", CONFIG_NODE_SCALAR, 1796, 0, 1473, 1473, 1473 },
    { "mods", CONFIG_NODE_SCALAR, 1796, 0, 1474, 1474, 1474 },
    { "ammo", CONFIG_NODE_SCALAR, 1796, 0, 1475, 1475, 1475 },
    { "amount", CONFIG_NODE_SCALAR, 1796, 0, 1476, 1476, 1476 },
    { "", CONFIG_NODE_RECORD, 1789, 3, 1477, 1481, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1802, 0, 1477, 1477, 1477 },
    { "arc", CONFIG_NODE_SCALAR, 1802, 0, 1478, 1478, 1478 },
    { "mods", CONFIG_NODE_SCALAR, 1802, 0, 1479, 1479, 1479 },
    { "ammo", CONFIG_NODE_SCALAR, 1802, 0, 1480, 1480, 1480 },
    { "amount", CONFIG_NODE_SCALAR, 1802, 0, 1481, 1481, 1481 },
    { "", CONFIG_NODE_RECORD, 1789, 4, 1482, 1486, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1808, 0, 1482, 1482, 1482 },
    { "arc", CONFIG_NODE_SCALAR, 1808, 0, 1483, 1483, 1483 },
    { "mods", CONFIG_NODE_SCALAR, 1808, 0, 1484, 1484, 1484 },
    { "ammo", CONFIG_NODE_SCALAR, 1808, 0, 1485, 1485, 1485 },
    { "amount", CONFIG_NODE_SCALAR, 1808, 0, 1486, 1486, 1486 },
    { "", CONFIG_NODE_RECORD, 1789, 5, 1487, 1491, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1814, 0, 1487, 1487, 1487 },
    { "arc", CONFIG_NODE_SCALAR, 1814, 0, 1488, 1488, 1488 },
    { "mods", CONFIG_NODE_SCALAR, 1814, 0, 1489, 1489, 1489 },
    { "ammo", CONFIG_NODE_SCALAR, 1814, 0, 1490, 1490, 1490 },
    { "amount", CONFIG_NODE_SCALAR, 1814, 0, 1491, 1491, 1491 },
    { "", CONFIG_NODE_RECORD, 1789, 6, 1492, 1496, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1820, 0, 1492, 1492, 1492 },
    { "arc", CONFIG_NODE_SCALAR, 1820, 0, 1493, 1493, 1493 },
    { "mods", CONFIG_NODE_SCALAR, 1820, 0, 1494, 1494, 1494 },
    { "ammo", CONFIG_NODE_SCALAR, 1820, 0, 1495, 1495, 1495 },
    { "amount", CONFIG_NODE_SCALAR, 1820, 0, 1496, 1496, 1496 },
    { "", CONFIG_NODE_RECORD, 1789, 7, 1497, 1501, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1826, 0, 1497, 1497, 1497 },
    { "arc", CONFIG_NODE_SCALAR, 1826, 0, 1498, 1498, 1498 },
    { "mods", CONFIG_NODE_SCALAR, 1826, 0, 1499, 1499, 1499 },
    { "ammo", CONFIG_NODE_SCALAR, 1826, 0, 1500, 1500, 1500 },
    { "amount", CONFIG_NODE_SCALAR, 1826, 0, 1501, 1501, 1501 },
    { "", CONFIG_NODE_RECORD, 1789, 8, 1502, 1506, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1832, 0, 1502, 1502, 1502 },
    { "arc", CONFIG_NODE_SCALAR, 1832, 0, 1503, 1503, 1503 },
    { "mods", CONFIG_NODE_SCALAR, 1832, 0, 1504, 1504, 1504 },
    { "ammo", CONFIG_NODE_SCALAR, 1832, 0, 1505, 1505, 1505 },
    { "amount", CONFIG_NODE_SCALAR, 1832, 0, 1506, 1506, 1506 },
    { "specials", CONFIG_NODE_SCALAR, 1782, 0, 1507, 1507, 1507 },
    { "", CONFIG_NODE_ARRAY, 694, 4, 1508, 1742, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1839, 0, 1508, 1554, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1840, 0, 1508, 1508, 1508 },
    { "hull_size", CONFIG_NODE_SCALAR, 1840, 0, 1509, 1509, 1509 },
    { "drive", CONFIG_NODE_SCALAR, 1840, 0, 1510, 1510, 1510 },
    { "armor", CONFIG_NODE_SCALAR, 1840, 0, 1511, 1511, 1511 },
    { "shield", CONFIG_NODE_SCALAR, 1840, 0, 1512, 1512, 1512 },
    { "computer", CONFIG_NODE_SCALAR, 1840, 0, 1513, 1513, 1513 },
    { "weapons", CONFIG_NODE_ARRAY, 1840, 0, 1514, 1553, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1847, 1, 1514, 1518, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1848, 0, 1514, 1514, 1514 },
    { "arc", CONFIG_NODE_SCALAR, 1848, 0, 1515, 1515, 1515 },
    { "mods", CONFIG_NODE_SCALAR, 1848, 0, 1516, 1516, 1516 },
    { "ammo", CONFIG_NODE_SCALAR, 1848, 0, 1517, 1517, 1517 },
    { "amount", CONFIG_NODE_SCALAR, 1848, 0, 1518, 1518, 1518 },
    { "", CONFIG_NODE_RECORD, 1847, 2, 1519, 1523, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1854, 0, 1519, 1519, 1519 },
    { "arc", CONFIG_NODE_SCALAR, 1854, 0, 1520, 1520, 1520 },
    { "mods", CONFIG_NODE_SCALAR, 1854, 0, 1521, 1521, 1521 },
    { "ammo", CONFIG_NODE_SCALAR, 1854, 0, 1522, 1522, 1522 },
    { "amount", CONFIG_NODE_SCALAR, 1854, 0, 1523, 1523, 1523 },
    { "", CONFIG_NODE_RECORD, 1847, 3, 1524, 1528, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1860, 0, 1524, 1524, 1524 },
    { "arc", CONFIG_NODE_SCALAR, 1860, 0, 1525, 1525, 1525 },
    { "mods", CONFIG_NODE_SCALAR, 1860, 0, 1526, 1526, 1526 },
    { "ammo", CONFIG_NODE_SCALAR, 1860, 0, 1527, 1527, 1527 },
    { "amount", CONFIG_NODE_SCALAR, 1860, 0, 1528, 1528, 1528 },
    { "", CONFIG_NODE_RECORD, 1847, 4, 1529, 1533, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1866, 0, 1529, 1529, 1529 },
    { "arc", CONFIG_NODE_SCALAR, 1866, 0, 1530, 1530, 1530 },
    { "mods", CONFIG_NODE_SCALAR, 1866, 0, 1531, 1531, 1531 },
    { "ammo", CONFIG_NODE_SCALAR, 1866, 0, 1532, 1532, 1532 },
    { "amount", CONFIG_NODE_SCALAR, 1866, 0, 1533, 1533, 1533 },
    { "", CONFIG_NODE_RECORD, 1847, 5, 1534, 1538, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1872, 0, 1534, 1534, 1534 },
    { "arc", CONFIG_NODE_SCALAR, 1872, 0, 1535, 1535, 1535 },
    { "mods", CONFIG_NODE_SCALAR, 1872, 0, 1536, 1536, 1536 },
    { "ammo", CONFIG_NODE_SCALAR, 1872, 0, 1537, 1537, 1537 },
    { "amount", CONFIG_NODE_SCALAR, 1872, 0, 1538, 1538, 1538 },
    { "", CONFIG_NODE_RECORD, 1847, 6, 1539, 1543, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1878, 0, 1539, 1539, 1539 },
    { "arc", CONFIG_NODE_SCALAR, 1878, 0, 1540, 1540, 1540 },
    { "mods", CONFIG_NODE_SCALAR, 1878, 0, 1541, 1541, 1541 },
    { "ammo", CONFIG_NODE_SCALAR, 1878, 0, 1542, 1542, 1542 },
    { "amount", CONFIG_NODE_SCALAR, 1878, 0, 1543, 1543, 1543 },
    { "", CONFIG_NODE_RECORD, 1847, 7, 1544, 1548, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1884, 0, 1544, 1544, 1544 },
    { "arc", CONFIG_NODE_SCALAR, 1884, 0, 1545, 1545, 1545 },
    { "mods", CONFIG_NODE_SCALAR, 1884, 0, 1546, 1546, 1546 },
    { "ammo", CONFIG_NODE_SCALAR, 1884, 0, 1547, 1547, 1547 },
    { "amount", CONFIG_NODE_SCALAR, 1884, 0, 1548, 1548, 1548 },
    { "", CONFIG_NODE_RECORD, 1847, 8, 1549, 1553, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1890, 0, 1549, 1549, 1549 },
    { "arc", CONFIG_NODE_SCALAR, 1890, 0, 1550, 1550, 1550 },
    { "mods", CONFIG_NODE_SCALAR, 1890, 0, 1551, 1551, 1551 },
    { "ammo", CONFIG_NODE_SCALAR, 1890, 0, 1552, 1552, 1552 },
    { "amount", CONFIG_NODE_SCALAR, 1890, 0, 1553, 1553, 1553 },
    { "specials", CONFIG_NODE_SCALAR, 1840, 0, 1554, 1554, 1554 },
    { "", CONFIG_NODE_RECORD, 1839, 1, 1555, 1601, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1897, 0, 1555, 1555, 1555 },
    { "hull_size", CONFIG_NODE_SCALAR, 1897, 0, 1556, 1556, 1556 },
    { "drive", CONFIG_NODE_SCALAR, 1897, 0, 1557, 1557, 1557 },
    { "armor", CONFIG_NODE_SCALAR, 1897, 0, 1558, 1558, 1558 },
    { "shield", CONFIG_NODE_SCALAR, 1897, 0, 1559, 1559, 1559 },
    { "computer", CONFIG_NODE_SCALAR, 1897, 0, 1560, 1560, 1560 },
    { "weapons", CONFIG_NODE_ARRAY, 1897, 0, 1561, 1600, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1904, 1, 1561, 1565, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1905, 0, 1561, 1561, 1561 },
    { "arc", CONFIG_NODE_SCALAR, 1905, 0, 1562, 1562, 1562 },
    { "mods", CONFIG_NODE_SCALAR, 1905, 0, 1563, 1563, 1563 },
    { "ammo", CONFIG_NODE_SCALAR, 1905, 0, 1564, 1564, 1564 },
    { "amount", CONFIG_NODE_SCALAR, 1905, 0, 1565, 1565, 1565 },
    { "", CONFIG_NODE_RECORD, 1904, 2, 1566, 1570, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1911, 0, 1566, 1566, 1566 },
    { "arc", CONFIG_NODE_SCALAR, 1911, 0, 1567, 1567, 1567 },
    { "mods", CONFIG_NODE_SCALAR, 1911, 0, 1568, 1568, 1568 },
    { "ammo", CONFIG_NODE_SCALAR, 1911, 0, 1569, 1569, 1569 },
    { "amount", CONFIG_NODE_SCALAR, 1911, 0, 1570, 1570, 1570 },
    { "", CONFIG_NODE_RECORD, 1904, 3, 1571, 1575, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1917, 0, 1571, 1571, 1571 },
    { "arc", CONFIG_NODE_SCALAR, 1917, 0, 1572, 1572, 1572 },
    { "mods", CONFIG_NODE_SCALAR, 1917, 0, 1573, 1573, 1573 },
    { "ammo", CONFIG_NODE_SCALAR, 1917, 0, 1574, 1574, 1574 },
    { "amount", CONFIG_NODE_SCALAR, 1917, 0, 1575, 1575, 1575 },
    { "", CONFIG_NODE_RECORD, 1904, 4, 1576, 1580, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1923, 0, 1576, 1576, 1576 },
    { "arc", CONFIG_NODE_SCALAR, 1923, 0, 1577, 1577, 1577 },
    { "mods", CONFIG_NODE_SCALAR, 1923, 0, 1578, 1578, 1578 },
    { "ammo", CONFIG_NODE_SCALAR, 1923, 0, 1579, 1579, 1579 },
    { "amount", CONFIG_NODE_SCALAR, 1923, 0, 1580, 1580, 1580 },
    { "", CONFIG_NODE_RECORD, 1904, 5, 1581, 1585, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1929, 0, 1581, 1581, 1581 },
    { "arc", CONFIG_NODE_SCALAR, 1929, 0, 1582, 1582, 1582 },
    { "mods", CONFIG_NODE_SCALAR, 1929, 0, 1583, 1583, 1583 },
    { "ammo", CONFIG_NODE_SCALAR, 1929, 0, 1584, 1584, 1584 },
    { "amount", CONFIG_NODE_SCALAR, 1929, 0, 1585, 1585, 1585 },
    { "", CONFIG_NODE_RECORD, 1904, 6, 1586, 1590, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1935, 0, 1586, 1586, 1586 },
    { "arc", CONFIG_NODE_SCALAR, 1935, 0, 1587, 1587, 1587 },
    { "mods", CONFIG_NODE_SCALAR, 1935, 0, 1588, 1588, 1588 },
    { "ammo", CONFIG_NODE_SCALAR, 1935, 0, 1589, 1589, 1589 },
    { "amount", CONFIG_NODE_SCALAR, 1935, 0, 1590, 1590, 1590 },
    { "", CONFIG_NODE_RECORD, 1904, 7, 1591, 1595, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1941, 0, 1591, 1591, 1591 },
    { "arc", CONFIG_NODE_SCALAR, 1941, 0, 1592, 1592, 1592 },
    { "mods", CONFIG_NODE_SCALAR, 1941, 0, 1593, 1593, 1593 },
    { "ammo", CONFIG_NODE_SCALAR, 1941, 0, 1594, 1594, 1594 },
    { "amount", CONFIG_NODE_SCALAR, 1941, 0, 1595, 1595, 1595 },
    { "", CONFIG_NODE_RECORD, 1904, 8, 1596, 1600, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1947, 0, 1596, 1596, 1596 },
    { "arc", CONFIG_NODE_SCALAR, 1947, 0, 1597, 1597, 1597 },
    { "mods", CONFIG_NODE_SCALAR, 1947, 0, 1598, 1598, 1598 },
    { "ammo", CONFIG_NODE_SCALAR, 1947, 0, 1599, 1599, 1599 },
    { "amount", CONFIG_NODE_SCALAR, 1947, 0, 1600, 1600, 1600 },
    { "specials", CONFIG_NODE_SCALAR, 1897, 0, 1601, 1601, 1601 },
    { "", CONFIG_NODE_RECORD, 1839, 2, 1602, 1648, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 1954, 0, 1602, 1602, 1602 },
    { "hull_size", CONFIG_NODE_SCALAR, 1954, 0, 1603, 1603, 1603 },
    { "drive", CONFIG_NODE_SCALAR, 1954, 0, 1604, 1604, 1604 },
    { "armor", CONFIG_NODE_SCALAR, 1954, 0, 1605, 1605, 1605 },
    { "shield", CONFIG_NODE_SCALAR, 1954, 0, 1606, 1606, 1606 },
    { "computer", CONFIG_NODE_SCALAR, 1954, 0, 1607, 1607, 1607 },
    { "weapons", CONFIG_NODE_ARRAY, 1954, 0, 1608, 1647, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 1961, 1, 1608, 1612, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1962, 0, 1608, 1608, 1608 },
    { "arc", CONFIG_NODE_SCALAR, 1962, 0, 1609, 1609, 1609 },
    { "mods", CONFIG_NODE_SCALAR, 1962, 0, 1610, 1610, 1610 },
    { "ammo", CONFIG_NODE_SCALAR, 1962, 0, 1611, 1611, 1611 },
    { "amount", CONFIG_NODE_SCALAR, 1962, 0, 1612, 1612, 1612 },
    { "", CONFIG_NODE_RECORD, 1961, 2, 1613, 1617, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1968, 0, 1613, 1613, 1613 },
    { "arc", CONFIG_NODE_SCALAR, 1968, 0, 1614, 1614, 1614 },
    { "mods", CONFIG_NODE_SCALAR, 1968, 0, 1615, 1615, 1615 },
    { "ammo", CONFIG_NODE_SCALAR, 1968, 0, 1616, 1616, 1616 },
    { "amount", CONFIG_NODE_SCALAR, 1968, 0, 1617, 1617, 1617 },
    { "", CONFIG_NODE_RECORD, 1961, 3, 1618, 1622, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1974, 0, 1618, 1618, 1618 },
    { "arc", CONFIG_NODE_SCALAR, 1974, 0, 1619, 1619, 1619 },
    { "mods", CONFIG_NODE_SCALAR, 1974, 0, 1620, 1620, 1620 },
    { "ammo", CONFIG_NODE_SCALAR, 1974, 0, 1621, 1621, 1621 },
    { "amount", CONFIG_NODE_SCALAR, 1974, 0, 1622, 1622, 1622 },
    { "", CONFIG_NODE_RECORD, 1961, 4, 1623, 1627, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1980, 0, 1623, 1623, 1623 },
    { "arc", CONFIG_NODE_SCALAR, 1980, 0, 1624, 1624, 1624 },
    { "mods", CONFIG_NODE_SCALAR, 1980, 0, 1625, 1625, 1625 },
    { "ammo", CONFIG_NODE_SCALAR, 1980, 0, 1626, 1626, 1626 },
    { "amount", CONFIG_NODE_SCALAR, 1980, 0, 1627, 1627, 1627 },
    { "", CONFIG_NODE_RECORD, 1961, 5, 1628, 1632, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1986, 0, 1628, 1628, 1628 },
    { "arc", CONFIG_NODE_SCALAR, 1986, 0, 1629, 1629, 1629 },
    { "mods", CONFIG_NODE_SCALAR, 1986, 0, 1630, 1630, 1630 },
    { "ammo", CONFIG_NODE_SCALAR, 1986, 0, 1631, 1631, 1631 },
    { "amount", CONFIG_NODE_SCALAR, 1986, 0, 1632, 1632, 1632 },
    { "", CONFIG_NODE_RECORD, 1961, 6, 1633, 1637, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1992, 0, 1633, 1633, 1633 },
    { "arc", CONFIG_NODE_SCALAR, 1992, 0, 1634, 1634, 1634 },
    { "mods", CONFIG_NODE_SCALAR, 1992, 0, 1635, 1635, 1635 },
    { "ammo", CONFIG_NODE_SCALAR, 1992, 0, 1636, 1636, 1636 },
    { "amount", CONFIG_NODE_SCALAR, 1992, 0, 1637, 1637, 1637 },
    { "", CONFIG_NODE_RECORD, 1961, 7, 1638, 1642, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 1998, 0, 1638, 1638, 1638 },
    { "arc", CONFIG_NODE_SCALAR, 1998, 0, 1639, 1639, 1639 },
    { "mods", CONFIG_NODE_SCALAR, 1998, 0, 1640, 1640, 1640 },
    { "ammo", CONFIG_NODE_SCALAR, 1998, 0, 1641, 1641, 1641 },
    { "amount", CONFIG_NODE_SCALAR, 1998, 0, 1642, 1642, 1642 },
    { "", CONFIG_NODE_RECORD, 1961, 8, 1643, 1647, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2004, 0, 1643, 1643, 1643 },
    { "arc", CONFIG_NODE_SCALAR, 2004, 0, 1644, 1644, 1644 },
    { "mods", CONFIG_NODE_SCALAR, 2004, 0, 1645, 1645, 1645 },
    { "ammo", CONFIG_NODE_SCALAR, 2004, 0, 1646, 1646, 1646 },
    { "amount", CONFIG_NODE_SCALAR, 2004, 0, 1647, 1647, 1647 },
    { "specials", CONFIG_NODE_SCALAR, 1954, 0, 1648, 1648, 1648 },
    { "", CONFIG_NODE_RECORD, 1839, 3, 1649, 1695, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 2011, 0, 1649, 1649, 1649 },
    { "hull_size", CONFIG_NODE_SCALAR, 2011, 0, 1650, 1650, 1650 },
    { "drive", CONFIG_NODE_SCALAR, 2011, 0, 1651, 1651, 1651 },
    { "armor", CONFIG_NODE_SCALAR, 2011, 0, 1652, 1652, 1652 },
    { "shield", CONFIG_NODE_SCALAR, 2011, 0, 1653, 1653, 1653 },
    { "computer", CONFIG_NODE_SCALAR, 2011, 0, 1654, 1654, 1654 },
    { "weapons", CONFIG_NODE_ARRAY, 2011, 0, 1655, 1694, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 2018, 1, 1655, 1659, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2019, 0, 1655, 1655, 1655 },
    { "arc", CONFIG_NODE_SCALAR, 2019, 0, 1656, 1656, 1656 },
    { "mods", CONFIG_NODE_SCALAR, 2019, 0, 1657, 1657, 1657 },
    { "ammo", CONFIG_NODE_SCALAR, 2019, 0, 1658, 1658, 1658 },
    { "amount", CONFIG_NODE_SCALAR, 2019, 0, 1659, 1659, 1659 },
    { "", CONFIG_NODE_RECORD, 2018, 2, 1660, 1664, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2025, 0, 1660, 1660, 1660 },
    { "arc", CONFIG_NODE_SCALAR, 2025, 0, 1661, 1661, 1661 },
    { "mods", CONFIG_NODE_SCALAR, 2025, 0, 1662, 1662, 1662 },
    { "ammo", CONFIG_NODE_SCALAR, 2025, 0, 1663, 1663, 1663 },
    { "amount", CONFIG_NODE_SCALAR, 2025, 0, 1664, 1664, 1664 },
    { "", CONFIG_NODE_RECORD, 2018, 3, 1665, 1669, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2031, 0, 1665, 1665, 1665 },
    { "arc", CONFIG_NODE_SCALAR, 2031, 0, 1666, 1666, 1666 },
    { "mods", CONFIG_NODE_SCALAR, 2031, 0, 1667, 1667, 1667 },
    { "ammo", CONFIG_NODE_SCALAR, 2031, 0, 1668, 1668, 1668 },
    { "amount", CONFIG_NODE_SCALAR, 2031, 0, 1669, 1669, 1669 },
    { "", CONFIG_NODE_RECORD, 2018, 4, 1670, 1674, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2037, 0, 1670, 1670, 1670 },
    { "arc", CONFIG_NODE_SCALAR, 2037, 0, 1671, 1671, 1671 },
    { "mods", CONFIG_NODE_SCALAR, 2037, 0, 1672, 1672, 1672 },
    { "ammo", CONFIG_NODE_SCALAR, 2037, 0, 1673, 1673, 1673 },
    { "amount", CONFIG_NODE_SCALAR, 2037, 0, 1674, 1674, 1674 },
    { "", CONFIG_NODE_RECORD, 2018, 5, 1675, 1679, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2043, 0, 1675, 1675, 1675 },
    { "arc", CONFIG_NODE_SCALAR, 2043, 0, 1676, 1676, 1676 },
    { "mods", CONFIG_NODE_SCALAR, 2043, 0, 1677, 1677, 1677 },
    { "ammo", CONFIG_NODE_SCALAR, 2043, 0, 1678, 1678, 1678 },
    { "amount", CONFIG_NODE_SCALAR, 2043, 0, 1679, 1679, 1679 },
    { "", CONFIG_NODE_RECORD, 2018, 6, 1680, 1684, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2049, 0, 1680, 1680, 1680 },
    { "arc", CONFIG_NODE_SCALAR, 2049, 0, 1681, 1681, 1681 },
    { "mods", CONFIG_NODE_SCALAR, 2049, 0, 1682, 1682, 1682 },
    { "ammo", CONFIG_NODE_SCALAR, 2049, 0, 1683, 1683, 1683 },
    { "amount", CONFIG_NODE_SCALAR, 2049, 0, 1684, 1684, 1684 },
    { "", CONFIG_NODE_RECORD, 2018, 7, 1685, 1689, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2055, 0, 1685, 1685, 1685 },
    { "arc", CONFIG_NODE_SCALAR, 2055, 0, 1686, 1686, 1686 },
    { "mods", CONFIG_NODE_SCALAR, 2055, 0, 1687, 1687, 1687 },
    { "ammo", CONFIG_NODE_SCALAR, 2055, 0, 1688, 1688, 1688 },
    { "amount", CONFIG_NODE_SCALAR, 2055, 0, 1689, 1689, 1689 },
    { "", CONFIG_NODE_RECORD, 2018, 8, 1690, 1694, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2061, 0, 1690, 1690, 1690 },
    { "arc", CONFIG_NODE_SCALAR, 2061, 0, 1691, 1691, 1691 },
    { "mods", CONFIG_NODE_SCALAR, 2061, 0, 1692, 1692, 1692 },
    { "ammo", CONFIG_NODE_SCALAR, 2061, 0, 1693, 1693, 1693 },
    { "amount", CONFIG_NODE_SCALAR, 2061, 0, 1694, 1694, 1694 },
    { "specials", CONFIG_NODE_SCALAR, 2011, 0, 1695, 1695, 1695 },
    { "", CONFIG_NODE_RECORD, 1839, 4, 1696, 1742, CONFIG_NO_VARIABLE },
    { "appearance", CONFIG_NODE_SCALAR, 2068, 0, 1696, 1696, 1696 },
    { "hull_size", CONFIG_NODE_SCALAR, 2068, 0, 1697, 1697, 1697 },
    { "drive", CONFIG_NODE_SCALAR, 2068, 0, 1698, 1698, 1698 },
    { "armor", CONFIG_NODE_SCALAR, 2068, 0, 1699, 1699, 1699 },
    { "shield", CONFIG_NODE_SCALAR, 2068, 0, 1700, 1700, 1700 },
    { "computer", CONFIG_NODE_SCALAR, 2068, 0, 1701, 1701, 1701 },
    { "weapons", CONFIG_NODE_ARRAY, 2068, 0, 1702, 1741, CONFIG_NO_VARIABLE },
    { "", CONFIG_NODE_RECORD, 2075, 1, 1702, 1706, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2076, 0, 1702, 1702, 1702 },
    { "arc", CONFIG_NODE_SCALAR, 2076, 0, 1703, 1703, 1703 },
    { "mods", CONFIG_NODE_SCALAR, 2076, 0, 1704, 1704, 1704 },
    { "ammo", CONFIG_NODE_SCALAR, 2076, 0, 1705, 1705, 1705 },
    { "amount", CONFIG_NODE_SCALAR, 2076, 0, 1706, 1706, 1706 },
    { "", CONFIG_NODE_RECORD, 2075, 2, 1707, 1711, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2082, 0, 1707, 1707, 1707 },
    { "arc", CONFIG_NODE_SCALAR, 2082, 0, 1708, 1708, 1708 },
    { "mods", CONFIG_NODE_SCALAR, 2082, 0, 1709, 1709, 1709 },
    { "ammo", CONFIG_NODE_SCALAR, 2082, 0, 1710, 1710, 1710 },
    { "amount", CONFIG_NODE_SCALAR, 2082, 0, 1711, 1711, 1711 },
    { "", CONFIG_NODE_RECORD, 2075, 3, 1712, 1716, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2088, 0, 1712, 1712, 1712 },
    { "arc", CONFIG_NODE_SCALAR, 2088, 0, 1713, 1713, 1713 },
    { "mods", CONFIG_NODE_SCALAR, 2088, 0, 1714, 1714, 1714 },
    { "ammo", CONFIG_NODE_SCALAR, 2088, 0, 1715, 1715, 1715 },
    { "amount", CONFIG_NODE_SCALAR, 2088, 0, 1716, 1716, 1716 },
    { "", CONFIG_NODE_RECORD, 2075, 4, 1717, 1721, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2094, 0, 1717, 1717, 1717 },
    { "arc", CONFIG_NODE_SCALAR, 2094, 0, 1718, 1718, 1718 },
    { "mods", CONFIG_NODE_SCALAR, 2094, 0, 1719, 1719, 1719 },
    { "ammo", CONFIG_NODE_SCALAR, 2094, 0, 1720, 1720, 1720 },
    { "amount", CONFIG_NODE_SCALAR, 2094, 0, 1721, 1721, 1721 },
    { "", CONFIG_NODE_RECORD, 2075, 5, 1722, 1726, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2100, 0, 1722, 1722, 1722 },
    { "arc", CONFIG_NODE_SCALAR, 2100, 0, 1723, 1723, 1723 },
    { "mods", CONFIG_NODE_SCALAR, 2100, 0, 1724, 1724, 1724 },
    { "ammo", CONFIG_NODE_SCALAR, 2100, 0, 1725, 1725, 1725 },
    { "amount", CONFIG_NODE_SCALAR, 2100, 0, 1726, 1726, 1726 },
    { "", CONFIG_NODE_RECORD, 2075, 6, 1727, 1731, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2106, 0, 1727, 1727, 1727 },
    { "arc", CONFIG_NODE_SCALAR, 2106, 0, 1728, 1728, 1728 },
    { "mods", CONFIG_NODE_SCALAR, 2106, 0, 1729, 1729, 1729 },
    { "ammo", CONFIG_NODE_SCALAR, 2106, 0, 1730, 1730, 1730 },
    { "amount", CONFIG_NODE_SCALAR, 2106, 0, 1731, 1731, 1731 },
    { "", CONFIG_NODE_RECORD, 2075, 7, 1732, 1736, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2112, 0, 1732, 1732, 1732 },
    { "arc", CONFIG_NODE_SCALAR, 2112, 0, 1733, 1733, 1733 },
    { "mods", CONFIG_NODE_SCALAR, 2112, 0, 1734, 1734, 1734 },
    { "ammo", CONFIG_NODE_SCALAR, 2112, 0, 1735, 1735, 1735 },
    { "amount", CONFIG_NODE_SCALAR, 2112, 0, 1736, 1736, 1736 },
    { "", CONFIG_NODE_RECORD, 2075, 8, 1737, 1741, CONFIG_NO_VARIABLE },
    { "type", CONFIG_NODE_SCALAR, 2118, 0, 1737, 1737, 1737 },
    { "arc", CONFIG_NODE_SCALAR, 2118, 0, 1738, 1738, 1738 },
    { "mods", CONFIG_NODE_SCALAR, 2118, 0, 1739, 1739, 1739 },
    { "ammo", CONFIG_NODE_SCALAR, 2118, 0, 1740, 1740, 1740 },
    { "amount", CONFIG_NODE_SCALAR, 2118, 0, 1741, 1741, 1741 },
    { "specials", CONFIG_NODE_SCALAR, 2068, 0, 1742, 1742, 1742 },
};

const size_t schema_node_count = sizeof(schema_nodes) / sizeof(schema_nodes[0]);

}
