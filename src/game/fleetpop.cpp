#include "pch.h"

namespace FLEETPOP {

    void __cdecl Draw_Damage_Bars_(int16_t x, int16_t y, int16_t width, int16_t y_offset, int16_t x_offset, int16_t ship_idx) {
        int16_t effective_width = width - 4;
        int16_t draw_x = x_offset + x;
        int16_t draw_y = y + y_offset - 5;

        int16_t max_hits = AIPOWER::Max_Ship_Hits_(ship_idx);
        int16_t current_hits = AIPOWER::Current_Ship_Hits_(ship_idx);

        int16_t green_width = (static_cast<int>(current_hits) * effective_width) / max_hits;
        if (green_width < 2) {
            green_width = 2;
        }

        int16_t red_width = effective_width - green_width;

        if (current_hits != max_hits) {
            if (red_width == 1) {
                red_width = 2;
                green_width = width - 6;
            }

            if (green_width > 0) {
                int16_t end_x = draw_x + green_width;
                int16_t bottom_y = draw_y + 4;

                line::Line_(draw_x, draw_y, end_x, draw_y, 0x70 /* 'p' */);
                line::Line_(draw_x, draw_y, draw_x, bottom_y, 0x70 /* 'p' */);
                line::Line_(end_x, draw_y + 1, end_x, bottom_y, 0x72 /* 'r' */);
                line::Line_(end_x, bottom_y, draw_x + 1, bottom_y, 0x72 /* 'r' */);
                graphics::Fill_(draw_x + 1, draw_y + 1, end_x - 1, draw_y + 3, 0x71 /* 'q' */);
            }

            if (red_width > 1) {
                int16_t start_red_x = draw_x + green_width + 1;
                int16_t end_red_x = start_red_x + red_width;
                int16_t bottom_y = draw_y + 4;

                line::Line_(start_red_x, draw_y, end_red_x, draw_y, 0x4B /* 'K' */);
                line::Line_(start_red_x, draw_y, start_red_x, bottom_y, 0x4B /* 'K' */);
                line::Line_(end_red_x, draw_y + 1, end_red_x, bottom_y, 0x4E /* 'N' */);
                line::Line_(end_red_x, bottom_y, start_red_x + 1, bottom_y, 0x4E /* 'N' */);
                graphics::Fill_(start_red_x + 1, draw_y + 1, end_red_x - 1, draw_y + 3, 0x4D /* 'M' */);
            }
        }
    }

    int16_t __cdecl Fleet_Box_Height_(int16_t fleet_count) {
        int16_t close_btn_height = animate::Get_Height_(MOX::_fleet_box_close_lg_button_seg);
        int16_t name_height;
        int16_t bottom_height;

        if (MOX::_fleet_box.is_small_frame == 0) {
            MOX::_fleet_box_name_lg_seg = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x33, MOX::_global_cache_seg);
            MOX::_fleet_box.name_seg = MOX::_fleet_box_name_lg_seg;
            name_height = animate::Get_Height_(MOX::_fleet_box.name_seg);

            MOX::_fleet_box.bottom_seg = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x37, MOX::_global_cache_seg);
            MOX::_fleet_box_bottom_lg_seg = MOX::_fleet_box.bottom_seg;
        }
        else {
            MOX::_fleet_box_name_sm_seg = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x34, MOX::_global_cache_seg);
            MOX::_fleet_box.name_seg = MOX::_fleet_box_name_sm_seg;
            name_height = animate::Get_Height_(MOX::_fleet_box.name_seg);

            MOX::_fleet_box.bottom_seg = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x38, MOX::_global_cache_seg);
            MOX::_fleet_box_bottom_sm_seg = MOX::_fleet_box.bottom_seg;
        }

        bottom_height = animate::Get_Height_(MOX::_fleet_box.bottom_seg);

        int16_t grid_height;
        if (fleet_count > 9) {
            MOX::_fleet_box_grid_lg_seg = (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x35, MOX::_global_cache_seg);
            grid_height = animate::Get_Height_(MOX::_fleet_box_grid_lg_seg);
        }
        else {
            int32_t rows = (static_cast<int32_t>(fleet_count) - 1) / 3 + 1;
            if (rows > 3) {
                rows = 3;
            }
            grid_height = FLEETPOP::Fleet_Box_Grid_Height_(static_cast<int16_t>(rows));
        }

        return grid_height + close_btn_height + bottom_height + name_height;
    }

void __cdecl Add_Fleet_Movement_Box_Fields_(int16_t ship_icon, int16_t flag) {
    int16_t local_a = -1;
    int16_t local_8 = -1;

    int16_t node_ptr = HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack);
    if (node_ptr == MOX::_first_node_ptr_in_stack_) {
        node_ptr = HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack);
        int16_t num_icons = N_Fleet_Icons_(node_ptr);
        Fleet_Box_Data_(num_icons);

        int16_t box_x = MOX::_moveable_box[2].x;
        int16_t box_y = MOX::_moveable_box[2].y;
        int16_t box_width = Fleet_Box_Width_(MOX::_fleet_box.count);

        MOX::_moveable_box[2].window_field_id = -1000;
        MOX::_moveable_box[2].drag_field = fields::Add_Hidden_Field_(
            box_x, box_y, box_width + box_x, box_y + 0x23, (char*)s__00551100, 0
        );

        int16_t end_idx = MOX::_fleet_icon_buttons_added + MOX::_first_fleet_movement_icon;
        if (MOX::_fleet_box.count < end_idx) {
            end_idx = MOX::_fleet_box.count;
        }

        for (int16_t i = MOX::_first_fleet_movement_icon; i < end_idx; i++) {
            int16_t icon_x = MOX::_fleet_movement_icon[i].x;
            int16_t icon_y = MOX::_fleet_movement_icon[i].y;

            if (local_a < 0) {
                local_a = icon_x;
            }
            if (local_8 < 0) {
                local_8 = icon_y;
            }

            HACCESS::Fleet_Movement_Ship_Id_(i);

            MOX::_fleet_movement_icon[i].field = fields::Add_Hidden_Field_(
                icon_x, icon_y, icon_x + 0x35, icon_y + 0x35, (char*)s__00551100, 0x29
            );
        }

        node_ptr = HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack);
        int16_t ship_idx = MOX::_ship_node[node_ptr].ship_idx;

        if (MOX::_moveable_box_order[0] == 2 && flag != 0 &&
            HACCESS::Ship_Owner_(ship_idx) == MOX::_PLAYER_NUM &&
            HACCESS::Player_Can_Order_Ship_(MOX::_PLAYER_NUM, &MOX::_ship[ship_idx])) {

            int16_t btn_x = box_x + 16;
            if (MOX::_fleet_box.is_small_frame == 1) {
                btn_x += 1;
            }

            MOX::_fleet_movement_all_button = fields::Add_Clipped_Button_Field_(
                btn_x, MOX::_fleet_box.field9_0x10 + box_y, (char*)s__00551100,
                MOX::_fleet_box_all_button_seg, (char*)s__00551100, 0x28
            );
        } else {
            MOX::_fleet_movement_all_button = -1000;
        }

        if (MOX::_fleet_box.count > 9) {
            int16_t up_btn = -1000;
            int16_t dn_btn = -1000;

            MOX::_fleet_box_scroll_up_button = -1000;
            MOX::_fleet_box_scroll_dn_button = -1000;

            if (flag != 0) {
                up_btn = fields::Add_Button_Field_(
                    box_x + 0xbf, box_y + 0x2a, (char*)s__00551100,
                    MOX::_fleet_box_scroll_up_button_seg, (char*)s_minus_0055111c, 0x28
                );
                MOX::_fleet_box_scroll_up_button = up_btn;

                dn_btn = fields::Add_Button_Field_(
                    box_x + 0xbf, box_y + 0xae, (char*)s__00551100,
                    MOX::_fleet_box_scroll_dn_button_seg, (char*)s_plus_00551118, 0x28
                );
            }

            MOX::_fleet_box_scroll_dn_button = dn_btn;
            MOX::_fltpop_scroll_bar.up_btn = up_btn;
            MOX::_fltpop_scroll_bar.dn_btn = dn_btn;

            MOX::_fleet_movement_bar_field = fields::Add_Scroll_Field_(
                box_x + 0xc0, box_y + 0x42, 0, 0x66, 0, 0x66, 10, 0x66,
                &MOX::_fltpop_scroll_bar.thumb_center_y, (char*)s__00551100, 0
            );
        }

        MOX::_fleet_box_outpost_button = -1000;
        MOX::_fleet_box_colonize_button = -1000;
        MOX::_fleet_box_engage_button = -1000;
        MOX::_fleet_box_transport_button = -1000;
        MOX::_fleet_box_attack_button = -1000;

        Add_Added_Buttons_(box_x, box_y);
        MOVEBOX::Add_Whole_Window_Field_(2);
    }
}

