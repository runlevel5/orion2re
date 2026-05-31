#pragma once

#include <guiddef.h>

namespace WINNET {
    void __cdecl Drop_Player_(uint32_t player_id);
    extern int32_t _provider_count;

    extern s_winnet_provider* _providers[9];

    extern clock_t _net_refresh_timer;

    extern GUID _game_guid;

    extern uint32_t _session_count;
    extern int8_t _is_searching_games;
    extern DPSESSIONDESC* _sessions[100];
    extern GUID _app_guid;

    extern uint32_t _player_count;

    extern s_net_player* _player_list[20];

    extern DPID _local_player_id;

    extern int32_t _provider_mode;

    extern char s_Error_Unknown_0055bf7c[16]; // "Error Unknown"
    extern char s_Invalid_Object_0055bf8c[16]; // "Invalid Object"
    extern char s_Invalid_Player_0055bf9c[16]; // "Invalid Player"
    extern char DAT_0055bfac[8]; // "Busy"
    extern char s_Active_Players_0055bfb4[16]; // "Active Players"
    extern char s_User_Cancel_0055bfc4[12]; // "User Cancel"
    extern char s_Unsupported_0055bfd0[12]; // "Unsupported"
    extern char s_Unavailable_0055bfdc[12]; // "Unavailable"
    extern char s_Generic_0055bfe8[8]; // "Generic"
    extern char s_Invalid_Params_0055bff0[16]; // "Invalid Params"
    extern char s_Unknown_Err_00560a08[12]; // "Unknown Err"
    extern char s_Buffer_To_Small_00560a14[16]; // "Buffer To Small"
    extern char s_No_Messages_00560a24[12]; // "No Messages"
    extern char s_DirectPlay_ERR_00560a30[20]; // "DirectPlay ERR: "
    extern const char s_modem_005607dc[6];
    extern const char s_ipx_005607e4[4];
    extern const char s_tcp_005607e8[4];

    uint32_t __cdecl Get_Player_Network_Id_(int32_t player_idx);

    int __stdcall Refresh_Game_List_Callback_(LPDPSESSIONDESC session_desc, void* context, uint32_t* timeout, uint32_t flags);

    int32_t __cdecl Refresh_Player_List_(DWORD session_id);

    BOOL __stdcall Player_Enumerate_Callback_(DPID dp_id, char* short_name, char* long_name, DWORD flags, void* context);

    void __cdecl Exit_With_DPlay_Error_(int32_t error_code);

    void __cdecl Register_Player_(uint32_t player_id, char* player_name, char* event_str);

    void __cdecl Create_Player_(DPID* pid, char* friendly_name, char* formal_name);
    void __cdecl Handle_Create_Player_(s_net_pack_create_player* pack);
    void __cdecl Handle_Destroy_Player_(s_net_pack_destroy_player* pack);

    uint32_t __cdecl Refresh_Game_List_(GUID guid);

    int8_t __cdecl Find_Provider_(const char* name);

    int32_t __cdecl Init_Connection_(int32_t mode);

    BOOL __stdcall Enumerate_Providers_(GUID* guid, char* name, DWORD major_version, DWORD minor_version, void* context);

    int32_t __cdecl Init_Providers_();

    void __cdecl Net_Enable_New_Players();

    void __cdecl Exit_With_Error_(HRESULT hr);

    void __cdecl SetPlayerLastKeepAlive_(s_comcore_poll_buf* poll_buf);
}