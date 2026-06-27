#include "pch.h"

#include "doctest/doctest.h"

DOCTEST_TEST_SUITE("netmox packet limits") {

DOCTEST_TEST_CASE("large combat payload allocations stay within the shared limit") {
    constexpr uint32_t k_medium_network_payload_bytes = 200 * 1024u;
    constexpr uint32_t k_large_network_payload_bytes = 0xE1000u;
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
        2 * sizeof(MOX::_player[0].tech_applications) +
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
        MAX_PLAYERS * sizeof(MOX::_player[0].tech_applications) +
        MAX_LEADERS * (sizeof(int16_t) + sizeof(s_leader_data)) +
        sizeof(int16_t) +
        MAX_PLAYERS * sizeof(uint8_t) +
        sizeof(int16_t) +
        5 * (sizeof(int16_t) * 2 + sizeof(uint8_t) * 2 + sizeof(s_colony)) +
        sizeof(int16_t) +
        MAX_SHIPS * (sizeof(int16_t) + sizeof(s_ship_data)) +
        sizeof(int16_t);

    DOCTEST_CHECK(game_orders_payload_bytes <= k_large_network_payload_bytes);
    DOCTEST_CHECK(battle_results_remote_payload_bytes <= k_medium_network_payload_bytes);
    DOCTEST_CHECK(you_are_target_remote_payload_bytes <= k_medium_network_payload_bytes);
    DOCTEST_CHECK(pick_target_remote_payload_bytes <= k_medium_network_payload_bytes);
}

}