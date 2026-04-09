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
        uint8_t fade_buffer[16];
        const char* empty_str = s__00551100;
        const char* myname_str = s_myname_005597dc;

        NETMOX::_net.packet_type = 5;

        NETTURN::Net_Init_Host_Election_();
        MULTPLAY::Reload_Initializing_Net_Info_();
        fields::Assign_Auto_Function_(MULTPLAY::Draw_Generic_Net_Info_Screen_, 1);
        MISC::Fade_Into_Screen_(MULTPLAY::Draw_Generic_Net_Info_Screen_, &fade_buffer[3], 11);
        MULTPLAY::Draw_Generic_Net_Info_Screen_();
        RUSS::Mox_Sync_Update_();
        RUSS::Mox_Update_();

        if (!WINNET::Init(NETMOX::_net.mode, NETSTART::GUID_0054F150, NETMOX::Receive_Message_, &NETMOX::_net)) {
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
}