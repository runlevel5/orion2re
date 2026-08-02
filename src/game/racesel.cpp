#include "pch.h"
#include "fields.h"

namespace RACESEL {

static bool Is_Valid_Last_Race_Base_Race_(uint8_t race_idx) {
    return race_idx < STOCK_RACE_COUNT;
}

int16_t _custom_flag;
int16_t _displayed_race;
int16_t _old_race;
int16_t _race_button_status[14];
uint8_t* _race_specials[14];
s_animation_header* _race_button[14];
char _last_race_name[15];
s_animation_header* _last_race_seg;
uint8_t _color_flag[MAX_PLAYERS];
uint8_t _race_flag[14];
char* _temp_ruler_name;

void __cdecl Convert_Custom_Race_Flags_(uint8_t* custom_race_flags) {
    for (int16_t bonus_idx = 0; bonus_idx < 9; ++bonus_idx) {
        const uint8_t bonus_value = custom_race_flags[bonus_idx + 1];

        if (MOX::_special_bonus[bonus_idx][0] == (int8_t)bonus_value) {
            custom_race_flags[bonus_idx + 1] = 1;
        } else if (MOX::_special_bonus[bonus_idx][1] == (int8_t)bonus_value) {
            custom_race_flags[bonus_idx + 1] = 2;
        } else if (MOX::_special_bonus[bonus_idx][2] == (int8_t)bonus_value) {
            custom_race_flags[bonus_idx + 1] = 3;
        } else {
            custom_race_flags[bonus_idx + 1] = 0;
        }
    }
}

void __cdecl Set_Race_Select_Help_() {
    MOX::_help_entry_list[0].help_id = 630;
    MOX::_help_entry_list[0].x1 = 351;
    MOX::_help_entry_list[0].y1 = 90;
    MOX::_help_entry_list[0].x2 = 473;
    MOX::_help_entry_list[0].y2 = 134;

    MOX::_help_entry_list[1].help_id = 631;
    MOX::_help_entry_list[1].x1 = 351;
    MOX::_help_entry_list[1].y1 = 138;
    MOX::_help_entry_list[1].x2 = 473;
    MOX::_help_entry_list[1].y2 = 182;

    MOX::_help_entry_list[2].help_id = 632;
    MOX::_help_entry_list[2].x1 = 351;
    MOX::_help_entry_list[2].y1 = 186;
    MOX::_help_entry_list[2].x2 = 473;
    MOX::_help_entry_list[2].y2 = 230;

    MOX::_help_entry_list[3].help_id = 633;
    MOX::_help_entry_list[3].x1 = 351;
    MOX::_help_entry_list[3].y1 = 234;
    MOX::_help_entry_list[3].x2 = 473;
    MOX::_help_entry_list[3].y2 = 278;

    MOX::_help_entry_list[4].help_id = 634;
    MOX::_help_entry_list[4].x1 = 351;
    MOX::_help_entry_list[4].y1 = 282;
    MOX::_help_entry_list[4].x2 = 473;
    MOX::_help_entry_list[4].y2 = 326;

    MOX::_help_entry_list[5].help_id = 635;
    MOX::_help_entry_list[5].x1 = 351;
    MOX::_help_entry_list[5].y1 = 330;
    MOX::_help_entry_list[5].x2 = 473;
    MOX::_help_entry_list[5].y2 = 374;

    MOX::_help_entry_list[6].help_id = 636;
    MOX::_help_entry_list[6].x1 = 351;
    MOX::_help_entry_list[6].y1 = 378;
    MOX::_help_entry_list[6].x2 = 473;
    MOX::_help_entry_list[6].y2 = 422;

    MOX::_help_entry_list[7].help_id = 637;
    MOX::_help_entry_list[7].x1 = 477;
    MOX::_help_entry_list[7].y1 = 90;
    MOX::_help_entry_list[7].x2 = 599;
    MOX::_help_entry_list[7].y2 = 134;

    MOX::_help_entry_list[8].help_id = 638;
    MOX::_help_entry_list[8].x1 = 477;
    MOX::_help_entry_list[8].y1 = 138;
    MOX::_help_entry_list[8].x2 = 599;
    MOX::_help_entry_list[8].y2 = 182;

    MOX::_help_entry_list[9].help_id = 639;
    MOX::_help_entry_list[9].x1 = 477;
    MOX::_help_entry_list[9].y1 = 186;
    MOX::_help_entry_list[9].x2 = 599;
    MOX::_help_entry_list[9].y2 = 230;

    MOX::_help_entry_list[10].help_id = 640;
    MOX::_help_entry_list[10].x1 = 477;
    MOX::_help_entry_list[10].y1 = 234;
    MOX::_help_entry_list[10].x2 = 599;
    MOX::_help_entry_list[10].y2 = 278;

    MOX::_help_entry_list[11].help_id = 641;
    MOX::_help_entry_list[11].x1 = 477;
    MOX::_help_entry_list[11].y1 = 282;
    MOX::_help_entry_list[11].x2 = 599;
    MOX::_help_entry_list[11].y2 = 326;

    MOX::_help_entry_list[12].help_id = 642;
    MOX::_help_entry_list[12].x1 = 477;
    MOX::_help_entry_list[12].y1 = 330;
    MOX::_help_entry_list[12].x2 = 599;
    MOX::_help_entry_list[12].y2 = 374;

    MOX::_help_entry_list[13].help_id = 643;
    MOX::_help_entry_list[13].x1 = 477;
    MOX::_help_entry_list[13].y1 = 378;
    MOX::_help_entry_list[13].x2 = 599;
    MOX::_help_entry_list[13].y2 = 422;

    MOX::_help_entry_list[14].help_id = 644;
    MOX::_help_entry_list[14].x1 = 0;
    MOX::_help_entry_list[14].y1 = 0;
    MOX::_help_entry_list[14].x2 = 639;
    MOX::_help_entry_list[14].y2 = 479;

    fields::Set_Help_List_(MOX::_help_entry_list, 15);
}

void __cdecl Reload_Race_Selection_Screen_() {
    uint8_t saved_race_index;

    allocate::Push_Block_(MOX::_screen_seg);

    for (int i = STOCK_RACE_ALKARI; i < STOCK_RACE_COUNT; ++i) {
        _race_specials[i] = (uint8_t*)farload::Far_Reload_Next_Data_("RACESTUF.LBX", 7, MOX::_screen_seg, i, 1, TRAIT_COUNT);
        RUSS::Mox_Update_();
    }

    FILE* file = fopen_case("lastrace.rac", "rb");
    if (file != nullptr) {
        // Load template for custom race (LBX entry 12) into the 14th slot
        _race_specials[13] = (uint8_t*)farload::Far_Reload_Next_Data_("RACESTUF.LBX", 7, MOX::_screen_seg, STOCK_RACE_TRILARIAN, 1, TRAIT_COUNT);

        // Overwrite template with saved race data
        fread(_race_specials[13], TRAIT_COUNT, 1, file);
        Convert_Custom_Race_Flags_(_race_specials[13]);
        RUSS::Mox_Update_();

        fread(&saved_race_index, sizeof(saved_race_index), 1, file);
        fread(_last_race_name, sizeof(_last_race_name), 1, file);
        (void)saved_race_index;

        RUSS::Mox_Update_();
        fclose(file);
    }

    for (int i = 0; i < 14; ++i) {
        _race_button[i] = (s_animation_header*)farload::Far_Reload_Next_("RACESEL.LBX", i + 1, MOX::_screen_seg);
        RUSS::Mox_Update_();
    }

    _last_race_seg = (s_animation_header*)farload::Far_Reload_Next_("patch13.lbx", 1, MOX::_screen_seg);
}

int16_t Race_Selection_Screen_(s_player* plr) {
    int16_t field_ids[14];
    int16_t hotkey_field;
    char msg_buffer[100];
    int16_t done_flag = 0;
    
    fields::Disable_Cancel_();
    fields::Deactivate_Help_List_();
    fields::Clear_Fields_();
    ERIC::Fast_Fade_Out_();
    fonts::Load_Palette_(9, 0, 0xFF);
    
    _custom_flag = 0;
    _displayed_race = STOCK_RACE_ELERIAN;
    for (int i = 0; i < 14; ++i) {
        _race_button_status[i] = 0;
    }
    _race_button_status[STOCK_RACE_ELERIAN] = 1;
    
    Reload_Race_Selection_Screen_();
    RACESEL::Set_Race_Select_Help_();
    video::Set_Page_Back_();
    auto raceopt_anim = (s_animation_header*)buffer::Buffer_Reload_("RACEOPT.LBX", 0, MOX::_global_cache_seg);
    RUSS::Mox_Update_();
    animate::Draw_(0, 0, raceopt_anim);
    video::Copy_Back_To_Off_();
    
    if (MOX::_game_type == 0 || MOX::_game_type == 1) {
        hotkey_field = fields::Add_Hot_Key_("\x1B");
    } else {
        hotkey_field = -1000;
    }
    
    RUSS::Mox_Update_();
    for (int i = 0; i < 14; ++i) {
        field_ids[i] = fields::Add_Radio_Button_Field_(
            (i / 7) * 126 + 351, (i % 7) * 48 + 90, "",
            _race_button[i], &_race_button_status[i], "", 40
        );
    }
    
    fields::Assign_Auto_Function_(Draw_Race_Selection_Screen_, 2);
    _old_race = 0;
    video::Set_Page_Off_();
    Draw_Race_Selection_Screen_();
    palstore::Darken_Palette_(100);
    palette::Clear_Palette_Changes_(0, 0xFF);
    RUSS::Mox_Sync_Update_();
    ERIC::Fast_Fade_In_();
    
    while (true) {
        int16_t input_field = fields::Get_Input_();
        int16_t scan_idx = fields::Scan_Input_();
        _displayed_race = scan_idx - field_ids[0];
        if (_displayed_race < 0 || _displayed_race > 13) {
            _displayed_race = _old_race;
        }
        
        for (int i = STOCK_RACE_ALKARI; i < STOCK_RACE_COUNT; ++i) {
            if (input_field == field_ids[i] && _race_flag[i] == 0) {
                uint8_t can_request =
                    (MOX::_game_type == 0 || MOX::_game_type == 1) ? 1 : NETSTART::Request_Race_((uint8_t)i);
                
                if (!can_request) {
                    KEN::Ken_Get_Text_Message_(72, msg_buffer, sizeof(msg_buffer));
                    GENDRAW::Message_Box_(msg_buffer);
                    video::Set_Page_Back_();
                    auto anim = (s_animation_header*)buffer::Buffer_Reload_("RACEOPT.LBX", 0, MOX::_global_cache_seg);
                    animate::Draw_(0, 0, anim);
                    video::Copy_Back_To_Off_();
                } else {
                    if (_custom_flag == 0) {
                        timer::Mark_Time_();
                        timer::Release_Time_(5);
                        fields::Clear_Fields_();
                        struct_::Copy_Structure_(plr->traits, _race_specials[i], sizeof(plr->traits));
                        plr->race = (uint8_t)i;
                        strlcpy(plr->race_name, MOX::_race_names[i], sizeof(plr->race_name));
                        fields::Deactivate_Help_List_();
                        
                        int screen_state;
                        do {
                            screen_state = Naming_Popup_(plr->name, sizeof(plr->name), i);
                            video::Set_Page_Back_();
                            auto anim = (s_animation_header*)buffer::Buffer_Reload_("RACEOPT.LBX", 0, MOX::_global_cache_seg);
                            animate::Draw_(0, 0, anim);
                            video::Copy_Back_To_Off_();
                            RUSS::Mox_Sync_Update_();
                            _old_race = i;
                            
                            if (screen_state == 1) {
                                screen_state += Flag_Screen_(&plr->color);
                                if (screen_state != 2) {
                                    Reload_Race_Selection_Screen_();
                                }
                            }
                        } while (screen_state == 1);
                        
                        if (screen_state == 2) {
                            for (int trait_idx = TRAIT_POP_GROWTH_BONUS; trait_idx < TRAIT_LOW_G_WORLD; ++trait_idx) {
                                if (plr->traits[trait_idx] != 0) {
                                    plr->traits[trait_idx] =
                                        MOX::_special_bonus[trait_idx - 1][plr->traits[trait_idx] - 1];
                                }
                            }
                            done_flag = 1;
                            _race_flag[i] = 1;
                        } else {
                            if (MOX::_game_type != 0 && MOX::_game_type != 1) {
                                Unrequest_Race_(i);
                            } else {
                                _race_flag[i] = 0;
                            }
                            RACESEL::Set_Race_Select_Help_();
                            video::Set_Page_Back_();
                            auto anim = (s_animation_header*)buffer::Buffer_Reload_("RACEOPT.LBX", 0, MOX::_global_cache_seg);
                            animate::Draw_(0, 0, anim);
                            video::Copy_Back_To_Off_();
                            _old_race = 0;
                            for (int j = 0; j < 14; ++j) _race_button_status[j] = 0;
                            _race_button_status[_displayed_race] = 1;
                            if (MOX::_game_type == 0 || MOX::_game_type == 1) hotkey_field = fields::Add_Hot_Key_("\x1B");
                            for (int j = 0; j < 14; ++j) {
                                field_ids[j] = fields::Add_Radio_Button_Field_((j / 7) * 126 + 351, (j % 7) * 48 + 90, "", _race_button[j], &_race_button_status[j], "", 40);
                            }
                            video::Set_Page_Off_();
                            Draw_Race_Selection_Screen_();
                            RUSS::Mox_Sync_Update_();
                            fields::Assign_Auto_Function_(Draw_Race_Selection_Screen_, 2);
                        }
                    } else {
                        struct_::Copy_Structure_(plr->traits, _race_specials[i], sizeof(plr->traits));
                        plr->race = (uint8_t)i;
                        strlcpy(plr->race_name, MOX::_race_names[i], sizeof(plr->race_name));
                        fields::Deactivate_Auto_Function_();
                        fields::Clear_Fields_();
                        if (Racial_Option_Screen_(plr) == 1) {
                            done_flag = 1;
                            _race_flag[i] = 1;
                        } else {
                            if (MOX::_game_type != 0 && MOX::_game_type != 1) {
                                Unrequest_Race_(i);
                            } else {
                                _race_flag[i] = 0;
                            }
                            allocate::Pop_Block_(MOX::_screen_seg);
                            Reload_Race_Selection_Screen_();
                            video::Set_Page_Back_();
                            auto anim = (s_animation_header*)buffer::Buffer_Reload_("RACEOPT.LBX", 0, MOX::_global_cache_seg);
                            animate::Draw_(0, 0, anim);
                            video::Copy_Back_To_Off_();
                            if (MOX::_game_type == 0 || MOX::_game_type == 1) hotkey_field = fields::Add_Hot_Key_("\x1B");
                            for (int j = 0; j < STOCK_RACE_COUNT; ++j) {
                                field_ids[j] = fields::Add_Radio_Button_Field_((j / 7) * 126 + 351, (j % 7) * 48 + 90, "", _race_button[j], &_race_button_status[j], "", 40);
                            }
                            field_ids[13] = fields::Add_Radio_Button_Field_(477, 378, "", _race_button[13], &_race_button_status[13], "", 40);
                            fields::Assign_Auto_Function_(Draw_Race_Selection_Screen_, 2);
                            RACESEL::Set_Race_Select_Help_();
                            _custom_flag = 0;
                        }
                    }
                }
            }
        }
        
        if (input_field == hotkey_field) {
            if (_custom_flag != 1) {
                fields::Clear_Fields_();
                fields::Deactivate_Auto_Function_();
                allocate::Pop_Block_(MOX::_screen_seg);
                return 0;
            }
            _custom_flag = 0;
            fields::Clear_Fields_Above_(field_ids[STOCK_RACE_TRILARIAN]);
            field_ids[13] = fields::Add_Radio_Button_Field_(477, 378, "", _race_button[13], &_race_button_status[13], "", 40);
        }
        
        if (input_field == field_ids[13]) {
            FILE* race_file = fopen_case("lastrace.rac", "rb");
            if (_custom_flag == 0) {
                _custom_flag = 1;
                fields::Clear_Fields_Above_(field_ids[STOCK_RACE_TRILARIAN]);
                if (!race_file) {
                    field_ids[13] = -1000;
                } else {
                    field_ids[13] = fields::Add_Radio_Button_Field_(477, 378, "", (s_animation_header*)_last_race_seg, &_race_button_status[13], "", 40);
                    fclose(race_file);
                }
            } else if (race_file) {
                uint8_t loaded_race;
                uint8_t discarded_traits[TRAIT_COUNT];
                fread(discarded_traits, sizeof(discarded_traits), 1, race_file);
                struct_::Copy_Structure_(plr->traits, _race_specials[13], sizeof(plr->traits));
                fread(&loaded_race, sizeof(loaded_race), 1, race_file);

                if (!Is_Valid_Last_Race_Base_Race_(loaded_race)) {
                    fclose(race_file);
                    field_ids[13] = -1000;
                    continue;
                }

                plr->race = loaded_race;
                uint8_t can_request =
                    (MOX::_game_type == 0 || MOX::_game_type == 1) ? 1 : NETSTART::Request_Race_(loaded_race);
                
                if (!can_request) {
                    fclose(race_file);
                    KEN::Ken_Get_Text_Message_(72, msg_buffer, sizeof(msg_buffer));
                    GENDRAW::Message_Box_(msg_buffer);
                    video::Set_Page_Back_();
                    auto anim = (s_animation_header*)buffer::Buffer_Reload_("RACEOPT.LBX", 0, MOX::_global_cache_seg);
                    animate::Draw_(0, 0, anim);
                    video::Copy_Back_To_Off_();
                } else {
                    fread(plr->race_name, sizeof(plr->race_name), 1, race_file);
                    fields::Deactivate_Auto_Function_();
                    fields::Clear_Fields_();
                    _old_race = loaded_race;
                    fclose(race_file);
                    if (Racial_Option_Screen_(plr) == 1) {
                        done_flag = 1;
                        _race_flag[loaded_race] = 1;
                    } else {
                        if (MOX::_game_type != 0 && MOX::_game_type != 1) {
                            Unrequest_Race_(loaded_race);
                        } else {
                            _race_flag[loaded_race] = 0;
                        }
                        allocate::Pop_Block_(MOX::_screen_seg);
                        Reload_Race_Selection_Screen_();
                        video::Set_Page_Back_();
                        auto anim = (s_animation_header*)buffer::Buffer_Reload_("RACEOPT.LBX", 0, MOX::_global_cache_seg);
                        animate::Draw_(0, 0, anim);
                        video::Copy_Back_To_Off_();
                        if (MOX::_game_type == 0 || MOX::_game_type == 1) hotkey_field = fields::Add_Hot_Key_("\x1B");
                        for (int j = 0; j < STOCK_RACE_COUNT; ++j) {
                            field_ids[j] = fields::Add_Radio_Button_Field_((j / 7) * 126 + 351, (j % 7) * 48 + 90, "", _race_button[j], &_race_button_status[j], "", 40);
                        }
                        field_ids[13] = fields::Add_Radio_Button_Field_(477, 378, "", _race_button[13], &_race_button_status[13], "", 40);
                        fields::Assign_Auto_Function_(Draw_Race_Selection_Screen_, 2);
                        RACESEL::Set_Race_Select_Help_();
                        _custom_flag = 0;
                    }
                }
            }
        }
        
        if (done_flag) break;
        
        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            video::Set_Page_Back_();
            graphics::Fill_(0, 0, 639, 479, 0);
            auto anim = (s_animation_header*)buffer::Buffer_Reload_("RACEOPT.LBX", 0, MOX::_global_cache_seg);
            animate::Draw_(0, 0, anim);
            video::Copy_Back_To_Off_();
            MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        }
        
        video::Set_Page_Off_();
        Draw_Race_Selection_Screen_();
        RUSS::Mox_Sync_Update_();
    }
    
