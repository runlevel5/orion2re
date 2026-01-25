#include "deps.h"

namespace HOMEGEN {

    uint32_t __cdecl Dist_Between_Stars_(uint16_t star_a, uint16_t star_b) {
        using Fn = uint32_t(__cdecl*)(uint16_t, uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00519f90});
        return fn(star_a, star_b);
    }

    void __cdecl Build_Min_Star_Distances_(int32_t min_distance) {
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_STARS; ++i) {
            MOX::Mox_Update_();

            s_good_star* current_star = &_good_stars[i];
            current_star->star_count = 0;

            for (int16_t j = 0; j < (int16_t)MOX::_NUM_STARS; ++j) {
                MOX::Mox_Update_();

                uint32_t dist = Dist_Between_Stars_(i, j);

                if (dist >= (uint32_t)min_distance) {
                    struct_::Set_Bit_Field_(current_star, j);

                    // Note: Potential out-of-bounds access if MOX::_NUM_STARS > 72
                    current_star->star_distances[j] = dist;
                    current_star->star_count++;
                }
            }
        }
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
                MOX::Mox_Update_();

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
                    MOX::Mox_Update_();
                    if (star_j == current_star) continue;

                    uint8_t* field = (uint8_t*)&_good_stars[current_star];
                    if (struct_::Test_Bit_Field_(field, star_j)) {
                        bool valid = true;
                        for (int k = 0; k < list->count; ++k) {
                            uint16_t existing_star = list->stars[k];
                            uint32_t dist = Dist_Between_Stars_(existing_star, star_j);
                            uint8_t* field_k = (uint8_t*)&_good_stars[existing_star];
                            uint16_t bit = struct_::Test_Bit_Field_(field_k, star_j);

                            MOX::Mox_Update_();
                            if (existing_star != star_j && (bit == 0 || dist < (uint32_t)min_dist)) {
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
            MOX::Mox_Update_();
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

    /**
     * @brief Randomizes elements in the home world array by performing 100 swaps.
     *
     * @param arr Pointer to an array of world identifiers (shorts).
     */
    void __cdecl Randomize_Home_Worlds_(int16_t* arr) {
        for (int32_t i = 0; i < 100; ++i) {
            // Random_ likely returns a value from 1 to N, so we subtract 1 for 0-based indexing.
            int16_t idx1 = static_cast<int16_t>(random::Random_(static_cast<int32_t>(MOX::_NUM_PLAYERS))) - 1;
            int16_t idx2 = static_cast<int16_t>(random::Random_(static_cast<int32_t>(MOX::_NUM_PLAYERS))) - 1;

            if (idx1 != idx2) {
                int16_t temp = arr[idx1];
                arr[idx1] = arr[idx2];
                arr[idx2] = temp;
            }
        }
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
        _good_stars = static_cast<s_good_star*>(allocate::Allocate_First_Block_(MOX::_screen_seg, 0x5418));
        // TODO: memset
        uint32_t* p_stars = reinterpret_cast<uint32_t*>(_good_stars);
        for (int i = 0; i < 72; ++i) {
            p_stars[i] = 0;
        }

        for (int i = 0; i < 4; ++i) {
            _nearest_food_worlds[i] = 0xffffffff;
        }

        MOX::Mox_Update_();
        video::Set_Refresh_Full_();

        int16_t base_players = (int16_t)MOX::_NUM_PLAYERS;
        int16_t adj_players = base_players;
        if (base_players > 6) {
            adj_players += (int16_t)MOX::_g_galaxy_size;
        }
        int16_t max_retries = adj_players + 4;

        MOX::Mox_Update_();
        Set_Up_Home_Gen_Msg_();

        while (true) {
            MOX::Mox_Update_();
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

            MOX::Mox_Update_();
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

            MOX::Mox_Update_();
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

    void __cdecl Set_Up_Home_Gen_Msg_() {
        uint32_t font_style_data[4];

        // Retrieve and cache font colors for rendering
        MISC::ColorPair* colors1 = MISC::Font_Colors2_(4, 0x77, 0x79);
        font_style_data[0] = colors1->c1;
        font_style_data[1] = colors1->c2;

        MISC::ColorPair* colors2 = MISC::Font_Colors2_(4, 0x77, 0x7C);
        font_style_data[2] = colors2->c1;
        font_style_data[3] = colors2->c2;

        // Determine home world generation counter limits
        _HOME_WORLD_GEN_MAX_CTR = (static_cast<int32_t>(MOX::_NUM_STARS) * 25) / 20;
        if (MOX::_NUM_STARS > 54) {
            _HOME_WORLD_GEN_MAX_CTR = 372;
        }

        // Potential redundant logic preserved from binary: 
        // concatenates companion star name if fill_msg is short, 
        // but fill_msg is completely overwritten later by H_Message.
        if (helpers::strlen(MAPGEN::_fill_msg) < 35) {
            helpers::strcat(MAPGEN::_fill_msg, MAPGEN::_companion_star);
        }

        video::Set_Page_Back_();
        void* lbx_buf = Buffer_Reload_(MAPGEN::s_raceopt_lbx, 0, MOX::_global_cache_seg);
        animate::Draw_(0, 0, lbx_buf);
        video::Copy_Back_To_Off_();

        // Initialize background bitmap for the progress message
        uint32_t bmp_size = bitmap::Get_Bitmap_Size_(325, 30);
        MAPGEN::_fill_msg_bitmap = allocate::Allocate_Next_Block_(MOX::_screen_seg, bmp_size);
        bitmap::Create_Blank_Bitmap_(325, 30, MAPGEN::_fill_msg_bitmap);
        MAPGEN::_fill_msg_ctr = 0;

        // Load specific UI string and render it to the cached bitmap
        const char* msg_str = HAROLD::H_Message_(0xA1);
        helpers::strcpy(MAPGEN::_fill_msg, msg_str);

        fonts::Set_Font_Style_Outline_Heavy_(4, font_style_data);
        fonts::Set_Outline_Color_(2, 1);
        fonts::Print_To_Bitmap_(0, 0, reinterpret_cast<uint8_t*>(MAPGEN::_fill_msg), MAPGEN::_fill_msg_bitmap);
    }

    /**
     * Ensures that a star system has at least 3 planetary bodies (planets, asteroid belts, or gas giants).
     * If the spectral class is not conducive to life/planets, it rerolls the star's class.
     * Then it populates empty orbits until the minimum count is met.
     */
    void __cdecl Enforce_3_Planet_Minimum_(int16_t star_idx) {
        int8_t planets_in_system = HAROLD::N_Planets_In_System_(star_idx);

        if (planets_in_system < 3) {
            // Ensure the star has a spectral class capable of hosting a system
            if (!HAROLD::Habitable_Spectral_Class_(star_idx)) {
                do {
                    MOX::_star[star_idx].spectral_class = MAPGEN::Generate_Spectral_Class_(MOX::_settings.galaxy_age);
                } while (!HAROLD::Habitable_Spectral_Class_(star_idx));
            }

            int32_t planets_to_add = 3 - (int32_t)planets_in_system;
            for (int32_t i = 0; i < planets_to_add; ++i) {
                int16_t orbit = Usable_Orbit_(star_idx);
                int16_t existing_planet_idx = MOX::_star[star_idx].planet_index[orbit];

                if (existing_planet_idx == -1) {
                    // Create a brand new planet record
                    Make_One_Planet_(star_idx, MOX::_planet_count, orbit, 0);
                    MOX::_planet_count++;
                } else {
                    // Convert an existing non-planet object (e.g. asteroid belt) into a planet
                    uint16_t saved_global_count = MOX::_planet_count;
                    MOX::_planet_count = (uint16_t)existing_planet_idx;

                    MOX::_planet[existing_planet_idx].orbit = (int8_t)orbit;
                    MOX::_planet[existing_planet_idx].planet_type = 3; // PLANET_TYPE_PLANET

                    MAPGEN::Generate_Satellite_(star_idx);
                    
                    MOX::_planet_count = saved_global_count;
                }
            }
        }
    }

    /**
     * @brief Configures homeworld systems for all players based on their racial traits.
     * Ensures systems have minimum planet counts, appropriate climates, sizes, and mineral richness.
     */
    void __cdecl Modify_Home_Worlds_() {
        int16_t home_star_ids[9];
        int16_t home_planet_id;
        int16_t home_star_count = 0;

        home_planet_id = 0;
        home_star_ids[1] = -1;
        home_star_ids[2] = -1;
        home_star_ids[3] = -1;
        home_star_ids[4] = -1;
        home_star_ids[5] = -1;
        home_star_ids[6] = -1;
        home_star_ids[7] = -1;
        home_star_ids[8] = -1;

        // Pass 1: Collect star IDs of all home worlds
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_STARS; ++i) {
            int8_t owner = MOX::_star[i].owner;
            if (owner >= 0 && owner < 8) {
                home_star_ids[home_star_count + 1] = i;
                home_star_count++;
            }
        }

        // Pass 2: Process and enforce traits on each home world
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_STARS; ++i) {
            int16_t owner_idx = (int16_t)MOX::_star[i].owner;
            if (owner_idx >= 0 && owner_idx < 8) {
                s_player* player = &MOX::_player[owner_idx];

                Enforce_3_Planet_Minimum_(i);

                int16_t climate_threshold = 8;
                if (player->traits[15] != 0) { // Aquatic
                    climate_threshold = 5;
                }

                Enforce_Climate_(i, climate_threshold, &home_planet_id);
                Remove_Specials_(i);

                player->home_planet_id = home_planet_id;

                if (player->traits[17] == 0) { // Not Large Home World
                    Enforce_Medium_Planet_Size_(home_planet_id);
                } else {
                    Set_Planet_Size(home_planet_id, 3); // PLANET_SIZE_LARGE
                }

                Enforce_Planet_Type_(i, home_planet_id);
                Enforce_Planet_Abundant_Mineral_Class(home_planet_id);

                if (player->traits[18] > 0) { // Rich Home World
                    Enforce_Mineral_Richness_(home_planet_id, 3);
                } else if (player->traits[18] < 0) { // Poor Home World
                    Enforce_Mineral_Richness_(home_planet_id, 1);
                }

                if (player->traits[19] != 0) { // Ancient Home World (Artifacts)
                    Set_System_Planet_Artifact_Special(home_planet_id, i);
                }

                int8_t gravity_type;
                if (player->traits[13] != 0) { // Low-G World
                    gravity_type = 0;
                } else {
                    gravity_type = (player->traits[14] == 0) ? 1 : 2; // Normal-G or Heavy-G
                }
                Enforce_Gravity_(home_planet_id, gravity_type, owner_idx);

                ERICNET::Init_Homeworld_Colony_(home_planet_id, owner_idx);

                Guarantee_A_Planet_At_Min_Dist_(i, 4, &home_star_ids[1]);

                if (MOX::_star[i].system_special == MOX::SYSTEM_SPECIAL_SPACE_MONSTER) {
                    MOX::_star[i].system_special = MOX::SYSTEM_SPECIAL_NO_SPECIAL;
                }
            }
        }

        MOX::_NUM_COLONIES = MOX::_NUM_PLAYERS;
        Assign_Home_Star_Names_();
        Ensure_At_Least_1_Food_Planet_();
    }
}