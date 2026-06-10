#include "pch.h"

namespace NETSTART {
    char* _dialing_failed_error;
    char* _receiving_chunk_d_of_d;
    char* _really_begin_game_with_unused_slots_msg;
    char* _really_start_with_only_1_msg;
    char* _no_more_player_slots_error;
    char* _game_full_string;
    char* _answering_failed_error;
    char* _windows_string;

    char s_myname_005597dc[7] = "myname";

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
        NETMOX::_net.sync_state = 7; /* PACKET_TYPE_RACE_SELECTION */

        NETMOX::Mox_Send_Message_(NETMOX::_net.host_election_id, 1, local_player, 3753 /* s_player size */);

        while (NETMOX::_net.total_chunk_count == 0) {
            fields::Get_Input_();
            MULTPLAY::Draw_Generic_Net_Info_Screen_();
            RUSS::Mox_Sync_Update_();
        }

        MULTPLAY::Reload_Getting_Data_Info_();

        strcpy(MOX::_temp_string, NETTURN::_waiting_message_fmt);
        video::Set_Page_Off_();
        MULTPLAY::Draw_SendGet_Net_Info_Screen_();
        fields::Assign_Auto_Function_(MULTPLAY::Draw_SendGet_Net_Info_Screen_, 2);
        RUSS::Mox_Sync_Update_();

