#include "pch.h"

namespace COLREFIT {

    int16_t _first_ship;
    int16_t _slider_bar_position;
    int16_t _fields[12];
    int16_t _esc;
    int16_t _scanned_ship_index = -1;
    s_animation_header* _anims[4];
    int16_t _scanned_ship_d_index = -1;
    int16_t _lscanned_field = -1000;
    int16_t* _g_ship_field_list_ptr;
    MOX2::action_func_t _saved_autofunction;
    int16_t* _g_ship_list_ptr;
    int16_t _n_ships_in_list;
    int16_t _g_ship_n;
    s_scroll_state* _g_scroll_ptr;

    void __cdecl Update_First_Ship_(int16_t* first_ship_ptr, int16_t* slider_pos_ptr) {
        int16_t rounded_ship_count = ((_n_ships_in_list + 5) / 5) * 5;

        if (rounded_ship_count < 15) {
            *slider_pos_ptr = -1;
            *first_ship_ptr = 0;
            return;
        }

        if ((rounded_ship_count - *slider_pos_ptr) - *first_ship_ptr != 15) {
            COLONY::_full_draw = 1;
            rounded_ship_count = (rounded_ship_count - *slider_pos_ptr) - 15;
            *first_ship_ptr = rounded_ship_count;
            *first_ship_ptr = (rounded_ship_count / 5) * 5;
            _slider_bar_position = (((_n_ships_in_list + 5) / 5) - 3) * 5 - _first_ship;
        }
    }

    void __cdecl Build_Ship_List_() {
        int16_t n_ships = 0;

        for (int16_t ship_idx = 0; ship_idx < MAX_SHIPS; ++ship_idx) {
            if (MOX::_ship[ship_idx].owner == MOX::_PLAYER_NUM &&
                MOX::_ship[ship_idx].status < 3 &&
                MOX::_ship[ship_idx].location == MOX::_screen_data &&
                MOX::_ship[ship_idx].d.ship_type == SHIP_TYPE_COMBAT &&
                n_ships < MAX_SHIPS) {

                _g_ship_list_ptr[n_ships] = ship_idx;
                n_ships++;
            }
        }

        _n_ships_in_list = n_ships;

        while (n_ships < MAX_SHIPS) {
            _g_ship_list_ptr[n_ships] = -1;
            n_ships++;
        }
    }

    int16_t __cdecl Colony_Refit_Popup_() {
        int16_t hovered_field_id;
        int16_t selected_field_id;
        s_scroll_state scroll_data = {0};
        int16_t ship_field_list[MAX_SHIPS];
        int16_t ship_list[MAX_SHIPS];
        int16_t popup_result = 0;

        selected_field_id = 0;

        _g_scroll_ptr = &scroll_data;
        _g_ship_list_ptr = ship_list;
        _g_ship_field_list_ptr = ship_field_list;
        
        Build_Ship_List_();
        
        _first_ship = 0;
        _slider_bar_position = (((_n_ships_in_list + 5) / 5) - 3) * 5;
        
        allocate::Push_Block_(MOX::_screen_seg);
        COLONY::Set_Colony_Font_();
        
        _esc = 0;
        
        Load_Refit_Ship_Anims_();
        
        MOX2::Get_Auto_Function_(&_saved_autofunction);
        fields::Deactivate_Auto_Function_();
        fields::Assign_Auto_Function_(Draw_Refit_Popup_, 1);
        
        ERICHELP::Set_Refit_Ship_Popup_Help_List_();
        Add_Fields_();
        
        graphics::Set_Window_(0, 0, 0x27f, 0x1df);
        video::Clip_On_();
        Draw_Refit_Popup_();
        video::Set_Refresh_Stencil_();
        
        COLONY::_full_draw = 1;
        
        do {
            selected_field_id = fields::Get_Input_();
            hovered_field_id = fields::Scan_Input_();
            
            Draw_Refit_Popup_();
            
            popup_result = Evaluate_Input_(&selected_field_id, &hovered_field_id);
            
            RUSS::Mox_Sync_Update_();
        } while (_esc == 0);
        
        fields::Clear_Fields_();
        fields::Deactivate_Help_List_();
        fields::Deactivate_Auto_Function_();
        
        MOX2::Put_Auto_Function_(&_saved_autofunction);
        
        video::Copy_Back_To_Off_();
        
        allocate::Pop_Block_(MOX::_screen_seg);
        video::Clip_Off_();
        
        return popup_result;
    }

