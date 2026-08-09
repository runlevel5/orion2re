#include "pch.h"

#include "build_queue.h"
#include "strbuild.h"

namespace COLBLDG {

    void Commit_Autobuild_State_() {
        if (BUILD_QUEUE::autobuild_settings.enabled == 0) {
            MOX::_colony[COLONY::_colony_handle].auto_building =
                static_cast<uint8_t>(_colony_auto_building);
        }
    }

    s_mouse_list _mouse_list_to[4] = {
        {18, 0, 199, 324, 465, 472},
        {16, 0, 476, 10, 632, 306},
        {16, 0, 10, 10, 188, 471},
        {1, 0, 0, 0, 639, 479},
    };
    s_mouse_list _mouse_list_what = {16, 0, 0, 0, 639, 479};
    s_mouse_list _mouse_list_which_ship = {17, 0, 0, 0, 639, 479};
    s_colors _l_font_color_array = {{184, 187, 186, 0, 0, 0, 0, 0}};
    s_colors _l_font_dim_color_array = {{183, 203, 202, 0, 0, 0, 0, 0}};
    uint8_t _return_to_build_popup_from_custom_design;
    uint8_t _hacky_speed_refitted_ship_flag;
    uint8_t _field_mode;
    int16_t _queue_fields[7] = {-1, -1, -1, -1, -1, 0, 0};
    int16_t _building_indexes[54] = {-1};
    int16_t _n_building_indexes;
    int16_t _military_indexes[54] = {-1};
    int16_t _n_military_indexes;
    int16_t _active_prod[4] = {-1, 0, 0, 0};
    int16_t _current_item[7] = {
        COLONY_PRODUCTION_NONE,
        COLONY_PRODUCTION_NONE,
        COLONY_PRODUCTION_NONE,
        COLONY_PRODUCTION_NONE,
        COLONY_PRODUCTION_NONE,
        0,
        0,
    };
    int16_t _last_app = -1;
    int16_t _last_setting = -1;
    int16_t _scanned_field = -1000;
    int16_t _military_fields[54];
    int16_t _colony_bldg_saved_screen;
    s_animation_header* _anims[7];
    int16_t _scanned_prod;
    int16_t _refitting_ship_n;
    int16_t _building_fields[54];
    uint8_t* _desc_record_ptr;
    int16_t _colony_auto_building;
    int16_t _fields[11];
    int16_t _build_queue_hotkeys[11];
    char _g_msg[800];
    s_animation_header* _colsum_pal_anim;
    s_animation_header* _bldg_bitm;
    uint8_t* _buffer;

    int16_t __cdecl Calc_Font_Off_() {
        if (COLONY::_step_squish == 0) {
            return 0;
        }

        // Returns 3 if _step_squish >= 5, otherwise 2.
        return (COLONY::_step_squish >= 5 ? 1 : 0) + 2;
    }

    void __cdecl Get_Font_Off_For_Military_(int16_t* font_offset) {
        if (_n_military_indexes <= 0) {
            return;
        }

        for (int16_t i = 0; i < _n_military_indexes; ++i) {
            int16_t text_width;
            do {
                fonts::Set_Font_Style_Shadow_Down_(4 - *font_offset, &_l_font_color_array);

                char* selection_name = Selection_Name_(_military_indexes[i]);

                text_width = BILL::_Get_Paragraph_Max_Width_(0x280, selection_name, 0);

                if (text_width > 138) {
                    *font_offset += 1;
                }

                if (*font_offset > 2) {
                    text_width = 0;
                }
            } while (text_width > 138);
        }
    }

    void __cdecl Get_Font_Off_For_Buildings_(int16_t* font_offset) {
        for (int16_t i = 0; i < _n_building_indexes; ++i) {
            int16_t width;
            do {
                int16_t building_idx = _building_indexes[i];
                s_colors* colors;

                if (Has_Prod_In_Queue_(building_idx)) {
                    colors = &_l_font_color_array;
                } else {
                    colors = &_l_font_dim_color_array;
                }

                fonts::Set_Font_Style_Shadow_Down_(4 - *font_offset, colors);

                char* name = Selection_Name_(building_idx);
                width = BILL::_Get_Paragraph_Max_Width_(0x280, name, 0);

                if (width > 171) {
                    *font_offset += 1;
                }

                if (*font_offset > 2) {
                    width = 0;
                }
            } while (width > 171);
        }
    }

