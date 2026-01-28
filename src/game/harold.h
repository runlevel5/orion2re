#pragma once

namespace HAROLD {
    uint32_t __cdecl Is_Colony_Star_(int16_t star_idx);

    int16_t __cdecl Get_Scaled_Value_(int16_t value);

    int16_t __cdecl Get_Upscaled_Value_(int16_t value);

    bool __cdecl Empty_System_(int16_t star_idx);

    bool __cdecl Is_Home_Star_(int16_t star_idx);

    uint8_t __cdecl N_Planets_In_System_(int16_t star_idx);

    uint8_t __cdecl N_Sattelites_(int16_t star_index);

    void __cdecl Set_Wormhole_Id_(int16_t star_idx, uint8_t* star_bitmap);

    uint8_t __cdecl N_Food_Planets_In_System_(int16_t system_index);

    bool __cdecl Star_Is_Orion_(int16_t star_idx);

    bool __cdecl Habitable_Spectral_Class_(int16_t star_idx);

    void __cdecl Get_Font_File_(char* buffer);
    void __cdecl Get_Officer_Data_LBX_Name_(char* out_name);

    void __cdecl Set_Sound_Volume_To_Settings_();

    void __cdecl Confirm_Planet_And_Star_Index_Link_(const char* msg);

    void __cdecl Planet_Number_(int16_t planet_index);

    void __cdecl Set_All_Visited_Flags_();

    void __cdecl Init_Moving_Boxes_();

    const char* __cdecl H_Message_(int16_t id);

    void __cdecl Save_Mouse_List_();

    inline uint8_t (&_saved_mouse_list)[120] =
        *reinterpret_cast<uint8_t(*)[120]>(std::uintptr_t{0x00571A40});

    inline uint32_t& _saved_mouse_list_offset_4 =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00571A44});

    inline uint16_t& _saved_mouse_list_count =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00571AB8});

    void __cdecl Init_Fleet_Movement_Box_();

    void __cdecl Init_System_Display_Box_();

    inline void __cdecl Harold_Init_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004F0DD0 })();
    }

    inline void __cdecl Harold_Defaults_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004f0aa0});
        fn();
    }

    inline void __cdecl Set_Player_Font_Style_(int16_t style, char param_2) {
        using Fn = void(__cdecl*)(int16_t, char);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004F1930 });
        fn(style, param_2);
    }

    inline void __cdecl Map_Scale_To_Zoom_Level_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004F1090 });
        fn();
    }

    void __cdecl Restore_Mouse_List_();

    void __cdecl Load_Leader_Titles();

    int16_t __cdecl Star_Owner_(int16_t star_idx, int16_t player_idx);

    inline int16_t __cdecl Planet_Type_(int16_t star_idx, int16_t planet_idx) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004F1520});
        return fn(star_idx, planet_idx);
    }

    inline void __cdecl Colony_Owners_(int16_t star_idx, int16_t* player_counts, int16_t* total_count, bool hide_if_no_lore) {
        using Fn = void(__cdecl*)(int16_t, int16_t*, int16_t*, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004f1190 });
        fn(star_idx, player_counts, total_count, hide_if_no_lore);
    }

    inline const char* s_after_home_world_gen = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575D4 }); // "after home world gen"

    inline const char* s___s__00556ae4 = reinterpret_cast<const char*>(0x00556AE4); // "(%s)"
    inline const char* s_relate_game_conditions_EXACTLY_a_0055a6cc = reinterpret_cast<const char*>(0x0055A6CC); // "relate game conditions EXACTLY as they occurred to technical support"
    inline const char* s_orbit__d_of_star__d_has_more_tha_0055a704 = reinterpret_cast<const char*>(0x0055A704); // "orbit %d of star %d has more than one planet linked to it"

    inline const char* s_HERODATI_LBX = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055A808 }); // "HERODATI.LBX"
    inline const char* s_HERODATS_LBX = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055A818 }); // "HERODATS.LBX"
    inline const char* s_HERODATF_LBX = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055A828 }); // "HERODATF.LBX"
    inline const char* s_HERODATG_LBX = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055A838 }); // "HERODATG.LBX"
    inline const char* s_HERODATA_LBX = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055A848 }); // "HERODATA.LBX"
}