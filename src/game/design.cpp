#include "pch.h"

namespace DESIGN {
    int16_t _repeat_count;
    uint8_t _replacement_wait_ctr;
    int16_t _design_size_field;
    uint8_t _use_augmented_engines;

    int32_t _printed_space_avail;
    int16_t _design_shield_replacement_field;
    int16_t _design_computer_replacement_field;
    int32_t _printed_cost;
    uint8_t _changed_design_name;
    uint8_t _add_size_button_flag[SHIP_SIZE_COUNT];

    const char s_comma_space[3] = ", ";

    s_animation_header* _dull_plus_button_seg;
    s_animation_header* _dull_minus_button_seg;
    s_animation_header* _dark_build_design_button_seg;

    void __cdecl Build_Initial_Design_Template_(int16_t player_idx, int16_t design_idx) {
        s_current_design* d = MOX::_design;

        struct_::Clear_Structure_(d, sizeof(*d));

        char* name = HACCESS::Design_Name_(player_idx, design_idx);
        strlcpy(d->name, name, sizeof(d->name));

        d->ftl_type = INITSHIP::Best_Warp_Drive_(player_idx);
        d->drive_speed = TECHDATA::_drives[d->ftl_type].warp_speed;
        d->ship_size = static_cast<int16_t>(static_cast<int8_t>(Current_Design_Size_(player_idx, design_idx)));
        d->hull_space = DESIGN::Total_Design_Space_();
        d->computer_type = INITSHIP::Best_Computer_(player_idx);
        d->picture_type = static_cast<int16_t>(static_cast<int8_t>(HACCESS::Design_Picture_Num_(player_idx, design_idx)));
        d->armor_type = INITSHIP::Best_Armor_(player_idx);
        d->fuel_type = INITSHIP::Best_Ship_Fuel_(player_idx);
        d->shield = INITSHIP::Best_Ship_Shield_(MOX::_PLAYER_NUM);

        for (int i = 0; i < 8; ++i) {
            d->weapon_space[i] = 0;
            d->weapon_cost[i] = 0;
        }

        d->num_weapon_slots = 0;
        for (int i = 0; i < 8; ++i) {
            d->weapon_type[i] = HACCESS::Design_Weapon_(player_idx, design_idx, i);
            d->weapon_count[i] = static_cast<int16_t>(static_cast<int8_t>(HACCESS::Design_Weapon_Count_(player_idx, design_idx, i)));
            d->weapon_specials[i] = HACCESS::Design_Weapon_Specials_(player_idx, design_idx, i);
            d->weapon_firing_arc[i] = static_cast<int8_t>(HACCESS::Design_Weapon_Firing_Arc_(player_idx, design_idx, i));
            d->weapon_ammo[i] = static_cast<int8_t>(HACCESS::Design_Weapon_Ammo_(player_idx, design_idx, i));

            int16_t rack_type = DESIGN::Weapon_Type_(d->weapon_type[i]);
            if (rack_type == 2) {
                if (d->weapon_firing_arc[i] != WEAPON_FIRING_ARC_360) {
                    d->weapon_firing_arc[i] = WEAPON_FIRING_ARC_360;
                }
            } else {
                if (Weapon_Needs_Racks_(d->weapon_type[i]) != 0 &&
                    (d->weapon_firing_arc[i] & WEAPON_FIRING_ARC_ALL_SECTORS) == WEAPON_FIRING_ARC_NONE) {
                    d->weapon_firing_arc[i] = WEAPON_FIRING_ARC_360;
                }
            }

            if (d->weapon_type[i] > 0) {
                d->weapon_space[i] = Weapon_Space_(
                    MOX::_PLAYER_NUM,
                    d->weapon_type[i],
                    d->weapon_count[i],
                    (int16_t)d->weapon_firing_arc[i],
                    (int16_t)d->weapon_ammo[i],
                    d->weapon_specials[i]
                );
                d->num_weapon_slots++;
            }
        }

        for (int i = 0; i < 8; ++i) {
            d->special_devices[i] = 0;
            d->special_device_cost[i] = 0;
            d->special_device_space[i] = 0;
        }

        int16_t special_idx = 0;
        for (int bit = 0; bit < 39; ++bit) {
            if (special_idx >= 8) break;
            if (struct_::Test_Bit_Field_(MOX::_player[player_idx].ship_designs[design_idx].special_device_flags, static_cast<uint16_t>(bit))) {
                d->special_devices[special_idx] = static_cast<int16_t>(bit);
                special_idx++;
            }
        }
        d->num_special_slots = special_idx;

        d->base_combat_speed = Current_Design_Combat_Speed_(d);
        d->special_system_scroll_thumb_y = 140;

        Update_Calculated_Design_Data_(player_idx);
    }

    int16_t __cdecl Cost_Of_Design_(int16_t player_idx, s_ship_design* ship_design) {
        // TODO: Pass player_idx through the design-cost helpers instead of relying on a global.
        int16_t previous_player_idx = MOX::_PLAYER_NUM;
        MOX::_PLAYER_NUM = player_idx;

        s_current_design current_design;

        struct_::Clear_Structure_(&current_design, sizeof(current_design));

        MOX::_design = &current_design;

        strlcpy(current_design.name, ship_design->name, sizeof(current_design.name));

        current_design.ftl_type = static_cast<int16_t>(ship_design->ftl_type);
        current_design.drive_speed = TECHDATA::_drives[current_design.ftl_type].warp_speed;
        current_design.ship_size = static_cast<int16_t>(ship_design->size);

        current_design.hull_space = DESIGN::Total_Design_Space_();
        current_design.computer_type = static_cast<int16_t>(ship_design->computer_type);
        current_design.picture_type = static_cast<int16_t>(ship_design->picture_num);
        current_design.armor_type = static_cast<int16_t>(ship_design->armor_type);

        current_design.fuel_type = INITSHIP::Best_Ship_Fuel_(player_idx);

        for (int i = 0; i < 8; ++i) {
            current_design.weapon_type[i] = ship_design->ship_weapon[i].type;
            current_design.weapon_count[i] = static_cast<int16_t>(ship_design->ship_weapon[i].count);
            current_design.weapon_specials[i] = ship_design->ship_weapon[i].specials;
            current_design.weapon_firing_arc[i] = ship_design->ship_weapon[i].firing_arc;
            current_design.weapon_ammo[i] = ship_design->ship_weapon[i].ammo;
        }

        for (int i = 0; i < 8; ++i) {
            current_design.special_devices[i] = 0;
            current_design.special_device_cost[i] = 0;
            current_design.special_device_space[i] = 0;
        }

        int16_t special_idx = 0;
        bool stop_search = false;
        for (uint32_t i = 0; i < 0x27 && !stop_search; ++i) {
            if (struct_::Test_Bit_Field_(ship_design->special_device_flags, static_cast<uint16_t>(i))) {
                current_design.special_devices[special_idx] = static_cast<int16_t>(i);
                special_idx++;
                if (special_idx >= 8) {
                    stop_search = true;
                }
            }
        }

        current_design.base_combat_speed = Current_Design_Combat_Speed_(MOX::_design);
        Update_Calculated_Design_Data_(player_idx);

        int16_t cost = static_cast<int16_t>(current_design.total_cost);
        MOX::_PLAYER_NUM = previous_player_idx;
        return cost;
    }

    uint8_t __cdecl Current_Design_Size_(int16_t player_idx, int16_t design_idx) {
        return static_cast<uint8_t>(MOX::_player[player_idx].ship_designs[design_idx].size);
    }

    int8_t __cdecl Count_Weapons_Loaded_() {
        int8_t count = 0;

        for (int i = 0; i < 8; ++i) {
            if (MOX::_design->weapon_type[i] > 0) {
                count++;
            }
        }

        return count;
    }

    uint8_t __cdecl Current_Design_Combat_Speed_(s_current_design* design) {
        uint8_t speed = Current_Design_Base_Combat_Speed_(design);

        if (Current_Design_Has_Special_System_(design, SPECIAL_AUGMENTED_ENGINES) != 0) {
            if (_use_augmented_engines != 0) {
                speed += 5;
            }
        }

        return speed;
    }

    uint8_t __cdecl Current_Design_Min_Combat_Speed_(s_current_design* design) {
        return TECHDATA::_drives[design->ftl_type].combat_speed_min[design->ship_size];
    }

    uint8_t __cdecl Current_Design_Max_Combat_Speed_(s_current_design* design) {
        return TECHDATA::_drives[design->ftl_type].combat_speed_max[design->ship_size];
    }

    uint8_t __cdecl Current_Design_Base_Combat_Speed_(s_current_design* design) {
        uint8_t base_speed = (uint8_t)Current_Design_Min_Combat_Speed_(design);
        uint8_t speed_mod = (uint8_t)Current_Design_Max_Combat_Speed_(design);

        int32_t space_utilization = (-100 * design->space_used) / design->hull_space;

        int16_t speed_diff = (int16_t)speed_mod - (int16_t)base_speed;

        int32_t adjustment = (space_utilization * (int32_t)speed_diff) / 100;

        int16_t final_speed = (int16_t)speed_mod + (int16_t)adjustment;

        if (final_speed < (int16_t)base_speed) {
            final_speed = (int16_t)base_speed;
        }

        return (uint8_t)final_speed;
    }

