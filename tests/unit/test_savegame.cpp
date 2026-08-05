#include "pch.h"

#include "doctest/doctest.h"

#include "game/savegame.h"

#include <cstdio>
#include <cstring>

namespace {
    constexpr size_t LEGACY_PLANET_SIZE = sizeof(s_planet_data) - sizeof(int8_t);
    constexpr size_t LEGACY_STAR_SIZE = sizeof(s_star_data) -
        (BITMAP(MAX_STARS) - BITMAP(ORIGINAL_MAX_STARS)) - 2 * sizeof(int8_t);

    struct State_ {
        s_colony colonies[MAX_COLONIES] = {};
        s_planet_data planets[MAX_PLANETS] = {};
        s_star_data stars[MAX_STARS] = {};
        s_leader_data leaders[MAX_LEADERS] = {};
        s_player players[MAX_PLAYERS] = {};
        s_ship_data ships[MAX_SHIPS] = {};
        s_nebula nebulas[MAX_NEBULAS] = {};
    };

    void Fill_Bytes_(void* data, size_t size, uint32_t seed) {
        uint8_t* bytes = static_cast<uint8_t*>(data);
        uint32_t value = seed;
        for (size_t i = 0; i < size; ++i) {
            value = value * 1664525u + 1013904223u;
            bytes[i] = static_cast<uint8_t>(value >> 24);
        }
    }

    void Bind_State_(State_& state) {
        MOX::_colony = state.colonies;
        MOX::_planet = state.planets;
        MOX::_star = state.stars;
        MOX::_leaders = state.leaders;
        MOX::_player = state.players;
        MOX::_ship = state.ships;
        MOX::_nebula = state.nebulas;
    }

    bool Is_Unused_Ship_(const s_ship_data& ship) {
        s_ship_data expected = {};
        expected.status = 5;
        expected.location = -1;
        expected.officer_index = -1;
        return memcmp(&ship, &expected, sizeof(ship)) == 0;
    }

    bool Is_Unused_Colony_(const s_colony& colony) {
        s_colony expected = {};
        expected.owner = -1;
        expected.allocated_to = -1;
        expected.planet = -1;
        expected.officer = -1;
        return memcmp(&colony, &expected, sizeof(colony)) == 0;
    }

    bool Is_Unused_Planet_(const s_planet_data& planet) {
        s_planet_data expected = {};
        expected.colony_index = -1;
        expected.star_index = -1;
        return memcmp(&planet, &expected, sizeof(planet)) == 0;
    }

    bool Is_Unused_Star_(const s_star_data& star) {
        s_star_data expected = {};
        expected.owner = -1;
        expected.colonize_player = -1;
        expected.wormhole_star_id = -1;
        expected.next_wfi_in_list = -1;
        memset(expected.officer_index, 0xFF, sizeof(expected.officer_index));
        for (int16_t& planet_index : expected.planet_index) {
            planet_index = -1;
        }
        for (int16_t& relocation : expected.relocate_ship_to) {
            relocation = -1;
        }
        memset(expected.surrender_to, 0xFF, sizeof(expected.surrender_to));
        return memcmp(&star, &expected, sizeof(star)) == 0;
    }

