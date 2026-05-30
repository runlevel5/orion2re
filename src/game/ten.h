#pragma once

namespace ten {
    using recv_fn = void(__cdecl*)(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet);

    inline void __cdecl Send_Message_(int32_t target_id, int16_t msg_type, uint8_t* data, uint32_t size) {
        using Fn = void(__cdecl*)(int32_t, int16_t, uint8_t*, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005338A0});
        fn(target_id, msg_type, data, size);
    }

    // Suggested name: ten::Initialize_Internal_Systems
    inline void init() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004D7D30 });
        fn();
    }

    inline uint32_t __cdecl TEN_Get_Host_Id_() {
        using Fn = uint32_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00533960 });
        return fn();
    }

    inline int32_t __cdecl TEN_Get_Users_(uint8_t* users) {
        using Fn = int32_t(__cdecl*)(uint8_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00533930 });
        return fn(users);
    }

    inline void __cdecl Init_TEN_(uint32_t mode, GUID* guid, recv_fn recv_fn_ptr, s_network* net) {
        using Fn = void(__cdecl*)(uint32_t, GUID*, recv_fn, s_network*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005337F0 });
        fn(mode, guid, recv_fn_ptr, net);
    }

    inline void __cdecl Ten_Stop(int32_t val) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0043AAA0 });
        fn(val);
    }

    inline void __cdecl Ten_Update_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00533890 });
        fn();
    }

    inline void __cdecl Ten_Flag() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0043AA90 });
        fn();
    }
}