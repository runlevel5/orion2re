#include "deps.h"

namespace DESIGN {
    int16_t __cdecl Cost_Of_Design_(int16_t player_idx, s_ship_design* ship_design) {
        s_current_design local_design;

        struct_::Clear_Structure_(&local_design, sizeof(local_design));

        _design = &local_design;

        // strcpy(local_design.name, ship_design->name)
        char* src = ship_design->name;
        char* dst = local_design.name;
        while (*src) {
            *dst++ = *src++;
        }
        *dst = '\0';

        local_design.ftl_type = static_cast<int16_t>(ship_design->ftl_type);
        local_design.drive_speed = TECHDATA::_drives[local_design.ftl_type].speed;
        local_design.ship_size = static_cast<int16_t>(ship_design->size);

        local_design.hull_space = INITGAME::Get_Design_Hull_Size_();
        local_design.computer_type = static_cast<int16_t>(ship_design->computer_type);
        local_design.picture_type = static_cast<int16_t>(ship_design->picture_num);
        local_design.armor_type = static_cast<int16_t>(ship_design->armor_type);

        local_design.fuel_type = INITSHIP::Best_Ship_Fuel_(player_idx);

        for (int i = 0; i < 8; ++i) {
            local_design.weapon_type[i] = ship_design->ship_weapon[i].type;
            local_design.weapon_count[i] = static_cast<int16_t>(ship_design->ship_weapon[i].count);
            local_design.weapon_specials[i] = ship_design->ship_weapon[i].specials;
            local_design.weapon_firing_arc[i] = ship_design->ship_weapon[i].firing_arc;
            local_design.weapon_ammo[i] = ship_design->ship_weapon[i].ammo;
        }

        for (int i = 0; i < 8; ++i) {
            local_design.special_devices[i] = 0;
            local_design.special_device_cost[i] = 0;
            local_design.special_device_space[i] = 0;
        }

        int16_t special_idx = 0;
        bool stop_search = false;
        for (uint32_t i = 0; i < 0x27 && !stop_search; ++i) {
            if (struct_::Test_Bit_Field_(ship_design->special_device_flags, static_cast<uint16_t>(i))) {
                local_design.special_devices[special_idx] = static_cast<int16_t>(i);
                special_idx++;
                if (special_idx >= 8) {
                    stop_search = true;
                }
            }
        }

        local_design.base_combat_speed = Get_Modified_Combat_Speed_(_design);
        Update_Calculated_Design_Data_(player_idx);

        return static_cast<int16_t>(local_design.total_cost);
    }

    int8_t __cdecl Count_Weapons_Loaded_() {
        int8_t count = 0;

        for (int i = 0; i < 8; ++i) {
            if (_design->weapon_type[i] > 0) {
                count++;
            }
        }

        return count;
    }

    int8_t __cdecl Get_Modified_Combat_Speed_(s_current_design* design) {
        int8_t speed = Current_Design_Base_Combat_Speed_(design);

        if (Current_Design_Has_Special_System_(design, SPECIAL_AUGMENTED_ENGINES) != 0) {
            if (_use_augmented_engines != 0) {
                speed += 5;
            }
        }

        return speed;
    }

    int8_t __cdecl Get_Base_Combat_Speed_(s_current_design* design) {
        return TECHDATA::_drives[design->ftl_type].combat_speed_modifier[design->ship_size];
    }

    int8_t __cdecl Get_Ship_Speed_Modifier_(s_current_design* design) {
        return TECHDATA::_drives[design->ftl_type].speed_modifier[design->ship_size];
    }

    int8_t __cdecl Current_Design_Base_Combat_Speed_(s_current_design* design) {
        uint8_t base_speed = (uint8_t)Get_Base_Combat_Speed_(design);
        uint8_t speed_mod = (uint8_t)Get_Ship_Speed_Modifier_(design);

        int32_t space_utilization = (-100 * design->space_used) / design->hull_space;

        int16_t speed_diff = (int16_t)speed_mod - (int16_t)base_speed;

        int32_t adjustment = (space_utilization * (int32_t)speed_diff) / 100;

        int16_t final_speed = (int16_t)speed_mod + (int16_t)adjustment;

        if (final_speed < (int16_t)base_speed) {
            final_speed = (int16_t)base_speed;
        }

        return (int8_t)final_speed;
    }

    uint8_t __cdecl Current_Design_Has_Special_System_(s_current_design* design, uint32_t system_id) {
        for (int16_t i = 0; i < 8; ++i) {
            if (design->special_devices[i] == static_cast<int16_t>(system_id)) {
                return 1;
            }
        }
        return 0;
    }