    fields::Clear_Fields_();
    fields::Deactivate_Auto_Function_();
    allocate::Pop_Block_(MOX::_screen_seg);
    ERIC::Fast_Fade_Out_();
    return 1;
}

int16_t __cdecl Racial_Option_Screen_(s_player* player) {
    int16_t finished;
    int16_t input_id;
    int16_t saved_button_variables[10];
    char warning_text[80];
    char* player_race_name = player->race_name;

    raceopt::_name_button = 0;
    raceopt::Set_Race_Option_Help_List_();
    fields::Clear_Fields_();
    raceopt::Convert_Player_To_Flags_(player);
    raceopt::Reload_Racial_Option_Screen_();
    memcpy(raceopt::_cur_race_name, player_race_name, strlen(player_race_name) + 1);
    raceopt::Calculate_Picks_();

    fields::Assign_Auto_Function_(raceopt::Draw_Racial_Option_Screen_, 1);

    raceopt::_color_opt.colors[0] = 0x8E;
    raceopt::_color_opt.colors[1] = 0x9D;
    raceopt::_color_opt.colors[2] = 0x9C;
    raceopt::_color_opt.colors[3] = 0x9D;
    raceopt::_color_opt.colors[4] = 0x9D;
    raceopt::_color_opt.colors[5] = 0x9D;
    raceopt::_color_opt.colors[6] = 0x9D;
    raceopt::_color_opt.colors[7] = 0x9D;

    RUSS::Mox_Update_();
    fonts::Set_Font_Style_(4, &raceopt::_color_opt);
    raceopt::Add_Race_Options_Fields_();
    memcpy(saved_button_variables, raceopt::_button_variable, sizeof(saved_button_variables));
    RUSS::Mox_Update_();
    raceopt::Add_Race_Options_Fields_();
    RUSS::Mox_Update_();

    video::Set_Page_Back_();
    raceopt::Draw_Race_Option_Background_();
    video::Copy_Back_To_Off_();
    video::Set_Page_Off_();
    raceopt::Draw_Racial_Option_Screen_();
    RUSS::Mox_Sync_Update_();

    while (true) {
        finished = 0;
        MOX::_FULL_DRAW_SCREEN_FLAG = 0;
        input_id = fields::Get_Input_();

        if (input_id != 0) {
            for (int32_t group_button_index = 0; group_button_index < 0x24; ++group_button_index) {
                if (raceopt::_group_button[group_button_index] == input_id) {
                    int16_t selected_group = static_cast<int16_t>(group_button_index / 4);

                    if (selected_group == 1 && raceopt::_option_toggle[9] == 1) {
                        KEN::Ken_Get_Text_Message_(0x49, warning_text, sizeof(warning_text));
                        GENDRAW::Warning_Box_(warning_text);
                        video::Set_Page_Back_();
                        raceopt::Draw_Race_Option_Background_();
                        video::Copy_Back_To_Off_();
                        video::Set_Page_Off_();
                        raceopt::Draw_Racial_Option_Screen_();
                        RUSS::Mox_Sync_Update_();
                        raceopt::_button_variable[selected_group] = 0;
                    } else {
                        int16_t selected_value = static_cast<int16_t>(group_button_index % 4);

                        if (saved_button_variables[selected_group] == selected_value) {
                            raceopt::_button_variable[selected_group] = 0;
                            raceopt::Calculate_Picks_();
                        } else {
                            int32_t cost_index = group_button_index - ((group_button_index + 3) / 4);
                            int32_t pick_total =
                                    static_cast<int32_t>(raceopt::_ability_costs[cost_index]) +
                                    static_cast<int32_t>(raceopt::_negative_picks);

                            if (pick_total < -10) {
                                KEN::Ken_Get_Text_Message_(0x4A, warning_text, sizeof(warning_text));
                                GENDRAW::Warning_Box_(warning_text);
                                video::Set_Page_Back_();
                                raceopt::Draw_Race_Option_Background_();
                                video::Copy_Back_To_Off_();
                                video::Set_Page_Off_();
                                raceopt::Draw_Racial_Option_Screen_();
                                RUSS::Mox_Sync_Update_();
                                raceopt::_button_variable[selected_group] = saved_button_variables[selected_group];
                                raceopt::Calculate_Picks_();
                            } else {
                                raceopt::_button_variable[selected_group] = selected_value;
                            }
                        }
                    }
                }
            }

            for (int32_t group_button_index = 0x25; group_button_index < 0x29; ++group_button_index) {
                if (raceopt::_group_button[group_button_index] == input_id) {
                    raceopt::Calculate_Picks_();

                    if (group_button_index - raceopt::_button_variable[9] == 0x25) {
                        raceopt::_button_variable[9] = static_cast<int16_t>(group_button_index - 0x25);
                    } else {
                        int32_t pick_total =
                                static_cast<int32_t>(raceopt::_ability_costs[group_button_index - 10]) +
                                static_cast<int32_t>(raceopt::_negative_picks);

                        if (pick_total < -10) {
                            KEN::Ken_Get_Text_Message_(0x4A, warning_text, sizeof(warning_text));
                            GENDRAW::Warning_Box_(warning_text);
                            video::Set_Page_Back_();
                            raceopt::Draw_Race_Option_Background_();
                            video::Copy_Back_To_Off_();
                            video::Set_Page_Off_();
                            raceopt::Draw_Racial_Option_Screen_();
                            RUSS::Mox_Sync_Update_();
                            raceopt::_button_variable[9] = saved_button_variables[9];
                            raceopt::Calculate_Picks_();
                        } else {
                            raceopt::_button_variable[9] = static_cast<int16_t>(group_button_index - 0x25);
                        }
                    }
                }
            }

            for (int32_t option_index = 0; option_index < 0x16; ++option_index) {
                if (raceopt::_option_button[option_index] == input_id) {
                    raceopt::_option_toggle[option_index] =
                            static_cast<int16_t>(1 - raceopt::_option_toggle[option_index]);

                    if (raceopt::_option_toggle[option_index] == 0) {
                        raceopt::Calculate_Picks_();
                    } else {
                        raceopt::Calculate_Picks_();

                        if (raceopt::_negative_picks < -10) {
                            KEN::Ken_Get_Text_Message_(0x4A, warning_text, sizeof(warning_text));
                            GENDRAW::Warning_Box_(warning_text);
                            video::Set_Page_Back_();
                            raceopt::Draw_Race_Option_Background_();
                            video::Copy_Back_To_Off_();
                            video::Set_Page_Off_();
                            raceopt::Draw_Racial_Option_Screen_();
                            RUSS::Mox_Sync_Update_();
                            raceopt::_option_toggle[option_index] = 0;
                        }

                        if (option_index == 5) {
                            raceopt::_option_toggle[6] = 0;
                        } else if (option_index == 6) {
                            raceopt::_option_toggle[5] = 0;
                        } else if (option_index == 0) {
                            raceopt::_option_toggle[1] = 0;
                        } else if (option_index == 1) {
                            raceopt::_option_toggle[0] = 0;
                        } else if (option_index == 9) {
                            raceopt::_option_toggle[8] = 0;
                            raceopt::_button_variable[1] = 0;
                        } else if (option_index == 8) {
                            raceopt::_option_toggle[9] = 0;
                        } else if (option_index == 10) {
                            raceopt::_option_toggle[11] = 0;
                        } else if (option_index == 11) {
                            raceopt::_option_toggle[10] = 0;
                        } else if (option_index == 12) {
                            raceopt::_option_toggle[13] = 0;
                        } else if (option_index == 13) {
                            raceopt::_option_toggle[12] = 0;
                        }
                    }
                }
            }

            if (input_id == raceopt::_defalt) {
                for (int32_t i = 0; i < 9; ++i) {
                    raceopt::_button_variable[i] = 0;
                }

                raceopt::_button_variable[9] = 1;

                for (int32_t i = 0; i < 0x16; ++i) {
                    raceopt::_option_toggle[i] = 0;
                }
            }

            if (input_id == raceopt::_exit_ken) {
                fields::Clear_Fields_();
                fields::Deactivate_Auto_Function_();
                allocate::Pop_Block_(MOX::_screen_seg);
                return 0;
            }

            if (input_id == raceopt::_accept) {
                if (raceopt::_picks < 0) {
                    KEN::Ken_Get_Text_Message_(0x4B, warning_text, sizeof(warning_text));
                    GENDRAW::Warning_Box_(warning_text);
                    video::Set_Page_Back_();
                    raceopt::Draw_Race_Option_Background_();
                    video::Copy_Back_To_Off_();
                    video::Set_Page_Off_();
                    raceopt::Draw_Racial_Option_Screen_();
                    RUSS::Mox_Sync_Update_();
                } else {
                    raceopt::Convert_Flags_To_Player_(player);
                    fields::Assign_Auto_Function_(SCORE::nullsub, 2);

                    video::Set_Page_Back_();
                    animate::Draw_(0, 0, (s_animation_header*)buffer::Buffer_Reload_("RACEOPT.LBX", 4, MOX::_global_cache_seg));
                    animate::Draw_(0, 0x1AE, (s_animation_header*)buffer::Buffer_Reload_("RACEOPT.LBX", 6, MOX::_global_cache_seg));
                    video::Set_Page_Off_();

                    int16_t popup_result;
                    while (true) {
                        fields::Deactivate_Help_List_();
                        popup_result = RACESEL::Naming_Popup_(player->name, sizeof(player->name), static_cast<int16_t>(player->race));

                        video::Set_Page_Back_();
                        animate::Draw_(0, 0, (s_animation_header*)buffer::Buffer_Reload_("RACEOPT.LBX", 4, MOX::_global_cache_seg));
                        animate::Draw_(0, 0x1AE, (s_animation_header*)buffer::Buffer_Reload_("RACEOPT.LBX", 6, MOX::_global_cache_seg));
                        video::Copy_Back_To_Off_();
                        RUSS::Mox_Sync_Update_();

                        if (popup_result != 1) {
                            break;
                        }

                        popup_result = static_cast<int16_t>(
                                popup_result + RACESEL::Flag_Screen_((uint8_t*)&player->color));

                        if (popup_result != 1) {
                            break;
                        }
                    }

                    if (popup_result == 2) {
                        FILE* race_file = fopen_case("lastrace.rac", "wb");
                        if (race_file != nullptr) {
                            fwrite(player->traits, sizeof(player->traits), 1, race_file);
                            fwrite(&player->race, sizeof(player->race), 1, race_file);
                            memcpy(player_race_name, raceopt::_cur_race_name, strlen(raceopt::_cur_race_name) + 1);
                            fwrite(player_race_name, sizeof(player->race_name), 1, race_file);
                            fclose(race_file);
                        }

                        MOX::_current_screen = MOX::_return_screen;
                        ERIC::Fast_Fade_Out_();
                        finished = 1;
                    } else {
                        raceopt::Set_Race_Option_Help_List_();
                        raceopt::_name_button = 0;
                        fields::Assign_Auto_Function_(raceopt::Draw_Racial_Option_Screen_, 2);
                    }
                }
            }

            memcpy(saved_button_variables, raceopt::_button_variable, sizeof(saved_button_variables));
            raceopt::Calculate_Picks_();
        }

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            video::Set_Page_Back_();
            raceopt::Draw_Race_Option_Background_();
            video::Copy_Back_To_Off_();
        }

        if (finished != 0) {
            fields::Deactivate_Help_List_();
            fields::Deactivate_Auto_Function_();
            allocate::Pop_Block_(MOX::_screen_seg);
            return 1;
        }

        raceopt::Add_Race_Options_Fields_();
        video::Set_Page_Off_();
        raceopt::Draw_Racial_Option_Screen_();
        RUSS::Mox_Sync_Update_();
    }
}

