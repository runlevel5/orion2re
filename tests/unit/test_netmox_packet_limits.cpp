#include "pch.h"

#include "doctest/doctest.h"

#include <array>
#include <cstring>
#include <vector>

namespace netcode {
    extern void (*g_net_update_hook)();
}

namespace {
    int g_net_update_hook_invocations = 0;

    void Snapshot_Encode_Mutation_Hook_() {
        ++g_net_update_hook_invocations;
        MOX::_player[1].bc += 999;
        netcode::g_net_update_hook = nullptr;
    }

    constexpr uint32_t Broadcast_Game_Data_Snapshot_Max_Bytes_() {
        return
            sizeof(NETMOX::_net.host_election_id) +
            sizeof(MOX::_stardate) +
            sizeof(MOX::_settings.random_events) +
            sizeof(MOX::_settings.game_difficulty) +
            sizeof(MOX::_settings.number_of_players) +
            sizeof(MOX::_settings.galaxy_size) +
            sizeof(MOX::_settings.galaxy_age) +
            sizeof(MOX::_settings.strategic_combat_flag) +
            sizeof(MOX::_settings.starting_civilization_level) +
            sizeof(MOX::_settings.xenons_exist) +
            sizeof(uint32_t) +
            sizeof(uint16_t) +
            MAX_PLAYERS * sizeof(s_player) +
            sizeof(uint16_t) +
            (MAX_STARS + 1) * sizeof(s_star_data) +
            sizeof(uint16_t) +
            MAX_PLANETS * sizeof(s_planet_data) +
            sizeof(uint16_t) +
            MAX_COLONIES * sizeof(s_colony) +
            sizeof(int16_t) +
            MAX_SHIPS * sizeof(s_ship_data) +
            MAX_LEADERS * sizeof(s_leader_data) +
            sizeof(uint16_t) +
            sizeof(REPORT::_reports) +
            sizeof(EVENTS::_event_data) +
            sizeof(MSG_::_first_msg) +
            sizeof(MSG_::_max_msgs) +
            sizeof(MSG_::_n_msgs) +
            sizeof(MSG_::_msgs) +
            sizeof(diplodef::_COUNCIL_FLAG) +
            sizeof(diplodef::_next_council_meeting_turn) +
            sizeof(MOX::_antarans) +
            sizeof(MOX::_NUM_NEBULAS) +
            MAX_NEBULAS * sizeof(s_nebula) +
            sizeof(EVENTS::_last_event_year) +
            sizeof(EVENTS::_event_delay_count) +
            sizeof(MOX::_bill_savegame);
    }

    struct Scoped_Netmox_Roundtrip_State_ {
        s_network saved_net = NETMOX::_net;
        uint32_t saved_stardate = MOX::_stardate;
        s_settings saved_settings = MOX::_settings;
        int16_t saved_player_num = MOX::_PLAYER_NUM;
        int16_t saved_num_players = MOX::_NUM_PLAYERS;
        int16_t saved_num_stars = MOX::_NUM_STARS;
        int16_t saved_planet_count = MOX::_planet_count;
        int16_t saved_num_colonies = MOX::_NUM_COLONIES;
        int16_t saved_num_ships = MOX::_NUM_SHIPS;
        uint8_t saved_num_nebulas = MOX::_NUM_NEBULAS;
        s_antaran saved_antarans = MOX::_antarans;
        uint32_t saved_random_seed = game_random::Get_Random_Seed_();
        int16_t saved_report_count = REPORT::_n_reports;
        std::array<int16_t, 6> saved_bill_savegame = {};
        std::array<s_report, 50> saved_reports = {};
        std::array<s_event_data, 36> saved_event_data = {};
        std::array<uint8_t, MAX_PLAYERS> saved_first_msg = {};
        std::array<uint8_t, MAX_PLAYERS> saved_max_msgs = {};
        std::array<uint8_t, MAX_PLAYERS> saved_n_msgs = {};
        std::array<s_MSG, 100> saved_msgs = {};
        s_player* saved_player_ptr = MOX::_player;
        s_star_data* saved_star_ptr = MOX::_star;
        s_planet_data* saved_planet_ptr = MOX::_planet;
        s_colony* saved_colony_ptr = MOX::_colony;
        s_ship_data* saved_ship_ptr = MOX::_ship;
        s_leader_data* saved_leader_ptr = MOX::_leaders;
        s_nebula* saved_nebula_ptr = MOX::_nebula;
        uint8_t* saved_snapshot_cache_buffer = NETMOX::_net_snapshot_cache.buffer;
        uint8_t* saved_snapshot_cache_current_ptr = NETMOX::_net_snapshot_cache.current_ptr;
        uint32_t saved_snapshot_cache_size = NETMOX::_net_snapshot_cache.buffer_size;

        std::vector<s_player> players = std::vector<s_player>(MAX_PLAYERS);
        std::vector<s_star_data> stars = std::vector<s_star_data>(MAX_STARS + 1);
        std::vector<s_planet_data> planets = std::vector<s_planet_data>(MAX_PLANETS);
        std::vector<s_colony> colonies = std::vector<s_colony>(MAX_COLONIES);
        std::vector<s_ship_data> ships = std::vector<s_ship_data>(MAX_SHIPS);
        std::vector<s_leader_data> leaders = std::vector<s_leader_data>(MAX_LEADERS);
        std::vector<s_nebula> nebulas = std::vector<s_nebula>(MAX_NEBULAS);

