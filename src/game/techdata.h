#pragma once
#include <cstdint>

namespace TECHDATA {
#pragma pack(push, 1)
    struct s_tech_armor_data {
        char* name;
        uint8_t field1_0x4;
        uint8_t field2_0x5;
        int16_t tech_app_id;
        uint8_t field4_0x8;
        uint8_t field5_0x9;
        int16_t armor_absorption_percent;
        int16_t cost;
        uint8_t field8_0xe;
    };

    struct s_tech_hull_data {
        int16_t hull_space;
        int16_t hull_size;
        uint8_t field2_0x4;
        uint8_t field3_0x5;
        int16_t armor_absorption;
        int16_t structure;
        uint8_t field6_0xa;
        uint8_t field7_0xb;
        uint8_t field8_0xc;
        uint8_t field9_0xd;
        uint8_t field10_0xe;
        uint8_t field11_0xf;
        int16_t beam_weapon_count;
        int16_t missle_weapon_count;
        int16_t special_weapon_count;
        int16_t bomb_weapon_count;
        uint8_t field16_0x18;
        uint8_t field17_0x19;
        uint8_t field18_0x1a;
        uint8_t field19_0x1b;
        char* name;
        char* size_name;
    };
#pragma pack(pop)

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

    struct s_weapon_mod_data {
        uint32_t field0_0x0;
        char* name;
        uint32_t field6_0x9;
        uint16_t field10_0xd;
    };

    inline s_weapon_mod_data* _weapon_modifications = reinterpret_cast<s_weapon_mod_data*>(std::uintptr_t{ 0x00555474 });

    inline int16_t __cdecl Weapon_Rack_(int16_t weapon_idx) {
        using Fn = int16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00432940});
        return fn(weapon_idx);
    }
}