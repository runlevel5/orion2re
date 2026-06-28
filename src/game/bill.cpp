#include "pch.h"

namespace BILL {
    int16_t __pp_max_width;
    int16_t __pp_max_height;

    uint8_t _space_limits[8] = {0, 1, 1, 1, 2, 3, 3, 0};

    char _dest_file_name[32];
    const char s_SR_R_x_SC_LBX[14] = "SR_R%x_SC.LBX";
    const char s_SR_R_wildcard_SC_LBX[13] = "SR_R?_SC.LBX";

    void __cdecl Bill_Init_() {
        MOX::_bill_savegame[0] = 2;
        MOX::_bill_savegame[1] = 0;
        MOX::_bill_savegame[2] = 1;
        MOX::_bill_savegame[3] = 1;
        MOX::_bill_savegame[4] = 1;
        MOX::_bill_savegame[5] = 1;
    }

    void __cdecl Copy_File_To_File_(const char* src_path, const char* dst_path) {
        char msg_buf[128];
        char read_buffer[4096];

        FILE* src_fp = fopen_case(src_path, "rb");
        FILE* dst_fp = fopen_case(dst_path, "wb");

        if (src_fp == nullptr) {
            snprintf(msg_buf, sizeof(msg_buf), "Could not open %s", src_path);
            exitm::Exit_With_Message(msg_buf);
        }

        if (dst_fp == nullptr) {
            snprintf(msg_buf, sizeof(msg_buf), "Could not open %s", dst_path);
            exitm::Exit_With_Message(msg_buf);
        }

        int32_t total_bytes_read = 0;
        size_t current_chunk_size;

        do {
            current_chunk_size = fread(read_buffer, 1, 4096, src_fp);
            total_bytes_read += (int32_t)current_chunk_size;

            RUSS::Mox_Update_();

            size_t written = fwrite(read_buffer, 1, current_chunk_size, dst_fp);
            if (written != current_chunk_size) {
                snprintf(msg_buf, sizeof(msg_buf), "Error copying %s to %s (bytes: %i)", src_path, dst_path, total_bytes_read);
                exitm::Exit_With_Message(msg_buf);
            }
        } while (current_chunk_size == 4096);

        fflush(dst_fp);
        fclose(dst_fp);
        fclose(src_fp);
    }

    char* __cdecl Cache_Player_Spy_Anim_() {
        char find_data[52];
        char src_file_name[128];

        snprintf(_dest_file_name, sizeof(_dest_file_name), SCIENCE::_spy_anim_fmt,
                 (uint32_t)(uint8_t)(MOX::_player[MOX::_PLAYER_NUM].race));

        if (dos::DIR_(_dest_file_name, find_data) == 0) {
            if (dos::DIR_(SCIENCE::_spy_anim_find_pat, find_data) != 0) {
                dos::Delete_File_(find_data);
            }

            strlcpy(src_file_name, farload::_secondary_drive_path, sizeof(src_file_name));
            strlcat(src_file_name, _dest_file_name, sizeof(src_file_name));

            Copy_File_To_File_(src_file_name, _dest_file_name);
        }

        return _dest_file_name;
    }

    void __cdecl Cache_Player_Scientist_Anim_() {
        Cache_Player_Scientist_Anim_Imp_(MOX::_PLAYER_NUM);
    }

    char* __cdecl Cache_Player_Scientist_Anim_Imp_(int16_t player_index) {
        char dir_entry[52];
        char source_path[128];

        s_player* player = &MOX::_player[player_index];

        snprintf(BILL::_dest_file_name, sizeof(BILL::_dest_file_name), s_SR_R_x_SC_LBX, (uint32_t)(uint8_t)player->race);

        if (!dos::DIR_(BILL::_dest_file_name, dir_entry)) {
            if (dos::DIR_(s_SR_R_wildcard_SC_LBX, dir_entry)) {
                dos::Delete_File_(dir_entry);
            }

            strlcpy(source_path, farload::_secondary_drive_path, sizeof(source_path));
            strlcat(source_path, BILL::_dest_file_name, sizeof(source_path));

            Copy_File_To_File_(source_path, BILL::_dest_file_name);
        }

        return BILL::_dest_file_name;
    }

