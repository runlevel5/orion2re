#pragma once

#include <cstdint>

namespace POP {
    using value_type = uint32_t;

    constexpr value_type MASK_RACE = 0x0000000Fu;
    constexpr value_type MASK_ORIGINAL_OWNER = 0x00000070u;
    constexpr value_type MASK_PROF = 0x00000180u;
    constexpr value_type MASK_ASSIGNED = 0x00000200u;
    constexpr value_type MASK_CONQUERED = 0x00000400u;

    constexpr uint32_t RACE_ANDROID = 8u;
    constexpr uint32_t RACE_NATIVE = 9u;

    enum e_prof : uint32_t {
        PROF_FARMER = 0,
        PROF_WORKER = 1,
        PROF_SCIENTIST = 2,
    };

    inline uint32_t __cdecl Get_Race(value_type pop) {
        return pop & MASK_RACE;
    }

    inline void __cdecl Set_Race(value_type* pop, uint32_t race) {
        *pop = (*pop & ~MASK_RACE) | (race & MASK_RACE);
    }

    inline uint32_t __cdecl Get_Original_Owner(value_type pop) {
        return (pop & MASK_ORIGINAL_OWNER) >> 4;
    }

    inline void __cdecl Set_Original_Owner(value_type* pop, uint32_t owner) {
        *pop = (*pop & ~MASK_ORIGINAL_OWNER) | ((owner & 0x07u) << 4);
    }

    inline e_prof __cdecl Get_Prof(value_type pop) {
        return static_cast<e_prof>((pop & MASK_PROF) >> 7);
    }

    inline void __cdecl Set_Prof(value_type* pop, e_prof prof) {
        *pop = (*pop & ~MASK_PROF) | ((static_cast<value_type>(prof) & 0x03u) << 7);
    }

    inline bool __cdecl Is_Farmer(value_type pop) {
        return Get_Prof(pop) == PROF_FARMER;
    }

    inline bool __cdecl Is_Worker(value_type pop) {
        return Get_Prof(pop) == PROF_WORKER;
    }

    inline bool __cdecl Is_Scientist(value_type pop) {
        return Get_Prof(pop) == PROF_SCIENTIST;
    }

    inline bool __cdecl Is_Assigned(value_type pop) {
        return (pop & MASK_ASSIGNED) != 0;
    }

    inline void __cdecl Set_Assigned(value_type* pop, bool assigned) {
        if (assigned) {
            *pop |= MASK_ASSIGNED;
        } else {
            *pop &= ~MASK_ASSIGNED;
        }
    }

    inline bool __cdecl Is_Conquered(value_type pop) {
        return (pop & MASK_CONQUERED) != 0;
    }

    inline void __cdecl Set_Conquered(value_type* pop, bool conquered) {
        if (conquered) {
            *pop |= MASK_CONQUERED;
        } else {
            *pop &= ~MASK_CONQUERED;
        }
    }

    inline bool __cdecl Is_Android(value_type pop) {
        return Get_Race(pop) == RACE_ANDROID;
    }

    inline bool __cdecl Is_Native(value_type pop) {
        return Get_Race(pop) == RACE_NATIVE;
    }

    inline bool __cdecl Is_Android_Or_Native(value_type pop) {
        const uint32_t race = Get_Race(pop);
        return race == RACE_ANDROID || race == RACE_NATIVE;
    }

    inline uint32_t __cdecl Get_Effective_Player(value_type pop, uint32_t colony_owner) {
        return Is_Android_Or_Native(pop) ? colony_owner : Get_Race(pop);
    }
}

namespace SETTLER_DATA {
    using value_type = uint32_t;

    constexpr value_type MASK_SOURCE_COLONY = 0x000000FFu;
    constexpr value_type MASK_DEST_PLANET = 0x0000FF00u;
    constexpr value_type MASK_RACE = 0x000F0000u;
    constexpr value_type MASK_ETA = 0x00F00000u;
    constexpr value_type MASK_PROF = 0x03000000u;

    inline uint32_t __cdecl Get_Source_Colony(value_type settler_data) {
        return settler_data & MASK_SOURCE_COLONY;
    }

    inline void __cdecl Set_Source_Colony(value_type* settler_data, uint32_t source_colony) {
        *settler_data = (*settler_data & ~MASK_SOURCE_COLONY) | (source_colony & 0xFFu);
    }

    inline uint32_t __cdecl Get_Dest_Planet(value_type settler_data) {
        return (settler_data & MASK_DEST_PLANET) >> 8;
    }

    inline void __cdecl Set_Dest_Planet(value_type* settler_data, uint32_t dest_planet) {
        *settler_data = (*settler_data & ~MASK_DEST_PLANET) | ((dest_planet & 0xFFu) << 8);
    }

    inline uint32_t __cdecl Get_Race(value_type settler_data) {
        return (settler_data & MASK_RACE) >> 16;
    }

    inline void __cdecl Set_Race(value_type* settler_data, uint32_t race) {
        *settler_data = (*settler_data & ~MASK_RACE) | ((race & 0x0Fu) << 16);
    }

    inline uint32_t __cdecl Get_Eta(value_type settler_data) {
        return (settler_data & MASK_ETA) >> 20;
    }

    inline void __cdecl Set_Eta(value_type* settler_data, uint32_t eta) {
        *settler_data = (*settler_data & ~MASK_ETA) | ((eta & 0x0Fu) << 20);
    }

    inline POP::e_prof __cdecl Get_Prof(value_type settler_data) {
        return static_cast<POP::e_prof>((settler_data & MASK_PROF) >> 24);
    }

    inline void __cdecl Set_Prof(value_type* settler_data, POP::e_prof prof) {
        *settler_data = (*settler_data & ~MASK_PROF) |
                        ((static_cast<value_type>(prof) & 0x03u) << 24);
    }
}