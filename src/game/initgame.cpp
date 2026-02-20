#include "deps.h"

namespace INITGAME {

    /**
     * Initializes the ship data array. 
     * Zeroes out the entire array (500 ships), then sets default status, 
     * location, and officer values for each.
     */
    void Init_Ships_Array_() {
        // Zero out the entire ship array.
        // Array size: 500 ships * 129 bytes (sizeof(s_ship_data)) = 64,500 bytes.
        // The assembly clears 16,125 DWORDS (64,500 bytes).
        uint32_t* raw_ship_data = reinterpret_cast<uint32_t*>(MOX::_ship);
        for (int i = 0; i < 16125; ++i) {
            raw_ship_data[i] = 0;
        }

        // Initialize default values for all 500 ship slots
        for (MOX::_NUM_SHIPS = 0; MOX::_NUM_SHIPS < 500; ++MOX::_NUM_SHIPS) {
            // Note: s_ship_data structure size is 129 bytes.
            s_ship_data& ship = MOX::_ship[MOX::_NUM_SHIPS];
            ship.status = 5;
            ship.location = -1;
            ship.officer_index = -1;
        }

        // Reset ship count to 0 and perform slot allocation logic
        MOX::_NUM_SHIPS = 0;
        SHIPMOVE::Allocate_New_Ship_Slots_();
    }

    /**
     * @brief Initializes the global colonies array.
     * Clears the entire array and sets initial sentinel values for owner, planet, and officer.
     */
    void Init_Colonies_Array_() {
        // The array consists of 250 colonies, each of 361 bytes (total 90250 bytes).
        // The assembly clears this memory using REP STOSD (0x5822 * 4 bytes) and STOSW (2 bytes).
        uint8_t* raw_colony_data = reinterpret_cast<uint8_t*>(MOX::_colony);
        for (uint32_t i = 0; i < 90250; ++i) {
            raw_colony_data[i] = 0;
        }

        // Loop through all 250 colony slots to set initial state.
        // The size of s_colony is 361 (0x169) bytes.
        for (MOX::_NUM_COLONIES = 0; MOX::_NUM_COLONIES < 250; ++MOX::_NUM_COLONIES) {
            s_colony& colony = MOX::_colony[MOX::_NUM_COLONIES];

            colony.planet = -1;
            colony.owner = -1;
            colony.officer = -1;
        }

        // Reset the active colony counter to 0 after initialization.
        MOX::_NUM_COLONIES = 0;
    }

    /**
     * Sets all game settings to their default values, clears specific memory ranges,
     * generates a default network game name using the current clock, and persists the settings.
     */
    void Set_Default_Game_Settings_() {
        // Zero out the first 553 bytes of the settings structure (0x8A * 4 + 1)
        uint8_t* settings_raw = reinterpret_cast<uint8_t*>(&MOX::_settings);
        for (int i = 0; i < 553; ++i) {
            settings_raw[i] = 0;
        }

        // Initialize general game settings
        MOX::_settings.end_of_turn_summary = 1;
        MOX::_settings.end_of_turn_wait = 1;
        MOX::_settings.random_events = 1;
        MOX::_settings.enemy_moves = 0;
        MOX::_settings.expanding_help = 0;
        MOX::_settings.auto_select_ships = 1;
        MOX::_settings.animations_on = 1;
        MOX::_settings.auto_select_colony = 0;
        MOX::_settings.show_relocation_lines = 1;
        MOX::_settings.show_gnn_report = 1;
        MOX::_settings.auto_delete_tg_housing = 0;
        MOX::_settings.auto_saves = 1;
        MOX::_settings.only_show_serious_turn_summary_reports = 0;
        MOX::_settings.ship_initiative = 0;
        MOX::_settings.field14_0xe = 0;
        MOX::_settings.field15_0xf = 0;

        // Sound and Music
        MOX::_settings.sound_fx_on = 1;
        MOX::_settings.sound_fx_level = 50;
        MOX::_settings.music_on = 1;
        MOX::_settings.music_level = 50;

        // Galaxy and Difficulty
        MOX::_settings.xenons_exist = 1;
        MOX::_settings.game_difficulty = 0;
        MOX::_settings.galaxy_size = 1;
        MOX::_settings.galaxy_age = 1;
        MOX::_settings.strategic_combat_flag = 1;
        MOX::_settings.starting_civilization_level = 1;
        MOX::_settings.number_of_players = 5;

        // Combat Flags
        MOX::_settings.combat_speedx2_flag = 0;
        MOX::_settings.combat_legal_moves_flag = 0;
        MOX::_settings.combat_missile_impact_flag = 1;
        MOX::_settings.combat_shields_arc_flag = 0;
        MOX::_settings.combat_grid_flag = 0;

        // Save Slot and Versioning
        MOX::_settings.active_save_slot = -1; // 0xFF
        MOX::_settings.version = 0x82;

        // Specific field writes (using absolute offsets as per disassembly)
        // field71_0xde (0xDE) and field72_0xdf (0xDF) as a word (value 2)
        *reinterpret_cast<uint16_t*>(&MOX::_settings.field71_0xde) = 2;
        // field73_0xe0 (0xE0) and field74_0xe1 (0xE1) as a word (value 1)
        *reinterpret_cast<uint16_t*>(&MOX::_settings.field73_0xe0) = 1;

        // Clear the first byte of 10 consecutive records starting at 0x5974F0 (stride 0x25)
        // TODO: Unknown
        for (uint8_t* ptr = &DAT_005974f0; ptr < &DAT_00597662; ptr += 0x25) {
            *ptr = 0;
        }

        // Generate default network name: MOX-<clock % 10>
        int clock_val = _clock();
        _sprintf(MOX::_settings.net_game_name, "MOX-%d", clock_val % 10);

        // Finalize default settings
        MULTPLAY::Set_Default_Modem_Settings_();
        INITGAME::Write_Game_Settings_();
    }

