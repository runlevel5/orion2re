#pragma once

namespace NETMOX {
    inline s_network& _net = *reinterpret_cast<s_network*>(std::uintptr_t{0x00595170});

    inline uint8_t& _sending_msg =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055AD40});

    inline char*& _lost_net_user_error =
        *reinterpret_cast<char**>(std::uintptr_t{0x00572358});

    inline char*& _only_1_host_allowed_error =
        *reinterpret_cast<char**>(std::uintptr_t{0x00572374});

    inline char*& _modem_diff_buffer_error =
        *reinterpret_cast<char**>(std::uintptr_t{0x00572378});

    inline char*& _modem_disk_error =
        *reinterpret_cast<char**>(std::uintptr_t{0x0057237C});

    inline s_net_modem_file& _secret_modem_file =
        *reinterpret_cast<s_net_modem_file*>(std::uintptr_t{0x00572360});

    inline const char* s_modem_tmp_0055ad4c =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055AD4C}); // "modem.tmp"

    inline char*& _sending_chunk_d_of_d_fmt =
        *reinterpret_cast<char**>(std::uintptr_t{0x00572380});

    inline char*& _incompatible_versions_error =
        *reinterpret_cast<char**>(std::uintptr_t{0x00572384});

    inline char*& _ambassador_left_before_you_arrived_fmt =
        *reinterpret_cast<char**>(std::uintptr_t{0x00572388});

    inline char*& _net_send_error =
        *reinterpret_cast<char**>(std::uintptr_t{0x0057238C});

    inline uint8_t& _in_receive_message =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0057235C});

    inline uint8_t& _is_updating = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055AD44});

    inline uint8_t& _next_update_player_counter = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055AD48});

    inline uint8_t& _net_players_active_mask =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00595300});

    int32_t __cdecl Human_Requesting_Diplomacy_();

    void __cdecl Accept_Diplomacy_Request_(int player_idx);

    void __cdecl Refuse_Diplomacy_Request_(int player_idx);

    void __cdecl Tell_Diplomacy_Requester_To_Wait_(int32_t player_idx);

    uint8_t Humans_Requesting_Diplomacy_();

    void __cdecl Russ_Change_Relations_(int32_t change, int16_t attacker, int16_t defender, int32_t reason, int16_t star_idx, int32_t param6);

    void __cdecl Send_Drop_Out_Info_(int player_idx, uint16_t msg_type);

    void __cdecl Russ_Event_Monster_Update_(int16_t owner, int16_t p2, int16_t p3);

    int32_t __cdecl Net_Id_To_Player_Id_(int32_t player_idx);

    void __cdecl Send_Pick_Your_Position_(int16_t network_player_id);

    char* __cdecl Load_Netmox_Strings_(char* strings);

    void __cdecl Update_();

    void __cdecl Mox_Net_Update_();

    void __cdecl Send_You_Are_Target_Info_(
        int16_t target_player_idx,
        int16_t other_player_idx,
        int16_t param_3,
        int16_t colony_idx,
        int16_t star_idx);

    void __cdecl Send_Pick_Target_Info_(int16_t player_idx, int16_t star_idx, s_combfind_target* target_info);

    void __cdecl Ditch_Leader_(int32_t leader_idx);

    int32_t __cdecl Mox_Send_Message_(int16_t param_1, uint16_t param_2, void* packet, uint16_t len);

    void __cdecl Decode_Game_Orders_(void* data, uint32_t len);

    void __cdecl Broadcast_Game_Data_Differential_(uint8_t unknown);

    int8_t __cdecl Decode_Broadcast_Game_Data_Differential_(void* buffer, size_t size, uint8_t flag);

    inline char* s_wb_00551178 = reinterpret_cast<char*>(std::uintptr_t{0x00551178});
    inline char* s_rb_0055117c = reinterpret_cast<char*>(std::uintptr_t{0x0055117C});
    inline char* s_error_decoding_broadcast_0055ad58 = reinterpret_cast<char*>(std::uintptr_t{0x0055AD58});
    inline const char* s_p_limit_p_diff_0055ad74 = reinterpret_cast<const char*>(std::uintptr_t{0x0055AD74}); /* "p_limit < p_diff" */
    inline const char* s_fseek_ahead_modem_file_0055ad88 = reinterpret_cast<const char*>(std::uintptr_t{0x0055AD88}); /* "fseek ahead modem_file" */
    inline const char* s_fread_modem_file_0055ada0 = reinterpret_cast<const char*>(std::uintptr_t{0x0055ADA0}); /* "fread modem_file" */
    inline const char* s_fseek_modem_file_0055adb4 = reinterpret_cast<const char*>(std::uintptr_t{0x0055ADB4}); /* "fseek modem_file" */

    s_net_modem_file* __cdecl Open_Modem_File_(char* mode, uint32_t size);

    int __cdecl fseek_modem_file_(s_net_modem_file* mod_file, int offset, int origin);

    size_t __cdecl fread_modem_file_(void* ptr, size_t size, size_t count, s_net_modem_file* stream);

    size_t __cdecl fwrite_modem_file_(const void* buffer, size_t size, size_t count, s_net_modem_file* stream);

    int __cdecl Decode_Difference_Buffer_(s_net_modem_file* modem_file, uint8_t* out_buffer, int max_out_len, uint8_t* diff_buffer, int diff_len);

    int32_t __cdecl Make_Difference_Buffer_(s_net_modem_file* file, uint8_t* buffer, int32_t buffer_size, uint8_t* diff_buffer, int32_t diff_buffer_max_size);

    int32_t __cdecl fgetc_modem_file_(s_net_modem_file* modem_file);

    void __cdecl Close_Modem_File_(s_net_modem_file* file_handle);

    void __cdecl Write_Modem_File_(s_net_modem_file* stream, const void* buffer, size_t size);

    void __cdecl Send_Game_Orders_();

    void __cdecl Receive_Message_(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet);

    inline char* net_field_1be = reinterpret_cast<char*>(std::uintptr_t{0x0059532E});

    void __cdecl Net_Request_AI_Diplomacy_(uint8_t player_idx);

    int8_t __cdecl Net_May_Begin_AI_Diplomacy_();

    bool __cdecl No_AI_Diplomacy_Allowed_();

    void __cdecl Net_Cancel_AI_Diplomacy_Request_();

    bool __cdecl Net_Cancel_AI_Diplomacy_Confirmed_();

    void __cdecl Net_End_AI_Diplomacy_();

    void __cdecl Net_Request_Human_Diplomacy_(uint8_t player_idx);

    int8_t __cdecl Net_May_Begin_Human_Diplomacy_();

    int8_t __cdecl Human_Hit_Next_Turn_(int16_t player_idx);

    bool __cdecl Allowed_To_Cancel_Human_Diplomacy_();

    int8_t __cdecl Human_Diplomat_Says_Wait_();

    bool __cdecl Net_Human_Rejected_Diplomacy_();

    void __cdecl Net_End_Human_Diplomacy_();

    void __cdecl Net_Cancel_Human_Diplomacy_Request_();

    int32_t __cdecl Net_Diplomacy_With_(int16_t limit);

    void __cdecl Human_Abdicates_(int player_idx);

    void __cdecl Human_Surrenders_(int player_idx);

    int8_t __cdecl Get_Next_AI_Talker_(int32_t target_player_id);

    void __cdecl Decode_Pick_Target_Info_(void* packet_data, uint32_t len);

    void __cdecl Decode_You_Are_Target_Info_(void* packet_data, uint32_t len);

    void __cdecl Decode_Battle_Results_(void* packet_data);

    void __cdecl Decode_Pick_Your_Position_(s_net_pack_pick_your_position* param_1);

    void __cdecl Decode_Dragon_Info_(int32_t* colony_id_ptr);

    void __cdecl Send_Dragon_Info_(int32_t colony_id);

    void __cdecl Decode_Drop_Out_Info_(int32_t player_idx, s_net_pack_drop_out_info* drop_out_info);

    void __cdecl Send_Battle_Results_(int16_t star_id, int16_t player_id, int16_t target_player_id, int16_t combat_defender_id, int16_t combat_planet_id, int16_t combat_result);

#pragma pack(push, 1)
    struct s_packet_dragon_info {
        int32_t colony_id;
    };

    struct s_packet_0x12 {
        int16_t star_idx;
        int16_t other_player_idx;
        int16_t param_3;
        int16_t colony_idx;
    };

    struct s_packet_0x34 {
        int16_t chunks;
    };

    struct s_packet_2E_chunks {
        int16_t chunks;
    };

    struct s_packet_32 {
        int16_t chunks;
    };

    struct s_NET_MSG_SYNC_PLAYER {
        int32_t player1_idx;
        s_player player1;
        int8_t surrender_to[72];
        int32_t player2_idx;
        s_player player2;
    };

    struct s_NET_MSG_RANK {
        int32_t rank;
        int8_t hit_next_turn;
    };

    struct s_NET_MSG_COLOR {
        int8_t color;
        int8_t player_idx;
    };

    struct s_NET_MSG_RACE {
        uint8_t race;
        int8_t player_idx;
    };
#pragma pack(pop)
}