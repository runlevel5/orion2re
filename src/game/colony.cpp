#include "deps.h"
#include "harold.h"

namespace COLONY {
    void __cdecl Init_Colonies_() {
        return;
    }

    void __cdecl No_Op1_() {
        return;
    }

    char* __cdecl CR_Building_Name_(int16_t building_id) {
        if (building_id == BUILDING_ARTEMIS_SYSTEM_NET || building_id == BUILDING_DIMENSIONAL_PORTAL ||
            building_id == BUILDING_STAR_BASE || building_id == BUILDING_STAR_FORTRESS) {
            return ESTRINGS::E_Strings_(ESTR_HOUSING);
        }

        if (building_id == BUILDING_BATTLESTATION) {
            return ESTRINGS::E_Strings_(ESTR_RUBBLE);
        }

        return Real_Building_Name_(building_id);
    }

    char* __cdecl Real_Building_Name_(int16_t building_id) {
        return TECHDATA::_buildings[building_id].name;
    }

    s_animation_header* __cdecl Buffer_Reload_Planets_(int16_t entry_num) {
        return reinterpret_cast<s_animation_header*>(Buffer_Reload_(
            COLONY::_PLANETS_LBX,
            static_cast<int>(entry_num),
            MOX::_global_cache_seg
        ));
    }

    s_animation_header* __cdecl Buffer_Reload_Colony_(int16_t index) {
        return reinterpret_cast<s_animation_header*>(Buffer_Reload_(
            _COLONY_LBX,
            static_cast<int32_t>(index) + 9,
            MOX::_global_cache_seg
        ));
    }

    int16_t __cdecl Satellite_Anim_Pic_(int16_t id) {
        switch (id) {
        case BUILDING_ARTEMIS_SYSTEM_NET:
            return 7;
        case BUILDING_BATTLESTATION:
            return 1;
        case BUILDING_DIMENSIONAL_PORTAL:
            return 3;
        case BUILDING_STAR_BASE:
            return 2;
        case BUILDING_STAR_FORTRESS:
            return 0;
        default:
            return -1;
        }
    }

    void __cdecl Update_Colony_Palette_() {
        fonts::Load_Palette_(1, 0, 255);
        s_animation_header* colony_anims = C_Anims_(0);
        animate::Draw_Palette_(colony_anims);
        remap::Update_Glass_Remap_Colors_();
    }

    s_animation_header* __cdecl Spy_Anim_(int16_t race_index) {
        int16_t entry_index = race_index * 13 + 11;

        return reinterpret_cast<s_animation_header*>(Buffer_Reload_(
            _RACEICON_LBX,
            static_cast<int32_t>(entry_index),
            MOX::_global_cache_seg
        ));
    }

    bool __cdecl Sat_Should_Be_Replaced_(int16_t building_id) {
        // Size of s_colony is 361 bytes (0x169)
        const s_colony& colony = MOX::_colony[COLONY::_colony_handle];

        if (building_id == BUILDING_STAR_BASE) {
            // A Star Base is redundant if either a Battlestation or a Star Fortress exists.
            if (colony.buildings[BUILDING_BATTLESTATION] != 0 || 
                colony.buildings[BUILDING_STAR_FORTRESS] != 0) {
                return true;
            }
        } 
        else if (building_id == BUILDING_BATTLESTATION) {
            // A Battlestation is redundant if a Star Fortress exists.
            if (colony.buildings[BUILDING_STAR_FORTRESS] != 0) {
                return true;
            }
        }

        return false;
    }

    void __cdecl Insert_Satellite_Into_Array_(int16_t satellite_id, int16_t* count) {
        if (!Sat_Should_Be_Replaced_(satellite_id)) {
            _colony_satellites[*count] = satellite_id;
            (*count)++;
        }
    }

    void __cdecl Clear_Colony_Buidlings_() {
        memset(_colony_bldgs, 0, sizeof(_colony_bldgs));
    }

    void __cdecl Clear_Colony_Sattelites_() {
        memset(_colony_satellites, 0xff, sizeof(_colony_satellites));
    }

    void __cdecl Find_Replacement_Slot_For_Building_(int16_t bldg_id, int16_t* out_row, int16_t* out_col) {
        int16_t min_cost = 1000;

        if (bldg_id >= 0) {
            *out_row = 0;
            *out_col = 0;

            for (int16_t i = 0; i < 6; ++i) {
                for (int16_t j = 0; j < 6; ++j) {
                    int16_t current_bldg_id = static_cast<int16_t>(COLONY::_colony_bldgs[i].buildings[j]);

                    if (current_bldg_id == BUILDING_ARTEMIS_SYSTEM_NET || 
                        current_bldg_id == BUILDING_DIMENSIONAL_PORTAL || 
                        current_bldg_id == BUILDING_STAR_BASE || 
                        current_bldg_id == BUILDING_STAR_FORTRESS) {
                        min_cost = 0;
                        *out_row = i;
                        *out_col = j;
                    } else {
                        int32_t current_cost = TECHDATA::_buildings[current_bldg_id].cost;
                        
                        if (current_cost < static_cast<int32_t>(min_cost)) {
                            *out_row = i;
                            *out_col = j;
                            min_cost = static_cast<int16_t>(current_cost);
                        }
                    }
                }
            }
        }
    }