    void Fill_Messages_() {
        memset(MSG_::_msgs, 0, sizeof(MSG_::_msgs));
        for (int i = 0; i < MAX_MESSAGES; ++i) {
            s_MSG& message = MSG_::_msgs[i];
            message.msg_type = static_cast<int8_t>(
                i < MSG_TYPE_SCRAP_LEADER + 1 ? i :
                i < MSG_TYPE_SCRAP_LEADER + 13 ? MSG_TYPE_COLONY :
                i % (MSG_TYPE_SCRAP_LEADER + 1));
            message.priority = static_cast<uint8_t>(i * 3);
            switch (message.msg_type) {
            case MSG_TYPE_COLONY:
                message.u.colony.planet_idx = static_cast<uint8_t>(i);
                message.u.colony.subtype = static_cast<int8_t>(
                    i >= MSG_TYPE_SCRAP_LEADER + 1 && i < MSG_TYPE_SCRAP_LEADER + 13 ?
                    i - (MSG_TYPE_SCRAP_LEADER + 1) : i % 12);
                switch (message.u.colony.subtype) {
                case 0:
                    message.u.colony.c.scrap.building_id = static_cast<int16_t>(100 + i);
                    message.u.colony.c.scrap.bc = static_cast<int16_t>(200 + i);
                    break;
                case 1:
                    message.u.colony.c.build.build_id = static_cast<int16_t>(100 + i);
                    message.u.colony.c.build.product_id = static_cast<int16_t>(200 + i);
                    message.u.colony.c.build.is_refit = (i & 1) != 0;
                    message.u.colony.c.build.append_fmt = static_cast<int8_t>(i);
                    break;
                case 2:
                case 3:
                    message.u.colony.c.assimilation.val = static_cast<uint8_t>(i);
                    break;
                case 4:
                    message.u.colony.c.starvation.a = static_cast<int8_t>(i);
                    message.u.colony.c.starvation.b = static_cast<int8_t>(i + 1);
                    break;
                case 5:
                    message.u.colony.c.settlers.settlers = static_cast<int8_t>(i);
                    message.u.colony.c.settlers.over_limit = static_cast<int8_t>(i + 1);
                    message.u.colony.c.settlers.blockaded = (i & 1) != 0;
                    break;
                case 9:
                    message.u.colony.c.rebels.owner_player = static_cast<int8_t>(i % MAX_PLAYERS);
                    message.u.colony.c.rebels.rebels = static_cast<int8_t>(i);
                    message.u.colony.c.rebels.lost = static_cast<int8_t>(i / 2);
                    break;
                case 11:
                    message.u.colony.c.android_killed.race_idx = static_cast<int8_t>(i % MAX_PLAYERS);
                    break;
                default:
                    break;
                }
                break;
            case MSG_TYPE_DECLARE_ALLIANCE:
            case MSG_TYPE_DECLARE_WAR:
                message.u.players.player_a_idx = static_cast<int16_t>(i % MAX_PLAYERS);
                message.u.players.player_b_idx = static_cast<int16_t>((i + 1) % MAX_PLAYERS);
                break;
            case MSG_TYPE_FINANCIAL:
                message.u.financial.a = static_cast<int16_t>(i * 2);
                message.u.financial.b = static_cast<int16_t>(i * 3);
                break;
            case MSG_TYPE_LOST_SHIP:
                message.u.lost_ship.star_idx = static_cast<int16_t>(i % MAX_STARS);
                message.u.lost_ship.ship_idx = static_cast<int16_t>(i);
                message.u.lost_ship.leader_idx = static_cast<int8_t>(i % MAX_LEADERS);
                message.u.lost_ship.fate = static_cast<int8_t>(i % 4);
                break;
            case MSG_TYPE_SCRAP_SHIP:
                message.u.scrap_ship.where = static_cast<int16_t>(i % MAX_STARS);
                message.u.scrap_ship.bc = static_cast<int16_t>(i * 5);
                message.u.scrap_ship.ship_idx = static_cast<int16_t>(i);
                break;
            case MSG_TYPE_LOST_COLONY:
                message.u.lost_colony.planet_idx = static_cast<int16_t>(i);
                message.u.lost_colony.reason = static_cast<int16_t>(i % 4);
                message.u.lost_colony.leader_idx = static_cast<int16_t>(i % MAX_LEADERS);
                message.u.lost_colony.colony_or_outpost = static_cast<int8_t>(i & 1);
                break;
            case MSG_TYPE_HYPERSPACE_BEAST:
                message.u.hyperspace_beast.ship_idx = static_cast<int16_t>(i);
                message.u.hyperspace_beast.star_idx = static_cast<int16_t>(i % ORIGINAL_MAX_STARS);
                message.u.hyperspace_beast.officer_idx = static_cast<int8_t>(i % MAX_LEADERS);
                message.u.hyperspace_beast.leader_status = static_cast<int8_t>(i % 6);
                message.u.hyperspace_beast.fate = static_cast<int8_t>(i % 4);
                message.u.hyperspace_beast.your_ship = static_cast<int8_t>(i & 1);
                break;
            case MSG_TYPE_LOST_ATTACKING_SPY:
                message.u.lost_attacking_spy.player_idx = static_cast<int16_t>(i % MAX_PLAYERS);
                break;
            case MSG_TYPE_LOST_DEFENDING_SPY:
                message.u.lost_defending_spy.count = static_cast<int16_t>(i);
                break;
            case MSG_TYPE_ARTEMIS:
                message.u.artemis.star_idx = static_cast<int16_t>(i % MAX_STARS);
                break;
            case MSG_TYPE_ARTEMIS_LEADER:
                message.u.artemis_leader.star_idx = static_cast<int16_t>(i % ORIGINAL_MAX_STARS);
                message.u.artemis_leader.leader_idx = static_cast<int8_t>(i % MAX_LEADERS);
                message.u.artemis_leader.fate = static_cast<int8_t>(i % 4);
                break;
            case MSG_TYPE_FAILED_RETREAT:
                message.u.retreat.star_idx = static_cast<int16_t>(i % ORIGINAL_MAX_STARS);
                message.u.retreat.reason = static_cast<int8_t>(i % 4);
                message.u.retreat.ships_lost = static_cast<int16_t>(i);
                message.u.retreat.leader_count = static_cast<int8_t>(i % 5);
                break;
            case MSG_TYPE_SPY_ASSASSINATION:
                message.u.spy_assasination.plr_a = static_cast<int8_t>(i % MAX_PLAYERS);
                message.u.spy_assasination.plr_b = static_cast<int8_t>((i + 1) % MAX_PLAYERS);
                message.u.spy_assasination.leader_id = static_cast<int8_t>(i % MAX_LEADERS);
                message.u.spy_assasination.victim_id = static_cast<int8_t>((i + 2) % MAX_PLAYERS);
                break;
            case MSG_TYPE_ANOMALY:
                message.u.anomaly.star_idx = static_cast<int16_t>(i % ORIGINAL_MAX_STARS);
                break;
            case MSG_TYPE_DIPLOMACY:
                message.u.diplomacy.diplomacy_idx = static_cast<int8_t>(i % 6);
                message.u.diplomacy.plr_a = static_cast<int8_t>(i % MAX_PLAYERS);
                message.u.diplomacy.plr_b = static_cast<int8_t>((i + 1) % MAX_PLAYERS);
                break;
            case MSG_TYPE_EEL_SPAWNED:
            case MSG_TYPE_EEL_VANISHED:
                message.u.eel.star_idx = static_cast<int16_t>(i % ORIGINAL_MAX_STARS);
                break;
            case MSG_TYPE_ARTIFACTS:
                message.u.artifacts.star_idx = static_cast<int16_t>(i % ORIGINAL_MAX_STARS);
                message.u.artifacts.tech1 = static_cast<int8_t>(i);
                message.u.artifacts.tech2 = static_cast<int8_t>(i + 1);
                break;
            case MSG_TYPE_SCRAP_LEADER:
                message.u.scrap_leader.leader_idx = static_cast<int8_t>(i % MAX_LEADERS);
                break;
            default:
                break;
            }
        }
    }

    void Fill_Reports_() {
        memset(REPORT::_reports, 0, sizeof(REPORT::_reports));
        for (int i = 0; i < MAX_REPORTS; ++i) {
            s_report& report = REPORT::_reports[i];
            report.type = static_cast<uint8_t>((i % 2) + 1);
            report.action = static_cast<uint8_t>((i / 2) % 2);
            report.player_idx = static_cast<int16_t>(i % MAX_PLAYERS);
            Fill_Bytes_(&report.d, sizeof(report.d), 0x56780000u + static_cast<uint32_t>(i));
            const uint16_t legacy_value = static_cast<uint16_t>(i * 17);
            report.reserved[8] = static_cast<uint8_t>(legacy_value);
            report.reserved[9] = static_cast<uint8_t>(legacy_value >> 8);
            report.reserved[10] = static_cast<uint8_t>(i * 5);
        }
    }

