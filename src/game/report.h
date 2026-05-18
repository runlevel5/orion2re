#pragma once

namespace REPORT {
    inline int16_t& _n_reports = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A4EAA});
    inline s_report (&_reports)[50] = *reinterpret_cast<s_report(*)[50]>(std::uintptr_t{0x005A4EC0});
    inline uint8_t& _done_player_reports = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A4EB8});
    inline uint8_t (&_something_interesting_happened)[8] = *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x005A4EB0});
    inline uint8_t (&_done_colonize_check)[9] = *reinterpret_cast<uint8_t(*)[9]>(std::uintptr_t{0x005A4EA0});
    inline uint8_t& _displayed_msgs = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A5276});
    inline uint8_t& _displayed_gnn = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A4EAC});

    void __cdecl Client_Initialize_Reports_();

    void __cdecl Initialize_Reports_();
    void __cdecl Add_Report_(s_report* report);

    inline char*& _your_spy_destroys_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x0056CC68});
    inline char*& _your_spy_steals_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x0056CC6C});
    inline char*& _you_framed_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x0056CC70});
    inline char*& _colonize_preselected_planet_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x0056CC74});
    inline char*& _enemy_spy_destroys_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x0056CC78});
    inline char*& _really_trash_colony_base_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x0056CC7C});
    inline char*& _enemy_spy_steals_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x0056CC80});
    inline char*& _outpost_preselected_planet_fmt = *reinterpret_cast<char**>(std::uintptr_t{0x0056CC84});

    char* __cdecl Load_Report_Strings_(char* data);

    s_colony* __cdecl Player_Colony_Base_At_Star_(short player_idx, short star_idx);
    s_ship_data* __cdecl Player_Ship_Type_At_Star_(short owner, short ship_type, short location, char include_just_built);
    int8_t __cdecl Has_Msg_(int16_t player_idx, int8_t show_summary);
    int8_t __cdecl Antarans_(int16_t param_1, int8_t param_2);

    int8_t __cdecl Colonization_(int16_t player_idx, char show_dialog);

    int8_t __cdecl Has_Report_(int16_t player_idx, bool show_messages);

    void __cdecl sprintf_report_(char* buffer, s_report* report);

    int8_t __cdecl Display_Report_Aux_(int16_t player_idx, uint8_t param2);

    uint8_t __cdecl Display_Report_(int16_t report_idx);

    void __cdecl Redirect_Newly_Built_Ships_(int16_t player_idx);

    uint8_t __cdecl Must_Display_Reports_(int16_t player_num);
    int8_t __cdecl Has_GNN_(uint8_t index, bool mark_as_displayed);
    int8_t __cdecl Set_Initial_Tech_(short player_idx, char select_tech);
    int8_t __cdecl Has_Officer_For_Hire_(int16_t player_idx, int8_t hire_flag);
    int8_t __cdecl Has_Research_Breakthrough_(int16_t player_idx, int8_t do_select);
    int8_t __cdecl Has_Evolutionary_Upgrade_(int16_t player_idx, int8_t do_setup);
    int8_t __cdecl Star_Has_Special_For_Player_(s_star_data* star, int player_idx);

    int8_t __cdecl Draw_Special_(s_star_data* star, int16_t param_2);

    int8_t __cdecl Explored_New_Star_(short player_idx, char show_popup);
    int8_t __cdecl Has_Diplomacy_Messages_(int16_t receiver_idx, bool show_messages);
    int8_t __cdecl Need_To_Set_Occupation_Policy_(int16_t player_idx, int8_t show_screen);
    int8_t __cdecl Empty_Colony_Queue_(int16_t player_idx, int8_t do_jump);
}