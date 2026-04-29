#include "pch.h"

namespace COLMOVE {
    void __cdecl Draw_Cluster_(int16_t x, int16_t y) {
        if (_cluster_colony_n != -1 && !COLONY::_full_draw) {
            if (Get_First_Unemployed_(_cluster_colony_n) != -1) {
                if (!HAROLD::Was_Mouse_List_Saved_() && _g_hacky_dont_draw_cluster_flag != 1) {
                    fields::Enable_Draw_All_Fields_();
                    fields::Draw_Visible_Fields_();
                    fields::Disable_Draw_All_Fields_();

                    graphics::Set_Window_(0, 0, 639 /* 0x27f */, 479 /* 0x1df */);
                    video::Clip_On_();

                    s_animation_header* anim = COLONY::C_Anims_(15 /* 0xf */);
                    animate::Draw_(x, y, anim);

                    s_colony& col = MOX::_colony[_cluster_colony_n];
                    if (col.n_pops > 0) {
                        int16_t current_x = x + 5;
                        for (int16_t i = 0; i < col.n_pops; ++i) {
                            if (!(col.pop[i] & 0x200)) {
                                s_animation_header* pop_anim = COLONY::Colony_Pop_Anim_(_cluster_colony_n, i);
                                animate::Draw_(current_x, y - 10, pop_anim);
                                current_x += 20; // 0x14
                            }
                        }
                    }

                    video::Clip_Off_();
                }
            }
        }
    }

    void __cdecl Clear_Cluster_() {
        int16_t unemployed_idx = Get_First_Unemployed_(_cluster_colony_n);

        while (unemployed_idx != -1) {
            // Accessing population flags within the colony structure
            // s_colony size is 361 bytes, pop array starts at offset 0xC
            uint32_t& pop_flags = MOX::_colony[_cluster_colony_n].pop[unemployed_idx];

            if ((pop_flags & 0x200) == 0) {
                pop_flags |= 0x200;
            }

            unemployed_idx = Get_First_Unemployed_(_cluster_colony_n);
        }

        _cluster_colony_n = -1;
        COLONY::Reset_Mouse_Picture_();
    }

    void __cdecl Get_Cluster_(int16_t colony_idx, int16_t start_pop_idx) {
        s_colony* colony = &MOX::_colony[colony_idx];

        // Check if the starting population group is 'Natives' (type 9)
        if ((colony->pop[start_pop_idx] & 0x0F) == 9) {
            char* message = ESTRINGS::E_Strings_(0x17e /* ESTR_NATIVES_CANNOT_BE_MOVED_TO_ANOTHER_JOB_OR_PLANET */);
            GENDRAW::Help_(message);
            COLONY::Reset_Field_Mode_();
            return;
        }

        _cluster_colony_n = colony_idx;

        // Iterate through all populations in the colony starting from start_pop_idx
        for (int16_t i = start_pop_idx; i < (int16_t)(uint8_t)colony->n_pops; ++i) {
            if (Pops_Identical_(colony_idx, start_pop_idx, colony_idx, i)) {
                // Clear specific flag bit (0x200) for identical populations
                colony->pop[i] &= 0xFFFFFDFF;
            }
        }

        COLCALC::Colony_Calculation_(colony_idx);
        COLONY::Clear_Mouse_Picture_();
    }

    int16_t __cdecl Get_First_Unemployed_(int16_t colony_idx) {
        s_colony& colony = MOX::_colony[colony_idx];
        uint8_t n_pops = (uint8_t)colony.n_pops;

        if (n_pops != 0) {
            for (int16_t i = 0; i < (int16_t)n_pops; ++i) {
                if ((colony.pop[i] & 0x200) == 0) {
                    return i;
                }
            }
        }

        return -1;
    }

