#pragma once

namespace ten {
    /* Handles an incoming TEN packet.
     * net_id: Sender or peer network identifier.
     * packet_id: Packet type identifier.
     * len: Packet payload size in bytes.
     * packet: Packet payload buffer.
     */
    using recv_fn = void(__cdecl*)(int16_t net_id, uint16_t packet_id, uint32_t len, uint8_t* packet);

    /* Sends a TEN packet; current stub performs no work. */
    inline void __cdecl Send_Message_(int32_t, int16_t, uint8_t*, uint32_t) {
    }

    /* Initializes TEN internal systems; current stub performs no work. */
    inline void init() {
    }

    /* Returns the current TEN host identifier; current stub always returns 0. */
    inline uint32_t __cdecl TEN_Get_Host_Id_() {
        return 0;
    }

    /* Returns TEN user data through a caller-provided buffer; current stub always returns 0. */
    inline int32_t __cdecl TEN_Get_Users_(uint8_t*) {
        return 0;
    }

    /* Initializes the TEN networking layer; current stub performs no work. */
    inline void __cdecl Init_TEN_(uint32_t, GUID*, recv_fn, s_network*) {
    }

    /* Stops the TEN subsystem; current stub performs no work. */
    inline void __cdecl Ten_Stop(int32_t) {
    }

    /* Updates TEN runtime state; current stub performs no work. */
    inline void __cdecl Ten_Update_() {
    }

    /* TEN flag entry point; current stub performs no work. */
    inline void __cdecl Ten_Flag() {
    }
}