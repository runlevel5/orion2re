#include "pch.h"

namespace CMBTFIR2 {
    int16_t _artemis_net_chance[6] = { 20, 30, 40, 50, 80, 100 };

    int16_t __cdecl Helmsman_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;
        uint32_t special_skills = MOX::_leaders[leader_idx].special_skills;

        if (special_skills & LEADER_SHIP_SKILL_HELMSMAN) {
            int16_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            return (int16_t)((level + 1) * 5);
        }

        if (special_skills & LEADER_SHIP_SKILL_HELMSMAN2) {
            int16_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            bonus = (int16_t)(((level * 3 + 3) * 5) / 2);
        }

        return bonus;
    }

    int16_t __cdecl Weaponry_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;

        if ((MOX::_leaders[leader_idx].special_skills & LEADER_SHIP_SKILL_WEAPONRY) != 0) {
            bonus = OFFICER::Owned_Officer_Level_(leader_idx);
            return (int16_t)((bonus + 1) * 5);
        }

        if ((MOX::_leaders[leader_idx].special_skills & LEADER_SHIP_SKILL_WEAPONRY2) != 0) {
            bonus = OFFICER::Owned_Officer_Level_(leader_idx);
            bonus = (int16_t)(((bonus * 3 + 3) * 5) / 2);
        }

        return bonus;
    }

    int16_t __cdecl Engineer_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;

        if ((MOX::_leaders[leader_idx].special_skills & LEADER_SHIP_SKILL_ENGINEER) != 0) {
            bonus = OFFICER::Owned_Officer_Level_(leader_idx);
            return bonus * 2 + 2;
        }

        if ((MOX::_leaders[leader_idx].special_skills & LEADER_SHIP_SKILL_ENGINEER2) != 0) {
            bonus = OFFICER::Owned_Officer_Level_(leader_idx);
            bonus = (bonus + 1) * 3;
        }

        return bonus;
    }

    int16_t __cdecl Fighter_Pilot_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;
        uint32_t special_skills = MOX::_leaders[leader_idx].special_skills;

        if (special_skills & LEADER_SHIP_SKILL_FIGHTER_PILOT) {
            int16_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            return (level + 1) * 5;
        }

        if (special_skills & LEADER_SHIP_SKILL_FIGHTER_PILOT2) {
            int16_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            bonus = (int16_t)(((level * 3 + 3) * 5) / 2);
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

        if (special_skills & LEADER_SHIP_SKILL_GALACTIC_LORE) {
            int16_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            return (int16_t)((level + 1) * 5);
        }

        if (special_skills & LEADER_SHIP_SKILL_GALACTIC_LORE2) {
            int32_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            bonus = (int16_t)(((level * 3 + 3) * 5) / 2);
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

    int16_t __cdecl Fleet_Defensive_Bonus_(int16_t player_idx, int16_t mode, int16_t* bonus_val_ptr) {
        int16_t max_helmsman_bonus = 0;

        if (mode == 1) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                s_combat_data* ship = &MOX::_combat_data[i];
                if ((int16_t)ship->owner == player_idx && ship->is_captured == 0 && ship->unit_status == 0) {
                    if (COMBINIT::Does_Combat_Ship_Have_Special_(i, SPECIAL_WIDE_AREA_JAMMER)) {
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
                if ((int16_t)ship->owner == player_idx && ship->is_captured == 0 && ship->unit_status == 0) {
                    if (ship->officer_idx > -1) {
                        int16_t bonus = Helmsman_Bonus_(ship->officer_idx);
                        if (bonus > max_helmsman_bonus) {
                            max_helmsman_bonus = bonus;
                        }
                    }
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

        if (MOX::_combat_data[combat_unit_idx].ftl_type > 0 && owner < MAX_PLAYERS) {
            trait_bonus = (int16_t)MOX::_player[owner].traits[TRAIT_SHIP_DEFENSE];
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

        if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_BATTLE_SCANNER) != 0) {
            bonus = 50;
        }

        return bonus;
    }

    int16_t __cdecl Fleet_Offensive_Bonus_(int16_t player_idx) {
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
        if (owner < MAX_PLAYERS) {
            trait_bonus = (int16_t)MOX::_player[owner].traits[TRAIT_SHIP_ATTACK];
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

    void __cdecl Set_Combat_Ship_Done_(int16_t ship_idx) {
        struct_::Set_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 2);
    }

    int16_t __cdecl Used_Teleporter_(int16_t ship_idx) {
        return (int16_t)struct_::Test_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 0);
    }

    void __cdecl Set_Teleporter_Used_(int16_t ship_idx) {
        struct_::Set_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 0);
    }

    int16_t __cdecl Get_Fleet_Ordnance_Bonus_(int16_t owner) {
        int16_t max_bonus = 0;

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            if (MOX::_combat_data[i].owner == owner &&
                MOX::_combat_data[i].is_captured == 0 &&
                MOX::_combat_data[i].unit_status == 0 &&
                MOX::_combat_data[i].officer_idx > -1) {

                int16_t bonus = Leader_Ordnance_Bonus_(MOX::_combat_data[i].officer_idx);

                if (bonus > max_bonus) {
                    max_bonus = bonus;
                }
            }
        }

        return max_bonus;
    }

    int16_t __cdecl Get_Fleet_Engineer_Bonus_(int16_t player_idx) {
        int16_t max_bonus = 0;

        if (MOX::_combat_total_ship_count <= 0) {
            return max_bonus;
        }

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            s_combat_data* ship = &MOX::_combat_data[i];

            if ((int16_t)ship->owner == player_idx && ship->is_captured == 0 && ship->unit_status == 0) {
                if (ship->officer_idx > -1) {
                    int16_t current_bonus = CMBTFIR2::Engineer_Bonus_(ship->officer_idx);
                    if (current_bonus > max_bonus) {
                        max_bonus = current_bonus;
                    }
                }
            }
        }

        return max_bonus;
    }

    int16_t __cdecl Get_Fleet_Pilot_Bonus_(int16_t owner_idx) {
        int16_t max_bonus = 0;

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            s_combat_data& ship = MOX::_combat_data[i];

            if ((int16_t)ship.owner == owner_idx) {
                if (ship.is_captured == 0) {
                    if (ship.unit_status == 0 /* ACTIVE */) {
                        if (ship.officer_idx > -1) {
                            int16_t bonus = Fighter_Pilot_Bonus_(ship.officer_idx);
                            if (bonus > max_bonus) {
                                max_bonus = bonus;
                            }
                        }
                    }
                }
            }
        }

        return max_bonus;
    }

    int16_t Leader_Ordnance_Bonus_(int16_t leader_idx) {
        int16_t bonus = 0;
        
        if ((MOX::_leaders[leader_idx].special_skills & LEADER_SHIP_SKILL_ORDNANCE) != 0) {
            bonus = (OFFICER::Owned_Officer_Level_(leader_idx) + 1) * 5;
        } else if ((MOX::_leaders[leader_idx].special_skills & LEADER_SHIP_SKILL_ORDNANCE2) != 0) {
            int16_t level = OFFICER::Owned_Officer_Level_(leader_idx);
            bonus = (int16_t)(((level * 3 + 3) * 5) / 2);
        }
        
        return bonus;
    }

    int16_t __cdecl Max_Defensive_Fire_Range_(int16_t ship_idx) {
        int16_t min_range = -1;

        for (int i = 0; i < 8; ++i) {
            uint16_t weapon_id = MOX::_combat_data[ship_idx].weapons[i].weapon_id;
            if ((int16_t)weapon_id > 0 &&
                TECHDATA::_weapons[(int16_t)weapon_id].type == WEAPON_TYPE_BEAM &&
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
                int32_t roll = game_random::Random_(100);

                if (chance < roll) {
                    is_active = false;
                } else if (strategic == 0) {
                    // Tactical combat
                    for (int i = 0; i < 4; ++i) {
                        MOX::_combat_data[ship_idx].shield_arc_current[i] = 0;
                    }

                    int32_t total_damage = game_random::Random_(0x191) + 0x95; // Random(401) + 149
                    
                    while (total_damage > 0 && !CMBTFIRE::Is_Combat_Ship_Dead_(ship_idx)) {
                        int16_t damage = 0x14; // 20
                        int16_t sys_damage = 0;
                        CMBTFIRE::Apply_Ship_Damage_(ship_idx, &damage, &sys_damage, 0, 0, 0, 0, 0, 0);
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
                    int16_t damage = (int16_t)(game_random::Random_(0xC9) + 0x4A); // Random(201) + 74
                    
                    MOX::_qcombat_data[ship_idx].shield_id = SHIELD_NO_SHIELD;
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

    int16_t __cdecl Max_Total_Internal_Space_(int16_t ship_idx) {
        s_combat_data* ship = &MOX::_combat_data[ship_idx];

        int16_t total_space = ship->drive_max_hits / 2;
        total_space += ship->computer_max_hits;

        for (int i = 0; i < 8; ++i) {
            total_space += ship->weapon_total_hits[i];
        }

        for (int i = 0; i < 8; ++i) {
            total_space += ship->special_device_hits[i];
        }

        total_space += ship->shield_max_hits;

        return total_space;
    }

    void __cdecl Init_Internal_Space_(s_combat_data* ship) {
        if (ship->ftl_type <= 0) {
            ship->drive_max_hits = 0;
        } else {
            ship->drive_max_hits = TECHDATA::_hull_data[ship->size_class].drive_hp;
        }

        if (struct_::Test_Bit_Field_(ship->special_device_flags, 27 /* SPEC_STRUCTURAL_ANALYZER */) &&
            !struct_::Test_Bit_Field_(ship->special_device_damage_flags, 27 /* SPEC_STRUCTURAL_ANALYZER */)) {
            ship->drive_max_hits *= 3;
        }

        ship->computer_max_hits = TECHDATA::_hull_data[ship->size_class].computer_hp;

        for (int i = 0; i < 8; ++i) {
            if (ship->weapons[i].weapon_id < 1) {
                ship->weapon_unit_hits[i] = 0;
                ship->weapon_total_hits[i] = 0;
            } else {
                int16_t space = DESIGN::One_Weapon_Space_Without_M_Level_(
                    ship->weapons[i].weapon_id,
                    ship->weapons[i].arc,
                    ship->weapons[i].ammo,
                    ship->weapons[i].specials
                );

                int16_t hits = space / 10;
                if (hits < 2) {
                    hits = 1;
                }
                ship->weapon_unit_hits[i] = hits;
                ship->weapon_total_hits[i] = (int16_t)ship->weapons[i].count * hits;
            }
        }

        memset(ship->special_device_hits, 0, sizeof(ship->special_device_hits));

        int device_idx = 0;
        for (int bit = 0; bit < 39; ++bit) {
            if (device_idx >= 8) {
                break;
            }

            if (struct_::Test_Bit_Field_(ship->special_device_flags, bit) &&
                !struct_::Test_Bit_Field_(ship->special_device_damage_flags, bit)) {

                if (bit == 27 /* SPEC_STRUCTURAL_ANALYZER */ || bit == 11 /* SPEC_HEAVIER_ARMOR */ || 
                    bit == 4 /* SPEC_BATTLE_PODS */ || bit == 14 /* SPEC_REINFORCED_HULL */ || 
                    bit == 24 /* SPEC_STEALTH_FIELD */) {
                    ship->special_device_hits[device_idx] = 0;
                } else {
                    int16_t hits = TECHDATA::_specials[bit].space_per_ship_type[ship->size_class] / 10;
                    if (hits < 2) {
                        hits = 1;
                    }
                    ship->special_device_hits[device_idx] = hits;
                }
                device_idx++;
            }
        }

        if (ship->shield_type <= 0) {
            ship->shield_max_hits = 0;
        } else {
            ship->shield_max_hits = TECHDATA::_hull_data[ship->size_class].shield_hp;
        }

        CMBTFIR2::Clear_Internal_Space_(ship);
    }

    void __cdecl Repair_Combat_Ship_(int16_t ship_idx, int16_t amount) {
        s_combat_data* ship = &MOX::_combat_data[ship_idx];
        int16_t armor_hits = 0;

        if (ship_idx == 0) {
            if (ship->structure_damage > 0) {
                int32_t repair = (amount * ship->structure_damage) / 100;
                if (repair < 2) repair = 1;
                ship->structure_damage -= (int16_t)repair;
                return;
            }
            if (ship->structure_max > 0) {
                int32_t repair = (amount * ship->structure_max) / 100;
                if (repair < 2) repair = 1;
                ship->structure_max -= (int16_t)repair;
                return;
            }
            return;
        }

        if (ship->ship_idx >= 0) {
            armor_hits = INITSHIP::Get_Ship_Armor_Hits_(ship->ship_idx);
        } else {
            int8_t size = ship->size_class;
            if (ship->ftl_type == 0) {
                if (size == 2) {
                    armor_hits = TECHDATA::_hull_data[HULL_STAR_BASE].armor_hp + 
                                 (int16_t)((int32_t)TECHDATA::_armor[ship->armor_type].ships_bonus * 
                                           TECHDATA::_hull_data[HULL_STAR_BASE].armor_hp / 100);
                } else if (size == 3) {
                    armor_hits = TECHDATA::_hull_data[HULL_BATTLESTATION].armor_hp + 
                                 (int16_t)((int32_t)TECHDATA::_armor[ship->armor_type].ships_bonus * 
                                           TECHDATA::_hull_data[HULL_STAR_BASE].armor_hp / 100);
                } else if (size == 4) {
                    armor_hits = TECHDATA::_hull_data[HULL_STAR_FORTRESS].armor_hp + 
                                 (int16_t)((int32_t)TECHDATA::_armor[ship->armor_type].ships_bonus * 
                                           TECHDATA::_hull_data[HULL_STAR_BASE].armor_hp / 100);
                }
            } else {
                armor_hits = TECHDATA::_hull_data[size].armor_hp + 
                             (int16_t)((int32_t)TECHDATA::_armor[ship->armor_type].ships_bonus * 
                                       TECHDATA::_hull_data[size].armor_hp / 100);
            }
            
            if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_HEAVY_ARMOR)) {
                armor_hits *= 3;
            }
        }

        int32_t hull_repair = (amount * (ship->structure_max + armor_hits)) / 100;
        if (hull_repair < 1) hull_repair = 1;

        ship->structure_damage -= (int16_t)hull_repair;
        if (ship->structure_damage < 0) {
            ship->armor_remaining -= ship->structure_damage;
            ship->structure_damage = 0;
            if (ship->armor_remaining > armor_hits) {
                ship->armor_remaining = armor_hits;
            }
        }

        int32_t pool = (amount * Max_Total_Internal_Space_(ship_idx)) / 200;
        if (pool < 2) pool = 1;

        if (ship->drive_current_hits < ship->drive_max_hits && pool > 0) {
            int16_t needed = ship->drive_max_hits - ship->drive_current_hits;
            int16_t amt = (pool < (int32_t)needed) ? (int16_t)pool : needed;
            ship->drive_current_hits += amt;
            pool -= (int32_t)amt;
        }

        if (ship->computer_current_hits < ship->computer_max_hits && pool > 0) {
            int16_t needed = ship->computer_max_hits - ship->computer_current_hits;
            int16_t amt = (pool < (int32_t)needed) ? (int16_t)pool : needed;
            ship->computer_current_hits += amt;
            pool -= (int32_t)amt;
        }

        for (int i = 0; i < 8 && pool > 0; i++) {
            if (ship->weapon_hits[i] < ship->weapon_total_hits[i]) {
                int16_t needed = ship->weapon_total_hits[i] - ship->weapon_hits[i];
                int16_t amt = (pool < (int32_t)needed) ? (int16_t)pool : needed;
                ship->weapon_hits[i] += amt;
                pool -= (int32_t)amt;
            }
        }

        if (ship->shield_current_hits < ship->shield_max_hits && pool > 0) {
            int16_t needed = (int16_t)ship->shield_max_hits - ship->shield_current_hits;
            int16_t amt = (pool < (int32_t)needed) ? (int16_t)pool : needed;
            ship->shield_current_hits += amt;
            pool -= (int32_t)amt;
        }

        for (int i = 0; i < 8 && pool > 0; i++) {
            if (ship->special_hits[i] < ship->special_device_hits[i]) {
                int16_t needed = ship->special_device_hits[i] - ship->special_hits[i];
                int16_t amt = (pool < (int32_t)needed) ? (int16_t)pool : needed;
                pool -= (int32_t)amt;
                // Note: Logic in binary sets hits to max even if pool was insufficient
                ship->special_hits[i] = ship->special_device_hits[i];
            }
        }
    }

    int16_t __cdecl Ship_Specials_Defensive_Bonus_(int16_t ship_idx, int16_t target_type, int16_t* field_bonus_out) {
        int16_t total_bonus = 0;
        s_combat_data* ship = &MOX::_combat_data[ship_idx];

        // target_type: 0 = Ship, 1 = Missile/Other
        if (ship->special_status_flag == 1 /* STATUS_PHANTOM_SHADOW */ && target_type == 0) {
            total_bonus = 80;
        }

        if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0x26 /* SPECIAL_DISPLACEMENT_FIELD */)) {
            if (target_type == 1) {
                total_bonus += 130;
                *field_bonus_out = 130;
            }
        } else if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0x15 /* SPECIAL_INERTIAL_NULLIFIER */)) {
            if (target_type == 1) {
                total_bonus += 100;
                *field_bonus_out = 100;
            }
        } else if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0x09 /* SPECIAL_INERTIAL_STABILIZER */)) {
            if (target_type == 1) {
                total_bonus += 70;
                *field_bonus_out = 70;
            }
        }

        if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0x11 /* SPECIAL_CLOAKING_DEVICE */)) {
            if (target_type == 0) {
                total_bonus += 100;
            }
            if (target_type == 1) {
                total_bonus += 50;
            }
        }

        if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, 0x12 /* SPECIAL_STEALTH_FIELD */)) {
            if (target_type == 0) {
                total_bonus += 50;
            }
            if (target_type == 1) {
                total_bonus += 25;
            }
        }

        return total_bonus;
    }

    void __cdecl Clear_Internal_Space_(s_combat_data* combat_data) {
        memcpy(
            &combat_data->drive_current_hits,
            &combat_data->drive_max_hits,
            sizeof(combat_data->drive_max_hits) +
                sizeof(combat_data->computer_max_hits) +
                sizeof(combat_data->weapon_total_hits) +
                sizeof(combat_data->weapon_unit_hits) +
                sizeof(combat_data->special_device_hits) +
                sizeof(combat_data->shield_max_hits));
    }

    void __cdecl Final_Fire_(int16_t ship_idx, int16_t target_x, int16_t target_y, int16_t missile_idx) {
        int16_t center_x_offset;
        int16_t center_y_offset;
        int16_t saved_active[8];

        COMBINIT::Ship_Center_Offsets_(ship_idx, &center_x_offset, &center_y_offset);

        if (CMBTAI::Will_Missile_Do_Damage_(ship_idx, missile_idx) != 0) {
            int16_t m_type = MOX::_missile[missile_idx].type;
            if (TECHDATA::_weapons[m_type].type != WEAPON_TYPE_TORPEDO && MOX::_missile[missile_idx].quantity > 0) {
                for (int16_t i = 0; i < 8; ++i) {
                    saved_active[i] = (int16_t)MOX::_combat_data[ship_idx].weapons[i].active;
                    MOX::_combat_data[ship_idx].weapons[i].active = -1;
                }

                for (int16_t i = 0; i < 8; ++i) {
                    if (MOX::_missile[missile_idx].type <= 0) {
                        break;
                    }

                    int16_t weapon_id = MOX::_combat_data[ship_idx].weapons[i].weapon_id;
                    bool can_fire_pulsar = false;

                    if (weapon_id == 13 /* PULSAR */) {
                        if (saved_active[i] != -1 && CMBTAI::Safe_To_Fire_Sphere_Weapon_(ship_idx)) {
                            can_fire_pulsar = true;
                        }
                    }

                    MOX::_combat_data[ship_idx].weapons[i].active = 1;

                    bool should_fire = false;
                    if (TECHDATA::_weapons[weapon_id].type == WEAPON_TYPE_BEAM) {
                        if (MOX::_combat_data[ship_idx].weapons[i].specials & 4) {
                            should_fire = true;
                        }
                    } else {
                        should_fire = false;
                    }

                    if (should_fire || can_fire_pulsar || weapon_id == 33 /* BLACK_HOLE_GENERATOR */) {
                        int16_t rel_target_x = target_x - center_x_offset;
                        int16_t rel_target_y = target_y - center_y_offset;

                        if (weapon_id == 13 /* PULSAR */) {
                            if ((rel_target_x % 20 == 0) && (rel_target_y % 20 == 0)) {
                                if ((int16_t)MOX::_combat_data[ship_idx].x == (rel_target_x / 20) &&
                                    (int16_t)MOX::_combat_data[ship_idx].y == (rel_target_y / 20)) {
                                    CMBTFIRE::Fire_Ship_(ship_idx, rel_target_x, rel_target_y, missile_idx,
                                                        MOX::_missile[missile_idx].x, MOX::_missile[missile_idx].y, 1);
                                }
                            }
                        } else {
                            CMBTFIRE::Fire_Ship_(ship_idx, rel_target_x, rel_target_y, missile_idx,
                                                MOX::_missile[missile_idx].x, MOX::_missile[missile_idx].y, 1);
                        }
                    }

                    MOX::_combat_data[ship_idx].weapons[i].active = -1;
                }

                for (int i = 0; i < 8; ++i) {
                    MOX::_combat_data[ship_idx].weapons[i].active = (int8_t)saved_active[i];
                }
            }
        }
    }

    void __cdecl Defensive_Fire_Check_(int16_t target_idx, int16_t target_x, int16_t target_y, int16_t dest_x, int16_t dest_y) {
        int16_t center_off_x;
        int16_t center_off_y;
        int16_t range1;
        int16_t range2;
        int16_t active_backup[8];

        if (MOX::_combat_data[target_idx].special_status_flag != 4 /* STASIS_FIELD */) {
            for (int16_t attacker_idx = 0; attacker_idx < MOX::_combat_total_ship_count; ++attacker_idx) {
                if (CMBTFIRE::Is_Combat_Ship_Dead_(target_idx)) {
                    return;
                }

                if (MOX::_combat_data[attacker_idx].owner != MOX::_combat_data[target_idx].owner) {
                    if (COMBAT1::Combat_Ship_Is_Functional_(attacker_idx)) {
                        int8_t status = MOX::_combat_data[attacker_idx].special_status_flag;
                        if (status != 1 && status != 4 && MOX::_combat_data[attacker_idx].ai_range_tactic > 0) {
                            COMBINIT::Ship_Center_Offsets_(target_idx, &center_off_x, &center_off_y);
                            range1 = CMBTAI::Range_To_XY_(attacker_idx, target_x + center_off_x, target_y + center_off_y);
                            range2 = CMBTAI::Range_To_XY_(attacker_idx, dest_x + center_off_x, dest_y + center_off_y);

                            if (range1 < 1) {
                                range1 = 1;
                            }

                            if (range1 <= 8 && range1 <= range2) {
                                int32_t threshold_denom = ((-2 - MOX::_combat_data[attacker_idx].ai_range_tactic) / 3) + 9;
                                int16_t threshold = (int16_t)((100 * (9 - range1)) / threshold_denom);

                                if ((int)game_random::Random_(100) <= threshold) {
                                    for (int16_t i = 0; i < 8; ++i) {
                                        active_backup[i] = MOX::_combat_data[attacker_idx].weapons[i].active;
                                        int16_t weapon_id = MOX::_combat_data[attacker_idx].weapons[i].weapon_id;

                                        if (TECHDATA::_weapons[weapon_id].type == WEAPON_TYPE_BEAM &&
                                            active_backup[i] != -1 &&
                                            (MOX::_combat_data[attacker_idx].weapons[i].specials & 6) == 0) {

                                            if (CMBTAI::Expected_Weapon_Damage_(attacker_idx, i, target_idx, 0, range1) > 0) {
                                                MOX::_combat_data[attacker_idx].weapons[i].active = 1;
                                            } else {
                                                MOX::_combat_data[attacker_idx].weapons[i].active = -1;
                                            }
                                        } else {
                                            MOX::_combat_data[attacker_idx].weapons[i].active = -1;
                                        }
                                    }

                                    CMBTFIRE::Fire_Ship_(attacker_idx, 
                                                         MOX::_combat_data[attacker_idx].x * 20, 
                                                         MOX::_combat_data[attacker_idx].y * 20, 
                                                         target_idx, target_x, target_y, 0);

                                    MOX::_combat_data[attacker_idx].ai_range_tactic = (int8_t)CMBTFIR2::Max_Defensive_Fire_Range_(attacker_idx);

                                    for (int16_t i = 0; i < 8; ++i) {
                                        MOX::_combat_data[attacker_idx].weapons[i].active = (int8_t)active_backup[i];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void __cdecl Kill_Crew_(int16_t combat_data_idx, int16_t kill_count) {
        s_combat_data& ship = MOX::_combat_data[combat_data_idx];

        // The assembly treats current marine_count as unsigned (0-255) for comparison
        if ((int16_t)(uint8_t)ship.marine_count < kill_count) {
            ship.marine_count = 0;
        } else {
            ship.marine_count -= (int8_t)kill_count;
        }
    }

    int16_t __cdecl Init_Boarding_Action_(int16_t unit_idx, int16_t* boarding_choice_out) {
        char message_buffer[80];
        int16_t boarding_strength;

        KEN::Ken_Get_Text_Message_(41 /* 0x29 */, message_buffer, sizeof(message_buffer));

        boarding_strength = (MOX::_combat_data[unit_idx].marine_count + 1) / 2;

        return CMBTDRW1::Board_Popup_(0, boarding_strength, message_buffer, boarding_choice_out);
    }

    int16_t __cdecl Combat_Ship_Done_(int16_t ship_idx) {
        return struct_::Test_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 2);
    }

    int16_t __cdecl Total_Internal_Space_(int16_t ship_idx) {
        s_combat_data* ship = &MOX::_combat_data[ship_idx];

        int16_t total = (ship->drive_current_hits / 2) + ship->computer_current_hits;

        for (int i = 0; i < 8; ++i) {
            total += ship->weapon_hits[i];
        }

        for (int i = 0; i < 8; ++i) {
            total += ship->special_hits[i];
        }

        total += ship->shield_current_hits;

        return total;
    }

    void __cdecl Select_Raid_Internal_(int16_t ship_idx, int16_t* raid_type, int16_t* component_idx) {
        uint16_t weights[20];
        int16_t total_space;
        int16_t choice;

        game_random::Random_(100);
        *raid_type = -1;

        total_space = CMBTFIR2::Total_Internal_Space_(ship_idx);
        memset(weights, 0, sizeof(weights));

        s_combat_data* ship = &MOX::_combat_data[ship_idx];

        weights[0] = ship->drive_current_hits / 2;
        weights[1] = ship->computer_current_hits;

        for (int i = 0; i < 8; ++i) {
            weights[i + 2] = ship->weapon_hits[i];
        }

        for (int i = 0; i < 8; ++i) {
            weights[i + 10] = ship->special_hits[i];
        }

        weights[18] = ship->shield_current_hits;

        for (int i = 0; i < 19; ++i) {
            if (weights[i] != 0) {
                // Calculate weight based on integrity percentage (100 - damage percentage)
                weights[i] = 100 - ((int32_t)weights[i] * 100 / total_space);
            }
        }

        choice = RUSS::Get_Weighted_Choice_Short_(weights, 19);

        if (choice == 0) {
            *raid_type = 5 /* RAID_DRIVE */;
            return;
        }

        if (choice == 1) {
            *raid_type = 8 /* RAID_COMPUTER */;
            return;
        }

        bool found = false;
        for (int i = 0; i < 8; ++i) {
            if (choice == i + 2) {
                found = true;
                *raid_type = 3 /* RAID_WEAPON */;
                *component_idx = i;
                break;
            }
        }

        if (!found) {
            for (int i = 0; i < 8; ++i) {
                if (choice == i + 10) {
                    found = true;
                    *raid_type = 9 /* RAID_SPECIAL */;
                    *component_idx = i;
                    break;
                }
            }
        }

        if (choice == 18) {
            *raid_type = 2 /* RAID_SHIELD */;
        }
    }

    void __cdecl Select_Internal_(int16_t ship_idx, int16_t* hit_type, int16_t* target_idx, int16_t heavy_hit, int16_t direct_hit, int16_t armor_piercing, int16_t internal_only) {
        int16_t random_roll;
        int16_t internal_chance;
        int16_t total_internal_space;
        uint16_t weights[19];
        int16_t choice;
        bool found;

        random_roll = (int16_t)game_random::Random_(100);
        *hit_type = -1;

        if (internal_only != 0) {
            *hit_type = 5;
            return;
        }

        if (armor_piercing == 0 && MOX::_combat_data[ship_idx].armor_remaining > 0 && direct_hit == 0) {
            *hit_type = 7;
            return;
        }

        total_internal_space = CMBTFIR2::Total_Internal_Space_(ship_idx);
        internal_chance = 100;

        if (direct_hit == 0) {
            internal_chance = (total_internal_space * 100) /
                ((MOX::_combat_data[ship_idx].structure_max - MOX::_combat_data[ship_idx].structure_damage) + total_internal_space);
        }

        if (MOX::_combat_data[ship_idx].owner > 9 && internal_chance > 25) {
            internal_chance = 25;
        }

        if (internal_chance < random_roll) {
            *hit_type = 0;
        } else {
            memset(weights, 0, sizeof(weights));

            weights[0] = MOX::_combat_data[ship_idx].drive_current_hits;
            weights[1] = MOX::_combat_data[ship_idx].computer_current_hits;

            for (int16_t i = 0; i < 8; ++i) {
                if (heavy_hit != 0) {
                    weights[i + 2] = MOX::_combat_data[ship_idx].weapon_hits[i] * 3;
                } else {
                    weights[i + 2] = MOX::_combat_data[ship_idx].weapon_hits[i];
                }
            }

            for (int16_t i = 0; i < 8; ++i) {
                weights[i + 10] = MOX::_combat_data[ship_idx].special_hits[i];
            }

            weights[18] = MOX::_combat_data[ship_idx].shield_current_hits;
            if (heavy_hit != 0) {
                weights[18] *= 3;
            }

            choice = RUSS::Get_Weighted_Choice_Short_(weights, 19);

            if (choice == 0) {
                *hit_type = 5;
                return;
            }

            if (choice == 1) {
                *hit_type = 8;
                return;
            }

            found = false;
            for (int16_t i = 0; i < 8; ++i) {
                if (i - choice == -2) {
                    found = true;
                    *hit_type = 3;
                    *target_idx = i;
                    break;
                }
            }

            if (!found) {
                for (int16_t i = 0; i < 8; ++i) {
                    if (i - choice == -10) {
                        found = true;
                        *hit_type = 9;
                        *target_idx = i;
                        break;
                    }
                }
            }

            if (choice == 18) {
                *hit_type = 2;
                return;
            }
        }
    }

    void __cdecl Clear_Capturing_Flag_(int16_t ship_idx) {
        struct_::Clear_Bit_Field_(&MOX::_combat_data[ship_idx].combat_status_flags, 4);
    }

    void __cdecl Apply_Internal_Damage_(int16_t target_ship_idx, int16_t* remaining_damage, int16_t sys_index, int16_t count, int16_t flag) {
        s_combat_data* ship = &MOX::_combat_data[target_ship_idx];

        if (sys_index == 7) {
            if (ship->armor_remaining < *remaining_damage) {
                sys_index = -1;
                *remaining_damage -= ship->armor_remaining;
                ship->armor_remaining = 0;
            } else {
                ship->armor_remaining -= *remaining_damage;
                *remaining_damage = 0;
            }
        }

        int16_t initial_damage = *remaining_damage;

        if (initial_damage > 0) {
            if (sys_index == 2) {
                ship->shield_current_hits -= initial_damage;
                if (ship->shield_current_hits <= 0) {
                    *remaining_damage = -ship->shield_current_hits;
                    ship->shield_current_hits = 0;
                    for (int i = 0; i < 4; ++i) {
                        ship->shield_arc_current[i] = 0;
                    }
                    ship->shield_arc_max = 0;
                } else {
                    int16_t max_shield = COMBINIT::Calc_Combat_Shield_Max_(target_ship_idx);
                    for (int i = 0; i < 4; ++i) {
                        if (ship->shield_arc_current[i] > max_shield) {
                            ship->shield_arc_current[i] = max_shield;
                        }
                    }
                    *remaining_damage = 0;
                }
            } else if (sys_index == 3) {
                int16_t rem = ship->weapon_hits[count] % ship->weapon_hits_per_unit[count];
                if (rem == 0) {
                    rem = ship->weapon_hits_per_unit[count];
                }

                if (rem < initial_damage) {
                    ship->weapon_hits[count] -= rem;
                    *remaining_damage -= rem;
                } else {
                    ship->weapon_hits[count] -= initial_damage;
                    *remaining_damage = 0;
                }

                int16_t old_count = ship->weapons[count].count;
                ship->weapons[count].count = (ship->weapon_hits[count] + ship->weapon_hits_per_unit[count] - 1) / ship->weapon_hits_per_unit[count];

                if (ship->weapons[count].weapon_id == 0x20) {
                    int16_t new_count = ship->weapons[count].count;
                    if (old_count != new_count) {
                        for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
                            if (MOX::_combat_data[i].stasis_source_idx == target_ship_idx) {
                                if (new_count == 0) {
                                    CMBTFIRE::Remove_Stasis_Beam_(i);
                                } else {
                                    new_count--;
                                }
                            }
                        }
                    }
                } else if (ship->weapons[count].weapon_id == 0x25) {
                    int16_t new_count = ship->weapons[count].count;
                    if (old_count != new_count) {
                        for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
                            if (MOX::_combat_data[i].black_hole_source_idx == target_ship_idx) {
                                if (new_count == 0) {
                                    MOX::_combat_data[i].black_hole_flag = 0;
                                    MOX::_combat_data[i].black_hole_source_idx = -1;
                                } else {
                                    new_count--;
                                }
                            }
                        }
                    }
                }
            } else if (sys_index == 5) {
                ship->drive_current_hits -= initial_damage;
                if (ship->drive_current_hits < 0) {
                    *remaining_damage = -ship->drive_current_hits;
                    ship->drive_current_hits = 0;
                } else {
                    *remaining_damage = 0;
                }
                ship->current_speed = COMBAT1::Calc_Current_Speed_(target_ship_idx);

                if (*remaining_damage > 0) {
                    ship->structure_damage += *remaining_damage;
                    if (ship->structure_damage > ship->structure_max) {
                        ship->structure_damage = ship->structure_max;
                    }
                    *remaining_damage = 0;
                }
            } else if (sys_index == 8) {
                ship->computer_current_hits -= initial_damage;
                if (ship->computer_current_hits < 0) {
                    *remaining_damage = -ship->computer_current_hits;
                    ship->computer_current_hits = 0;
                } else {
                    *remaining_damage = 0;
                }
            } else if (sys_index == 9) {
                ship->special_hits[count] -= initial_damage;
                if (ship->special_hits[count] < 0) {
                    *remaining_damage = -ship->special_hits[count];
                    ship->special_hits[count] = 0;

                    int16_t flag_idx = 0;
                    for (int16_t i = 0; i < 39; ++i) {
                        if (struct_::Test_Bit_Field_(ship->special_device_flags, i)) {
                            if (flag_idx == count) {
                                struct_::Set_Bit_Field_(ship->special_device_damage_flags, i);
                                if (i == 13 && MOX::_in_nebula_flag == 1) {
                                    for (int j = 0; j < 4; ++j) {
                                        ship->shield_arc_current[j] = 0;
                                    }
                                    ship->shield_arc_max = 0;
                                }
                                break;
                            }
                            flag_idx++;
                        }
                    }
                } else {
                    *remaining_damage = 0;
                }
            } else if (sys_index == 7 || sys_index == 0) {
                ship->structure_damage += initial_damage;
                if (ship->structure_damage > ship->structure_max) {
                    ship->structure_damage = ship->structure_max;
                }
                *remaining_damage = 0;
            }
        }

        int16_t damage_absorbed = initial_damage - *remaining_damage;

        if (flag == 1 && ship->marine_count != 0xFF && damage_absorbed > 0) {
            for (int16_t i = 0; i < damage_absorbed; ++i) {
                if (game_random::Random_(100) <= 1) {
                    Kill_Crew_(target_ship_idx, 1);
                }
                MOX::_ken_trace = 0;
            }
        }
    }

    void __cdecl Resolve_Raid_(int16_t attacker_idx, int16_t target_idx, int16_t* raiders) {
        int16_t total_damage = 0;

        s_combat_data* target = &MOX::_combat_data[target_idx];
        s_combat_data* attacker = &MOX::_combat_data[attacker_idx];

        int16_t target_owner = target->owner;
        int16_t attacker_owner = attacker->owner;

        if (attacker_owner == target_owner) {
            return;
        }

        int16_t raid_results[19];
        for (int i = 0; i < 19; i++) {
            raid_results[i] = 0;
        }

        int16_t marines = (uint8_t)target->marine_count;
        int16_t target_raiders = *raiders;

        int16_t attacker_commando_bonus = Get_Fleet_Commando_Bonus_(attacker_owner);
        int16_t attacker_boarding_bonus = Crew_Boarding_Combat_Bonus_(attacker_idx);
        int16_t target_boarding_bonus = Crew_Boarding_Combat_Bonus_(target_idx);
        int16_t has_special_1d = COMBINIT::Does_Combat_Ship_Have_Special_(target_idx, SPECIAL_SECURITY_STATIONS);
        int16_t target_commando_bonus = Get_Fleet_Commando_Bonus_(target_owner);
        int16_t target_security_bonus = Get_Fleet_Security_Bonus_(target_owner);

        s_invasion_side_info side1;
        s_invasion_side_info side2;;

        invasion::Get_Boarding_Info_(
            attacker_owner,
            *raiders,
            attacker_commando_bonus + attacker_boarding_bonus,
            target_owner,
            marines,
            target_boarding_bonus + has_special_1d * 20 + target_commando_bonus + target_security_bonus,
            &side1,
            &side2
        );

        int16_t attacker_strength = side1.unit_strengths[1];
        int16_t target_strength = side2.unit_strengths[1];

        int16_t j = 0;
        if (*raiders > 0) {
            do {
                int16_t raid_target = 50;
                int16_t diff = (int16_t)(attacker_strength + (int16_t)game_random::Random_(100));
                diff -= (int16_t)(target_strength + (int16_t)game_random::Random_(100));

                if (diff > 20) {
                    int16_t hits = 2;
                    do {
                        int16_t subsystem;
                        int16_t status = -1;
                        Select_Raid_Internal_(target_idx, &status, &subsystem);
                        if (status != -1) {
                            switch (status) {
                            case 2: raid_results[1]++; break;
                            case 3: raid_results[subsystem + 3]++; break;
                            case 5: raid_results[0]++; break;
                            case 8: raid_results[2]++; break;
                            case 9: raid_results[subsystem + 11]++; break;
                            }
                            Apply_Internal_Damage_(target_idx, &raid_target, status, subsystem, 0);
                        }
                        hits--;
                    } while (hits != 0);
                }

                if (diff > 50 && marines > 0) {
                    marines--;
                }
                if (diff < -50) {
                    target_raiders--;
                }
                j++;
            } while (j < *raiders);
        }

        for (int i = 0; i < 19; i++) {
            total_damage += raid_results[i];
        }

        char title_str[80];
        char body_str[500];
        char temp_str[80];
        char num_buf[12];

        KEN::Ken_Get_Text_Message_(42, temp_str, sizeof(temp_str));
        strlcpy(title_str, temp_str, sizeof(title_str));
        strlcat(title_str, target->name, sizeof(title_str));

        KEN::Ken_Get_Text_Message_(92, temp_str, sizeof(temp_str));
        strlcpy(body_str, temp_str, sizeof(body_str));

        snprintf(num_buf, sizeof(num_buf), "%i", attacker_strength);
        strlcat(body_str, num_buf, sizeof(body_str));

        strlcat(body_str, "          ", sizeof(body_str));

        KEN::Ken_Get_Text_Message_(93, temp_str, sizeof(temp_str));
        strlcat(body_str, temp_str, sizeof(body_str));

        snprintf(num_buf, sizeof(num_buf), "%i", target_strength);
        strlcat(body_str, num_buf, sizeof(body_str));

        strlcat(body_str, "\n\n", sizeof(body_str));

        if (total_damage <= 0) {
            KEN::Ken_Get_Text_Message_(47, temp_str, sizeof(temp_str));
            strlcat(body_str, temp_str, sizeof(body_str));
        } else {
            int16_t is_dead = CMBTFIRE::Is_Combat_Ship_Dead_(target_idx);
            if (is_dead > 0) {
                CMBTFIRE::Destroy_Ship_(target_idx, target->x * 20, target->y * 20, is_dead);
            }

            if (raid_results[0] > 0) {
                KEN::Ken_Get_Text_Message_(43, temp_str, sizeof(temp_str));
                strlcat(body_str, temp_str, sizeof(body_str));
            }

            if (raid_results[1] > 0) {
                KEN::Ken_Get_Text_Message_(44, temp_str, sizeof(temp_str));
                strlcat(body_str, temp_str, sizeof(body_str));

                if (target->shield_current_hits < 1) {
                    KEN::Ken_Get_Text_Message_(46, temp_str, sizeof(temp_str));
                } else {
                    KEN::Ken_Get_Text_Message_(45, temp_str, sizeof(temp_str));
                }
                strlcat(body_str, temp_str, sizeof(body_str));
            }

            if (raid_results[2] > 0) {
                KEN::Ken_Get_Text_Message_(21, temp_str, sizeof(temp_str));
                strlcat(body_str, temp_str, sizeof(body_str));
                strlcat(body_str, " ", sizeof(body_str));

                if (target->computer_current_hits < 1) {
                    KEN::Ken_Get_Text_Message_(46, temp_str, sizeof(temp_str));
                } else {
                    KEN::Ken_Get_Text_Message_(45, temp_str, sizeof(temp_str));
                }
                strlcat(body_str, temp_str, sizeof(body_str));
            }

            for (int k = 0; k < 8; k++) {
                if (raid_results[k + 3] > 0) {
                    snprintf(num_buf, sizeof(num_buf), "%i", raid_results[k + 3]);
                    strlcat(body_str, num_buf, sizeof(body_str));
                    strlcat(body_str, " ", sizeof(body_str));

                    if (raid_results[k + 3] < 2) {
                        strlcat(body_str, TECHDATA::_weapons[target->weapons[k].weapon_id].name, sizeof(body_str));
                    } else {
                        strlcat(body_str, TECHDATA::_weapons[target->weapons[k].weapon_id].name_plural, sizeof(body_str));
                    }
                    strlcat(body_str, " ", sizeof(body_str));

                    KEN::Ken_Get_Text_Message_(46, temp_str, sizeof(temp_str));
                    strlcat(body_str, temp_str, sizeof(body_str));
                }
            }

            for (int k = 0; k < 8; k++) {
                if (raid_results[k + 11] > 0) {
                    snprintf(num_buf, sizeof(num_buf), "%i", raid_results[k + 11]);
                    strlcat(body_str, num_buf, sizeof(body_str));
                    strlcat(body_str, " ", sizeof(body_str));

                    int16_t special_id = Get_Combat_Ship_Special_Id_(target_idx, k);
                    strlcat(body_str, TECHDATA::_specials[special_id].name, sizeof(body_str));
                    strlcat(body_str, " ", sizeof(body_str));

                    KEN::Ken_Get_Text_Message_(46, temp_str, sizeof(temp_str));
                    strlcat(body_str, temp_str, sizeof(body_str));
                }
            }
        }

        if (target_raiders < *raiders) {
            int16_t lost = *raiders - target_raiders;
            snprintf(num_buf, sizeof(num_buf), "%i", lost);
            strlcat(body_str, string::s_Newline, sizeof(body_str));
            strlcat(body_str, num_buf, sizeof(body_str));
            strlcat(body_str, " ", sizeof(body_str));

            if (lost < 2) {
                KEN::Ken_Get_Text_Message_(94, temp_str, sizeof(temp_str));
            } else {
                KEN::Ken_Get_Text_Message_(95, temp_str, sizeof(temp_str));
            }
            strlcat(body_str, temp_str, sizeof(body_str));
        }

        if (marines < (int16_t)(uint8_t)target->marine_count) {
            int16_t lost = (int16_t)(uint8_t)target->marine_count - marines;
            snprintf(num_buf, sizeof(num_buf), "%i", lost);
            strlcat(body_str, string::s_Newline, sizeof(body_str));
            strlcat(body_str, num_buf, sizeof(body_str));
            strlcat(body_str, " ", sizeof(body_str));

            if (lost < 2) {
                KEN::Ken_Get_Text_Message_(96, temp_str, sizeof(temp_str));
            } else {
                KEN::Ken_Get_Text_Message_(97, temp_str, sizeof(temp_str));
            }
            strlcat(body_str, temp_str, sizeof(body_str));
        }

        int sound_handle = ERICNET::Mox_Play_Sound_(67); // 0x43

        int16_t is_auto;
        if (MOX::_combat_attacker_player == target_owner) {
            is_auto = MOX::_auto_combat_flag[1];
        } else {
            is_auto = MOX::_auto_combat_flag[0];
        }

        bool show_timed_box = true;
        if (is_auto == 0 && target_owner < MAX_PLAYERS && MOX::_player[target_owner].objectives == PLAYER_OBJECTIVE_HUMAN) {
            show_timed_box = false;
        } else if (MOX::_auto_combat == 0 && attacker_owner < MAX_PLAYERS && MOX::_player[attacker_owner].objectives == PLAYER_OBJECTIVE_HUMAN) {
            show_timed_box = false;
        }

        if (show_timed_box) {
            TEXTBOX::Timed_Text_Box_(title_str, body_str, 54);
        } else {
            TEXTBOX::Text_Box_(title_str, body_str);
        }

        sound::Fadeout_Sound_(sound_handle, 10);

        target->marine_count = (int8_t)marines;
        *raiders = target_raiders;

        CMBTDRW1::Refresh_Combat_Screen_Full_();
    }

    void __cdecl Resolve_Capture_(int16_t attacker_idx, int16_t defender_idx, int16_t attacker_marines, int16_t target_x, int16_t target_y) {
        s_combat_data* attacker = &MOX::_combat_data[attacker_idx];
        s_combat_data* defender = &MOX::_combat_data[defender_idx];

        if (attacker->owner == defender->owner) {
            return;
        }

        int16_t defender_marines_start = (uint8_t)defender->marine_count;
        int16_t defender_marines_current = defender_marines_start;
        int16_t attacker_marines_start = attacker_marines;
        int16_t defender_marines_orig = defender_marines_start;

        int16_t attacker_commando_bonus = Get_Fleet_Commando_Bonus_((int16_t)attacker->owner);
        int16_t has_dashboards = COMBINIT::Does_Combat_Ship_Have_Special_(defender_idx, SPECIAL_SECURITY_STATIONS);
        int16_t defender_commando_bonus = Get_Fleet_Commando_Bonus_((int16_t)defender->owner);
        int16_t defender_security_bonus = Get_Fleet_Security_Bonus_((int16_t)defender->owner);
        int16_t defender_total_bonus = has_dashboards * 20 + defender_commando_bonus + defender_security_bonus;

        s_invasion_side_info side1;
        s_invasion_side_info side2;

        invasion::Get_Boarding_Info_(
            (int16_t)attacker->owner, 
            attacker_marines, 
            attacker_commando_bonus, 
            (int16_t)defender->owner, 
            defender_marines_start, 
            defender_total_bonus, 
            &side1, 
            &side2
        );

        if (defender_marines_start > 0) {
            while (attacker_marines > 0) {
                invasion::Ground_Combat_Round_(&side1, &side2);
                if (side1.round_killed_unit_type > -1) {
                    attacker_marines--;
                }
                if (side2.round_killed_unit_type > -1) {
                    defender_marines_current--;
                }
                if (defender_marines_current <= 0) break;
            }
        }

        int16_t attacker_marines_lost = attacker_marines_start - attacker_marines;
        int16_t defender_marines_lost = defender_marines_start - defender_marines_current;

        char tmp_buf[80];
        char header_buf[80];
        char msg_buf[500];

        KEN::Ken_Get_Text_Message_(0x30, tmp_buf, sizeof(tmp_buf));
        strlcpy(header_buf, tmp_buf, sizeof(header_buf));
        strlcat(header_buf, defender->name, sizeof(header_buf));

        if (attacker->owner >= MAX_PLAYERS || MOX::_player[attacker->owner].objectives != PLAYER_OBJECTIVE_HUMAN) {
            side1.unit_strengths[1] -= side1.combat_modifiers.difficulty_bonus;
        }

        if (defender->owner >= MAX_PLAYERS || MOX::_player[defender->owner].objectives != PLAYER_OBJECTIVE_HUMAN) {
            side2.unit_strengths[1] -= side2.combat_modifiers.difficulty_bonus;
        }

        FMTPARA::Set_Paragraph_Sint_Pointer_(0, &attacker_marines_start);
        FMTPARA::Set_Paragraph_Sint_Pointer_(1, &defender_marines_orig);
        FMTPARA::Set_Paragraph_Sint_Pointer_(2, &attacker_marines);
        FMTPARA::Set_Paragraph_Sint_Pointer_(3, &attacker_marines_lost);
        FMTPARA::Set_Paragraph_Sint_Pointer_(4, &defender_marines_lost);
        FMTPARA::Set_Paragraph_Sint_Pointer_(5, &defender_marines_current);
        FMTPARA::Set_Paragraph_Sint_Pointer_(6, &side1.unit_strengths[1]);
        FMTPARA::Set_Paragraph_Sint_Pointer_(7, &side2.unit_strengths[1]);

        // fmtpara 0x1A + '2' switches the paragraph to centered alignment.
        strlcpy(msg_buf, "\0322", sizeof(msg_buf));
        KEN::Ken_Get_Text_Message_(0x5C, tmp_buf, sizeof(tmp_buf));
        strlcat(msg_buf, tmp_buf, sizeof(msg_buf));
        // fmtpara 0x18 + 'B' inserts paragraph variable 6, and \aX175. moves the text cursor horizontally.
        strlcat(msg_buf, "\030B6\aX175.", sizeof(msg_buf));
        KEN::Ken_Get_Text_Message_(0x5D, tmp_buf, sizeof(tmp_buf));
        strlcat(msg_buf, tmp_buf, sizeof(msg_buf));
        KEN::Ken_Get_Text_Message_(0x64, tmp_buf, sizeof(tmp_buf));
        // fmtpara 0x18 + 'B' inserts variables 7, 0, 3, and 1, while the alignment codes and
        // \aX cursor moves lay out the boarding-strength summary across centered columns.
        strlcat(msg_buf, "\030B7\n\0321\030B0\aX80.\0321\030B3\aX255.\0320\n\0321\030B1\aX80.\0320\aX82. ", sizeof(msg_buf));
        strlcat(msg_buf, tmp_buf, sizeof(msg_buf));
        // fmtpara right-aligns variable 4, then restores left alignment and advances the cursor.
        strlcat(msg_buf, "\0321\030B4\aX255.\0320\aX257. ", sizeof(msg_buf));
        strlcat(msg_buf, tmp_buf, sizeof(msg_buf));
        // fmtpara cursor moves place divider lines and the trailing summary fields before restoring centered alignment.
        strlcat(msg_buf, "\n\aX48.-------\aX223.-------\n\0321\030B2\aX80.\0321\030B5\aX255.\0320\n\n\0322", sizeof(msg_buf));

        bool is_timed = true;
        int16_t auto_combat_flag = (defender->owner == MOX::_combat_attacker_player) ? MOX::_auto_combat_flag[0] : MOX::_auto_combat_flag[1];

        if (auto_combat_flag == 0 && defender->owner < MAX_PLAYERS && MOX::_player[defender->owner].objectives == PLAYER_OBJECTIVE_HUMAN) {
            is_timed = false;
        } else if (MOX::_auto_combat == 0 && attacker->owner < MAX_PLAYERS && MOX::_player[attacker->owner].objectives == PLAYER_OBJECTIVE_HUMAN) {
            is_timed = false;
        }

        int16_t result_msg_id;
        if (attacker_marines > 0) {
            defender->marine_count = (int8_t)(attacker_marines - defender_marines_current);
            Capture_Ship_(defender_idx, target_x, target_y);
            result_msg_id = (defender->unit_status == 0) ? 0x32 : 0x31;
        } else {
            defender->marine_count = (int8_t)defender_marines_current;
            result_msg_id = 0x33;
        }

        KEN::Ken_Get_Text_Message_(result_msg_id, tmp_buf, sizeof(tmp_buf));
        strlcat(msg_buf, tmp_buf, sizeof(msg_buf));

        int32_t sound_handle = ERICNET::Mox_Play_Sound_(0x43);

        if (!is_timed) {
            TEXTBOX::Text_Box_(header_buf, msg_buf);
        } else {
            TEXTBOX::Timed_Text_Box_(header_buf, msg_buf, 54);
        }

        sound::Fadeout_Sound_(sound_handle, 10);
        CMBTDRW1::Refresh_Combat_Screen_Full_();
    }

    int16_t __cdecl Get_Fleet_Commando_Bonus_(int16_t player_idx) {
        int16_t max_bonus = 0;

        for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
            s_combat_data& ship = MOX::_combat_data[i];

            if (ship.owner == player_idx && ship.is_captured == 0 && ship.unit_status == 0) {
                if (ship.officer_idx > -1) {
                    int16_t bonus = CMBTFIR2::Commando_Bonus_(ship.officer_idx);
                    if (bonus > max_bonus) {
                        max_bonus = bonus;
                    }
                }
            }
        }

        return max_bonus;
    }

    int16_t __cdecl Commando_Bonus_(int16_t leader_idx) {
        const s_leader_data& leader = MOX::_leaders[leader_idx];

        if (leader.general_skills & LEADER_GENERAL_SKILL_COMMANDO) {
            return OFFICER::Owned_Officer_Level_(leader_idx) * 2 + 2;
        }

        if (leader.general_skills & LEADER_GENERAL_SKILL_COMMANDO2) {
            return (OFFICER::Owned_Officer_Level_(leader_idx) + 1) * 3;
        }

        return 0;
    }

    int16_t __cdecl Crew_Boarding_Combat_Bonus_(int16_t ship_idx) {
        return MOX::_crew_data[MOX::_combat_data[ship_idx].crew_quality].ground;
    }

    int16_t __cdecl Get_Fleet_Security_Bonus_(int16_t player_idx) {
        int16_t max_bonus = 0;

        if (MOX::_combat_total_ship_count > 0) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                s_combat_data& ship = MOX::_combat_data[i];

                if (ship.owner == player_idx && ship.is_captured == 0 && ship.unit_status == 0) {
                    if (ship.officer_idx > -1) {
                        int16_t current_bonus = CMBTFIR2::Security_Bonus_(ship.officer_idx);
                        if (current_bonus > max_bonus) {
                            max_bonus = current_bonus;
                        }
                    }
                }
            }
        }

        return max_bonus;
    }

    int16_t __cdecl Security_Bonus_(int16_t leader_idx) {
        if ((MOX::_leaders[leader_idx].special_skills & LEADER_SHIP_SKILL_SECURITY) != 0) {
            return OFFICER::Owned_Officer_Level_(leader_idx) * 2 + 2;
        }

        if ((MOX::_leaders[leader_idx].special_skills & LEADER_SHIP_SKILL_SECURITY2) != 0) {
            return (OFFICER::Owned_Officer_Level_(leader_idx) + 1) * 3;
        }

        return 0;
    }

    int16_t __cdecl Get_Combat_Ship_Special_Id_(int16_t ship_idx, int16_t occurrence) {
        int16_t found_special_id = -1;
        bool is_found = false;
        int16_t current_occurrence_count = 0;

        for (int16_t bit_idx = 0; bit_idx < 39; ++bit_idx) {
            if (is_found) {
                break;
            }

            if (struct_::Test_Bit_Field_(MOX::_combat_data[ship_idx].special_device_flags, bit_idx)) {
                if (current_occurrence_count == occurrence) {
                    is_found = true;
                    found_special_id = bit_idx;
                } else {
                    current_occurrence_count++;
                }
            }
        }

        if (!is_found) {
            found_special_id = -1;
        }

        return found_special_id;
    }

    void __cdecl Capture_Ship_(int16_t ship_idx, int16_t x, int16_t y) {
        int16_t new_owner;
        s_combat_data* combat_ship = &MOX::_combat_data[ship_idx];

        if (combat_ship->officer_idx > -1) {
            OFFICER::Kill_Officer_(combat_ship->officer_idx);
            combat_ship->officer_idx = -1;
            if (combat_ship->ship_idx != -1) {
                MOX::_ship[combat_ship->ship_idx].officer_index = -1;
            }
        }

        CMBTFIR2::Clear_Capturing_Flag_(ship_idx);

        if (COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_QUANTUM_DETONATOR)) {
            if (game_random::Random_(10) < 6) {
                CMBTFIRE::Destroy_Ship_(ship_idx, x, y, 4);
                return;
            }
        }

        if (combat_ship->owner == MOX::_combat_attacker_player) {
            new_owner = MOX::_combat_defender_player;
        } else {
            new_owner = MOX::_combat_attacker_player;
        }

        combat_ship->owner = (int8_t)new_owner;

        CMBTFIRE::Retarget_Missiles_Aimed_At_(ship_idx);
        COMBAT1::Remove_Continuing_Effect_Specials_(ship_idx);

        if (new_owner >= MAX_PLAYERS || !MOX::_player[new_owner].traits[TRAIT_TELEPATHIC]) {
            combat_ship->is_captured = 1;
        }

        combat_ship->is_retreating = 0;
        combat_ship->special_cooldown = 0xFF;

        int16_t i = 0;
        while (MOX::_ship_turn_order[i] != -10) {
            i++;
        }

        for (int16_t j = i; j >= 0; j--) {
            MOX::_ship_turn_order[j + 1] = MOX::_ship_turn_order[j];
        }

        MOX::_ship_turn_order[0] = -1;
        MOX::_cur_ptr++;
        MOX::_wait_ptr++;
    }

    int16_t __cdecl Damage_From_Ship_Exploding_(int16_t ship_idx) {
        int16_t damage;
        int16_t has_special;
        int8_t size_class;

        has_special = COMBINIT::Does_Combat_Ship_Have_Special_(ship_idx, SPECIAL_QUANTUM_DETONATOR);

        size_class = MOX::_combat_data[ship_idx].size_class;
        damage = TECHDATA::_hull_data[size_class].drive_hp * 5;

        if (has_special) {
            damage *= 3;
        }

        return damage;
    }
}