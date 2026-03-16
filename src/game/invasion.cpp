#include "pch.h"

namespace invasion {

void __cdecl Compute_Player_Ground_Combat_Bonuses_(int player_idx, s_invasion_player_bonuses* bonuses, char is_defender) {
    memset(bonuses, 0, sizeof(s_invasion_player_bonuses));

    if (player_idx >= 8) {
        bonuses->antigrav_bonus = 10;
        bonuses->battleoids_bonus = 10;
        bonuses->has_battleoids = 1;
        bonuses->powered_armor_bonus = 10;
        bonuses->has_powered_armor = 1;
        bonuses->best_armor_tech_app_id = (int8_t)TECHDATA::_armor[6].tech_app_id;
        bonuses->troop_bonus = TECHDATA::_armor[6].troops_bonus;
        bonuses->rifle_tech_app_id = (int8_t)TECHDATA::_rifles[5].tech_app_id;
        bonuses->rifle_bonus = TECHDATA::_rifles[5].bonus;
        bonuses->personal_shield_tech_app_id = (int8_t)TECHDATA::_personal_shields[1].tech_app_id;
        bonuses->personal_shield_bonus = TECHDATA::_personal_shields[1].bonus;
        bonuses->difficulty_bonus = (MOX::_settings.game_difficulty * 2) - 4;
        return;
    }

    s_player* player = &MOX::_player[player_idx];

    if (player->tech_applications[76 /* TECH_APP_ANTI_GRAV_HARNESS */] == 3) {
        bonuses->antigrav_bonus = 10;
    }
    if (player->tech_applications[91 /* TECH_APP_BATTLEOIDS */] == 3) {
        bonuses->battleoids_bonus = 10;
        bonuses->has_battleoids = 1;
    }
    if (player->tech_applications[211 /* TECH_APP_POWERED_ARMOR */] == 3) {
        bonuses->powered_armor_bonus = 10;
        bonuses->has_powered_armor = 1;
    }

    int armor_idx = aitech::Player_Best_Armor_(player);
    bonuses->best_armor_tech_app_id = (int8_t)TECHDATA::_armor[armor_idx].tech_app_id;
    bonuses->troop_bonus = TECHDATA::_armor[armor_idx].troops_bonus;

    int rifle_idx = aitech::Player_Best_Rifle_(player);
    bonuses->rifle_tech_app_id = (int8_t)TECHDATA::_rifles[rifle_idx].tech_app_id;
    bonuses->rifle_bonus = TECHDATA::_rifles[rifle_idx].bonus;

    int shield_idx = aitech::Player_Best_Personal_Shield_(player);
    bonuses->personal_shield_tech_app_id = (int8_t)TECHDATA::_personal_shields[shield_idx].tech_app_id;
    bonuses->personal_shield_bonus = TECHDATA::_personal_shields[shield_idx].bonus;

    bonuses->ground_combat_bonus = player->traits[TRAIT_GROUND_COMBAT_BONUS];

    if (player->traits[TRAIT_HEAVY_G_WORLD] != 0) {
        bonuses->heavy_g_bonus = 1;
    } else if (player->traits[TRAIT_LOW_G_WORLD] != 0) {
        bonuses->low_g_penalty = (int8_t)0xF6; /* -10 */
    }

    if (player->traits[TRAIT_SUBTERRANEAN] != 0 && is_defender != 0) {
        bonuses->subterranean_bonus = 10;
    }

    if (player->objectives == 100) {
        bonuses->difficulty_bonus = 0;
    } else {
        bonuses->difficulty_bonus = MOX::_settings.game_difficulty - 2;
    }
}

void __cdecl Check_All_Rebellions_() {
    s_colony* colony = MOX::_colony;
    s_colony* end = &colony[MOX::_NUM_COLONIES];

    while (colony < end) {
        if (colony->owner != -1 && colony->outpost_flag == 0) {
            Check_Rebellion_(colony);
        }
        colony++;
    }
}

void __cdecl Ground_Combat_Round_(s_invasion_side_info* side1, s_invasion_side_info* side2) {
    int16_t roll1 = side1->unit_strengths[(uint8_t)side1->current_troop_type_idx] + (int16_t)random::Random_(100);
    int16_t roll2 = side2->unit_strengths[(uint8_t)side2->current_troop_type_idx] + (int16_t)random::Random_(100);

    side1->round_hit_unit_type = -1;
    side1->round_killed_unit_type = -1;

    if (roll2 >= roll1) {
        uint8_t idx = (uint8_t)side1->current_troop_type_idx;
        side1->current_unit_damage++;

        if (side1->unit_hps[idx] == side1->current_unit_damage) {
            side1->round_killed_unit_type = idx;
            side1->unit_counts[idx]--;
            side1->current_unit_damage = 0;

            if (side1->unit_counts[idx] == 0) {
                Find_Troops_(side1);
            }
        } else {
            side1->round_hit_unit_type = idx;
        }
    }

    side2->round_hit_unit_type = -1;
    side2->round_killed_unit_type = -1;

    if (roll2 <= roll1) {
        uint8_t idx = (uint8_t)side2->current_troop_type_idx;
        side2->current_unit_damage++;

        if (side2->unit_hps[idx] == side2->current_unit_damage) {
            side2->round_killed_unit_type = idx;
            side2->unit_counts[idx]--;
            side2->current_unit_damage = 0;

            if (side2->unit_counts[idx] == 0) {
                Find_Troops_(side2);
            }
        } else {
            side2->round_hit_unit_type = idx;
        }
    }
}

void __cdecl Find_Troops_(s_invasion_side_info* side_info) {
    while (side_info->unit_counts[side_info->current_troop_type_idx] == 0) {
        // Potential bug: out-of-bounds access to unit_counts[4] if current_troop_type_idx reaches 4
        if (side_info->current_troop_type_idx >= 4) {
            break;
        }
        side_info->current_troop_type_idx++;
    }
    side_info->current_unit_damage = 0;
}

}