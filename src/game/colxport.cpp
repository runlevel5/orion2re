#include "pch.h"

namespace COLXPORT {
    uint8_t _cheezy_hack_col_count_state;

    int16_t _cheezy_hack_col_count[MAX_PLAYERS];

    int16_t* _g_colony_list_ptr;

    MOX2::action_func_t _saved_autofunction;

    int16_t _list_col[6];

    s_colsum_col_pop_ref _list_col_pop_ref[6];

    int16_t _list_col_coord_vars[6];

    int16_t _list_col_pop_count[6];

    int16_t _list_col_job_fields[6];

    int16_t _list_fields[6];

    int16_t _x_fields_array[4];

    s_animation_header* _anims[4];

    void __cdecl Draw_Xport_Popup_() {
        COLONY::Set_Colony_Font_();

        if (COLONY::_full_draw != 0) {
            COLMOVE::_g_hacky_dont_draw_cluster_flag = 1;
            MOX2::Execute_Auto_Function_(&COLXPORT::_saved_autofunction);
            COLMOVE::_g_hacky_dont_draw_cluster_flag = 0;
            COLONY::_full_draw = 1;

            COLXPORT::_anims[0] = (s_animation_header*)buffer::Buffer_Reload_(COLONY::_COLPUPS_LBX, 0x10, MOX::_global_cache_seg);
            animate::Draw_(0, 0, COLXPORT::_anims[0]);
            video::Copy_Off_To_Back_();

            COLONY::_full_draw = 0;
        }

        COLXPORT::Clear_List_Col_Array_();
        COLSUM::Update_First_(6, &COLSUM::_first, &COLSUM::_slider_bar_position);

        for (int16_t i = 0; i < 6; ++i) {
            int16_t colony_idx = COLXPORT::_g_colony_list_ptr[i + COLSUM::_first];
            if (colony_idx != -1) {
                COLXPORT::Draw_Colony_Summary_For_Colony_(colony_idx, i);
            }
        }

        COLXPORT::Draw_Bar_Indicator_();

        int16_t mouse_y = mouse::Pointer_Y_();
        int16_t mouse_x = mouse::Pointer_X_();
        COLMOVE::Draw_Cluster_(mouse_x, mouse_y);
    }

/**
 * @brief Returns the number of non-outpost colonies belonging to a specific player.
 * 
 * If a "cheezy hack" state is active, it returns a cached value.
 * Otherwise, it iterates through the global colony list.
 */
int16_t __cdecl N_Colonies_(int16_t player_idx) {
    if (_cheezy_hack_col_count_state != 0) {
        return _cheezy_hack_col_count[(int16_t)player_idx];
    }

    int16_t colony_count = 0;
    for (int16_t i = 0; i < MOX::_NUM_COLONIES; ++i) {
        s_colony* colony = &MOX::_colony[i];

        if ((int16_t)colony->owner == player_idx && colony->outpost_flag == 0) {
            colony_count++;
        }
    }

    return colony_count;
}

/**
 * @brief Rebuilds the global list of colonies belonging to the current player.
 *
 * Scans through all existing colonies, filtering for those owned by the current player
 * and not marked as outposts. The resulting indices are stored in a global list,
 * which is then padded with -1 up to its capacity of 250.
 */
void __cdecl Build_Global_Colony_List_() {
    int16_t count = 0;

    for (int16_t i = 0; i < MOX::_NUM_COLONIES; ++i) {
        if (MOX::_colony[i].owner == (char)MOX::_PLAYER_NUM && MOX::_colony[i].outpost_flag == 0) {
            _g_colony_list_ptr[count] = i;
            count++;
        }
    }

    for (; count < MAX_COLONIES; ++count) {
        _g_colony_list_ptr[count] = -1;
    }
}

void __cdecl Load_Xport_Anims_() {
    _anims[3] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLPUPS_LBX, 0x11, MOX::_screen_seg);
    _anims[1] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLPUPS_LBX, 0x13, MOX::_screen_seg);
    _anims[2] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_COLPUPS_LBX, 0x14, MOX::_screen_seg);
}

