#include "pch.h"

namespace HACCESS {
    int16_t __cdecl Ship_Owner_(int16_t ship_idx) {
        int16_t error_code = 0;

        if (ship_idx < 0) {
            error_code = 1;
        } else if (ship_idx >= 500) {
            error_code = 2;
        } else {
            int8_t owner = MOX::_ship[ship_idx].owner;
            if (owner < 0) {
                error_code = 3;
            } else if (owner > 15) {
                error_code = 4;
            }
        }

        if (error_code == 0) {
            return (int16_t)MOX::_ship[ship_idx].owner;
        }

        char message[200];
        // Note: The original code accesses the array even if the index is invalid in error paths.
        int32_t current_owner = (int32_t)MOX::_ship[ship_idx].owner;

        _sprintf(message, s_Memory_Corruption_val_00558028, (int)error_code, (int)ship_idx, current_owner, MOX::_stardate);

        exitm::Exit_With_Message(message);

        return 0;
    }

    int16_t __cdecl Ship_Stack_From_Saved_Array_() {
        int16_t best_owner_match_idx = -1;
        int16_t stack_index = -1;
        bool found = false;

        if (MOX::_ship_stack_count < 1) {
            return -1;
        }

        for (int16_t i = 0; i < 500; ++i) {
            // Status >= 3 typically indicates the ship is inactive/destroyed
            if (MOX::_ship[i].status < 3 && struct_::Test_Bit_Field_(&MOX::_fleet_box_ship_ids, i)) {
                int16_t target_owner = Ship_Owner_(i);

                for (int16_t j = 0; j < (int16_t)MOX::_ship_stack_count; ++j) {
                    if (found) break;

                    int16_t node_idx = MOX::_ship_stack_start[j];
                    int16_t stack_ship_idx = MOX::_ship_node[node_idx].ship_idx;

                    if (best_owner_match_idx == -1) {
                        if (Ship_Owner_(stack_ship_idx) == target_owner) {
                            best_owner_match_idx = j;
                        }
                    }

                    if (stack_ship_idx == i) {
                        found = true;
                        stack_index = j;
                    }
                }
            }
            if (found) break;
        }

        if (!found && best_owner_match_idx != -1) {
            stack_index = best_owner_match_idx;
        }

        return stack_index;
    }

    int16_t __cdecl Node_Ptr_From_Ship_Stack_(int16_t index) {
        if (MOX::_ship_stack_count == 0) {
            return 0;
        }

        if (MOX::_ship_stack_count < index) {
            MOVEBOX::Vanish_Fleet_Movement_Box_();
            return 0;
        }

        if (MOX::_ship_stack_count == index) {
            return Ship_Stack_From_Saved_Array_();
        }

        return MOX::_ship_stack_start[index];
    }

    bool __cdecl Star_Has_Relocation_(int16_t star_idx, int16_t player_idx) {
        return MOX::_star[star_idx].relocate_ship_to[player_idx] != -1;
    }

    int16_t __cdecl Relocation_(int16_t star_idx, int16_t player_idx) {
        return MOX::_star[star_idx].relocate_ship_to[player_idx];
    }

    void __cdecl Set_Colony_Auto_Select_For_Star_(int16_t star_idx, int16_t player_idx, int16_t* out_planet_idx) {
        int16_t colony_found = -1;
        bool found = false;
        *out_planet_idx = -1;

        for (int16_t i = 0; i < 5; ++i) {
            if (found) {
                break;
            }

            int16_t planet_idx = MOX::_star[star_idx].planet_index[i];
            if (planet_idx > -1) {
                *out_planet_idx = planet_idx;
                int16_t colony_idx = MOX::_planet[planet_idx].colony_index;

                if (colony_idx > -1) {
                    colony_found = colony_idx;
                    // Check if colony belongs to the player and is not an outpost
                    if (MOX::_colony[colony_idx].outpost_flag == 0 && MOX::_colony[colony_idx].owner == (char)player_idx) {
                        MOX::_star[star_idx].last_planet_selected[player_idx] = (int8_t)i;
                        found = true;
                    } else {
                        colony_found = -1;
                    }
                }
            }
        }

        if (colony_found == -1) {
            *out_planet_idx = -1;
        }
    }

