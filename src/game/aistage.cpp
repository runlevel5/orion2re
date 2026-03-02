#include "pch.h"

namespace AISTAGE {

void __cdecl Compute_Defensive_Stage_Points_(int16_t player_idx) {
    uint8_t processed_stars[72];
    int picked_count = 0;
    bool finished;
    int32_t best_weight;
    int16_t best_star;

    if (MOX::_player[player_idx].eliminated != 0) {
        return;
    }

    memset(processed_stars, 0, sizeof(processed_stars));
    MOX::_player[player_idx].offensive_stagepoint = -1;

    for (int i = 0; i < MOX::_NUM_STARS; ++i) {
        MOX::_star[i].is_stagepoint &= ~(1 << (player_idx & 0x1F));
    }

    do {
        finished = false;
        best_star = -1;
        best_weight = 0;

        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            if (processed_stars[i] == 0) {
                int32_t weight = (*AIDATA::_star_stage_weights)[i][player_idx];
                if (best_weight < weight) {
                    best_weight = weight;
                    best_star = i;
                }
            }
        }

        if (best_star == -1) {
            finished = true;
        } else {
            MOX::_star[best_star].is_stagepoint |= (1 << (player_idx & 0x1F));

            if (picked_count == 0) {
                MOX::_player[player_idx].offensive_stagepoint = (int8_t)best_star;
            }

            picked_count++;
            processed_stars[best_star] = 1;

            for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                if (i != best_star) {
                    if ((*AIDATA::_star_stage_weights)[i][player_idx] != 0 &&
                        (MOX::_star[i].is_stagepoint & (1 << (player_idx & 0x1F))) == 0) {
                        
                        if ((*AIDATA::_n_turns_from_to)[best_star][i][player_idx] <= 1) {
                            (*AIDATA::_star_stage_weights)[i][player_idx] = 0;
                            processed_stars[i] = 1;
                        }
                    }
                }
            }
        }
    } while (!finished);

    if (picked_count == 0) {
        int32_t max_weight = 0;
        int16_t candidate_star = -1;
        int speed = (uint8_t)MOX::_player[player_idx].ship_speed;

        do {
            for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                if (((MOX::_star[i].has_colony >> (player_idx & 0x1F)) & 1) != 0) {
                    uint8_t in_range_mask = (*AIDATA::_star_in_range_of_player)[i];
                    if ((in_range_mask & ~(1 << (player_idx & 0x1F))) != 0) {
                        for (int other_player = 0; other_player < MOX::_NUM_PLAYERS; ++other_player) {
                            if (other_player != player_idx && ((in_range_mask >> (other_player & 0x1F)) & 1) != 0) {
                                if (SHIPMOVE::Star_In_Range_Of_Player_(&MOX::_star[i], (int16_t)other_player, (int16_t)speed)) {
                                    int32_t weight = (*AIDATA::_base_star_stage_weights)[i][player_idx];
                                    if (max_weight < weight) {
                                        max_weight = weight;
                                        candidate_star = i;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (candidate_star != -1) break;
            speed += 3;
        } while (speed < 50);

        if (candidate_star == -1) {
            int16_t ship_range = MOX::_player[player_idx].ship_range;
            for (int16_t s6 = 0; s6 < MOX::_NUM_STARS; ++s6) {
                if (((MOX::_star[s6].has_colony >> (player_idx & 0x1F)) & 1) != 0) {
                    for (int16_t s9 = 0; s9 < MOX::_NUM_STARS; ++s9) {
                        if (((MOX::_star[s9].has_colony & ~(1 << (player_idx & 0x1F))) != 0)) {
                            if (GEO::Parsecs_Between_Stars_(s6, s9) <= ship_range) {
                                int32_t weight = (*AIDATA::_base_star_stage_weights)[s6][player_idx];
                                if (max_weight < weight) {
                                    max_weight = weight;
                                    candidate_star = s6;
                                }
                            }
                        }
                    }
                }
            }

            if (candidate_star == -1) {
                for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
                    if (((MOX::_star[i].has_colony >> (player_idx & 0x1F)) & 1) != 0) {
                        int32_t weight = (*AIDATA::_base_star_stage_weights)[i][player_idx];
                        if (max_weight <= weight) {
                            max_weight = weight;
                            candidate_star = i;
                        }
                    }
                }
            }
        }

        MOX::_player[player_idx].offensive_stagepoint = (int8_t)candidate_star;
        MOX::_star[candidate_star].is_stagepoint |= (1 << (player_idx & 0x1F));
    }
}

} // namespace AISTAGE