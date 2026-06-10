#pragma once

#include <guiddef.h>

namespace WINNET {
    extern int32_t _provider_count;

    extern s_winnet_provider* _providers[9];

    extern clock_t _net_refresh_timer;

    extern GUID _game_guid;

    extern uint32_t _session_count;
    extern GUID _app_guid;

    extern uint32_t _player_count;

    extern uint32_t _local_player_id;

    extern int32_t _provider_mode;

    uint32_t __cdecl Get_Player_Network_Id_(int32_t player_idx);

    void __cdecl Handle_Create_Player_(s_net_pack_create_player* pack);

    void __cdecl Handle_Destroy_Player_(s_net_pack_destroy_player* pack);

    uint32_t __cdecl Refresh_Game_List_(GUID guid);

    int32_t __cdecl Init_Connection_(int32_t mode);

    int32_t __cdecl Init_Providers_();

    void __cdecl Net_Enable_New_Players();

    void __cdecl Net_Disable_New_Players_();

    char* __cdecl Get_Game_Name_(uint32_t session_idx);

    int32_t __cdecl Join_Game_(const char* game_name, const char* password, const char* player_name);

    int32_t __cdecl Create_Game_(GUID guid, const char* name, int32_t max_players, const char* password, const char* player_name);

    void __cdecl Shutdown_();

    int32_t __cdecl Send_Message_(uint32_t from_player_id, uint32_t to_player_id, void* data, uint32_t size);

    int32_t __cdecl Poll_Message_(s_comcore_poll_buf* poll_buf);

    void __cdecl SetPlayerLastKeepAlive_(s_comcore_poll_buf* poll_buf);
}