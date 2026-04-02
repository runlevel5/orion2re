#pragma once

namespace HAROLD {
    uint32_t __cdecl Is_Colony_Star_(int16_t star_idx);

    int16_t __cdecl Get_Scaled_Value_(int16_t value);

    int32_t __cdecl Get_Up_Scaled_Value_(int16_t value);

    bool __cdecl Empty_System_(int16_t star_idx);

    bool __cdecl Is_Home_Star_(int16_t star_idx);

    uint8_t __cdecl N_Planets_In_System_(int16_t star_idx);

    int16_t __cdecl N_Colony_Owners_(int16_t star_idx);

    uint8_t __cdecl N_Satellites(int16_t star_index);

    void __cdecl Set_Wormhole_Id_(int16_t star_idx, uint8_t* star_bitmap);

    uint8_t __cdecl N_Food_Planets_In_System_(int16_t system_index);

    bool __cdecl Star_Is_Orion_(int16_t star_idx);

    bool __cdecl Star_Is_Outpost_Star_(int16_t star_idx);

    bool __cdecl Habitable_Spectral_Class_(int16_t star_idx);

    void __cdecl Get_Font_File_(char* buffer);
    void __cdecl Get_Officer_Data_LBX_Name_(char* out_name);

    void __cdecl Set_Sound_Volume_To_Settings_();

    void __cdecl Confirm_Planet_And_Star_Index_Link_(const char* msg);

    int8_t __cdecl Planet_Number_(int16_t planet_index);

    void __cdecl Set_All_Visited_Flags_();

    void __cdecl Init_Moving_Boxes_();

    const char* __cdecl H_Message_(int16_t id);

