#include "pch.h"

namespace COMBAT {
    int16_t __cdecl Strategic_Combat_(int16_t* plr1_ships, int16_t plr1_ship_count, int16_t* plr2_ships, int16_t plr2_ship_count, int16_t star_idx, int16_t colony_idx, char* summary, char* details) {
        void* auto_fn;
        int16_t leader_combat_bonuses[20];

        for (int i = 0; i < 20; i++) {
            leader_combat_bonuses[i] = 0;
        }

        MOX2::Get_Auto_Function_(&auto_fn);
        fields::Assign_Auto_Function_(reinterpret_cast<void(*)()>(MAINSCR::Draw_Mini_Main_Screen_), 1);

        MOX::_cmbt_colony_id = colony_idx;
        MOX::_combat_turn = 0;
        MOX::_qcombat_data = reinterpret_cast<s_qship*>(allocate::Allocate_First_Block_(MOX::_screen_seg, 0x4075));
        struct_::Clear_Structure_(MOX::_qcombat_data, 0x4095);

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

        if (MOX::_combat_attacker_player == 9 && MOX::_combat_defender_player > 7) {
            MOX::_combat_attacker_player = 9;
            for (int i = 0; i < plr2_ship_count; i++) {
                SHIPS::Kill_Ship_(plr2_ships[i]);
            }
            return 1;
        }

        if (MOX::_combat_defender_player == 9 && MOX::_combat_attacker_player > 7) {
            for (int i = 0; i < plr1_ship_count; i++) {
                SHIPS::Kill_Ship_(plr1_ships[i]);
            }
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
        int16_t survived_officer_ids[8];
        bool skip_combat = false;

        if (MOX::_combat_defender_player < 8 && star_idx < MOX::_NUM_STARS) {
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
            _no_op3();

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

                _no_op3();
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
                if (MOX::_qcombat_data[i].size == 6) {
                    building_id = 0x28; // Star Base
                } else if (MOX::_qcombat_data[i].size == 7) {
                    building_id = 8; // Battlestation
                } else if (MOX::_qcombat_data[i].size == 8) {
                    building_id = 0x29; // Star Fortress
                }
                
                if (building_id != 0) {
                    ERICNET::Remove_Building_(MOX::_cmbt_colony_id, building_id);
                }
            }

            int16_t ship_design_idx = MOX::_qcombat_data[i].ship_design_idx;
            if (ship_design_idx != -1) {
                int8_t player = MOX::_qcombat_data[i].player;
                if (leader_combat_bonuses[player] <= 0 && 
                   (player >= 8 || (MOX::_player[player].tech_applications[4] != 3 && MOX::_player[player].traits[17] == 0))) {
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
                        if (MOX::_player[MOX::_qcombat_data[i].player].tech_applications[179] == 3) {
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

        if (MOX::_combat_attacker_player < 8 && MOX::_combat_defender_player < 8) {
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
                int8_t player_idx = MOX::_leaders[officer_idx].player_index;

                for (int16_t i = 1; i < MOX::_combat_total_ship_count; i++) {
                    if (MOX::_qcombat_data[i].status == 0 && MOX::_qcombat_data[i].ship_design_idx != -1) {
                        if (MOX::_qcombat_data[i].player == player_idx) {
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

        if (result >= 1 && MOX::_combat_attacker_player < 8 && MOX::_combat_defender_player >= 10) {
            if (MOX::_ship[plr2_ships[0]].d.ftl_type > 0) {
                NETMOX::Russ_Event_Monster_Update_(MOX::_combat_defender_player, -1, MOX::_combat_attacker_player);
                result = 1;
            }
        }

        if (result <= -1 && MOX::_combat_defender_player < 8 && MOX::_combat_attacker_player >= 10) {
            if (MOX::_ship[plr1_ships[0]].d.ftl_type > 0) {
                NETMOX::Russ_Event_Monster_Update_(MOX::_combat_attacker_player, -1, MOX::_combat_defender_player);
                result = -1;
            }
        }

        Build_End_Of_Combat_Message_(summary, details, star_idx, colony_defense_flags, result);

        if (MOX::_cmbt_colony_id != -1) {
            if (MOX::_colony[MOX::_cmbt_colony_id].n_pops == 0) {
                int16_t has_buildings = 0;
                for (int i = 0; i < 49; i++) {
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
}