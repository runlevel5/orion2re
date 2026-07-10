#ifndef ORION2RE_SIZES_H
#define ORION2RE_SIZES_H

#define ORION2RE_STATIC_SIZE_ASSERT(type, expected_size) static_assert(sizeof(type) == (expected_size), #type " size mismatch")
#define ORION2RE_STATIC_SIZE_ASSERT_32_ONLY(type, expected_size) static_assert(ORION2RE_64BIT_BUILD || sizeof(type) == (expected_size), #type " size mismatch")
#define ORION2RE_STATIC_SIZE_ASSERT_PTR(type, size32, size64) static_assert(sizeof(type) == (ORION2RE_64BIT_BUILD ? (size64) : (size32)), #type " size mismatch")

ORION2RE_STATIC_SIZE_ASSERT(s_current_design, 0x125);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_main_menu, 0xd2, 0x11a);
ORION2RE_STATIC_SIZE_ASSERT(s_event_data, 9);
ORION2RE_STATIC_SIZE_ASSERT(s_movable_box, 0x1c);
ORION2RE_STATIC_SIZE_ASSERT(s_game_popup_fields, 0x278);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_game_anims, 0x65, 0xc9);
ORION2RE_STATIC_SIZE_ASSERT(s_nebula, 0x5);
ORION2RE_STATIC_SIZE_ASSERT(s_settings, 0x229);
ORION2RE_STATIC_SIZE_ASSERT(s_player, 0xea9);
ORION2RE_STATIC_SIZE_ASSERT(s_planet_data, 0x11);
ORION2RE_STATIC_SIZE_ASSERT(s_colony, 0x169);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_tech_building_data, 0x14, 0x18);
ORION2RE_STATIC_SIZE_ASSERT(s_star_data, 0x71);
ORION2RE_STATIC_SIZE_ASSERT(s_ship_data, 0x81);
ORION2RE_STATIC_SIZE_ASSERT(s_leader_data, 0x3b);
ORION2RE_STATIC_SIZE_ASSERT(s_antaran, 0x42);
ORION2RE_STATIC_SIZE_ASSERT(s_ship_icon, 12);
ORION2RE_STATIC_SIZE_ASSERT(s_ship_node, 5);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_skill_data, 0x18, 0x20);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_tech_fuel_data, 12, 16);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_tech_app_data, 16, 24);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_tech_field_data, 28, 32);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_tech_drive_data, 48, 56);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_tech_armor_data, 16, 24);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_tech_hull_data, 36, 48);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_tech_computer_data, 24, 32);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_tech_weapon_data, 28, 40);
ORION2RE_STATIC_SIZE_ASSERT(s_font, 0x235c);
ORION2RE_STATIC_SIZE_ASSERT(s_MSG, 0x12);
ORION2RE_STATIC_SIZE_ASSERT(s_report, 0x13);
static_assert(sizeof(REPORT::_reports) == 50 * sizeof(s_report), "REPORT::_reports size mismatch");
static_assert(sizeof(EVENTS::_event_data) == 36 * sizeof(s_event_data), "EVENTS::_event_data size mismatch");
static_assert(sizeof(MSG_::_msgs) == 100 * sizeof(s_MSG), "MSG_::_msgs size mismatch");
ORION2RE_STATIC_SIZE_ASSERT(s_palette_entry, 4);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_field, 0x35, 0x51);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_network, 0x20e, 0x216);
ORION2RE_STATIC_SIZE_ASSERT(s_net_player_info, 0xc);
ORION2RE_STATIC_SIZE_ASSERT(s_net_tactical_combat_message, 0x10);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_battle_results, 0x6ce, 0x6d6);
ORION2RE_STATIC_SIZE_ASSERT(s_load_net_screen_packet, 0x60);

ORION2RE_STATIC_SIZE_ASSERT(AIDATA::s_turns_from_to_intermediate_cache_entry, 4);
ORION2RE_STATIC_SIZE_ASSERT(AIDATA::s_ship_n_turns_to_intermediate_cache_entry, 2);
ORION2RE_STATIC_SIZE_ASSERT(s_npcdiplo_tech, 5);
ORION2RE_STATIC_SIZE_ASSERT(s_combat_weapon_slot, 11);
ORION2RE_STATIC_SIZE_ASSERT(s_missile, 0x1A);
ORION2RE_STATIC_SIZE_ASSERT(s_combfind_star_has_player, 0xb);
ORION2RE_STATIC_SIZE_ASSERT(s_combfind_target, 0x2c);

ORION2RE_STATIC_SIZE_ASSERT(s_player_ship_sizes, 0x38);
ORION2RE_STATIC_SIZE_ASSERT(s_ai_refit_entry, 6);
ORION2RE_STATIC_SIZE_ASSERT(s_ai_refit_player, 3002);
ORION2RE_STATIC_SIZE_ASSERT(s_animation_header, 0x0c);
ORION2RE_STATIC_SIZE_ASSERT(s_colony_job_production, 0x256);
ORION2RE_STATIC_SIZE_ASSERT(s_help_record, 0x57b);
ORION2RE_STATIC_SIZE_ASSERT(s_palette_header, 4);
ORION2RE_STATIC_SIZE_ASSERT(s_lbx_stream_entry, 8);
ORION2RE_STATIC_SIZE_ASSERT(s_lbx_disk_header, 8);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_lbx_stream_header, 0x104c, 0x1050);
ORION2RE_STATIC_SIZE_ASSERT_PTR(s_wave_stream_header, 0x104c, 0x1050);
ORION2RE_STATIC_SIZE_ASSERT(s_packed_frame_header, 4);
ORION2RE_STATIC_SIZE_ASSERT(s_packed_frame_run, 4);
ORION2RE_STATIC_SIZE_ASSERT(s_packed_screen_background_run32, 8);
ORION2RE_STATIC_SIZE_ASSERT(s_packed_screen_background_run8, 2);

#undef ORION2RE_STATIC_SIZE_ASSERT
#undef ORION2RE_STATIC_SIZE_ASSERT_32_ONLY
#undef ORION2RE_STATIC_SIZE_ASSERT_PTR

#endif