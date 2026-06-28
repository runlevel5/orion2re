#include "pch.h"

namespace newtech {
    int16_t _best_category[12];

    int32_t _tech_research_level_values[30] = {
        0, 1, 3, 6, 12, 22, 36, 62, 102, 172, 292, 492,
        812, 1372, 2372, 3972, 6372, 9972, 10400, 10800, 11200, 11600, 12000, 12400,
        12800, 13200, 13600, 14000, 14400, 14800,
    };

    int16_t __cdecl Get_Competition_Tech_Values_(int16_t target_ai_group, int16_t exclude_player_idx) {
        int16_t max_ai_group = 0;

        if (MOX::_NUM_PLAYERS > 0) {
            for (int player_idx = 0; player_idx < (int)MOX::_NUM_PLAYERS; ++player_idx) {
                if (player_idx == (int)exclude_player_idx) {
                    continue;
                }

                s_player* player = &MOX::_player[player_idx];

                for (int app_idx = 0; app_idx < 75; ++app_idx) {
                    if (TECHDATA::_technology_applications[app_idx].ai_group == target_ai_group) {
                        if (player->tech_applications[app_idx] == TECH_RESEARCH_STATUS_RESEARCHED) {
                            int16_t field_id = TECHDATA::_technology_applications[app_idx].tech_field_id;
                            uint8_t field_ai_group = TECHDATA::_technology_fields[field_id].ai_group;

                            if (max_ai_group < (int16_t)field_ai_group) {
                                max_ai_group = (int16_t)field_ai_group;
                            }
                        }
                    }
                }
            }
        }

        return max_ai_group;
    }

    void __cdecl Set_Competition_Tech_Values_(int16_t player_idx) {
        _best_category[0] = Get_Competition_Tech_Values_(0x19, player_idx);
        _best_category[1] = Get_Competition_Tech_Values_(0x12, player_idx);
        _best_category[2] = Get_Competition_Tech_Values_(0x0a, player_idx);
        _best_category[3] = Get_Competition_Tech_Values_(0x13, player_idx);
        _best_category[4] = Get_Competition_Tech_Values_(0x0c, player_idx);
        _best_category[5] = Get_Competition_Tech_Values_(0x10, player_idx);
        _best_category[6] = Get_Competition_Tech_Values_(0x0f, player_idx);
        _best_category[7] = Get_Competition_Tech_Values_(0x1c, player_idx);
        _best_category[8] = Get_Competition_Tech_Values_(0x18, player_idx);
        _best_category[9] = Get_Competition_Tech_Values_(0x1a, player_idx);
        _best_category[10] = Get_Competition_Tech_Values_(0x15, player_idx);
        _best_category[11] = Get_Competition_Tech_Values_(0x21, player_idx);
    }

    int32_t __cdecl Get_Competition_Player_Tech_Value_(int16_t player_idx, uint16_t target_ai_group) {
        int32_t max_field_ai_group = 0;

        for (int i = 0; i < 75; ++i) {
            if (TECHDATA::_technology_applications[i].ai_group == target_ai_group) {
                if (MOX::_player[player_idx].tech_applications[i] == TECH_RESEARCH_STATUS_RESEARCHED) {
                    int16_t tech_field_id = TECHDATA::_technology_applications[i].tech_field_id;
                    uint8_t field_ai_group = TECHDATA::_technology_fields[tech_field_id].ai_group;

                    if (field_ai_group > max_field_ai_group) {
                        max_field_ai_group = (int16_t)field_ai_group;
                    }
                }
            }
        }

        return max_field_ai_group;
    }