int16_t __cdecl Fleet_Box_Grid_Height_(int16_t grid_type) {
    if (grid_type == 1) {
        return 0x38;
    }

    if (grid_type == 2) {
        return 0x70;
    }

    if (grid_type == 3) {
        MOX::_fleet_box_grid_sm_seg4 = buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x36, MOX::_global_cache_seg);
        return animate::Get_Height_((s_animation_header*)MOX::_fleet_box_grid_sm_seg4);
    }

    return 0;
}

void __cdecl Set_Fleet_Movement_Box_Dimensions_(int16_t count) {
    int32_t rows = (static_cast<int32_t>(count) - 1) / 3 + 1;
    if (rows > 3) {
        rows = 3;
    }

    MOX::_fleet_box_map_height = Fleet_Box_Grid_Height_(static_cast<int16_t>(rows));
    MOX::_moveable_box[2].height = Fleet_Box_Height_(count);
}

int16_t __cdecl Fleet_Box_Width_(int16_t count) {
    return animate::Get_Width_(MOX::_fleet_box.name_seg);
}

/**
 * @address 00405990
 */
void __cdecl Reload_Fleet_Box_Data_(void) {
    if (HACCESS::Get_Stack_Start_From_Saved_Ship_Array_() == 0) {
        MOVEBOX::Vanish_Fleet_Movement_Box_();
        return;
    }

    int16_t node_ptr = HACCESS::Node_Ptr_From_Ship_Stack_(static_cast<int16_t>(MOX::_fleet_box_ship_stack));
    int16_t num_icons = FLEETPOP::N_Fleet_Icons_(node_ptr);
    FLEETPOP::Fleet_Box_Data_(num_icons);
}

void __cdecl Set_Fleet_Movement_Icon_Selections_(int16_t state, uint8_t auto_select_flag) {
    for (int16_t i = 0; i < MOX::_fleet_box.count; ++i) {
        int16_t node_idx = MOX::_fleet_movement_icon[i].ship_node_idx;
        s_ship_node& node = MOX::_ship_node[node_idx];
        int16_t ship_idx = node.ship_idx;

        if (HACCESS::Ship_Owner_(ship_idx) == MOX::_PLAYER_NUM) {
            if (state == 0) {
                node.selected = 0;
            } else if (state == 1) {
                if (auto_select_flag != 0) {
                    if (FLEETPOP::Ship_Should_Be_Auto_Selected_(ship_idx)) {
                        node.selected = 1;
                    }
                } else {
                    if (MAINSCR::Ship_Can_Be_Selected_(ship_idx)) {
                        node.selected = 1;
                    }
                }
            }
        }
    }

    MOX::_fleet_movement_icons_on = static_cast<uint8_t>(state);
}

/**
 * @address 00406700
 */
bool __cdecl Some_Icon_Selected_() {
    int16_t selected_ship_ids[500];

    if (MOVEBOX::Moveable_Box_Selected_(2)) {
        int16_t count = HACCESS::Get_Fleet_Box_Selected_Ship_Ids_(selected_ship_ids, 500, 255);
        return count > 0;
    }

    return false;
}

/**
 * @address 004059e0
 */
void __cdecl Reload_Ship_Selection_Status_() {
    int16_t node_ptr = HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack);
    int16_t stack_owner = HACCESS::Ship_Stack_Owner_(node_ptr);

    if (static_cast<int16_t>(MOX::_PLAYER_NUM) == stack_owner) {
        for (int16_t i = 0; i < 500; ++i) {
            int16_t ship_idx = MOX::_ship_node[i].ship_idx;

            bool is_selectable = false;
            if (struct_::Test_Bit_Field_(&MOX::_black_hole_anim_bitfield, ship_idx)) {
                if (MAINSCR::Ship_Can_Be_Selected_(ship_idx)) {
                    is_selectable = true;
                }
            }

            MOX::_ship_node[i].selected = is_selectable ? 1 : 0;
        }
    }
}

/**
 * @address 00404cc0
 */
int16_t __cdecl First_Unselected_Icon_() {
    int16_t first_unselected_index = 0;
    bool found = false;

    if (MOX::_fleet_box.count > 0) {
        for (int16_t i = 0; i < MOX::_fleet_box.count; ++i) {
            if (found) {
                break;
            }

            if (!HACCESS::Fleet_Movement_Icon_Selected_(i)) {
                first_unselected_index = i;
                found = true;
            }
        }
    }

    return first_unselected_index;
}

/**
 * @address 00405a60
 */
void __cdecl Save_Ship_Selection_Status_() {
    int16_t node_ptr = HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack);
    int16_t owner = HACCESS::Ship_Stack_Owner_(node_ptr);

    if (MOX::_PLAYER_NUM == owner) {
        for (int16_t i = 0; i < MOX::_fleet_box.count; ++i) {
            int16_t ship_node_idx = MOX::_fleet_movement_icon[i].ship_node_idx;

            if (ship_node_idx != -1) {
                s_ship_node& node = MOX::_ship_node[ship_node_idx];

                if (node.selected == 0) {
                    struct_::Clear_Bit_Field_(&MOX::_black_hole_anim_bitfield, node.ship_idx);
                } else {
                    struct_::Set_Bit_Field_(&MOX::_black_hole_anim_bitfield, node.ship_idx);
                }
            }
        }
    }
}

/**
 * @address 00405070
 */
int16_t __cdecl Fleet_Buttons_Chosen_() {
    int16_t selected_count = 0;
    int16_t i = 0;

    if (MOX::_fleet_box.count > 0) {
        do {
            int16_t node_idx = MOX::_fleet_movement_icon[i].ship_node_idx;

            if (MOX::_ship_node[node_idx].selected != 0) {
                selected_count++;
            }

            i++;
        } while (i < MOX::_fleet_box.count);
    }

    return selected_count;
}

/**
 * @address 00406750
 */
bool __cdecl Can_Select_Fleet_Box_Ship_(int16_t index) {
    int16_t ship_node_idx = MOX::_fleet_movement_icon[index].ship_node_idx;
    int16_t owner_id = HACCESS::Ship_Stack_Owner_(ship_node_idx);

    return owner_id == MOX::_PLAYER_NUM;
}

