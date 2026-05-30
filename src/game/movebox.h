#pragma once
#include <cstdint>

namespace MOVEBOX {
    extern s_movable_box _moveable_box[5];

    extern s_fleet_box _fleet_box;

    void __cdecl Clear_Moveable_Box_Fields_(int16_t box_index);

    int16_t __cdecl Moveable_Box_Selected_(int16_t index);
    uint16_t __cdecl Box_Field_Selected_(int16_t box_index, int16_t field_id);

    int8_t __cdecl Box_Was_Open_(int16_t box_idx);

    void __cdecl Vanish_Fleet_Movement_Box_();

    void __cdecl Vanish_System_Display_Box_();

    void __cdecl Save_And_Remove_Boxes_();

    void __cdecl Restore_Boxes_();

    void __cdecl Move_Box_To_Top_(int16_t box_id);

    bool __cdecl Mouse_Within_Fleet_Box_();

    void __cdecl Move_Moveable_Box_(int16_t moveable_box_idx, int16_t mouse_button_state);

    void __cdecl Draw_Moveable_Box_(int16_t box_id);

    void __cdecl Box_Fleet_Screen_Scanned_Star_();

    void __cdecl Box_Officer_Screen_Scanned_Star_();

    void __cdecl Box_Officer_Screen_Chosen_Star_(int16_t left, int16_t top, int16_t x_divisor, int16_t y_divisor);

    void __cdecl Box_Fleet_Screen_Scanned_Ship_();

    extern const char s__00551100[1];

    void __cdecl Do_Fleet_Screen_Stuff_(int16_t map_x_off, int16_t map_y_off, int32_t scale_x, int32_t scale_y);

    void __cdecl Do_Officer_Screen_Stuff_(int16_t map_x_off, int16_t map_y_off, int16_t scale_x, int16_t scale_y);

    void __cdecl Draw_Galaxy_Map_Box_(s_animation_header* anim1, s_animation_header* anim2,
                                    int16_t x_base, int16_t y_base,
                                    int16_t width, int16_t height,
                                    int16_t map_x_off, int16_t map_y_off,
                                    int16_t x_adj, int16_t y_adj,
                                    int16_t view_mode, int16_t exit_flag);

    void __cdecl Get_Galaxy_Map_Star_XY_(int16_t star_idx,
                                         int16_t base_x,
                                         int16_t base_y,
                                         int16_t map_width,
                                         int16_t map_height,
                                         int16_t offset_x,
                                         int16_t offset_y,
                                         int16_t star_marker_mode,
                                         int16_t* out_x,
                                         int16_t* out_y);

    void __cdecl Get_Galaxy_Map_Space_XY_(int16_t ship_icon_idx,
                                          int16_t origin_x,
                                          int16_t origin_y,
                                          int16_t x_zoom,
                                          int16_t y_zoom,
                                          int16_t* out_x,
                                          int16_t* out_y);

    void __cdecl Add_Galaxy_Map_Fields_2_(
            int16_t map_x,
            int16_t map_y,
            int16_t map_width,
            int16_t map_height,
            int16_t offset_x,
            int16_t offset_y,
            uint32_t unused_7,
            uint32_t unused_8,
            int16_t field_style,
            int16_t* out_special_field_id,
            int16_t* star_field_ids,
            int16_t* out_grid_field_id);

    void __cdecl Add_Whole_Window_Field_(int16_t moveable_box_index);

    void __cdecl Moveable_Box_Dimensions_(
        int16_t box_index,
        int16_t* left,
        int16_t* top,
        int16_t* right,
        int16_t* bottom);

    int16_t __cdecl N_Moveable_Boxes_Selected_(int16_t excluded_box_idx);

    void __cdecl Remove_Box_Check_();
}