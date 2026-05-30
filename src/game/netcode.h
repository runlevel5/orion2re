#pragma once

namespace netcode {
    using recv_fn = void(__cdecl*)(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet);

    extern uint32_t _netInitialized;

    extern int8_t _tpSending;

    extern uint8_t _inComms;

    extern IDirectPlay* _dplay;

    extern recv_fn _recv_fn;

    extern int32_t _connection_type;

    extern int32_t _in_send_message;

    int32_t __cdecl Do_Net_Init_(uint32_t mode, GUID guid, recv_fn recv_fn_ptr, s_network* network);

    int32_t __cdecl Net_Init_(uint32_t mode, GUID guid, recv_fn recv_fn_ptr, s_network* network);

    int32_t __cdecl Net_Get_Game_Count_();

    int32_t __cdecl Get_Player_Count_();

    char* __cdecl Net_Get_Game_Name_(uint32_t session_idx);

    int32_t __cdecl Net_Join_Game_(char* game_name, char* password, char* player_name);

    void __cdecl Net_Uninit_();

    int32_t __cdecl Net_Is_Sending_();

    void __cdecl Set_Null_Modem_Slave_();

    void __cdecl Set_Null_Modem_Host_();

    int32_t __cdecl Net_Get_Users_(uint8_t* users);

    void __cdecl Net_Disable_New_Players_();

    uint32_t __cdecl Net_Host_ID_();

    uint32_t __cdecl Net_Send_Message_(int32_t target_id, int16_t msg_type, void* data, uint32_t size);

    void __cdecl Net_Update_();

    uint32_t __cdecl Net_Status_();

    int32_t __cdecl Net_Create_Game_(GUID guid, char* name, int32_t max_players, char* password, char* player_name);
}