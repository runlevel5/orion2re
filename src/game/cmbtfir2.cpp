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

    /**
     * @brief Calculates the Galactic Lore bonus for a leader.
     * @param leader_idx Index of the leader in the global leaders array.
     * @return The calculated bonus value.
     */
    int16_t __cdecl Galactic_Lore_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;
        uint32_t special_skills = MOX::_leaders[leader_idx].special_skills;

        // Check for bit 0x10 (likely Galactic Lore skill)
        if (special_skills & 0x10) {
            int16_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            return (int16_t)((level + 1) * 5);
        }

        // Check for bit 0x20 (likely Advanced Galactic Lore skill)
        if (special_skills & 0x20) {
            int32_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            // Calculation: ((level * 3 + 3) * 5) / 2
            bonus = (int16_t)(((level * 3 + 3) * 5) / 2);
        }

        return bonus;
    }

    void __cdecl Set_Combat_Ordnance_Values_() {
        int16_t attacker_idx = MOX::_combat_attacker_player;
        int16_t attacker_bonus = CMBTFIR2::Get_Fleet_Ordnance_Bonus_(attacker_idx) + 100;

        // Load defender index before storing attacker result to match register usage (SI)
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

    int16_t __cdecl Being_Captured_(int16_t ship_idx) {
        return (int16_t)struct_::Test_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 4);
    }

    void __cdecl Set_Capturing_Flag_(int16_t unit_idx) {
        struct_::Set_Bit_Field_(&MOX::_combat_data[unit_idx].combat_status_flags, 4);
    }

    void __cdecl Set_Boarded_Flag_(int16_t ship_idx) {
        struct_::Set_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 1);
    }

    int16_t __cdecl Already_Boarded_(int16_t ship_idx) {
        return (int16_t)struct_::Test_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 1);
    }

    void __cdecl Set_Teleporter_Used_(int16_t ship_idx) {
        // Bit 0 of combat_status_flags indicates if teleporter was used
        struct_::Set_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 0);
    }

    int16_t __cdecl Get_Fleet_Ordnance_Bonus_(int16_t owner) {
        int16_t max_bonus = 0;

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            if (MOX::_combat_data[i].owner == (int8_t)owner &&
                MOX::_combat_data[i].is_captured == 0 &&
                MOX::_combat_data[i].unit_status == 0 &&
                MOX::_combat_data[i].officer_idx != -1) {

                int16_t bonus = Leader_Ordnance_Bonus_(MOX::_combat_data[i].officer_idx);

                if (bonus > max_bonus) {
                    max_bonus = bonus;
                }
            }
        }

        return max_bonus;
    }

    int16_t Leader_Ordnance_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;
        
        if ((MOX::_leaders[leader_idx].special_skills & 0x400) != 0) {
            bonus = (Owned_Officer_Level_(leader_idx) + 1) * 5;
        } else if ((MOX::_leaders[leader_idx].special_skills & 0x800) != 0) {
            int16_t level = Owned_Officer_Level_(leader_idx);
            bonus = (int16_t)(((level * 3 + 3) * 5) / 2);
        }
        
        return bonus;
    }

    int16_t __cdecl Max_Defensive_Fire_Range_(int16_t ship_idx) {
        int16_t min_range = -1;

        for (int i = 0; i < 8; ++i) {
            uint16_t weapon_id = MOX::_combat_data[ship_idx].weapons[i].weapon_id;
            if ((int16_t)weapon_id > 0 &&
                TECHDATA::_weapons[(int16_t)weapon_id].rack == 0 &&
                (MOX::_combat_data[ship_idx].weapons[i].specials & 6) == 0 &&
                MOX::_combat_data[ship_idx].weapons[i].shots_left > 0) {
                min_range = 24;
            }
        }

        if (min_range > 0 && MOX::_combat_total_ship_count > 1) {
            for (int i = 1; i < MOX::_combat_total_ship_count; ++i) {
                if (MOX::_combat_data[ship_idx].owner != MOX::_combat_data[i].owner &&
                    MOX::_combat_data[i].is_captured == 0 &&
                    MOX::_combat_data[i].unit_status == 0) {
                    int16_t move_cost = CMBTMOV1::Move_Cost_(i, (int16_t)MOX::_combat_data[ship_idx].x, (int16_t)MOX::_combat_data[ship_idx].y);
                    int16_t current_speed = COMBAT1::Calc_Current_Speed_(i);
                    int16_t range = 1;
                    if (move_cost - current_speed > 1) {
                        range = move_cost - current_speed;
                    }
                    if (range < min_range) {
                        min_range = range;
                    }
                }
            }
        }
        return min_range;
    }

    bool __cdecl Apply_Combat_Artemis_Net_(char strategic) {
        int16_t ship_idx = 1;
        bool is_active = true;

        if (ship_idx < MOX::_combat_total_ship_count) {
            do {
                int16_t size_class;
                if (strategic == 0) {
                    size_class = MOX::_combat_data[ship_idx].size_class;
                } else {
                    size_class = MOX::_qcombat_data[ship_idx].size;
                }

                int16_t chance = _artemis_net_chance[size_class];
                int32_t roll = random::Random_(100);

                if (chance < roll) {
                    is_active = false;
                } else if (strategic == 0) {
                    // Tactical combat
                    for (int i = 0; i < 4; ++i) {
                        MOX::_combat_data[ship_idx].shield_arc_current[i] = 0;
                    }

                    int32_t total_damage = random::Random_(0x191) + 0x95; // Random(401) + 149
                    
                    while (total_damage > 0 && !CMBTFIRE::Is_Combat_Ship_Dead_(ship_idx)) {
                        int16_t damage = 0x14; // 20
                        int16_t unk = 0;
                        CMBTFIRE::Apply_Ship_Damage_(ship_idx, &damage, &unk, 0, 0, nullptr, 0, 0, 0);
                        total_damage -= 0x14;
                    }

                    if (!CMBTFIRE::Is_Combat_Ship_Dead_(ship_idx)) {
                        is_active = false;
                        int16_t shield_max = COMBINIT::Calc_Combat_Shield_Max_(ship_idx);
                        for (int i = 0; i < 4; ++i) {
                            MOX::_combat_data[ship_idx].shield_arc_current[i] = shield_max;
                        }
                    } else {
                        MOX::_combat_data[ship_idx].unit_status = 5; // Destroyed
                    }
                } else {
                    // Strategic combat
                    int16_t shield_id = MOX::_qcombat_data[ship_idx].shield_id;
                    int16_t damage = (int16_t)(random::Random_(0xC9) + 0x4A); // Random(201) + 74
                    
                    MOX::_qcombat_data[ship_idx].shield_id = 0;
                    COMBAT::Apply_Damage_To_Qship_(ship_idx, damage);
                    MOX::_qcombat_data[ship_idx].shield_id = shield_id;

                    if (MOX::_qcombat_data[ship_idx].status != 1) { // 1 = Destroyed?
                        is_active = false;
                    } else {
                        MOX::_qcombat_data[ship_idx].status = 2; // Inactive?
                        MOX::_combat_attacker_count--;
                    }
                }
                ship_idx++;
            } while (ship_idx < MOX::_combat_total_ship_count);
        }
        return is_active;
    }
}