#include "deps.h"
#include "diplodef.h"

namespace MOX2 {
    void __cdecl Init_Map_Pictures_Default__() {
        MAPGEN::Load_Nebula_Pictures_();
        Init_Map_Defaults_();
    }

    void __cdecl Set_Game_Seed() {
        capture::Check_Release_Version_();

        if (MOX::_cmd_line_seed != 0) {
            random::Set_Random_Seed_(MOX::_cmd_line_seed);
        } else {
            random::Randomize_();
        }
    }

    void __cdecl Screen_Control_() {
        while (true) {
            MOX::_global_esc = 0;
            SCREEN current = MOX::_current_screen;

            switch (current) {
                case SCREEN_MAIN:
                    MAINSCR::Main_Screen_();
                    MOX::_previous_screen = SCREEN_MAIN;
                    break;
                case SCREEN_COLONY:
                    COLONY::Colony_Screen_();
                    MOX::_previous_screen = SCREEN_COLONY;
                    break;
                case SCREEN_DESIGN:
                    DESIGN::Design_Screen_();
                    MOX::_previous_screen = SCREEN_DESIGN;
                    break;
                case SCREEN_FLEET:
                    FLT1::Fleet_Screen_();
                    MOX::_previous_screen = SCREEN_FLEET;
                    break;
                case SCREEN_RACE:
                    RACESCRN::Race_Screen_();
                    MOX::_previous_screen = SCREEN_RACE;
                    break;
                case SCREEN_EXIT:
                    return;
                case SCREEN_GAME:
                    LOADSAVE::_Game_Popup_();
                    MOX::_previous_screen = SCREEN_GAME;
                    break;
                case SCREEN_INFO:
                    INFO::Info_Screen_();
                    MOX::_previous_screen = SCREEN_INFO;
                    break;
                case SCREEN_MAIN_MENU:
                    MAINMENU::Main_Menu_Screen_();
                    MOX::_previous_screen = SCREEN_MAIN_MENU;
                    break;
                case SCREEN_MAIN_NEXT_TURN:
                    NEXTTURN::Next_Turn_();
                    MOX::_previous_screen = SCREEN_MAIN_NEXT_TURN;
                    break;
                case SCREEN_NEW_GAME:
                    if (MOX::_game_type == 0) {
                        if (NEWGAME::Newgame_Screen_() == 0) {
                            MOX::_current_screen = SCREEN_MAIN_MENU;
                        } else {
                            INITGAME::Init_New_Game_(
                                MOX::_settings.game_difficulty,
                                MOX::_settings.galaxy_size,
                                MOX::_settings.galaxy_age,
                                MOX::_settings.number_of_players,
                                MOX::_settings.strategic_combat_flag,
                                MOX::_settings.starting_civilization_level
                            );
                        }
                    } else {
                        MOX::_return_screen = SCREEN_MULTI_PLAYER;
                        NEWGAME::Newgame_Screen_();
                    }
                    MOX::_previous_screen = SCREEN_NEW_GAME;
                    break;
                case SCREEN_HALL_OF_FAME:
                    SCORE::Hall_Of_Fame_Screen_(-1);
                    MOX::_previous_screen = SCREEN_HALL_OF_FAME;
                    MOX::_current_screen = SCREEN_MAIN_MENU;
                    break;
                case SCREEN_MULTI_PLAYER:
                    MULTPLAY::Multi_Player_Screen_();
                    MOX::_previous_screen = SCREEN_MULTI_PLAYER;
                    break;
                case SCREEN_HOTSEAT:
                    HOTPOP::Hotseat_Screen_();
                    MOX::_previous_screen = SCREEN_HOTSEAT;
                    break;
                case SCREEN_HOTSEAT_SELECT_PLAYER:
                    MULTPLAY::Hotseat_Select_Player_();
                    MOX::_previous_screen = SCREEN_HOTSEAT_SELECT_PLAYER;
                    break;
                case SCREEN_PLANET_DATA:
                    HAROLD::Planet_Data_Screen_();
                    MOX::_previous_screen = SCREEN_PLANET_DATA;
                    break;
                case SCREEN_COLONY_SUMMARY:
                    COLSUM::Colony_Summary_Screen_();
                    MOX::_previous_screen = SCREEN_COLONY_SUMMARY;
                    break;
                case SCREEN_START_NET:
                    if (NETSTART::Start_Net_Screen_()) {
                        MOX::_current_screen = SCREEN_REPORTS; // 0x27
                    }
                    MOX::_previous_screen = SCREEN_START_NET;
                    break;
                case SCREEN_JOIN_NET:
                    if (NETSTART::Join_Net_Screen_()) {
                        MOX::_current_screen = SCREEN_REPORTS; // 0x27
                    }
                    MOX::_previous_screen = SCREEN_JOIN_NET;
                    break;
                case SCREEN_QUEUE_POPUP:
                    COLBLDG::Build_Queue_Popup_();
                    MOX::_previous_screen = SCREEN_QUEUE_POPUP;
                    break;
                case SCREEN_DISTANCE_BETWEEN_STARS:
                    MAINSCR2::Distance_Between_Stars_Screen_();
                    MOX::_previous_screen = SCREEN_DISTANCE_BETWEEN_STARS;
                    MOX::_current_screen = MOX::_return_screen;
                    break;
                case SCREEN_OFFICERS:
                    OFFICER::Officers_Screen_();
                    MOX::_previous_screen = SCREEN_OFFICERS;
                    break;
                case SCREEN_COLONIZATION_IN_MAIN:
                    MAINSCR2::Planet_Colonization_In_Main_Screen_();
                    MOX::_previous_screen = SCREEN_COLONIZATION_IN_MAIN;
                    break;
                case SCREEN_BUILDING_OUTPOSTS_IN_MAIN:
                    MAINSCR2::Building_Outposts_In_Main_Screen_();
                    MOX::_previous_screen = SCREEN_BUILDING_OUTPOSTS_IN_MAIN;
                    break;
                case SCREEN_PLANET_SUMMARY:
                    PLNTSUM::Planet_Summary_Screen_();
                    MOX::_previous_screen = SCREEN_PLANET_SUMMARY;
                    break;
                case SCREEN_COLONY_LANDING:
                    COLLAND::Colony_Landing_Screen_();
                    MOX::_previous_screen = SCREEN_COLONY_LANDING;
                    break;
                case SCREEN_UNLOADING_TRANSPORTS_IN_MAIN:
                    MAINSCR2::Unloading_Transports_In_Main_Screen_();
                    MOX::_previous_screen = SCREEN_UNLOADING_TRANSPORTS_IN_MAIN;
                    break;
                case SCREEN_SCRAP_FREIGHTERS:
                    MAINSCR2::Scrap_Freighters_();
                    MOX::_previous_screen = SCREEN_SCRAP_FREIGHTERS;
                    break;
                case SCREEN_TECH_CHANGE:
                    TECH::Tech_Change_();
                    MOX::_previous_screen = SCREEN_TECH_CHANGE;
                    break;
                case SCREEN_NET_NEXT_TURN:
                    NETTURN::Net_Next_Turn_();
                    break;
                case SCREEN_REPORTS:
                    MAINSCR2::Reports_Screen_();
                    MOX::_previous_screen = SCREEN_REPORTS;
                    break;
                case SCREEN_TURN_SUMMARY:
                    TURNSUM::Turn_Summary_Popup_();
                    MOX::_previous_screen = SCREEN_TURN_SUMMARY;
                    break;
                case SCREEN_LOAD_NET:
                    if (NETSTART::Load_Net_Screen_()) {
                        MOX::_current_screen = SCREEN_REPORTS; // 0x27
                    }
                    MOX::_previous_screen = SCREEN_JOIN_NET; // Note: Original code set it to 0x16
                    break;
                case SCREEN_SHOW_COMMAND_POINTS:
                    MAINSCR2::Show_Command_Points_Screen_();
                    MOX::_previous_screen = SCREEN_SHOW_COMMAND_POINTS;
                    break;
                default:
                    break;
            }
        }
    }

