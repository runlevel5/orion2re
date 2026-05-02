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

        FLEETPOP::Initialize_Scroll_Bar_(&MOX::_fltpop_scroll_bar, scroll_rows);
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

}