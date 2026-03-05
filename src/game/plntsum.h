#pragma once

namespace PLNTSUM {
    inline const char* s__00551100 = reinterpret_cast<const char*>(std::uintptr_t{0x00551100}); // ""
    inline const char* s_plus = reinterpret_cast<const char*>(std::uintptr_t{0x00551118}); // "+"
    inline const char* s_minus = reinterpret_cast<const char*>(std::uintptr_t{0x0055111C}); // "-"
    inline const char* s_space = reinterpret_cast<const char*>(std::uintptr_t{0x00551120}); // " "
    inline const char* s_C = reinterpret_cast<const char*>(std::uintptr_t{0x00552420}); // "C"
    inline const char* s_O = reinterpret_cast<const char*>(std::uintptr_t{0x00556AFC}); // "O"
    inline const char* s_1 = reinterpret_cast<const char*>(std::uintptr_t{0x00556B10}); // "1"
    inline const char* s_2 = reinterpret_cast<const char*>(std::uintptr_t{0x00556B0C}); // "2"
    inline const char* s_3 = reinterpret_cast<const char*>(std::uintptr_t{0x00556B08}); // "3"
    inline const char* s_4 = reinterpret_cast<const char*>(std::uintptr_t{0x00556B04}); // "4"
    inline const char* s_5 = reinterpret_cast<const char*>(std::uintptr_t{0x00556B00}); // "5"

