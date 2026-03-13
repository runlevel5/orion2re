#include "pch.h"

namespace CMBTFIR2 {
    int16_t __cdecl Helmsman_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;
        uint32_t special_skills = MOX::_leaders[leader_idx].special_skills;

        if (special_skills & 0x40) {
            int16_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            return (int16_t)((level + 1) * 5);
        }

        if (special_skills & 0x80) {
            int16_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            bonus = (int16_t)(((level * 3 + 3) * 5) / 2);
        }

        return bonus;
    }

    int16_t __cdecl Weaponry_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;

        if ((MOX::_leaders[leader_idx].special_skills & 0x4000) != 0) {
            bonus = OFFICER::Owned_Officer_Level_(leader_idx);
            return (int16_t)((bonus + 1) * 5);
        }

        if ((MOX::_leaders[leader_idx].special_skills & 0x8000) != 0) {
            bonus = OFFICER::Owned_Officer_Level_(leader_idx);
            bonus = (int16_t)(((bonus * 3 + 3) * 5) / 2);
        }

        return bonus;
    }

    void __cdecl Set_Combat_Ordnance_Values_() {
        int16_t attacker_idx = MOX::_combat_attacker_player;
        int16_t attacker_bonus = CMBTFIR2::Get_Fleet_Ordnance_Bonus_(attacker_idx) + 100;

        int16_t defender_idx = MOX::_combat_defender_player;
        MOX::_ordnance_bonus[attacker_idx] = attacker_bonus;

        int16_t defender_bonus = CMBTFIR2::Get_Fleet_Ordnance_Bonus_(defender_idx) + 100;
        MOX::_ordnance_bonus[defender_idx] = defender_bonus;
    }

    int16_t __cdecl Crew_Defense_Bonus_(int16_t combat_idx) {
        int8_t quality = MOX::_combat_data[combat_idx].crew_quality;
        return MOX::_crew_data[quality].defense;
    }

    short __cdecl Fleet_Defensive_Bonus_(short player_idx, short mode, short* bonus_val_ptr) {
        int16_t max_helmsman_bonus = 0;

        if (mode == 1) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                s_combat_data* ship = &MOX::_combat_data[i];
                if (ship->owner == (int8_t)player_idx && ship->is_captured == 0 && ship->unit_status == 0) {
                    if (COMBINIT::Does_Combat_Ship_Have_Special_(i, 38 /* 0x26 */)) {
                        *bonus_val_ptr = 70 /* 0x46 */;
                    }
                    if (ship->officer_idx > -1) {
                        int16_t bonus = Helmsman_Bonus_(ship->officer_idx);
                        if (bonus > max_helmsman_bonus) {
                            max_helmsman_bonus = bonus;
                        }
                    }
                }
            }
            return (max_helmsman_bonus / 2) + *bonus_val_ptr;
        } else {
            int16_t extra_bonus = 0;
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                s_combat_data* ship = &MOX::_combat_data[i];
                if (ship->owner == (int8_t)player_idx && ship->is_captured == 0 && ship->unit_status == 0) {
                    if (ship->officer_idx > -1) {
                        int16_t bonus = Helmsman_Bonus_(ship->officer_idx);
                        if (bonus > max_helmsman_bonus) {
                            max_helmsman_bonus = bonus;
                        }
                    }
                    // This block appears to be a no-op but is preserved from the original binary
                    if (ship->ftl_type == 0) {
                        if (ship->size_class == 2 || ship->size_class == 3 || ship->size_class == 4) {
                            extra_bonus = 0;
                        }
                    }
                }
            }
            return max_helmsman_bonus + extra_bonus;
        }
    }

    int16_t __cdecl Defensive_Combat_Bonus_(int16_t combat_unit_idx, int16_t target_type) {
        int16_t specials_base = 0;
        int16_t fleet_base = 0;
        int16_t trait_bonus = 0;
        int16_t speed_bonus;
        int16_t crew_bonus = CMBTFIR2::Crew_Defense_Bonus_(combat_unit_idx);

        if (target_type == 0) {
            speed_bonus = MOX::_combat_data[combat_unit_idx].current_speed * 5;
            if (MOX::_combat_data[combat_unit_idx].current_speed == 0) {
                speed_bonus = -20;
            }
        } else {
            speed_bonus = 0;
            crew_bonus /= 2;
        }

        int16_t specials_bonus = CMBTFIR2::Ship_Specials_Defensive_Bonus_(combat_unit_idx, target_type, &specials_base);
        int8_t owner = MOX::_combat_data[combat_unit_idx].owner;
        int16_t fleet_bonus = CMBTFIR2::Fleet_Defensive_Bonus_(owner, target_type, &fleet_base);

        if (fleet_base > specials_base) {
            specials_bonus -= specials_base;
        } else {
            fleet_bonus -= fleet_base;
        }

        if (MOX::_combat_data[combat_unit_idx].ftl_type > 0 && owner < 8) {
            // Index 6 corresponds to TRAIT_SHIP_DEFENSE
            trait_bonus = (int16_t)MOX::_player[owner].traits[6 /* TRAIT_SHIP_DEFENSE */];
        }

        return fleet_bonus + trait_bonus + speed_bonus + specials_bonus + crew_bonus;
    }

    int16_t __cdecl Computer_Bonus_(int16_t ship_idx) {
        if (MOX::_combat_data[ship_idx].computer_current_hits < 1) {
            return 0;
        }

        return TECHDATA::_computers[MOX::_combat_data[ship_idx].computer_type].bonus;
    }

    int16_t __cdecl Crew_Attack_Bonus_(int16_t ship_idx) {
        int8_t quality = MOX::_combat_data[ship_idx].crew_quality;
        return MOX::_crew_data[quality].attack;
    }

    int16_t __cdecl Ship_Specials_Offensive_Bonus_(int16_t ship_idx) {
        int16_t bonus = 0;

        if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 5 /* SPEC_STRUCTURAL_ANALYZER */) != 0) {
            bonus = 50;
        }

        return bonus;
    }

    short __cdecl Fleet_Offensive_Bonus_(short player_idx) {
        int16_t size_bonus = 0;
        int16_t special_bonus = 0;
        int16_t max_weaponry_bonus = 0;
        int16_t max_lore_bonus = 0;
        int16_t is_ancient = 0;

        if (MOX::_combat_attacker_player > 8 || MOX::_combat_defender_player > 8) {
            is_ancient = 1;
        }

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            s_combat_data* ship = &MOX::_combat_data[i];

            if (ship->owner == (int8_t)player_idx && ship->is_captured == 0 && ship->unit_status == 0) {
                if (ship->ftl_type == 0) {
                    if (ship->size_class == 2) {
                        size_bonus = 0;
                    } else if (ship->size_class == 3) {
                        size_bonus = 10;
                    } else if (ship->size_class == 4) {
                        size_bonus = 20;
                    }
                }

                int16_t officer_idx = ship->officer_idx;
                if (officer_idx > -1) {
                    int16_t w_bonus = CMBTFIR2::Weaponry_Bonus_(officer_idx);
                    if (w_bonus > max_weaponry_bonus) {
                        max_weaponry_bonus = w_bonus;
                    }

                    if (is_ancient != 0) {
                        int16_t l_bonus = CMBTFIR2::Galactic_Lore_Bonus_(officer_idx);
                        if (l_bonus > max_lore_bonus) {
                            max_lore_bonus = l_bonus;
                        }
                    }
                }

                if (is_ancient == 1) {
                    if (COMBINIT::Does_Combat_Ship_Have_Special_(i, 28 /* 0x1c */)) {
                        special_bonus = 20;
                    }
                }
            }
        }

        return max_lore_bonus + size_bonus + max_weaponry_bonus + special_bonus;
    }

    int16_t __cdecl Offensive_Combat_Bonus_(int16_t ship_idx) {
        int16_t computer_bonus;
        int16_t crew_bonus;
        int16_t specials_bonus;
        int16_t fleet_bonus;
        int16_t trait_bonus;
        int8_t owner;

        computer_bonus = Computer_Bonus_(ship_idx);
        crew_bonus = Crew_Attack_Bonus_(ship_idx);
        specials_bonus = Ship_Specials_Offensive_Bonus_(ship_idx);

        owner = MOX::_combat_data[ship_idx].owner;
        fleet_bonus = Fleet_Offensive_Bonus_(owner);

        trait_bonus = 0;
        if (owner < 8) {
            trait_bonus = (int16_t)MOX::_player[owner].traits[7 /* TRAIT_SHIP_ATTACK */];
        }

        return computer_bonus + crew_bonus + trait_bonus + fleet_bonus + specials_bonus;
    }
}