#include "pch.h"
#include <cstring>

namespace HAROLD {
    s_mouse_list _saved_mouse_list[10];
    uint16_t _saved_mouse_list_count;
    uint8_t _set_fade_for_first_fltpop;

    const char s_after_home_world_gen[] = "after home world gen";

    const char s___s__00556ae4[] = "(%s)";
    const char s_in_next_turn[] = "in next-turn";

    uint32_t __cdecl Is_Colony_Star_(int16_t star_idx) {
        uint32_t has_colony = 0;

        for (int i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[star_idx].planet_index[i];

            if (planet_idx > -1) {
                if (MOX::_planet[planet_idx].colony_index != -1) {
                    has_colony = 1;
                }
            }
        }

        return has_colony;
    }

    uint8_t __cdecl N_Planets_In_System_(int16_t star_idx) {
        uint8_t count = 0;

        for (int16_t i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[star_idx].planet_index[i];

            if (planet_idx != -1) {
                if (MOX::_planet[planet_idx].planet_type == PLANET_TYPE_PLANET) {
                    count++;
                }
            }
        }

        return count;
    }

    void __cdecl Get_Officer_Data_LBX_Name_(char* out_name) {
        const char* lbx_name;

        switch (MOX::_settings.language) {
        case 1:  // German
            lbx_name = "HERODATG.LBX";
            break;
        case 2:  // French
            lbx_name = "HERODATF.LBX";
            break;
        case 3:  // Spanish
            lbx_name = "HERODATS.LBX";
            break;
        case 4:  // Italian
            lbx_name = "HERODATI.LBX";
            break;
        default: // English / Standard
            lbx_name = "HERODATA.LBX";
            break;
        }

        memcpy(out_name, lbx_name, sizeof("HERODATA.LBX"));
    }

    /** Returns whether the star's name matches Orion. */
    bool __cdecl Star_Is_Orion_(int16_t star_idx) {
        const char* orion_str = HOMEGEN::s_Orion;
        const char* star_name = MOX::_star[star_idx].name;

        for (int i = 0; i < 6; ++i) {
            if (star_name[i] != orion_str[i]) {
                return false;
            }
        }
        return true;
    }

    /** Returns whether the star's spectral class falls in the habitable range. */
    bool __cdecl Habitable_Spectral_Class_(int16_t star_idx) {
        return MOX::_star[star_idx].spectral_class < STAR_CLASS_BLACK_HOLE;
    }

    /** Returns whether the system contains outposts but no full colonies. */
    bool __cdecl Star_Is_Outpost_Star_(int16_t star_idx) {
        int8_t outpost_count = 0;
        int8_t colony_count = 0;

        for (int32_t i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[star_idx].planet_index[i];

            if (planet_idx != -1) {
                int16_t colony_idx = MOX::_planet[planet_idx].colony_index;

                if (colony_idx != -1) {
                    if (MOX::_colony[colony_idx].outpost_flag == 0) {
                        colony_count++;
                    } else {
                        outpost_count++;
                    }
                }
            }
        }

        // Returns true only if the star system is occupied solely by outposts
        if (outpost_count > 0 && colony_count == 0) {
            return true;
        }

        return false;
    }

    int8_t __cdecl Planet_Number_(int16_t planet_index) {
        int16_t star_idx = MOX::_planet[planet_index].star_index;
        s_star_data* star = &MOX::_star[star_idx];

        int8_t relative_planet_num = 0;
        for (int i = 0; i < 5; ++i) {
            int16_t system_planet_id = star->planet_index[i];

            if (system_planet_id == planet_index) {
                return relative_planet_num;
            }

            // Only increment the count if the slot contains a valid planet (not -1)
            if (system_planet_id > -1) {
                relative_planet_num++;
            }
        }

        return relative_planet_num;
    }

    int16_t __cdecl Planet_Index_(int16_t star_idx, int16_t planet_idx) {
        return MOX::_star[star_idx].planet_index[planet_idx];
    }

    void __cdecl Get_Font_File_(char* buffer) {
        switch (MOX::_settings.language) {
        case 1: // German
            memcpy(buffer, "fontsg.lbx", sizeof("fontsg.lbx"));
            return;
        case 2: // French
            memcpy(buffer, "fontsf.lbx", sizeof("fontsf.lbx"));
            return;
        case 3: // Spanish
            memcpy(buffer, "fontss.lbx", sizeof("fontss.lbx"));
            return;
        case 4: // Italian
            memcpy(buffer, "fontsi.lbx", sizeof("fontsi.lbx"));
            return;
        default: // English or Other
            memcpy(buffer, "fonts.lbx", sizeof("fonts.lbx"));
            return;
        }
    }

    void __cdecl Load_Language_Fonts_() {
        char font_file_path[20];
        font_file_path[0] = '\0';
        Get_Font_File_(font_file_path);
        fonts::Load_Font_File_(font_file_path);
    }

    int16_t __cdecl Get_Scaled_Value_(int16_t value) {
        return (int16_t)(((int32_t)value * 10) / (int32_t)MOX::_cur_map_scale);
    }

    int32_t __cdecl Get_Up_Scaled_Value_(int16_t value) {
        return ((int32_t)value * (int32_t)MOX::_cur_map_scale) / 10;
    }

    uint8_t __cdecl N_Satellites(int16_t star_index) {
        uint8_t count = 0;

        for (int i = 0; i < 5; ++i) {
            if (MOX::_star[star_index].planet_index[i] != -1) {
                count++;
            }
        }

        return count;
    }

    /**
     * @address 004f10f0
     */
    int16_t __cdecl N_Colony_Owners_(int16_t star_idx) {
        uint8_t owners_seen[MAX_PLAYERS];
        int16_t unique_count = 0;

        for (int i = 0; i < MAX_PLAYERS; ++i) {
            owners_seen[i] = 0;
        }

        for (int16_t i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[star_idx].planet_index[i];

            if (planet_idx > -1) {
                int16_t colony_idx = MOX::_planet[planet_idx].colony_index;

                if (colony_idx > -1) {
                    int8_t owner_id = MOX::_colony[colony_idx].owner;

                    if (owners_seen[owner_id] == 0) {
                        unique_count++;
                        owners_seen[owner_id] = 1;
                    }
                }
            }
        }

        return unique_count;
    }

