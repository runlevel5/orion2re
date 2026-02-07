#include "deps.h"

namespace DESIGN {
    void __cdecl Build_Initial_Design_Template_(int16_t player_idx, int16_t design_idx) {
        s_current_design* d = _design;

        for (int i = 0; i < sizeof(s_current_design); ++i) {
            ((uint8_t*)d)[i] = 0;
        }

        char* name = HACCESS::Design_Name_(player_idx, design_idx);
        int len = 0;
        while (name[len] != '\0') len++;
        for (int i = 0; i <= len && i < 16; ++i) {
            d->name[i] = name[i];
        }

        d->ftl_type = INITSHIP::Best_Warp_Drive_(player_idx);
        d->drive_speed = TECHDATA::_drives[d->ftl_type].speed;
        d->ship_size = static_cast<int16_t>(static_cast<int8_t>(Current_Design_Size_(player_idx, design_idx)));
        d->hull_space = INITGAME::Get_Design_Hull_Size_();
        d->computer_type = INITSHIP::Best_Computer_(player_idx);
        d->picture_type = static_cast<int16_t>(static_cast<int8_t>(HACCESS::Design_Picture_Num_(player_idx, design_idx)));
        d->armor_type = INITSHIP::Best_Armor_(player_idx);
        d->fuel_type = INITSHIP::Best_Ship_Fuel_(player_idx);
        d->shield = INITSHIP::Best_Ship_Shield_(MOX::_PLAYER_NUM);

        for (int i = 0; i < 8; ++i) {
            d->weapon_space[i] = 0;
            d->weapon_cost[i] = 0;
        }

        d->help1 = 0;
        for (int i = 0; i < 8; ++i) {
            d->weapon_type[i] = HACCESS::Design_Weapon_(player_idx, design_idx, i);
            d->weapon_count[i] = static_cast<int16_t>(static_cast<int8_t>(HACCESS::Design_Weapon_Count_(player_idx, design_idx, i)));
            d->weapon_specials[i] = HACCESS::Design_Weapon_Specials_(player_idx, design_idx, i);
            d->weapon_firing_arc[i] = static_cast<int8_t>(HACCESS::Design_Weapon_Firing_Arc_(player_idx, design_idx, i));
            d->weapon_ammo[i] = static_cast<int8_t>(HACCESS::Design_Weapon_Ammo_(player_idx, design_idx, i));

            int16_t rack_type = TECHDATA::Weapon_Rack_(d->weapon_type[i]);
            if (rack_type == 2) {
                if (d->weapon_firing_arc[i] != 16) {
                    d->weapon_firing_arc[i] = 16;
                }
            } else {
                if (Weapon_Needs_Racks_(d->weapon_type[i]) != 0 && (d->weapon_firing_arc[i] & 0xF) == 0) {
                    d->weapon_firing_arc[i] = 16;
                }
            }

            if (d->weapon_type[i] > 0) {
                d->help1++;
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
        d->help2 = special_idx;

        d->base_combat_speed = static_cast<int16_t>(Get_Base_Combat_Speed_(d));
        d->field83_0x123 = 140;

        Update_Calculated_Design_Data_(player_idx);
    }

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

    uint8_t __cdecl Current_Design_Size_(int16_t player_idx, int16_t design_idx) {
        return static_cast<uint8_t>(MOX::_player[player_idx].ship_designs[design_idx].size);
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

        design->field18_0x29 = 0;

        // Calculate Weapon space for all 8 slots
        for (int i = 0; i < 8; ++i) {
            if (design->weapon_type[i] > 0) {
                design->weapon_space[i] = Weapon_Space_(
                    MOX::_PLAYER_NUM,
                    design->weapon_type[i],
                    design->weapon_count[i],
                    design->weapon_specials[i],
                    static_cast<uint16_t>(design->weapon_firing_arc[i]),
                    design->ship_size
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

    void __cdecl Print_Ship_Size_Error_Message_(int size_idx) {
        using Fn = void(__cdecl*)(int);
        reinterpret_cast<Fn>(std::uintptr_t{0x00431850})(size_idx);
    }

    void __cdecl Design_Screen_() {
        int16_t input_field;
        int16_t design_idx_snapshot;
        bool rebuild_buttons = true;
        int16_t exit_flag = 0;
        int16_t refresh_needed = 0;
        char log_buf[100];

        video::Set_Refresh_Full_();
        design_idx_snapshot = _player_design_num;
        MOX::_first_weapon_ctr = 0;
        MOX::_multi_colored_line_start = 0;
        MOX::_first_special_system_ctr = 0;
        _replacement_wait_ctr = 0;
        DESBOX::_incr_value = 1;
        _use_augmented_engines = 1;

        fields::Assign_Auto_Function_(reinterpret_cast<void*>(Draw_Design_Screen_), 2);
        fields::Clear_Fields_();
        _design_size_field = -1000;
        mouse::Set_Mouse_List_(reinterpret_cast<uint8_t*>(MOX::_mouse_list_variable), 1);

        if (MOX::_first_design_screen_call == 0) {
            _changed_design_name = 0;
            KEN::Load_Player_Ship_Palette_(MOX::_PLAYER_NUM);
            graphics::Reset_Window_();
            video::Clip_Off_();
            MOX::_design_size = MOX::_design->ship_size;
            fields::Clear_Fields_();
            _design_size_field = -1000;
            Add_Design_Buttons_();
            MOX::_draw_design_screen_to_back = 1;
            video::Set_Page_Off_();
            _printed_space_avail = MOX::_design->hull_space - MOX::_design->space_used;
            _printed_cost = static_cast<uint16_t>(MOX::_design->total_cost);
            Draw_Design_Screen_();
            RUSS::Mox_Sync_Update_();
        } else {
            MOX::_draw_design_screen_to_back = 1;
            Load_Design_Screen_Pictures_();
            Build_Initial_Design_Template_(MOX::_PLAYER_NUM, _player_design_num);
            MOX::_original_cost = Total_Design_Cost_();
            if (MOX::_refit_ship != 0) {
                MOX::_design->total_cost = 0;
            }
            _printed_space_avail = MOX::_design->hull_space - MOX::_design->space_used;
            _printed_cost = static_cast<uint16_t>(MOX::_design->total_cost);
            MOX::_design_size = MOX::_design->ship_size;
            fields::Clear_Fields_();
            _design_size_field = -1000;
            Add_Design_Buttons_();
            fields::Disable_Draw_All_Fields_();
            video::Set_Refresh_Full_();
            ERIC::Fast_Fade_Out_();
            fonts::Load_Palette_(4, 0, 0xFF);
            palette::Set_Palette_Changes_(0, 0xFF);
            KEN::Load_Player_Ship_Palette_(MOX::_PLAYER_NUM);
            palette::Refresh_Palette_();
            palstore::Darken_Palette_(100);
            video::Set_Page_Off_();
            Draw_Design_Screen_();
            palette::Clear_Palette_Changes_(0, 0xFF);
            RUSS::Mox_Sync_Update_();
            JIM::Fast_Fade_In_();
            fields::Enable_Draw_All_Fields_();
            MOX::_first_design_screen_call = 0;
            graphics::Reset_Window_();
            video::Clip_Off_();
        }

        rebuild_buttons = true;
        mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
        EVANHELP::Set_Design_Screen_Help_List_(MOX::_design->help1, MOX::_design->help2);
        exit_flag = 0;

        do {
            timer::Mark_Time_();
            input_field = fields::Get_Input_();
            MOX::_scanned_field = fields::Scan_Input_();
            s_current_design* d = MOX::_design;

            if (_changed_design_name == 0) {
                char* default_name = HACCESS::Design_Name_(MOX::_PLAYER_NUM, _player_design_num);
                // Manual strcmp implementation
                char* d_name_ptr = d->name;
                char* def_name_ptr = default_name;
                int name_cmp = 0;
                while (true) {
                    if (*d_name_ptr != *def_name_ptr) {
                        name_cmp = (*d_name_ptr < *def_name_ptr) ? -1 : 1;
                        break;
                    }
                    if (*d_name_ptr == 0) break;
                    if (d_name_ptr[1] != def_name_ptr[1]) {
                        name_cmp = (d_name_ptr[1] < def_name_ptr[1]) ? -1 : 1;
                        break;
                    }
                    if (d_name_ptr[1] == 0) break;
                    d_name_ptr += 2;
                    def_name_ptr += 2;
                }
                if (name_cmp != 0) {
                    _changed_design_name = 1;
                }
            }

            if (MOX::_design_name_field == input_field) {
                rebuild_buttons = false;
            } else if (input_field != 0) {
                rebuild_buttons = true;
            }

            if (MOX::_design_cancel_button == input_field) {
                exit_flag = 1;
                _player_design_num = 0;
                MOX::_current_screen = static_cast<SCREEN>(MOX::_return_screen);
            }

            if (MOX::_design_clear_button == input_field) {
                Clear_Design_();
                EVANHELP::Set_Design_Screen_Help_List_(MOX::_design->help1, MOX::_design->help2);
            }

            if (MOX::_design_build_button == input_field) {
                Update_Player_Design_(MOX::_PLAYER_NUM, design_idx_snapshot);
                MOX::_current_screen = static_cast<SCREEN>(MOX::_return_screen);
                exit_flag = 1;
                _player_design_num = 1;
            }

            if (MAINSCR::_debug_field == input_field) {
                _sprintf(log_buf, s_cstdes_log, (int)MOX::_design->ship_size);
            }

            refresh_needed = 0;
            Check_Generic_Replacement_Fields_(input_field, &refresh_needed, &exit_flag);
            Check_Weapon_Fields_(input_field, &refresh_needed, &exit_flag);
            Check_Special_System_Fields_(input_field, &refresh_needed, &exit_flag);
            Check_Plus_Minus_Buttons_(input_field);

            if (MOX::_design->ship_size != MOX::_design_size) {
                MOX::_design->ship_size = MOX::_design_size;
                MOX::_design->hull_space = (int)TECHDATA::_hull_data[MOX::_design->ship_size].hull_size;
                refresh_needed = 1;
                if (MOX::_design->ship_size == 5) {
                    MOX::_design->picture_type = 0x2B;
                } else {
                    MOX::_design->picture_type = MOX::_design->ship_size * 8;
                }
            }

            bool weapon_handled = false;
            for (int i = 0; i < 8; ++i) {
                if (weapon_handled) break;
                int weapon_idx = MOX::_first_weapon_ctr + i;
                if (MOX::_weapons_count_plus_button[i] == input_field) {
                    int new_space = Weapon_Space_(MOX::_PLAYER_NUM, MOX::_design->weapon_type[weapon_idx],
                                                  MOX::_design->weapon_count[weapon_idx] + 1,
                                                  MOX::_design->weapon_specials[weapon_idx],
                                                  static_cast<uint16_t>(MOX::_design->weapon_firing_arc[weapon_idx]),
                                                  MOX::_design->ship_size);
                    weapon_handled = true;
                    int space_diff = (int16_t)new_space - (int16_t)MOX::_design->weapon_space[weapon_idx];
                    if ((int)(int16_t)((int16_t)MOX::_design->space_used + space_diff) <= MOX::_design->hull_space) {
                        MOX::_design->weapon_count[weapon_idx]++;
                        if (MOX::_design->weapon_count[weapon_idx] > 99) {
                            MOX::_design->weapon_count[weapon_idx] = 99;
                        }
                        _replacement_wait_ctr = 0;
                        refresh_needed = 1;
                    }
                } else if (MOX::_weapons_count_minus_button[i] == input_field) {
                    MOX::_design->weapon_count[weapon_idx]--;
                    if (MOX::_design->weapon_count[weapon_idx] < 1) {
                        MOX::_design->weapon_count[weapon_idx] = 1;
                    }
                    _replacement_wait_ctr = 0;
                    refresh_needed = 1;
                    weapon_handled = true;
                }
            }

            if (refresh_needed != 0) {
                Update_Calculated_Design_Data_(MOX::_PLAYER_NUM);
                refresh_needed = 0;
            }

            if (rebuild_buttons) {
                fields::Clear_Fields_();
                _design_size_field = -1000;
                Add_Design_Buttons_();
            }

            for (int i = 0; i < 6; ++i) {
                if (MOX::_design_ship_size_field[i] == input_field) {
                    if (_add_size_button_flag[i] < 0xF1) {
                        _changed_design_name = 0;
                        int pic_logic = MOX::_design->picture_type;
                        int pic_val = (pic_logic ^ (pic_logic >> 15)) - (pic_logic >> 15);
                        Get_Ship_Design_Name_(
                            (uint16_t)MOX::_player[MOX::_PLAYER_NUM].race,
                            MOX::_design->ship_size,
                            (pic_val & 7),
                            MOX::_design->name
                        );
                    } else {
                        Print_Ship_Size_Error_Message_(i);
                    }
                }
            }

            if (exit_flag == 0) {
                video::Set_Page_Off_();
                _printed_space_avail = MOX::_design->hull_space - MOX::_design->space_used;
                _printed_cost = static_cast<uint16_t>(MOX::_design->total_cost);
                Draw_Design_Screen_();
                RUSS::Mox_Sync_Update_();
            } else if (exit_flag == 99) {
                video::Set_Page_Off_();
                Draw_Design_Screen_();
                RUSS::Mox_Sync_Update_();
            }

            strings::Trim_(MOX::_design->name);
            if (MOX::_design->name[0] == '\0') {
                char* def_name = HACCESS::Design_Name_(MOX::_PLAYER_NUM, design_idx_snapshot);
                char* src = def_name;
                char* dst = MOX::_design->name;
                while ((*dst++ = *src++) != '\0');
            }

            timer::Release_Time_(2);
            if (exit_flag != 0) {
                fields::Enable_Mouse_Wait_();
                fields::Clear_Fields_();
                MOX::_reloaded_ship_palette = 0;
                if (MOX::_current_screen != SCREEN_DESIGN) {
                    JIM::Play_Background_Music_();
                }
                return;
            }
        } while (true);
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

    int32_t __cdecl Get_Design_Hull_Size_() {
        return INITGAME::Get_Design_Hull_Size_();
    }

    int32_t __cdecl Weapon_Space_(int16_t player_idx, int16_t weapon_idx, int16_t quantity,
                                  uint16_t mod_flags, uint16_t arc_type, uint16_t ship_size) {
        int16_t base_size = TECHDATA::_weapons[weapon_idx].base_size;
        int32_t rack_size = 0;

        if (Weapon_Needs_Racks_(weapon_idx) != 0) {
            rack_size = Weapon_Rack_Size_(arc_type);
        }

        int16_t dummy_out;
        int16_t special_mod_mult;
        Bonus_For_Special_Mods_(ship_size, &dummy_out, &special_mod_mult);

        int16_t mod_rack_offset = 0;
        if (TECHDATA::Weapon_Rack_(weapon_idx) == 0) {
            mod_rack_offset = Get_Weapon_Mod_Space_Bonus_(mod_flags);
        }

        int16_t regular_mod_add;
        Bonus_For_Regular_Mods_(ship_size, &dummy_out, &regular_mod_add);

        int16_t tech_app_id = TECHDATA::_weapons[weapon_idx].tech_application_id;
        uint32_t miniaturization_cat = Miniaturization_Category_(tech_app_id);
        int16_t tech_level = Tech_Level_(player_idx, tech_app_id);

        int32_t intermediate = (special_mod_mult * (base_size + rack_size)) / 100;

        int32_t raw_space_calc = ((regular_mod_add + mod_rack_offset + 100) * intermediate) / 100;

        int32_t space_per_weapon = Space_Given_M_Level_(static_cast<int16_t>(raw_space_calc), tech_level, static_cast<uint8_t>(miniaturization_cat));

        int32_t total_space = static_cast<int32_t>(quantity) * space_per_weapon;

        if (total_space <= 1) {
            total_space = 1;
        }

        return total_space;
    }

    void __cdecl Draw_Design_Screen_() {
        animate::Draw_(0, 0, MOX::_design_screen_seg);
        Draw_Down_Buttons_();
        Print_Current_Design_();
        Check_For_Auto_Repeat_();
        JIM::Play_Streaming_Music_(8, -1, 1);
    }

    void __cdecl Update_Player_Design_(int16_t player_idx, int16_t design_idx) {
        Current_Design_Shield_();
        strings::Trim_(MOX::_design->name);

        if (MOX::_design->name[0] == '\0') {
            char* default_name = HACCESS::Design_Name_(MOX::_PLAYER_NUM, design_idx);
            char* dest = MOX::_design->name;
            while ((*dest++ = *default_name++) != '\0');
        }

        s_player* plr = &MOX::_player[player_idx];
        s_ship_design* target_design = &plr->ship_designs[design_idx];

        // Copy name from current design to player record
        char* src_name = MOX::_design->name;
        char* dest_name = target_design->name;
        while ((*dest_name++ = *src_name++) != '\0');

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
        target_design->combat_speed = (int8_t)Get_Modified_Combat_Speed_(MOX::_design);
        target_design->speed = (int8_t)INITSHIP::Get_FTL_Speed_(player_idx, MOX::_design->ftl_type);
    }

    void __cdecl Clear_Design_() {
        int16_t player_num = MOX::_PLAYER_NUM;
        s_current_design* design = _design;

        design->ftl_type = INITSHIP::Best_Warp_Drive_(player_num);
        design->drive_speed = TECHDATA::_drives[design->ftl_type].speed;

        design->computer_type = INITSHIP::Best_Computer_(player_num);
        design->armor_type = INITSHIP::Best_Armor_(player_num);
        design->fuel_type = INITSHIP::Best_Ship_Fuel_(player_num);

        for (int i = 0; i < 8; ++i) {
            design->weapon_type[i] = 0;
            design->weapon_count[i] = 0;
            design->weapon_firing_arc[i] = 0;
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

    void __cdecl Load_Design_Screen_Pictures_() {
        MOX::Mox_Update_();
        MOX::_design_screen_seg = (s_animation_header*)MOX::farload::Far_Reload_(PTR_s_DESIGN_LBX_005560D8, 0, MOX::_screen_seg);
        MOX::_design_icon_lf_arrow_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, 1, MOX::_screen_seg);
        MOX::_design_icon_rt_arrow_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, 2, MOX::_screen_seg);
        MOX::_clear_design_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, 3, MOX::_screen_seg);
        MOX::_cancel_design_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, 4, MOX::_screen_seg);
        MOX::_build_design_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, 5, MOX::_screen_seg);
        MOX::Mox_Update_();

        for (int i = 0; i < 6; ++i) {
            MOX::_design_size_button_seg[i] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, 6 + i, MOX::_screen_seg);
        }
        for (int i = 0; i < 6; ++i) {
            MOX::_darkened_size_button_seg[i] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, 6 + i, MOX::_screen_seg);
        }

        int idx = 12;
        _dark_build_design_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::Mox_Update_();

        MOX::_plus_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_minus_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_d_scroll_up_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_d_scroll_dn_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        _dull_plus_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        _dull_minus_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_dull_d_scroll_up_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_dull_d_scroll_dn_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);

        MOX::DAT_00597994 = MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);

        MOX::_beam_filter_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_missile_filter_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_bomb_filter_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_special_filter_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_weapons_cancel_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_weapons_accept_button_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::Mox_Update_();

        MOX::_weapons_top_box_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_weapons_mid_box_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_weapons_bot_box_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_weapons_selected_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::Mox_Update_();

        MOX::_weapons_rack_seg = MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);

