#pragma once
#include <cstdint>
#include <guiddef.h>

struct s_network;

namespace WINNET {
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

    inline s_net_player*& _player_list =
        *reinterpret_cast<s_net_player**>(std::uintptr_t{0x005899A8});

    inline DPID& _local_player_id = *reinterpret_cast<DPID*>(std::uintptr_t{0x005899F8});

    inline uint32_t __cdecl TEN_Get_Host_Id_() {
        using Fn = uint32_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00533960});
        return fn();
    }

    inline int32_t __cdecl TEN_Get_Users_(uint8_t* users) {
        using Fn = int32_t(__cdecl*)(uint8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00533930});
        return fn(users);
    }

    uint32_t __cdecl Get_Player_Network_Id_(int32_t player_idx);

    int __stdcall Refresh_Game_List_Callback_(LPDPSESSIONDESC session_desc, void* context, uint32_t* timeout, uint32_t flags);

    int __cdecl Refresh_Player_List_(DWORD dwSession);

    inline BOOL __stdcall Player_Enumerate_Callback_(DPID dpId, LPSTR lpFriendlyName, LPSTR lpFormalName, DWORD dwFlags, LPVOID lpContext) {
        using Fn = BOOL(__stdcall*)(DPID, LPSTR, LPSTR, DWORD, LPVOID);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00538780});
        return fn(dpId, lpFriendlyName, lpFormalName, dwFlags, lpContext);
    }

    inline void __cdecl Exit_With_DPlay_Error_(HRESULT error_code) {
        using Fn = void(__cdecl*)(HRESULT);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00539CE0});
        fn(error_code);
    }

    inline void __cdecl Register_Player_(DPID pid, char* friendly_name, char* formal_name) {
        using Fn = void(__cdecl*)(DPID, char*, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005387A0});
        fn(pid, friendly_name, formal_name);
    }

    void __cdecl Create_Player_(DPID* pid, char* friendly_name, char* formal_name);

    uint32_t __cdecl Refresh_Game_List_(GUID guid);

    int8_t __cdecl Find_Provider_(const char* name);

    int32_t __cdecl Init_Connection_(int32_t mode);

    inline void __cdecl Init_TEN_(uint32_t mode, GUID* guid, void* recv_fn, s_network* net) {
        using Fn = void(__cdecl*)(uint32_t, GUID*, void*, s_network*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005337F0});
        fn(mode, guid, recv_fn, net);
    }

    inline void __cdecl Ten_Stop(int32_t val) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0043AAA0});
        fn(val);
    }

    inline void __cdecl Ten_Update_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00533890});
        fn();
    }

    inline void __cdecl Ten_Flag() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0043AA90});
        fn();
    }

    BOOL __stdcall Enumerate_Providers_(GUID* guid, char* name, DWORD major_version, DWORD minor_version, void* context);

    int32_t __cdecl Init_Providers_();

    inline void __cdecl Net_Enable_New_Players() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00535400});
        fn();
    }
}