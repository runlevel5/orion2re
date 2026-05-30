#pragma once
#include <cstdint>

struct s_unit_data;

namespace TECHDATA {
    // Correct addresses for various TECHDATA structures. If address does not match, then it points inside of structure.
    extern s_techdata_planetary_shield _planetary_shields[4];
    extern s_tech_armor_data _armor[7];
    extern s_tech_building_data _buildings[49];
    extern s_tech_hull_data _hull_data[9];
    extern s_tech_fuel_data _fuel[6];
    extern s_tech_app_data _technology_applications[212];
    extern s_tech_field_data _technology_fields[83];
    extern s_tech_drive_data _drives[7];
    extern s_tech_shield_data _shields[6];
    extern s_tech_computer_data _computers[6];
    extern s_tech_specials_data _specials[40];
    extern s_tech_weapon_data _weapons[46];
    extern s_tech_weapon_mod_data _weapon_modifications[15];

    extern int16_t _starting_tech_field_ids[6];

    extern s_unit_data _units[4];

    extern s_tech_data_rifle _rifles[6];

    extern s_tech_data_personal_shield _personal_shields[2];

    inline uint16_t& _fuel_ship_range(int32_t fuel_idx) {
      return _fuel[fuel_idx].ship_range;
    }
}