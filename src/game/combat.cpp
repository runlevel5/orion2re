#include "pch.h"

namespace COMBAT {
    constexpr size_t kQCombatDataShipCount = MAX_SHIPS + 1;
    constexpr size_t kQCombatDataSize = kQCombatDataShipCount * sizeof(s_qship);

    const char s_Crystal_00557e24[8] = "Crystal";
    const char s_Eel_00557e2c[4] = "Eel";
    const char s_Hydra_00557e30[8] = "Hydra";
    const char s_Dragon_00557e38[8] = "Dragon";
    const char s_Amoeba_00557e40[8] = "Amoeba";
    const char s_Guardian_00557e48[12] = "Guardian";
    const char s_Antaran_00557e54[8] = "Antaran";

    int16_t __cdecl Strategic_Combat_(int16_t* plr1_ships, int16_t plr1_ship_count, int16_t* plr2_ships, int16_t plr2_ship_count, int16_t star_idx, int16_t colony_idx, char* summary, int32_t summary_size, char* details, int32_t details_size) {
        MOX2::action_func_t auto_fn;
        int16_t leader_combat_bonuses[20];

        for (int i = 0; i < 20; i++) {
            leader_combat_bonuses[i] = 0;
        }

        MOX2::Get_Auto_Function_(&auto_fn);
        fields::Assign_Auto_Function_(MAINSCR::Draw_Mini_Main_Screen_, 1);

        MOX::_cmbt_colony_id = colony_idx;
        MOX::_combat_turn = 0;
        MOX::_qcombat_data = (s_qship*)allocate::Allocate_First_Block_(MOX::_screen_seg, kQCombatDataSize);
        struct_::Clear_Structure_(MOX::_qcombat_data, kQCombatDataSize);

        MOX::_combat_attacker_player = MOX::_ship[plr1_ships[0]].owner;

        if (MOX::_NUM_STARS == star_idx) {
            MOX::_combat_defender_player = 8;
        } else {
            if (plr2_ship_count == 0) {
                MOX::_combat_defender_player = MOX::_colony[MOX::_cmbt_colony_id].owner;
            } else {
                MOX::_combat_defender_player = MOX::_ship[plr2_ships[0]].owner;
            }
        }

        if (MOX::_combat_attacker_player == 9 && MOX::_combat_defender_player >= MAX_PLAYERS) {
            MOX::_combat_attacker_player = 9;
            for (int i = 0; i < plr2_ship_count; i++) {
                SHIPS::Kill_Ship_(plr2_ships[i]);
            }
            MOX2::Put_Auto_Function_(&auto_fn);
            return 1;
        }

        if (MOX::_combat_defender_player == 9 && MOX::_combat_attacker_player >= MAX_PLAYERS) {
            for (int i = 0; i < plr1_ship_count; i++) {
                SHIPS::Kill_Ship_(plr1_ships[i]);
            }
            MOX2::Put_Auto_Function_(&auto_fn);
            return -1;
        }

        MOX::_colony_defense_flags = 0;
        MOX::_combat_total_ship_count = 1;

        int16_t bonus1 = 0;
        int16_t bonus2 = 0;
        int16_t bonus3 = 0;
        int16_t bonus4 = 0;

        Get_Leader_Combat_Bonuses_(plr1_ships, plr1_ship_count, &bonus1, &bonus2, &leader_combat_bonuses[MOX::_combat_attacker_player], &bonus3, &bonus4);
        Qload_Ships_(plr1_ships, plr1_ship_count, bonus1, bonus2, bonus3, bonus4);

        MOX::_combat_attacker_count = MOX::_combat_total_ship_count - 1;

        int16_t result = 0;
        int16_t colony_defense_flags = 0;
        int16_t survived_officers_count = 0;
        int16_t size_sum = 0;
        int16_t survived_officer_ids[8] = {};
        bool skip_combat = false;

        if (MOX::_combat_defender_player < MAX_PLAYERS && star_idx < MOX::_NUM_STARS) {
            if (HAROLD::Player_Has_Artemis_Net_(&MOX::_player[MOX::_combat_defender_player], &MOX::_star[star_idx])) {
                if (CMBTFIR2::Apply_Combat_Artemis_Net_(1)) {
                    result = -1;
                    skip_combat = true;
                }
            }
        }

        if (!skip_combat) {
            if (MOX::_NUM_STARS == star_idx) {
                QLoad_Antaran_Defense_Fleet_();
            } else {
                bonus1 = 0;
                bonus2 = 0;
                bonus3 = 0;
                bonus4 = 0;
                Get_Leader_Combat_Bonuses_(plr2_ships, plr2_ship_count, &bonus1, &bonus2, &leader_combat_bonuses[MOX::_combat_defender_player], &bonus3, &bonus4);
                Qload_Ships_(plr2_ships, plr2_ship_count, bonus1, bonus2, bonus3, bonus4);
            }

            MOX::_combat_defender_count = MOX::_combat_total_ship_count - MOX::_combat_attacker_count;
            if (MOX::_colony_defense_flags == 0) {
                MOX::_combat_defender_count--;
            }

            colony_defense_flags = MOX::_colony_defense_flags;

            while (MOX::_combat_attacker_count > 0 && MOX::_combat_defender_count > 0) {
                if (MOX::_combat_turn >= 50) {
                    break;
                }
                
                MOX::_combat_turn++;

                for (int i = 0; i < MOX::_combat_total_ship_count; i++) {
                    if (MOX::_qcombat_data[i].status == 2) {
                        continue;
                    }

                    int16_t target = QGet_Target_(i);
                    if (target != -1) {
                        if (i == 0 && (MOX::_colony_defense_flags & 4)) {
                            int16_t sc_target = QGet_Target_SC_(i);
                            Stellar_Converter_Attack_(sc_target);
                            target = QGet_Target_(0);
                        }

                        if (target != -1) {
                            if (MOX::_combat_turn < 4 || MOX::_qcombat_data[i].size > 5 || i == 0) {
                                if (MOX::_qcombat_data[i].player == 8) {
                                    Beam_Attack_(i, target);
                                } else {
                                    Missile_Attack_(i, target);
                                }
                            }

                            if (MOX::_combat_turn > 2 || i == 0) {
                                Strat_Special_Attack_(i, target);
                            }

                            if (MOX::_combat_turn > 2 || i == 0) {
                                Beam_Attack_(i, target);
                            }
                        }
                    }

                    if (MOX::_combat_turn > 3) {
                        if (MOX::_qcombat_data[i].player == MOX::_combat_attacker_player && MOX::_qcombat_data[0].status == 0 && MOX::_cmbt_colony_id != -1) {
                            Strat_Bomb_Attack_(i);
                        }
                    }
                }

                Resolve_Strat_Colony_Damage_();

                for (int i = 0; i < MOX::_combat_total_ship_count; i++) {
                    if (MOX::_qcombat_data[i].status == 1) {
                        MOX::_qcombat_data[i].status = 2;
                        if (MOX::_qcombat_data[i].player == MOX::_combat_attacker_player) {
                            MOX::_combat_attacker_count--;
                        } else {
                            MOX::_combat_defender_count--;
                        }
                    }
                }
            }

            if (MOX::_combat_turn < 50) {
                result = MOX::_combat_attacker_count - MOX::_combat_defender_count;
            } else {
                result = (MOX::_combat_defender_count <= 0) ? 0 : -1;
            }
        }

        for (int i = 0; i < MOX::_combat_total_ship_count; i++) {
            if (MOX::_qcombat_data[i].status != 0 && MOX::_qcombat_data[i].size > 4 && star_idx != MOX::_NUM_STARS) {
                int16_t building_id = 0;
                if (MOX::_qcombat_data[i].size == HULL_STAR_BASE) {
                    building_id = BUILDING_STAR_BASE;
                } else if (MOX::_qcombat_data[i].size == HULL_BATTLESTATION) {
                    building_id = BUILDING_BATTLESTATION;
                } else if (MOX::_qcombat_data[i].size == HULL_STAR_FORTRESS) {
                    building_id = BUILDING_STAR_FORTRESS;
                }
                
                if (building_id != 0) {
                    ERICNET::Remove_Building_(MOX::_cmbt_colony_id, building_id);
                }
            }

            int16_t ship_design_idx = MOX::_qcombat_data[i].ship_design_idx;
            if (ship_design_idx != -1) {
                uint8_t player = (uint8_t)MOX::_qcombat_data[i].player;
                if (leader_combat_bonuses[player] <= 0 && 
                   (player >= MAX_PLAYERS || (MOX::_player[player].tech_applications[TECH_APP_ADVANCED_DAMAGE_CONTROL] != TECH_RESEARCH_STATUS_RESEARCHED && MOX::_player[player].traits[TRAIT_CYBERNETIC] == 0))) {
                    if (MOX::_settings.strategic_combat_flag == 1) {
                        MOX::_ship[ship_design_idx].structural_damage = MOX::_qcombat_data[i].structural_damage;
                    } else {
                        int32_t struct_dmg = MOX::_qcombat_data[i].structural_damage;
                        int32_t max_hits = MOX::_qcombat_data[i].max_hits;
                        int32_t percentage = (struct_dmg * 100) / max_hits;
                        int32_t structure = INITSHIP::Get_Ship_Structure_(ship_design_idx);
                        MOX::_ship[ship_design_idx].structural_damage = (structure * percentage) / 100;
                    }
                } else {
                    MOX::_qcombat_data[i].structural_damage = 0;
                    MOX::_ship[ship_design_idx].structural_damage = 0;
                }
            }

            if (MOX::_qcombat_data[i].status != 0 && MOX::_qcombat_data[i].ship_design_idx != -1) {
                if (MOX::_combat_attacker_player != MOX::_qcombat_data[i].player) {
                    size_sum += MOX::_qcombat_data[i].size + 1;
                }

                int16_t officer_idx = MOX::_qcombat_data[i].officer_idx;
                if (officer_idx != -1) {
                    if (MOX::_leaders[officer_idx].eta <= 0) {
                        if (MOX::_player[MOX::_qcombat_data[i].player].tech_applications[TECH_APP_SURVIVAL_PODS] == TECH_RESEARCH_STATUS_RESEARCHED) {
                            survived_officer_ids[survived_officers_count] = officer_idx;
                            survived_officers_count++;
                        } else {
                            OFFICER::Kill_Officer_(officer_idx);
                        }
                    } else {
                        OFFICER::Move_Officer_To_Pool_(officer_idx);
                        MOX::_ship[MOX::_qcombat_data[i].ship_design_idx].officer_index = -1;
                    }
                }
            }
        }

        if (MOX::_combat_attacker_player < MAX_PLAYERS && MOX::_combat_defender_player < MAX_PLAYERS) {
            if (MOX::_cmbt_colony_id != -1) {
                NETMOX::Russ_Change_Relations_(-10000, MOX::_combat_attacker_player, MOX::_combat_defender_player, 10, star_idx, 1);
            }
            NETMOX::Russ_Change_Relations_(size_sum * -15, MOX::_combat_attacker_player, MOX::_combat_defender_player, 11, star_idx, 0);
        }

        if (survived_officers_count > 0) {
            for (int16_t j = 0; j < survived_officers_count; j++) {
                bool found = false;
                bool move_to_pool = false;
                int16_t officer_idx = survived_officer_ids[j];
                int16_t player_idx = MOX::_leaders[officer_idx].player_index;

                for (int16_t i = 1; i < MOX::_combat_total_ship_count; i++) {
                    if (MOX::_qcombat_data[i].status == 0 && MOX::_qcombat_data[i].ship_design_idx != -1) {
                        if ((uint8_t)MOX::_qcombat_data[i].player == player_idx) {
                            if (MOX::_qcombat_data[i].officer_idx == -1) {
                                found = true;
                                MOX::_ship[MOX::_qcombat_data[i].ship_design_idx].officer_index = officer_idx;
                                MOX::_qcombat_data[i].officer_idx = officer_idx;
                                MOX::_ship[MOX::_leaders[officer_idx].location].officer_index = -1;
                                MOX::_leaders[officer_idx].location = MOX::_qcombat_data[i].ship_design_idx;
                                break;
                            }
                            move_to_pool = true;
                        }
                    }
                }

                if (!found) {
                    if (move_to_pool) {
                        OFFICER::Move_Officer_To_Pool_(officer_idx);
                    } else {
                        OFFICER::Kill_Officer_(officer_idx);
                    }
                }
            }
        }

        for (int i = 0; i < MOX::_combat_total_ship_count; i++) {
            if (star_idx == MOX::_NUM_STARS && MOX::_qcombat_data[i].player == 8 && MOX::_qcombat_data[i].status != 0 && MOX::_qcombat_data[i].size <= 5) {
                antarans::Kill_Antaran_Defensive_Ship_(MOX::_qcombat_data[i].size);
            } else {
                if (MOX::_qcombat_data[i].status != 0 && MOX::_qcombat_data[i].ship_design_idx != -1) {
                    if (MOX::_qcombat_data[i].player == 8) {
                        antarans::Kill_Antaran_Offensive_Ship_(MOX::_qcombat_data[i].size);
                    }
                    if (MOX::_ship[MOX::_qcombat_data[i].ship_design_idx].officer_index != -1) {
                        OFFICER::Move_Officer_To_Pool_(MOX::_ship[MOX::_qcombat_data[i].ship_design_idx].officer_index);
                    }
                    SHIPS::Kill_Ship_(MOX::_qcombat_data[i].ship_design_idx);
                }
            }
        }

        if (result >= 1 && MOX::_combat_attacker_player < MAX_PLAYERS && MOX::_combat_defender_player >= 10) {
            if (MOX::_ship[plr2_ships[0]].d.ftl_type > 0) {
                NETMOX::Russ_Event_Monster_Update_(MOX::_combat_defender_player, -1, MOX::_combat_attacker_player);
                result = 1;
            }
        }

        if (result <= -1 && MOX::_combat_defender_player < MAX_PLAYERS && MOX::_combat_attacker_player >= 10) {
            if (MOX::_ship[plr1_ships[0]].d.ftl_type > 0) {
                NETMOX::Russ_Event_Monster_Update_(MOX::_combat_attacker_player, -1, MOX::_combat_defender_player);
                result = -1;
            }
        }

        Build_End_Of_Combat_Message_(summary, summary_size, details, details_size, star_idx, colony_defense_flags, result);

        if (MOX::_cmbt_colony_id != -1) {
            if (MOX::_colony[MOX::_cmbt_colony_id].n_pops == 0) {
                int16_t has_buildings = 0;
                for (int i = 0; i < BUILDING_COUNT; i++) {
                    has_buildings += MOX::_colony[MOX::_cmbt_colony_id].buildings[i];
                }
                
                if (has_buildings == 0) {
                    BOMB::Destroy_Colony_(&MOX::_colony[MOX::_cmbt_colony_id], 1);
                }
            }
        }

        MOX2::Put_Auto_Function_(&auto_fn);
        return result;
    }

