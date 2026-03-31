#pragma once

namespace council {
    inline uint8_t& _council_state =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00596028});
    inline int16_t (&_council_nominee_vote_total)[2] = *reinterpret_cast<int16_t(*)[2]>(std::uintptr_t{0x0059601C});

    inline const char* s__00551100 = reinterpret_cast<const char*>(std::uintptr_t{0x00551100});

    void Check_For_Council_Meeting_();

    void __cdecl Next_Council_State_();

    void __cdecl Main_Council_Screen_();

    inline void __cdecl Draw_Main_Council_Screen_() {
        using Fn = void(__cdecl*)();
        auto func = reinterpret_cast<Fn>(std::uintptr_t{0x004d8130});
        func();
    }

    void __cdecl Control_Main_Council_Screen_();

    inline void __cdecl Reload_Main_Council_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004d8440})();
    }

    inline void __cdecl Get_Council_Message_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004d89a0})();
    }

    inline void __cdecl Council_Votes_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004d8ce0})();
    }

    inline void __cdecl Council_Finished_Voting_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004d8c30})();
    }

    inline void __cdecl Council_Accept_Ruling_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004d9430})();
    }
}