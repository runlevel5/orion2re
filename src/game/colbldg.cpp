#include "deps.h"
#include <cstring>

namespace COLBLDG {

    int16_t __cdecl N_Prods_In_Queue_(int16_t product_id) {
        int16_t match_count = 0;
        for (int32_t i = 0; i < 7; ++i) {
            if (_current_item[i] == product_id) {
                match_count++;
            }
        }
        return match_count;
    }

    int __cdecl qsort_building_by_name_(const void* a, const void* b) {
        const int16_t* bld1 = static_cast<const int16_t*>(a);
        const int16_t* bld2 = static_cast<const int16_t*>(b);
        char* name1 = COLONY::Real_Building_Name_(*bld1);
        char* name2 = COLONY::Real_Building_Name_(*bld2);

        return strcmp(name1, name2);
    }

    void __cdecl Calculate_Building_Array_() {
        _building_indexes[0] = -2;
        _building_indexes[1] = -3;
        _n_building_indexes = 2;

        for (int16_t building_id = 1; building_id < BUILDING_COUNT; ++building_id) {
            bool can_build = COLCALC::Colony_Can_Build_Product_(&MOX::_colony[COLONY::_colony_handle], building_id);

            if ((((_n_building_indexes <= 54) && can_build) || COLONY::_cheats) && building_id != BUILDING_COLONY_BASE) {
                int16_t current_index = _n_building_indexes;
                _n_building_indexes++;
                _building_indexes[current_index] = building_id;
            }
        }

        int count_to_sort = static_cast<int>(_n_building_indexes) - 2;
        _qsort(&_building_indexes[2], count_to_sort, sizeof(int16_t), qsort_building_by_name_);
    }

    void __cdecl Calculate_Military_Array_() {
        _n_military_indexes = 0;

        s_colony& colony = MOX::_colony[COLONY::_colony_handle];
        s_planet_data& planet = MOX::_planet[colony.planet];

        int16_t uncolonized_worlds = Star_N_Uncolonized_Worlds_(planet.star_index);
        if (N_Prods_In_Queue_(11 /* Housing */) < uncolonized_worlds) {
            if (COLCALC::Colony_Can_Build_Product_(&colony, 11 /* Housing */)) {
                _military_indexes[_n_military_indexes++] = 11;
            }
        }

        s_player& current_player = MOX::_player[MOX::_PLAYER_NUM];

        if (current_player.tech_applications[TECH_APP_FREIGHTERS] == 3) {
            if (COLCALC::Colony_Can_Build_Product_(&colony, -15 /* Freighters */)) {
                _military_indexes[_n_military_indexes++] = -15;
            }
        }

        s_player& owner_player = MOX::_player[colony.owner];

        if (owner_player.ship_range > 0 && owner_player.ship_speed != 0) {
            if (owner_player.tech_applications[TECH_APP_COLONY_SHIP] == 3) {
                _military_indexes[_n_military_indexes++] = -12;
            }

            if (owner_player.tech_applications[TECH_APP_OUTPOST_SHIP] == 3) {
                _military_indexes[_n_military_indexes++] = -17;
            }

            if (owner_player.tech_applications[TECH_APP_TRANSPORT] == 3 &&
                colony.buildings[BUILDING_MARINE_BARRACKS] != 0) {
                _military_indexes[_n_military_indexes++] = -11;
            }

            if (_n_military_indexes != 0) {
                _military_indexes[_n_military_indexes++] = -9 /* Separator */;
            }

            for (int16_t i = 0; i < 5; ++i) {
                const char* default_design_text = ESTRINGS::E_Strings_(12 /* HESTR_00C_YOUR_CURRENT_DESIGN_IS */);
                const char* design_name = Selection_Name_(-50 - i);

                if (strcmp(default_design_text, design_name) != 0) {
                    bool can_build = false;

                    if (MOX::_settings.strategic_combat_flag == 0) {
                        can_build = true;
                    } else if (current_player.ship_designs[i].size != 4 /* Not a Titan */) {
                        can_build = true;
                    } else if (owner_player.tech_applications[TECH_APP_TITAN_CONSTRUCTION] == 3) {
                        can_build = true;
                    }

                    if (can_build) {
                        _military_indexes[_n_military_indexes++] = -50 - i;
                    }
                }
            }

            if (owner_player.tech_applications[TECH_APP_DOOM_STAR_CONSTRUCTION] == 3 &&
                MOX::_settings.strategic_combat_flag != 0) {
                _military_indexes[_n_military_indexes++] = -16;
            }
        }

        if (_n_military_indexes != 0) {
            _military_indexes[_n_military_indexes++] = -9 /* Separator */;
        }

        if (COLCALC::Colony_Can_Build_Product_(&colony, -4)) {
            _military_indexes[_n_military_indexes++] = -4;
        }
        if (COLCALC::Colony_Can_Build_Product_(&colony, -5)) {
            _military_indexes[_n_military_indexes++] = -5;
        }
        if (COLCALC::Colony_Can_Build_Product_(&colony, -6)) {
            _military_indexes[_n_military_indexes++] = -6;
        }
        if (COLCALC::Colony_Can_Build_Product_(&colony, -7)) {
            _military_indexes[_n_military_indexes++] = -7;
        }
    }

