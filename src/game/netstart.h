#pragma once
#include <cstdint>

namespace NETSTART {
    uint8_t __cdecl Start_Net_Screen_();
    uint8_t __cdecl Join_Net_Screen_();
    uint8_t __cdecl Load_Net_Screen_();

    int32_t __cdecl Russ_Net_Create_Game_(GUID guid, char* game_name, int32_t max_players, const char* str1, const char* str2);

    bool __cdecl Do_Dial_(char* phone_number);

    char* __cdecl Load_Netstart_Strings_(char* buffer);
    int8_t __cdecl Request_Race_(uint8_t race_idx);

    void __cdecl Mox_Net_Uninit_();

    int __cdecl Dump_Users_();

    inline char*& _dialing_failed_error =
        *reinterpret_cast<char**>(std::uintptr_t{0x0056CE78});
    inline char*& _receiving_chunk_d_of_d =
        *reinterpret_cast<char**>(std::uintptr_t{0x0056CE7C});
    inline char*& _really_begin_game_with_unused_slots_msg =
        *reinterpret_cast<char**>(std::uintptr_t{0x0056CE80});
    inline char*& _really_start_with_only_1_msg =
        *reinterpret_cast<char**>(std::uintptr_t{0x0056CE84});
    inline char*& _no_more_player_slots_error =
        *reinterpret_cast<char**>(std::uintptr_t{0x0056CE8C});
    inline char*& _game_full_string =
        *reinterpret_cast<char**>(std::uintptr_t{0x0056CE90});
    inline char*& _answering_failed_error =
        *reinterpret_cast<char**>(std::uintptr_t{0x0056CE94});
    inline char*& _windows_string =
        *reinterpret_cast<char**>(std::uintptr_t{0x0056CE98});

    int8_t __cdecl Net_Select_Race_();

    int8_t __cdecl Net_Pick_Position_();

    int8_t __cdecl Do_Answer_();

    inline const char* s__00551100 = reinterpret_cast<const char*>(std::uintptr_t{0x00551100});
    inline const char* s_myname_005597dc = reinterpret_cast<const char*>(std::uintptr_t{0x005597DC});
    inline const char* s__s_00551130 = reinterpret_cast<const char*>(std::uintptr_t{0x00551130});
    inline const char* s_Mar_26_1997_005578e0 = reinterpret_cast<const char*>(std::uintptr_t{0x005578E0});

    inline GUID& GUID_0054F150 = *reinterpret_cast<GUID*>(std::uintptr_t{0x0054F150});
}