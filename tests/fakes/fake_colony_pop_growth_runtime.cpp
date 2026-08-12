#include "pch.h"

namespace {
    s_colony colonies[2];
    s_planet_data planet_storage[2];
    s_player players[1];
    s_star_data stars[1];
    s_ship_data ships[1];
    s_leader_data leaders[1];
    int32_t destroy_colony_reason;
    int16_t add_spy_result;
    int16_t deleted_product;
    bool population_boom;
    bool plague;
}

namespace TEST_COLONY_POP_GROWTH {
    void Reset_() {
        memset(colonies, 0, sizeof(colonies));
        memset(planet_storage, 0, sizeof(planet_storage));
        memset(players, 0, sizeof(players));
        memset(stars, 0, sizeof(stars));
        memset(ships, 0, sizeof(ships));
        memset(leaders, 0, sizeof(leaders));
        memset(TECHDATA::_buildings, 0, sizeof(TECHDATA::_buildings));
        planet_storage[0].colony_index = 1;
        planet_storage[1].size = PLANET_SIZE_HUGE;
        colonies[1].buildings[BUILDING_CAPITOL] = 1;
        stars[0].officer_index[0] = -1;
        TECHDATA::_buildings[BUILDING_CAPITOL].tech_app = TECH_APP_CAPITOL;
        destroy_colony_reason = -1;
        add_spy_result = 1;
        deleted_product = COLONY_PRODUCTION_NONE;
        population_boom = false;
        plague = false;
        COLCALC::growth_rate_bonus = {
            0,
            25,
            50,
            100,
            -200,
        };
        COLCALC::_pop_bonus_biospheres = 2;
        COLCALC::_housing_creates_no_farmers = 0;
        COLCALC::_planet_extinction = 0;
        COLCALC::_zero_colony_lost = 0;
        COLCALC::_freighters_cash_bonus = 0;
        COLCALC::lander_ship_cost.colony_ship = 500;
        COLCALC::lander_ship_cost.outpost_ship = 100;
        COLCALC::lander_ship_cost.transport_ship = 100;
        COLCALC::misc_costs.android_farmer = 50;
        COLCALC::misc_costs.android_worker = 50;
        COLCALC::misc_costs.android_scientist = 50;
    }

    int32_t Destroy_Colony_Reason_() {
        return destroy_colony_reason;
    }

    void Set_Add_Spy_Result_(int16_t result) {
        add_spy_result = result;
    }

    int16_t Deleted_Product_() {
        return deleted_product;
    }

    void Set_Population_Boom_(bool enabled) {
        population_boom = enabled;
    }

    void Set_Plague_(bool enabled) {
        plague = enabled;
    }
}

namespace MOX {
    int16_t _NUM_COLONIES = 1;
    int16_t _NUM_PLAYERS = 1;
    s_colony* _colony = colonies;
    s_planet_data* _planet = &planet_storage[1];
    s_player* _player = players;
    s_star_data* _star = stars;
    s_ship_data* _ship = ships;
    s_leader_data* _leaders = leaders;
    s_settings _settings = {};
    s_crew_data _crew_data[CREW_QUALITY_COUNT] = {};
    int8_t _planet_max_population[PLANET_SIZE_COUNT] = {5, 10, 15, 20, 25};
}

namespace TECHDATA {
    s_tech_building_data _buildings[BUILDING_COUNT] = {};
    s_tech_field_data _technology_fields[TECH_FIELD_COUNT] = {};
}

namespace EVENTS {
    int16_t __cdecl Event_Check_Space_Anomaly_(int16_t) {
        return 0;
    }

    int16_t __cdecl Event_Check_Colony_Researching_(int16_t) {
        return 0;
    }

    int16_t __cdecl Event_Check_Population_Boom_(int16_t) {
        return population_boom;
    }

    int16_t __cdecl Event_Check_Plague_(int16_t) {
        return plague;
    }
}

namespace OFFICER {
    int16_t __cdecl Owned_Officer_Level_(int16_t) {
        return 0;
    }
}

namespace special {
    uint32_t __cdecl isqrt_(uint32_t) {
        return 100;
    }
}

namespace MSG_ {
    void __cdecl Add_Msg_(s_MSG, int16_t) {
    }
}

namespace RUSS {
    void __cdecl Shuffle_Sint_(int16_t*, int16_t) {
    }

    int16_t __cdecl Get_Weighted_Choice_Int_(uint32_t*, int16_t) {
        return 1;
    }
}

namespace BOMB {
    void __cdecl Destroy_Colony_(s_colony*, int32_t capturing_player) {
        destroy_colony_reason = capturing_player;
    }
}

namespace ERICNET {
    void __cdecl Add_Building_(int16_t colony_idx, int16_t building_idx) {
        MOX::_colony[colony_idx].buildings[building_idx] = 1;
    }

    void __cdecl Calc_Ship_Level_(int16_t) {
    }

    void __cdecl Remove_Building_(int16_t, int16_t) {
    }
}

namespace AIBUILD {
    void __cdecl Player_Colony_Autobuild_(s_colony*) {
    }
}

namespace BILL {
    int16_t __cdecl Add_Their_Spy_(int16_t, int16_t) {
        return add_spy_result;
    }
}

namespace BUILD_QUEUE {
    bool Apply_Custom_Autobuild_(int16_t) {
        return false;
    }
}

namespace COLBLDG {
    int16_t __cdecl Create_Ship_(int16_t, int16_t, int16_t) {
        return -1;
    }

    void __cdecl Delete_Prod_From_Colony_Queue_(int16_t, int16_t product) {
        deleted_product = product;
    }
}

namespace DESIGN {
    int16_t __cdecl Cost_Of_Design_(int16_t, s_ship_design*) {
        return 0;
    }

    void __cdecl Cost_Reduction_For_Govt_Type_(int16_t, int32_t*) {
    }
}

namespace COLCALC {
    void __cdecl Colony_Morale_(s_colony*, STRBUILD::s_builder*) {
    }

    void __cdecl Colony_Industry_Production_(s_colony*, STRBUILD::s_builder*) {
    }

    void __cdecl Colony_Food_Production_(s_colony*, STRBUILD::s_builder*) {
    }

    void __cdecl Colony_Replicators_(s_colony*) {
    }

    void __cdecl Colony_Research_Production_(s_colony*, STRBUILD::s_builder*) {
    }

    void __cdecl Colony_BC_Production_(s_colony*, STRBUILD::s_builder*) {
    }

    void __cdecl Pass_Out_Imports_(int16_t, s_colony*) {
    }

    void __cdecl Update_Player_Stats_(s_player*) {
    }
}

namespace COLONY {
    void __cdecl Fix_Colony_Queue_(int16_t) {
    }
}

namespace game_random {
    uint32_t __cdecl Random_(uint32_t) {
        return 1;
    }
}