    void Fill_State_(State_& state) {
        Fill_Bytes_(state.colonies, sizeof(state.colonies), 1);
        Fill_Bytes_(state.planets, sizeof(state.planets), 2);
        Fill_Bytes_(state.stars, sizeof(state.stars), 3);
        Fill_Bytes_(state.leaders, sizeof(state.leaders), 4);
        Fill_Bytes_(state.players, sizeof(state.players), 5);
        Fill_Bytes_(state.ships, sizeof(state.ships), 6);
        Fill_Bytes_(state.nebulas, sizeof(state.nebulas), 7);
        for (int i = 0; i < MAX_NEBULAS; ++i) {
            state.nebulas[i].type = static_cast<int8_t>(i % 12);
        }
        Fill_Bytes_(&MOX::_settings, sizeof(MOX::_settings), 8);
        Fill_Bytes_(&MOX::_antarans, sizeof(MOX::_antarans), 9);
        Fill_Bytes_(MOX::_moveable_box, sizeof(MOX::_moveable_box), 10);
        Fill_Bytes_(MOX::_moveable_box_order, sizeof(MOX::_moveable_box_order), 11);
        Fill_Bytes_(MOX::_bill_savegame, sizeof(MOX::_bill_savegame), 12);
        Fill_Bytes_(REPORT::_something_interesting_happened, sizeof(REPORT::_something_interesting_happened), 13);
        Fill_Bytes_(REPORT::_done_colonize_check, sizeof(REPORT::_done_colonize_check), 14);
        Fill_Bytes_(EVENTS::_event_data, sizeof(EVENTS::_event_data), 15);
        Fill_Bytes_(MSG_::_first_msg, sizeof(MSG_::_first_msg), 16);
        Fill_Bytes_(MSG_::_max_msgs, sizeof(MSG_::_max_msgs), 17);
        Fill_Bytes_(MSG_::_n_msgs, sizeof(MSG_::_n_msgs), 18);
        Fill_Reports_();
        Fill_Messages_();

        MOX::_NUM_COLONIES = 700;
        MOX::_planet_count = 900;
        MOX::_NUM_STARS = 512;
        MOX::_NUM_PLAYERS = 8;
        MOX::_NUM_SHIPS = 1000;
        MOX::_PLAYER_NUM = 7;
        MOX::_max_map_scale = 31;
        MOX::_max_zoom_count = 19;
        MOX::_star_bg_index = 6;
        MOX::_MAP_MAX_X = 311;
        MOX::_MAP_MAX_Y = 177;
        MOX::_fleet_icon_button_count = 421;
        MOX::_NUM_NEBULAS = MAX_NEBULAS;
        REPORT::_n_reports = MAX_REPORTS;
        REPORT::_done_player_reports = 0xA5;
        REPORT::_displayed_msgs = 0x5A;
        REPORT::_displayed_gnn = 0xC3;
        diplodef::_COUNCIL_FLAG = 3;
        diplodef::_next_council_meeting_turn = 1777;
        MOX::_antaran_resource_level = 4321;
        MOX::_chance_for_antaran_invasion = 1234;
        MOX::_disallow_saves_code = 0xBEEF;
        EVENTS::_last_event_year = 0x12345678;
        EVENTS::_event_delay_count = -123;
        MOX::_cur_map_scale = 23;
        MOX::_cur_map_x = -144;
        MOX::_cur_map_y = 155;
        MOX::_stardate = 35123;
        MOX::_game_type = 2;
    }

    void Clear_State_(State_& state) {
        memset(&state, 0, sizeof(state));
        memset(&MOX::_settings, 0, sizeof(MOX::_settings));
        memset(&MOX::_antarans, 0, sizeof(MOX::_antarans));
        memset(MOX::_moveable_box, 0, sizeof(MOX::_moveable_box));
        memset(MOX::_moveable_box_order, 0, sizeof(MOX::_moveable_box_order));
        memset(MOX::_bill_savegame, 0, sizeof(MOX::_bill_savegame));
        memset(REPORT::_reports, 0, sizeof(REPORT::_reports));
        memset(REPORT::_something_interesting_happened, 0, sizeof(REPORT::_something_interesting_happened));
        memset(REPORT::_done_colonize_check, 0, sizeof(REPORT::_done_colonize_check));
        memset(EVENTS::_event_data, 0, sizeof(EVENTS::_event_data));
        memset(MSG_::_msgs, 0, sizeof(MSG_::_msgs));
        memset(MSG_::_first_msg, 0, sizeof(MSG_::_first_msg));
        memset(MSG_::_max_msgs, 0, sizeof(MSG_::_max_msgs));
        memset(MSG_::_n_msgs, 0, sizeof(MSG_::_n_msgs));
        MOX::_NUM_COLONIES = 0;
        MOX::_planet_count = 0;
        MOX::_NUM_STARS = 0;
        MOX::_NUM_PLAYERS = 0;
        MOX::_NUM_SHIPS = 0;
        MOX::_PLAYER_NUM = 0;
        MOX::_max_map_scale = 0;
        MOX::_max_zoom_count = 0;
        MOX::_star_bg_index = 0;
        MOX::_MAP_MAX_X = 0;
        MOX::_MAP_MAX_Y = 0;
        MOX::_fleet_icon_button_count = 0;
        MOX::_NUM_NEBULAS = 0;
        REPORT::_n_reports = 0;
        REPORT::_done_player_reports = 0;
        REPORT::_displayed_msgs = 0;
        REPORT::_displayed_gnn = 0;
        diplodef::_COUNCIL_FLAG = 0;
        diplodef::_next_council_meeting_turn = 0;
        MOX::_antaran_resource_level = 0;
        MOX::_chance_for_antaran_invasion = 0;
        MOX::_disallow_saves_code = 0;
        EVENTS::_last_event_year = 0;
        EVENTS::_event_delay_count = 0;
        MOX::_cur_map_scale = 0;
        MOX::_cur_map_x = 0;
        MOX::_cur_map_y = 0;
    }

    bool Is_Zero_(const void* data, size_t size) {
        const uint8_t* bytes = static_cast<const uint8_t*>(data);
        for (size_t i = 0; i < size; ++i) {
            if (bytes[i] != 0) return false;
        }
        return true;
    }

