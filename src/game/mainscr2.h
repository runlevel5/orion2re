#pragma once
#include <cstdint>

struct s_mouse_list;

namespace MAINSCR2 {
    extern s_mouse_list _mouse_list_from[5];
    extern s_mouse_list _mouse_list_to[5];

    void __cdecl Draw_Main_Screen_For_Star_Distan();
    extern uint8_t _main_screen_fade_in_flag;
    extern uint8_t _force_main_screen_fade;

    extern uint8_t _in_reports_phase;

    void __cdecl Begin_Of_Turn_(int8_t* p_report_flag);

    void __cdecl Main_Screen_Report_Handler_(int8_t *reports_available);

    void __cdecl Check_Fleet_Movement_Box_Status_();

    void __cdecl Restore_Fleet_Box_Palette_();

    void __cdecl Distance_Between_Stars_Screen_();
    void __cdecl Planet_Colonization_In_Main_Screen_();
    void __cdecl Building_Outposts_In_Main_Screen_();
    void __cdecl Unloading_Transports_In_Main_Screen_();
    void __cdecl Scrap_Freighters_();
    void __cdecl Do_Begin_Of_Turn_();
    void __cdecl Reports_Screen_();
    void Show_Command_Points_Screen_();
    void __cdecl Redraw_Main_Screen_After_Popups_(uint8_t param_1);
}