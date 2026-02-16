#include "deps.h"
#include "harold.h"

namespace COLONY {
    void __cdecl Init_Colonies_() {
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

    void __cdecl Make_Bldg_Array_For_Colony_(int16_t colony_idx, uint8_t is_ui_view) {
        uint32_t previous_seed = random::Get_Random_Seed_();
        random::Set_Random_Seed_(static_cast<uint32_t>(colony_idx));

        int16_t satellite_count = 0;
        Clear_Colony_Buidlings_();
        Clear_Colony_Sattelites_();

        int bldg_count = 0;
        for (int16_t bldg_id = 0; bldg_id < 49; ++bldg_id) {
            if (MOX::_colony[colony_idx].buildings[bldg_id] != 0) {
                if (is_ui_view == 0 || bldg_count < 18) {
                    if (TECHDATA::_buildings[bldg_id].type == 7 /* Satellite */) {
                        Insert_Satellite_Into_Array_(bldg_id, &satellite_count);
                    } else {
                        Insert_Bldg_Into_Array_(colony_idx, bldg_id, bldg_count, is_ui_view);
                        bldg_count++;
                    }
                }
            }
        }

        int housing_to_add = (static_cast<uint8_t>(MOX::_colony[colony_idx].n_pops) / 3) + 1;
        for (int h = 0; h < housing_to_add; ++h) {
            if (bldg_count < 36) {
                if (is_ui_view == 0 || (is_ui_view == 1 && bldg_count < 18)) {
                    Insert_Bldg_Into_Array_(colony_idx, -3 /* ECON_HOUSING */, bldg_count, is_ui_view);
                    bldg_count++;
                }
            }
        }

        Sort_Bldg_Array_Columns_();

        if (is_ui_view == 0) {
            BUILDING target_jitter_ids[4] = {
                BUILDING_ARTEMIS_SYSTEM_NET,
                BUILDING_DIMENSIONAL_PORTAL,
                BUILDING_STAR_BASE,
                BUILDING_STAR_FORTRESS
            };
            for (int k = 0; k < 8; ++k) {
                int16_t bldg_id = target_jitter_ids[k % 4];
                int16_t row, col;
                if (Get_Bldg_CR_(bldg_id, &row, &col)) {
                    bool swapped = false;
                    for (int16_t si = 1; si >= -1 && !swapped; --si) {
                        for (int16_t di = 1; di >= -1 && !swapped; --di) {
                            int tr = row - si;
                            if (tr < 0) tr = 0;
                            if (tr > 5) tr = 5;
                            int tc = tr; 
                            bool should_swap = false;
                            if (COLONY::_colony_bldgs[tr].buildings[tc] == 0) {
                                if (random::Random_(3) == 1) should_swap = true;
                            } else {
                                if (random::Random_(3) == 1) {
                                    should_swap = true;
                                } else if (random::Random_(3) == 1) {
                                    should_swap = true;
                                }
                            }
                            if (should_swap) {
                                int32_t temp_id = COLONY::_colony_bldgs[tr].buildings[tc];
                                COLONY::_colony_bldgs[tr].buildings[tc] = COLONY::_colony_bldgs[row].buildings[col];
                                COLONY::_colony_bldgs[row].buildings[col] = temp_id;
                                swapped = true;
                            }
                        }
                    }
                }
            }
            int16_t sp_row, sp_col;
            if (Get_Bldg_CR_(BUILDING_CAPITOL, &sp_row, &sp_col)) {
                if (COLONY::_colony_bldgs[1].buildings[1] != 0) {
                    int32_t temp_val = COLONY::_colony_bldgs[1].buildings[1];
                    COLONY::_colony_bldgs[1].buildings[1] = COLONY::_colony_bldgs[sp_row].buildings[sp_col];
                    COLONY::_colony_bldgs[sp_row].buildings[sp_col] = temp_val;
                }
            }
        }

        COLDRAW::Build_Road_List_Based_On_Bldg_List_();

        if (is_ui_view == 0) {
            COLDRAW::Build_Veggie_List_Based_On_Bldg_List_();
        } else {
            COLDRAW::Clear_Veggie_Draw_Flags_();
        }

        random::Set_Random_Seed_(previous_seed);
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

    void __cdecl Insert_Bldg_Into_Array_(int16_t num_houses_delta, int16_t bldg_id, int16_t bldg_type, int8_t use_back_to_front) {
        int16_t row = -1;
        int16_t col = -1;
        int16_t n_houses = COLONY::N_Houses_In_Bldg_Array_();

        int total = (int)n_houses + (int)num_houses_delta;
        int16_t house_mod = (int16_t)(total % 4);

        if (bldg_type < 36 /* 0x24 */) {
            if (use_back_to_front == 1) {
                // TYPE MISMATCH: existing Bldg_Back_To_Front_ uses uint16_t*
                COLONY::Bldg_Back_To_Front_(bldg_type, (uint16_t*)&row, (uint16_t*)&col);
            } else {
                int16_t empty_count = 0;
                for (int16_t i = 0; i < 6; ++i) {
                    for (int16_t j = 0; j < 6; ++j) {
                        if (COLONY::_colony_bldgs[i].buildings[j] == 0) {
                            empty_count++;
                            if (random::Random_((int)empty_count) == 1) {
                                row = i;
                                col = j;
                            }
                        }
                    }
                }

                if (row == -1 || (col == -1 && bldg_id != -3)) {
                    COLONY::Find_Replacement_Slot_For_Building_(bldg_id, &row, &col);
                }
            }
        } else {
            COLONY::Find_Replacement_Slot_For_Building_(bldg_id, &row, &col);
        }

        if (row == -1 || col == -1) {
            return;
        }

        if (bldg_id >= 0) {
            COLONY::_colony_bldgs[row].buildings[col] = (int32_t)bldg_id;
        } else if (bldg_id == -3) {
            house_mod++;
            int switch_idx = (int)house_mod % 4;

            if (static_cast<uint32_t>(switch_idx) > 3) {
                COLONY::_colony_bldgs[row].buildings[col] = BUILDING_DIMENSIONAL_PORTAL /* 0x0E */;
            } else {
                switch (switch_idx) {
                case 0: COLONY::_colony_bldgs[row].buildings[col] = BUILDING_ARTEMIS_SYSTEM_NET /* 3 */; break;
                case 1: COLONY::_colony_bldgs[row].buildings[col] = BUILDING_DIMENSIONAL_PORTAL /* 0x0E */; break;
                case 2: COLONY::_colony_bldgs[row].buildings[col] = BUILDING_STAR_BASE /* 0x28 */; break;
                case 3: COLONY::_colony_bldgs[row].buildings[col] = BUILDING_STAR_FORTRESS /* 0x29 */; break;
                }
            }
        } else if (bldg_id == -1) {
            COLONY::_colony_bldgs[row].buildings[col] = BUILDING_BATTLESTATION /* 8 */;
        }
    }

    void Sort_Bldg_Array_Columns_() {
        int16_t iteration = 0;
        int16_t swapped_flag;

        do {
            swapped_flag = 0;

            for (int16_t row = 0; row < 5; ++row) {
                for (int16_t col = 0; col < 5; ++col) {
                    int32_t& b1 = _colony_bldgs[row].buildings[col];

                    int32_t& b_diag = _colony_bldgs[row + 1].buildings[col + 1];
                    if (b1 != 0 && b_diag != 0 &&
                        static_cast<uint8_t>(TECHDATA::_buildings[b_diag].type) < static_cast<uint8_t>(TECHDATA::_buildings[b1].type)) {
                        swapped_flag = static_cast<int16_t>(b1);
                        int32_t temp = b1;
                        b1 = b_diag;
                        b_diag = temp;
                    }

                    int32_t& b_below = _colony_bldgs[row + 1].buildings[col];
                    if (b1 != 0 && b_below != 0 &&
                        static_cast<uint8_t>(TECHDATA::_buildings[b_below].type) < static_cast<uint8_t>(TECHDATA::_buildings[b1].type)) {
                        swapped_flag = static_cast<int16_t>(b1);
                        int32_t temp = b1;
                        b1 = b_below;
                        b_below = temp;
                    }

                    int32_t& b_right = _colony_bldgs[row].buildings[col + 1];
                    if (b1 != 0 && b_right != 0 &&
                        static_cast<uint8_t>(TECHDATA::_buildings[b_right].type) < static_cast<uint8_t>(TECHDATA::_buildings[b1].type)) {
                        swapped_flag = static_cast<int16_t>(b1);
                        int32_t temp = b1;
                        b1 = b_right;
                        b_right = temp;
                    }
                }
            }
        } while (swapped_flag != 0 && ++iteration < 6);
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

    void __cdecl Tested_Colony_Buys_Outright_(int16_t colony_idx) {
        s_colony* col = &MOX::_colony[colony_idx];
        char buf[500];

        if (col->bought_outright != 0) {
            GENDRAW::Help_(ESTRINGS::E_Strings_(ESTR_YOU_HAVE_ALREADY_BOUGHT_THIS_PRODUCT));
            COLONY::Show_Colony_Screen_();
            return;
        }

        int star_idx = MOX::_planet[col->planet].star_index;
        if ((MOX::_star[star_idx].blockaded >> (MOX::_PLAYER_NUM & 0x1f)) & 1) {
            GENDRAW::Help_(ESTRINGS::E_Strings_(ESTR_YOU_CANNOT_BUY_WHILE_THIS_STAR_IS_BLOCKADED));
            COLONY::Show_Colony_Screen_();
            return;
        }

        // -3 = ECON_HOUSING, -2 = ECON_TRADE_GOODS
        if (col->producing[0] == -3 || col->producing[0] == -2) {
            GENDRAW::Help_(ESTRINGS::E_Strings_(ESTR_YOU_CANNOT_BUY_HOUSING_OR_TRADE_GOODS_AS_THEY_ARE_SETTINGS_NOT_SPECIFIC_ITEMS));
            COLONY::Show_Colony_Screen_();
            return;
        }

        int buy_cost = COLCALC::Colony_Cost_To_Buy_Producing_Product_(col);
        if (buy_cost <= 0) {
            if (col->producing[0] != -1 /* ECON_NONE */) {
                GENDRAW::Help_(ESTRINGS::E_Strings_(ESTR_YOU_CANNOT_SPEND_BCS_TO_GET_THIS_FASTER_THAN_IN_A_SINGLE_TURN));
            } else {
                GENDRAW::Help_(ESTRINGS::E_Strings_(ESTR_YOU_HAVE_NO_PRODUCT_SELECTED_TO_BUY));
            }
            COLONY::Show_Colony_Screen_();
            return;
        }

        if (COLCALC::Colony_Can_Afford_To_Buy_Product_(colony_idx)) {
            if (COLONY::Colony_Can_Buy_Product_0_(colony_idx)) {
                int prod_remaining = (int)COLCALC::Colony_Cost_To_Buy_Producing_Product_(col) - (int)col->production_spent;
                int actual_buy_cost = COLCALC::Colony_Cost_To_Buy_Producing_Product_(col);

                _sprintf(buf, ESTRINGS::E_Strings_(ESTR_DO_YOU_WANT_TO_SPEND_D_BC_TO_BUY_D_PRODUCTION),
                        actual_buy_cost, prod_remaining);

                if (GENDRAW::Warning_Box_(buf) != 0) {
                    COLCALC::Colony_Buy_Outright_(colony_idx);
                }
                COLONY::Col_Calc_Wrapper_(colony_idx);
            } else {
                GENDRAW::Help_(ESTRINGS::E_Strings_(ESTR_PURCHASING_FAILED_UNEXPECTEDLY_PLEASE_TELL_ERIC));
                COLONY::Show_Colony_Screen_();
            }
        } else {
            int short_by = (int)(short)COLCALC::Colony_Cost_To_Buy_Producing_Product_(col) - MOX::_player[col->owner].bc;
            _sprintf(buf, ESTRINGS::E_Strings_(ESTR_YOU_CANNOT_AFFORD_THIS_INCREASE_IN_PRODUCTION_YOU_ARE_SHORT_BY_D_BC),
                    short_by);
            GENDRAW::Help_(buf);
            COLONY::Show_Colony_Screen_();
        }
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

    void __cdecl Draw_Colony_Info_Military_() {
        int32_t last_result = 0;
        int32_t counter = 0;
        int32_t max_val = 0;
        int32_t i = 0;

        do {
            last_result = Do_Colony_Info_Military_Stuff_For_(i, last_result, counter);
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

    void Draw_Info_Name_And_Pop_() {
        char buffer[104];
        int32_t handle = _colony_handle;

        if (MOX::_colony[handle].occupation_policy == 0) {
            char* planet_name = HACCESS::Get_Planet_Name_(static_cast<int16_t>(_planet_handle));
            char* fmt = ESTRINGS::E_Strings_(ESTR_ANNIHILATING_S);
            _sprintf(buffer, fmt, planet_name);
            int16_t cycle_val = ERIC::Global_Cycler_(30, 2);
            Set_Colony_Font_To_Red_(4, cycle_val);
        } else {
            char* planet_name = HACCESS::Get_Planet_Name_(static_cast<int16_t>(_planet_handle));
            char* colony_type = Colony_Type_(handle);
            char* fmt = ESTRINGS::E_Strings_(ESTR_SCOLONY_OF_S);
            _sprintf(buffer, fmt, colony_type, planet_name);
            COLSUM::Set_Colony_Font_To_(4);
        }

        fonts::Set_Font_Spacing_(2);
        fonts::Print_Centered_(320, 0, buffer);

        int16_t colony_planet = MOX::_colony[handle].planet;
        int8_t star_idx = MOX::_planet[colony_planet].star_index;

        int16_t status_msg_id = -1;
        if ((MOX::_star[star_idx].blockaded >> (MOX::_PLAYER_NUM & 0x1F)) & 1) {
            int16_t cycle_val = ERIC::Global_Cycler_(30, 2);
            Set_Colony_Font_To_Red_(4, cycle_val);
            fonts::Set_Font_Spacing_(2);
            status_msg_id = ESTR_BLOCKADED1;
        } else if (EVENTS::Event_Check_Plague_(static_cast<int16_t>(handle))) {
            int16_t cycle_val = ERIC::Global_Cycler_(30, 2);
            Set_Colony_Font_To_Red_(4, cycle_val);
            fonts::Set_Font_Spacing_(2);
            status_msg_id = ESTR_PLAGUE;
        } else if (EVENTS::Event_Check_Population_Boom_(static_cast<int16_t>(handle))) {
            COLSUM::Set_Colony_Font_To_(4);
            fonts::Set_Font_Spacing_(2);
            status_msg_id = ESTR_POP_BOOM;
        }

        if (status_msg_id != -1) {
            char* status_text = ESTRINGS::E_Strings_(status_msg_id);
            fonts::Print_(0, 0, status_text);
        }

        COLSUM::Set_Colony_Font_To_(4);
        fonts::Set_Font_Spacing_(3);

        int sum_roundoff = 0;
        int sum_growth = 0;
        for (int i = 0; i < 10; ++i) {
            sum_roundoff += MOX::_colony[handle].pop_roundoff[i];
            sum_growth += MOX::_colony[handle].pop_growth[i];
        }

        int16_t n_base_pop = static_cast<int16_t>(Sum_Colonists_(handle, -1, -1, -1, -1, -1));
        int total_pop = n_base_pop + (sum_roundoff / 1000);
        int roundoff_rem = sum_roundoff % 1000;

        char* pop_fmt = ESTRINGS::E_Strings_(ESTR_POP_D03D_K_DK);
        _sprintf(buffer, pop_fmt, total_pop, roundoff_rem, sum_growth);

        fonts::Set_Font_Style_Shadow_Down_(2, &_font_color_array);
        fonts::Print_Right_(638, 3, buffer);
        Set_Colony_Font_();
    }

    void __cdecl Draw_Current_Building2_(int16_t x, int16_t y, int16_t building_id) {
        if (building_id == -3) {
            graphics::Set_Window_(517, 17, 639, 158);
            video::Clip_On_();
            COLONY::Draw_Building_With_Bottom_Centered_(579, 106, 14, 5, 5);
            video::Clip_Off_();
            return;
        }

        if (TECHDATA::_buildings[building_id].type != 7) {
            graphics::Set_Window_(517, 17, 639, 158);
            video::Clip_On_();
            COLONY::Draw_Building_With_Bottom_Centered_(579, 106, building_id, 5, 5);
            video::Clip_Off_();
            return;
        }

        s_animation_header* animation = COLONY::Satellite_Anim_(building_id);
        ERIC::Draw_Centered_(x, (int16_t)(y - 70), animation);
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

    void __cdecl Draw_Info_Build_() {
        char name_buffer[100];
        s_colony& colony = MOX::_colony[COLONY::_colony_handle];
        int16_t producing_id = colony.producing[0];

        switch (producing_id) {
            case -15:
                COLONY::Remap_Draw_Ship_Seg_N_Centered_(0x242 /* 578 */, 0x4d /* 77 */, 0x30 /* 48 */);
                break;
            case -12:
                COLONY::Remap_Draw_Ship_Seg_N_Centered_(0x242 /* 578 */, 0x39 /* 57 */, 0x2d /* 45 */);
                break;
            case -11:
                COLONY::Remap_Draw_Ship_Seg_N_Centered_(0x242 /* 578 */, 0x39 /* 57 */, 0x2f /* 47 */);
                break;
            case -8:
            case -2:
            case -1:
            case 0:
                // Do nothing
                break;
            case -7: {
                uint16_t race = (uint16_t)(uint8_t)MOX::_player[MOX::_PLAYER_NUM].race;
                s_animation_header* anim = COLONY::Spy_Anim_(race);
                ERIC::Draw_Centered_(0x242 /* 578 */, 0x4d /* 77 */, anim);
                break;
            }
            case -6:
            case -5:
            case -4: {
                uint16_t race = (uint16_t)(uint8_t)MOX::_player[MOX::_PLAYER_NUM].race;
                s_animation_header* anim = COLONY::People_Anim_(0, 4, race);
                ERIC::Draw_Centered_(0x242 /* 578 */, 0x4d /* 77 */, anim);
                break;
            }
            case -3:
                COLONY::Draw_Current_Building_(-3);
                break;
            default:
                COLONY::Set_Colony_Font_();
                if (producing_id >= 1 && producing_id < 49) {
                    COLONY::Draw_Current_Building_(producing_id);
                } else if (producing_id <= -100) {
                    COLONY::Draw_Current_Ship_(-100 - producing_id);
                } else {
                    char* err_msg = ESTRINGS::E_Strings_(0xf7);
                    RUSS::PFMT_(0, 0, err_msg);
                    assert::hgetch_();
                }
                break;
        }

        COLONY::Draw_Current_Production_Graph_();

        char* name_str = COLBLDG::Selection_Name_(producing_id);
        strcpy(name_buffer, name_str);

        fonts::Set_Font_Style_Shadow_Down_(1, &COLONY::_font_color_array);
        BILL::Squeeze_Print_Paragraph_(0x20a /* 522 */, 0x26 /* 38 */, 0x70 /* 112 */, 0x8d /* 141 */, name_buffer, 2);

        if (colony.auto_building != 0) {
            fonts::Set_Font_Style_Shadow_Down_(1, &COLONY::_bright_blue_font_color_array);
            char* auto_msg = ESTRINGS::E_Strings_(0xbd);
            fonts::Print_Centered_(0x242 /* 578 */, 0x1c /* 28 */, auto_msg);
        }

        COLONY::Draw_Turn_Count_();
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
}