/**
 * @brief Cancels a race request for the local player and notifies the host.
 * @param race_idx The index of the race being unrequested.
 */
void __cdecl Unrequest_Race_(uint8_t race_idx) {
    NETMOX::_net.race_selection_changed = 1;
    RACESEL::_race_flag[race_idx] = 0;

    // Send network message to the host about updated race selection status
    NETMOX::Mox_Send_Message_(NETMOX::_net.host_election_id, NET_PACKET_PLAYER_RACE_DESELECT_REQUEST, &race_idx, sizeof(race_idx));
}

void __cdecl Draw_Naming_Popup_() {
  s_animation_header* anim;

  if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
    video::Set_Page_Off_();
    graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
    anim = (s_animation_header*)buffer::Buffer_Reload_("RACEOPT.LBX", 0, MOX::_global_cache_seg);
    animate::Draw_(0, 0, anim);
  }
}

void __cdecl Message_Build_(int16_t* message_count, int16_t split_at_count, char* output_text, uint32_t output_text_size) {
    int16_t next_count = static_cast<int16_t>(*message_count + 1);
    *message_count = next_count;

    if (split_at_count == next_count) {
        if (next_count > 1) {
            char separator_text[80];

            KEN::Ken_Get_Text_Message_(0x68, separator_text, sizeof(separator_text));
            strlcat(output_text, separator_text, output_text_size);
        }
    } else if (next_count > 1) {
        strlcat(output_text, DESIGN::s_comma_space, output_text_size);
    }
}

