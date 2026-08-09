#include "pch.h"

namespace NETTURN {

    char* _waiting_message_fmt;

    int32_t __cdecl Determine_Lowest_Active_Host_Player_(const s_player* players, int32_t num_players) {
        if (players == nullptr || num_players <= 0) {
            return -1;
        }

        for (int32_t i = 0; i < num_players; ++i) {
            if (players[i].objectives == PLAYER_OBJECTIVE_HUMAN && players[i].network_player_id != -1 && players[i].eliminated == 0) {
                return i;
            }
        }

        return -1;
    }

    void __cdecl Net_Next_Turn_() {
        s_net_chat chat_buffer;
        s_net_election_msg election_msg;

        CMBTFIR2::Save_Artemis_Random_Seed_();

        if (MOX::_player[MOX::_PLAYER_NUM].surrender_to != -1) {
            NETMOX::Human_Surrenders_(MOX::_PLAYER_NUM);
            NETSTART::Mox_Net_Uninit_();
            MOX::_current_screen = 10;
            MOX::_return_screen = 10;
            MOX::_previous_screen = 10;
            return;
        }

        NETMOX::_net.players_ready_mask[0] = 0;
        NETMOX::_net.players_active_mask = 0;
        memset(&chat_buffer, 0, sizeof(chat_buffer));
        CHAT::_chat_info = &chat_buffer;

        MULTPLAY::Load_Net_Next_Turn_Screen_(0);
        fields::Assign_Auto_Function_(MULTPLAY::Draw_Net_Next_Turn_Screen_, 1);
        MULTPLAY::Add_Net_Next_Turn_Fields_();

        NETMOX::_net.sync_state = NET_SYNC_STATE_NEXT_TURN_READY;
        NETMOX::_net.players_hit_next_turn[MOX::_PLAYER_NUM] = 1;

        if (NETMOX::_net.host_election_player_rank[MOX::_PLAYER_NUM] == -1) {
            NETMOX::_net.host_election_player_rank[MOX::_PLAYER_NUM] = (int8_t)MOX::_PLAYER_NUM;
        }

        election_msg.hit_next_turn = 1;

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].objectives == PLAYER_OBJECTIVE_HUMAN) {
                if (MOX::_PLAYER_NUM != i) {
                    election_msg.rank = (int32_t)NETMOX::_net.host_election_player_rank[MOX::_PLAYER_NUM];
                    NETMOX::Mox_Send_Message_(MOX::_player[i].network_player_id, NET_PACKET_HOST_ELECTION_RANK, &election_msg, sizeof(election_msg));
                }
            }
            MULTPLAY::Chat_Box_Input_Loop_();
        }

        while (true) {
            int elected_host_player_idx = Determine_Lowest_Active_Host_Player_(MOX::_player, MOX::_NUM_PLAYERS);

            if (elected_host_player_idx != -1) {
                NETMOX::_net.local_net_id = MOX::_player[elected_host_player_idx].network_player_id;
            }

            MULTPLAY::Chat_Box_Input_Loop_();

            if (NETMOX::_net.local_net_id != -1) {
                if (NETMOX::_net.local_net_id == NETMOX::_net.net_host_id) {
                    Host_Next_Turn_();
                } else {
                    Client_Next_Turn_();
                }

                NETMOX::_net.players_active_mask = 0;
                NETMOX::_net.players_ready_mask[0] = 0;
                CHAT::_chat_info = nullptr;

                if (MOX::_player[MOX::_PLAYER_NUM].eliminated == 0) {
                    ERICNET::Mox_Play_Sound_(0x33);
                    MOX::_previous_screen = SCREEN_NET_NEXT_TURN;
                    MOX::_current_screen = SCREEN_REPORTS;
                    return;
                }

                ERIC::Play_Loose_Flic_();
                while (netcode::Net_Is_Sending_()) {
                    RUSS::Mox_Update_();
                }

                NETSTART::Mox_Net_Uninit_();
                MOX::_game_type = 0;
                MOX::_previous_screen = SCREEN_NET_NEXT_TURN;
                MOX::_current_screen = SCREEN_MAIN_MENU;
                return;
            }
        }
    }

    char* __cdecl Load_Netturn_Strings_(char* string_block) {
        _waiting_message_fmt = string_block;

        char* next_ptr = _waiting_message_fmt + strlen(_waiting_message_fmt) + 1;

        return next_ptr + strlen(next_ptr) + 1;
    }

    void __cdecl Host_Next_Turn_() {
        NETMOX::Net_Debug_Log_(
            "Host_Next_Turn_: start local_player=%d stardate=%u connected=%d",
            (int)MOX::_PLAYER_NUM,
            (unsigned int)MOX::_stardate,
            (int)NETMOX::_net.num_connected_players);
        NETMOX::Clear_Leader_Deassignments_();
        NETMOX::_net.host_sync_counter = 0;

        if (MOX::_NUM_PLAYERS > 0) {
            for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                if (MOX::_player[i].objectives != PLAYER_OBJECTIVE_HUMAN || MOX::_PLAYER_NUM == i || MOX::_player[i].eliminated != 0) {
                    NETMOX::_net.host_sync_counter++;
                    NETMOX::_net.players_ready_mask[i + 1] = 1;
                }
            }
        }

        NETMOX::_net.sync_state = NET_SYNC_STATE_HOST_WAITING_FOR_ORDERS;
        if (NETMOX::_net.host_sync_counter >= MOX::_NUM_PLAYERS) {
            NETMOX::_net.sync_state = NET_SYNC_STATE_HOST_BEGIN_TURN;
        }

        while (NETMOX::_net.sync_state != NET_SYNC_STATE_HOST_BEGIN_TURN) {
            for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                MULTPLAY::Chat_Box_Input_Loop_();

                if (NETMOX::_net.players_hit_next_turn[i] != 0 && NETMOX::_net.player_is_active_flags[i] != 0) {
                    NETMOX::_net.player_is_active_flags[i] = 0;
                    NETMOX::_net.host_active_player_id = (int8_t)i;
                }

                if (NETMOX::_net.host_active_player_id != -1) {
                    break;
                }
            }

            if (NETMOX::_net.host_active_player_id != -1) {
                NETMOX::_net.sync_state = NET_SYNC_STATE_HOST_RECEIVING_PLAYER_DATA;
                NETMOX::Net_Debug_Log_(
                    "Host_Next_Turn_: requesting game orders from player=%d host_sync_counter=%d",
                    (int)NETMOX::_net.host_active_player_id,
                    (int)NETMOX::_net.host_sync_counter);
                NETMOX::Mox_Send_Message_(MOX::_player[NETMOX::_net.host_active_player_id].network_player_id, NET_PACKET_SEND_GAME_ORDERS_REQUEST, nullptr, 0);

                while (NETMOX::_net.sync_state == NET_SYNC_STATE_HOST_RECEIVING_PLAYER_DATA) {
                    MULTPLAY::Chat_Box_Input_Loop_();
                }

                NETMOX::Net_Debug_Log_(
                    "Host_Next_Turn_: received game orders from player=%d bytes=%u next_sync=%u",
                    (int)NETMOX::_net.host_active_player_id,
                    (unsigned int)NETMOX::_net.total_bytes_received,
                    (unsigned int)NETMOX::_net.sync_state);
                NETMOX::Decode_Game_Orders_(NETMOX::_net.network_data_ptr, NETMOX::_net.total_bytes_received);
                NETMOX::_net.network_data_ptr = nullptr;
                allocate::Pop_Block_(MOX::_screen_seg);
                NETMOX::_net.players_ready_mask[NETMOX::_net.host_active_player_id + 1] = 1;
                NETMOX::_net.host_active_player_id = -1;
            }
        }

        MAINSCR::Update_Ship_Icons_In_Between_Turns_();
        COLCALC::No_Op1_();
        NETMOX::_net.sync_phase_active_flag = 0;

        memset(
            &NETMOX::_net.player_net_info,
            0,
            offsetof(s_network, sync_state) - offsetof(s_network, player_net_info)
        );

        if (MOX::_NUM_PLAYERS > 0) {
            for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                NETMOX::_net.player_net_info[i].combat_attacker_id = -1;
                NETMOX::_net.player_net_info[i].player_state = NET_PLAYER_STATE_COMBAT_READY;
            }
        }

        NETMOX::_net.sync_state = NET_SYNC_STATE_COMBAT_READY;
        NEXTTURN::Next_Turn_();

        NETMOX::_net.host_election_winner_id = NETMOX::_net.net_host_id;
        Net_Init_Host_Election_();
        NETMOX::_net.field130_0x1a4 = 0;

        if (MOX::_NUM_PLAYERS > 0) {
            for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                if (MOX::_PLAYER_NUM != i && MOX::_player[i].network_player_id != -1 && MOX::_player[i].eliminated != 0) {
                    NETMOX::Mox_Send_Message_(MOX::_player[i].network_player_id, NET_PACKET_FORCE_PLAYER_ELIMINATED, nullptr, 0);
                    MOX::_player[i].objectives = 0;
                    MOX::_player[i].network_player_id = -1;
                    NETMOX::_net.num_connected_players--;
                    RUSS::Mox_Update_();
                }
            }
        }

        NETMOX::Net_Debug_Log_(
            "Host_Next_Turn_: broadcasting differential stardate=%u connected=%d",
            (unsigned int)MOX::_stardate,
            (int)NETMOX::_net.num_connected_players);
        NETMOX::Broadcast_Game_Data_Differential_(0);

        if (MOX::_player[MOX::_PLAYER_NUM].eliminated == 0) {
            NETMOX::_net.player_race_selected_flags[MOX::_PLAYER_NUM] = 1;
            NETMOX::_net.field_1db = 0;
            NETMOX::_net.field_1df = 0;
            NETMOX::_net.sync_state = NET_SYNC_STATE_READY;

            if (MOX::_NUM_PLAYERS > 0) {
                for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                    NETMOX::_net.player_net_info[i].player_state = NET_PLAYER_STATE_READY;
                    NETMOX::_net.player_net_info[i].diplomacy_target_id = -1;
                }
            }

            MAINSCR::Update_Ship_Icons_In_Between_Turns_();

            if (NETMOX::_net.num_connected_players == 1) {
                NETSTART::Mox_Net_Uninit_();
                MOX::_game_type = 0;
                REPORT::_something_interesting_happened[MOX::_PLAYER_NUM] = 1;
            }
        }

        NETMOX::Net_Debug_Log_(
            "Host_Next_Turn_: done stardate=%u sync=%u connected=%d",
            (unsigned int)MOX::_stardate,
            (unsigned int)NETMOX::_net.sync_state,
            (int)NETMOX::_net.num_connected_players);
    }

    int8_t __cdecl Must_Broadcast_AI_Player_Data_() {
        int16_t num_players = MOX::_NUM_PLAYERS;

        for (int i = 0; i < num_players; ++i) {
            uint8_t sync_mask = NETMOX::_net.players_sync_mask[i];

            for (int j = 0; j < num_players; ++j) {
                if ((sync_mask >> j) & 1) {
                    return 1;
                }
            }
        }

        return 0;
    }

    void __cdecl Net_Init_Host_Election_() {
        memset(
            NETMOX::_net.host_election_player_rank,
            -1,
            sizeof(NETMOX::_net.host_election_player_rank)
        );
        memset(
            NETMOX::_net.host_election_rank_sync_cache,
            -1,
            sizeof(NETMOX::_net.host_election_rank_sync_cache)
        );
        memset(
            NETMOX::_net.players_hit_next_turn,
            0,
            sizeof(NETMOX::_net.players_hit_next_turn)
        );

        NETMOX::_net.local_net_id = -1;
        NETMOX::_net.host_active_player_id = -1;
        NETMOX::_net.field23_0xc6 = 0;
        NETMOX::_net.diplomacy_initiator_id = -1;

        memset(
            &NETMOX::_net.players_ready_mask[1],
            0,
            sizeof(NETMOX::_net.players_ready_mask) - sizeof(NETMOX::_net.players_ready_mask[0])
        );
        NETMOX::_net.field132_0x1ad = 0;

        memset(
            NETMOX::_net.player_is_active_flags,
            0,
            sizeof(NETMOX::_net.player_is_active_flags)
        );
        memset(
            NETMOX::_net.player_race_selected_flags,
            0,
            sizeof(NETMOX::_net.player_race_selected_flags)
        );
    }

    void __cdecl Client_Next_Turn_() {
        uint8_t star_has_nonplayer;
        s_combfind_star_has_player star_has_player[MAX_PLAYERS];

        NETMOX::Net_Debug_Log_(
            "Client_Next_Turn_: start local_player=%d stardate=%u local_net_id=%d host_id=%d sync=%u",
            (int)MOX::_PLAYER_NUM,
            (unsigned int)MOX::_stardate,
            (int)NETMOX::_net.local_net_id,
            (int)NETMOX::_net.net_host_id,
            (unsigned int)NETMOX::_net.sync_state);

        NETMOX::_net.network_data_ptr = nullptr;
        NETMOX::_net.total_bytes_received = 0;

        NETMOX::Mox_Send_Message_(NETMOX::_net.local_net_id, NET_PACKET_TURN_END_REQUEST, nullptr, 0);
        NETMOX::Net_Debug_Log_("Client_Next_Turn_: sent TURN_END_REQUEST");

        do {
            MULTPLAY::Chat_Box_Input_Loop_();
        } while (NETMOX::_net.sync_state == NET_SYNC_STATE_NEXT_TURN_READY);

        NETMOX::Net_Debug_Log_(
            "Client_Next_Turn_: left NEXT_TURN_READY sync=%u",
            (unsigned int)NETMOX::_net.sync_state);

        COMBFIND::_current_star_has_player = star_has_player;
        COMBFIND::_current_star_has_nonplayer = &star_has_nonplayer;

        while (NETTURN::Must_Broadcast_AI_Player_Data_()) {
            MULTPLAY::Chat_Box_Input_Loop_();
        }

        NETMOX::Send_Game_Orders_();
        NETMOX::Net_Debug_Log_(
            "Client_Next_Turn_: sent game orders sync=%u",
            (unsigned int)NETMOX::_net.sync_state);
        COMBFIND::Client_Combat_Loop_();

        COMBFIND::_current_star_has_player = nullptr;
        COMBFIND::_current_star_has_nonplayer = nullptr;

        strlcpy(MOX::_temp_string, NETTURN::_waiting_message_fmt, sizeof(MOX::_temp_string));
        NETMOX::Net_Debug_Log_("Client_Next_Turn_: waiting for broadcast game data payload");

        while (NETMOX::_net.network_data_ptr == nullptr && MOX::_player[MOX::_PLAYER_NUM].eliminated == 0) {
            MULTPLAY::Chat_Box_Input_Loop_();
        }

        if (MOX::_player[MOX::_PLAYER_NUM].eliminated == 0) {
            NETMOX::Net_Debug_Log_(
                "Client_Next_Turn_: received payload bytes=%u sync=%u",
                (unsigned int)NETMOX::_net.total_bytes_received,
                (unsigned int)NETMOX::_net.sync_state);
            while (!NETMOX::Decode_Broadcast_Game_Data_Differential_(NETMOX::_net.network_data_ptr, NETMOX::_net.total_bytes_received, 0)) {
                exitm::Exit_With_Message("craziness");
            }

            NETMOX::_net.field_1db = 0;
            NETMOX::_net.field_1df = 0;
            NETMOX::_net.network_data_ptr = nullptr;
            allocate::Pop_Block_(MOX::_screen_seg);
            NETMOX::_net.sync_state = NET_SYNC_STATE_READY;

            MAINSCR::Update_Ship_Icons_In_Between_Turns_();

            if (NETMOX::_net.num_connected_players == 1) {
                NETSTART::Mox_Net_Uninit_();
                MOX::_game_type = 0;
            }

            NETMOX::Net_Debug_Log_(
                "Client_Next_Turn_: decode complete stardate=%u sync=%u connected=%d",
                (unsigned int)MOX::_stardate,
                (unsigned int)NETMOX::_net.sync_state,
                (int)NETMOX::_net.num_connected_players);
        }

        COMBFIND::_current_star_has_player = nullptr;
        COMBFIND::_current_star_has_nonplayer = nullptr;
    }
}