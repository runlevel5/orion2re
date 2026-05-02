#pragma once
#include <cstdint>

#pragma pack(push, 1)
struct s_window_status_and_height {
    uint16_t window_status;
    int16_t fleet_box_inner_height;
};
#pragma pack(pop)

struct s_fleet_box;

namespace FLEETPOP {
    inline s_fleet_box& _fleet_box =
        *reinterpret_cast<s_fleet_box*>(std::uintptr_t{0x005977C0});

    inline int16_t& _fleet_box_count =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005977CC});

    inline int8_t& _fleet_box_attack_btn_active =
        *reinterpret_cast<int8_t*>(std::uintptr_t{0x005977DC});

    inline int8_t& _fleet_box_engage_btn_active =
        *reinterpret_cast<int8_t*>(std::uintptr_t{0x005977DD});

    inline uint8_t __cdecl Set_Attack_Antares_Button_Flag_() {
        using Fn = uint8_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00405C40});
        return fn();
    }

    int8_t __cdecl Set_Transport_Troops_Flag_(int16_t ship_count);

    int8_t __cdecl Set_Colonize_Button_Flag_(int16_t ship_count);

    uint8_t __cdecl Set_Build_Outpost_Button_Flag_(int16_t ship_count);

    inline uint8_t __cdecl Can_Build_Outpost_(int16_t planet_idx) {
        using Fn = uint8_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00405EE0});
        return fn(planet_idx);
    }

    inline uint8_t __cdecl Set_Engage_Enemy_Forces_Flag_(int16_t fleet_count) {
        using Fn = uint8_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00406100});
        return fn(fleet_count);
    }

    inline int8_t __cdecl Planet_Is_Colonizable_(int16_t planet_idx) {
        using Fn = int8_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004060A0});
        return fn(planet_idx);
    }

    inline const char* s_BUFFER0_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x00551124}); // "BUFFER0.LBX"

    inline int16_t& _fleet_box_width =
            *reinterpret_cast<int16_t*>(std::uintptr_t{0x005977C6});

    inline int16_t& _fleet_box_field8_0xe =
            *reinterpret_cast<int16_t*>(std::uintptr_t{0x005977CE});

    void __cdecl Reload_Fleet_Box_Data_(void);

    bool __cdecl Ship_Should_Be_Auto_Selected_(int16_t ship_idx);

    int16_t __cdecl N_Fleet_Icons_(int16_t node_idx);

    void __cdecl Initialize_Scroll_Bar_(s_scroll_bar* scroll_bar, int16_t total_rows);

    void __cdecl Buffer_Reload_Fleetpop_Frame_();

    inline s_animation_header*& _fleet_box_name_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005977D2});

    inline s_animation_header*& _fleet_box_bottom_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x005977D6});

    inline int8_t& _fleet_box_is_small_frame =
        *reinterpret_cast<int8_t*>(std::uintptr_t{0x005977DA});

    void __cdecl Draw_Large_Fleet_Box_Frame_(
        int16_t x,
        int16_t y,
        int16_t x_offset,
        int16_t y_offset,
        uint32_t unused_param_5,
        int16_t* current_y);

    void __cdecl Draw_Small_Fleet_Box_Frame_(
        int16_t x,
        int16_t y,
        int16_t x_offset,
        int16_t y_offset,
        int16_t fleet_count,
        int16_t* current_y);

    void __cdecl Draw_Fleet_Pop_Added_Buttons_(int16_t x, int16_t y);

    void __cdecl Get_Fleet_Icon_Button_XY_(int16_t fleet_icon_index, int16_t* out_x, int16_t* out_y);

    void __cdecl Print_Fleet_Box_Info_();

    void __cdecl Get_Absolute_Location_Scan_Info_(
        int16_t ship_idx,
        int16_t* value,
        int16_t* star_idx,
        int16_t* info_type);

    void __cdecl Get_Fleetpop_Scanned_Ship_Info_(char* out_text);

    inline uint8_t& _n_added_buttons =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00564734});

    bool __cdecl Check_For_Colonization_Button_();

    bool __cdecl Check_For_Transport_Troops_Button_();

    bool __cdecl Check_For_Build_Outpost_Button_();

    bool __cdecl Check_For_Engage_Enemy_Forces_Button_();

    bool __cdecl Check_For_Attack_Antares_Button_();

    void __cdecl Set_Fleet_Box_Data_Dimensions_(void);

    void __cdecl Set_Added_Button_Stuff_();

    void __cdecl Fleet_Box_Data_(int16_t icon_count);

    int16_t __cdecl Fleet_Buttons_Chosen_();
    bool __cdecl At_Least_One_Ship_Of_Type_Selected_(int16_t ship_type);

    void __cdecl Set_Fleet_Movement_Icon_Selections_(int16_t state, uint8_t auto_select_flag);

    bool __cdecl Some_Icon_Selected_();

    void __cdecl Reload_Ship_Selection_Status_();

    int16_t __cdecl First_Unselected_Icon_();

    void __cdecl Save_Ship_Selection_Status_();

    int16_t __cdecl Fleet_Box_Grid_Height_(int16_t grid_type);
    int16_t __cdecl Fleet_Box_Width_(int16_t count);
    int16_t __cdecl Fleet_Box_Height_(int16_t fleet_count);

    void __cdecl Set_Fleet_Movement_Box_Dimensions_(int16_t count);

    bool __cdecl Can_Select_Fleet_Box_Ship_(int16_t index);

    int16_t __cdecl Icon_Selection_Status_(int16_t icon_field_id);

    void __cdecl Clear_Ship_Node_Selection_Flags_();

    void __cdecl Reload_Fleet_Box_Palette_(int16_t player_idx, uint32_t is_not_player);

    void __cdecl Draw_Damage_Bars_(int16_t x, int16_t y, int16_t width, int16_t y_offset, int16_t x_offset, int16_t ship_idx);

    void __cdecl Add_Added_Buttons_(int16_t x_offset, int16_t y_offset);

    void __cdecl Add_Fleet_Movement_Box_Fields_(int16_t ship_icon, int16_t flag);

    void __cdecl Draw_Fleet_Movement_Box_();
}