    int16_t __cdecl Colony_Refit2_Popup_(int16_t ship_idx) {
        int16_t match_count = 0;
        int16_t selected_field_id = 0;
        int16_t hovered_field_id;
        s_scroll_state scroll_data = {0};
        int16_t ship_field_list[MAX_SHIPS];
        int16_t ship_list[MAX_SHIPS];

        _g_ship_n = ship_idx;
        _g_scroll_ptr = &scroll_data;
        _g_ship_list_ptr = ship_list;
        _g_ship_field_list_ptr = ship_field_list;

        for (int16_t design_idx = 0; design_idx < 5; ++design_idx) {
            if (MOX::_player[MOX::_PLAYER_NUM].ship_designs[design_idx].size == MOX::_ship[ship_idx].d.size) {
                match_count++;
            }
        }

        if (match_count == 0) {
            return -2;
        }

        allocate::Push_Block_(MOX::_screen_seg);
        COLONY::Set_Colony_Font_();
        _esc = 0;

        Load_Refit2_Ship_Anims_();

        MOX2::Get_Auto_Function_(&_saved_autofunction);
        fields::Deactivate_Auto_Function_();
        fields::Assign_Auto_Function_(Draw_Refit2_Popup_, 1);
        Do_Add2_Fields_();

        graphics::Set_Window_(0, 0, 0x27f, 0x1df);
        video::Clip_On_();
        Draw_Refit2_Popup_();
        video::Set_Refresh_Stencil_();
        COLONY::_full_draw = 1;

        int16_t popup_result = 0;
        do {
            selected_field_id = fields::Get_Input_();
            hovered_field_id = fields::Scan_Input_();
            Draw_Refit2_Popup_();
            popup_result = Evaluate2_Input_(&selected_field_id, &hovered_field_id);
            RUSS::Mox_Sync_Update_();
        } while (_esc == 0);

        fields::Clear_Fields_();
        fields::Deactivate_Help_List_();
        fields::Deactivate_Auto_Function_();
        MOX2::Put_Auto_Function_(&_saved_autofunction);

        video::Copy_Back_To_Off_();
        allocate::Pop_Block_(MOX::_screen_seg);
        video::Clip_Off_();

        return popup_result;
    }

    int16_t __cdecl Evaluate_Input_(int16_t* selected_field_id, int16_t* hovered_field_id) {
        char text[500];

        COLREFIT::_scanned_ship_index = -1;

        for (int16_t i = 0; i < 15; i++) {
            int16_t ship_idx = COLREFIT::_g_ship_list_ptr[COLREFIT::_first_ship + i];

            if (COLREFIT::_g_ship_field_list_ptr[i] == *selected_field_id && ship_idx != -1) {
                if (MOX::_ship[ship_idx].d.picture_num == 44) {
                    GENDRAW::Help_(ESTRINGS::E_Strings_(0x23F));
                    COLONY::_full_draw = 1;
                } else {
                    int16_t planet_idx = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);
                    if (COLCALC::Colony_Can_Build_Product_(&MOX::_colony[MOX::_planet[planet_idx].colony_index], COLBLDG::Colony_Production_From_Ship_Index_(ship_idx))) {
                        if (MOX::_ship[ship_idx].d.previous_owner == MOX::_PLAYER_NUM) {
                            if (MOX::_ship[ship_idx].officer_index == -1) {
                                COLREFIT::_esc = 1;
                                return ship_idx;
                            } else {
                                char* leader_name = OFFICER::Leader_Name_(MOX::_ship[ship_idx].officer_index, 0);
                                char* format_str = ESTRINGS::E_Strings_(0x212);
                                snprintf(text, sizeof(text), format_str, leader_name);

                                if (GENDRAW::Do_Confirmation_Box_(text) != 0) {
                                    OFFICER::Move_Officer_To_Pool_(MOX::_ship[ship_idx].officer_index);
                                    MOX::_ship[ship_idx].officer_index = -1;
                                    COLREFIT::_esc = 1;
                                    return ship_idx;
                                } else {
                                    COLREFIT::_esc = 1;
                                    return ship_idx;
                                }
                            }
                        } else {
                            GENDRAW::Help_(ESTRINGS::E_Strings_(0x23D));
                            COLONY::_full_draw = 1;
                        }
                    } else {
                        GENDRAW::Help_(ESTRINGS::E_Strings_(0x23E));
                        COLONY::_full_draw = 1;
                    }
                }
            }
        }