/**
 * @address 00406610
 */
int16_t __cdecl Icon_Selection_Status_(int16_t icon_field_id) {
    int16_t first_icon_idx = static_cast<int16_t>(MOX::_first_fleet_movement_icon);
    int16_t limit_idx = MOX::_fleet_icon_buttons_added + first_icon_idx;

    if (limit_idx > MOX::_fleet_box.count) {
        limit_idx = MOX::_fleet_box.count;
    }

    for (int16_t i = first_icon_idx; i < limit_idx; ++i) {
        if (MOX::_fleet_movement_icon[i].field == icon_field_id) {
            int16_t ship_node_idx = MOX::_fleet_movement_icon[i].ship_node_idx;
            return static_cast<int16_t>(MOX::_ship_node[ship_node_idx].selected);
        }
    }

    return 0;
}

/**
 * @address 00403ec0
 */
void __cdecl Clear_Ship_Node_Selection_Flags_() {
    MOX::_fleet_movement_icons_on = 0;

    for (int i = 0; i < 500; ++i) {
        MOX::_ship_node[i].selected = 0;
    }
}

/**
 * @address 004059c0
 */
void __cdecl Reload_Fleet_Box_Palette_(int16_t player_idx, uint32_t is_not_player) {
    KEN::Load_Player_Ship_Palette_(player_idx);
    MOX::_mainscr_palette = 2;
}

/**
 * @address 00406590
 */
bool __cdecl Ship_Should_Be_Auto_Selected_(int16_t ship_idx) {
    if (MAINSCR::Ship_Can_Be_Selected_(ship_idx) && MOX::_just_moved_stack == 0) {
        return true;
    }
    return false;
}

void __cdecl Add_Added_Buttons_(int16_t x_offset, int16_t y_offset) {
    int16_t btn_height;
    int16_t x_pos;
    int16_t y_base;
    int16_t stack_y_offset;
    s_animation_header* close_seg;
    s_animation_header* engage_seg;
    s_animation_header* colonize_seg;
    s_animation_header* transport_seg;
    s_animation_header* outpost_seg;
    s_animation_header* attack_seg;

    btn_height = animate::Get_Height_(MOX::_fleet_box_close_lg_button_seg);

    if (MOX::_fleet_box.count > 9) {
        attack_seg = MOX::_fleet_box_att_xenons_lg_button_seg;
        colonize_seg = MOX::_fleet_box_colonize_lg_button_seg;
        transport_seg = MOX::_fleet_box_transport_lg_button_seg;
        outpost_seg = MOX::_fleet_box_outpost_lg_button_seg;
        engage_seg = MOX::_fleet_box_engage_lg_button_seg;
        close_seg = MOX::_fleet_box_close_lg_button_seg;
    } else {
        attack_seg = MOX::_fleet_box_att_xenons_sm_button_seg;
        colonize_seg = MOX::_fleet_box_colonize_sm_button_seg;
        transport_seg = MOX::_fleet_box_transport_sm_button_seg;
        outpost_seg = MOX::_fleet_box_outpost_sm_button_seg;
        engage_seg = MOX::_fleet_box_engage_sm_button_seg;
        close_seg = MOX::_fleet_box_close_sm_button_seg;
    }

    x_pos = x_offset + 2;
    y_base = (MOX::_fleet_box.field8_0xe - btn_height) + y_offset;

    MOX::_moveable_box[2].close_btn = fields::Add_Clipped_Button_Field_(x_pos, y_base, s__00551100, close_seg, (void*)s__00551120, 40 /* 0x28 */);

    stack_y_offset = btn_height;

    if (MOX::_fleet_box.outpost_btn_active != 0) {
        MOX::_fleet_box_outpost_button = fields::Add_Clipped_Button_Field_(x_pos, y_base + btn_height, s__00551100, outpost_seg, (void*)s__00551100, 40 /* 0x28 */);
        stack_y_offset = btn_height * 2;
    }

    if (MOX::_fleet_box.colonize_btn_active != 0) {
        MOX::_fleet_box_colonize_button = fields::Add_Clipped_Button_Field_(x_pos, y_base + stack_y_offset, s__00551100, colonize_seg, (void*)s__00551100, 40 /* 0x28 */);
        stack_y_offset += btn_height;
    }

    if (MOX::_fleet_box.engage_btn_active != 0) {
        MOX::_fleet_box_engage_button = fields::Add_Clipped_Button_Field_(x_pos, y_base + stack_y_offset, s__00551100, engage_seg, (void*)s__00551100, 40 /* 0x28 */);
        stack_y_offset += btn_height;
    }

    if (MOX::_fleet_box.transport_btn_active != 0) {
        MOX::_fleet_box_transport_button = fields::Add_Clipped_Button_Field_(x_pos, y_base + stack_y_offset, s__00551100, transport_seg, (void*)s__00551100, 40 /* 0x28 */);
        stack_y_offset += btn_height;
    }

    if (MOX::_fleet_box.attack_btn_active != 0) {
        MOX::_fleet_box_attack_button = fields::Add_Clipped_Button_Field_(x_pos, y_base + stack_y_offset, s__00551100, attack_seg, (void*)s__00551100, 40 /* 0x28 */);
    }
}

/**
 * @address 004051e0
 */
int16_t __cdecl N_Fleet_Icons_(int16_t node_idx) {
    int16_t count = 1;

    // Traverse the ship node chain starting from node_idx
    while (MOX::_ship_node[node_idx].next_node != -1 && count < 500) {
        count++;
        HACCESS::Ship_Owner_(MOX::_ship_node[node_idx].ship_idx);
        node_idx = MOX::_ship_node[node_idx].next_node;
    }

    return count;
}

/**
 * @address 00403e90
 */
bool __cdecl At_Least_One_Ship_Of_Type_Selected_(int16_t ship_type) {
    int16_t selected_ship_ids[500];

    // HACCESS::Get_Fleet_Box_Selected_Ship_Ids_ returns the number of ships found.
    int16_t count = HACCESS::Get_Fleet_Box_Selected_Ship_Ids_(selected_ship_ids, 500, ship_type);

    return count > 0;
}

