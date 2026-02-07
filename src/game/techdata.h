#pragma once
#include <cstdint>

namespace TECHDATA {
    // Correct addresses for various TECHDATA structures. If address does not match, then it points inside of structure.
    inline s_tech_armor_data* _armor = reinterpret_cast<s_tech_armor_data*>(std::uintptr_t{ 0x00554d90 });
    inline s_tech_building_data* _buildings = reinterpret_cast<s_tech_building_data*>(std::uintptr_t{ 0x00554290 });
    inline s_tech_hull_data* _hull_data = reinterpret_cast<s_tech_hull_data*>(std::uintptr_t{ 0x00555798 });
    inline s_tech_fuel_data* _fuel = reinterpret_cast<s_tech_fuel_data*>(std::uintptr_t{ 0x00555758 });
    inline s_tech_app_data* _technology_applications = reinterpret_cast<s_tech_app_data*>(std::uintptr_t{ 0x005537c8 });
    inline s_tech_field_data* _technology_fields = reinterpret_cast<s_tech_field_data*>(std::uintptr_t{ 0x00553050 });
    inline s_tech_drive_data* _drives = reinterpret_cast<s_tech_drive_data*>(std::uintptr_t{ 0x005555e0 });
    inline s_tech_shield_data* _shields = reinterpret_cast<s_tech_shield_data*>(std::uintptr_t{ 0x00554e00 });
    inline s_tech_computer_data* _computers = reinterpret_cast<s_tech_computer_data*>(std::uintptr_t{ 0x00555558 });
    inline s_tech_specials_data* _specials = reinterpret_cast<s_tech_specials_data*>(std::uintptr_t{ 0x00554638 });
    inline s_tech_weapon_data* _weapons = reinterpret_cast<s_tech_weapon_data*>(std::uintptr_t{ 0x00554f68 });

    inline int16_t __cdecl Weapon_Rack_(int16_t weapon_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00432940});
        return fn(weapon_idx);
    }
}