        for (int16_t i = 0; i < 15; i++) {
            int16_t ship_idx = COLREFIT::_g_ship_list_ptr[COLREFIT::_first_ship + i];
            if (COLREFIT::_g_ship_field_list_ptr[i] == *hovered_field_id && ship_idx != -1) {
                ERIC::Pulsing_Box_((i % 5) * 60 + 161, (i / 5) * 61 + 49, 55, 56, 1, 1, 2);
                COLREFIT::_scanned_ship_index = COLREFIT::_first_ship + i;
            }
        }

        int32_t next_page_ship_index = COLREFIT::_first_ship + 15;
        if (*selected_field_id == _fields[1] && next_page_ship_index < MAX_SHIPS && COLREFIT::_g_ship_list_ptr[next_page_ship_index] != -1) {
            int32_t next_first = COLREFIT::_first_ship + 5;
            if (next_first >= MAX_SHIPS) {
                next_first = MAX_SHIPS;
            }
            COLREFIT::_first_ship = (int16_t)next_first;
            COLONY::_full_draw = 1;
            COLREFIT::_slider_bar_position = (((COLREFIT::_n_ships_in_list + 5) / 5) - 3) * 5 - COLREFIT::_first_ship;
            COLREFIT::Update_First_Ship_(&COLREFIT::_first_ship, &COLREFIT::_slider_bar_position);
        }

        if (*selected_field_id == _fields[2]) {
            int32_t next_first = COLREFIT::_first_ship - 5;
            if (next_first <= 0) {
                next_first = 0;
            }
            COLREFIT::_first_ship = (int16_t)next_first;
            COLREFIT::_slider_bar_position = (((COLREFIT::_n_ships_in_list + 5) / 5) - 3) * 5 - COLREFIT::_first_ship;
            COLREFIT::Update_First_Ship_(&COLREFIT::_first_ship, &COLREFIT::_slider_bar_position);
            COLONY::_full_draw = 1;
        }