    int16_t __cdecl Get_Their_Spy_Number_(int16_t player_idx, int16_t target_idx) {
        return (int16_t)(MOX::_player[player_idx].spies[target_idx] & 0x3F);
    }

    int16_t __cdecl Add_Their_Spy_(int16_t player_idx, int16_t other_player_idx) {
        const uint16_t spy_count =
                ((uint8_t)MOX::_player[player_idx].spies[other_player_idx] & 0x3f) + 1;

        if (spy_count < 0x40) {
            MOX::_player[player_idx].spies[other_player_idx] = (uint8_t)spy_count;
            return (int16_t)spy_count;
        }

        return 0;
    }

    int16_t __cdecl Get_My_Spy_Number_(int16_t spy_idx) {
        return (int16_t)(MOX::_player[MOX::_PLAYER_NUM].spies[spy_idx] & 0x3F);
    }

    void __cdecl Set_Their_Spy_Number_(int16_t player_idx, int16_t target_player_idx, int16_t count) {
        MOX::_player[player_idx].spies[target_player_idx] &= 0xC0;
        MOX::_player[player_idx].spies[target_player_idx] |= (count & 0x3F);
    }

    int16_t __cdecl _Get_Paragraph_Max_Width_(int16_t width, char* text, int16_t alignment) {
        _Print_Paragraph_Bill_(0, 0, width, text, (uint16_t)alignment, 0, nullptr);
        return __pp_max_width;
    }

    int16_t __cdecl _Get_Paragraph_Max_Height_(int16_t width, const char* text) {
        BILL::_Print_Paragraph_Bill_(0, 0, width, text, 0, 0, nullptr);
        return BILL::__pp_max_height;
    }

    void __cdecl _Print_Paragraph_(int16_t x, int16_t y, int16_t width, const char* text, int16_t alignment) {
        BILL::_Print_Paragraph_Bill_(x, y, width, text, (uint16_t)alignment, 1, nullptr);
    }

    void __cdecl Squeeze_Print_Centered_(int16_t x, int16_t y, const char* str, int16_t width) {
        _Squeeze_Print_(x - (width / 2), y, str, width, nullptr, 2, true);
    }

    void __cdecl Squeeze_Centered_Print_(uint16_t x, int16_t y, char* text, uint16_t limit) {
        _Squeeze_Print_((int16_t)x, y, text, (int16_t)limit, nullptr, 2, true);
    }

    void __cdecl Squeeze_Print_(int16_t x, int16_t y, const char* text, int16_t max_width) {
        _Squeeze_Print_(x, y, text, max_width, nullptr, 0, true);
    }

    void __cdecl _Squeeze_Print_(int16_t x, int16_t y, const char* str, int16_t limit_w, s_animation_header* bitmap, int16_t alignment, bool adjust_y) {
        bool resized = false;
        int16_t current_width = fonts::Get_String_Width_(str);

        if (current_width <= limit_w) {
            _Print_String_Bill_(x, y, limit_w, str, alignment, 1, bitmap);
            return;
        }

        int8_t orig_space_width = fonts::_font_header.font_style_widths[32];
        int16_t orig_spacing = fonts::_font_header.font_style_spacing;
        int16_t orig_current_font_style = fonts::_current_font_style;
        int16_t orig_font_style = fonts::_font_header.font_style;
        int16_t y_offset = 0;
        int16_t initial_height = fonts::Get_Font_Height_();

        while (true) {
            while (BILL::_space_limits[fonts::_current_font_style] < fonts::_font_header.font_style_widths[32]) {
                if (fonts::Get_String_Width_(str) <= limit_w) {
                    break;
                }
                fonts::_font_header.font_style_widths[32]--;
            }

            current_width = fonts::Get_String_Width_(str);
            while (current_width > limit_w && fonts::_font_header.font_style_spacing > 1) {
                fonts::_font_header.font_style_spacing--;
                current_width = fonts::Get_String_Width_(str);
            }

            current_width = fonts::Get_String_Width_(str);
            if (current_width <= limit_w || fonts::_current_font_style <= 0) {
                fonts::_font_header.font_style = orig_font_style;
                _Print_String_Bill_(x, y + y_offset, limit_w, str, alignment, 1, bitmap);

                fonts::_font_header.font_style_spacing = orig_spacing;
                fonts::_font_header.font_style_widths[32] = orig_space_width;

                if (resized) {
                    fonts::Set_Font_Style_(orig_current_font_style, &fonts::_font_header.selected_colors);
                    fonts::_font_header.font_style = orig_font_style;
                }
                return;
            }

            fonts::Set_Font_Style_(fonts::_current_font_style - 1, &fonts::_font_header.normal_colors);

            if (adjust_y) {
                int16_t new_height = fonts::Get_Font_Height_();
                y_offset = (initial_height - new_height) / 2;
            }
            resized = true;
        }
    }

