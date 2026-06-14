#include "pch.h"
#include <cstring>

namespace SHIPSTAK {
    void __cdecl Delete_Ship_Node_(int16_t stack_idx, int16_t prev_node_idx, int16_t node_idx) {
        if (prev_node_idx == -1) {
            MOX::_ship_stack_start[stack_idx] = MOX::_ship_node[node_idx].next_node;
        } else {
            MOX::_ship_node[prev_node_idx].next_node = MOX::_ship_node[node_idx].next_node;
        }
    }

    int16_t __cdecl Ship_Has_Special_(int16_t ship_idx, int16_t special_idx) {
        int16_t has_special = 0;

        if (struct_::Test_Bit_Field_(MOX::_ship[ship_idx].d.special_device_flags, special_idx)) {
            if (!struct_::Test_Bit_Field_(MOX::_ship[ship_idx].special_device_damage_flags, special_idx)) {
                has_special = 1;
            }
        }

        return has_special;
    }

    int16_t __cdecl Ship_Stack_Star_Id_(int16_t node_idx) {
        return MOX::_ship[MOX::_ship_node[node_idx].ship_idx].location;
    }

    bool __cdecl Ship_Has_Stealth_Device_(int16_t ship_idx) {
        if (Ship_Has_Special_(ship_idx, SPECIAL_STEALTH_FIELD) != 0) {
            return true;
        }

        if (Ship_Has_Special_(ship_idx, SPECIAL_CLOAKING_DEVICE) != 0) {
            return true;
        }

        if (Ship_Has_Special_(ship_idx, SPECIAL_PHASING_CLOAK) != 0) {
            return true;
        }

        return false;
    }

    void __cdecl Find_Ship_Stacks_() {
        uint8_t* ship_skip_flags = (uint8_t*)allocate::Allocate_First_Block_(MOX::_global_data_seg, MAX_SHIPS);

        if (MOX::_NUM_SHIPS > 0) {
            memset(ship_skip_flags, 0, MOX::_NUM_SHIPS);
        }

        MOX::_ship_stack_count = 0;
        MOX::_next_free_node = 0;

        for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
            if (MOX::_ship[i].status >= 3) {
                ship_skip_flags[i] = 1;
            }
        }

        for (int16_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
            if (ship_skip_flags[i] == 1) {
                continue;
            }

            bool found_stack = false;
            for (int16_t j = 0; j < (int16_t)MOX::_ship_stack_count; ++j) {
                int16_t first_node_idx = MOX::_ship_stack_start[j];
                int16_t first_ship_idx = MOX::_ship_node[first_node_idx].ship_idx;

                if (MOX::_ship[first_ship_idx].location == MOX::_ship[i].location &&
                    MOX::_ship[first_ship_idx].x == MOX::_ship[i].x &&
                    MOX::_ship[first_ship_idx].y == MOX::_ship[i].y &&
                    MOX::_ship[first_ship_idx].owner == MOX::_ship[i].owner) {

                    int16_t node_idx = first_node_idx;
                    int16_t last_node = node_idx;

                    while (node_idx != -1) {
                        last_node = node_idx;
                        node_idx = MOX::_ship_node[node_idx].next_node;
                    }

                    int16_t next_free = MOX::_next_free_node;
                    MOX::_ship_node[last_node].next_node = next_free;
                    MOX::_ship_node[next_free].ship_idx = i;
                    MOX::_ship_node[next_free].next_node = -1;
                    MOX::_ship_node[next_free].selected = 0;
                    MOX::_next_free_node++;

                    found_stack = true;
                    break;
                }
            }

            if (!found_stack) {
                int16_t next_free = MOX::_next_free_node;
                MOX::_ship_stack_start[MOX::_ship_stack_count] = next_free;
                MOX::_ship_node[next_free].ship_idx = i;
                MOX::_ship_node[next_free].next_node = -1;
                MOX::_ship_node[next_free].selected = 0;
                MOX::_next_free_node++;
                MOX::_ship_stack_count++;
            }
        }

        if (MOX::_player[MOX::_PLAYER_NUM].traits[TRAIT_OMNISCIENCE] == 0) {
            Remove_Non_Detected_Ships_();
        }

