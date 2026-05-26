#include "pch.h"

namespace LIST {

    void __cdecl Set_List_Page_Pointer_(s_list_state* list) {
        uint8_t target_page = list->cur_page;
        list->cur_page = 1;

        if (target_page == 0) {
            target_page = 1;
        }

        s_list_item* items_start = list->items;
        s_list_item* current = items_start;
        // The list item count is treated as an unsigned 8-bit value (0-255).
        // Each list item occupies 158 (0x9E) bytes.
        s_list_item* items_end = &items_start[list->num_items];

        while (current < items_end) {
            if (target_page <= 1) {
                list->selected_item = current;
                return;
            }

            ++current;

            if (current >= items_end) {
                break;
            }

            // Check if we hit a boundary that signifies the start of a new page/group.
            if (current->stop_flag != 0) {
                --target_page;
                ++list->cur_page;
            }
        }

        list->selected_item = items_start;
    }

    void __cdecl Display_List_Text_(s_list_state* state, s_list_item* item) {
        if (item == nullptr || item == (s_list_item*)-7) {
            return;
        }

        s_colors* current_color = state->group_colors[item->group_color_index];
        fonts::Set_Font_Style_(state->group_font, current_color);
        BILL::Squeeze_Print_(item->x, item->y, item->group_name, state->item_width);

        if (state->draw_app_cb != nullptr) {
            fonts::Set_Font_Style_(state->item_font, current_color);

            if (item->app_count != 0) {
                for (uint16_t i = 0; i < (uint16_t)item->app_count; ++i) {
                    s_colors* app_color = state->group_colors[item->tech_app_status[i]];

                    if (app_color != current_color) {
                        current_color = app_color;
                        fonts::Set_Font_Style_(state->item_font, current_color);
                    }

                    fonts::Set_Font_Style_(state->item_font, current_color);

                    if (RACERPRT::_race_report_jim_hack == 1) {
                        uint8_t app_id = item->tech_app_ids[i];
                        if (MOX::_player[MOX::_PLAYER_NUM].tech_applications[app_id] != 3) {
                            fonts::Set_Font_Style_(state->item_font, state->disabled_color);
                        }
                    }

                    if (state->draw_app_cb != nullptr) {
                        auto draw_fn = reinterpret_cast<draw_app_fn>(state->draw_app_cb);
                        draw_fn(item->app_base_x, item->app_y_offsets[i] + item->y, item->tech_app_ids[i]);
                    }
                }
            }
        }
    }

    void __cdecl Display_List_Page_(s_list_state* state) {
        Set_List_Page_Pointer_(state);

        s_list_item* item = state->selected_item;
        s_list_item* end = &state->items[state->num_items];

        while (item != end) {
            Display_List_Text_(state, item);

            item++;

            if (item->stop_flag != 0) {
                break;
            }
        }
    }

    int16_t __cdecl Add_Fields_To_List_Page_(s_list_state* state) {
        s_list_item* item = state->selected_item;
        s_list_item* end_item = &state->items[state->num_items];

        while (item < end_item) {
            if (item->app_count != 0) {
                for (uint16_t i = 0; i < item->app_count; ++i) {
                    int16_t field_id = fields::Add_Hidden_Field_(
                        item->x,
                        state->app_ymin_offsets[i] + item->y,
                        item->app_x_max,
                        state->app_ymax_offsets[i] + item->y,
                        s__00551100,
                        0
                    );

                    item->app_ids[i] = (uint8_t)field_id;
                }
            }

            if (item[1].stop_flag != 0) {
                break;
            }
            item++;
        }

        return fields::Get_Field_Counter_() - 1;
    }

    void __cdecl Set_List_Up_Down_Field_Drawing_(s_list_state* state, int16_t up_field_id, int16_t down_field_id) {
        if (state->cur_page < state->max_page) {
            fields::Enable_Draw_Field_(down_field_id);
        } else {
            fields::Disable_Draw_Field_(down_field_id);
        }

        if (state->cur_page > 1) {
            fields::Enable_Draw_Field_(up_field_id);
        } else {
            fields::Disable_Draw_Field_(up_field_id);
        }
    }

    int8_t __cdecl Next_List_Page_(s_list_state* state) {
        if (state->cur_page < state->max_page) {
            state->cur_page++;
            LIST::Set_List_Page_Pointer_(state);
            return 1;
        }

        return 0;
    }

    int8_t __cdecl Previous_List_Page_(s_list_state* state) {
        if (state->cur_page > 1) {
            state->cur_page--;
            LIST::Set_List_Page_Pointer_(state);
            return 1;
        }

        return 0;
    }

    s_list_item* __cdecl Set_Selected_List_Entry_(s_list_state* state, int16_t target_app_id) {
        s_list_item* item = state->selected_item;
        s_list_item* end_item = &state->items[state->num_items];
        s_list_item* found_item = nullptr;

        while (item < end_item) {
            item->is_selectable = 0;

            if (found_item == nullptr) {
                for (uint8_t i = 0; i < item->app_count; ++i) {
                    // Sign-extend app_id byte to match int16 target_app_id (MOVSX in assembly)
                    if ((int16_t)(int8_t)item->app_ids[i] == target_app_id) {
                        item->is_selectable = 1;
                        item->current_app_index = i;
                        found_item = item;
                    }
                }
            }

            // Check if next item in memory has stop_flag set to terminate traversal
            if (item[1].stop_flag != 0) {
                break;
            }
            item++;
        }

        return found_item;
    }

