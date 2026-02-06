#pragma once
#include <cstdint>

namespace TECHDATA {
    #pragma pack(push, 1)

    struct s_tech_app_data {
        char* name;
        uint16_t  id;
        short tech_field_id;
        byte unk_class;
        byte ai_group;
        byte unknown1;
        byte in_strategic;
        byte type;
    };

    typedef struct s_tech_armor_data s_tech_armor_data, * Ps_tech_armor_data;


    /* WARNING! conflicting data type names: /orion2.xml.h/undefined - /undefined */

    struct s_tech_armor_data {
        undefined field0_0x0;
        undefined field1_0x1;
        undefined field2_0x2;
        undefined field3_0x3;
        undefined field4_0x4;
        undefined field5_0x5;
        short tech_app_id; /* Type 'TECH_APP' was deleted */
        undefined field7_0x8;
        undefined field8_0x9;
        undefined field9_0xa;
        undefined field10_0xb;
        short cost;
        undefined field13_0xe;
    };

    typedef struct s_tech_building_data s_tech_building_data, * Ps_tech_building_data;

    struct s_tech_building_data {
        char* name;
        undefined field1_0x4;
        undefined field2_0x5;
        undefined field3_0x6;
        undefined field4_0x7;
        int cost;
        short maintenance;
        undefined field7_0xe;
        undefined field8_0xf;
        undefined field9_0x10;
        undefined field10_0x11;
        undefined field11_0x12;
    };

    typedef struct s_tech_computer_data s_tech_computer_data, * Ps_tech_computer_data;

    struct s_tech_computer_data {
        char* name;
        undefined field4_0x4;
        undefined field5_0x5;
        short tech_app;
        short cost_by_ship_type[6];
        undefined field8_0x9;
        undefined field9_0xa;
    };

    typedef struct s_tech_drive_data s_tech_drive_data, * Ps_tech_drive_data;

    typedef char int8_t;

    struct s_tech_drive_data {
        char* name;
        undefined field1_0x4;
        undefined field2_0x5;
        short tech_app;
        short space_per_ship_type[6];
        short cost_per_ship_type[6];
        int8_t combat_speed_modifier[6];
        undefined field7_0x26;
        undefined field8_0x27;
        undefined field9_0x28;
        undefined field10_0x29;
        undefined field11_0x2a;
        undefined field12_0x2b;
        short speed;
    };

    typedef struct s_tech_field_data s_tech_field_data, * Ps_tech_field_data;

    struct s_tech_field_data {
        char* name;
        short id;
        short prev_field_id;
        short next_field_id;
        uint16_t  tech1;
        uint16_t  tech2;
        uint16_t  tech3;
        uint16_t  tech4;
        int32_t cost;
        byte ai_group;
    };

    typedef struct s_tech_fuel_data s_tech_fuel_data, * Ps_tech_fuel_data;

    struct s_tech_fuel_data {
        char* name;
        short field4;
        short tech_app;
        short ship_range;
    };

    typedef struct s_tech_hull_data s_tech_hull_data, * Ps_tech_hull_data;

    struct s_tech_hull_data {
        short hull_space;
        undefined field1_0x2;
        undefined field2_0x3;
        undefined field3_0x4;
        undefined field4_0x5;
        undefined field5_0x6;
        undefined field6_0x7;
        undefined field8_0x8;
        undefined field9_0x9;
        undefined field10_0xa;
        undefined field11_0xb;
        undefined field12_0xc;
        undefined field13_0xd;
        undefined field14_0xe;
        undefined field15_0xf;
        short beam_weapon_count;
        short missle_weapon_count;
        short special_weapon_count;
        short bomb_weapon_count;
        undefined field20_0x18;
        undefined field21_0x19;
        undefined field22_0x1a;
        undefined field23_0x1b;
        char* name;
        char* size_name;
    };

    typedef struct s_tech_shield_data s_tech_shield_data, * Ps_tech_shield_data;

    struct s_tech_shield_data {
        undefined field0_0x0;
        undefined field1_0x1;
        undefined field2_0x2;
        undefined field3_0x3;
        undefined field4_0x4;
        undefined field5_0x5;
        short tech_app_id;
        short space_per_ship_type[6];
        short cost_per_ship_type[6];
        undefined field20_0x15;
        undefined field21_0x16;
        undefined field22_0x17;
        undefined field23_0x18;
        undefined field24_0x19;
        undefined field25_0x1a;
        undefined field26_0x1b;
        undefined field27_0x1c;
        undefined field28_0x1d;
        undefined field29_0x1e;
        undefined field30_0x1f;
        undefined field31_0x20;
        undefined field32_0x21;
        undefined field33_0x22;
        undefined field34_0x23;
        undefined field35_0x24;
        undefined field36_0x25;
        undefined field37_0x26;
        undefined field38_0x27;
        undefined field39_0x28;
        undefined field40_0x29;
        undefined field41_0x2a;
        undefined field42_0x2b;
        undefined field43_0x2c;
        undefined field44_0x2d;
        undefined field45_0x2e;
        undefined field46_0x2f;
    };

    typedef struct s_tech_specials_data s_tech_specials_data, * Ps_tech_specials_data;

    struct s_tech_specials_data {
        undefined field0_0x0;
        undefined field1_0x1;
        undefined field2_0x2;
        undefined field3_0x3;
        undefined field4_0x4;
        undefined field5_0x5;
        short tech_application_id;
        short space_per_ship_type[6];
        short cost_per_ship_type[6];
        undefined field20_0x15;
        undefined field21_0x16;
        undefined field22_0x17;
        undefined field23_0x18;
        undefined field24_0x19;
        undefined field25_0x1a;
        undefined field26_0x1b;
        undefined field27_0x1c;
        undefined field28_0x1d;
        undefined field29_0x1e;
        undefined field30_0x1f;
        undefined field31_0x20;
        undefined field32_0x21;
        undefined field33_0x22;
        undefined field34_0x23;
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
}