    void __cdecl Get_Leader_Combat_Bonuses_(int16_t* ship_indices, int16_t count, int16_t* helmsman_max, 
                                            int16_t* ordnance_max, int16_t* engineer_max, 
                                            int16_t* weaponry_max, int16_t* lore_max) {
        if (count <= 0) {
            return;
        }

        for (int16_t i = 0; i < count; ++i) {
            int16_t ship_idx = ship_indices[i];
            int16_t officer_idx = MOX::_ship[ship_idx].officer_index;

            if (officer_idx != -1 && MOX::_leaders[officer_idx].eta == 0) {
                int16_t bonus;

                bonus = CMBTFIR2::Helmsman_Bonus_(officer_idx);
                if (*helmsman_max < bonus) {
                    *helmsman_max = bonus;
                }

                bonus = CMBTFIR2::Leader_Ordnance_Bonus_(MOX::_ship[ship_idx].officer_index);
                if (*ordnance_max < bonus) {
                    *ordnance_max = bonus;
                }

                bonus = CMBTFIR2::Engineer_Bonus_(MOX::_ship[ship_idx].officer_index);
                if (*engineer_max < bonus) {
                    *engineer_max = bonus;
                }

                bonus = CMBTFIR2::Weaponry_Bonus_(MOX::_ship[ship_idx].officer_index);
                if (*weaponry_max < bonus) {
                    *weaponry_max = bonus;
                }

                bonus = CMBTFIR2::Galactic_Lore_Bonus_(MOX::_ship[ship_idx].officer_index);
                if (*lore_max < bonus) {
                    *lore_max = bonus;
                }
            }
        }
    }