    void __cdecl Add_Build_Queue_Fields_() {
        int16_t y_end = 350;

        for (int32_t i = 0; i < 7; ++i) {
            char* hotkey = ESTRINGS::E_Strings_(12);

            _queue_fields[i] = fields::Add_Hidden_Field_(
                207,
                y_end - 21,
                458,
                y_end,
                hotkey,
                41
            );

            y_end += 20;
        }
    }

    void __cdecl Build_Arrays_() {
        Calculate_Building_Array_();
        Calculate_Military_Array_();
    }

    int16_t __cdecl Create_Ship_(int16_t player_idx, int16_t design_idx, int16_t colony_idx) {
        int16_t ship_idx = 0;
        int16_t planet_idx = MOX::_colony[colony_idx].planet;
        int16_t star_idx = (int16_t)MOX::_planet[planet_idx].star_index;

        while (ship_idx < 500) {
            auto& ship = MOX::_ship[ship_idx];
            if (ship.owner == (int8_t)player_idx) {
                if (ship.status == 5 || ship_idx >= (int16_t)MOX::_NUM_SHIPS) {
                    break;
                }
            }
            ship_idx++;
        }

        if (ship_idx >= 500) {
            return -1;
        }

        if (ship_idx >= (int16_t)MOX::_NUM_SHIPS) {
            MOX::_NUM_SHIPS = ship_idx + 1;
        }

        auto& new_ship = MOX::_ship[ship_idx];
        memset(&new_ship, 0, sizeof(s_ship_data));

        Copy_Structure_(&new_ship.d, &MOX::_player[player_idx].ship_designs[design_idx], 0x63);
        new_ship.owner = (int8_t)player_idx;
        new_ship.d.previous_owner = (int8_t)player_idx;
        new_ship.status = 0;
        new_ship.location = star_idx;
        new_ship.x = MOX::_star[star_idx].x;
        new_ship.y = MOX::_star[star_idx].y;
        new_ship.officer_index = -1;

        if (MOX::_settings.strategic_combat_flag != 0) {
            char name_buffer[500];
            int ships_built_count = MOX::_player[player_idx].ships_built[(int)new_ship.d.size];

            _sprintf(name_buffer, ESTRINGS::E_Strings_(0x2a), new_ship.d.name, ships_built_count);

            if (strlen(name_buffer) >= 15) {
                _sprintf(name_buffer, ESTRINGS::E_Strings_(0x48), new_ship.d.name, ships_built_count);
                if (strlen(name_buffer) >= 15) {
                    strcpy(name_buffer, new_ship.d.name);
                }
            }

            strcpy(new_ship.d.name, name_buffer);
            MOX::_player[player_idx].ships_built[(int)new_ship.d.size]++;
        }

        return ship_idx;
    }

    void __cdecl Build_Queue_Popup_() {
        uint32_t original_seed = random::Get_Random_Seed_();
        random::Set_Random_Seed_(static_cast<uint32_t>(COLONY::_colony_handle));

        COLONY::_star_handle = static_cast<int32_t>(MOX::_screen_data);
        COLONY::_orbit_n = static_cast<int32_t>(COLONY::_orbit_temp);

        int16_t planet_idx = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);
        COLONY::_planet_handle = static_cast<int32_t>(planet_idx);

        int32_t colony_idx = static_cast<int32_t>(MOX::_planet[planet_idx].colony_index);
        COLONY::_colony_handle = colony_idx;
        COLBLDG::_last_app = -1;
        COLONY::_using_colony_screen_palette = 1;

        if (MOX::_settings.strategic_combat_flag != 0) {
            int8_t owner = MOX::_colony[colony_idx].owner;
            INITSHIP::Load_Doom_Star_Ship_Design_(owner, &MOX::_player[owner].ship_designs[5]);
        }

        if (MOX::_return_screen != MOX::_current_screen && COLBLDG::_return_to_build_popup_from_custom_design == 0) {
            COLBLDG::_colony_bldg_saved_screen = MOX::_return_screen;
        }

        COLONY::_sys_disp_star_handle = COLONY::_star_handle;
        COLBLDG::Clear_Last_App_Desc_();
        fonts::Set_Font_Style_Shadow_Down_(4, reinterpret_cast<s_colors*>(COLBLDG::_l_font_color_array));
        fonts::Set_Font_Spacing_(1);
        COLONY::Load_Colony_Screen_Seg_();
        COLONY::Load_Planet_Screen_();
        ERICHELP::Set_Building_Queue_Screen_Help_List_();

