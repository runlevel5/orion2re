#include "pch.h"

namespace NETSTART {
    int8_t __cdecl Net_Select_Race_() {
        uint8_t fade_flag;
        s_player* local_player = &MOX::_player[MOX::_PLAYER_NUM];

        RACESEL::Race_Selection_Screen_(local_player);

        local_player->objectives = 100;
        MULTPLAY::Reload_Generating_Map_Info_();

        MISC::Fade_Into_Screen_(MULTPLAY::Draw_Generic_Net_Info_Screen_, &fade_flag, 11);
        MULTPLAY::Draw_Generic_Net_Info_Screen_();
        RUSS::Mox_Sync_Update_();

        NETMOX::_net.total_chunk_count = 0;
        NETMOX::_net.packet_type = 7; /* PACKET_TYPE_RACE_SELECTION */

        NETMOX::Mox_Send_Message_(NETMOX::_net.host_election_id, 1, local_player, 3753 /* s_player size */);

        while (NETMOX::_net.total_chunk_count == 0) {
            fields::Get_Input_();
            MULTPLAY::Draw_Generic_Net_Info_Screen_();
            RUSS::Mox_Sync_Update_();
        }

        MULTPLAY::Reload_Getting_Data_Info_();

        strcpy(MOX::_temp_string, NETTURN::_waiting_fmt);
        video::Set_Page_Off_();
        MULTPLAY::Draw_SendGet_Net_Info_Screen_();
        fields::Assign_Auto_Function_(MULTPLAY::Draw_SendGet_Net_Info_Screen_, 2);
        RUSS::Mox_Sync_Update_();

        while (NETMOX::_net.received_chunk_count < NETMOX::_net.total_chunk_count) {
            _sprintf(MOX::_temp_string, NETSTART::_receiving_chunk_d_of_d, 
                    NETMOX::_net.received_chunk_count + 1, NETMOX::_net.total_chunk_count);
            MULTPLAY::Draw_SendGet_Net_Info_Screen_();
            fields::Get_Input_();
            RUSS::Mox_Sync_Update_();
        }

        NETMOX::Decode_Broadcast_Game_Data_Differential_(NETMOX::_net.network_data_ptr, 
                                                       NETMOX::_net.total_bytes_received, 1);
        
        NETMOX::_net.field_1db = 0;
        NETMOX::_net.field_1df = 0;
        NETMOX::_net.network_data_ptr = nullptr;

        allocate::Pop_Block_(MOX::_screen_seg);
        NETMOX::_net.total_chunk_count = 0;
        NETMOX::_net.packet_type = 4; /* PACKET_TYPE_MAP_SYNC */

        MOX2::Init_Map_Pictures_Default__();
        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        MAPGEN::Init_Main_Screen_(MOX::_settings.galaxy_size);
        HAROLD::Harold_Init_();
        MOX2::Init_Map_Defaults_();
        MAINSCR2::_main_screen_fade_in_flag = 1;
        MAINSCR::Update_Ship_Icons_In_Between_Turns_();

        return 1;
    }

    int8_t __cdecl Net_Pick_Position_() {
        int8_t connected_players[8];
        int16_t selection;
        int32_t message_buffer;

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            connected_players[i] = (MOX::_player[i].network_player_id != -1);
        }

        if (MULTPLAY::Choose_Network_Plyrs_Screen_(&selection, connected_players, false, true) == -1) {
            message_buffer = -1;
            NETMOX::Mox_Send_Message_(NETMOX::_net.host_election_id, 33 /* 0x21 */, &message_buffer, 4);
            return 0;
        }

