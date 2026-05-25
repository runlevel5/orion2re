#pragma once
#include <cstdint>

struct s_ship_design;
struct s_ship_data;
struct s_animation_header;
struct s_ship_icon;
struct s_colors;

namespace SHIPS {
    int16_t __cdecl Build_Nonplayer_Ship_(int16_t type, int16_t system_idx, uint8_t owner);

    void __cdecl Build_Ship_Icons_();

    int __cdecl qsort_ship_icon_(const void* a, const void* b);

    void __cdecl Set_Ship_Icon_XYs_();

    void __cdecl Draw_Directional_Multi_Colored_Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, s_colors* colors, int16_t phase);

    void __cdecl Get_Ship_Icon_Coords_(int16_t ship_idx, int16_t* x, int16_t* y);

    void __cdecl Get_Ship_Icon_Coords_In_Space_(int16_t icon_id, int16_t* x, int16_t* y);

    void __cdecl Get_XYs_For_Orbiting_Ships_(int16_t star_idx, int16_t ship_icon_idx, int16_t* out_x1, int16_t* out_y1, int16_t* out_x2, int16_t* out_y2, int16_t* out_arr_x, int16_t* out_arr_y);

    void __cdecl Get_Ship_Icon_Dimensions_(int16_t zoom_level, uint8_t* width, uint8_t* height);

    s_animation_header* __cdecl Get_Ship_Icon_Pict_Seg_(int16_t ship_type_idx, uint8_t is_highlighted);

    void __cdecl Draw_Ship_Destination_Line_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, s_colors* colors);

    void __cdecl Print_Eta_On_Ship_Icon_(int16_t node_idx, int16_t style_toggle, int16_t x_offset, int16_t y_offset);

    void __cdecl Do_Ship_Destination_Lines_();

    int16_t __cdecl Overlapped_Ship_Icon_Button_(int16_t* ship_icon_id, int16_t threshold_x, int16_t threshold_y);

    int16_t __cdecl Set_Nonplayer_Ship_XY_(int16_t system_idx, int16_t type, s_ship_design* design, uint8_t randomize_xy);

    int16_t __cdecl Set_Nonplayer_Ship_Destination_(int16_t star_idx, int16_t ship_idx, uint8_t is_moving);

    void __cdecl Kill_Ship_(int16_t ship_idx);

    void __cdecl Clear_Auto_Colonization_Flag_(s_ship_data* ship);

    void __cdecl Clear_All_Ships_Auto_Colonization_Flag_(int16_t* ship_list);

    bool __cdecl Enemy_Ship_Heading_Toward_Our_Colony_(int16_t node_idx);
    int16_t __cdecl Deploy_Ships_To_Attack_Antares_();
}