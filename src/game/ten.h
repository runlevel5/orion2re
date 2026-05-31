#pragma once

namespace ten {
    using recv_fn = void(__cdecl*)(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet);

    inline void __cdecl Send_Message_(int32_t, int16_t, uint8_t*, uint32_t) {
    }

    // Suggested name: ten::Initialize_Internal_Systems
    inline void init() {
    }

    inline uint32_t __cdecl TEN_Get_Host_Id_() {
        return 0;
    }

    inline int32_t __cdecl TEN_Get_Users_(uint8_t*) {
        return 0;
    }

    inline void __cdecl Init_TEN_(uint32_t, GUID*, recv_fn, s_network*) {
    }

    inline void __cdecl Ten_Stop(int32_t) {
    }

    inline void __cdecl Ten_Update_() {
    }

    inline void __cdecl Ten_Flag() {
    }
}