    inline int16_t __cdecl N_Player_Ships_(int16_t player_num) {
        using Fn = int16_t(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004f1330})(player_num);
    }

    void __cdecl Get_Monetary_Unit_String_(char* buffer);

    void __cdecl Randomize_List_(int16_t* list, int16_t size, int8_t use_clock);

    void __cdecl Save_Mouse_List_();

    inline s_mouse_list (&_saved_mouse_list)[10] =
        *reinterpret_cast<s_mouse_list(*)[10]>(std::uintptr_t{0x00571A40});

    inline uint32_t& _saved_mouse_list_offset_4 =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00571A44});

    inline int16_t& _saved_mouse_list_count =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00571AB8});

    void __cdecl Init_Fleet_Movement_Box_();

    void __cdecl Init_System_Display_Box_();

    inline void __cdecl Planet_Data_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004f1420})();
    }

    inline void __cdecl Load_Language_Fonts_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004F24A0});
        fn();
    }

    uint8_t* __cdecl Harold_Init_();

    inline void __cdecl Harold_Defaults_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004f0aa0});
        fn();
    }

    void __cdecl Set_Player_Font_Style_(int16_t player_idx, bool param_2);

    int8_t __cdecl Map_Scale_To_Zoom_Level_();

    void __cdecl Decrement_Zoom_Count_();

    int16_t __cdecl Increment_Zoom_Count_With_Limit_();

    void __cdecl Increment_Zoom_Count_();

    int16_t __cdecl Get_Map_Scale_(int16_t layer_type);

    inline void __cdecl Draw_Multi_Colored_Box_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t* colors, uint32_t color_inc) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, uint8_t*, uint32_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004F1FB0 })(x1, y1, x2, y2, colors, color_inc);
    }

    int16_t __cdecl Map_Scale_Star_Size_To_Zoom_Level_(int16_t star_idx);

    int16_t __cdecl Galaxy_Size_From_N_Stars_();

    void __cdecl Restore_Mouse_List_();

    inline bool __cdecl Was_Mouse_List_Saved_() {
        using Fn = bool(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x004F2200})();
    }

    bool __cdecl Player_Sighted_Monster_(int16_t player_id, int16_t star_id);

    void __cdecl Load_Leader_Titles();

    inline void Load_H_Strings_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004f2210});
        fn();
    }

    inline short __cdecl User_Box_(uint8_t* screen_ptr, const char* message, int16_t type) {
        using Fn = short(__cdecl*)(uint8_t*, const char*, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004F1B50});
        return fn(screen_ptr, message, type);
    }

    int16_t __cdecl Get_N_Remaining_Humans_();

    int16_t __cdecl Star_Owner_(int16_t star_idx, int16_t player_idx);

    int16_t __cdecl Absolute_Location_(int16_t location);

    int16_t __cdecl Actual_Star_Id_(int16_t star_id, int16_t* out_actual_id);

    int16_t __cdecl XY_To_Star_Id_(int16_t x, int16_t y);

    int16_t __cdecl N_Turns_To_Star_(int16_t ship_id, int16_t star_id);

    bool __cdecl Player_Is_Omniscient_(int16_t player_idx);

    inline uint8_t& _set_fade_for_first_fltpop = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00595394});

    int16_t __cdecl Zoom_Level_Font_Style_();

    void __cdecl Get_Player_Font_Colors_(int16_t player_idx, int16_t style_idx, s_colors* style_buf, bool param_2);

    bool __cdecl Contact_With_Player_(int16_t player_idx, int16_t target_idx);

    char __cdecl Contact_With_One_Colony_(int16_t star_idx);

    bool __cdecl Player_Has_Stargate_(int16_t player_idx, int16_t star_idx);

    bool __cdecl Player_Has_Jumpgate_(int16_t player_idx, int16_t star_idx);

    bool __cdecl Player_Has_Colony_(int16_t player_num, int16_t star_idx);

    bool __cdecl Player_Has_Outpost_(int16_t player_num, int16_t star_idx);

    bool __cdecl Player_Has_Visited_(int16_t player_idx, int16_t star_idx);

    bool __cdecl Player_Has_Artemis_Net_(s_player* player, s_star_data* star);

    bool __cdecl Player_Has_Dimensional_Portal_(s_player* player, s_star_data* star);

    int16_t __cdecl Planet_Type_(int16_t star_idx, int16_t star_planet);

    void __cdecl Colony_Owners_(int16_t star_idx, int16_t* player_counts, int16_t* total_count, bool have_contact);

    int16_t __cdecl Players_Home_Star_(int16_t star_idx);

    int16_t __cdecl Planet_Index_(int16_t star_idx, int16_t planet_idx);

    inline const char* s_after_home_world_gen = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575D4 }); // "after home world gen"

    inline const char* s___s__00556ae4 = reinterpret_cast<const char*>(0x00556AE4); // "(%s)"
    inline const char* s_relate_game_conditions_EXACTLY_a_0055a6cc = reinterpret_cast<const char*>(0x0055A6CC); // "relate game conditions EXACTLY as they occurred to technical support"
    inline const char* s_orbit__d_of_star__d_has_more_tha_0055a704 = reinterpret_cast<const char*>(0x0055A704); // "orbit %d of star %d has more than one planet linked to it"

    inline const char* s_HERODATI_LBX = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055A808 }); // "HERODATI.LBX"
    inline const char* s_HERODATS_LBX = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055A818 }); // "HERODATS.LBX"
    inline const char* s_HERODATF_LBX = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055A828 }); // "HERODATF.LBX"
    inline const char* s_HERODATG_LBX = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055A838 }); // "HERODATG.LBX"
    inline const char* s_HERODATA_LBX = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055A848 }); // "HERODATA.LBX"

    inline const char* s_MC_0055823c = reinterpret_cast<const char*>(std::uintptr_t{0x0055823c}); // "MC"
    inline const char* s_BC_00558244 = reinterpret_cast<const char*>(std::uintptr_t{0x00558244}); // "BC"
    inline const char* s_in_next_turn = reinterpret_cast<const char*>(std::uintptr_t{0x00558004}); // "in next-turn"

    bool __cdecl Star_Guarded_By_Monster_(int16_t location_idx, int16_t* monster_owner);

    inline int16_t __cdecl First_Ship_Orbiting_Star_(s_star_data* star) {
        using Fn = int16_t(__cdecl*)(s_star_data*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004F1D00});
        return fn(star);
    }
}