        while (NETMOX::_net.received_chunk_count < NETMOX::_net.total_chunk_count) {
            sprintf(MOX::_temp_string, NETSTART::_receiving_chunk_d_of_d, 
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
        NETMOX::_net.sync_state = 4; /* PACKET_TYPE_MAP_SYNC */

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
        uint8_t connected_players[MAX_PLAYERS];
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
        fields::Add_Hidden_Field_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */, "", 0);

        NETMOX::_net.sync_state = 7;
        message_buffer = (int32_t)selection;
        NETMOX::Mox_Send_Message_(NETMOX::_net.host_election_id, 33 /* 0x21 */, &message_buffer, 4);

        fields::Assign_Auto_Function_(MULTPLAY::Draw_SendGet_Net_Info_Screen_, 2);
        MULTPLAY::Reload_Getting_Data_Info_();

        strcpy(MOX::_temp_string, NETTURN::_waiting_message_fmt);

        while (NETMOX::_net.total_chunk_count == 0) {
            fields::Get_Input_();
            video::Set_Page_Off_();
            MULTPLAY::Draw_SendGet_Net_Info_Screen_();
            RUSS::Mox_Sync_Update_();
        }

        while (NETMOX::_net.received_chunk_count < NETMOX::_net.total_chunk_count) {
            sprintf(MOX::_temp_string, NETSTART::_receiving_chunk_d_of_d, (int)NETMOX::_net.received_chunk_count + 1, (int)NETMOX::_net.total_chunk_count);
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

        NETMOX::_net.sync_state = 4;
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
        clock_t start_time = clock();
        simmodem::Modem_Reset_(NETMOX::_net.modem_settings.cmd_reset);

        int8_t is_connected = 0;
        if (simmodem::Modem_Answer_() != 0) {
            if (simmodem::Modem_Connected_() == 0) {
                while ((clock() - start_time) < 90000) {
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

    bool __cdecl Do_Dial_(char* phone_number) {
        clock_t start_time = clock();
        bool dial_success = simmodem::Modem_Dial_Number_(phone_number);

        if (dial_success) {
            char modem_error[80];
            simmodem::Get_Modem_Error_(modem_error);

            if (strlen(modem_error) > 1) {
                GENDRAW::Message_Box_(modem_error);
                netcode::Net_Uninit_();
                return false;
            }

            while (!simmodem::Modem_Connected_()) {
                if (clock() - start_time >= 90000) {
                    break;
                }
                RUSS::Mox_Update_();
            }

            dial_success = simmodem::Modem_Connected_() != 0;
            if (dial_success) {
                return true;
            }
        }

        GENDRAW::Message_Box_(_dialing_failed_error);
        netcode::Net_Uninit_();
        return false;
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
        const char* empty_str = "";
        const char* myname_str = s_myname_005597dc;

        NETMOX::_net.sync_state = 5;

        NETTURN::Net_Init_Host_Election_();
        MULTPLAY::Reload_Initializing_Net_Info_();
        fields::Assign_Auto_Function_(MULTPLAY::Draw_Generic_Net_Info_Screen_, 1);
        MISC::Fade_Into_Screen_(MULTPLAY::Draw_Generic_Net_Info_Screen_, &fade_status, 11);
        MULTPLAY::Draw_Generic_Net_Info_Screen_();
        RUSS::Mox_Sync_Update_();
        RUSS::Mox_Update_();

        if (!netcode::Do_Net_Init_(NETMOX::_net.mode, NETMOX::Receive_Message_, &NETMOX::_net)) {
            RUSS::Mox_Update_();
            GENDRAW::Message_Box_(NETSTRAT::_comm_failure_string);
            MOX::_current_screen = 15 /* SCREEN_MAIN_MENU */;
            NETMOX::_net.sync_state = 0;
            return 0;
        }

        if (NETMOX::_net.mode == 0) {
            MULTPLAY::Choose_Multi_Network_Game_Screen_();
            if (NETMOX::_net.cur_game_name[0] == '\0') {
                MOX::_current_screen = 15 /* SCREEN_MAIN_MENU */;
                NETMOX::_net.sync_state = 0;
                return 0;
            }

            if (!netcode::Net_Join_Game_(NETMOX::_net.cur_game_name, empty_str, myname_str)) {
                GENDRAW::Message_Box_(NETSTART::_game_full_string);
                NETMOX::_net.sync_state = 0;
                MOX::_current_screen = 15 /* SCREEN_MAIN_MENU */;
                NETSTART::Mox_Net_Uninit_();
                return 0;
            }
        } else {
            netcode::Set_Null_Modem_Slave_();
            if (NETMOX::_net.mode == 1) {
                if (!NETSTART::Do_Answer_()) {
                    MOX::_current_screen = 15 /* SCREEN_MAIN_MENU */;
                    NETMOX::_net.sync_state = 0;
                    return 0;
                }
            }
        }

        NETMOX::_net.is_network_active = 1;
        NETMOX::_net.net_host_id = netcode::Net_Host_ID_();
        MULTPLAY::Reload_Join_Net_Screen_();
        fields::Clear_Fields_();
        fields::Add_Hidden_Field_(0, 0, 639, 479, empty_str, 0);
        fields::Assign_Auto_Function_(MULTPLAY::Draw_Generic_Net_Info_Screen_, 2);

        memset(RACESEL::_color_flag, 0, 8);
        memset(RACESEL::_race_flag, 0, 14);

        while (NETMOX::_net.sync_state == 5) {
            fields::Get_Input_();
            MULTPLAY::Draw_Generic_Net_Info_Screen_();
            RUSS::Mox_Sync_Update_();
        }

        if (NETMOX::_net.sync_state == 45) {
            NETMOX::_net.sync_state = 0;
            MOX::_current_screen = 15 /* SCREEN_MAIN_MENU */;
            NETSTART::Mox_Net_Uninit_();
            GENDRAW::Message_Box_(NETSTART::_no_more_player_slots_error);
            return 0;
        }

        if (NETMOX::_net.sync_state == 6) {
            NETMOX::Mox_Send_Message_(NETMOX::_net.host_election_id, 66, &MOX::_nebula_screen_seg, 4);
            result = (uint8_t)NETSTART::Net_Select_Race_();
        } else if (NETMOX::_net.sync_state == 44) {
            NETMOX::Mox_Send_Message_(NETMOX::_net.host_election_id, 66, &MOX::_nebula_screen_seg, 4);
            result = (uint8_t)NETSTART::Net_Pick_Position_();
            if (!result) {
                NETMOX::_net.sync_state = 0;
                MOX::_current_screen = 15 /* SCREEN_MAIN_MENU */;
                NETSTART::Mox_Net_Uninit_();
            }
        }

        if (NETMOX::_net.mode == 3) {
            ten::Ten_Stop(1);
            ten::Ten_Flag();
        }

        return result;
    }

    uint8_t __cdecl Start_Net_Screen_() {
        NETMOX::_net.sync_state = 1;
        NETTURN::Net_Init_Host_Election_();
        MULTPLAY::Reload_Initializing_Net_Info_();
        fields::Assign_Auto_Function_(MULTPLAY::Draw_Generic_Net_Info_Screen_, 1);

        uint8_t unused;
        MISC::Fade_Into_Screen_(MULTPLAY::Draw_Generic_Net_Info_Screen_, &unused, 11);
        MULTPLAY::Draw_Generic_Net_Info_Screen_();
        RUSS::Mox_Sync_Update_();

        if (netcode::Do_Net_Init_(NETMOX::_net.mode, NETMOX::Receive_Message_, &NETMOX::_net) == 0) {
            NETMOX::_net.sync_state = 0;
            GENDRAW::Message_Box_(NETSTRAT::_comm_failure_string);
            MOX::_current_screen = 15;
            return 0;
        }

        if (NETMOX::_net.mode == 0) {
            if (Russ_Net_Create_Game_(NETMOX::_net.cur_game_name, MAX_PLAYERS, "", s_myname_005597dc) == 0) {
                GENDRAW::Message_Box_(NETSTRAT::_comm_failure_string);
                NETMOX::_net.sync_state = 0;
                MOX::_current_screen = 15;
                Mox_Net_Uninit_();
                return 0;
            }
        }

        if (NETMOX::_net.mode == 2 || NETMOX::_net.mode == 1) {
            netcode::Set_Null_Modem_Host_();
            if (NETMOX::_net.mode == 1) {
                if (Do_Dial_(NETMOX::_net.modem_settings.cmd_reset) == 0) {
                    NETMOX::_net.sync_state = 0;
                    MOX::_current_screen = 15;
                    return 0;
                }
            }
        }

        NETMOX::_net.is_network_active = 1;
        NETMOX::_net.net_host_id = netcode::Net_Host_ID_();
        NETMOX::_net.host_election_id = NETMOX::_net.net_host_id;

        if (MOX::_nebula_screen_seg == 0) {
            NETMOX::_net.network_sync_complete_flag = 0;
        } else {
            NETMOX::_net.network_sync_complete_flag = 1;
        }

        if (NETMOX::_net.mode == 0 || NETMOX::_net.mode == 3) {
            MULTPLAY::Reload_Waiting_For_Joiners_Screen_();
            fields::Clear_Fields_();
            MULTPLAY::Add_Waiting_For_Joiners_Field_();
            fields::Assign_Auto_Function_(MULTPLAY::Draw_Wait_For_New_Joiners_Screen_, 2);
            MULTPLAY::Draw_Wait_For_New_Joiners_Screen_();
            RUSS::Mox_Sync_Update_();

            while (true) {
                int32_t input = fields::Get_Input_();
                MULTPLAY::Draw_Wait_For_New_Joiners_Screen_();
                RUSS::Mox_Sync_Update_();

                if (input != 0) {
                    if (netcode::Net_Get_Users_(nullptr) == 1) {
                        input = GENDRAW::Confirmation_Box_(_really_start_with_only_1_msg);
                    }
                    if (input != 0) {
                        break;
                    }
                }

                if (netcode::Net_Get_Users_(nullptr) >= MOX::_settings.number_of_players) {
                    break;
                }
            }

            netcode::Net_Disable_New_Players_();
        }

        uint8_t users[256];
        NETMOX::_net.num_connected_players = static_cast<int8_t>(netcode::Net_Get_Users_(users));

        if (NETMOX::_net.num_connected_players > MOX::_settings.number_of_players) {
            NETMOX::_net.num_connected_players = MOX::_settings.number_of_players;
        }

        Dump_Users_();

        s_net_pack_start_game msg;
        memset(msg.net_ids, 0, sizeof(msg.net_ids));

        NETMOX::_net.sync_state = 2;
        MOX::_PLAYER_NUM = -1;

        if (NETMOX::_net.num_connected_players != 0) {
            for (int i = 0; i < NETMOX::_net.num_connected_players; i++) {
                MOX::_player[i].network_player_id = users[i];
                msg.net_ids[i] = users[i];

                if (NETMOX::_net.net_host_id == users[i]) {
                    MOX::_PLAYER_NUM = static_cast<int16_t>(i);
                }
            }
        }

        if (MOX::_PLAYER_NUM == -1) {
            MOX::_player[0].network_player_id = NETMOX::_net.net_host_id;
            msg.net_ids[0] = NETMOX::_net.net_host_id;
            MOX::_PLAYER_NUM = 0;
        }

        Dump_Users_();

        MOX::_NUM_PLAYERS = MOX::_settings.number_of_players;

        msg.net_host_id = NETMOX::_net.net_host_id;
        msg.num_connected_players = NETMOX::_net.num_connected_players;
        msg.random_events = MOX::_settings.random_events;
        msg.game_difficulty = MOX::_settings.game_difficulty;
        msg.number_of_players = MOX::_settings.number_of_players;
        msg.galaxy_size = MOX::_settings.galaxy_size;
        msg.galaxy_age = MOX::_settings.galaxy_age;
        msg.strategic_combat_flag = MOX::_settings.strategic_combat_flag;
        msg.starting_civilization_level = MOX::_settings.starting_civilization_level;
        msg.xenons_exist = MOX::_settings.xenons_exist;

        strcpy(msg.version_string, GAME_BUILD_DATE);

        memset(RACESEL::_color_flag, 0, sizeof(RACESEL::_color_flag));
        memset(NETMOX::_net.player_connected_flags, 0, sizeof(NETMOX::_net.player_connected_flags));
        memset(RACESEL::_race_flag, 0, sizeof(RACESEL::_race_flag));
        memset(NETMOX::_net.player_color_selected_flags, 0, sizeof(NETMOX::_net.player_color_selected_flags));

        NETMOX::_net.sync_state = 3;

        if (MOX::_NUM_PLAYERS > 0) {
            for (int i = 0; i < MOX::_NUM_PLAYERS; i++) {
                MOX::_player[i].color = 8;
            }
        }

        RACESEL::Race_Selection_Screen_(&MOX::_player[MOX::_PLAYER_NUM]);

        NETMOX::_net.player_color_selected_flags[MOX::_PLAYER_NUM] = 1;
        MOX::_player[MOX::_PLAYER_NUM].objectives = 100;

        if (NETMOX::_net.mode == 3) {
            ten::Ten_Stop(1);
            ten::Ten_Flag();
        }

        RUSS::Mox_Update_();

        NETMOX::Mox_Send_Message_(0x7FFE, 0, &msg, sizeof(msg));

        MULTPLAY::Reload_Wait_For_Race_Info_();
        MISC::Fade_Into_Screen_(MULTPLAY::Draw_Generic_Net_Info_Screen_, &unused, 11);

        NETMOX::_net.player_connected_flags[MOX::_PLAYER_NUM] = 1;

        if (NETMOX::_net.num_connected_players != 0) {
            while (true) {
                MULTPLAY::Draw_Wait_For_Race_Info_Screen_();
                
                int32_t ready_count = 0;
                if (MOX::_NUM_PLAYERS > 0) {
                    for (int i = 0; i < MOX::_NUM_PLAYERS; i++) {
                        if (NETMOX::_net.player_connected_flags[i] != 0) {
                            ready_count++;
                        }
                    }
                }

                fields::Get_Input_();
                RUSS::Mox_Sync_Update_();

                if (NETMOX::_net.num_connected_players <= ready_count) {
                    break;
                }
            }
        }

        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();

        INITGAME::Init_New_Game_(
            (int16_t)MOX::_settings.game_difficulty,
            (int16_t)MOX::_settings.galaxy_size,
            (int16_t)MOX::_settings.galaxy_age,
            (int16_t)MOX::_settings.number_of_players,
            (int16_t)MOX::_settings.strategic_combat_flag,
            (int16_t)MOX::_settings.starting_civilization_level
        );

        if (MOX::_NUM_PLAYERS > NETMOX::_net.num_connected_players) {
            for (int i = NETMOX::_net.num_connected_players; i < MOX::_NUM_PLAYERS; i++) {
                MOX::_player[i].network_player_id = -1;
            }
        }

        uint8_t color_used[8] = {0};

        if (MOX::_NUM_PLAYERS > 0) {
            for (int i = 0; i < MOX::_NUM_PLAYERS; i++) {
                if (MOX::_player[i].color != 8) {
                    color_used[MOX::_player[i].color] = 1;
                }
            }
        }

        if (MOX::_NUM_PLAYERS > 0) {
            for (int i = 0; i < MOX::_NUM_PLAYERS; i++) {
                if (MOX::_player[i].color == 8) {
                    int8_t selected_color = -1;
                    int32_t num_available = 0;
                    
                    for (int j = 0; j < 8; j++) {
                        if (color_used[j] == 0) {
                            num_available++;
                            if (random::Random_(num_available) == 1) {
                                selected_color = static_cast<int8_t>(j);
                            }
                        }
                    }
                    
                    MOX::_player[i].color = selected_color;
                    color_used[selected_color] = 1;
                }
            }
        }

        MULTPLAY::Reload_Sending_Data_Info_();
        video::Set_Page_Off_();
        MULTPLAY::Draw_SendGet_Net_Info_Screen_();
        fields::Assign_Auto_Function_(MULTPLAY::Draw_SendGet_Net_Info_Screen_, 2);
        RUSS::Mox_Sync_Update_();

        if (NETMOX::_net.network_sync_complete_flag == 0) {
            MOX::_NUM_NEBULAS = 0;
        }

        if (MOX::_NUM_PLAYERS > 0) {
            for (int i = 0; i < MOX::_NUM_PLAYERS; i++) {
                NETMOX::_net.player_net_info[i].packet_type = 4;
                NETMOX::_net.player_net_info[i].diplomacy_target_id = -1;
            }
        }

        NETMOX::_net.host_election_winner_id = NETMOX::_net.net_host_id;
        transprt::_num_net_players = NETMOX::_net.num_connected_players;

        NETMOX::Broadcast_Game_Data_Differential_(1);

        NETMOX::_net.player_race_selected_flags[MOX::_PLAYER_NUM] = 1;
        NETMOX::_net.field_1db = 0;
        NETMOX::_net.field_1df = 0;
        NETMOX::_net.sync_state = 4;

        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();

        MAPGEN::Init_Main_Screen_(MOX::_settings.galaxy_size);
        HAROLD::Harold_Init_();
        MOX2::Init_Map_Defaults_();

        MAINSCR2::_main_screen_fade_in_flag = 1;
        MAINSCR::Update_Ship_Icons_In_Between_Turns_();

        return 1;
    }

    void __cdecl Mox_Net_Uninit_() {
        netcode::Net_Uninit_();

        for (int i = 0; i < MAX_PLAYERS; ++i) {
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
        uint8_t status[MAX_PLAYERS] = { 0 };
        s_load_net_screen_packet payload = {};
        uint8_t user_list[256];

        NETMOX::_net.sync_state = 36;
        NETTURN::Net_Init_Host_Election_();
        MULTPLAY::Reload_Initializing_Net_Info_();

        fields::Assign_Auto_Function_(MULTPLAY::Draw_Generic_Net_Info_Screen_, 1);
        MISC::Fade_Into_Screen_(MULTPLAY::Draw_Generic_Net_Info_Screen_, &fade_flag, 11);
        MULTPLAY::Draw_Generic_Net_Info_Screen_();
        RUSS::Mox_Sync_Update_();

        if (netcode::Do_Net_Init_(NETMOX::_net.mode, NETMOX::Receive_Message_, &NETMOX::_net) == 0) {
            GENDRAW::Message_Box_(NETSTRAT::_comm_failure_string);
            MOX::_current_screen = 15;
            NETMOX::_net.sync_state = 0;
            return 0;
        }

        if (NETMOX::_net.mode == 0) {
            if (Russ_Net_Create_Game_(NETMOX::_net.cur_game_name, MAX_PLAYERS, "", s_myname_005597dc) == 0) {
                GENDRAW::Message_Box_(NETSTRAT::_comm_failure_string);
                NETMOX::_net.sync_state = 0;
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
                    NETMOX::_net.sync_state = 0;
                    MOX::_current_screen = 15;
                    return 0;
                }
            }
        }

        fields::Clear_Fields_();
        fields::Add_Hidden_Field_(0, 0, 639, 479, "", 0);

        fields::Assign_Auto_Function_(SCORE::nullsub, 2);

        NETMOX::_net.is_network_active = 1;
        NETMOX::_net.net_host_id = netcode::Net_Host_ID_();
        NETMOX::_net.host_election_id = NETMOX::_net.net_host_id;

        if (MOX::_nebula_screen_seg != 0) {
            NETMOX::_net.network_sync_complete_flag = 1;
        } else {
            NETMOX::_net.network_sync_complete_flag = 0;
        }

        int16_t choose_result = MULTPLAY::Choose_Network_Plyrs_Screen_(&payload.begin_state, status, 0, 1);
        
        fields::Clear_Fields_();
        fields::Add_Hidden_Field_(0, 0, 639, 479, "", 0);

        if (choose_result == -1) {
            NETMOX::_net.sync_state = 0;
            MOX::_current_screen = 15;
            return 0;
        }

        int16_t player_0 = payload.begin_state;
        status[player_0] = 1;
        MOX::_player[player_0].network_player_id = NETMOX::_net.net_host_id;
        NETMOX::_net.joining_net_id = -1;
        MOX::_PLAYER_NUM = player_0;

        payload.begin_state = 0;

        fields::Assign_Auto_Function_(MULTPLAY::Draw_Wait_For_New_Joiners_Screen_, 2);

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
                    payload.begin_state = 1;
                } else {
                    if (netcode::Net_Get_Users_(nullptr) == connected_count) {
                        if (fields::Get_Input_() == MOX::_mp_screen->field_id_begin) {
                            if (NETMOX::_net.num_connected_players > connected_count) {
                                payload.begin_state = (int16_t)(uint8_t)GENDRAW::Confirmation_Box_(_really_begin_game_with_unused_slots_msg);
                            } else {
                                payload.begin_state = 1;
                            }
                        }
                    }
                }
            }

            RUSS::Mox_Update_();
            MULTPLAY::Draw_Wait_For_New_Joiners_Screen_();
            RUSS::Mox_Sync_Update_();

            if (payload.begin_state != 0) {
                break;
            }
        }

        if (NETMOX::_net.mode == 0 || NETMOX::_net.mode == 3) {
            netcode::Net_Disable_New_Players_();
        }

        fields::Assign_Auto_Function_(SCORE::nullsub, 2);

        if (NETMOX::_net.mode == 3) {
            ten::Ten_Stop(1);
            ten::Ten_Flag();
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

        fields::Assign_Auto_Function_(MULTPLAY::Draw_SendGet_Net_Info_Screen_, 2);
        RUSS::Mox_Sync_Update_();

        if (MOX::_NUM_PLAYERS > 0) {
            for (int32_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
                NETMOX::_net.player_net_info[i].packet_type = 4;
                NETMOX::_net.player_net_info[i].diplomacy_target_id = -1;
            }
        }

        payload.number_of_players = (int8_t)MOX::_NUM_PLAYERS;
        if (MOX::_NUM_PLAYERS > 0) {
            for (int32_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
                payload.network_ids[i] = MOX::_player[i].network_player_id;
            }
        }

        strcpy(payload.version_string, GAME_BUILD_DATE);
        NETMOX::Mox_Send_Message_(0x7FFE, 42, &payload, sizeof(payload));

        NETMOX::_net.host_election_winner_id = NETMOX::_net.net_host_id;
        transprt::_num_net_players = NETMOX::_net.num_connected_players;
        NETMOX::Broadcast_Game_Data_Differential_(1);
        NETMOX::_net.player_race_selected_flags[MOX::_PLAYER_NUM] = 1;

        NETMOX::_net.field_1db = 0;
        NETMOX::_net.field_1df = 0;
        NETMOX::_net.sync_state = 4;

        fields::Clear_Fields_();
        fields::Deactivate_Auto_Function_();
        MAPGEN::Init_Main_Screen_(MOX::_settings.galaxy_size);
        HAROLD::Harold_Init_();
        MAINSCR2::_main_screen_fade_in_flag = 1;
        MOX2::Init_Map_Defaults_();
        MAINSCR::Update_Ship_Icons_In_Between_Turns_();

        return 1;
    }

    int32_t __cdecl Russ_Net_Create_Game_(const char* game_name, int32_t max_players, const char* str1, const char* str2) {
        RUSS::Mox_Update_();
        int32_t result = netcode::Net_Create_Game_(game_name, max_players, str1, str2);
        RUSS::Mox_Update_();
        return result;
    }

    int __cdecl Dump_Users_() {
        uint8_t buffer[20];
        return netcode::Net_Get_Users_(buffer);
    }

    int8_t __cdecl Request_Banner_Color_(uint8_t requested_color) {
        if (RACESEL::_color_flag[requested_color] != 0) {
            return 0;
        }

        NETMOX::Mox_Send_Message_(NETMOX::_net.host_election_id, 0x2c, &requested_color, 1);

        while (RACESEL::_color_flag[requested_color] == 0) {
            int32_t unused_zero = 0;
            (void)unused_zero;
            RUSS::Mox_Update_();
        }

        return static_cast<int8_t>(
            static_cast<uint8_t>(MOX::_player[MOX::_PLAYER_NUM].color) == requested_color
        );
    }
}