        fields::Clear_Fields_();
        fields::Add_Hidden_Field_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */, s__00551100, 0);

        NETMOX::_net.packet_type = 7;
        message_buffer = (int32_t)selection;
        NETMOX::Mox_Send_Message_(NETMOX::_net.host_election_id, 33 /* 0x21 */, &message_buffer, 4);

        fields::Assign_Auto_Function_(MULTPLAY::Draw_SendGet_Net_Info_Screen_, 2);
        MULTPLAY::Reload_Getting_Data_Info_();

        strcpy(MOX::_temp_string, NETTURN::_waiting_fmt);

        while (NETMOX::_net.total_chunk_count == 0) {
            fields::Get_Input_();
            video::Set_Page_Off_();
            MULTPLAY::Draw_SendGet_Net_Info_Screen_();
            RUSS::Mox_Sync_Update_();
        }

        while (NETMOX::_net.received_chunk_count < NETMOX::_net.total_chunk_count) {
            _sprintf(MOX::_temp_string, NETSTART::_receiving_chunk_d_of_d, (int)NETMOX::_net.received_chunk_count + 1, (int)NETMOX::_net.total_chunk_count);
            fields::Get_Input_();
            MULTPLAY::Draw_SendGet_Net_Info_Screen_();
            RUSS::Mox_Sync_Update_();
        }

        NETMOX::Decode_Broadcast_Game_Data_Differential_(NETMOX::_net.network_data_ptr, NETMOX::_net.total_bytes_received, 1);
        NETMOX::_net.field_1db = 0;
        NETMOX::_net.field_1df = 0;
        NETMOX::_net.network_data_ptr = nullptr;
        allocate::Pop_Block_(MOX::_screen_seg);
        NETMOX::_net.total_chunk_count = 0;

        MOX::_temp_string[0] = '\0';
        MOX::_PLAYER_NUM = -1;
        NETMOX::_net.num_connected_players = 0;

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].objectives == 100 /* 'd' */) {
                NETMOX::_net.num_connected_players++;
                if (MOX::_player[i].network_player_id == NETMOX::_net.net_host_id) {
                    MOX::_PLAYER_NUM = (int16_t)i;
                }
            }
        }

        NETMOX::_net.packet_type = 4;
        MOX2::Init_Map_Pictures_Default__();
        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        MAPGEN::Init_Main_Screen_(MOX::_settings.galaxy_size);
        HAROLD::Harold_Init_();
        MOX2::Init_Map_Defaults_();
        MAINSCR2::_main_screen_fade_in_flag = 1;
        MAINSCR::Update_Ship_Icons_In_Between_Turns_();

        return 1;
    }

    int8_t __cdecl Do_Answer_() {
        clock_t start_time = _clock();
        simmodem::Modem_Reset_(NETMOX::_net.modem_settings.cmd_reset);

        int8_t is_connected = 0;
        if (simmodem::Modem_Answer_() != 0) {
            if (simmodem::Modem_Connected_() == 0) {
                while ((_clock() - start_time) < 90000) {
                    RUSS::Mox_Update_();
                    if (simmodem::Modem_Connected_() != 0) {
                        break;
                    }
                }
            }
            is_connected = (int8_t)simmodem::Modem_Connected_();
        }

        if (is_connected != 0) {
            return is_connected;
        }

        GENDRAW::Message_Box_(NETSTART::_answering_failed_error);
        netcode::Net_Uninit_();
        return 0;
    }

    char* __cdecl Load_Netstart_Strings_(char* buffer) {
        char* current = buffer;

        _windows_string = current;
        current += strlen(current) + 1;

        NETSTRAT::_comm_failure_string = current;
        current += strlen(current) + 1;

        _game_full_string = current;
        current += strlen(current) + 1;

        _receiving_chunk_d_of_d = current;
        current += strlen(current) + 1;

        _answering_failed_error = current;
        current += strlen(current) + 1;

        _no_more_player_slots_error = current;
        current += strlen(current) + 1;

        _dialing_failed_error = current;
        current += strlen(current) + 1;

        _really_begin_game_with_unused_slots_msg = current;
        current += strlen(current) + 1;

        _really_start_with_only_1_msg = current;
        current += strlen(current) + 1;

        return current;
    }

    uint8_t __cdecl Join_Net_Screen_() {
        uint8_t result = 0;
        uint8_t fade_status;
        const char* empty_str = s__00551100;
        const char* myname_str = s_myname_005597dc;

        NETMOX::_net.packet_type = 5;

        NETTURN::Net_Init_Host_Election_();
        MULTPLAY::Reload_Initializing_Net_Info_();
        fields::Assign_Auto_Function_(MULTPLAY::Draw_Generic_Net_Info_Screen_, 1);
        MISC::Fade_Into_Screen_(MULTPLAY::Draw_Generic_Net_Info_Screen_, &fade_status, 11);
        MULTPLAY::Draw_Generic_Net_Info_Screen_();
        RUSS::Mox_Sync_Update_();
        RUSS::Mox_Update_();

        // TYPE MISMATCH: Receive_Message_ signature differs from WINNET::Init expectation
        if (!WINNET::Init(NETMOX::_net.mode, NETSTART::GUID_0054F150, reinterpret_cast<void (*)(void*)>(NETMOX::Receive_Message_), &NETMOX::_net)) {
            RUSS::Mox_Update_();
            GENDRAW::Message_Box_(NETSTRAT::_comm_failure_string);
            MOX::_current_screen = 15 /* SCREEN_MAIN_MENU */;
            NETMOX::_net.packet_type = 0;
            return 0;
        }

        if (NETMOX::_net.mode == 0) {
            MULTPLAY::Choose_Multi_Network_Game_Screen_();
            if (NETMOX::_net.cur_game_name[0] == '\0') {
                MOX::_current_screen = 15 /* SCREEN_MAIN_MENU */;
                NETMOX::_net.packet_type = 0;
                return 0;
            }

            if (!netcode::Net_Join_Game_(NETMOX::_net.cur_game_name, empty_str, myname_str)) {
                GENDRAW::Message_Box_(NETSTART::_game_full_string);
                NETMOX::_net.packet_type = 0;
                MOX::_current_screen = 15 /* SCREEN_MAIN_MENU */;
                NETSTART::Mox_Net_Uninit_();
                return 0;
            }
        } else {
            netcode::Set_Null_Modem_Slave_();
            if (NETMOX::_net.mode == 1) {
                if (!NETSTART::Do_Answer_()) {
                    MOX::_current_screen = 15 /* SCREEN_MAIN_MENU */;
                    NETMOX::_net.packet_type = 0;
                    return 0;
                }
            }
        }

        NETMOX::_net.is_network_active = 1;
        NETMOX::_net.net_host_id = netcode::Net_Host_ID_();
        MULTPLAY::Reload_Join_Net_Screen_();
        fields::Clear_Fields_();
        fields::Add_Hidden_Field_(0, 0, 639, 479, empty_str, 0);
        fields::Assign_Auto_Function_(MULTPLAY::thunk_Draw_Generic_Net_Info_Screen_, 2);

        memset(RACESEL::_color_flag, 0, 8);
        memset(RACESEL::_race_flag, 0, 14);

        while (NETMOX::_net.packet_type == 5) {
            fields::Get_Input_();
            MULTPLAY::thunk_Draw_Generic_Net_Info_Screen_();
            RUSS::Mox_Sync_Update_();
        }

        if (NETMOX::_net.packet_type == 45) {
            NETMOX::_net.packet_type = 0;
            MOX::_current_screen = 15 /* SCREEN_MAIN_MENU */;
            NETSTART::Mox_Net_Uninit_();
            GENDRAW::Message_Box_(NETSTART::_no_more_player_slots_error);
            return 0;
        }

        if (NETMOX::_net.packet_type == 6) {
            NETMOX::Mox_Send_Message_(NETMOX::_net.host_election_id, 66, &MOX::_nebula_screen_seg, 4);
            result = (uint8_t)NETSTART::Net_Select_Race_();
        } else if (NETMOX::_net.packet_type == 44) {
            NETMOX::Mox_Send_Message_(NETMOX::_net.host_election_id, 66, &MOX::_nebula_screen_seg, 4);
            result = (uint8_t)NETSTART::Net_Pick_Position_();
            if (!result) {
                NETMOX::_net.packet_type = 0;
                MOX::_current_screen = 15 /* SCREEN_MAIN_MENU */;
                NETSTART::Mox_Net_Uninit_();
            }
        }

        if (NETMOX::_net.mode == 3) {
            WINNET::Ten_Stop(1);
            WINNET::Ten_Flag();
        }

        return result;
    }

    void __cdecl Mox_Net_Uninit_() {
        netcode::Net_Uninit_();

        for (int i = 0; i < 8; ++i) {
            MOX::_player[i].network_player_id = -1;
        }

        NETMOX::_net.is_network_active = 0;
    }

    int8_t __cdecl Request_Race_(uint8_t race_idx) {
        while (NETMOX::_net.race_selection_changed != 0) {
            RUSS::Mox_Update_();
        }

        MOX::_player[MOX::_PLAYER_NUM].race = 14;

        if (RACESEL::_race_flag[race_idx] != 0) {
            return 0;
        }

        NETMOX::Mox_Send_Message_(NETMOX::_net.host_election_id, 58, &race_idx, 1);

        while (RACESEL::_race_flag[race_idx] == 0) {
            RUSS::Mox_Update_();
        }

        return MOX::_player[MOX::_PLAYER_NUM].race == race_idx;
    }

    uint8_t __cdecl Load_Net_Screen_() {
        uint8_t fade_flag = 0;
        int8_t status[8] = { 0 };
        uint8_t payload[96] = { 0 };
        uint8_t user_list[256];

        NETMOX::_net.packet_type = 36;
        NETTURN::Net_Init_Host_Election_();
        MULTPLAY::Reload_Initializing_Net_Info_();

        auto draw_generic_fn = reinterpret_cast<void(__cdecl*)()>(std::uintptr_t{0x00502d70});
        fields::Assign_Auto_Function_(draw_generic_fn, 1);
        MISC::Fade_Into_Screen_(draw_generic_fn, &fade_flag, 11);
        MULTPLAY::Draw_Generic_Net_Info_Screen_();
        RUSS::Mox_Sync_Update_();

        void (*receive_msg_fn)(void*) = reinterpret_cast<void (*)(void*)>(NETMOX::Receive_Message_);
        if (WINNET::Init(NETMOX::_net.mode, GUID_0054F150, receive_msg_fn, &NETMOX::_net) == 0) {
            GENDRAW::Message_Box_(NETSTRAT::_comm_failure_string);
            MOX::_current_screen = 15;
            NETMOX::_net.packet_type = 0;
            return 0;
        }

        if (NETMOX::_net.mode == 0) {
            if (Russ_Net_Create_Game_(GUID_0054F150, NETMOX::_net.cur_game_name, 8, s__00551100, s_myname_005597dc) == 0) {
                GENDRAW::Message_Box_(NETSTRAT::_comm_failure_string);
                NETMOX::_net.packet_type = 0;
                MOX::_current_screen = 15;
                Mox_Net_Uninit_();
                return 0;
            }
        } else {
            if (NETMOX::_net.mode == 2 || NETMOX::_net.mode == 1) {
                netcode::Set_Null_Modem_Host_();
            }
            if (NETMOX::_net.mode == 1) {
                if (Do_Dial_(NETMOX::_net.modem_settings.cmd_reset) == 0) {
                    NETMOX::_net.packet_type = 0;
                    MOX::_current_screen = 15;
                    return 0;
                }
            }
        }

        fields::Clear_Fields_();
        fields::Add_Hidden_Field_(0, 0, 639, 479, s__00551100, 0);

        auto score_nullsub_fn = reinterpret_cast<void(__cdecl*)()>(std::uintptr_t{0x004d7840});
        fields::Assign_Auto_Function_(score_nullsub_fn, 2);

        NETMOX::_net.is_network_active = 1;
        NETMOX::_net.net_host_id = netcode::Net_Host_ID_();
        NETMOX::_net.host_election_id = NETMOX::_net.net_host_id;

        if (MOX::_nebula_screen_seg != 0) {
            NETMOX::_net.network_sync_complete_flag = 1;
        } else {
            NETMOX::_net.network_sync_complete_flag = 0;
        }

        int16_t player_0 = MULTPLAY::Choose_Network_Plyrs_Screen_(reinterpret_cast<int16_t*>(payload), status, 0, 1);
        
        fields::Clear_Fields_();
        fields::Add_Hidden_Field_(0, 0, 639, 479, s__00551100, 0);

        if (player_0 == -1) {
            NETMOX::_net.packet_type = 0;
            MOX::_current_screen = 15;
            return 0;
        }

        status[player_0] = 1;
        MOX::_player[player_0].network_player_id = NETMOX::_net.net_host_id;
        NETMOX::_net.joining_net_id = -1;
        MOX::_PLAYER_NUM = player_0;

        payload[0] = 0;

        auto draw_wait_fn = reinterpret_cast<void(__cdecl*)()>(std::uintptr_t{0x00503560});
        fields::Assign_Auto_Function_(draw_wait_fn, 2);

        MULTPLAY::Reload_Waiting_For_Joiners_Screen_();
        fields::Clear_Fields_();
        MULTPLAY::Add_Waiting_For_Joiners_Field_();

        NETMOX::_net.num_connected_players = 0;
        if (MOX::_NUM_PLAYERS > 0) {
            for (int32_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
                if (MOX::_player[i].objectives == 100) {
                    NETMOX::_net.num_connected_players++;
                }
            }
        }

        int32_t connected_count = 1;

        while (true) {
            if (NETMOX::_net.joining_net_id == -1) {
                int32_t num_users = netcode::Net_Get_Users_(user_list);
                if (num_users > 0) {
                    for (int32_t i = num_users - 1; i >= 0; i--) {
                        if (NETMOX::_net.joining_net_id != -1) {
                            break;
                        }
                        if (NETMOX::Net_Id_To_Player_Id_(user_list[i]) == -1) {
                            connected_count++;
                            NETMOX::_net.joining_net_id = user_list[i];
                            NETMOX::Send_Pick_Your_Position_(user_list[i]);
                        }
                    }
                }
            }

            MULTPLAY::Draw_Wait_For_New_Joiners_Screen_();
            RUSS::Mox_Sync_Update_();

            if (NETMOX::_net.joining_net_id == -1) {
                if (NETMOX::_net.num_connected_players == connected_count ||
                    (connected_count == 2 && (NETMOX::_net.mode == 1 || NETMOX::_net.mode == 2))) {
                    payload[0] = 1;
                } else {
                    if (netcode::Net_Get_Users_(nullptr) == connected_count) {
                        if (fields::Get_Input_() == MOX::_mp_screen->field_id_begin) {
                            if (NETMOX::_net.num_connected_players > connected_count) {
                                payload[0] = GENDRAW::Warning_Box2_(_really_begin_game_with_unused_slots_msg);
                            } else {
                                payload[0] = 1;
                            }
                        }
                    }
                }
            }

            RUSS::Mox_Update_();
            MULTPLAY::Draw_Wait_For_New_Joiners_Screen_();
            RUSS::Mox_Sync_Update_();

            if (payload[0] != 0) {
                break;
            }
        }

        if (NETMOX::_net.mode == 0 || NETMOX::_net.mode == 3) {
            netcode::Net_Disable_New_Players_();
        }

        fields::Assign_Auto_Function_(score_nullsub_fn, 2);

        if (NETMOX::_net.mode == 3) {
            WINNET::Ten_Stop(1);
            WINNET::Ten_Flag();
        }

        NETMOX::_net.num_connected_players = 0;
        if (MOX::_NUM_PLAYERS > 0) {
            for (int32_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
                if (MOX::_player[i].network_player_id == -1) {
                    if (MOX::_player[i].objectives == 100) {
                        RUSS::Make_Player_Into_AI_(i);
                    }
                } else {
                    MOX::_player[i].objectives = 100;
                    NETMOX::_net.num_connected_players++;
                }
            }
        }

        if (MOX::_NUM_PLAYERS > 0) {
            for (int32_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
                COLCALC::Do_Player_Colony_Calculations_(i);
            }
        }

        if (NETMOX::_net.network_sync_complete_flag == 0) {
            MOX::_NUM_NEBULAS = 0;
            if (MOX::_NUM_STARS > 0) {
                for (int32_t i = 0; i < MOX::_NUM_STARS; i++) {
                    MOX::_star[i].in_nebula = 0;
                }
            }
        }

        MOX::_temp_string[0] = '\0';
        MULTPLAY::Reload_Sending_Data_Info_();
        video::Set_Page_Off_();
        MULTPLAY::Draw_SendGet_Net_Info_Screen_();

        auto draw_sendget_fn = reinterpret_cast<void(__cdecl*)()>(std::uintptr_t{0x00503390});
        fields::Assign_Auto_Function_(draw_sendget_fn, 2);
        RUSS::Mox_Sync_Update_();

        if (MOX::_NUM_PLAYERS > 0) {
            for (int32_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
                NETMOX::_net.player_net_info[i].packet_sync_status = 4;
                NETMOX::_net.player_net_info[i].remote_player_id = -1;
            }
        }

        payload[40] = static_cast<uint8_t>(MOX::_NUM_PLAYERS);
        if (MOX::_NUM_PLAYERS > 0) {
            for (int32_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
                *reinterpret_cast<int32_t*>(&payload[6 + i * 4]) = static_cast<int32_t>(MOX::_player[i].network_player_id);
            }
        }

        _sprintf(reinterpret_cast<char*>(&payload[46]), s__s_00551130, s_Mar_26_1997_005578e0);
        NETMOX::Mox_Send_Message_(0x7FFE, 42, payload, 96);

        NETMOX::_net.host_election_winner_id = NETMOX::_net.net_host_id;
        transprt::_num_net_players = NETMOX::_net.num_connected_players;
        NETMOX::Broadcast_Game_Data_Differential_(1);
        NETMOX::_net.player_race_selected_flags[MOX::_PLAYER_NUM] = 1;

        NETMOX::_net.field_1db = 0;
        NETMOX::_net.field_1df = 0;
        NETMOX::_net.packet_type = 4;

        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        MAPGEN::Init_Main_Screen_(MOX::_settings.galaxy_size);
        HAROLD::Harold_Init_();
        MAINSCR2::_main_screen_fade_in_flag = 1;
        MOX2::Init_Map_Defaults_();
        MAINSCR::Update_Ship_Icons_In_Between_Turns_();

        return 1;
    }
}