    void Expand_Legacy_Planets_(s_planet_data* planets) {
        uint8_t legacy[ORIGINAL_MAX_PLANETS * LEGACY_PLANET_SIZE];
        Fill_Bytes_(legacy, sizeof(legacy), 2);
        const size_t star_offset = offsetof(s_planet_data, star_index);
        for (size_t i = 0; i < ORIGINAL_MAX_PLANETS; ++i) {
            const uint8_t* source = legacy + i * LEGACY_PLANET_SIZE;
            uint8_t* destination = reinterpret_cast<uint8_t*>(&planets[i]);
            memcpy(destination, source, star_offset);
            planets[i].star_index = static_cast<int8_t>(source[star_offset]);
            memcpy(destination + star_offset + sizeof(planets[i].star_index),
                   source + star_offset + sizeof(int8_t),
                   LEGACY_PLANET_SIZE - star_offset - sizeof(int8_t));
        }
    }

    void Expand_Legacy_Stars_(s_star_data* stars) {
        uint8_t legacy[ORIGINAL_MAX_STARS * LEGACY_STAR_SIZE];
        Fill_Bytes_(legacy, sizeof(legacy), 3);
        const size_t black_hole_offset = offsetof(s_star_data, black_hole_blocks);
        const size_t legacy_black_hole_size = BITMAP(ORIGINAL_MAX_STARS);
        const size_t expanded_black_hole_size =
            BITMAP(MAX_STARS) - legacy_black_hole_size;
        const size_t legacy_wormhole_offset =
            offsetof(s_star_data, wormhole_star_id) - expanded_black_hole_size;
        const size_t legacy_next_wfi_offset =
                offsetof(s_star_data, next_wfi_in_list) - expanded_black_hole_size - sizeof(int8_t);
        for (size_t i = 0; i < ORIGINAL_MAX_STARS; ++i) {
            const uint8_t* source = legacy + i * LEGACY_STAR_SIZE;
            uint8_t* destination = reinterpret_cast<uint8_t*>(&stars[i]);
            memcpy(destination, source, black_hole_offset + legacy_black_hole_size);
            memset(destination + black_hole_offset + legacy_black_hole_size, 0,
                   expanded_black_hole_size);
            memcpy(destination + black_hole_offset + BITMAP(MAX_STARS),
                   source + black_hole_offset + legacy_black_hole_size,
                   legacy_wormhole_offset - black_hole_offset - legacy_black_hole_size);
            stars[i].wormhole_star_id = static_cast<int8_t>(source[legacy_wormhole_offset]);
            memcpy(destination + offsetof(s_star_data, wormhole_star_id) +
                       sizeof(stars[i].wormhole_star_id),
                   source + legacy_wormhole_offset + sizeof(int8_t),
                   legacy_next_wfi_offset - legacy_wormhole_offset - sizeof(int8_t));
            stars[i].next_wfi_in_list = source[legacy_next_wfi_offset];
            memcpy(destination + offsetof(s_star_data, next_wfi_in_list) +
                       sizeof(stars[i].next_wfi_in_list),
                   source + legacy_next_wfi_offset + sizeof(uint8_t),
                   LEGACY_STAR_SIZE - legacy_next_wfi_offset - sizeof(uint8_t));
        }
    }

    void Check_Versioned_State_(const State_& expected, const State_& actual) {
        DOCTEST_CHECK(memcmp(expected.colonies, actual.colonies,
                             MOX::_NUM_COLONIES * sizeof(*expected.colonies)) == 0);
        for (int i = MOX::_NUM_COLONIES; i < MAX_COLONIES; ++i) {
            DOCTEST_CHECK(Is_Unused_Colony_(actual.colonies[i]));
        }
        DOCTEST_CHECK(memcmp(expected.planets, actual.planets,
                             MOX::_planet_count * sizeof(*expected.planets)) == 0);
        for (int i = MOX::_planet_count; i < MAX_PLANETS; ++i) {
            DOCTEST_CHECK(Is_Unused_Planet_(actual.planets[i]));
        }
        DOCTEST_CHECK(memcmp(expected.stars, actual.stars,
                             MOX::_NUM_STARS * sizeof(*expected.stars)) == 0);
        for (int i = MOX::_NUM_STARS; i < MAX_STARS; ++i) {
            DOCTEST_CHECK(Is_Unused_Star_(actual.stars[i]));
        }
        DOCTEST_CHECK(memcmp(expected.leaders, actual.leaders, sizeof(expected.leaders)) == 0);
        DOCTEST_CHECK(memcmp(expected.players, actual.players, sizeof(expected.players)) == 0);
        DOCTEST_CHECK(memcmp(expected.ships, actual.ships,
                             MOX::_NUM_SHIPS * sizeof(*expected.ships)) == 0);
        for (int i = MOX::_NUM_SHIPS; i < MAX_SHIPS; ++i) {
            DOCTEST_CHECK(Is_Unused_Ship_(actual.ships[i]));
        }
        DOCTEST_CHECK(memcmp(expected.nebulas, actual.nebulas, sizeof(expected.nebulas)) == 0);
    }