int16_t __cdecl Xport_Popup_() {
    int16_t colony_list_buffer[MAX_COLONIES];
    int16_t input_id;
    int16_t selected_colony = -1;

    for (int32_t i = 0; i < 15; ++i) {
        colony_list_buffer[i] = -1;
    }

    memset(&colony_list_buffer[15], 0, (MAX_COLONIES - 15) * sizeof(colony_list_buffer[0]));

    _g_colony_list_ptr = colony_list_buffer;
    COLONY::_full_draw = 1;

    Build_Global_Colony_List_();
    fonts::Set_Font_Style_Shadow_Down_(1, &COLONY::_font_color_array);
    MOX2::Get_Auto_Function_(&_saved_autofunction);
    fields::Deactivate_Auto_Function_();
    fields::Assign_Auto_Function_(Draw_Xport_Popup_, 1);
    ERICHELP::Set_Transport_Popup_Help_List_();
    Draw_Xport_Popup_();
    video::Set_Refresh_Stencil_();
    Add_Fields_();
    RUSS::Mox_Sync_Update_();

    do {
        input_id = fields::Get_Input_();

        if (input_id == _x_fields_array[2]) {
            COLSUM::Decrement_First_(6);
        } else if (input_id == _x_fields_array[3]) {
            if (_g_colony_list_ptr[COLSUM::_first + 6] != -1) {
                COLSUM::Increment_First_(6);
            }
        } else {
            for (int32_t i = 0; i < 6; ++i) {
                if (_list_fields[i] == input_id) {
                    selected_colony = _list_col[i];
                }
            }
        }

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
            COLONY::_full_draw = 1;
            ERICHELP::Set_Transport_Popup_Help_List_();
        }

        Draw_Xport_Popup_();
        Add_Fields_();
        RUSS::Mox_Sync_Update_();

    } while (input_id != _x_fields_array[1] && selected_colony == -1);

    fields::Clear_Fields_();
    fields::Deactivate_Help_List_();
    fields::Deactivate_Auto_Function_();
    MOX2::Put_Auto_Function_(&_saved_autofunction);

    return selected_colony;
}

void __cdecl Add_Fields_()
{
    int16_t num_colonies;
    int16_t y_offset;
    char* empty_string;

    fields::Clear_Fields_();

    num_colonies = COLXPORT::N_Colonies_(MOX::_PLAYER_NUM);

    if (num_colonies < 6) {
        COLXPORT::_x_fields_array[0] = -1000;
    }
    else {
        empty_string = ESTRINGS::E_Strings_(12);
        COLXPORT::_x_fields_array[0] = fields::Add_Scroll_Field_(
            449, 
            75, 
            0, 
            num_colonies, 
            0, 
            num_colonies - 6, 
            14, 
            164, 
            &COLSUM::_slider_bar_position, 
            empty_string, 
            0
        );
    }

    COLXPORT::_x_fields_array[2] = fields::Add_Button_Field_(
        448, 
        52, 
        ESTRINGS::E_Strings_(12), 
        _anims[1], 
        ESTRINGS::E_Strings_(12), 
        40
    );

    COLXPORT::_x_fields_array[3] = fields::Add_Button_Field_(
        447, 
        243, 
        ESTRINGS::E_Strings_(12), 
        _anims[2], 
        ESTRINGS::E_Strings_(12), 
        40
    );

    COLXPORT::_x_fields_array[1] = fields::Add_Button_Field_(
        352, 
        285, 
        ESTRINGS::E_Strings_(12), 
        _anims[3], 
        ESTRINGS::E_Strings_(12), 
        40
    );

    y_offset = 83;
    for (int32_t i = 0; i < 6; ++i) {
        if (COLXPORT::_list_col[i] != -1) {
            char* scroll_str = ESTRINGS::E_Strings_(12);
            COLXPORT::_list_fields[i] = fields::Add_Hidden_Field_(
                15, 
                y_offset - 31, 
                441, 
                y_offset, 
                scroll_str, 
                0
            );
        }
        y_offset += 38;
    }
}