    inline int16_t (&_plntscrn_delta_y)[8] = *reinterpret_cast<int16_t(*)[8]>(std::uintptr_t{0x00556A78});
    inline int16_t& _sort_choice = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556ACC});
    inline int16_t& _filter_out_adverse_gravity_planets = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556AD0});
    inline int16_t& _filter_out_hostile_environment_planets = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556AD4});
    inline int16_t& _filter_out_mineral_scarcity_planets = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556AD8});
    inline int16_t& _filter_out_enemy_controlled_planets = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556ADC});
    inline int16_t& _filter_out_planets_out_of_range = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00556AE0});

    inline s_plntsum_screen*& _screen = *reinterpret_cast<s_plntsum_screen**>(std::uintptr_t{0x0056582C});

    inline s_plntsum_planet*& _planet_summary =
        *reinterpret_cast<s_plntsum_planet**>(std::uintptr_t{0x005657E8});

    struct s_plntsum_ship {
        int16_t colony_ship_idx;
        int16_t outpost_ship_idx;
        int16_t ship_icon_id;
        bool colony_available;
        bool output_available;
    };

    inline s_plntsum_ship*& _ship_summary =
        *reinterpret_cast<s_plntsum_ship**>(std::uintptr_t{0x005658A0});

    inline uint16_t& _n_ships_in_summary = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00565898});
    inline uint32_t (&_can_reach_star)[18] = *reinterpret_cast<uint32_t(*)[18]>(std::uintptr_t{0x00565848});

    inline uint8_t (&_explored_bitmap)[45] =
        *reinterpret_cast<uint8_t(*)[45]>(std::uintptr_t{0x005657F8});

    void __cdecl Reset_Explored_Planets_();

    inline uint8_t& _found_outpost_ship =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00565828});

    inline uint8_t& _draw_colony_button_status =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00565890});

    inline int16_t& _send_choice =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565894});

    inline uint8_t& _found_colony_ship =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005658AC});

    inline uint8_t& _draw_outpost_button_status =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005658C4});

    inline uint16_t& _plntsum_scroll_bar =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00565844});

    void __cdecl Add_Planet_Summary_Bar_Field_();

    void __cdecl Add_Colony_Ship_Icon_Field_(int16_t* field_id, int16_t row_index);

    void __cdecl Add_Outpost_Ship_Icon_Field_(int16_t* field_id, int16_t row);

    void __cdecl Add_PlntSum_Galaxy_Map_Fields_(int16_t* fields_array);

    void __cdecl Add_Plntsum_Fields_();

    inline void __cdecl Planet_Summary_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004488e0})();
    }

    bool __cdecl Send_Colony_Button_Selectable_();
    bool __cdecl Send_Outpost_Button_Selectable_();
    bool __cdecl Planet_Is_Outpost_Planet_(short planet_idx);
    bool __cdecl Planet_Has_Players_Colony_(int16_t planet_idx);
    bool __cdecl Planet_Has_Been_Visited_(int16_t planet_index);
    bool __cdecl Planet_In_Range_Of_A_Colony_(short planet_index);

    int16_t __cdecl Productivity_Penalty_(int16_t player_idx, int16_t planet_gravity);

    int16_t __cdecl First_Planet_In_Summary_List_(int16_t star_index);

    int16_t __cdecl Summary_Planet_Index_(int16_t star_index);

    void __cdecl Set_Planet_Explored_Flags_();

    bool __cdecl Planet_Has_Adverse_Gravity_(int16_t planet_idx, int16_t player_idx);
    bool __cdecl Planet_Is_Enemy_Controlled_(int16_t planet_idx);
    bool __cdecl Planet_Has_Hostile_Environment_(short planet_idx);

    bool __cdecl Planet_Is_Mineral_Scarce_(short planet_idx);
    int16_t __cdecl Colony_Ship_Auto_Colonization_(s_ship_data* ship);
    bool __cdecl Ship_Is_Not_Colonizing_(s_ship_data* ship);

    inline int16_t __cdecl Planet_Summary_List_Id_(int16_t planet_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044b6c0});
        return fn(planet_idx);
    }

    inline void __cdecl Set_Colony_Ship_To_Star_Distances_(int16_t summary_idx, int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044ad00});
        fn(summary_idx, ship_idx);
    }

    inline void __cdecl Set_Outpost_Ship_To_Star_Distances_(int16_t summary_idx, int16_t ship_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044aed0});
        fn(summary_idx, ship_idx);
    }

    void __cdecl Update_Ship_Etas_();

    inline char*& PTR_s_PLNTSUM_LBX_00556AC0 =
        *reinterpret_cast<char**>(std::uintptr_t{0x00556AC0});

    inline uint32_t& _send_colony_ship_button_seg =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00556AC4});

    inline uint32_t& _send_outpost_ship_button_seg =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00556AC8});

    inline void __cdecl Load_Player_Icon_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00449f80});
        fn();
    }

    void __cdecl Load_Planet_Summary_Pictures_();

    void __cdecl Allocate_Space_For_Summary_Screen_();

    void __cdecl Determine_Button_Status_();

    void __cdecl Build_Ship_Summary_();

    inline int __cdecl qsort_by_climate_(const void* a, const void* b) {
        using Fn = int(__cdecl*)(const void*, const void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044B200});
        return fn(a, b);
    }

    inline int __cdecl qsort_by_minerals_(const void* a, const void* b) {
        using Fn = int(__cdecl*)(const void*, const void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044B220});
        return fn(a, b);
    }

    inline int __cdecl qsort_by_max_population_(const void* a, const void* b) {
        using Fn = int(__cdecl*)(const void*, const void*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044B240});
        return fn(a, b);
    }

    void __cdecl Sort_Planet_Summary_();
    void __cdecl Filter_Explored_Planets_(short* summary_count);
    int16_t __cdecl Planet_Summary_Id_(int16_t explored_id);

    inline int16_t __cdecl Nearest_Ship_Id_(int16_t planet_id) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044A6E0});
        return fn(planet_id);
    }

    inline void __cdecl Send_Nearest_Ship_(int16_t explored_idx) {
        using Fn = void(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044A730});
        fn(explored_idx);
    }

    inline void __cdecl Reset_Ship_To_Star_Distances_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044A600});
        fn();
    }

    inline void __cdecl Undo_Send_Order_(int16_t explored_idx, int16_t order_type) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0044B260});
        fn(explored_idx, order_type);
    }

    inline bool& _can_reach_scanned_planet =
        *reinterpret_cast<bool*>(std::uintptr_t{0x00565830});

    inline int8_t& _box_field_scanned =
        *reinterpret_cast<int8_t*>(std::uintptr_t{0x005658BC});

    void __cdecl Scan_Summary_Box_Fields_(int16_t input_field_id, int16_t clicked_field_id, int16_t* last_input_field_id_ptr);
}