void __cdecl Draw_Fleet_Movement_Box_() {
    s_colors font_colors;
    char title[120];
    char race_or_ship_name[80];
    int16_t frame_out_value;
    int16_t icon_button_x;
    int16_t icon_button_y;

    memcpy(title, "llllllll", 8);

    for (int16_t i = 0; i < 8; ++i) {
        if (i == 0) {
            font_colors.colors[i] = 0x6E;
        } else {
            font_colors.colors[i] = static_cast<uint8_t>(i + 0x71);
        }
    }

    const int16_t content_offset_x = MOX::_moveable_box[2].content_offset_x;
    const int16_t content_offset_y = MOX::_moveable_box[2].content_offset_y;
    const int16_t head_node_ptr = HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack);

    bool rebuild_saved_ship_array;
    if (head_node_ptr == MOX::_first_node_ptr_in_stack_) {
        rebuild_saved_ship_array = false;
    } else {
        const int16_t fleet_icon_count =
            FLEETPOP::N_Fleet_Icons_(HACCESS::Node_Ptr_From_Ship_Stack_(MOX::_fleet_box_ship_stack));

        FLEETPOP::Fleet_Box_Data_(fleet_icon_count);

        int32_t scroll_rows = (fleet_icon_count + 2) / 3;
        if (scroll_rows <= 1) {
            scroll_rows = 1;
        }

        FLEETPOP::Initialize_Scroll_Bar_(&MOX::_fltpop_scroll_bar, static_cast<int16_t>(scroll_rows));
        rebuild_saved_ship_array = true;
        HACCESS::Clear_Fleet_Box_Selected_Ship_Status_();
    }

    const int16_t stack_owner = HACCESS::Ship_Stack_Owner_(head_node_ptr);
    if (stack_owner != (int16_t)MOX::_ship_icon_player_id) {
        MOX::_ship_icon_player_id = (uint16_t)stack_owner;
    }

    FLEETPOP::Buffer_Reload_Fleetpop_Frame_();

    const int16_t fleet_box_x = MOX::_fleet_box.x;
    const int16_t fleet_box_y = MOX::_fleet_box.y;
    const int16_t fleet_box_width = MOX::_fleet_box.width;

    if (MOX::_draw_expanding_box == 0) {
        if (MOX::_fleet_box.is_small_frame == 0) {
            FLEETPOP::Draw_Large_Fleet_Box_Frame_(
                MOX::_fleet_box.x,
                MOX::_fleet_box.y,
                content_offset_x,
                content_offset_y,
                MOX::_fleet_box.count,
                &frame_out_value
            );
        } else {
            FLEETPOP::Draw_Small_Fleet_Box_Frame_(
                MOX::_fleet_box.x,
                MOX::_fleet_box.y,
                content_offset_x,
                content_offset_y,
                MOX::_fleet_box.count,
                &frame_out_value
            );
        }

        const int16_t title_origin_x = static_cast<int16_t>(fleet_box_x + content_offset_x);
        const int16_t title_origin_y = static_cast<int16_t>(fleet_box_y + content_offset_y);

        FLEETPOP::Draw_Fleet_Pop_Added_Buttons_(title_origin_x, title_origin_y);

        if (stack_owner < 9) {
            MOX::_mox_font_colors_offset = 0x71;
            MISC::Get_Mox_Font_Colors_(4, 'o', 's', &font_colors);
            HACCESS::Race_Name_(stack_owner, race_or_ship_name);
            _sprintf(title, HAROLD::H_Message_(0x66), race_or_ship_name);
            fonts::Set_Font_Style_Outline_(4, &font_colors);
            fonts::Set_Outline_Color_(2, 1);
            fonts::Set_Font_Spacing_(1);
        } else {
            MISC::Get_Mox_Font_Colors_(4, 'K', 'N', &font_colors);
            strcpy(title, MOX::_ship[MOX::_ship_node[head_node_ptr].ship_idx].d.name);
            strings::String_To_Upper_(title);
            fonts::Set_Font_Style_Outline_(4, &font_colors);
            fonts::Set_Outline_Color_(2, 1);
        }

        fonts::Print_Centered_(
            static_cast<int16_t>(title_origin_x + fleet_box_width / 2),
            static_cast<int16_t>(title_origin_y + 14),
            title
        );
    }

    if ((head_node_ptr != MOX::_first_node_ptr_in_stack_) || (MOX::_first_node_ptr_in_stack_ == -1)) {
        memset(&MOX::_fleet_movement_icon[0], 0, 500 * sizeof(s_fleet_movement_icon));
        MOX::_fleet_icon_buttons_added = 0;
    }

    MOX::_first_fleet_movement_icon =
        static_cast<int16_t>(MOX::_fltpop_scroll_bar.first_visible_row * 3);

    bool stop_building_icons = false;
    int16_t icon_slot = 0;
    int16_t current_node_ptr = head_node_ptr;

    if (head_node_ptr != MOX::_first_node_ptr_in_stack_) {
        while (true) {
            if ((current_node_ptr == -1) || (icon_slot >= 500)) {
                stop_building_icons = true;
            }

            if (MOX::_fleet_icon_buttons_added < 9) {
                ++MOX::_fleet_icon_buttons_added;
            }

            if (icon_slot >= 500) {
                break;
            }

            MOX::_fleet_movement_icon[icon_slot].ship_node_idx = current_node_ptr;
            MOX::_fleet_movement_icon[icon_slot].field = -1000;
            ++icon_slot;

            current_node_ptr = MOX::_ship_node[current_node_ptr].next_node;
            if (current_node_ptr == -1) {
                stop_building_icons = true;
            }

            if (!stop_building_icons) {
                continue;
            }

            break;
        }
    }

    if ((MOX::_first_time != 0) &&
        (MOX::_settings.auto_select_ships != 0) &&
        (MOX::_PLAYER_NUM == stack_owner)) {
        FLEETPOP::Set_Fleet_Movement_Icon_Selections_(1, 1);
    }

    if (rebuild_saved_ship_array) {
        HACCESS::Build_Saved_Ship_Array_();
    }

    int16_t visible_icon_end =
        static_cast<int16_t>(MOX::_fleet_icon_buttons_added + MOX::_first_fleet_movement_icon);
    if (visible_icon_end >= MOX::_fleet_box.count) {
        visible_icon_end = MOX::_fleet_box.count;
    }

    int16_t visible_icon_idx = MOX::_first_fleet_movement_icon;
    int16_t visible_button_slot = 0;

    if ((visible_icon_end > visible_icon_idx) && (MOX::_draw_expanding_box == 0)) {
        do {
            const int16_t ship_node_idx =
                MOX::_fleet_movement_icon[visible_icon_idx].ship_node_idx;
            const int16_t ship_idx = MOX::_ship_node[ship_node_idx].ship_idx;

            MOX::_fleet_movement_box_ptr[visible_button_slot] = visible_icon_idx;

            FLEETPOP::Get_Fleet_Icon_Button_XY_(visible_button_slot, &icon_button_x, &icon_button_y);

            icon_button_x = static_cast<int16_t>(icon_button_x + content_offset_x + fleet_box_x);
            icon_button_y = static_cast<int16_t>(icon_button_y + content_offset_y + fleet_box_y);

            MOX::_fleet_movement_icon[visible_icon_idx].x = icon_button_x;
            MOX::_fleet_movement_icon[visible_icon_idx].y = icon_button_y;

            int16_t ship_picture_y_offset = 2;
            if (MOX::_ship[ship_idx].d.size == 5) {
                ship_picture_y_offset = 0;
            }

            int16_t picture_ship_idx;
            if (HACCESS::Fleet_Movement_Icon_Selected_(visible_icon_idx)) {
                int16_t selection_x_offset = 0;
                if (MOX::_fleet_box.is_small_frame == 1) {
                    selection_x_offset = 1;
                }

                animate::Draw_(
                    static_cast<int16_t>(icon_button_x + selection_x_offset),
                    icon_button_y,
                    MOX::_fleet_box_selection_seg
                );

                picture_ship_idx =
                    MOX::_ship_node[MOX::_fleet_movement_icon[visible_icon_idx].ship_node_idx].ship_idx;
            } else {
                picture_ship_idx =
                    MOX::_ship_node[MOX::_fleet_movement_icon[visible_icon_idx].ship_node_idx].ship_idx;
            }

            animate::Draw_(
                static_cast<int16_t>(icon_button_x + 2),
                static_cast<int16_t>(icon_button_y + ship_picture_y_offset),
                KEN::Get_Ship_Id_Picture_Seg_(picture_ship_idx)
            );

            if (MOX::_PLAYER_NUM == stack_owner) {
                const int16_t fleet_ship_id =
                    HACCESS::Fleet_Movement_Ship_Id_(visible_icon_idx);

                if (HACCESS::Is_Combat_Ship_(fleet_ship_id)) {
                    FLEETPOP::Draw_Damage_Bars_(
                        static_cast<int16_t>(MOX::_fleet_movement_icon[visible_icon_idx].x - 1),
                        MOX::_fleet_movement_icon[visible_icon_idx].y,
                        0x35,
                        0x35,
                        4,
                        fleet_ship_id
                    );
                }
            }

            ++visible_button_slot;
            ++visible_icon_idx;
        } while (visible_icon_idx < visible_icon_end);
    }

    KEN::Load_Player_Ship_Palette_(
        static_cast<int16_t>(
            static_cast<int8_t>(MOX::_ship[MOX::_ship_node[head_node_ptr].ship_idx].owner)
        )
    );

    MOX::_first_node_ptr_in_stack_ = head_node_ptr;

    if (MOX::_draw_expanding_box == 0) {
        FLEETPOP::Print_Fleet_Box_Info_();

        if (MOX::_fleet_box.count > 9) {
            SCROLL::Draw_Generic_Vertical_Scroll_Bar_(&MOX::_fltpop_scroll_bar, (void(__cdecl*)())Fleet_Movement_Scroll_Callback);
        }
    }

    MOX::_first_time = 0;

    if (MOX::_draw_expanding_box != 0) {
        const int16_t ship_icon_id =
            HACCESS::Ship_Icon_Id_From_Node_Ptr_(MOX::_first_node_ptr_in_stack_);

        MOX::_draw_expanding_box = 0;

        MAINSCR::Draw_Exploding_Plasma_Box_(
            static_cast<int16_t>(MOX::_ship_icon[ship_icon_id].x + 3),
            static_cast<int16_t>(MOX::_ship_icon[ship_icon_id].y + 3),
            MOX::_moveable_box[2].x,
            MOX::_moveable_box[2].y,
            MOX::_moveable_box[2].width,
            MOX::_moveable_box[2].height,
            2
        );

        FLEETPOP::Draw_Fleet_Movement_Box_();
    }

    if (MOX::_add_help_fields == 0) {
        MOX::_add_help_fields = 0xFF;
        EVANHELP::Set_Main_Screen_Help_List_();
    }
}