    int32_t __cdecl Calc_Tech_Value_(int16_t player_idx, int16_t tech_app_idx) {
        if (!COLCALC::Player_May_Get_Tech_App_(&MOX::_player[player_idx], tech_app_idx)) {
            return 0;
        }

        if (MOX::_player[player_idx].tech_applications[tech_app_idx] == TECH_RESEARCH_STATUS_RESEARCHED) {
            return 0;
        }

        if (MOX::_player[player_idx].objectives != 100 && tech_app_idx == 52) {
            return 0;
        }

        uint8_t ai_group = TECHDATA::_technology_applications[tech_app_idx].ai_group;
        int category_is_multi = MOX::_tech_categories[ai_group][1];

        int tech_field_ai_group = TECHDATA::_technology_fields[TECHDATA::_technology_applications[tech_app_idx].tech_field_id].ai_group;
        if (tech_field_ai_group > 22) {
            tech_field_ai_group = 22;
        }

        int base_value = newtech::_tech_research_level_values[tech_field_ai_group];
        int modifier = MOX::_tech_categories[ai_group][0];

        switch (MOX::_player[player_idx].ship_design_theme) {
            case 0:
                if (ai_group == 0x1A || ai_group == 0x1B || ai_group == 0x19) {
                    modifier = 100;
                }
                break;
            case 1:
                if (ai_group == 0x1A) modifier = 50;
                else if (ai_group == 0x13 || ai_group == 0x1E) modifier = 100;
                else if (ai_group == 0x19) modifier = 20;
                break;
            case 2:
                if (ai_group == 0x15 || ai_group == 0x1D || ai_group == 0x18) modifier = 100;
                break;
            case 3:
                if (ai_group == 0x24) modifier = 50;
                else if (ai_group == 0x26) modifier = 100;
                break;
        }

        switch (MOX::_player[player_idx].ship_special_theme) {
            case 0:
                if (ai_group == 0x12) modifier = 50;
                else if (ai_group == 0x17) modifier = 50;
                break;
            case 1:
                if (ai_group == 0xF) modifier = 20;
                else if (ai_group == 0x10) modifier = 20;
                else if (ai_group == 0x20) modifier = 50;
                else if (ai_group == 0x23) modifier = 100;
                break;
            case 2:
                if (ai_group == 0x12) modifier = 50;
                else if (ai_group == 0x14) modifier = 100;
                break;
            case 3:
                if (ai_group == 0x1C) modifier = 100;
                else if (ai_group == 0x1A) modifier = 20;
                break;
            case 4:
                if (ai_group == 0x25) modifier = 100;
                break;
            case 5:
                if (ai_group == 0x20 || ai_group == 0x1F) modifier = 100;
                break;
            case 6:
                if (ai_group == 0x21 || ai_group == 0x22) modifier = 100;
                break;
        }

        switch (MOX::_player[player_idx].objectives) {
            case 0:
                if (ai_group == 0xC || ai_group == 3 || ai_group == 0xB) modifier = 100;
                break;
            case 1:
                if (ai_group == 0x11 || ai_group == 0x21) modifier = 100;
                break;
            case 2:
                if (ai_group == 0x27) modifier = 50;
                else if (ai_group == 9 || ai_group == 10) modifier = 100;
                break;
            case 3:
                if (ai_group == 2) modifier = 100;
                break;
            case 4:
                if (ai_group == 1 || ai_group == 4) modifier = 100;
                break;
            case 5:
                if (ai_group == 0 || ai_group == 4) modifier = 100;
                break;
        }

        switch (ai_group) {
            case 0:
                if (MOX::_player[player_idx].traits[TRAIT_FOOD_BONUS] < 0) modifier = 100;
                else if (MOX::_player[player_idx].traits[TRAIT_FOOD_BONUS] > 0) modifier = 10;

                if (MOX::_player[player_idx].traits[TRAIT_EATS_MINERALS] != 0) modifier = 1;
                else if (MOX::_player[player_idx].traits[TRAIT_CYBERNETIC] != 0) modifier = 20;
                break;
            case 1:
                if (MOX::_player[player_idx].traits[TRAIT_PRODUCTION_BONUS] < 0) modifier = 100;
                break;
            case 2:
                if (MOX::_player[player_idx].traits[TRAIT_RESEARCH_BONUS] < 0) modifier = 100;
                else if (MOX::_player[player_idx].traits[TRAIT_RESEARCH_BONUS] > 0) modifier = 100;
                break;
            case 3:
                if (MOX::_player[player_idx].traits[TRAIT_TAX_BONUS] < 0) modifier = 100;
                else if (MOX::_player[player_idx].traits[TRAIT_TAX_BONUS] > 0) modifier = 20;
                break;
            case 4:
                if (MOX::_player[player_idx].traits[TRAIT_PRODUCTION_BONUS] > 0) modifier = 100;
                if (MOX::_player[player_idx].traits[TRAIT_ENVIRONMENT_IMMUNE] != 0) modifier = 1;
                break;
            case 6:
                if (MOX::_player[player_idx].traits[TRAIT_SUBTERRANEAN] != 0) modifier = 20;
                if (MOX::_player[player_idx].traits[TRAIT_POP_GROWTH_BONUS] < 0) modifier = 100;
                else if (MOX::_player[player_idx].traits[TRAIT_POP_GROWTH_BONUS] > 0) modifier = 5;
                break;
            case 12:
                if (MOX::_player[player_idx].traits[TRAIT_SPYING_BONUS] < 0 || MOX::_player[player_idx].traits[TRAIT_SPYING_BONUS] > 0) modifier = 50;
                if (MOX::_player[player_idx].traits[TRAIT_CURRENT_GOVERNMENT] / 2 == 2) modifier = 50;
                break;
            case 16:
                if (MOX::_player[player_idx].traits[TRAIT_GROUND_COMBAT_BONUS] < 0) modifier = 20;
                break;
            case 18:
                if (MOX::_player[player_idx].traits[TRAIT_SHIP_DEFENSE] < 0) modifier = 50;
                break;
            case 25:
                if (MOX::_player[player_idx].traits[TRAIT_SHIP_ATTACK] < 0) modifier = 100;
                break;
            case 27:
                if (MOX::_player[player_idx].traits[TRAIT_SHIP_ATTACK] > 0) modifier = 100;
                break;
            case 28:
                if (MOX::_player[player_idx].traits[TRAIT_SHIP_DEFENSE] > 0) modifier = 100;
                break;
            case 37:
                if (MOX::_player[player_idx].traits[TRAIT_STEALTHY_SHIPS] != 0) modifier = 1;
                break;
            case 40:
                if (MOX::_player[player_idx].traits[TRAIT_CURRENT_GOVERNMENT] / 2 == 3) modifier = 1;
                break;
        }

        if (tech_app_idx == 5) {
            if (MOX::_player[player_idx].traits[TRAIT_TELEPATHIC] != 0) modifier = 1;
        } else if (tech_app_idx == TECH_APP_PLANETARY_GRAVITY_GENERATOR) {
            if (MOX::_player[player_idx].traits[TRAIT_HEAVY_G_WORLD] != 0) modifier = 1;
            else if (MOX::_player[player_idx].traits[TRAIT_LOW_G_WORLD] != 0) modifier = 50;
        }

        if (MOX::_player[player_idx].objectives == 100) {
            int diff_mult = MOX::_settings.game_difficulty * MOX::_settings.game_difficulty * 5 + 10;
            modifier = diff_mult * 5;
            if (ai_group == 0x1A || ai_group == 0x1B || ai_group == 0x1D || ai_group == 0x1E || ai_group == 0x21 || ai_group == 0x15) {
                modifier = diff_mult * 20;
            }
        }

        uint32_t val = 0;
        if (category_is_multi == 0) {
            int last_known_field_ai_group = 0;
            for (int i = 0; i < TECH_APP_BIOLOGY; ++i) {
                if (TECHDATA::_technology_applications[i].ai_group == ai_group &&
                    MOX::_player[player_idx].tech_applications[i] == TECH_RESEARCH_STATUS_RESEARCHED) {
                    last_known_field_ai_group = TECHDATA::_technology_fields[TECHDATA::_technology_applications[i].tech_field_id].ai_group;
                }
            }

            if (last_known_field_ai_group + 3 <= tech_field_ai_group) {
                val = (tech_field_ai_group - last_known_field_ai_group) * base_value * modifier / 3;
            } else {
                if (ai_group == 0xE || ai_group == 0x10 || ai_group == 0x12 || ai_group == 0x18 || ai_group == 0x19 || ai_group == 0x20 || ai_group == 0x27) {
                    val = 0;
                } else {
                    val = (base_value * modifier * 2) / (last_known_field_ai_group - tech_field_ai_group + 3);
                }
            }
        } else {
            val = base_value * modifier;
        }

        if (ai_group == 0x19 && base_value < newtech::_best_category[1]) {
            val = (newtech::_best_category[1] - base_value) * val;
        }
        if (ai_group == 0x12 && base_value < newtech::_best_category[0]) {
            val = (newtech::_best_category[0] - base_value) * val;
        }
        if (ai_group == 0x13 && base_value < newtech::_best_category[2]) {
            val = (newtech::_best_category[2] - base_value) * val;
        }
        if (ai_group == 0xA && base_value < newtech::_best_category[3]) {
            val = (newtech::_best_category[3] - base_value) * val;
        }
        if (ai_group == 0xC && base_value < newtech::_best_category[4]) {
            val = (newtech::_best_category[4] - base_value) * val;
        }
        if (ai_group == 0xF && base_value < newtech::_best_category[6]) {
            val = (newtech::_best_category[6] - base_value) * val;
        }
        if (ai_group == 0x8) {
            uint32_t val1 = newtech::Get_Competition_Player_Tech_Value_(player_idx, 0xF);
            uint32_t val2 = newtech::Get_Competition_Player_Tech_Value_(player_idx, 0x10);
            if (val1 + val2 < (uint32_t)(newtech::_best_category[6] * 2)) {
                val *= 2;
            }
        }

        int theme = MOX::_player[player_idx].ship_design_theme;
        if (theme == 2 && ai_group == 0x18 && base_value < newtech::_best_category[7]) {
            val = (newtech::_best_category[7] - base_value) * val;
        }

        if (theme == 0) {
            if (ai_group == 0x1A && base_value < newtech::_best_category[11]) val = (newtech::_best_category[11] - base_value) * val;
        } else if (theme == 1) {
            if (ai_group == 0x1A && base_value < newtech::_best_category[11]) val = (newtech::_best_category[11] - base_value) * val;
            if (ai_group == 0x13 && base_value < newtech::_best_category[11]) val = (newtech::_best_category[11] - base_value) * val;
        } else if (theme == 2) {
            if (ai_group == 0x15 && base_value < newtech::_best_category[11]) val = (newtech::_best_category[11] - base_value) * val;
        }

        if (MOX::_stardate - 35000 < 150 && ai_group == 0x12) {
            val *= 2;
        }

        int16_t tech_field_id = TECHDATA::_technology_applications[tech_app_idx].tech_field_id;
        if (MOX::_player[player_idx].tech_fields[tech_field_id] == 3) {
            val = (val * 5) >> 2;
        }

        if (val == 0 && MOX::_player[player_idx].tech_fields[tech_field_id] == 2) {
            bool found = false;
            for (int i = 0; i < TECH_APP_BIOLOGY; ++i) {
                if (found) break;
                if (i == tech_app_idx) continue;
                if (TECHDATA::_technology_applications[i].tech_field_id == tech_field_id) {
                    if (MOX::_player[player_idx].tech_applications[i] != TECH_RESEARCH_STATUS_RESEARCHED) {
                        found = true;
                    }
                }
            }
            if (!found) {
                val = base_value * 10;
            }
        }

        return (int32_t)val;
    }

