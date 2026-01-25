#pragma once

namespace HAROLD {
    uint32_t __cdecl Is_Colony_Star_(int16_t star_idx);

    inline int __cdecl Get_Scaled_Value_(int value) {
        using Fn = int(__cdecl*)(int);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004f0a30})(value);
    }

    inline bool __cdecl Empty_System_(int16_t star_idx) {
        using Fn = bool(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004f0860})(star_idx);
    }

    inline bool __cdecl Is_Home_Star_(int16_t star_idx) {
        using Fn = bool(__cdecl*)(int16_t);
        return reinterpret_cast<Fn>(std::uintptr_t{0x004f1030})(star_idx);
    }

    uint8_t __cdecl N_Planets_In_System_(int16_t star_idx);

    bool __cdecl Star_Is_Orion_(int16_t star_idx);

    bool __cdecl Habitable_Spectral_Class_(int16_t star_idx);

    void __cdecl Get_Font_File_(char* buffer);
    void __cdecl Get_Officer_Data_LBX_Name_(char* out_name);

    inline void Set_Sound_Volume_To_Settings_() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004F1970 });
        fn();
    }

    inline void __cdecl Confirm_Planet_And_Star_Index_Link_(const char* msg) {
        using Fn = void(__cdecl*)(const char*);
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004F0640 })(msg);
    }

    inline void __cdecl Set_All_Visited_Flags_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004F18F0 })();
    }

    inline void __cdecl Harold_Init_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{ 0x004F0DD0 })();
    }

    inline const char* __cdecl H_Message_(int32_t message_id) {
        using Fn = const char*(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004F2440});
        return fn(message_id);
    }

    inline const char* s_after_home_world_gen = reinterpret_cast<const char*>(std::uintptr_t{ 0x005575D4 }); // "after home world gen"

    inline const char* s_HERODATI_LBX = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055A808 }); // "HERODATI.LBX"
    inline const char* s_HERODATS_LBX = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055A818 }); // "HERODATS.LBX"
    inline const char* s_HERODATF_LBX = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055A828 }); // "HERODATF.LBX"
    inline const char* s_HERODATG_LBX = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055A838 }); // "HERODATG.LBX"
    inline const char* s_HERODATA_LBX = reinterpret_cast<const char*>(std::uintptr_t{ 0x0055A848 }); // "HERODATA.LBX"
}