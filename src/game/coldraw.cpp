#include "pch.h"

namespace COLDRAW {

void __cdecl Calculate_Squish_Step_(int16_t param_1, int16_t param_2, int16_t param_3, int16_t param_4) {
    int16_t divisor = param_3;
    int16_t base_val = param_4;

    // Perform initial division of base value by -3
    int16_t intermediate = (int16_t)(base_val / -3);

    // Adjust intermediate value
    intermediate = (intermediate - param_1) + param_2;

    // Ensure divisor is at least 1
    if (divisor <= 1) {
        divisor = 1;
    }

    // Calculate step adjustment
    int16_t step_calc = intermediate / divisor;

    // Clamping calculation to minimum of 1
    if (step_calc <= 1) {
        step_calc = 1;
    }

    // If calculated step is within the base value range, set squish step
    if (step_calc < base_val) {
        COLONY::_step_squish = base_val - step_calc;
    } else {
        COLONY::_step_squish = 0;
    }
}

void __cdecl Draw_Colony_Prod_Both_(int16_t colony_idx, int16_t prod_type, int16_t x, int16_t y, int16_t width, int16_t spacing, int8_t frame) {
    int16_t import_tens = 0;
    int16_t import_units = 0;
    int16_t secondary_tens = 0;
    int16_t secondary_units = 0;
    int16_t drawn_count = 0;

    s_colony* colony = &MOX::_colony[colony_idx];

    if (prod_type != 1) {
        int32_t imp_val = abs((int32_t)colony->imports[prod_type]);
        import_tens = (int16_t)(imp_val / 10);
        import_units = (int16_t)(imp_val % 10);
    }

    if (prod_type == 0) {
        int32_t imp_val_1 = abs((int32_t)colony->imports[1]);
        secondary_tens = (int16_t)(imp_val_1 / 10);
        secondary_units = (int16_t)(imp_val_1 % 10);
    } else if (prod_type == 1) {
        secondary_tens = colony->pollution / 10;
        secondary_units = colony->pollution % 10;
    }

    int16_t prod_val = colony->production[prod_type];
    int16_t shortage = (int16_t)colony->maintenance[prod_type] - colony->imports[prod_type] - prod_val;
    if (shortage < 1) {
        shortage = 0;
    }

    int32_t abs_shortage = abs((int32_t)shortage);
    int16_t shortage_tens = (int16_t)(abs_shortage / 10);
    int16_t shortage_units = (int16_t)(abs_shortage % 10);

    int16_t net_tens;
    int16_t net_units;

    if (colony->imports[prod_type] < 0) {
        if (prod_type == 1) {
            int32_t adj_prod = (int32_t)prod_val - (uint8_t)colony->maintenance[prod_type];
            if (adj_prod < 0) adj_prod = 0;
            net_tens = (int16_t)(adj_prod / 10);
            net_units = (int16_t)(adj_prod % 10);
        } else {
            int32_t adj_prod = (int32_t)prod_val - abs((int32_t)colony->imports[prod_type]);
            net_tens = (int16_t)(adj_prod / 10);
            net_units = (int16_t)(adj_prod % 10);
        }
    } else {
        if (colony->maintenance[1] == 0 || prod_type != 1) {
            net_tens = prod_val / 10;
            net_units = prod_val % 10;
        } else {
            int32_t adj_prod = (int32_t)prod_val - (uint8_t)colony->maintenance[prod_type];
            if (adj_prod < 0) adj_prod = 0;
            net_tens = (int16_t)(adj_prod / 10);
            net_units = (int16_t)(adj_prod % 10);
        }
    }

    int16_t total_tens = import_tens + secondary_tens + net_tens + shortage_tens;
    int16_t total_units = import_units + secondary_units + net_units + shortage_units;

    if (colony->imports[prod_type] != 0 && prod_type != 1) {
        total_units++;
    }
    if (secondary_tens != 0 || secondary_units != 0) {
        total_units++;
    }

    int16_t total_icons = total_tens + total_units;
    COLDRAW::Calculate_Squish_Step_(x, width, total_icons, spacing);

    if (spacing <= COLONY::_step_squish + 1) {
        COLONY::_step_squish = spacing - 2;
        graphics::Set_Window_(x, y - 20, width, y + 20);
        video::Clip_On_();
    }

    int16_t step = spacing - COLONY::_step_squish;

    for (int i = 0; i < net_tens; ++i) {
        animate::Draw_(step * drawn_count + x, y, COLONY::Prod_Anims_(prod_type, 1, frame));
        drawn_count++;
    }

    for (int i = 0; i < net_units; ++i) {
        animate::Draw_(step * drawn_count + x, y, COLONY::Prod_Anims_(prod_type, 0, frame));
        drawn_count++;
    }

    if (secondary_tens != 0 || secondary_units != 0) {
        if (prod_type == 1) {
            for (int i = 0; i < secondary_tens; ++i) {
                animate::Draw_(step * drawn_count + x, y, COLONY::Import_Anims_(1, 1, frame));
                drawn_count++;
            }
            for (int i = 0; i < secondary_units; ++i) {
                animate::Draw_(step * drawn_count + x, y, COLONY::Import_Anims_(1, 0, frame));
                drawn_count++;
            }
        } else if (prod_type == 0) {
            for (int i = 0; i < secondary_tens; ++i) {
                animate::Draw_(step * drawn_count + x, y, COLONY::Prod_Anims_(1, 1, frame));
                drawn_count++;
            }
            for (int i = 0; i < secondary_units; ++i) {
                animate::Draw_(step * drawn_count + x, y, COLONY::Prod_Anims_(1, 0, frame));
                drawn_count++;
            }
        }
        drawn_count++;
    }

    drawn_count++;

    if (colony->imports[prod_type] <= 0 || prod_type == 1) {
        for (int i = 0; i < import_tens; ++i) {
            animate::Draw_(step * drawn_count + x, y, COLONY::Prod_Anims_(prod_type, 1, frame));
            drawn_count++;
        }
        for (int i = 0; i < import_units; ++i) {
            animate::Draw_(step * drawn_count + x, y, COLONY::Prod_Anims_(prod_type, 0, frame));
            drawn_count++;
        }
    } else {
        for (int i = 0; i < import_tens; ++i) {
            animate::Draw_(step * drawn_count + x, y, COLONY::Import_Anims_(prod_type, 1, frame));
            drawn_count++;
        }
        for (int i = 0; i < import_units; ++i) {
            animate::Draw_(step * drawn_count + x, y, COLONY::Import_Anims_(prod_type, 0, frame));
            drawn_count++;
        }
        for (int i = 0; i < shortage_tens; ++i) {
            animate::Draw_(step * drawn_count + x, y, COLONY::Short_Anims_(prod_type, 1, frame));
            drawn_count++;
        }
        for (int i = 0; i < shortage_units; ++i) {
            animate::Draw_(step * drawn_count + x, y, COLONY::Short_Anims_(prod_type, 0, frame));
            drawn_count++;
        }
    }

    video::Clip_Off_();
}

void __cdecl Draw_Colony_Prod_(int16_t colony_handle, int32_t selection, int32_t p3, int16_t y_offset, int32_t p5, int32_t p6) {
    Draw_Colony_Prod_Both_(colony_handle, (int16_t)selection, (int16_t)p3, y_offset, (int16_t)p5, (int16_t)p6, 0);
}

void __cdecl Draw_Info_Morale_Both_(int16_t colony_idx, int16_t x, int16_t y, int width, char is_mini) {
    s_colony* colony = &MOX::_colony[colony_idx];
    int16_t morale_div2 = (int16_t)colony->morale / 2;

    int32_t morale_count_abs = abs((int32_t)morale_div2);
    int32_t morale_count = morale_count_abs;

    s_player* player = &MOX::_player[(uint8_t)colony->owner];

    if (player->traits[0] >= 6) {
        morale_count = 0;
    }

    if (is_mini == 0) {
        s_animation_header* govt_anim = COLONY::Govt_Anim_((int16_t)player->traits[0]);
        animate::Draw_(x, y - 1, govt_anim);
        x += 30;
    }

    Calculate_Squish_Step_(x, (int16_t)width, (int16_t)morale_count, 30);

    if (player->traits[0] < 6) {
        int32_t loop_limit = morale_count_abs;
        if (loop_limit > 20) {
            loop_limit = 20;
        }

        if (loop_limit > 0) {
            for (int32_t i = 0; i < loop_limit; ++i) {
                s_animation_header* anim;
                int16_t draw_y;

                if (morale_div2 > 0) {
                    if (is_mini != 0) {
                        anim = COLONY::C_Anims_(8);
                        draw_y = y + 1;
                    } else {
                        anim = COLONY::C_Anims_(5);
                        draw_y = y + 2;
                    }
                } else {
                    if (is_mini != 0) {
                        anim = COLONY::C_Anims_(9);
                        draw_y = y + 1;
                    } else {
                        anim = COLONY::C_Anims_(6);
                        draw_y = y + 2;
                    }
                }

                int16_t draw_x = x + (int16_t)i * (30 - COLONY::_step_squish);
                animate::Draw_(draw_x, draw_y, anim);
            }
        }
    }
}

void __cdecl Draw_Info_Morale_(int16_t param_1, int16_t param_2, int16_t param_3, int32_t param_4) {
    COLDRAW::Draw_Info_Morale_Both_(param_1, param_2, param_3, param_4, 0);
}

void __cdecl Draw_Colony_PC_Bar_(int16_t x, int16_t y, int16_t total, int16_t current) {
    int32_t max_div = (total > 1000) ? (int32_t)total : 1000;
    int32_t capped_cur = (current < total) ? (int32_t)current : (int32_t)total;

    // Calculate frame for the first animation (anim 35)
    int16_t frame1 = (int16_t)((capped_cur * 50) / max_div);

    // Calculate frame for the second animation (anim 34)
    // Note: Assembly uses ESI for this calculation and then refers to SI.
    int32_t frame2_full = ((capped_cur - (int32_t)total) * 50) / max_div + 50;
    int16_t frame2 = (int16_t)frame2_full;

    // Calculate Y offset for the second animation drawing
    // Assembly uses EDI for the first part, then modifies only DI (lower 16 bits).
    int32_t edi_val = ((capped_cur - max_div) * 10) / max_div;
    int16_t di_part = (int16_t)edi_val;
    di_part += (int16_t)(((int32_t)total - capped_cur) * 64 / max_div);
    di_part -= (int16_t)(((int32_t)total * 64) / max_div);

    // Reconstruct EDI value to match register behavior
    int32_t final_edi = (edi_val & 0xFFFF0000) | (uint16_t)di_part;

    graphics::Set_Window_(0, 0, 639 /* 0x27F */, 479 /* 0x1DF */);
    video::Clip_On_();

    // Draw the first part of the bar
    s_animation_header* anim_35 = COLONY::C_Anims_(35 /* 0x23 */);
    animate::Set_Animation_Frame_(anim_35, (int32_t)frame1);
    anim_35 = COLONY::C_Anims_(35 /* 0x23 */);
    animate::Draw_(x, y, anim_35);

    // Draw the second part of the bar at a calculated vertical offset
    s_animation_header* anim_34 = COLONY::C_Anims_(34 /* 0x22 */);
    animate::Set_Animation_Frame_(anim_34, (int32_t)frame2);
    anim_34 = COLONY::C_Anims_(34 /* 0x22 */);

    // Final Y is y + offset + 10
    int16_t y_final = (int16_t)(final_edi + (int32_t)y + 10);
    animate::Draw_(x, y_final, anim_34);

    video::Clip_Off_();
}

int16_t __cdecl Do_Colony_Info_Pop_Stuff_For_Pop_(
    int16_t colony_idx, int16_t param_2, int16_t param_3, int16_t param_4, int16_t param_5,
    int16_t param_6, int16_t param_7, int16_t* param_8, int16_t* param_9, int16_t* param_10, int16_t* param_11)
{
    int16_t pop_order[10];
    pop_order[0] = 9;
    pop_order[1] = 0;
    pop_order[2] = 1;
    pop_order[3] = 2;
    pop_order[4] = 3;
    pop_order[5] = 4;
    pop_order[6] = 5;
    pop_order[7] = 6;
    pop_order[8] = 7;
    pop_order[9] = 8;

    int16_t pop_draw_index = 0;

    if (param_3 != 2) {
        Do_Colony_Info_Pop_Stuff_For_Pop_(colony_idx, param_2, 2, param_4, param_5, param_6, param_7, param_8, param_9, param_10, param_11);
    }

    int16_t start_idx;
    int16_t end_idx;
    if (param_2 == -1) {
        start_idx = 0;
        end_idx = 3;
    } else {
        start_idx = param_2;
        end_idx = param_2 + 1;
    }

    if (param_3 == 0 && param_2 == 0 && MOX::_colony[colony_idx].max_farms == 0) {
        COLONY::Set_Colony_Font_To_(3);
        if (MOX::_settings.language == 1 && MOX::_current_screen == SCREEN_COLONY_SUMMARY) {
            BILL::Squeeze_Print_Paragraph_(param_4, param_5 - 1, param_6 - param_4, 34, ESTRINGS::E_Strings_(387 /* ESTR_NO_FARMING */), 2);
        } else {
            BILL::Squeeze_Print_Paragraph_(param_4, param_5 + 5, param_6 - param_4, 28, ESTRINGS::E_Strings_(387 /* ESTR_NO_FARMING */), 2);
        }
    }

    for (int16_t state_idx = 0; state_idx < 7; state_idx++) {
        for (int16_t race_idx = 0; race_idx < 2; race_idx++) {
            for (int16_t job_idx = start_idx; job_idx < end_idx; job_idx++) {
                for (int16_t order_idx = 0; order_idx < 10; order_idx++) {
                    int16_t current_order = pop_order[order_idx];
                    for (int16_t pop_idx = 0; pop_idx < MOX::_colony[colony_idx].n_pops; pop_idx++) {
                        uint32_t pop_val = MOX::_colony[colony_idx].pop[pop_idx];
                        if ((pop_val & 0xf) == current_order &&
                            COLONY::Pop_To_Pop_State_(colony_idx, pop_idx) == state_idx &&
                            ((pop_val & 0x180) >> 7) == job_idx &&
                            (pop_val & 0x200) != 0 &&
                            ((pop_val & 0x400) >> 10) == race_idx) {
                            
                            switch (param_3) {
                                case 0:
                                    COLONY::Set_Colony_Font_To_(2);
                                    if (pop_idx == COLONY::_scanned_pop && COLONY::_g_colony_n == colony_idx) {
                                        if (ERIC::Global_Cycler_(30, 2) != 0) {
                                            COLONY::Draw_Pop_Dark_((30 - COLONY::_step_squish) * pop_draw_index + param_4, param_5, colony_idx, pop_idx);
                                            break;
                                        }
                                    }
                                    {
                                        s_animation_header *anim = COLONY::Colony_Pop_Anim_(colony_idx, pop_idx);
                                        animate::Draw_((30 - COLONY::_step_squish) * pop_draw_index + param_4, param_5, anim);
                                    }
                                    break;

                                case 1:
                                    param_8[pop_draw_index] = pop_idx;
                                    break;

                                case 2:
                                    break;

                                case 3:
                                    if (*param_10 <= (30 - COLONY::_step_squish) * (pop_draw_index + 1) + param_4 || *param_9 == pop_draw_index) {
                                        if (COLONY::_debug_flag != 0) {
                                            assert::hgetch_();
                                        }
                                        return param_8[pop_draw_index];
                                    }
                                    break;

                                case 4:
                                    if (mouse::Pointer_X_() <= (30 - COLONY::_step_squish) * (pop_draw_index + 1) + param_4 || *param_9 == pop_draw_index) {
                                        return param_8[pop_draw_index];
                                    }
                                    break;
                            }
                            pop_draw_index++;
                        }
                    }
                }
            }
        }
    }

    switch (param_3) {
        case 0:
            if (param_7 != 0 && param_2 == 0) {
                if (MOX::_colony[colony_idx].max_farms != (char)0xff) {
                    if (pop_draw_index < MOX::_colony[colony_idx].max_farms) {
                        COLONY::Create_Shadow_Farmer_(MOX::_PLAYER_NUM);
                        while (pop_draw_index < MOX::_colony[colony_idx].max_farms) {
                            s_animation_header *anim = COLONY::Shadow_Farmer_Bitm_();
                            animate::Draw_((30 - COLONY::_step_squish) * pop_draw_index + param_4, param_5, anim);
                            pop_draw_index++;
                        }
                    }
                }
            }
            COLONY::Reset_Step_Squish_();
            break;

        case 1:
            *param_9 = pop_draw_index - 1;
            *param_11 = fields::Add_Scroll_Field_(param_4, param_5, param_4, param_6 + 8, param_4, param_6, param_6 - param_4 + 8, 30, param_10, ESTRINGS::E_Strings_(12 /* ESTR_SCROLL_BAR_EMPTY */), 0);
            COLONY::Reset_Step_Squish_();
            break;

        case 2:
            {
                int16_t squish_count = pop_draw_index;
                if (param_7 != 0 && param_2 == 0 && MOX::_colony[colony_idx].max_farms != (char)0xff) {
                    squish_count = MOX::_colony[colony_idx].max_farms;
                }
                COLDRAW::Calculate_Squish_Step_(param_4, param_6, squish_count, 30);
            }
            break;

        case 3:
        case 4:
            COLONY::Reset_Step_Squish_();
            break;
    }

    return -1;
}

void __cdecl Draw_Simplest_Path_(int16_t prev_y, int16_t prev_x, int16_t local_y, int16_t local_x, uint8_t* p1, uint8_t* p2) {
}

void __cdecl Road_Back_To_Front_(int16_t index, int16_t* out_val1, int16_t* out_val2) {
    uint8_t values[98] = {
        6, 6, 5, 6, 6, 5, 4, 6, 5, 5, 6, 4, 3, 6, 4, 3,
        5, 4, 6, 3, 2, 6, 3, 5, 4, 4, 5, 3, 6, 2, 1, 6,
        2, 5, 3, 4, 4, 3, 5, 2, 6, 1, 0, 6, 1, 5, 2, 4,
        3, 3, 4, 2, 5, 1, 6, 0, 0, 5, 1, 4, 2, 3, 3, 2,
        4, 1, 5, 0, 0, 4, 1, 3, 2, 2, 3, 1, 4, 0, 0, 3,
        1, 2, 2, 1, 3, 0, 0, 2, 1, 1, 2, 0, 0, 1, 1, 0,
        0, 0
    };

    *out_val1 = values[index * 2];
    *out_val2 = values[index * 2 + 1];
}

void __cdecl Draw_Road_List_() {
    int16_t y;
    int16_t x;
    uint8_t draw_order[4] = { 3, 2, 0, 1 };

    for (int16_t i = 0; i < 49; ++i) {
        Road_Back_To_Front_(i, &y, &x);

        for (uint8_t j = 0; j < 4; ++j) {
            uint8_t dir = draw_order[j];

            if (_road_list[y][x].connections[dir] != 0) {
                s_animation_header* anim = reinterpret_cast<s_animation_header*>(::Buffer_Reload_(
                    COLONY::_COLROADS_LBX,
                    _road_list[y][x].anims[dir],
                    MOX::_global_cache_seg
                ));

                animate::Draw_(0, 0, anim);
            }
        }
    }
}

void __cdecl Load_Road_List_Anims_() {
    memset(_road_list, 0, sizeof(_road_list));

    int16_t anim_id = 0;
    for (uint8_t c = 0; c < 4; c++) {
        for (int16_t i = 0; i < 7; i++) {
            for (int16_t j = 0; j < 7; j++) {
                if ((i == 6 && c != 0) || (j == 6 && c != 1)) {
                    _road_list[i][j].anims[c] = -1;
                    _road_list[i][j].connections[c] = 0;
                } else {
                    _road_list[i][j].anims[c] = anim_id++;
                    _road_list[i][j].connections[c] = 0;
                }
            }
        }
    }
}

void __cdecl Link_Buildings_() {
    int16_t prev_y = -1;
    int16_t prev_x = -1;

    for (int16_t i = 35; i >= 0; --i) {
        int16_t local_y;
        int16_t local_x;

        COLONY::Bldg_Back_To_Front_(i, (uint16_t*)&local_y, (uint16_t*)&local_x);

        if (COLONY::_colony_bldgs[local_y].buildings[local_x] != 0) {
            if (prev_y == -1) {
                prev_y = local_y;
                prev_x = local_x;
            } else {
                Draw_Simplest_Path_(prev_y, prev_x, local_y, local_x, &_DAT_5a7760, &_DAT_5a778a);
            }
        }
    }
}

void __cdecl Build_Road_List_Based_On_Bldg_List_() {
    Link_Buildings_();

    for (int16_t x = 0; x < 6; ++x) {
        for (int16_t y = 0; y < 6; ++y) {
            if (COLONY::_colony_bldgs[x].buildings[y] != 0) {
                if (random::Random_(2) == 1) {
                    _road_list[x][y].connections[0] = 1;
                } else {
                    _road_list[x][y].connections[1] = 1;
                }

                if (random::Random_(2) != 1) {
                    _road_list[x + 1][y].connections[0] = 1;
                }

                if (random::Random_(2) != 1) {
                    _road_list[x][y + 1].connections[1] = 1;
                }

                _road_list[x][y + 1].connections[2] = 0;
                _road_list[x][y + 1].connections[3] = 0;
            } else {
                if (_road_list[x][y + 1].connections[2] != 0 &&
                    _road_list[x][y + 1].connections[3] != 0) {
                    
                    if (random::Random_(2) == 1) {
                        _road_list[x][y + 1].connections[3] = 0;
                    } else {
                        _road_list[x][y + 1].connections[2] = 0;
                    }
                }

                if (N_Roads_Around_CR_(x, y) == 4) {
                    if (_road_list[x][y].connections[2] == 1 ||
                        _road_list[x][y].connections[3] == 1) {
                        
                        int16_t rand_x = x;
                        int16_t rand_y = y;
                        int16_t rand_dir = 0;
                        
                        if (Do_Pick_Random_Road_At_And_Around_CR_(&rand_x, &rand_y, &rand_dir) != 0) {
                            _road_list[rand_x][rand_y].connections[rand_dir] = 0;
                        }
                    }
                }
            }
        }
    }
}

int16_t __cdecl Pick_Random_Road_At_And_Around_CR_(int16_t* x, int16_t* y, int16_t* conn_idx, int16_t search_count) {
    int16_t best_x = *x;
    int16_t best_y = *y;
    int16_t best_conn = *conn_idx;
    int16_t match_count = 0;

    const uint8_t offsets[12] = {
        0, 0, 0,
        0, 0, 1,
        0, 1, 0,
        1, 0, 1
    };

    for (int16_t i = 0; i < search_count; ++i) {
        int32_t base_idx = i * 3;
        uint8_t dx = offsets[base_idx];
        uint8_t dy = offsets[base_idx + 1];
        uint8_t dconn = offsets[base_idx + 2];

        int16_t new_x = *x + dx;
        int16_t new_y = *y + dy;

        if (_road_list[new_x][new_y].connections[dconn] == *conn_idx) {
            match_count++;
            if (random::Random_(match_count) == 1) {
                best_x = new_x;
                best_y = new_y;
                best_conn = dconn;
            }
        }
    }

    *x = best_x;
    *y = best_y;
    *conn_idx = best_conn;

    return match_count;
}

uint8_t __cdecl Do_Pick_Random_Road_At_And_Around_CR_(int16_t* c, int16_t* r, int16_t* param_3) {
    return (uint8_t)Pick_Random_Road_At_And_Around_CR_(c, r, param_3, 4);
}

int16_t __cdecl N_Roads_Around_CR_(int16_t x, int16_t y) {
    uint8_t offsets[4][3] = {
        {0, 0, 0},
        {0, 0, 1},
        {0, 1, 0},
        {1, 0, 1}
    };

    int16_t count = 0;
    for (int16_t i = 0; i < 4; i++) {
        int16_t check_x = x + offsets[i][0];
        int16_t check_y = y + offsets[i][1];
        uint8_t dir = offsets[i][2];

        if (_road_list[check_x][check_y].connections[dir] != 0) {
            count++;
        }
    }

    return count;
}

void __cdecl Draw_Colony_Wee_Prod_(int16_t colony_idx, int16_t prod_idx, int16_t x, int16_t y, int16_t width, int16_t height) {
    Draw_Colony_Prod_Both_(colony_idx, prod_idx, x, y, width, height, 1);
}

void __cdecl Draw_Info_Wee_Morale_(int16_t colony_idx, int16_t x, int16_t y, int16_t width) {
    Draw_Info_Morale_Both_(colony_idx, x, y, (int)width, 1);
}

}