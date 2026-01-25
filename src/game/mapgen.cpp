#include "deps.h"

namespace MAPGEN {
    uint8_t __cdecl Generate_Spectral_Class_(int16_t star_type_index) {
        uint8_t weights[7];

        for (int32_t i = 0; i < 7; ++i) {
            weights[i] = MOX::_star_class_table[i * 3 + star_type_index];
        }

        return RUSS::Get_Weighted_Choice_Char_(weights, 7);
    }

    /**
     * @brief Determines the base food production per farmer for a given planet.
     * 
     * Planets in systems with non-habitable spectral classes produce no food
     * unless the system contains the Orion star special.
     */
    uint8_t __cdecl Generate_Food_Per_Farmer_(int16_t star_index, int16_t planet_index) {
        bool habitable = HAROLD::Habitable_Spectral_Class_(star_index);

        // Check if the system is either habitable or is the Orion system
        if (!habitable && MOX::_star[star_index].system_special != MOX::SYSTEM_SPECIAL_ORION_STAR) {
            return 0;
        }

        s_planet_data* planet = &MOX::_planet[planet_index];
        uint8_t climate_idx = static_cast<uint8_t>(planet->climate);
        
        return MOX::_food_per_farmer_table[climate_idx];
    }
}