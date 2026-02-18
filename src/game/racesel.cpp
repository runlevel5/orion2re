#include "deps.h"

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

int16_t __cdecl Flag_Screen_(uint8_t* color_ptr) {
    using Fn = int16_t(__cdecl*)(uint8_t*);
    return reinterpret_cast<Fn>(0x004996C0)(color_ptr);
}

int16_t __cdecl Racial_Option_Screen_(s_player* plr) {
    using Fn = int16_t(__cdecl*)(s_player*);
    return reinterpret_cast<Fn>(0x00438EF0)(plr);
}

short Race_Selection_Screen_(s_player *plr)
{
    int16_t button_ids[14];
    int16_t hotkey_id;
    int16_t input_id;
    int16_t scan_id;
    int16_t naming_res;
    bool exit_flag = false;
    char text_buffer[100];
    uint8_t traits_tmp[31];

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
    void* raceopt_bg = Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
    MOX::Mox_Update_();
    animate::Draw_(0, 0, (s_animation_header*)raceopt_bg);
    video::Copy_Back_To_Off_();

    if (MOX::_game_type == 0 || MOX::_game_type == 1) {
        hotkey_id = fields::Add_Hot_Key_(s__00551120);
    } else {
        hotkey_id = -1000;
    }

    MOX::Mox_Update_();

    for (int i = 0; i < 14; ++i) {
        button_ids[i] = fields::Add_Radio_Button_Field_(
            (i / 7) * 0x7E + 0x15F, (i % 7) * 0x30 + 0x5A,
            s__00551100, _race_button[i], &_race_button_status[i],
            s__00551100, 0x28
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
        input_id = fields::Get_Input_();
        scan_id = fields::Scan_Input_();

        _displayed_race = scan_id - button_ids[0];
        if (_displayed_race < 0 || _displayed_race > 13) {
            _displayed_race = _old_race;
        }

        for (int i = 0; i < 14; ++i) {
            if (button_ids[i] == input_id && _race_flag[i] == 0) {
                int16_t ok = 0;
                if (_custom_flag == 0) {
                    if (MOX::_game_type == 0 || MOX::_game_type == 1) {
                        ok = 1;
                    } else {
                        ok = NETSTART::Request_Race_(i);
                    }

                    if (ok == 0) {
                        KEN::Ken_Get_Text_Message_(0x48, text_buffer, 100);
                        GENDRAW::Message_Box_(text_buffer);
                        video::Set_Page_Back_();
                        void* raceopt_bg2 = Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
                        animate::Draw_(0, 0, (s_animation_header*)raceopt_bg2);
                        video::Copy_Back_To_Off_();
                    } else {
                        timer::Mark_Time_();
                        timer::Release_Time_(5);
                        fields::Clear_Fields_();
                        struct_::Copy_Structure_(plr->traits, _race_specials[i], 31);
                        plr->race = i;
                        strcpy(plr->race_name, MOX::_race_names[i]);
                        fields::Deactivate_Help_List_();

                        while (true) {
                            naming_res = Naming_Popup_(plr->name, i);
                            video::Set_Page_Back_();
                            void* bg3 = Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
                            animate::Draw_(0, 0, (s_animation_header*)bg3);
                            video::Copy_Back_To_Off_();
                            RUSS::Mox_Sync_Update_();
                            _old_race = i;

                            if (naming_res != 1) break;
                            naming_res += Flag_Screen_((uint8_t*)&plr->color);
                            if (naming_res != 2) {
                                Reload_Race_Selection_Screen_();
                            }
                        }

                        if (naming_res == 2) {
                            for (int k = 1; k < 10; ++k) {
                                if (plr->traits[k] != 0) {
                                    plr->traits[k] = MOX::_special_bonus[k][plr->traits[k]];
                                }
                            }
                            exit_flag = true;
                            _race_flag[i] = 1;
                        } else {
                            if (MOX::_game_type == 0 || MOX::_game_type == 1) {
                                _race_flag[i] = 0;
                            } else {
                                NETSTART::Unrequest_Race_(i);
                            }
                            ERICHELP::Set_Race_Select_Help_();
                            video::Set_Page_Back_();
                            void* bg4 = Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
                            animate::Draw_(0, 0, (s_animation_header*)bg4);
                            video::Copy_Back_To_Off_();

                            for (int k = 0; k < 14; ++k) _race_button_status[k] = 0;
                            _old_race = 0;
                            _race_button_status[_displayed_race] = 1;

                            if (MOX::_game_type == 0 || MOX::_game_type == 1) {
                                hotkey_id = fields::Add_Hot_Key_(s__00551120);
                            } else {
                                hotkey_id = -1000;
                            }

                            for (int k = 0; k < 14; ++k) {
                                button_ids[k] = fields::Add_Radio_Button_Field_(
                                    (k / 7) * 0x7E + 0x15F, (k % 7) * 0x30 + 0x5A,
                                    s__00551100, _race_button[k], &_race_button_status[k],
                                    s__00551100, 0x28
                                );
                            }
                            video::Set_Page_Off_();
                            Draw_Race_Selection_Screen_();
                            RUSS::Mox_Sync_Update_();
                            fields::Assign_Auto_Function_(Draw_Race_Selection_Screen_, 2);
                        }
                    }
                } else {
                    if (MOX::_game_type == 0 || MOX::_game_type == 1) {
                        ok = 1;
                    } else {
                        ok = NETSTART::Request_Race_(i);
                    }

                    if (ok == 0) {
                        KEN::Ken_Get_Text_Message_(0x48, text_buffer, 100);
                        GENDRAW::Message_Box_(text_buffer);
                        video::Set_Page_Back_();
                        void* bg5 = Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
                        animate::Draw_(0, 0, (s_animation_header*)bg5);
                        video::Copy_Back_To_Off_();
                    } else {
                        struct_::Copy_Structure_(plr->traits, _race_specials[i], 31);
                        plr->race = i;
                        strcpy(plr->race_name, MOX::_race_names[i]);
                        fields::Deactivate_Auto_Function_();
                        fields::Clear_Fields_();
                        if (Racial_Option_Screen_(plr) == 1) {
                            exit_flag = true;
                            _race_flag[i] = 1;
                        } else {
                            if (MOX::_game_type == 0 || MOX::_game_type == 1) {
                                _race_flag[i] = 0;
                            } else {
                                NETSTART::Unrequest_Race_(i);
                            }
                            allocate::Pop_Block_(MOX::_screen_seg);
                            Reload_Race_Selection_Screen_();
                            video::Set_Page_Back_();
                            void* bg6 = Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
                            animate::Draw_(0, 0, (s_animation_header*)bg6);
                            video::Copy_Back_To_Off_();
                            if (MOX::_game_type == 0 || MOX::_game_type == 1) {
                                hotkey_id = fields::Add_Hot_Key_(s__00551120);
                            } else {
                                hotkey_id = -1000;
                            }
                            for (int k = 0; k < 13; ++k) {
                                button_ids[k] = fields::Add_Radio_Button_Field_(
                                    (k / 7) * 0x7E + 0x15F, (k % 7) * 0x30 + 0x5A,
                                    s__00551100, _race_button[k], &_race_button_status[k],
                                    s__00551100, 0x28
                                );
                            }
                            button_ids[13] = fields::Add_Radio_Button_Field_(0x1DD, 0x17A, s__00551100, (s_animation_header*)_last_race_seg, &_race_button_status[13], s__00551100, 0x28);
                            fields::Assign_Auto_Function_(Draw_Race_Selection_Screen_, 2);
                            ERICHELP::Set_Race_Select_Help_();
                            _custom_flag = 0;
                        }
                    }
                }
            }
        }

        if (hotkey_id == input_id) {
            if (_custom_flag != 1) {
                fields::Clear_Fields_();
                fields::Deactivate_Auto_Function_();
                allocate::Pop_Block_(MOX::_screen_seg);
                return 0;
            }
            _custom_flag = 0;
            fields::Clear_Fields_Above_(button_ids[12]);
            button_ids[13] = fields::Add_Radio_Button_Field_(0x1DD, 0x17A, s__00551100, _race_button[13], &_race_button_status[13], s__00551100, 0x28);
        }

        if (input_id == button_ids[13]) {
            FILE* f = _wfopen(s_lastrace_rac, s_rb_0055117c);
            if (_custom_flag == 0) {
                _custom_flag = 1;
                fields::Clear_Fields_Above_(button_ids[12]);
                if (!f) {
                    button_ids[13] = -1000;
                } else {
                    button_ids[13] = fields::Add_Radio_Button_Field_(0x1DD, 0x17A, s__00551100, (s_animation_header*)_last_race_seg, &_race_button_status[13], s__00551100, 0x28);
                    _fclose(f);
                }
            } else if (f) {
                _fread(traits_tmp, 31, 1, f);
                struct_::Copy_Structure_(plr->traits, _race_specials[13], 31);
                _fread(&plr->race, 1, 1, f);
                int16_t ok = (MOX::_game_type == 0 || MOX::_game_type == 1) ? 1 : NETSTART::Request_Race_(plr->race);
                if (ok == 0) {
                    _fclose(f);
                    KEN::Ken_Get_Text_Message_(0x48, text_buffer, 100);
                    GENDRAW::Message_Box_(text_buffer);
                    video::Set_Page_Back_();
                    void* bg7 = Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
                    animate::Draw_(0, 0, (s_animation_header*)bg7);
                    video::Copy_Back_To_Off_();
                } else {
                    _fread(plr->race_name, 15, 1, f);
                    fields::Deactivate_Auto_Function_();
                    fields::Clear_Fields_();
                    _old_race = plr->race;
                    _fclose(f);
                    if (Racial_Option_Screen_(plr) == 1) {
                        exit_flag = true;
                        _race_flag[plr->race] = 1;
                    } else {
                        if (MOX::_game_type == 0 || MOX::_game_type == 1) {
                            _race_flag[plr->race] = 0;
                        } else {
                            NETSTART::Unrequest_Race_(plr->race);
                        }
                        allocate::Pop_Block_(MOX::_screen_seg);
                        Reload_Race_Selection_Screen_();
                        video::Set_Page_Back_();
                        void* bg8 = Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
                        animate::Draw_(0, 0, (s_animation_header*)bg8);
                        video::Copy_Back_To_Off_();
                        if (MOX::_game_type == 0 || MOX::_game_type == 1) {
                            hotkey_id = fields::Add_Hot_Key_(s__00551120);
                        } else {
                            hotkey_id = -1000;
                        }
                        for (int k = 0; k < 13; ++k) {
                            button_ids[k] = fields::Add_Radio_Button_Field_(
                                (k / 7) * 0x7E + 0x15F, (k % 7) * 0x30 + 0x5A,
                                s__00551100, _race_button[k], &_race_button_status[k],
                                s__00551100, 0x28
                            );
                        }
                        button_ids[13] = fields::Add_Radio_Button_Field_(0x1DD, 0x17A, s__00551100, _race_button[13], &_race_button_status[13], s__00551100, 0x28);
                        fields::Assign_Auto_Function_(Draw_Race_Selection_Screen_, 2);
                        ERICHELP::Set_Race_Select_Help_();
                        _custom_flag = 0;
                    }
                }
            }
        }

        if (exit_flag) break;

        if (MOX::_FULL_DRAW_SCREEN_FLAG != 0) {
            video::Set_Page_Back_();
            graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
            void* bg9 = Buffer_Reload_(s_RACEOPT_LBX_00556320, 0, MOX::_global_cache_seg);
            animate::Draw_(0, 0, (s_animation_header*)bg9);
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