    void __cdecl Init_Map_Defaults_() {
        MOX::_cur_map_x = 0;
        MOX::_cur_map_y = 0;
        MOX::_current_screen = SCREEN_REPORTS;
        diplodef::_council_winner = -1;

        KEN::Ken_Init2_();
        HAROLD::Harold_Defaults_();
        ERIC::Eric_Defaults_();
        BILL::Bill_Default_();

        MOX::_global_esc = 0;
    }

    void __cdecl Set_Mox_Language_() {
        char dir_buffer[80];
        char line_buffer[80];

        // Check if language.ini exists
        if (dos::DIR_(s_language_ini, dir_buffer) != 0) {
            FILE* file = _wfopen(s_language_ini, s_rt);
            if (file) {
                // Read language code from file
                _fgets(line_buffer, 0x3C, file);
                _fclose(file);

                // Convert value (offset 16 used in assembly, likely skipping a header)
                // but the result is currently discarded in this implementation.
                _atoi(line_buffer);
            }
        }

        // Hardcoded language override to 0 (English)
        MOX::_settings_language = 0;
    }

    void main2_() {
        char font_file[16];
        char loading_buffer[64];
        char mox_set_dir[32];

        MOX::_previous_screen = SCREEN_UNKNOWN;
        MOX::_max_debug_saves = 20;
        MOX::_first_debug_save = 20;
        MOX2::_user_wants_n_space_monsters = 0xFF;
        MOX::_debug_saves_on = 1;
        MOX::_turns_between_debug_saves = 1;

        MOX2::Release_Version_();

        MOX::_debug_saves_on = 1;
        MOX::_max_debug_saves = 10;
        MOX::_first_debug_save = 20;
        MOX::_turns_between_debug_saves = 1;

        MOX::_sound_buffer = allocate::Allocate_Space_(0x64000);

        if (dos::DIR_(MOX2::s_MOX_SET, mox_set_dir) != 0) {
            FILEDEF::Load_Game_Settings_();
            if (MOX::_settings.version != 0x82) {
                FILEDEF::Update_Game_Settings_Version_Number_();
            }
        }
        else {
            INITGAME::Set_Default_Game_Settings_();
        }

        MOX2::Set_Mox_Language_();
        HAROLD::Get_Font_File_(font_file);
        sound::Set_Audio_Buffer_(MOX::_sound_buffer);

        if (init::Init_Drivers_(4, font_file) == 0) {
            return;
        }

        MOX2::Set_Mox_Alt_Path_();
        ten::init();
        fields::Assign_Update_Function_(MOX::Mox_Update_);
        fonts::Load_Palette_(5, 0, 0xFF);
        mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
        fonts::Set_PSTR_Style_(2);
        MOX::_game_type = 0;

        HAROLD::Set_Sound_Volume_To_Settings_();
        allocate::Allocate_Data_Space_();
        MOX2::_pan_main_menu_panel = 1;

        if (MOX2::_skip_intro == 0) {
            JIM::Draw_Logos_();
            fields::Assign_Update_Function_(MOX::Mox_Update_);
            fonts::Load_Palette_(5, 0, 0xFF);
            mouse::Set_Mouse_List_(MOX::_mouse_list_arrow, 1);
        }

        JIM::Play_Background_Music_();
        MOX::Mox_Update_();
        MOX2::Set_Game_Seed();
        MOX::_refresh_mode = 0;
        video::Set_Refresh_Full_();
        MOX::_mox_font_colors_offset = 0x75;

        s_colors font_colors = *MISC::Font_Colors2_(4, 0xD3, 0xDC);
        fonts::Set_Font_Style_(4, &font_colors);
        video::Set_Page_Off_();

        // Prepare loading message
        const char* loading_src = nullptr;
        int copy_len = 0;
        switch (MOX::_settings.language) {
        case 1: loading_src = MOX2::s_Loading_German; copy_len = 48 + 2; break; // 0xc * 4 + 2
        case 2: loading_src = MOX2::s_Loading_French; copy_len = 64 + 2; break; // 0x10 * 4 + 2
        case 3: loading_src = MOX2::s_Loading_Spanish; copy_len = 48 + 2; break; // 0xc * 4 + 2
        case 4: loading_src = MOX2::s_Loading_Italian; copy_len = 52 + 3; break; // 0xd * 4 + 3
        default: loading_src = MOX2::s_Loading_English; copy_len = 44; break;   // 0xb * 4
        }

        for (int i = 0; i < copy_len; ++i) {
            loading_buffer[i] = loading_src[i];
        }

        fonts::Print_(5, 10, loading_buffer);
        RUSS::Mox_Sync_Update_();
        LOADER::Load_Pictures_();
        strings::Load_Strings();
        MOX::Mox_Update_();
        MOX::_temp_string = 0;

        if (MOX2::_quick_start != 0) {
            s_player* player = &MOX::_player[MOX::_PLAYER_NUM];
            if (MOX::Read_Moo_Custom_Player_Data_(player)) {
                INITGAME::Init_New_Game_(
                    MOX::_settings.game_difficulty,
                    MOX::_settings.galaxy_size,
                    MOX::_settings.galaxy_age,
                    MOX::_settings.number_of_players,
                    MOX::_settings.strategic_combat_flag,
                    MOX::_settings.starting_civilization_level
                );
                MOX::_current_screen = SCREEN_REPORTS; // 0x27
            }
            else {
                MOX::_current_screen = SCREEN_MAIN_MENU;
            }
        }
        else {
            MOX::_current_screen = SCREEN_MAIN_MENU;
        }

        KEN::Ken_Init_();
        FUN_004d7d80();
        MOX2::Screen_Control_();
        FUN_004d7da0();

        // Copy leave message
        const char* thanks_src = nullptr;
        int thanks_len = 0;
        switch (MOX::_settings.language) {
        case 1: thanks_src = MOX2::s_Thanks_German; thanks_len = 48; break; // 0xc * 4
        case 2: thanks_src = MOX2::s_Thanks_French; thanks_len = 36; break; // 0x9 * 4
        case 3: thanks_src = MOX2::s_Thanks_Spanish; thanks_len = 40; break; // 0xa * 4
        case 4: thanks_src = MOX2::s_Thanks_Italian; thanks_len = 44 + 3; break; // 0xb * 4 + 3
        default: thanks_src = MOX2::s_Thanks_English; thanks_len = 36 + 2; break; // 0x9 * 4 + 2
        }

        for (int i = 0; i < thanks_len; ++i) {
            MOX2::_leave_game_message[i] = thanks_src[i];
        }

        exitm::Exit_With_Message(MOX2::_leave_game_message);
    }

} // namespace MOX2