    void __cdecl Qload_Ships_(int16_t* ship_indices, int16_t num_ships, int16_t beam_defense_bonus, int16_t damage_bonus, int16_t base_ocv_bonus, int16_t unused_lore_bonus) {
        int16_t player_idx;
        int16_t warp_bonus;
        int16_t best_armor_id;
        int16_t armor_absorption;
        int16_t special_ocv = 0;
        int16_t special_missile_dmg = 0;
        int16_t special_beam_def = 0;
        int16_t special_missile_def = 0;
        int16_t special_beam_dmg = 0;

        if (num_ships == 0) {
            player_idx = MOX::_colony[MOX::_cmbt_colony_id].owner;
        } else {
            player_idx = MOX::_ship[ship_indices[0]].owner;
        }

        int16_t best_missile = INITSHIP::Best_Missile_Weapon_(player_idx);
        int16_t best_beam = INITSHIP::Best_Beam_Weapon_(player_idx);
        int16_t best_bomb = INITSHIP::Best_Bomb_Weapon_(player_idx);
        int16_t best_shield = INITSHIP::Best_Ship_Shield_(player_idx);
        int16_t base_ocv = AIPOWER::Base_Generic_Ocv_(player_idx);
        int16_t best_warp = INITSHIP::Best_Warp_Drive_(player_idx);

        warp_bonus = (best_warp - 1) * 10;
        if (player_idx < MAX_PLAYERS) {
            warp_bonus += MOX::_player[player_idx].traits[6];
        }

        best_armor_id = INITSHIP::Best_Armor_(player_idx);
        armor_absorption = TECHDATA::_armor[best_armor_id].ships_bonus;

        if (num_ships > 0) {
            for (int16_t i = 0; i < num_ships; ++i) {
                int16_t ship_idx = ship_indices[i];
                if (MOX::_ship[ship_idx].d.ship_type == SHIP_TYPE_COMBAT) {
                    uint8_t ship_size = MOX::_ship[ship_idx].d.size;
                    int16_t best_strat_special = INITSHIP::Best_Strat_Special_Weapon_(player_idx, ship_size);
                    INITSHIP::Update_Strategic_Ship_(ship_idx);

                    s_qship* q = &MOX::_qcombat_data[MOX::_combat_total_ship_count];
                    q->player = MOX::_ship[ship_idx].owner;
                    q->ship_design_idx = ship_idx;
                    q->size = ship_size;
                    q->status = 0;
                    q->field3_0x4 = -1;
                    q->missile_weapon_id = best_missile;
                    q->beam_weapon_id = best_beam;
                    q->bomb_weapon_id = best_bomb;
                    q->special_weapon_id = best_strat_special;
                    q->shield_id = best_shield;

                    if ((uint8_t)q->player < 10) {
                        q->max_hits = (int16_t)(((int32_t)TECHDATA::_hull_data[ship_size].strat_hits * (armor_absorption + 100)) / 100);
                    } else {
                        q->max_hits = TECHDATA::_hull_data[ship_size].strat_hits;
                    }

                    q->missile_defense_bonus = 0;
                    q->beam_defense_bonus = TECHDATA::_hull_data[ship_size].strat_def_bonus + warp_bonus;
                    q->base_ocv = base_ocv;
                    q->missile_damage_modifier = 0;
                    q->beam_damage_modifier = 0;

                    for (int16_t device_idx = 0; device_idx < 39; ++device_idx) {
                        if (struct_::Test_Bit_Field_(MOX::_ship[ship_idx].d.special_device_flags, device_idx) != 0 &&
                            struct_::Test_Bit_Field_(MOX::_ship[ship_idx].special_device_damage_flags, device_idx) == 0) {

                            int16_t bonus = TECHDATA::_specials[device_idx].strategic;
                            switch (TECHDATA::_specials[device_idx].category) {
                                case 1:
                                    if (special_ocv < bonus) special_ocv = bonus;
                                    break;
                                case 2:
                                    if (special_beam_dmg < bonus) special_beam_dmg = bonus;
                                    break;
                                case 3:
                                    if (special_beam_def < bonus) special_beam_def = bonus;
                                    break;
                                case 4:
                                    if (special_missile_def < bonus) special_missile_def = bonus;
                                    break;
                                case 5:
                                    if (special_missile_dmg < bonus) special_missile_dmg = bonus;
                                    break;
                                case 6:
                                    if (special_beam_def < bonus) special_beam_def = bonus;
                                    if (special_missile_def < bonus) special_missile_def = bonus;
                                    break;
                                case 7:
                                    if (special_beam_dmg < bonus) special_beam_dmg = bonus;
                                    if (special_missile_dmg < bonus) special_missile_dmg = bonus;
                                    break;
                            }
                        }
                    }

                    q->base_ocv += base_ocv_bonus + special_ocv;
                    q->beam_defense_bonus += beam_defense_bonus + special_beam_def;
                    q->missile_defense_bonus += special_missile_def;
                    q->beam_damage_modifier += damage_bonus + special_beam_dmg;
                    q->missile_damage_modifier += damage_bonus + special_missile_dmg;
                    q->officer_idx = MOX::_ship[ship_idx].officer_index;
                    q->structural_damage = MOX::_ship[ship_idx].structural_damage;
                    
                    MOX::_combat_total_ship_count++;
                }
            }
        }

        int16_t best_colony_strat_special = INITSHIP::Best_Strat_Special_Weapon_(player_idx, 5);
        s_qship* q0 = &MOX::_qcombat_data[0];
        q0->size = 4;
        q0->ship_design_idx = -1;
        q0->field3_0x4 = -1;
        q0->structural_damage = 0;
        q0->max_hits = 0;
        q0->missile_weapon_id = 0;
        q0->beam_weapon_id = 0;
        q0->bomb_weapon_id = 0;
        q0->special_weapon_id = 0;
        q0->missile_defense_bonus = 0;
        q0->beam_defense_bonus = 0;

        if (MOX::_cmbt_colony_id == -1 || MOX::_combat_defender_player != player_idx) {
            q0->status = 2 /* INACTIVE */;
        } else {
            q0->player = MOX::_colony[MOX::_cmbt_colony_id].owner;
            q0->max_hits = Get_Colony_Hits_();
            q0->ship_design_idx = -1;

            if (MOX::_colony[MOX::_cmbt_colony_id].buildings[BUILDING_RADIATION_SHIELD] != 0) {
                q0->missile_defense_bonus += 10;
                q0->beam_defense_bonus = (int16_t)(q0->beam_defense_bonus * 10);
            }
            if (MOX::_colony[MOX::_cmbt_colony_id].buildings[BUILDING_BARRIER_SHIELD] != 0) {
                q0->missile_defense_bonus += 30;
                q0->beam_defense_bonus = (int16_t)(q0->beam_defense_bonus * 30);
            }
            if (MOX::_colony[MOX::_cmbt_colony_id].buildings[BUILDING_FLUX_SHIELD] != 0) {
                q0->missile_defense_bonus += 20;
                q0->beam_defense_bonus = (int16_t)(q0->beam_defense_bonus * 20);
            }

            q0->missile_weapon_id = 0;
            if (MOX::_colony[MOX::_cmbt_colony_id].buildings[BUILDING_MISSILE_BASE] != 0) {
                q0->missile_weapon_id = best_missile;
                MOX::_colony_defense_flags += 1;
            }
            if (MOX::_colony[MOX::_cmbt_colony_id].buildings[BUILDING_FIGHTER_GARRISON] != 0) {
                q0->special_weapon_id = best_colony_strat_special;
                MOX::_colony_defense_flags += 8;
            }
            if (MOX::_colony[MOX::_cmbt_colony_id].buildings[BUILDING_GROUND_BATTERIES] != 0) {
                q0->beam_weapon_id = best_beam;
                MOX::_colony_defense_flags += 2;
            }
            if (MOX::_colony[MOX::_cmbt_colony_id].buildings[BUILDING_STELLAR_CONVERTER] != 0) {
                MOX::_colony_defense_flags += 4;
            }

            if (MOX::_colony_defense_flags == 0) {
                q0->status = 2 /* INACTIVE */;
            } else {
                q0->status = 0 /* ACTIVE */;
            }

            s_colony* colony_ptr = &MOX::_colony[MOX::_cmbt_colony_id];
            if (colony_ptr->buildings[BUILDING_STAR_BASE] != 0) {
                s_qship* qs = &MOX::_qcombat_data[MOX::_combat_total_ship_count];
                qs->player = colony_ptr->owner;
                qs->ship_design_idx = -1;
                qs->size = HULL_STAR_BASE;
                qs->status = 0;
                qs->field3_0x4 = -1;
                qs->missile_weapon_id = best_missile;
                qs->beam_weapon_id = best_beam;
                qs->bomb_weapon_id = best_bomb;
                qs->special_weapon_id = best_colony_strat_special;
                qs->shield_id = best_shield;
                qs->max_hits = (int16_t)(((armor_absorption + 100) * (int32_t)TECHDATA::_hull_data[HULL_STAR_BASE].strat_hits) / 100);
                qs->missile_defense_bonus = 0;
                qs->beam_defense_bonus = TECHDATA::_hull_data[HULL_STAR_BASE].strat_def_bonus;
                qs->base_ocv = base_ocv;
                qs->structural_damage = 0;
                MOX::_combat_total_ship_count++;
                return;
            }
            if (colony_ptr->buildings[BUILDING_BATTLESTATION] != 0) {
                s_qship* qs = &MOX::_qcombat_data[MOX::_combat_total_ship_count];
                qs->player = colony_ptr->owner;
                qs->ship_design_idx = -1;
                qs->size = HULL_BATTLESTATION;
                qs->status = 0;
                qs->field3_0x4 = -1;
                qs->missile_weapon_id = best_missile;
                qs->beam_weapon_id = best_beam;
                qs->bomb_weapon_id = best_bomb;
                qs->special_weapon_id = best_colony_strat_special;
                qs->shield_id = best_shield;
                qs->max_hits = (int16_t)(((armor_absorption + 100) * (int32_t)TECHDATA::_hull_data[HULL_BATTLESTATION].strat_hits) / 100);
                qs->missile_defense_bonus = 0;
                qs->beam_defense_bonus = TECHDATA::_hull_data[HULL_BATTLESTATION].strat_def_bonus;
                qs->base_ocv = base_ocv;
                qs->structural_damage = 0;
                MOX::_combat_total_ship_count++;
                return;
            }
            if (colony_ptr->buildings[BUILDING_STAR_FORTRESS] != 0) {
                s_qship* qs = &MOX::_qcombat_data[MOX::_combat_total_ship_count];
                qs->player = colony_ptr->owner;
                qs->ship_design_idx = -1;
                qs->size = HULL_STAR_FORTRESS;
                qs->status = 0;
                qs->field3_0x4 = -1;
                qs->missile_weapon_id = best_missile;
                qs->beam_weapon_id = best_beam;
                qs->bomb_weapon_id = best_bomb;
                qs->special_weapon_id = best_colony_strat_special;
                qs->shield_id = best_shield;
                qs->max_hits = (int16_t)(((armor_absorption + 100) * (int32_t)TECHDATA::_hull_data[HULL_STAR_FORTRESS].strat_hits) / 100);
                qs->missile_defense_bonus = 0;
                qs->beam_defense_bonus = TECHDATA::_hull_data[HULL_STAR_FORTRESS].strat_def_bonus;
                qs->base_ocv = base_ocv;
                qs->structural_damage = 0;
                MOX::_combat_total_ship_count++;
                return;
            }
        }
    }