int8_t __cdecl Naming_Popup_(char* output_name, uint32_t output_name_size, int16_t race_idx) {
    char original_name[20];
    char popup_text[80];

    fields::Clear_Fields_();
    RUSS::Mox_Sync_Update_();
    fields::Assign_Auto_Function_(RACESEL::Draw_Naming_Popup_, 1);
    allocate::Push_Block_(MOX::_screen_seg);

    RACESEL::_temp_ruler_name = (char*)farload::Far_Reload_Next_Data_(
        "RACENAME.LBX",
        0,
        MOX::_screen_seg,
        (int32_t)game_random::Random_(8) + (int32_t)race_idx * 8 - 1,
        1,
        sizeof(original_name)
    );

    strlcpy(original_name, RACESEL::_temp_ruler_name, sizeof(original_name));

    KEN::Ken_Get_Text_Message_(0x6A, popup_text, sizeof(popup_text));

    const int16_t center_y = namestar::Star_Name_Popup_Screen_Center_Y_();
    const int16_t center_x = namestar::Star_Name_Popup_Screen_Center_X_();

    const int8_t popup_result = namestar::Input_Box_Popup_Animated_(
        RACESEL::_temp_ruler_name,
        sizeof(original_name),
        sizeof(original_name),
        0x66,
        popup_text,
        center_x,
        center_y
    );

    strlcpy(
        output_name,
        (RACESEL::_temp_ruler_name[0] != '\0') ? RACESEL::_temp_ruler_name : original_name,
        output_name_size
    );

    allocate::Pop_Block_(MOX::_screen_seg);
    fields::Clear_Fields_();

    return popup_result;
}