    void __cdecl _Squeeze_Print_Paragraph_(int16_t x, int16_t y, int16_t width, int16_t max_height, char* text, int32_t color_or_alignment, s_animation_header* bitmap, bool is_formatted, bool center_y) {
        int8_t saved_space_width = fonts::_font_header.font_style_widths[32];
        int16_t saved_spacing = fonts::_font_header.font_style_spacing;
        uint16_t saved_leading = fonts::_font_header.leading_offset;
        int16_t saved_font_style = fonts::_font_header.font_style;
        int16_t saved_current_font = fonts::_current_font_style;

        using GetHeightFn = int16_t(*)(int16_t, char*);
        GetHeightFn get_height = is_formatted ? (GetHeightFn)FMTPARA::Get_Formatted_Paragraph_Max_Height_ : (GetHeightFn)BILL::_Get_Paragraph_Max_Height_;

        bool font_changed = false;
        bool done_squeezing = false;
        int16_t height = 0;

        do {
            height = get_height(width, text);
            if (max_height >= height) {
                break;
            }

            done_squeezing = true;

            while (true) {
                height = get_height(width, text);
                if (max_height >= height) {
                    break;
                }

                if (BILL::_space_limits[fonts::_current_font_style] >= fonts::_font_header.font_style_widths[32]) {
                    break;
                }

                fonts::_font_header.font_style_widths[32]--;
                done_squeezing = false;
            }

            while (true) {
                height = get_height(width, text);
                if (max_height >= height) {
                    break;
                }

                if ((int16_t)fonts::_font_header.leading_offset <= 2) {
                    break;
                }

                fonts::_font_header.leading_offset--;
                done_squeezing = false;
            }

            height = get_height(width, text);
            if (max_height < height && fonts::_current_font_style > 0) {
                fonts::Set_Font_Style_(fonts::_current_font_style - 1, &fonts::_font_header.normal_colors);
                font_changed = true;
                done_squeezing = false;
            }
        } while (!done_squeezing);

        if (center_y) {
            y = y - (height / 2);
        }

        if (is_formatted) {
            FMTPARA::Print_Formatted_Paragraph_To_Bitmap_(x, y, width, text, (int16_t)color_or_alignment, bitmap);
        } else {
            BILL::_Print_Paragraph_Bill_(x, y, width, text, (uint16_t)color_or_alignment, 1, bitmap);
        }

        fonts::_font_header.font_style_spacing = saved_spacing;
        fonts::_font_header.font_style_widths[32] = saved_space_width;
        fonts::_font_header.leading_offset = saved_leading;

        if (font_changed) {
            fonts::Set_Font_Style_(saved_current_font, &fonts::_font_header.selected_colors);
            fonts::_font_header.font_style = saved_font_style;
        }
    }