    void Check_Legacy_State_Except_Players_(const State_& expected, const State_& actual) {
        s_planet_data expected_planets[MAX_PLANETS] = {};
        s_star_data expected_stars[MAX_STARS] = {};
        s_nebula expected_nebulas[MAX_NEBULAS] = {};
        Expand_Legacy_Planets_(expected_planets);
        Expand_Legacy_Stars_(expected_stars);
        Fill_Bytes_(expected_nebulas, sizeof(expected_nebulas), 7);
        DOCTEST_CHECK(memcmp(expected.colonies, actual.colonies,
                             ORIGINAL_MAX_COLONIES * sizeof(*expected.colonies)) == 0);
        for (int i = ORIGINAL_MAX_COLONIES; i < MAX_COLONIES; ++i) {
            DOCTEST_CHECK(Is_Unused_Colony_(actual.colonies[i]));
        }
        DOCTEST_CHECK(memcmp(expected_planets, actual.planets,
                             ORIGINAL_MAX_PLANETS * sizeof(*expected_planets)) == 0);
        for (int i = ORIGINAL_MAX_PLANETS; i < MAX_PLANETS; ++i) {
            DOCTEST_CHECK(Is_Unused_Planet_(actual.planets[i]));
        }
        DOCTEST_CHECK(memcmp(expected_stars, actual.stars,
                             ORIGINAL_MAX_STARS * sizeof(*expected_stars)) == 0);
        for (int i = ORIGINAL_MAX_STARS; i < MAX_STARS; ++i) {
            DOCTEST_CHECK(Is_Unused_Star_(actual.stars[i]));
        }
        DOCTEST_CHECK(memcmp(expected.leaders, actual.leaders, sizeof(expected.leaders)) == 0);
        int converted_locations = 0;
        for (int i = 0; i < ORIGINAL_MAX_SHIPS; ++i) {
            s_ship_data expected_ship = expected.ships[i];
            const int16_t old_location = expected_ship.location;
            expected_ship.location = SAVEGAME::Convert_Legacy_Ship_Location_(old_location);
            converted_locations += expected_ship.location != old_location;
            DOCTEST_CHECK(memcmp(&expected_ship, &actual.ships[i], sizeof(expected_ship)) == 0);
        }
        for (int i = ORIGINAL_MAX_SHIPS; i < MAX_SHIPS; ++i) {
            DOCTEST_CHECK(Is_Unused_Ship_(actual.ships[i]));
        }
        DOCTEST_CHECK(converted_locations > 0);
        DOCTEST_CHECK(memcmp(expected_nebulas, actual.nebulas,
                             ORIGINAL_MAX_NEBULAS * sizeof(*expected_nebulas)) == 0);
        DOCTEST_CHECK(Is_Zero_(&actual.nebulas[ORIGINAL_MAX_NEBULAS],
                               (MAX_NEBULAS - ORIGINAL_MAX_NEBULAS) * sizeof(*actual.nebulas)));
    }

    void Check_Reports_(const s_report* expected) {
        for (int i = 0; i < MAX_REPORTS; ++i) {
            const s_report& actual = REPORT::_reports[i];
            DOCTEST_CHECK(actual.type == expected[i].type);
            DOCTEST_CHECK(actual.player_idx == expected[i].player_idx);
            DOCTEST_CHECK(actual.action == expected[i].action);
            if (actual.type == 1 && actual.action == 0) {
                DOCTEST_CHECK(memcmp(&actual.d.steal, &expected[i].d.steal,
                                     sizeof(actual.d.steal)) == 0);
            } else if (actual.type == 1 && actual.action == 1) {
                DOCTEST_CHECK(actual.d.sabotage.framed_player_idx ==
                              expected[i].d.sabotage.framed_player_idx);
                DOCTEST_CHECK(actual.d.sabotage.victim_player_idx ==
                              expected[i].d.sabotage.victim_player_idx);
                DOCTEST_CHECK(actual.d.sabotage.planet_idx ==
                              expected[i].d.sabotage.planet_idx);
                DOCTEST_CHECK(actual.d.sabotage.building_id ==
                              expected[i].d.sabotage.building_id);
            } else if (actual.type == 2 && actual.action == 0) {
                DOCTEST_CHECK(memcmp(&actual.d.enemy_steal,
                                     &expected[i].d.enemy_steal,
                                     sizeof(actual.d.enemy_steal)) == 0);
            } else {
                DOCTEST_CHECK(actual.d.enemy_sabotage.enemy_player_idx ==
                              expected[i].d.enemy_sabotage.enemy_player_idx);
                DOCTEST_CHECK(actual.d.enemy_sabotage.planet_idx ==
                              expected[i].d.enemy_sabotage.planet_idx);
                DOCTEST_CHECK(actual.d.enemy_sabotage.building_id ==
                              expected[i].d.enemy_sabotage.building_id);
            }
            DOCTEST_CHECK(memcmp(actual.reserved, expected[i].reserved,
                                 sizeof(actual.reserved)) == 0);
        }
    }

}

DOCTEST_TEST_CASE("version 0 ship locations migrate to configured ranges") {
    DOCTEST_CHECK(SAVEGAME::Convert_Legacy_Ship_Location_(-1) == -1);
    DOCTEST_CHECK(SAVEGAME::Convert_Legacy_Ship_Location_(71) == 71);
    DOCTEST_CHECK(SAVEGAME::Convert_Legacy_Ship_Location_(499) == 499);
    DOCTEST_CHECK(SAVEGAME::Convert_Legacy_Ship_Location_(500) == SHIP_LOCATION_MOVING_OFFSET);
    DOCTEST_CHECK(SAVEGAME::Convert_Legacy_Ship_Location_(571) == SHIP_LOCATION_MOVING_OFFSET + 71);
    DOCTEST_CHECK(SAVEGAME::Convert_Legacy_Ship_Location_(999) == SHIP_LOCATION_MOVING_OFFSET + 499);
    DOCTEST_CHECK(SAVEGAME::Convert_Legacy_Ship_Location_(1000) == SHIP_LOCATION_WORMHOLE_OFFSET);
    DOCTEST_CHECK(SAVEGAME::Convert_Legacy_Ship_Location_(1071) == SHIP_LOCATION_WORMHOLE_OFFSET + 71);
    DOCTEST_CHECK(SAVEGAME::Convert_Legacy_Ship_Location_(1499) == SHIP_LOCATION_WORMHOLE_OFFSET + 499);
    DOCTEST_CHECK(SAVEGAME::Convert_Legacy_Ship_Location_(1500) == 1500);
}

