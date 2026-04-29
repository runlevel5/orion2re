#include "pch.h"

namespace RUSS {
    void __cdecl Assert_Settings_() {
    }

    int16_t __cdecl Get_Weighted_Choice_Char_(uint8_t* weights, int16_t count) {
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

    int16_t __cdecl Get_Weighted_Choice_Short_(const uint16_t* weights, int16_t count) {
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

    int16_t __cdecl Get_Weighted_Choice_Int_(uint32_t* weights, int16_t count) {
        uint32_t total_weight = 0;
        for (int16_t i = 0; i < count; ++i) {
            total_weight += weights[i];
        }

        if (total_weight == 0) {
            return -1;
        }

        uint32_t roll = static_cast<uint32_t>(random::Random_(static_cast<int32_t>(total_weight)));
        int16_t choice = 0;

        if (weights[0] < roll) {
            do {
                roll -= weights[choice];
                ++choice;
            } while (weights[choice] < roll);
        }

        return choice;
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
            RUSS::_all_ai_game = 1;

            if (MOX::_player[MOX::_PLAYER_NUM].objectives == 0x64 /* 'd' */) {
                Make_Player_Into_AI_(static_cast<int32_t>(MOX::_PLAYER_NUM));
            }

            _all_ai_game = 1;
        } else {
            GENDRAW::Message_Box_(const_cast<char*>(s_This_isnt_a_single_player_game));
        }
    }

    void __cdecl Shuffle_Char_(int8_t* buffer, int16_t length) {
        int16_t index = 0;

        if (length > 0) {
            do {
                int current = index;
                int random_value = random::Random_(length - current);
                int16_t swap_index = static_cast<int16_t>(index + random_value - 1);
                ++index;

                int8_t temp = buffer[current];
                buffer[current] = buffer[swap_index];
                buffer[swap_index] = temp;
            } while (index < length);
        }
    }

    void __cdecl Shuffle_Int_(int32_t* array, int16_t count) {
        if (count <= 0) {
            return;
        }

        // Standard Fisher-Yates shuffle implementation.
        // Note: random::Random_(n) in this context is assumed to return a value in the range [1, n].
        for (int16_t i = 0; i < count; ++i) {
            int32_t r = random::Random_(static_cast<int32_t>(count) - i);
            int16_t target_idx = static_cast<int16_t>(i + r - 1);

            int32_t temp = array[i];
            array[i] = array[target_idx];
            array[target_idx] = temp;
        }
    }

    void __cdecl Shuffle_Sint_(int16_t* data, int16_t count) {
        if (count <= 0) {
            return;
        }

        for (int16_t i = 0; i < count; ++i) {
            // Note: The logic suggests random::Random_(n) returns a value in range [1, n].
            int r = random::Random_(static_cast<int>(count) - i);

            int16_t temp = data[i];

            // target_idx = i + r - 1
            // If Random_(n) is 1-based, this maps [1, count-i] to [i, count-1].
            int16_t target_idx = static_cast<int16_t>(i + r - 1);

            data[i] = data[target_idx];
            data[target_idx] = temp;
        }
    }

    int16_t __cdecl Star_To_Monster_(int16_t star_index) {
        return static_cast<int16_t>(star_index % 5 + 1);
    }

    char* __cdecl Star_Name_(int32_t star_idx) {
        if (star_idx < MOX::_NUM_STARS) {
            return MOX::_star[star_idx].name;
        }
        return const_cast<char*>(s_Antares_00551288);
    }

    void __cdecl Shuffle_(void* buffer, int16_t element_size, int16_t num_elements) {
        if (element_size == 1) {
            RUSS::Shuffle_Char_(static_cast<int8_t*>(buffer), num_elements);
            return;
        }

        if (element_size == 2) {
            RUSS::Shuffle_Sint_(static_cast<int16_t*>(buffer), num_elements);
            return;
        }

        if (element_size == 4) {
            RUSS::Shuffle_Int_(static_cast<int32_t*>(buffer), num_elements);
            return;
        }

        if (num_elements > 0) {
            uint8_t temp[128]; // Scratch buffer for element swapping
            for (int16_t i = 0; i < num_elements; ++i) {
                // Random_ returns a value in range [1, range]
                int r = random::Random_(static_cast<int>(num_elements) - static_cast<int>(i));
                int16_t j = i + static_cast<int16_t>(r) - 1;

                if (j != i) {
                    uint8_t* p_i = static_cast<uint8_t*>(buffer) + static_cast<int>(i) * static_cast<int>(element_size);
                    uint8_t* p_j = static_cast<uint8_t*>(buffer) + static_cast<int>(j) * static_cast<int>(element_size);

                    memcpy(temp, p_i, element_size);
                    memcpy(p_i, p_j, element_size);
                    memcpy(p_j, temp, element_size);
                }
            }
        }
    }

    void __cdecl Mox_Update_() {
        sound::Sound_Update_();
        NETMOX::Mox_Net_Update_();
    }

    void __cdecl Mox_Sync_Update_() {
        Mox_Update_();
        platform::Toggle_Pages_Stub_();
    }

    void __cdecl Make_Player_Into_AI_(int32_t player_idx) {
        MOX::_player[player_idx].objectives = static_cast<uint8_t>(random::Random_(5));

        INITSHIP::Update_Player_Ship_Designs_(static_cast<int16_t>(player_idx));

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_COLONIES; ++i) {
            if (MOX::_colony[i].owner == player_idx) {
                COLBLDG::Clear_Colony_Queue_(i);
            }
        }

        COLCALC::Do_Player_Colony_Calculations_(static_cast<int16_t>(player_idx));
    }

    int __cdecl Compare_Ratios_(uint32_t first_numerator, uint32_t first_denominator, uint32_t second_numerator, uint32_t second_denominator) {
        int comparison = 2;

        do {
            uint32_t saved_first_numerator = first_numerator;

            if (first_numerator == 0) {
                comparison = (second_numerator != 0) ? -1 : 0;
            } else if (second_numerator == 0) {
                comparison = 1;
            } else if (first_denominator > first_numerator) {
                if (second_denominator <= second_numerator) {
                    comparison = -1;
                } else {
                    uint32_t second_quotient = second_denominator / second_numerator;
                    uint32_t first_quotient = first_denominator / first_numerator;

                    if (first_quotient > second_quotient) {
                        comparison = -1;
                    } else if (first_quotient < second_quotient) {
                        comparison = 1;
                    } else {
                        uint32_t second_remainder = second_denominator % second_numerator;
                        uint32_t first_remainder = first_denominator % first_numerator;

                        first_numerator = second_remainder;
                        first_denominator = second_numerator;
                        second_numerator = first_remainder;
                        second_denominator = saved_first_numerator;
                    }
                }
            } else {
                if (second_denominator > second_numerator) {
                    comparison = 1;
                } else {
                    uint32_t first_quotient = first_numerator / first_denominator;
                    uint32_t second_quotient = second_numerator / second_denominator;

                    if (second_quotient > first_quotient) {
                        comparison = -1;
                    } else if (second_quotient < first_quotient) {
                        comparison = 1;
                    } else {
                        first_numerator %= first_denominator;
                        second_numerator %= second_denominator;
                    }
                }
            }
        } while (comparison == 2);

        return comparison;
    }
}