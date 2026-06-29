#include "pch.h"

namespace RUSS {
    int8_t _all_ai_game;
    char _russ_strings[0x1964];
    const char s_Seed_ld[] = "Seed: %ld";
    const char s_Map_d[] = "Map:  %d";

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

        uint16_t r = (uint16_t)game_random::Random_(total_weight);
        int16_t index = 0;

        if (weights[index] < r) {
            do {
                r -= (uint16_t)weights[index];
                index++;
            } while (weights[index] < r);
        }

        return index;
    }

    int16_t __cdecl Get_Weighted_Choice_Short_(const uint16_t* weights, int16_t count) {
        uint32_t total_weight = 0;

        if (count > 0) {
            for (int16_t i = 0; i < count; ++i) {
                total_weight += (uint16_t)weights[i];
            }
        }

        if (total_weight == 0) {
            return -1;
        }

        uint32_t random_val = (uint32_t)game_random::Random_((int32_t)total_weight);
        int16_t index = 0;

        if ((uint16_t)weights[index] < random_val) {
            do {
                uint32_t current_weight = (uint16_t)weights[index];
                index++;
                random_val -= current_weight;
            } while ((uint16_t)weights[index] < random_val);
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

        uint32_t roll = (uint32_t)game_random::Random_((int32_t)total_weight);
        int16_t selected_index = 0;

        if (weights[0] < roll) {
            do {
                roll -= weights[selected_index];
                ++selected_index;
            } while (weights[selected_index] < roll);
        }

        return selected_index;
    }

    void __cdecl Release_All_Blocks_(void* segment) {
        allocate::Allocate_First_Block_(segment, 1);
    }

    void __cdecl Load_Russ_Strings_() {
        char filename_buffer[20];

        uint8_t language = MOX::_settings.language;

        snprintf(filename_buffer, sizeof(filename_buffer), "rstring%d.lbx", (uint32_t)language);

        farload::Farload_Data_Static_(filename_buffer, 0, &RUSS::_russ_strings, 0, 1, (int32_t)sizeof(RUSS::_russ_strings));

        char* string_ptr;
        string_ptr = COLCALC::Load_Colcalc_Strings_(RUSS::_russ_strings);
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

            if (MOX::_player[MOX::_PLAYER_NUM].objectives == PLAYER_OBJECTIVE_HUMAN) {
                Make_Player_Into_AI_((int32_t)MOX::_PLAYER_NUM);
            }

            NETMOX::_net.all_ai_game_sync_flag = 1;
        } else {
            GENDRAW::Message_Box_((char*)"This isn't a single player game, cheeky monkey!");
        }
    }

    void __cdecl Shuffle_Char_(int8_t* buffer, int16_t length) {
        int16_t index = 0;

        if (length > 0) {
            do {
                int16_t current_index = index;
                int32_t random_offset = game_random::Random_(length - current_index);
                int16_t swap_index = (int16_t)(index + random_offset - 1);
                ++index;

                int8_t swapped_value = buffer[current_index];
                buffer[current_index] = buffer[swap_index];
                buffer[swap_index] = swapped_value;
            } while (index < length);
        }
    }

    void __cdecl Shuffle_Int_(int32_t* array, int16_t count) {
        if (count <= 0) {
            return;
        }

        for (int16_t i = 0; i < count; ++i) {
            int32_t random_offset = game_random::Random_((int32_t)count - i);
            int16_t target_idx = (int16_t)(i + random_offset - 1);

            int32_t swapped_value = array[i];
            array[i] = array[target_idx];
            array[target_idx] = swapped_value;
        }
    }

    void __cdecl Shuffle_Sint_(int16_t* data, int16_t count) {
        if (count <= 0) {
            return;
        }

        for (int16_t i = 0; i < count; ++i) {
            int32_t random_offset = game_random::Random_((int32_t)count - i);

            int16_t swapped_value = data[i];
            int16_t target_idx = (int16_t)(i + random_offset - 1);

            data[i] = data[target_idx];
            data[target_idx] = swapped_value;
        }
    }

    int16_t __cdecl Star_To_Monster_(int16_t star_index) {
        return (int16_t)(star_index % 5 + 1);
    }

    char* __cdecl Star_Name_(int32_t star_index) {
        if (star_index < MOX::_NUM_STARS) {
            return MOX::_star[star_index].name;
        }
        return (char*)"Antares";
    }

    void __cdecl Shuffle_(void* buffer, int16_t element_size, int16_t num_elements) {
        if (element_size == 1) {
            RUSS::Shuffle_Char_((int8_t*)buffer, num_elements);
            return;
        }

        if (element_size == 2) {
            RUSS::Shuffle_Sint_((int16_t*)buffer, num_elements);
            return;
        }

        if (element_size == 4) {
            RUSS::Shuffle_Int_((int32_t*)buffer, num_elements);
            return;
        }

        if (num_elements > 0) {
            uint8_t swap_buffer[128];
            for (int16_t i = 0; i < num_elements; ++i) {
                int32_t random_offset = game_random::Random_((int32_t)num_elements - i);
                int16_t j = i + (int16_t)random_offset - 1;

                if (j != i) {
                    uint8_t* element_i = (uint8_t*)buffer + (int32_t)i * (int32_t)element_size;
                    uint8_t* element_j = (uint8_t*)buffer + (int32_t)j * (int32_t)element_size;

                    memcpy(swap_buffer, element_i, element_size);
                    memcpy(element_i, element_j, element_size);
                    memcpy(element_j, swap_buffer, element_size);
                }
            }
        }
    }

    void __cdecl Mox_Update_() {
        platform::Check_Exit_();
        sound::Sound_Update_();
        NETMOX::Mox_Net_Update_();
    }

    void __cdecl Mox_Sync_Update_() {
        Mox_Update_();
        platform::Toggle_Pages_Stub_();
    }

    void __cdecl Make_Player_Into_AI_(int32_t player_idx) {
        MOX::_player[player_idx].objectives = (uint8_t)game_random::Random_(5);

        INITSHIP::Update_Player_Ship_Designs_((int16_t)player_idx);

        for (int16_t i = 0; i < (int16_t)MOX::_NUM_COLONIES; ++i) {
            if (MOX::_colony[i].owner == player_idx) {
                COLBLDG::Clear_Colony_Queue_(i);
            }
        }

        COLCALC::Do_Player_Colony_Calculations_((int16_t)player_idx);
    }

    int32_t __cdecl Compare_Ratios_(uint32_t first_numerator, uint32_t first_denominator, uint32_t second_numerator, uint32_t second_denominator) {
        int32_t comparison = 2;

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

    void __cdecl PFMT_(int32_t unused1, int32_t unused2, const char* unused_str, ...) {
    }
}