    void __cdecl Set_Wormhole_Id_(int16_t star_idx, uint8_t* star_bitmap) {
        int16_t candidates[20];
        for (int i = 0; i < 20; ++i) {
            candidates[i] = -1;
        }

        int16_t count = 0;
        bool buffer_full = false;

        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            if (buffer_full) break;

            uint32_t distance = (uint32_t)GEO::Parsecs_Between_Stars_(star_idx, i);

            if (distance > (uint32_t)MOX::_settings.galaxy_size + 3 &&
                star_bitmap[i] == 0 &&
                MOX::_star[i].wormhole_star_id == -1 &&
                MOX::_star[i].spectral_class != STAR_CLASS_BLACK_HOLE &&
                MOX::_star[i].system_special == SYSTEM_SPECIAL_NO_SPECIAL &&
                i != star_idx)
            {
                candidates[count++] = i;
                if (count == 19) {
                    buffer_full = true;
                }
            }
        }

        if (count > 0) {
            int16_t invalidated_count = 0;
            for (int i = 0; i < count; ++i) {
                int16_t star_id = candidates[i];
                if (MOX::_star[star_id].wormhole_star_id > 0 || N_Satellites(star_id) == 0) {
                    candidates[i] = -1;
                    invalidated_count++;
                }
            }

            if (invalidated_count < count) {
                int32_t iters = ((int32_t)count * 5) / 3;
                for (int32_t i = 0; i < iters; ++i) {
                    int16_t idx1 = (int16_t)game_random::Random_(count);
                    int16_t idx2 = (int16_t)game_random::Random_(count);
                    if (idx1 != idx2 && candidates[idx1] != -1 && candidates[idx2] != -1) {
                        int16_t swapped_candidate = candidates[idx1];
                        candidates[idx1] = candidates[idx2];
                        candidates[idx2] = swapped_candidate;
                    }
                }

                int16_t wormhole_exit_id = -1;
                int16_t pick_counter = 1;
                for (int i = 0; i < count; ++i) {
                    if (candidates[i] != -1) {
                        if (game_random::Random_(pick_counter) == 1) {
                            wormhole_exit_id = candidates[i];
                            pick_counter++;
                        }
                    }
                }

                if (wormhole_exit_id != -1) {
                    MOX::_star[wormhole_exit_id].wormhole_star_id = star_idx;
                    MOX::_star[star_idx].wormhole_star_id = wormhole_exit_id;
                    return;
                }
            }
        }

