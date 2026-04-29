#pragma once
#include <cstdint>

struct s_mouse_list;
struct s_colors;

namespace ANTAROOM {
    inline s_colors& _antaran_room_normal_colors =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x00556090});

    inline char*& _antaran_room_message =
        *reinterpret_cast<char**>(std::uintptr_t{0x005A7B78});
    inline uint8_t*& _antaran_room_mouse_bitmap =
        *reinterpret_cast<uint8_t**>(std::uintptr_t{0x005A7B7C});

    // "antaroom.LBX"
    inline char const* const s_antaroom_LBX_00556098 =
        reinterpret_cast<char const*>(std::uintptr_t{0x00556098});

    inline const char* s_antarmsg_LBX_005560a8 = reinterpret_cast<const char*>(std::uintptr_t{0x005560A8}); // "antarmsg.LBX"

    inline s_mouse_list* _antaran_room_blank_mouse =
        reinterpret_cast<s_mouse_list*>(std::uintptr_t{ 0x00556080 });

    void __cdecl Reload_Main_Antaran_Room_Screen_();
    void __cdecl Get_Antaran_Room_Message_(int16_t message_index);
    void __cdecl Draw_Main_Antaran_Room_Screen_();

    void __cdecl Main_Antaran_Room_Screen_(int16_t param_1);
}