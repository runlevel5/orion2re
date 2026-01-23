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

            // Set planet index to -1 (unassigned/none)
            colony.planet = -1;
            // Set owner index to -1 (no owner)
            colony.owner = -1;
            // Set officer index to -1 (no officer assigned)
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

    void __cdecl Init_New_Game_(int16_t game_difficulty, int16_t galaxy_size, int32_t galaxy_age, int16_t num_players,
                                int16_t strategic_combat_flag, int16_t starting_civ_level) {
        bool extra_maps_generated = false;
        int map_index = 0;
        uint16_t generation_success;

        MOX::_settings.start_of_game_seed = random::Get_Random_Seed_();
        MOX2::Set_Game_Seed();
        MOX::_stardate = 35000;
        MOX::_NUM_PLAYERS = num_players;

        Init_Ships_Array_();
        RUSS::Mox_Update_();
        Init_Colonies_Array_();
        RUSS::Mox_Update_();

        // Clear antaran invasion data (66 bytes)
        for (int i = 0; i < 66; ++i) {
            MOX::_antarans_blob[i] = 0;
        }
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

        do {
            MOX2::Set_Game_Seed();
            MOX::_settings.start_of_game_seed = random::Get_Random_Seed_();

            MAPGEN::Universe_Generation_(galaxy_size, galaxy_age, game_difficulty);
            RUSS::Mox_Update_();

            generation_success = HOMEGEN::Generate_Home_Worlds_();
            RUSS::Mox_Update_();

            if (generation_success != 0) {
                video::Set_Page_Off_();
                video::Fill_(0, 0, 0x27F, 0x1DF, 0);
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

        RUSS::Mox_Update_();
        HOMEGEN::Make_System_Monsters_();
        RUSS::Mox_Update_();
        Make_System_Monsters_Into_Ships_();
        RUSS::Mox_Update_();
        MAPGEN::Generate_Wormhole_Links_();
        RUSS::Mox_Update_();

        if (MOX::_settings.starting_civilization_level == 2) {
            FUN_00443C60();
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

}