        Scoped_Netmox_Roundtrip_State_() {
            std::memcpy(saved_bill_savegame.data(), MOX::_bill_savegame, sizeof(MOX::_bill_savegame));
            std::memcpy(saved_reports.data(), REPORT::_reports, sizeof(REPORT::_reports));
            std::memcpy(saved_event_data.data(), EVENTS::_event_data, sizeof(EVENTS::_event_data));
            std::memcpy(saved_first_msg.data(), MSG_::_first_msg, sizeof(MSG_::_first_msg));
            std::memcpy(saved_max_msgs.data(), MSG_::_max_msgs, sizeof(MSG_::_max_msgs));
            std::memcpy(saved_n_msgs.data(), MSG_::_n_msgs, sizeof(MSG_::_n_msgs));
            std::memcpy(saved_msgs.data(), MSG_::_msgs, sizeof(MSG_::_msgs));

            MOX::_player = players.data();
            MOX::_star = stars.data();
            MOX::_planet = planets.data();
            MOX::_colony = colonies.data();
            MOX::_ship = ships.data();
            MOX::_leaders = leaders.data();
            MOX::_nebula = nebulas.data();
        }

        ~Scoped_Netmox_Roundtrip_State_() {
            MOX::_player = saved_player_ptr;
            MOX::_star = saved_star_ptr;
            MOX::_planet = saved_planet_ptr;
            MOX::_colony = saved_colony_ptr;
            MOX::_ship = saved_ship_ptr;
            MOX::_leaders = saved_leader_ptr;
            MOX::_nebula = saved_nebula_ptr;

            NETMOX::_net = saved_net;
            MOX::_stardate = saved_stardate;
            MOX::_settings = saved_settings;
            MOX::_PLAYER_NUM = saved_player_num;
            MOX::_NUM_PLAYERS = saved_num_players;
            MOX::_NUM_STARS = saved_num_stars;
            MOX::_planet_count = saved_planet_count;
            MOX::_NUM_COLONIES = saved_num_colonies;
            MOX::_NUM_SHIPS = saved_num_ships;
            MOX::_NUM_NEBULAS = saved_num_nebulas;
            MOX::_antarans = saved_antarans;
            game_random::Set_Random_Seed_(saved_random_seed);
            REPORT::_n_reports = saved_report_count;
            std::memcpy(MOX::_bill_savegame, saved_bill_savegame.data(), sizeof(MOX::_bill_savegame));
            std::memcpy(REPORT::_reports, saved_reports.data(), sizeof(REPORT::_reports));
            std::memcpy(EVENTS::_event_data, saved_event_data.data(), sizeof(EVENTS::_event_data));
            std::memcpy(MSG_::_first_msg, saved_first_msg.data(), sizeof(MSG_::_first_msg));
            std::memcpy(MSG_::_max_msgs, saved_max_msgs.data(), sizeof(MSG_::_max_msgs));
            std::memcpy(MSG_::_n_msgs, saved_n_msgs.data(), sizeof(MSG_::_n_msgs));
            std::memcpy(MSG_::_msgs, saved_msgs.data(), sizeof(MSG_::_msgs));

            if (NETMOX::_net_snapshot_cache.buffer != saved_snapshot_cache_buffer) {
                std::free(NETMOX::_net_snapshot_cache.buffer);
            }
            NETMOX::_net_snapshot_cache.buffer = saved_snapshot_cache_buffer;
            NETMOX::_net_snapshot_cache.current_ptr = saved_snapshot_cache_current_ptr;
            NETMOX::_net_snapshot_cache.buffer_size = saved_snapshot_cache_size;
        }
    };

    void Fill_Netmox_Roundtrip_State_() {
        MOX::_PLAYER_NUM = 1;
        MOX::_NUM_PLAYERS = 3;
        MOX::_NUM_STARS = 4;
        MOX::_planet_count = 6;
        MOX::_NUM_COLONIES = 5;
        MOX::_NUM_SHIPS = 7;
        MOX::_NUM_NEBULAS = 2;
        MOX::_stardate = 35123;
        MOX::_settings.random_events = 1;
        MOX::_settings.game_difficulty = 3;
        MOX::_settings.number_of_players = 3;
        MOX::_settings.galaxy_size = 2;
        MOX::_settings.galaxy_age = 1;
        MOX::_settings.strategic_combat_flag = 1;
        MOX::_settings.starting_civilization_level = 4;
        MOX::_settings.xenons_exist = 1;
        NETMOX::_net.host_election_id = 77;
        diplodef::_COUNCIL_FLAG = 1;
        diplodef::_next_council_meeting_turn = 42;
        EVENTS::_last_event_year = 123456u;
        EVENTS::_event_delay_count = 9;
        REPORT::_n_reports = 3;
        game_random::Set_Random_Seed_(0x13572468u);

        std::memset(MOX::_player, 0, MAX_PLAYERS * sizeof(s_player));
        std::memset(MOX::_star, 0, (MAX_STARS + 1) * sizeof(s_star_data));
        std::memset(MOX::_planet, 0, MAX_PLANETS * sizeof(s_planet_data));
        std::memset(MOX::_colony, 0, MAX_COLONIES * sizeof(s_colony));
        std::memset(MOX::_ship, 0, MAX_SHIPS * sizeof(s_ship_data));
        std::memset(MOX::_leaders, 0, MAX_LEADERS * sizeof(s_leader_data));
        std::memset(MOX::_nebula, 0, MAX_NEBULAS * sizeof(s_nebula));
        std::memset(REPORT::_reports, 0, sizeof(REPORT::_reports));
        std::memset(EVENTS::_event_data, 0, sizeof(EVENTS::_event_data));
        std::memset(MSG_::_first_msg, 0, sizeof(MSG_::_first_msg));
        std::memset(MSG_::_max_msgs, 0, sizeof(MSG_::_max_msgs));
        std::memset(MSG_::_n_msgs, 0, sizeof(MSG_::_n_msgs));
        std::memset(MSG_::_msgs, 0, sizeof(MSG_::_msgs));
        std::memset(MOX::_bill_savegame, 0, sizeof(MOX::_bill_savegame));
        std::memset(&MOX::_antarans, 0, sizeof(MOX::_antarans));

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            MOX::_player[i].network_player_id = (int16_t)(100 + i);
            MOX::_player[i].objectives = (i == 2) ? PLAYER_OBJECTIVE_MILITARIST : PLAYER_OBJECTIVE_HUMAN;
            MOX::_player[i].race = (uint8_t)(i + 3);
            MOX::_player[i].color = (uint8_t)(i + 1);
            MOX::_player[i].home_planet_id = (int16_t)i;
            MOX::_player[i].ship_range = (int16_t)(30 + i);
            MOX::_player[i].bc = 1000 + i * 77;
            MOX::_player[i].tech_applications[0] = (uint8_t)(11 + i);
            MOX::_player[i].tech_applications[17] = (uint8_t)(22 + i);
            MOX::_player[i].name[0] = (char)('A' + i);
            MOX::_player[i].race_name[0] = (char)('K' + i);
        }