        if (COLBLDG::_return_to_build_popup_from_custom_design == 0) {
            for (int i = 0; i < 7; ++i) {
                COLBLDG::_current_item[i] = MOX::_colony[colony_idx].producing[i];
            }
            COLBLDG::_active_prod = -1;
        }

        COLBLDG::Build_Arrays_();

        if (COLBLDG::_return_to_build_popup_from_custom_design == 0) {
            COLBLDG::_colony_auto_building = static_cast<int16_t>(static_cast<uint8_t>(MOX::_colony[colony_idx].auto_building));
        }

        COLONY::Make_Bldg_Array_For_Colony_(COLONY::_colony_handle, 0);

        if (ERIC::Fast_Fade_Out_Needed_For_Colony_Palette_()) {
            ERIC::Fast_Fade_Out_();
            COLONY::Update_Colony_Palette_();
        }

        COLBLDG::Update_Fields_();
        COLONY::_full_draw = 1;
        fields::Assign_Auto_Function_(COLBLDG::Draw_Build_Queue_Popup_, 1);
        ERICHELP::Set_Building_Queue_Screen_Help_List_();
        palette::Clear_Palette_Changes_(0, 0xFF);
        COLBLDG::Draw_Build_Queue_Popup_();
        JIM::Fast_Fade_In_();
        video::Set_Refresh_Stencil_();

        int16_t field_id;
        int16_t scan_code;
        do {
            COLBLDG::Draw_Build_Queue_Popup_();
            RUSS::Mox_Sync_Update_();
            field_id = fields::Get_Input_();
            scan_code = fields::Scan_Input_();
            fields::Evaluate_Input_(&field_id, &scan_code);

            if (field_id == COLBLDG::_fields[3] ||
                field_id == COLBLDG::_fields[0] ||
                field_id == COLBLDG::_fields[1])
            {
                break;
            }
            if (COLBLDG::_return_to_build_popup_from_custom_design != 0) {
                break;
            }
            if (field_id == COLBLDG::_fields[9] ||
                field_id == COLBLDG::_fields[8])
            {
                break;
            }
        } while (true);

        fields::Clear_Fields_();
        if (COLBLDG::_active_prod != -1) {
            COLONY::Reset_Mouse_Picture_();
        }

        fonts::Set_Font_Spacing_(3);
        fields::Deactivate_Help_List_();
        fields::Deactivate_Auto_Function_();
        video::Copy_Back_To_Off_();

        if (field_id != COLBLDG::_fields[9] && field_id != COLBLDG::_fields[8]) {
            if (COLBLDG::_return_to_build_popup_from_custom_design != 0) {
                MOX::_first_design_screen_call = 1;
                MOX::_return_screen = MOX::_current_screen;
                MOX::_current_screen = SCREEN_DESIGN;
            } else {
                if (MOX::_return_screen != MOX::_current_screen) {
                    MOX::_current_screen = MOX::_return_screen;
                } else {
                    MOX::_current_screen = COLBLDG::_colony_bldg_saved_screen;
                    MOX::_return_screen = COLBLDG::_colony_bldg_saved_screen;
                }
            }
        }

        random::Set_Random_Seed_(original_seed);
    }

    int16_t __cdecl Star_N_Uncolonized_Worlds_(int16_t star_index) {
        int16_t uncolonized_count = 0;
        const s_star_data& star = MOX::_star[star_index];

        for (int16_t i = 0; i < 5; ++i) {
            int16_t planet_idx = star.planet_index[i];
            if (planet_idx != -1) {
                const s_planet_data& planet = MOX::_planet[planet_idx];

                bool is_available = false;
                if (planet.colony_index == -1) {
                    is_available = true;
                } else {
                    const s_colony& colony = MOX::_colony[planet.colony_index];
                    if (colony.outpost_flag != 0) {
                        is_available = true;
                    }
                }

                if (is_available && planet.planet_type == 3) {
                    uncolonized_count++;
                }
            }
        }

        return uncolonized_count;
    }

    void __cdecl Clear_Last_App_Description_String_() {
        _last_app = -1;
    }

    void __cdecl Clear_Last_App_Desc_() {
        Clear_Last_App_Description_String_();
        _buffer = allocate::Allocate_Next_Block_(MOX::_screen_seg, 0xAF6);
    }

    char* __cdecl Selection_Name_(int16_t id) {
        if (id > 0 && id < BUILDING_COUNT) {
            return COLONY::Real_Building_Name_(id);
        }

        if (id > -100) {
            return Option_String_(id);
        }

        int index = -100 - static_cast<int>(id);
        s_ship_data* ship = &MOX::_ship[index];

        if (ship->status != 4) {
            return reinterpret_cast<char*>(ship);
        }

        _last_setting = -1;
        _last_app = -1;

        char* opt_str = Option_String_(-8);
        char* format = ESTRINGS::E_Strings_(45);
        _sprintf(_g_msg, format, opt_str, ship);

        return _g_msg;
    }

} // namespace COLBLDG