    uint8_t __cdecl Current_Design_Has_Special_System_(s_current_design* design, int16_t system_id) {
        uint8_t found = 0;

        for (int16_t i = 0; i < 8 && found == 0; ++i) {
            if (design->special_devices[i] == system_id) {
                found = 1;
            }
        }

        return found;
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

    bool __cdecl Has_Damper_Field_() {
        bool found_damper = false;

        for (int16_t i = 0; i < 8; ++i) {
            if (MOX::_design->special_devices[i] == SPECIAL_DAMPER_FIELD) {
                found_damper = true;
                break;
            }
        }

        if (found_damper) {
            MOX::_design->shield = SHIELD_NO_SHIELD;
        }

        return found_damper;
    }

    uint8_t __cdecl Current_Design_Shield_() {
        return static_cast<uint8_t>(MOX::_design->shield);
    }

    int16_t __cdecl Base_Shield_Cost() {
        return MOX::_design->shield_cost;
    }

    int16_t __cdecl Get_Shild_Space_() {
        return MOX::_design->shield_space;
    }

    int32_t __cdecl Total_Design_Space_() {
        int16_t ship_size = MOX::_design->ship_size;
        int32_t hull_size = TECHDATA::_hull_data[ship_size].size;

        if (MOX::_player[MOX::_PLAYER_NUM].tech_applications[TECH_APP_MEGAFLUXERS] == TECH_RESEARCH_STATUS_RESEARCHED) {
            hull_size = (hull_size * 125) / 100;
        }

        return hull_size;
    }

    void __cdecl Update_Calculated_Design_Data_(int16_t player_idx) {
        Has_Damper_Field_();
        Current_Design_Shield_();
        Design_Template_Costs_(player_idx);
        Design_Template_Space_Requirements_(player_idx);

        MOX::_design->total_cost = Total_Design_Cost_();
        MOX::_design->space_used = Total_Space_Used_();
        MOX::_design->hull_space = Total_Design_Space_();

        MOX::_n_weapons_loaded = Count_Weapons_Loaded_();
        MOX::_n_special_systems = Count_Special_Devices_();

        _printed_space_avail = MOX::_design->hull_space - MOX::_design->space_used;
        _printed_cost = MOX::_design->total_cost;

        Update_N_Spec_Systems_In_Design_();
        Update_Weapon_Count_();

        if (MOX::_design->show_help != 0) {
            EVANHELP::Set_Design_Screen_Help_List_(MOX::_design->num_weapon_slots, MOX::_design->num_special_slots);
            MOX::_design->show_help = 0;
        }
    }

    void __cdecl Design_Template_Costs_(int16_t player_idx) {
        s_current_design* design = MOX::_design;

        // Calculate Shield Cost
        int16_t shield_idx = design->shield;
        int16_t shield_tech_level = Tech_Level_(player_idx, TECHDATA::_shields[shield_idx].tech_app_id);
        int16_t shield_base = Shield_Cost_(shield_idx);
        design->shield_cost = Cost_Given_M_Level_(shield_base, shield_tech_level);

        // Calculate Drive Cost
        int16_t drive_idx = design->ftl_type;
        int16_t drive_tech_level = Tech_Level_(player_idx, TECHDATA::_drives[drive_idx].tech_app_id);
        int16_t drive_base = TECHDATA::_drives[drive_idx].cost_per_ship_type[design->ship_size];
        design->drive_cost = Cost_Given_M_Level_(drive_base, drive_tech_level);

        // Calculate Armor Cost (Scaled by hull space and divided by 100)
        int32_t hull_space = TECHDATA::_hull_data[design->ship_size].cost;
        int32_t armor_unit_cost = TECHDATA::_armor[design->armor_type].cost_percent;
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
                if (special_idx == SPECIAL_BATTLE_PODS) {
                    design->special_device_cost[i] = base_cost;
                } else {
                    int16_t special_tech_level = Tech_Level_(player_idx, TECHDATA::_specials[special_idx].tech_app_id);
                    design->special_device_cost[i] = Cost_Given_M_Level_(base_cost, special_tech_level);
                }
            }
        }