    /**
     * @address 00478fb0
     */
    void Write_Game_Settings_() {
        FILE* file = _wfopen(s_mox_set_0055760c, s_wb_00551178);

        if (file == nullptr) {
            exitm::Exit_With_Message(s_Unable_to_open_mox_set_005575f4);
        }

        _fwrite(&MOX::_settings, sizeof(MOX::_settings), 1, file);
        _fflush(file);
        _fclose(file);
    }

    void __cdecl Init_New_Game_(int16_t game_difficulty, int16_t galaxy_size, int16_t galaxy_age, int16_t num_players,
                                int16_t strategic_combat_flag, int16_t starting_civ_level) {
        bool extra_maps_generated = false;
        int map_index = 0;
        uint16_t generation_success;

        MOX::_settings.start_of_game_seed = random::Get_Random_Seed_();
        MOX2::Set_Game_Seed();
        MOX::_stardate = 35000;
        MOX::_NUM_PLAYERS = num_players;

        Init_Ships_Array_();
        MOX::Mox_Update_();
        Init_Colonies_Array_();
        MOX::Mox_Update_();

        // Clear antaran invasion data (66 bytes)
        memset(&MOX::_antarans, 0, sizeof(MOX::_antarans));
        MOX::Mox_Update_();

        if (MOX::_game_type != 2) {
            MOX::_PLAYER_NUM = 0;
        }

        Init_Leaders_();
        MOX::Mox_Update_();
        Init_Players_();
        MOX::Mox_Update_();
        BILL::Cache_Player_Scientist_Anim_();
        MOX::Mox_Update_();
        MSG_::Allocate_Msg_Slots_();

        do {
            MOX2::Set_Game_Seed();
            MOX::_settings.start_of_game_seed = random::Get_Random_Seed_();

            MAPGEN::Universe_Generation_((uint8_t)galaxy_size, (uint8_t)galaxy_age, (uint8_t)game_difficulty);
            MOX::Mox_Update_();

            generation_success = HOMEGEN::Generate_Home_Worlds_();
            MOX::Mox_Update_();

            if (generation_success != 0) {
                video::Set_Page_Off_();
                graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
                platform::Toggle_Pages_Stub_();
                MAPGEN::Enforce_Planet_Max_(250);
                MAPGEN::Set_Black_Hole_Visited_Flags_();
                HAROLD::Confirm_Planet_And_Star_Index_Link_(HAROLD::s_after_home_world_gen);

                if (MOX::_settings.starting_civilization_level == 2) {
                    ADVCIV::Advanced_Civilization_Colonies_();
                    ADVCIV::Assign_Advanced_Civilization_Starting_Ships_();
                }
            }

            if (map_index < (int)MOX::_maps_to_generate) {
                extra_maps_generated = true;
                MOX::_star_fields_dim[0] = 33;
                MOX::_star_fields_dim[1] = 29;
                MOX::_star_fields_dim[2] = 25;
                MOX::_star_fields_dim[3] = 23;
                MOX::_star_fields_dim[4] = 21;
                MOX::_star_fields_dim[5] = 17;

                HAROLD::Set_All_Visited_Flags_();
                MAINSCR::Update_Star_Owners_();
                video::Set_Page_Off_();
                MAINSCR::Draw_Mini_Main_Screen_();
                RUSS::Mox_Sync_Update_();

                RUSS::PFMT_(0, 0x17, RUSS::s_Map_d, map_index);
                map_index++;

                RUSS::PFMT_(0, 0x18, RUSS::s_Seed_ld, MOX::_settings.start_of_game_seed);
                capture::Screen_Flic_Capture_();
            }
        } while (generation_success == 0);

        if (extra_maps_generated) {
            exitm::Exit_With_Message("done");
        }

        MOX::Mox_Update_();
        HOMEGEN::Make_System_Monsters_();
        MOX::Mox_Update_();
        Make_System_Monsters_Into_Ships_();
        MOX::Mox_Update_();
        MAPGEN::Generate_Wormhole_Links_();
        MOX::Mox_Update_();

        if (MOX::_settings.starting_civilization_level == 2) {
            OFFICER::Allocate_Adv_Civ_Game_Officers_();
            MOX::Mox_Update_();
        }

        MAPGEN::Assign_Marooned_Heroes_();
        MOX::Mox_Update_();
        MOX::Mox_Update_();

        GEO::Initialize_Black_Hole_Blocks_(45);
        MOX::Mox_Update_();
        GEO::Initialize_Star_In_Nebula_Info_();
        MOX::Mox_Update_();

        MOX2::Init_Map_Defaults_();
        KEN::Ken_Init_();
        HAROLD::Harold_Init_();
        ERIC::Eric_Init_();
        BILL::Bill_Init_();
        MAINSCR::Update_Star_Owners_();
        MOX::Mox_Update_();

        COLONIZE::Allocate_New_Colony_Slots_();
        MOX::Mox_Update_();
        COLCALC::Twiddle_Initial_Homeworlds_();
        MOX::Mox_Update_();
        SHIPMOVE::Allocate_New_Ship_Slots_();
        MOX::Mox_Update_();
        COLCALC::Compute_All_Star_Colony_Stuff_();
        MOX::Mox_Update_();
        TRADE::Process_Trade_And_Research_Agreements_();
        MOX::Mox_Update_();

        if (MOX::_settings.auto_select_colony != 0) {
            Set_Initial_Colony_Auto_Selects_();
        }
        MOX::Mox_Update_();
        REPORT::Initialize_Reports_();
        MOX::Mox_Update_();
        COLCALC::Init_Surrenders_();
        MOX::Mox_Update_();
        MOX::Mox_Update_();

        FILEDEF::Save_Game_(9);
        MOX::Mox_Update_();
        FILEDEF::Save_Game_Settings_();
        MOX::Mox_Update_();
        MOX2::Init_Map_Pictures_Default__();

        if (MOX::_game_type == 0) {
            FILEDEF::Save_Moo_Custom_Player_Data_(MOX::_player);
        }

        random::Set_Random_Seed_(MOX::_settings.start_of_game_seed);
    }

