#include "deps.h"

namespace HOMEGEN {
    void __cdecl Guarantee_A_Planet_At_Min_Dist_(int16_t origin_star_idx, int16_t min_dist, int16_t* player_home_stars) {
        _star_list = (s_star_dist*)allocate::Allocate_First_Block_(MOX::_global_data_seg, 0x120);

        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            _star_list[i].dist = 999;
            _star_list[i].index = 999;
        }

        bool planet_found = false;
        for (int16_t i = 1; i < MOX::_NUM_STARS; ++i) {
            if (i == origin_star_idx) continue;

            _star_list[i].index = i;
            int16_t dist = GEO::Parsecs_Between_Stars_(origin_star_idx, i);
            _star_list[i].dist = dist;

            if (Star_Has_Planet_(i) && dist <= min_dist) {
                planet_found = true;
                break;
            }
        }

        if (!planet_found) {
            _qsort(_star_list, MOX::_NUM_STARS, sizeof(s_star_dist), qsort_star_dist_list_);

            int16_t list_idx = 0;
            bool target_found = false;
            while (list_idx < MOX::_NUM_STARS) {
                int16_t s_idx = _star_list[list_idx].index;
                bool is_home_star = false;
                for (int16_t p = 0; p < MOX::_NUM_PLAYERS; ++p) {
                    if (player_home_stars[p] == s_idx) {
                        is_home_star = true;
                        break;
                    }
                }
                if (!is_home_star) {
                    target_found = true;
                    break;
                }
                list_idx++;
            }

            if (!target_found) return;

            int16_t target_star_idx = _star_list[list_idx].index;

            if (!Star_Has_Planet_(target_star_idx)) {
                s_star_data* s = &MOX::_star[target_star_idx];
                s->spectral_class = 4; // M-class (Red)

                int32_t r_size = random::Random_(10) - 1;
                if (r_size < 3) s->size = 0;
                else if (r_size < 7) s->size = 1;
                else s->size = 2;

                s->pict_type = (char)(random::Random_(3) - 1);

                for (int j = 0; j < 5; ++j) {
                    s->planet_index[j] = -1;
                }

                for (int j = 0; j < 2; ++j) {
                    char orbit = MAPGEN::Generate_Orbit_(target_star_idx);
                    s->planet_index[(uint8_t)orbit] = MOX::_planet_count;

                    s_planet_data* p = &MOX::_planet[MOX::_planet_count];
                    p->orbit = orbit;
                    p->planet_type = 3; // Terran

                    MAPGEN::Generate_Satellite_(target_star_idx);
                    MOX::_planet_count++;
                }
            }

            while (GEO::Parsecs_Between_Stars_(origin_star_idx, target_star_idx) > min_dist) {
                int16_t delta_x, delta_y;
                CMBTMIS::Coord_Movement_(
                    MOX::_star[target_star_idx].x, MOX::_star[target_star_idx].y,
                    MOX::_star[origin_star_idx].x, MOX::_star[origin_star_idx].y,
                    10, &delta_x, &delta_y
                );
                MOX::_star[target_star_idx].x += delta_x;
                MOX::_star[target_star_idx].y += delta_y;
            }
        }
    }

    uint32_t __cdecl Dist_Between_Stars_(int16_t star_a, int16_t star_b) {
        using Fn = uint32_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00519f90});
        return fn(star_a, star_b);
    }

    int32_t __cdecl qsort_star_dist_list_(const void* param_1, const void* param_2) {
        const int16_t val1 = *reinterpret_cast<const int16_t*>(param_1);
        const int16_t val2 = *reinterpret_cast<const int16_t*>(param_2);

        if (val1 == val2) {
            return 0;
        }

        return (val1 > val2) ? 1 : -1;
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
        MISC::s_colors colors1 = *MISC::Font_Colors2_(4, 0x77, 0x79);
        MISC::s_colors colors2 = *MISC::Font_Colors2_(4, 0x77, 0x7c);

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
    /**
     * Checks if a given world ID is already present in the nearest food worlds list.
     * The list size is bounded by the number of players.
     */
    bool __cdecl Is_Added_Food_World_(int16_t world_id) {
        int16_t num_players = static_cast<int16_t>(MOX::_NUM_PLAYERS);

        // Initial check matches 0051b19e CMP SI,AX ; 0051b1a1 JLE ...
        if (num_players <= 0) {
            return false;
        }

        for (int16_t i = 0; i < num_players; ++i) {
            // Disassembly uses a short-circuit check at the start of loop iterations (0051b1a8 TEST AL, AL)
            // which is equivalent to returning true immediately upon finding the world.
            if (_nearest_food_worlds[i] == world_id) {
                return true;
            }
        }

        return false;
    }

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
        uint16_t home_star_ids[8] = {};

        MISC::s_colors local_color1 = *MISC::Font_Colors2_(4, 0x77, 0x79);
        MISC::s_colors local_color2 = *MISC::Font_Colors2_(4, 0x77, 0x7c);

        // TODO: 72 * 299 bitmap (const?)
        _good_stars = static_cast<s_good_star*>(allocate::Allocate_First_Block_(MOX::_screen_seg, 0x5418));
        // TODO: memset
        uint32_t* p_stars = reinterpret_cast<uint32_t*>(_good_stars);
        for (int i = 0; i < 72; ++i) {
            p_stars[i] = 0;
        }

        for (int i = 0; i < 8; ++i) {
            _nearest_food_worlds[i] = -1;
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

            int32_t dx = ((int32_t)MOX::_MAP_MAX_X * (int32_t)MOX::_max_map_scale) / (adj_players * 10);
            int32_t dy = ((int32_t)MOX::_MAP_MAX_Y * (int32_t)MOX::_max_map_scale) / (adj_players * 10);
            int32_t min_dist_sq = dx * dx + dy * dy;

            if (min_dist_sq < 15000) {
                min_dist_sq = 15000;
            }

            if (MAPGEN::_fill_msg_ctr < (uint16_t)_HOME_WORLD_GEN_MAX_CTR) {
                fonts::Set_Font_Style_(4, &local_color1);
                int16_t text_width = fonts::Get_String_Width_(MAPGEN::_fill_msg);
                int16_t x_pos = (0x280 - text_width) / 2;
                MISC::Draw_Advancing_Fill_Message_(4, &MAPGEN::_fill_msg_ctr, (uint16_t)_HOME_WORLD_GEN_MAX_CTR, &local_color1, &local_color2, x_pos, 215, MAPGEN::_fill_msg, MAPGEN::_fill_msg_bitmap, 0);
                RUSS::Mox_Sync_Update_();
            }

            MOX::Mox_Update_();
            Build_Min_Star_Distances_(min_dist_sq);

            if (MAPGEN::_fill_msg_ctr < (uint16_t)_HOME_WORLD_GEN_MAX_CTR) {
                fonts::Set_Font_Style_(4, &local_color1);
                int16_t text_width = fonts::Get_String_Width_(MAPGEN::_fill_msg);
                int16_t x_pos = (0x280 - text_width) / 2;
                MISC::Draw_Advancing_Fill_Message_(4, &MAPGEN::_fill_msg_ctr, (uint16_t)_HOME_WORLD_GEN_MAX_CTR, &local_color1, &local_color2, x_pos, 215, MAPGEN::_fill_msg, MAPGEN::_fill_msg_bitmap, 0);
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

        fonts::Set_Font_Style_(4, &local_color1);
        int16_t text_width = fonts::Get_String_Width_(MAPGEN::_fill_msg);
        int16_t x_pos = (0x280 - text_width) / 2;
        MISC::Draw_Advancing_Fill_Message_(4, &MAPGEN::_fill_msg_ctr, (uint16_t)_HOME_WORLD_GEN_MAX_CTR, &local_color1, &local_color2, x_pos, 215, MAPGEN::_fill_msg, MAPGEN::_fill_msg_bitmap, 1);
        RUSS::Mox_Sync_Update_();

        Generate_Orion_();
        return 1;
    }

    void __cdecl Set_Up_Home_Gen_Msg_() {
        // Retrieve and cache font colors for rendering
        MISC::s_colors* colors1 = MISC::Font_Colors2_(4, 0x77, 0x79);
        MISC::s_colors* colors2 = MISC::Font_Colors2_(4, 0x77, 0x7C);

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
        animate::s_animation_header* lbx_buf = reinterpret_cast<animate::s_animation_header*>(Buffer_Reload_(MAPGEN::s_raceopt_lbx, 0, MOX::_global_cache_seg));
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

        fonts::Set_Font_Style_Outline_Heavy_(4, colors1);
        fonts::Set_Outline_Color_(2, 1);
        fonts::Print_To_Bitmap_(0, 0, MAPGEN::_fill_msg, MAPGEN::_fill_msg_bitmap);
    }

    /**
     * Ensures that a star system has at least 3 planetary bodies (planets, asteroid belts, or gas giants).
     * If the spectral class is not conducive to life/planets, it rerolls the star's class.
     * Then it populates empty orbits until the minimum count is met.
     */
    void __cdecl Assign_Home_Star_Names_() {
        uint8_t is_race_assigned[14];
        for (int i = 0; i < 14; ++i) {
            is_race_assigned[i] = 0;
        }

        char* star_names = (char*)farload::Far_Reload_Data_(s_starname::lbx_00558b30, 0, MOX::_screen_seg, 0, 0x0D, 0x0F);

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            s_player* plr = &MOX::_player[i];
            int race_idx = (int)plr->race;

            if (is_race_assigned[race_idx] == 0) {
                char* src_name = &star_names[race_idx * 15];
                int16_t home_planet_id = plr->home_planet_id;
                int8_t star_idx = (int8_t)MOX::_planet[home_planet_id].star_index;
                s_star_data* star = &MOX::_star[star_idx];

                char* dst = star->name;
                char* src = src_name;
                while ((*dst++ = *src++) != '\0');

                is_race_assigned[race_idx] = 1;
            }
        }
    }

    /**
     * @brief Generates space monsters at eligible star systems based on user preferences and map scale.
     */
    void __cdecl Make_System_Monsters_() {
        int scaled_x = HAROLD::Get_Scaled_Value_(MOX::_max_map_scale);
        int scaled_y = HAROLD::Get_Scaled_Value_(MOX::_MAP_MAX_Y);
        uint32_t dist_limit_sq = static_cast<uint32_t>(scaled_x * scaled_x + scaled_y * scaled_y);

        if (MOX2::_user_wants_n_space_monsters == 0xFF) {
            if (MAPGEN::_n_space_monsters > 20) {
                MAPGEN::_n_space_monsters = 20;
            }
        } else {
            int invalid_stars_count = 0;
            for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                // Determine stars that are ineligible for monsters
                if (HAROLD::Star_Is_Orion_(i) ||
                    HAROLD::Is_Home_Star_(i) ||
                    HAROLD::Is_Colony_Star_(i) ||
                    Is_Added_Food_World_(i) ||
                    HAROLD::Empty_System_(i) ||
                    MOX::_star[i].spectral_class == 6) // Black Hole
                {
                    invalid_stars_count++;
                }
            }

            int valid_stars_count = MOX::_NUM_STARS - invalid_stars_count;
            int desired_monsters = static_cast<int>(MOX2::_user_wants_n_space_monsters);

            if (desired_monsters > valid_stars_count) {
                desired_monsters = valid_stars_count;
            }

            MAPGEN::_n_space_monsters = static_cast<uint8_t>(desired_monsters);
            MOX2::_user_wants_n_space_monsters = MAPGEN::_n_space_monsters;
            // Disable distance constraints for custom monster counts
            dist_limit_sq = 0;
        }

        if (MAPGEN::_n_space_monsters == 0) {
            return;
        }

        int16_t weights[72];
        uint8_t flags[72];
        int16_t placed_monster_stars[72];

        helpers::memset(weights, 0, sizeof(weights));
        helpers::memset(flags, 0, sizeof(flags));
        helpers::memset(placed_monster_stars, 0xFF, sizeof(placed_monster_stars));

        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            if (!HAROLD::Star_Is_Orion_(i) &&
                !Is_Added_Food_World_(i) &&
                !HAROLD::Is_Home_Star_(i) &&
                !HAROLD::Is_Colony_Star_(i) &&
                !HAROLD::Empty_System_(i) &&
                MOX::_star[i].spectral_class != 6)
            {
                weights[i] = 1;
            }
        }

        for (int m = 0; m < static_cast<int>(MAPGEN::_n_space_monsters); ++m) {
            int divisor = (m * 2 > 1) ? (m * 2) : 1;
            uint32_t current_min_dist = dist_limit_sq / divisor;

            int16_t selected_star = -1;
            int attempts = 0;
            bool success = false;

            do {
                int16_t star_idx = RUSS::Get_Weighted_Choice_Short_(weights, MOX::_NUM_STARS);
                bool dist_ok = true;

                for (int j = 0; j < m; ++j) {
                    if (Dist_Between_Stars_(star_idx, placed_monster_stars[j]) < current_min_dist) {
                        dist_ok = false;
                        break;
                    }
                }

                if (dist_ok || attempts > 250) {
                    selected_star = star_idx;
                    success = true;
                } else {
                    attempts++;
                }
            } while (!success);

            if (selected_star == -1) {
                return;
            }

            s_star_data& star = MOX::_star[selected_star];
            // Store original system special for later use
            star.not_used1 = static_cast<uint8_t>(star.system_special);
            star.system_special = SYSTEM_SPECIAL_SPACE_MONSTER;

            flags[selected_star] = 1;
            weights[selected_star] = 0;
            placed_monster_stars[m] = selected_star;
        }
    }

    void __cdecl Make_One_Planet_(int16_t star_idx, int16_t planet_idx, int16_t orbit_idx, int8_t is_food_planet) {
        s_star_data& star = MOX::_star[star_idx];

        if (is_food_planet != 0 && star.spectral_class >= 5) {
            star.spectral_class = (int8_t)(random::Random_(5) - 1);
        }

        MAPGEN::_make_food_planet = (uint8_t)is_food_planet;

        s_planet_data& planet = MOX::_planet[planet_idx];

        planet.colony_index = 0;
        planet.star_index = 0;
        planet.orbit = 0;
        planet.planet_type = 0;
        planet.size = 0;
        planet.gravity_class = 0;
        planet.group = 0;
        planet.climate = 0;
        planet.climate_bg_type = 0;
        planet.mineral_class = 0;
        planet.food_per_farmer = 0;
        planet.n_terraforms = 0;
        planet.max_farms = 0;
        planet.max_population = 0;
        planet.planet_special = 0;
        planet.environmental_alterations = 0;

        planet.orbit = (int8_t)orbit_idx;
        planet.planet_type = 3;

        MAPGEN::Generate_Satellite_(star_idx);
        MAPGEN::_make_food_planet = 0;

        if (MOX::_planet_count == planet_idx) {
            int16_t existing_idx = star.planet_index[orbit_idx];
            if (existing_idx >= 0) {
                MOX::_planet[existing_idx] = planet;
                MOX::_planet_count--;
                return;
            }
        }

        star.planet_index[orbit_idx] = planet_idx;
    }

    int16_t __cdecl Usable_Orbit_(int16_t star_idx) {
        int16_t result_orbit = -1;
        bool found = false;

        // Try 101 times to find a random usable orbit
        for (int i = 0; i <= 100; ++i) {
            if (found) break;

            int r = random::Random_(5);
            int16_t orbit_candidate = static_cast<int16_t>(r - 1);

            if (orbit_candidate < 0 || orbit_candidate > 4) continue;

            int16_t p_idx = MOX::_star[star_idx].planet_index[orbit_candidate];

            if (p_idx == -1) {
                result_orbit = orbit_candidate;
                found = true;
            } else if (p_idx >= 0 && p_idx < MOX::_planet_count) {
                if (MOX::_planet[MOX::_planet_count].planet_type != PLANET_TYPE_PLANET) {
                    result_orbit = orbit_candidate;
                    found = true;
                }
            }
        }

        // Sequential search fallback if random attempts fail
        if (!found) {
            for (int16_t i = 0; i < 5; ++i) {
                int16_t p_idx = MOX::_star[star_idx].planet_index[i];
                if (p_idx == -1 || MOX::_planet[p_idx].planet_type != PLANET_TYPE_PLANET) {
                    result_orbit = i;
                    found = true;
                    break;
                }
            }
        }

        // Final fallback: just pick a random orbit if search failed
        if (result_orbit == -1) {
            result_orbit = static_cast<int16_t>(random::Random_(5) - 1);
        }

        return result_orbit;
    }

    int16_t __cdecl Random_Planet_(int16_t star_idx) {
        int16_t weights[5] = {0, 0, 0, 0, 0};
        const int16_t* planet_indices = MOX::_star[star_idx].planet_index;

        for (int16_t i = 0; i < 5; ++i) {
            int16_t p_idx = planet_indices[i];
            if (p_idx != -1) {
                int8_t type = MOX::_planet[p_idx].planet_type;
                if (type > 0 && type < 4) {
                    weights[i] = 1;
                }
            }
        }

        uint16_t choice_idx = RUSS::Get_Weighted_Choice_Short_(weights, 5);
        return planet_indices[choice_idx];
    }

    void __cdecl Set_Special_For_Star_Guarded_By_Monster_(s_star_data* star, int16_t monster_type) {
        char race_name[200];
        int16_t star_idx = static_cast<int16_t>(star - MOX::_star);
        int16_t need_new_special = 0;

        HACCESS::Race_Name_(monster_type, race_name);

        int32_t monster_mask = 2;
        if (monster_type == 12) {
            monster_mask = 0xA96;
        } else if (monster_type == 13) {
            monster_mask = 0xF4E;
        }

        bool has_planet_special = false;
        for (int i = 0; i < 5; ++i) {
            int16_t p_idx = star->planet_index[i];
            if (p_idx > -1 && MOX::_planet[p_idx].planet_special > 0) {
                has_planet_special = true;
                break;
            }
        }

        if (has_planet_special) {
            if ((monster_mask >> (static_cast<uint8_t>(star->system_special) & 0x1F)) & 1) {
                for (int i = 0; i < 5; ++i) {
                    int16_t p_idx = star->planet_index[i];
                    if (p_idx > -1) {
                        MOX::_planet[p_idx].planet_special = 0;
                    }
                }
                need_new_special = 1;
            }
        } else {
            need_new_special = 1;
        }

        if (need_new_special) {
            do {
                MAPGEN::Get_A_Planet_Or_Star_Special_(star_idx, &need_new_special, &star->system_special, 1, monster_mask);
            } while (star->system_special <= 0);
        }
    }

    void __cdecl Upgrade_Planet_Guarded_By_Monster_(s_planet_data* planet, int16_t monster_type) {
        bool should_upgrade_climate = true;
        int16_t size_weights[5] = {0};
        int16_t mineral_weights[5] = {0};
        int16_t climate_weights[10] = {0};

        switch (monster_type) {
        case 10:
            size_weights[2] = 1;
            size_weights[3] = 1;
            size_weights[4] = 1;
            mineral_weights[3] = 1;
            mineral_weights[4] = 1;
            if (planet->planet_type == 3) {
                should_upgrade_climate = false;
            } else {
                for (int i = 0; i < 10; ++i) climate_weights[i] = 1;
            }
            break;
        case 11:
        case 14:
            size_weights[3] = 1;
            size_weights[4] = 1;
            mineral_weights[3] = 1;
            mineral_weights[4] = 1;
            climate_weights[8] = 1;
            climate_weights[9] = 1;
            break;
        case 12:
            size_weights[2] = 1;
            size_weights[3] = 1;
            size_weights[4] = 1;
            mineral_weights[3] = 1;
            mineral_weights[4] = 1;
            climate_weights[8] = 1;
            climate_weights[9] = 1;
            break;
        case 13:
            size_weights[3] = 1;
            size_weights[4] = 1;
            mineral_weights[3] = 1;
            mineral_weights[4] = 1;
            if (planet->planet_type == 3) {
                should_upgrade_climate = false;
            } else {
                for (int i = 0; i < 10; ++i) climate_weights[i] = 1;
            }
            break;
        default:
            break;
        }

        planet->planet_type = 3;

        uint16_t new_size = RUSS::Get_Weighted_Choice_Short_(size_weights, 5);
        if (planet->size < (int8_t)new_size) {
            planet->size = (int8_t)new_size;
        }

        uint16_t new_minerals = RUSS::Get_Weighted_Choice_Short_(mineral_weights, 5);
        if (planet->mineral_class < (int8_t)new_minerals) {
            planet->mineral_class = (int8_t)new_minerals;
        }

        if (should_upgrade_climate) {
            uint16_t new_climate = RUSS::Get_Weighted_Choice_Short_(climate_weights, 10);
            if (planet->climate < (int8_t)new_climate) {
                planet->climate = (int8_t)new_climate;
            }
        }

        int16_t planet_index = static_cast<int16_t>(planet - MOX::_planet);
        MAPGEN::Set_Secondary_Planet_Data_(static_cast<int16_t>(planet->star_index), planet_index);

        s_star_data* star = &MOX::_star[planet->star_index];
        Set_Special_For_Star_Guarded_By_Monster_(star, monster_type);
    }

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
                if (player->traits[TRAIT_AQUATIC] != 0) { // Aquatic
                    climate_threshold = 5;
                }

                Enforce_Climate_(i, climate_threshold, &home_planet_id);
                Remove_Specials_(i);

                player->home_planet_id = home_planet_id;

                if (player->traits[TRAIT_LARGE_HOME_WORLD] == 0) { // Not Large Home World
                    Enforce_Medium_Planet_Size_(home_planet_id);
                } else {
                    Set_Planet_Size(home_planet_id, 3); // PLANET_SIZE_LARGE
                }

                Enforce_Planet_Type_(i, home_planet_id);
                Enforce_Planet_Abundant_Mineral_Class(home_planet_id);

                if (player->traits[TRAIT_RICH_HOME_WORLD] > 0) { // Rich Home World
                    Enforce_Mineral_Richness_(home_planet_id, 3);
                } else if (player->traits[TRAIT_RICH_HOME_WORLD] < 0) { // Poor Home World
                    Enforce_Mineral_Richness_(home_planet_id, 1);
                }

                if (player->traits[TRAIT_ANCIENT_HOME_WORLD] != 0) { // Ancient Home World (Artifacts)
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

                if (MOX::_star[i].system_special == SYSTEM_SPECIAL_SPACE_MONSTER) {
                    MOX::_star[i].system_special = SYSTEM_SPECIAL_NO_SPECIAL;
                }
            }
        }

        MOX::_NUM_COLONIES = MOX::_NUM_PLAYERS;
        Assign_Home_Star_Names_();
        Ensure_At_Least_1_Food_Planet_();
    }

    void __cdecl Enforce_Climate_(int16_t star_idx, int16_t climate_id, int16_t* out_planet_idx) {
        struct PlanetRef {
            int16_t planet_idx;
            int16_t orbit;
        };

        PlanetRef candidates[5];
        int16_t candidate_count = 0;
        bool found_climate = false;
        *out_planet_idx = -1;

        for (int16_t orbit = 0; orbit < 5; ++orbit) {
            int16_t p_idx = MOX::_star[star_idx].planet_index[orbit];
            if (p_idx != -1) {
                candidates[candidate_count].planet_idx = p_idx;
                candidates[candidate_count].orbit = orbit;
                candidate_count++;

                if (MOX::_planet[p_idx].climate == (int8_t)climate_id) {
                    found_climate = true;
                    *out_planet_idx = p_idx;
                }
            }
        }

        if (candidate_count == 0) {
            char error_msg[128];
            _sprintf(error_msg, s_Found_no_satellites_after_3_MIN_e_0055adc8, (int)star_idx);
            exitm::Exit_With_Message(error_msg);
            return;
        }

        if (found_climate) {
            int16_t p_idx = *out_planet_idx;
            MOX::_planet[p_idx].food_per_farmer = MAPGEN::Generate_Food_Per_Farmer_(star_idx, p_idx);
            return;
        }

        // If no planet has the requested climate, we force one.
        while (true) {
            int16_t valid_groups[4];
            int16_t valid_group_count = 0;

            for (int16_t group_idx = 0; group_idx < 4; ++group_idx) {
                if (MOX::_climate_roll_table[climate_id][group_idx] > 0) {
                    valid_groups[valid_group_count++] = group_idx;
                }
            }

            int8_t current_spectral = MOX::_star[star_idx].spectral_class;
            bool possible_with_spectral = false;

            for (int16_t i = 0; i < 5; ++i) {
                uint8_t allowed_group = MOX::_class_to_group[current_spectral].group[i];
                for (int16_t j = 0; j < valid_group_count; ++j) {
                    if ((int16_t)allowed_group == valid_groups[j]) {
                        possible_with_spectral = true;
                        break;
                    }
                }
                if (possible_with_spectral) break;
            }

            if (possible_with_spectral) {
                // Pick a random planet from the candidates and force the climate/group
                int16_t chosen_candidate = random::Random_(candidate_count) - 1;
                int16_t p_idx = candidates[chosen_candidate].planet_idx;
                *out_planet_idx = p_idx;

                MOX::_planet[p_idx].climate = (int8_t)climate_id;

                int16_t chosen_group_idx = random::Random_(valid_group_count) - 1;
                MOX::_planet[p_idx].group = (int8_t)valid_groups[chosen_group_idx];

                MOX::_planet[p_idx].food_per_farmer = MAPGEN::Generate_Food_Per_Farmer_(star_idx, p_idx);
                return;
            } else {
                // If the current star's spectral class cannot support this climate, regenerate it
                MOX::_star[star_idx].spectral_class = MAPGEN::Generate_Spectral_Class_((int16_t)MOX::_settings_galaxy_age);
            }
        }
    }

    void __cdecl Remove_Specials_(int16_t star_idx) {
        s_star_data& star = MOX::_star[star_idx];

        star.system_special = SYSTEM_SPECIAL_NO_SPECIAL;
        star.wormhole_star_id = -1;

        for (int i = 0; i < 5; ++i) {
            int16_t planet_idx = star.planet_index[i];
            if (planet_idx > -1) {
                MOX::_planet[planet_idx].planet_special = 0;
            }
        }
    }

    void __cdecl Set_Planet_Size(int16_t planet_index, int8_t size) {
        // s_planet_data structure size is 17 (0x11) bytes
        MOX::_planet[planet_index].size = size;
    }

    void __cdecl Enforce_Medium_Planet_Size_(int16_t planet_idx) {
        // Size 2 corresponds to PLANET_SIZE_MEDIUM
        const int8_t PLANET_SIZE_MEDIUM = 2;

        s_planet_data& planet = MOX::_planet[planet_idx];

        if (planet.size != PLANET_SIZE_MEDIUM) {
            planet.size = PLANET_SIZE_MEDIUM;
            planet.max_farms = MOX::_planet_max_farms[planet.size];
        }
    }

    void __cdecl Enforce_Planet_Type_(int16_t star_idx, int16_t planet_idx) {
        // Search the star's planet indices to confirm membership
        for (int i = 0; i < 5; ++i) {
            if (MOX::_star[star_idx].planet_index[i] == planet_idx) {
                break;
            }
        }

        // Suggested name: PLANET_TYPE_PLANET (3)
        MOX::_planet[planet_idx].planet_type = 3;
    }

    void __cdecl Enforce_Planet_Abundant_Mineral_Class(int16_t planet_idx) {
        // Accessing planet data array (struct size 0x11/17 bytes)
        if (MOX::_planet[planet_idx].mineral_class != MINERAL_RESOURCE_ABUNDANT) {
            MOX::_planet[planet_idx].mineral_class = MINERAL_RESOURCE_ABUNDANT;
        }
    }

    void __cdecl Set_System_Planet_Artifact_Special(int16_t planet_idx, int16_t star_idx) {
        MOX::_planet[planet_idx].planet_special = (int8_t)SYSTEM_SPECIAL_ANCIENT_ARTIFACTS;
        MOX::_star[star_idx].system_special = SYSTEM_SPECIAL_ANCIENT_ARTIFACTS;
    }

    void __cdecl Enforce_Gravity_(int16_t planet_idx, int8_t gravity_class, int16_t player_idx) {
        MOX::_planet[planet_idx].gravity_class = gravity_class;

        // Mineral Class 4 = Ultra Rich
        if (MOX::_planet[planet_idx].mineral_class == MINERAL_RESOURCE_ULTRA_RICH) {
            // Trait index 0 corresponds to Heavy-G World
            if (MOX::_player[player_idx].traits[TRAIT_HEAVY_G_WORLD] == 0) {
                uint8_t new_minerals = MAPGEN::Generate_Mineral_Class_(MOX::_planet[planet_idx].star_index, planet_idx);
                
                // Logic quirk: The reassignment target is the planet at the player's slot index.
                MOX::_planet[player_idx].mineral_class = (int8_t)new_minerals;
            }
        }
    }

    uint8_t __cdecl Ensure_At_Least_1_Food_Planet_() {
        int16_t planet_buffer[360];
        int16_t buffer_count = 0;
        uint8_t overall_success = 1;

        for (int i = 0; i < 360; ++i) {
            planet_buffer[i] = -1;
        }

        for (int16_t colony_idx = 0; colony_idx < MOX::_NUM_COLONIES; ++colony_idx) {
            s_colony& colony = MOX::_colony[colony_idx];
            int8_t owner = colony.owner;
            int16_t planet_idx = colony.planet;

            if (planet_idx != -1 && MOX::_player[owner].traits[TRAIT_EATS_MINERALS] == 0) {
                int16_t food_star = -1;
                int16_t nearest_empty_star = -1;
                int16_t nearest_star_in_range = -1;
                int16_t min_dist = 999;
                uint8_t food_count_in_system = 0;
                buffer_count = 0;

                int8_t colony_star = MOX::_planet[planet_idx].star_index;

                for (int16_t star_idx = 1; star_idx < MOX::_NUM_STARS; ++star_idx) {
                    if (food_count_in_system != 0) break;

                    if (star_idx != colony_star) {
                        int16_t dist = GEO::Parsecs_Between_Stars_(colony_star, star_idx);

                        if (dist < min_dist) {
                            min_dist = dist;
                            if (dist <= 6) {
                                nearest_star_in_range = star_idx;
                            }
                        }

                        if (dist <= 6) {
                            if (HAROLD::N_Planets_In_System_(star_idx) == 0) {
                                if (MOX::_star[star_idx].spectral_class != STAR_CLASS_BLACK_HOLE) {
                                    nearest_empty_star = star_idx;
                                }
                            } else {
                                uint8_t food_planets = HAROLD::N_Food_Planets_In_System_(star_idx);
                                if (food_planets == 0) {
                                    Add_Non_Food_Planets_To_List_(star_idx, planet_buffer, &buffer_count);
                                } else {
                                    food_count_in_system = food_planets;
                                    if (food_star == -1) {
                                        food_star = star_idx;
                                    }
                                }
                            }
                        }
                    }
                }

                if (food_count_in_system == 0) {
                    if (nearest_empty_star != -1) {
                        int16_t orbit = Usable_Orbit_(nearest_empty_star);
                        Make_One_Planet_(nearest_empty_star, MOX::_planet_count, orbit, 1);
                        food_star = nearest_empty_star;
                        MOX::_planet_count++;
                    } else if (nearest_star_in_range != -1 && buffer_count > 0) {
                        int32_t rnd = random::Random_(buffer_count);
                        int16_t target_p_idx = planet_buffer[rnd - 1];
                        int8_t original_special = MOX::_planet[target_p_idx].planet_special;
                        int16_t star_of_planet = MOX::_planet[target_p_idx].star_index;

                        int16_t saved_planet_count = MOX::_planet_count;
                        MOX::_planet_count = target_p_idx;

                        uint32_t orbit_index = ERIC::Get_Sint_Index_From_List_(target_p_idx, MOX::_star[star_of_planet].planet_index, 5);
                        Make_One_Planet_(star_of_planet, target_p_idx, (int16_t)orbit_index, 1);

                        MOX::_planet[target_p_idx].planet_special = original_special;
                        MOX::_planet_count = saved_planet_count;
                    }
                } else if (buffer_count > 0 && food_star == -1) {
                    food_star = MOX::_planet[planet_buffer[0]].star_index;
                }

                reinterpret_cast<int16_t*>(_nearest_food_worlds)[owner] = food_star;

                if (food_star == -1 && MOX::_player[owner].traits[TRAIT_EATS_MINERALS] == 0) {
                    overall_success = 0;
                }
            }
        }

        return overall_success;
    }

    void __cdecl Add_Non_Food_Planets_To_List_(int16_t star_idx, int16_t* planet_list, int16_t* count) {
        s_star_data* star = &MOX::_star[star_idx];

        for (int i = 0; i < 5; ++i) {
            int16_t planet_id = star->planet_index[i];

            if (planet_id > -1) {
                planet_list[*count] = planet_id;
                *count += 1;
            }
        }
    }

    int16_t __cdecl Get_Orion_Star_Id_() {
        int16_t center_x = (MOX::_cur_map_scale * 253) / 10 + 21;
        int16_t center_y = (MOX::_cur_map_scale * 200) / 10 + 21;
        int16_t map_x, map_y;

        MAINSCR::Get_PointXY_Draw_Coords_(center_x, center_y, &map_x, &map_y);

        uint8_t visited[72];
        helpers::memset(&visited, 0, sizeof(visited));

        StarCandidate candidates[72];
        for (int i = 0; i < 72; ++i) {
            candidates[i].distance = -1;
            candidates[i].star_id = -1;
        }

        int candidate_count = 0;
        int16_t search_radius = 4;

        while (candidate_count < 4 && search_radius <= 7) {
            for (int16_t star_id = 0; star_id < MOX::_NUM_STARS; ++star_id) {
                if (visited[star_id] == 0) {
                    if (!HAROLD::Is_Home_Star_(star_id) && Is_Added_Food_World_(star_id) == 0) {
                        int16_t dist = GEO::Parsecs_Between_Points_(MOX::_star[star_id].x, MOX::_star[star_id].y, center_x, center_y);
                        if (dist < search_radius) {
                            visited[star_id] = 1;
                            candidates[star_id].distance = (int32_t)dist;
                            candidates[star_id].star_id = star_id;
                            candidate_count++;
                        }
                    }
                }
            }
            if (candidate_count >= 4) break;
            search_radius++;
        }

        int valid_count = 0;
        for (int i = 0; i < MOX::_NUM_STARS; ++i) {
            if (candidates[i].distance > 0) {
                candidates[valid_count].star_id = candidates[i].star_id;
                candidates[valid_count].distance = candidates[i].distance;
                valid_count++;
            }
        }

        if (valid_count >= 2) {
            _qsort(candidates, (size_t)valid_count, sizeof(StarCandidate), qsort_1_);
            int random_idx = random::Random_(valid_count) - 1;
            return candidates[random_idx].star_id;
        } else {
            while (true) {
                int random_star = random::Random_(MOX::_NUM_STARS) - 1;
                int16_t selected_star_id = (int16_t)random_star;
                if (!HAROLD::Is_Home_Star_(selected_star_id) && Is_Added_Food_World_(selected_star_id) == 0) {
                    return selected_star_id;
                }
            }
        }
    }

    int16_t __cdecl Generate_Orion_() {
        int16_t orion_star_id = Get_Orion_Star_Id_();
        int16_t orion_orbit = -1;
        int16_t temp_planet_indices[5];

        for (int16_t i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[0].planet_index[i];
            temp_planet_indices[i] = planet_idx;
            if (planet_idx != -1 && i > 2) {
                if (random::Random_(i - 1) == 1) {
                    orion_orbit = i;
                }
            }
        }

        if (orion_orbit < 2) {
            orion_orbit = (int16_t)random::Random_(3) + 1;
        }

        int16_t orion_planet_idx = MOX::_star[0].planet_index[orion_orbit];
        if (orion_planet_idx == -1) {
            orion_planet_idx = MOX::_planet_count;
            MOX::_star[0].planet_index[orion_orbit] = orion_planet_idx;
            MOX::_planet_count++;
        }

        for (int16_t i = 0; i < 5; ++i) {
            if (i != orion_orbit && MOX::_star[0].planet_index[i] != -1) {
                MOX::_planet[MOX::_star[0].planet_index[i]].planet_type = 1;
            }
        }

        MOX::_star[0].system_special = SYSTEM_SPECIAL_ORION_STAR;

        for (int i = 0; i < 6; ++i) {
            MOX::_star[0].name[i] = s_Orion_0055a738[i];
        }

        if (MOX::_star[0].spectral_class == 6) {
            MOX::_star[0].spectral_class = (int8_t)random::Random_(6) - 1;
        }

        s_star_data backed_up_star = MOX::_star[orion_star_id];
        MOX::_star[orion_star_id] = MOX::_star[0];

        int16_t saved_template_x = MOX::_star[0].x;
        int16_t saved_template_y = MOX::_star[0].y;

        MOX::_star[orion_star_id].x = saved_template_x;
        MOX::_star[orion_star_id].y = saved_template_y;

        MOX::_star[0].x = backed_up_star.x;
        MOX::_star[0].y = backed_up_star.y;

        s_planet_data* p = &MOX::_planet[orion_planet_idx];
        helpers::memset(p, 0, sizeof(s_planet_data));

        p->colony_index = -1;
        p->orbit = (int8_t)orion_orbit;
        p->planet_type = PLANET_TYPE_PLANET;
        p->size = PLANET_SIZE_HUGE;
        p->gravity_class = PLANET_GRAVITY_NORMAL_G;
        p->group = 2;
        p->climate = PLANET_CLIMAGE_GAIA;
        p->mineral_class = MINERAL_RESOURCE_ULTRA_RICH;
        p->planet_special = 11;

        p->food_per_farmer = MAPGEN::Generate_Food_Per_Farmer_(0, orion_planet_idx);
        p->max_farms = MAPGEN::Generate_Max_Farms_(0, orion_planet_idx);

        return orion_star_id;
    }

    void __cdecl Enforce_Mineral_Richness_(int16_t planet_index, int16_t mineral_class) {
        constexpr int8_t STAR_CLASS_DWARF = 5;

        s_planet_data* planet = &MOX::_planet[planet_index];
        planet->mineral_class = (int8_t)mineral_class;

        int8_t star_idx = planet->star_index;
        s_star_data* star = &MOX::_star[star_idx];

        while (true) {
            bool richness_is_valid = false;

            // Only spectral classes 0-4 have specific mineral restrictions checked via table
            if (star->spectral_class < STAR_CLASS_DWARF) {
                for (int i = 0; i < 10; ++i) {
                    // _class_to_mineral is a 10x6 table mapping star classes to valid minerals
                    if (MOX::_class_to_mineral[i * 6 + star->spectral_class] == (uint8_t)mineral_class) {
                        richness_is_valid = true;
                        break;
                    }
                }
            }

            if (richness_is_valid) {
                break;
            }

            // If class is Dwarf (>=5) or richness not allowed for this class, regenerate spectral class
            star->spectral_class = MAPGEN::Generate_Spectral_Class_((int16_t)MOX::_settings_galaxy_age);
        }

        // Update gravity class based on mineral class and planet size
        // _gravity_table is 5 columns wide (planet size categories)
        int32_t gravity_idx = (planet->mineral_class * 5) + planet->size;
        planet->gravity_class = MOX::_gravity_table[gravity_idx];
    }
}