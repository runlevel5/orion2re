#include "pch.h"

namespace RUSS {
    int16_t __cdecl Get_Weighted_Choice_Char_(int8_t* weights, int16_t count) {
        uint32_t total_weight = 0;
        for (int16_t i = 0; i < count; ++i) {
            total_weight += weights[i];
        }

        if (total_weight == 0) {
            return -1;
        }

        uint16_t r = static_cast<uint16_t>(random::Random_(total_weight));
        int16_t index = 0;

        // Cumulative sum check to determine which index the random value falls into.
        if (weights[index] < r) {
            do {
                r -= static_cast<uint16_t>(weights[index]);
                index++;
            } while (weights[index] < r);
        }

        return index;
    }

    int16_t __cdecl Get_Weighted_Choice_Short_(const int16_t* weights, int16_t count) {
        uint32_t total_weight = 0;

        // Calculate the sum of all weights
        if (count > 0) {
            for (int16_t i = 0; i < count; ++i) {
                total_weight += static_cast<uint16_t>(weights[i]);
            }
        }

        if (total_weight == 0) {
            return -1;
        }

        // Obtain a random target value from [0, total_weight - 1] 
        // (Assuming Random_ behaves as such, or [1, total_weight])
        uint32_t random_val = static_cast<uint32_t>(random::Random_(static_cast<int32_t>(total_weight)));
        int16_t index = 0;

        // Perform weighted selection
        // Logic: Return the first index such that the sum of weights up to that index 
        // satisfies the random_val threshold.
        if (static_cast<uint16_t>(weights[index]) < random_val) {
            do {
                uint32_t current_weight = static_cast<uint16_t>(weights[index]);
                index++;
                random_val -= current_weight;
            } while (static_cast<uint16_t>(weights[index]) < random_val);
        }

        return index;
    }

    /**
     * @brief Releases all allocated blocks within a memory segment by resetting it.
     * 
     * This function effectively resets the memory segment provided, allowing it 
     * to be reused from the beginning. It calls the underlying allocation system 
     * to reset the segment.
     * 
     * @param seg Pointer to the memory segment to be released/reset.
     */
    void __cdecl Release_All_Blocks_(void* seg) {
        allocate::Allocate_First_Block_(seg, 1);
    }

    void __cdecl Load_Russ_Strings_() {
        char filename_buffer[20];

        uint8_t language = MOX::_settings_language;

        _sprintf(filename_buffer, s_rstring_d_lbx, (uint32_t)language);

        farload::Farload_Data_Static_(filename_buffer, 0, &RUSS::_russ_strings, 0, 1, 0x1964);

        char* string_ptr;
        string_ptr = COLCALC::Load_Colcalc_Strings_(&RUSS::_russ_strings);
        string_ptr = COMBFIND::Load_Combfind_Strings_(string_ptr);
        string_ptr = MSG_::Load_Msg_Strings_(string_ptr);
        string_ptr = NETMOX::Load_Netmox_Strings_(string_ptr);
        string_ptr = NETSTART::Load_Netstart_Strings_(string_ptr);
        string_ptr = NETTURN::Load_Netturn_Strings_(string_ptr);
        REPORT::Load_Report_Strings_(string_ptr);
    }

    void __cdecl Make_All_AI_Game_() {
        if (MOX::_game_type == 0) {
            MOX::_all_ai_game = 1;

            if (MOX::_player[MOX::_PLAYER_NUM].objectives == 0x64 /* 'd' */) {
                Make_Player_Into_AI_(static_cast<int32_t>(MOX::_PLAYER_NUM));
            }

            _all_ai_game = 1;
        } else {
            GENDRAW::Message_Box_(const_cast<char*>(s_This_isnt_a_single_player_game));
        }
    }
}