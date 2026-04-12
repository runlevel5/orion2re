#pragma once

namespace ten {
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
}