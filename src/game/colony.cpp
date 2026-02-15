#include "deps.h"

namespace COLONY {
    void __cdecl Init_Colonies_() {
        return;
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

    s_animation_header* __cdecl Satellite_Anim_(int16_t colony_id) {
        int16_t pic_index = Satellite_Anim_Pic_(colony_id);
        return Buffer_Reload_Colony_(pic_index);
    }

    s_animation_header* __cdecl C_Anims_(int16_t index) {
        MOX::Mox_Update_();

        if (index > 0x23) {
            return nullptr;
        }

        switch (index) {
        case 0: {
            auto& colony = MOX::_colony[COLONY::_colony_handle];
            auto& planet = MOX::_planet[colony.planet];
            int16_t reload_arg = (int16_t)planet.climate_bg_type + (int16_t)colony.climate * 3;
            return Buffer_Reload_Planets_(reload_arg);
        }

        case 1:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x31, MOX::_global_cache_seg));

        case 2:
        case 11:
        case 12:
        case 16: case 17: case 18: case 19: case 20:
        case 21: case 22: case 23: case 24: case 25:
        case 26: case 27: case 28:
        case 30: case 31: case 32:
            return _c_anims[index];

        case 3:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLPUPS_LBX, 5, MOX::_global_cache_seg));

        case 4:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLPUPS_LBX, 6, MOX::_global_cache_seg));

        case 5:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x10, MOX::_global_cache_seg));

        case 6:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x11, MOX::_global_cache_seg));

        case 7:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x12, MOX::_global_cache_seg));

        case 8:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x2b, MOX::_global_cache_seg));

        case 9:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x2c, MOX::_global_cache_seg));

        case 10:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x2d, MOX::_global_cache_seg));

        case 13:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x32, MOX::_global_cache_seg));

        case 14:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x33, MOX::_global_cache_seg));

        case 15:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x34, MOX::_global_cache_seg));

        case 33:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLPUPS_LBX, 0x16, MOX::_global_cache_seg));

        case 34:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY_LBX, 0x11, MOX::_global_cache_seg));

        case 35:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY_LBX, 0x12, MOX::_global_cache_seg));

        default:
            return nullptr;
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

    /**
     * @address 004dc4a0
     */
    s_animation_header* __cdecl People_Anim_(int16_t param_1, int16_t param_2, int16_t param_3) {
        if (param_2 == 2) {
            int32_t entry_num = (static_cast<int32_t>(param_3) * 13) + (static_cast<int32_t>(param_1) * 2) + 1;
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_RACEICON_LBX, entry_num, MOX::_global_cache_seg));
        }

        if (param_2 == 0) {
            int32_t entry_num = (static_cast<int32_t>(param_3) * 13) + (static_cast<int32_t>(param_1) * 2);
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_RACEICON_LBX, entry_num, MOX::_global_cache_seg));
        }

        if (param_2 == 3) {
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_RACEICON_LBX, 0xAA, MOX::_global_cache_seg));
        }

        if (param_2 == 4) {
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_RACEICON_LBX, 0xA9, MOX::_global_cache_seg));
        }

        return nullptr;
    }

    /**
     * Checks if a defensive satellite building (Star Base or Battlestation) should be replaced 
     * because a superior version already exists in the colony.
     */
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

    /**
     * @address 004dd1b0
     */
    void __cdecl Clear_Colony_Sattelites_() {
        memset(_colony_satellites, 0xff, sizeof(_colony_satellites));
    }

    void __cdecl Draw_Building_With_Bottom_Centered_(int16_t x, int16_t y, int16_t type, int16_t col, int16_t row) {
        s_animation_header* bldg_anim = COLBCACH::Cache_Load_Bldg_(col, row, type);

        int32_t cr_x = CR_To_XY_(col, row, 0);
        int32_t cr_y = CR_To_XY_(col, row, 1);

        bitmap::Create_Blank_Bitmap_(120, 120, COLBLDG::_bldg_bitm);

        int16_t offset_x = -(static_cast<int16_t>(cr_x) - 60);
        int16_t offset_y = -(static_cast<int16_t>(cr_y) - 120);

        bitmap::Add_Picture_To_Bitmap_(offset_x, offset_y, bldg_anim, (s_animation_header*)COLBLDG::_bldg_bitm);
        bitmap::Mask_Out_Colors_(COLBLDG::_bldg_bitm, 240, 255);

        int16_t min_x, min_y, actual_w, actual_h;
        bitmap::Get_Bitmap_Actual_Size_((s_animation_header*)COLBLDG::_bldg_bitm, &min_x, &min_y, &actual_w, &actual_h);

        int16_t draw_x = x - (actual_w / 2) - min_x;
        int16_t draw_y = y - actual_h - min_y;

        animate::Draw_(draw_x, draw_y, (s_animation_header*)COLBLDG::_bldg_bitm);
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

    /**
     * @address 004dc800
     */
    void __cdecl Load_Bldg_Bitm_() {
        COLBLDG::_bldg_bitm = allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x3860);
        _bldg_bitm_n = -1;
    }

    /**
     * @address 004dc830
     */
    void __cdecl Load_Ship_Bitm_() {
        _ship_bitm = allocate::Allocate_Next_Block_(MOX::_screen_seg, 0xE30);
        _ship_pic_bitm = allocate::Allocate_Next_Block_(MOX::_screen_seg, 0xE30);

        _ship_bitm_n = -1;
        _g_ship_pic_n = -1;
        _g_ship_player_n = -1;
    }

    /**
     * @address 004dc890
     */
    void __cdecl Load_Galaxy_Map_Anims_() {
        int entry_num = 23;
        void* screen_seg = MOX::_screen_seg;

        for (int i = 0; i < 10; ++i) {
            MOX::_colony_galaxy_star_seg[i] = (s_animation_header*)farload::Far_Reload_Next_(s_gstar_lbx_00558544, entry_num, screen_seg);
            entry_num++;

            screen_seg = MOX::_screen_seg;
        }
    }

    /**
     * @address 004dc9e0
     */
    void __cdecl Load_Officer_Anim_() {
        int8_t officer_idx = MOX::_star[COLONY::_star_handle].officer_index[MOX::_PLAYER_NUM];

        if (officer_idx != -1) {
            _c_anims[30] = (s_animation_header*)OFFICER::Load_Officer_Picture_(
                (int32_t)MOX::_PLAYER_NUM,
                (int16_t)officer_idx,
                (uint32_t)(uintptr_t)MOX::_screen_seg,
                0,
                1
            );
        }
    }

    void __cdecl Load_Planet_Screen_() {
        return;
    }

    /**
     * @address 004dc8d0
     */
    void __cdecl Load_Colony_Screen_Seg_() {
        COLBLDG::Load_Build_Anims_();
        MOX::Mox_Update_();
        COLXPORT::Load_Xport_Anims_();
        MOX::Mox_Update_();
        Load_Ship_Bitm_();
        MOX::Mox_Update_();
        Load_Bldg_Bitm_();
        MOX::Mox_Update_();
        Load_Officer_Anim_();
        MOX::Mox_Update_();
        COLDRAW::Load_Road_List_Anims_();
        COLBLDG::Clear_Last_App_Desc_();
        MOX::Mox_Update_();
        Load_Galaxy_Map_Anims_();
        MOX::Mox_Update_();

        _c_anims[19] = (s_animation_header*)farload::Far_Reload_Next_(_COLPUPS_LBX, 1, MOX::_screen_seg);
        _c_anims[20] = (s_animation_header*)farload::Far_Reload_Next_(_COLPUPS_LBX, 2, MOX::_screen_seg);
        _c_anims[21] = (s_animation_header*)farload::Far_Reload_Next_(_COLPUPS_LBX, 4, MOX::_screen_seg);
        _c_anims[22] = (s_animation_header*)farload::Far_Reload_Next_(_COLPUPS_LBX, 7, MOX::_screen_seg);
        _c_anims[23] = (s_animation_header*)farload::Far_Reload_Next_(_COLPUPS_LBX, 8, MOX::_screen_seg);
        _c_anims[24] = (s_animation_header*)farload::Far_Reload_Next_(_COLPUPS_LBX, 9, MOX::_screen_seg);
        _c_anims[32] = (s_animation_header*)farload::Far_Reload_Next_(_COLPUPS_LBX, 21, MOX::_screen_seg);
    }

    /**
     * @address 004dcd70
     */
    s_animation_header* __cdecl Prod_Anims_(int16_t param_1, int16_t param_2, int8_t param_3) {
        int32_t index = (int32_t)param_1 + (int32_t)param_2 * 4;

        if (param_3 != 0) {
            index += 27;
        }

        return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, index, MOX::_global_cache_seg));
    }

    /**
     * @address 004e1cd0
     */
    void __cdecl Reset_Mouse_Picture_() {
        mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
        fields::Enable_Draw_All_Fields_();
    }

    /**
     * Draws a ship design centered at specified coordinates.
     * 
     * @param x The screen x-coordinate.
     * @param y The screen y-coordinate.
     * @param design_index The index of the ship design in the player's ship_designs array.
     * @address 004dbbd0
     */
    void __cdecl Remap_Draw_Ship_D_Centered_(int16_t x, int16_t y, int16_t design_index) {
        int16_t picture_num = static_cast<int16_t>(MOX::_player[MOX::_PLAYER_NUM].ship_designs[design_index].picture_num);

        Remap_Draw_Ship_Seg_N_Centered_(x, y, picture_num);
    }

    /**
     * @address 004dce50
     */
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

    /**
     * @address 004ddcd0
     */
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

    /**
     * @address 004db8c0
     */
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

    /**
     * @address 004dd5b0
     */
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

    /**
     * @address 004dd340
     */
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

    /**
     * @address 004dd1f0
     */
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

    /**
     * @address 004ddda0
     */
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

    void __cdecl Draw_Colony_Bldg_(int16_t col, int16_t row) {
        if (col >= 0 && col < 6 && row >= 0 && row < 6) {
            int32_t building_id = _colony_bldgs[col].buildings[row];
            if (building_id != 0) {
                s_animation_header* seg = COLBCACH::Cache_Load_Bldg_(col, row, building_id);

                if (_full_draw) {
                    int16_t prev_frame = animate::Get_Current_Frame_(seg);
                    animate::Reset_Animation_Frame_(seg);

                    while (animate::Get_Current_Frame_(seg) < prev_frame) {
                        animate::Draw_(0, 0, seg);
                    }

                    if (_full_draw) {
                        animate::Draw_(0, 0, seg);
                        return;
                    }
                }

                if (_scanned_bldg_C == col && _scanned_bldg_R == row) {
                    if (ERIC::Global_Cycler_(30, 2)) {
                        Draw_Bldg_CR_Dark_(col, row);
                        return;
                    }
                }

                animate::Draw_No_Glass_(0, 0, seg);
            }
        }
    }

    void __cdecl Draw_Colony_Bldgs_() {
        bool is_scanning_valid = (_scanned_bldg_C != -1 && _scanned_bldg_R != -1);

        for (int16_t i = 0; i < 36 /* 0x24 */; ++i) {
            uint16_t row, col;
            Bldg_Back_To_Front_(i, &col, &row);

            if (_full_draw != 0 || (_scanned_bldg_C != -1 && _scanned_bldg_R != -1)) {
                COLDRAW::Draw_Veggie_CR_(col, row, !is_scanning_valid);

                int32_t building_id = _colony_bldgs[col].buildings[row];
                if (building_id != 0) {
                    s_animation_header* anim = COLBCACH::Cache_Load_Bldg_(col, row, building_id);
                    int16_t frame_count = animate::Get_Animation_Frame_Count_(anim);

                    if (frame_count > 1 || _full_draw != 0 || is_scanning_valid) {
                        Draw_Colony_Bldg_(col, row);
                    }
                }
            }
        }
    }

    void __cdecl Allocate_Mini_Planet_Cache_() {
        return;
    }

    /**
     * @address 004df750
     */
    void __cdecl Clear_Galaxy_Map_Fields_() {
        _galaxy_map_name_field = -1000;
        MOX::_galaxy_map_grid_field = -1000;

        for (int i = 0; i < 72; ++i) {
            MOX::_galaxy_map_star_field[i] = -1000;
        }
    }

    /**
     * @address 004dbe20
     */
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

    void __cdecl Show_Colony_Screen_() {
        MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        fields::Deactivate_Auto_Function_();
        _full_draw = 1;
        fields::Assign_Auto_Function_(Draw_Colony_Screen_, 1);
        ERICHELP::Set_Colony_Screen_Help_List_();
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

    void __cdecl Draw_Planet_Summary_(int16_t x, int16_t y, int16_t planet_idx) {
        char buffer[2000];

        fonts::Set_Font_Style_Shadow_Down_(1, &_font_color_array);

        graphics::Fill_(x + 5, y + 2, x + 0xb0, y + 0xc3, 0xe9);

        s_animation_header* planet_anim = Mini_Planet_Anim_(planet_idx);
        remap::Calculate_Picture_Remap_Colors_(planet_anim);

        s_animation_header* ui_anim = C_Anims_(0xe);
        animate::Draw_(x, y, ui_anim);

        MAINPUPS::Build_Generic_Planet_Or_Colony_Description_String_Formatted_(planet_idx, buffer);

        BILL::Squeeze_Print_Formatted_Paragraph_(x + 14, y + 45, 154, 140, buffer, 0);
    }

    /**
     * @address 004dd880
     */
    int16_t __cdecl Bldg_Coords_To_Effective_Frame_(int16_t x, int32_t y) {
        if ((y % 2) == 0) {
            return (int16_t)y * 6 + x;
        } else {
            return ((int16_t)y * 6 - x) + 5;
        }
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
}