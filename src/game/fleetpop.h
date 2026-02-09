#pragma once
#include <cstdint>

namespace FLEETPOP {
    inline const char* s_BUFFER0_LBX = reinterpret_cast<const char*>(std::uintptr_t{0x00551124}); // "BUFFER0.LBX"

    inline void __cdecl Reload_Fleet_Box_Data_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00405990})();
    }

    inline int16_t __cdecl Fleet_Buttons_Chosen_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(0x00405070)();
    }

    inline void __cdecl Set_Fleet_Movement_Icon_Selections_(int a, uint8_t b) {
        using Fn = void(__cdecl*)(int, uint8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00406330});
        fn(a, b);
    }

    inline bool __cdecl Some_Icon_Selected_() {
        using Fn = bool(__cdecl*)();
        return reinterpret_cast<Fn>(0x00406700)();
    }

    inline void __cdecl Reload_Ship_Selection_Status_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(0x004059E0)();
    }

    inline int16_t __cdecl First_Unselected_Icon_() {
        using Fn = int16_t(__cdecl*)();
        return reinterpret_cast<Fn>(std::uintptr_t{0x00404CC0})();
    }

    inline void __cdecl Save_Ship_Selection_Status_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00405A60})();
    }

    int16_t __cdecl Fleet_Box_Grid_Height_(int16_t grid_type);
    int16_t __cdecl Fleet_Box_Width_();

    inline uint16_t __cdecl Fleet_Box_Height_(int16_t count) {
        using Fn = uint16_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00404F40});
        return fn(count);
    }

    void __cdecl Set_Fleet_Movement_Box_Dimensions_(int16_t count);

    inline int8_t __cdecl Can_Select_Fleet_Box_Ship_(int32_t param_1) {
        using Fn = int8_t(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00406750});
        return fn(param_1);
    }

    inline uint32_t __cdecl Icon_Selection_Status_(int16_t field) {
        using Fn = uint32_t(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00406610});
        return fn(field);
    }

    inline void __cdecl Clear_Ship_Node_Selection_Flags_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x00403ec0})();
    }

    inline void __cdecl Reload_Fleet_Box_Palette_(int16_t player_id, int16_t force_reload) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x004059c0})(player_id, force_reload);
    }
}