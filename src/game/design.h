#pragma once
#include <cstdint>

namespace DESIGN {
    inline s_current_design*& _design = *reinterpret_cast<s_current_design**>(std::uintptr_t{ 0x0059B330 });
    inline uint8_t& _replacement_wait_ctr = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A7B6E});
    inline int16_t& _design_size_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00565040});
    inline uint8_t& _use_augmented_engines = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056503C});

    inline uint32_t& _printed_space_avail = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00565054});
    inline uint16_t& _printed_cost = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00565058});
    inline uint8_t& _changed_design_name = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0056504C});
    inline int16_t& _player_design_num = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A1B28});
    inline uint8_t* _add_size_button_flag = reinterpret_cast<uint8_t*>(std::uintptr_t{0x00565064});

    inline const char* s_cstdes_log = reinterpret_cast<const char*>(0x005560ec);

    int16_t __cdecl Cost_Of_Design_(int16_t player_idx, struct s_ship_design* ship_design);

    uint8_t __cdecl Current_Design_Size_(int16_t player_idx, int16_t design_idx);

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

    int32_t __cdecl Weapon_Space_(int16_t player_idx, int16_t weapon_idx, int16_t quantity, uint16_t mod_flags, uint16_t arc_type, uint16_t ship_size);

    int32_t __cdecl Total_Space_Used_();

    int32_t __cdecl Get_Design_Hull_Size_();

    int8_t __cdecl Count_Special_Devices_();

    void __cdecl Update_N_Spec_Systems_In_Design_();

    void __cdecl Update_Weapon_Count_();

    void __cdecl Update_Calculated_Design_Data_(int16_t player_idx);

    void __cdecl Design_Screen_();

    int32_t __cdecl Get_Design_Hull_Size_();

    inline void __cdecl Draw_Design_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042f470 })();
    }

    inline void __cdecl Load_Design_Screen_Pictures_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042f9a0 })();
    }

    void __cdecl Build_Initial_Design_Template_(int16_t player_idx, int16_t design_idx);

    inline void __cdecl Add_Design_Buttons__() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042d080 })();
    }

    inline void __cdecl Clear_Design_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042df20 })();
    }

    inline void __cdecl Update_Player_Design_(int16_t player, int16_t design_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x00432360 })(player, design_idx);
    }

    inline void __cdecl Check_Generic_Replacement_Fields_(int16_t field, int16_t* refresh, int16_t* flag) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042d930 })(field, refresh, flag);
    }

    inline void __cdecl Check_Weapon_Fields_(int16_t field, int16_t* refresh, int16_t* flag) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042dda0 })(field, refresh, flag);
    }

    inline void __cdecl Check_Special_System_Fields_(int16_t field, int16_t* refresh, int16_t* flag) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042dc20 })(field, refresh, flag);
    }

    inline void __cdecl Check_Plus_Minus_Buttons_(int16_t field) {
        using Fn = void(__cdecl*)(int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042dad0 })(field);
    }

    inline void __cdecl Get_Ship_Design_Name_(uint16_t race, int16_t size, int picture_idx, char* out_name) {
        using Fn = void(__cdecl*)(uint16_t, int16_t, int, char*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x0042f830 })(race, size, picture_idx, out_name);
    }
}