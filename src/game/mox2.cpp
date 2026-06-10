#include "pch.h"

namespace ALLOC = allocate;

namespace MOX2 {
    int8_t _user_wants_n_space_monsters;
    uint8_t _pan_main_menu_panel;
    uint8_t _skip_intro;
    uint8_t _quick_start;
    char _leave_game_message[100];

    const char s_language_ini[13] = "language.ini";
    const char s_language_ini_write_mode[3] = "wt";

    void __cdecl Init_Map_Pictures_Default__() {
        MAPGEN::Load_Nebula_Pictures_();
        Init_Map_Defaults_();
    }

    void __cdecl Set_Game_Random_Seed_() {
        capture::Check_Release_Version_();

        if (MOX::_cmd_line_seed != 0) {
            game_random::Set_Random_Seed_(MOX::_cmd_line_seed);
        } else {
            game_random::Randomize_();
        }
    }

    void __cdecl Screen_Control_() {
        while (true) {
            MOX::_global_esc = 0;
            int16_t current_screen = MOX::_current_screen;

            switch (current_screen) {
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
                        MOX::_current_screen = SCREEN_REPORTS;
                    }
                    MOX::_previous_screen = SCREEN_START_NET;
                    break;
                case SCREEN_JOIN_NET:
                    if (NETSTART::Join_Net_Screen_()) {
                        MOX::_current_screen = SCREEN_REPORTS;
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
                        MOX::_current_screen = SCREEN_REPORTS;
                    }
                    MOX::_previous_screen = SCREEN_JOIN_NET;
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

        if (dos::DIR_("language.ini", dir_buffer) != 0) {
            FILE* file = fopen("language.ini", "rt");
            fgets(line_buffer, 0x3C, file);
            fclose(file);
            atoi(line_buffer + 4);
        }

        MOX::_settings.language = 0;
    }

    void __cdecl main2_() {
        char font_file[16];
        char loading_buffer[66];
        char mox_set_dir[32];

        MOX::_previous_screen = SCREEN_UNKNOWN;
        MOX::_max_debug_saves = 20;
        MOX::_first_debug_save = 20;
        MOX2::_user_wants_n_space_monsters = -1;
        MOX::_debug_saves_on = 1;
        MOX::_turns_between_debug_saves = 1;

        capture::Release_Version_();

        MOX::_debug_saves_on = 1;
        MOX::_max_debug_saves = 10;
        MOX::_first_debug_save = 20;
        MOX::_turns_between_debug_saves = 1;

        MOX::_sound_buffer = allocate::Allocate_Space_(0x64000);

        if (dos::DIR_("MOX.SET", mox_set_dir) != 0) {
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
        fields::Assign_Update_Function_(RUSS::Mox_Update_);
        fonts::Load_Palette_(5, 0, 0xFF);
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        fonts::Set_PSTR_Style_(2);
        MOX::_game_type = 0;

        HAROLD::Set_Sound_Volume_To_Settings_();
        ALLOC::Allocate_Data_Space_();
        MOX2::_pan_main_menu_panel = 1;

        if (MOX2::_skip_intro == 0) {
            JIM::Draw_Logos_();
            fields::Assign_Update_Function_(RUSS::Mox_Update_);
            fonts::Load_Palette_(5, 0, 0xFF);
            mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
        }

        JIM::Play_Background_Music_();
        RUSS::Mox_Update_();
        MOX2::Set_Game_Random_Seed_();
        MOX::_refresh_mode = 0;
        video::Set_Refresh_Full_();
        MOX::_mox_font_colors_offset = 0x75;

        s_colors font_colors = *MISC::Font_Colors2_(4, 0xD3, 0xDC);
        fonts::Set_Font_Style_(4, &font_colors);
        video::Set_Page_Off_();

        // Prepare loading message
        const char* loading_src = nullptr;
        size_t loading_size = 0;
        switch (MOX::_settings.language) {
        case 1:
            loading_src = "Master of Orion II wird geladen, bitte warten....";
            loading_size = sizeof("Master of Orion II wird geladen, bitte warten....");
            break;
        case 2:
            loading_src = "Chargement de Master of Orion II en cours, veuillez patienter....";
            loading_size = sizeof("Chargement de Master of Orion II en cours, veuillez patienter....");
            break;
        case 3:
            loading_src = "Cargando Master of Orion II, espera, por favor...";
            loading_size = sizeof("Cargando Master of Orion II, espera, por favor...");
            break;
        case 4:
            loading_src = "Caricamento di Master of Orion II. Attendere, prego...";
            loading_size = sizeof("Caricamento di Master of Orion II. Attendere, prego...");
            break;
        default:
            loading_src = "Loading Master of Orion II, please wait....";
            loading_size = sizeof("Loading Master of Orion II, please wait....");
            break;
        }

        memcpy(loading_buffer, loading_src, loading_size);

        fonts::Print_(5, 10, loading_buffer);
        RUSS::Mox_Sync_Update_();
        LOADER::Load_Pictures_();
        strings::Load_Strings();
        RUSS::Mox_Update_();
        MOX::_temp_string[0] = 0;

        if (MOX2::_quick_start != 0) {
            s_player* player = &MOX::_player[MOX::_PLAYER_NUM];
            if (FILEDEF::Read_Moo_Custom_Player_Data_(player)) {
                INITGAME::Init_New_Game_(
                    MOX::_settings.game_difficulty,
                    MOX::_settings.galaxy_size,
                    MOX::_settings.galaxy_age,
                    MOX::_settings.number_of_players,
                    MOX::_settings.strategic_combat_flag,
                    MOX::_settings.starting_civilization_level
                );
                MOX::_current_screen = SCREEN_REPORTS;
            }
            else {
                MOX::_current_screen = SCREEN_MAIN_MENU;
            }
        }
        else {
            MOX::_current_screen = SCREEN_MAIN_MENU;
        }

        KEN::Ken_Init_();
        // TEN-only startup hook removed: TEN restoration is intentionally out of scope for this codebase.
        MOX2::Screen_Control_();
        // TEN-only shutdown hook removed: TEN restoration is intentionally out of scope for this codebase.

        // Copy leave message
        const char* thanks_src = nullptr;
        size_t thanks_size = 0;
        switch (MOX::_settings.language) {
        case 1:
            thanks_src = "Danke, da\xDF Sie Master of Orion II gew\xE4hlt haben.";
            thanks_size = sizeof("Danke, da\xDF Sie Master of Orion II gew\xE4hlt haben.");
            break;
        case 2:
            thanks_src = "Merci de jouer \xE0 Master of Orion II";
            thanks_size = sizeof("Merci de jouer \xE0 Master of Orion II");
            break;
        case 3:
            thanks_src = "Gracias por jugar con Master of Orion II";
            thanks_size = sizeof("Gracias por jugar con Master of Orion II");
            break;
        case 4:
            thanks_src = "Grazie per aver giocato a Master of Orion II !";
            thanks_size = sizeof("Grazie per aver giocato a Master of Orion II !");
            break;
        default:
            thanks_src = "Thanks for playing Master of Orion ][";
            thanks_size = sizeof("Thanks for playing Master of Orion ][");
            break;
        }

        memcpy(MOX2::_leave_game_message, thanks_src, thanks_size);

        exitm::Exit_With_Message(MOX2::_leave_game_message);
    }

    int SDLCALL thunk_main2_(void* unused) {
        (void)unused;
        main2_();
        return 0;
    }

    void __cdecl Get_Auto_Function_(MOX2::action_func_t* function_slot) {
        *function_slot = fields::_function;
    }

    void __cdecl Put_Auto_Function_(MOX2::action_func_t* function_slot) {
        fields::_function = *function_slot;
    }

    void __cdecl Execute_Auto_Function_(MOX2::action_func_t* function_entry)
    {
        if (*function_entry != nullptr) {
            (*function_entry)();
        }
    }

    void __cdecl Set_Mox_Alt_Path_() {
        char alternate_path[61];
        char error_message[100];
        char diplomat_path[80];
        FILE* config_file;
        int16_t path_length;
        const char* error_text = nullptr;

        if (!dos::DIR_("orioncd.ini", alternate_path)) {
            if (MOX::_settings.language == 0) {
                error_text = "ORIONCD.INI not found. Please Re-Install Orion ][.";
            }
            else if (MOX::_settings.language == 1) {
                error_text = "ORIONCD.INI nicht gefunden. Bitte Orion II neu installieren.";
            }
            else if (MOX::_settings.language == 2) {
                error_text = "ORIONCD.INI introuvable. Veuillez r\x82installer Orion II.";
            }
            else if (MOX::_settings.language == 3) {
                error_text = "ORIONCD.INI no se ha encontrado. Vuelve a instalar Orion ][.";
            }
            else if (MOX::_settings.language == 4) {
                error_text = "Non trovo il file ORIONCD.INI. Bisogna reinstallare Orion II.";
            }

            if (error_text != nullptr) {
                sprintf(error_message, error_text);
            }

            exitm::Exit_With_Message(error_message);
            return;
        }

        config_file = fopen("orioncd.ini", "rt");
        fgets(alternate_path, 0x3c, config_file);
        fclose(config_file);

        alternate_path[60] = '\0';
        path_length = (int16_t)strlen(alternate_path);

        if (alternate_path[path_length - 1] == '\r') {
            --path_length;
            alternate_path[path_length] = '\0';
        }

        if (alternate_path[path_length - 1] == '\n') {
            --path_length;
            alternate_path[path_length] = '\0';
        }

        farload::Set_Alternate_Path_(alternate_path);

        if (alternate_path[path_length - 1] != '\\') {
            strcat(alternate_path, "\\");
        }

        sprintf(diplomat_path, "%sdiplomat.lbx", alternate_path);

        if (dos::DIR_("diplomat.lbx", alternate_path)) {
            return;
        }

        if (dos::DIR_(diplomat_path, alternate_path)) {
            return;
        }

        error_text = nullptr;

        if (MOX::_settings.language == 0) {
            error_text = "Cannot locate all game files.  Please insert CD or re-install.";
        }
        else if (MOX::_settings.language == 1) {
            error_text = "Nicht alle Spieldateien auffindbar. Bitte CD einlegen oder neu installieren.";
        }
        else if (MOX::_settings.language == 2) {
            error_text = "Echec de la localisation de tous les fichiers de jeu. Veuillez ins\x82rer le CD ou r\x82installer.";
        }
        else if (MOX::_settings.language == 3) {
            error_text = "No se puede localizar todos los ficheros del juego.  Por favor mete el CD-ROM o vuelve a instalar.";
        }
        else if (MOX::_settings.language == 4) {
            error_text = "Non riesco a trovare qualche file. Bisogna reinserire il CD o reinstallare il gioco.";
        }

        if (error_text != nullptr) {
            sprintf(error_message, error_text);
        }

        exitm::Exit_With_Message(error_message);
    }

} // namespace MOX2
