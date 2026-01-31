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

        for (int16_t skill_idx = 0; skill_idx < 54; ++skill_idx) {
            MOX::s_skill_data& skill = MOX::skill_data[skill_idx];

            if (skill.unknown1 == 0) {
                if (OFFICER::Officer_Has_General_Skill_(MOX::_PLAYER_NUM, leader_index, skill_idx)) {
                    int16_t val;
                    if (skill_idx == 2 || skill_idx == 3) {
                        // type 0 (Ship Officer) gets 3, type 1 (Colony Leader) gets 1
                        val = (leader.type == 0) ? 3 : 1;
                        if (skill_idx == 3) {
                            val *= 2;
                        }
                    } else {
                        val = (int8_t)skill.unknown3;
                    }
                    total_value += val;
                }
            }

            if (leader.type == 0) { // Ship Officer
                if (skill.unknown1 == 1) {
                    if (OFFICER::Officer_Has_Special_Skill_(MOX::_PLAYER_NUM, leader_index, skill_idx)) {
                        total_value += (int8_t)skill.unknown3;
                    }
                }
            } else if (leader.type == 1) { // Colony Leader
                if (skill.unknown1 == 2) {
                    if (OFFICER::Officer_Has_Special_Skill_(MOX::_PLAYER_NUM, leader_index, skill_idx)) {
                        total_value += (int8_t)skill.unknown3;
                    }
                }
            }
        }

        for (int i = 0; i < 3; ++i) {
            uint8_t app_idx = leader.tech_application[i];
            if (app_idx != 0) {
                int16_t field_id = TECHDATA::_technology_applications[app_idx].tech_field_id;
                if (field_id < 0) {
                    // Logic from disassembly: resets total_value to 1 if field_id is invalid/negative
                    total_value = 1;
                } else {
                    total_value += OFFICER::Get_Research_Cost_Skill_Value_(TECHDATA::_technology_fields[field_id].cost);
                }
            }
        }

        return total_value;
    }

    void __cdecl Get_Random_Skills_(uint32_t* primary_mask, uint32_t* secondary_mask, int16_t skill_class, int16_t count, int8_t randomize_class) {
        *primary_mask = 0;
        *secondary_mask = 0;

        if (count <= 0) {
            return;
        }

        int16_t remaining_count = count;
        do {
            int16_t selected_idx = 0;
            int16_t options_found = 0;
            int16_t current_class = skill_class;

            if (randomize_class != 0) {
                if (random::Random_(7) < 3) {
                    current_class = 0;
                }
            }

            for (int j = 0; j < 54; j += 2) {
                if (current_class == 0) {
                    // Search for secondary skills
                    if (MOX::skill_data[j].unknown1 == 0 &&
                        ((*secondary_mask & MOX::skill_data[j].skill_mask) == 0) &&
                        ((*secondary_mask & MOX::skill_data[j + 1].skill_mask) == 0)) 
                    {
                        options_found++;
                        if (random::Random_(options_found) == 1) {
                            if (random::Random_(2) == 1) {
                                selected_idx = (int16_t)j;
                            } else {
                                selected_idx = (int16_t)(j + 1);
                            }
                        }
                    }
                } else {
                    // Search for primary skills of specific class
                    if ((int16_t)MOX::skill_data[j].unknown1 == skill_class &&
                        ((*primary_mask & MOX::skill_data[j].skill_mask) == 0) &&
                        ((*primary_mask & MOX::skill_data[j + 1].skill_mask) == 0)) 
                    {
                        options_found++;
                        if (random::Random_(options_found) == 1) {
                            if (random::Random_(2) == 1) {
                                selected_idx = (int16_t)j;
                            } else {
                                selected_idx = (int16_t)(j + 1);
                            }
                        }
                    }
                }
            }

            if (current_class == 0) {
                *secondary_mask += MOX::skill_data[selected_idx].skill_mask;
            } else {
                *primary_mask += MOX::skill_data[selected_idx].skill_mask;
            }

            remaining_count--;
        } while (remaining_count != 0);
    }

    bool __cdecl One_Leader_With_Galactic_Lore_(int16_t player_idx) {
        return (bool)MOX::_player[player_idx].has_galactic_lore;
    }
}