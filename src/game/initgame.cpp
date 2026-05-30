#include "pch.h"

namespace INITGAME {

    /**
     * Initializes the ship data array. 
     * Zeroes out the entire array (500 ships), then sets default status, 
     * location, and officer values for each.
     */
    void Init_Ships_Array_() {
        // Clear 500 packed 0x81-byte ship slots.
        memset(MOX::_ship, 0, 500 * sizeof(s_ship_data));

        // After the clear, initialize:
        //   status        @ +0x64 = 5
        //   location      @ +0x65 = -1
        //   officer_index @ +0x74 = -1
        for (MOX::_NUM_SHIPS = 0; MOX::_NUM_SHIPS < 500; ++MOX::_NUM_SHIPS) {
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
        memset(MOX::_colony, 0, 250 * sizeof(s_colony));

        for (MOX::_NUM_COLONIES = 0; MOX::_NUM_COLONIES < 250; ++MOX::_NUM_COLONIES) {
            s_colony& colony = MOX::_colony[MOX::_NUM_COLONIES];

            colony.planet = -1;
            colony.owner = -1;
            colony.officer = -1;
        }

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
        MOX::_settings.tactical_combat_speed = 0;
        MOX::_settings.message_delay = 0;

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

        MOX::_settings.sound_driver_id = 2;
        MOX::_settings.music_driver_id = 1;

        // Clear the first byte of each save-game description record.
        for (auto& save_game_description : MOX::_save_game_description) {
            save_game_description.description[0] = '\0';
        }

        // Generate default network name: MOX-<clock % 10>
        int clock_val = clock();
        sprintf(MOX::_settings.net_game_name, "MOX-%d", clock_val % 10);

        // Finalize default settings
        MULTPLAY::Set_Default_Modem_Settings_();
        INITGAME::Write_Game_Settings_();
    }

    /**
     * @address 00478fb0
     */
    void Write_Game_Settings_() {
        FILE* file = fopen(s_mox_set_0055760c, s_wb_00551178);

        if (file == nullptr) {
            exitm::Exit_With_Message(s_Unable_to_open_mox_set_005575f4);
        }

        fwrite(&MOX::_settings, sizeof(MOX::_settings), 1, file);
        fflush(file);
        fclose(file);
    }

    void __cdecl Init_New_Game_(int16_t game_difficulty, int16_t galaxy_size, int16_t galaxy_age, int16_t num_players,
                                int16_t strategic_combat_flag, int16_t starting_civ_level) {
        bool extra_maps_generated = false;
        int16_t map_index = 0;
        int16_t generation_success;

        MOX::_settings.start_of_game_seed = random::Get_Random_Seed_();
        MOX2::Set_Game_Seed();
        MOX::_stardate = 35000;
        MOX::_NUM_PLAYERS = num_players;

        Init_Ships_Array_();
        RUSS::Mox_Update_();
        Init_Colonies_Array_();
        RUSS::Mox_Update_();

        // Clear antaran invasion data (66 bytes)
        memset(&MOX::_antarans, 0, sizeof(MOX::_antarans));
        RUSS::Mox_Update_();

        if (MOX::_game_type != 2) {
            MOX::_PLAYER_NUM = 0;
        }

        Init_Leaders_();
        RUSS::Mox_Update_();
        Init_Players_();
        RUSS::Mox_Update_();
        BILL::Cache_Player_Scientist_Anim_();
        RUSS::Mox_Update_();
        MSG_::Allocate_Msg_Slots_();

        MOX2::Set_Game_Seed();
        MOX::_settings.start_of_game_seed = random::Get_Random_Seed_();

        do {
            MAPGEN::Universe_Generation_((uint8_t)galaxy_size, (uint8_t)galaxy_age, (int8_t)game_difficulty);
            RUSS::Mox_Update_();

            generation_success = HOMEGEN::Generate_Home_Worlds_();
            RUSS::Mox_Update_();

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

            if (map_index < MOX::_maps_to_generate) {
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

                const int16_t map_no = map_index;
                map_index++;
                RUSS::PFMT_(0, 0x17, RUSS::s_Map_d, map_no);

                RUSS::PFMT_(0, 0x18, RUSS::s_Seed_ld, MOX::_settings.start_of_game_seed);
                capture::Screen_Flic_Capture_();
            }
        } while (generation_success == 0);

        if (extra_maps_generated) {
            exitm::Exit_With_Message("done");
        }

        RUSS::Mox_Update_();
        HOMEGEN::Make_System_Monsters_();
        RUSS::Mox_Update_();
        Make_System_Monsters_Into_Ships_();
        RUSS::Mox_Update_();
        MAPGEN::Generate_Wormhole_Links_();
        RUSS::Mox_Update_();

        if (MOX::_settings.starting_civilization_level == 2) {
            OFFICER::Allocate_Adv_Civ_Game_Officers_();
            RUSS::Mox_Update_();
        }

        MAPGEN::Assign_Marooned_Heroes_();
        RUSS::Mox_Update_();
        RUSS::Mox_Update_();

        GEO::Initialize_Black_Hole_Blocks_(45);
        RUSS::Mox_Update_();
        GEO::Initialize_Star_In_Nebula_Info_();
        RUSS::Mox_Update_();

        MOX2::Init_Map_Defaults_();
        KEN::Ken_Init_();
        HAROLD::Harold_Init_();
        ERIC::Eric_Init_();
        BILL::Bill_Init_();
        MAINSCR::Update_Star_Owners_();
        RUSS::Mox_Update_();

        COLONIZE::Allocate_New_Colony_Slots_();
        RUSS::Mox_Update_();
        COLCALC::Twiddle_Initial_Homeworlds_();
        RUSS::Mox_Update_();
        SHIPMOVE::Allocate_New_Ship_Slots_();
        RUSS::Mox_Update_();
        COLCALC::Compute_All_Star_Colony_Stuff_();
        RUSS::Mox_Update_();
        TRADE::Process_Trade_And_Research_Agreements_();
        RUSS::Mox_Update_();

        if (MOX::_settings.auto_select_colony != 0) {
            Set_Initial_Colony_Auto_Selects_();
        }
        RUSS::Mox_Update_();
        REPORT::Initialize_Reports_();
        RUSS::Mox_Update_();
        COLCALC::Init_Surrenders_();
        RUSS::Mox_Update_();
        RUSS::Mox_Update_();

        FILEDEF::Save_Game_(9);
        RUSS::Mox_Update_();
        FILEDEF::Save_Game_Settings_();
        RUSS::Mox_Update_();
        MOX2::Init_Map_Pictures_Default__();

        if (MOX::_game_type == 0) {
            FILEDEF::Save_Moo_Custom_Player_Data_(MOX::_player);
        }

        random::Set_Random_Seed_(MOX::_settings.start_of_game_seed);
    }

    void __cdecl Init_Players_(void) {
      RACESEL::_race_specials[0] = (uint8_t*)farload::Far_Reload_Data_(
          s_RACESTUF_LBX,
          7,
          MOX::_global_data_seg,
          0,
          1,
          31);
      RUSS::Mox_Update_();

      for (int16_t race_special_idx = 1; race_special_idx < 13; ++race_special_idx) {
        RACESEL::_race_specials[race_special_idx] =
            (uint8_t*)farload::Far_Reload_Next_Data_(
                s_RACESTUF_LBX,
                7,
                MOX::_global_data_seg,
                race_special_idx,
                1,
                31);
        RUSS::Mox_Update_();
      }

      RUSS::Mox_Update_();

      for (int16_t player_idx = 0; player_idx < MOX::_NUM_PLAYERS; ++player_idx) {
        s_player& player = MOX::_player[player_idx];

        if (player.objectives == 100) {
          struct_::Copy_Structure_(
              player.evolutionary_upgrade_traits,
              player.traits,
              31);
          continue;
        }

        int16_t race_idx = 0;
        while (true) {
          bool unique_race = true;
          race_idx = static_cast<int16_t>(random::Random_(13) - 1);

          for (int16_t prev_player_idx = 0; prev_player_idx < player_idx; ++prev_player_idx) {
            if (!unique_race) {
              break;
            }

            if (static_cast<uint8_t>(MOX::_player[prev_player_idx].race) ==
                static_cast<uint8_t>(race_idx)) {
              unique_race = false;
            }
          }

          if (unique_race) {
            break;
          }
        }

        player.race = static_cast<uint8_t>(race_idx);
        strcpy(player.race_name, MOX::_race_names[race_idx]);

        struct_::Copy_Structure_(player.traits, RACESEL::_race_specials[race_idx], 31);
        struct_::Copy_Structure_(player.evolutionary_upgrade_traits, RACESEL::_race_specials[race_idx], 31);
        RUSS::Mox_Update_();

        RACECUST::Race_Customization_(player_idx);
        RUSS::Mox_Update_();

        for (int16_t trait_idx = 1; trait_idx < 10; ++trait_idx) {
          uint8_t v = (uint8_t)player.traits[trait_idx];
          player.traits[trait_idx] =
              v ? MOX::_special_bonus[trait_idx - 1][v - 1] : 0;

          v = (uint8_t)player.evolutionary_upgrade_traits[trait_idx];
          player.evolutionary_upgrade_traits[trait_idx] =
              v ? MOX::_special_bonus[trait_idx - 1][v - 1] : 0;
        }

        RUSS::Mox_Update_();

        int16_t color_idx = (int16_t)(random::Random_(8) - 1);
        while (true) {
          bool duplicate_color = false;

          for (int16_t prev_player_idx = 0; prev_player_idx < player_idx; ++prev_player_idx) {
            if (MOX::_player[prev_player_idx].color == (uint8_t)color_idx) {
              duplicate_color = true;
            }
          }

          if (!duplicate_color) {
            break;
          }

          int32_t t = (int16_t)color_idx + 1;
          int32_t s = t >> 31;
          color_idx = (int16_t)((((t ^ s) - s) & 7 ^ s) - s);
        }

        player.color = (uint8_t)color_idx;
        RUSS::Mox_Update_();

        allocate::Push_Block_(MOX::_screen_seg);

        char* player_name = static_cast<char*>(farload::Far_Reload_Next_Data_(
            s_RACENAME_LBX,
            0,
            MOX::_screen_seg,
            race_idx * 8 + random::Random_(8) - 1,
            1,
            20));
        RUSS::Mox_Update_();

        allocate::Pop_Block_(MOX::_screen_seg);

        strcpy(player.name, player_name);
      }

      RUSS::Mox_Update_();
      KEN::Init_NPC_Personalities_Objectives_Themes_();
      RUSS::Mox_Update_();

      for (int16_t player_idx = 0; player_idx < MOX::_NUM_PLAYERS; ++player_idx) {
        s_player& player = MOX::_player[player_idx];

        player.officer_for_hire = -1;
        player.offensive_stagepoint = static_cast<char>(0xFF);
        player.contact[player_idx] = 1;
        player.player_done_flags = 0;
        player.bc = 50;
        RUSS::Mox_Update_();

        player.spies[player_idx] = 0;
        player.history_btns = 'O';
        player.ignoring = 0;
        player.n_freighters = 0;

        for (int16_t other_player_idx = 0; other_player_idx < MOX::_NUM_PLAYERS; ++other_player_idx) {
          player.last_tech_report_date[other_player_idx] = MOX::_stardate;
        }

        RUSS::Mox_Update_();
        TECHINIT::Init_Player_Tech_(player_idx);
        RUSS::Mox_Update_();
        INITSHIP::Init_Ship_Designs_(player_idx);

        const int16_t best_fuel_idx = INITSHIP::Best_Ship_Fuel_(player_idx);
        player.ship_range = TECHDATA::_fuel[best_fuel_idx].ship_range;
      }

      MOX::_player[MOX::_PLAYER_NUM].contact[MOX::_PLAYER_NUM] = 1;
      RUSS::Mox_Update_();
      HOMEGEN::Assign_Home_Star_Names_();
      RUSS::Mox_Update_();
      diplomac::Init_Diplomatic_Relations_();
      RUSS::Mox_Update_();
    }

    /**
     * Initializes leader data from LBX, sets random skills for specific officers,
     * and calculates initial officer skill values.
     */
    /**
     * @address 004794b0
     */
    void __cdecl Init_Colony_(int16_t colony_idx, int16_t planet_idx, uint8_t player_num) {
        s_colony* colony = &MOX::_colony[colony_idx];

        // Zero-initialize the entire colony structure
        memset(colony, 0, sizeof(s_colony));

        s_planet_data* planet = &MOX::_planet[planet_idx];
        planet->colony_index = (int16_t)colony_idx;

        colony->planet = (int16_t)planet_idx;
        colony->owner = player_num;
        colony->officer = -1;

        int8_t star_idx = planet->star_index;
        // Mark the star system as visited/seen by the player
        uint8_t player_bit = (uint8_t)(1u << (player_num & 0x1f));
        MOX::_star[star_idx].visited |= player_bit;

        for (int i = 0; i < 7; ++i) {
            colony->producing[i] = -1;
        }

        colony->occupation_policy = 4;
        colony->just_produced = -1;
    }

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
            RUSS::Mox_Update_();
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