void __cdecl Set_Added_Button_Stuff_() {
    int16_t added_buttons_height = 0;

    FLEETPOP::_n_added_buttons = 1;

    if (FLEETPOP::Check_For_Colonization_Button_()) {
        added_buttons_height = animate::Get_Height_(MOX::_fleet_box_colonize_lg_button_seg);
        ++FLEETPOP::_n_added_buttons;
    }

    if (FLEETPOP::Check_For_Transport_Troops_Button_()) {
        added_buttons_height = static_cast<int16_t>(
            static_cast<uint16_t>(added_buttons_height) +
            static_cast<uint16_t>(animate::Get_Height_(MOX::_fleet_box_transport_lg_button_seg)));
        ++FLEETPOP::_n_added_buttons;
    }

    if (FLEETPOP::Check_For_Build_Outpost_Button_()) {
        added_buttons_height = static_cast<int16_t>(
            static_cast<uint16_t>(added_buttons_height) +
            static_cast<uint16_t>(animate::Get_Height_(MOX::_fleet_box_outpost_lg_button_seg)));
        ++FLEETPOP::_n_added_buttons;
    }

    if (FLEETPOP::Check_For_Engage_Enemy_Forces_Button_()) {
        added_buttons_height = static_cast<int16_t>(
            static_cast<uint16_t>(added_buttons_height) +
            static_cast<uint16_t>(animate::Get_Height_(MOX::_fleet_box_engage_lg_button_seg)));
        ++FLEETPOP::_n_added_buttons;
    }

    if (FLEETPOP::Check_For_Attack_Antares_Button_()) {
        added_buttons_height = static_cast<int16_t>(
            static_cast<uint16_t>(added_buttons_height) +
            static_cast<uint16_t>(animate::Get_Height_(MOX::_fleet_box_att_xenons_lg_button_seg)));
        ++FLEETPOP::_n_added_buttons;
    }

    MOX::_fleet_box.height = static_cast<int16_t>(
        static_cast<uint16_t>(MOX::_fleet_box.field8_0xe) +
        static_cast<uint16_t>(added_buttons_height));

    FLEETPOP::Set_Fleet_Box_Data_Dimensions_();
}

void __cdecl Get_Fleet_Icon_Button_XY_(int16_t fleet_icon_index, int16_t* out_x, int16_t* out_y) {
    *out_x = static_cast<int16_t>((fleet_icon_index % 3) * 58 + 14);
    *out_y = static_cast<int16_t>((fleet_icon_index / 3) * 56 + 38);
}

void __cdecl Draw_Small_Fleet_Box_Frame_(
        int16_t x,
        int16_t y,
        int16_t x_offset,
        int16_t y_offset,
        int16_t fleet_count,
        int16_t* current_y) {
    int32_t grid_rows = (((int32_t)fleet_count - 1) / 3) + 1;
    if (grid_rows >= 3) {
        grid_rows = 3;
    }

    const int16_t left_x = static_cast<int16_t>(x + x_offset);
    const int16_t right_x = static_cast<int16_t>(left_x + ::_fleet_box_width);
    const int16_t start_y = static_cast<int16_t>(y + y_offset);
    uint32_t saved_window_status = 0;

    *current_y = start_y;

    MISC::Save_Window_Status_(&saved_window_status);

    int32_t clipped_bottom = (int32_t)::_fleet_box_field8_0xe + (int32_t)y_offset + (int32_t)y;
    if (clipped_bottom >= 0x1A5) {
        clipped_bottom = 0x1A5;
    }

    graphics::Set_Window_(left_x, start_y, right_x, static_cast<int16_t>(clipped_bottom));
    video::Clip_On_();
    graphics::Fill_(
            static_cast<int16_t>(left_x + 6),
            static_cast<int16_t>(start_y + 0x29),
            static_cast<int16_t>(right_x - 6),
            static_cast<int16_t>(start_y + ::_fleet_box_field8_0xe + 0x1D),
            0);
    MISC::Restore_Window_Status_(saved_window_status);

    *current_y = start_y;

    s_animation_header* top_frame =
            (s_animation_header*)buffer::Buffer_Reload_(::s_BUFFER0_LBX_00551124, 0x34, MOX::_global_cache_seg);
    animate::Draw_(left_x, *current_y, top_frame);
    *current_y = static_cast<int16_t>(*current_y + animate::Get_Height_(top_frame));

    (void)animate::Get_Height_(top_frame);

    const int16_t grid_height =
            FLEETPOP::Fleet_Box_Grid_Height_(static_cast<int16_t>(grid_rows));

    clipped_bottom = (int32_t)(*current_y) + (int32_t)grid_height;
    if (clipped_bottom >= 0x1A5) {
        clipped_bottom = 0x1A5;
    }

    MISC::Save_Window_Status_(&saved_window_status);
    graphics::Set_Window_(left_x, *current_y, right_x, static_cast<int16_t>(clipped_bottom));
    video::Clip_On_();

    MOX::_fleet_box_grid_sm_seg4 =
            buffer::Buffer_Reload_(::s_BUFFER0_LBX_00551124, 0x36, MOX::_global_cache_seg);
    animate::Draw_(left_x, *current_y, (s_animation_header*)MOX::_fleet_box_grid_sm_seg4);

    MISC::Restore_Window_Status_(saved_window_status);

    *current_y = static_cast<int16_t>(*current_y + grid_height);

    s_animation_header* bottom_frame =
            (s_animation_header*)buffer::Buffer_Reload_(::s_BUFFER0_LBX_00551124, 0x38, MOX::_global_cache_seg);
    animate::Draw_(left_x, *current_y, bottom_frame);
    *current_y = static_cast<int16_t>(*current_y + animate::Get_Height_(bottom_frame));
}