        // Calculate Computer Cost
        int16_t computer_idx = design->computer_type;
        int16_t comp_tech_level = Tech_Level_(player_idx, TECHDATA::_computers[computer_idx].tech_app_id);
        int16_t comp_base = Computer_Cost_(computer_idx);
        design->computer_cost = Cost_Given_M_Level_(comp_base, comp_tech_level);
    }

    int32_t __cdecl Total_Design_Cost_() {
        int16_t shield_cost = Base_Shield_Cost();
        int16_t ship_size = MOX::_design->ship_size;
        int weapon_total_cost = 0;

        for (int i = 0; i < 8; ++i) {
            if (MOX::_design->weapon_type[i] > 0) {
                weapon_total_cost += MOX::_design->weapon_cost[i];
            }
        }

        int special_device_total_cost = 0;
        for (int i = 0; i < 8; ++i) {
            if (MOX::_design->special_devices[i] > 0) {
                special_device_total_cost += MOX::_design->special_device_cost[i];
            }
        }

        int total_cost = (int32_t)(int16_t)TECHDATA::_hull_data[ship_size].cost +
                         (int)MOX::_design->computer_cost +
                         (int)MOX::_design->drive_cost +
                         (int)MOX::_design->armor_cost +
                         (int32_t)(int16_t)shield_cost +
                         weapon_total_cost +
                         special_device_total_cost;

        Cost_Reduction_For_Govt_Type_(MOX::_PLAYER_NUM, &total_cost);

        return total_cost;
    }

    void __cdecl Design_Template_Space_Requirements_(int16_t player_num) {
        s_current_design* design = MOX::_design;

        // Calculate FTL drive space
        int16_t ftl_type = design->ftl_type;
        int16_t drive_tech_app = TECHDATA::_drives[ftl_type].tech_app_id;
        int8_t drive_min_cat = Miniaturization_Category_(drive_tech_app);
        int16_t drive_tech_level = Tech_Level_(player_num, drive_tech_app);

        int16_t base_drive_space = TECHDATA::_drives[ftl_type].space_per_ship_type[design->ship_size];
        design->base_space = Space_Given_M_Level_(base_drive_space, drive_tech_level, drive_min_cat);

        design->field18_0x29 = 0;

        // Calculate Weapon space for all 8 slots
        for (int i = 0; i < 8; ++i) {
            if (design->weapon_type[i] > 0) {
                design->weapon_space[i] = Weapon_Space_(
                    MOX::_PLAYER_NUM,
                    design->weapon_type[i],
                    design->weapon_count[i],
                    (int16_t)design->weapon_firing_arc[i],
                    (int16_t)design->weapon_ammo[i],
                    design->weapon_specials[i]
                );
            }
        }

        // Calculate Shield space
        design->shield_space = (int16_t)Shield_Space_Given_M_Level_(design->shield, design->ship_size, MOX::_PLAYER_NUM);

        // Calculate Special Device space for all 8 slots
        for (int i = 0; i < 8; ++i) {
            int16_t device_id = design->special_devices[i];
            if (device_id > 0) {
                if (device_id == SPECIAL_BATTLE_PODS) {
                    design->special_device_space[i] = TECHDATA::_specials[SPECIAL_BATTLE_PODS].space_per_ship_type[design->ship_size];
                } else {
                    int16_t tech_app = TECHDATA::_specials[device_id].tech_app_id;
                    int8_t min_cat = Miniaturization_Category_(tech_app);
                    int16_t tech_level = Tech_Level_(player_num, tech_app);

                    int16_t base_device_space = TECHDATA::_specials[device_id].space_per_ship_type[design->ship_size];
                    design->special_device_space[i] = (int16_t)Space_Given_M_Level_(base_device_space, tech_level, min_cat);
                }
            }
        }
    }

    void __cdecl Update_Weapon_Count_() {
        MOX::_design->num_weapon_slots = 0;
        for (int i = 0; i < 8; ++i) {
            if (MOX::_design->weapon_type[i] > 0) {
                MOX::_design->num_weapon_slots++;
            }
        }
    }

    void __cdecl Update_N_Spec_Systems_In_Design_() {
        MOX::_design->num_special_slots = 0;

        for (int16_t i = 0; i < 8; ++i) {
            if (MOX::_design->special_devices[i] > 0) {
                MOX::_design->num_special_slots++;

                if (MOX::_design->num_special_slots >= 8) {
                    break;
                }
            }
        }
    }

    /**
     * @brief Applies cost reduction to the provided cost based on the player's government type (trait 0).
     * 
     * @param player_idx Index of the player in the global player array.
     * @param cost Pointer to the cost value to be modified.
     */
    void __cdecl Cost_Reduction_For_Govt_Type_(int16_t player_idx, int32_t *cost) {
        s_player *player = &MOX::_player[player_idx];
        int8_t govt_trait = player->traits[TRAIT_CURRENT_GOVERNMENT];

        if (govt_trait == GOVERNMENT_FEUDAL) {
            *cost = (*cost * 2 + 2) / 3;
        } else if (govt_trait == GOVERNMENT_CONFEDERATION) {
            *cost = (*cost + 2) / 3;
        }
    }

    int16_t __cdecl Current_Design_Structural_Points_() {
        int16_t player_num = MOX::_PLAYER_NUM;
        uint8_t has_reinforced_hull = Current_Design_Has_Special_System_(MOX::_design, SPECIAL_REINFORCED_HULL);
        return INITSHIP::Get_Structure_(MOX::_design->ship_size, has_reinforced_hull != 0, player_num);
    }

    int16_t __cdecl Current_Design_Armor_Absorption_() {
        s_current_design* design = MOX::_design;

        int16_t base_absorption = TECHDATA::_hull_data[design->ship_size].armor_hp;

        int32_t bonus_percent = (int32_t)TECHDATA::_armor[design->armor_type].ships_bonus;
        int16_t total_absorption = base_absorption + (int16_t)((int32_t)base_absorption * bonus_percent / 100);

        if (Current_Design_Has_Special_System_(design, SPECIAL_HEAVY_ARMOR)) {
            total_absorption *= 3;
        }

        return total_absorption;
    }

    int16_t __cdecl Shield_Strength_() {
        s_current_design* design = MOX::_design;
        int16_t multiplier;

        if (design->ship_size < SHIP_SIZE_DOOM_STAR) {
            multiplier = design->ship_size + 1;
        } else {
            multiplier = design->ship_size + 2;
        }

        int16_t strength = TECHDATA::_shields[design->shield].shield_class * multiplier * 5;

        if (Current_Design_Has_Special_System_(design, SPECIAL_MULTI_PHASED_SHIELDS)) {
            strength = (int16_t)((strength * 3) / 2);
        }

        return strength;
    }

    int16_t __cdecl Shield_Space_(int16_t shield_idx, int16_t ship_size) {
        int16_t space = TECHDATA::_shields[shield_idx].space_per_ship_type[ship_size];

        if (shield_idx > 0 && space == 0) {
            return 1;
        }

        return space;
    }

    /**
     * Returns the shield space after applying the owning player's miniaturization level.
     *
     * @param shield_id Shield entry to evaluate.
     * @param ship_type Hull size index.
     * @param player_idx Player whose tech level drives miniaturization.
     */
    int32_t __cdecl Shield_Space_Given_M_Level_(int16_t shield_id, int16_t ship_type, int16_t player_idx) {
        // Calculate the base space requirement for this shield and ship type
        int16_t base_space = DESIGN::Shield_Space_(shield_id, ship_type);

        // Determine the effective technology level for the owning player and shield's tech app
        int16_t tech_level = DESIGN::Tech_Level_(player_idx, TECHDATA::_shields[shield_id].tech_app_id);

        // Adjust the base space requirement according to the technology level
        return DESIGN::Space_Given_M_Level_(base_space, tech_level, 0);
    }

    int16_t __cdecl Current_Shield_Damage_Points_Blocked_() {
        int16_t shield_idx = MOX::_design->shield;

        int16_t blocked_points = TECHDATA::_shields[shield_idx].shield_class;

        if (Current_Design_Has_Special_System_(MOX::_design, SPECIAL_HARD_SHIELDS)) {
            blocked_points += 3;
        }

        return blocked_points;
    }

    int16_t __cdecl Computer_Cost_(int16_t computer_idx) {
        int16_t cost = TECHDATA::_computers[computer_idx].cost_by_ship_type[MOX::_design->ship_size];

        if (cost == 0 && computer_idx > 0) {
            return 1;
        }

        return cost;
    }

    int32_t __cdecl Total_Space_Used_() {
        int32_t total_space = (int32_t)Get_Shild_Space_() + (int32_t)MOX::_design->base_space;

        int32_t weapon_space_sum = 0;
        for (int i = 0; i < 8; ++i) {
            if (MOX::_design->weapon_type[i] > 0) {
                weapon_space_sum += MOX::_design->weapon_space[i];
            }
        }
        total_space += weapon_space_sum;

        int32_t special_space_sum = 0;
        for (int i = 0; i < 8; ++i) {
            if (MOX::_design->special_devices[i] > 0) {
                special_space_sum += (int32_t)MOX::_design->special_device_space[i];
            }
        }
        total_space += special_space_sum;

        return total_space;
    }

    int16_t __cdecl Weapon_Type_(int16_t weapon_idx) {
        return static_cast<int16_t>(static_cast<int8_t>(TECHDATA::_weapons[weapon_idx].type));
    }

    int16_t __cdecl Weapon_Needs_Racks_(int16_t weapon_idx) {
        return DESIGN::Weapon_Type_(weapon_idx) == 1;
    }

    int16_t __cdecl Weapon_Rack_Size_(int16_t count) {
        switch (count) {
            case 2:
                return 10;
            case 5:
                return 20;
            case 10:
                return 30;
            case 15: // 0x0F
                return 35; // 0x23
            case 20: // 0x14
                return 40; // 0x28
            default:
                return 0;
        }
    }

    int32_t __cdecl Weapon_Space_(int16_t player_idx, int16_t weapon_idx, int16_t quantity,
                                  int16_t firing_arc, int16_t ammo, int16_t specials) {
        // The original helpers consume this bitmap as a plain 16-bit stack word.
        // Keep the design-time interface as int16_t to match the binary even though callers use it as flags.
        int16_t base_size = TECHDATA::_weapons[weapon_idx].size;
        int32_t rack_size = 0;

        if (Weapon_Needs_Racks_(weapon_idx) != 0) {
            rack_size = Weapon_Rack_Size_(ammo);
        }

        int16_t dummy_out;
        int16_t special_mod_mult;
        Bonus_For_Special_Mods_(specials, &dummy_out, &special_mod_mult);

        int16_t mod_rack_offset = 0;
        if (DESIGN::Weapon_Type_(weapon_idx) == 0) {
            mod_rack_offset = Get_Weapon_Mod_Space_Bonus_(firing_arc);
        }

        int16_t regular_mod_add;
        Bonus_For_Regular_Mods_(specials, &dummy_out, &regular_mod_add);

        int16_t tech_app_id = TECHDATA::_weapons[weapon_idx].tech_app_id;
        int8_t miniaturization_cat = Miniaturization_Category_(tech_app_id);
        int16_t tech_level = Tech_Level_(player_idx, tech_app_id);

        int32_t intermediate = (special_mod_mult * (base_size + rack_size)) / 100;

        int16_t raw_space_calc = (int16_t)(((regular_mod_add + mod_rack_offset + 100) * intermediate) / 100);

        int32_t space_per_weapon = Space_Given_M_Level_(raw_space_calc, tech_level, miniaturization_cat);

        int32_t total_space = static_cast<int32_t>(quantity) * space_per_weapon;

        if (total_space <= 1) {
            total_space = 1;
        }

        return total_space;
    }

    int16_t __cdecl Weapon_Cost_(int16_t weapon_idx, int16_t count, int16_t arc, int16_t ammo, int16_t specials) {
        // The original helpers consume this bitmap as a plain 16-bit stack word.
        // Keep the design-time interface as int16_t to match the binary even though callers use it as flags.
        int32_t rack_size = 0;
        if (Weapon_Needs_Racks_(weapon_idx) != 0) {
            rack_size = static_cast<int32_t>(Weapon_Rack_Size_(ammo));
        }

        int16_t special_bonus_pct;
        int16_t cost_bonus_pct;
        Bonus_For_Special_Mods_(specials, &special_bonus_pct, &cost_bonus_pct);

        int16_t space_bonus = 0;
        if (DESIGN::Weapon_Type_(weapon_idx) == 0) {
            space_bonus = Get_Weapon_Mod_Space_Bonus_(arc);
        }

        int16_t regular_bonus_add;
        int16_t regular_bonus_space;
        Bonus_For_Regular_Mods_(specials, &regular_bonus_add, &regular_bonus_space);

        const auto& weapon = TECHDATA::_weapons[weapon_idx];

        int32_t base_scaled_cost =
            (static_cast<int32_t>(special_bonus_pct) * (static_cast<int32_t>(weapon.cost) + rack_size)) / 100;

        int32_t total_modified_cost =
            (static_cast<int32_t>(space_bonus) + regular_bonus_add + 100) * base_scaled_cost / 100;

        int16_t tech_level = Tech_Level_(MOX::_PLAYER_NUM, weapon.tech_app_id);

        return Cost_Given_M_Level_(total_modified_cost * static_cast<int32_t>(count), tech_level);
    }

    void __cdecl Draw_Design_Screen_() {
        animate::Draw_(0, 0, MOX::_design_screen_seg);
        Draw_Down_Buttons_();
        Print_Current_Design_();
        Check_For_Auto_Repeat_();
        JIM::Play_Streaming_Music_(8, -1, 1);
    }

    void __cdecl Check_For_Auto_Repeat_() {
        int16_t mouse_btn = mouse::Mouse_Button_();
        if (mouse_btn != 1) {
            _repeat_count = 0;
            return;
        }

        _repeat_count++;
        int16_t input_id = fields::Auto_Input_();
        bool updated = false;
        bool done = false;

        for (int16_t i = 0; i < 8 && !done; ++i) {
            int16_t weapon_slot = (int16_t)MOX::_first_weapon_ctr + i;
            if (MOX::_weapons_count_plus_button[i] != -1000 && input_id == MOX::_weapons_count_plus_button[i]) {
                if (MOX::_design->weapon_type[i] > 0 && _repeat_count > 4) {
                    done = true;

                    int16_t space = (int16_t)Weapon_Space_(
                        MOX::_PLAYER_NUM,
                        MOX::_design->weapon_type[i],
                        1,
                        (int16_t)MOX::_design->weapon_firing_arc[i],
                        (int16_t)MOX::_design->weapon_ammo[i],
                        MOX::_design->weapon_specials[i]
                    );

                    if (MOX::_design->space_used + space <= MOX::_design->hull_space) {
                        if (MOX::_design->weapon_count[weapon_slot] < 99) {
                            MOX::_design->weapon_count[weapon_slot]++;
                        } else {
                            MOX::_design->weapon_count[weapon_slot] = 99;
                        }
                        updated = true;
                    }
                }
            } else if (MOX::_weapons_count_minus_button[i] != -1000 && input_id == MOX::_weapons_count_minus_button[i]) {
                if (MOX::_design->weapon_type[i] > 0 && _repeat_count > 5) {
                    done = true;

                    if (MOX::_design->weapon_count[weapon_slot] > 1) {
                        MOX::_design->weapon_count[weapon_slot]--;
                    } else {
                        MOX::_design->weapon_count[weapon_slot] = 1;
                    }
                    updated = true;
                }
            }
        }

        _replacement_wait_ctr++;

        if (updated) {
            Update_Calculated_Design_Data_(MOX::_PLAYER_NUM);
        }
    }

    void __cdecl Update_Player_Design_(int16_t player_idx, int16_t design_idx) {
        Current_Design_Shield_();
        strings::Trim_(MOX::_design->name);

        if (MOX::_design->name[0] == '\0') {
            char* default_name = HACCESS::Design_Name_(MOX::_PLAYER_NUM, design_idx);
            strlcpy(MOX::_design->name, default_name, sizeof(MOX::_design->name));
        }

        s_player* plr = &MOX::_player[player_idx];
        s_ship_design* target_design = &plr->ship_designs[design_idx];

        strlcpy(target_design->name, MOX::_design->name, sizeof(target_design->name));

        target_design->size = (int8_t)MOX::_design->ship_size;
        target_design->shield_type = (int8_t)MOX::_design->shield;
        target_design->ftl_type = (int8_t)MOX::_design->ftl_type;
        target_design->computer_type = (int8_t)MOX::_design->computer_type;
        target_design->armor_type = (int8_t)MOX::_design->armor_type;

        for (int i = 0; i < 5; ++i) {
            target_design->special_device_flags[i] = 0;
        }

        for (int i = 0; i < 8; ++i) {
            if (MOX::_design->special_devices[i] > 0) {
                struct_::Set_Bit_Field_(target_design->special_device_flags, (uint16_t)MOX::_design->special_devices[i]);
            }
        }

        for (int i = 0; i < 8; ++i) {
            target_design->ship_weapon[i].type = MOX::_design->weapon_type[i];
            target_design->ship_weapon[i].count = (int8_t)MOX::_design->weapon_count[i];
            target_design->ship_weapon[i].current_count = (int8_t)MOX::_design->weapon_count[i];
            target_design->ship_weapon[i].firing_arc = MOX::_design->weapon_firing_arc[i];
            target_design->ship_weapon[i].specials = MOX::_design->weapon_specials[i];
            target_design->ship_weapon[i].ammo = MOX::_design->weapon_ammo[i];
        }

        target_design->picture_num = (int8_t)MOX::_design->picture_type;
        target_design->cost = (int16_t)MOX::_design->total_cost;
        target_design->date_of_design = (uint16_t)MOX::_stardate;

        _use_augmented_engines = 0;
        target_design->combat_speed = Current_Design_Combat_Speed_(MOX::_design);
        target_design->speed = (int8_t)INITSHIP::Get_FTL_Speed_(player_idx, MOX::_design->ftl_type);
    }

    /**
     * Opens the shield or computer replacement picker for the clicked field when upgrades are available.
     *
     * @param clicked_field Field identifier that was activated.
     * @param refresh_needed Set when the design screen should recalculate and redraw.
     * @param out_status Set to 99 when a replacement sub-screen takes over.
     */
    void __cdecl Check_Generic_Replacement_Fields_(int16_t clicked_field, int16_t* refresh_needed, int16_t* out_status) {
        if (_design_computer_replacement_field == clicked_field) {
            if (N_Replacements_For_Generic_Item_(TECH_APPLICATION_TYPE_COMPUTER) == 0) {
                const char* msg = HAROLD::H_Message_(HESTR_050_YOU_MAY_NOT_UPGRADE_TH);
                GENDRAW::Warning_Box_(msg);
            } else {
                MOX::_design_screen_replacement_item = MOX::_design->computer_type;
                MOX::_design_screen_replacement_type = TECH_APPLICATION_TYPE_COMPUTER;
                _printed_cost -= MOX::_design->computer_cost;
                DESBOX::Generic_Replacement_Box_();
                MOX::_design->show_help = 1;
                MOX::_current_screen = SCREEN_DESIGN;
                *refresh_needed = 1;
                *out_status = 99;
            }
        }

        if (_design_shield_replacement_field == clicked_field) {
            if (Has_Damper_Field_()) {
                const char* msg = HAROLD::H_Message_(HESTR_051_YOU_MAY_NOT_PLACE_SHIE);
                GENDRAW::Warning_Box_(msg);
                return;
            }

            if (N_Replacements_For_Generic_Item_(TECH_APPLICATION_TYPE_SHIELD) != 0) {
                MOX::_design_screen_replacement_item = MOX::_design->shield;
                MOX::_design_screen_replacement_type = TECH_APPLICATION_TYPE_SHIELD;
                _printed_cost -= MOX::_design->shield_cost;
                _printed_space_avail += MOX::_design->shield_space;
                DESBOX::Generic_Replacement_Box_();
                MOX::_design->show_help = 1;
                MOX::_current_screen = SCREEN_DESIGN;
                *refresh_needed = 1;
                *out_status = 99;
            } else {
                const char* msg = HAROLD::H_Message_(HESTR_052_YOU_MAY_NOT_UPGRADE_TH);
                GENDRAW::Warning_Box_(msg);
            }
        }
    }

    /**
     * Opens the weapon replacement picker or weapon help text for the clicked weapon row.
     *
     * @param clicked_field Field identifier that was activated.
     * @param out_status Set to 99 when a replacement sub-screen takes over.
     */
    void __cdecl Check_Weapon_Fields_(int16_t clicked_field, int16_t* /* refresh */, int16_t* out_status) {
        char description_buffer[1332];

        for (int16_t i = 0; i < 8; ++i) {
            int16_t weapon_field_id = MOX::_weapons_field[i];
            int16_t design_weapon_index = (int16_t)MOX::_first_weapon_ctr + i;

            if (MOX::_scanned_field == weapon_field_id) {
                MOX::_scanned_field = i + 1000;
            }

            if (clicked_field == weapon_field_id) {
                fields::Clear_Mouse_Buffer_();
                fields::Set_Input_Delay_(20);

                if (MOX::_design->weapon_type[i] == 0) {
                    for (int16_t j = 0; j < 8; ++j) {
                        if (MOX::_design->weapon_type[j] == 0) {
                            design_weapon_index = j;
                            break;
                        }
                    }
                }

                MOX::_design_screen_replacement_type = TECH_APPLICATION_TYPE_WEAPON;
                _printed_space_avail += MOX::_design->weapon_space[design_weapon_index];
                _printed_cost -= MOX::_design->weapon_cost[design_weapon_index];
                MOX::_design_screen_replacement_slot = design_weapon_index;

                fields::Deactivate_Help_List_();
                DESBOX::Weapons_Replacement_Box_();

                MOX::_design->show_help = 1;
                MOX::_current_screen = SCREEN_DESIGN;
                *out_status = 99;
                continue;
            } else if (clicked_field + weapon_field_id == 0) {
                int16_t weapon_type = MOX::_design->weapon_type[i];
                if (weapon_type > 0) {
                    EVANHELP::Get_Application_Description_(description_buffer, sizeof(description_buffer), TECHDATA::_weapons[weapon_type].tech_app_id);
                    TEXTBOX::Text_Box_(TECHDATA::_weapons[weapon_type].name, description_buffer);
                }
            }
        }
    }

    /**
     * Opens the special-system replacement picker or special-system help text for the clicked row.
     *
     * @param clicked_field_id Field identifier that was activated.
     * @param out_status Set to 99 when a replacement sub-screen takes over.
     */
    void __cdecl Check_Special_System_Fields_(int16_t clicked_field_id, int16_t* /* unused */, int16_t* out_status) {
        char description_buffer[1332];

        for (int16_t i = 0; i < 8; ++i) {
            int16_t slot_idx = (int16_t)MOX::_first_special_system_ctr + i;
            int16_t field_id = MOX::_special_system_field[i];

            if (MOX::_scanned_field == field_id) {
                MOX::_scanned_field = i + 2000;
            }

            if (field_id == clicked_field_id) {
                if (MOX::_design->special_devices[slot_idx] == 0) {
                    for (int16_t search_idx = 0; search_idx < 8; ++search_idx) {
                        if (MOX::_design->special_devices[search_idx] == 0) {
                            slot_idx = search_idx;
                            break;
                        }
                    }
                }

                MOX::_design_screen_replacement_type = TECH_APPLICATION_TYPE_SPECIAL;
                _printed_space_avail += MOX::_design->special_device_space[slot_idx];
                _printed_cost -= MOX::_design->special_device_cost[slot_idx];
                MOX::_design_screen_replacement_slot = slot_idx;

                fields::Deactivate_Help_List_();
                DESBOX::Special_Systems_Box_();

                MOX::_current_screen = SCREEN_DESIGN;
                MOX::_design->show_help = 1;
                *out_status = 99;
            } else if ((field_id + clicked_field_id == 0)) {
                int16_t special_id = MOX::_design->special_devices[i];
                if (special_id > 0) {
                    EVANHELP::Get_Application_Description_(description_buffer, sizeof(description_buffer), TECHDATA::_specials[special_id].tech_app_id);

                    const char* title = TECHDATA::_specials[special_id].name;
                    TEXTBOX::Text_Box_(title, description_buffer);
                }
            }
        }
    }

    void __cdecl Check_Plus_Minus_Buttons_(int16_t field) {
        uint8_t player_race = MOX::_player[MOX::_PLAYER_NUM].race;
        int16_t icon_index = 0;

        if (MOX::_ship_icon_lf_button == static_cast<uint16_t>(field)) {
            int16_t ship_size = MOX::_design->ship_size;
            if (ship_size != SHIP_SIZE_DOOM_STAR) {
                MOX::_design->picture_type--;
                if (MOX::_design->picture_type < ship_size * 8) {
                    MOX::_design->picture_type = ship_size * 8 + 7;
                }
                icon_index = MOX::_design->picture_type % 8;
            }

            if (_changed_design_name == 0) {
                Get_Ship_Design_Name_(player_race, ship_size, icon_index, MOX::_design->name, sizeof(MOX::_design->name));
            }
        } else if (MOX::_ship_icon_rt_button == static_cast<uint16_t>(field)) {
            int16_t ship_size = MOX::_design->ship_size;
            if (ship_size != SHIP_SIZE_DOOM_STAR) {
                MOX::_design->picture_type++;
                if (MOX::_design->picture_type > (ship_size * 8 + 7)) {
                    MOX::_design->picture_type = ship_size * 8;
                }
                icon_index = MOX::_design->picture_type % 8;
            }

            if (_changed_design_name == 0) {
                Get_Ship_Design_Name_(player_race, ship_size, icon_index, MOX::_design->name, sizeof(MOX::_design->name));
            }
        }
    }

    void __cdecl Clear_Design_() {
        int16_t player_num = MOX::_PLAYER_NUM;
        s_current_design* design = MOX::_design;

        design->ftl_type = INITSHIP::Best_Warp_Drive_(player_num);
        design->drive_speed = TECHDATA::_drives[design->ftl_type].warp_speed;

        design->computer_type = INITSHIP::Best_Computer_(player_num);
        design->armor_type = INITSHIP::Best_Armor_(player_num);
        design->fuel_type = INITSHIP::Best_Ship_Fuel_(player_num);

        for (int i = 0; i < 8; ++i) {
            design->weapon_type[i] = 0;
            design->weapon_count[i] = 0;
            design->weapon_firing_arc[i] = WEAPON_FIRING_ARC_NONE;
            design->weapon_specials[i] = 0;
            design->weapon_ammo[i] = 0;
            design->weapon_cost[i] = 0;
            design->weapon_space[i] = 0;
        }

        for (int i = 0; i < 8; ++i) {
            design->special_devices[i] = 0;
            design->special_device_cost[i] = 0;
            design->special_device_space[i] = 0;
        }

        Update_Calculated_Design_Data_(player_num);
    }

    void __cdecl Get_Tech_Description_Lbx_Name_(char* buffer, int32_t buffer_size) {
        if (buffer_size <= 0) {
            return;
        }

        const char* src;

        switch (MOX::_settings.language) {
            case 1: // German
                src = "GERTECD.LBX";
                break;
            case 2: // French
                src = "FRETECD.LBX";
                break;
            case 3: // Spanish
                src = "SPATECD.LBX";
                break;
            case 4: // Italian
                src = "ITATECD.LBX";
                break;
            default: // English or other
                src = "TECHDESC.LBX";
                break;
        }

        strlcpy(buffer, src, (size_t)buffer_size);
    }

    int16_t __cdecl Printed_Design_Cost_() {
        if (MOX::_refit_ship != 0) {
            s_ship_design new_design;
            Build_Design_Template_(&new_design);

            s_player* player = &MOX::_player[MOX::_PLAYER_NUM];
            s_ship_design* old_design = &player->ship_designs[MOX::_temp_star_handle];

            return AIBUILD::Refit_Cost_(MOX::_PLAYER_NUM, old_design, &new_design);
        }

        return static_cast<int16_t>(_printed_cost);
    }

    void __cdecl Build_Design_Template_(s_ship_design* template_out) {
        s_current_design* d = MOX::_design;

        strlcpy(template_out->name, d->name, sizeof(template_out->name));

        template_out->size = (int8_t)d->ship_size;
        template_out->shield_type = SHIELD_NO_SHIELD;
        template_out->ftl_type = (int8_t)d->ftl_type;
        template_out->computer_type = (int8_t)d->computer_type;
        template_out->armor_type = (int8_t)d->armor_type;

        for (int i = 0; i < 5; ++i) {
            template_out->special_device_flags[i] = 0;
        }

        for (int i = 0; i < 8; ++i) {
            if (d->special_devices[i] > 0) {
                struct_::Set_Bit_Field_(template_out->special_device_flags, (uint16_t)d->special_devices[i]);
            }
        }

        for (int i = 0; i < 8; ++i) {
            template_out->ship_weapon[i].type = d->weapon_type[i];
            template_out->ship_weapon[i].count = (int8_t)d->weapon_count[i];
            template_out->ship_weapon[i].firing_arc = d->weapon_firing_arc[i];
            template_out->ship_weapon[i].specials = d->weapon_specials[i];
            template_out->ship_weapon[i].ammo = d->weapon_ammo[i];
        }

        template_out->picture_num = (int8_t)d->picture_type;
        template_out->cost = (int16_t)d->total_cost;
        template_out->date_of_design = (uint16_t)MOX::_stardate;
        template_out->combat_speed = Current_Design_Combat_Speed_(d);
    }

    void __cdecl Get_Ship_Size_Button_Field_XY_(int16_t size_idx, int16_t* x1, int16_t* y1, int16_t* x2, int16_t* y2) {
        if (size_idx >= 0 && size_idx <= SHIP_SIZE_DOOM_STAR) {
            *x1 = 118;
            *x2 = 227;

            switch (size_idx) {
                case SHIP_SIZE_SMALL:
                    *y1 = 54;
                    *y2 = 69;
                    break;
                case SHIP_SIZE_MEDIUM:
                    *y1 = 70;
                    *y2 = 84;
                    break;
                case SHIP_SIZE_LARGE:
                    *y1 = 85;
                    *y2 = 102;
                    break;
                case SHIP_SIZE_HUGE:
                    *y1 = 103;
                    *y2 = 117;
                    break;
                case SHIP_SIZE_TITAN:
                    *y1 = 118;
                    *y2 = 132;
                    break;
                case SHIP_SIZE_DOOM_STAR:
                    *y1 = 133;
                    *y2 = 149;
                    break;
                default:
                    break;
            }
        } else {
            *x1 = 0;
            *y1 = 0;
            *x2 = 0;
            *y2 = 0;
        }
    }

    void __cdecl Get_Ship_Design_Name_(int16_t race_idx, int16_t design_idx, int16_t variant_idx, char* out_name, int32_t out_name_size) {
        if (out_name_size <= 0) {
            return;
        }

        allocate::Push_Block_(MOX::_global_data_seg);

        // shipname.lbx entry: variant_idx + ((race_idx * 6 + design_idx) * 8)
        // original code performs this through 16-bit arithmetic before sign-extension
        int16_t entry_idx = (int16_t)(variant_idx + (int16_t)((int16_t)(race_idx * SHIP_SIZE_COUNT) + design_idx) * 8);

        char* loaded_name = (char*)farload::Far_Reload_Next_Data_(
            "shipname.lbx",
            0,
            MOX::_global_data_seg,
            (int32_t)entry_idx,
            1,
            sizeof(MOX::_design->name)
        );

        allocate::Pop_Block_(MOX::_global_data_seg);

        strlcpy(out_name, loaded_name, (size_t)out_name_size);
    }

    void __cdecl Set_Flashing_Colors_(int16_t font_style, int16_t spacing) {
        s_colors local_colors;
        fonts::Get_Colors_(&local_colors);

        for (int i = 1; i < 8; ++i) {
            int adjusted_color = (int)local_colors.colors[i] + (int)MOX::_multi_colored_line_start;
            if (adjusted_color <= (int)local_colors.colors[0]) {
                adjusted_color = (int)local_colors.colors[0];
            }
            local_colors.colors[i] = (uint8_t)adjusted_color;
        }

        fonts::Set_Font_Style_(font_style, &local_colors);
        fonts::Set_Font_Spacing_(spacing);

        int8_t incr_value = DESBOX::_incr_value;
        int16_t next_color_start = (int16_t)(MOX::_multi_colored_line_start + incr_value);
        MOX::_multi_colored_line_start = next_color_start;

        if (next_color_start >= 3) {
            DESBOX::_incr_value = (int8_t)-incr_value;
            MOX::_multi_colored_line_start = 3;
            return;
        }

        MOX::_multi_colored_line_start = next_color_start;
        if (next_color_start <= -2) {
            MOX::_multi_colored_line_start = -2;
            DESBOX::_incr_value = (int8_t)-incr_value;
        }
    }

    bool __cdecl Space_For_Weapon_Replacement_() {
        bool space_found = false;
        int16_t app_idx = 0;

        do {
            if (space_found) {
                return true;
            }

            if (MOX::_player[MOX::_PLAYER_NUM].tech_applications[app_idx] == TECH_RESEARCH_STATUS_RESEARCHED) {
                if ((int16_t)DESBOX::Application_Type_(app_idx) == TECH_APPLICATION_TYPE_WEAPON) {
                    int16_t tech_id = DESIGN::Tech_App_To_Tech_Id_(app_idx, TECH_APPLICATION_TYPE_WEAPON);
                    uint32_t space_req = DESIGN::One_Weapon_Space_(tech_id);

                    if ((int16_t)space_req <= DESIGN::_printed_space_avail) {
                        space_found = true;
                    }
                }
            }
            app_idx++;
        } while (app_idx < TECH_APP_COUNT);

        return space_found;
    }

    int16_t __cdecl One_Weapon_Space_(int16_t weapon_id) {
        int16_t needs_racks = Weapon_Needs_Racks_(weapon_id);

        int16_t tech_id = TECHDATA::_weapons[weapon_id].tech_app_id;
        int8_t category = Miniaturization_Category_(tech_id);
        int16_t tech_lvl = Tech_Level_(MOX::_PLAYER_NUM, tech_id);

        int16_t base_weapon_size = TECHDATA::_weapons[weapon_id].size;
        int16_t space_req = static_cast<int16_t>(Space_Given_M_Level_(base_weapon_size, tech_lvl, category));

        if (needs_racks != 0) {
            tech_id = TECHDATA::_weapons[weapon_id].tech_app_id;
            int8_t rack_category = Miniaturization_Category_(tech_id);
            int16_t rack_tech_lvl = Tech_Level_(MOX::_PLAYER_NUM, tech_id);
            int16_t rack_base_size = Weapon_Rack_Size_(2);
            int16_t rack_space = (int16_t)Space_Given_M_Level_(rack_base_size, rack_tech_lvl, rack_category);
            return space_req + rack_space;
        }

        return space_req;
    }

    void __cdecl Draw_Down_Buttons_() {
        int16_t y = 169;
        int8_t first_weapon_idx = (int8_t)MOX::_first_weapon_ctr;

        for (int i = 0; i < 8; ++i) {
            if (MOX::_weapons_count_minus_button[i] == -1000) {
                if (MOX::_design->weapon_type[first_weapon_idx + i] > 0) {
                    animate::Draw_(55, y, _dull_minus_button_seg);
                }
            }
            if (MOX::_weapons_count_plus_button[i] == -1000) {
                if (MOX::_design->weapon_type[first_weapon_idx + i] > 0) {
                    animate::Draw_(19, y, _dull_plus_button_seg);
                }
            }
            y += 13;
        }

        if (MOX::_design_build_button == -1000) {
            animate::Draw_(544, 439, _dark_build_design_button_seg);
        }

        for (int size_idx = 0; size_idx < SHIP_SIZE_COUNT; ++size_idx) {
            int16_t btn_x, btn_y, btn_w, btn_h;
            if (MOX::_refit_ship == 0) {
                if (_add_size_button_flag[size_idx] != 1) {
                    Get_Ship_Size_Button_Field_XY_(size_idx, &btn_x, &btn_y, &btn_w, &btn_h);
                    animate::Reset_Animation_Frame_(MOX::_darkened_size_button_seg[size_idx]);
                    animate::Draw_(btn_x, btn_y, MOX::_darkened_size_button_seg[size_idx]);
                }
            } else {
                if (MOX::_design->ship_size == size_idx) {
                    Get_Ship_Size_Button_Field_XY_(size_idx, &btn_x, &btn_y, &btn_w, &btn_h);
                    animate::Reset_Animation_Frame_(MOX::_darkened_size_button_seg[size_idx]);
                    animate::Draw_(btn_x, btn_y, MOX::_darkened_size_button_seg[size_idx]);
                    return;
                }
            }
        }
    }

    void __cdecl Print_Ship_Size_Error_Message_(int16_t ship_size_index) {
        bool is_formatted = false;
        int16_t message_id = -1;
        char planet_name_buf[80];
        char formatted_msg[100];
        char lbx_msg[200];

        planet_name_buf[0] = '\0';

        switch (DESIGN::_add_size_button_flag[ship_size_index]) {
            case 0xF1:
                message_id = ship_size_index - 3;
                break;
            case 0xF2:
                message_id = ship_size_index + 1;
                break;
            case 0xF3:
                message_id = ship_size_index + 7;
                break;
            case 0xF4:
                message_id = HESTR_00D_YOU_MAY_NOT_CHANGE_HUL;
                break;
            case 0xF5:
                message_id = HESTR_00E_SINCE_S_CANNOT_BUILD_S;
                if (MOX::_return_screen == 0x19) {
                    int16_t colony_idx = DESIGN::Get_Colony_Handle_();
                    char* planet_name = HACCESS::Get_Planet_Name_(MOX::_colony[colony_idx].planet);
                    is_formatted = true;
                    strlcpy(planet_name_buf, planet_name, sizeof(planet_name_buf));
                }
                break;
            default:
                break;
        }

        if (message_id >= 0) {
            MISC::Get_LBX_Message_(message_id, lbx_msg);
            if (is_formatted) {
                const char* hull_name = TECHDATA::_hull_data[MOX::_design->ship_size].name;
                snprintf(formatted_msg, sizeof(formatted_msg), lbx_msg, planet_name_buf, hull_name);
                GENDRAW::Message_Box_(formatted_msg);
            } else {
                GENDRAW::Message_Box_(lbx_msg);
            }
        }
    }

    bool __cdecl Space_For_System_Replacement_() {
        int16_t found_count = 0;

        for (int16_t app_idx = 0; app_idx < TECH_APP_COUNT; ++app_idx) {
            if (found_count != 0) break;

            if (MOX::_player[MOX::_PLAYER_NUM].tech_applications[app_idx] == TECH_RESEARCH_STATUS_RESEARCHED) {
                if ((int16_t)DESBOX::Application_Type_(app_idx) == TECH_APPLICATION_TYPE_SPECIAL) {
                    bool device_fits = false;

                    for (int16_t special_idx = 1; special_idx < 39; ++special_idx) {
                        if (device_fits) break;

                        if (TECHDATA::_technology_applications[app_idx].tech_app_id == TECHDATA::_specials[special_idx].tech_app_id) {
                            bool already_on_ship = false;
                            for (int i = 0; i < 8; ++i) {
                                if (MOX::_design->special_devices[i] == special_idx) {
                                    already_on_ship = true;
                                    break;
                                }
                            }

                            if (!already_on_ship) {
                                uint16_t tech_id = TECHDATA::_specials[special_idx].tech_app_id;
                                int8_t category = Miniaturization_Category_(tech_id);
                                int16_t tech_lvl = Tech_Level_(MOX::_PLAYER_NUM, tech_id);

                                int16_t base_space = TECHDATA::_specials[special_idx].space_per_ship_type[MOX::_design->ship_size];
                                int32_t required_space = Space_Given_M_Level_(base_space, tech_lvl, category);

                                if (MOX::_design->space_used + required_space <= MOX::_design->hull_space) {
                                    device_fits = true;
                                    found_count++;
                                }
                            }
                        }
                    }
                }
            }
        }

        return found_count > 0;
    }

    int16_t __cdecl Tech_Level_(int16_t player_idx, int16_t tech_idx) {
        if (player_idx > -1 && player_idx < MOX::_NUM_PLAYERS) {
            if (tech_idx < 0 || tech_idx >= TECH_APP_COUNT) {
                RUSS::PFMT_(0, 2, "Bad required_tech in Tech_Level()... %d", (int32_t)tech_idx);
                key::Wait_For_Key_();
            }

            if (tech_idx != TECH_APP_NO_TECH && MOX::_player[player_idx].tech_applications[tech_idx] != TECH_RESEARCH_STATUS_AVAILABLE) {
                return Miniaturization_Level_(player_idx, tech_idx);
            }
        }

        return 0;
    }

    int16_t __cdecl Tech_App_To_Tech_Id_(int16_t app_id, int16_t category) {
        int16_t result_id = -1;

        if (category == TECH_APPLICATION_TYPE_WEAPON) {
            for (int16_t i = 0; i < WEAPON_DRAGON_BREATH; ++i) {
                if (TECHDATA::_weapons[i].tech_app_id == app_id) {
                    result_id = i;
                    break;
                }
            }
        } else if (category == TECH_APPLICATION_TYPE_SPECIAL) {
            for (int16_t i = 0; i < 39; ++i) {
                if (TECHDATA::_specials[i].tech_app_id == app_id) {
                    result_id = i;
                    break;
                }
            }
        }

        return result_id;
    }

    void __cdecl Weapon_Arc_String_(uint16_t arc_flags, char* dest_str, int32_t dest_str_size) {
        if (dest_str_size <= 0) {
            return;
        }

        const size_t destination_size = (size_t)dest_str_size;
        char buffer[8];
        dest_str[0] = '\0';

        if (arc_flags & WEAPON_FIRING_ARC_FORWARD) {
            KEN::Ken_Get_Text_Message_(3, buffer, 6);
            strlcat(dest_str, buffer, destination_size);
            return;
        }

        if (arc_flags & WEAPON_FIRING_ARC_FORWARD_EXTENDED) {
            KEN::Ken_Get_Text_Message_(4, buffer, 6);
            strlcat(dest_str, buffer, destination_size);
            return;
        }

        if (arc_flags & WEAPON_FIRING_ARC_BACK_EXTENDED) {
            KEN::Ken_Get_Text_Message_(6, buffer, 6);
            strlcat(dest_str, buffer, destination_size);
            return;
        }

        if (arc_flags & WEAPON_FIRING_ARC_REAR) {
            KEN::Ken_Get_Text_Message_(5, buffer, 6);
            strlcat(dest_str, buffer, destination_size);
            return;
        }

        if (arc_flags & WEAPON_FIRING_ARC_360) {
            strlcpy(dest_str, "360", destination_size);
        }
    }

    bool __cdecl Replacement_Exists_(int16_t application_type) {
        int16_t replacement_count =
            (application_type == TECH_APPLICATION_TYPE_SHIELD || application_type == TECH_APPLICATION_TYPE_DRIVE) ? 1 : 0;

        s_player* player = &MOX::_player[MOX::_PLAYER_NUM];

        for (int16_t i = 0; i < TECH_APP_COUNT; ++i) {
            if (player->tech_applications[i] == TECH_RESEARCH_STATUS_RESEARCHED) {
                if ((int16_t)DESBOX::Application_Type_(i) == application_type) {
                    replacement_count++;
                }
            }
        }

        return replacement_count > 0;
    }

    int16_t __cdecl N_Replacements_For_Generic_Item_(int16_t item_type) {
        if (item_type == TECH_APPLICATION_TYPE_SHIELD) {
            return INITSHIP::Best_Ship_Shield_(MOX::_PLAYER_NUM);
        }

        if (item_type == TECH_APPLICATION_TYPE_COMPUTER) {
            return INITSHIP::Best_Computer_(MOX::_PLAYER_NUM);
        }

        return 0;
    }

    int8_t __cdecl Add_Size_Button_() {
        if (MOX::_refit_ship != 0) {
            return (int8_t)0xF4;
        }
        return 1;
    }

    int16_t __cdecl Shield_Cost_(int16_t shield_type) {
        int16_t cost = TECHDATA::_shields[shield_type].cost_per_ship_type[MOX::_design->ship_size];

        if (shield_type > 0 && cost == 0) {
            return 1;
        }

        return cost;
    }

    int16_t __cdecl Cost_Given_M_Level_(int16_t base_cost, int16_t m_level) {
        int16_t multiplier;

        switch (m_level) {
            case 0:
                multiplier = 100;
                break;
            case 1:
                multiplier = 75;
                break;
            case 2:
                multiplier = 55;
                break;
            case 3:
                multiplier = 40;
                break;
            case 4:
                multiplier = 30;
                break;
            default:
                multiplier = 25;
                break;
        }

        int32_t result = ((int32_t)multiplier * (int32_t)base_cost) / 100;

        if ((int16_t)result == 0 && base_cost > 0) {
            result = 1;
        }

        return (int16_t)result;
    }

    void __cdecl Weapon_Mod_String_(int16_t mod_mask, char* out_buffer, int32_t out_buffer_size) {
        if (out_buffer_size <= 0) {
            return;
        }

        const size_t destination_size = (size_t)out_buffer_size;
        bool any_mod = false;
        out_buffer[0] = '\0';

        for (int16_t i = 0; i < WEAPON_MOD_COUNT; ++i) {
            if ((mod_mask >> (i & 0x1f)) & 1) {
                const char* mod_name = TECHDATA::_weapon_modifications[i].name_plural;

                strlcat(out_buffer, mod_name, destination_size);
                strlcat(out_buffer, s_comma_space, destination_size);

                any_mod = true;
            }
        }

        if (!any_mod) {
            const char* no_mods = HAROLD::H_Message_(HESTR_064_NO_MODIFICATIONS);
            strlcpy(out_buffer, no_mods, destination_size);
        }

        size_t len = strlen(out_buffer);
        if (len > 2 && out_buffer[len - 2] == ',' && out_buffer[len - 1] == ' ') {
            out_buffer[len - 2] = '\0';
        }
    }

    int32_t __cdecl Space_Given_M_Level_(int16_t amount, int16_t miniaturization_level, int8_t mode) {
        int multiplier;

        if (mode == 0) {
            switch (miniaturization_level) {
                case 0:
                    multiplier = 1000;
                    break;
                case 1:
                    multiplier = 800;
                    break;
                case 2:
                    multiplier = 650;
                    break;
                case 3:
                    multiplier = 500;
                    break;
                case 4:
                    multiplier = 350;
                    break;
                default:
                    multiplier = 250;
                    break;
            }
        } else if (mode == 1) {
            switch (miniaturization_level) {
                case 0:
                    multiplier = 1000;
                    break;
                case 1:
                    multiplier = 800;
                    break;
                case 2:
                    multiplier = 700;
                    break;
                case 3:
                    multiplier = 600;
                    break;
                case 4:
                    multiplier = 500;
                    break;
                default:
                    multiplier = 400;
                    break;
            }
        } else {
            multiplier = 1000;
        }

        int result = (static_cast<int>(amount) * multiplier + 500) / 1000;

        if (result == 0 && amount > 0) {
            result = 1;
        }

        return result;
    }

    int8_t __cdecl Miniaturization_Category_(int16_t tech_app) {
        auto& app = TECHDATA::_technology_applications[tech_app];

        if (app.type == TECH_APPLICATION_TYPE_WEAPON) {
            if (tech_app == TECHDATA::_weapons[WEAPON_INTERCEPTOR].tech_app_id ||
                tech_app == TECHDATA::_weapons[WEAPON_BOMBER].tech_app_id ||
                tech_app == TECHDATA::_weapons[WEAPON_ASSAULT_SHUTTLE].tech_app_id ||
                tech_app == TECHDATA::_weapons[WEAPON_HEAVY_FIGHTER].tech_app_id) {
                return 2;
            }

            int16_t weapon_idx = HACCESS::Weapons_Index_From_Tech_App_(tech_app);
            int8_t weapon_type_class = TECHDATA::_weapons[weapon_idx].type;

            if (weapon_type_class == WEAPON_TYPE_BOMB || weapon_type_class == WEAPON_TYPE_MISSILE || weapon_type_class == WEAPON_TYPE_BEAM) {
                return 0;
            }
        } else if (app.type == TECH_APPLICATION_TYPE_SPECIAL) {
            if (tech_app == TECHDATA::_specials[SPECIAL_AUGMENTED_ENGINES].tech_app_id ||
                tech_app == TECHDATA::_specials[SPECIAL_REINFORCED_HULL].tech_app_id ||
                tech_app == TECHDATA::_specials[SPECIAL_EXTENDED_FUEL_TANKS].tech_app_id ||
                tech_app == TECHDATA::_specials[SPECIAL_HEAVY_ARMOR].tech_app_id ||
                tech_app == TECHDATA::_specials[SPECIAL_TROOP_PODS].tech_app_id) {
                return 2;
            }
            return 1;
        }

        return 1;
    }

    void __cdecl Bonus_For_Special_Mods_(int16_t flags, int16_t *bonus1_out, int16_t *bonus2_out) {
        *bonus1_out = 100;
        *bonus2_out = 100;

        if ((flags & WEAPON_MOD_FLAG_HEAVY_MOUNT) != 0) {
            int16_t index = MISC::Bit_Value_To_Num_(WEAPON_MOD_FLAG_HEAVY_MOUNT, WEAPON_MOD_COUNT);
            *bonus1_out += TECHDATA::_weapon_modifications[index].cost;
            *bonus2_out += TECHDATA::_weapon_modifications[index].space;
            return;
        }

        if ((flags & WEAPON_MOD_FLAG_POINT_DEFENSE) != 0) {
            int16_t index = MISC::Bit_Value_To_Num_(WEAPON_MOD_FLAG_POINT_DEFENSE, WEAPON_MOD_COUNT);
            *bonus1_out += TECHDATA::_weapon_modifications[index].cost;
            *bonus2_out += TECHDATA::_weapon_modifications[index].space;
        }
    }

    int16_t __cdecl Get_Weapon_Mod_Space_Bonus_(int16_t flags) {
        if ((flags & (WEAPON_FIRING_ARC_FORWARD_EXTENDED | WEAPON_FIRING_ARC_BACK_EXTENDED)) != 0) {
            return 25;
        }

        if ((flags & WEAPON_FIRING_ARC_360) != 0) {
            return 50;
        }

        return 0;
    }

    void __cdecl Bonus_For_Regular_Mods_(int16_t mods, int16_t* bonus1, int16_t* bonus2) {
        *bonus1 = 0;
        *bonus2 = 0;

        if (mods & WEAPON_MOD_FLAG_OVERLOADED) {
            int16_t idx = MISC::Bit_Value_To_Num_(WEAPON_MOD_FLAG_OVERLOADED, WEAPON_MOD_COUNT);
            *bonus1 += TECHDATA::_weapon_modifications[idx].cost;
            *bonus2 += TECHDATA::_weapon_modifications[idx].space;
        }

        if (mods & WEAPON_MOD_FLAG_ARMOR_PIERCING) {
            int16_t idx = MISC::Bit_Value_To_Num_(WEAPON_MOD_FLAG_ARMOR_PIERCING, WEAPON_MOD_COUNT);
            *bonus1 += TECHDATA::_weapon_modifications[idx].cost;
            *bonus2 += TECHDATA::_weapon_modifications[idx].space;
        }

        if (mods & WEAPON_MOD_FLAG_CONTINUOUS) {
            int16_t idx = MISC::Bit_Value_To_Num_(WEAPON_MOD_FLAG_CONTINUOUS, WEAPON_MOD_COUNT);
            *bonus1 += TECHDATA::_weapon_modifications[idx].cost;
            *bonus2 += TECHDATA::_weapon_modifications[idx].space;
        }

        if (mods & WEAPON_MOD_FLAG_NO_RANGE_DISSIPATION) {
            int16_t idx = MISC::Bit_Value_To_Num_(WEAPON_MOD_FLAG_NO_RANGE_DISSIPATION, WEAPON_MOD_COUNT);
            *bonus1 += TECHDATA::_weapon_modifications[idx].cost;
            *bonus2 += TECHDATA::_weapon_modifications[idx].space;
        }

        if (mods & WEAPON_MOD_FLAG_SHIELD_PIERCING) {
            int16_t idx = MISC::Bit_Value_To_Num_(WEAPON_MOD_FLAG_SHIELD_PIERCING, WEAPON_MOD_COUNT);
            *bonus1 += TECHDATA::_weapon_modifications[idx].cost;
            *bonus2 += TECHDATA::_weapon_modifications[idx].space;
        }

        if (mods & WEAPON_MOD_FLAG_AUTO_FIRE) {
            int16_t idx = MISC::Bit_Value_To_Num_(WEAPON_MOD_FLAG_AUTO_FIRE, WEAPON_MOD_COUNT);
            *bonus1 += TECHDATA::_weapon_modifications[idx].cost;
            *bonus2 += TECHDATA::_weapon_modifications[idx].space;
        }

        if (mods & WEAPON_MOD_FLAG_ENVELOPING) {
            int16_t idx = MISC::Bit_Value_To_Num_(WEAPON_MOD_FLAG_ENVELOPING, WEAPON_MOD_COUNT);
            *bonus1 += TECHDATA::_weapon_modifications[idx].cost;
            *bonus2 += TECHDATA::_weapon_modifications[idx].space;
        }

        if (mods & WEAPON_MOD_FLAG_MIRV) {
            int16_t idx = MISC::Bit_Value_To_Num_(WEAPON_MOD_FLAG_MIRV, WEAPON_MOD_COUNT);
            *bonus1 += TECHDATA::_weapon_modifications[idx].cost;
            *bonus2 += TECHDATA::_weapon_modifications[idx].space;
        }

        if (mods & WEAPON_MOD_FLAG_ECCM) {
            int16_t idx = MISC::Bit_Value_To_Num_(WEAPON_MOD_FLAG_ECCM, WEAPON_MOD_COUNT);
            *bonus1 += TECHDATA::_weapon_modifications[idx].cost;
            *bonus2 += TECHDATA::_weapon_modifications[idx].space;
        }

        if (mods & WEAPON_MOD_FLAG_HEAVILY_ARMORED) {
            int16_t idx = MISC::Bit_Value_To_Num_(WEAPON_MOD_FLAG_HEAVILY_ARMORED, WEAPON_MOD_COUNT);
            *bonus1 += TECHDATA::_weapon_modifications[idx].cost;
            *bonus2 += TECHDATA::_weapon_modifications[idx].space;
        }

        if (mods & WEAPON_MOD_FLAG_FAST) {
            int16_t idx = MISC::Bit_Value_To_Num_(WEAPON_MOD_FLAG_FAST, WEAPON_MOD_COUNT);
            *bonus1 += TECHDATA::_weapon_modifications[idx].cost;
            *bonus2 += TECHDATA::_weapon_modifications[idx].space;
        }

        if (mods & WEAPON_MOD_FLAG_EMISSION_GUIDANCE) {
            int16_t idx = MISC::Bit_Value_To_Num_(WEAPON_MOD_FLAG_EMISSION_GUIDANCE, WEAPON_MOD_COUNT);
            *bonus1 += TECHDATA::_weapon_modifications[idx].cost;
            *bonus2 += TECHDATA::_weapon_modifications[idx].space;
        }
    }

    int16_t __cdecl Miniaturization_Level_(int16_t player_idx, int16_t app_id) {
        int16_t field_id = TECHDATA::_technology_applications[app_id].tech_field_id;
        int16_t next_id = TECHDATA::_technology_fields[field_id].next_field_id;
        int16_t level = 0;

        while (next_id != 0) {
            if (next_id >= TECH_FIELD_BIOLOGY) {
                level += (int16_t)MOX::_player[player_idx].hyper_advanced_tech[next_id - TECH_FIELD_BIOLOGY];
                break;
            }

            if (MOX::_player[player_idx].tech_fields[next_id] != 3) {
                break;
            }

            level++;
            next_id = TECHDATA::_technology_fields[next_id].next_field_id;
        }

        return level;
    }

    int16_t __cdecl Get_Colony_Handle_() {
        int16_t planet_idx = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);

        return MOX::_planet[planet_idx].colony_index;
    }

    void __cdecl Clear_Special_System_Slot_(int16_t slot_idx) {
        int n_systems = (int)MOX::_n_special_systems;

        for (int i = (int)slot_idx; i < n_systems - 1; ++i) {
            MOX::_design->special_devices[i] = MOX::_design->special_devices[i + 1];
            MOX::_design->special_device_cost[i] = MOX::_design->special_device_cost[i + 1];
            MOX::_design->special_device_space[i] = MOX::_design->special_device_space[i + 1];
        }

        MOX::_design->special_devices[n_systems - 1] = 0;
        MOX::_design->special_device_cost[n_systems - 1] = 0;
        MOX::_design->special_device_space[n_systems - 1] = 0;
    }

    int16_t __cdecl One_Weapon_Space_Without_M_Level_(int16_t weapon_idx, int16_t mod_bonus_idx, int16_t rack_type, int16_t mod_flags) {
        int16_t base_size = TECHDATA::_weapons[weapon_idx].size;
        int32_t rack_size = DESIGN::Weapon_Rack_Size_(rack_type);
        int32_t total_size = base_size + rack_size;

        int16_t temp_bonus;
        int16_t special_multiplier;
        DESIGN::Bonus_For_Special_Mods_(mod_flags, &temp_bonus, &special_multiplier);

        // Apply special modification multiplier
        total_size = (total_size * (int32_t)special_multiplier) / 100;

        int16_t mod_space_bonus = 0;
        if (DESIGN::Weapon_Type_(weapon_idx) == 0) {
            mod_space_bonus = DESIGN::Get_Weapon_Mod_Space_Bonus_(mod_bonus_idx);
        }

        int16_t regular_multiplier;
        DESIGN::Bonus_For_Regular_Mods_(mod_flags, &temp_bonus, &regular_multiplier);

        // Result is based on additive regular multipliers and the previously calculated size
        int32_t final_multiplier = (int32_t)regular_multiplier + (int32_t)mod_space_bonus + 100;
        return (int16_t)((total_size * final_multiplier) / 100);
    }

    void __cdecl Clear_Weapon_Slot_(int16_t slot_idx) {
        int last_active_slot = (int)MOX::_n_weapons_loaded - 1;

        if ((int)slot_idx < last_active_slot) {
            for (int i = (int)slot_idx; i < last_active_slot; ++i) {
                MOX::_design->weapon_type[i] = MOX::_design->weapon_type[i + 1];
                MOX::_design->weapon_count[i] = MOX::_design->weapon_count[i + 1];
                MOX::_design->weapon_firing_arc[i] = MOX::_design->weapon_firing_arc[i + 1];
                MOX::_design->weapon_specials[i] = MOX::_design->weapon_specials[i + 1];
                MOX::_design->weapon_cost[i] = MOX::_design->weapon_cost[i + 1];
                MOX::_design->weapon_space[i] = MOX::_design->weapon_space[i + 1];
                MOX::_design->weapon_ammo[i] = MOX::_design->weapon_ammo[i + 1];
            }
        }

        MOX::_design->weapon_type[last_active_slot] = 0;
        MOX::_design->weapon_count[last_active_slot] = 0;
        MOX::_design->weapon_firing_arc[last_active_slot] = WEAPON_FIRING_ARC_NONE;
        MOX::_design->weapon_specials[last_active_slot] = 0;
        MOX::_design->weapon_cost[last_active_slot] = 0;
        MOX::_design->weapon_space[last_active_slot] = 0;
        MOX::_design->weapon_ammo[last_active_slot] = 0;
    }

    void __cdecl Design_Weapon_Damage_String_(int16_t weapon_id_or_slot, char* buffer, int32_t buffer_size, int16_t field_item_chosen, int16_t row_idx) {
        if (buffer_size <= 0) {
            return;
        }

        const size_t destination_size = (size_t)buffer_size;
        bool is_design_slot = false;
        int16_t weapon_idx = 0;
        int16_t weapon_id;

        if (row_idx == -1 && field_item_chosen == -1) {
            is_design_slot = true;
        }

        if (is_design_slot) {
            weapon_idx = weapon_id_or_slot;
            weapon_id = MOX::_design->weapon_type[weapon_idx];
        } else {
            weapon_id = weapon_id_or_slot;
        }

        int16_t min_damage = TECHDATA::_weapons[weapon_id].min_damage;
        int16_t max_damage = TECHDATA::_weapons[weapon_id].max_damage;

        buffer[0] = '\0';
        char special_msg[20];
        special_msg[0] = '\0';

        // Check for weapons with special descriptive text instead of damage numbers
        if (weapon_id == WEAPON_SPATIAL_COMPRESSOR || weapon_id == WEAPON_ASSAULT_SHUTTLE ||
            weapon_id == WEAPON_STASIS_FIELD || weapon_id == WEAPON_ANTI_MISSILE_ROCKET ||
            weapon_id == WEAPON_GYRO_DESTABILIZER || weapon_id == WEAPON_BIO_TERMINATOR ||
            weapon_id == WEAPON_DEATH_SPORE || weapon_id == WEAPON_INTERCEPTOR ||
            weapon_id == WEAPON_BOMBER || weapon_id == WEAPON_HEAVY_FIGHTER ||
            weapon_id == WEAPON_PULSAR || weapon_id == WEAPON_PLASMA_WEB ||
            weapon_id == WEAPON_BLACK_HOLE_GENERATOR || weapon_id == WEAPON_TRACTOR_BEAM) {

            const char* msg = HAROLD::H_Message_(HESTR_065_SPECIAL);
            strlcpy(special_msg, msg, sizeof(special_msg));
        } else if (weapon_id == WEAPON_HEAVY_FIGHTER || weapon_id == WEAPON_BOMBER) {
            min_damage = INITSHIP::Best_Bomb_Weapon_(MOX::_PLAYER_NUM);
            max_damage = min_damage;
        } else if (weapon_id == WEAPON_INTERCEPTOR) {
            // Point Defense damage is half of its best weapon's damage
            min_damage = INITSHIP::Best_Point_Defense_Weapon_(MOX::_PLAYER_NUM) / 2;
            max_damage = min_damage;
        }

        if (is_design_slot) {
            uint16_t specials = MOX::_design->weapon_specials[weapon_idx];
            if (specials & WEAPON_MOD_FLAG_OVERLOADED) {
                min_damage = (min_damage * 150) / 100;
                max_damage = (max_damage * 150) / 100;
            } else if (specials & WEAPON_MOD_FLAG_POINT_DEFENSE) {
                min_damage = (min_damage * 50 + 50) / 100;
                max_damage = (max_damage * 50) / 100;
                if (max_damage < min_damage) {
                    max_damage = min_damage;
                }
            } else if (specials & WEAPON_MOD_FLAG_HEAVY_MOUNT) {
                min_damage = (min_damage * 150) / 100;
                max_damage = (max_damage * 150) / 100;
            }
        } else {
            if (row_idx == field_item_chosen) {
                if (MOX::_weapon_mod_field_status[WEAPON_MOD_OVERLOADED] > 0) {
                    min_damage = (min_damage * 150) / 100;
                    max_damage = (max_damage * 150) / 100;
                } else if (MOX::_weapon_mod_field_status[WEAPON_MOD_POINT_DEFENSE] > 0) {
                    min_damage = (min_damage * 50 + 50) / 100;
                    max_damage = (max_damage * 50) / 100;
                    if (max_damage < min_damage) {
                        max_damage = min_damage;
                    }
                } else if (MOX::_weapon_mod_field_status[WEAPON_MOD_HEAVY_MOUNT] > 0) {
                    min_damage = (min_damage * 150) / 100;
                    max_damage = (max_damage * 150) / 100;
                }
            }
        }

        if (weapon_id == WEAPON_NO_WEAPONS) {
            strlcpy(buffer, "0", destination_size);
            return;
        }

        if (special_msg[0] != '\0') {
            strlcpy(buffer, special_msg, destination_size);
        } else {
            if (min_damage != max_damage) {
                snprintf(buffer, destination_size, "%d-%d", (int)min_damage, (int)max_damage);
            } else {
                snprintf(buffer, destination_size, "%d", (int)max_damage);
            }
        }
    }
}