    void __cdecl Squeeze_Print_Paragraph_(uint16_t x, int16_t y, int16_t width, int16_t height, char* text, uint16_t alignment) {
        _Squeeze_Print_Paragraph_(x, y, width, height, text, alignment, nullptr, false, false);
    }

    void __cdecl Bill_Default_(void) {
        if (MOX::_settings.game_difficulty < 2) {
            BILL::Set_Plyr_Info_Btns_(4);
        }
        FMTPARA::Init_();
    }

    void __cdecl Squeeze_Paragraph_Centered_(int16_t x, int16_t y, int16_t width, int16_t height, char* text, int16_t color) {
        int16_t centered_y = (int16_t)(height / 2) + y;

        _Squeeze_Print_Paragraph_(
            x,
            centered_y,
            width,
            height,
            text,
            color,
            nullptr,
            0,
            1
        );
    }

    void __cdecl Squeeze_Formatted_Paragraph_Centered_(int16_t x, int16_t y, int16_t width, int16_t height, char* text, int16_t color) {
        BILL::_Squeeze_Print_Paragraph_(x, y + (height / 2), width, height, text, color, nullptr, true, true);
    }

    void __cdecl Squeeze_Print_Formatted_Paragraph_(int16_t x, int16_t y, int16_t width, int16_t height, char* text, int16_t flags) {
        _Squeeze_Print_Paragraph_(
            x,
            y,
            width,
            height,
            text,
            flags,
            nullptr,
            1,
            0
        );
    }

    void __cdecl Set_My_Spy_Mission_(int16_t target_player_idx, int8_t mission) {
        BILL::Set_Their_Spy_Mission_(MOX::_PLAYER_NUM, target_player_idx, mission);
    }

    void __cdecl Set_My_Spy_Number_(int16_t target_player_idx, uint8_t n_spies) {
        BILL::Set_Their_Spy_Number_(MOX::_PLAYER_NUM, target_player_idx, n_spies);
    }

    void __cdecl Set_My_Agent_Number_(uint8_t agent_num) {
        BILL::Set_Their_Spy_Number_(MOX::_PLAYER_NUM, MOX::_PLAYER_NUM, agent_num);
    }