        for (int16_t j = 0; j < (int16_t)MOX::_ship_stack_count; ++j) {
            Sort_Ships_In_Stack_(MOX::_ship_stack_start[j]);
        }
    }

    void __cdecl Remove_Non_Detected_Ships_() {
        const int16_t player_num = (int16_t)MOX::_PLAYER_NUM;
        int16_t scan_range = 2;

        if ((int8_t)MOX::_player[player_num].tech_applications[TECH_APP_TACHYON_SCANNER] == 3) {
            scan_range = 4;
        }
        if ((int8_t)MOX::_player[player_num].tech_applications[TECH_APP_NEUTRON_SCANNER] == 3) {
            scan_range = 6;
        }
        if ((int8_t)MOX::_player[player_num].tech_applications[TECH_APP_SENSORS] == 3) {
            scan_range = 9;
        }

        for (int16_t stack_idx = 0; stack_idx < (int16_t)MOX::_ship_stack_count; ++stack_idx) {
            int16_t* stack_head_ptr = &MOX::_ship_stack_start[stack_idx];
            int16_t stack_head_node = *stack_head_ptr;

            const s_ship_data& stack_head_ship = MOX::_ship[MOX::_ship_node[stack_head_node].ship_idx];
            if ((int16_t)(int8_t)stack_head_ship.owner != player_num) {
                int16_t best_detection = -300;

                for (int16_t colony_idx = 0; colony_idx < (int16_t)MOX::_NUM_COLONIES; ++colony_idx) {
                    const s_colony& colony = MOX::_colony[colony_idx];
                    if ((int16_t)(int8_t)colony.owner == player_num) {
                        const s_star_data& colony_star =
                            MOX::_star[MOX::_planet[colony.planet].star_index];

                        int16_t range_steps = (int16_t)((special::Range_(
                                                             stack_head_ship.x,
                                                             stack_head_ship.y,
                                                             colony_star.x,
                                                             colony_star.y) +
                                                         29) /
                                                        30);

                        if (range_steps == 0) {
                            best_detection = 1000;
                        } else {
                            int16_t detection = scan_range;
                            if (colony.buildings[BUILDING_STAR_BASE] != 0) {
                                detection += 2;
                            }
                            if (colony.buildings[BUILDING_BATTLESTATION] != 0) {
                                detection += 4;
                            }
                            if (colony.buildings[BUILDING_STAR_FORTRESS] != 0) {
                                detection += 6;
                            }
                            detection -= range_steps;
                            if (best_detection < detection) {
                                best_detection = detection;
                            }
                        }
                    }
                }

                for (int16_t other_ship_idx = 0; other_ship_idx < (int16_t)MOX::_NUM_SHIPS; ++other_ship_idx) {
                    const s_ship_data& other_ship = MOX::_ship[other_ship_idx];
                    if ((int16_t)(int8_t)other_ship.owner == player_num && (int8_t)other_ship.status < 3) {
                        int16_t range_steps = (int16_t)((special::Range_(
                                                             stack_head_ship.x,
                                                             stack_head_ship.y,
                                                             other_ship.x,
                                                             other_ship.y) +
                                                         29) /
                                                        30);

                        if (range_steps == 0) {
                            best_detection = 1000;
                        } else {
                            int16_t detection = scan_range - range_steps;
                            if (SHIPSTAK::Ship_Has_Special_(other_ship_idx, SPECIAL_BATTLE_SCANNER) != 0) {
                                detection += 2;
                            }
                            if (best_detection < detection) {
                                best_detection = detection;
                            }
                        }
                    }
                }

                int16_t prev_node = -1;
                for (int16_t node_idx = *stack_head_ptr; node_idx != -1; ) {
                    int16_t current_node = node_idx;
                    int16_t next_node = MOX::_ship_node[current_node].next_node;
                    int16_t node_ship_idx = MOX::_ship_node[current_node].ship_idx;
                    s_ship_data& ship = MOX::_ship[node_ship_idx];
                    int16_t owner = (int16_t)(int8_t)ship.owner;

                    if (owner <= MAX_PLAYERS || ship.d.speed != 0) {
                        int16_t detection = 0;

                        if (owner == MAX_PLAYERS) {
                            detection = -5;
                        } else if (owner < MAX_PLAYERS && MOX::_player[owner].traits[TRAIT_STEALTHY_SHIPS] == 1) {
                            detection = 500;
                        } else {
                            if ((int8_t)ship.status == 1) {
                                detection = -(int16_t)ship.d.size;
                            }
                        }

                        if (SHIPSTAK::Ship_Has_Special_(node_ship_idx, SPECIAL_STEALTH_FIELD) != 0) {
                            detection += 500;
                        }
                        if (SHIPSTAK::Ship_Has_Special_(node_ship_idx, SPECIAL_CLOAKING_DEVICE) != 0) {
                            detection += 500;
                        }
                        if (SHIPSTAK::Ship_Has_Special_(node_ship_idx, SPECIAL_PHASING_CLOAK) != 0) {
                            detection += 500;
                        }

                        if (best_detection < detection) {
                            SHIPSTAK::Delete_Ship_Node_(stack_idx, prev_node, current_node);
                        } else {
                            prev_node = current_node;
                        }
                    } else {
                        if (!HAROLD::Player_Has_Visited_(player_num, ship.location)) {
                            SHIPSTAK::Delete_Ship_Node_(stack_idx, prev_node, current_node);
                        }
                    }

                    node_idx = next_node;
                }

                if (*stack_head_ptr == -1) {
                    SHIPSTAK::Delete_Ship_Stack_(stack_idx);
                    --stack_idx;
                }
            }
        }
    }

    void __cdecl Sort_Ships_In_Stack_(int16_t start_node) {
        int16_t ship_count;
        int16_t ship_entries[MAX_SHIPS * 2];
        int16_t current_node = start_node;

        Build_Ship_Type_Array_(start_node, ship_entries, &ship_count);

        if (ship_count > 1) {
            qsort(ship_entries, (size_t)ship_count, sizeof(ship_entries[0]) * 2, qsort_ships_by_type_);

            if (ship_count > 0) {
                int16_t entry_idx = 0;
                while (entry_idx < ship_count) {
                    MOX::_ship_node[current_node].ship_idx = ship_entries[entry_idx * 2];
                    current_node = MOX::_ship_node[current_node].next_node;
                    ++entry_idx;
                }
            }
        }
    }

    void __cdecl Build_Ship_Type_Array_(int16_t node_idx, int16_t* stack_array, int16_t* count_ptr) {
        *count_ptr = 0;
        int16_t current_node_idx = node_idx;

        do {
            const auto& node = MOX::_ship_node[current_node_idx];
            int16_t ship_idx = node.ship_idx;

            if (ship_idx >= 0) {
                stack_array[*count_ptr * 2] = ship_idx;
                const auto& ship = MOX::_ship[ship_idx];

                switch (ship.d.ship_type) {
                case SHIP_TYPE_COMBAT:
                    stack_array[*count_ptr * 2 + 1] = (int16_t)ship.d.size * 50 + (int16_t)ship.d.picture_num;
                    break;
                case SHIP_TYPE_COLONY:
                    stack_array[*count_ptr * 2 + 1] = -1;
                    break;
                case SHIP_TYPE_TRANSPORT:
                    stack_array[*count_ptr * 2 + 1] = -2;
                    break;
                case SHIP_TYPE_OUTPOST:
                    stack_array[*count_ptr * 2 + 1] = -3;
                    break;
                default:
                    // case 3 and values > 4 skip setting the second array element
                    break;
                }
                (*count_ptr)++;
            }

            current_node_idx = node.next_node;
        } while (current_node_idx > 0 && current_node_idx < MAX_SHIPS);
    }

    int __cdecl qsort_ships_by_type_(const void* left_entry, const void* right_entry) {
        const int16_t* left_words = (const int16_t*)left_entry;
        const int16_t* right_words = (const int16_t*)right_entry;

        return (int)right_words[1] - (int)left_words[1];
    }

    void __cdecl Delete_Ship_Stack_(int16_t stack_idx) {
        strings::Delete_Structure_(
            (int32_t)stack_idx,
            MOX::_ship_stack_start,
            (int32_t)sizeof(MOX::_ship_stack_start[0]),
            (int32_t)MOX::_ship_stack_count
        );

        MOX::_ship_stack_count--;
    }
}