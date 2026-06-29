#include "pch.h"

#include "doctest/doctest.h"

#include "game/netmox.h"
#include "game/netturn.h"

DOCTEST_TEST_SUITE("netturn host selection") {

DOCTEST_TEST_CASE("selects the lowest active player slot") {
    s_player players[3]{};
    players[0].objectives = PLAYER_OBJECTIVE_DIPLOMATIC;
    players[0].network_player_id = 10;
    players[1].objectives = PLAYER_OBJECTIVE_HUMAN;
    players[1].network_player_id = 21;
    players[2].objectives = PLAYER_OBJECTIVE_HUMAN;
    players[2].network_player_id = 22;

    DOCTEST_CHECK(NETTURN::Determine_Lowest_Active_Host_Player_(players, 3) == 1);
}

DOCTEST_TEST_CASE("skips active slots without a valid network id") {
    s_player players[3]{};
    players[0].objectives = PLAYER_OBJECTIVE_HUMAN;
    players[0].network_player_id = -1;
    players[1].objectives = PLAYER_OBJECTIVE_HUMAN;
    players[1].network_player_id = -1;
    players[2].objectives = PLAYER_OBJECTIVE_HUMAN;
    players[2].network_player_id = 42;

    DOCTEST_CHECK(NETTURN::Determine_Lowest_Active_Host_Player_(players, 3) == 2);
}

DOCTEST_TEST_CASE("ignores inactive or eliminated slots even if they have network ids") {
    s_player players[3]{};
    players[0].objectives = 12;
    players[0].network_player_id = 7;
    players[1].objectives = 99;
    players[1].network_player_id = 8;
    players[2].objectives = PLAYER_OBJECTIVE_HUMAN;
    players[2].network_player_id = 9;

    DOCTEST_CHECK(NETTURN::Determine_Lowest_Active_Host_Player_(players, 3) == 2);
}

DOCTEST_TEST_CASE("skips eliminated active players") {
    s_player players[3]{};
    players[0].objectives = PLAYER_OBJECTIVE_HUMAN;
    players[0].network_player_id = 15;
    players[0].eliminated = 1;
    players[1].objectives = PLAYER_OBJECTIVE_HUMAN;
    players[1].network_player_id = 27;
    players[1].eliminated = 0;
    players[2].objectives = PLAYER_OBJECTIVE_HUMAN;
    players[2].network_player_id = 39;
    players[2].eliminated = 0;

    DOCTEST_CHECK(NETTURN::Determine_Lowest_Active_Host_Player_(players, 3) == 1);
}

DOCTEST_TEST_CASE("returns no host when no active player has a valid network id") {
    s_player players[3]{};
    players[0].objectives = PLAYER_OBJECTIVE_DIPLOMATIC;
    players[0].network_player_id = -1;
    players[1].objectives = PLAYER_OBJECTIVE_HUMAN;
    players[1].network_player_id = -1;
    players[2].objectives = PLAYER_OBJECTIVE_DIPLOMATIC;
    players[2].network_player_id = 12;

    DOCTEST_CHECK(NETTURN::Determine_Lowest_Active_Host_Player_(players, 3) == -1);
}

DOCTEST_TEST_CASE("returns no host for empty input") {
    DOCTEST_CHECK(NETTURN::Determine_Lowest_Active_Host_Player_(static_cast<const s_player*>(nullptr), 0) == -1);
}

}

DOCTEST_TEST_SUITE("netmox disconnect recovery") {

DOCTEST_TEST_CASE("client handles host disconnect when local player becomes recovery coordinator") {
    s_player players[3]{};

    players[0].network_player_id = 202;
    players[0].objectives = PLAYER_OBJECTIVE_HUMAN;
    players[0].eliminated = 0;

    players[1].network_player_id = 101;
    players[1].objectives = PLAYER_OBJECTIVE_HUMAN;
    players[1].eliminated = 0;

    players[2].network_player_id = 303;
    players[2].objectives = PLAYER_OBJECTIVE_HUMAN;
    players[2].eliminated = 0;

    MOX::_player = players;
    MOX::_PLAYER_NUM = 0;
    MOX::_NUM_PLAYERS = 3;
    MOX::_NUM_STARS = 0;

    NETMOX::_net = {};
    NETMOX::_net.is_network_active = 1;
    NETMOX::_net.net_host_id = 101;
    NETMOX::_net.local_net_id = 202;
    NETMOX::_net.host_election_id = 101;
    NETMOX::_net.host_election_winner_id = 101;
    NETMOX::_net.num_connected_players = 3;

    NETMOX::Handle_Network_Player_Disconnected_(1);

    DOCTEST_CHECK(MOX::_player[1].network_player_id == -1);
    DOCTEST_CHECK(NETMOX::_net.num_connected_players == 2);
    DOCTEST_CHECK(NETMOX::_net.players_hit_next_turn[1] == 1);
    DOCTEST_CHECK(NETMOX::_net.host_election_id == 202);
    DOCTEST_CHECK(NETMOX::_net.host_election_winner_id == 202);
    DOCTEST_CHECK(NETMOX::_net.local_net_id == 202);
}

DOCTEST_TEST_CASE("local player disconnect is ignored") {
    s_player players[2]{};

    players[0].network_player_id = 202;
    players[0].objectives = PLAYER_OBJECTIVE_HUMAN;
    players[1].network_player_id = 101;
    players[1].objectives = PLAYER_OBJECTIVE_HUMAN;

    MOX::_player = players;
    MOX::_PLAYER_NUM = 0;
    MOX::_NUM_PLAYERS = 2;
    MOX::_NUM_STARS = 0;

    NETMOX::_net = {};
    NETMOX::_net.is_network_active = 1;
    NETMOX::_net.net_host_id = 101;
    NETMOX::_net.local_net_id = 202;
    NETMOX::_net.num_connected_players = 2;

    NETMOX::Handle_Network_Player_Disconnected_(0);

    DOCTEST_CHECK(MOX::_player[0].network_player_id == 202);
    DOCTEST_CHECK(NETMOX::_net.num_connected_players == 2);
}

}