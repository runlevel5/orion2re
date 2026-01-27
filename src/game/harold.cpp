#include "deps.h"

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
        const char* orion_str = s_Orion_0055a738;
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

    void __cdecl Planet_Number_(int16_t planet_index) {
        s_planet_data* planet = &MOX::_planet[planet_index];
        int8_t star_idx = planet->star_index;
        s_star_data* star = &MOX::_star[star_idx];

        char orbital_pos = 0;
        for (int i = 0; i < 5; ++i) {
            int16_t current_p_idx = star->planet_index[i];

            if (current_p_idx == planet_index) {
                return;
            }

            if (current_p_idx > -1) {
                orbital_pos++;
            }
        }

        return;
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

    uint8_t __cdecl N_Sattelites_(int16_t star_index) {
        uint8_t count = 0;

        for (int i = 0; i < 5; ++i) {
            if (MOX::_star[star_index].planet_index[i] != -1) {
                count++;
            }
        }

        return count;
    }

    /**
     * @address 004f0860
     */
    bool __cdecl Empty_System_(int16_t star_idx) {
        return N_Sattelites_(star_idx) == 0;
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
        box.field4_0x8 = 0x16;
        box.field5_0xa = 0x16;

        box.field12_0x14 = box.width;
        box.field13_0x16 = 0x27;
    }

    /**
     * @address 004f0f60
     */
    void __cdecl Init_System_Display_Box_() {
        MOX::_moveable_box_pict = Buffer_Reload_(s_BUFFER0_LBX, 0, MOX::_global_cache_seg);

        MOX::_moveable_box[0].width = sprite::Get_Sprite_Width_(MOX::_moveable_box_pict);
        MOX::_moveable_box[0].height = sprite::Get_Sprite_Height_(MOX::_moveable_box_pict);

        if (MOX::_moveable_box[0].field4_0x8 < 0) {
            MOX::_moveable_box[0].field4_0x8 = 22;
        }

        if (MOX::_moveable_box[0].field5_0xa < 0) {
            MOX::_moveable_box[0].field5_0xa = 22;
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

            MOX::_moveable_box[i].field4_0x8 = -1;
            MOX::_moveable_box[i].field5_0xa = -1;
        }

        Init_Fleet_Movement_Box_();
        Init_System_Display_Box_();
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

    char* __cdecl H_Message_(int16_t id) {
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
}