        if (*selected_field_id == _fields[3]) {
            COLREFIT::_esc = 1;
        }

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            COLONY::_full_draw = 1;
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        }

        return -1;
    }

    void __cdecl Draw_Refit2_Popup_() {
        if (COLONY::_full_draw == 0) {
            Draw_Ship_D_List_();
        } else {
            COLONY::Set_Colony_Font_();
            video::Set_Page_Off_();
            MOX2::Execute_Auto_Function_(&_saved_autofunction);
            COLONY::_full_draw = 1;
            animate::Draw_(0x93, 6, _anims[0]);
            Draw_Ship_D_List_();
            video::Copy_Off_To_Back_();
            COLONY::_full_draw = 0;
        }

        if (_scanned_ship_d_index != -1) {
            Show_Ship_Design_(&MOX::_player[MOX::_PLAYER_NUM].ship_designs[_scanned_ship_d_index]);
        }

        if (_lscanned_field == _fields[11]) {
            COLONY::Set_Colony_Font_To_Reg_Blue_(4, 1);
            char* str = ESTRINGS::E_Strings_(0x322);
            BILL::Squeeze_Formatted_Paragraph_Centered_(0xA8, 0xF8, 0x131, 0xA9, str, 2);
        }
    }

    void __cdecl Draw_Refit_Popup_() {
        COLONY::_full_draw = 1;
        if (COLONY::_full_draw == 0) {
            Draw_Bar_Indicator_();
            if (_scanned_ship_index != -1) {
                Scan_Ship_Info_(_g_ship_list_ptr[_scanned_ship_index]);
            }
        } else {
            COLONY::Set_Colony_Font_();
            video::Set_Page_Off_();
            MOX2::Execute_Auto_Function_(&_saved_autofunction);
            COLONY::_full_draw = 1;
            animate::Draw_(0x93, 6, _anims[0]);
            Draw_Bar_Indicator_();
            Draw_Ship_List_();
            video::Copy_Off_To_Back_();
            COLONY::_full_draw = 0;

            if (_scanned_ship_index != -1) {
                Scan_Ship_Info_(_g_ship_list_ptr[_scanned_ship_index]);
            }
        }
    }

    void __cdecl Load_Refit_Ship_Anims_() {
        _anims[0] = (s_animation_header*)farload::Far_Reload_Next_("refitpup.LBX", 0, MOX::_screen_seg);
        _anims[2] = (s_animation_header*)farload::Far_Reload_Next_("refitpup.LBX", 1, MOX::_screen_seg);
        _anims[1] = (s_animation_header*)farload::Far_Reload_Next_("refitpup.LBX", 2, MOX::_screen_seg);
        _anims[3] = (s_animation_header*)farload::Far_Reload_Next_("refitpup.LBX", 3, MOX::_screen_seg);
    }

    void __cdecl Load_Refit2_Ship_Anims_() {
        _anims[0] = (s_animation_header*)farload::Far_Reload_Next_("refitpup.LBX", 4, MOX::_screen_seg);
        _anims[2] = (s_animation_header*)farload::Far_Reload_Next_("refitpup.LBX", 1, MOX::_screen_seg);
        _anims[1] = (s_animation_header*)farload::Far_Reload_Next_("refitpup.LBX", 2, MOX::_screen_seg);
        _anims[3] = (s_animation_header*)farload::Far_Reload_Next_("refitpup.LBX", 3, MOX::_screen_seg);
    }

    void __cdecl Show_Ship_Design_(s_ship_design* design) {
        char buffer[10000];

        COLONY::Set_Colony_Font_To_(1);
        COLBLDG::Build_Ship_Design_Info_Formatted_Paragraph_(design, buffer);
        BILL::Squeeze_Print_Formatted_Paragraph_(168, 248, 305, 169, buffer, 0);
    }

    void __cdecl Do_Add2_Fields_() {
        fields::Clear_Fields_();
        Add2_Fields_();
    }

    void __cdecl Add_Fields_() {
        fields::Clear_Fields_();
        Add_Ship_List_Fields_();

        _fields[1] = fields::Add_Button_Field_(
            0x1D5, 0x197, 
            ESTRINGS::E_Strings_(0xC), 
            _anims[1], 
            ESTRINGS::E_Strings_(0x89), 
            '('
        );

        _fields[2] = fields::Add_Button_Field_(
            0x1D4, 0x2E, 
            ESTRINGS::E_Strings_(0xC), 
            _anims[2], 
            ESTRINGS::E_Strings_(0xB), 
            '('
        );

        _fields[3] = fields::Add_Button_Field_(
            0x112, 0x1B8, 
            ESTRINGS::E_Strings_(0xC), 
            _anims[3], 
            ESTRINGS::E_Strings_(0xC), 
            '('
        );

        int16_t list_pages = (_n_ships_in_list + 5) / 5;

        _fields[4] = fields::Add_Scroll_Field_(
            0x1D4, 0x45, 0, list_pages * 5, 0, (list_pages - 3) * 5, 0xB, 0x150,
            &_slider_bar_position, ESTRINGS::E_Strings_(0xC), 0
        );

        _fields[0] = fields::Add_Hidden_Field_(
            0, 0, 0x27F, 0x1DF, ESTRINGS::E_Strings_(0xC), 0
        );
    }

    void __cdecl Add_Ship_List_Fields_() {
        for (int16_t slot_idx = 0; slot_idx < 15; ++slot_idx) {
            if (_g_ship_list_ptr[slot_idx] == -1) {
                _g_ship_field_list_ptr[slot_idx] = -1000;
            } else {
                int32_t y_offset = (slot_idx / 5) * 61;
                int32_t x_offset = (slot_idx % 5) * 60;
                char* hotkey = ESTRINGS::E_Strings_(12);

                _g_ship_field_list_ptr[slot_idx] = fields::Add_Hidden_Field_(
                    x_offset + 161,
                    y_offset + 49,
                    x_offset + 216,
                    y_offset + 105,
                    hotkey,
                    0
                );
            }
        }
    }

    void __cdecl Draw_Bar_Indicator_() {
        Update_First_Ship_(&_first_ship, &_slider_bar_position);

        if (_n_ships_in_list >= 15) {
            int32_t y1 = (_first_ship * 335) / _n_ships_in_list + 69;
            int32_t y2 = ((_first_ship + 15) * 335) / (((_n_ships_in_list + 5) / 5) * 5) + 69;

            graphics::Fill_(0x1d4, y1, 0x1de, y2, 0xe5);

            line::Line_(0x1d4, y1, 0x1de, y1, 0xe6);
            line::Line_(0x1d4, y1, 0x1d4, y2, 0xe6);
            line::Line_(0x1de, y1, 0x1de, y2, 0xe4);
            line::Line_(0x1d4, y2, 0x1de, y2, 0xe4);

            graphics::Dot_(0x1d4, 0x45, 0x50);
            graphics::Dot_(0x1de, 0x45, 0x50);
            graphics::Dot_(0x1d4, 0x194, 0x50);
            graphics::Dot_(0x1de, 0x194, 0x50);
        }
    }

    void __cdecl Draw_Ship2_(int16_t ship_idx, int16_t slot_idx) {
        int16_t y_offset = (slot_idx / 5) * 61;
        int16_t x_offset = (slot_idx % 5) * 60;

        COLONY::Remap_Draw_Ship_Centered_(x_offset + 188, y_offset + 77, ship_idx);

        int16_t planet_idx = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);
        int16_t colony_idx = MOX::_planet[planet_idx].colony_index;

        if (!COLCALC::Colony_Can_Build_Product_(&MOX::_colony[colony_idx], COLBLDG::Colony_Production_From_Ship_Index_(ship_idx))) {
            COLONY::Set_Colony_Font_To_Red_(1, 1);
            char* text = ESTRINGS::E_Strings_(0x18B);
            BILL::Squeeze_Paragraph_Centered_(x_offset + 161, y_offset + 49, 55, 56, text, 2);
            return;
        }

        if (MOX::_ship[ship_idx].d.previous_owner != MOX::_PLAYER_NUM) {
            COLONY::Set_Colony_Font_To_Red_(1, 1);
            char* text = ESTRINGS::E_Strings_(0xD8);
            BILL::Squeeze_Paragraph_Centered_(x_offset + 161, y_offset + 49, 55, 56, text, 2);
            return;
        }

        if (MOX::_ship[ship_idx].d.picture_num == 44) {
            COLONY::Set_Colony_Font_To_Red_(1, 1);
            char* text = ESTRINGS::E_Strings_(0x16A);
            BILL::Squeeze_Paragraph_Centered_(x_offset + 161, y_offset + 49, 55, 56, text, 2);
        }
    }

    void __cdecl Scan_Ship_Info_(int16_t ship_idx) {
        char buffer[10000];

        COLONY::Set_Colony_Font_To_(1);
        COLBLDG::Build_Ship_Info_Formatted_Paragraph_(&MOX::_ship[ship_idx], buffer);
        BILL::Squeeze_Print_Formatted_Paragraph_(168, 248, 283, 169, buffer, 0);
    }

    void __cdecl Draw_Ship_List_() {
        COLONY::Set_Colony_Font_To_(1);

        for (int16_t i = 0; i < 15; ++i) {
            int16_t ship_idx = _g_ship_list_ptr[_first_ship + i];
            if (ship_idx != -1 && COLONY::_full_draw != 0) {
                Draw_Ship2_(ship_idx, i);
            }
        }
    }

    void __cdecl Draw_Ship_D_List_() {
        char buffer[476];
        int16_t player_num = MOX::_PLAYER_NUM;

        for (int16_t design_idx = 0; design_idx < 5; ++design_idx) {
            int32_t font_color = 0;

            if (MOX::_player[player_num].ship_designs[design_idx].size == MOX::_ship[_g_ship_n].d.size) {
                if (_fields[5 + design_idx] == _lscanned_field) {
                    font_color = 1;
                }
                font_color++;
            }

            COLONY::Set_Colony_Font_To_Reg_Blue_(4, font_color);

            char* empty_str = ESTRINGS::E_Strings_(12);
            char* selection_name = COLBLDG::Selection_Name_(COLBLDG::Colony_Production_From_Ship_Design_Index_(design_idx));

            if (strcmp(selection_name, empty_str) != 0) {
                int32_t y = ((179 * (design_idx + 1)) / 8) + 49;

                char* name_to_print = COLBLDG::Selection_Name_(COLBLDG::Colony_Production_From_Ship_Design_Index_(design_idx));
                fonts::Print_(183, (int16_t)y, name_to_print);

                char* hull_name = TECHDATA::_hull_data[MOX::_player[player_num].ship_designs[design_idx].size].name;
                fonts::Print_Right_(457, (int16_t)y, hull_name);

                fonts::Print_Centered_(320, (int16_t)y, ":");
            }
        }

        int32_t bottom_font_color = 1;
        if (_lscanned_field == _fields[11]) {
            bottom_font_color = 2;
        }

        COLONY::Set_Colony_Font_To_Reg_Blue_(4, bottom_font_color);

        char* refit_str = ESTRINGS::E_Strings_(800);
        int32_t bottom_y = ((179 * 7) / 8) + 49;
        fonts::Print_Centered_(320, (int16_t)bottom_y, refit_str);

        char* ship_format = ESTRINGS::E_Strings_(801);
        snprintf(buffer, sizeof(buffer), ship_format, MOX::_ship[_g_ship_n].d.name);

        COLONY::Set_Colony_Font_To_Reg_Blue_(4, 1);
        fonts::Print_Centered_(320, 20, buffer);
    }

    int16_t __cdecl Evaluate2_Input_(int16_t* selected_field_id, int16_t* hovered_field_id) {
        char buffer[500];

        _scanned_ship_d_index = -1;
        _lscanned_field = -1000;

        for (int16_t design_idx = 0; design_idx < 5; design_idx++) {
            if (_fields[design_idx + 5] == *hovered_field_id) {
                _lscanned_field = *hovered_field_id;
                _scanned_ship_d_index = design_idx;
            }
            if (*selected_field_id == _fields[design_idx + 5]) {
                if (MOX::_player[MOX::_PLAYER_NUM].ship_designs[design_idx].size == MOX::_ship[_g_ship_n].d.size) {
                    struct_::Copy_Structure_(
                        &MOX::_player[MOX::_PLAYER_NUM].ship_designs[5],
                        &MOX::_player[MOX::_PLAYER_NUM].ship_designs[design_idx],
                        sizeof(s_ship_design)
                    );
                    MOX::_temp_star_handle = 5;
                    _esc = 1;
                    return -3;
                }
                GENDRAW::Help_(ESTRINGS::E_Strings_(0x323));
                _esc = 0;
            }
            if (*selected_field_id + _fields[design_idx + 5] == 0) {
                snprintf(
                    buffer,
                    sizeof(buffer),
                    ESTRINGS::E_Strings_(0x324),
                    &MOX::_ship[_g_ship_n],
                    &MOX::_player[MOX::_PLAYER_NUM].ship_designs[design_idx]
                );
                TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x325), buffer);
            }
        }

        if (_fields[11] == *hovered_field_id) {
            _lscanned_field = *hovered_field_id;
        }
        if (*selected_field_id == _fields[11]) {
            _esc = 1;
            return -2;
        }
        if (*selected_field_id + _fields[11] == 0) {
            TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x326), ESTRINGS::E_Strings_(0x327));
        }
        if (*selected_field_id == _fields[3]) {
            _esc = 1;
        }
        if (*selected_field_id + _fields[0] == 0) {
            TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x328), ESTRINGS::E_Strings_(0x329));
        }
        if (*selected_field_id + _fields[3] == 0) {
            TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x32a), ESTRINGS::E_Strings_(0x32b));
        }
        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            COLONY::_full_draw = 1;
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        }

        return -1;
    }

    void __cdecl Add2_Fields_() {
        _fields[3] = fields::Add_Button_Field_(
            0x112, 0x1B8, 
            ESTRINGS::E_Strings_(0xC), 
            _anims[3], 
            ESTRINGS::E_Strings_(0xC), 
            0x28
        );

        for (int16_t design_idx = 0; design_idx < 5; ++design_idx) {
            if (strcmp(ESTRINGS::E_Strings_(0xC), COLBLDG::Selection_Name_(COLBLDG::Colony_Production_From_Ship_Design_Index_(design_idx))) == 0) {
                _fields[5 + design_idx] = -1000;
            } else {
                _fields[5 + design_idx] = fields::Add_Hidden_Field_(
                    0xA1, 
                    ((design_idx + 1) * 179) / 8 + 49, 
                    0x1DE, 
                    ((design_idx + 2) * 179) / 8 + 48, 
                    ESTRINGS::E_Strings_(0xC), 
                    0x29
                );
            }
        }

        _fields[11] = fields::Add_Hidden_Field_(
            0xA1, 
            ((6 + 1) * 179) / 8 + 49, 
            0x1DE, 
            ((6 + 2) * 179) / 8 + 48, 
            ESTRINGS::E_Strings_(0xC), 
            0x29
        );

        _fields[0] = fields::Add_Hidden_Field_(
            0, 0, 0x27F, 0x1DF, 
            ESTRINGS::E_Strings_(0xC), 
            0
        );
    }

}