        for (int i = 0; i <= MOX::_NUM_STARS; ++i) {
            MOX::_star[i].x = (int16_t)(10 + i * 3);
            MOX::_star[i].y = (int16_t)(20 + i * 5);
            MOX::_star[i].owner = (int8_t)(i % MOX::_NUM_PLAYERS);
            MOX::_star[i].surrender_to[0] = (int8_t)(i + 1);
            MOX::_star[i].surrender_to[1] = (int8_t)(i + 2);
            MOX::_star[i].officer_index[0] = (int8_t)(30 + i);
        }

        for (int i = 0; i < MOX::_planet_count; ++i) {
            MOX::_planet[i].star_index = (int8_t)(i % MOX::_NUM_STARS);
            MOX::_planet[i].orbit = (int8_t)(i % 5);
            MOX::_planet[i].colony_index = (int16_t)((i < MOX::_NUM_COLONIES) ? i : -1);
            MOX::_planet[i].planet_type = (int8_t)(i + 1);
            MOX::_planet[i].climate = (char)(i + 2);
        }

        for (int i = 0; i < MOX::_NUM_COLONIES; ++i) {
            MOX::_colony[i].owner = (int8_t)(i % MOX::_NUM_PLAYERS);
            MOX::_colony[i].planet = (int16_t)i;
            MOX::_colony[i].morale = (int8_t)(50 + i);
            MOX::_colony[i].pollution = (int16_t)(i * 2);
        }

        for (int i = 0; i < MOX::_NUM_SHIPS; ++i) {
            MOX::_ship[i].owner = (int8_t)(i % MOX::_NUM_PLAYERS);
            MOX::_ship[i].location = (int16_t)(i % MOX::_NUM_STARS);
            MOX::_ship[i].status = 0;
            MOX::_ship[i].turns_left = (int8_t)(i + 1);
            MOX::_ship[i].d.ship_type = (int8_t)(i % 3);
        }

        for (int i = 0; i < MAX_LEADERS; ++i) {
            MOX::_leaders[i].player_index = (int8_t)(i % MOX::_NUM_PLAYERS);
            MOX::_leaders[i].location = (int8_t)(i % MOX::_NUM_STARS);
            MOX::_leaders[i].eta = (int8_t)(i + 3);
        }

        for (int i = 0; i < REPORT::_n_reports; ++i) {
            REPORT::_reports[i].type = (uint8_t)(i + 1);
            REPORT::_reports[i].player_idx = (int16_t)i;
            REPORT::_reports[i].action = (uint8_t)(7 + i);
            REPORT::_reports[i].d.steal.tech_app_id = (int16_t)(100 + i);
            for (size_t j = 0; j < sizeof(REPORT::_reports[i].reserved); ++j) {
                REPORT::_reports[i].reserved[j] = (uint8_t)(10 + i + j);
            }
        }

        for (int i = 0; i < 36; ++i) {
            EVENTS::_event_data[i].player_idx = (int8_t)(i % MOX::_NUM_PLAYERS);
            EVENTS::_event_data[i].status = (int8_t)(i % 7);
            EVENTS::_event_data[i].turn_count = (int16_t)(i * 3);
            EVENTS::_event_data[i].target_id = (int16_t)(1000 + i);
            EVENTS::_event_data[i].amount = (int16_t)(2000 + i);
            EVENTS::_event_data[i].max_amount = (int16_t)(3000 + i);
        }

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            MSG_::_first_msg[i] = (uint8_t)(i * 2);
            MSG_::_max_msgs[i] = (uint8_t)(10 + i);
            MSG_::_n_msgs[i] = (uint8_t)(3 + i);
        }

        for (int i = 0; i < 100; ++i) {
            MSG_::_msgs[i].msg_type = (int8_t)(i % 9);
            MSG_::_msgs[i].priority = (uint8_t)(i % 5);
            MSG_::_msgs[i].u.financial.a = (int16_t)(500 + i);
            MSG_::_msgs[i].u.financial.b = (int16_t)(700 + i);
        }

        for (int i = 0; i < MOX::_NUM_NEBULAS; ++i) {
            MOX::_nebula[i].x = (int16_t)(5 + i);
            MOX::_nebula[i].y = (int16_t)(7 + i);
            MOX::_nebula[i].type = (int8_t)(9 + i);
        }

        for (int i = 0; i < 6; ++i) {
            MOX::_bill_savegame[i] = (int16_t)(600 + i);
        }

        MOX::_antarans.attack_notified_mask = 3;
        MOX::_antarans.attack_pressure = 4;
        MOX::_antarans.resource_offensive = 5;
        MOX::_antarans.resource_defensive = 6;
        MOX::_antarans.invasion_stardate = 777;
    }
}

