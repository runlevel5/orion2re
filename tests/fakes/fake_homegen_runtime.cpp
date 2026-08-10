#include "pch.h"

void Legacy_Qsort_(void* base, size_t count, size_t size, int (*compare)(const void*, const void*)) {
    if (count == 2) {
        uint8_t* bytes = (uint8_t*)base;
        if (compare(bytes, bytes + size) > 0) {
            for (size_t i = 0; i < size; ++i) {
                uint8_t value = bytes[i];
                bytes[i] = bytes[size + i];
                bytes[size + i] = value;
            }
        }
    }
}

namespace {
    HOMEGEN::s_star_dist star_distances[2];
    s_planet_data planets[2];
    s_star_data stars[2];
    int32_t movement_count;
    int32_t orbit_roll_count;
    int32_t satellite_generation_count;
    int32_t non_black_hole_generation_count;
    int8_t generated_spectral_class;
}

namespace TEST_HOMEGEN {
    void Reset_() {
        memset(star_distances, 0, sizeof(star_distances));
        memset(planets, 0, sizeof(planets));
        memset(stars, 0, sizeof(stars));

        for (int16_t star_idx = 0; star_idx < 2; ++star_idx) {
            for (int16_t orbit = 0; orbit < 5; ++orbit) {
                stars[star_idx].planet_index[orbit] = -1;
            }
        }

        stars[1].planet_index[2] = 0;
        planets[0].star_index = 1;
        planets[0].orbit = 2;
        planets[0].planet_type = PLANET_TYPE_ASTEROID;

        MOX::_planet_count = 1;
        MAPGEN::_make_food_planet = 0;
        movement_count = 0;
        orbit_roll_count = 0;
        satellite_generation_count = 0;
        non_black_hole_generation_count = 0;
        generated_spectral_class = STAR_CLASS_G;
    }

    int32_t Satellite_Generation_Count_() {
        return satellite_generation_count;
    }

    void Set_Generated_Spectral_Class_(int8_t spectral_class) {
        generated_spectral_class = spectral_class;
    }

    int32_t Non_Black_Hole_Generation_Count_() {
        return non_black_hole_generation_count;
    }
}

namespace MOX {
    int16_t _NUM_PLAYERS = 1;
    int16_t _NUM_STARS = 2;
    void* _global_data_seg = nullptr;
    int16_t _planet_count = 1;
    s_planet_data* _planet = planets;
    s_star_data* _star = stars;
    s_settings _settings = {};
}

namespace allocate {
    void* __cdecl Allocate_First_Block_(void*, uint32_t) {
        return star_distances;
    }
}

namespace CMBTMIS {
    void __cdecl Coord_Movement_(int16_t, int16_t, int16_t, int16_t, uint8_t, int16_t* dx, int16_t* dy) {
        *dx = 0;
        *dy = 0;
        movement_count++;
    }
}

namespace GEO {
    int32_t __cdecl Parsecs_Between_Stars_(int16_t, int16_t) {
        return movement_count == 0 ? 5 : 4;
    }
}

namespace HAROLD {
    bool __cdecl Habitable_Spectral_Class_(int16_t star_idx) {
        return MOX::_star[star_idx].spectral_class < STAR_CLASS_BLACK_HOLE;
    }

    uint8_t __cdecl N_Planets_In_System_(int16_t star_idx) {
        uint8_t count = 0;
        for (int16_t orbit = 0; orbit < 5; ++orbit) {
            int16_t planet_idx = MOX::_star[star_idx].planet_index[orbit];
            if (planet_idx >= 0 && MOX::_planet[planet_idx].planet_type == PLANET_TYPE_PLANET) {
                count++;
            }
        }
        return count;
    }
}

namespace MAPGEN {
    uint8_t _make_food_planet = 0;

    int8_t __cdecl Generate_Non_Black_Hole_Spectral_Class_(int16_t) {
        non_black_hole_generation_count++;
        return generated_spectral_class;
    }

    void __cdecl Generate_Satellite_(int16_t star_idx, int16_t orbit, int16_t) {
        MOX::_planet[MOX::_planet_count].star_index = star_idx;
        MOX::_planet[MOX::_planet_count].orbit = (int8_t)orbit;
        satellite_generation_count++;
    }
}

namespace game_random {
    uint32_t __cdecl Random_(uint32_t limit) {
        if (limit == 5) {
            orbit_roll_count++;
            return orbit_roll_count == 1 ? 3 : 4;
        }
        return 1;
    }
}