    // BUG: The original implementation contains a potential bug where it skips every 
    // second node in the linked list chain during traversal.
    int16_t __cdecl Ship_Icon_Id_From_Node_Ptr_(int16_t node_ptr) {
        int16_t found_icon_idx = 0;
        bool found = false;
        int16_t i = 0;

        if (MOX::_ship_icon_count > 0) {
            do {
                if (found) {
                    break;
                }

                int16_t current_node_idx = MOX::_ship_icon[i].node_idx;

                if (current_node_idx == node_ptr) {
                    found = true;
                    found_icon_idx = i;
                } else {
                    // Traverse the node chain.
                    while (!found) {
                        if (current_node_idx == -1) {
                            break;
                        }

                        // Advance to next node and compare
                        current_node_idx = MOX::_ship_node[current_node_idx].next_node;
                        if (current_node_idx == node_ptr) {
                            found = true;
                            found_icon_idx = i;
                        } else if (current_node_idx != -1) {
                            // Advance to the next node again without comparing in this iteration.
                            current_node_idx = MOX::_ship_node[current_node_idx].next_node;
                        }
                    }
                }
                i++;
            } while (i < MOX::_ship_icon_count);
        }

        return found_icon_idx;
    }

    int16_t __cdecl Ship_Icon_Owner_(int16_t icon_id) {
        int16_t node_idx = MOX::_ship_icon[icon_id].node_idx;

        return Ship_Owner_(MOX::_ship_node[node_idx].ship_idx);
    }

    bool __cdecl Fleet_Movement_Icon_Selected_(int16_t icon_idx) {
        int16_t node_idx = MOX::_fleet_movement_icon[icon_idx].ship_node_idx;
        return (bool)MOX::_ship_node[node_idx].selected;
    }

    void __cdecl Do_Get_Planet_Name_(int16_t planet_idx, char* dest_buffer, uint8_t localized_flag) {
        char temp_buf[200];

        int8_t star_index = MOX::_planet[planet_idx].star_index;

        int8_t roman_index = HAROLD::Planet_Number_(planet_idx);
        const char* roman_str = MOX::_roman_literals[roman_index];

        if (localized_flag != 0) {
            if (strcmp(roman_str, s_I) == 0) {
                roman_str = HAROLD::H_Message_(0xA0);
            }
        }

        _sprintf(temp_buf, s_format_star_roman, &MOX::_star[star_index].name, roman_str);

        if (strlen(temp_buf) > 30) {
            temp_buf[30] = '\0';
        }

        strcpy(dest_buffer, temp_buf);
    }

    char* __cdecl Get_Planet_Name_(int16_t planet_idx) {
        Do_Get_Planet_Name_(planet_idx, MOX::_temp_string, 1);
        return MOX::_temp_string;
    }

    char* __cdecl Get_Planet_Name_No_Prime_(int16_t planet_idx) {
        Do_Get_Planet_Name_(planet_idx, MOX::_temp_string, 0);
        return MOX::_temp_string;
    }

    bool __cdecl Is_Combat_Ship_(int16_t ship_index) {
        return MOX::_ship[ship_index].d.ship_type == SHIP_TYPE_COMBAT;
    }

    int16_t __cdecl Last_Colony_Selected_(int16_t star_idx, int16_t player_idx) {
        int16_t colony_idx = -1;
        int16_t planet_idx = -1;

        int8_t last_sel_idx = MOX::_star[star_idx].last_planet_selected[player_idx];

        if (last_sel_idx >= 0 && last_sel_idx < 5) {
            planet_idx = MOX::_star[star_idx].planet_index[last_sel_idx];
        }

        if (planet_idx >= 0 && planet_idx < (int16_t)MOX::_planet_count) {
            colony_idx = MOX::_planet[planet_idx].colony_index;
        }

        if (colony_idx == -1 || MOX::_colony[colony_idx].owner != (int8_t)player_idx) {
            Set_Colony_Auto_Select_For_Star_(star_idx, player_idx, &planet_idx);

            if (planet_idx >= 0 && planet_idx < (int16_t)MOX::_planet_count) {
                return MOX::_planet[planet_idx].colony_index;
            }
            return -1;
        }

        return colony_idx;
    }

    bool __cdecl Player_Has_Colony_In_System_(int16_t player_idx, int16_t system_idx) {
        return HAROLD::Player_Has_Colony_(player_idx, system_idx);
    }

    void __cdecl Set_Relocation_(int16_t star_idx, int16_t player_idx, int16_t target_star_idx) {
        MOX::_star[star_idx].relocate_ship_to[player_idx] = target_star_idx;
    }