    int8_t __cdecl Count_Special_Devices_() {
        int8_t device_count = 0;

        for (int16_t i = 0; i < 8; ++i) {
            if (MOX::_design->special_devices[i] > 0) {
                device_count++;
            }
        }

        return device_count;
    }

    void __cdecl Has_Damper_Field_() {
        bool found_damper = false;

        for (int16_t i = 0; i < 8; ++i) {
            if (_design->special_devices[i] == SPECIAL_DAMPER_FIELD) {
                found_damper = true;
                break;
            }
        }

        if (found_damper) {
            _design->shield = 0;
        }
    }

    uint8_t __cdecl Current_Design_Shield_() {
        return static_cast<uint8_t>(_design->shield);
    }

    int16_t __cdecl Base_Shield_Cost() {
        return _design->shield_cost;
    }

    int16_t __cdecl Get_Shild_Space_() {
        return MOX::_design->shield_space;
    }

    void __cdecl Update_Calculated_Design_Data_(int16_t player_idx) {
        Has_Damper_Field_();
        Current_Design_Shield_();
        Design_Template_Costs_(player_idx);
        Design_Template_Space_Requirements_(player_idx);

        _design->total_cost = Total_Design_Cost_();
        _design->space_used = Total_Space_Used_();
        _design->hull_space = Get_Design_Hull_Size_();

        MOX::_n_weapons_loaded = Count_Weapons_Loaded_();
        MOX::_n_special_systems = Count_Special_Devices_();

        _printed_space_avail = _design->hull_space - _design->space_used;
        _printed_cost = static_cast<uint16_t>(_design->total_cost);

        Update_N_Spec_Systems_In_Design_();
        Update_Weapon_Count_();

        if (_design->show_help != 0) {
            EVANHELP::Set_Design_Screen_Help_List_(_design->help1, _design->help2);
            _design->show_help = 0;
        }
    }

    void __cdecl Design_Template_Costs_(int16_t player_idx) {
        s_current_design* design = MOX::_design;

        // Calculate Shield Cost
        int16_t shield_idx = design->shield;
        int16_t shield_tech_level = Tech_Level_(player_idx, TECHDATA::_shields[shield_idx].tech_app_id);
        int16_t shield_base = Shield_Base_Cost_(shield_idx);
        design->shield_cost = Cost_Given_M_Level_(shield_base, shield_tech_level);

        // Calculate Drive Cost
        int16_t drive_idx = design->ftl_type;
        int16_t drive_tech_level = Tech_Level_(player_idx, TECHDATA::_drives[drive_idx].tech_app);
        int16_t drive_base = TECHDATA::_drives[drive_idx].cost_per_ship_type[design->ship_size];
        design->drive_cost = Cost_Given_M_Level_(drive_base, drive_tech_level);

        // Calculate Armor Cost (Scaled by hull space and divided by 100)
        int32_t hull_space = TECHDATA::_hull_data[design->ship_size].hull_space;
        int32_t armor_unit_cost = TECHDATA::_armor[design->armor_type].cost;
        design->armor_cost = static_cast<int16_t>((hull_space * armor_unit_cost) / 100);

        // Calculate Weapon Costs
        for (int i = 0; i < 8; ++i) {
            if (design->weapon_type[i] > 0) {
                design->weapon_cost[i] = Weapon_Cost_(
                    design->weapon_type[i],
                    design->weapon_count[i],
                    static_cast<int16_t>(design->weapon_firing_arc[i]),
                    static_cast<int16_t>(design->weapon_ammo[i]),
                    design->weapon_specials[i]
                );
            }
        }

        // Calculate Special Device Costs
        for (int i = 0; i < 8; ++i) {
            int16_t special_idx = design->special_devices[i];
            if (special_idx > 0) {
                int16_t base_cost = TECHDATA::_specials[special_idx].cost_per_ship_type[design->ship_size];
                if (special_idx == 4) { // Special logic for specific device ID 4 (Reinforced Hull?)
                    design->special_device_cost[i] = base_cost;
                } else {
                    int16_t special_tech_level = Tech_Level_(player_idx, TECHDATA::_specials[special_idx].tech_application_id);
                    design->special_device_cost[i] = Cost_Given_M_Level_(base_cost, special_tech_level);
                }
            }
        }

        // Calculate Computer Cost
        int16_t computer_idx = design->computer_type;
        int16_t comp_tech_level = Tech_Level_(player_idx, TECHDATA::_computers[computer_idx].tech_app);
        int16_t comp_base = Base_Computer_Cost_(computer_idx);
        design->computer_cost = Cost_Given_M_Level_(comp_base, comp_tech_level);
    }

