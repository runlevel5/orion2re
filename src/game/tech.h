#pragma once
#include <cstdint>

namespace TECH {
    inline void __cdecl _Tech_Select_(uint8_t param_1) {
        using Fn = void(__cdecl*)(uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004840C0});
        fn(param_1);
    }

    inline int8_t (&_saved_hyper_advanced_tech)[8] = *reinterpret_cast<int8_t(*)[8]>(std::uintptr_t{0x005A6338});

    void __cdecl Tech_Change_();

    /**
     * @brief Saves the hyper-advanced tech progress for a specific player into a temporary global buffer.
     * @param player_idx The index of the player whose tech levels are to be saved.
     */
    void __cdecl Save_Hyper_Tech_Values_(int16_t player_idx);

    /**
     * @address 00486120
     */
    void __cdecl Promote_Hyper_Field_Values_(int16_t player_idx);

    inline char* __cdecl Technology_Applications_Name_(int16_t app_id, int16_t param_3) {
        using Fn = char*(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00485F20});
        return fn(app_id, param_3);
    }
}