    bool __cdecl Has_Prod_In_Queue_(int16_t prod_id) {
        for (int32_t i = 0; i < 7; ++i) {
            if (_current_item[i] == prod_id) {
                return true;
            }
        }
        return false;
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

    int16_t __cdecl Get_Prod_Queue_Len_() {
        for (int16_t i = 0; i < 7; ++i) {
            if (_current_item[i] == COLONY_PRODUCTION_NONE) {
                return i;
            }
        }
        return -1;
    }

    int __cdecl qsort_building_by_name_(const void* a, const void* b) {
        const int16_t* bld1 = (const int16_t*)a;
        const int16_t* bld2 = (const int16_t*)b;
        char* name1 = COLONY::Real_Building_Name_(*bld1);
        char* name2 = COLONY::Real_Building_Name_(*bld2);

        return strcmp(name1, name2);
    }

    void __cdecl Calculate_Building_Array_() {
        _building_indexes[0] = COLONY_PRODUCTION_TRADE_GOODS;
        _building_indexes[1] = COLONY_PRODUCTION_HOUSING;
        _n_building_indexes = 2;

        for (int16_t building_id = BUILDING_NO_BUILDING + 1; building_id < BUILDING_COUNT; ++building_id) {
            uint8_t can_build = COLCALC::Colony_Can_Build_Product_(&MOX::_colony[COLONY::_colony_handle], building_id);

            if ((((_n_building_indexes <= 54) && can_build == 1) || COLONY::_cheats) && building_id != BUILDING_COLONY_BASE) {
                int16_t current_index = _n_building_indexes;
                _n_building_indexes++;
                _building_indexes[current_index] = building_id;
            }
        }

        int32_t count_to_sort = (int32_t)_n_building_indexes - 2;
        qsort(&_building_indexes[2], count_to_sort, sizeof(int16_t), qsort_building_by_name_);
    }

    void __cdecl Calculate_Military_Array_() {
        _n_military_indexes = 0;

        s_colony& colony = MOX::_colony[COLONY::_colony_handle];
        s_planet_data& planet = MOX::_planet[colony.planet];

        int16_t uncolonized_worlds = Star_N_Uncolonized_Worlds_(planet.star_index);
        if (N_Prods_In_Queue_(BUILDING_COLONY_BASE) < uncolonized_worlds) {
            if (COLCALC::Colony_Can_Build_Product_(&colony, BUILDING_COLONY_BASE)) {
                _military_indexes[_n_military_indexes++] = BUILDING_COLONY_BASE;
            }
        }

        s_player& current_player = MOX::_player[MOX::_PLAYER_NUM];

        if (current_player.tech_applications[TECH_APP_FREIGHTERS] == TECH_RESEARCH_STATUS_RESEARCHED) {
            if (COLCALC::Colony_Can_Build_Product_(&colony, COLONY_PRODUCTION_FREIGHTERS)) {
                _military_indexes[_n_military_indexes++] = COLONY_PRODUCTION_FREIGHTERS;
            }
        }

        s_player& owner_player = MOX::_player[colony.owner];

        if (owner_player.ship_range > 0 && owner_player.ship_speed != 0) {
            if (owner_player.tech_applications[TECH_APP_COLONY_SHIP] == TECH_RESEARCH_STATUS_RESEARCHED) {
                _military_indexes[_n_military_indexes++] = COLONY_PRODUCTION_COLONY_SHIP;
            }

            if (owner_player.tech_applications[TECH_APP_OUTPOST_SHIP] == TECH_RESEARCH_STATUS_RESEARCHED) {
                _military_indexes[_n_military_indexes++] = COLONY_PRODUCTION_OUTPOST_SHIP;
            }

            if (owner_player.tech_applications[TECH_APP_TRANSPORT] == TECH_RESEARCH_STATUS_RESEARCHED &&
                colony.buildings[BUILDING_MARINE_BARRACKS] != 0) {
                _military_indexes[_n_military_indexes++] = COLONY_PRODUCTION_TRANSPORT_SHIP;
            }

            if (_n_military_indexes != 0) {
                _military_indexes[_n_military_indexes++] = COLONY_PRODUCTION_SEPARATOR;
            }

            for (int16_t i = 0; i < 5; ++i) {
                const char* default_design_text = ESTRINGS::E_Strings_(12 /* HESTR_00C_YOUR_CURRENT_DESIGN_IS */);
                const char* design_name = Selection_Name_(COLBLDG::Colony_Production_From_Ship_Design_Index_(i));

                if (strcmp(default_design_text, design_name) != 0) {
                    bool can_build = false;

                    if (MOX::_settings.strategic_combat_flag == 0) {
                        can_build = true;
                    } else if (current_player.ship_designs[i].size != SHIP_SIZE_TITAN) {
                        can_build = true;
                    } else if (owner_player.tech_applications[TECH_APP_TITAN_CONSTRUCTION] == TECH_RESEARCH_STATUS_RESEARCHED) {
                        can_build = true;
                    }

                    if (can_build) {
                        _military_indexes[_n_military_indexes++] = COLBLDG::Colony_Production_From_Ship_Design_Index_(i);
                    }
                }
            }

            if (owner_player.tech_applications[TECH_APP_DOOM_STAR_CONSTRUCTION] == TECH_RESEARCH_STATUS_RESEARCHED &&
                MOX::_settings.strategic_combat_flag != 0) {
                _military_indexes[_n_military_indexes++] = COLONY_PRODUCTION_DOOM_STAR;
            }
        }

        if (_n_military_indexes != 0) {
            _military_indexes[_n_military_indexes++] = COLONY_PRODUCTION_SEPARATOR;
        }

        if (COLCALC::Colony_Can_Build_Product_(&colony, COLONY_PRODUCTION_FARMER)) {
            _military_indexes[_n_military_indexes++] = COLONY_PRODUCTION_FARMER;
        }
        if (COLCALC::Colony_Can_Build_Product_(&colony, COLONY_PRODUCTION_WORKER)) {
            _military_indexes[_n_military_indexes++] = COLONY_PRODUCTION_WORKER;
        }
        if (COLCALC::Colony_Can_Build_Product_(&colony, COLONY_PRODUCTION_SCIENTIST)) {
            _military_indexes[_n_military_indexes++] = COLONY_PRODUCTION_SCIENTIST;
        }
        if (COLCALC::Colony_Can_Build_Product_(&colony, COLONY_PRODUCTION_SPY)) {
            _military_indexes[_n_military_indexes++] = COLONY_PRODUCTION_SPY;
        }
    }

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

    void __cdecl Load_Build_Anims_() {
        for (int16_t i = 0; i < 7; ++i) {
            if (i == 0) {
                _anims[i] = (s_animation_header*)farload::Far_Reload_(COLONY::_COLBLDG_LBX, 0, MOX::_screen_seg);
            } else {
                _anims[i] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLBLDG_LBX, i, MOX::_screen_seg);
            }
        }

        _bldg_bitm = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x1FA4);
        _colsum_pal_anim = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLSUM_LBX, 0x15, MOX::_screen_seg);
    }

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

        _fields[2] = fields::Add_Radio_Button_Field_(490, 342, ESTRINGS::E_Strings_(12), _anims[2], (int16_t*)&_colony_auto_building, ESTRINGS::E_Strings_(12), 41);

        if (MOX::_settings.strategic_combat_flag != 0) {
            _fields[4] = -1000;
            _fields[5] = -1000;
        } else {
            _fields[4] = fields::Add_Button_Field_(492, 379, ESTRINGS::E_Strings_(12), _anims[4], ESTRINGS::E_Strings_(436 /* ESTR_R */), 40);
            _fields[5] = fields::Add_Button_Field_(561, 379, ESTRINGS::E_Strings_(12), _anims[5], ESTRINGS::E_Strings_(251 /* ESTR_D3 */), 40);
        }

        _fields[6] = fields::Add_Button_Field_(503, 411, ESTRINGS::E_Strings_(12), _anims[6], (char*)"e", 40);
        _fields[7] = -1000;

        _build_queue_hotkeys[0] = fields::Add_Hot_Key_("q");
        for (int preset = 1; preset <= 9; ++preset) {
            const char hotkey[2] = {static_cast<char>('0' + preset), '\0'};
            _build_queue_hotkeys[preset] = fields::Add_Hot_Key_(hotkey);
        }
        _build_queue_hotkeys[10] = fields::Add_Hot_Key_("0");

        Add_Build_Queue_Fields_();
        Add_Buildings_Fields_();
        Add_Militarys_Fields_();
    }

    bool __cdecl Apply_Build_Queue_Hotkey_(int16_t field_id) {
        for (int preset = 0; preset < BUILD_QUEUE::PRESET_COUNT; ++preset) {
            if (field_id != _build_queue_hotkeys[preset]) {
                continue;
            }
            Do_Exit_Screen_Cleanup_();
            if (BUILD_QUEUE::Apply_Preset_To_Colony_(COLONY::_colony_handle, preset)) {
                memcpy(_current_item, MOX::_colony[COLONY::_colony_handle].producing,
                       sizeof(_current_item));
                _active_prod[0] = -1;
                _field_mode = 0;
                Build_Arrays_();
                Do_Draw_Build_Queue_();
            }
            return true;
        }
        return false;
    }

    void __cdecl Build_Arrays_() {
        Calculate_Building_Array_();
        Calculate_Military_Array_();
    }

    int16_t __cdecl Create_Ship_(int16_t player_idx, int16_t design_idx, int16_t colony_idx) {
        int16_t ship_idx = 0;
        int16_t planet_idx = MOX::_colony[colony_idx].planet;
        int16_t star_idx = MOX::_planet[planet_idx].star_index;

        while (ship_idx < MAX_SHIPS) {
            auto& ship = MOX::_ship[ship_idx];
            if (ship.owner == (int8_t)player_idx) {
                if (ship.status == 5 || ship_idx >= MOX::_NUM_SHIPS) {
                    break;
                }
            }
            ship_idx++;
        }

        if (ship_idx >= MAX_SHIPS) {
            return -1;
        }

        if (ship_idx >= MOX::_NUM_SHIPS) {
            MOX::_NUM_SHIPS = ship_idx + 1;
        }

        auto& new_ship = MOX::_ship[ship_idx];
        memset(&new_ship, 0, sizeof(s_ship_data));

        struct_::Copy_Structure_(&new_ship.d, &MOX::_player[player_idx].ship_designs[design_idx], sizeof(new_ship.d));
        new_ship.owner = (int8_t)player_idx;
        new_ship.d.previous_owner = (uint8_t)player_idx;
        new_ship.status = 0;
        new_ship.location = star_idx;
        new_ship.x = MOX::_star[star_idx].x;
        new_ship.y = MOX::_star[star_idx].y;
        new_ship.officer_index = -1;

        if (MOX::_settings.strategic_combat_flag != 0) {
            char name_buffer[500];
            int32_t ships_built_count = MOX::_player[player_idx].ships_built[(int32_t)new_ship.d.size];

            snprintf(name_buffer, sizeof(name_buffer), ESTRINGS::E_Strings_(0x2a), new_ship.d.name, ships_built_count);

            if (strlen(name_buffer) >= 15) {
                snprintf(name_buffer, sizeof(name_buffer), ESTRINGS::E_Strings_(0x48), new_ship.d.name, ships_built_count);
                if (strlen(name_buffer) >= 15) {
                    strlcpy(name_buffer, new_ship.d.name, sizeof(name_buffer));
                }
            }

            strlcpy(new_ship.d.name, name_buffer, sizeof(new_ship.d.name));
            MOX::_player[player_idx].ships_built[(int32_t)new_ship.d.size]++;
        }

        return ship_idx;
    }

    void __cdecl Build_Queue_Popup_() {
        uint32_t original_seed = game_random::Get_Random_Seed_();
        game_random::Set_Random_Seed_(COLONY::_colony_handle);

        COLONY::_star_handle = MOX::_screen_data;
        COLONY::_orbit_n = COLONY::_orbit_temp;

        int16_t planet_idx = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);
        COLONY::_planet_handle = planet_idx;

        int16_t colony_idx = MOX::_planet[planet_idx].colony_index;
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
            COLBLDG::_active_prod[0] = -1;
        }

        COLBLDG::Build_Arrays_();

        if (COLBLDG::_return_to_build_popup_from_custom_design == 0) {
            COLBLDG::_colony_auto_building =
                MOX::_colony[colony_idx].auto_building != 0;
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
        ERIC::Fast_Fade_In_();
        video::Set_Refresh_Stencil_();

        int16_t field_id;
        int16_t scan_code;
        do {
            COLBLDG::Draw_Build_Queue_Popup_();
            RUSS::Mox_Sync_Update_();
            field_id = fields::Get_Input_();
            scan_code = fields::Scan_Input_();
            COLBLDG::Evaluate_Input_(&field_id, &scan_code);

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
        if (COLBLDG::_active_prod[0] != -1) {
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

        game_random::Set_Random_Seed_(original_seed);
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

                if (is_available && planet.planet_type == PLANET_TYPE_PLANET) {
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
        _buffer = (uint8_t*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0xAF6);
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
            s_colors* current_palette = &_l_font_color_array;

            if (COLBLDG::Colony_Production_Is_Ship_Design_(mil_idx)) {
                s_colony* colony = &MOX::_colony[COLONY::_colony_handle];

                if (colony->buildings[BUILDING_STAR_BASE] == 0 &&
                    colony->buildings[BUILDING_STAR_FORTRESS] == 0 &&
                    colony->buildings[BUILDING_BATTLESTATION] == 0 &&
                    MOX::_player[MOX::_PLAYER_NUM].ship_designs[COLBLDG::Colony_Production_Ship_Design_Index_(mil_idx)].size >= SHIP_SIZE_LARGE &&
                    _field_mode != 1) {
                    current_palette = &_l_font_dim_color_array;
                } else if (_field_mode == 1 && mil_idx == _scanned_prod) {
                    if (ERIC::Global_Cycler_(30, 2) != 0) {
                        current_palette = &_l_font_dim_color_array;
                    }
                } else if (!COLCALC::Colony_Can_Build_Product_(&MOX::_colony[COLONY::_colony_handle], mil_idx)) {
                    if (_field_mode != 1) {
                        current_palette = &_l_font_dim_color_array;
                    }
                }
            } else if (mil_idx == COLONY_PRODUCTION_TRANSPORT_SHIP ||
                       mil_idx == COLONY_PRODUCTION_COLONY_SHIP ||
                       mil_idx == COLONY_PRODUCTION_CUSTOM_DESIGN ||
                       mil_idx == COLONY_PRODUCTION_OUTPOST_SHIP ||
                       mil_idx == COLONY_PRODUCTION_DOOM_STAR) {
                if (Auto_Design_Type_(mil_idx) == 0) {
                    if (mil_idx != COLONY_PRODUCTION_DOOM_STAR ||
                        !COLCALC::Colony_Can_Build_Product_(&MOX::_colony[COLONY::_colony_handle], COLBLDG::Colony_Production_From_Ship_Design_Index_(SHIP_SIZE_DOOM_STAR))) {
                        current_palette = &_l_font_dim_color_array;
                    }
                }
            }

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

    int16_t __cdecl Auto_Design_Type_(int16_t design_code) {
        if (design_code == COLONY_PRODUCTION_COLONY_SHIP) {
            return SHIP_TYPE_COLONY;
        }
        if (design_code == COLONY_PRODUCTION_OUTPOST_SHIP) {
            return SHIP_TYPE_OUTPOST;
        }
        if (design_code == COLONY_PRODUCTION_TRANSPORT_SHIP) {
            return SHIP_TYPE_TRANSPORT;
        }

        return SHIP_TYPE_COMBAT;
    }

    void __cdecl Draw_Build_Queue_() {
        char buffer[500];
        int16_t y_pos = 335;
        int16_t scanned_id = _scanned_prod;

        for (int16_t i = 0; i < 7; ++i) {
            int16_t active_idx = (_active_prod[0] < BUILDING_COUNT) ? -1 : (_active_prod[0] - BUILDING_COUNT);

            bool show_delete = false;

            if (i == active_idx) {
                if (_current_item[i] == scanned_id) {
                    show_delete = true;
                } else if (i < 6 && _current_item[i + 1] == scanned_id && _current_item[i + 1] == COLONY_PRODUCTION_REPEAT) {
                    show_delete = true;
                } else if (i != 0 && _current_item[i - 1] == scanned_id && _current_item[i] == COLONY_PRODUCTION_REPEAT) {
                    show_delete = true;
                }
            }

            if (!show_delete) {
                if (i != 0 && _current_item[i] == COLONY_PRODUCTION_REPEAT && (_current_item[i] == scanned_id || _current_item[i - 1] == scanned_id)) {
                    if (i == active_idx || i == active_idx + 1) {
                        show_delete = true;
                    }
                }
                if (!show_delete && i < 6 && _current_item[i + 1] == COLONY_PRODUCTION_REPEAT && (_current_item[i] == scanned_id || _current_item[i + 1] == scanned_id)) {
                    if (i == active_idx || i == active_idx - 1) {
                        show_delete = true;
                    }
                }
            }

            const char* text_to_print;
            if (show_delete && _scanned_field >= _queue_fields[0] && _scanned_field <= _queue_fields[6] && _current_item[i] != COLONY_PRODUCTION_NONE) {
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                char* item_name = Selection_Name_(_current_item[i]);
                snprintf(buffer, sizeof(buffer), ESTRINGS::E_Strings_(ESTR_DELETE_S), item_name);
                text_to_print = buffer;
            } else {
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                text_to_print = Selection_Name_(_current_item[i]);
            }

            BILL::Squeeze_Print_Centered_(332, y_pos, text_to_print, 251);
            y_pos += 20;
        }
    }

    void __cdecl Get_App_Description_String_(char* dest_str, uint32_t dest_str_size, int16_t app_id) {
        char lbx_name[20];
        s_help_record* record_ptr;

        TEXTBOX::Get_Help_Lbx_Name_(lbx_name, sizeof(lbx_name));

        if (_last_app != app_id) {
            _last_app = app_id;
            record_ptr = static_cast<s_help_record*>(farload::Far_Reload_Data_(lbx_name, 0, _buffer, (int)app_id, 1, sizeof(s_help_record)));
        } else {
            record_ptr = reinterpret_cast<s_help_record*>(_desc_record_ptr);
        }

        _desc_record_ptr = (uint8_t*)record_ptr;

        strlcpy(dest_str, record_ptr->body, dest_str_size);
    }

    void __cdecl Get_Setting_Description_String_(char* buffer, int16_t setting_id) {
        if (setting_id != _last_setting) {
            _last_setting = setting_id;
            _last_app = -1;

            char* file_path = ESTRINGS::E_Strings_(HESTR_170_MAX_POPULATION);
            char* loaded_data = (char*)farload::Far_Reload_Data_(
                file_path,
                (int32_t)setting_id,
                _buffer,
                0,
                1,
                0x258 /* 600 */
            );

            if (loaded_data != nullptr) {
                strlcpy(_g_msg, loaded_data, sizeof(_g_msg));
            }
        }
    }

    char* __cdecl Selection_Name_(int16_t id) {
        if (COLBLDG::Colony_Production_Is_Building_(id)) {
            return COLONY::Real_Building_Name_(id);
        }

        if (!COLBLDG::Colony_Production_Is_Queued_Ship_(id)) {
            return Option_String_(id);
        }

        int16_t index = COLBLDG::Colony_Production_Ship_Index_(id);
        s_ship_data* ship = &MOX::_ship[index];

        if (ship->status != 4) {
            return ship->d.name;
        }

        _last_setting = -1;
        _last_app = -1;

        char* opt_str = Option_String_(COLONY_PRODUCTION_REFIT);
        char* format = ESTRINGS::E_Strings_(45);
        snprintf(_g_msg, sizeof(_g_msg), format, opt_str, ship->d.name);

        return _g_msg;
    }

    void __cdecl Draw_Current_Selection_() {
        int16_t y_line_count = 0;
        char buffer[500];
        int16_t item_id = (_scanned_prod != -1) ? _scanned_prod : _current_item[0];

        switch (item_id) {
            case COLONY_PRODUCTION_OUTPOST_SHIP: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                Print_App_Description_(209, 119, 248, 109 /* 0x6D */);
                COLONY::Remap_Draw_Ship_Seg_N_Centered_(244, 56, 46 /* 0x2E */);
                break;
            }
            case COLONY_PRODUCTION_DOOM_STAR: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Ship_Design_Info_(COLBLDG::Colony_Production_From_Ship_Design_Index_(SHIP_SIZE_DOOM_STAR));
                break;
            }
            case COLONY_PRODUCTION_FREIGHTERS: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                fonts::Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id));
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                Print_App_Description_(209, 119, 248, 69 /* 0x45 */);
                COLONY::Remap_Draw_Ship_Seg_N_Centered_(244, 56, 48 /* 0x30 */);
                break;
            }
            case COLONY_PRODUCTION_CUSTOM_DESIGN: {
                y_line_count = 1;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Ship_Design_Info_(-54 /* 0x36 */);
                break;
            }
            case COLONY_PRODUCTION_COLONY_SHIP: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                Print_App_Description_(209, 119, 248, 41 /* 0x29 */);
                COLONY::Remap_Draw_Ship_Seg_N_Centered_(244, 56, 45 /* 0x2D */);
                break;
            }
            case COLONY_PRODUCTION_TRANSPORT_SHIP: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                fonts::Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id));
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                COLONY::Remap_Draw_Ship_Seg_N_Centered_(244, 56, 47 /* 0x2F */);
                Print_App_Description_(209, 119, 248, 189 /* 0xBD */);
                break;
            }
            case COLONY_PRODUCTION_REPEAT: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                snprintf(buffer, sizeof(buffer), ESTRINGS::E_Strings_(0x54), ESTRINGS::E_Strings_(0xF3), ESTRINGS::E_Strings_(0x1EA));
                fonts::Print_(306, (y_line_count * 13 + 21), buffer);
                y_line_count++;
                Print_Selection_Centered_(Selection_Name_(item_id));
                break;
            }
            case COLONY_PRODUCTION_REFIT: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                fonts::Print_(306, (y_line_count * 13 + 21), ESTRINGS::E_Strings_(0x1BD));
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                fonts::Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id));
                snprintf(buffer, sizeof(buffer), ESTRINGS::E_Strings_(0x54), ESTRINGS::E_Strings_(0xF3), ESTRINGS::E_Strings_(0x1EA));
                y_line_count++;
                fonts::Print_(306, (y_line_count * 13 + 21), buffer);
                y_line_count++;
                fonts::Set_Font_Style_Shadow_Down_(5, &_l_font_color_array);
                Print_Selection_Centered_(ESTRINGS::E_Strings_(0x7D));
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                break;
            }
            case COLONY_PRODUCTION_SPY: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                fonts::Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id));
                y_line_count += 2;
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                Print_App_Description_(209, 119, 248, 166 /* 0xA6 */);
                Draw_Icon_(COLONY::Spy_Anim_(MOX::_player[MOX::_PLAYER_NUM].race));
                break;
            }
            case COLONY_PRODUCTION_SCIENTIST: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                Draw_Icon_(COLONY::People_Anim_(0, 4, MOX::_player[MOX::_PLAYER_NUM].race));
                Print_App_Description_(209, 119, 248, 7);
                break;
            }
            case COLONY_PRODUCTION_WORKER: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                Draw_Icon_(COLONY::People_Anim_(0, 4, MOX::_player[MOX::_PLAYER_NUM].race));
                Print_App_Description_(209, 119, 248, 8);
                break;
            }
            case COLONY_PRODUCTION_FARMER: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                Draw_Icon_(COLONY::People_Anim_(0, 4, MOX::_player[MOX::_PLAYER_NUM].race));
                Print_App_Description_(209, 119, 248, 6);
                break;
            }
            case COLONY_PRODUCTION_HOUSING: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                Draw_Building_Centered_In_Window_(14 /* 0xE */);
                Print_Setting_Description_(209, 119, 248, 13 /* 0xD */);
                break;
            }
            case COLONY_PRODUCTION_TRADE_GOODS: {
                fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);
                BILL::Squeeze_Print_(306, (y_line_count * 13 + 21), Selection_Name_(item_id), 156);
                y_line_count += 2;
                animate::Draw_(235 /* 0xEB */, 54 /* 0x36 */, COLONY::Prod_Anims_(3, 1, 0));
                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item_id, &y_line_count);
                Print_Setting_Description_(209, 119, 248, 12 /* 0xC */);
                break;
            }
            case COLONY_PRODUCTION_NONE:
            case 0:
                return;
            default: {
                if (COLBLDG::Colony_Production_Is_Building_(item_id)) {
                    Draw_Building_Info_(item_id);
                    return;
                }
                if (COLBLDG::Colony_Production_Is_Queued_Ship_(item_id)) {
                    Draw_Ship_Info_(item_id);
                    return;
                }
                if (COLBLDG::Colony_Production_Is_Ship_Design_(item_id)) {
                    Draw_Ship_Design_Info_(item_id);
                    return;
                }
                RUSS::PFMT_(0, 0, ESTRINGS::E_Strings_(248 /* 0xF8 */), item_id);
                key::Wait_For_Key_();
                break;
            }
        }
    }

    void __cdecl Print_Selection_Centered_(const char* text) {
        fonts::Print_Centered_(244, 56, text);
    }

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
            snprintf(buffer, sizeof(buffer), format, planet_name);

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

    void __cdecl Print_Setting_Description_(int16_t x, int16_t y, int16_t width, int16_t setting_id) {
        fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
        fonts::Set_Font_Spacing_(1);
        fonts::Set_Font_Spacing_Width_(2);

        Get_Setting_Description_String_(_g_msg, setting_id);

        BILL::_Print_Paragraph_(x, y, width, _g_msg, 3);

        fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
    }

    void __cdecl Print_App_Description_(uint16_t x, int16_t y, uint16_t width, int16_t app_id) {
        char help_lbx_name[20];
        char description_buffer[1332];

        TEXTBOX::Get_Help_Lbx_Name_(help_lbx_name, sizeof(help_lbx_name));
        fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
        fonts::Set_Font_Spacing_(1);
        fonts::Set_Font_Spacing_Width_(2);

        if (app_id != _last_app) {
            _last_app = app_id;
            _desc_record_ptr = (uint8_t*)farload::Far_Reload_Data_(help_lbx_name, 0, _buffer, app_id, 1, sizeof(s_help_record));
        }

        const char* record_text = reinterpret_cast<const s_help_record*>(_desc_record_ptr)->body;
        strlcpy(description_buffer, record_text, sizeof(description_buffer));

        BILL::_Print_Paragraph_(x, y, width, description_buffer, 3);

        fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
    }

    void __cdecl Draw_Ship_Design_Info_(int16_t id) {
        int16_t index = COLBLDG::Colony_Production_Ship_Design_Index_(id);

        if (COLBLDG::Colony_Production_Is_Ship_Design_(id)) {
            int16_t line_count = 0;
            fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);

            if (id != 4) {
                s_player* player = &MOX::_player[MOX::_PLAYER_NUM];
                s_ship_design* design = &player->ship_designs[index];

                int16_t y_pos = line_count * 13 + 21;
                line_count++;

                fonts::Print_(306, y_pos, design->name);

                line_count++;

                fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
                fonts::Set_Font_Spacing_(1);
                fonts::Set_Font_Spacing_Width_(2);

                Draw_Cost_And_Time_Info_(COLONY::_colony_handle, id, &line_count);
                Draw_Ship_Design_Info2_(design);
            }
        }

        COLONY::Remap_Draw_Ship_D_Centered_(244, 56, index);
    }

    void __cdecl Draw_Building_Centered_In_Window_(int16_t building_id) {
        graphics::Set_Window_(203, 9, 285, 103);
        video::Clip_On_();
        COLONY::Draw_Building_With_Bottom_Centered_(244, 93, building_id, 5, 5);
        video::Clip_Off_();
    }

    void __cdecl Draw_Cost_And_Time_Info_(int16_t colony_idx, int16_t product_idx, int16_t* line_count) {
        char buffer[500];
        int32_t y_pos;
        char* format_str;

        // 1. Draw Production Cost
        int16_t cost = COLCALC::Colony_Product_Cost_(&MOX::_colony[colony_idx], product_idx);
        format_str = ESTRINGS::E_Strings_(ESTR_COST_HERE_D);
        snprintf(buffer, sizeof(buffer), format_str, (int32_t)cost);

        y_pos = (*line_count) * 13 + 21;
        fonts::Print_(306, (int16_t)y_pos, buffer);
        (*line_count)++;

        // 2. Draw Maintenance if applicable
        if (COLBLDG::Colony_Production_Is_Building_(product_idx)) {
            int32_t maint = (int32_t)TECHDATA::_buildings[product_idx].maintenance;
            format_str = ESTRINGS::E_Strings_(HESTR_16F_PENALTY);
            snprintf(buffer, sizeof(buffer), format_str, maint);

            y_pos = (*line_count) * 13 + 21;
            fonts::Print_(306, (int16_t)y_pos, buffer);
            (*line_count)++;
        }

        // 3. Draw Build Time
        int16_t build_time = COLONY::Calculate_Colony_Turn_Count_From_Scrap_For_Prod_(colony_idx, product_idx);
        format_str = ESTRINGS::E_Strings_(ESTR_BUILD_TIME_D);
        snprintf(buffer, sizeof(buffer), format_str, (int32_t)build_time);

        y_pos = (*line_count) * 13 + 21;
        BILL::Squeeze_Print_(306, (int16_t)y_pos, buffer, 156);
        (*line_count)++;

        // 4. Draw Turns Left if currently producing
        if (MOX::_colony[colony_idx].producing[0] == product_idx) {
            int16_t turns_left = COLONY::Calculate_Current_Production_Turn_Count_(colony_idx);
            format_str = ESTRINGS::E_Strings_(ESTR_TURNS_LEFT_D);
            snprintf(buffer, sizeof(buffer), format_str, (int32_t)turns_left);

            y_pos = (*line_count) * 13 + 21;
            BILL::Squeeze_Print_(306, (int16_t)y_pos, buffer, 156);
            (*line_count)++;
            return;
        }

        // 5. Special case for Gravity Generator (ID 25)
        if (COLBLDG::Colony_Production_Is_Building_(product_idx) && product_idx == BUILDING_GRAVITY_GENERATOR) {
            int16_t planet_idx = MOX::_colony[colony_idx].planet;
            int8_t gravity_class = MOX::_planet[planet_idx].gravity_class;
            char* gravity_str = MOX::_planet_gravity_string[gravity_class];

            format_str = ESTRINGS::E_Strings_(ESTR_GRAVITY_HERE_S);
            snprintf(buffer, sizeof(buffer), format_str, gravity_str);

            y_pos = (*line_count) * 13 + 21;
            BILL::Squeeze_Print_(306, (int16_t)y_pos, buffer, 156);
            (*line_count)++;
        }
    }

    void __cdecl Draw_Building_Info_(int16_t building_idx) {
        // BUG: building_idx might be -1 when accessing _buildings
        if (TECHDATA::_buildings[building_idx].type == 7) {
            s_animation_header* anim = COLONY::Satellite_Anim_(building_idx);
            Draw_Icon_(anim);
        } else {
            Draw_Building_Centered_In_Window_(building_idx);
        }

        if (building_idx != -1) {
            int16_t y_offset = 0;
            fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);

            int y_pos = (y_offset * 13) + 21;
            y_offset++;

            char* building_name = COLONY::Real_Building_Name_(building_idx);
            BILL::Squeeze_Print_(0x132, y_pos, building_name, 0x9c);

            y_offset++;
            fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);

            Draw_Cost_And_Time_Info_(COLONY::_colony_handle, building_idx, &y_offset);

            y_offset = 0;
            Print_App_Description_(0xd1, 0x77, 0xf8, *(int16_t*)&TECHDATA::_buildings[building_idx].tech_app);
        }
    }

    void __cdecl Draw_Ship_Design_Info2_(s_ship_design* design) {
        char buffer[20000];
        STRBUILD::s_builder builder = STRBUILD::Init(buffer, sizeof(buffer));

        fonts::Set_Font_Style_Shadow_Down_(1, &_l_font_color_array);
        fonts::Set_Font_Spacing_(1);
        fonts::Set_Font_Spacing_Width_(2);

        Build_Ship_Design_Info_Formatted_Paragraph_(design, &builder);
        BILL::Squeeze_Print_Formatted_Paragraph_(212, 114, 248, 186, buffer, 0);
    }

    static void Append_Ship_Design_Info_Formatted_Paragraph_Details_(s_ship_design* design, STRBUILD::s_builder* builder) {
        char prefix_str[16];
        char wide_item_strings[8][200];

        strlcpy(prefix_str, strings::s_t_T0_5_124__0055ac38, sizeof(prefix_str));
        STRBUILD::Sprintf(builder, ESTRINGS::E_Strings_(70), prefix_str);

        for (int i = 0; i < 8; i++) {
            wide_item_strings[i][0] = '\0';
        }

        STRBUILD::Sprintf(builder, ESTRINGS::E_Strings_(99), strings::s_0_0055110c, strings::s__0055ac34, design->cost);
        STRBUILD::Sprintf(builder, ESTRINGS::E_Strings_(119), strings::s_0_0055110c, strings::s__0055ac34, TECHDATA::_shields[design->shield_type].name);
        STRBUILD::Sprintf(builder, ESTRINGS::E_Strings_(98), strings::s_0_0055110c, strings::s__0055ac34, TECHDATA::_computers[design->computer_type].name);
        STRBUILD::Sprintf(builder, ESTRINGS::E_Strings_(91), strings::s_0_0055110c, strings::s__0055ac34, TECHDATA::_armor[design->armor_type].name);
        STRBUILD::Sprintf(builder, ESTRINGS::E_Strings_(104), strings::s_0_0055110c, strings::s__0055ac34, TECHDATA::_drives[design->ftl_type].name);
        STRBUILD::Sprintf(builder, ESTRINGS::E_Strings_(130), strings::s_0_0055110c);

        int weapons_added = 0;
        int num_wide_weapons = 0;
        int wide_weapon_counts[8];

        for (int i = 0; i < 8; i++) {
            if (design->ship_weapon[i].type != 0) {
                char temp_weapon_str[500];
                Build_Weapon_String_(&design->ship_weapon[i], temp_weapon_str, sizeof(temp_weapon_str), '\0', design->ship_weapon[i].count);

                if (fonts::Get_String_Width_(temp_weapon_str) <= 110) {
                    int str_id = ((weapons_added % 2) == 0) ? 87 : 88;
                    STRBUILD::Sprintf(builder, ESTRINGS::E_Strings_(str_id), strings::s_0_0055110c, (int)design->ship_weapon[i].count, temp_weapon_str);
                    weapons_added++;
                } else {
                    strlcpy(wide_item_strings[num_wide_weapons], temp_weapon_str, sizeof(wide_item_strings[num_wide_weapons]));
                    wide_weapon_counts[num_wide_weapons] = design->ship_weapon[i].count;
                    num_wide_weapons++;
                }
            }
        }

        if (num_wide_weapons > 0) {
            if ((weapons_added % 2) != 0) {
                STRBUILD::Sprintf(builder, ESTRINGS::E_Strings_(127));
            }
            for (int i = 0; i < num_wide_weapons; i++) {
                STRBUILD::Sprintf(builder, ESTRINGS::E_Strings_(88), strings::s_0_0055110c, wide_weapon_counts[i], wide_item_strings[i]);
            }
        }

        if ((weapons_added % 2) == 1) {
            STRBUILD::Sprintf(builder, ESTRINGS::E_Strings_(127));
        }

        for (int i = 0; i < 8; i++) {
            wide_item_strings[i][0] = '\0';
        }

        int specials_added = 0;
        int num_wide_specials = 0;
        bool has_specials_prefix = false;

        for (int i = 0; i < 39; i++) {
            if (struct_::Test_Bit_Field_(design->special_device_flags, i)) {
                if (!has_specials_prefix) {
                    has_specials_prefix = true;
                    STRBUILD::Sprintf(builder, ESTRINGS::E_Strings_(129), strings::s_0_0055110c);
                }

                if (fonts::Get_String_Width_(TECHDATA::_specials[i].name) <= 115) {
                    int str_id = ((specials_added % 2) == 0) ? 89 : 90;
                    STRBUILD::Sprintf(builder, ESTRINGS::E_Strings_(str_id), strings::s_0_0055110c, TECHDATA::_specials[i].name);
                    specials_added++;
                } else {
                    strlcpy(wide_item_strings[num_wide_specials], TECHDATA::_specials[i].name, sizeof(wide_item_strings[num_wide_specials]));
                    num_wide_specials++;
                }
            }
        }

        if (num_wide_specials > 0) {
            if ((specials_added % 2) != 0) {
                STRBUILD::Sprintf(builder, ESTRINGS::E_Strings_(127));
            }
            for (int i = 0; i < num_wide_specials; i++) {
                STRBUILD::Sprintf(builder, ESTRINGS::E_Strings_(90), strings::s_0_0055110c, wide_item_strings[i]);
            }
        }
    }

    void __cdecl Build_Ship_Design_Info_Formatted_Paragraph_(s_ship_design* design, STRBUILD::s_builder* builder) {
        Append_Ship_Design_Info_Formatted_Paragraph_Details_(design, builder);
    }

    void __cdecl Build_Ship_Info_Formatted_Paragraph_(s_ship_data* ship, STRBUILD::s_builder* builder) {
        if (ship->officer_index < 0) {
            STRBUILD::Sprintf(
                builder,
                ESTRINGS::E_Strings_(79),
                strings::s_0_0055110c,
                ship->d.name,
                FLT2::Crew_Description_String_((int16_t)(ship - MOX::_ship)),
                strings::s_1_00551110
            );
        } else {
            char leader_name[500];
            char crew_desc[500];
            strlcpy(leader_name, OFFICER::Leader_Name_(ship->officer_index, '\0'), sizeof(leader_name));
            strlcpy(crew_desc, FLT2::Crew_Description_String_((int16_t)(ship - MOX::_ship)), sizeof(crew_desc));

            STRBUILD::Sprintf(
                builder,
                ESTRINGS::E_Strings_(78),
                strings::s_0_0055110c,
                ship->d.name,
                leader_name,
                crew_desc
            );
        }

        Append_Ship_Design_Info_Formatted_Paragraph_Details_(&ship->d, builder);
    }

    void __cdecl Build_Ship_Info_Formatted_Paragraph_Binary_(void* ship_or_design, STRBUILD::s_builder* builder, char include_crew_details) {
        // The original binary routes both plain design pointers and ship-backed design subobjects through 0050A5C0.
        // Split the source API into design-only and ship-only entry points, and keep this wrapper only for binary call compatibility.
        if (include_crew_details != '\0') {
            Build_Ship_Info_Formatted_Paragraph_((s_ship_data*)ship_or_design, builder);
        } else {
            Build_Ship_Design_Info_Formatted_Paragraph_((s_ship_design*)ship_or_design, builder);
        }
    }

    int16_t __cdecl Colbldg_Create_Ship_(int16_t design_slot) {
        int16_t ship_idx = Create_Ship_(
            MOX::_colony[COLONY::_colony_handle].owner,
            design_slot,
            COLONY::_colony_handle
        );

        if (ship_idx != -1) {
            MOX::_ship[ship_idx].status = 6;
        }

        return ship_idx;
    }

    int16_t __cdecl First_Prod_Queue_Slot_(int16_t prod_idx) {
        for (int16_t i = 0; i < 7; ++i) {
            if (COLBLDG::Colony_Production_Is_Building_(prod_idx)) {
                if (_current_item[i] == prod_idx) {
                    return i;
                }
            } else {
                if (COLBLDG::Colony_Production_Is_Ship_Design_(prod_idx)) {
                    return -1;
                }

                int16_t curr = _current_item[i];
                if (COLBLDG::Colony_Production_Is_Queued_Ship_(curr)) {
                    if (prod_idx == curr) {
                        return i;
                    }
                } else if (prod_idx >= BUILDING_COUNT) {
                    if (prod_idx - i == BUILDING_COUNT) {
                        return i;
                    }
                } else if (prod_idx == COLONY_PRODUCTION_SCIENTIST ||
                           prod_idx == COLONY_PRODUCTION_FARMER ||
                           prod_idx == COLONY_PRODUCTION_WORKER ||
                           prod_idx == COLONY_PRODUCTION_SPY ||
                           prod_idx == COLONY_PRODUCTION_HOUSING ||
                           prod_idx == COLONY_PRODUCTION_TRADE_GOODS ||
                           prod_idx == COLONY_PRODUCTION_REPEAT ||
                           prod_idx == COLONY_PRODUCTION_FREIGHTERS) {
                    if (prod_idx == curr) {
                        return i;
                    }
                }
            }
        }
        return -1;
    }

    int16_t __cdecl First_Prod_Colony_Queue_Slot_(int16_t colony_idx, int16_t item_id) {
        for (int16_t i = 0; i < 7; ++i) {
            if (COLBLDG::Colony_Production_Is_Building_(item_id)) {
                if (MOX::_colony[colony_idx].producing[i] == item_id) {
                    return i;
                }
            } else {
                if (COLBLDG::Colony_Production_Is_Ship_Design_(item_id)) {
                    return -1;
                }

                int16_t produced_item = MOX::_colony[colony_idx].producing[i];

                if (COLBLDG::Colony_Production_Is_Queued_Ship_(produced_item)) {
                    if (item_id == produced_item) {
                        return i;
                    }
                } else if (item_id >= BUILDING_COUNT) {
                    if (item_id - i == BUILDING_COUNT) {
                        return i;
                    }
                } else {
                    if (item_id == COLONY_PRODUCTION_SCIENTIST ||
                        item_id == COLONY_PRODUCTION_FARMER ||
                        item_id == COLONY_PRODUCTION_WORKER ||
                        item_id == COLONY_PRODUCTION_SPY ||
                        item_id == COLONY_PRODUCTION_HOUSING ||
                        item_id == COLONY_PRODUCTION_TRADE_GOODS ||
                        item_id == COLONY_PRODUCTION_REPEAT ||
                        item_id == COLONY_PRODUCTION_FREIGHTERS) {
                        if (item_id == produced_item) {
                            return i;
                        }
                    }
                }
            }
        }

        return -1;
    }

    void __cdecl Evaluate_Input_(int16_t* selected_field_id, int16_t* scanned_field_id) {
        _scanned_prod = -1;
        _scanned_field = *scanned_field_id;

        if (_return_to_build_popup_from_custom_design != 0 || _hacky_speed_refitted_ship_flag != 0) {
            _return_to_build_popup_from_custom_design = 0;
            _hacky_speed_refitted_ship_flag = 0;
            _field_mode = 0;
            COLONY::Reset_Mouse_Picture_();

            if (MOX::_temp_star_handle == 0) {
                MOX::_refit_ship = 0;
            } else if (MOX::_refit_ship != 0) {
                Add_Refited_Ship_To_Queue_();
                MOX::_refit_ship = 0;
            }
        }

        int16_t input_field = *selected_field_id;

        if (Apply_Build_Queue_Hotkey_(input_field)) {
            *selected_field_id = 0;
            return;
        }

        if (_fields[2] == input_field && BUILD_QUEUE::autobuild_settings.enabled != 0) {
            if (MOX::_colony[COLONY::_colony_handle].auto_building !=
                BUILD_QUEUE::AUTOBUILD_DISABLED) {
                MOX::_colony[COLONY::_colony_handle].auto_building =
                    BUILD_QUEUE::AUTOBUILD_DISABLED;
            } else {
                if (COLONY::Prompt_For_Autobuild_Queue_(COLONY::_colony_handle)) {
                    memcpy(_current_item,
                           MOX::_colony[COLONY::_colony_handle].producing,
                           sizeof(_current_item));
                    _active_prod[0] = -1;
                    _field_mode = 0;
                    Build_Arrays_();
                }
            }
            _colony_auto_building =
                MOX::_colony[COLONY::_colony_handle].auto_building != 0;
            Do_Draw_Build_Queue_();
            *selected_field_id = 0;
            return;
        }

        if (_fields[3] == input_field) {
            _field_mode = 0;
            COLONY::Reset_Mouse_Picture_();
            Commit_Autobuild_State_();

            if (Something_Behind_Settting_() == 0) {
                Do_Exit_Screen_Cleanup_();
            } else {
                while (true) {
                    int16_t current_item = 0;
                    for (int i = 0; i < 7; ++i) {
                        int16_t item = _current_item[i];
                        if (item == COLONY_PRODUCTION_HOUSING ||
                            item == COLONY_PRODUCTION_TRADE_GOODS ||
                            item == COLONY_PRODUCTION_REPEAT) {
                            if (Something_Behind_Settting_() != 0) {
                                current_item = item;
                                break;
                            }
                        }
                    }

                    char* str2 = Selection_Name_(current_item);
                    char* str1 = Selection_Name_(current_item);
                    char* format = ESTRINGS::E_Strings_(0x3b);
                    char buffer[500];
                    snprintf(buffer, sizeof(buffer), format, str1, str2);

                    if (MOX::_settings.auto_delete_tg_housing == 0) {
                        if (GENDRAW::Do_Confirmation_Box_(buffer) == 0) {
                            Do_Draw_Build_Queue_();
                            break;
                        }
                    }

                    for (int i = 0; i < 7; ++i) {
                        int16_t item = _current_item[i];
                        if (item == COLONY_PRODUCTION_HOUSING ||
                            item == COLONY_PRODUCTION_TRADE_GOODS ||
                            item == COLONY_PRODUCTION_REPEAT) {
                            if (Something_Behind_Settting_() != 0) {
                                Squish_Out_Queue_Slot_(i);
                            }
                        }
                    }

                    Do_Draw_Build_Queue_();
                    if (Something_Behind_Settting_() == 0) {
                        break;
                    }
                }
                Do_Exit_Screen_Cleanup_();
            }
        } else if (_fields[0] == input_field || _fields[1] == input_field) {
            if (_field_mode == 0 && _active_prod[0] == -1) {
                bool flag = false;
                for (int i = 0; i < 7; ++i) {
                    if (flag) {
                        break;
                    }
                    int16_t item = _current_item[i];
                    if (COLBLDG::Colony_Production_Is_Queued_Ship_(item)) {
                        if (ERIC::Sint_Is_In_List_(item, MOX::_colony[COLONY::_colony_handle].producing, 14) == 0) {
                            if (MOX::_ship[COLBLDG::Colony_Production_Ship_Index_(item)].status == 4) {
                                if (GENDRAW::Do_Confirmation_Box_(ESTRINGS::E_Strings_(0x1dc)) == 0) {
                                    *selected_field_id = 0;
                                    return;
                                }
                                flag = true;
                                Do_Draw_Build_Queue_();
                            }
                        }
                    }
                }

                for (int i = 0; i < 7; ++i) {
                    int16_t item = _current_item[i];
                    if (COLBLDG::Colony_Production_Is_Queued_Ship_(item)) {
                        ERIC::Sint_Is_In_List_(item, MOX::_colony[COLONY::_colony_handle].producing, 14);
                    }
                }

                Remove_Ships_(0);
            } else {
                _field_mode = 0;
                COLONY::Reset_Mouse_Picture_();
                _active_prod[0] = -1;
                *selected_field_id = 0;
                COLONY::Reset_Mouse_Picture_();
            }
        } else if (_fields[5] == input_field) {
            mouse::Set_Mouse_List_(&_mouse_list_which_ship, 1);
            _field_mode = 1;
        } else if (_fields[6] == input_field) {
            _field_mode = 2;
            mouse::Set_Mouse_List_(&_mouse_list_what, 1);
        } else if (_fields[4] == input_field) {
            int owner = MOX::_colony[COLONY::_colony_handle].owner;
            if (MOX::_player[owner].tech_fields[0x16] == 3 && MOX::_player[owner].tech_fields[0x17] == 3) {
                _field_mode = 0;
                COLONY::Reset_Mouse_Picture_();

                int16_t refit_ship = COLREFIT::Colony_Refit_Popup_();
                if (refit_ship != -1) {
                    Do_Draw_Build_Queue_();
                    int16_t popup2 = COLREFIT::Colony_Refit2_Popup_(refit_ship);
                    if (popup2 == -3) {
                        _refitting_ship_n = refit_ship;
                        MOX::_temp_star_handle = 1;
                        MOX::_refit_ship = 1;
                        _hacky_speed_refitted_ship_flag = 1;
                    } else if (popup2 == -2) {
                        _refitting_ship_n = refit_ship;
                        MOX::_refit_ship = 1;
                        struct_::Copy_Structure_(&MOX::_player[owner].ship_designs[5], &MOX::_ship[refit_ship].d, sizeof(MOX::_player[owner].ship_designs[5]));
                        _return_to_build_popup_from_custom_design = 1;
                        MOX::_temp_star_handle = 5;
                    }
                    Do_Draw_Build_Queue_();
                } else {
                    Do_Draw_Build_Queue_();
                }
            } else {
                GENDRAW::Help_(ESTRINGS::E_Strings_(0x244));
                Do_Draw_Build_Queue_();
            }
        } else if (_fields[2] != input_field) {
            if (_fields[10] == input_field) {
                Do_Exit_Screen_Cleanup_();
                COLONY::Tested_Colony_Buys_Outright_(COLONY::_colony_handle);
                _active_prod[0] = -1;
                Do_Draw_Build_Queue_();
                fields::Clear_Mouse_Buffer_();
                fields::Set_Input_Delay_(3);
            } else if (_fields[8] == input_field) {
                int16_t next_colony = COLONY::Get_Next_Colony_(COLONY::_colony_handle, 0);
                _field_mode = 0;
                COLONY::Reset_Mouse_Picture_();
                Commit_Autobuild_State_();
                Do_Exit_Screen_Cleanup_();
                if (next_colony != COLONY::_colony_handle) {
                    COLONY::Set_Colony_Screen_To_New_Colony_(next_colony);
                }
                COLONY::_full_draw = 1;
            } else if (_fields[9] == input_field) {
                int16_t next_colony = COLONY::Get_Next_Colony_(COLONY::_colony_handle, 1);
                if (next_colony != COLONY::_colony_handle) {
                    COLONY::Set_Colony_Screen_To_New_Colony_(next_colony);
                }
                _field_mode = 0;
                COLONY::Reset_Mouse_Picture_();
                Commit_Autobuild_State_();
                Do_Exit_Screen_Cleanup_();
                COLONY::_full_draw = 1;
            } else if (_fields[7] == input_field) {
                COLONY::_cheats = (COLONY::_cheats == 0) ? 1 : 0;
                GENDRAW::Help_(ESTRINGS::E_Strings_(0xda));
                Do_Draw_Build_Queue_();
            }
        }

        if (_field_mode != 1) {
            for (int i = 0; i < _n_building_indexes; ++i) {
                if (*scanned_field_id == _building_fields[i]) {
                    _scanned_prod = _building_indexes[i];
                }
                if (*selected_field_id == _building_fields[i]) {
                    int16_t bidx = _building_indexes[i];
                    if (Has_Prod_In_Queue_(bidx)) {
                        int16_t first_slot = First_Prod_Queue_Slot_(bidx);
                        if (MOX::_colony[COLONY::_colony_handle].bought_outright != 0 && first_slot == 0) {
                            GENDRAW::Help_(ESTRINGS::E_Strings_(0x23a));
                            Do_Draw_Build_Queue_();
                            _active_prod[0] = -1;
                            COLONY::Reset_Mouse_Picture_();
                            return;
                        }
                        Delete_From_Queue_(first_slot);
                        if (_active_prod[0] >= BUILDING_COUNT) {
                            if ((_active_prod[0] - first_slot) == BUILDING_COUNT) {
                                _active_prod[0] = -1;
                            }
                        }
                    } else {
                        if (Add_To_Queue_(bidx) == 0) {
                            GENDRAW::Help_(ESTRINGS::E_Strings_(0x188));
                            Do_Draw_Build_Queue_();
                        }
                    }
                }
            }
        }

        for (int i = 0; i < _n_military_indexes; ++i) {
            if (*scanned_field_id == _military_fields[i] && _military_indexes[i] != COLONY_PRODUCTION_SEPARATOR) {
                _scanned_prod = _military_indexes[i];
            }
            if (*selected_field_id == _military_fields[i] && _military_indexes[i] != COLONY_PRODUCTION_SEPARATOR) {
                if (_military_indexes[i] == BUILDING_COLONY_BASE) {
                    Calculate_Military_Array_();
                    COLONY::_full_draw = 1;
                }

                if (_military_indexes[i] == BUILDING_COLONY_BASE) {
                    int16_t star_idx = MOX::_planet[MOX::_colony[COLONY::_colony_handle].planet].star_index;
                    int16_t uncolonized = Star_N_Uncolonized_Worlds_(star_idx);
                    int16_t in_queue = N_Prods_In_Queue_(BUILDING_COLONY_BASE);

                    if (uncolonized <= in_queue) {
                        GENDRAW::Help_(ESTRINGS::E_Strings_(0x232));
                        Do_Draw_Build_Queue_();
                        continue;
                    }
                }

                int16_t midx = _military_indexes[i];
                if (_field_mode == 1) {
                    if (COLBLDG::Colony_Production_Is_Ship_Design_(midx)) {
                        _return_to_build_popup_from_custom_design = 1;
                        MOX::_temp_star_handle = COLBLDG::Colony_Production_Ship_Design_Index_(midx);
                        COLONY::Reset_Mouse_Picture_();
                        return;
                    }
                } else {
                    if (Add_To_Queue_(midx) == 0) {
                        GENDRAW::Help_(ESTRINGS::E_Strings_(0x188));
                        Do_Draw_Build_Queue_();
                    }
                }
            }
        }

        for (int i = 0; i < 7; ++i) {
            if (*scanned_field_id == _queue_fields[i]) {
                _scanned_prod = _current_item[i];
            }
            if (*selected_field_id == _queue_fields[i]) {
                if (_active_prod[0] == -1) {
                    int16_t selected_idx = static_cast<int16_t>(i);
                    if (selected_idx > 0 &&
                        _current_item[selected_idx] == COLONY_PRODUCTION_REPEAT) {
                        selected_idx--;
                    }
                    if (MOX::_colony[COLONY::_colony_handle].bought_outright != 0 &&
                        selected_idx == 0) {
                        GENDRAW::Help_(ESTRINGS::E_Strings_(0x23a));
                        Do_Draw_Build_Queue_();
                        return;
                    }
                    _active_prod[0] = selected_idx + BUILDING_COUNT;
                } else {
                    int16_t active_idx = _active_prod[0] - BUILDING_COUNT;
                    bool do_delete = false;

                    if (MOX::_colony[COLONY::_colony_handle].bought_outright != 0) {
                        if (i == 0 || active_idx == 0 ||
                            (i == 1 && _current_item[i] == COLONY_PRODUCTION_REPEAT)) {
                            GENDRAW::Help_(ESTRINGS::E_Strings_(0x23a));
                            Do_Draw_Build_Queue_();
                            _active_prod[0] = -1;
                            COLONY::Reset_Mouse_Picture_();
                            return;
                        }
                    }

                    if (i == active_idx) {
                        do_delete = true;
                    } else if (_current_item[active_idx] == COLONY_PRODUCTION_REPEAT && active_idx != 0 && (active_idx - i) == 1) {
                        do_delete = true;
                    } else if ((active_idx + 1) < 7 && _current_item[active_idx + 1] == COLONY_PRODUCTION_REPEAT && (active_idx - i) == -1) {
                        do_delete = true;
                    }

                    if (do_delete) {
                        Delete_From_Queue_(i);
                        _active_prod[0] = -1;
                        COLONY::Reset_Mouse_Picture_();
                    } else {
                        Insert_Before_Queue_Slot_(active_idx, i);
                        Defragment_Queue_();
                        COLONY::Reset_Mouse_Picture_();
                    }
                }

                if (_field_mode != 0) {
                    _field_mode = 0;
                    COLONY::Reset_Mouse_Picture_();
                    _active_prod[0] = -1;
                    *selected_field_id = 0;
                    COLONY::Reset_Mouse_Picture_();
                }
            }
        }

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            Do_Draw_Build_Queue_();
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        }

        if (selected_field_id != nullptr) {
            COLCALC::Colony_Calculation_(COLONY::_colony_handle);
        }
    }

    void __cdecl Draw_Active_Prod_() {
        int16_t active_idx = _active_prod[0];
        if (active_idx == -1) {
            return;
        }

        if (active_idx >= BUILDING_COUNT) {
            int16_t rel_idx = active_idx - BUILDING_COUNT;
            int16_t current = _current_item[rel_idx];
            int16_t scanned_prod = _scanned_prod;
            int16_t scanned_field = _scanned_field;

            bool is_match = false;
            if (current == scanned_prod) {
                is_match = true;
            } else if (rel_idx != 0 && current == COLONY_PRODUCTION_REPEAT && _current_item[rel_idx - 1] == scanned_prod) {
                is_match = true;
            } else if (rel_idx < 6 && _current_item[rel_idx + 1] == COLONY_PRODUCTION_REPEAT && scanned_prod == COLONY_PRODUCTION_REPEAT) {
                is_match = true;
            }

            if (is_match && scanned_field >= _queue_fields[0] && scanned_field <= _queue_fields[6] && current != -1) {
                int16_t base_y = rel_idx * 20;
                graphics::Box_(207, base_y + 332, 252, 16, 1, 239);

                if (rel_idx != 0 && current == COLONY_PRODUCTION_REPEAT) {
                    graphics::Box_(207, base_y + 312, 252, 16, 1, 239);
                }

                if (rel_idx < 6 && _current_item[rel_idx + 1] == COLONY_PRODUCTION_REPEAT) {
                    graphics::Box_(207, base_y + 352, 252, 16, 1, 239);
                }

                COLONY::Reset_Mouse_Picture_();
            } else {
                int16_t base_y = rel_idx * 20;
                graphics::Box_(207, base_y + 332, 252, 16, 1, 100);

                if (rel_idx != 0 && current == COLONY_PRODUCTION_REPEAT) {
                    graphics::Box_(207, base_y + 312, 252, 16, 1, 100);
                }

                if (rel_idx < 6 && _current_item[rel_idx + 1] == COLONY_PRODUCTION_REPEAT) {
                    graphics::Box_(207, base_y + 352, 252, 16, 1, 100);
                }

                mouse::Set_Mouse_List_(_mouse_list_to, 4);
            }
        } else {
            for (int16_t i = 0; i < _n_building_indexes; ++i) {
                if (_building_indexes[i] == active_idx) {
                    int16_t step = COLONY::_step_squish;
                    int16_t height = 18 - step;
                    int16_t row_spacing = 19 - step;
                    int16_t y = row_spacing * i + 18;
                    graphics::Box_(8, y, 171, height, 1, 100);
                }
            }
        }
    }

    void __cdecl Draw_Icon_(s_animation_header* anim) {
        ERIC::Draw_Centered_(244, 56, anim);
    }

    void __cdecl Clear_Colony_Queue_(int16_t colony_idx) {
        int16_t count = 7;
        do {
            Delete_From_Colony_Queue_(colony_idx, 0);
            count--;
        } while (count != 0);
    }

    void __cdecl Delete_Prod_From_Colony_Queue_(int16_t colony_idx, int16_t product_id) {
        int16_t slot_idx;

        slot_idx = COLBLDG::First_Prod_Colony_Queue_Slot_(colony_idx, product_id);
        while (slot_idx != -1) {
            COLBLDG::Delete_From_Colony_Queue_(colony_idx, slot_idx);
            slot_idx = COLBLDG::First_Prod_Colony_Queue_Slot_(colony_idx, product_id);
        }
    }

    void __cdecl Delete_Ship_(int16_t ship_index) {
        MOX::_ship[ship_index].status = 5;
        MOX::_ship[ship_index].location = -1;
    }

    void __cdecl Draw_Ship_Info_(int16_t item) {
        int16_t index = COLBLDG::Colony_Production_Ship_Index_(item);

        if (COLBLDG::Colony_Production_Is_Queued_Ship_(item)) {
            index = COLBLDG::Colony_Production_Ship_Index_(item);
            int16_t y_offset_counter = 0;
            fonts::Set_Font_Style_Shadow_Down_(4, &_l_font_color_array);

            int16_t current_y = y_offset_counter * 13 + 21;
            y_offset_counter++;

            s_ship_data* ship = &MOX::_ship[index];
            fonts::Print_(306, current_y, ship->d.name);

            y_offset_counter++;
            fonts::Set_Font_Style_Shadow_Down_(3, &_l_font_color_array);
            fonts::Set_Font_Spacing_(1);
            fonts::Set_Font_Spacing_Width_(2);

            COLBLDG::Draw_Cost_And_Time_Info_(COLONY::_colony_handle, item, &y_offset_counter);
            COLBLDG::Draw_Ship_Design_Info2_(&ship->d);
        }

        COLONY::Remap_Draw_Ship_Centered_(244, 56, index);
    }

    void __cdecl Squish_Out_Colony_Queue_Slot_(int16_t colony_idx, int16_t queue_idx) {
        s_colony* col = &MOX::_colony[colony_idx];

        do {
            for (int16_t i = queue_idx; i < 6; i++) {
                col->producing[i] = col->producing[i + 1];
            }
            col->producing[6] = COLONY_PRODUCTION_NONE;
        } while (col->producing[queue_idx] == COLONY_PRODUCTION_REPEAT);
    }

    /**
     * @brief Removes an item from a colony's production queue and cleans up associated ship data if necessary.
     *
     * @param colony_idx Index of the colony in the global colony array.
     * @param queue_idx Index of the production queue slot to delete.
     */
    void __cdecl Delete_From_Colony_Queue_(int16_t colony_idx, int16_t queue_idx) {
        if (queue_idx != -1) {
            if (queue_idx == 0 &&
                !BUILD_QUEUE::Unbuy_Product_(&MOX::_colony[colony_idx])) {
                return;
            }

            int16_t producing_id = MOX::_colony[colony_idx].producing[queue_idx];

            // Queued ship production items use the queued-ship encoding helper/base.
            if (COLBLDG::Colony_Production_Is_Queued_Ship_(producing_id)) {
                // Calculate original ship design ID or instance ID from the queue value
                COLBLDG::Delete_Ship_(COLBLDG::Colony_Production_Ship_Index_(producing_id));
            }

            COLBLDG::Squish_Out_Colony_Queue_Slot_(colony_idx, queue_idx);
        }
    }

    void __cdecl Add_Refited_Ship_To_Queue_() {
        MOX::_refit_ship = 0;
        int16_t ship_n = _refitting_ship_n;

        if (!Add_To_Queue_(COLBLDG::Colony_Production_From_Ship_Index_(ship_n))) {
            GENDRAW::Help_(ESTRINGS::E_Strings_(ESTR_NO_ROOM_IN_QUEUE_SHIP_NOT_REFITTED));
            Do_Draw_Build_Queue_();
            MAINSCR::Update_Ship_Icons_In_Between_Turns_();
            return;
        }

        int8_t owner = MOX::_colony[COLONY::_colony_handle].owner;
        s_ship_design old_design = MOX::_ship[ship_n].d;
        s_ship_design new_design = MOX::_player[owner].ship_designs[5];
        new_design.cost = DESIGN::Cost_Of_Design_(owner, &new_design);
        int16_t refit_cost = AIBUILD::Refit_Cost_(owner, &old_design, &new_design);

        struct_::Copy_Structure_(
            &MOX::_ship[ship_n].d, &new_design, sizeof(MOX::_ship[ship_n].d));
        MOX::_ship[ship_n].status = 4;
        MOX::_ship[ship_n].d.cost = refit_cost;
        MAINSCR::Update_Ship_Icons_In_Between_Turns_();
    }

    void __cdecl Do_Draw_Build_Queue_() {
        fields::Deactivate_Auto_Function_();
        fields::Assign_Auto_Function_(Draw_Build_Queue_Popup_, 1);
        Update_Fields_();
        COLONY::_full_draw = 1;
    }

    void __cdecl Squish_Out_Queue_Slot_(int16_t slot_idx) {
        do {
            for (int16_t i = slot_idx; i < 6; ++i) {
                _current_item[i] = _current_item[i + 1];
            }
            _current_item[6] = COLONY_PRODUCTION_NONE;
        } while (_current_item[slot_idx] == COLONY_PRODUCTION_REPEAT);
    }

    int8_t __cdecl Something_Behind_Settting_() {
        int16_t found_idx = -1;
        int8_t has_other = 0;

        for (int16_t i = 0; i < 7; ++i) {
            if (found_idx == -1) {
                int16_t item = _current_item[i];
                if (item != COLONY_PRODUCTION_NONE &&
                    (item == COLONY_PRODUCTION_HOUSING ||
                     item == COLONY_PRODUCTION_TRADE_GOODS ||
                     item == COLONY_PRODUCTION_REPEAT)) {
                    found_idx = i;
                }
            } else if (_current_item[i] != COLONY_PRODUCTION_NONE) {
                has_other = 1;
            }
        }

        return has_other;
    }

    void __cdecl Do_Exit_Screen_Cleanup_() {
        s_colony* colony = &MOX::_colony[COLONY::_colony_handle];
        int16_t original_front_item = colony->producing[0];

        Remove_Ships_(1);

        for (int16_t i = 0; i < 7; ++i) {
            colony->producing[i] = _current_item[i];
        }

        if (original_front_item != colony->producing[0]) {
            colony->bought_outright = 0;
        }
    }

    int8_t __cdecl Add_To_Queue_(int16_t product_id) {
        int16_t q_len = COLBLDG::Get_Prod_Queue_Len_();
        if (q_len == -1) {
            if (COLBLDG::_field_mode != 0) {
                COLBLDG::_field_mode = 0;
                COLONY::Reset_Mouse_Picture_();
            }
            return 0;
        }

        bool add_to_queue = false;

        if (COLBLDG::Colony_Production_Is_Building_(product_id) ||
            product_id == COLONY_PRODUCTION_HOUSING ||
            product_id == COLONY_PRODUCTION_TRADE_GOODS ||
            product_id == COLONY_PRODUCTION_REPEAT ||
            product_id == COLONY_PRODUCTION_FREIGHTERS ||
            product_id == COLONY_PRODUCTION_SPY ||
            COLBLDG::Colony_Production_Is_Queued_Ship_(product_id))
        {
            add_to_queue = true;
        } else if (product_id == COLONY_PRODUCTION_SCIENTIST ||
                   product_id == COLONY_PRODUCTION_FARMER ||
                   product_id == COLONY_PRODUCTION_WORKER) {
            int16_t planet_idx = MOX::_colony[COLONY::_colony_handle].planet;
            int16_t n_terra = COLBLDG::N_Prods_In_Queue_(COLONY_PRODUCTION_SCIENTIST);
            int16_t n_rad = COLBLDG::N_Prods_In_Queue_(COLONY_PRODUCTION_WORKER);
            int16_t n_grav = COLBLDG::N_Prods_In_Queue_(COLONY_PRODUCTION_FARMER);
            uint8_t pops = MOX::_colony[COLONY::_colony_handle].n_pops;
            int16_t max_pop = COLCALC::Planet_Max_Population_For_Player_(&MOX::_planet[planet_idx], MOX::_PLAYER_NUM);
            
            if ((n_terra + n_rad + n_grav + pops + 1) >= max_pop) {
                if (GENDRAW::Do_Confirmation_Box_(ESTRINGS::E_Strings_(0x208))) {
                    add_to_queue = true;
                } else {
                    _field_mode = 0;
                    COLONY::Reset_Mouse_Picture_();
                }
            } else {
                add_to_queue = true;
            }
        } else if (product_id == COLONY_PRODUCTION_TRANSPORT_SHIP ||
                   product_id == COLONY_PRODUCTION_COLONY_SHIP ||
                   product_id == COLONY_PRODUCTION_CUSTOM_DESIGN ||
                   product_id == COLONY_PRODUCTION_DOOM_STAR ||
                   product_id == COLONY_PRODUCTION_OUTPOST_SHIP) {
            int16_t design_slot = 4;
            int16_t auto_type = COLBLDG::Auto_Design_Type_(product_id);
            int16_t owner = MOX::_colony[COLONY::_colony_handle].owner;
            
            if (auto_type == SHIP_TYPE_COLONY) {
                design_slot = 5;
                INITSHIP::Load_Colony_Ship_Design_(owner, &MOX::_player[owner].ship_designs[5]);
            } else if (auto_type == SHIP_TYPE_OUTPOST) {
                design_slot = 5;
                INITSHIP::Load_Outpost_Ship_Design_(owner, &MOX::_player[owner].ship_designs[5]);
            } else if (auto_type == SHIP_TYPE_TRANSPORT) {
                design_slot = 5;
                INITSHIP::Load_Transport_Ship_Design_(owner, &MOX::_player[owner].ship_designs[5]);
            } else if (auto_type == SHIP_TYPE_COMBAT) {
                if (product_id == COLONY_PRODUCTION_DOOM_STAR) {
                    design_slot = 5;
                    if (!COLCALC::Colony_Can_Build_Product_(&MOX::_colony[COLONY::_colony_handle], COLBLDG::Colony_Production_From_Ship_Design_Index_(SHIP_SIZE_DOOM_STAR))) {
                        GENDRAW::Help_(ESTRINGS::E_Strings_(0x245));
                        COLBLDG::Do_Draw_Build_Queue_();
                        return 1;
                    }
                } else {
                    MOX::_temp_star_handle = 4;
                    COLBLDG::_return_to_build_popup_from_custom_design = 1;
                    COLBLDG::Commit_Autobuild_State_();
                    return 1;
                }
            }
            
            int16_t created_ship = COLBLDG::Colbldg_Create_Ship_(design_slot);
            if (created_ship == -1) {
                GENDRAW::Help_(ESTRINGS::E_Strings_(0x21a));
                COLBLDG::Do_Draw_Build_Queue_();
                COLBLDG::_field_mode = 0;
            } else {
                COLBLDG::_current_item[q_len] = COLBLDG::Colony_Production_From_Ship_Index_(created_ship);
            }
        } else if (!COLBLDG::Colony_Production_Is_Ship_Design_(product_id)) {
            GENDRAW::Help_(ESTRINGS::E_Strings_(0x14c));
            COLBLDG::Do_Draw_Build_Queue_();
            return 0;
        } else {
            if (COLCALC::Colony_Can_Build_Product_(&MOX::_colony[COLONY::_colony_handle], product_id)) {
                int16_t created_ship = COLBLDG::Colbldg_Create_Ship_(COLBLDG::Colony_Production_Ship_Design_Index_(product_id));
                if (created_ship == -1) {
                    GENDRAW::Help_(ESTRINGS::E_Strings_(0x21a));
                    COLBLDG::Do_Draw_Build_Queue_();
                    COLBLDG::_field_mode = 0;
                } else {
                    COLBLDG::_current_item[q_len] = COLBLDG::Colony_Production_From_Ship_Index_(created_ship);
                }
            } else {
                int8_t size = MOX::_player[MOX::_PLAYER_NUM].ship_designs[COLBLDG::Colony_Production_Ship_Design_Index_(product_id)].size;
                int16_t owner = MOX::_colony[COLONY::_colony_handle].owner;
                if (size == SHIP_SIZE_TITAN && MOX::_player[owner].tech_applications[TECH_APP_TITAN_CONSTRUCTION] != TECH_RESEARCH_STATUS_RESEARCHED) {
                    GENDRAW::Help_(ESTRINGS::E_Strings_(0x246));
                    COLBLDG::Do_Draw_Build_Queue_();
                } else if (size == SHIP_SIZE_DOOM_STAR && MOX::_player[owner].tech_applications[TECH_APP_DOOM_STAR_CONSTRUCTION] != TECH_RESEARCH_STATUS_RESEARCHED) {
                    GENDRAW::Help_(ESTRINGS::E_Strings_(0x245));
                    COLBLDG::Do_Draw_Build_Queue_();
                } else {
                    GENDRAW::Help_(ESTRINGS::E_Strings_(0x237));
                    COLBLDG::Do_Draw_Build_Queue_();
                }
            }
        }

        if (add_to_queue) {
            COLBLDG::_current_item[q_len] = product_id;
        }

        if (COLBLDG::_active_prod[0] != -1 && COLBLDG::_field_mode != 2) {
            COLBLDG::Insert_Before_Queue_Slot_(q_len, COLBLDG::_active_prod[0] - BUILDING_COUNT);
            COLONY::Reset_Mouse_Picture_();
        }
        
        if (COLBLDG::_field_mode == 2) {
            COLBLDG::_field_mode = 0;
            COLONY::Reset_Mouse_Picture_();
            if (product_id != COLONY_PRODUCTION_HOUSING &&
                product_id != COLONY_PRODUCTION_TRADE_GOODS &&
                product_id != COLONY_PRODUCTION_REPEAT) {
                if (COLBLDG::Add_To_Queue_(COLONY_PRODUCTION_REPEAT) == 0) {
                    GENDRAW::Help_(ESTRINGS::E_Strings_(0x188));
                    COLBLDG::Do_Draw_Build_Queue_();
                    if (COLBLDG::_field_mode != 0) {
                        COLBLDG::_field_mode = 0;
                        COLONY::Reset_Mouse_Picture_();
                    }
                }
            }
        }
        
        COLBLDG::Defragment_Queue_();
        return 1;
    }

    void __cdecl Delete_From_Queue_(int16_t slot_idx) {
        if (slot_idx == -1) {
            return;
        }

        int16_t item = _current_item[slot_idx];

        if (COLBLDG::Colony_Production_Is_Queued_Ship_(item)) {
            int16_t ship_idx = COLBLDG::Colony_Production_Ship_Index_(item);
            if (MOX::_ship[ship_idx].status == 4) {
                if (GENDRAW::Do_Confirmation_Box_(ESTRINGS::E_Strings_(0x213 /* Cancel ship building? */)) == 0) {
                    Do_Draw_Build_Queue_();
                    return;
                }
            }

            if (!ERIC::Sint_Is_In_List_(item, (int16_t*)MOX::_colony[COLONY::_colony_handle].producing, 14)) {
                Delete_Ship_(ship_idx);
            }
            Do_Draw_Build_Queue_();
        }

        if (slot_idx != 0 && item == COLONY_PRODUCTION_REPEAT) {
            int16_t prev_item = _current_item[slot_idx - 1];
            if (COLBLDG::Colony_Production_Is_Queued_Ship_(prev_item)) {
                int16_t prev_ship_idx = COLBLDG::Colony_Production_Ship_Index_(prev_item);
                if (MOX::_ship[prev_ship_idx].status == 4) {
                    if (GENDRAW::Do_Confirmation_Box_(ESTRINGS::E_Strings_(0x213 /* Cancel ship building? */)) == 0) {
                        Do_Draw_Build_Queue_();
                        return;
                    }
                }

                if (!ERIC::Sint_Is_In_List_(prev_item, (int16_t*)MOX::_colony[COLONY::_colony_handle].producing, 14)) {
                    Delete_Ship_(prev_ship_idx);
                }
                Do_Draw_Build_Queue_();
            }

            Squish_Out_Queue_Slot_(slot_idx);
            Squish_Out_Queue_Slot_(slot_idx - 1);
            return;
        }

        if (slot_idx < 6 && _current_item[slot_idx + 1] == COLONY_PRODUCTION_REPEAT) {
            Squish_Out_Queue_Slot_(slot_idx + 1);
            Squish_Out_Queue_Slot_(slot_idx);
            return;
        }

        Squish_Out_Queue_Slot_(slot_idx);
    }

    void __cdecl Defragment_Queue_() {
        for (int16_t pass = 2; pass != 0; --pass) {
            for (int16_t slot = 0; slot < 7; ++slot) {
                if (_current_item[slot] == COLONY_PRODUCTION_NONE) {
                    Squish_Out_Queue_Slot_(slot);
                }
            }
        }
    }

    void __cdecl Insert_Before_Queue_Slot_(int16_t source_slot_idx, int16_t target_slot_idx) {
        int16_t item1 = _current_item[source_slot_idx];
        int16_t item2 = (source_slot_idx < 6) ? _current_item[source_slot_idx + 1] : -1;

        if (_current_item[target_slot_idx] == COLONY_PRODUCTION_REPEAT) {
            target_slot_idx--;
        }

        if (item1 == COLONY_PRODUCTION_REPEAT) {
            source_slot_idx--;
            item1 = _current_item[source_slot_idx];
            item2 = (source_slot_idx < 6) ? _current_item[source_slot_idx + 1] : -1;
        }

        Squish_Out_Queue_Slot_(source_slot_idx);

        if (target_slot_idx <= 5) {
            for (int16_t i = 5; i >= target_slot_idx; i--) {
                _current_item[i + 1] = _current_item[i];
            }
        }

        _current_item[target_slot_idx] = item1;

        if (item2 == COLONY_PRODUCTION_REPEAT) {
            int16_t offset = (source_slot_idx >= target_slot_idx) ? 1 : 0;
            source_slot_idx += offset;
            Squish_Out_Queue_Slot_(source_slot_idx);

            int16_t target = target_slot_idx + offset;
            if (target <= 5) {
                for (int16_t i = 5; i >= target; i--) {
                    _current_item[i + 1] = _current_item[i];
                }
            }

            _current_item[target] = item2;
        }

        if (_field_mode != 2) {
            _active_prod[0] = -1;
        }
    }

    void __cdecl Remove_Ships_(int8_t flag) {
        for (int16_t i = 0; i < 7; ++i) {
            int16_t prod;
            if (flag == 0) {
                prod = _current_item[i];
                if (COLBLDG::Colony_Production_Is_Queued_Ship_(prod)) {
                    if (First_Prod_Colony_Queue_Slot_(COLONY::_colony_handle, prod) == -1) {
                        int16_t ship_idx = COLBLDG::Colony_Production_Ship_Index_(prod);
                        int16_t officer_idx = MOX::_ship[ship_idx].officer_index;
                        if (officer_idx != -1) {
                            OFFICER::Move_Officer_To_Pool_(officer_idx);
                            MOX::_ship[ship_idx].officer_index = -1;
                        }
                        Delete_Ship_(ship_idx);
                    }
                }
            } else {
                prod = MOX::_colony[COLONY::_colony_handle].producing[i];
                if (COLBLDG::Colony_Production_Is_Queued_Ship_(prod)) {
                    if (First_Prod_Queue_Slot_(prod) == -1) {
                        int16_t ship_idx = COLBLDG::Colony_Production_Ship_Index_(prod);
                        int16_t officer_idx = MOX::_ship[ship_idx].officer_index;
                        if (officer_idx != -1) {
                            OFFICER::Move_Officer_To_Pool_(officer_idx);
                            MOX::_ship[ship_idx].officer_index = -1;
                        }
                        Delete_Ship_(ship_idx);
                    }
                }
            }
        }
    }

    void __cdecl Build_Weapon_String_(s_ship_weapons* weapon, char* dest, uint32_t dest_size, uint8_t include_postfix_modifiers, uint8_t weapon_count) {
        char buf1[500];
        char buf2[500];
        char buf3[500];

        int16_t weapon_id = weapon->type;

        char* name;
        if (weapon_count <= 1) {
            name = TECHDATA::_weapons[weapon_id].name;
        } else {
            name = TECHDATA::_weapons[weapon_id].name_plural;
        }

        strlcpy(buf2, name, sizeof(buf2));
        strlcpy(buf1, ESTRINGS::E_Strings_(0xC), sizeof(buf1));
        strlcpy(buf3, ESTRINGS::E_Strings_(0xC), sizeof(buf3));

        uint8_t* mods_ptr = (uint8_t*)&weapon->specials;
        for (int16_t bit = 0; bit < WEAPON_MOD_COUNT; bit++) {
            if (struct_::Test_Bit_Field_(mods_ptr, bit)) {
                if (bit == 1 || bit == 2) {
                    strlcat(buf1, TECHDATA::_weapon_modifications[bit].name_plural, sizeof(buf1));
                    strlcat(buf1, " ", sizeof(buf1));
                } else {
                    strlcat(buf3, TECHDATA::_weapon_modifications[bit].name_plural, sizeof(buf3));
                }
            }
        }

        if (include_postfix_modifiers != 0) {
            snprintf(dest, dest_size, ESTRINGS::E_Strings_(0x51), buf1, buf2, buf3);
        } else {
            snprintf(dest, dest_size, ESTRINGS::E_Strings_(0x51), buf1, buf2, ESTRINGS::E_Strings_(0xC));
        }
    }

    char* __cdecl Option_String_(int16_t option_id) {
        switch (option_id) {
            case COLONY_PRODUCTION_OUTPOST_SHIP:
                return ESTRINGS::E_Strings_(0x195);
            case COLONY_PRODUCTION_DOOM_STAR:
                return ESTRINGS::E_Strings_(0x10F);
            case COLONY_PRODUCTION_FREIGHTERS:
                return ESTRINGS::E_Strings_(0x12D);
            case COLONY_PRODUCTION_CUSTOM_DESIGN:
                return ESTRINGS::E_Strings_(0x0FA);
            case COLONY_PRODUCTION_COLONY_SHIP:
                return ESTRINGS::E_Strings_(0x0E6);
            case COLONY_PRODUCTION_TRANSPORT_SHIP:
                return ESTRINGS::E_Strings_(0x21E);
            case COLONY_PRODUCTION_REPEAT:
                return ESTRINGS::E_Strings_(0x088);
            case COLONY_PRODUCTION_SEPARATOR:
            case COLONY_PRODUCTION_NONE:
            case 0:
                return ESTRINGS::E_Strings_(0x00C);
            case COLONY_PRODUCTION_REFIT:
                return ESTRINGS::E_Strings_(0x1BD);
            case COLONY_PRODUCTION_SPY:
                return ESTRINGS::E_Strings_(0x1EC);
            case COLONY_PRODUCTION_SCIENTIST:
                return ESTRINGS::E_Strings_(0x0B1);
            case COLONY_PRODUCTION_WORKER:
                return ESTRINGS::E_Strings_(0x0B2);
            case COLONY_PRODUCTION_FARMER:
                return ESTRINGS::E_Strings_(0x0B0);
            case COLONY_PRODUCTION_HOUSING:
                return ESTRINGS::E_Strings_(0x142);
            case COLONY_PRODUCTION_TRADE_GOODS:
                return ESTRINGS::E_Strings_(0x21D);
            default:
                break;
        }

        if (COLBLDG::Colony_Production_Is_Ship_Design_(option_id)) {
            const int32_t colony_owner = (int32_t)MOX::_colony[COLONY::_colony_handle].owner;
            const int32_t ship_design_index = COLBLDG::Colony_Production_Ship_Design_Index_(option_id);
            return MOX::_player[colony_owner].ship_designs[ship_design_index].name;
        }

        RUSS::PFMT_(0, 0, ESTRINGS::E_Strings_(248 /* 0xF8 */), option_id);
        key::Wait_For_Key_();
        return ESTRINGS::E_Strings_(0x00C);
    }

} // namespace COLBLDG