void __cdecl Fleet_Box_Data_(int16_t ship_count) {
    if (MOX::_first_time != 0) {
        MOX::_fleet_box_scroll_first_ship = 0;
        MOX::_fleet_box.mouse_pressed = 0;
    }

    if (ship_count > 9) {
        MOX::_fleet_box.is_small_frame = 0;

        MOX::_fleet_box_bottom_lg_seg =
            (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x37, MOX::_global_cache_seg);
        MOX::_fleet_box.bottom_seg = MOX::_fleet_box_bottom_lg_seg;

        MOX::_fleet_box_name_lg_seg =
            (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x33, MOX::_global_cache_seg);
        MOX::_fleet_box.x = MOX::_moveable_box_2_x;
        MOX::_fleet_box.y = MOX::_moveable_box_2_y;
        MOX::_fleet_box.name_seg = MOX::_fleet_box_name_lg_seg;

        MOX::_fleet_box.width = Fleet_Box_Width_(ship_count);
        MOX::_fleet_box.height = Fleet_Box_Height_(ship_count);

        MOX::_fleet_box_grid_lg_seg =
            (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x35, MOX::_global_cache_seg);
        MOX::_fleet_box.map_height = animate::Get_Height_(MOX::_fleet_box_grid_lg_seg);

        MOX::_fleet_box.field9_0x10 = static_cast<int16_t>(
            MOX::_fleet_box.map_height + animate::Get_Height_(MOX::_fleet_box.name_seg) + 4);
    } else {
        MOX::_fleet_box.is_small_frame = 1;

        MOX::_fleet_box_bottom_sm_seg =
            (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x38, MOX::_global_cache_seg);
        MOX::_fleet_box.bottom_seg = MOX::_fleet_box_bottom_sm_seg;

        MOX::_fleet_box_name_sm_seg =
            (s_animation_header*)buffer::Buffer_Reload_(s_BUFFER0_LBX, 0x34, MOX::_global_cache_seg);
        MOX::_fleet_box.name_seg = MOX::_fleet_box_name_sm_seg;

        Set_Fleet_Movement_Box_Dimensions_(ship_count);

        MOX::_fleet_box.x = MOX::_moveable_box_2_x;
        MOX::_fleet_box.y = MOX::_moveable_box_2_y;
        MOX::_fleet_box.width = Fleet_Box_Width_(ship_count);
        MOX::_fleet_box.height = Fleet_Box_Height_(ship_count);
        MOX::_fleet_box.map_height = MOX::_fleet_box_map_height;

        MOX::_fleet_box.field9_0x10 = static_cast<int16_t>(
            animate::Get_Height_(MOX::_fleet_box.name_seg) + MOX::_fleet_box.map_height + 4);
    }

    MOX::_fleet_box.count = ship_count;
    MOX::_fleet_box.field8_0xe = MOX::_fleet_box.height;

    if (MOX::_fleet_box.mouse_pressed == 0) {
        Set_Added_Button_Stuff_();
    }
}

void __cdecl Print_Fleet_Box_Info_() {
    int16_t info_value = -1;
    int16_t destination_star = -1;
    int16_t info_type = 9;
    bool use_primary_colors = true;
    char text[80];
    s_colors font_colors;
    s_colors fitted_colors;

    const int16_t ship_idx = HACCESS::Fleet_Movement_Ship_Id_(0);
    const bool icon_selected = FLEETPOP::Some_Icon_Selected_();

    int16_t max_width;
    int16_t text_y;

    if (MOX::_fleet_box.count > 9) {
        max_width = 0x84;
        text_y = 0x00d6;
    } else {
        max_width = 0x70;
        text_y = static_cast<int16_t>(
                MOX::_fleet_box.map_height +
                animate::Get_Height_(MOX::_fleet_box.name_seg) +
                10);
    }

    const int16_t text_x = static_cast<int16_t>(
            MOX::_fleet_box.x +
            MOX::_moveable_box[2].content_offset_x +
            (MOX::_fleet_box.is_small_frame == 1 ? 0x80 : 0x8a));

    text_y = static_cast<int16_t>(
            MOX::_moveable_box[2].content_offset_y +
            MOX::_moveable_box[2].y +
            text_y);

    if (MOX::_eta_star_id == -1) {
        if (MOX::_fleet_ship_scanned != -1) {
            info_type = 0;
        } else if (!icon_selected) {
            if (MOX::_fleet_ship_scanned == -1) {
                if (MAINSCR::_fleet_icon_chosen != -1) {
                    MOX::_fleet_ship_scanned = MAINSCR::_fleet_icon_chosen;
                }

                if (MOX::_fleet_ship_scanned == -1) {
                    if (MOX::_ship[ship_idx].status == 0) {
                        info_type = 1;
                        info_value = MOX::_ship[ship_idx].location;
                    } else {
                        FLEETPOP::Get_Absolute_Location_Scan_Info_(
                                ship_idx,
                                &info_value,
                                &destination_star,
                                &info_type);
                    }
                }
            }
        } else {
            if (MOX::_ship[ship_idx].status == 0) {
                info_type = 1;
                info_value = MOX::_ship[ship_idx].location;
            } else {
                FLEETPOP::Get_Absolute_Location_Scan_Info_(
                        ship_idx,
                        &info_value,
                        &destination_star,
                        &info_type);
            }
        }
    } else if ((MOX::_eta_star_id > -1) && icon_selected) {
        if (MOX::_g_ship_move_info.moving != 0) {
            if (MOX::_g_ship_move_info.turns_left == 0) {
                info_type = 1;
                info_value = MOX::_eta_star_id;
            } else if (HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, MOX::_eta_star_id)) {
                info_type = 2;
                info_value = static_cast<uint8_t>(MOX::_g_ship_move_info.turns_left);
                destination_star = MOX::_eta_star_id;
            } else {
                info_type = 3;
                info_value = static_cast<uint8_t>(MOX::_g_ship_move_info.turns_left);
            }
        } else if (MOX::_g_ship_move_info.blackhole_blocks != 0) {
            info_type = 4;
        } else if (MOX::_g_ship_move_info.immobile != 0) {
            info_type = 5;
        } else if (MOX::_g_ship_move_info.out_of_range != 0) {
            if (HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, MOX::_eta_star_id)) {
                info_type = 6;
                info_value = static_cast<uint8_t>(MOX::_g_ship_move_info.parsecs);
                destination_star = MOX::_eta_star_id;
            } else {
                info_type = 7;
                info_value = static_cast<uint8_t>(MOX::_g_ship_move_info.parsecs);
            }
        } else if (MOX::_g_ship_move_info.hyperspace_flux != 0) {
            info_type = 8;
        }
    }

    switch (info_type) {
    case 0:
        FLEETPOP::Get_Fleetpop_Scanned_Ship_Info_(text);
        break;

    case 1:
        _sprintf(text, HAROLD::H_Message_(0x67), MOX::_star[info_value].name);
        break;

    case 2:
        if (MOX::_NUM_STARS == static_cast<int16_t>(destination_star)) {
            strcpy(text, HAROLD::H_Message_(0x68));
        } else if (info_value == 1) {
            _sprintf(text, HAROLD::H_Message_(0x69), 1, MOX::_star[destination_star].name);
        } else {
            _sprintf(text, HAROLD::H_Message_(0x6a), info_value, MOX::_star[destination_star].name);
        }
        break;

    case 3:
        if (info_value == 1) {
            _sprintf(text, HAROLD::H_Message_(0x6b), info_value);
        } else {
            _sprintf(text, HAROLD::H_Message_(0x6c), info_value);
        }
        break;

    case 4:
        strcpy(text, HAROLD::H_Message_(0x6d));
        use_primary_colors = false;
        break;

    case 5:
        strcpy(text, HAROLD::H_Message_(0x6e));
        use_primary_colors = false;
        break;

    case 6:
        if (info_value == 1) {
            _sprintf(text, HAROLD::H_Message_(0x6f), 1, MOX::_star[destination_star].name);
        } else {
            _sprintf(text, HAROLD::H_Message_(0x70), info_value, MOX::_star[destination_star].name);
        }
        use_primary_colors = false;
        break;

    case 7:
        if (info_value == 0) {
            _sprintf(text, HAROLD::H_Message_(0x71));
        } else {
            _sprintf(text, HAROLD::H_Message_(0xeb), info_value);
        }
        use_primary_colors = false;
        break;

    case 8:
        strcpy(text, HAROLD::H_Message_(0x73));
        use_primary_colors = false;
        break;

    case 9:
    default:
        text[0] = '\0';
        break;
    }

    if (text[0] == '\0') {
        return;
    }

    MISC::Set_Fitted_Font_Style_(text, &fitted_colors, 3, 2, 0, max_width, 1);

    if (use_primary_colors) {
        MOX::_mox_font_colors_offset = 0x71;
        MISC::Get_Mox_Font_Colors_(fonts::_current_font_style, 0x70, 0x73, &font_colors);
    } else {
        MISC::Get_Mox_Font_Colors_(fonts::_current_font_style, 0x4b, 0x50, &font_colors);
    }

    MISC::Set_Font_Colors_(&font_colors);

    if (fonts::_current_font_style == 1) {
        text_y = static_cast<int16_t>(text_y + 2);
    }

    const int16_t string_width = fonts::Get_String_Width_(text);
    if (string_width > max_width) {
        ERIC::Print_Paragraph_Centered_(
                text_x - max_width / 2,
                text_y - 6,
                max_width,
                0x14,
                text);
        return;
    }

    fonts::Print_Centered_(text_x, text_y, text);
}

