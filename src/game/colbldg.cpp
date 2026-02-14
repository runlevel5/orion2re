#include "deps.h"
#include <cstring>

namespace COLBLDG {

    int16_t __cdecl Calc_Font_Off_() {
        if (COLONY::_step_squish == 0) {
            return 0;
        }

        // Returns 3 if _step_squish >= 5, otherwise 2.
        return (COLONY::_step_squish >= 5 ? 1 : 0) + 2;
    }

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

    /**
     * @address 0050a0b0
     */
    void __cdecl Add_Militarys_Fields_() {
        COLDRAW::Calculate_Squish_Step_(13, 308, _n_military_indexes, 19);

        for (int16_t i = 0; i < _n_military_indexes; ++i) {
            int16_t step = 19 - COLONY::_step_squish;
            char* hotkey = ESTRINGS::E_Strings_(12);

            int16_t field_id = fields::Add_Hidden_Field_(
                485,
                i * step + 20,
                623,
                (i + 1) * step + 19,
                hotkey,
                41
            );

            _military_fields[i] = field_id;
        }
    }

    /**
     * @address 0050a020
     */
    void __cdecl Add_Buildings_Fields_() {
        COLDRAW::Calculate_Squish_Step_(13 /* 0x0D */, 469 /* 0x1D5 */, _n_building_indexes, 19 /* 0x13 */);

        if (_n_building_indexes > 0) {
            for (int16_t i = 0; i < _n_building_indexes; ++i) {
                int16_t step_size = 19 /* 0x13 */ - COLONY::_step_squish;
                char* hotkey = ESTRINGS::E_Strings_(12 /* 0x0C */);

                // Coordinates calculation
                int16_t y1 = i * step_size + 20 /* 0x14 */;
                int16_t y2 = (i + 1) * step_size + 19 /* 0x13 */;

                _building_fields[i] = fields::Add_Hidden_Field_(
                    13 /* 0x0D */,
                    y1,
                    184 /* 0xB8 */,
                    y2,
                    hotkey,
                    41 /* 0x29 */
                );
            }
        }
    }

    /**
     * @address 0050e440
     */
    void __cdecl Add_Build_Queue_Fields_() {
        int16_t y_end = 350 /* 0x15e */;

        for (int i = 0; i < 7; ++i) {
            char* hotkey = ESTRINGS::E_Strings_(12 /* 0xc */);

            // Adds a hidden field for each item in the build queue
            _queue_fields[i] = fields::Add_Hidden_Field_(
                207 /* 0xcf */,
                y_end - 21 /* 0x15 */,
                458 /* 0x1ca */,
                y_end,
                hotkey,
                41 /* 0x29 */
            );

            y_end += 20 /* 0x14 */;
        }
    }