    void __cdecl Set_All_Star_Relocations_(int16_t target_star_idx, int16_t player_idx) {
        for (int16_t i = 0; i < (int16_t)MOX::_NUM_STARS; ++i) {
            if (Star_Has_Relocation_(i, player_idx)) {
                Set_Relocation_(i, player_idx, target_star_idx);
            }
        }
    }

    char* __cdecl Design_Name_(int16_t player_idx, int16_t design) {
        // Redundant operation from original code
        *(uint16_t*)MOX::_temp_string = *reinterpret_cast<const uint16_t*>(s__005510f0);

        strcpy(MOX::_temp_string, MOX::_player[player_idx].ship_designs[design].name);

        return MOX::_temp_string;
    }

    int16_t __cdecl Design_Weapon_(int16_t player_idx, int16_t design_idx, int16_t weapon_idx) {
        return MOX::_player[player_idx].ship_designs[design_idx].ship_weapon[weapon_idx].type;
    }

    int16_t __cdecl Design_Weapon_Count_(int16_t player_idx, int16_t design_idx, int16_t weapon_idx) {
        return MOX::_player[player_idx].ship_designs[design_idx].ship_weapon[weapon_idx].count;
    }

    int16_t __cdecl Design_Weapon_Specials_(int16_t player_idx, int16_t design_idx, int16_t weapon_idx) {
        return MOX::_player[player_idx].ship_designs[design_idx].ship_weapon[weapon_idx].specials;
    }

    int8_t __cdecl Design_Weapon_Firing_Arc_(int16_t player_idx, int16_t design_idx, int16_t weapon_idx) {
        return MOX::_player[player_idx].ship_designs[design_idx].ship_weapon[weapon_idx].firing_arc;
    }

    int8_t __cdecl Design_Weapon_Ammo_(int16_t player_idx, int16_t design_idx, int16_t weapon_idx) {
        return MOX::_player[player_idx].ship_designs[design_idx].ship_weapon[weapon_idx].ammo;
    }

    int8_t __cdecl Design_Picture_Num_(int16_t player_idx, int16_t design_idx) {
        return MOX::_player[player_idx].ship_designs[design_idx].picture_num;
    }

    int16_t __cdecl Fleet_Movement_Ship_Id_(int16_t icon_idx) {
        return MOX::_ship_node[MOX::_fleet_movement_icon[icon_idx].ship_node_idx].ship_idx;
    }

    int16_t __cdecl Ship_Stack_Owner_(int16_t stack_idx) {
        return Ship_Owner_(MOX::_ship_node[stack_idx].ship_idx);
    }

    /**
     * Finds the index in the current ship stack matching the node_idx of a given ship icon.
     *
     * @param icon_idx Index into the _ship_icon array.
     * @return The stack index if found, -1 if not found, or 0 if the stack is empty.
     */
    int16_t __cdecl Ship_Stack_From_Ship_Icon_(int16_t icon_idx) {
        int16_t target_node_idx = MOX::_ship_icon[icon_idx].node_idx;
        int16_t count = (int16_t)MOX::_ship_stack_count;
        int16_t i = 0;

        // If the stack is empty, the function immediately returns 0.
        if (count == 0) {
            return 0;
        }

        bool found = false;
        while (i < count) {
            if (MOX::_ship_stack_start[i] == target_node_idx) {
                found = true;
                break;
            }
            i++;
        }

        if (!found) {
            return -1;
        }

        return i;
    }

    char* __cdecl Race_Name_String_(int16_t race_index) {
        char local_buffer[20];

        Race_Name_(race_index, local_buffer);

        strcpy(MOX::_temp_string, local_buffer);

        return MOX::_temp_string;
    }

    int16_t __cdecl Weapons_Index_From_Tech_App_(int16_t tech_app_id) {
        int16_t result_index = 0;
        bool found = false;

        for (int16_t i = 0; i < 40 /* 0x28 */; ++i) {
            // Check if we have already found the weapon to early exit
            if (found) {
                break;
            }

            if (TECHDATA::_weapons[i].tech_application_id == tech_app_id) {
                result_index = i;
                found = true;
            }
        }

        return result_index;
    }

