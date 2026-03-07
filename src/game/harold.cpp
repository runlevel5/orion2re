#include "pch.h"

namespace HAROLD {
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

            // Check if slot contains a valid planet (-1 is empty/gas giant in some contexts)
            if (planet_idx != -1) {
                // The assembly specifically checks if planet_type is 0x03
                if (MOX::_planet[planet_idx].planet_type == 3) {
                    count++;
                }
            }
        }

        return count;
    }

    void __cdecl Get_Officer_Data_LBX_Name_(char* out) {
        const char* filename;

        switch (MOX::_settings.language) {
        case 1:  // German
            filename = s_HERODATG_LBX;
            break;
        case 2:  // French
            filename = s_HERODATF_LBX;
            break;
        case 3:  // Spanish
            filename = s_HERODATS_LBX;
            break;
        case 4:  // Italian
            filename = s_HERODATI_LBX;
            break;
        default: // English / Standard
            filename = s_HERODATA_LBX;
            break;
        }

        for (int32_t i = 0; i < 13; ++i) {
            out[i] = filename[i];
        }
    }

    /**
     * Checks if a star system is Orion.
     * Uses assembly REPE CMPSB with ECX=6 comparison.
     */
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

    /**
     * Checks if a star system has a habitable spectral class (O, B, A, F, G, or K).
     * Spectral classes are indexed such that habitable ones are < 6.
     */
    bool __cdecl Habitable_Spectral_Class_(int16_t star_idx) {
        // Star data structure size is 113 bytes.
        return MOX::_star[star_idx].spectral_class < 6;
    }

    /**
     * Determines if a star system contains only outposts (no full colonies).
     * Returns true if there is at least one outpost and zero established colonies.
     * @address 004f1a20
     */
    bool __cdecl Star_Is_Outpost_Star_(int16_t star_idx) {
        int8_t outpost_count = 0;
        int8_t colony_count = 0;

        for (int32_t i = 0; i < 5; ++i) {
            // Planet index at offset 0x4a in s_star_data
            int16_t planet_idx = MOX::_star[star_idx].planet_index[i];

            if (planet_idx != -1) {
                // Colony index at offset 0x0 in s_planet_data
                int16_t colony_idx = MOX::_planet[planet_idx].colony_index;

                if (colony_idx != -1) {
                    // outpost_flag at offset 0x6 in s_colony
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
        int8_t star_idx = MOX::_planet[planet_index].star_index;
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
        const char* src;

        switch (MOX::_settings.language) {
        case 1: // German
            src = "fontsg.lbx";
            break;
        case 2: // French
            src = "fontsf.lbx";
            break;
        case 3: // Spanish
            src = "fontss.lbx";
            break;
        case 4: // Italian
            src = "fontsi.lbx";
            break;
        default: // English or Other
            src = "fonts.lbx";
            break;
        }

        _memcpy(buffer, src, 10);
    }

    int16_t __cdecl Get_Scaled_Value_(int16_t value) {
        return static_cast<int16_t>(static_cast<int32_t>(value * 10) / static_cast<int32_t>(MOX::_cur_map_scale));
    }

    int32_t __cdecl Get_Up_Scaled_Value_(int16_t value) {
        return (static_cast<int32_t>(value) * static_cast<int32_t>(MOX::_cur_map_scale)) / 10;
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
        uint8_t owners_seen[8];
        int16_t unique_count = 0;

        for (int i = 0; i < 8; ++i) {
            owners_seen[i] = 0;
        }

        for (int16_t i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[star_idx].planet_index[i];

            if (planet_idx > -1) {
                int16_t colony_idx = MOX::_planet[planet_idx].colony_index;

                if (colony_idx > -1) {
                    uint8_t owner_id = static_cast<uint8_t>(MOX::_colony[colony_idx].owner);

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
                int iters = (count * 5) / 3;
                for (int i = 0; i < iters; ++i) {
                    int idx1 = random::Random_(count) - 1;
                    int idx2 = random::Random_(count) - 1;
                    if (idx1 != idx2 && candidates[idx1] != -1 && candidates[idx2] != -1) {
                        int16_t temp = candidates[idx1];
                        candidates[idx1] = candidates[idx2];
                        candidates[idx2] = temp;
                    }
                }

                int16_t wormhole_exit_id = -1;
                int pick_counter = 1;
                for (int i = 0; i < count; ++i) {
                    if (candidates[i] != -1) {
                        if (random::Random_(pick_counter) == 1) {
                            wormhole_exit_id = candidates[i];
                            pick_counter++;
                        }
                    }
                }

                if (wormhole_exit_id != -1) {
                    MOX::_star[wormhole_exit_id].wormhole_star_id = (int8_t)star_idx;
                    MOX::_star[star_idx].wormhole_star_id = (int8_t)wormhole_exit_id;
                    return;
                }
            }
        }

        MOX::_star[star_idx].wormhole_star_id = 0;
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
    uint8_t __cdecl N_Food_Planets_In_System_(int16_t system_index) {
        uint8_t food_planet_count = 0;

        for (int16_t i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[system_index].planet_index[i];

            if (planet_idx > -1) {
                if (MOX::_planet[planet_idx].food_per_farmer > 0) {
                    food_planet_count++;
                }
            }
        }

        return food_planet_count;
    }

    /**
     * @address 004f1930
     */
    void __cdecl Set_Player_Font_Style_(int16_t player_idx, bool param_2) {
        s_colors style;

        int16_t style_idx = Zoom_Level_Font_Style_();

        Get_Player_Font_Colors_(player_idx, style_idx, &style, param_2);

        fonts::Set_Font_Style_(style_idx, &style);
    }

    void __cdecl Set_Sound_Volume_To_Settings_() {
        uint8_t music_lvl = static_cast<uint8_t>(MOX::_settings.music_level);
        if (music_lvl > 100) {
            music_lvl = 100;
        }

        if (MOX::_settings.music_on == 0) {
            sound::Stop_Stream_();
        }
        else {
            sound::Stream_On_();
        }

        int32_t volume_param = static_cast<int32_t>(music_lvl);
        sound::Set_Stream_Max_Volume_(volume_param);
        sound::Set_Stream_Volume_(volume_param);

        if (MOX::_settings.sound_fx_on == 0) {
            sound::Stop_All_Sounds_();
        }
        else {
            sound::Sound_On_();
            sound::Resume_Sound_(-1);
        }

        sound::Set_Sound_Max_Volume_(static_cast<int32_t>(MOX::_settings.sound_fx_level));
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

        box.field10_0x14 = box.width;
        box.field11_0x16 = 0x27;
    }

    /**
     * @address 004f0f60
     */
    void __cdecl Init_System_Display_Box_() {
        MOX::_moveable_box_pict = reinterpret_cast<s_animation_header*>(Buffer_Reload_(s_BUFFER0_LBX, 0, MOX::_global_cache_seg));

        MOX::_moveable_box[0].width = animate::Get_Width_(MOX::_moveable_box_pict);
        MOX::_moveable_box[0].height = animate::Get_Height_(MOX::_moveable_box_pict);

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
        uint32_t* raw_ptr = reinterpret_cast<uint32_t*>(MOX::_moveable_box);
        for (int i = 0; i < 35; ++i) {
            raw_ptr[i] = 0;
        }

        for (int i = 0; i < 5; ++i) {
            MOX::_moveable_box[i].index = static_cast<uint16_t>(i);
            MOX::_moveable_box_order[i] = static_cast<uint16_t>(i);

            MOX::_moveable_box[i].off_flag = static_cast<int16_t>(MOX::_box_off_flag[i]);

            MOX::_moveable_box[i].x = -1;
            MOX::_moveable_box[i].y = -1;
        }

        Init_Fleet_Movement_Box_();
        Init_System_Display_Box_();
    }

    /**
     * @address 004f1590
     */
    bool __cdecl Player_Has_Colony_(short player_num, short star_idx) {
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
    bool __cdecl Player_Has_Visited_(int16_t player_idx, int16_t star_idx) {
        return (MOX::_star[star_idx].visited & (1 << (static_cast<uint8_t>(player_idx) & 0x1F))) != 0;
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
     * @param player_counts Array of size 8 that receives colony count per player index.
     * @param total_count Pointer to variable that receives the number of unique players owning colonies in this system.
     * @param have_contact If true, only counts owners that the current player has diplomatic contact with.
     */
    void __cdecl Colony_Owners_(int16_t star_idx, int16_t* player_counts, int16_t* total_count, bool have_contact) {
        *total_count = 0;
        for (int i = 0; i < 8; ++i) {
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
    void __cdecl Randomize_List_(int16_t* list, int16_t size, int8_t use_clock) {
        // The number of shuffle iterations is 1.5 * size
        int32_t iterations = (static_cast<int32_t>(size) * 3) / 2;

        if (iterations <= 0) {
            return;
        }

        int16_t count = 0;
        do {
            int16_t idx1, idx2;

            if (use_clock == 0) {
                // random::Random_ likely returns a value in range [1, size]
                idx1 = static_cast<int16_t>(random::Random_(static_cast<int32_t>(size))) - 1;
                idx2 = static_cast<int16_t>(random::Random_(static_cast<int32_t>(size))) - 1;
            } else {
                idx1 = static_cast<int16_t>(_clock() % static_cast<uint32_t>(size));
                idx2 = static_cast<int16_t>(_clock() % static_cast<uint32_t>(size));
            }

            if (idx1 != idx2) {
                int16_t temp = list[idx1];
                list[idx1] = list[idx2];
                list[idx2] = temp;
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

    const char* __cdecl H_Message_(int16_t id) {
        uint16_t buffer_offset = 0;

        if (id >= 0 && id < 397) {
            buffer_offset = MOX::_h_string_ix[id];
        }

        return &MOX::_h_strings[buffer_offset];
    }

    /**
     * @address 004f2190
     */
    void __cdecl Save_Mouse_List_() {
        uint16_t count = mouse::_current_mouse_list_count;

        if (count > 10) {
            count = 10;
        }

        _saved_mouse_list_count = count;

        uint32_t dword_count = static_cast<uint32_t>(count) * 3;
        uint32_t* src = reinterpret_cast<uint32_t*>(mouse::_current_mouse_list);
        uint32_t* dest = reinterpret_cast<uint32_t*>(_saved_mouse_list);

        for (uint32_t i = 0; i < dword_count; ++i) {
            dest[i] = src[i];
        }
    }

    /**
     * @address 004f0640
     */
    /**
     * @address 004f21d0
     */
    void __cdecl Restore_Mouse_List_() {
        mouse::Set_Mouse_List_(_saved_mouse_list, _saved_mouse_list_count);
        _saved_mouse_list_count = 0;
    }

    void Confirm_Planet_And_Star_Index_Link_(const char* msg) {
        for (int16_t i = 0; i < MOX::_planet_count; ++i) {
            Planet_Number_(i);
        }

        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            int16_t orbit_taken[5] = { 0, 0, 0, 0, 0 };
            s_star_data* star = &MOX::_star[star_idx];

            for (int16_t orbit_idx = 0; orbit_idx < 5; ++orbit_idx) {
                int16_t planet_idx = star->planet_index[orbit_idx];
                if (planet_idx != -1) {
                    s_planet_data* planet = &MOX::_planet[planet_idx];
                    int8_t planet_orbit = planet->orbit;

                    if (orbit_taken[planet_orbit] != 0) {
                        RUSS::PFMT_(0, 1, s_orbit__d_of_star__d_has_more_tha_0055a704, (int32_t)planet_orbit, (int32_t)star_idx);
                        RUSS::PFMT_(0, 2, s_relate_game_conditions_EXACTLY_a_0055a6cc);
                        RUSS::PFMT_(0, 3, s___s__00556ae4, star->name);
                        hgetch_();
                    }
                    orbit_taken[orbit_idx] = 1;
                }
            }
        }
    }

    void __cdecl Load_Leader_Titles() {
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
                    char* src_title = reinterpret_cast<char*>(record_ptr) + 15;
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
            if (MOX::_player[i].objectives == 0x64 /* 'd' */ && MOX::_player[i].eliminated == 0) {
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
     * @address 004f1ab0
     */
    int16_t __cdecl XY_To_Star_Id_(int16_t x, int16_t y) {
        int32_t count = (int32_t)MOX::_NUM_STARS + 1;

        for (int32_t i = 0; i < count; ++i) {
            if (MOX::_star[i].x == x && MOX::_star[i].y == y) {
                return (int16_t)i;
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
        if (location >= 500 && location < 1000) {
            return location - 500;
        }

        if (location >= 1000 && location < 1072 /* 0x430 */) {
            return location - 1000;
        }

        return location;
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
                // Owners >= 8 (specifically owner 8) are typically neutral monsters (Orion Guardians, etc).
                if (ship->status == 0 && ship->owner >= 8 && ship->location == location_idx) {
                    found = true;
                    *monster_owner = (int16_t)ship->owner;
                    break;
                }

                node_idx = MOX::_ship_node[node_idx].next_node;
            }
        }

        return found;
    }

    int16_t __cdecl Star_Owner_(int16_t star_idx, int16_t player_idx) {
        int16_t player_counts[MAX_PLAYERS];
        int16_t unk_val;

        bool hide_unexplored = !HAROLD::Player_Is_Omniscient_(player_idx);

        Colony_Owners_(star_idx, player_counts, &unk_val, hide_unexplored);

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

    int16_t __cdecl Zoom_Level_Font_Style_() {
        int16_t scale = MOX::_cur_map_scale;

        if (scale == 10) {
            return 3;
        }

        if (scale == 15 || scale == 20) {
            return 2;
        }

        if (scale == 30) {
            return 1;
        }

        // BUG: the function returns an uninitialized value if no conditions are met
        //int16_t uninitialized_result;
        //return uninitialized_result;
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

        // Zero out the 8-byte array tracking home star rename status
        for (int i = 0; i < 8; ++i) {
            MOX::_given_chance_to_rename_home_star[i] = 0x00;
        }

        return MOX::_given_chance_to_rename_home_star;
    }

    /**
     * @brief Checks if a player has an outpost in a specific star system.
     * 
     * @param param_1 Unused in original binary.
     * @param star_idx Index of the star system to check.
     * @return true if an outpost is found on any planet in the system, false otherwise.
     */
    bool __cdecl Player_Has_Outpost_(int16_t param_1, int16_t star_idx) {
        bool has_outpost = false;

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

    /**
     * @address 004f1050
     */
    int16_t __cdecl Galaxy_Size_From_N_Stars_() {
        int16_t default_galaxy_size = static_cast<int16_t>(static_cast<int8_t>(MOX::_settings.galaxy_size));

        if (MOX::_NUM_STARS <= 20) {
            return 0;
        }
        if (MOX::_NUM_STARS <= 36) {
            return 1;
        }
        if (MOX::_NUM_STARS <= 54) {
            return 2;
        }
        if (MOX::_NUM_STARS <= 72) {
            return 3;
        }

        return default_galaxy_size;
    }

    int16_t __cdecl Map_Scale_Star_Size_To_Zoom_Level_(int16_t star_idx) {
        int8_t star_size = MOX::_star[star_idx].size;
        int16_t zoom_offset = Map_Scale_To_Zoom_Level_();

        int16_t combined_index = (int16_t)(zoom_offset + (int16_t)star_size);
        return static_cast<int16_t>(MOX::_star_fields_dim[combined_index]);
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
     * @brief Increments the zoom count up to the maximum zoom limit,
     * then caps it further based on the current map scale.
     *
     * @return int16_t 0 if zoom was capped by the map scale limit, 1 otherwise.
     */
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
        int32_t next_zoom = static_cast<int32_t>(MOX::_zoom_count) + 1;
        int32_t max_allowed = static_cast<int32_t>(MOX::_max_zoom_count);

        if (next_zoom >= max_allowed) {
            next_zoom = max_allowed;
        }
        MOX::_zoom_count = static_cast<int16_t>(next_zoom);

        // Fetch a dynamic limit based on the current map scale
        int16_t scaled_limit = HAROLD::Map_Scale_To_Zoom_Level_();

        if (scaled_limit <= MOX::_zoom_count) {
            MOX::_zoom_count = scaled_limit;
            return 0;
        }

        return 1;
    }

    int16_t __cdecl Get_Map_Scale_(int16_t param_1) {
        int16_t result = MOX::_max_map_scale;

        if (param_1 == 0) {
            if (result > 10) {
                result = 10;
            }
        }
        else if (param_1 == 1) {
            if (result > 15) {
                result = 15;
            }
        }
        else if (param_1 == 2) {
            if (result > 20) {
                result = 20;
            }
        }
        else {
            if (result > 30) {
                result = 30;
            }
        }

        // If no truncation occurred, reset star coordinates
        if (result == MOX::_max_map_scale) {
            MOX::_far_star_x = 0;
            MOX::_far_star_y = 0;
            MOX::_mid_star_x = 0;
            MOX::_mid_star_y = 0;
            MOX::_near_star_x = 0;
            MOX::_near_star_y = 0;

            // Re-load result as per original binary's return sequence
            result = MOX::_max_map_scale;
        }

        return result;
    }

    /**
     * Copies the currency unit string (e.g., "MC" or "BC") into the provided buffer.
     * The currency unit depends on the current game language setting.
     */
    void __cdecl Get_Monetary_Unit_String_(char* out) {
        const char* src;

        switch (MOX::_settings.language) {
        case 1:
            src = s_MC_0055823c;
            break;
        case 2:
        case 3:
        case 4:
        default:
            src = s_BC_00558244;
            break;
        }

        // Binary-faithful copy of 3 bytes (including null terminator)
        out[0] = src[0];
        out[1] = src[1];
        out[2] = src[2];
    }

    void __cdecl Get_Player_Font_Colors_(int16_t player_idx, int16_t style_idx, s_colors* style_buf, bool param_2) {
        uint8_t base_color_idx = 1;
        uint8_t mode = 0;
        int16_t limit_color = 14;
        int16_t offset = param_2 ? static_cast<int16_t>(static_cast<int8_t>(MOX::_selected_star_color_offset)) : 0;

        if (player_idx >= 8 && player_idx < 15) {
            base_color_idx = 4;
            mode = 1;
            limit_color = 11;
        } else {
            uint8_t p_color = MOX::_player[player_idx].color;
            if (p_color == 3) {
                base_color_idx = 12;
                mode = 2;
                limit_color = 22;
            } else if (player_idx < 8) {
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
}