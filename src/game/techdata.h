#pragma once
#include <cstdint>

struct s_unit_data;

namespace TECHDATA {
    // Correct addresses for various TECHDATA structures. If address does not match, then it points inside of structure.
    inline s_techdata_planetary_shield* _planetary_shields =
        reinterpret_cast<s_techdata_planetary_shield*>(std::uintptr_t{ 0x0054F048 });
    inline s_tech_armor_data* _armor = reinterpret_cast<s_tech_armor_data*>(std::uintptr_t{ 0x00554d90 });
    inline s_tech_building_data* _buildings = reinterpret_cast<s_tech_building_data*>(std::uintptr_t{ 0x00554290 });
    inline s_tech_hull_data* _hull_data = reinterpret_cast<s_tech_hull_data*>(std::uintptr_t{ 0x00555798 });
    inline s_tech_fuel_data* _fuel = reinterpret_cast<s_tech_fuel_data*>(std::uintptr_t{ 0x00555758 });
    inline s_tech_app_data* _technology_applications = reinterpret_cast<s_tech_app_data*>(std::uintptr_t{ 0x005537c8 });
    inline s_tech_field_data (&_technology_fields)[83] =
        *reinterpret_cast<s_tech_field_data (*)[83]>(std::uintptr_t{ 0x00553050 });
    inline s_tech_drive_data* _drives = reinterpret_cast<s_tech_drive_data*>(std::uintptr_t{ 0x005555e0 });
    inline s_tech_shield_data* _shields = reinterpret_cast<s_tech_shield_data*>(std::uintptr_t{ 0x00554e00 });
    inline s_tech_computer_data* _computers = reinterpret_cast<s_tech_computer_data*>(std::uintptr_t{ 0x00555558 });
    inline s_tech_specials_data* _specials = reinterpret_cast<s_tech_specials_data*>(std::uintptr_t{ 0x00554638 });
    inline s_tech_weapon_data (&_weapons)[46] =
        *reinterpret_cast<s_tech_weapon_data(*)[46]>(std::uintptr_t{0x00554F68});
    inline s_tech_weapon_mod_data (&_weapon_modifications)[15] =
        *reinterpret_cast<s_tech_weapon_mod_data(*)[15]>(std::uintptr_t{0x00555470});

    inline s_unit_data* _units = reinterpret_cast<s_unit_data*>(std::uintptr_t{0x00555728});

    inline s_tech_data_rifle (&_rifles)[6] =
        *reinterpret_cast<s_tech_data_rifle(*)[6]>(std::uintptr_t{0x0054F028});

    inline s_tech_data_personal_shield (&_personal_shields)[6] =
        *reinterpret_cast<s_tech_data_personal_shield(*)[6]>(std::uintptr_t{0x0054F040});

    inline int16_t __cdecl Weapon_Rack_(int16_t weapon_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00432940});
        return fn(weapon_idx);
    }
}