void __cdecl Draw_Fleet_Pop_Added_Buttons_(int16_t x, int16_t y) {
    s_animation_header* colonize_seg;
    s_animation_header* engage_seg;
    s_animation_header* transport_seg;
    s_animation_header* attack_seg;
    s_animation_header* outpost_seg;
    s_animation_header* close_seg;

    const int16_t button_height = animate::Get_Height_(MOX::_fleet_box_close_lg_button_seg);

    if (MOX::_fleet_box_count > 9) {
        attack_seg = MOX::_fleet_box_att_xenons_lg_button_seg;
        colonize_seg = MOX::_fleet_box_colonize_lg_button_seg;
        transport_seg = MOX::_fleet_box_transport_lg_button_seg;
        outpost_seg = MOX::_fleet_box_outpost_lg_button_seg;
        close_seg = MOX::_fleet_box_close_lg_button_seg;
        engage_seg = MOX::_fleet_box_engage_lg_button_seg;
    } else {
        attack_seg = MOX::_fleet_box_att_xenons_sm_button_seg;
        colonize_seg = MOX::_fleet_box_colonize_sm_button_seg;
        transport_seg = MOX::_fleet_box_transport_sm_button_seg;
        outpost_seg = MOX::_fleet_box_outpost_sm_button_seg;
        close_seg = MOX::_fleet_box_close_sm_button_seg;
        engage_seg = MOX::_fleet_box_engage_sm_button_seg;
    }

    const int16_t base_y = static_cast<int16_t>(MOX::_fleet_box_field8_0xe - button_height + y);

    animate::Reset_Animation_Frame_(close_seg);
    x = static_cast<int16_t>(x + 2);
    animate::Draw_(x, base_y, close_seg);

    int16_t next_offset = button_height;

    if (MOX::_fleet_box_outpost_btn_active != 0) {
        animate::Reset_Animation_Frame_(outpost_seg);
        animate::Draw_(x, static_cast<int16_t>(base_y + button_height), outpost_seg);
        next_offset = static_cast<int16_t>(button_height * 2);
    }

    if (MOX::_fleet_box_colonize_btn_active != 0) {
        animate::Reset_Animation_Frame_(colonize_seg);
        animate::Draw_(x, static_cast<int16_t>(base_y + next_offset), colonize_seg);
        next_offset = static_cast<int16_t>(next_offset + button_height);
    }

    if (MOX::_fleet_box_engage_btn_active != 0) {
        animate::Reset_Animation_Frame_(engage_seg);
        animate::Draw_(x, static_cast<int16_t>(base_y + next_offset), engage_seg);
        next_offset = static_cast<int16_t>(next_offset + button_height);
    }

    if (MOX::_fleet_box_transport_btn_active != 0) {
        animate::Reset_Animation_Frame_(transport_seg);
        animate::Draw_(x, static_cast<int16_t>(base_y + next_offset), transport_seg);
        next_offset = static_cast<int16_t>(next_offset + button_height);
    }

    if (MOX::_fleet_box_attack_btn_active != 0) {
        animate::Reset_Animation_Frame_(attack_seg);
        animate::Draw_(x, static_cast<int16_t>(base_y + next_offset), attack_seg);
    }
}

void __cdecl Draw_Large_Fleet_Box_Frame_(
        int16_t x,
        int16_t y,
        int16_t x_offset,
        int16_t y_offset,
        uint32_t unused_param_5,
        int16_t* current_y) {
    s_window_status_and_height window_status;
    s_animation_header* segment;
    int16_t frame_left;
    int32_t window_bottom;

    frame_left = static_cast<int16_t>(x + x_offset);
    window_status.fleet_box_inner_height = FLEETPOP::_fleet_box_field8_0xe;
    *current_y = static_cast<int16_t>(y_offset + y);

    MISC::Save_Window_Status_(&window_status);

    window_bottom =
            static_cast<int32_t>(y) +
            static_cast<int32_t>(window_status.fleet_box_inner_height) +
            static_cast<int32_t>(y_offset);
    if (window_bottom >= 0x1A5) {
        window_bottom = 0x1A5;
    }

    graphics::Set_Window_(
            frame_left,
            *current_y,
            static_cast<int16_t>(x + FLEETPOP::_fleet_box_width + x_offset),
            static_cast<int16_t>(window_bottom));
    video::Clip_On_();

    graphics::Fill_(
            static_cast<int16_t>(frame_left + 6),
            static_cast<int16_t>(*current_y + 0x29),
            static_cast<int16_t>(frame_left + FLEETPOP::_fleet_box_width - 6),
            static_cast<int16_t>(*current_y + window_status.fleet_box_inner_height + 0x1D),
            0);

    MISC::Restore_Window_Status_(*reinterpret_cast<uint32_t*>(&window_status));

    segment = (s_animation_header*)buffer::Buffer_Reload_(
            (char*)FLEETPOP::s_BUFFER0_LBX,
            0x33,
            MOX::_global_cache_seg);
    animate::Draw_(frame_left, *current_y, segment);
    *current_y = static_cast<int16_t>(*current_y + animate::Get_Height_(segment));

    MOX::_fleet_box_grid_lg_seg = (s_animation_header*)buffer::Buffer_Reload_(
            (char*)FLEETPOP::s_BUFFER0_LBX,
            0x35,
            MOX::_global_cache_seg);
    animate::Draw_(frame_left, *current_y, MOX::_fleet_box_grid_lg_seg);
    *current_y = static_cast<int16_t>(*current_y + animate::Get_Height_(MOX::_fleet_box_grid_lg_seg));

    segment = (s_animation_header*)buffer::Buffer_Reload_(
            (char*)FLEETPOP::s_BUFFER0_LBX,
            0x37,
            MOX::_global_cache_seg);
    animate::Draw_(frame_left, *current_y, segment);
    *current_y = static_cast<int16_t>(*current_y + animate::Get_Height_(segment));

    (void)unused_param_5;
}

