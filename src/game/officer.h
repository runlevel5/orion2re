#pragma once

namespace OFFICER {
    inline s_colors& _normal_text_colors = *reinterpret_cast<s_colors*>(std::uintptr_t{0x005569A0});
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
    inline uint32_t& _prev_leaders = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00565740});
    inline void*& _ldrpict = *reinterpret_cast<void**>(std::uintptr_t{0x005657B4});

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

    void __cdecl Allocate_Adv_Civ_Game_Officers_();

    bool __cdecl One_Leader_With_Galactic_Lore_(int16_t player_idx);

    int16_t __cdecl Officer_Maintenance_(int16_t player_num, int16_t leader_idx);

    int32_t __cdecl Officer_Cost_(int16_t player_idx, int16_t leader_idx);

    bool Is_Fleet_Officer_Screen_();

    void __cdecl Draw_Officer_Screen_();

    void __cdecl Draw_Officer_Screen_To_Back_();

    inline void __cdecl Reload_Officer_Screen_Pictures_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004415E0})();
    }
    inline void __cdecl Add_Officer_Screen_Fields_(int16_t* p1, int16_t* p2, int16_t* p3, int16_t* p4, int16_t* p5, int p6, int p7, int16_t* p8, int16_t* p9, int16_t* p10, int16_t* p11, int16_t* p12) {
        using Fn = void(__cdecl*)(int16_t*, int16_t*, int16_t*, int16_t*, int16_t*, int, int, int16_t*, int16_t*, int16_t*, int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0043F440})(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
    }
    void __cdecl Fade_Into_Leaders_Screen_();

    inline void __cdecl Already_Hired_Message_(int16_t leader_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00443430})(leader_idx);
    }

    inline void __cdecl Not_Enough_BCs_Message_(int16_t leader_idx, int32_t cost) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004434c0})(leader_idx, static_cast<int16_t>(cost));
    }

    inline uint8_t __cdecl Ask_For_Hire_(int16_t leader_idx) {
        using Fn = uint8_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00443580})(leader_idx);
    }

    int16_t __cdecl Do_Hire_Officer_(int16_t leader_idx);

    void __cdecl Check_Officer_Fields_(int16_t input, int16_t scanned, short* p3, short* p4);

    inline int16_t __cdecl Scan_Officer_Fields_(int16_t* out_p1, int16_t* out_p2, int16_t p2, int16_t p1, int8_t* scanned_flag, int16_t* out_field2, int16_t* out_field1) {
        using Fn = int16_t(__cdecl*)(int16_t*, int16_t*, int16_t, int16_t, int8_t*, int16_t*, int16_t*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00441df0})(out_p1, out_p2, p2, p1, scanned_flag, out_field2, out_field1);
    }

    inline int __cdecl Scan_Offscrn_Big_Icon_Fields_(int16_t scanned, int16_t input, int16_t* out_scanned, uint16_t* out_idx) {
        using Fn = int(__cdecl*)(int16_t, int16_t, int16_t*, uint16_t*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00441F90})(scanned, input, out_scanned, out_idx);
    }

    int16_t __cdecl Officer_Screen_User_Box_(const char* message, int16_t type);

    void __cdecl Dismiss_Officer_(int16_t leader_idx);

    inline void __cdecl Assign_Officer_(int16_t officer_idx, int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00444080})(officer_idx, ship_idx);
    }

    bool __cdecl Leader_Can_Be_Assigned_(int16_t leader_idx);

    inline char* __cdecl Leader_Name_(int16_t leader_idx, int8_t flag, void* star_ptr = nullptr) {
        using Fn = char*(__cdecl*)(int16_t, int8_t, void*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x00442710})(leader_idx, flag, star_ptr);
    }

    inline void __cdecl Print_Officer_Skill_Help_(int16_t leader_idx, int16_t skill_id, int16_t bonus) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004412c0})(leader_idx, skill_id, bonus);
    }

    inline void __cdecl Find_Selected_Leader_(int16_t leader_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00442f90})(leader_idx);
    }
    inline void __cdecl Reset_Officer_Selection_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0043D750})();
    }
    inline void __cdecl Move_Officer_To_Limbo_(int16_t officer_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0043FF00})(officer_idx);
    }
    inline void __cdecl Change_Screen_Type_(char* p1) {
        using Fn = void(__cdecl*)(char*);
        reinterpret_cast<Fn>(std::uintptr_t{0x00440040})(p1);
    }

    inline void __cdecl Wrapper_For_Offscrn_System_Popup_(int16_t star_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00442640})(star_idx);
    }

    inline void __cdecl Print_Galmap_Scanned_Ship_(int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00440340})(ship_idx);
    }

    inline void __cdecl Print_Scanned_Ship_Name_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004414a0})();
    }

    inline void __cdecl Wrapper_For_Offscrn_Data_(int16_t slot, int16_t officer_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x00442370})(slot, officer_idx);
    }

    inline int8_t __cdecl Leaders_In_Hiring_Pool_() {
        using Fn = int8_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x0043e1d0})();
    }

    inline void __cdecl Get_Officer_Costs_(int16_t officer_idx, int16_t* hire, int16_t* maint) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0043df20})(officer_idx, hire, maint);
    }

    void __cdecl Check_For_Colony_Calculation_Update_();

    void __cdecl Move_From_Limbo_To_Pool_();

    void __cdecl Set_Original_Stars_();

    void __cdecl Set_Original_Ships_();

    void __cdecl Draw_Colony_Box_();

    void __cdecl Draw_Fleet_Box_();
}