#include "pch.h"

namespace BILL {
    /**
     * @address 004077e0
     */
    void __cdecl Copy_File_To_File_(const char* src_path, const char* dst_path) {
        char msg_buf[128];
        char read_buffer[4096];

        FILE* src_fp = fopen(src_path, s_rb);
        FILE* dst_fp = fopen(dst_path, s_wb);

        if (src_fp == nullptr) {
            _sprintf(msg_buf, s_Could_not_open_s, src_path);
            exitm::Exit_With_Message(msg_buf);
        }

        if (dst_fp == nullptr) {
            _sprintf(msg_buf, s_Could_not_open_s, dst_path);
            exitm::Exit_With_Message(msg_buf);
        }

        int32_t total_bytes_read = 0;
        size_t current_chunk_size;

        do {
            current_chunk_size = fread(read_buffer, 1, 4096, src_fp);
            total_bytes_read += (int32_t)current_chunk_size;

            MOX::Mox_Update_();

            size_t written = fwrite(read_buffer, 1, current_chunk_size, dst_fp);
            if (written != current_chunk_size) {
                _sprintf(msg_buf, s_Error_copying_s_to_s_bytes_i, src_path, dst_path, total_bytes_read);
                exitm::Exit_With_Message(msg_buf);
            }
        } while (current_chunk_size == 4096);

        fflush(dst_fp);
        fclose(dst_fp);
        fclose(src_fp);
    }

    void __cdecl Cache_Player_Scientist_Anim_() {
        Cache_Player_Scientist_Anim_Imp_(MOX::_PLAYER_NUM);
    }

    char* __cdecl Cache_Player_Scientist_Anim_Imp_(int16_t player_index) {
        char dir_entry[52];
        char source_path[128];

        s_player* player = &MOX::_player[player_index];

        _sprintf(BILL::_dest_file_name, s_SR_R_x_SC_LBX, (uint32_t)(uint8_t)player->race);

        if (!dos::DIR_(BILL::_dest_file_name, dir_entry)) {
            if (dos::DIR_(s_SR_R_wildcard_SC_LBX, dir_entry)) {
                dos::resolve_wildcard(dir_entry);
            }

            char* d = source_path;
            char* s = farload::_secondary_drive_path;
            while ((*d++ = *s++) != '\0');
            d--;
            s = BILL::_dest_file_name;
            while ((*d++ = *s++) != '\0');

            Copy_File_To_File_(source_path, BILL::_dest_file_name);
        }

        return BILL::_dest_file_name;
    }

    int16_t __cdecl Get_Their_Spy_Number_(int16_t player_idx, int16_t target_idx) {
        return static_cast<int16_t>(MOX::_player[player_idx].spies[target_idx] & 0x3F);
    }

    /**
     * @brief Updates the spy count for a specific target player.
     * 
     * This function modifies the lower 6 bits of the spy count byte while 
     * preserving the upper 2 bits (bits 6-7).
     * 
     * @param player_idx The index of the player who owns the spies.
     * @param target_player_idx The index of the player being spied upon.
     * @param count The new spy count (masked to 0-63).
     */
    void __cdecl Set_Their_Spy_Number_(int16_t player_idx, int16_t target_player_idx, uint8_t count) {
        // s_player size: 0xEA9 (3753 bytes)
        // spies offset: 0xE57
        
        // Clear lower 6 bits (intermediate write observed in assembly)
        MOX::_player[player_idx].spies[target_player_idx] &= 0xC0;
        
        // Set new count in lower 6 bits
        MOX::_player[player_idx].spies[target_player_idx] |= (count & 0x3F);
    }

    int16_t __cdecl _Get_Paragraph_Max_Width_(int16_t width, char* text, int16_t height) {
        _Print_Paragraph_Bill_(0, 0, width, text, height, 0, nullptr);
        return __pp_max_width;
    }

    void __cdecl Squeeze_Print_Centered_(int16_t x, int16_t y, const char* str, int16_t width) {
        _Squeeze_Print_(x - (width / 2), y, str, width, nullptr, 2, true);
    }

    /**
     * @address 00407450
     */
    void __cdecl Squeeze_Centered_Print_(uint16_t x, int16_t y, char* text, uint16_t limit) {
        _Squeeze_Print_((int16_t)x, y, text, (int16_t)limit, nullptr, 2, true);
    }

    /**
     * @address 004073e0
     */
    void __cdecl Squeeze_Print_(int16_t x, int16_t y, const char* text, int16_t max_width) {
        _Squeeze_Print_(x, y, text, max_width, nullptr, 0, true);
    }

    /**
     * @address 00407210
     */
    void __cdecl _Squeeze_Print_(int16_t x, int16_t y, const char* str, int16_t limit_w, void* p5, int16_t p6, bool adjust_y) {
        bool resized = false;
        int16_t current_width = fonts::Get_String_Width_(str);

        if (current_width <= limit_w) {
            _Print_String_Bill_(x, y, limit_w, str, p6, 1, p5);
            return;
        }

        uint8_t orig_space_width = (uint8_t)fonts::_font_header.font_style_widths[32];
        int16_t orig_spacing = fonts::_font_header.font_style_spacing;
        uint16_t orig_current_font_style = fonts::_current_font_style;
        int16_t orig_font_style = fonts::_font_header.font_style;
        int16_t y_offset = 0;
        int16_t initial_height = fonts::Get_Font_Height_();

        while (true) {
            while ((uint8_t)fonts::_font_header.font_style_widths[32] > BILL::_space_limits[fonts::_current_font_style]) {
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
                _Print_String_Bill_(x, y + y_offset, limit_w, str, p6, 1, p5);

                fonts::_font_header.font_style_spacing = orig_spacing;
                fonts::_font_header.font_style_widths[32] = (char)orig_space_width;

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

    /**
     * @address 00407650
     */
    void __cdecl Squeeze_Print_Paragraph_(uint16_t param_1, int16_t param_2, int16_t param_3, int16_t param_4, char* param_5, uint16_t param_6) {
        // Calls the internal version with default terminal parameters
        _Squeeze_Print_Paragraph_(param_1, param_2, param_3, param_4, param_5, param_6, nullptr, 0, 0);
    }

    /**
     * @address 004067b0
     */
    void __cdecl Bill_Default_(void) {
        if (MOX::_settings.game_difficulty < 2) {
            BILL::Set_Plyr_Info_Btns_(4);
        }
        FMTPARA::Init_();
    }

    /**
     * @address 004076b0
     */
    void __cdecl Squeeze_Paragraph_Centered_(int16_t x, int16_t y, int16_t width, int16_t height, char* text, int16_t color) {
        // Centering logic: y_offset = height / 2
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

    /**
     * @address 00407680
     */
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

    /**
     * @address 00407140
     */
    void __cdecl Set_My_Spy_Mission_(int16_t target_player_idx, int8_t mission) {
        BILL::Set_Their_Spy_Mission_(MOX::_PLAYER_NUM, target_player_idx, mission);
    }

    /**
     * @address 00407120
     */
    void __cdecl Set_My_Spy_Number_(int16_t target_player_idx, uint8_t n_spies) {
        BILL::Set_Their_Spy_Number_(MOX::_PLAYER_NUM, target_player_idx, n_spies);
    }

    /**
     * @address 00407160
     */
    void __cdecl Set_My_Agent_Number_(uint8_t agent_num) {
        BILL::Set_Their_Spy_Number_(MOX::_PLAYER_NUM, MOX::_PLAYER_NUM, agent_num);
    }
}