#pragma once
#include <cstdint>
#include <guiddef.h>

struct s_network;

namespace WINNET {
    inline uint32_t& _net_refresh_timer =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055A99C});

    inline GUID& _game_guid = *reinterpret_cast<GUID*>(std::uintptr_t{0x0054F150});

    inline void __stdcall Refresh_Game_List_(GUID guid) {
        using Fn = void(__stdcall*)(GUID);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00534FA0});
        fn(guid);
    }

    inline int32_t __cdecl Init(uint32_t mode, GUID guid, void (*recv_fn)(void*), s_network* net) {
        using Fn = int32_t(__cdecl*)(uint32_t, GUID, void (*)(void*), s_network*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004c2a40});
        return fn(mode, guid, recv_fn, net);
    }

    inline void __cdecl Ten_Stop(int32_t val) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0043AAA0});
        fn(val);
    }

    inline void __cdecl Ten_Flag() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0043AA90});
        fn();
    }
}