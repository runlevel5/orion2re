#include "deps.h"

namespace OFFICER {
    int16_t __cdecl Get_Officer_Base_Level_(int16_t xp, uint8_t warlord) {
        if (xp < 60) {
            return 0;
        }
        if (xp < 150) {
            return 1;
        }
        if (xp < 300) {
            return 2;
        }
        if (xp < 500) {
            return 3;
        }
        if (xp < 1000) {
            return 4;
        }
        return (warlord != 0) ? 5 : 4;
    }

    int16_t __cdecl Owned_Officer_Level_(int leader_idx) {
        int8_t warlord = 0;

        if (leader_idx != 0x42) {
            int8_t player_idx = MOX::_leaders[leader_idx].player_index;
            warlord = MOX::_player[player_idx].traits[0];
        }

        int16_t xp = MOX::_leaders[leader_idx].xp;
        int16_t level = OFFICER::Get_Officer_Base_Level_(xp, warlord);

        if (level > 5) {
            level = 5;
        }

        return level;
    }

    int16_t __cdecl Officer_Skill_Value_(int16_t leader_index) {
        int16_t total_value = 0;
        s_leader_data& leader = MOX::_leaders[leader_index];

        for (int skill_id = 0; skill_id < 54; ++skill_id) {
            const MOX::s_skill_data& skill = MOX::_skill_data[skill_id];
            int8_t skill_bonus_byte = *reinterpret_cast<const int8_t*>(reinterpret_cast<std::uintptr_t>(&skill.skill_mask) + 3);

            if (skill.num == 0) {
                if (OFFICER::Officer_Has_General_Skill_(MOX::_PLAYER_NUM, leader_index, (int16_t)skill_id)) {
                    int16_t bonus = (int16_t)skill_bonus_byte;

                    if (skill_id == 2 || skill_id == 3) {
                        bonus = (leader.type == 0) ? 3 : 1;
                        if (skill_id == 3) {
                            bonus *= 2;
                        }
                    }
                    total_value += bonus;
                }
            }

            if (leader.type == 0) {
                if (skill.num == 1) {
                    if (OFFICER::Officer_Has_Special_Skill_(MOX::_PLAYER_NUM, leader_index, (int16_t)skill_id)) {
                        total_value += (int16_t)skill_bonus_byte;
                    }
                }
            } else if (leader.type == 1) {
                if (skill.num == 2) {
                    if (OFFICER::Officer_Has_Special_Skill_(MOX::_PLAYER_NUM, leader_index, (int16_t)skill_id)) {
                        total_value += (int16_t)skill_bonus_byte;
                    }
                }
            }
        }

        for (int i = 0; i < 3; ++i) {
            uint8_t app_idx = leader.tech_application[i];
            if (app_idx != 0) {
                int16_t field_id = TECHDATA::_technology_applications[app_idx].tech_field_id;
                if (field_id < 0) {
                    total_value = 1;
                } else {
                    uint32_t cost = TECHDATA::_technology_fields[field_id].cost;
                    total_value += OFFICER::map_1000_5000(cost);
                }
            }
        }

        return total_value;
    }
}