        for (int i = 0; i < 5; ++i) {
            MOX::_weapons_arc_pict_seg[i] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        }
        for (int i = 0; i < 5; ++i) {
            MOX::_weapons_arc_text_seg[i] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        }
        MOX::Mox_Update_();
        for (int i = 0; i < 5; ++i) {
            MOX::_weapons_rack_text_seg[i] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        }

        MOX::_weapons_blank_mods_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        for (int i = 0; i < 18; ++i) {
            MOX::_weapons_special_seg[i] = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        }
        MOX::Mox_Update_();

        MOX::_system_repl_scroll_bar_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_system_repl_box_top_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_system_repl_box_mid_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_system_repl_box_btm_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);
        MOX::_system_repl_box_cancel_btn = (s_animation_header*)MOX::farload::Far_Reload_Next_(PTR_s_DESIGN_LBX_005560D8, idx++, MOX::_screen_seg);

        MOX::allocate::Get_Remaining_Block_Space_(MOX::_screen_seg);
        MOX::_design = MOX::allocate::Allocate_First_Block_(MOX::_global_data_seg, 0x125);
        MOX::_weapon_filter = MOX::allocate::Allocate_Next_Block_(MOX::_global_data_seg, 0xF0);

        char tech_lbx[20];
        Get_Tech_Description_Lbx_Name_(tech_lbx);

        for (int i = 0; i < 39; ++i) {
            MOX::_special_desc_string[i] = (char*)MOX::farload::Far_Reload_Next_Data_(tech_lbx, 1, MOX::_global_data_seg, i, 1, 100);
            MOX::Mox_Update_();
        }
        for (int i = 0; i < 40; ++i) {
            MOX::_weapon_notes_string[i] = (char*)MOX::farload::Far_Reload_Next_Data_(tech_lbx, 3, MOX::_global_data_seg, i, 1, 100);
            MOX::Mox_Update_();
        }
    }

    void __cdecl Print_Current_Design_() {
        s_current_design local_design;
        int16_t missile_evasion, beam_defense, beam_attack_bonus;
        char buffer[256];
        s_colors colors_primary, colors_secondary;
        char arc_str[32], mod_str[128];

        helpers::memset(&local_design, 0, sizeof(local_design));

        Build_Design_Template_(&local_design);
        INITSHIP::Get_Design_Combat_Bonuses_(&local_design, &missile_evasion, &beam_defense, &beam_attack_bonus);

        graphics::Reset_Window_();
        video::Clip_Off_();
        fonts::Set_Outline_Color_(0, 0);

        MISC::Get_Mox_Font_Colors_(3, 'U', '[', &colors_primary);
        MISC::Get_Mox_Font_Colors_(2, 'U', '[', &colors_secondary);

        for (int i = 1; i < 8; ++i) {
            colors_primary.colors[i] += 2;
            colors_secondary.colors[i] += 2;
        }

        fonts::Set_Font_Style_(3, &colors_primary);
        fonts::Set_Font_Spacing_(1);
        fonts::Print_(0xEC, 0x38, TECHDATA::_drives[MOX::_design->ftl_type].name);

        fonts::Set_Font_Style_(2, &colors_secondary);
        fonts::Set_Font_Spacing_(1);
        _sprintf(buffer, HAROLD::H_Message_(0x17E /* HESTR_17E_D_PARSECS_TURN */), TECHDATA::_drives[MOX::_design->ftl_type].speed);
        fonts::Print_(0xFB, 0x47, buffer);

        _sprintf(buffer, HAROLD::H_Message_(0x54 /* HESTR_054_D_COMBAT_SPEED */), (int)Get_Base_Combat_Speed_(MOX::_design));
        fonts::Print_(0xFB, 0x56, buffer);

        fonts::Set_Font_Style_(3, &colors_primary);
        fonts::Print_(0xEC, 0x6B, TECHDATA::_armor[MOX::_design->armor_type].name);

        fonts::Set_Font_Style_(2, &colors_secondary);
        _sprintf(buffer, HAROLD::H_Message_(0x55 /* HESTR_055_D_STRUCTURE_POINTS */), (int)Current_Design_Has_Special_System_Count_());
        fonts::Print_(0xFB, 0x7A, buffer);

        _sprintf(buffer, HAROLD::H_Message_(0x56 /* HESTR_056_D_ARMOR_POINTS */), (int)Current_Design_Armor_Absorption_());
        fonts::Print_(0xFB, 0x89, buffer);

        fonts::Set_Font_Style_(3, &colors_primary);
        if (_design_shield_replacement_field == MOX::_scanned_field) {
            Set_Flashing_Colors_(3, 1);
        }

        if (MOX::_design->shield == 0) {
            fonts::Print_(0x1B5, 0x38, HAROLD::H_Message_(0x36 /* HESTR_036_NO_SHIELD */));
            fonts::Set_Font_Style_(2, &colors_secondary);
            fonts::Print_(0x1C5, 0x47, HAROLD::H_Message_(0x57 /* HESTR_057_NO_STRENGTH */));
            _sprintf(buffer, "%s", HAROLD::H_Message_(0x58 /* HESTR_058_NO_DAMAGE_POINTS_BLOCK */));
        } else {
            fonts::Print_(0x1B5, 0x38, TECHDATA::_shields[MOX::_design->shield].name);
            fonts::Get_String_Width_(TECHDATA::_shields[MOX::_design->shield].name);
            fonts::Set_Font_Style_(2, &colors_secondary);
            _sprintf(buffer, HAROLD::H_Message_(0x59 /* HESTR_059_D_STRENGTH */), (int)Shield_Strength_());
            fonts::Print_(0x1C5, 0x47, buffer);

            int block = Current_Shield_Damage_Points_Blocked_();
            const char* suffix = (block == 1) ? "" : "s";
            _sprintf(buffer, HAROLD::H_Message_(0x5A /* HESTR_05A_D_DAMAGE_POINTS_BLOCKE */), block, suffix);
        }
        fonts::Print_(0x1C5, 0x55, buffer);

        fonts::Set_Font_Style_(3, &colors_primary);
        if (_design_computer_replacement_field == MOX::_scanned_field) {
            Set_Flashing_Colors_(3, 1);
        }
        fonts::Print_(0x1B5, 0x61, TECHDATA::_computers[MOX::_design->computer_type].name);

        fonts::Set_Font_Style_(2, &colors_secondary);
        _sprintf(buffer, HAROLD::H_Message_(0x5B /* HESTR_05B_D_BEAM_ATTACK */), (int)TECHDATA::_computers[MOX::_design->computer_type].field5_0x14);
        fonts::Print_(0x1C5, 0x70, buffer);

        fonts::Set_Font_Style_(3, &colors_primary);
        fonts::Print_(0x1B5, 0x7D, HAROLD::H_Message_(0x5C /* HESTR_05C_BEAM_DEFENSE */));
        _sprintf(buffer, s_pct_s_pct_d, (beam_defense >= 0 ? s_plus : s_minus), (int)beam_defense);
        fonts::Print_Right_(0x26A, 0x7D, buffer);

        fonts::Print_(0x1B5, 0x89, HAROLD::H_Message_(0x5D /* HESTR_05D_MISSILE_EVASION */));
        _sprintf(buffer, s_pct_d_pct, (int)missile_evasion);
        fonts::Print_Right_(0x26A, 0x89, buffer);

        fonts::Set_Font_Spacing_(2);
        graphics::Set_Window_(0x10, 0x9B, 0x272, 0x11C);
        video::Clip_On_();

        fonts::Set_Font_Style_(3, &colors_primary);
        fonts::Print_(0x53, 0x9C, HAROLD::H_Message_(0x5E /* HESTR_05E_WEAPON_TYPE */));
        fonts::Print_(0xF9, 0x9C, HAROLD::H_Message_(0x5F /* HESTR_05F_DAMAGE */));
        fonts::Print_(0x13C, 0x9C, HAROLD::H_Message_(0x60 /* HESTR_060_ARC */));
        fonts::Print_(0x19E, 0x9C, HAROLD::H_Message_(0x30 /* HESTR_030_SPACE */));
        fonts::Print_(0x167, 0x9C, HAROLD::H_Message_(0x185 /* HESTR_185_COST */));
        fonts::Print_(0x1DF, 0x9C, HAROLD::H_Message_(0x61 /* HESTR_061_MODIFICATIONS */));

        bool show_weapon_replacement = false;
        int y_cursor = 0xAA;
        for (int i = 0; i < 8; ++i) {
            int weapon_slot = i + MOX::_first_weapon_ctr;

            if ((i - (int)MOX::_scanned_field) == -1000) {
                if (MOX::_design->weapon_type[weapon_slot] <= 0) {
                    show_weapon_replacement = true;
                    fonts::Set_Font_Style_(2, &colors_secondary);
                } else {
                    Set_Flashing_Colors_(2, 2);
                }
            } else {
                fonts::Set_Font_Style_(2, &colors_secondary);
            }

            if (MOX::_design->weapon_type[weapon_slot] > 0) {
                Weapon_Mod_String_(MOX::_design->weapon_specials[i], mod_str, 1);
                Weapon_Arc_String_((int16_t)MOX::_design->weapon_firing_arc[i], arc_str);

                const char* w_name = (MOX::_design->weapon_count[weapon_slot] > 1)
                                     ? TECHDATA::_weapons[MOX::_design->weapon_type[weapon_slot]].name_plural
                                     : TECHDATA::_weapons[MOX::_design->weapon_type[weapon_slot]].name;

                if (Weapon_Needs_Racks_(MOX::_design->weapon_type[weapon_slot])) {
                    _sprintf(buffer, s_pct_s_bracket_pct_d, w_name, (int)MOX::_design->weapon_ammo[i]);
                    fonts::Print_(0x55, y_cursor, buffer);
                } else {
                    fonts::Print_(0x55, y_cursor, w_name);
                }

                fonts::Print_Integer_Right_(0x31, y_cursor, MOX::_design->weapon_count[weapon_slot]);
                Design_Weapon_Damage_String_(weapon_slot, buffer, -1, -1);

                if (helpers::strcmp(buffer, s_special_tag) == 0) {
                    MISC::Change_Font_Style_(1);
                    fonts::Print_Centered_(0x10B, y_cursor, buffer);
                    MISC::Change_Font_Style_(2);
                } else {
                    fonts::Print_Centered_(0x10B, y_cursor, buffer);
                }

                fonts::Print_Centered_(0x149, y_cursor, arc_str);
                _sprintf(buffer, s_pct_d, (MOX::_design->weapon_count[weapon_slot] > 0) ? (MOX::_design->weapon_space[i] / MOX::_design->weapon_count[weapon_slot]) : 0);
                fonts::Print_Centered_(0x1B4, y_cursor, buffer);
                _sprintf(buffer, s_pct_d, (MOX::_design->weapon_count[weapon_slot] > 0) ? (MOX::_design->weapon_cost[i] / MOX::_design->weapon_count[weapon_slot]) : 0);
                fonts::Print_Centered_(0x179, y_cursor, buffer);
                fonts::Print_Centered_(0x211, y_cursor, mod_str);

                y_cursor += 0x0D;
            }
        }

        if (show_weapon_replacement) {
            int slots_left = 8 - MOX::_n_weapons_loaded;
            if (slots_left > 0) {
                if (slots_left > 1) {
                    y_cursor += ((MOX::_n_weapons_loaded * -13 + 127) / 2) - 14;
                }
                if (Space_For_Weapon_Replacement_()) {
                    fonts::Set_Font_Style_(2, &colors_secondary);
                    Set_Flashing_Colors_(2, 2);
                    fonts::Print_Centered_(0x136, y_cursor, HAROLD::H_Message_(0x17F));
                } else {
                    s_colors* custom_colors = MISC::Font_Colors2_(2, 'I', 'M');
                    fonts::Set_Font_Style_(2, custom_colors);
                    Set_Flashing_Colors_(2, 2);
                    int msg = (_printed_space_avail < 1) ? 0x1A /* HESTR_01A_NOT_ENOUGH_SPACE_TO_AD */ : 0x18 /* HESTR_018_NO_NEW_WEAPONS_CAN_BE_ */;
                    fonts::Print_Centered_(0x136, y_cursor, HAROLD::H_Message_(msg));
                }
            }
        }

        video::Clip_Off_();
        graphics::Set_Window_(0x11, 0x131, 0x272, 0x1B0);
        video::Clip_On_();

        fonts::Set_Font_Style_(3, &colors_primary);
        fonts::Print_(0x1E, 0x132, HAROLD::H_Message_(0x62 /* HESTR_062_SPECIAL_SYSTEM */));
        fonts::Print_(0x118, 0x132, HAROLD::H_Message_(0x38 /* HESTR_038_DESCRIPTION */));

        bool show_system_replacement = false;
        int y_special = 0x140;
        for (int i = 0; i < 8; ++i) {
            int special_slot = i + MOX::_first_special_system_ctr;
            if ((i - (int)MOX::_scanned_field) == -2000) {
                if (MOX::_design->special_devices[special_slot] <= 0) {
                    show_system_replacement = true;
                    fonts::Set_Font_Style_(2, &colors_secondary);
                } else {
                    fonts::Set_Font_Style_(2, &colors_secondary);
                    Set_Flashing_Colors_(2, 2);
                }
            } else {
                fonts::Set_Font_Style_(2, &colors_secondary);
            }

            if (MOX::_design->special_devices[special_slot] > 0) {
                int dev_idx = MOX::_design->special_devices[special_slot];
                const char* dev_name = *reinterpret_cast<const char**>(reinterpret_cast<uint8_t*>(TECHDATA::_specials) + dev_idx * 0x30);
                fonts::Print_(0x23, y_special, dev_name);
                fonts::Print_(0x10E, y_special, MOX::_special_desc_string[dev_idx]);
                y_special += 0x0D;
            }
        }

        if (show_system_replacement) {
            int slots_left = 8 - MOX::_n_special_systems;
            if (slots_left > 0) {
                if (slots_left > 1) {
                    y_special += ((MOX::_n_special_systems * -13 + 125) / 2) - 14;
                }
                if (Space_For_System_Replacement_()) {
                    fonts::Set_Font_Style_(2, &colors_secondary);
                    Set_Flashing_Colors_(2, 2);
                    fonts::Print_Centered_(0x136, y_special, HAROLD::H_Message_(0x180));
                } else {
                    s_colors* custom_colors = MISC::Font_Colors2_(2, 'I', 'N');
                    fonts::Set_Font_Style_(2, custom_colors);
                    Set_Flashing_Colors_(2, 2);
                    int msg = (_printed_space_avail < 1) ? 0x1B : 0x19;
                    fonts::Print_Centered_(0x136, y_special, HAROLD::H_Message_(msg));
                }
            }
        }

        video::Clip_Off_();
        graphics::Reset_Window_();
        void* ship_pic = KEN::Get_Ship_Picture_Seg_(MOX::_PLAYER_NUM, MOX::_design->picture_type);
        int pic_w = animate::Get_Width_((s_animation_header*)ship_pic);
        int pic_h = animate::Get_Height_((s_animation_header*)ship_pic);
        animate::Draw_(((0x36 - pic_w) / 2) + 0x27, ((0x3A - pic_h) / 2) + 0x3A, (s_animation_header*)ship_pic);

        fonts::Set_Font_Style_(3, &colors_primary);
        fonts::Get_Font_Height_();
        fonts::Print_(0x19, 0x81, HAROLD::H_Message_(0x30 /* HESTR_030_SPACE */));
        fonts::Print_Integer_Right_(0x6A, 0x81, MOX::_design->hull_space);

        s_colors footer_colors;
        MISC::Get_Mox_Font_Colors_(4, 'U', 'Y', &footer_colors);
        fonts::Set_Font_Style_(4, &footer_colors);
        int f_h = fonts::Get_Font_Height_();
        int footer_y = ((0x18 - f_h) / 2) + 0x1BC;

        fonts::Print_(0x18, footer_y, HAROLD::H_Message_(0x185 /* HESTR_185_COST */));
        fonts::Print_Integer_Right_(0x87, footer_y, (int)Printed_Design_Cost_());
        fonts::Print_(0xA3, footer_y, HAROLD::H_Message_(0x63 /* HESTR_063_SPACE_AVAILABLE */));
        fonts::Print_Integer_Right_(0x160, footer_y, _printed_space_avail);
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

        for (int size_idx = 0; size_idx < 6; ++size_idx) {
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
}