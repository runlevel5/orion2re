#pragma once

namespace netcode {
    inline uint32_t& _netInitialized = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055BF24});

    inline uint8_t& _inComms =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055BF34});

    inline IDirectPlay*& _dplay = *reinterpret_cast<IDirectPlay**>(std::uintptr_t{0x0055BF28});

    inline void*& _recv_fn =
        *reinterpret_cast<void**>(std::uintptr_t{0x00589990});

    inline int32_t& _connection_type =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x00589A0C});

    inline int32_t& _in_send_message = *reinterpret_cast<int32_t*>(std::uintptr_t{0x0055BF38});

    int32_t __cdecl Do_Net_Init_(uint32_t mode, GUID guid, void* recv_fn, s_network* network);

    int32_t __cdecl Net_Init_(uint32_t mode, GUID guid, void* recv_fn, s_network* network);

    int32_t __cdecl Net_Get_Game_Count_();

    int32_t __cdecl Get_Player_Count_();

    char* __cdecl Net_Get_Game_Name_(uint32_t session_idx);

    int32_t __cdecl Net_Join_Game_(char* game_name, char* password, char* player_name);

    void __cdecl Net_Uninit_();

    int __cdecl Net_Is_Sending_();

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