    /**
     * @brief Finds the index of the ship stack containing the given ship ID.
     * 
     * Iterates through all active ship stacks. For each stack, traverses the linked 
     * list of ship nodes until a matching ship_idx is found.
     * 
     * @param ship_id The ID of the ship to search for.
     * @return The index of the stack containing the ship, or 0 if not found.
     */
    int16_t __cdecl Ship_Stack_From_Ship_Id_(int16_t ship_id) {
        int16_t found_stack_index = 0;
        uint8_t match_found = 0;

        for (int16_t i = 0; i < (int16_t)MOX::_ship_stack_count; ++i) {
            // Check if we found the match in a previous iteration
            if (match_found != 0) {
                break;
            }

            int16_t node_idx = MOX::_ship_stack_start[i];
            uint8_t stack_traversal_done = 0;

            do {
                if (MOX::_ship_node[node_idx].ship_idx == ship_id) {
                    found_stack_index = i;
                    match_found = 1;
                    stack_traversal_done = 1;
                } else {
                    node_idx = MOX::_ship_node[node_idx].next_node;
                    // End of the linked list for this stack
                    if (node_idx == -1) {
                        stack_traversal_done = 1;
                    }
                }
            } while (stack_traversal_done == 0);
        }

        return found_stack_index;
    }

    int16_t __cdecl N_Colonies_At_Star_(int16_t star_index, int16_t player_index) {
        int16_t n_colonies = 0;

        for (int i = 0; i < 5; ++i) {
            int16_t planet_idx = MOX::_star[star_index].planet_index[i];

            // Check if there is a planet in this orbit
            if (planet_idx > -1) {
                int16_t colony_idx = MOX::_planet[planet_idx].colony_index;

                // Check if the planet has a colony
                if (colony_idx != -1) {
                    s_colony& colony = MOX::_colony[colony_idx];

                    // Outpost flag must be 0 (full colony, not an outpost)
                    if (colony.outpost_flag == 0) {
                        // Filter by owner index, or accept any if player_index is -1
                        if (colony.owner == player_index || player_index == -1) {
                            n_colonies++;
                        }
                    }
                }
            }
        }

        return n_colonies;
    }

    uint8_t __cdecl Get_Stack_Start_From_Saved_Ship_Array_() {
        MOX::_fleet_box_ship_stack = -1;
        int16_t stack_idx = 0;
        bool found = false;
        bool finished = false;

        while (!finished) {
            int16_t node_idx = MOX::_ship_stack_start[stack_idx];

            while (node_idx != -1) {
                // Check if ship from this stack is in the saved ship collection
                if (struct_::Test_Bit_Field_(&MOX::_fleet_box_ship_ids, MOX::_ship_node[node_idx].ship_idx)) {
                    MOX::_fleet_box_ship_stack = stack_idx;
                    found = true;
                    break;
                }
                node_idx = MOX::_ship_node[node_idx].next_node;
            }

            if (found) {
                break;
            }

            stack_idx++;
            if (stack_idx >= (int16_t)MOX::_ship_stack_count) {
                finished = true;
            }
        }

        if (MOX::_fleet_box_ship_stack > -1 && MOX::_fleet_box_ship_stack < (int16_t)MOX::_ship_stack_count) {
            return 1;
        }

        return 0;
    }

    /**
     * @brief Scans the ship node list and collects indices of selected ships belonging to the player,
     * optionally filtered by ship type.
     * 
     * @param selected_ids_out Buffer to store the resulting ship indices.
     * @param max The number of int16_t elements in the output buffer to initialize with -1.
     * @param filter The ship type to filter for (0xFF for no filter).
     * @return The number of selected ships found.
     */
    int16_t __cdecl Get_Fleet_Box_Selected_Ship_Ids_(int16_t* selected_ids_out, int max, int filter) {
        // Initialize output buffer with 0xFFFF (-1)
        memset(selected_ids_out, 0xff, max * sizeof(int16_t));

        int16_t selected_count = 0;
        bool stop_flag = false;

        for (int16_t i = 0; i < 500; ++i) {
            // Check stop flag at the top of the loop as per assembly
            if (stop_flag) {
                break;
            }

            int16_t ship_idx = MOX::_ship_node[i].ship_idx;
            s_ship_data& ship = MOX::_ship[ship_idx];

            bool is_match = false;
            // Verify ownership and optional ship type filter
            if (ship.owner == (char)MOX::_PLAYER_NUM) {
                if (filter == 0x00FF || (int8_t)ship.d.ship_type == (int8_t)filter) {
                    is_match = true;
                }
            }

            // If node is selected and criteria matches, record the index
            if (MOX::_ship_node[i].selected != 0 && is_match) {
                // BUG: Potential overflow if selected_count reached max capacity and original logic used hardcoded 500
                selected_ids_out[selected_count] = ship_idx;
                selected_count++;

                // Stop recording if count exceeds limit (safety check)
                if (selected_count >= max) {
                    stop_flag = true;
                }
            }
        }

        return selected_count;
    }
}