    int8_t __cdecl All_Gray_Boxes_Researched_(s_player* player) {
        if (player->tech_fields[TECH_FIELD_NUCLEAR_FISSION] == 3 &&
            player->tech_fields[TECH_FIELD_COLD_FUSION] == 3 &&
            player->tech_fields[TECH_FIELD_PHYSICS] == 3 &&
            player->tech_fields[TECH_FIELD_ENGINEERING] == 3 &&
            player->tech_fields[TECH_FIELD_CHEMISTRY] == 3 &&
            player->tech_fields[TECH_FIELD_ELECTRONICS] == 3) {
            return 1;
        }
        return 0;
    }

    int32_t __cdecl Choose_Hyper_Advanced_Tech_() {
        int32_t rand_val = game_random::Random_(0x14);

        switch (rand_val) {
            case 1:
                return TECH_APP_BIOLOGY;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
                return TECH_APP_POWER;
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
                return TECH_APP_PHYSICS;
            case 12:
            case 13:
                return TECH_APP_CONSTRUCTION;
            case 14:
            case 15:
            case 16:
            case 17:
                return TECH_APP_FIELDS;
            case 18:
                return TECH_APP_CHEMISTRY;
            case 19:
                return TECH_APP_COMPUTERS;
            case 20:
                return TECH_APP_SOCIOLOGY;
            default:
                return -1;
        }
    }

