#pragma once
#include <cstdint>

struct s_unit_data;

namespace TECHDATA {
    // Correct addresses for various TECHDATA structures. If address does not match, then it points inside of structure.
    inline s_techdata_planetary_shield* _planetary_shields =
        reinterpret_cast<s_techdata_planetary_shield*>(std::uintptr_t{ 0x0054F048 });
    inline s_tech_armor_data (&_armor)[7] = *reinterpret_cast<s_tech_armor_data (*)[7]>(std::uintptr_t{0x00554D90});
    inline s_tech_building_data (&_buildings)[49] = *reinterpret_cast<s_tech_building_data(*)[49]>(std::uintptr_t{0x00554290});
    inline s_tech_hull_data (&_hull_data)[9] =
        *reinterpret_cast<s_tech_hull_data(*)[9]>(std::uintptr_t{0x00555798});
    inline s_tech_fuel_data (&_fuel)[6] = *reinterpret_cast<s_tech_fuel_data (*)[6]>(std::uintptr_t{0x00555758});
    inline s_tech_app_data (&_technology_applications)[212] =
        *reinterpret_cast<s_tech_app_data(*)[212]>(std::uintptr_t{0x005537C8});
    inline s_tech_field_data (&_technology_fields)[83] =
        *reinterpret_cast<s_tech_field_data (*)[83]>(std::uintptr_t{ 0x00553050 });
    inline s_tech_drive_data (&_drives)[7] = *reinterpret_cast<s_tech_drive_data (*)[7]>(std::uintptr_t{0x005555E0});
    inline s_tech_shield_data (&_shields)[6] = *reinterpret_cast<s_tech_shield_data (*)[6]>(std::uintptr_t{0x00554E00});
    inline s_tech_computer_data (&_computers)[6] = *reinterpret_cast<s_tech_computer_data (*)[6]>(std::uintptr_t{0x00555558});
    inline s_tech_specials_data (&_specials)[40] =
        *reinterpret_cast<s_tech_specials_data(*)[40]>(std::uintptr_t{0x00554638});
    inline s_tech_weapon_data (&_weapons)[46] =
        *reinterpret_cast<s_tech_weapon_data(*)[46]>(std::uintptr_t{0x00554F68});
    inline s_tech_weapon_mod_data (&_weapon_modifications)[15] =
        *reinterpret_cast<s_tech_weapon_mod_data(*)[15]>(std::uintptr_t{0x00555470});

    inline int16_t (&_starting_tech_field_ids)[6] =
        *reinterpret_cast<int16_t (*)[6]>(std::uintptr_t{0x0055A960});

    inline s_unit_data (&_units)[4] = *reinterpret_cast<s_unit_data (*)[4]>(std::uintptr_t{0x00555728});

    inline s_tech_data_rifle (&_rifles)[6] =
        *reinterpret_cast<s_tech_data_rifle(*)[6]>(std::uintptr_t{0x0054F028});

    inline s_tech_data_personal_shield (&_personal_shields)[6] =
        *reinterpret_cast<s_tech_data_personal_shield(*)[6]>(std::uintptr_t{0x0054F040});
}