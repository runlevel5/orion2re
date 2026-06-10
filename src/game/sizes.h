#ifndef ORION2RE_SIZES_H
#define ORION2RE_SIZES_H

static_assert(sizeof(s_current_design) == 0x125, "s_current_design size mismatch");
static_assert(sizeof(s_main_menu) == 0xd2, "s_main_menu size mismatch");
static_assert(sizeof(s_event_data) == 9, "s_event_data size mismatch");
static_assert(sizeof(s_movable_box) == 0x1c, "s_movable_box size mismatch");
static_assert(sizeof(s_game_popup_fields) == 0x278, "s_game_popup_fields size mismatch");
static_assert(sizeof(s_game_anims) == 0x65, "s_game_anims size mismatch");
static_assert(sizeof(s_nebula) == 0x5, "s_nebula size mismatch");
static_assert(sizeof(s_settings) == 0x229, "s_settings size mismatch");
static_assert(sizeof(s_player) == 0xea9, "s_player size mismatch");
static_assert(sizeof(s_planet_data) == 0x11, "s_planet_data size mismatch");
static_assert(sizeof(s_colony) == 0x169, "s_colony size mismatch");
static_assert(sizeof(s_tech_building_data) == 0x13, "s_tech_building_data size mismatch");
static_assert(sizeof(s_star_data) == 0x71, "s_star_data size mismatch");
static_assert(sizeof(s_ship_data) == 0x81, "s_ship_data size mismatch");
static_assert(sizeof(s_leader_data) == 0x3b, "s_leader_data size mismatch");
static_assert(sizeof(s_ship_icon) == 12, "s_ship_icon size mismatch");
static_assert(sizeof(s_ship_node) == 5, "s_ship_node size mismatch");
static_assert(sizeof(s_skill_data) == 0x12, "s_skill_data size mismatch");
static_assert(sizeof(s_tech_fuel_data) == 10, "s_tech_fuel_data size mismatch");
static_assert(sizeof(s_tech_app_data) == 13, "s_tech_app_data size mismatch");
static_assert(sizeof(s_tech_field_data) == 0x17, "s_tech_field_data size mismatch");
static_assert(sizeof(s_tech_drive_data) == 0x2e, "s_tech_drive_data size mismatch");
static_assert(sizeof(s_tech_armor_data) == 15, "s_tech_armor_data size mismatch");
static_assert(sizeof(s_tech_hull_data) == 36, "s_tech_hull_data size mismatch");
static_assert(sizeof(s_tech_computer_data) == 22, "s_tech_computer_data size mismatch");
static_assert(sizeof(s_tech_weapon_data) == 28, "s_tech_weapon_data size mismatch");
static_assert(sizeof(s_font) == 0x235c, "s_font size mismatch");
static_assert(sizeof(s_MSG) == 0x12, "s_MSG size mismatch");
static_assert(sizeof(s_palette_entry) == 4, "s_palette_entry size mismatch");
static_assert(sizeof(s_field) == 0x37, "s_field size mismatch");
static_assert(sizeof(s_mp_screen) == 0x1eb, "s_mp_screen size mismatch");
static_assert(sizeof(s_network) == 0x20a, "s_network size mismatch");
static_assert(sizeof(s_net_player_info) == 0xc, "s_net_player_info size mismatch");
static_assert(sizeof(s_net_tactical_combat_message) == 0x10, "s_net_tactical_combat_message size mismatch");
static_assert(sizeof(s_battle_results) == 0x6ce, "s_battle_results size mismatch");
static_assert(sizeof(s_load_net_screen_packet) == 0x60, "s_load_net_screen_packet size mismatch");

static_assert(sizeof(AIDATA::s_turns_from_to_intermediate_cache_entry) == 4);
static_assert(sizeof(AIDATA::s_ship_n_turns_to_intermediate_cache_entry) == 2);
static_assert(sizeof(s_npcdiplo_tech) == 5, "s_npcdiplo_tech size mismatch");
static_assert(sizeof(s_combat_weapon_slot) == 11);
static_assert(sizeof(s_missile) == 0x1A);

static_assert(sizeof(s_player_ship_sizes) == 0x38, "s_player_ship_sizes size mismatch");
static_assert(sizeof(s_ai_refit_entry) == 6, "s_ai_refit_entry size mismatch");
static_assert(sizeof(s_ai_refit_player) == 3002, "s_ai_refit_player size mismatch");
static_assert(sizeof(s_animation_header) == 0x0c, "s_animation_header size mismatch");
static_assert(sizeof(s_colony_job_production) == 0x256, "s_colony_job_production size mismatch");
static_assert(sizeof(s_help_record) == 0x57b, "s_help_record size mismatch");
static_assert(sizeof(s_palette_header) == 4, "s_palette_header size mismatch");
static_assert(sizeof(s_packed_frame_header) == 4, "s_packed_frame_header size mismatch");
static_assert(sizeof(s_packed_frame_run) == 4, "s_packed_frame_run size mismatch");
static_assert(sizeof(s_packed_screen_background_run32) == 8, "s_packed_screen_background_run32 size mismatch");
static_assert(sizeof(s_packed_screen_background_run8) == 2, "s_packed_screen_background_run8 size mismatch");

#endif