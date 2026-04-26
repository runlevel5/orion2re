#pragma once
#include <cstdint>

struct s_help_box;

namespace BILLHELP {
    void __cdecl My_Set_Help_List_(s_help_box* help_list, int16_t count);

    inline s_help_box (&_info_help_list)[3] =
        *reinterpret_cast<s_help_box(*)[3]>(std::uintptr_t{0x00558740});

    inline s_help_box (&_reference_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x00558768});

    inline s_help_box (&_ref_category_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x00558778});

    inline s_help_box (&_ref_how_to_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x00558788});

    inline s_help_box (&_turn_summary_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x00558798});

    inline s_help_box (&_turn_summary_popup_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x005587A8});

    inline s_help_box (&_race_statistics_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x005587B8});

    inline s_help_box (&_tech_review_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x005587C8});

    inline s_help_box (&_history_graph_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x005587D8});

    inline s_help_box (&_technology_select_help_list)[3] =
        *reinterpret_cast<s_help_box(*)[3]>(std::uintptr_t{0x005587E8});

    inline s_help_box (&_technology_change_help_list)[3] =
        *reinterpret_cast<s_help_box (*)[3]>(std::uintptr_t{0x00558808});

    void __cdecl History_Graph_Help_();
    void __cdecl Tech_Review_Help_();
    void __cdecl Race_Statistics_Help_();
    void __cdecl Turn_Summary_Popup_Help_();
    void __cdecl Turn_Summary_Help_();

    void __cdecl Races_Help_();
    void __cdecl Technology_Change_Help_();
    void __cdecl Technology_Select_Help_();
    void __cdecl Report_Help_();
    void __cdecl Reference_Help_();
    void __cdecl Ref_How_To_Help_();
    void __cdecl Ref_Category_Help_();
    void __cdecl Tech_Change_List_Right_Help_();
    void __cdecl Tech_Select_List_Right_Help_();
    void __cdecl Tech_Change_List_Left_Help_();
    void __cdecl Tech_Select_List_Left_Help_();

    inline s_help_box (&_technology_list1_help_list)[2] =
        *reinterpret_cast<s_help_box(*)[2]>(std::uintptr_t{0x00558828});

    inline s_help_box (&_technology_list2_help_list)[2] =
        *reinterpret_cast<s_help_box(*)[2]>(std::uintptr_t{0x00558840});

    inline s_help_box (&_technology_list3_help_list)[2] =
        *reinterpret_cast<s_help_box (*)[2]>(std::uintptr_t{0x00558858});

    inline s_help_box (&_technology_list4_help_list)[2] =
        *reinterpret_cast<s_help_box(*)[2]>(std::uintptr_t{0x00558870});

    inline s_help_box (&_races_help_list)[8] =
        *reinterpret_cast<s_help_box(*)[8]>(std::uintptr_t{0x00558888});

    inline s_help_box (&_report_help_list)[1] =
        *reinterpret_cast<s_help_box(*)[1]>(std::uintptr_t{0x005588D8});

}