    s_list_item* __cdecl Get_Selected_List_Entry_(s_list_state* state) {
        s_list_item* item = state->selected_item;
        s_list_item* end = &state->items[(uint8_t)state->num_items];

        while (item < end) {
            if (item->is_selectable != 0) {
                return item;
            }

            // If the current item is not selectable, we check the next item's stop_flag.
            // If the stop_flag is set, the selectable range has ended.
            // Note: (item + 1) points to the next s_list_item record.
            if ((item + 1)->stop_flag != 0) {
                break;
            }

            item++;
        }

        return nullptr;
    }

    void __cdecl Init_List_Data_(s_list_state* state) {
        s_list_item* item = state->items;
        int16_t current_y = state->y;
        int16_t y_max = state->y_max;

        state->selected_item = state->items;
        state->max_page = 1;
        state->cur_page = 1;

        // Size of s_list_item is 0x9E (158 bytes)
        s_list_item* end_item = &state->items[state->num_items];

        do {
            item->x = state->x;
            item->app_base_x = state->x + (int16_t)state->app_x_offset;
            item->app_x_max = (state->item_width - (int16_t)state->app_x_offset) + state->x;
            item->y = current_y;
            item->is_selectable = 0;

            uint8_t i = 0;
            if (item->app_count != 0) {
                do {
                    item->app_y_offsets[i] = state->app_y_table[i];
                    i++;
                } while (i < item->app_count);
            }

            // i is now equal to item->app_count
            int16_t next_y_step = state->app_y_table[i] + (int16_t)state->item_spacing;
            current_y += next_y_step;

            if (current_y > y_max) {
                // Item overflows current page, move it to the start of the next page
                item->stop_flag = 1;
                state->max_page++;
                item->y = state->y;
                current_y = state->y + next_y_step;
            } else {
                item->stop_flag = 0;
            }

            item++;
        } while (item < end_item);

        // The first item always starts a page
        state->items[0].stop_flag = 1;
    }

    void __cdecl Display_Selected_List_Text_(s_list_state* state, s_list_item* item) {
        if (item == nullptr || item == (s_list_item*)-7) {
            return;
        }

        fonts::Set_Font_Style_(state->group_font, state->active_color);
        BILL::Squeeze_Print_(item->x, item->y, item->group_name, state->item_width);

        if (state->draw_app_cb != nullptr) {
            fonts::Set_Font_Style_(state->item_font, state->active_color);

            auto draw_cb = reinterpret_cast<draw_app_fn>(state->draw_app_cb);

            int16_t current_app_y = item->app_y_offsets[item->current_app_index] + item->y;
            draw_cb(item->app_base_x, current_app_y, item->tech_app_ids[item->current_app_index]);

            if (state->marker_color != 0) {
                int16_t marker_x = item->x + 2;
                int16_t base_x_minus_2 = item->app_base_x - 2;
                int16_t base_x_minus_5 = item->app_base_x - 5;
                int16_t marker_y_bottom = current_app_y + 6;
                int16_t marker_y_top = item->app_y_offsets[0] + item->y - 3;

                line::Line_(marker_x, marker_y_top, marker_x, marker_y_bottom, state->marker_color);
                line::Line_(marker_x, marker_y_bottom, base_x_minus_2, marker_y_bottom, state->marker_color);
                line::Line_(base_x_minus_5, marker_y_bottom - 3, base_x_minus_2, marker_y_bottom, state->marker_color);
                line::Line_(base_x_minus_5, marker_y_bottom + 3, base_x_minus_2, marker_y_bottom, state->marker_color);
            }
        }
    }

    int8_t __cdecl Goto_Page_With_Entry_(s_list_state* state, s_list_item* target_item) {
        uint8_t page = 0;
        uint16_t item_index = 0;
        s_list_item* last_page_item;
        s_list_item* item = state->items;
        uint8_t item_count = state->num_items;

        if (item_count != 0) {
            do {
                if (item->stop_flag != 0) {
                    ++page;
                    last_page_item = item;
                }

                if (item == target_item) {
                    state->cur_page = page;
                    state->selected_item = last_page_item;
                    return 1;
                }

                ++item_index;
                ++item;
            } while (item_index < item_count);
        }

        return 0;
    }

    s_list_item* __cdecl Select_And_Page_By_Head_ID_(s_list_state* state, uint8_t head_id) {
        s_list_item* items = state->items;
        s_list_item* selected_item = items;
        uint16_t item_index = 0;
        uint8_t item_count = state->num_items;

        do {
            if (items[item_index].group_id == head_id) {
                selected_item = items + item_index;
                break;
            }
            ++item_index;
        } while (item_count > item_index);

        selected_item->is_selectable = 1;
        LIST::Goto_Page_With_Entry_(state, selected_item);
        return selected_item;
    }

    void __cdecl Display_Selected_List_Text_For_Tech_List_(s_list_state* list_state, s_list_item* item) {
        if (item != nullptr && item != (s_list_item*)-7) {
            fonts::Set_Font_Style_(list_state->group_font, list_state->active_color);
            BILL::Squeeze_Print_(item->x, item->y, item->group_name, list_state->item_width);

            if (list_state->draw_app_cb != nullptr) {
                fonts::Set_Font_Style_(list_state->item_font, list_state->active_color);

                auto draw_cb = reinterpret_cast<draw_app_fn>(list_state->draw_app_cb);

                uint8_t app_idx = item->current_app_index;
                draw_cb(item->app_base_x,
                        item->app_y_offsets[app_idx] + item->y,
                        item->tech_app_ids[app_idx]);
            }
        }
    }

}