void __cdecl Draw_Flag_Screen_() {
    s_animation_header* animation =
        (s_animation_header*)buffer::Buffer_Reload_("racesel.lbx", 0x20, MOX::_global_cache_seg);

    animate::Draw_(0x37, 0x43, animation);

    void* cache_seg = MOX::_global_cache_seg;

    for (int16_t race_index = 0; race_index < MAX_PLAYERS; ++race_index) {
        if (RACESEL::_color_flag[race_index] == 0) {
            int16_t x_adjust = 0;
            if (race_index == 3 || race_index == 7) {
                x_adjust = -1;
            }

            animation = (s_animation_header*)buffer::Buffer_Reload_(
                "racesel.lbx",
                static_cast<int32_t>(race_index) + static_cast<int32_t>(RACESEL::_old_race) * MAX_PLAYERS + 0x22,
                cache_seg);

            const int16_t y =
                static_cast<int16_t>((race_index / 4) * 0x88 + 0x91);
            const int16_t x =
                static_cast<int16_t>(((race_index & 3) * 0x7c) + x_adjust + 0x60);

            animate::Draw_(x, y, animation);
            cache_seg = MOX::_global_cache_seg;
        }
    }
}

int16_t __cdecl Flag_Screen_(uint8_t* selected_color) {
    int16_t cancel_field_id;
    int16_t selection_made;
    int16_t field_ids[MAX_PLAYERS];
    char message_text[0x50];

    RUSS::Mox_Sync_Update_();

    s_animation_header* palette = (s_animation_header*)buffer::Buffer_Reload_(
        "racesel.lbx",
        0x20,
        MOX::_global_cache_seg
    );
    animate::Draw_Palette_(palette);

    RUSS::Mox_Sync_Update_();
    fields::Assign_Auto_Function_(RACESEL::Draw_Flag_Screen_, 2);
    RACESEL::Set_Flag_Screen_Help_();

    selection_made = 0;

    while (true) {
        fields::Clear_Fields_();

        for (int16_t flag_index = 0; flag_index < MAX_PLAYERS; ++flag_index) {
            int16_t column_adjust = 0;
            if (flag_index == 3 || flag_index == 7) {
                column_adjust = -1;
            }

            if (RACESEL::_color_flag[flag_index] == 0) {
                const int16_t row_offset = static_cast<int16_t>((flag_index / 4) * 0x88);
                const int16_t column_offset = static_cast<int16_t>((flag_index % 4) * 0x7C + column_adjust);

                field_ids[flag_index] = fields::Add_Hidden_Field_(
                    static_cast<int16_t>(column_offset + 0x60),
                    static_cast<int16_t>(row_offset + 0x90),
                    static_cast<int16_t>(column_offset + 0xB3),
                    static_cast<int16_t>(row_offset + 0xF2),
                    "",
                    0x29
                );
            } else {
                field_ids[flag_index] = -1000;
            }
        }

        cancel_field_id = fields::Add_Hot_Key_("\x1B");
        const int16_t input_field_id = fields::Get_Input_();

        for (int16_t flag_index = 0; flag_index < MAX_PLAYERS; ++flag_index) {
            if (field_ids[flag_index] == input_field_id) {
                uint8_t accepted;

                if (MOX::_game_type == 0 || MOX::_game_type == 1) {
                    accepted = 1;
                } else {
                    accepted = NETSTART::Request_Banner_Color_(static_cast<uint8_t>(flag_index));
                }

                if (accepted != 0) {
                    selection_made = 1;
                    *selected_color = static_cast<uint8_t>(flag_index);
                    RACESEL::_color_flag[flag_index] = 1;
                } else {
                    KEN::Ken_Get_Text_Message_(0x69, message_text, sizeof(message_text));
                    GENDRAW::Message_Box_(message_text);
                }
            }
        }

        if (cancel_field_id == input_field_id) {
            fields::Clear_Fields_();
            fields::Deactivate_Auto_Function_();
            MOX::_hot_seat_flag = 0;
            return 0;
        }

        if (selection_made != 0) {
            fields::Clear_Fields_();
            fields::Deactivate_Auto_Function_();
            MOX::_cmbt_mouse_list_clock.icon = 7;
            mouse::Set_Mouse_List_(&MOX::_cmbt_mouse_list_clock, 1);
            video::Set_Page_Off_();
            RACESEL::Draw_Flag_Screen_();
            RUSS::Mox_Sync_Update_();
            return 1;
        }

        video::Set_Page_Off_();
        RACESEL::Draw_Flag_Screen_();
        RUSS::Mox_Sync_Update_();
    }
}

