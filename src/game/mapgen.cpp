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

    uint8_t __cdecl Generate_Max_Farms_(int16_t star_idx, int16_t planet_idx) {
        bool is_habitable = HAROLD::Habitable_Spectral_Class_(star_idx);

        if (!is_habitable && MOX::_star[star_idx].system_special != MOX::SYSTEM_SPECIAL_ORION_STAR) {
            return 0;
        }

        int8_t size_idx = MOX::_planet[planet_idx].size;
        return MOX::_planet_max_farms[size_idx];
    }

    int8_t __cdecl Generate_Orbit_(int16_t star_id) {
        uint8_t weights[5];

        static const uint8_t weight_table[15] = {
            25, 20, 10,
            18, 20, 22,
            17, 20, 30,
            15, 20, 33,
            25, 20, 5
        };

        for (int i = 0; i < 5; ++i) {
            weights[i] = weight_table[i * 3 + MOX::_settings_galaxy_age];
        }

        int8_t choice;
        bool found = false;

        do {
            choice = RUSS::Get_Weighted_Choice_Char_(weights, 5);
            if (MOX::_star[star_id].planet_index[choice] == -1) {
                found = true;
            }
        } while (!found);

        return choice;
    }

    /**
     * @brief Generates initial data for a satellite (moon) in a star system.
     * @param star_index Index of the star this satellite orbits.
     */
    void __cdecl Generate_Satellite_(int16_t star_index) {
        s_planet_data* planet = &MOX::_planet[MOX::_planet_count];

        planet->star_index = (int8_t)star_index;

        planet->size = Generate_Size_();

        planet->mineral_class = Generate_Mineral_Class_(star_index, MOX::_planet_count);

        Get_Climate_Modifier_(planet->mineral_class);

        planet->gravity_class = (int8_t)Generate_Gravity_Class_(MOX::_planet_count);

        planet->group = (int8_t)Get_Planet_Group_(MOX::_planet_count);

        planet->climate = Generate_Climate_((int16_t)planet->group);

        planet->climate_bg_type = (int8_t)(random::Random_(3) - 1);

        Set_Secondary_Planet_Data_(star_index, MOX::_planet_count);
    }
}