    void __cdecl Get_Players_Alive_Or_Omniscient_(int32_t start_idx, char include_previously_contacted) {
        Get_Active_Players_(start_idx, include_previously_contacted);

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            // Do not add yourself
            if (i == MOX::_PLAYER_NUM) {
                continue;
            }

            // If the current player is omniscient, add all other alive players to the active list
            if (HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM)) {
                if (MOX::_player[i].eliminated == 0) {
                    bool is_found = false;
                    for (int16_t j = 0; j < MOX::_active_count; ++j) {
                        if ((uint8_t)MOX::_active_player[j] == (uint8_t)i) {
                            is_found = true;
                            break;
                        }
                    }

                    if (!is_found) {
                        int16_t index = MOX::_active_count;
                        MOX::_active_count++;
                        MOX::_active_player[index] = (uint8_t)i;
                    }
                }
            }
        }
    }

    void __cdecl Get_Players_Dead_Or_Alive_Or_Omniscient_(int32_t start_idx, char include_previously_contacted) {
        BILL::Get_Active_Players_(start_idx, include_previously_contacted);

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            // Skip the current player being updated
            if (i == MOX::_PLAYER_NUM) {
                continue;
            }

            // Logic to include players even if they are dead (eliminated) or if the current player sees all
            if (HAROLD::Player_Is_Omniscient_(MOX::_PLAYER_NUM) || MOX::_player[i].eliminated != 0) {
                bool already_in_list = false;
                for (int16_t j = 0; j < MOX::_active_count; ++j) {
                    if ((uint8_t)MOX::_active_player[j] == (uint8_t)i) {
                        already_in_list = true;
                        break;
                    }
                }

                if (!already_in_list) {
                    int16_t index = MOX::_active_count;
                    MOX::_active_count++;
                    MOX::_active_player[index] = (uint8_t)i;
                }
            }
        }
    }

    int8_t __cdecl Get_My_Spy_Mission_(int16_t target_player_idx) {
        return (uint8_t)MOX::_player[MOX::_PLAYER_NUM].spies[target_player_idx] >> 6;
    }

    void __cdecl Set_Their_Spy_Mission_(int16_t player_idx, int16_t target_player_idx, int8_t mission) {
        MOX::_player[player_idx].spies[target_player_idx] &= 0x3f;
        MOX::_player[player_idx].spies[target_player_idx] |= (mission << 6);
    }

    int16_t __cdecl Get_My_Agent_Number_() {
        int16_t player_num = MOX::_PLAYER_NUM;
        return (int16_t)(MOX::_player[player_num].spies[player_num] & 0x3f);
    }

    int16_t __cdecl Get_Their_Spy_Mission_(int16_t player_idx, int16_t target_player_idx) {
        return (int16_t)((uint8_t)MOX::_player[player_idx].spies[target_player_idx] >> 6);
    }

    void __cdecl _Print_1_Or_More_Lines_Centered_(int16_t x, int16_t y, int16_t width, const char* text, s_animation_header* bitmap) {
        int16_t max_height = _Get_Paragraph_Max_Height_(width, text);
        int16_t current_height = (int16_t)fonts::_font_header.current_height;

        int16_t new_y = y + (int16_t)((current_height - max_height) / 2);

        if (new_y < 0) {
            new_y = 0;
        }

        if (bitmap == nullptr) {
            _Print_Paragraph_(x, new_y, width, text, 2);
        } else {
            _Print_Paragraph_Bill_Aux_(x, new_y, width, text, 2, bitmap);
        }
    }

    void __cdecl Print_1_Or_More_Lines_Centered_To_Bitmap_(int32_t x, int32_t y, int32_t width, const char* str, s_animation_header* bmp) {
        _Print_1_Or_More_Lines_Centered_((int16_t)x, (int16_t)y, (int16_t)width, str, bmp);
    }

    void __cdecl Set_Plyr_Info_Btns_(int16_t btn_val) {
        MOX::_player[MOX::_PLAYER_NUM].history_btns &= 0x8F;
        MOX::_player[MOX::_PLAYER_NUM].history_btns |= (btn_val & 0x7) << 4;
    }

    void __cdecl _Print_String_Bill_(int16_t x, int16_t y, int16_t width, const char* str, uint16_t mode, uint8_t draw_flag, s_animation_header* bitmap) {
        if (width >= 10 && width <= 640 &&
            x >= 0 && x < 640 &&
            y >= 0 && y < 480 &&
            str != nullptr) 
        {
            int16_t str_width = fonts::Get_String_Width_(str);
            
            if (mode == 1 || mode == 3) {
                str_width = width;
            }

            if (draw_flag != 0) {
                if (bitmap != nullptr) {
                    mode |= 0x800;
                }

                switch (mode) {
                    case 0:
                        fonts::Print_(x, y, str);
                        break;
                    case 1:
                        fonts::Print_Right_(x + width - 1, y, str);
                        break;
                    case 2:
                        fonts::Print_Centered_(x + width / 2, y, str);
                        break;
                    case 3:
                        Print_Display_Auto_Width_(x, y, str, (int32_t)width);
                        break;
                    case 0x800:
                        fonts::Print_To_Bitmap_(x, y, str, (void*)bitmap);
                        break;
                    case 0x801:
                        Print_Right_Aligned_To_Bitmap_(x + width - 1, y, str, bitmap);
                        break;
                    case 0x802:
                        fonts::Print_Centered_To_Bitmap_(x + width / 2, y, str, bitmap);
                        break;
                    case 0x803:
                        Print_Display_To_Bitmap_Auto_Width_(x, y, str, width, bitmap);
                        break;
                }
            }

            if ((int32_t)(uint16_t)(__pp_max_width) < (int32_t)str_width) {
                __pp_max_width = str_width;
            }
        }
    }

    void __cdecl Record_History_()
    {
        uint32_t building_costs[MAX_PLAYERS];
        uint32_t fleet_history_tmp[350];
        uint32_t pop_history_tmp[350];
        uint32_t prod_history_tmp[350];
        uint32_t tech_history_tmp[350];

        COLCALC::Compute_Total_Building_Costs_((int32_t*)building_costs);

        if (MOX::_bill_savegame[2] == 0) {
            MOX::_bill_savegame[2] = 1;
        }
        if (MOX::_bill_savegame[3] == 0) {
            MOX::_bill_savegame[3] = 1;
        }
        if (MOX::_bill_savegame[4] == 0) {
            MOX::_bill_savegame[4] = 1;
        }
        if (MOX::_bill_savegame[5] == 0) {
            MOX::_bill_savegame[5] = 1;
        }

        uint16_t savegame2 = MOX::_bill_savegame[2];
        uint16_t savegame3 = MOX::_bill_savegame[3];
        uint16_t savegame4 = MOX::_bill_savegame[4];
        uint16_t savegame5 = MOX::_bill_savegame[5];

        memset(fleet_history_tmp, 0, sizeof(fleet_history_tmp));

        if (MOX::_NUM_SHIPS > 0) {
            for (int16_t ship_idx = 0; ship_idx < (int16_t)MOX::_NUM_SHIPS; ++ship_idx) {
                if (MOX::_ship[ship_idx].status < 2) {
                    int32_t owner = MOX::_ship[ship_idx].owner;
                    uint32_t size = MOX::_ship[ship_idx].d.size;
                    fleet_history_tmp[owner] += (10 << (size + 1));
                }
            }
        }

        if (MOX::_NUM_PLAYERS > 0) {
            for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                pop_history_tmp[i] = MOX::_player[i].total_pop;
                prod_history_tmp[i] = building_costs[i];
                tech_history_tmp[i] = MOX::_player[i].total_known_tech_cost;

                while (pop_history_tmp[i] / (uint32_t)savegame2 > 250) {
                    savegame2++;
                }
                while (prod_history_tmp[i] / (uint32_t)savegame3 > 250) {
                    savegame3++;
                }
                while (tech_history_tmp[i] / (uint32_t)savegame5 > 250) {
                    savegame5++;
                }
                while (fleet_history_tmp[i] / (uint32_t)savegame4 > 250) {
                    savegame4++;
                }
            }
        }

        if (MOX::_NUM_PLAYERS > 0) {
            for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                pop_history_tmp[i] /= (uint32_t)savegame2;
                prod_history_tmp[i] /= (uint32_t)savegame3;
                tech_history_tmp[i] /= (uint32_t)savegame5;
                fleet_history_tmp[i] /= (uint32_t)savegame4;
            }
        }

        if (MOX::_NUM_PLAYERS > 0) {
            for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                if (savegame2 != MOX::_bill_savegame[2] ||
                    savegame3 != MOX::_bill_savegame[3] ||
                    savegame4 != MOX::_bill_savegame[4] ||
                    savegame5 != MOX::_bill_savegame[5])
                {
                    for (int32_t j = 0; j < 350; ++j) {
                        MOX::_player[i].population_history[j] = (int8_t)(
                            ((int32_t)(uint8_t)MOX::_player[i].population_history[j] * MOX::_bill_savegame[2]) / (int32_t)savegame2
                        );
                        MOX::_player[i].production_history[j] = (int8_t)(
                            ((int32_t)(uint8_t)MOX::_player[i].production_history[j] * MOX::_bill_savegame[3]) / (int32_t)savegame3
                        );
                        MOX::_player[i].tech_history[j] = (int8_t)(
                            ((int32_t)(uint8_t)MOX::_player[i].tech_history[j] * MOX::_bill_savegame[5]) / (int32_t)savegame5
                        );
                        MOX::_player[i].fleet_history[j] = (int8_t)(
                            ((int32_t)(uint8_t)MOX::_player[i].fleet_history[j] * MOX::_bill_savegame[4]) / (int32_t)savegame4
                        );
                    }
                }
                int turn = MOX::_bill_savegame[1];
                MOX::_player[i].population_history[turn] = (int8_t)pop_history_tmp[i];
                MOX::_player[i].production_history[turn] = (int8_t)prod_history_tmp[i];
                MOX::_player[i].tech_history[turn] = (int8_t)tech_history_tmp[i];
                MOX::_player[i].fleet_history[turn] = (int8_t)fleet_history_tmp[i];
            }
        }

        MOX::_bill_savegame[2] = savegame2;
        MOX::_bill_savegame[3] = savegame3;
        MOX::_bill_savegame[4] = savegame4;
        MOX::_bill_savegame[5] = savegame5;

        MOX::_bill_savegame[1]++;
        if (MOX::_bill_savegame[1] >= 350) {
            MOX::_bill_savegame[1] = 0;
        }
    }

    int16_t __cdecl Get_History_Params_(int16_t* out_history_length) {
        int16_t history_param = (int16_t)MOX::_stardate + 30536;

        if (history_param > 350) {
            *out_history_length = 350;
            return MOX::_bill_savegame[1];
        }

        *out_history_length = history_param;
        return 0;
    }

    void __cdecl Print_Display_Auto_Width_(int16_t x, int16_t y, const char* text, int32_t width) {
        if (width < 0) {
            width = 0;
        }
        fonts::Print_Display_(x, y, text, width);
    }

    void __cdecl Get_Active_Players_(int32_t start_idx, int8_t include_previously_contacted) {
        if (MOX::_NUM_PLAYERS > 0) {
            for (int16_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
                if (i != MOX::_PLAYER_NUM) {
                    if (MOX::_player[i].eliminated == 0) {
                        if (MOX::_player[MOX::_PLAYER_NUM].contact[i] != 0) {
                            MOX::_active_player[start_idx] = (uint8_t)i;
                            start_idx++;
                        }
                    }
                }
            }
        }

        if (include_previously_contacted != 0) {
            if (MOX::_NUM_PLAYERS > 0) {
                for (int16_t i = 0; i < MOX::_NUM_PLAYERS; i++) {
                    if (i != MOX::_PLAYER_NUM) {
                        if (MOX::_player[i].eliminated == 0) {
                            if (MOX::_player[MOX::_PLAYER_NUM].contact[i] == 0) {
                                if (MOX::_player[MOX::_PLAYER_NUM].n_times_established_contact[i] != 0) {
                                    MOX::_active_player[start_idx] = (uint8_t)i;
                                    start_idx++;
                                }
                            }
                        }
                    }
                }
            }
        }

        MOX::_active_count = (int16_t)start_idx;
    }

    void __cdecl Print_Right_Aligned_To_Bitmap_(int16_t x, int16_t y, const char* str, s_animation_header* bitmap) {
        int16_t width = fonts::Get_String_Width_(str);
        fonts::Print_To_Bitmap_(x - (width - 1), y, str, (void*)bitmap);
    }

    void __cdecl Print_Display_To_Bitmap_Auto_Width_(int16_t x, int16_t y, const char* text, int16_t width, s_animation_header* bitmap) {
        if (width < 0) {
            width = 0;
        }
        fonts::Print_Display_To_Bitmap_(x, y, text, (int32_t)width, bitmap);
    }

    void __cdecl Turn_Off_Palettes_(int16_t start, int16_t end) {
        palstore::Darken_Palette_(100);
        palette::Clear_Palette_Changes_(0, 255);
    }

    void __cdecl _Print_Paragraph_Bill_Aux_(int16_t x, int16_t y, int16_t width, const char* text, int16_t color, s_animation_header* bitmap) {
        _Print_Paragraph_Bill_(x, y, width, text, (uint16_t)color, 1, bitmap);
    }

    void __cdecl _Print_Paragraph_Bill_(int16_t x, int16_t y, int16_t width, const char* text, uint16_t color, uint8_t draw_flag, s_animation_header* bitmap) {
        if (width >= 10 && width <= 0x280 && x >= 0 && x < 0x280 && y >= 0 && y < 0x1E0 && text != nullptr) {
            int16_t char_index = 0;
            char break_char = 0;
            __pp_max_width = 0;
            int16_t out_index = 0;
            int16_t current_width = 0;
            int16_t last_break_char_index = 0;
            int16_t last_break_out_index = 0;
            int16_t current_y = y;
            int16_t max_y = 0x1DF - (int16_t)fonts::_font_header.current_height;
            char wrapped_line[200];

            char current_char = text[0];
            while (current_char != '\0') {
                if (current_char == '\t') {
                    current_char = ' ';
                }

                if (current_char == ' ' || (current_char == '\b' && (current_width + fonts::_font_header.font_style_widths['-'] <= width))) {
                    last_break_char_index = char_index;
                    break_char = current_char;
                    last_break_out_index = out_index;
                }

                bool process_line_break = false;

                if (current_char == '\r' || current_char == '\n') {
                    break_char = '\r';
                    process_line_break = true;
                } else if ((uint8_t)current_char >= 0x20) {
                    current_width += fonts::_font_header.font_style_widths[(uint8_t)current_char];

                    if (current_char != ' ' && current_width > width) {
                        if (last_break_out_index != 0) {
                            out_index = last_break_out_index;
                        }

                        if (last_break_char_index != 0) {
                            char_index = last_break_char_index;
                        }
                        process_line_break = true;
                    } else {
                        char_index++;
                        wrapped_line[out_index] = current_char;
                        out_index++;
                        current_width += fonts::_font_header.font_style_spacing;
                    }
                } else {
                    char_index++;
                }

                if (process_line_break) {
                    if (out_index > 0 && wrapped_line[out_index - 1] == ' ') {
                        while (out_index > 1) {
                            out_index--;
                            if (wrapped_line[out_index - 1] != ' ') {
                                break;
                            }
                        }
                    }

                    if (out_index > 0 || break_char == '\r') {
                        if (break_char == '\b') {
                            wrapped_line[out_index] = '-';
                            out_index++;
                        } else if ((uint8_t)break_char <= 0x20) {
                            char_index++;
                        }

                        wrapped_line[out_index] = '\0';
                        _Print_String_Bill_(x, current_y, width, wrapped_line, color, draw_flag, bitmap);

                        current_y += (int16_t)fonts::_font_header.line_height;
                        out_index = 0;
                        if (current_y > max_y) {
                            break;
                        }
                    }

                    current_width = 0;
                    break_char = 0;
                    last_break_char_index = 0;
                    last_break_out_index = 0;
                }

                current_char = text[char_index];
            }

            wrapped_line[out_index] = '\0';
            if (out_index > 0) {
                if (color == 3) {
                    color = 0;
                }
                _Print_String_Bill_(x, current_y, width, wrapped_line, color, draw_flag, bitmap);
            }

            __pp_max_height = current_y - y + (int16_t)fonts::_font_header.current_height;
        }
    }

char* __cdecl Cache_Player_Trooper_Anim_() {
    char found_path[52];
    char source_path[128];

    snprintf(
        BILL::_dest_file_name,
        sizeof(BILL::_dest_file_name),
        SCIENCE::_trooper_anim_fmt,
        (unsigned int)(uint8_t)(MOX::_player[MOX::_PLAYER_NUM].race)
    );

    if (dos::DIR_(BILL::_dest_file_name, found_path) == 0) {
        if (dos::DIR_(SCIENCE::_trooper_anim_find_pat, found_path) != 0) {
            dos::Delete_File_(found_path);
        }

        strlcpy(source_path, farload::_secondary_drive_path, sizeof(source_path));
        strlcat(source_path, BILL::_dest_file_name, sizeof(source_path));
        BILL::Copy_File_To_File_(source_path, BILL::_dest_file_name);
    }

    return BILL::_dest_file_name;
}
}