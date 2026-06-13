#pragma once
#include <cstdint>

struct s_unit_data;

namespace TECHDATA {
    extern s_techdata_planetary_shield _planetary_shields[4];
    extern s_tech_armor_data _armor[ARMOR_COUNT];
    extern s_tech_building_data _buildings[BUILDING_COUNT];
    extern s_tech_hull_data _hull_data[HULL_CLASS_COUNT];
    extern s_tech_fuel_data _fuel[FUEL_COUNT];
    extern s_tech_app_data _technology_applications[TECH_APP_COUNT];
    extern s_tech_field_data _technology_fields[TECH_FIELD_COUNT];
    extern s_tech_drive_data _drives[DRIVE_COUNT];
    extern s_tech_shield_data _shields[SHIELD_COUNT];
    extern s_tech_computer_data _computers[COMPUTER_COUNT];
    extern s_tech_specials_data _specials[SPECIAL_COUNT];
    extern s_tech_weapon_data _weapons[WEAPON_COUNT];
    extern s_tech_weapon_mod_data _weapon_modifications[WEAPON_MOD_COUNT];

    extern int16_t _starting_tech_field_ids[6];

    extern s_unit_data _units[UNIT_COUNT];

    extern s_tech_data_rifle _rifles[6];

    extern s_tech_data_personal_shield _personal_shields[2];

    /**
     * Returns a reference to the ship-range value for a fuel technology entry.
     * @param fuel_index Index into the TECHDATA fuel table.
     */
    inline uint16_t& _fuel_ship_range(int32_t fuel_index) {
      return _fuel[fuel_index].ship_range;
    }
}