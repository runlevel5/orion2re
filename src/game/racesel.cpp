#include "pch.h"

namespace RACESEL {

void __cdecl Reload_Race_Selection_Screen_() {
    uint8_t dummy_byte;

    allocate::Push_Block_(MOX::_screen_seg);

    for (int i = 0; i < 13; ++i) {
        _race_specials[i] = (uint8_t*)MOX::farload::Far_Reload_Next_Data_(s_RACESTUF_LBX, 7, MOX::_screen_seg, i, 1, 0x1F);
        MOX::Mox_Update_();
    }

    FILE* file = _wfopen(s_lastrace_rac, s_rb_0055117c);
    if (file != nullptr) {
        // Load template for custom race (LBX entry 12) into the 14th slot
        _race_specials[13] = (uint8_t*)MOX::farload::Far_Reload_Next_Data_(s_RACESTUF_LBX, 7, MOX::_screen_seg, 12, 1, 0x1F);

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
        _race_button[i] = (s_animation_header*)MOX::farload::Far_Reload_Next_(s_RACESEL_LBX, i + 1, MOX::_screen_seg);
        MOX::Mox_Update_();
    }

    _last_race_seg = (s_animation_header*)MOX::farload::Far_Reload_Next_(s_patch13_lbx, 1, MOX::_screen_seg);
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
    ERICHELP::Set_Race_Select_Help_();
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
                                    plr->traits[trait_idx] = MOX::_special_bonus[trait_idx - 1][plr->traits[trait_idx]];
                                }
                            }
                            done_flag = 1;
                            _race_flag[i] = 1;
                        } else {
                            if (MOX::_game_type != 0 && MOX::_game_type != 1) {
                                NETSTART::Unrequest_Race_(i);
                            }
                            ERICHELP::Set_Race_Select_Help_();
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
                            if (MOX::_game_type != 0 && MOX::_game_type != 1) NETSTART::Unrequest_Race_(i);
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
                            ERICHELP::Set_Race_Select_Help_();
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
                        if (MOX::_game_type != 0 && MOX::_game_type != 1) NETSTART::Unrequest_Race_(loaded_race);
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
                        ERICHELP::Set_Race_Select_Help_();
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

}