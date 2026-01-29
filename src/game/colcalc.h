#pragma once
#include <cstdint>

namespace COLCALC {
    void __cdecl Twiddle_Initial_Homeworlds_();

    inline void __cdecl Twiddle_Initial_Homeworld_(s_colony* colony) {
        using Fn = void(__cdecl*)(s_colony*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004fc4b0 });
        fn(colony);
    }

    void __cdecl Compute_All_Star_Colony_Stuff_();
    void __cdecl Compute_Star_Colony_Stuff_(s_star_data* star);

    void __cdecl Init_Surrenders_();

    inline void __cdecl No_Op1_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004FBEF0})();
    }
}