    void __cdecl Init_Players_() {
        // Load race trait template data from RACESTUF.LBX
        RACESEL::_race_specials[0] = (uint8_t*)farload::Far_Reload_Data_(s_RACESTUF_LBX, 7, (void*)MOX::_global_data_seg, 0, 1, 31);
        MOX::Mox_Update_();

        for (int i = 1; i < 13; ++i) {
            RACESEL::_race_specials[i] = (uint8_t*)farload::Far_Reload_Next_Data_(s_RACESTUF_LBX, 7, (void*)MOX::_global_data_seg, i, 1, 31);
            MOX::Mox_Update_();
        }

        MOX::Mox_Update_();

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            s_player* plr = &MOX::_player[i];

            if (plr->objectives == 100) {
                // Predefined or special player setup: copy traits to evolutionary upgrades
                struct_::Copy_Structure_(&plr->evolutionary_upgrade_traits, &plr->traits, sizeof(plr->traits));
            } else {
                int16_t race_idx;
                bool unique_race;
                do {
                    race_idx = (int16_t)random::Random_(13) - 1;
                    unique_race = true;
                    for (int16_t j = 0; j < i; ++j) {
                        if (MOX::_player[j].race == (char)race_idx) {
                            unique_race = false;
                            break;
                        }
                    }
                } while (!unique_race);

                plr->race = (char)race_idx;

                // Copy race name
                char* src_name = MOX::_race_names[race_idx];
                for (int k = 0; k < 15; ++k) {
                    plr->race_name[k] = src_name[k];
                    if (src_name[k] == '\0') break;
                }

                // Copy default traits from template
                struct_::Copy_Structure_(&plr->traits, &RACESEL::_race_specials[race_idx], sizeof(plr->traits));
                struct_::Copy_Structure_(&plr->evolutionary_upgrade_traits, &RACESEL::_race_specials[race_idx], sizeof(plr->evolutionary_upgrade_traits));

                MOX::Mox_Update_();
                RACECUST::Race_Customization_(i);
                MOX::Mox_Update_();

                // Apply special trait bonuses (lookup table)
                for (int k = 1; k < TRAIT_COUNT; ++k) {
                    if (plr->traits[k] != 0) {
                        plr->traits[k] = MOX::_special_bonus[k][plr->traits[k]];
                    }
                    if (plr->evolutionary_upgrade_traits[k] != 0) {
                        plr->evolutionary_upgrade_traits[k] = MOX::_special_bonus[k][plr->evolutionary_upgrade_traits[k]];
                    }
                }
                MOX::Mox_Update_();

                // Assign unique player color
                int16_t color = (int16_t)random::Random_(8) - 1;
                bool duplicate_color;
                do {
                    duplicate_color = false;
                    for (int16_t j = 0; j < i; ++j) {
                        if (MOX::_player[j].color == (char)color) {
                            duplicate_color = true;
                            break;
                        }
                    }
                    if (duplicate_color) {
                        int val = color + 1;
                        color = (val < 0 ? -val : val) % 8;
                    }
                } while (duplicate_color);

                plr->color = (char)color;
                MOX::Mox_Update_();

                // Load a random race name string as the player name
                allocate::Push_Block_(MOX::_screen_seg);
                char* name_ptr = (char*)farload::Far_Reload_Next_Data_(s_RACENAME_LBX, 0, MOX::_screen_seg, (random::Random_(8) + i * 8) - 1, 1, 20);
                MOX::Mox_Update_();
                allocate::Pop_Block_(MOX::_screen_seg);

                for (int k = 0; k < 20; ++k) {
                    plr->name[k] = name_ptr[k];
                    if (name_ptr[k] == '\0') break;
                }
            }
        }

