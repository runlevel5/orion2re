#include "deps.h"

namespace INITGAME {

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

}