    void __cdecl Update_Fields_() {
        fields::Clear_Fields_();

        _fields[0] = fields::Add_Button_Field_(493, 447, ESTRINGS::E_Strings_(12), _anims[1], ESTRINGS::E_Strings_(12), 40);
        _fields[3] = fields::Add_Button_Field_(560, 447, ESTRINGS::E_Strings_(12), _anims[3], ESTRINGS::E_Strings_(12), 40);

        _fields[1] = fields::Add_Hot_Key_(ESTRINGS::E_Strings_(135));
        _fields[8] = fields::Add_Hot_Key_(ESTRINGS::E_Strings_(139));
        _fields[9] = fields::Add_Hot_Key_(ESTRINGS::E_Strings_(140));
        _fields[10] = fields::Add_Hot_Key_(ESTRINGS::E_Strings_(193));

        _fields[2] = fields::Add_Radio_Button_Field_(490, 342, ESTRINGS::E_Strings_(12), _anims[2], &_colony_auto_building, ESTRINGS::E_Strings_(12), 41);

        if (MOX::_settings.strategic_combat_flag != 0) {
            _fields[4] = -1000;
            _fields[5] = -1000;
        } else {
            _fields[4] = fields::Add_Button_Field_(492, 379, ESTRINGS::E_Strings_(12), _anims[4], ESTRINGS::E_Strings_(436 /* ESTR_R */), 40);
            _fields[5] = fields::Add_Button_Field_(561, 379, ESTRINGS::E_Strings_(12), _anims[5], ESTRINGS::E_Strings_(251 /* ESTR_D3 */), 40);
        }

        _fields[6] = fields::Add_Button_Field_(503, 411, ESTRINGS::E_Strings_(12), _anims[6], const_cast<char*>(s_e_0055ac48), 40);
        _fields[7] = -1000;

        Add_Build_Queue_Fields_();
        Add_Buildings_Fields_();
        Add_Militarys_Fields_();
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
        fonts::Set_Font_Style_Shadow_Down_(4, &COLBLDG::_l_font_color_array);
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

    void __cdecl Draw_Militarys_List_() {
        int16_t font_offset;

        COLDRAW::Calculate_Squish_Step_(13, 308, _n_military_indexes, 19);
        font_offset = Calc_Font_Off_();
        Get_Font_Off_For_Military_(&font_offset);

        int16_t style_y_offset = 4 - font_offset;
        fonts::Set_Font_Style_Shadow_Down_(style_y_offset, &_l_font_color_array);

        for (int16_t i = 0; i < _n_military_indexes; ++i) {
            int16_t mil_idx = _military_indexes[i];
            bool use_dimmed_font = false;

            if (mil_idx > -100 && mil_idx <= -50) {
                s_colony* colony = &MOX::_colony[COLONY::_colony_handle];

                if (colony->buildings[40] == 0 && colony->buildings[41] == 0 && colony->buildings[8] == 0) {
                    int design_idx = -(mil_idx + 50);
                    // TODO: This does not seem to be correct check
                    if (MOX::_player[MOX::_PLAYER_NUM].ship_designs[design_idx].size < 2) {
                        if (_field_mode != 1) {
                            use_dimmed_font = true;
                        }
                    }
                }
            }

            if (!use_dimmed_font) {
                if (_field_mode == 1 && mil_idx == _scanned_prod && mil_idx > -100 && mil_idx <= -50) {
                    if (ERIC::Global_Cycler_(30, 2) != 0) {
                        use_dimmed_font = true;
                    }
                }
            }

            if (!use_dimmed_font && mil_idx > -100 && mil_idx <= -50) {
                if (!COLCALC::Colony_Can_Build_Product_(&MOX::_colony[COLONY::_colony_handle], mil_idx)) {
                    if (_field_mode != 1) {
                        use_dimmed_font = true;
                    }
                }
            }

            if (!use_dimmed_font) {
                if (mil_idx == -11 || mil_idx == -12 || mil_idx == -13 || mil_idx == -17 || mil_idx == -16) {
                    if (Auto_Design_Type_(mil_idx) == 0) {
                        if (mil_idx == -16) {
                            if (!COLCALC::Colony_Can_Build_Product_(&MOX::_colony[COLONY::_colony_handle], -55)) {
                                use_dimmed_font = true;
                            }
                        }
                    }
                }
            }

            s_colors* current_palette = use_dimmed_font ? &_l_font_dim_color_array : &_l_font_color_array;
            fonts::Set_Font_Style_Shadow_Down_(style_y_offset, current_palette);

            char* selection_name = Selection_Name_(mil_idx);
            int16_t draw_y = (19 - COLONY::_step_squish) * i + 20;

            BILL::Squeeze_Print_(485, draw_y, selection_name, 138);
        }
    }

    void __cdecl Draw_Buildings_List_() {
        COLDRAW::Calculate_Squish_Step_(13, 469, _n_building_indexes, 19);

        int16_t font_offset = Calc_Font_Off_();
        Get_Font_Off_For_Buildings_(&font_offset);

        fonts::Set_Font_Style_Shadow_Down_(4 - font_offset, &_l_font_color_array);

        if (_n_building_indexes > 0) {
            for (int16_t i = 0; i < _n_building_indexes; ++i) {
                int16_t building_id = _building_indexes[i];

                s_colors* colors;
                if (Has_Prod_In_Queue_(building_id)) {
                    colors = &_l_font_color_array;
                } else {
                    colors = &_l_font_dim_color_array;
                }

                fonts::Set_Font_Style_Shadow_Down_(4 - font_offset, colors);

                char* building_name = Selection_Name_(building_id);
                int16_t y_pos = (19 - COLONY::_step_squish) * i + 20;

                BILL::Squeeze_Print_(13, y_pos, building_name, 171);
            }
        }
    }

    /**
     * @address 0050e060
     */
    void __cdecl Draw_Build_Queue_() {
        char buffer[500];
        int16_t y_pos = 335;
        int16_t scanned_id = _scanned_prod;

        for (int16_t i = 0; i < 7; ++i) {
            int16_t active_idx = (_active_prod < 49) ? -1 : (_active_prod - 49);

            bool show_delete = false;

            if (i == active_idx) {
                if (_current_item[i] == scanned_id) {
                    show_delete = true;
                } else if (i < 6 && _current_item[i + 1] == scanned_id && _current_item[i + 1] == -10) {
                    show_delete = true;
                } else if (i != 0 && _current_item[i - 1] == scanned_id && _current_item[i] == -10) {
                    show_delete = true;
                }
            }

            if (!show_delete) {
                if (i != 0 && _current_item[i] == -10 && (_current_item[i] == scanned_id || _current_item[i - 1] == scanned_id)) {
                    if (i == active_idx || i == active_idx + 1) {
                        show_delete = true;
                    }
                }
                if (!show_delete && i < 6 && _current_item[i + 1] == -10 && (_current_item[i] == scanned_id || _current_item[i + 1] == scanned_id)) {
                    if (i == active_idx || i == active_idx - 1) {
                        show_delete = true;
                    }
                }
            }

            const char* text_to_print;
            if (show_delete && _scanned_field >= _queue_fields[0] && _scanned_field <= _queue_fields[6] && _current_item[i] != -1) {
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                char* item_name = Selection_Name_(_current_item[i]);
                _sprintf(buffer, ESTRINGS::E_Strings_(ESTR_DELETE_S), item_name);
                text_to_print = buffer;
            } else {
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                text_to_print = Selection_Name_(_current_item[i]);
            }

            BILL::Squeeze_Print_Centered_(332, y_pos, text_to_print, 251);
            y_pos += 20;
        }
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

    void __cdecl Draw_Current_Selection_() {
        int16_t y_line_count = 0;
        char buffer[480];
        int16_t item_id = (_scanned_prod != -1) ? _scanned_prod : _current_item[0];

        switch (item_id) {
            case -17: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                Print_App_Description_(209, 119, 248, 109 /* 0x6D */);
                COLONY::Remap_Draw_Ship_Seg_N_Centered_(244, 56, 46 /* 0x2E */);
                break;
            }
            case -16: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Ship_Design_Info_(-55 /* 0x37 */);
                break;
            }
            case -15: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                fonts::Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id));
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                Print_App_Description_(209, 119, 248, 69 /* 0x45 */);
                COLONY::Remap_Draw_Ship_Seg_N_Centered_(244, 56, 48 /* 0x30 */);
                break;
            }
            case -13: {
                y_line_count = 1;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Ship_Design_Info_(-54 /* 0x36 */);
                break;
            }
            case -12: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                Print_App_Description_(209, 119, 248, 41 /* 0x29 */);
                COLONY::Remap_Draw_Ship_Seg_N_Centered_(244, 56, 45 /* 0x2D */);
                break;
            }
            case -11: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                fonts::Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id));
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                COLONY::Remap_Draw_Ship_Seg_N_Centered_(244, 56, 47 /* 0x2F */);
                Print_App_Description_(209, 119, 248, 189 /* 0xBD */);
                break;
            }
            case -10: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                _sprintf(buffer, ESTRINGS::E_Strings_(0x54), ESTRINGS::E_Strings_(0xF3), ESTRINGS::E_Strings_(0x1EA));
                fonts::Print_(306, (y_line_count * 13 + 21), buffer);
                y_line_count++;
                Print_Selection_Centered_(Selection_Name_(item_id));
                break;
            }
            case -8: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                fonts::Print_(306, (y_line_count * 13 + 21), ESTRINGS::E_Strings_(0x1BD));
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                fonts::Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id));
                _sprintf(buffer, ESTRINGS::E_Strings_(0x54), ESTRINGS::E_Strings_(0xF3), ESTRINGS::E_Strings_(0x1EA));
                y_line_count++;
                fonts::Print_(306, (y_line_count * 13 + 21), buffer);
                y_line_count++;
                fonts::Set_Font_Style_Shadow_Down_(5, &_l_font_color_array);
                Print_Selection_Centered_(ESTRINGS::E_Strings_(0x7D));
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                break;
            }
            case -7: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                fonts::Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id));
                y_line_count += 2;
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                Print_App_Description_(209, 119, 248, 166 /* 0xA6 */);
                ERIC::Draw_Centered_(COLONY::Spy_Anim_(MOX::_player[MOX::_PLAYER_NUM].race));
                break;
            }
            case -6: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                ERIC::Draw_Centered_(COLONY::People_Anim_(0, 4, MOX::_player[MOX::_PLAYER_NUM].race));
                Print_App_Description_(209, 119, 248, 7);
                break;
            }
            case -5: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                ERIC::Draw_Centered_(COLONY::People_Anim_(0, 4, MOX::_player[MOX::_PLAYER_NUM].race));
                Print_App_Description_(209, 119, 248, 8);
                break;
            }
            case -4: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                ERIC::Draw_Centered_(COLONY::People_Anim_(0, 4, MOX::_player[MOX::_PLAYER_NUM].race));
                Print_App_Description_(209, 119, 248, 6);
                break;
            }
            case -3: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                Draw_Building_Centered_In_Window_(14 /* 0xE */);
                Print_Setting_Description_(209, 119, 248, 13 /* 0xD */);
                break;
            }
            case -2: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                animate::Draw_(235 /* 0xEB */, 54 /* 0x36 */, COLONY::Prod_Anims_(3, 1, 0));
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                Print_Setting_Description_(209, 119, 248, 12 /* 0xC */);
                break;
            }
            default: {
                if (item_id > 0 && item_id < 49 /* 0x31 */) {
                    Draw_Building_Info_(item_id);
                    return;
                }
                if (item_id <= -100) {
                    Draw_Ship_Info_(item_id);
                    return;
                }
                if (item_id <= -50) {
                    Draw_Ship_Design_Info_(item_id);
                    return;
                }
                RUSS::PFMT_(0, 0, ESTRINGS::E_Strings_(248 /* 0xF8 */));
                assert::hgetch_();
                break;
            }
        }
    }

    /**
     * @address 0050deb0
     */
    void __cdecl Draw_Build_Queue_Popup_() {
        char buffer[200];

        if (COLONY::_full_draw != 0) {
            graphics::Fill_(0, 0, 640, 479, 0);

            s_animation_header* anim = COLONY::C_Anims_(0);
            animate::Draw_(0, 0, anim);

            COLDRAW::Draw_Road_List_();
            COLONY::Draw_Colony_Bldgs_();

            replace::Interlaced_Gray_Scale_Off_Page_(246, 9, 244, 10, 0);

            animate::Draw_Palette_(COLBLDG::_anims[0]);
            palette::Set_Palette_Changes_(0, 255);
            remap::Update_Glass_Remap_Colors_();

            graphics::Darken_Fill_(202, 304, 463, 470);
            animate::Draw_(0, 0, COLBLDG::_anims[0]);

            char* planet_name = HACCESS::Get_Planet_Name_(MOX::_colony[COLONY::_colony_handle].planet);
            char* format = ESTRINGS::E_Strings_(ESTR_BUILD_LIST_FOR_S);
            _sprintf(buffer, format, planet_name);

            fonts::Set_Font_Style_Shadow_Down_(3, &COLBLDG::_l_font_color_array);
            BILL::Squeeze_Paragraph_Centered_(235, 309, 194, 17, buffer, 2);

            if (MOX::_previous_screen == SCREEN_COLONY_SUMMARY ||
                COLBLDG::_colony_bldg_saved_screen == SCREEN_COLONY_SUMMARY)
            {
                animate::Draw_Palette_(COLBLDG::_anims[0]);
                remap::Update_Glass_Remap_Colors_();
                animate::Draw_Palette_(COLBLDG::_colsum_pal_anim);
            } else {
                remap::Update_Glass_Remap_Colors_();
            }

            video::Copy_Off_To_Back_();
            COLONY::_full_draw = 0;
        }

        fonts::Set_Font_Style_Shadow_Down_(4, &COLBLDG::_l_font_color_array);
        fonts::Set_Font_Spacing_(1);

        COLBLDG::Draw_Buildings_List_();
        COLBLDG::Draw_Militarys_List_();
        COLBLDG::Draw_Build_Queue_();
        COLBLDG::Draw_Current_Selection_();
        COLBLDG::Draw_Active_Prod_();
    }
} // namespace COLBLDG