#include "pch.h"
#include "fields.h"

namespace RACESEL {

void __cdecl Set_Race_Select_Help_() {
    MOX::_help_entry_list[0] = 630;
    MOX::_help_entry_list[1] = 351;
    MOX::_help_entry_list[2] = 90;
    MOX::_help_entry_list[3] = 473;
    MOX::_help_entry_list[4] = 0x86;

    MOX::_help_entry_list[5] = 631;
    MOX::_help_entry_list[6] = 351;
    MOX::_help_entry_list[7] = 0x8a;
    MOX::_help_entry_list[8] = 473;
    MOX::_help_entry_list[9] = 0xb6;

    MOX::_help_entry_list[10] = 0x278;
    MOX::_help_entry_list[11] = 351;
    MOX::_help_entry_list[12] = 0xba;
    MOX::_help_entry_list[13] = 473;
    MOX::_help_entry_list[14] = 0xe6;

    MOX::_help_entry_list[15] = 0x279;
    MOX::_help_entry_list[16] = 351;
    MOX::_help_entry_list[17] = 0xea;
    MOX::_help_entry_list[18] = 473;
    MOX::_help_entry_list[19] = 0x116;

    MOX::_help_entry_list[20] = 0x27a;
    MOX::_help_entry_list[21] = 351;
    MOX::_help_entry_list[22] = 0x11a;
    MOX::_help_entry_list[23] = 473;
    MOX::_help_entry_list[24] = 0x146;

    MOX::_help_entry_list[25] = 0x27b;
    MOX::_help_entry_list[26] = 351;
    MOX::_help_entry_list[27] = 0x14a;
    MOX::_help_entry_list[28] = 473;
    MOX::_help_entry_list[29] = 0x176;

    MOX::_help_entry_list[30] = 0x27c;
    MOX::_help_entry_list[31] = 351;
    MOX::_help_entry_list[32] = 0x17a;
    MOX::_help_entry_list[33] = 473;
    MOX::_help_entry_list[34] = 0x1a6;

    MOX::_help_entry_list[35] = 0x27d;
    MOX::_help_entry_list[36] = 0x1dd;
    MOX::_help_entry_list[37] = 90;
    MOX::_help_entry_list[38] = 0x257;
    MOX::_help_entry_list[39] = 0x86;

    MOX::_help_entry_list[40] = 0x27e;
    MOX::_help_entry_list[41] = 0x1dd;
    MOX::_help_entry_list[42] = 0x8a;
    MOX::_help_entry_list[43] = 0x257;
    MOX::_help_entry_list[44] = 0xb6;

    MOX::_help_entry_list[45] = 0x27f;
    MOX::_help_entry_list[46] = 0x1dd;
    MOX::_help_entry_list[47] = 0xba;
    MOX::_help_entry_list[48] = 0x257;
    MOX::_help_entry_list[49] = 0xe6;

    MOX::_help_entry_list[50] = 0x280;
    MOX::_help_entry_list[51] = 0x1dd;
    MOX::_help_entry_list[52] = 0xea;
    MOX::_help_entry_list[53] = 0x257;
    MOX::_help_entry_list[54] = 0x116;

    MOX::_help_entry_list[55] = 0x281;
    MOX::_help_entry_list[56] = 0x1dd;
    MOX::_help_entry_list[57] = 0x11a;
    MOX::_help_entry_list[58] = 0x257;
    MOX::_help_entry_list[59] = 0x146;

    MOX::_help_entry_list[60] = 0x282;
    MOX::_help_entry_list[61] = 0x1dd;
    MOX::_help_entry_list[62] = 0x14a;
    MOX::_help_entry_list[63] = 0x257;
    MOX::_help_entry_list[64] = 0x176;

    MOX::_help_entry_list[65] = 0x283;
    MOX::_help_entry_list[66] = 0x1dd;
    MOX::_help_entry_list[67] = 0x17a;
    MOX::_help_entry_list[68] = 0x257;
    MOX::_help_entry_list[69] = 0x1a6;

    MOX::_help_entry_list[70] = 0x284;
    MOX::_help_entry_list[71] = 0;
    MOX::_help_entry_list[72] = 0;
    MOX::_help_entry_list[73] = 0x27f;
    MOX::_help_entry_list[74] = 0x1df;

    fields::Set_Help_List_(MOX::_help_entry_list, 15);
}

void __cdecl Reload_Race_Selection_Screen_() {
    uint8_t dummy_byte;

    allocate::Push_Block_(MOX::_screen_seg);

    for (int i = 0; i < 13; ++i) {
        _race_specials[i] = (uint8_t*)farload::Far_Reload_Next_Data_(s_RACESTUF_LBX, 7, MOX::_screen_seg, i, 1, 0x1F);
        MOX::Mox_Update_();
    }

    FILE* file = _wfopen(s_lastrace_rac, s_rb_0055117c);
    if (file != nullptr) {
        // Load template for custom race (LBX entry 12) into the 14th slot
        _race_specials[13] = (uint8_t*)farload::Far_Reload_Next_Data_(s_RACESTUF_LBX, 7, MOX::_screen_seg, 12, 1, 0x1F);

        // Overwrite template with saved race data
        _fread(_race_specials[13], 31, 1, file);
        Convert_Custom_Race_Flags_(_race_specials[13]);
        MOX::Mox_Update_();

        _fread(&dummy_byte, 1, 1, file);
        _fread(_last_race_name, 15, 1, file);

        MOX::Mox_Update_();
        _fclose(file);
    }

    for (int i = 0; i < 14; ++i) {
        _race_button[i] = (s_animation_header*)farload::Far_Reload_Next_(s_RACESEL_LBX, i + 1, MOX::_screen_seg);
        MOX::Mox_Update_();
    }

    _last_race_seg = (s_animation_header*)farload::Far_Reload_Next_(s_patch13_lbx, 1, MOX::_screen_seg);
}

void __cdecl Draw_Race_Selection_Screen_() {
    using Fn = void(__cdecl*)();
    reinterpret_cast<Fn>(0x004982E0)();
}

uint8_t __cdecl Naming_Popup_(char* name, int16_t race_idx) {
    using Fn = uint8_t(__cdecl*)(char*, int16_t);
    return reinterpret_cast<Fn>(0x00499900)(name, race_idx);
}

int16_t __cdecl Flag_Screen_(char* color_ptr) {
    using Fn = int16_t(__cdecl*)(char*);
    return reinterpret_cast<Fn>(0x004996C0)(color_ptr);
}

int16_t __cdecl Racial_Option_Screen_(s_player* plr) {
    using Fn = int16_t(__cdecl*)(s_player*);
    return reinterpret_cast<Fn>(0x00438EF0)(plr);
}

short Race_Selection_Screen_(s_player* plr) {
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
    _displayed_race = 3;
    for (int i = 0; i < 14; ++i) {
        _race_button_status[i] = 0;
    }
    _race_button_status[3] = 1;
    
    Reload_Race_Selection_Screen_();
    RACESEL::Set_Race_Select_Help_();
    video::Set_Page_Back_();
    auto raceopt_anim = (s_animation_header*)Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
    MOX::Mox_Update_();
    animate::Draw_(0, 0, raceopt_anim);
    video::Copy_Back_To_Off_();
    
    if (MOX::_game_type == 0 || MOX::_game_type == 1) {
        hotkey_field = fields::Add_Hot_Key_(s_space_00551120);
    } else {
        hotkey_field = -1000;
    }
    
    MOX::Mox_Update_();
    for (int i = 0; i < 14; ++i) {
        field_ids[i] = fields::Add_Radio_Button_Field_(
            (i / 7) * 126 + 351, (i % 7) * 48 + 90, s_empty_00551100,
            _race_button[i], &_race_button_status[i], s_empty_00551100, 40
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
        
        for (int i = 0; i < 13; ++i) {
            if (input_field == field_ids[i] && _race_flag[i] == 0) {
                bool can_request = (MOX::_game_type == 0 || MOX::_game_type == 1) ? true : NETSTART::Request_Race_(i);
                
                if (!can_request) {
                    KEN::Ken_Get_Text_Message_(72 /* 0x48 */, msg_buffer, 100);
                    GENDRAW::Message_Box_(msg_buffer);
                    video::Set_Page_Back_();
                    auto anim = (s_animation_header*)Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
                    animate::Draw_(0, 0, anim);
                    video::Copy_Back_To_Off_();
                } else {
                    struct_::Copy_Structure_(plr->traits, _race_specials[i], 31);
                    plr->race = i;
                    strcpy(plr->race_name, MOX::_race_names[i]);
                    
                    if (_custom_flag == 0) {
                        timer::Mark_Time_();
                        timer::Release_Time_(5);
                        fields::Clear_Fields_();
                        fields::Deactivate_Help_List_();
                        
                        int screen_state;
                        do {
                            screen_state = Naming_Popup_(plr->name, i);
                            video::Set_Page_Back_();
                            auto anim = (s_animation_header*)Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
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
                            for (int trait_idx = 1; trait_idx < 10; ++trait_idx) {
                                if (plr->traits[trait_idx] != 0) {
                                    plr->traits[trait_idx] = MOX::_special_bonus[trait_idx - 1][plr->traits[trait_idx] - 1];
                                }
                            }
                            done_flag = 1;
                            _race_flag[i] = 1;
                        } else {
                            if (MOX::_game_type != 0 && MOX::_game_type != 1) {
                                Unrequest_Race_(i);
                            }
                            RACESEL::Set_Race_Select_Help_();
                            video::Set_Page_Back_();
                            auto anim = (s_animation_header*)Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
                            animate::Draw_(0, 0, anim);
                            video::Copy_Back_To_Off_();
                            _old_race = 0;
                            for (int j = 0; j < 14; ++j) _race_button_status[j] = 0;
                            _race_button_status[_displayed_race] = 1;
                            if (MOX::_game_type == 0 || MOX::_game_type == 1) hotkey_field = fields::Add_Hot_Key_(s_space_00551120);
                            for (int j = 0; j < 14; ++j) {
                                field_ids[j] = fields::Add_Radio_Button_Field_((j / 7) * 126 + 351, (j % 7) * 48 + 90, s_empty_00551100, _race_button[j], &_race_button_status[j], s_empty_00551100, 40);
                            }
                            video::Set_Page_Off_();
                            Draw_Race_Selection_Screen_();
                            RUSS::Mox_Sync_Update_();
                            fields::Assign_Auto_Function_(Draw_Race_Selection_Screen_, 2);
                        }
                    } else {
                        fields::Deactivate_Auto_Function_();
                        fields::Clear_Fields_();
                        if (Racial_Option_Screen_(plr) == 1) {
                            done_flag = 1;
                            _race_flag[i] = 1;
                        } else {
                            if (MOX::_game_type != 0 && MOX::_game_type != 1) Unrequest_Race_(i);
                            allocate::Pop_Block_(MOX::_screen_seg);
                            Reload_Race_Selection_Screen_();
                            video::Set_Page_Back_();
                            auto anim = (s_animation_header*)Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
                            animate::Draw_(0, 0, anim);
                            video::Copy_Back_To_Off_();
                            if (MOX::_game_type == 0 || MOX::_game_type == 1) hotkey_field = fields::Add_Hot_Key_(s_space_00551120);
                            for (int j = 0; j < 13; ++j) {
                                field_ids[j] = fields::Add_Radio_Button_Field_((j / 7) * 126 + 351, (j % 7) * 48 + 90, s_empty_00551100, _race_button[j], &_race_button_status[j], s_empty_00551100, 40);
                            }
                            field_ids[13] = fields::Add_Radio_Button_Field_(477, 378, s_empty_00551100, (s_animation_header*)_last_race_seg, &_race_button_status[13], s_empty_00551100, 40);
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
            fields::Clear_Fields_Above_(field_ids[12]);
            field_ids[13] = fields::Add_Radio_Button_Field_(477, 378, s_empty_00551100, _race_button[13], &_race_button_status[13], s_empty_00551100, 40);
        }
        
        if (input_field == field_ids[13]) {
            FILE* f = _wfopen(s_lastrace_rac, s_rb_0055117C);
            if (_custom_flag == 0) {
                _custom_flag = 1;
                fields::Clear_Fields_Above_(field_ids[12]);
                if (!f) {
                    field_ids[13] = -1000;
                } else {
                    field_ids[13] = fields::Add_Radio_Button_Field_(477, 378, s_empty_00551100, (s_animation_header*)_last_race_seg, &_race_button_status[13], s_empty_00551100, 40);
                    _fclose(f);
                }
            } else if (f) {
                uint8_t loaded_race;
                uint8_t temp_traits[31];
                _fread(temp_traits, 31, 1, f);
                struct_::Copy_Structure_(plr->traits, _race_specials[13], 31);
                _fread(&loaded_race, 1, 1, f);
                plr->race = loaded_race;
                bool can_request = (MOX::_game_type == 0 || MOX::_game_type == 1) ? true : NETSTART::Request_Race_(loaded_race);
                
                if (!can_request) {
                    _fclose(f);
                    KEN::Ken_Get_Text_Message_(72, msg_buffer, 100);
                    GENDRAW::Message_Box_(msg_buffer);
                    video::Set_Page_Back_();
                    auto anim = (s_animation_header*)Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
                    animate::Draw_(0, 0, anim);
                    video::Copy_Back_To_Off_();
                } else {
                    _fread(plr->race_name, 15, 1, f);
                    fields::Deactivate_Auto_Function_();
                    fields::Clear_Fields_();
                    _old_race = loaded_race;
                    _fclose(f);
                    if (Racial_Option_Screen_(plr) == 1) {
                        done_flag = 1;
                        _race_flag[loaded_race] = 1;
                    } else {
                        if (MOX::_game_type != 0 && MOX::_game_type != 1) Unrequest_Race_(loaded_race);
                        allocate::Pop_Block_(MOX::_screen_seg);
                        Reload_Race_Selection_Screen_();
                        video::Set_Page_Back_();
                        auto anim = (s_animation_header*)Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
                        animate::Draw_(0, 0, anim);
                        video::Copy_Back_To_Off_();
                        if (MOX::_game_type == 0 || MOX::_game_type == 1) hotkey_field = fields::Add_Hot_Key_(s_space_00551120);
                        for (int j = 0; j < 13; ++j) {
                            field_ids[j] = fields::Add_Radio_Button_Field_((j / 7) * 126 + 351, (j % 7) * 48 + 90, s_empty_00551100, _race_button[j], &_race_button_status[j], s_empty_00551100, 40);
                        }
                        field_ids[13] = fields::Add_Radio_Button_Field_(477, 378, s_empty_00551100, _race_button[13], &_race_button_status[13], s_empty_00551100, 40);
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
            auto anim = (s_animation_header*)Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
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

/**
 * @brief Cancels a race request for the local player and notifies the host.
 * @param race_idx The index of the race being unrequested.
 */
void __cdecl Unrequest_Race_(uint8_t race_idx) {
    NETMOX::_net.race_selection_changed = 1;
    RACESEL::_race_flag[race_idx] = 0;

    // Send network message to the host about updated race selection status
    NETMOX::Mox_Send_Message_(NETMOX::_net.host_election_id, 0x3e /* NET_MSG_RACE_SELECTION_STATUS */, &race_idx, 1);
}

}