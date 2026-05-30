#include "pch.h"

namespace MOX {
    const char _ship_type_names[5][6][16] = {
        {
            "combat ships",
            "not used",
            "",
            "",
            "",
            "",
        },
        {
            "Colony Ship",
            "Kolonieschiff",
            "Colonial",
            "Nave Colonia",
            "Nave colonia",
            "Colony Ship",
        },
        {
            "Transport",
            "Transporter",
            "Transport",
            "Transporte",
            "Nave truppa",
            "Transport",
        },
        {
            "tanks",
            "not used",
            "",
            "",
            "",
            "",
        },
        {
            "Outpost Ship",
            "Au|enposten",
            "Avante-poste",
            "Puesto Avanzado",
            "Nave avamposto",
            "Outpost Ship",
        },
    };

    s_fleet_movement_icon _fleet_movement_icon[500];

    void __cdecl Apply_Save_Settings(s_settings* s) {
        MOX::_settings.xenons_exist = s->xenons_exist;
        MOX::_settings.game_difficulty = s->game_difficulty;
        MOX::_settings.number_of_players = s->number_of_players;
        MOX::_settings.galaxy_size = s->galaxy_size;
        MOX::_settings.galaxy_age = s->galaxy_age;
        MOX::_settings.strategic_combat_flag = s->strategic_combat_flag;
        MOX::_settings.starting_civilization_level = s->starting_civilization_level;
        MOX::_settings.start_of_game_seed = s->start_of_game_seed;
        MOX::_settings.random_events = s->random_events;
    }
}