void __cdecl Set_Flag_Screen_Help_() {
    MOX::_help_entry_list[0].help_id = 0x02A4;
    MOX::_help_entry_list[0].x1 = 0;
    MOX::_help_entry_list[0].x2 = 0x027F;
    MOX::_help_entry_list[0].y2 = 0x01DF;
    MOX::_help_entry_list[0].y1 = 0;
    fields::Set_Help_List_(MOX::_help_entry_list, 1);
}

void __cdecl Draw_Race_Selection_Screen_(void) {
    s_colors shadow_colors = {{0x78, 0x7c, 0x7c, 0x7c, 0x7c, 0x7c, 0x7c, 0x7c}};
    int16_t line_index = 0;
    int16_t total_lines = 0;
    char bonus_text[8];
    char paragraph[200];
    char text_message[100];
    s_animation_header* animation;

    paragraph[0] = '\0';

    if (RACESEL::_custom_flag == 0) {
        animation = (s_animation_header*)buffer::Buffer_Reload_("RACESEL.LBX", 0x21, MOX::_global_cache_seg);
        animate::Draw_(0x16e, 0x34, animation);
    } else {
        animation = (s_animation_header*)buffer::Buffer_Reload_("RACESEL.LBX", 0x1d, MOX::_global_cache_seg);
        animate::Draw_(0x16f, 0x30, animation);
    }

    if (RACESEL::_old_race != RACESEL::_displayed_race) {
        RACESEL::_race_button_status[static_cast<int32_t>(RACESEL::_old_race)] = 0;
        RACESEL::_race_button_status[static_cast<int32_t>(RACESEL::_displayed_race)] = 1;
    }

    if (RACESEL::_race_flag[static_cast<int32_t>(RACESEL::_displayed_race)] == 0) {
        animation = (s_animation_header*)buffer::Buffer_Reload_(
                "RACESEL.LBX",
                static_cast<int32_t>(RACESEL::_displayed_race) + 0x0f,
                MOX::_global_cache_seg);
        animate::Draw_(0x36, 0x3f, animation);
    } else {
        fonts::Set_Font_Style_Shadow_Down_(4, &shadow_colors);
        KEN::Ken_Get_Text_Message_(0x48, text_message, sizeof(text_message));
        BILL::_Print_Paragraph_(0x3c, 0xaf, 0x118, text_message, 2);
    }

    RACESEL::_old_race = RACESEL::_displayed_race;

    if ((((RACESEL::_displayed_race == 13) && (RACESEL::_race_specials[13] != nullptr)) ||
         ((RACESEL::_displayed_race != 13) &&
          (RACESEL::_race_flag[static_cast<int32_t>(RACESEL::_displayed_race)] == 0))) &&
        (((RACESEL::_custom_flag == 0) && (RACESEL::_displayed_race != 13)) ||
         ((RACESEL::_custom_flag == 1) && (RACESEL::_displayed_race == 13)))) {
        uint8_t* const race_specials = RACESEL::_race_specials[static_cast<int32_t>(RACESEL::_displayed_race)];

        line_index = 0;
        total_lines = 0;

        for (int16_t ability_id = TRAIT_POP_GROWTH_BONUS; ability_id < TRAIT_LOW_G_WORLD; ++ability_id) {
            if (race_specials[ability_id] != 0) {
                ++total_lines;
            }
        }

        for (int16_t ability_id = TRAIT_LOW_G_WORLD; ability_id < TRAIT_COUNT; ++ability_id) {
            if (race_specials[ability_id] != 0) {
                ++total_lines;
            }
        }

        ++total_lines;

        for (int16_t ability_id = TRAIT_POP_GROWTH_BONUS; ability_id < TRAIT_LOW_G_WORLD; ++ability_id) {
            if (race_specials[ability_id] != 0) {
                RACESEL::Message_Build_(&line_index, total_lines, paragraph, sizeof(paragraph));
                strlcat(paragraph, MOX::_special_ability_names[ability_id], sizeof(paragraph));

                const int32_t special_level = static_cast<int8_t>(race_specials[ability_id]);
                const int8_t bonus =
                        MOX::_special_bonus[ability_id - 1][special_level - 1];

                if (ability_id == 5) {
                    if (bonus == 1) {
                        strlcpy(bonus_text, "1/2", sizeof(bonus_text));
                    } else {
                        strlcpy(bonus_text, "1", sizeof(bonus_text));
                    }
                } else if (ability_id == 2) {
                    if (bonus == 1) {
                        strlcpy(bonus_text, "1/2", sizeof(bonus_text));
                    } else {
                        snprintf(bonus_text, sizeof(bonus_text), "%i", static_cast<int32_t>(bonus / 2));
                    }
                } else {
                    snprintf(bonus_text, sizeof(bonus_text), "%i", static_cast<int32_t>(bonus));
                }

                if (bonus > 0) {
                    strlcat(paragraph, "+", sizeof(paragraph));
                }

                strlcat(paragraph, bonus_text, sizeof(paragraph));
            }
        }

        for (int16_t ability_id = TRAIT_LOW_G_WORLD; ability_id < TRAIT_COUNT; ++ability_id) {
            if (race_specials[ability_id] == 1) {
                RACESEL::Message_Build_(&line_index, total_lines, paragraph, sizeof(paragraph));
                strlcat(paragraph, MOX::_special_ability_names[ability_id], sizeof(paragraph));
            }
        }

        RACESEL::Message_Build_(&line_index, total_lines, paragraph, sizeof(paragraph));
        strlcat(paragraph, MOX::_government_labels[static_cast<int8_t>(race_specials[0])], sizeof(paragraph));

        fonts::Set_Font_Style_Shadow_Down_(2, &shadow_colors);
        fonts::Set_Outline_Color_(0, 0);
        BILL::_Print_Paragraph_(0x32, 0x187, 0x127, paragraph, 0);

        if (RACESEL::_displayed_race == 13) {
            fonts::Set_Font_Style_Shadow_Down_(4, &shadow_colors);
            fonts::Set_Outline_Color_(0, 0);
            fonts::Print_Centered_(0xc5, 0x2a, RACESEL::_last_race_name);
        }
    }

    animation = (s_animation_header*)buffer::Buffer_Reload_("RACESEL.LBX", 0, MOX::_global_cache_seg);
    animate::Draw_(0x30, 0x3a, animation);
}

}