void __cdecl Draw_Colony_Summary_For_Colony_(int16_t colony_idx, int16_t entry_idx) {
    COLXPORT::_list_col[entry_idx] = colony_idx;

    int16_t planet_idx = MOX::_colony[colony_idx].planet;
    int16_t star_idx = MOX::_planet[planet_idx].star_index;

    if (((MOX::_star[star_idx].blockaded >> MOX::_PLAYER_NUM) & 1) != 0 || EVENTS::Event_Check_Plague_(colony_idx) != 0) {
        COLONY::Set_Colony_Font_To_Red_(4, 0);
    } else {
        COLONY::Set_Colony_Font_To_(4);
    }

    char* planet_name = HACCESS::Get_Planet_Name_(planet_idx);
    int16_t y_pos = entry_idx * 38 + 54;
    BILL::Squeeze_Print_Paragraph_(15, y_pos, 130, 28, planet_name, 0);

    COLXPORT::Draw_Info_Pop_For_(entry_idx);
}

void __cdecl Clear_List_Col_Array_() {
    memset(_list_col, 0xff, sizeof(_list_col));
}

void __cdecl Draw_Bar_Indicator_() {
    int16_t n_colonies = COLXPORT::N_Colonies_(MOX::_PLAYER_NUM);
    COLSUM::Update_First_(6, &COLSUM::_first, &COLSUM::_slider_bar_position);

    if (n_colonies >= 6) {
        int16_t y1 = (int16_t)((COLSUM::_first * 164) / n_colonies) + 75 /* 0x4b */;
        int16_t y2 = (int16_t)(((COLSUM::_first + 6) * 164) / n_colonies) + 75 /* 0x4b */;

        graphics::Fill_(449 /* 0x1c1 */, y1, 457 /* 0x1c9 */, y2, 229 /* 0xe5 */);

        line::Line_(449 /* 0x1c1 */, y1, 457 /* 0x1c9 */, y1, 230 /* 0xe6 */);
        line::Line_(449 /* 0x1c1 */, y1, 449 /* 0x1c1 */, y2, 230 /* 0xe6 */);
        line::Line_(457 /* 0x1c9 */, y1, 457 /* 0x1c9 */, y2, 228 /* 0xe4 */);
        line::Line_(449 /* 0x1c1 */, y2, 457 /* 0x1c9 */, y2, 228 /* 0xe4 */);

        graphics::Dot_(449 /* 0x1c1 */, 75 /* 0x4b */, 80 /* 0x50 / 'P' */);
        graphics::Dot_(457 /* 0x1c9 */, 75 /* 0x4b */, 80 /* 0x50 / 'P' */);
        graphics::Dot_(449 /* 0x1c1 */, 239 /* 0xef */, 80 /* 0x50 / 'P' */);
        graphics::Dot_(457 /* 0x1c9 */, 239 /* 0xef */, 80 /* 0x50 / 'P' */);
    } else {
        graphics::Fill_(449 /* 0x1c1 */, 75 /* 0x4b */, 457 /* 0x1c9 */, 239 /* 0xef */, 80 /* 0x50 / 'P' */);
    }
}

void __cdecl Draw_Info_Pop_For_(int16_t entry_index) {
    const int32_t index = (int32_t)entry_index;
    int16_t y = (int16_t)(entry_index * 0x26);
    y = (int16_t)(y + 0x36);

    COLDRAW::Do_Colony_Info_Pop_Stuff_For_Pop_(
        COLXPORT::_list_col[index],
        -1,
        0,
        0x94,
        y,
        0x1B9,
        0,
        COLXPORT::_list_col_pop_ref[index].refs,
        &COLXPORT::_list_col_pop_count[index],
        &COLXPORT::_list_col_coord_vars[index],
        &COLXPORT::_list_col_job_fields[index]
    );
}

} // namespace COLXPORT