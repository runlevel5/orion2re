#pragma once

namespace OFFICER {
    inline char* s_OFFICER_LBX_00556980 = reinterpret_cast<char*>(std::uintptr_t{0x00556980});

    s_animation_header* __cdecl Load_Officer_Picture_(
        int32_t unused,
        int16_t leader_idx,
        s_animation_header* dest,
        uint8_t use_far_reload,
        int16_t pic_type
    );
    inline s_colors& _normal_text_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x005569A0});
    inline s_colors& _red_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00556998});
    inline const char* s_GEGGGGG_format = reinterpret_cast<const char*>(std::uintptr_t{0x005569C0});
    inline int16_t& _officer_dismiss_btn = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056566C});
    inline uint8_t& _offscrn_skip_draw_ships = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00565670});
    inline uint8_t& _full_draw = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005656C8});
    inline int8_t& _officer_button_mode = *reinterpret_cast<int8_t*>(std::uintptr_t{0x0056573C});
    inline int16_t& _officer_pool_btn = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005657B8});
    inline uint8_t& _offscrn_flash_incr = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A77A0});
    inline uint8_t& _offscrn_ask_for_hire_popup = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00565668});
    inline int16_t& _fleet_icon_ship_id_to_move = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A77A2});
    inline int8_t& _offscrn_flash_dir = *reinterpret_cast<int8_t*>(std::uintptr_t{0x005A77A4});
    inline int16_t& _officer_star_displayed = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A77A6});
    inline uint8_t& _offscrn_flash_value = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A77A8});
    inline int16_t& _officer_star_chosen = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A77AA});
    inline s_prev_leader_data*& _prev_leaders =
        *reinterpret_cast<s_prev_leader_data**>(std::uintptr_t{0x00565740});
    inline s_officer_picture_slot*& _ldrpict = *reinterpret_cast<s_officer_picture_slot**>(std::uintptr_t{0x005657B4});
    inline const char*& PTR_s_OFFICER_LBX = *reinterpret_cast<const char**>(std::uintptr_t{0x00556990});
    inline char*& PTR_s_OFFICER_LBX_00556990 =
        *reinterpret_cast<char**>(std::uintptr_t{0x00556990});
    inline const char* const s_ITASKLLS_LBX_005569fc =
        reinterpret_cast<const char*>(std::uintptr_t{ 0x005569FC }); // "ITASKLLS.LBX"
    inline const char* const s_SPASKLLS_LBX_00556a0c =
        reinterpret_cast<const char*>(std::uintptr_t{ 0x00556A0C }); // "SPASKLLS.LBX"
    inline const char* const s_FRESKLLS_LBX_00556a1c =
        reinterpret_cast<const char*>(std::uintptr_t{ 0x00556A1C }); // "FRESKLLS.LBX"
    inline const char* const s_GERSKLLS_LBX_00556a2c =
        reinterpret_cast<const char*>(std::uintptr_t{ 0x00556A2C }); // "GERSKLLS.LBX"
    inline const char* const s_SKILDESC_LBX_00556a3c =
        reinterpret_cast<const char*>(std::uintptr_t{ 0x00556A3C }); // "SKILDESC.LBX"

    int16_t __cdecl Get_Officer_Base_Level_(int16_t xp, int8_t warlord);

    int16_t __cdecl Get_Officer_Level_(int16_t leader_idx);

    int16_t __cdecl Bonus_For_Famous_Heroes_(int16_t player_idx);

    int16_t __cdecl Owned_Officer_Level_(int leader_idx);

    int32_t __cdecl Officer_Skill_Bonus_(uint32_t param_1, int16_t leader_idx, int16_t skill_idx);

    int16_t __cdecl Officer_Skill_Value_(int16_t leader_index);

    bool __cdecl Officer_Has_General_Skill_(int16_t player_num, int16_t leader_index, int16_t skill_id);

    bool __cdecl Officer_Has_Special_Skill_(int16_t player_num, int16_t leader_idx, int16_t skill_idx);

    int16_t __cdecl Get_Research_Cost_Skill_Value_(uint32_t cost);

    void __cdecl Get_Random_Skills_(uint32_t* primary_mask, uint32_t* secondary_mask, int16_t skill_class, int16_t count, int8_t randomize_class);

    void __cdecl Increase_Officer_Level_By_One_(int16_t leader_idx);

    void __cdecl Officers_Screen_();

    void __cdecl Deassign_Officer_(int16_t leader_idx);

    void __cdecl Move_Officer_To_Pool_(int16_t leader_idx);

    void __cdecl Allocate_Adv_Civ_Game_Officers_();

    bool __cdecl One_Leader_With_Galactic_Lore_(int16_t player_idx);

    int16_t __cdecl Officer_Maintenance_(int16_t player_idx, int16_t leader_idx);

    int32_t __cdecl Officer_Cost_(int16_t player_idx, int16_t leader_idx);

    bool Is_Fleet_Officer_Screen_();

    void __cdecl Draw_Officer_Screen_();

    void __cdecl Draw_Officer_Screen_To_Back_();

    void __cdecl Reload_Officer_Screen_Pictures_();

    void __cdecl Build_Captain_Id_List_();

    void __cdecl Get_Officer_Pictures_();

    s_animation_header* __cdecl Load_Darkened_Officer_Picture_(int16_t player_num, int16_t officer_id, void* buffer, uint8_t load_now);

    void __cdecl Load_Officer_Skill_Name_Descriptions_();

    void __cdecl Reload_Offscrn_Small_Ship_Icons_();

    void __cdecl Add_Officer_Screen_Fields_(
        int16_t* hire_btn,
        int16_t* cancel_hire_btn,
        int16_t* ship_ldrs_btn,
        int16_t* colony_ldrs_btn,
        int16_t* return_btn,
        int16_t* help_field_1,
        int16_t* help_field_2,
        int16_t* up_btn,
        int16_t* dn_btn,
        int16_t* next_btn,
        int16_t* prev_btn,
        int16_t* big_icon_fields
    );

    void __cdecl Initialize_Scroll_Bar_(s_scroll_bar* sb);

    void __cdecl Fade_Into_Leaders_Screen_();

    void __cdecl Already_Hired_Message_(int16_t officer_id);

    void __cdecl Not_Enough_BCs_Message_(int16_t leader_idx, int16_t cost);

    uint8_t __cdecl Ask_For_Hire_(int16_t leader_idx);

    int16_t __cdecl Do_Hire_Officer_(int16_t leader_idx);

    int16_t __cdecl Assign_Loknar_Ship_To_Player_(int16_t player_idx);

    int8_t __cdecl Officer_Made_Level_Report_(int16_t player_idx, int8_t process_popups);
    void __cdecl Check_For_Officer_Level_();
    void __cdecl Decrement_Officer_ETA_();
    void __cdecl Random_Officer_Check_(int16_t player_idx);
    void __cdecl Generate_Random_Officer_(int16_t player_index);
    void __cdecl Assert_Marooned_Leaders_();

    void __cdecl Kill_Officer_(int16_t leader_idx);

    void __cdecl Give_Officer_XP_(int16_t player_idx, int16_t leader_idx, int16_t xp_amount);

    void __cdecl Check_Officer_Fields_(int16_t input, int16_t scanned, short* p3, short* p4);

    inline int16_t __cdecl Scan_Officer_Fields_(int16_t* out_p1, int16_t* out_p2, int16_t p2, int16_t p1, int8_t* scanned_flag, int16_t* out_field2, int16_t* out_field1) {
        using Fn = int16_t(__cdecl*)(int16_t*, int16_t*, int16_t, int16_t, int8_t*, int16_t*, int16_t*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00441df0})(out_p1, out_p2, p2, p1, scanned_flag, out_field2, out_field1);
    }

    uint32_t __cdecl Scan_Offscrn_Big_Icon_Fields_(int16_t param_1, int16_t param_2, int16_t* param_3, int16_t* param_4);

    int16_t __cdecl Officer_Screen_User_Box_(const char* message, int16_t type);

    void __cdecl Dismiss_Officer_(int16_t leader_idx);

    void __cdecl Assign_Officer_(int16_t leader_idx, int16_t ship_idx);

    int8_t __cdecl Get_Ship_Leader_ETA_(int16_t player_num, int16_t leader_idx, int16_t ship_idx);

    int8_t __cdecl Get_Star_Leader_ETA_(int16_t player_num, int16_t leader_idx, int16_t star_idx);

    void __cdecl Assign_Leader_To_Ship_(s_player* player, s_leader_data* leader, s_ship_data* ship, int8_t eta);

    void __cdecl Ai_Assign_Leader_To_Ship_(s_player* player, s_leader_data* leader, s_ship_data* ship);

    bool __cdecl Leader_Can_Be_Assigned_(int16_t leader_idx);

    char* __cdecl Leader_Name_(int16_t leader_idx, char include_title, void* star_ptr = nullptr);

    void __cdecl Print_ETA_On_Officer_Picture_(int16_t leader_idx, int16_t x, int16_t y, int16_t w);

    inline char* (&_officer_skill_description_string)[27] =
        *reinterpret_cast<char*(*)[27]>(std::uintptr_t{0x005656D0});

    inline char* (&_officer_skill_name_string)[27] =
        *reinterpret_cast<char*(*)[27]>(std::uintptr_t{0x00565748});

    void __cdecl Text_Box_With_Refresh_(const char* title, const char* body);

    void __cdecl Get_Skill_Description_LBX_Name_(char* name);

    void __cdecl Print_Officer_Skill_Help_(int16_t skill_id, int16_t bonus, int16_t leader_idx);

    void __cdecl Find_Selected_Leader_(int16_t leader_idx);

    void __cdecl Give_Player_Loknar_Special_(int16_t player_idx, uint8_t* techs, int16_t* hero_id);

    void __cdecl Get_Available_Antaran_Tech_(int16_t player_idx, int16_t* available_techs, int16_t* available_count);

    void __cdecl Get_Random_Antaran_Tech_(int16_t player_idx, int16_t count, int16_t* tech_list, int16_t* num_techs);

    void __cdecl Rescued_Loknar_Popup_(int16_t param_1, int16_t* param_2, int16_t param_3);

    void __cdecl Make_Loknar_Message_(char* output, const int16_t* tech_app_ids, int16_t tech_app_count);

    int16_t __cdecl N_Fleet_Officers_Owned_By_Player_(int16_t player_idx);

    int16_t __cdecl N_Star_Officers_Owned_By_Player_(int16_t player_idx);

    int16_t __cdecl Chance_To_Hire_Hero_(int16_t player_idx);

    bool __cdecl Player_Can_Hire_More_Officers_(int16_t player_idx);

    void __cdecl Deselect_Offpop_Icons_();
    void __cdecl Move_Officer_To_Limbo_(int16_t officer_idx);
    void __cdecl Change_Screen_Type_(int8_t* out_flag);

    void __cdecl Wrapper_For_Offscrn_System_Popup_(int16_t system_id);

    void __cdecl Print_Galmap_Scanned_Ship_(int16_t icon_idx);

    void __cdecl Print_Scanned_Ship_Name_();

    inline void __cdecl Print_Officer_Data_(int16_t p1, int16_t p2, int32_t p3, int32_t p4, int32_t p5, int32_t p6, int32_t p7, int32_t p8, int32_t p9, int32_t p10) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t, int32_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004407E0})(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
    }

    void __cdecl Wrapper_For_Offscrn_Data_(int16_t param_1, int16_t param_2);

    bool __cdecl Leaders_In_Hiring_Pool_();

    void __cdecl Get_Officer_Costs_(int16_t officer_idx, int16_t* out_cost, int16_t* out_maintenance);

    void __cdecl Set_Officer_To_Player_(int16_t officer_idx, int16_t player_idx, uint8_t is_instant);

    void __cdecl Give_Leader_Specials_To_Player_(int16_t officer_idx, int16_t player_idx);

    void __cdecl Hire_Officer_(int16_t player_idx, int16_t leader_idx);

    void __cdecl Check_For_Colony_Calculation_Update_();

    void __cdecl Move_From_Limbo_To_Pool_();

    void __cdecl Save_Officers_Original_Colony_Id_();

    void __cdecl Save_Officers_Original_Ship_Id_();

    void __cdecl Draw_Colony_Box_();

    void __cdecl Draw_Fleet_Box_();

    void __cdecl Assign_Leader_To_Star_(s_player* player, s_leader_data* leader, s_star_data* star, int8_t eta);

    bool __cdecl Ships_In_Same_Place_(int16_t ship_index_1, int16_t ship_index_2);

    void __cdecl Load_Fleet_Galaxy_Stars_();

    void __cdecl Load_Officer_Skill_Icons_(void* param_1);

    uint8_t __cdecl Hire_Officer_Popup_(int16_t player_num, int16_t officer_id, char* message);

    void __cdecl Decrement_Officer_ETA_();

    int16_t __cdecl Select_Leader_For_Hire_(int16_t player_index);

    int8_t __cdecl Leader_Available_For_Hire_(int16_t leader_idx);

    bool __cdecl Officer_Status_Ok_(int16_t officer_index);

    int32_t __cdecl Calculate_Officer_Cost_(int16_t player_idx, int16_t leader_idx);

    bool __cdecl Marooned_Hero_At_Star_Can_Join_(int16_t star_idx, int16_t* hero_idx, int16_t player_idx);

    void __cdecl Ai_Assign_Leader_To_Star_(s_player* player, s_leader_data* leader, s_star_data* star);

    inline void __cdecl Wrapper_For_Offscrn_Help_Fields_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004423B0})();
    }

    void __cdecl Add_Display_Popup_Fields_(int16_t star_displayed);

    bool __cdecl Officer_Is_Female_(int16_t officer_idx);

    inline char (&s__005510f0)[2] =
        *reinterpret_cast<char(*)[2]>(std::uintptr_t{0x005510F0}); /* " " */

    void __cdecl Get_Italian_The_(int16_t leader_idx, char* dest);

    inline void __cdecl Wrapper_For_Officer_Popup_Help_Fields_(int16_t input, int16_t officer_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004422E0});
        fn(input, officer_idx);
    }

    inline void __cdecl Wrapper_For_Colony_Officer_Popup_Data_(int32_t x, int32_t y, int32_t officer_idx) {
        using Fn = void(__cdecl*)(int32_t, int32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00442290});
        fn(x, y, officer_idx);
    }

    inline void __cdecl Wrapper_To_Add_Officer_Popup_Help_Fields_(int32_t x, int32_t y, uint16_t officer) {
        using Fn = void(__cdecl*)(int32_t, int32_t, uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00442690});
        fn(x, y, officer);
    }
}