    int16_t __cdecl Get_Num_Unemployed_(int16_t colony_idx) {
        int16_t unemployed_count = 0;
        int16_t n_pops = (uint8_t)MOX::_colony[colony_idx].n_pops;

        for (int16_t i = 0; i < n_pops; ++i) {
            // TODO: Figure out bitmap
            if ((MOX::_colony[colony_idx].pop[i] & 0x200) == 0) {
                unemployed_count++;
            }
        }

        return unemployed_count;
    }

    bool __cdecl Pops_Identical_(int16_t param_1, int16_t param_2, int16_t param_3, int16_t param_4) {
        uint32_t pop1 = MOX::_colony[param_1].pop[param_2];
        uint32_t pop2 = MOX::_colony[param_3].pop[param_4];

        // Check first set of flags (bits 7 and 8)
        if (((pop1 ^ pop2) & 0x180) != 0) {
            return false;
        }

        int16_t state2 = COLONY::Pop_To_Pop_State_(param_3, param_4);
        int16_t state1 = COLONY::Pop_To_Pop_State_(param_1, param_2);

        // Compare derived population states
        if (state2 != state1) {
            return false;
        }

        // Check second set of flags (bits 0, 1, 2, 3, and 10)
        if (((pop2 ^ pop1) & 0x40f) != 0) {
            return false;
        }

        return true;
    }