    void __cdecl Load_Bldg_Bitm_() {
        COLBLDG::_bldg_bitm = allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x3860);
        _bldg_bitm_n = -1;
    }

    void __cdecl Load_Ship_Bitm_() {
        _ship_bitm = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0xE30);
        _ship_pic_bitm = (s_animation_header*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0xE30);

        _ship_bitm_n = -1;
        _g_ship_pic_n = -1;
        _g_ship_player_n = -1;
    }

    void __cdecl Load_Galaxy_Map_Anims_() {
        int entry_num = 23;
        void* screen_seg = MOX::_screen_seg;

        for (int i = 0; i < 10; ++i) {
            MOX::_colony_galaxy_star_seg[i] = (s_animation_header*)farload::Far_Reload_Next_(s_gstar_lbx_00558544, entry_num, screen_seg);
            entry_num++;

            screen_seg = MOX::_screen_seg;
        }
    }

    void __cdecl Load_Planet_Screen_() {
        return;
    }

    s_animation_header* __cdecl Prod_Anims_(int16_t param_1, int16_t param_2, int8_t param_3) {
        int32_t index = (int32_t)param_1 + (int32_t)param_2 * 4;

        if (param_3 != 0) {
            index += 27;
        }

        return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, index, MOX::_global_cache_seg));
    }

    void __cdecl Reset_Mouse_Picture_() {
        mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
        fields::Enable_Draw_All_Fields_();
    }

    void __cdecl Clear_Mouse_Picture_() {
        bitmap::Clear_Mouse_Image_(0x13);
        mouse::Set_Mouse_List(MOX::_mouse_list_variable, 1);

        if (COLMOVE::_cluster_colony_n != -1) {
            int16_t first_unemployed = COLMOVE::Get_First_Unemployed_(COLMOVE::_cluster_colony_n);
            if (first_unemployed != -1) {
                fields::Disable_Draw_All_Fields_();
            }
        }
    }

    void __cdecl Remap_Draw_Ship_D_Centered_(int16_t x, int16_t y, int16_t design_index) {
        int16_t picture_num = static_cast<int16_t>(MOX::_player[MOX::_PLAYER_NUM].ship_designs[design_index].picture_num);

        Remap_Draw_Ship_Seg_N_Centered_(x, y, picture_num);
    }

    bool __cdecl Get_Bldg_CR_(int16_t building_id, int16_t* row_out, int16_t* col_out) {
        int16_t empty_count = 0;
        *row_out = 0;
        *col_out = 0;

        for (int16_t i = 0; i < 6; ++i) {
            for (int16_t j = 0; j < 6; ++j) {
                int32_t current_id = _colony_bldgs[i].buildings[j];

                if (current_id == (int32_t)building_id) {
                    *row_out = i;
                    *col_out = j;
                    return true;
                }

                if (current_id == 0) {
                    empty_count++;
                    if (random::Random_((int32_t)empty_count) == 1) {
                        *row_out = i;
                        *col_out = j;
                    }
                }
            }
        }

        return building_id == -1;
    }

    int16_t __cdecl Calculate_Colony_Turn_Count_From_Scrap_For_Prod_(int16_t colony_idx, int16_t product_id) {
        s_colony* colony = &MOX::_colony[colony_idx];
        int16_t remaining_cost = COLCALC::Colony_Product_Cost_(colony, product_id);
        int16_t net_production = colony->production[1];

        if (net_production >= remaining_cost) {
            return 1;
        }

        if (net_production <= 0) {
            return 10000;
        }

        if (net_production < 2) {
            net_production = 1;
        }

        return (remaining_cost / net_production) + 1;
    }

    /**
     * @address 004ddd30
     */
    int16_t __cdecl Calculate_Current_Production_Turn_Count_(int16_t colony_idx) {
        int16_t turns = COLCALC::Colony_N_Turns_To_Produce_(&MOX::_colony[colony_idx]);

        if (MOX::_colony[colony_idx].bought_outright != 0) {
            return 1;
        }

        if (turns == -1) {
            return 10000;
        }

        return turns;
    }

    void __cdecl Remap_Draw_Ship_Centered_(int16_t x, int16_t y, int16_t ship_index) {
        uint8_t palette_buffer[31];

        for (int i = 0; i < 31; ++i) {
            palette_buffer[i] = 0x51 + i;
        }

        s_ship_data* ship = &MOX::_ship[ship_index];
        int16_t picture_num = (int16_t)ship->d.picture_num;
        int16_t previous_owner = (int16_t)ship->d.previous_owner;

        if (picture_num != _ship_bitm_n || (int32_t)previous_owner != _g_ship_pic_n) {
            _ship_bitm_n = picture_num;
            _g_ship_pic_n = (int32_t)previous_owner;

            void* picture_seg = KEN::Get_Ship_Picture_Seg_(previous_owner, picture_num);
            bitmap::Draw_Picture_To_Bitmap_((s_animation_header*)_ship_bitm, picture_seg);

            replace::Replace_Color_Range_(_ship_bitm, palette_buffer, 1, 31);

            for (int16_t i = 0; i < 16; ++i) {
                uint8_t replacement_color = palette_buffer[i * 2];
                replace::Replace_Color_(_ship_bitm, i + 0xC0, (int32_t)replacement_color);
                replace::Replace_Color_(_ship_bitm, i + 0xD0, (int32_t)replacement_color);
            }
        }

        ERIC::Draw_Centered_(x, y, _ship_bitm);
    }

    int16_t __cdecl N_Houses_In_Bldg_Array_() {
        int16_t house_count = 0;

        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 6; ++j) {
                int32_t building_id = _colony_bldgs[i].buildings[j];

                if (building_id == BUILDING_ARTEMIS_SYSTEM_NET || 
                    building_id == BUILDING_DIMENSIONAL_PORTAL || 
                    building_id == BUILDING_STAR_BASE || 
                    building_id == BUILDING_STAR_FORTRESS) {
                    house_count++;
                }
            }
        }

        return house_count;
    }

    void __cdecl Bldg_Back_To_Front_(int16_t building_type, uint16_t* out_val1, uint16_t* out_val2) {
        const uint8_t lookup_table[72] = {
            5, 5, 5, 4, 4, 5, 5, 3, 3, 5, 4, 4, 5, 2, 2, 5, 4, 3,
            3, 4, 5, 1, 1, 5, 4, 2, 2, 4, 3, 3, 5, 0, 0, 5, 4, 1,
            1, 4, 2, 3, 3, 2, 4, 0, 0, 4, 3, 1, 1, 3, 2, 2, 3, 0,
            0, 3, 2, 1, 1, 2, 2, 0, 0, 2, 1, 1, 1, 0, 0, 1, 0, 0
        };

        if (out_val1) {
            *out_val1 = static_cast<uint16_t>(lookup_table[building_type * 2]);
        }

        if (out_val2) {
            *out_val2 = static_cast<uint16_t>(lookup_table[building_type * 2 + 1]);
        }
    }

    void __cdecl Allocate_Mini_Planet_Cache_() {
        return;
    }

    void __cdecl Clear_Galaxy_Map_Fields_() {
        _galaxy_map_name_field = -1000;
        MOX::_galaxy_map_grid_field = -1000;

        for (int i = 0; i < 72; ++i) {
            MOX::_galaxy_map_star_field[i] = -1000;
        }
    }

    void __cdecl Set_Colony_Font_To_Blue_(int16_t style, uint32_t is_bright) {
        fonts::Set_Outline_Color_(0x50, 0x50);

        s_colors* primary_colors;
        if (is_bright == 0) {
            primary_colors = &_font_color_array;
        } else {
            primary_colors = &_font_bright_color_array;
        }

        fonts::Set_Font_Style_Shadow_Down_(static_cast<int32_t>(is_bright), primary_colors);
        fonts::Set_Font_Highlight_Colors_(&_bright_blue_font_color_array);
        fonts::Set_Font_Special_Colors_(&_red_font_color_array[is_bright & 1]);
    }

    void __cdecl Set_Colony_Font_() {
        fonts::Set_Outline_Color_(0x50, 0x50);
        COLSUM::Set_Colony_Font_To_(3);
    }

    void __cdecl Clear_Sys_Disp_Fields_() {
        for (int i = 0; i < 5; ++i) {
            _sys_disp_planet_fields[i] = -1000;
        }

        for (int i = 0; i < 3; ++i) {
            _sys_disp_fields[i] = -1000;
        }
    }

    void __cdecl Reset_Field_Arrays_() {
        COLONY::Clear_Sys_Disp_Fields_();

        for (int i = 0; i < 5; ++i) {
            COLONY::_sys_disp_summ_fields[i] = -1000;
        }

        for (int i = 0; i < 21; ++i) {
            COLONY::_colony_fields[i] = -1000;
        }

        for (int i = 0; i < 2; ++i) {
            COLONY::_military_fields[i] = -1000;
        }

        for (int i = 0; i < 42; ++i) {
            COLONY::_pop_fields[i] = -1000;
        }

        for (int i = 0; i < 3; ++i) {
            COLONY::_job_fields[i] = -1000;
        }
    }


    bool __cdecl Colony_Can_Buy_Product_0_(int16_t colony_idx) {
        if (COLCALC::Colony_Can_Afford_To_Buy_Product_(colony_idx)) {
            s_colony& colony = MOX::_colony[colony_idx];

            if (colony.bought_outright == 0) {
                int16_t planet_idx = colony.planet;
                int8_t star_idx = MOX::_planet[planet_idx].star_index;
                s_star_data& star = MOX::_star[star_idx];

                if (!struct_::Test_Bit_Field_(&star.blockaded, MOX::_PLAYER_NUM)) {
                    int32_t cost = COLCALC::Colony_Cost_To_Buy_Producing_Product_(&colony);
                    if (cost > 0) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    void __cdecl Draw_Colony_Info_Background_() {
        if (_full_draw != 0) {
            graphics::Darken_Fill_(0, 0, 640, 150);

            int16_t unemployed_idx = COLMOVE::Get_First_Unemployed_(static_cast<int16_t>(_colony_handle));

            if (unemployed_idx == -1) {
                s_animation_header* anim = C_Anims_(3);
                animate::Draw_(0, 0, anim);
            } else {
                s_animation_header* anim = C_Anims_(4);
                animate::Draw_(0, 0, anim);
            }
        }
    }

    void __cdecl Draw_Colony_Satellites_() {
        if (HAROLD::Was_Mouse_List_Saved_()) {
            return;
        }

        for (int16_t i = 0; i < 10; ++i) {
            int16_t satellite_id = _colony_satellites[i];
            if (satellite_id == -1) {
                continue;
            }

            // The X coordinate oscillates around center (295) based on index parity
            // i=0: 295, i=1: 245, i=2: 395, i=3: 145, i=4: 495, etc.
            int16_t multiplier = (i % 2 == 0) ? 1 : -1;
            int16_t x = (multiplier * i * 50) + 295;
            int16_t y = 162; /* 0xA2 */

            if (satellite_id == _scanned_satellite && ERIC::Global_Cycler_(30 /* 0x1E */, 2) != 0) {
                Draw_Satellite_Dark_(x, y, satellite_id);
            } else {
                s_animation_header* anim = Satellite_Anim_(satellite_id);
                animate::Draw_(x, y, anim);
            }
        }
    }

    void __cdecl Box_Galaxy_Map_Star_(int16_t star_idx, int16_t param_2) {
        int16_t x;
        int16_t y;

        MOVEBOX::Get_Galaxy_Map_Star_XY_(
            star_idx,
            7,
            24,
            MOX::_galaxy_map_height,
            MOX::_galaxy_map_grid_y,
            11,
            31,
            0,
            &y,
            &x
        );

        ERIC::Pulsing_Box_(y, x, 5, 5, 1, 1, 2);
        timer::Release_Time_(2);
    }

    int __cdecl Draw_Colony_Info_Military_For_(int param_1, int param_2, int param_3) {
        return COLONY::Do_Colony_Info_Military_Stuff_For_(param_1, param_2, param_3, 0);
    }

    void __cdecl Draw_Colony_Info_Military_() {
        int last_result = 0;
        int counter = 0;
        int max_val = 0;
        int i = 0;

        do {
            last_result = Draw_Colony_Info_Military_For_(i, last_result, counter);
            if (max_val < last_result) {
                max_val = last_result;
                counter++;
            }
            i++;
        } while (i < 2);
    }

    void Draw_Colony_Info_Officer_() {
        int8_t officer_idx = MOX::_star[COLONY::_star_handle].officer_index[MOX::_PLAYER_NUM];

        if (officer_idx != -1) {
            MOX::_main_screen_seg = reinterpret_cast<s_animation_header*>(Buffer_Reload_(s_BUFFER0_LBX, 0, MOX::_global_cache_seg));
            remap::Calculate_Picture_Remap_Colors_(MOX::_main_screen_seg);

            s_animation_header* anim1 = C_Anims_(33);
            animate::Draw_(544, 351, anim1);

            s_animation_header* anim2 = C_Anims_(30);
            animate::Draw_(550, 357, anim2);

            COLSUM::Set_Colony_Font_To_(2);

            s_leader_data& leader = MOX::_leaders[officer_idx];

            if (leader.eta > 0) {
                char local_buffer[500];
                char* format = ESTRINGS::E_Strings_(285);
                _sprintf(local_buffer, format, (int)leader.eta);
                fonts::Print_Centered_(586, 433, local_buffer);
                return;
            } else {
                graphics::Darken_Fill_(550, 433, 623, 442);
                fonts::Print_Centered_(586, 433, leader.name);
            }
        }
    }

    void __cdecl Draw_Colony_Info_Production_() {
        Draw_Colony_Info_Production_For_(3);
        Draw_Colony_Info_Production_For_(0);
        Draw_Colony_Info_Production_For_(1);
        Draw_Colony_Info_Production_For_(2);
    }

    void __cdecl Draw_Colony_Info_Production_For_(int32_t selection) {
        int16_t v1;
        int16_t v2;

        if (selection == 3) {
            v1 = -1;
            v2 = -3;
        } else {
            v1 = (int16_t)selection;
            v2 = -1;
        }

        // Calculate y-coordinate: (v1 * 30) + v2 + 65
        int16_t y_offset = (v1 * 0x1e) + v2 + 0x41;

        COLDRAW::Draw_Colony_Prod_(
            (int16_t)_colony_handle,
            selection,
            128,
            y_offset,
            301,
            20
        );
    }

    void __cdecl Draw_Colony_Info_Pop_() {
        Draw_Colony_Info_Pop_For_(0);
        Draw_Colony_Info_Pop_For_(1);
        Draw_Colony_Info_Pop_For_(2);
        Draw_Colony_Info_Pop_Scanned_();
    }

    void __cdecl Draw_Current_Building_(int16_t building_id) {
        Draw_Current_Building2_(0x242, 0x94, building_id);
    }

    void __cdecl Draw_Current_Ship_(int16_t ship_id) {
        graphics::Set_Window_(517, 17, 639, 158);
        video::Clip_On_();
        Remap_Draw_Ship_Centered_(578, 77, ship_id);
        video::Clip_Off_();
    }

    void __cdecl Draw_Sys_Disp_Scanned_() {
        if (_drawing_display == 0) {
            COLSYSDI::Draw_Col_Sys_Disp_Last_Scanned_(7, 24);
        } else if (_drawing_display == 1) {
            if (_g_scanned_star != -1) {
                s_star_data* star = &MOX::_star[_g_scanned_star];
                Print_Sys_Disp_Scanned_String_(reinterpret_cast<char*>(star));

                Box_Galaxy_Map_Star_(_g_scanned_star, 1);
            }
        }
    }

    void __cdecl Print_Building_Name_(int16_t param_1, int16_t param_2) {
        if (param_1 != -1 && param_2 != -1) {
            int32_t building_id = _colony_bldgs[param_1].buildings[param_2];

            char* name = CR_Building_Name_(static_cast<int16_t>(building_id));
            Print_Scanned_String_(name);
        }
    }

    void __cdecl Print_Scanned_String_(char* text) {
        Set_Colony_Font_();

        int16_t width = BILL::_Get_Paragraph_Max_Width_(0x280, text, 0);
        int16_t half_width = width / 2;

        graphics::Darken_Fill_(0x135 - half_width, 0x1cf, half_width + 0x149, 0x1de);

        fonts::Print_Centered_(0x13f, 0x1d3, text);
    }

    void __cdecl Print_Scanned_Satellite_() {
        if (_scanned_satellite != -1) {
            char* building_name = Real_Building_Name_(_scanned_satellite);
            Print_Scanned_String_(building_name);
        }
    }

    void __cdecl Draw_Colony_Info_Scanned_() {
        Print_Scanned_Military_();
        Print_Scanned_Satellite_();
        Print_Building_Name_(_scanned_bldg_C, _scanned_bldg_R);
    }

    int16_t __cdecl Get_Colony_Planet_Id_(int32_t colony_idx) {
        if (colony_idx == -1) {
            return -1;
        }

        return MOX::_colony[colony_idx].planet;
    }

    void __cdecl Draw_Satellite_Dark_(int16_t x, int16_t y, int16_t satellite_id) {
        s_animation_header* satellite_anim = Satellite_Anim_(satellite_id);

        bitmap::Create_Blank_Bitmap_(120, 120, COLBLDG::_bldg_bitm);
        bitmap::Add_Picture_To_Bitmap_(0, 0, satellite_anim, (s_animation_header*)COLBLDG::_bldg_bitm);
        bitmap::Mask_Out_Colors_(COLBLDG::_bldg_bitm, 240, 255);
        bitmap::Tint_Bitmap_(COLBLDG::_bldg_bitm, COLBLDG::_bldg_bitm, 2);

        graphics::Set_Window_(0, 0, 639, 479);
        video::Clip_On_();
        animate::Draw_(x, y, (s_animation_header*)COLBLDG::_bldg_bitm);
        video::Clip_Off_();
    }

    void __cdecl Print_Scanned_Military_() {
        char buffer[500];

        if (COLONY::_scanned_military != (int16_t)-1) {
            // Units are indexed with +1 for scanned military data
            char* unit_name = TECHDATA::_units[COLONY::_scanned_military + 1].name;
            int unit_count = (int)MOX::_colony[COLONY::_colony_handle].military[COLONY::_scanned_military];

            char* format = ESTRINGS::E_Strings_(14 /* ESTR_MILITARY_COUNT_FORMAT */);
            _sprintf(buffer, format, unit_count, unit_name);

            COLONY::Print_Scanned_String_(buffer);
        }
    }

    char* __cdecl Job_String_(int16_t job_id) {
        if (job_id == 0) {
            return ESTRINGS::E_Strings_(ESTR_FARMER);
        }

        if (job_id == 1) {
            return ESTRINGS::E_Strings_(ESTR_WORKER);
        }

        return ESTRINGS::E_Strings_(ESTR_SCIENTIST);
    }

    /**
     * @address 004dc030
     */
    char* __cdecl Conquered_String_(bool is_conquered) {
        if (is_conquered) {
            return ESTRINGS::E_Strings_(ESTR_CONQUERED);
        }
        return ESTRINGS::E_Strings_(12);
    }

    char* __cdecl Pop_State_String_(int16_t state) {
        switch (state) {
        case 0:
            return ESTRINGS::E_Strings_(ESTR_EXPERT);
        case 2:
        case 3:
        case 4:
            return ESTRINGS::E_Strings_(12);
        default:
            // Case 1 and any value not explicitly handled above return the default string
            return ESTRINGS::E_Strings_(12);
        }
    }

    void __cdecl Draw_Colony_Info_Pop_Scanned_() {
        char buffer[500];

        if (COLONY::_scanned_pop != -1) {
            int16_t handle_s = (int16_t)COLONY::_colony_handle;
            uint32_t pop_data = MOX::_colony[COLONY::_colony_handle].pop[COLONY::_scanned_pop];

            char* str_state;
            char* str_conquered;
            char* str_race;
            char* str_job;
            char* format_str;

            // Branching logic based on language affects the order of components in the final string
            if (MOX::_settings.language == 4 /* Localized variant */) {
                str_state = Pop_State_String_(Pop_To_Pop_State_(handle_s, COLONY::_scanned_pop));
                str_conquered = Conquered_String_((bool)((pop_data >> 10) & 1));
                str_race = Pop_Race_String_(handle_s, COLONY::_scanned_pop);
                str_job = Job_String_((int16_t)((pop_data >> 7) & 3));

                format_str = ESTRINGS::E_Strings_(76);
                _sprintf(buffer, format_str, str_job, str_race, str_conquered, str_state);
            } else {
                str_job = Job_String_((int16_t)((pop_data >> 7) & 3));
                str_state = Pop_State_String_(Pop_To_Pop_State_(handle_s, COLONY::_scanned_pop));
                str_race = Pop_Race_String_(handle_s, COLONY::_scanned_pop);
                str_conquered = Conquered_String_((bool)((pop_data >> 10) & 1));

                format_str = ESTRINGS::E_Strings_(76);
                _sprintf(buffer, format_str, str_conquered, str_race, str_state, str_job);
            }

            Set_Colony_Font_();
            Print_Scanned_String_(buffer);
        }
    }

    char* __cdecl Pop_Race_String_(int16_t colony_idx, int16_t pop_idx) {
        // Each population unit's race index is stored in the lower 4 bits of its data
        uint32_t race_index = MOX::_colony[colony_idx].pop[pop_idx] & 0x0F;

        if (race_index == 8) {
            return ESTRINGS::E_Strings_(static_cast<int16_t>(0xB3));
        }

        if (race_index == 9) {
            return ESTRINGS::E_Strings_(static_cast<int16_t>(0x255));
        }

        return MOX::_player[race_index].race_name;
    }

    void __cdecl Draw_Colony_Info_Morale_() {
        COLDRAW::Draw_Info_Morale_(COLONY::_colony_handle, 310, 33, 510);
    }

    void __cdecl Draw_Current_Production_Graph_() {
        s_colony* colony = &MOX::_colony[COLONY::_colony_handle];

        int cost = COLCALC::Colony_Producing_Product_Cost_(colony);
        int spent = (int)colony->production_spent;

        if (colony->bought_outright != 0) {
            spent = cost;
        }

        COLDRAW::Draw_Colony_PC_Bar_(606, 43, cost, spent);
    }

    void __cdecl Draw_Turn_Count_() {
        char buffer[500];
        int16_t turns;
        char* format;

        turns = Calculate_Current_Production_Turn_Count_(static_cast<int16_t>(_colony_handle));

        fonts::Set_Font_Style_Shadow_Down_(0, &_font_color_array);

        format = ESTRINGS::E_Strings_(ESTR_D_TURNS);

        _sprintf(buffer, format, static_cast<int32_t>(turns));

        fonts::Print_Right_(624, 103, buffer);
    }

    void __cdecl Do_Informational_And_Decision_Popups_() {
        char buffer[500];
        s_colony* colony = &MOX::_colony[COLONY::_colony_handle];

        if (colony->occupation_policy == 4) {
            int16_t population_count = Sum_Colonists_(static_cast<int16_t>(COLONY::_colony_handle), -1, -1, -1, -1, 1);
            if (population_count < 1) {
                if (!Colony_Has_Natives_(static_cast<int16_t>(COLONY::_colony_handle))) {
                    goto check_production;
                }
            }
            OCPOLPUP::Occupation_Policy_Popup_();
            Col_Calc_Wrapper_(static_cast<int16_t>(COLONY::_colony_handle));
            Show_Colony_Screen_();
        }

    check_production:
        int16_t production_id = colony->just_produced;
        if (production_id == -1) {
            return;
        }

        if (production_id == -39) {
            char* planet_name = HACCESS::Get_Planet_Name_(colony->planet);
            char* format = ESTRINGS::E_Strings_(ESTR_S_JUST_COLONIZED_PLACE_YOUR_ORDERS_FOR_FUTURE_PRODUCTION);
            _sprintf(buffer, format, planet_name);
            GENDRAW::Help_(buffer);
            Show_Colony_Screen_();
            colony->just_produced = -1;
            return;
        }

        if (production_id == -40) {
            char* planet_name = HACCESS::Get_Planet_Name_(colony->planet);
            char* format = ESTRINGS::E_Strings_(ESTR_S_JUST_CONQUERED_PLACE_YOUR_ORDERS_FOR_FUTURE_PRODUCTION);
            _sprintf(buffer, format, planet_name);
            GENDRAW::Help_(buffer);
            Show_Colony_Screen_();
            colony->just_produced = -1;
            return;
        }

        if (production_id == -41) {
            char* planet_name = HACCESS::Get_Planet_Name_(colony->planet);
            char* format = ESTRINGS::E_Strings_(ESTR_S_JUST_LIBERATED_PLACE_YOUR_ORDERS_FOR_FUTURE_PRODUCTION);
            _sprintf(buffer, format, planet_name);
            GENDRAW::Help_(buffer);
            Show_Colony_Screen_();
            colony->just_produced = -1;
            return;
        }

        char* selection_name = COLBLDG::Selection_Name_(production_id);
        char* planet_name = HACCESS::Get_Planet_Name_(colony->planet);
        char* format = ESTRINGS::E_Strings_(ESTR_S_FINISHED_BUILDING_S_PLACE_YOUR_ORDERS_FOR_FUTURE_PRODUCTION);
        _sprintf(buffer, format, planet_name, selection_name);
        GENDRAW::Help_(buffer);
        Show_Colony_Screen_();
        colony->just_produced = -1;
    }

    void __cdecl Fix_Colony_Queue_(int16_t colony_idx) {
        Defragment_Colony_Queue_(colony_idx);

        while (MOX::_colony[colony_idx].producing[0] == -10) {
            Squish_Out_Colony_Queue_Slot_(colony_idx, 0);
        }
    }

    void __cdecl Defragment_Colony_Queue_(int16_t colony_idx) {
        for (int16_t i = 0; i < 7; ++i) {
            // BUG: if multiple slots are -1, some might be skipped due to shifting
            if (MOX::_colony[colony_idx].producing[i] == -1) {
                COLONY::Squish_Out_Colony_Queue_Slot_(colony_idx, i);
            }
        }
    }

    void __cdecl Cleanup_Colony_Queue_(int16_t colony_idx) {
        COLONY::Fix_Colony_Queue_(colony_idx);

        if (MOX::_colony[colony_idx].producing[0] == -1) {
            MOX::_colony[colony_idx].producing[0] = -2;
        }
    }

    void __cdecl Squish_Out_Colony_Queue_Slot_(int16_t colony_idx, int16_t slot_idx) {
        if (slot_idx < 6) {
            for (int i = (int)slot_idx; i < 6; i++) {
                MOX::_colony[colony_idx].producing[i] = MOX::_colony[colony_idx].producing[i + 1];
            }
        }
        MOX::_colony[colony_idx].producing[6] = -1;
    }

    void __cdecl Col_Calc_Wrapper_(int16_t colony_idx) {
        COLCALC::Colony_Calculation_(colony_idx);

        if (COLONY::_move_colonist != -1 && COLONY::_field_mode == 1) {
            s_colony* colony = &MOX::_colony[colony_idx];

            if (colony->n_pops != 0) {
                // Iterates through colony pops to find the last one without flag 0x200
                for (int16_t i = 0; i < (int16_t)(uint8_t)colony->n_pops; ++i) {
                    if ((colony->pop[i] & 0x200 /* FLAG_COLONIST_MOVING */) == 0) {
                        COLONY::_move_colonist = i;
                    }
                }
            }
        }
    }

    void __cdecl Reset_Field_Mode_() {
        int16_t unemployed_index = COLMOVE::Get_First_Unemployed_(static_cast<int16_t>(COLONY::_colony_handle));

        if (unemployed_index != -1) {
            if (COLONY::_field_mode == 1) {
                COLMOVE::Clear_Cluster_();
            }
            else if (COLONY::_field_mode == 2) {
                COLONY::Drop_Military_();
            }

            COLONY::Col_Calc_Wrapper_(static_cast<int16_t>(COLONY::_colony_handle));
        }

        COLONY::_field_mode = 0;
    }

    void __cdecl Drop_Military_() {
        char* help_text;

        help_text = ESTRINGS::E_Strings_(ESTR_NO_SHIPS_IN_THIS_HALF_FINISHED_GAME_AND_NO_INTERFACE_FOR_PUTTING_THIS_GUY_ON_ONE);
        GENDRAW::Help_(help_text);

        Show_Colony_Screen_();

        MOX::_colony[COLONY::_colony_handle].military[COLONY::_move_colonist]++;

        Clear_Move_Military_();
        Col_Calc_Wrapper_(static_cast<int16_t>(COLONY::_colony_handle));
        Reset_Field_Mode_();
    }

    char* __cdecl Colony_Type_(int32_t colony_idx) {
        int8_t specialty = MOX::_colony[colony_idx].specialty;

        switch (specialty) {
            case -1:
                return ESTRINGS::E_Strings_(12);

            case 0:
                return ESTRINGS::E_Strings_(ESTR_AGRICULTURAL);

            case 1:
                return ESTRINGS::E_Strings_(ESTR_INDUSTRIAL);

            case 2:
                return ESTRINGS::E_Strings_(ESTR_RESEARCH);

            default:
                return ESTRINGS::E_Strings_(ESTR_RESEARCH);
        }
    }

    void __cdecl Set_Colony_Font_To_Red_(int16_t style, int16_t color_idx) {
        fonts::Set_Outline_Color_(0x50, 0x50);

        s_colors* colors_ptr = &_red_font_color_array[color_idx];

        fonts::Set_Font_Style_Shadow_Down_(style, colors_ptr);
        fonts::Set_Font_Highlight_Colors_(&_bright_blue_font_color_array);
        fonts::Set_Font_Special_Colors_(colors_ptr);
    }

    /**
     * @address 004f54d0
     */
    void __cdecl Show_BC_Production_(int16_t colony_idx) {
        char local_buffer[3000];

        COLCALC::Colony_BC_Production_(&MOX::_colony[colony_idx], local_buffer);

        TEXTBOX::Text_Box_(COLCALC::_bc_summary_msg, local_buffer);
    }

    void __cdecl Show_Research_Production_(int32_t colony_handle) {
        char buffer[3000];

        COLCALC::Colony_Research_Production_(&MOX::_colony[colony_handle], buffer);

        TEXTBOX::Text_Box_(COLCALC::_research_summary_msg, buffer);
    }

    void __cdecl Show_Morale_(int16_t colony_idx) {
        char buffer[2000];

        s_colony* colony = &MOX::_colony[colony_idx];

        COLCALC::Colony_Morale_(colony, buffer);

        TEXTBOX::Text_Box_(COLCALC::_morale_summary_msg, buffer);
    }

    void __cdecl Remap_Draw_Ship_Seg_N_Centered_(int16_t param_1, int16_t param_2, int16_t param_3) {
        uint8_t local_20[32];

        // Manual assembly initialization of local buffer
        memcpy(local_20, "QRSTUVWXYZ[\\]^_`abcdefghijklmno", 31);

        if (COLONY::_ship_bitm_n != param_3) {
            COLONY::_ship_bitm_n = param_3;
            COLONY::_g_ship_player_n = (int32_t)MOX::_PLAYER_NUM;

            s_animation_header* ship_seg = KEN::Get_Ship_Picture_Seg_(MOX::_PLAYER_NUM, param_3);
            bitmap::Draw_Picture_To_Bitmap_(COLONY::_ship_bitm, ship_seg);

            // remap starting from local_20[8] which is 'Y'
            replace::Replace_Color_Range_(COLONY::_ship_bitm, &local_20[8], 1, 31 /* 0x1f */);

            for (int16_t i = 0; i < 16; ++i) {
                uint8_t remap_color = local_20[i * 2];
                replace::Replace_Color_(COLONY::_ship_bitm, i + 0xc0, remap_color);
                replace::Replace_Color_(COLONY::_ship_bitm, i + 0xd0, remap_color);
            }
        }

        ERIC::Draw_Centered_(param_1, param_2, COLONY::_ship_bitm);
    }

    short __cdecl Pop_To_Pop_State_(short colony_idx, short pop_idx) {
        // Extract the lower nibble of the population data to determine status
        uint32_t pop_status = MOX::_colony[colony_idx].pop[pop_idx] & 0xf;

        if (pop_status == 9) {
            return 3;
        }

        // Logic based on assembly: if status is 8, return 4, otherwise return 2
        // Assembly uses SUB EAX, 8; CMP EAX, 1; SBB EAX, EAX; which evaluates to CF=1 only if EAX=0 (status=8)
        if (pop_status == 8) {
            return 4;
        }

        return 2;
    }

    int16_t __cdecl Get_Effective_Pop_Player_(int16_t colony_idx, int16_t pop_idx) {
        uint32_t pop_val = MOX::_colony[colony_idx].pop[pop_idx];
        uint32_t pop_type = pop_val & 0x0F;

        if (pop_type == 8 || pop_type == 9) {
            return static_cast<int16_t>(MOX::_colony[colony_idx].owner);
        }

        return static_cast<int16_t>(pop_type);
    }

    s_animation_header* __cdecl Colony_Pop_Anim_(int16_t colony_idx, int16_t pop_idx) {
        uint32_t pop_data = MOX::_colony[colony_idx].pop[pop_idx];
        int16_t pop_state = static_cast<int16_t>(Pop_To_Pop_State_(colony_idx, pop_idx));

        uint32_t type = (pop_data >> 7) & 0x3;

        int16_t player_idx = Get_Effective_Pop_Player_(colony_idx, pop_idx);
        uint32_t race = static_cast<uint32_t>(static_cast<uint8_t>(MOX::_player[player_idx].race));

        // Check for specific flag in pop data (bit 10 / 0x400)
        if ((pop_data & 0x400) != 0) {
            return Colony_Pop_Icon_(race);
        }

        return People_Anim_(static_cast<int16_t>(type), pop_state, static_cast<int16_t>(race));
    }

    s_animation_header* __cdecl Colony_Pop_Icon_(int16_t race_index) {
        // Entry calculation based on race index: each race has a set of icons starting at offset 12 with stride 13
        int16_t entry_index = (int16_t)(race_index * 13 + 12);

        return reinterpret_cast<s_animation_header*>(Buffer_Reload_(COLONY::_RACEICON_LBX, (int)entry_index, MOX::_global_cache_seg));
    }

    void __cdecl Print_Sys_Disp_Scanned_String_(char* text) {
        Set_Colony_Font_();
        fonts::Print_Centered_(61, 135, text);
    }

    s_animation_header* __cdecl Military_Anims_(int16_t param_1, int16_t param_2) {
        // Calculation is done in 16-bit registers in disassembly
        int16_t entry_index = (int16_t)(param_2 * 13 + param_1 + 6);

        return reinterpret_cast<s_animation_header*>(Buffer_Reload_(
            COLONY::_RACEICON_LBX,
            (int)entry_index,
            MOX::_global_cache_seg
        ));
    }

    s_animation_header* __cdecl Military_Anim_(int16_t animation_type, int16_t param_2) {
        if (animation_type == -1) {
            return COLONY::Military_Anims_(0, param_2);
        }

        if (animation_type == 0) {
            // Check technology application at index 0x90 (144)
            if (MOX::_player[MOX::_PLAYER_NUM].tech_applications[0x90] == 3) {
                return COLONY::Military_Anims_(2, param_2);
            }
            return COLONY::Military_Anims_(1, param_2);
        }

        if (animation_type == 1) {
            // Check technology application at index 0x18 (24)
            if (MOX::_player[MOX::_PLAYER_NUM].tech_applications[0x18] == 3) {
                return COLONY::Military_Anims_(4, param_2);
            }
            return COLONY::Military_Anims_(3, param_2);
        }

        // Fallthrough for other animation types
        return COLONY::Military_Anims_(animation_type, param_2);
    }

    int __cdecl Do_Colony_Info_Military_Stuff_For_(int military_index, int start_n, int param_3, int mode) {
        int military_count;
        int owner;
        int local_n;
        char buffer[256];
        s_animation_header* anim;
        int16_t height;

        military_count = MOX::_colony[_colony_handle].military[military_index];
        owner = MOX::_colony[_colony_handle].owner;
        _step_squish = 0;

        if (mode != 2) {
            Do_Colony_Info_Military_Stuff_For_(military_index, start_n, param_3, 2);
        }

        Set_Colony_Font_();

        if (_step_squish == 0) {
            local_n = 0;
            if (military_count > 0) {
                do {
                    if (mode == 0) {
                        int race = MOX::_player[owner].race;
                        anim = Military_Anim_(static_cast<int16_t>(military_index), static_cast<int16_t>(race));
                        height = animate::Get_Height_(anim);
                        animate::Draw_(static_cast<int16_t>((local_n + start_n) * (30 - _step_squish)), static_cast<int16_t>(0x1df - height), anim);
                    }
                    local_n++;
                } while (local_n < military_count);
            }
        } else {
            _step_squish = 0;
            local_n = 0;
            if (military_count > 0) {
                int race = MOX::_player[owner].race;
                anim = Military_Anim_(static_cast<int16_t>(military_index), static_cast<int16_t>(race));
                height = animate::Get_Height_(anim);

                animate::Draw_(static_cast<int16_t>(start_n * 30), static_cast<int16_t>(0x1df - height), anim);

                _sprintf(buffer, ESTRINGS::E_Strings_(0x7c /* "x %d" */), military_count);
                Set_Colony_Font_To_(3);

                anim = Military_Anim_(static_cast<int16_t>(military_index), static_cast<int16_t>(race));
                height = animate::Get_Height_(anim);

                fonts::Print_(static_cast<int16_t>((start_n + 1) * (30 - _step_squish)), static_cast<int16_t>(0x1df - height), buffer);
                local_n = 3;
            }
        }

        if (mode == 1) {
            if (local_n > 0) {
                int step = 30 - _step_squish;
                int race = MOX::_player[owner].race;
                anim = Military_Anim_(static_cast<int16_t>(military_index), static_cast<int16_t>(race));
                height = animate::Get_Height_(anim);

                int x1 = start_n * step;
                int y1 = 0x1df - height;
                int x2 = (local_n + start_n) * step - 1;
                int y2 = 0x1df;

                _military_fields[military_index] = fields::Add_Hidden_Field_(static_cast<int16_t>(x1), static_cast<int16_t>(y1), static_cast<int16_t>(x2), static_cast<int16_t>(y2), ESTRINGS::E_Strings_(0xc), 0);
            } else {
                _military_fields[military_index] = -1000;
            }
        } else if (mode == 2) {
            int other_index = (military_index + 1) % 2;
            int total_military = military_count + MOX::_colony[_colony_handle].military[other_index];

            if (total_military < 2) {
                total_military = 1;
            }

            int step = 260 / total_military;
            if (step < 1) {
                step = 1;
            }

            if (step < 30) {
                _step_squish = static_cast<short>(30 - step);
            } else {
                _step_squish = 0;
            }
        }

        return start_n + local_n;
    }

    void __cdecl Draw_Colony_Info_Pop_For_(int32_t type) {
        // Vertical offset calculation: 0x3E (62) is base Y, 0x1E (30) is row height.
        int16_t y_coord = static_cast<int16_t>(type * 30 + 62);

        COLDRAW::Do_Colony_Info_Pop_Stuff_For_Pop_(
            static_cast<int16_t>(COLONY::_colony_handle),
            static_cast<int16_t>(type),
            0,
            0x136 /* 310 */,
            y_coord,
            0x1FE /* 510 */,
            1,
            COLONY::_pop_fields_ref[type],
            &COLONY::_pop_fields_count[type],
            &COLONY::_pop_fields_coord_var[type],
            &COLONY::_job_fields[type]
        );
    }

    int8_t __cdecl Evaluate_Input_(int16_t* param_1, int16_t* param_2) {
        uint32_t system_fields = Check_System_Display_Fields_(param_1, param_2);

        // Mask: 0xFFFF00FF clears bits 8-15 (AH register)
        uint32_t masked_fields = system_fields & 0xFFFF00FF;
        int8_t result = (int8_t)masked_fields;

        // TEST AX, AX checks bits 0-15 of EAX. Since bits 8-15 are 0, this checks AL.
        if (COLONY::_drawing_display != 2 && (uint16_t)masked_fields != 0) {
            result = Evaluate_Screen_Input_(param_1);
            Check_Job_Fields_(param_1, param_2);
            Check_Military_Fields_(param_1, param_2);
            Check_Satellite_Fields_(param_1, param_2);
            Check_Bldg_Fields_(param_1, param_2);
        }

        return result;
    }

    s_animation_header* __cdecl Mini_Planet_Anim_(int16_t planet_idx) {
        s_planet_data* planet = &MOX::_planet[planet_idx];
        uint8_t climate;

        if (planet->colony_index == -1) {
            climate = (uint8_t)planet->climate;
        } else {
            climate = (uint8_t)MOX::_colony[planet->colony_index].climate;
        }

        int32_t anim_index = static_cast<int32_t>(planet->size) + static_cast<int32_t>(climate) * 5;

        char* resource_name = ESTRINGS::E_Strings_(ESTR_CMBTPLNTLBX);

        return reinterpret_cast<s_animation_header*>(Buffer_Reload_(
            resource_name,
            anim_index,
            MOX::_global_cache_seg
        ));
    }

    bool __cdecl Colony_Has_Natives_(int16_t colony_idx) {
        int8_t n_pops = MOX::_colony[colony_idx].n_pops;

        if (n_pops != 0) {
            for (int16_t i = 0; i < (int16_t)n_pops; i++) {
                if ((MOX::_colony[colony_idx].pop[i] & 0x0F) == 8) {
                    return true;
                }
            }
        }

        return false;
    }

    void __cdecl Update_Fields_() {
        fields::Clear_Fields_();
        FAKEBUTS::Clear_Fake_Buttons_();
        COLONY::Clear_Sys_Disp_Fields_();
        COLONY::Clear_Galaxy_Map_Fields_();

        if (COLONY::_valid_colony != 0) {
            COLONY::Add_Job_Fields_();
            COLONY::Add_Military_Fields_();
        }

        if (COLONY::_drawing_display == 0) {
            COLSYSDI::Add_Col_Sys_Disp_Fields_(7, 24, 115);
        } else if (COLONY::_drawing_display == 1) {
            MOVEBOX::Add_Galaxy_Map_Fields_2_(
                7,
                24,
                MOX::_galaxy_map_height,
                MOX::_galaxy_map_grid_y,
                11,
                31,
                0,
                0,
                0,
                &COLONY::_galaxy_map_name_field,
                MOX::_galaxy_map_star_field,
                &MOX::_galaxy_map_grid_field
            );
        }

        COLONY::Add_Screen_Fields_();
    }

    int16_t __cdecl Get_Selected_Pop_(int16_t param_1) {
        int32_t index = (int32_t)param_1;

        // Note: The assembly calculates (short)(param_1 * 30 + 62) as the 5th argument.
        return COLDRAW::Do_Colony_Info_Pop_Stuff_For_Pop_(
            static_cast<int16_t>(_colony_handle),
            param_1,
            3,
            310,
            static_cast<int16_t>(param_1 * 30 + 62),
            510,
            1,
            _pop_fields_ref[index],
            &_pop_fields_count[index],
            &_pop_fields_coord_var[index],
            &_job_fields[index]
        );
    }

    void __cdecl Add_Job_Fields_() {
        uint8_t mode = _field_mode;
        for (int16_t i = 0; i < 3; ++i) {
            if (mode == 0) {
                Add_Job_Field_For_(i);
                mode = _field_mode;
            } else if (mode == 1) {
                Add_Move_Colonist_Job_Field_For_(i);
                mode = _field_mode;
            }
        }
    }

    void __cdecl Check_Job_Fields_(int16_t* p_clicked_id, int16_t* p_target_id) {
        Set_Colony_Font_();
        _scanned_pop = -1;

        if (_field_mode == 0) {
            for (int16_t i = 0; i < 3; ++i) {
                if (*p_clicked_id == _job_fields[i]) {
                    int16_t pop_index = Get_Selected_Pop_(i);
                    if (pop_index != -1) {
                        Pickup_Colonists_(pop_index);
                        *p_clicked_id = 0;
                    }
                }

                if (_job_fields[i] == *p_target_id) {
                    _scanned_pop = Get_Scanned_Pop_(i);
                }
            }
        } else if (_field_mode == 1) {
            for (int16_t i = 0; i < 3; ++i) {
                if (_job_fields[i] == *p_clicked_id) {
                    COLMOVE::Send_Cluster_((int16_t)_colony_handle, i);
                    Show_Colony_Screen_();
                    *p_clicked_id = 0;
                }
            }
        }
    }

    void __cdecl Add_Military_Fields_() {
        int prev_ret = 0;
        int count = 0;
        int max_val = 0;

        for (int i = 0; i < 2; i++) {
            int current_ret = COLONY::Add_Military_Field_For_(i, prev_ret, count);

            if (current_ret > max_val) {
                max_val = current_ret;
                count++;
            }

            prev_ret = current_ret;
        }
    }

    void __cdecl Set_Colony_Screen_To_New_Colony_(int16_t colony_idx) {
        Cleanup_Colony_Queue_(static_cast<int16_t>(_colony_handle));

        int16_t planet_idx = MOX::_colony[colony_idx].planet;

        (&MOX::_screen_data)[0] = static_cast<int16_t>(MOX::_planet[planet_idx].star_index);
        (&MOX::_screen_data)[1] = static_cast<int16_t>(MOX::_planet[planet_idx].orbit);
        (&MOX::_screen_data)[2] = colony_idx;

        Reset_Field_Mode_();
        fields::Clear_Fields_();
        FAKEBUTS::Clear_Fake_Buttons_();
        fields::Deactivate_Auto_Function_();

        _full_draw = 1;
        Set_Last_Colony_Selected_(colony_idx, MOX::_PLAYER_NUM);
    }

    void __cdecl Clear_Move_Military_() {
        _move_colonist = -1;
        Reset_Mouse_Picture_();
        fields::Enable_Draw_All_Fields_();
    }

    void __cdecl Check_Satellite_Fields_(int16_t* param_1, int16_t* param_2) {
        char local_buffer[500];

        _scanned_satellite = -1;

        for (int16_t i = 0; i < 10; ++i) {
            if (_colony_satellite_fields[i] == *param_1) {
                int16_t building_idx = _colony_satellites[i];

                if (MOX::_colony[_colony_handle].last_turn_building_destroyed == (short)MOX::_stardate) {
                    char* help_text = ESTRINGS::E_Strings_(0x243);
                    GENDRAW::Help_(help_text);
                    Show_Colony_Screen_();
                } else {
                    int32_t scrap_value = TECHDATA::_buildings[building_idx].cost / 2;
                    char* bld_name = Real_Building_Name_(building_idx);
                    char* fmt = ESTRINGS::E_Strings_(0x107);

                    _sprintf(local_buffer, fmt, bld_name, scrap_value);

                    if (GENDRAW::Warning_Box_(local_buffer) != 0) {
                        MOX::_colony[_colony_handle].last_turn_building_destroyed = (short)MOX::_stardate;
                        ERICNET::Remove_Building_(static_cast<int16_t>(_colony_handle), building_idx);
                        Remove_Satellite_From_Array_(building_idx);
                        COLCALC::Colony_Calculation_(static_cast<int16_t>(_colony_handle));
                        MOX::_player[MOX::_PLAYER_NUM].bc += scrap_value;
                        Show_Colony_Screen_();
                    } else {
                        Show_Colony_Screen_();
                    }
                }
            } else if (_colony_satellite_fields[i] + *param_1 == 0) {
                int16_t building_idx = _colony_satellites[i];
                Get_App_Description_String_(local_buffer, TECHDATA::_buildings[building_idx].tech_app);
                char* bld_name = Real_Building_Name_(building_idx);
                TEXTBOX::Text_Box_(bld_name, local_buffer);
            }

            if (_colony_satellite_fields[i] == *param_2) {
                _scanned_satellite = _colony_satellites[i];
            }
        }
    }

    void __cdecl Pickup_Colonists_(int16_t colonists) {
        _field_mode = 1;
        COLMOVE::Get_Cluster_(static_cast<int16_t>(_colony_handle), colonists);
        _full_draw = 1;
        Col_Calc_Wrapper_(static_cast<int16_t>(_colony_handle));
    }

    /**
     * @brief Finds the next or previous colony belonging to the same owner that is not an outpost.
     * @param current_idx The starting colony index.
     * @param direction If 0, searches forward (next); otherwise, searches backward (previous).
     * @return The index of the next/previous matching colony, or the original index if no others exist.
     */
    short __cdecl Get_Next_Colony_(short current_idx, short direction) {
        int8_t target_owner = MOX::_colony[current_idx].owner;
        short search_idx = current_idx;

        do {
            if (direction == 0) {
                // Next colony logic: (current + 1) % _NUM_COLONIES
                search_idx = (short)((search_idx + 1) % (int)(int16_t)MOX::_NUM_COLONIES);
            }
            else {
                // Previous colony logic
                search_idx--;
                if (search_idx < 0) {
                    // Wrap around to the end of the list
                    // BUG: This should likely be MOX::_NUM_COLONIES - 1
                    search_idx = (short)MOX::_NUM_COLONIES;
                }
            }

            // Check if colony matches owner and is not an outpost (outpost_flag == 0)
            s_colony* col = &MOX::_colony[search_idx];
            if (col->owner == target_owner && col->outpost_flag == 0) {
                return search_idx;
            }

        } while (search_idx != current_idx);

        return current_idx;
    }

}