DOCTEST_TEST_CASE("latest and version 0 save streams restore the complete persisted state") {
    State_ expected;
    State_ actual;
    Bind_State_(expected);
    Fill_State_(expected);

    s_settings expected_settings = MOX::_settings;
    s_antaran expected_antarans = MOX::_antarans;
    s_movable_box expected_boxes[5] = {};
    int16_t expected_box_order[5] = {};
    int16_t expected_bill[6] = {};
    s_report expected_reports[MAX_REPORTS] = {};
    s_report expected_legacy_reports[MAX_REPORTS] = {};
    s_event_data expected_events[36] = {};
    s_MSG expected_messages[MAX_MESSAGES] = {};
    s_MSG expected_legacy_messages[MAX_MESSAGES] = {};
    uint8_t expected_interesting[MAX_PLAYERS] = {};
    uint8_t expected_colonize_check[BITMAP(MAX_STARS)] = {};
    uint8_t expected_first_msg[MAX_PLAYERS] = {};
    uint8_t expected_max_msgs[MAX_PLAYERS] = {};
    uint8_t expected_n_msgs[MAX_PLAYERS] = {};
    expected.players[0].settlers[0].val = 0;
    SETTLER::Set_Source_Colony(&expected.players[0].settlers[0].val, 699);
    SETTLER::Set_Dest_Planet(&expected.players[0].settlers[0].val, 899);
    SETTLER::Set_Race(&expected.players[0].settlers[0].val, 7);
    SETTLER::Set_Eta(&expected.players[0].settlers[0].val, 12);
    SETTLER::Set_Prof(&expected.players[0].settlers[0].val, POP::PROF_SCIENTIST);
    memcpy(expected_legacy_reports, REPORT::_reports, sizeof(expected_legacy_reports));
    memcpy(expected_legacy_messages, MSG_::_msgs, sizeof(expected_legacy_messages));
    for (s_MSG& message : expected_legacy_messages) {
        switch (message.msg_type) {
        case MSG_TYPE_LOST_SHIP:
            message.u.lost_ship.star_idx %= ORIGINAL_MAX_STARS;
            break;
        case MSG_TYPE_SCRAP_SHIP:
            message.u.scrap_ship.where %= ORIGINAL_MAX_STARS;
            break;
        case MSG_TYPE_ARTEMIS:
            message.u.artemis.star_idx %= ORIGINAL_MAX_STARS;
            break;
        default:
            break;
        }
    }
    for (s_report& report : expected_legacy_reports) {
        uint8_t* data = reinterpret_cast<uint8_t*>(&report.d);
        if (report.type == 1 && report.action == 1) {
            report.d.sabotage.building_id = data[3];
            report.d.sabotage.planet_idx = data[2];
        } else if (report.type == 2 && report.action == 1) {
            report.d.enemy_sabotage.building_id = data[3];
            report.d.enemy_sabotage.planet_idx = data[2];
        } else {
            data[4] = 0;
        }
    }

    expected.planets[358].star_index = 511;
    expected.stars[70].wormhole_star_id = 510;
    expected.stars[70].next_wfi_in_list = 509;
    expected.players[0].offensive_stagepoint = 508;
    REPORT::_reports[2].d.sabotage.planet_idx = 359;
    REPORT::_reports[3].d.enemy_sabotage.planet_idx = 358;
    for (s_MSG& message : MSG_::_msgs) {
        switch (message.msg_type) {
        case MSG_TYPE_COLONY:
            message.u.colony.planet_idx = 359;
            break;
        case MSG_TYPE_LOST_SHIP:
            message.u.lost_ship.star_idx = 511;
            message.u.lost_ship.ship_idx = 999;
            break;
        case MSG_TYPE_SCRAP_SHIP:
            message.u.scrap_ship.where = 510;
            message.u.scrap_ship.ship_idx = 998;
            break;
        case MSG_TYPE_LOST_COLONY:
            message.u.lost_colony.planet_idx = 899;
            break;
        case MSG_TYPE_HYPERSPACE_BEAST:
            message.u.hyperspace_beast.star_idx = 511;
            break;
        case MSG_TYPE_ARTEMIS:
            message.u.artemis.star_idx = 511;
            break;
        case MSG_TYPE_ARTEMIS_LEADER:
            message.u.artemis_leader.star_idx = 510;
            break;
        case MSG_TYPE_FAILED_RETREAT:
            message.u.retreat.star_idx = 509;
            break;
        case MSG_TYPE_ANOMALY:
            message.u.anomaly.star_idx = 508;
            break;
        case MSG_TYPE_EEL_SPAWNED:
        case MSG_TYPE_EEL_VANISHED:
            message.u.eel.star_idx = 507;
            break;
        case MSG_TYPE_ARTIFACTS:
            message.u.artifacts.star_idx = 506;
            break;
        default:
            break;
        }
    }
    memcpy(expected_boxes, MOX::_moveable_box, sizeof(MOX::_moveable_box));
    memcpy(expected_box_order, MOX::_moveable_box_order, sizeof(MOX::_moveable_box_order));
    memcpy(expected_bill, MOX::_bill_savegame, sizeof(MOX::_bill_savegame));
    memcpy(expected_reports, REPORT::_reports, sizeof(REPORT::_reports));
    memcpy(expected_events, EVENTS::_event_data, sizeof(EVENTS::_event_data));
    memcpy(expected_messages, MSG_::_msgs, sizeof(MSG_::_msgs));
    memcpy(expected_interesting, REPORT::_something_interesting_happened, sizeof(expected_interesting));
    memcpy(expected_colonize_check, REPORT::_done_colonize_check, sizeof(expected_colonize_check));
    memcpy(expected_first_msg, MSG_::_first_msg, sizeof(expected_first_msg));
    memcpy(expected_max_msgs, MSG_::_max_msgs, sizeof(expected_max_msgs));
    memcpy(expected_n_msgs, MSG_::_n_msgs, sizeof(expected_n_msgs));

    uint8_t* versioned = static_cast<uint8_t*>(malloc(SAVEGAME::BUFFER_CAPACITY));
    DOCTEST_REQUIRE(versioned != nullptr);
    SERIAL::Writer_ writer(versioned, SAVEGAME::BUFFER_CAPACITY, "test save overflow");
    char description[37] = "Exhaustive save state";
    constexpr uint32_t random_seed = 0xCAFEBABE;
    memset(&expected.colonies[699], 0, sizeof(expected.colonies[699]));
    expected.colonies[699].allocated_to = 5;
    SAVEGAME::Write_Game_(&writer, description, random_seed);
    memset(&expected.colonies[699], 0, sizeof(expected.colonies[699]));
    expected.colonies[699].owner = -1;
    expected.colonies[699].allocated_to = -1;
    expected.colonies[699].planet = -1;
    expected.colonies[699].officer = -1;
    DOCTEST_CHECK(writer.Length_() < SAVEGAME::BUFFER_CAPACITY);

    uint8_t* short_nebula_save = static_cast<uint8_t*>(malloc(SAVEGAME::BUFFER_CAPACITY));
    DOCTEST_REQUIRE(short_nebula_save != nullptr);
    SERIAL::Writer_ short_nebula_writer(short_nebula_save, SAVEGAME::BUFFER_CAPACITY, "test save overflow");
    MOX::_NUM_NEBULAS = ORIGINAL_MAX_NEBULAS;
    SAVEGAME::Write_Game_(&short_nebula_writer, description, random_seed);
    MOX::_NUM_NEBULAS = MAX_NEBULAS;
    DOCTEST_CHECK(writer.Length_() - short_nebula_writer.Length_() ==
                  (MAX_NEBULAS - ORIGINAL_MAX_NEBULAS) * sizeof(s_nebula));
    free(short_nebula_save);

    FILE* legacy_file = fopen(SAVEGAME_TEST_DATA_DIR "/savegame_v0.sav", "rb");
    DOCTEST_REQUIRE(legacy_file != nullptr);
    DOCTEST_REQUIRE(fseek(legacy_file, 0, SEEK_END) == 0);
    long legacy_size_long = ftell(legacy_file);
    DOCTEST_REQUIRE(legacy_size_long == 208000);
    DOCTEST_REQUIRE(fseek(legacy_file, 0, SEEK_SET) == 0);
    uint32_t legacy_size = static_cast<uint32_t>(legacy_size_long);
    uint8_t* legacy = static_cast<uint8_t*>(malloc(SAVEGAME::BUFFER_CAPACITY));
    DOCTEST_REQUIRE(legacy != nullptr);
    DOCTEST_REQUIRE(fread(legacy, 1, legacy_size, legacy_file) == legacy_size);
    fclose(legacy_file);

    const SETTLER::legacy_value_type first_legacy_settler = 0x7C17F2EAu;
    uint8_t* first_legacy_settler_ptr = nullptr;
    for (uint32_t offset = 0;
         offset + sizeof(first_legacy_settler) <= legacy_size; ++offset) {
        if (memcmp(legacy + offset, &first_legacy_settler,
                   sizeof(first_legacy_settler)) == 0) {
            DOCTEST_REQUIRE(first_legacy_settler_ptr == nullptr);
            first_legacy_settler_ptr = legacy + offset;
        }
    }
    DOCTEST_REQUIRE(first_legacy_settler_ptr != nullptr);
    *first_legacy_settler_ptr = SETTLER::LEGACY_SOURCE_COLONY_IN_TRANSIT;

    constexpr uint32_t version_offset =
            sizeof(uint32_t) + sizeof(SAVEGAME::s_metadata::description) +
            sizeof(int32_t) + sizeof(int8_t);
    uint8_t* version_zero = static_cast<uint8_t*>(malloc(SAVEGAME::BUFFER_CAPACITY));
    DOCTEST_REQUIRE(version_zero != nullptr);
    memcpy(version_zero, legacy, version_offset);
    const uint32_t versioned_header = SAVEGAME::VERSIONED_HEADER;
    const uint16_t legacy_version = 0;
    memcpy(version_zero, &versioned_header, sizeof(versioned_header));
    memcpy(version_zero + version_offset, &legacy_version, sizeof(legacy_version));
    memcpy(version_zero + version_offset + sizeof(legacy_version),
           legacy + version_offset, legacy_size - version_offset);
    const uint32_t version_zero_size = legacy_size + sizeof(legacy_version);

    for (int format = 0; format < 3; ++format) {
        Bind_State_(actual);
        Clear_State_(actual);
        const uint8_t* data = format == 0 ? versioned :
                              format == 1 ? legacy : version_zero;
        uint32_t size = format == 0 ? writer.Length_() :
                        format == 1 ? legacy_size : version_zero_size;
        SERIAL::Reader_ reader(data, size, "test save underflow");
        SAVEGAME::s_metadata metadata = {};
        s_settings loaded_settings = {};
        uint32_t loaded_seed = 0;

        DOCTEST_REQUIRE(SAVEGAME::Read_Game_(&reader, &metadata, &loaded_settings, &loaded_seed));
        DOCTEST_CHECK(reader.Length_() == size);
        DOCTEST_CHECK(metadata.header == (format == 1 ? SAVEGAME::LEGACY_HEADER : SAVEGAME::VERSIONED_HEADER));
        DOCTEST_CHECK(metadata.version == (format == 0 ? SAVEGAME::CURRENT_VERSION : 0));
        DOCTEST_CHECK(memcmp(metadata.description, description, sizeof(description)) == 0);
        DOCTEST_CHECK(metadata.stardate == 35123);
        DOCTEST_CHECK(metadata.game_type == 2);
        DOCTEST_CHECK(loaded_seed == random_seed);
        DOCTEST_CHECK(memcmp(&loaded_settings, &expected_settings, sizeof(expected_settings)) == 0);
        if (format == 0) {
            Check_Versioned_State_(expected, actual);
            DOCTEST_CHECK(SETTLER::Get_Source_Colony(actual.players[0].settlers[0].val) == 699);
            DOCTEST_CHECK(SETTLER::Get_Dest_Planet(actual.players[0].settlers[0].val) == 899);
        } else {
            Check_Legacy_State_Except_Players_(expected, actual);
            DOCTEST_CHECK(actual.players[0].settlers[0].val == 0x00007C1700F2FFFFull);
            DOCTEST_CHECK(SETTLER::Get_Source_Colony(actual.players[0].settlers[0].val) ==
                          SETTLER::SOURCE_COLONY_IN_TRANSIT);
            DOCTEST_CHECK(actual.players[0].settlers[1].val == 0x0000A01400730027ull);
            DOCTEST_CHECK(actual.players[0].settlers[24].val == 0x0000A58E00130067ull);
            DOCTEST_CHECK(actual.players[7].settlers[0].val == 0x0000478B00CD000Aull);
            DOCTEST_CHECK(actual.players[7].settlers[24].val == 0x0000D81700EE00D1ull);
        }
        DOCTEST_CHECK(memcmp(&MOX::_antarans, &expected_antarans, sizeof(expected_antarans)) == 0);
        DOCTEST_CHECK(memcmp(MOX::_moveable_box, expected_boxes, sizeof(MOX::_moveable_box)) == 0);
        DOCTEST_CHECK(memcmp(MOX::_moveable_box_order, expected_box_order, sizeof(MOX::_moveable_box_order)) == 0);
        DOCTEST_CHECK(memcmp(MOX::_bill_savegame, expected_bill, sizeof(MOX::_bill_savegame)) == 0);
        Check_Reports_(format == 0 ? expected_reports : expected_legacy_reports);
        DOCTEST_CHECK(memcmp(EVENTS::_event_data, expected_events, sizeof(EVENTS::_event_data)) == 0);
        DOCTEST_CHECK(memcmp(MSG_::_msgs,
                             format == 0 ? expected_messages : expected_legacy_messages,
                             sizeof(MSG_::_msgs)) == 0);
        DOCTEST_CHECK(memcmp(REPORT::_something_interesting_happened, expected_interesting, sizeof(expected_interesting)) == 0);
        const size_t colonize_check_size = format == 0 ?
            sizeof(expected_colonize_check) : BITMAP(ORIGINAL_MAX_STARS);
        DOCTEST_CHECK(memcmp(REPORT::_done_colonize_check, expected_colonize_check,
                             colonize_check_size) == 0);
        DOCTEST_CHECK(Is_Zero_(REPORT::_done_colonize_check + colonize_check_size,
                               sizeof(expected_colonize_check) - colonize_check_size));
        DOCTEST_CHECK(memcmp(MSG_::_first_msg, expected_first_msg, sizeof(expected_first_msg)) == 0);
        DOCTEST_CHECK(memcmp(MSG_::_max_msgs, expected_max_msgs, sizeof(expected_max_msgs)) == 0);
        DOCTEST_CHECK(memcmp(MSG_::_n_msgs, expected_n_msgs, sizeof(expected_n_msgs)) == 0);
        DOCTEST_CHECK(MOX::_NUM_COLONIES == (format == 0 ? 700 : 249));
        DOCTEST_CHECK(MOX::_planet_count == (format == 0 ? 900 : 359));
        DOCTEST_CHECK(MOX::_NUM_STARS == (format == 0 ? 512 : 71));
        DOCTEST_CHECK(MOX::_NUM_PLAYERS == 8);
        DOCTEST_CHECK(MOX::_NUM_SHIPS == (format == 0 ? 1000 : 499));
        DOCTEST_CHECK(MOX::_PLAYER_NUM == 7);
        DOCTEST_CHECK(MOX::_max_map_scale == 31);
        DOCTEST_CHECK(MOX::_max_zoom_count == 19);
        DOCTEST_CHECK(MOX::_star_bg_index == 6);
        DOCTEST_CHECK(MOX::_MAP_MAX_X == 311);
        DOCTEST_CHECK(MOX::_MAP_MAX_Y == 177);
        DOCTEST_CHECK(MOX::_fleet_icon_button_count == 421);
        DOCTEST_CHECK(MOX::_NUM_NEBULAS == (format == 0 ? MAX_NEBULAS : ORIGINAL_MAX_NEBULAS));
        DOCTEST_CHECK(REPORT::_n_reports == MAX_REPORTS);
        DOCTEST_CHECK(REPORT::_done_player_reports == 0xA5);
        DOCTEST_CHECK(REPORT::_displayed_msgs == 0x5A);
        DOCTEST_CHECK(REPORT::_displayed_gnn == 0xC3);
        DOCTEST_CHECK(diplodef::_COUNCIL_FLAG == 3);
        DOCTEST_CHECK(diplodef::_next_council_meeting_turn == 1777);
        DOCTEST_CHECK(MOX::_antaran_resource_level == 4321);
        DOCTEST_CHECK(MOX::_chance_for_antaran_invasion == 1234);
        DOCTEST_CHECK(MOX::_disallow_saves_code == 0xBEEF);
        DOCTEST_CHECK(EVENTS::_last_event_year == 0x12345678);
        DOCTEST_CHECK(EVENTS::_event_delay_count == -123);
        DOCTEST_CHECK(MOX::_cur_map_scale == 23);
        DOCTEST_CHECK(MOX::_cur_map_x == -144);
        DOCTEST_CHECK(MOX::_cur_map_y == 155);
    }
    free(version_zero);
    free(legacy);
    free(versioned);
}

