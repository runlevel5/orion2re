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
}