        MOX::_star[star_idx].wormhole_star_id = -1;
    }

    /**
     * @address 004f0860
     */
    bool __cdecl Empty_System_(int16_t star_idx) {
        return N_Satellites(star_idx) == 0;
    }

    bool __cdecl Is_Home_Star_(int16_t star_idx) {
        return Players_Home_Star_(star_idx) != -1;
    }

    /**
     * @address 004f1270
     */
    int16_t __cdecl N_Food_Planets_In_System_(int16_t system_index) {
        int16_t food_planet_count = 0;

        if (system_index < 0) {
            return 0;
        }

        for (int16_t i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[system_index].planet_index[i];

            if (planet_idx > -1 &&
                MOX::_planet[planet_idx].planet_type == PLANET_TYPE_PLANET &&
                MOX::_planet[planet_idx].food_per_farmer > 0) {
                food_planet_count++;
            }
        }

        return food_planet_count;
    }

    /**
     * @address 004f1930
     */
    void __cdecl Set_Player_Font_Style_(int16_t player_idx, bool apply_selected_color_offset) {
        s_colors style;

        int16_t style_idx = Zoom_Level_Font_Style_();

        Get_Player_Font_Colors_(player_idx, style_idx, &style, apply_selected_color_offset);

        fonts::Set_Font_Style_(style_idx, &style);
    }

    void __cdecl Set_Sound_Volume_To_Settings_() {
        int8_t music_lvl = MOX::_settings.music_level;
        if (music_lvl >= 100) {
            music_lvl = 100;
        }

        if (MOX::_settings.music_on == 0) {
            sound::Stop_Stream_();
        }
        else {
            sound::Stream_On_();
        }

        int32_t volume_param = music_lvl;
        sound::Set_Stream_Max_Volume_(volume_param);
        sound::Set_Stream_Volume_(volume_param);

        if (MOX::_settings.sound_fx_on == 0) {
            sound::Stop_All_Sounds_();
        }
        else {
            sound::Sound_On_();
            sound::Resume_Sound_(-1);
        }

        sound::Set_Sound_Max_Volume_(MOX::_settings.sound_fx_level);
    }

    /**
     * @address 004f0e90
     */
    void __cdecl Init_Fleet_Movement_Box_() {
        auto& box = MOX::_moveable_box[2];

        box.width = 200;
        box.height = 0x110;
        box.x = 0x16;
        box.y = 0x16;

        box.unselected_width = box.width;
        box.unselected_height = 0x27;
    }

    /**
     * @address 004f0f60
     */
    void __cdecl Init_System_Display_Box_() {
        MOX::_moveable_box_pict[0] = (s_animation_header*)buffer::Buffer_Reload_("BUFFER0.LBX", 0, MOX::_global_cache_seg);

        MOX::_moveable_box[0].width = animate::Get_Width_(MOX::_moveable_box_pict[0]);
        MOX::_moveable_box[0].height = animate::Get_Height_(MOX::_moveable_box_pict[0]);

        if (MOX::_moveable_box[0].x < 0) {
            MOX::_moveable_box[0].x = 22;
        }

        if (MOX::_moveable_box[0].y < 0) {
            MOX::_moveable_box[0].y = 22;
        }
    }

    /**
     * @address 004f0ed0
     */
    void __cdecl Init_Moving_Boxes_() {
        memset(MOX::_moveable_box, 0, sizeof(MOX::_moveable_box));

        for (int i = 0; i < 5; ++i) {
            MOX::_moveable_box[i].index = (uint16_t)i;
            MOX::_moveable_box_order[i] = (uint16_t)i;

            MOX::_moveable_box[i].id = (int16_t)MOX::_box_off_flag[i];

            MOX::_moveable_box[i].x = -1;
            MOX::_moveable_box[i].y = -1;
        }

        Init_Fleet_Movement_Box_();
        Init_System_Display_Box_();
    }

    /**
     * @address 004f1590
     */
    bool __cdecl Player_Has_Colony_(int16_t player_num, int16_t star_idx) {
        return struct_::Test_Bit_Field_(&MOX::_star[star_idx].has_colony, player_num) != 0;
    }

    /**
     * @address 004f1610
     */
    bool __cdecl Player_Has_Jumpgate_(int16_t player_idx, int16_t star_idx) {
        uint8_t jumpgate_bits = MOX::_star[star_idx].has_jumpgate;

        return (bool)((jumpgate_bits >> (static_cast<uint8_t>(player_idx) & 0x1f)) & 1);
    }

    /**
     * @address 004f16d0
     */
    bool __cdecl Player_Has_Stargate_(int16_t player_idx, int16_t star_idx) {
        uint8_t stargate_bits = MOX::_star[star_idx].has_stargate;

        return (bool)((stargate_bits >> (static_cast<uint8_t>(player_idx) & 0x1f)) & 1);
    }

    /**
     * @address 004f1750
     */
    uint8_t __cdecl Player_Has_Visited_(int16_t player_idx, int16_t star_idx) {
        uint8_t mask = (uint8_t)(1u << ((uint8_t)player_idx & 0x1F));

        return MOX::_star[star_idx].visited & mask;
    }

    /**
     * @brief Checks if a player has sighted a monster at a specific star system.
     *
     * A monster is sighted if the star system has a special type of 9 (monster)
     * and the player has visited the star system.
     *
     * @param player_id The ID of the player.
     * @param star_id The ID of the star system.
     * @return true if the monster is sighted, false otherwise.
     * @address 004f1780
     */
    bool __cdecl Player_Sighted_Monster_(int16_t player_id, int16_t star_id) {
        int8_t system_special = MOX::_star[star_id].system_special;
        bool has_visited = HAROLD::Player_Has_Visited_(player_id, star_id);

        if (system_special == SYSTEM_SPECIAL_SPACE_MONSTER && has_visited) {
            return true;
        }

        return false;
    }

    /**
     * Finds the index of the player who has their home planet in the specified star system.
     * Returns -1 if no home planet in the system belongs to any player.
     * @address 004f17d0
     */
    int16_t __cdecl Players_Home_Star_(int16_t star_idx) {
        int16_t found_player_idx = -1;

        for (int planet_slot = 0; planet_slot < 5; ++planet_slot) {
            // Early exit if a home planet was already identified in an earlier slot
            if (found_player_idx != -1) {
                break;
            }

            int16_t planet_id = MOX::_star[star_idx].planet_index[planet_slot];
            if (planet_id > -1) {
                for (int16_t p = 0; p < (int16_t)MOX::_NUM_PLAYERS; ++p) {
                    if (MOX::_player[p].home_planet_id == planet_id) {
                        found_player_idx = p;
                    }
                }
            }
        }

        return found_player_idx;
    }

    int16_t __cdecl Planet_Type_(int16_t star_idx, int16_t star_planet) {
        int16_t planet_idx = MOX::_star[star_idx].planet_index[star_planet];
        return (int16_t)MOX::_planet[planet_idx].planet_type;
    }

    /**
     * Calculates how many colonies each player owns in a specific star system.
     *
     * @param star_idx Index of the star system to check.
     * @param player_counts Array of size MAX_PLAYERS that receives colony count per player index.
     * @param total_count Pointer to variable that receives the number of unique players owning colonies in this system.
     * @param have_contact If true, only counts owners that the current player has diplomatic contact with.
     */
    void __cdecl Colony_Owners_(int16_t star_idx, int16_t* player_counts, int16_t* total_count, bool have_contact) {
        *total_count = 0;
        for (int i = 0; i < MAX_PLAYERS; ++i) {
            player_counts[i] = 0;
        }

        // A star system in Master of Orion 2 has up to 5 planets.
        for (int i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[star_idx].planet_index[i];
            if (planet_idx > -1) {
                int16_t colony_idx = MOX::_planet[planet_idx].colony_index;
                if (colony_idx > -1) {
                    int8_t owner = MOX::_colony[colony_idx].owner;
                    if (owner > -1) {
                        bool visible = true;
                        if (have_contact && owner != MOX::_PLAYER_NUM) {
                            visible = Contact_With_Player_(MOX::_PLAYER_NUM, (int16_t)owner);
                        }

                        if (visible) {
                            if (player_counts[owner] == 0) {
                                *total_count += 1;
                            }
                            player_counts[owner] += 1;
                        }
                    }
                }
            }
        }
    }

    /**
     * Checks if the specified star system has a dimensional portal.
     * @param star Pointer to the star data structure.
     * @return True if a dimensional portal is present, false otherwise.
     * @address 004f15c0
     */
    bool __cdecl Star_Has_Dimensional_Portal_(s_star_data* star) {
        return star->has_dimensional_portal != 0;
    }

    /**
     * @address 004f15d0
     */
    bool __cdecl Star_Has_Artemis_Net_(s_star_data* star) {
        return star->has_artemis_net != 0;
    }

    /**
     * @address 004f1560
     */
    bool __cdecl Player_Has_Artemis_Net_(s_player* player, s_star_data* star) {
        int32_t player_index = static_cast<int32_t>(player - MOX::_player);
        return (bool)((star->has_artemis_net >> (player_index & 0x1f)) & 1);
    }

    /**
     * @address 004f15e0
     */
    bool __cdecl Player_Has_Dimensional_Portal_(s_player* player, s_star_data* star) {
        int32_t player_index = static_cast<int32_t>(player - MOX::_player);
        return (bool)((star->has_dimensional_portal >> (player_index & 0x1f)) & 1);
    }

    /**
     * @brief Randomizes the elements of a list using either a PRNG or the system clock.
     *
     * @param list Pointer to the array of shorts to be shuffled.
     * @param size The number of elements in the list.
     * @param use_clock If non-zero, uses clock() % size for indices; otherwise uses Random_(size) - 1.
     * @address 004f1860
     */
    void __cdecl Randomize_List_(int16_t* list, int16_t size, uint8_t use_clock) {
        // The number of shuffle iterations is 1.5 * size
        int32_t iterations = (static_cast<int32_t>(size) * 3) / 2;

        if (iterations <= 0) {
            return;
        }

        int16_t count = 0;
        do {
            int16_t idx1, idx2;

            if (use_clock == 0) {
                // Binary returns a 1-based random index here; convert to 0-based with -1
                idx1 = static_cast<int16_t>(game_random::Random_(static_cast<int32_t>(size))) - 1;
                idx2 = static_cast<int16_t>(game_random::Random_(static_cast<int32_t>(size))) - 1;
            } else {
                idx1 = (int16_t)(clock() % (int32_t)size);
                idx2 = (int16_t)(clock() % (int32_t)size);
            }

            if (idx1 != idx2) {
                int16_t swapped_entry = list[idx1];
                list[idx1] = list[idx2];
                list[idx2] = swapped_entry;
            }

            count++;
            // Note: If iterations > 32767, this comparison will exhibit
            // unexpected behavior due to the 16-bit sign extension of count.
        } while (static_cast<int32_t>(count) < iterations);
    }

    /**
     * @address 004f18f0
     */
    void __cdecl Set_All_Visited_Flags_() {
        int16_t num_stars = MOX::_NUM_STARS;
        if (num_stars <= 0) {
            return;
        }

        uint8_t bit_mask = 1 << (static_cast<uint8_t>(MOX::_PLAYER_NUM) & 0x1F);

        for (int16_t i = 0; i < num_stars; ++i) {
            MOX::_star[i].visited |= bit_mask;
        }
    }

    const char* __cdecl H_Message_(int16_t id, int unused_save_slot, const s_save_game_description* unused_save_description) {
        uint16_t buffer_offset = 0;

        (void)unused_save_slot;
        (void)unused_save_description;

        if (id >= 0 && id < 397) {
            buffer_offset = MOX::_h_string_ix[id];
        }

        return &MOX::_h_strings[buffer_offset];
    }

    void __cdecl Save_Mouse_List_() {
        HAROLD::_saved_mouse_list_count = mouse::Copy_Current_Mouse_List_(_saved_mouse_list, 10);
    }

    void __cdecl Restore_Mouse_List_() {
        mouse::Set_Mouse_List_(_saved_mouse_list, _saved_mouse_list_count);
        _saved_mouse_list_count = 0;
    }

    void Confirm_Planet_And_Star_Index_Link_(const char* unused_context) {
        (void)unused_context;

        for (int16_t i = 0; i < MOX::_planet_count; ++i) {
            Planet_Number_(i);
        }

        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            int16_t orbit_taken[5] = { 0, 0, 0, 0, 0 }; /* keep exact 5-entry bitmap, indexed by planet->orbit */
            s_star_data* star = &MOX::_star[star_idx];

            for (int16_t orbit_idx = 0; orbit_idx < 5; ++orbit_idx) {
                int16_t planet_idx = star->planet_index[orbit_idx];
                if (planet_idx != -1) {
                    s_planet_data* planet = &MOX::_planet[planet_idx];
                    int8_t planet_orbit = planet->orbit;

                    if (orbit_taken[planet_orbit] != 0) {
                        RUSS::PFMT_(0, 1, "orbit %d of star %d has more than 1 planet in it", (int32_t)planet_orbit, star_idx);
                        RUSS::PFMT_(0, 2, "relate game conditions EXACTLY as they just happened");
                        RUSS::PFMT_(0, 3, s___s__00556ae4, star->name);
                        key::Wait_For_Key_();
                    }
                    orbit_taken[planet_orbit] = 1;
                }
            }
        }
    }

    void __cdecl Load_Leader_Titles_() {
        if (MOX::_screen_seg != NULL) {
            char lbx_filename[20];
            Get_Officer_Data_LBX_Name_(lbx_filename);

            for (int16_t i = 0; i < MAX_LEADERS; ++i) {
                allocate::Push_Block_(MOX::_screen_seg);

                void* record_ptr = farload::Far_Reload_Next_Data_(
                    lbx_filename,
                    0,
                    MOX::_screen_seg,
                    static_cast<uint32_t>(i),
                    1,
                    59
                );

                if (record_ptr != nullptr) {
                    char* src_title = (char*)record_ptr + 15;
                    char* dest_title = MOX::_leaders[i].title;

                    while ((*dest_title++ = *src_title++) != '\0');
                }

                allocate::Pop_Block_(MOX::_screen_seg);
            }
        }
    }

    /**
     * @address 004f07f0
     */
    bool __cdecl Contact_With_Player_(int16_t player_idx, int16_t target_idx) {
        if (target_idx >= 0 && target_idx < (int16_t)MOX::_NUM_PLAYERS) {
            return MOX::_player[player_idx].contact[target_idx] != 0;
        }
        return false;
    }

    /**
     * @address 004f0760
     */
    char __cdecl Contact_With_One_Colony_(int16_t star_idx) {
        for (int16_t i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[star_idx].planet_index[i];

            if (planet_idx > -1) {
                int16_t colony_idx = MOX::_planet[planet_idx].colony_index;

                if (colony_idx > -1) {
                    int16_t colony_owner = static_cast<int16_t>(MOX::_colony[colony_idx].owner);

                    if (Contact_With_Player_(MOX::_PLAYER_NUM, colony_owner)) {
                        return 1;
                    }
                }
            }
        }

        return 0;
    }

    /**
     * Counts the number of non-eliminated players with human objectives.
     */
    int16_t __cdecl Get_N_Remaining_Humans_() {
        int16_t count = 0;

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            if (MOX::_player[i].objectives == PLAYER_OBJECTIVE_HUMAN && MOX::_player[i].eliminated == 0) {
                count++;
            }
        }

        return count;
    }

    bool __cdecl Player_Is_Omniscient_(int16_t player_idx) {
        if (OFFICER::One_Leader_With_Galactic_Lore_(player_idx)) {
            return true;
        }

        if (MOX::_player[player_idx].traits[TRAIT_OMNISCIENCE] != 0) {
            return true;
        }

        return false;
    }

    /**
     * @address 004f1dc0
     */
    int16_t __cdecl XY_To_Star_Id_(int16_t x, int16_t y) {
        for (int16_t i = 0; i < MOX::_NUM_STARS + 1; ++i) {
            if (MOX::_star[i].x == x && MOX::_star[i].y == y) {
                return i;
            }
        }

        return -1;
    }

    /**
     * @address 004f13d0
     */
    int16_t __cdecl N_Turns_To_Star_(int16_t ship_id, int16_t star_id) {
        int16_t ship_ids[2];
        s_ship_move_info move_info;
        int16_t result = -1;

        // Initialize ship IDs list with a terminator
        ship_ids[0] = ship_id;
        ship_ids[1] = -1;

        SHIPMOVE::Ships_Try_To_Move_To_(ship_ids, star_id, &move_info);

        // If the ships are able to move, return the number of turns required
        if (move_info.moving != 0) {
            result = (int16_t)move_info.turns_left;
        }

        return result;
    }

    /**
     * @address 004f05c0
     */
    int16_t __cdecl Absolute_Location_(int16_t location) {
        if (location >= SHIP_LOCATION_MOVING_OFFSET && location < SHIP_LOCATION_WORMHOLE_OFFSET) {
            return location - SHIP_LOCATION_MOVING_OFFSET;
        }

        if (location >= SHIP_LOCATION_WORMHOLE_OFFSET && location < SHIP_LOCATION_LIMIT) {
            return location - SHIP_LOCATION_WORMHOLE_OFFSET;
        }

        return location;
    }

    /**
     * @address 004f05f0
     */
    int16_t __cdecl Actual_Star_Id_(int16_t star_id, int16_t* out_actual_id) {
        int16_t type = 0;

        if (star_id >= SHIP_LOCATION_MOVING_OFFSET && star_id < SHIP_LOCATION_WORMHOLE_OFFSET) {
            *out_actual_id = star_id - SHIP_LOCATION_MOVING_OFFSET;
            return 1;
        }

        if (star_id >= SHIP_LOCATION_WORMHOLE_OFFSET && star_id < SHIP_LOCATION_LIMIT) {
            star_id -= SHIP_LOCATION_WORMHOLE_OFFSET;
            type = 2;
        }

        *out_actual_id = star_id;
        return type;
    }

    /**
     * @address 004f1e80
     */
    bool __cdecl Star_Guarded_By_Monster_(int16_t location_idx, int16_t* monster_owner) {
        *monster_owner = 0;
        bool found = false;

        for (int16_t i = 0; i < (int16_t)MOX::_ship_stack_count; ++i) {
            if (found) {
                break;
            }

            int16_t node_idx = MOX::_ship_stack_start[i];
            while (node_idx != -1) {
                int16_t ship_idx = MOX::_ship_node[node_idx].ship_idx;
                s_ship_data* ship = &MOX::_ship[ship_idx];

                // Status 0 indicates the ship is active/available.
                // Owners >= MAX_PLAYERS (specifically owner 8) are typically neutral monsters (Orion Guardians, etc).
                if (ship->status == 0 && ship->owner >= MAX_PLAYERS && ship->location == location_idx) {
                    found = true;
                    *monster_owner = (int16_t)ship->owner;
                    break;
                }

                node_idx = MOX::_ship_node[node_idx].next_node;
            }
        }

        return found;
    }

    int16_t __cdecl Star_Owner_(int16_t star_idx, int16_t viewer_player_idx) {
        int16_t player_counts[MAX_PLAYERS];
        int16_t visible_owner_count;

        bool hide_unexplored = !HAROLD::Player_Is_Omniscient_(viewer_player_idx);

        Colony_Owners_(star_idx, player_counts, &visible_owner_count, hide_unexplored);

        if (player_counts[MOX::_PLAYER_NUM] > 0) {
            return MOX::_PLAYER_NUM;
        }

        int16_t max_colonies = 0;
        int16_t winner = -1;

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_PLAYERS; ++i) {
            if (player_counts[i] > max_colonies) {
                max_colonies = player_counts[i];
                winner = i;
            }
        }

        return winner;
    }

    /**
     * @address 004f1d00
     */
    int16_t __cdecl First_Ship_Orbiting_Star_(s_star_data* star) {
        int16_t ship_ids[2];
        int16_t count = 2;

        Star_XY_To_Ship_Id_(star, ship_ids, &count);

        return ship_ids[0];
    }

    /**
     * @address 004f1d30
     */
    void __cdecl Star_XY_To_Ship_Id_(s_star_data* target_star, int16_t* out_indices, int16_t* count) {
        int16_t max_count = *count;
        if (max_count < 1 || MOX::_NUM_SHIPS < max_count) {
            max_count = (int16_t)MOX::_NUM_SHIPS;
        }

        *count = 0;

        if (MOX::_NUM_SHIPS <= 0) {
            return;
        }

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_SHIPS; ++i) {
            if (max_count < *count) {
                break;
            }

            if (MOX::_star[i].x == target_star->x && MOX::_star[i].y == target_star->y) {
                out_indices[*count] = i;
                *count += 1;
            }
        }
    }

    int16_t __cdecl Zoom_Level_Font_Style_() {
        int16_t scale = MOX::_cur_map_scale;

        if (scale == 10) {
            return 3;
        }

        if (scale == 15 || scale == 20) {
            return 2;
        }

        if (scale >= 30) {
            return 1;
        }

        // The original binary returns an uninitialized stack word here for unexpected scale values.
        // Preserve defined behavior in the restored source and fall back to style 0 instead.
        return 0;
    }

    /**
     * @address 004f0dd0
     */
    uint8_t* __cdecl Harold_Init_() {
        MOX::_box_off_flag[0] = -1;
        MOX::_box_off_flag[1] = 0x0000;
        MOX::_box_off_flag[2] = -1;
        MOX::_box_off_flag[3] = 0x0000;
        MOX::_box_off_flag[4] = 0x0000;

        Init_Moving_Boxes_();

        // Zero out the MAX_PLAYERS-sized array tracking home star rename status
        for (int i = 0; i < MAX_PLAYERS; ++i) {
            MOX::_given_chance_to_rename_home_star[i] = 0x00;
        }

        return MOX::_given_chance_to_rename_home_star;
    }

    /**
     * @brief Checks if a player has an outpost in a specific star system.
     * 
     * @param player_num Unused in original binary.
     * @param star_idx Index of the star system to check.
     * @return true if an outpost is found on any planet in the system, false otherwise.
     */
    bool __cdecl Player_Has_Outpost_(int16_t unused_player_num, int16_t star_idx) {
        bool has_outpost = false;

        (void)unused_player_num;

        for (int16_t i = 0; i < 5; ++i) {
            // The binary checks result flag at the start of the loop block
            if (has_outpost) {
                break;
            }

            int16_t planet_idx = MOX::_star[star_idx].planet_index[i];
            if (planet_idx > -1) {
                int16_t colony_idx = MOX::_planet[planet_idx].colony_index;
                if (colony_idx > -1) {
                    // Offset 0x6 in s_colony is outpost_flag
                    if (MOX::_colony[colony_idx].outpost_flag != 0) {
                        has_outpost = true;
                    }
                }
            }
        }

        return has_outpost;
    }

    int8_t __cdecl Planet_Has_Outpost_(int16_t planet_idx) {
        int8_t has_outpost = 0;

        if (planet_idx != -1) {
            const int16_t colony_idx = MOX::_planet[planet_idx].colony_index;
            if (colony_idx != -1) {
                if (MOX::_colony[colony_idx].outpost_flag != 0) {
                    has_outpost = 1;
                }
            }
        }

        return has_outpost;
    }

    /**
     * @address 004f1050
     */
    int16_t __cdecl Galaxy_Size_From_N_Stars_() {
        int16_t default_galaxy_size = static_cast<int16_t>(static_cast<int8_t>(MOX::_settings.galaxy_size));

        if (MOX::_NUM_STARS <= 20) {
            return GALAXY_SIZE_SMALL;
        }
        if (MOX::_NUM_STARS <= 36) {
            return GALAXY_SIZE_MEDIUM;
        }
        if (MOX::_NUM_STARS <= 54) {
            return GALAXY_SIZE_LARGE;
        }
        if (MOX::_NUM_STARS <= 71) {
            return GALAXY_SIZE_HUGE;
        }
#if MAX_STARS > ORIGINAL_MAX_STARS
        if (MOX::_NUM_STARS < MAX_STARS) {
            return GALAXY_SIZE_MAXIMUM;
        }
#endif

        return default_galaxy_size;
    }

    bool __cdecl Has_Massive_Star_Count_() {
        return MAP_SCALE::Uses_Extended_Scaling_(MOX::_NUM_STARS, ORIGINAL_MAX_STARS);
    }

    int16_t __cdecl Map_Star_Scale_Percent_() {
        return MAP_SCALE::Star_Scale_Percent_(MOX::_NUM_STARS, ORIGINAL_MAX_STARS, MOX::_cur_map_scale);
    }

    int16_t __cdecl Map_Scale_Star_Size_To_Zoom_Level_(int16_t star_idx) {
        int8_t star_size = MOX::_star[star_idx].size;
        int16_t zoom_offset = Map_Scale_To_Zoom_Level_();

        int16_t combined_index = (int16_t)(zoom_offset + (int16_t)star_size);
        int16_t field_size = static_cast<int16_t>(MOX::_star_fields_dim[combined_index]);
        return MAP_SCALE::Scale_Star_Dimension_(field_size, Map_Star_Scale_Percent_());
    }

    /**
     * @address 004f0840
     */
    void __cdecl Decrement_Zoom_Count_() {
        MOX::_zoom_count--;

        if (MOX::_zoom_count < 1) {
            MOX::_zoom_count = 0;
        }
    }

    int16_t __cdecl Map_Scale_To_Zoom_Level_() {
        int16_t max_zoom = MOX::_max_zoom_count;
        int16_t scale = MOX::_cur_map_scale;

        if (Has_Massive_Star_Count_()) {
            return MAP_SCALE::Extended_Zoom_Level_For_Scale_(scale, MOX::_max_map_scale, max_zoom);
        }

        if (scale == 10) {
            if (max_zoom >= 0) {
                return 0;
            }
        } else if (scale == 15) {
            if (max_zoom >= 1) {
                return 1;
            }
        } else if (scale == 20) {
            if (max_zoom >= 2) {
                return 2;
            }
        } else if (max_zoom >= 3) {
            return 3;
        }

        return max_zoom;
    }

    /**
     * @address 004f0e20
     */
    void __cdecl Increment_Zoom_Count_() {
        MOX::_zoom_count++;
        if (MOX::_zoom_count >= MOX::_max_zoom_count) {
            MOX::_zoom_count = MOX::_max_zoom_count;
        }
    }

    int16_t __cdecl Increment_Zoom_Count_With_Limit_() {
        int32_t next_zoom = (int32_t)MOX::_zoom_count + 1;

        if (next_zoom >= (int32_t)MOX::_max_zoom_count) {
            next_zoom = (int32_t)MOX::_max_zoom_count;
        }

        MOX::_zoom_count = (int16_t)next_zoom;

        int16_t mapped_zoom = Map_Scale_To_Zoom_Level_();

        // Check if the mapped zoom level is lower or equal to the incremented zoom count
        if (mapped_zoom <= MOX::_zoom_count) {
            MOX::_zoom_count = mapped_zoom;
            return 0;
        }

        return 1;
    }

    int16_t __cdecl Get_Map_Scale_(int16_t layer_type) {
        int16_t scale;

        if (Has_Massive_Star_Count_()) {
            scale = MAP_SCALE::Extended_Scale_For_Zoom_Level_(MOX::_max_map_scale, MOX::_max_zoom_count, layer_type);
        } else if (layer_type == 0) {
            scale = MOX::_max_map_scale;
            if (scale >= 10) {
                scale = 10;
            }
        } else if (layer_type == 1) {
            scale = MOX::_max_map_scale;
            if (scale >= 15) {
                scale = 15;
            }
        } else if (layer_type == 2) {
            scale = MOX::_max_map_scale;
            if (scale >= 20) {
                scale = 20;
            }
        } else {
            scale = MOX::_max_map_scale;
            if (scale >= 30) {
                scale = 30;
            }
        }

        // If the resulting scale is unconstrained (equals the max), reset background star shifts
        if (scale == MOX::_max_map_scale) {
            MOX::_far_star_x = 0;
            MOX::_far_star_y = 0;
            MOX::_mid_star_x = 0;
            MOX::_mid_star_y = 0;
            MOX::_near_star_x = 0;
            MOX::_near_star_y = 0;
            scale = MOX::_max_map_scale;
        }

        return scale;
    }

    /**
     * Copies the currency unit string (e.g., "MC" or "BC") into the provided buffer.
     * The currency unit depends on the current game language setting.
     */
    void __cdecl Get_Monetary_Unit_String_(char* buffer) {
        const char* unit_text;

        switch (MOX::_settings.language) {
        case 1:
            unit_text = "MC";
            break;
        case 2:
        case 3:
        case 4:
        default:
            unit_text = "BC";
            break;
        }

        memcpy(buffer, unit_text, sizeof("MC"));
    }

    void __cdecl Planet_Data_Screen_() {
    }

    /**
     * @address 004f1330
     */
    int16_t __cdecl N_Player_Ships_(int16_t player_idx) {
        int16_t ship_idx = 0;
        int16_t ship_count = 0;

        do {
            int16_t owner_idx = HACCESS::Ship_Owner_(ship_idx);
            if (owner_idx == player_idx && static_cast<int8_t>(MOX::_ship[ship_idx].status) < 3) {
                ship_count = ship_count + 1;
            }
            ship_idx = ship_idx + 1;
        } while (ship_idx < MAX_SHIPS);

        return ship_count;
    }

    void __cdecl Get_Player_Font_Colors_(int16_t player_idx, int16_t style_idx, s_colors* style_buf, bool apply_selected_color_offset) {
        uint8_t base_color_idx = 1;
        uint8_t mode = 0;
        int16_t limit_color = 14;
        int16_t offset = apply_selected_color_offset ? (int16_t)(int8_t)MOX::_selected_star_color_offset : 0;

        if (player_idx >= MAX_PLAYERS && player_idx < 15) {
            base_color_idx = 4;
            mode = 1;
            limit_color = 11;
        } else {
            uint8_t p_color = MOX::_player[player_idx].color;
            if (p_color == 3) {
                base_color_idx = 12;
                mode = 2;
                limit_color = 22;
            } else if (player_idx < MAX_PLAYERS) {
                uint8_t palette_val = MOX::_main_palette_player_colors[p_color];
                base_color_idx = palette_val + 1;
                limit_color = palette_val + 6;
                if (p_color == 4) {
                    limit_color = palette_val + 5;
                }
            }
        }

        MISC::Get_Mox_Font_Colors_(style_idx, static_cast<char>(base_color_idx), static_cast<char>(limit_color), style_buf);

        for (int i = 1; i < 8; ++i) {
            int8_t adj;
            if (mode == 1) {
                adj = static_cast<int8_t>(offset);
            } else if (mode == 2) {
                adj = static_cast<int8_t>((offset * 7) / 4);
            } else {
                adj = static_cast<int8_t>((offset * 3) / 2);
            }

            int8_t color_val = static_cast<int8_t>(style_buf->colors[i]) + adj;
            int16_t limit = limit_color + 8;
            if (static_cast<int16_t>(color_val) > limit) {
                color_val = static_cast<int8_t>(limit);
            }
            style_buf->colors[i] = static_cast<uint8_t>(color_val);
        }
    }

    int16_t __cdecl User_Box_(uint8_t* refresh_flag, const char* message, int16_t type) {
        char buffer[100];
        int16_t result = -1;

        switch (type) {
        case 1:
            video::Set_Refresh_Full_();
            *refresh_flag = 1;
            result = GENDRAW::Confirmation_Box_(message);
            *refresh_flag = 1;
            video::Set_Refresh_Stencil_();
            break;

        case 2:
            strlcpy(buffer, MOX::_temp_string, sizeof(buffer));
            video::Set_Refresh_Full_();
            *refresh_flag = 1;
            TEXTBOX::Text_Box_(buffer, message);
            *refresh_flag = 1;
            video::Set_Refresh_Stencil_();
            break;

        case 3:
            video::Set_Refresh_Full_();
            *refresh_flag = 1;
            GENDRAW::Warning_Box_(message);
            *refresh_flag = 1;
            video::Set_Refresh_Stencil_();
            break;

        case 4:
            if (MOX::_global_var < 0) {
                MOX::_global_var = 0x7E; /* '~' */
            }
            video::Set_Refresh_Full_();
            *refresh_flag = 1;
            TEXTBOX::Timed_Text_Box_(buffer, message, static_cast<int16_t>(MOX::_global_var));
            *refresh_flag = 1;
            video::Set_Refresh_Stencil_();
            break;

        case 5:
            if (MOX::_global_var < 0) {
                MOX::_global_var = 0x7E; /* '~' */
            }
            video::Set_Refresh_Full_();
            *refresh_flag = 1;
            TEXTBOX::Timed_Text_Box_(nullptr, message, static_cast<int16_t>(MOX::_global_var));
            *refresh_flag = 1;
            video::Set_Refresh_Stencil_();
            MOX::_global_var = 0;
            break;

        default:
            video::Set_Refresh_Full_();
            *refresh_flag = 1;
            GENDRAW::Message_Box_(message);
            *refresh_flag = 1;
            video::Set_Refresh_Stencil_();
            break;
        }

        return result;
    }

    /**
     * @address 004f2200
     */
    int8_t __cdecl Was_Mouse_List_Saved_() {
        return _saved_mouse_list_count != 0;
    }

    /**
     * @address 004f1fb0
     */
    void __cdecl Draw_Multi_Colored_Box_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, s_colors* colors, int16_t color_offset) {
        int16_t inv_offset = 7 - color_offset;
        if (inv_offset <= 0) {
            inv_offset = 0;
        }

        s_colors rev_colors;
        for (int i = 0; i < 8; ++i) {
            rev_colors.colors[7 - i] = colors->colors[i];
        }

        // Top edges
        line::Multi_Colored_Line_(x1, y1, x2, y1, colors, color_offset);
        line::Multi_Colored_Line_(x1 + 1, y1 + 1, x2 - 1, y1 + 1, colors, inv_offset);
        line::Multi_Colored_Line_(x1 + 2, y1 + 2, x2 - 2, y1 + 2, colors, color_offset);

        // Right edges
        line::Multi_Colored_Line_(x2, y1, x2, y2, colors, color_offset);
        line::Multi_Colored_Line_(x2 - 1, y1 + 1, x2 - 1, y2 - 1, colors, inv_offset);
        line::Multi_Colored_Line_(x2 - 2, y1 + 2, x2 - 2, y2 - 2, colors, color_offset);

        // Bottom edges
        line::Multi_Colored_Line_(x1, y2, x2, y2, &rev_colors, color_offset);
        line::Multi_Colored_Line_(x1 + 1, y2 - 1, x2 - 1, y2 - 1, &rev_colors, inv_offset);
        line::Multi_Colored_Line_(x1 + 2, y2 - 2, x2 - 2, y2 - 2, &rev_colors, color_offset);

        // Left edges
        line::Multi_Colored_Line_(x1, y1, x1, y2, &rev_colors, color_offset);
        line::Multi_Colored_Line_(x1 + 1, y1 + 1, x1 + 1, y2 - 1, &rev_colors, inv_offset);
        line::Multi_Colored_Line_(x1 + 2, y1 + 2, x1 + 2, y2 - 2, &rev_colors, color_offset);
    }

    void __cdecl Set_Language_Race_Names_() {
        for (int16_t player_index = 0; player_index < MOX::_NUM_PLAYERS; ++player_index) {
            s_player* const player = &MOX::_player[player_index];

            if (player->objectives != PLAYER_OBJECTIVE_HUMAN) {
                const int16_t race_index =
                        static_cast<int16_t>(static_cast<uint8_t>(player->race));

                if ((race_index > -1) && (race_index < 20)) {
                    const char* const race_name = MOX::_race_names[race_index];
                    memcpy(player->race_name, race_name, strlen(race_name) + 1);
                }
            }
        }
    }

    void __cdecl Write_Language_Setting_File_() {
        char language_text[80];
        FILE* file = fopen_case(MOX2::s_language_ini, MOX2::s_language_ini_write_mode);

        if (file != nullptr) {
            snprintf(language_text, sizeof(language_text), "%u", (uint32_t)(uint8_t)MOX::_settings.language);
            fputs(language_text, file);
            fclose(file);
        }
    }

    void __cdecl Harold_Defaults_() {
        SYS::_remap_gray_percentages[0] = 0;
        SYS::_remap_gray_percentages[1] = 4;
        SYS::_remap_gray_percentages[2] = 9;
        SYS::_remap_gray_percentages[3] = 15;
        SYS::_remap_gray_percentages[4] = 22;
        SYS::_remap_gray_percentages[5] = 30;
        SYS::_remap_gray_percentages[6] = 39;
        SYS::_remap_gray_percentages[7] = 48;
        SYS::_remap_gray_percentages[8] = 56;
        SYS::_remap_gray_percentages[9] = 64;

        MOX::_multi_colored_line_start = 0;
        MOX::_multi_colored_box_color_start = 0;
        MOX::_far_star_x = 0;
        MOX::_far_star_y = 0;
        MOX::_box_off_flag[0] = -1;
        MOX::_mid_star_x = 0;
        MOX::_mid_star_y = 0;
        MOX::_near_star_x = 0;
        MOX::_near_star_y = 0;
        MOX::_box_off_flag[1] = 0;
        MOX::_box_off_flag[2] = 0;
        MOX::_box_off_flag[3] = 0;
        MOX::_box_off_flag[4] = 0;

        if (MOX::_game_type == 1) {
            memset(MAINSCR::_hotseat_fleet_box, 0, sizeof(MAINSCR::_hotseat_fleet_box));
            memset(MAINSCR::_hotseat_system_box, 0, sizeof(MAINSCR::_hotseat_system_box));
            memset(MAINSCR::_hotseat_map_scale, 0, sizeof(MAINSCR::_hotseat_map_scale));
        }

        MOX::_selected_star_color_mod = 1;
        MOX::_selected_star_color_offset = 0;

        for (int16_t i = 0; i < MAX_PLAYERS; ++i) {
            MAINSCR::_hotseat_system_box[i].system_id = -1;
        }

        for (int16_t i = 0; i < 5; ++i) {
            MOX::_saved_boxes[i].field_id = -1000;
            MOX::_saved_boxes[i].is_open = 0;
            MOX::_moveable_box[i].id = MOX::_box_off_flag[i];
        }

        MOX::_cur_map_scale = MOX::_max_map_scale;
        MOX::_galaxy_map_scanned_star = -1;
        MOX::_previous_report = 0;
        OFFICER::_officer_star_displayed = -1;
        MAINSCR::_already_faded_into_main_screen = 0;
        MOX::_star_bg_index = 2;
        MOX::_screen_data = 0;
        MOX::_animated_star = -1;
        MOX::_first_fleet_movement_icon = 0;
        MOX::_fleet_movement_bar_value = 0x66;
        MOX::_first_node_ptr_in_stack_ = -1;
        MOX::_eta_star_id = -1;
        MOX::_global_var = 0;
        MOX::_stencil_box_id = -1;
        MOX::_mainscr_palette = 0xFF;
        MOX::_draw_main_screen_stencilled = 0;
        MOX::_draw_expanding_box = 0;
        MOX::_fleet_ship_scanned = -1;
        MOX::_just_moved_stack = 0;
        MOX::_system_icon_scanned = -1;
        MOX::_draw_system_display_without_button = 0;
        MOX::_stardate_at_beg_of_turn = 0;
        MOX::_fltscrn_support_button_status = 1;
        MOX::_player_has_ship_in_system = 0;
        MOX::_fltscrn_combat_button_status = 1;
        MAINSCR::_next_turn_hit = 0;
        MAINSCR::_main_screen_ship_icon_scanned = -1;
        MOX::_officer_scrn_type = 1;
        MAINSCR2::_main_screen_fade_in_flag = 0;
        HAROLD::_set_fade_for_first_fltpop = 1;
        MOX::_star_fields_dim[0] = 0x21;
        MOX::_add_help_fields = 0xFF;
        MOX::_star_fields_dim[1] = 0x1D;
        MOX::_star_fields_dim[2] = 0x19;
        MOX::_star_fields_dim[3] = 0x17;
        MOX::_star_fields_dim[5] = 0x11;
        MOX::_system_display_ring_radius[2] = 0x2D;
        MOX::_system_display_ring_radius[3] = 0x39;
        MOX::_rot_plan_dim[0] = 0x13;
        MOX::_rot_plan_dim[1] = 0x14;
        MOX::_system_display_ring_radius[4] = 0x45;
        MOX::_rot_plan_dim[4] = 0x18;
        MOX::_galaxy_map_height = 0xA0;
        MOX::_galaxy_map_width = 0x75;
        MOX::_special_scroll_up_button[1] = 0x3B;
        MOX::_star_fields_dim[4] = 0x15;
        MOX::_system_display_ring_radius[0] = 0x15;
        MOX::_system_display_ring_radius[1] = 0x21;
        MOX::_rot_plan_dim[2] = 0x16;
        MOX::_rot_plan_dim[3] = 0x16;
        MOX::_blue_color2 = 0x3D;
        MOX::_red_color_pulse_value = 0;
        MOX::_red_color_pulse_incr = 1;
        MOX::_red_color1 = 0x49;
        MOX::_red_color2 = 0x4C;

        for (int16_t i = 0; i < MAX_STARS; ++i) {
            MOX::_black_hole_anim_count[i] = (int8_t)((i % 10) * 2);
        }
        MAINSCR::Clear_Twinkling_Star_Data_();
        memset(MOX::_fleet_icon_selection_status, 0, sizeof(MOX::_fleet_icon_selection_status));
    }

    void __cdecl Load_H_Strings_() {
        int16_t string_index = 0;
        int16_t scan_offset = 0;
        int16_t next_string_offset = 0;
        int32_t max_length = 0;
        uint32_t resource_id = 0;
        char strings_file[13];
        char error_message[200];

        if (static_cast<uint8_t>(MOX::_settings.language) >= 6) {
            MOX::_settings.language = 0;
        }

        switch (MOX::_settings.language) {
        case 1:
            memcpy(strings_file, "HGSTRNGS.LBX", sizeof(strings_file));
            max_length = 0x1A0;
            resource_id = 0x3D01;
            break;

        case 2:
            memcpy(strings_file, "HFSTRNGS.LBX", sizeof(strings_file));
            max_length = 0x118;
            resource_id = 0x408D;
            break;

        case 3:
            memcpy(strings_file, "HSSTRNGS.LBX", sizeof(strings_file));
            max_length = 0x1A1;
            resource_id = 0x3CE8;
            break;

        case 4:
            memcpy(strings_file, "HISTRNGS.LBX", sizeof(strings_file));
            max_length = 0x172;
            resource_id = 0x36CF;
            break;

        default:
            memcpy(strings_file, "HESTRNGS.LBX", sizeof(strings_file));
            max_length = 0x181;
            resource_id = 0x34D1;
            break;
        }

        memset(MOX::_h_strings, 0, sizeof(MOX::_h_strings));
        farload::Farload_Data_Static_(strings_file, 0, MOX::_h_strings, 0, 1, (int32_t)resource_id);

        for (;;) {
            int32_t string_length = 0;

            if (static_cast<int32_t>(scan_offset) < 0x4074 &&
                MOX::_h_strings[static_cast<int32_t>(scan_offset)] != '\0') {
                while (static_cast<int32_t>(scan_offset) < 0x4074 &&
                       MOX::_h_strings[static_cast<int32_t>(scan_offset)] != '\0') {
                    ++scan_offset;
                    ++string_length;
                }
            }

            if (static_cast<int32_t>(scan_offset) + 1 > 0x4074) {
                return;
            }

            if (string_length > max_length) {
                snprintf(
                    error_message,
                    sizeof(error_message),
                    "(%s) String #%d Too Long, check terminator (strlen: %i)",
                    strings_file,
                    static_cast<int32_t>(string_index),
                    string_length
                );
                exitm::Exit_With_Message(error_message);
            }

            ++scan_offset;
            MOX::_h_string_ix[static_cast<int32_t>(string_index)] = next_string_offset;
            ++string_index;
            next_string_offset = scan_offset;

            if (string_index == 397) {
                return;
            }
        }
    }
}