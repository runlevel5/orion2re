#include "deps.h"
#include <cstdio>

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
     * Sets the number of spies for a specific player against a target player.
     * The spy count is packed into the lower 6 bits of the spies byte, 
     * preserving the upper 2 bits (likely used for flags).
     * 
     * @param player_idx Index of the player whose spies are being set
     * @param target_idx Index of the target player the spies are spying on
     * @param n_spies The number of spies (only the lower 6 bits are used)
     */
    void __cdecl Set_Their_Spy_Number_(int16_t player_idx, int16_t target_idx, uint8_t n_spies) {
        // Offset 0xE57 is the 'spies' field within s_player
        int8_t& spy_data = MOX::_player[player_idx].spies[target_idx];

        // Preserve bits 6-7, clear bits 0-5, then set bits 0-5 from n_spies
        uint8_t flags = static_cast<uint8_t>(spy_data) & 0xC0;
        uint8_t packed_count = n_spies & 0x3F;

        spy_data = static_cast<int8_t>(flags | packed_count);
    }

    int16_t __cdecl _Get_Paragraph_Max_Width_(int16_t width, char* text, int16_t height) {
        _Print_Paragraph_Bill_(0, 0, width, text, height, 0, nullptr);
        return __pp_max_width;
    }

    void __cdecl Squeeze_Print_Centered_(int16_t x, int16_t y, const char* str, int16_t width) {
        _Squeeze_Print_(x - (width / 2), y, str, width, nullptr, 2, 1);
    }
}