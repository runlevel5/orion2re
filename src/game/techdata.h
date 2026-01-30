#pragma once
#include <cstdint>

namespace TECHDATA {
    #pragma pack(push, 1)
    struct s_building_data {
        uint8_t filler[8];
        int32_t cost; // 0x8
        uint8_t filler2[37];
    };

    struct s_tech_app_data {
        char* name;
        uint16_t id;
        short tech_field_id;
        byte unk_class;
        byte ai_group;
        byte unknown1;
        byte in_strategic;
        byte type;
    };

    struct s_tech_field_data {
        char* name;
        short id;
        short prev_field_id;
        short next_field_id;
        uint16_t tech1;
        uint16_t tech2;
        uint16_t tech3;
        uint16_t tech4;
        uint32_t cost;
        byte ai_group;
    };

    struct s_tech_fuel_data {
        char* name;
        short field4;
        short tech_app;
        short ship_range;
    };

    struct s_tech_drive_data {
        char* name;
        uint8_t field1_0x4;
        uint8_t field2_0x5;
        int16_t tech_app;
        uint8_t padding[36];
        int16_t speed;
    };
    #pragma pack(pop)

    inline s_building_data* _buildings = reinterpret_cast<s_building_data*>(std::uintptr_t{ 0x00554290 });
    inline s_tech_fuel_data* _fuel = reinterpret_cast<s_tech_fuel_data*>(std::uintptr_t{ 0x00555758 });
    inline s_tech_app_data* _technology_applications = reinterpret_cast<s_tech_app_data*>(std::uintptr_t{ 0x005537c8 });
    inline s_tech_field_data* _technology_fields = reinterpret_cast<s_tech_field_data*>(std::uintptr_t{ 0x00553050 });
    inline s_tech_drive_data* _drives = reinterpret_cast<s_tech_drive_data*>(std::uintptr_t{ 0x005555e0 });
}