        MOX::Mox_Update_();
        KEN::Init_NPC_Personalities_Objectives_Themes_();
        MOX::Mox_Update_();

        for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            s_player* plr = &MOX::_player[i];
            plr->officer_for_hire = -1;
            plr->offensive_stagepoint = -1;
            plr->applied_evolutionary_upgrade = 1;
            plr->player_done_flags = 0;
            plr->bc = 50;

            MOX::Mox_Update_();
            plr->cheated = 0;
            plr->history_btns = 'O'; // Hex 0x4F
            plr->ignoring = 0;
            plr->n_freighters = 0;

            for (int j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                plr->last_tech_report_date[j] = MOX::_stardate;
            }

            MOX::Mox_Update_();
            TECHINIT::Init_Player_Tech_(i);
            MOX::Mox_Update_();
            INITSHIP::Init_Ship_Designs_(i);
            
            int16_t fuel_idx = INITSHIP::Best_Ship_Fuel_(i);
            plr->ship_range = TECHDATA::_fuel[fuel_idx].ship_range;
        }

        // Ensure current human player evolution flag is set
        MOX::_player[MOX::_PLAYER_NUM].applied_evolutionary_upgrade = 1;

        MOX::Mox_Update_();
        HOMEGEN::Assign_Home_Star_Names_();
        MOX::Mox_Update_();
        diplomac::Init_Diplomatic_Relations_();
        MOX::Mox_Update_();
    }

    /**
     * Initializes leader data from LBX, sets random skills for specific officers,
     * and calculates initial officer skill values.
     */
    void __cdecl Init_Leaders_() {
        char lbx_filename[20];
        HAROLD::Get_Officer_Data_LBX_Name_(lbx_filename);

        // Loads 67 leader records (59 bytes each) into a temporary buffer.
        void* loaded_data = farload::Far_Reload_Data_(lbx_filename, 0, MOX::_screen_seg, 0, 67, 59);

        // Bulk copy loaded data into the global leaders array.
        uint8_t* src = reinterpret_cast<uint8_t*>(loaded_data);
        uint8_t* dst = reinterpret_cast<uint8_t*>(MOX::_leaders);
        for (int32_t i = 0; i < (67 * 59); ++i) {
            dst[i] = src[i];
        }

        for (int16_t i = 0; i < 67; ++i) {
            if (i == 41) {
                // Special handling for leader at index 41
                OFFICER::Get_Random_Skills_(&MOX::_leaders[i].special_skills, &MOX::_leaders[i].general_skills, 2, 5, 1);
            }
            else if (i == 52) {
                // Special handling for leader at index 52
                OFFICER::Get_Random_Skills_(&MOX::_leaders[i].special_skills, &MOX::_leaders[i].general_skills, 1, 5, 1);
            }
            else if (i == 65) {
                // Special randomized tech application for leader at index 65
                MOX::_leaders[i].tech_application[0] = static_cast<uint8_t>(random::Random_(102)) - 1;
                MOX::_leaders[i].tech_application[1] = static_cast<uint8_t>(random::Random_(102)) + 101;
            }

            // Calculate and set the skill value for this leader.
            MOX::_leaders[i].skill_value = OFFICER::Officer_Skill_Value_(i);

            // Refresh systems after each leader update.
            MOX::Mox_Update_();
        }
    }

    /**
     * @address 00478d10
     */
    void Set_Initial_Colony_Auto_Selects_() {
        if (MOX::_NUM_STARS <= 0) {
            return;
        }

        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            // Initialize selection history to none (-1) for all planets in this system
            for (int planet_idx = 0; planet_idx < 8; ++planet_idx) {
                MOX::_star[star_idx].last_planet_selected[planet_idx] = -1;
            }

            // Determine and set the default auto-selection logic for this star
            int16_t temp_auto_select;
            HACCESS::Set_Colony_Auto_Select_For_Star_(star_idx, MOX::_PLAYER_NUM, &temp_auto_select);
        }
    }

    /**
     * Iterates through all systems to spawn Space Monsters and Orion Guardians.
     * Replaces monster star markers with their original values and upgrades guarded planets.
     */
    /**
     * @address 00432100
     */
    int32_t __cdecl Get_Design_Hull_Size_() {
        // Access hull_size from TECHDATA::_hull_data array based on current design's ship size
        int32_t hull_size = static_cast<int32_t>(TECHDATA::_hull_data[MOX::_design->ship_size].hull_size);

        // Check for Megafluxers (TECH_APP_MEGAFLUXERS = 156)
        // BUG: The decompiled code uses 156 (TECH_APP_ROBO_MINERS) instead of Megafluxers (89) for hull bonus.
        if (MOX::_player[MOX::_PLAYER_NUM].tech_applications[TECH_APP_ROBO_MINERS] == 3) {
            // Apply 25% hull space bonus: (hull_size * 125) / 100
            hull_size = (hull_size * 125) / 100;
        }

        return hull_size;
    }

    void __cdecl Make_System_Monsters_Into_Ships_() {
        int16_t total_stars = MOX::num_stars;
        s_star_data* star_ptr = MOX::_star;
        s_star_data* end_ptr = &MOX::_star[total_stars];

        // Process standard Space Monsters
        while (star_ptr < end_ptr) {
            if (star_ptr->system_special == SYSTEM_SPECIAL_SPACE_MONSTER) {
                int32_t ship_type = random::Random_(5) + 9;
                int16_t star_idx = static_cast<int16_t>(star_ptr - MOX::_star);
                int16_t planet_idx = HOMEGEN::Random_Planet_(star_idx);

                // Initialize monster ship owner to Neutral/Orion
                MOX::_ship[MOX::_NUM_SHIPS].owner = 8;
                SHIPS::Build_Nonplayer_Ship_(ship_type, star_idx, 0);

                // Restore previous special status from backup field and clear backup
                star_ptr->system_special = star_ptr->twinkling1;
                star_ptr->twinkling1 = 0;

                HOMEGEN::Upgrade_Planet_Guarded_By_Monster_(&MOX::_planet[planet_idx], static_cast<int16_t>(ship_type));
            }
            star_ptr++;
        }

        // Process Orion Star Guardian (Limit to one)
        bool orion_spawned = false;
        for (int16_t i = 0; i < total_stars; ++i) {
            if (orion_spawned) break;

            if (MOX::_star[i].system_special == SYSTEM_SPECIAL_ORION_STAR) {
                orion_spawned = true;
                MOX::_ship[MOX::_NUM_SHIPS].owner = 8;
                SHIPS::Build_Nonplayer_Ship_(9, i, 0);
            }
        }
    }

}