    void __cdecl Send_Cluster_(int16_t colony_idx, int16_t param_2) {
        if (colony_idx < 0) {
            GENDRAW::Help_(ESTRINGS::E_Strings_(0x210 /* ESTR_THIS_IS_NOT_A_VALID_COLONY */));
            return;
        }

        int16_t msg_state = 0;
        int16_t moved_count = 0;
        uint8_t changed = 0;
        uint8_t flags[16];
        char local_41c[52];
        char planet1_name[20];
        char planet2_name[20];
        char local_3e8[1000];

        int16_t sVar3 = Get_First_Unemployed_(_cluster_colony_n);
        if (sVar3 != -1) {
            Colony_Pop_String_(_cluster_colony_n, sVar3, local_41c);

            while (sVar3 != -1) {
                if (colony_idx == _cluster_colony_n) {
                    uint32_t current_job = (MOX::_colony[_cluster_colony_n].pop[sVar3] & 0x180) >> 7;
                    if (param_2 == -1 || current_job == (uint32_t)param_2) {
                        MOX::_colony[_cluster_colony_n].pop[sVar3] |= 0x200;
                    } else {
                        if (Give_Colonist_New_Job_(_cluster_colony_n, sVar3, param_2) == 0) {
                            return;
                        }
                    }
                } else {
                    settler::Pop_Tries_To_Settle_(
                        &MOX::_colony[_cluster_colony_n],
                        sVar3,
                        &MOX::_colony[colony_idx],
                        flags
                    );

                    if (flags[0] != 0) {
                        if (flags[13] == 0 && flags[9] != 0) {
                            if (msg_state != 2) {
                                if (moved_count == 0) {
                                    _sprintf(local_3e8, ESTRINGS::E_Strings_(0x205));
                                } else {
                                    _sprintf(local_3e8, ESTRINGS::E_Strings_(0x203), moved_count, Get_Num_Unemployed_(_cluster_colony_n));
                                }
                                msg_state = 2;
                                COLONY::Reset_Mouse_Picture_();
                                flags[0] = (uint8_t)GENDRAW::Do_Confirmation_Box_(local_3e8);
                                COLONY::_full_draw = 1;
                                COLONY::Clear_Mouse_Picture_();
                            }
                        } else if (flags[13] != 0 && flags[9] == 0) {
                            if (msg_state != 1) {
                                _sprintf(local_3e8, ESTRINGS::E_Strings_(0x209), flags[13]);
                                msg_state = 1;
                                COLONY::Reset_Mouse_Picture_();
                                flags[0] = (uint8_t)GENDRAW::Do_Confirmation_Box_(local_3e8);
                                COLONY::_full_draw = 1;
                                COLONY::Clear_Mouse_Picture_();
                            }
                        } else if (flags[13] != 0 && flags[9] != 0) {
                            if (msg_state != 3) {
                                if (moved_count == 0) {
                                    _sprintf(local_3e8, ESTRINGS::E_Strings_(0x206), flags[13]);
                                } else {
                                    _sprintf(local_3e8, ESTRINGS::E_Strings_(0x204), moved_count, Get_Num_Unemployed_(_cluster_colony_n), flags[13]);
                                }
                                msg_state = 3;
                                COLONY::Reset_Mouse_Picture_();
                                flags[0] = (uint8_t)GENDRAW::Do_Confirmation_Box_(local_3e8);
                                COLONY::_full_draw = 1;
                                COLONY::Clear_Mouse_Picture_();
                            }
                        }

                        if (flags[0] != 0) {
                            if (flags[10] != 0) {
                                changed = 1;
                            }

                            uint32_t current_job = (MOX::_colony[_cluster_colony_n].pop[sVar3] & 0x180) >> 7;
                            if (current_job != (uint32_t)param_2 && param_2 != -1) {
                                if (Give_Colonist_New_Job_(_cluster_colony_n, sVar3, param_2) == 0) {
                                    return;
                                }
                                changed = 1;
                                MOX::_colony[_cluster_colony_n].pop[sVar3] &= 0xFFFFFDFF;
                            }

                            settler::Settle_Pop_(&MOX::_colony[_cluster_colony_n], sVar3, &MOX::_colony[colony_idx]);
                            moved_count++;

                            sVar3 = Get_First_Unemployed_(_cluster_colony_n);
                            continue;
                        }
                    }

                    sVar3 = -1;
                    int msg_count = 0;

                    if (flags[13] == 0) {
                        if (flags[9] != 0) {
                            ERIC::String_Builder2_(local_3e8, 0, ESTRINGS::E_Strings_(0xFD));
                            msg_count = 1;
                        }
                    } else {
                        if (flags[9] == 0) {
                            ERIC::String_Builder2_(local_3e8, 0, ESTRINGS::E_Strings_(0xFE), flags[13]);
                            msg_count = 1;
                        } else {
                            ERIC::String_Builder2_(local_3e8, 0, ESTRINGS::E_Strings_(0xFC), flags[13]);
                            msg_count = 1;
                        }
                    }

                    if (flags[4] != 0) {
                        ERIC::String_Builder2_(local_3e8, (int16_t)msg_count++, ESTRINGS::E_Strings_(0x1FD));
                    }
                    if (flags[5] != 0) {
                        ERIC::String_Builder2_(local_3e8, (int16_t)msg_count++, ESTRINGS::E_Strings_(0x241));
                    }
                    if (flags[7] != 0) {
                        ERIC::String_Builder2_(local_3e8, (int16_t)msg_count++, ESTRINGS::E_Strings_(0x24D));
                    }
                    if (flags[2] != 0) {
                        ERIC::String_Builder2_(local_3e8, (int16_t)msg_count++, ESTRINGS::E_Strings_(0x207));
                    }
                    if (flags[6] != 0) {
                        ERIC::String_Builder2_(local_3e8, (int16_t)msg_count++, ESTRINGS::E_Strings_(0x184));
                    }
                    if (flags[1] != 0) {
                        ERIC::String_Builder2_(local_3e8, (int16_t)msg_count++, ESTRINGS::E_Strings_(0x211));
                    }
                    if (flags[10] != 0) {
                        ERIC::String_Builder2_(local_3e8, (int16_t)msg_count++, ESTRINGS::E_Strings_(0x20F));
                    }
                    if (flags[14] != 0) {
                        ERIC::String_Builder2_(local_3e8, (int16_t)msg_count++, ESTRINGS::E_Strings_(0x20E));
                    }

                    if (msg_count <= 0) {
                        COLONY::Reset_Mouse_Picture_();
                        COLONY::_full_draw = 1;
                        GENDRAW::Help_(ESTRINGS::E_Strings_(0xE4));
                        COLONY::Clear_Mouse_Picture_();
                    } else {
                        if (moved_count > 0) {
                            ERIC::String_Builder2_(local_3e8, (int16_t)msg_count, ESTRINGS::E_Strings_(0x80), moved_count);
                        }
                        COLONY::Reset_Mouse_Picture_();
                        COLONY::_full_draw = 1;
                        TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x1D6), local_3e8);
                        COLONY::Clear_Mouse_Picture_();
                    }
                }

                if (sVar3 != -1) {
                    sVar3 = Get_First_Unemployed_(_cluster_colony_n);
                }
            }
        }

        COLONY::Col_Calc_Wrapper_(_cluster_colony_n);
        if (colony_idx != _cluster_colony_n) {
            COLONY::Col_Calc_Wrapper_(colony_idx);
        }

        if (moved_count > 0) {
            strcpy(planet1_name, HACCESS::Get_Planet_Name_(MOX::_colony[_cluster_colony_n].planet));
            strcpy(planet2_name, HACCESS::Get_Planet_Name_(MOX::_colony[colony_idx].planet));

            ERIC::String_Builder2_(local_3e8, 0, ESTRINGS::E_Strings_(0x1D1), moved_count, local_41c, planet1_name, planet2_name, flags[13]);

            int16_t end_msg_count = 1;
            if (flags[11] != 0) {
                ERIC::String_Builder2_(local_3e8, end_msg_count++, ESTRINGS::E_Strings_(0x84));
            }
            if (flags[10] != 0) {
                ERIC::String_Builder2_(local_3e8, end_msg_count++, ESTRINGS::E_Strings_(0x85));
            }
            if (changed != 0) {
                ERIC::String_Builder2_(local_3e8, end_msg_count++, ESTRINGS::E_Strings_(0x83));
            }

            COLONY::Reset_Mouse_Picture_();
            TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x1D5), local_3e8);
            COLONY::Clear_Mouse_Picture_();

            COLCALC::Colony_Calculation_(_cluster_colony_n);
            COLCALC::Colony_Calculation_(colony_idx);
        }

        if (Get_First_Unemployed_(_cluster_colony_n) == -1) {
            COLONY::Reset_Mouse_Picture_();
            COLONY::Reset_Field_Mode_();
            _cluster_colony_n = -1;
        }
    }

    uint8_t __cdecl Give_Colonist_New_Job_(int16_t colony_idx, int16_t pop_idx, int16_t new_job) {
        int16_t pop_state = COLONY::Pop_To_Pop_State_(colony_idx, pop_idx);

        if ((pop_state == 3 || pop_state == 6) && (new_job == 2 || new_job == 1)) {
            GENDRAW::Help_(ESTRINGS::E_Strings_(0x20A));
            return 0;
        }

        if (pop_state == 4) {
            int16_t current_job = (MOX::_colony[colony_idx].pop[pop_idx] & 0x180) >> 7;
            if (current_job != new_job) {
                GENDRAW::Help_(ESTRINGS::E_Strings_(0x20C));
                return 0;
            }
        }

        int16_t sum_colonists = COLONY::Sum_Colonists_(colony_idx, new_job, -1, -1, 0, -1);
        if (sum_colonists >= 42) {
            GENDRAW::Help_(ESTRINGS::E_Strings_(0x187));
            return 0;
        }

        uint8_t max_farms = MOX::_colony[colony_idx].max_farms;
        int16_t sum_colonists_again = COLONY::Sum_Colonists_(colony_idx, new_job, -1, -1, 0, -1);

        if (max_farms > sum_colonists_again || new_job != 0) {
            uint32_t pop_val = MOX::_colony[colony_idx].pop[pop_idx];
            uint32_t job_shifted = (uint32_t)new_job << 7;

            MOX::_colony[colony_idx].pop[pop_idx] = ((job_shifted ^ pop_val) & 0x180) ^ pop_val;
            MOX::_colony[colony_idx].pop[pop_idx] |= 0x200;

            return 1;
        }

        GENDRAW::Help_(ESTRINGS::E_Strings_(0x187));
        return 0;
    }
}