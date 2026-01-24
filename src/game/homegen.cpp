#include "deps.h"

namespace HOMEGEN {

    int32_t __cdecl Dist_Between_Stars_(uint16_t star_a, uint16_t star_b) {
        using Fn = int32_t(__cdecl*)(uint16_t, uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00519f90});
        return fn(star_a, star_b);
    }

    uint8_t __cdecl Build_Home_Star_List_(uint16_t* home_stars_out, int32_t min_dist) {
        MISC::ColorPair colors1 = *MISC::Font_Colors2_(4, 0x77, 0x79);
        MISC::ColorPair colors2 = *MISC::Font_Colors2_(4, 0x77, 0x7c);

        CandidateList candidates[72];
        for (int i = 0; i < 72; ++i) {
            candidates[i].count = 0;
            for (int j = 0; j < 20; ++j) candidates[i].stars[j] = 0;
        }

        if (MOX::_NUM_STARS > 1) {
            for (uint16_t current_star = 1; current_star < MOX::_NUM_STARS; ++current_star) {
                RUSS::Mox_Update_();

                CandidateList* list = &candidates[current_star]; // Note: Original code uses index up to NUM_STARS
                list->count = 1;
                list->stars[0] = current_star;

                if (MAPGEN::_fill_msg_ctr < _HOME_WORLD_GEN_MAX_CTR) {
                    fonts::Set_Font_Style_(4, &colors1);
                    int16_t width = fonts::Get_String_Width_(MAPGEN::_fill_msg);
                    MISC::Draw_Advancing_Fill_Message_(4, &MAPGEN::_fill_msg_ctr, (uint16_t)_HOME_WORLD_GEN_MAX_CTR,
                                                      &colors1, &colors2, (640 - width) / 2, 215,
                                                      MAPGEN::_fill_msg, MAPGEN::_fill_msg_bitmap, 0);
                    RUSS::Mox_Sync_Update_();
                } else {
                    Set_Up_Home_Gen_Msg_();
                }

                for (uint16_t star_j = 1; star_j < MOX::_NUM_STARS; ++star_j) {
                    RUSS::Mox_Update_();
                    if (star_j == current_star) continue;

                    uint8_t* field = _good_stars + (current_star * 299);
                    if (struct_::Test_Bit_Field_(field, star_j)) {
                        bool valid = true;
                        for (int k = 0; k < list->count; ++k) {
                            uint16_t existing_star = list->stars[k];
                            int32_t dist = Dist_Between_Stars_(existing_star, star_j);
                            uint8_t* field_k = _good_stars + (existing_star * 299);
                            uint16_t bit = struct_::Test_Bit_Field_(field_k, star_j);

                            RUSS::Mox_Update_();
                            if (existing_star != star_j && (bit == 0 || dist < min_dist)) {
                                valid = false;
                                break;
                            }
                        }

                        if (valid) {
                            // Original code has potential OOB here; we replicate it.
                            // stars[count] writes to 'count' if count == 20.
                            list->stars[list->count] = star_j;
                            int32_t next_count = (int32_t)list->count + 1;
                            if (next_count > 20) next_count = 20;
                            list->count = (uint16_t)next_count;
                        }
                    }
                }
            }
        }

        int16_t selected_index = -1;
        int32_t pick_count = 1;
        for (uint16_t si = 0; si < 72; ++si) {
            RUSS::Mox_Update_();
            if (candidates[si].count >= MOX::_NUM_PLAYERS) {
                if (random::Random_(pick_count) == 1) {
                    selected_index = (int16_t)si;
                    pick_count++;
                }
            }
        }

        if (selected_index >= 0) {
            CandidateList* sel_list = &candidates[selected_index];
            for (int i = 0; i < 4; ++i) {
                int idx1 = random::Random_(sel_list->count) - 1;
                int idx2 = random::Random_(sel_list->count) - 1;
                uint16_t tmp = sel_list->stars[idx1];
                sel_list->stars[idx1] = sel_list->stars[idx2];
                sel_list->stars[idx2] = tmp;
            }

            // Initialize output buffer (8 uint16_t slots as per 0xFFFFFFFF x 4)
            for (int i = 0; i < 8; ++i) home_stars_out[i] = 0xFFFF;

            for (int i = 0; i < (int)MOX::_NUM_PLAYERS; ++i) {
                home_stars_out[i] = sel_list->stars[i];
            }
            return 0; // Success
        }

        return 1; // Failure
    }

    uint32_t __cdecl Generate_Home_Worlds_() {
        uint32_t local_color1[2];
        uint32_t local_color2[2];
        uint16_t home_star_ids[8] = {};

        MISC::ColorPair* colors = MISC::Font_Colors2_(4, 0x77, 0x79);
        local_color1[0] = colors->c1;
        local_color1[1] = colors->c2;

        colors = MISC::Font_Colors2_(4, 0x77, 0x7c);
        local_color2[0] = colors->c1;
        local_color2[1] = colors->c2;

        // TODO: 72 * 299 bitmap (const?)
        _good_stars = static_cast<uint8_t*>(allocate::Allocate_First_Block_(MOX::_screen_seg, 0x5418));
        // TODO: memset
        uint32_t* p_stars = reinterpret_cast<uint32_t*>(_good_stars);
        for (int i = 0; i < 72; ++i) {
            p_stars[i] = 0;
        }

        for (int i = 0; i < 4; ++i) {
            _nearest_food_worlds[i] = 0xffffffff;
        }

        RUSS::Mox_Update_();
        video::Set_Refresh_Full_();

        int16_t base_players = (int16_t)MOX::_NUM_PLAYERS;
        int16_t adj_players = base_players;
        if (base_players > 6) {
            adj_players += (int16_t)MOX::_g_galaxy_size;
        }
        int16_t max_retries = adj_players + 4;

        RUSS::Mox_Update_();
        Set_Up_Home_Gen_Msg_();

        while (true) {
            RUSS::Mox_Update_();
            for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                MOX::_star[i].owner = -1;
            }

            int32_t x_dist = ((int32_t)MOX::_max_map_scale * (int32_t)MOX::_MAP_MAX_X) / (adj_players * 10);
            int32_t y_dist = ((int32_t)MOX::_MAP_MAX_Y * (int32_t)MOX::_MAP_MAX_X) / (adj_players * 10);
            int32_t min_dist_sq = x_dist * x_dist + y_dist * y_dist;

            if (min_dist_sq < 15000) {
                min_dist_sq = 15000;
            }

            if (MAPGEN::_fill_msg_ctr < (uint16_t)_HOME_WORLD_GEN_MAX_CTR) {
                fonts::Set_Font_Style_(4, (MISC::ColorPair*)local_color1);
                int16_t text_width = fonts::Get_String_Width_(MAPGEN::_fill_msg);
                int16_t x_pos = (0x280 - text_width) / 2;
                MISC::Draw_Advancing_Fill_Message_(4, &MAPGEN::_fill_msg_ctr, (uint16_t)_HOME_WORLD_GEN_MAX_CTR, (MISC::ColorPair*)local_color1, (MISC::ColorPair*)local_color2, x_pos, 215, MAPGEN::_fill_msg, MAPGEN::_fill_msg_bitmap, 0);
                RUSS::Mox_Sync_Update_();
            }

            RUSS::Mox_Update_();
            Build_Min_Star_Distances_(min_dist_sq);

            if (MAPGEN::_fill_msg_ctr < (uint16_t)_HOME_WORLD_GEN_MAX_CTR) {
                fonts::Set_Font_Style_(4, (MISC::ColorPair*)local_color1);
                int16_t text_width = fonts::Get_String_Width_(MAPGEN::_fill_msg);
                int16_t x_pos = (0x280 - text_width) / 2;
                MISC::Draw_Advancing_Fill_Message_(4, &MAPGEN::_fill_msg_ctr, (uint16_t)_HOME_WORLD_GEN_MAX_CTR, (MISC::ColorPair*)local_color1, (MISC::ColorPair*)local_color2, x_pos, 215, MAPGEN::_fill_msg, MAPGEN::_fill_msg_bitmap, 0);
                RUSS::Mox_Sync_Update_();
            }

            if (!Build_Home_Star_List_(home_star_ids, min_dist_sq)) {
                break;
            }

            RUSS::Mox_Update_();
            adj_players++;
            if (adj_players > max_retries) {
                return 0;
            }
        }

        Randomize_Home_Worlds_((int16_t*)home_star_ids);

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            MOX::_star[home_star_ids[i]].owner = (char)i;
        }

        Modify_Home_Worlds_();

        fonts::Set_Font_Style_(4, (MISC::ColorPair*)local_color1);
        int16_t text_width = fonts::Get_String_Width_(MAPGEN::_fill_msg);
        int16_t x_pos = (0x280 - text_width) / 2;
        MISC::Draw_Advancing_Fill_Message_(4, &MAPGEN::_fill_msg_ctr, (uint16_t)_HOME_WORLD_GEN_MAX_CTR, (MISC::ColorPair*)local_color1, (MISC::ColorPair*)local_color2, x_pos, 215, MAPGEN::_fill_msg, MAPGEN::_fill_msg_bitmap, 1);
        RUSS::Mox_Sync_Update_();

        FUN_0051ad50();
        return 1;
    }
}