DOCTEST_TEST_SUITE("netmox packet limits") {

DOCTEST_TEST_CASE("large combat payload allocations stay within the shared limit") {
    constexpr uint32_t max_network_payload_bytes = NETMOX::NETMOX_MAX_GAME_DATA_PAYLOAD_BYTES;
    constexpr uint32_t game_orders_payload_bytes =
        sizeof(uint32_t) +
        sizeof(uint8_t) +
        sizeof(s_player) +
        MAX_STARS * sizeof(s_net_pack_star_order) +
        sizeof(int16_t) +
        MAX_COLONIES * (sizeof(int16_t) + sizeof(s_colony)) +
        sizeof(int16_t) +
        sizeof(int16_t) +
        MAX_SHIPS * (sizeof(int16_t) + sizeof(s_ship_data)) +
        sizeof(int16_t) +
        MAX_LEADERS * (sizeof(int16_t) + sizeof(s_leader_data)) +
        sizeof(int16_t);

    constexpr uint32_t battle_results_remote_payload_bytes =
        sizeof(int16_t) * 4 +
        sizeof(int16_t) +
        MAX_PLAYERS * sizeof(s_combfind_star_has_player) +
        sizeof(uint8_t) +
        sizeof(int16_t) * 2 +
        2 * (sizeof(MOX::_player[0].tech_applications) + sizeof(uint8_t)) +
        MAX_LEADERS * (sizeof(int16_t) + sizeof(s_leader_data)) +
        sizeof(int16_t) +
        (sizeof(int16_t) * 2 + sizeof(uint8_t) * 2 + sizeof(s_colony)) +
        sizeof(int16_t) +
        MAX_SHIPS * (sizeof(int16_t) + sizeof(s_ship_data)) +
        sizeof(int16_t);

    constexpr uint32_t you_are_target_remote_payload_bytes =
        sizeof(NETMOX::s_packet_0x12) +
        MAX_PLAYERS * sizeof(s_combfind_star_has_player) +
        sizeof(uint8_t) +
        2 * (sizeof(MOX::_player[0].tech_fields) + sizeof(MOX::_player[0].tech_applications)) +
        MAX_LEADERS * (sizeof(int16_t) + sizeof(s_leader_data)) +
        sizeof(int16_t) +
        2 * sizeof(uint8_t) +
        (sizeof(int16_t) * 3 + sizeof(uint8_t) * 2 + sizeof(s_colony)) +
        sizeof(int16_t) +
        MAX_SHIPS * (sizeof(int16_t) + sizeof(s_ship_data)) +
        sizeof(int16_t);

    constexpr uint32_t pick_target_remote_payload_bytes =
        sizeof(int16_t) +
        sizeof(s_combfind_target) +
        MAX_PLAYERS * sizeof(s_combfind_star_has_player) +
        sizeof(uint8_t) +
        MAX_PLAYERS * (sizeof(MOX::_player[0].tech_fields) + sizeof(MOX::_player[0].tech_applications)) +
        MAX_LEADERS * (sizeof(int16_t) + sizeof(s_leader_data)) +
        sizeof(int16_t) +
        MAX_PLAYERS * sizeof(uint8_t) +
        sizeof(int16_t) +
        5 * (sizeof(int16_t) * 2 + sizeof(uint8_t) * 2 + sizeof(s_colony)) +
        sizeof(int16_t) +
        MAX_SHIPS * (sizeof(int16_t) + sizeof(s_ship_data)) +
        sizeof(int16_t);

    DOCTEST_CHECK(game_orders_payload_bytes <= max_network_payload_bytes);
    DOCTEST_CHECK(battle_results_remote_payload_bytes <= max_network_payload_bytes);
    DOCTEST_CHECK(you_are_target_remote_payload_bytes <= max_network_payload_bytes);
    DOCTEST_CHECK(pick_target_remote_payload_bytes <= max_network_payload_bytes);
}

DOCTEST_TEST_CASE("broadcast differential layout still matches legacy fixed-size arrays") {
    DOCTEST_CHECK(sizeof(REPORT::_reports) == 50 * sizeof(s_report));
    DOCTEST_CHECK(sizeof(EVENTS::_event_data) == 36 * sizeof(s_event_data));
    DOCTEST_CHECK(sizeof(MSG_::_msgs) == 100 * sizeof(s_MSG));
    DOCTEST_CHECK(sizeof(MSG_::_first_msg) == MAX_PLAYERS * sizeof(MSG_::_first_msg[0]));
    DOCTEST_CHECK(sizeof(MSG_::_max_msgs) == MAX_PLAYERS * sizeof(MSG_::_max_msgs[0]));
    DOCTEST_CHECK(sizeof(MSG_::_n_msgs) == MAX_PLAYERS * sizeof(MSG_::_n_msgs[0]));
    DOCTEST_CHECK(sizeof(MOX::_bill_savegame) == 6 * sizeof(MOX::_bill_savegame[0]));
}

DOCTEST_TEST_CASE("broadcast snapshot maximum fits the shared network payload limit") {
    constexpr uint32_t broadcast_snapshot_payload_bytes = Broadcast_Game_Data_Snapshot_Max_Bytes_();

    DOCTEST_CHECK(broadcast_snapshot_payload_bytes <= NETMOX::NETMOX_MAX_GAME_DATA_PAYLOAD_BYTES);
}

DOCTEST_TEST_CASE("differential decode requires full shared payload workspace on the receive side") {
    constexpr uint32_t worst_case_received_diff_bytes = Broadcast_Game_Data_Snapshot_Max_Bytes_() + 1;
    constexpr uint32_t required_decode_workspace_bytes = NETMOX::NETMOX_MAX_GAME_DATA_PAYLOAD_BYTES;

    DOCTEST_CHECK(required_decode_workspace_bytes >= worst_case_received_diff_bytes);
    DOCTEST_CHECK(Broadcast_Game_Data_Snapshot_Max_Bytes_() +
                  worst_case_received_diff_bytes <= required_decode_workspace_bytes);
}

DOCTEST_TEST_CASE("network packet structs stay byte packed and size stable") {
    DOCTEST_CHECK(alignof(NETMOX::s_packet_dragon_info) == 1);
    DOCTEST_CHECK(sizeof(NETMOX::s_packet_dragon_info) == sizeof(int16_t));

    DOCTEST_CHECK(alignof(NETMOX::s_packet_0x12) == 1);
    DOCTEST_CHECK(sizeof(NETMOX::s_packet_0x12) ==
                  sizeof(int16_t) * 4);

    DOCTEST_CHECK(alignof(NETMOX::s_NET_MSG_RANK) == 1);
    DOCTEST_CHECK(sizeof(NETMOX::s_NET_MSG_RANK) ==
                  sizeof(int32_t) + sizeof(int8_t));

    DOCTEST_CHECK(alignof(NETMOX::s_NET_MSG_COLOR) == 1);
    DOCTEST_CHECK(sizeof(NETMOX::s_NET_MSG_COLOR) ==
                  sizeof(int8_t) + sizeof(int8_t));

    DOCTEST_CHECK(alignof(NETMOX::s_NET_MSG_RACE) == 1);
    DOCTEST_CHECK(sizeof(NETMOX::s_NET_MSG_RACE) ==
                  sizeof(uint8_t) + sizeof(int8_t));

    DOCTEST_CHECK(alignof(NETMOX::s_NET_MSG_SYNC_PLAYER) == 1);
    DOCTEST_CHECK(sizeof(NETMOX::s_NET_MSG_SYNC_PLAYER) ==
                  sizeof(int32_t) + sizeof(s_player) + MAX_STARS * sizeof(int8_t) + sizeof(int32_t) + sizeof(s_player));

    DOCTEST_CHECK(alignof(s_net_pack_drop_out_info) == 1);
    DOCTEST_CHECK(sizeof(s_net_pack_drop_out_info) ==
                  sizeof(s_player) + MAX_STARS * sizeof(int8_t));

    DOCTEST_CHECK(alignof(s_net_pack_end_ai_diplomacy) == 1);
    DOCTEST_CHECK(sizeof(s_net_pack_end_ai_diplomacy) ==
                  sizeof(int32_t) + sizeof(s_player) + MAX_STARS * sizeof(int8_t) + sizeof(int32_t) + sizeof(s_player));
}

DOCTEST_TEST_CASE("broadcast snapshot encode and decode round trip serialized game state") {
    Scoped_Netmox_Roundtrip_State_ scope;
    Fill_Netmox_Roundtrip_State_();

    const s_network expected_net = NETMOX::_net;
    const uint32_t expected_stardate = MOX::_stardate;
    const s_settings expected_settings = MOX::_settings;
    const uint32_t expected_random_seed = game_random::Get_Random_Seed_();
    const int16_t expected_num_players = MOX::_NUM_PLAYERS;
    const int16_t expected_num_stars = MOX::_NUM_STARS;
    const int16_t expected_planet_count = MOX::_planet_count;
    const int16_t expected_num_colonies = MOX::_NUM_COLONIES;
    const int16_t expected_num_ships = MOX::_NUM_SHIPS;
    const uint8_t expected_num_nebulas = MOX::_NUM_NEBULAS;
    const int16_t expected_report_count = REPORT::_n_reports;
    std::vector<s_player> expected_players(MOX::_player, MOX::_player + MAX_PLAYERS);
    std::vector<s_star_data> expected_stars(MOX::_star, MOX::_star + MAX_STARS + 1);
    std::vector<s_planet_data> expected_planets(MOX::_planet, MOX::_planet + MAX_PLANETS);
    std::vector<s_colony> expected_colonies(MOX::_colony, MOX::_colony + MAX_COLONIES);
    std::vector<s_ship_data> expected_ships(MOX::_ship, MOX::_ship + MAX_SHIPS);
    std::vector<s_leader_data> expected_leaders(MOX::_leaders, MOX::_leaders + MAX_LEADERS);
    std::array<s_report, 50> expected_reports = {};
    std::array<s_event_data, 36> expected_events = {};
    std::array<uint8_t, MAX_PLAYERS> expected_first_msg = {};
    std::array<uint8_t, MAX_PLAYERS> expected_max_msgs = {};
    std::array<uint8_t, MAX_PLAYERS> expected_n_msgs = {};
    std::array<s_MSG, 100> expected_msgs = {};
    std::array<int16_t, 6> expected_bill = {};
    std::vector<s_nebula> expected_nebulas(MOX::_nebula, MOX::_nebula + MAX_NEBULAS);
    const s_antaran expected_antarans = MOX::_antarans;

    std::memcpy(expected_reports.data(), REPORT::_reports, sizeof(REPORT::_reports));
    std::memcpy(expected_events.data(), EVENTS::_event_data, sizeof(EVENTS::_event_data));
    std::memcpy(expected_first_msg.data(), MSG_::_first_msg, sizeof(MSG_::_first_msg));
    std::memcpy(expected_max_msgs.data(), MSG_::_max_msgs, sizeof(MSG_::_max_msgs));
    std::memcpy(expected_n_msgs.data(), MSG_::_n_msgs, sizeof(MSG_::_n_msgs));
    std::memcpy(expected_msgs.data(), MSG_::_msgs, sizeof(MSG_::_msgs));
    std::memcpy(expected_bill.data(), MOX::_bill_savegame, sizeof(MOX::_bill_savegame));

    std::vector<uint8_t> payload(Broadcast_Game_Data_Snapshot_Max_Bytes_());
    const uint32_t payload_len = NETMOX::Encode_Broadcast_Game_Data_Snapshot_(payload.data(), (uint32_t)payload.size());

    std::memset(&NETMOX::_net, 0, sizeof(NETMOX::_net));
    MOX::_stardate = 0;
    std::memset(&MOX::_settings, 0, sizeof(MOX::_settings));
    MOX::_NUM_PLAYERS = 0;
    MOX::_NUM_STARS = 0;
    MOX::_planet_count = 0;
    MOX::_NUM_COLONIES = 0;
    MOX::_NUM_SHIPS = 0;
    MOX::_NUM_NEBULAS = 0;
    REPORT::_n_reports = 0;
    std::memset(MOX::_player, 0, MAX_PLAYERS * sizeof(s_player));
    std::memset(MOX::_star, 0, (MAX_STARS + 1) * sizeof(s_star_data));
    std::memset(MOX::_planet, 0, MAX_PLANETS * sizeof(s_planet_data));
    std::memset(MOX::_colony, 0, MAX_COLONIES * sizeof(s_colony));
    std::memset(MOX::_ship, 0, MAX_SHIPS * sizeof(s_ship_data));
    std::memset(MOX::_leaders, 0, MAX_LEADERS * sizeof(s_leader_data));
    std::memset(REPORT::_reports, 0, sizeof(REPORT::_reports));
    std::memset(EVENTS::_event_data, 0, sizeof(EVENTS::_event_data));
    std::memset(MSG_::_first_msg, 0, sizeof(MSG_::_first_msg));
    std::memset(MSG_::_max_msgs, 0, sizeof(MSG_::_max_msgs));
    std::memset(MSG_::_n_msgs, 0, sizeof(MSG_::_n_msgs));
    std::memset(MSG_::_msgs, 0, sizeof(MSG_::_msgs));
    std::memset(&MOX::_antarans, 0, sizeof(MOX::_antarans));
    std::memset(MOX::_nebula, 0, MAX_NEBULAS * sizeof(s_nebula));
    std::memset(MOX::_bill_savegame, 0, sizeof(MOX::_bill_savegame));
    game_random::Set_Random_Seed_(0);

    DOCTEST_REQUIRE(NETMOX::Decode_Broadcast_Game_Data_Differential_(payload.data(), payload_len, 1) == 1);

    DOCTEST_CHECK(NETMOX::_net.host_election_id == expected_net.host_election_id);
    DOCTEST_CHECK(MOX::_stardate == expected_stardate);
    DOCTEST_CHECK(std::memcmp(&MOX::_settings, &expected_settings, sizeof(expected_settings)) == 0);
    DOCTEST_CHECK(game_random::Get_Random_Seed_() == expected_random_seed);
    DOCTEST_CHECK(MOX::_NUM_PLAYERS == expected_num_players);
    DOCTEST_CHECK(MOX::_NUM_STARS == expected_num_stars);
    DOCTEST_CHECK(MOX::_planet_count == expected_planet_count);
    DOCTEST_CHECK(MOX::_NUM_COLONIES == expected_num_colonies);
    DOCTEST_CHECK(MOX::_NUM_SHIPS == expected_num_ships);
    DOCTEST_CHECK(MOX::_NUM_NEBULAS == expected_num_nebulas);
    DOCTEST_CHECK(REPORT::_n_reports == expected_report_count);
    DOCTEST_CHECK(std::memcmp(MOX::_player, expected_players.data(), MAX_PLAYERS * sizeof(s_player)) == 0);
    DOCTEST_CHECK(std::memcmp(MOX::_star, expected_stars.data(), (MAX_STARS + 1) * sizeof(s_star_data)) == 0);
    DOCTEST_CHECK(std::memcmp(MOX::_planet, expected_planets.data(), MAX_PLANETS * sizeof(s_planet_data)) == 0);
    DOCTEST_CHECK(std::memcmp(MOX::_colony, expected_colonies.data(), MAX_COLONIES * sizeof(s_colony)) == 0);
    DOCTEST_CHECK(std::memcmp(MOX::_ship, expected_ships.data(), MAX_SHIPS * sizeof(s_ship_data)) == 0);
    DOCTEST_CHECK(std::memcmp(MOX::_leaders, expected_leaders.data(), MAX_LEADERS * sizeof(s_leader_data)) == 0);
    DOCTEST_CHECK(std::memcmp(REPORT::_reports, expected_reports.data(), sizeof(REPORT::_reports)) == 0);
    DOCTEST_CHECK(std::memcmp(EVENTS::_event_data, expected_events.data(), sizeof(EVENTS::_event_data)) == 0);
    DOCTEST_CHECK(std::memcmp(MSG_::_first_msg, expected_first_msg.data(), sizeof(MSG_::_first_msg)) == 0);
    DOCTEST_CHECK(std::memcmp(MSG_::_max_msgs, expected_max_msgs.data(), sizeof(MSG_::_max_msgs)) == 0);
    DOCTEST_CHECK(std::memcmp(MSG_::_n_msgs, expected_n_msgs.data(), sizeof(MSG_::_n_msgs)) == 0);
    DOCTEST_CHECK(std::memcmp(MSG_::_msgs, expected_msgs.data(), sizeof(MSG_::_msgs)) == 0);
    DOCTEST_CHECK(std::memcmp(&MOX::_antarans, &expected_antarans, sizeof(expected_antarans)) == 0);
    DOCTEST_CHECK(std::memcmp(MOX::_nebula, expected_nebulas.data(), MAX_NEBULAS * sizeof(s_nebula)) == 0);
    DOCTEST_CHECK(std::memcmp(MOX::_bill_savegame, expected_bill.data(), sizeof(MOX::_bill_savegame)) == 0);

    std::vector<uint8_t> payload_roundtrip(Broadcast_Game_Data_Snapshot_Max_Bytes_());
    const uint32_t payload_roundtrip_len = NETMOX::Encode_Broadcast_Game_Data_Snapshot_(payload_roundtrip.data(), (uint32_t)payload_roundtrip.size());
    DOCTEST_CHECK(payload_roundtrip_len == payload_len);
    DOCTEST_CHECK(std::memcmp(payload_roundtrip.data(), payload.data(), payload_len) == 0);
}

DOCTEST_TEST_CASE("broadcast differential decode round trips against cached prior snapshot") {
    Scoped_Netmox_Roundtrip_State_ scope;
    Fill_Netmox_Roundtrip_State_();

    std::vector<uint8_t> base_payload(Broadcast_Game_Data_Snapshot_Max_Bytes_());
    const uint32_t base_payload_len = NETMOX::Encode_Broadcast_Game_Data_Snapshot_(base_payload.data(), (uint32_t)base_payload.size());

    s_net_snapshot_cache* cache = NETMOX::Open_Net_Snapshot_Cache_(base_payload_len);
    NETMOX::Write_Net_Snapshot_Cache_(cache, base_payload.data(), base_payload_len);

    MOX::_stardate += 19;
    MOX::_settings.galaxy_age = 3;
    MOX::_player[1].bc += 321;
    MOX::_player[1].tech_applications[3] ^= 0x5a;
    MOX::_star[2].owner = 2;
    MOX::_star[2].surrender_to[1] = 7;
    MOX::_planet[3].planet_type = 9;
    MOX::_colony[1].morale = 88;
    MOX::_ship[3].turns_left = 12;
    MOX::_leaders[4].eta = 21;
    REPORT::_n_reports = 2;
    REPORT::_reports[0].type = 9;
    EVENTS::_event_data[5].status = 6;
    MSG_::_first_msg[1] = 44;
    MSG_::_msgs[7].priority = 4;
    diplodef::_COUNCIL_FLAG = 0;
    diplodef::_next_council_meeting_turn = 99;
    MOX::_antarans.attack_pressure = 8;
    MOX::_nebula[0].x = 44;
    EVENTS::_last_event_year = 654321u;
    EVENTS::_event_delay_count = 4;
    MOX::_bill_savegame[1] = 777;

    const s_network expected_net = NETMOX::_net;
    const uint32_t expected_stardate = MOX::_stardate;
    const s_settings expected_settings = MOX::_settings;
    const uint32_t expected_random_seed = game_random::Get_Random_Seed_();
    const int16_t expected_num_players = MOX::_NUM_PLAYERS;
    const int16_t expected_num_stars = MOX::_NUM_STARS;
    const int16_t expected_planet_count = MOX::_planet_count;
    const int16_t expected_num_colonies = MOX::_NUM_COLONIES;
    const int16_t expected_num_ships = MOX::_NUM_SHIPS;
    const uint8_t expected_num_nebulas = MOX::_NUM_NEBULAS;
    const int16_t expected_report_count = REPORT::_n_reports;
    std::vector<s_player> expected_players(MOX::_player, MOX::_player + MAX_PLAYERS);
    std::vector<s_star_data> expected_stars(MOX::_star, MOX::_star + MAX_STARS + 1);
    std::vector<s_planet_data> expected_planets(MOX::_planet, MOX::_planet + MAX_PLANETS);
    std::vector<s_colony> expected_colonies(MOX::_colony, MOX::_colony + MAX_COLONIES);
    std::vector<s_ship_data> expected_ships(MOX::_ship, MOX::_ship + MAX_SHIPS);
    std::vector<s_leader_data> expected_leaders(MOX::_leaders, MOX::_leaders + MAX_LEADERS);
    std::array<s_report, 50> expected_reports = {};
    std::array<s_event_data, 36> expected_events = {};
    std::array<uint8_t, MAX_PLAYERS> expected_first_msg = {};
    std::array<uint8_t, MAX_PLAYERS> expected_max_msgs = {};
    std::array<uint8_t, MAX_PLAYERS> expected_n_msgs = {};
    std::array<s_MSG, 100> expected_msgs = {};
    std::array<int16_t, 6> expected_bill = {};
    std::vector<s_nebula> expected_nebulas(MOX::_nebula, MOX::_nebula + MAX_NEBULAS);
    const s_antaran expected_antarans = MOX::_antarans;

    std::memcpy(expected_reports.data(), REPORT::_reports, sizeof(REPORT::_reports));
    std::memcpy(expected_events.data(), EVENTS::_event_data, sizeof(EVENTS::_event_data));
    std::memcpy(expected_first_msg.data(), MSG_::_first_msg, sizeof(MSG_::_first_msg));
    std::memcpy(expected_max_msgs.data(), MSG_::_max_msgs, sizeof(MSG_::_max_msgs));
    std::memcpy(expected_n_msgs.data(), MSG_::_n_msgs, sizeof(MSG_::_n_msgs));
    std::memcpy(expected_msgs.data(), MSG_::_msgs, sizeof(MSG_::_msgs));
    std::memcpy(expected_bill.data(), MOX::_bill_savegame, sizeof(MOX::_bill_savegame));

    std::vector<uint8_t> next_payload(Broadcast_Game_Data_Snapshot_Max_Bytes_());
    const uint32_t next_payload_len = NETMOX::Encode_Broadcast_Game_Data_Snapshot_(next_payload.data(), (uint32_t)next_payload.size());

    std::vector<uint8_t> diff_payload(NETMOX::NETMOX_MAX_GAME_DATA_PAYLOAD_BYTES - next_payload_len);
    const int32_t diff_payload_len = NETMOX::Make_Difference_Buffer_(
        cache,
        next_payload.data(),
        (int32_t)next_payload_len,
        diff_payload.data(),
        (int32_t)diff_payload.size());

    std::memset(&NETMOX::_net, 0, sizeof(NETMOX::_net));
    MOX::_stardate = 0;
    std::memset(&MOX::_settings, 0, sizeof(MOX::_settings));
    MOX::_NUM_PLAYERS = 0;
    MOX::_NUM_STARS = 0;
    MOX::_planet_count = 0;
    MOX::_NUM_COLONIES = 0;
    MOX::_NUM_SHIPS = 0;
    MOX::_NUM_NEBULAS = 0;
    REPORT::_n_reports = 0;
    std::memset(MOX::_player, 0, MAX_PLAYERS * sizeof(s_player));
    std::memset(MOX::_star, 0, (MAX_STARS + 1) * sizeof(s_star_data));
    std::memset(MOX::_planet, 0, MAX_PLANETS * sizeof(s_planet_data));
    std::memset(MOX::_colony, 0, MAX_COLONIES * sizeof(s_colony));
    std::memset(MOX::_ship, 0, MAX_SHIPS * sizeof(s_ship_data));
    std::memset(MOX::_leaders, 0, MAX_LEADERS * sizeof(s_leader_data));
    std::memset(REPORT::_reports, 0, sizeof(REPORT::_reports));
    std::memset(EVENTS::_event_data, 0, sizeof(EVENTS::_event_data));
    std::memset(MSG_::_first_msg, 0, sizeof(MSG_::_first_msg));
    std::memset(MSG_::_max_msgs, 0, sizeof(MSG_::_max_msgs));
    std::memset(MSG_::_n_msgs, 0, sizeof(MSG_::_n_msgs));
    std::memset(MSG_::_msgs, 0, sizeof(MSG_::_msgs));
    std::memset(&MOX::_antarans, 0, sizeof(MOX::_antarans));
    std::memset(MOX::_nebula, 0, MAX_NEBULAS * sizeof(s_nebula));
    std::memset(MOX::_bill_savegame, 0, sizeof(MOX::_bill_savegame));
    game_random::Set_Random_Seed_(0);

    DOCTEST_REQUIRE(NETMOX::Decode_Broadcast_Game_Data_Differential_(diff_payload.data(), (size_t)diff_payload_len, 0) == 1);

    DOCTEST_CHECK(NETMOX::_net.host_election_id == expected_net.host_election_id);
    DOCTEST_CHECK(MOX::_stardate == expected_stardate);
    DOCTEST_CHECK(std::memcmp(&MOX::_settings, &expected_settings, sizeof(expected_settings)) == 0);
    DOCTEST_CHECK(game_random::Get_Random_Seed_() == expected_random_seed);
    DOCTEST_CHECK(MOX::_NUM_PLAYERS == expected_num_players);
    DOCTEST_CHECK(MOX::_NUM_STARS == expected_num_stars);
    DOCTEST_CHECK(MOX::_planet_count == expected_planet_count);
    DOCTEST_CHECK(MOX::_NUM_COLONIES == expected_num_colonies);
    DOCTEST_CHECK(MOX::_NUM_SHIPS == expected_num_ships);
    DOCTEST_CHECK(MOX::_NUM_NEBULAS == expected_num_nebulas);
    DOCTEST_CHECK(REPORT::_n_reports == expected_report_count);
    DOCTEST_CHECK(std::memcmp(MOX::_player, expected_players.data(), MAX_PLAYERS * sizeof(s_player)) == 0);
    DOCTEST_CHECK(std::memcmp(MOX::_star, expected_stars.data(), (MAX_STARS + 1) * sizeof(s_star_data)) == 0);
    DOCTEST_CHECK(std::memcmp(MOX::_planet, expected_planets.data(), MAX_PLANETS * sizeof(s_planet_data)) == 0);
    DOCTEST_CHECK(std::memcmp(MOX::_colony, expected_colonies.data(), MAX_COLONIES * sizeof(s_colony)) == 0);
    DOCTEST_CHECK(std::memcmp(MOX::_ship, expected_ships.data(), MAX_SHIPS * sizeof(s_ship_data)) == 0);
    DOCTEST_CHECK(std::memcmp(MOX::_leaders, expected_leaders.data(), MAX_LEADERS * sizeof(s_leader_data)) == 0);
    DOCTEST_CHECK(std::memcmp(REPORT::_reports, expected_reports.data(), sizeof(REPORT::_reports)) == 0);
    DOCTEST_CHECK(std::memcmp(EVENTS::_event_data, expected_events.data(), sizeof(EVENTS::_event_data)) == 0);
    DOCTEST_CHECK(std::memcmp(MSG_::_first_msg, expected_first_msg.data(), sizeof(MSG_::_first_msg)) == 0);
    DOCTEST_CHECK(std::memcmp(MSG_::_max_msgs, expected_max_msgs.data(), sizeof(MSG_::_max_msgs)) == 0);
    DOCTEST_CHECK(std::memcmp(MSG_::_n_msgs, expected_n_msgs.data(), sizeof(MSG_::_n_msgs)) == 0);
    DOCTEST_CHECK(std::memcmp(MSG_::_msgs, expected_msgs.data(), sizeof(MSG_::_msgs)) == 0);
    DOCTEST_CHECK(std::memcmp(&MOX::_antarans, &expected_antarans, sizeof(expected_antarans)) == 0);
    DOCTEST_CHECK(std::memcmp(MOX::_nebula, expected_nebulas.data(), MAX_NEBULAS * sizeof(s_nebula)) == 0);
    DOCTEST_CHECK(std::memcmp(MOX::_bill_savegame, expected_bill.data(), sizeof(MOX::_bill_savegame)) == 0);

    std::vector<uint8_t> payload_roundtrip(Broadcast_Game_Data_Snapshot_Max_Bytes_());
    const uint32_t payload_roundtrip_len = NETMOX::Encode_Broadcast_Game_Data_Snapshot_(payload_roundtrip.data(), (uint32_t)payload_roundtrip.size());
    DOCTEST_CHECK(payload_roundtrip_len == next_payload_len);
    DOCTEST_CHECK(std::memcmp(payload_roundtrip.data(), next_payload.data(), next_payload_len) == 0);
}

DOCTEST_TEST_CASE("broadcast snapshot encode suppresses live network updates while serializing") {
    Scoped_Netmox_Roundtrip_State_ scope;
    Fill_Netmox_Roundtrip_State_();

    MOX::_game_type = 3;
    NETMOX::_net.is_network_active = 1;
    NETMOX::_net.sync_state = NET_SYNC_STATE_READY;
    g_net_update_hook_invocations = 0;
    netcode::g_net_update_hook = &Snapshot_Encode_Mutation_Hook_;

    std::vector<uint8_t> payload(Broadcast_Game_Data_Snapshot_Max_Bytes_());
    const int32_t expected_bc = MOX::_player[1].bc;
    const uint32_t payload_len = NETMOX::Encode_Broadcast_Game_Data_Snapshot_(payload.data(), (uint32_t)payload.size());

    DOCTEST_CHECK(payload_len > 0);
    DOCTEST_CHECK(g_net_update_hook_invocations == 0);
    DOCTEST_CHECK(MOX::_player[1].bc == expected_bc);

    netcode::g_net_update_hook = nullptr;
}

}