    int32_t __cdecl Total_Design_Cost_() {
        int16_t shield_cost = Base_Shield_Cost();
        int weapon_total_cost = 0;

        for (int i = 0; i < 8; ++i) {
            if (_design->weapon_type[i] > 0) {
                weapon_total_cost += _design->weapon_cost[i];
            }
        }

        int special_device_total_cost = 0;
        for (int i = 0; i < 8; ++i) {
            if (_design->special_devices[i] > 0) {
                special_device_total_cost += _design->special_device_cost[i];
            }
        }

        int total_cost = (int)TECHDATA::_hull_data[_design->ship_size].hull_space +
                         (int)_design->computer_cost +
                         (int)_design->drive_cost +
                         (int)_design->armor_cost +
                         (int)shield_cost +
                         weapon_total_cost +
                         special_device_total_cost;

        Cost_Reduction_For_Govt_Type_(MOX::_PLAYER_NUM, &total_cost);

        return total_cost;
    }

    void __cdecl Design_Template_Space_Requirements_(int16_t player_num) {
        s_current_design* design = MOX::_design;

        // Calculate FTL drive space
        int16_t ftl_type = design->ftl_type;
        int16_t drive_tech_app = TECHDATA::_drives[ftl_type].tech_app;
        uint8_t drive_min_cat = static_cast<uint8_t>(Miniaturization_Category_(drive_tech_app));
        int16_t drive_tech_level = Tech_Level_(player_num, drive_tech_app);

        int16_t base_drive_space = TECHDATA::_drives[ftl_type].space_per_ship_type[design->ship_size];
        design->base_space = (int8_t)Space_Given_M_Level_(base_drive_space, drive_tech_level, drive_min_cat);

        design->field19_0x29 = 0;

        // Calculate Weapon space for all 8 slots
        for (int i = 0; i < 8; ++i) {
            if (design->weapon_type[i] > 0) {
                design->weapon_space[i] = Weapon_Space_(
                    MOX::_PLAYER_NUM,
                    design->weapon_type[i],
                    design->weapon_count[i],
                    static_cast<int16_t>(design->weapon_firing_arc[i]),
                    static_cast<int16_t>(design->weapon_ammo[i]),
                    design->weapon_specials[i]
                );
            }
        }

        // Calculate Shield space
        design->shield_space = (int16_t)Shield_Space_Per_Tech_(design->shield, design->ship_size, MOX::_PLAYER_NUM);

        // Calculate Special Device space for all 8 slots
        for (int i = 0; i < 8; ++i) {
            int16_t device_id = design->special_devices[i];
            if (device_id > 0) {
                if (device_id == 4) {
                    // Device 4 has fixed space requirements (not miniaturized)
                    design->special_device_space[i] = TECHDATA::_specials[4].space_per_ship_type[design->ship_size];
                } else {
                    int16_t tech_app = TECHDATA::_specials[device_id].tech_application_id;
                    uint8_t min_cat = static_cast<uint8_t>(Miniaturization_Category_(tech_app));
                    int16_t tech_level = Tech_Level_(player_num, tech_app);

                    int16_t base_device_space = TECHDATA::_specials[device_id].space_per_ship_type[design->ship_size];
                    design->special_device_space[i] = (int16_t)Space_Given_M_Level_(base_device_space, tech_level, min_cat);
                }
            }
        }
    }

    void __cdecl Update_Weapon_Count_() {
        MOX::_design->help1 = 0;
        for (int i = 0; i < 8; ++i) {
            if (MOX::_design->weapon_type[i] > 0) {
                MOX::_design->help1++;
            }
        }
    }

    void __cdecl Update_N_Spec_Systems_In_Design_() {
        MOX::_design->help2 = 0;

        for (int16_t i = 0; i < 8; ++i) {
            if (MOX::_design->special_devices[i] > 0) {
                MOX::_design->help2++;

                if (MOX::_design->help2 >= 8) {
                    break;
                }
            }
        }
    }

    int32_t __cdecl Get_Design_Hull_Size_() {
        return INITGAME::Get_Design_Hull_Size_();
    }

    int32_t __cdecl Total_Space_Used_() {
        int32_t total_space = (int32_t)Get_Shild_Space_() + (int32_t)_design->base_space;

        int32_t weapon_space_sum = 0;
        for (int i = 0; i < 8; ++i) {
            if (_design->weapon_type[i] > 0) {
                weapon_space_sum += _design->weapon_space[i];
            }
        }
        total_space += weapon_space_sum;

        int32_t special_space_sum = 0;
        for (int i = 0; i < 8; ++i) {
            if (_design->special_devices[i] > 0) {
                special_space_sum += (int32_t)_design->special_device_space[i];
            }
        }
        total_space += special_space_sum;

        return total_space;
    }
}