DOCTEST_TEST_CASE("save reader rejects invalid and unsupported headers") {
    uint8_t buffer[64] = {};
    uint8_t description[37] = {};
    s_settings settings = {};
    uint32_t random_seed = 0;
    SAVEGAME::s_metadata metadata = {};

    {
        SERIAL::Writer_ writer(buffer, sizeof(buffer), "test overflow");
        writer.Write_Uint32_(0xDEADBEEF);
        SERIAL::Reader_ reader(buffer, writer.Length_(), "test underflow");
        DOCTEST_CHECK_FALSE(SAVEGAME::Read_Game_(&reader, &metadata, &settings, &random_seed));
    }

    {
        SERIAL::Writer_ writer(buffer, sizeof(buffer), "test overflow");
        writer.Write_Uint32_(SAVEGAME::VERSIONED_HEADER);
        writer.Write_Bytes_(description, sizeof(description));
        writer.Write_Int32_(0);
        writer.Write_Int8_(0);
        writer.Write_Uint16_(SAVEGAME::CURRENT_VERSION + 1);
        SERIAL::Reader_ reader(buffer, writer.Length_(), "test underflow");
        DOCTEST_CHECK_FALSE(SAVEGAME::Read_Game_(&reader, &metadata, &settings, &random_seed));
    }
}