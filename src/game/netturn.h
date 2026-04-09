#pragma once
#include <cstdint>

namespace NETTURN {
    void __cdecl Net_Next_Turn_();

    void __cdecl Host_Next_Turn_();

    void __cdecl Client_Next_Turn_();

    char* __cdecl Load_Netturn_Strings_(char* input_strings);

    inline char*& _waiting_fmt =
        *reinterpret_cast<char**>(std::uintptr_t{0x005A63E4});

    inline char*& _have_d_of_d_chunks_fmt =
        *reinterpret_cast<char**>(std::uintptr_t{0x005662A8});

    inline bool __cdecl Must_Broadcast_AI_Player_Data_() {
        using Fn = bool(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0047dd00});
        return fn();
    }

    inline void __cdecl Net_Init_Host_Election_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0047DD40});
        fn();
    }
}