    int16_t __cdecl QGet_Target_(int16_t ship_idx) {
        int8_t player = MOX::_qcombat_data[ship_idx].player;
        int16_t targets[502];
        int16_t target_count = 0;

        for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
            // Target must belong to another player and be active
            if (MOX::_qcombat_data[i].player != player && MOX::_qcombat_data[i].status == 0 /* Active */) {
                targets[target_count] = i;
                target_count++;
            }
        }

        if (target_count != 0) {
            // Random_ returns 1..N based on assembly array access pattern [rand*2 + 0x12]
            int random_val = game_random::Random_(target_count);
            return targets[random_val - 1];
        }

        // If no ship targets were found, check if we can target the fixed defense (index 0)
        // Only the attacker can target the colony/starbase.
        if (MOX::_combat_attacker_player == (int16_t)player && MOX::_cmbt_colony_id != (int16_t)-1) {
            if (MOX::_qcombat_data[0].status == 0 /* Active */) {
                return 0;
            }
        }

        return -1;
    }

    void __cdecl QLoad_Combat_Antaran_Ship_(int16_t q_idx, int16_t size_idx) {
        int16_t missile_id;
        int16_t beam_id;
        int16_t bomb_id;
        int16_t shield_id;
        int16_t ocv;
        int16_t armor_idx;
        int16_t absorption_percent;
        int16_t special_weapon_id;

        missile_id = INITSHIP::Best_Missile_Weapon_(8);
        beam_id = INITSHIP::Best_Beam_Weapon_(8);
        bomb_id = INITSHIP::Best_Bomb_Weapon_(8);
        shield_id = INITSHIP::Best_Ship_Shield_(8);
        ocv = AIPOWER::Base_Generic_Ocv_(8);
        armor_idx = INITSHIP::Best_Armor_(8);
        
        // Explicit lookup based on struct layout and assembly
        absorption_percent = TECHDATA::_armor[armor_idx].ships_bonus;
        
        special_weapon_id = INITSHIP::Best_Strat_Special_Weapon_(8, size_idx);

        s_qship& ship = MOX::_qcombat_data[q_idx];

        ship.player = 8;
        ship.ship_design_idx = -1;
        ship.size = (uint8_t)size_idx;
        ship.status = 0;
        ship.field3_0x4 = -1;
        ship.missile_weapon_id = missile_id;
        ship.beam_weapon_id = beam_id;
        ship.bomb_weapon_id = bomb_id;
        ship.special_weapon_id = special_weapon_id;
        ship.shield_id = shield_id;

        if (ship.player <= 9) {
            ship.max_hits = (int16_t)(((int32_t)TECHDATA::_hull_data[size_idx].strat_hits * (absorption_percent + 100)) / 100);
        } else {
            ship.max_hits = TECHDATA::_hull_data[size_idx].strat_hits;
        }

        ship.missile_defense_bonus = 0;
        ship.beam_defense_bonus = TECHDATA::_hull_data[size_idx].strat_def_bonus + 95 /* 0x5f */;
        ship.base_ocv = ocv + 45 /* 0x2d */;
        ship.missile_damage_modifier = 0;
        ship.beam_damage_modifier = 105 /* 0x69 */;
        ship.officer_idx = -1;
        ship.structural_damage = 0;
    }

    void __cdecl QLoad_Antaran_Defense_Fleet_() {
        if (MOX::_antarans.defensive_ships[4] < 1) {
            MOX::_antarans.defensive_ships[4] = 1;
        }

        for (int16_t ship_type = 0; ship_type < 5; ++ship_type) {
            int16_t count = MOX::_antarans.defensive_ships[ship_type];
            if (count > 0) {
                for (int16_t i = 0; i < count; ++i) {
                    QLoad_Combat_Antaran_Ship_(MOX::_combat_total_ship_count, ship_type);
                    MOX::_combat_total_ship_count++;
                    MOX::_combat_defender_count++;
                }
            }
        }

        s_qship* qship = &MOX::_qcombat_data[MOX::_combat_total_ship_count];

        qship->player = 8;
        qship->ship_design_idx = -1;
        qship->size = HULL_STAR_FORTRESS;
        qship->status = 0;
        qship->field3_0x4 = -1;
        qship->missile_weapon_id = INITSHIP::Best_Missile_Weapon_(8);
        qship->beam_weapon_id = INITSHIP::Best_Beam_Weapon_(8);
        qship->bomb_weapon_id = 0;
        qship->special_weapon_id = 0;
        qship->shield_id = 7;
        qship->max_hits = (int16_t)(((int32_t)TECHDATA::_hull_data[HULL_STAR_FORTRESS].strat_hits * 1000) / 100);
        qship->missile_defense_bonus = 0;
        qship->beam_defense_bonus = TECHDATA::_hull_data[HULL_STAR_FORTRESS].strat_def_bonus + 45 /* 0x2d */;
        qship->base_ocv = AIPOWER::Base_Generic_Ocv_(8) + 45 /* 0x2d */;
        qship->missile_damage_modifier = 0;
        qship->beam_damage_modifier = 105 /* 0x69 */;
        qship->structural_damage = 0;

        MOX::_combat_total_ship_count++;
        MOX::_combat_defender_count++;
    }

    void __cdecl Stellar_Converter_Attack_(int16_t target_idx) {
        for (int i = 0; i < 4; ++i) {
            Apply_Damage_To_Qship_(target_idx, TECHDATA::_weapons[WEAPON_STELLAR_CONVERTER].max_damage);
        }
    }

    int16_t __cdecl QGet_Target_SC_(int16_t attacker_idx) {
        int8_t attacker_player = MOX::_qcombat_data[attacker_idx].player;
        int16_t target_idx = -1;
        int16_t max_hits_remaining = 0;

        // The loop starts at ship index 1, effectively ignoring index 0
        if (MOX::_combat_total_ship_count > 1) {
            for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
                s_qship* ship = &MOX::_qcombat_data[i];

                // Target must belong to a different player and be active
                if (ship->player != attacker_player && ship->status == 0 /* Active */) {
                    int16_t hits_remaining = ship->max_hits - ship->structural_damage;

                    // Prefer targets with the highest remaining hit points
                    if (hits_remaining > max_hits_remaining) {
                        max_hits_remaining = hits_remaining;
                        target_idx = i;
                    }
                }
            }
        }

        return target_idx;
    }

    void __cdecl Beam_Attack_(int16_t attacker_idx, int16_t target_idx) {
        s_qship* qships = MOX::_qcombat_data;
        int16_t beam_weapon_id = qships[attacker_idx].beam_weapon_id;
        int16_t beam_weapon_count = TECHDATA::_hull_data[qships[attacker_idx].size].strat_beam;

        // Base miss threshold (hit chance = 100 - miss_threshold)
        int16_t miss_threshold = (qships[target_idx].beam_defense_bonus - qships[attacker_idx].base_ocv) + 40;
        
        if (miss_threshold > 95) {
            miss_threshold = 95;
        }

        if (beam_weapon_id > 0 && beam_weapon_count > 0) {
            int min_damage = (int)TECHDATA::_weapons[beam_weapon_id].min_damage;
            int max_damage = (int)TECHDATA::_weapons[beam_weapon_id].max_damage;

            if (qships[attacker_idx].beam_damage_modifier > 0) {
                int modifier = qships[attacker_idx].beam_damage_modifier + 100;
                min_damage = (min_damage * modifier) / 100;
                max_damage = (max_damage * modifier) / 100;
            }

            for (int i = 0; i < (int)beam_weapon_count; ++i) {
                int roll = game_random::Random_(100);
                
                // If miss_threshold <= roll, the attack hits
                if (miss_threshold <= (int16_t)roll) {
                    int damage_roll = game_random::Random_((max_damage - min_damage) + 1);
                    int final_damage = min_damage + damage_roll - 1;
                    
                    COMBAT::Apply_Damage_To_Qship_(target_idx, (int16_t)final_damage);

                    if (qships[target_idx].status == 1 /* STATUS_DESTROYED */) {
                        target_idx = COMBAT::QGet_Target_(attacker_idx);
                    }
                }

                if (target_idx == -1) {
                    break;
                }
            }
        }
    }

    void __cdecl Missile_Attack_(int16_t attacker_idx, int16_t target_idx) {
        s_qship* attacker = &MOX::_qcombat_data[attacker_idx];
        int16_t defense_bonus = MOX::_qcombat_data[target_idx].missile_defense_bonus;
        int16_t missile_weapon_id = attacker->missile_weapon_id;
        int16_t missile_count = INITSHIP::Strat_Missile_Weapon_Count_(attacker->size);

        if (missile_weapon_id > 0 && missile_count > 0) {
            int16_t weapon_damage = TECHDATA::_weapons[missile_weapon_id].max_damage;
            int16_t damage_modifier = attacker->missile_damage_modifier;

            if (damage_modifier > 0) {
                weapon_damage = (int16_t)(((damage_modifier + 100) * (int32_t)weapon_damage) / 100);
            }

            int8_t attacker_player_idx = attacker->player;

            // Apply sensor bonus to hit chance by reducing target defense
            if (attacker_player_idx < MAX_PLAYERS) {
                s_player* player = &MOX::_player[attacker_player_idx];
                if (player->tech_applications[TECH_APP_SENSORS] == TECH_RESEARCH_STATUS_RESEARCHED) {
                    defense_bonus -= 70;
                } else if (player->tech_applications[TECH_APP_NEUTRON_SCANNER] == TECH_RESEARCH_STATUS_RESEARCHED) {
                    defense_bonus -= 40;
                } else if (player->tech_applications[TECH_APP_TACHYON_SCANNER] == TECH_RESEARCH_STATUS_RESEARCHED) {
                    defense_bonus -= 20;
                }
            } else {
                defense_bonus -= 70;
            }

            do {
                int16_t roll = (int16_t)game_random::Random_(100);
                if (roll > 95) {
                    roll = 1000;
                }

                if (defense_bonus <= roll) {
                    COMBAT::Apply_Damage_To_Qship_(target_idx, weapon_damage);
                    s_qship* target = &MOX::_qcombat_data[target_idx];
                    
                    if (target->max_hits <= target->structural_damage) {
                        target->status = 1 /* Destroyed */;
                        target_idx = COMBAT::QGet_Target_(attacker_idx);
                    }
                }
                missile_count--;
            } while (missile_count > 0 && target_idx != -1);
        }
    }

    void __cdecl Strat_Special_Attack_(int16_t attacker_idx, int16_t target_idx) {
        s_qship* attacker = &MOX::_qcombat_data[attacker_idx];
        int16_t weapon_id = attacker->special_weapon_id;
        int16_t count = INITSHIP::Strat_Special_Weapon_Count_((uint32_t)attacker->size);

        if (weapon_id == 0x25 /* PULSAR */) {
            count = (count + 1) / 2;
        } else if (weapon_id == 0x26 /* GRAVITY_BEAM */) {
            count = (count + 3) / 4;
        }

        if (weapon_id > 0 && count > 0) {
            while (count > 0 && target_idx != -1) {
                int16_t min_dmg = TECHDATA::_weapons[weapon_id].min_damage;
                int16_t max_dmg = TECHDATA::_weapons[weapon_id].max_damage;

                int damage_roll = min_dmg + game_random::Random_(max_dmg - min_dmg + 1) - 1;
                
                COMBAT::Apply_Damage_To_Qship_(target_idx, (int16_t)damage_roll);

                if (MOX::_qcombat_data[target_idx].status == 1 /* Destroyed */) {
                    target_idx = COMBAT::QGet_Target_(attacker_idx);
                }

                count--;
            }
        }
    }

    void __cdecl Strat_Bomb_Attack_(int16_t attacker_idx) {
        s_qship* attacker = &MOX::_qcombat_data[attacker_idx];
        int16_t weapon_id = attacker->bomb_weapon_id;
        int16_t weapon_count = TECHDATA::_hull_data[attacker->size].strat_bomb;

        if (weapon_id > 0 && weapon_count > 0) {
            int16_t min_dmg = TECHDATA::_weapons[weapon_id].min_damage;
            int16_t max_dmg = TECHDATA::_weapons[weapon_id].max_damage;

            // The loop processes damage for each bomb weapon on the ship
            for (int16_t i = weapon_count; i > 0; --i) {
                int range = (int)max_dmg - (int)min_dmg + 1;
                int random_val = game_random::Random_(range);
                
                // Damage calculation includes an off-by-one adjustment as per original logic
                int applied_damage = (int)min_dmg + random_val - 1;

                // Index 0 in qcombat_data is always the target of the attack
                Apply_Damage_To_Qship_(0, (int16_t)applied_damage);

                // If target takes structural damage exceeding its max hits, mark it destroyed
                if (MOX::_qcombat_data[0].max_hits <= MOX::_qcombat_data[0].structural_damage) {
                    MOX::_qcombat_data[0].status = 1 /* Destroyed */;
                }
            }
        }
    }

    void __cdecl Message_Box_Titled_(char* summary, char* details) {
        TEXTBOX::Do_Text_Box_(summary, details, 0);
    }

    void __cdecl Resolve_Strat_Colony_Damage_() {
        int8_t msg_buffer[73];
        struct_::Clear_Structure_(msg_buffer, sizeof(msg_buffer));

        if ((uint8_t)MOX::_qcombat_data->status <= 1) {
            if ((MOX::_qcombat_data->structural_damage / 40 > 0) && (MOX::_cmbt_colony_id != -1)) {
                // Potential message construction, though buffer remains unused locally
                msg_buffer[8] = (int8_t)MOX::_combat_attacker_player;
                msg_buffer[9] = (int8_t)MOX::_cmbt_colony_id;

                // Check and destroy Missile Base
                if ((MOX::_qcombat_data->structural_damage / 40 > 0) && (MOX::_colony_defense_flags & 0x4)) {
                    MOX::_colony_defense_flags ^= 0x4;
                    ERICNET::Remove_Building_(MOX::_cmbt_colony_id, BUILDING_STELLAR_CONVERTER);
                    MOX::_qcombat_data->structural_damage -= 40;
                }

                // Check and destroy Star Base
                if ((MOX::_qcombat_data->structural_damage / 40 > 0) && (MOX::_colony_defense_flags & 0x1)) {
                    MOX::_colony_defense_flags ^= 0x1;
                    ERICNET::Remove_Building_(MOX::_cmbt_colony_id, BUILDING_MISSILE_BASE);
                    MOX::_qcombat_data->structural_damage -= 40;
                }

                // Check and destroy Battle Station
                if ((MOX::_qcombat_data->structural_damage / 40 > 0) && (MOX::_colony_defense_flags & 0x2)) {
                    MOX::_colony_defense_flags ^= 0x2;
                    ERICNET::Remove_Building_(MOX::_cmbt_colony_id, BUILDING_GROUND_BATTERIES);
                    MOX::_qcombat_data->missile_defense_bonus -= 10;
                    MOX::_qcombat_data->structural_damage -= 40;
                }

                // Check and destroy Star Fortress
                if ((MOX::_qcombat_data->structural_damage / 40 > 0) && (MOX::_colony_defense_flags & 0x8)) {
                    MOX::_colony_defense_flags ^= 0x8;
                    ERICNET::Remove_Building_(MOX::_cmbt_colony_id, BUILDING_FIGHTER_GARRISON);
                    MOX::_qcombat_data->structural_damage -= 40;
                }

                // Any remaining strategic damage less than 40 is kept
                MOX::_qcombat_data->structural_damage %= 40;

                // If all defenses are gone, mark the strategic combat entity as destroyed
                if ((MOX::_colony_defense_flags == 0) && (MOX::_qcombat_data->status == 0)) {
                    MOX::_qcombat_data->status = 1;
                }
            }
        }
    }

    void __cdecl Apply_Damage_To_Qship_(int16_t qship_idx, int16_t damage) {
        s_qship* ship = &MOX::_qcombat_data[qship_idx];
        int16_t applied_damage;

        if (ship->shield_id == 7) {
            // Special case for Antaran Damper (shield_id 7) which reduces incoming damage to 1/4.
            applied_damage = damage / 4;
        } else {
            // Normal shields block a flat amount of damage.
            applied_damage = damage - TECHDATA::_shields[ship->shield_id].shield_class;
        }

        if (applied_damage < 0) {
            applied_damage = 0;
        }

        ship->structural_damage += applied_damage;

        // Cap structural damage at 30000.
        if (ship->structural_damage > 30000) {
            ship->structural_damage = 30000;
        }

        // If structural damage meets or exceeds max hits, the ship is destroyed.
        if (ship->structural_damage >= ship->max_hits) {
            ship->status = 1 /* Destroyed */;
        }
    }

    int16_t __cdecl Combat_Satelite_Check_(uint8_t type) {
        if (type == BUILDING_BATTLESTATION || type == BUILDING_STAR_BASE || type == BUILDING_STAR_FORTRESS) {
            return 1;
        }
        return 0;
    }

    /**
     * Calculates total hits for a colony during combat based on population, 
     * military units, and non-satellite buildings.
     */
    int16_t __cdecl Get_Colony_Hits_() {
        s_colony* colony = &MOX::_colony[MOX::_cmbt_colony_id];

        // Start hits count with population and military units (tanks/infantry)
        // Note: population is treated as unsigned byte here.
        int16_t hits = (uint8_t)colony->n_pops + colony->military[0] + colony->military[1];

        // Add 1 hit for every building that is not a combat satellite
        for (uint8_t i = 0; i < BUILDING_COUNT; ++i) {
            if (colony->buildings[i] != 0) {
                if (Combat_Satelite_Check_(i) == 0) {
                    hits++;
                }
            }
        }

        // Each "hit" unit represents 40 actual hit points
        return hits * 40;
    }

    void __cdecl Get_Non_Player_Name_(int16_t player_idx, char* out_name, int32_t out_name_size) {
        if (out_name_size <= 0) {
            return;
        }

        const char* name = "";

        switch (player_idx) {
            case 8:
                name = s_Antaran_00557e54;
                break;
            case 9:
                name = s_Guardian_00557e48;
                break;
            case 10:
                name = s_Amoeba_00557e40;
                break;
            case 11:
                name = s_Crystal_00557e24;
                break;
            case 12:
                name = s_Dragon_00557e38;
                break;
            case 13:
                name = s_Eel_00557e2c;
                break;
            case 14:
                name = s_Hydra_00557e30;
                break;
            default:
                break;
        }

        strlcpy(out_name, name, (size_t)out_name_size);
    }

    void __cdecl Build_End_Of_Combat_Message_(char* result_msg, int32_t result_msg_size, char* stats_msg, int32_t stats_msg_size, int16_t star_idx, int16_t colony_damage_flags, int16_t outcome_type) {
        if (result_msg_size <= 0 || stats_msg_size <= 0) {
            return;
        }

        const size_t result_capacity = (size_t)result_msg_size;
        const size_t stats_capacity = (size_t)stats_msg_size;
        int16_t attacker_total_ships[SHIP_SIZE_COUNT];
        int16_t attacker_lost_ships[SHIP_SIZE_COUNT];
        int16_t defender_total_ships[SHIP_SIZE_COUNT];
        int16_t defender_lost_ships[SHIP_SIZE_COUNT];
        int16_t special_hull_idx = -1;
        int16_t special_alive = 1;
        char attacker_race_name[20];
        char defender_race_name[20];
        char itoa_buffer[12];
        char result_prefix[80];
        char msg_buffer[80];

        for (int i = 0; i < SHIP_SIZE_COUNT; ++i) {
            attacker_total_ships[i] = 0;
            attacker_lost_ships[i] = 0;
            defender_total_ships[i] = 0;
            defender_lost_ships[i] = 0;
        }

        if (MOX::_combat_total_ship_count > 1) {
            for (int i = 1; i < MOX::_combat_total_ship_count; ++i) {
                s_qship* ship = &MOX::_qcombat_data[i];
                uint8_t size = ship->size;
                if (size < SHIP_SIZE_COUNT) {
                    if (ship->player == MOX::_combat_attacker_player) {
                        if (ship->status == 0) {
                            attacker_total_ships[size]++;
                        } else {
                            attacker_lost_ships[size]++;
                        }
                    } else {
                        if (ship->status == 0) {
                            defender_total_ships[size]++;
                        } else {
                            defender_lost_ships[size]++;
                        }
                    }
                } else {
                    special_hull_idx = (int16_t)size;
                    special_alive = (ship->status == 0);
                }
            }
        }

        for (int i = 0; i < SHIP_SIZE_COUNT; ++i) {
            attacker_total_ships[i] += attacker_lost_ships[i];
            defender_total_ships[i] += defender_lost_ships[i];
        }

        KEN::Ken_Get_Text_Message_(98 /* 0x62 */, result_prefix, 80);
        strlcpy(result_msg, result_prefix, result_capacity);

        const char* location_name;
        if (star_idx == MOX::_NUM_STARS) {
            location_name = string::s_Antares;
        } else if (MOX::_cmbt_colony_id == -1) {
            location_name = MOX::_star[star_idx].name;
        } else {
            location_name = HACCESS::Get_Planet_Name_(MOX::_colony[MOX::_cmbt_colony_id].planet);
        }
        strlcat(result_msg, location_name, result_capacity);

        if (MOX::_combat_attacker_player < MAX_PLAYERS) {
            strlcpy(attacker_race_name, MOX::_player[MOX::_combat_attacker_player].race_name, sizeof(attacker_race_name));
        } else {
            Get_Non_Player_Name_(MOX::_combat_attacker_player, attacker_race_name, sizeof(attacker_race_name));
        }

        if (MOX::_combat_defender_player < MAX_PLAYERS) {
            strlcpy(defender_race_name, MOX::_player[MOX::_combat_defender_player].race_name, sizeof(defender_race_name));
        } else {
            Get_Non_Player_Name_(MOX::_combat_defender_player, defender_race_name, sizeof(defender_race_name));
        }

        strlcpy(stats_msg, string::s_X35, stats_capacity);
        strlcat(stats_msg, attacker_race_name, stats_capacity);
        strlcat(stats_msg, string::s_X255, stats_capacity);
        strlcat(stats_msg, defender_race_name, stats_capacity);
        strlcat(stats_msg, string::s_Newline, stats_capacity);

        KEN::Ken_Get_Text_Message_(99 /* 0x63 */, msg_buffer, 80);
        strlcat(stats_msg, msg_buffer, stats_capacity);
        strlcat(stats_msg, string::s_Xplus25, stats_capacity);

        KEN::Ken_Get_Text_Message_(100 /* 0x64 */, msg_buffer, 80);
        strlcat(stats_msg, msg_buffer, stats_capacity);
        strlcat(stats_msg, string::s_X227, stats_capacity);

        KEN::Ken_Get_Text_Message_(99 /* 0x63 */, msg_buffer, 80);
        strlcat(stats_msg, msg_buffer, stats_capacity);
        strlcat(stats_msg, string::s_Xplus21, stats_capacity);

        KEN::Ken_Get_Text_Message_(100 /* 0x64 */, msg_buffer, 80);
        strlcat(stats_msg, msg_buffer, stats_capacity);
        strlcat(stats_msg, string::s_Newline, stats_capacity);

        for (int i = 0; i < SHIP_SIZE_COUNT; ++i) {
            if (attacker_total_ships[i] + defender_total_ships[i] > 0) {
                strlcat(stats_msg, string::s_X18, stats_capacity);
                if (attacker_total_ships[i] == 0) {
                    strlcat(stats_msg, string::s_Dash, stats_capacity);
                } else {
                    snprintf(itoa_buffer, sizeof(itoa_buffer), "%i", attacker_total_ships[i]);
                    strlcat(stats_msg, itoa_buffer, stats_capacity);
                }

                strlcat(stats_msg, string::s_X80, stats_capacity);
                if (attacker_lost_ships[i] == 0) {
                    strlcat(stats_msg, string::s_Dash, stats_capacity);
                } else {
                    snprintf(itoa_buffer, sizeof(itoa_buffer), "%i", attacker_lost_ships[i]);
                    strlcat(stats_msg, itoa_buffer, stats_capacity);
                }

                strlcat(stats_msg, string::s_X120, stats_capacity);
                strlcat(stats_msg, TECHDATA::_hull_data[i].size_name, stats_capacity);

                strlcat(stats_msg, string::s_X250, stats_capacity);
                if (defender_total_ships[i] == 0) {
                    strlcat(stats_msg, string::s_Dash, stats_capacity);
                } else {
                    snprintf(itoa_buffer, sizeof(itoa_buffer), "%i", defender_total_ships[i]);
                    strlcat(stats_msg, itoa_buffer, stats_capacity);
                }

                strlcat(stats_msg, string::s_X312, stats_capacity);
                if (defender_lost_ships[i] == 0) {
                    strlcat(stats_msg, string::s_Dash, stats_capacity);
                } else {
                    snprintf(itoa_buffer, sizeof(itoa_buffer), "%i", defender_lost_ships[i]);
                    strlcat(stats_msg, itoa_buffer, stats_capacity);
                }
                strlcat(stats_msg, string::s_Newline, stats_capacity);
            }
        }

        if (special_hull_idx != -1) {
            strlcat(stats_msg, string::s_X120, stats_capacity);
            strlcat(stats_msg, TECHDATA::_hull_data[special_hull_idx].name, stats_capacity);
            strlcat(stats_msg, string::s_X250, stats_capacity);
            strlcat(stats_msg, string::s_1, stats_capacity);
            strlcat(stats_msg, string::s_X312, stats_capacity);
            if (special_alive) {
                strlcat(stats_msg, string::s_Dash, stats_capacity);
            } else {
                strlcat(stats_msg, string::s_1, stats_capacity);
            }
            strlcat(stats_msg, string::s_Newline, stats_capacity);
        }

        if (colony_damage_flags & 0x1) {
            strlcat(stats_msg, string::s_X120, stats_capacity);
            strlcat(stats_msg, TECHDATA::_buildings[BUILDING_MISSILE_BASE].name, stats_capacity);
            strlcat(stats_msg, string::s_X250, stats_capacity);
            strlcat(stats_msg, string::s_1, stats_capacity);
            strlcat(stats_msg, string::s_X312, stats_capacity);
            if (MOX::_colony_defense_flags & 0x1) {
                strlcat(stats_msg, string::s_Dash, stats_capacity);
            } else {
                strlcat(stats_msg, string::s_1, stats_capacity);
            }
            strlcat(stats_msg, string::s_Newline, stats_capacity);
        }

        if (colony_damage_flags & 0x2) {
            strlcat(stats_msg, string::s_X120, stats_capacity);
            strlcat(stats_msg, TECHDATA::_buildings[BUILDING_GROUND_BATTERIES].name, stats_capacity);
            strlcat(stats_msg, string::s_X250, stats_capacity);
            strlcat(stats_msg, string::s_1, stats_capacity);
            strlcat(stats_msg, string::s_X312, stats_capacity);
            if (MOX::_colony_defense_flags & 0x2) {
                strlcat(stats_msg, string::s_Dash, stats_capacity);
            } else {
                strlcat(stats_msg, string::s_1, stats_capacity);
            }
            strlcat(stats_msg, string::s_Newline, stats_capacity);
        }

        if (colony_damage_flags & 0x8) {
            strlcat(stats_msg, string::s_X120, stats_capacity);
            strlcat(stats_msg, TECHDATA::_buildings[BUILDING_FIGHTER_GARRISON].name, stats_capacity);
            strlcat(stats_msg, string::s_X250, stats_capacity);
            strlcat(stats_msg, string::s_1, stats_capacity);
            strlcat(stats_msg, string::s_X312, stats_capacity);
            if (MOX::_colony_defense_flags & 0x8) {
                strlcat(stats_msg, string::s_Dash, stats_capacity);
            } else {
                strlcat(stats_msg, string::s_1, stats_capacity);
            }
            strlcat(stats_msg, string::s_Newline, stats_capacity);
        }

        if (colony_damage_flags & 0x4) {
            strlcat(stats_msg, string::s_X120, stats_capacity);
            strlcat(stats_msg, TECHDATA::_buildings[BUILDING_STELLAR_CONVERTER].name, stats_capacity);
            strlcat(stats_msg, string::s_X250, stats_capacity);
            strlcat(stats_msg, string::s_1, stats_capacity);
            strlcat(stats_msg, string::s_X312, stats_capacity);
            if (MOX::_colony_defense_flags & 0x4) {
                strlcat(stats_msg, string::s_Dash, stats_capacity);
            } else {
                strlcat(stats_msg, string::s_1, stats_capacity);
            }
            strlcat(stats_msg, string::s_Newline, stats_capacity);
        }

        strlcat(stats_msg, string::s_Newline2, stats_capacity);
        strlcat(stats_msg, string::s_2, stats_capacity);
        if (outcome_type > 0) {
            KEN::Ken_Get_Text_Message_(101 /* 0x65 */, msg_buffer, 80);
        } else if (outcome_type < 0) {
            KEN::Ken_Get_Text_Message_(102 /* 0x66 */, msg_buffer, 80);
        } else {
            KEN::Ken_Get_Text_Message_(103 /* 0x67 */, msg_buffer, 80);
        }
        strlcat(stats_msg, msg_buffer, stats_capacity);
    }

    /**
     * @brief Comparison function for qsort to sort ship indices by ship size (descending).
     * 
     * This function compares the design size of two ships indexed by p1 and p2.
     * Logic: (size_of_ship2 - size_of_ship1).
     * 
     * @param a Pointer to the first ship index.
     * @param b Pointer to the second ship index.
     * @return 1 if ship2 is larger, -1 if ship1 is larger, 0 if equal.
     */
    int __cdecl qsort_ship_array_by_size_(const void* a, const void* b) {
        const int16_t* p1 = (const int16_t*)a;
        const int16_t* p2 = (const int16_t*)b;
        int8_t size2 = MOX::_ship[*p2].d.size;
        int8_t size1 = MOX::_ship[*p1].d.size;

        if (size2 == size1) {
            return 0;
        }

        if (size2 > size1) {
            return 1;
        }

        return -1;
    }

    int16_t __cdecl Strategic_Bombardment_(int16_t* attacker_ship_indices, int16_t attacker_count, int16_t colony_id) {
        MOX::_cmbt_colony_id = colony_id;
        MOX::_qcombat_data = (s_qship*)allocate::Allocate_First_Block_(MOX::_screen_seg, kQCombatDataSize);
        MOX::_combat_attacker_count = attacker_count;
        MOX::_combat_defender_count = 0;

        MOX::_combat_attacker_player = (int16_t)MOX::_ship[*attacker_ship_indices].owner;
        MOX::_combat_defender_player = (int16_t)MOX::_colony[colony_id].owner;

        MOX::_combat_total_ship_count = 1;
        MOX::_colony_defense_flags = 0;

        COMBAT::Qload_Ships_(attacker_ship_indices, attacker_count, 0, 0, 0, 0);
        COMBAT::Qload_Ships_(attacker_ship_indices, 0, 0, 0, 0, 0);

        int16_t rounds = 0;
        do {
            int16_t ship_idx = 1;
            if (MOX::_combat_total_ship_count > 1) {
                do {
                    COMBAT::Missile_Attack_(ship_idx, 0);
                    COMBAT::Beam_Attack_(ship_idx, 0);
                    COMBAT::Strat_Bomb_Attack_(ship_idx);

                    if (MOX::_qcombat_data->structural_damage > 30000) {
                        rounds = 6;
                        break;
                    }
                    ship_idx++;
                } while (ship_idx < MOX::_combat_total_ship_count);
            }
            rounds++;
        } while (rounds < 3);

        return MOX::_qcombat_data->structural_damage / 40 /* 0x28 */;
    }

    void __cdecl Bombardment_(s_combat_bomb_result* result) {
        struct_::Clear_Structure_(&MOX::_combat_bomb_result, sizeof(s_combat_bomb_result));

        int16_t num_ships = 0;
        uint8_t colony_idx = result->colony_idx;
        int16_t ships[MAX_SHIPS];

        for (int16_t i = 0; i < MAX_SHIPS; ++i) {
            int16_t planet_idx = MOX::_colony[colony_idx].planet;

            if (MOX::_planet[planet_idx].star_index == MOX::_ship[i].location &&
                MOX::_ship[i].d.ship_type == SHIP_TYPE_COMBAT &&
                MOX::_ship[i].status == 0 &&
                MOX::_ship[i].owner == result->attacker_idx) {

                ships[num_ships++] = i;
            }
        }

        if (MOX::_settings.strategic_combat_flag == 1) {
            int16_t remaining = Strategic_Bombardment_(ships, num_ships, colony_idx);

            result->bomb_hits_remaining = remaining;
            result->is_planetary_hit = 1;
            result->bio_hits_remaining = 0;
            result->is_bio_weapon = 0;
        } else {
            Tactical_Bombardment_(ships, num_ships, result);
        }
    }

    void __cdecl Tactical_Bombardment_(int16_t* ship_array, int16_t ship_count, s_combat_bomb_result* bomb_result)
    {
        MOX::_cmbt_colony_id = (int16_t)bomb_result->colony_idx;
        MOX::_apply_damage_to_planet = 0;
        MOX::_combat_total_ship_count = 1;

        allocate::Push_Block_(MOX::_screen_seg);

        MOX::_combat_data = (s_combat_data*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x100C2);
        struct_::Clear_Structure_(MOX::_combat_data, 0x100C2);

        MOX::_missile = (s_missile*)allocate::Allocate_Next_Block_(MOX::_screen_seg, 0x1E78);
        struct_::Clear_Structure_(MOX::_missile, 0x1E78);

        MOX::_combat_attacker_player = MOX::_ship[ship_array[0]].owner;
        MOX::_combat_defender_player = MOX::_colony[MOX::_cmbt_colony_id].owner;

        qsort(ship_array, (size_t)ship_count, sizeof(*ship_array), qsort_ship_array_by_size_);

        if (ship_count > 0) {
            for (int16_t i = 0; i < ship_count; ++i) {
                if (MOX::_ship[ship_array[i]].d.ship_type == SHIP_TYPE_COMBAT) {
                    COMBINIT::Load_Combat_Ship_(ship_array[i], MOX::_combat_total_ship_count);
                    MOX::_combat_data[MOX::_combat_total_ship_count].x = 0;
                    MOX::_combat_data[MOX::_combat_total_ship_count].y = 0;
                    MOX::_combat_total_ship_count++;
                }
            }
        }

        COMBINIT::Load_Tactical_Colony_(MOX::_cmbt_colony_id, 0);
        CMBTFIR2::Set_Combat_Ordnance_Values_();

        MOX::_combat_data[0].x = 0;
        MOX::_combat_data[0].y = 0;

        if (MOX::_combat_total_ship_count > 0) {
            for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                MOX::_combat_data[i].tac_val_attack = CMBTFIR2::Offensive_Combat_Bonus_(i);
                MOX::_combat_data[i].tac_val_defense = CMBTFIR2::Defensive_Combat_Bonus_(i, 0);
                MOX::_combat_data[i].tac_val_evade = CMBTFIR2::Defensive_Combat_Bonus_(i, 1);
                COMBINIT::Init_Special_Devices_(i);
            }
        }

        int16_t turns = 10;
        do {
            if (MOX::_combat_total_ship_count > 0) {
                for (int16_t i = 0; i < MOX::_combat_total_ship_count; ++i) {
                    for (int16_t j = 0; j < 8; ++j) {
                        MOX::_combat_data[i].weapons[j].shots_left = MOX::_combat_data[i].weapons[j].count;
                        MOX::_combat_data[i].weapons[j].active = 1;

                        if (MOX::_combat_data[i].weapons[j].weapon_id > 0) {
                            if (TECHDATA::_weapons[MOX::_combat_data[i].weapons[j].weapon_id].type != 1) {
                                if (MOX::_combat_data[i].weapons[j].ammo > 0) {
                                    MOX::_combat_data[i].weapons[j].ammo--;
                                }
                            }
                        }
                    }
                }
            }

            if (MOX::_combat_total_ship_count > 1) {
                for (int16_t i = 1; i < MOX::_combat_total_ship_count; ++i) {
                    CMBTFIRE::Fire_Ship_(i, 0, 0, 0, 0, 0, 0);
                }
            }

            turns--;
        } while (turns != 0);

        bomb_result->bomb_hits_remaining = MOX::_combat_data[0].structure_max / 100;
        bomb_result->is_planetary_hit = MOX::_combat_bomb_result.is_planetary_hit;
        bomb_result->is_bio_weapon = MOX::_combat_bomb_result.is_bio_weapon;
        bomb_result->bio_hits_remaining = MOX::_combat_bomb_result.bio_hits_remaining;

        allocate::Pop_Block_(MOX::_screen_seg);
    }
}