#pragma once
#include <cstdint>

struct s_help_box;

namespace BILLHELP {
    // Clears the active help list and installs `help_list` with `count` entries.
    void __cdecl My_Set_Help_List_(s_help_box* help_list, int16_t count);

    extern s_help_box _info_help_list[3];

    extern s_help_box _reference_help_list[1];

    extern s_help_box _ref_category_help_list[1];

    extern s_help_box _ref_how_to_help_list[1];

    extern s_help_box _turn_summary_help_list[1];

    extern s_help_box _turn_summary_popup_help_list[1];

    extern s_help_box _race_statistics_help_list[1];

    extern s_help_box _tech_review_help_list[1];

    extern s_help_box _history_graph_help_list[1];

    extern s_help_box _technology_select_help_list[3];

    extern s_help_box _technology_change_help_list[3];

    // Activates the help list for the history graph screen.
    void __cdecl History_Graph_Help_();
    // Activates the help list for the technology review screen.
    void __cdecl Tech_Review_Help_();
    // Activates the help list for the race statistics screen.
    void __cdecl Race_Statistics_Help_();
    // Activates the help list for the turn summary popup.
    void __cdecl Turn_Summary_Popup_Help_();
    // Activates the help list for the turn summary screen.
    void __cdecl Turn_Summary_Help_();

    // Activates the help list for the races screen.
    void __cdecl Races_Help_();
    // Activates the help list for the technology change screen.
    void __cdecl Technology_Change_Help_();
    // Activates the help list for the technology selection screen.
    void __cdecl Technology_Select_Help_();
    // Activates the help list for the report screen.
    void __cdecl Report_Help_();
    // Activates the help list for the reference screen.
    void __cdecl Reference_Help_();
    // Activates the help list for the reference how-to screen.
    void __cdecl Ref_How_To_Help_();
    // Activates the help list for the reference category screen.
    void __cdecl Ref_Category_Help_();
    // Activates the help list for the right-side technology change list.
    void __cdecl Tech_Change_List_Right_Help_();
    // Activates the help list for the right-side technology selection list.
    void __cdecl Tech_Select_List_Right_Help_();
    // Activates the help list for the left-side technology change list.
    void __cdecl Tech_Change_List_Left_Help_();
    // Activates the help list for the left-side technology selection list.
    void __cdecl Tech_Select_List_Left_Help_();

    extern s_help_box _technology_list1_help_list[2];

    extern s_help_box _technology_list2_help_list[2];

    extern s_help_box _technology_list3_help_list[2];

    extern s_help_box _technology_list4_help_list[2];

    extern s_help_box _races_help_list[8];

    extern s_help_box _report_help_list[1];

}