void __cdecl Initialize_Scroll_Bar_(s_scroll_bar* scroll_bar, int16_t total_rows) {
    scroll_bar->total_rows = total_rows;
    scroll_bar->visible_rows = 3;
    scroll_bar->track_len_px = 102;
    scroll_bar->first_visible_row = 0;

    scroll_bar->thumb_height_px = SCROLL::Get_Vertical_Scroll_Bar_Height_(scroll_bar);
    scroll_bar->last_thumb_center_y = -1;
    scroll_bar->type = 2;
    scroll_bar->row_step = 1;
    scroll_bar->thumb_center_y =
            static_cast<int16_t>(scroll_bar->track_len_px - (scroll_bar->thumb_height_px / 2));

    MOX::_scroll_bar = scroll_bar;
}

void __cdecl Buffer_Reload_Fleetpop_Frame_() {
    s_animation_header* seg;

    if (FLEETPOP::_fleet_box_is_small_frame == 0) {
        seg = (s_animation_header*)buffer::Buffer_Reload_(::s_BUFFER0_LBX_00551124, 0x37, MOX::_global_cache_seg);
        MOX::_fleet_box_bottom_lg_seg = seg;
        FLEETPOP::_fleet_box_bottom_seg = seg;

        seg = (s_animation_header*)buffer::Buffer_Reload_(::s_BUFFER0_LBX_00551124, 0x33, MOX::_global_cache_seg);
        MOX::_fleet_box_name_lg_seg = seg;
        FLEETPOP::_fleet_box_name_seg = seg;
        return;
    }

    seg = (s_animation_header*)buffer::Buffer_Reload_(::s_BUFFER0_LBX_00551124, 0x38, MOX::_global_cache_seg);
    MOX::_fleet_box_bottom_sm_seg = seg;
    FLEETPOP::_fleet_box_bottom_seg = seg;

    seg = (s_animation_header*)buffer::Buffer_Reload_(::s_BUFFER0_LBX_00551124, 0x34, MOX::_global_cache_seg);
    MOX::_fleet_box_name_sm_seg = seg;
    FLEETPOP::_fleet_box_name_seg = seg;
}

void __cdecl Get_Fleetpop_Scanned_Ship_Info_(char* out_text) {
    out_text[0] = '\0';

    if (MOX::_fleet_ship_scanned <= -1) {
        return;
    }

    const int16_t ship_idx = HACCESS::Fleet_Movement_Ship_Id_(MOX::_fleet_ship_scanned);
    const int16_t ship_owner = HACCESS::Ship_Owner_(ship_idx);
    s_ship_data* const ship = &MOX::_ship[ship_idx];
    const int16_t hull_size = static_cast<int8_t>(ship->d.size);

    if (MOX::_PLAYER_NUM == ship_owner) {
        strcpy(out_text, ship->d.name);
        return;
    }

    if (ship_owner <= 8) {
        if (HACCESS::Is_Combat_Ship_(ship_idx)) {
            _sprintf(out_text, s__s_00551130, TECHDATA::_hull_data[hull_size].name);
            return;
        }
    }

    strcpy(out_text, ship->d.name);
}

bool __cdecl Check_For_Colonization_Button_() {
    const uint8_t colonize_button_flag =
            FLEETPOP::Set_Colonize_Button_Flag_(FLEETPOP::_fleet_box.count);

    FLEETPOP::_fleet_box.colonize_btn_active =
            static_cast<int8_t>(colonize_button_flag);

    return colonize_button_flag != 0;
}

bool __cdecl Check_For_Transport_Troops_Button_(void) {
    const uint8_t transport_button_state =
        FLEETPOP::Set_Transport_Troops_Flag_(FLEETPOP::_fleet_box.count);

    FLEETPOP::_fleet_box.transport_btn_active =
        static_cast<int8_t>(transport_button_state);

    return transport_button_state != 0;
}

bool __cdecl Check_For_Build_Outpost_Button_() {
    const uint8_t outpost_button_flag =
        FLEETPOP::Set_Build_Outpost_Button_Flag_(FLEETPOP::_fleet_box.count);

    FLEETPOP::_fleet_box.outpost_btn_active =
        static_cast<int8_t>(outpost_button_flag);

    return outpost_button_flag != 0;
}

bool __cdecl Check_For_Engage_Enemy_Forces_Button_(void) {
    const uint8_t engage_btn_active =
        FLEETPOP::Set_Engage_Enemy_Forces_Flag_(FLEETPOP::_fleet_box_count);

    FLEETPOP::_fleet_box_engage_btn_active =
        static_cast<int8_t>(engage_btn_active);

    return engage_btn_active != 0;
}

bool __cdecl Check_For_Attack_Antares_Button_() {
    const uint8_t attack_btn_active = FLEETPOP::Set_Attack_Antares_Button_Flag_();
    FLEETPOP::_fleet_box_attack_btn_active = static_cast<int8_t>(attack_btn_active);
    return attack_btn_active != 0;
}

void __cdecl Set_Fleet_Box_Data_Dimensions_(void) {
    MOX::_moveable_box[2].width = MOX::_fleet_box.width;
    MOX::_moveable_box[2].height = MOX::_fleet_box.height;

    if (MOX::_fleet_box.width + MOX::_fleet_box.x > 0x20f) {
        MOX::_fleet_box.x = static_cast<int16_t>(0x20f - MOX::_fleet_box.width);
        MOX::_moveable_box[2].x = MOX::_fleet_box.x;
    }

    if (MOX::_fleet_box.y + MOX::_fleet_box.height > 0x1a5) {
        MOX::_fleet_box.y = static_cast<int16_t>(0x1a5 - MOX::_fleet_box.height);
        MOX::_moveable_box[2].y = MOX::_fleet_box.y;
    }
}

void __cdecl Get_Absolute_Location_Scan_Info_(
    int16_t ship_idx,
    int16_t* value,
    int16_t* star_idx,
    int16_t* info_type
) {
    s_ship_data& ship = MOX::_ship[ship_idx];
    int16_t absolute_location = HAROLD::Absolute_Location_(ship.location);
    uint16_t turns_left = static_cast<uint16_t>(static_cast<uint8_t>(ship.turns_left));

    if (static_cast<uint16_t>(absolute_location) != static_cast<uint16_t>(MOX::_NUM_STARS)) {
        if (!HAROLD::Player_Has_Visited_(MOX::_PLAYER_NUM, absolute_location)) {
            *value = static_cast<int16_t>(turns_left);
            *info_type = 3;
            return;
        }
    }

    *value = static_cast<int16_t>(turns_left);
    *star_idx = absolute_location;
    *info_type = 2;
}

}