    int16_t __cdecl Choose_Tech_Application_(int16_t player_idx) {
        uint32_t tech_values[TECH_APP_COUNT] = { 0 };
        uint32_t weighted_choices[TECH_APP_COUNT] = { 0 };
        const int32_t tech_choice_count = (int32_t)(sizeof(tech_values) / sizeof(tech_values[0]));
        uint32_t max_weighted_choice = 0;
        bool has_candidate_tech = false;

        s_player* player = &MOX::_player[player_idx];
        bool all_gray_boxes = All_Gray_Boxes_Researched_(player) != 0;
        Set_Competition_Tech_Values_(player_idx);

        for (int32_t i = 1; i < tech_choice_count; ++i) {
            if (player->tech_applications[i] == TECH_RESEARCH_STATUS_AVAILABLE &&
                (MOX::_settings.strategic_combat_flag == 0 || TECHDATA::_technology_applications[i].in_strategic != 0)) {

                int16_t tech_field_id = TECHDATA::_technology_applications[i].tech_field_id;

                if (tech_field_id < TECH_FIELD_BIOLOGY && player->tech_fields[tech_field_id] == 2) {
                    uint32_t value = Calc_Tech_Value_(player_idx, i);

                    if (!all_gray_boxes) {
                        switch (tech_field_id) {
                            case TECH_FIELD_CHEMISTRY:
                            case TECH_FIELD_COLD_FUSION:
                            case TECH_FIELD_ELECTRONICS:
                            case TECH_FIELD_ENGINEERING:
                            case TECH_FIELD_NUCLEAR_FISSION:
                            case TECH_FIELD_PHYSICS:
                                value += 1;
                                break;
                            default:
                                value = 0;
                                break;
                        }
                    } else {
                        if (tech_field_id == 4) value *= 2;
                        if (i == 114) value *= 2;
                        if (i == 51) value *= 5;
                        if (tech_field_id == 73) value *= 2;
                        if (value == 0) value = 1;
                    }

                    has_candidate_tech = true;
                    tech_values[i] = value;
                }
            }
        }

        if (has_candidate_tech) {
            int32_t research_threshold = 15;
            // BUG: Original assembly unconditionally reads from Player 0 instead of the current player.
            int32_t research_produced = MOX::_player[0].research_produced;
            if (research_produced == 0) {
                research_produced = 1;
            }

            bool has_choices = false;
            while (!has_choices) {
                max_weighted_choice = 0;

                for (int32_t i = 1; i < tech_choice_count; ++i) {
                    if (tech_values[i] == 0) {
                        weighted_choices[i] = 0;
                    } else {
                        int16_t tech_field_id = TECHDATA::_technology_applications[i].tech_field_id;
                        uint32_t cost = TECHDATA::_technology_fields[tech_field_id].cost;
                        int32_t turns = cost / research_produced;
                        if (turns == 0) {
                            turns = 1;
                        }

                        if (research_threshold < turns) {
                            weighted_choices[i] = 0;
                        } else {
                            uint32_t choice_val = (uint32_t)((research_threshold * tech_values[i]) / turns);
                            if (max_weighted_choice < choice_val) {
                                max_weighted_choice = choice_val;
                            }
                            has_choices = true;
                            weighted_choices[i] = choice_val;
                        }
                    }
                }

                research_threshold = (research_threshold * 3) / 2;
            }
        }

        if (MOX::_settings.game_difficulty > 0) {
            if (max_weighted_choice == 0) {
                return Choose_Hyper_Advanced_Tech_();
            }

            for (int32_t i = 1; i < tech_choice_count; ++i) {
                uint32_t val = weighted_choices[i];
                if (val != 0) {
                    bool keep = true;
                    switch (player->objectives) {
                        case 0:
                        case 5:
                            if (val * 2 < max_weighted_choice) keep = false;
                            break;
                        case 1:
                        case 2:
                            if (val * 6 < max_weighted_choice) keep = false;
                            break;
                        case 3:
                        case 4:
                            if (val * 5 < max_weighted_choice * 4) keep = false;
                            break;
                        default:
                            break;
                    }

                    if (!keep) {
                        weighted_choices[i] = 0;
                    }
                }
            }
        }

        if (max_weighted_choice != 0) {
            return RUSS::Get_Weighted_Choice_Int_(weighted_choices, tech_choice_count);
        }

        return Choose_Hyper_Advanced_Tech_();
    }
}