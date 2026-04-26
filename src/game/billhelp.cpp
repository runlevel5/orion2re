#include "pch.h"

namespace BILLHELP {
    void __cdecl My_Set_Help_List_(s_help_box* help_list, int16_t count) {
        fields::Deactivate_Help_List_();
        fields::Set_Help_List_(help_list, count);
    }

    void __cdecl History_Graph_Help_() {
        _info_help_list[2] = _history_graph_help_list[0];
        My_Set_Help_List_(_info_help_list, 3);
    }

    void __cdecl Tech_Review_Help_() {
        _info_help_list[2] = _tech_review_help_list[0];
        My_Set_Help_List_(_info_help_list, 3);
    }

    void __cdecl Race_Statistics_Help_() {
        _info_help_list[2] = _race_statistics_help_list[0];
        My_Set_Help_List_(_info_help_list, 3);
    }

    void __cdecl Turn_Summary_Popup_Help_() {
        My_Set_Help_List_(_turn_summary_popup_help_list, 1);
    }

    void __cdecl Turn_Summary_Help_() {
        _info_help_list[2] = _turn_summary_help_list[0];
        My_Set_Help_List_(_info_help_list, 3);
    }

    void __cdecl Races_Help_() {
        My_Set_Help_List_(_races_help_list, 8);
    }

    void __cdecl Technology_Change_Help_() {
        My_Set_Help_List_(_technology_change_help_list, 3);
    }

    void __cdecl Technology_Select_Help_() {
        My_Set_Help_List_(_technology_select_help_list, 3);
    }

    void __cdecl Report_Help_() {
        My_Set_Help_List_(_report_help_list, 1);
    }

    void __cdecl Reference_Help_() {
        _info_help_list[2] = _reference_help_list[0];
        My_Set_Help_List_(_info_help_list, 3);
    }

    void __cdecl Ref_How_To_Help_() {
        _info_help_list[2] = _ref_category_help_list[0];
        My_Set_Help_List_(_info_help_list, 3);
    }

    void __cdecl Ref_Category_Help_() {
        _info_help_list[2] = _ref_how_to_help_list[0];
        My_Set_Help_List_(_info_help_list, 3);
    }

    void __cdecl Tech_Change_List_Right_Help_() {
        My_Set_Help_List_(_technology_list3_help_list, 2);
    }

    void __cdecl Tech_Select_List_Right_Help_() {
        My_Set_Help_List_(_technology_list1_help_list, 2);
    }

    void __cdecl Tech_Change_List_Left_Help_() {
        My_Set_Help_List_(_technology_list4_help_list, 2);
    }

    void __cdecl Tech_Select_List_Left_Help_() {
        My_Set_Help_List_(_technology_list2_help_list, 2);
    }
}