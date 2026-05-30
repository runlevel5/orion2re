#pragma once
#include <cstdint>

struct s_mouse_list;
struct s_colors;

namespace ANTAROOM {
    extern s_colors _antaran_room_normal_colors;

    extern char* _antaran_room_message;
    extern uint8_t* _antaran_room_mouse_bitmap;

    // "antaroom.LBX"
    extern char const s_antaroom_LBX_00556098[13];

    extern char const s_antarmsg_LBX_005560a8[13]; // "antarmsg.LBX"

    extern s_mouse_list _antaran_room_blank_mouse[1];

    void __cdecl Reload_Main_Antaran_Room_Screen_();
    void __cdecl Get_Antaran_Room_Message_(int16_t message_index);
    void __cdecl Draw_Main_Antaran_Room_Screen_();

    void __cdecl Main_Antaran_Room_Screen_(int16_t param_1);
}