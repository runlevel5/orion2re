#include "deps.h"

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

    bonuses->ground_combat_bonus = player->traits.ground_combat_bonus;

    if (player->traits.heavy_g_world != 0) {
        bonuses->heavy_g_bonus = 1;
    } else if (player->traits.low_g_world != 0) {
        bonuses->low_g_penalty = (int8_t)0xF6; /* -10 */
    }

    if (player->traits.subterranean != 0 && is_defender != 0) {
        bonuses->subterranean_bonus = 10;
    }

    if (player->objectives == 100) {
        bonuses->difficulty_bonus = 0;
    } else {
        bonuses->difficulty_bonus = MOX::_settings.game_difficulty - 2;
    }
}

}