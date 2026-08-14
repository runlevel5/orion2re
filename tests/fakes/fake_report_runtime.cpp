#include "pch.h"

namespace {
    s_colony colonies[MAX_COLONIES];
    s_planet_data planets[MAX_PLANETS];
    s_player players[MAX_PLAYERS];
    s_ship_data ships[MAX_SHIPS];
    s_star_data stars[MAX_STARS];
    char planet_name[] = "Planet";
}

namespace TEST_REPORT {
    int16_t colonizable_planet_count;

    void Reset_() {
        memset(colonies, 0, sizeof(colonies));
        memset(planets, 0, sizeof(planets));
        memset(players, 0, sizeof(players));
        memset(ships, 0, sizeof(ships));
        memset(stars, 0xff, sizeof(stars));
        memset(REPORT::_done_colonize_check, 0, sizeof(REPORT::_done_colonize_check));
        memset(&MOX::_settings, 0, sizeof(MOX::_settings));

        MOX::_NUM_STARS = 1;
        MOX::_NUM_SHIPS = 1;
        MOX::_star[0].colonize_player = 0;
        MOX::_star[0].ignore_colony_ships = 0;
        MOX::_ship[0].owner = 0;
        MOX::_ship[0].location = 0;
        MOX::_ship[0].d.ship_type = SHIP_TYPE_OUTPOST;
        REPORT::_no_outpost_prompt = 0;
        colonizable_planet_count = 0;
    }

    void Add_Colony_Ship_() {
        MOX::_ship[1].owner = 0;
        MOX::_ship[1].location = 0;
        MOX::_ship[1].d.ship_type = SHIP_TYPE_COLONY;
        MOX::_NUM_SHIPS = 2;
    }
}

namespace MOX {
    int16_t _NUM_STARS;
    int16_t _NUM_SHIPS;
    int16_t _current_screen;
    int16_t _return_screen;
    int16_t _screen_data;
    uint16_t _global_esc;
    s_colony* _colony = colonies;
    s_planet_data* _planet = planets;
    s_player* _player = players;
    s_ship_data* _ship = ships;
    s_star_data* _star = stars;
    s_settings _settings;
}

namespace COLCALC {
    void __cdecl Update_Player_Stats_(s_player*) {
    }
}

namespace COLONIZE {
    void __cdecl Make_New_Colony_(int16_t, int16_t) {
    }

    void __cdecl Make_New_Outpost_(int16_t, int16_t) {
    }

    int16_t Star_N_Colonizable_Planets_For_Player_(s_star_data*, int16_t) {
        return TEST_REPORT::colonizable_planet_count;
    }
}

namespace COLONY {
    int16_t _orbit_temp;
}

namespace ERICNET {
    void __cdecl Remove_Building_(int16_t, int16_t) {
    }
}

namespace GENDRAW {
    int16_t __cdecl Do_Confirmation_Box_(const char*) {
        return 0;
    }
}

namespace HACCESS {
    char* __cdecl Get_Planet_Name_(int16_t) {
        return planet_name;
    }
}

namespace MAINPUPS {
    int16_t __cdecl New_Colony_Selection_Popup_(int16_t, uint8_t) {
        return PLANET_INDEX_NONE;
    }

    int16_t __cdecl New_Outpost_Selection_Popup_(int16_t) {
        return PLANET_INDEX_NONE;
    }
}

namespace MAINSCR {
    void __cdecl Update_Ship_Icons_In_Between_Turns_() {
    }
}

namespace MOVEBOX {
    void __cdecl Remove_Box_Check_() {
    }
}

namespace PLNTSUM {
    int16_t __cdecl Colony_Ship_Auto_Colonization_(s_ship_data*) {
        return PLANET_INDEX_NONE;
    }
}

namespace SHIPS {
    void __cdecl Clear_Auto_Colonization_Flag_(s_ship_data*) {
    }

    void __cdecl Kill_Ship_(int16_t) {
    }
}

namespace TECHDATA {
    s_tech_building_data _buildings[BUILDING_COUNT];
}

namespace namestar {
    int8_t __cdecl Can_Change_Star_Name_(int16_t) {
        return 0;
    }

    void __cdecl Change_Star_Name_Popup_(int16_t, int16_t, int16_t, int8_t) {
    }

    int16_t __cdecl Star_Name_Popup_Screen_Center_X_() {
        return 0;
    }

    int16_t __cdecl Star_Name_Popup_Screen_Center_Y_() {
        return 0;
    }
}

namespace struct_ {
    int16_t __cdecl Test_Bit_Field_(void* bit_field, uint16_t bit_index) {
        const uint8_t* bytes = static_cast<const uint8_t*>(bit_field);
        return (bytes[bit_index / 8] >> (bit_index % 8)) & 1;
    }

    void __cdecl Set_Bit_Field_(void* bit_field, uint16_t bit_index) {
        uint8_t* bytes = static_cast<uint8_t*>(bit_field);
        bytes[bit_index / 8] |= (uint8_t)(1u << (bit_index % 8));
    }
}