#pragma once

#include <guiddef.h>

namespace WINNET {
    void __cdecl Drop_Player_(uint32_t player_id);
    inline uint32_t& _provider_count =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00560798});

    inline s_winnet_provider* (&_providers)[9] =
        *reinterpret_cast<s_winnet_provider*(*)[9]>(std::uintptr_t{0x0058D770});

    inline uint32_t& _net_refresh_timer =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055A99C});

    inline GUID& _game_guid = *reinterpret_cast<GUID*>(std::uintptr_t{0x0054F150});

    inline uint32_t& _session_count = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055BF2C});
    inline int8_t& _is_searching_games = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0056079C});
    inline DPSESSIONDESC* (&_sessions)[100] = *reinterpret_cast<DPSESSIONDESC*(*)[100]>(std::uintptr_t{0x005897F8});
    inline GUID& _app_guid = *reinterpret_cast<GUID*>(std::uintptr_t{0x00589998});

    inline uint32_t& _player_count =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00589988});

    inline s_net_player** _player_list = reinterpret_cast<s_net_player**>(std::uintptr_t{0x005899A8});

    inline DPID& _local_player_id = *reinterpret_cast<DPID*>(std::uintptr_t{0x005899F8});

    inline char* s_Error_Unknown_0055bf7c = reinterpret_cast<char*>(std::uintptr_t{0x0055BF7C}); // "Error Unknown"
    inline char* s_Invalid_Object_0055bf8c = reinterpret_cast<char*>(std::uintptr_t{0x0055BF8C}); // "Invalid Object"
    inline char* s_Invalid_Player_0055bf9c = reinterpret_cast<char*>(std::uintptr_t{0x0055BF9C}); // "Invalid Player"
    inline char* DAT_0055bfac = reinterpret_cast<char*>(std::uintptr_t{0x0055BFAC});
    inline char* s_Active_Players_0055bfb4 = reinterpret_cast<char*>(std::uintptr_t{0x0055BFB4}); // "Active Players"
    inline char* s_User_Cancel_0055bfc4 = reinterpret_cast<char*>(std::uintptr_t{0x0055BFC4}); // "User Cancel"
    inline char* s_Unsupported_0055bfd0 = reinterpret_cast<char*>(std::uintptr_t{0x0055BFD0}); // "Unsupported"
    inline char* s_Unavailable_0055bfdc = reinterpret_cast<char*>(std::uintptr_t{0x0055BFDC}); // "Unavailable"
    inline char* s_Generic_0055bfe8 = reinterpret_cast<char*>(std::uintptr_t{0x0055BFE8}); // "Generic"
    inline char* s_Invalid_Params_0055bff0 = reinterpret_cast<char*>(std::uintptr_t{0x0055BFF0}); // "Invalid Params"
    inline char* s_Unknown_Err_00560a08 = reinterpret_cast<char*>(std::uintptr_t{0x00560A08}); // "Unknown Err"
    inline char* s_Buffer_To_Small_00560a14 = reinterpret_cast<char*>(std::uintptr_t{0x00560A14}); // "Buffer To Small"
    inline char* s_No_Messages_00560a24 = reinterpret_cast<char*>(std::uintptr_t{0x00560A24}); // "No Messages"
    inline char* s_DirectPlay_ERR_00560a30 = reinterpret_cast<char*>(std::uintptr_t{0x00560A30}); // "DirectPlay ERR: "

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