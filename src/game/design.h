#pragma once
#include <cstdint>

namespace DESIGN {

#pragma pack(push, 1)
    struct s_current_design {
        char name[16];
        uint8_t field1_0x10;
        int16_t shield;
        int16_t shield_cost;
        int16_t shield_space;
        uint8_t field5_0x17;
        uint8_t field6_0x18;
        uint8_t field7_0x19;
        uint8_t field8_0x1a;
        int16_t ftl_type;
        int16_t drive_speed;
        int16_t drive_cost;
        int16_t base_space;
        uint8_t field14_0x23;
        uint8_t field15_0x24;
        int16_t base_combat_speed;
        uint8_t field17_0x27;
        uint8_t field18_0x28;
        int16_t field19_0x29;
        int16_t computer_type;
        int16_t computer_cost;
        int16_t fuel_type;
        int16_t armor_type;
        int16_t armor_cost;
        int16_t weapon_type[8];
        int16_t weapon_count[8];
        int8_t weapon_firing_arc[8];
        int16_t weapon_specials[8];
        int32_t weapon_cost[8];
        int32_t weapon_space[8];
        int8_t weapon_ammo[8];
        int16_t special_devices[8];
        int16_t special_device_cost[8];
        int16_t special_device_space[8];
        int16_t ship_size;
        int16_t picture_type;
        int32_t hull_space;
        int32_t space_used;
        int32_t total_cost;
        int8_t show_help;
        int16_t help1;
        int16_t help2;
        uint8_t field43_0xfa[0x125 - 0xfa];
    };
#pragma pack(pop)

    inline s_current_design*& _design = *reinterpret_cast<s_current_design**>(std::uintptr_t{ 0x0059B330 });
    inline uint8_t& _use_augmented_engines = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056503C});

    inline uint32_t& _printed_space_avail = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00565054});
    inline uint16_t& _printed_cost = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00565058});

    int16_t __cdecl Cost_Of_Design_(int16_t player_idx, struct s_ship_design* ship_design);

    int8_t __cdecl Count_Weapons_Loaded_();

    int8_t __cdecl Get_Modified_Combat_Speed_(s_current_design* design);

    int8_t __cdecl Get_Base_Combat_Speed_(s_current_design* design);

    int8_t __cdecl Get_Ship_Speed_Modifier_(s_current_design* design);

    int8_t __cdecl Current_Design_Base_Combat_Speed_(s_current_design* design);

    uint8_t __cdecl Current_Design_Has_Special_System_(s_current_design* design, uint32_t system_id);

    void __cdecl Has_Damper_Field_();

    uint8_t __cdecl Current_Design_Shield_();

    int16_t __cdecl Base_Shield_Cost();

    int16_t __cdecl Get_Shild_Space_();

    void __cdecl Design_Template_Costs_(int16_t player_id);

    void __cdecl Design_Template_Space_Requirements_(int16_t player_id);

    int32_t __cdecl Total_Design_Cost_();

    int32_t __cdecl Total_Space_Used_();

    int32_t __cdecl Get_Design_Hull_Size_();

    int8_t __cdecl Count_Special_Devices_();

    void __cdecl Update_N_Spec_Systems_In_Design_();

    void __cdecl Update_Weapon_Count_();

    void __cdecl Update_Calculated_Design_Data_(int16_t player_idx);

    inline void __cdecl Design_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0042e6b0})();
    }
}