#pragma once
#include <cstdint>

struct s_colony;
struct s_planet_data;
struct s_star_data;
struct s_ship_data;

namespace REPORT {
    inline uint16_t& _n_reports = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A4EAA});
    inline uint8_t (&_reports)[950] = *reinterpret_cast<uint8_t(*)[950]>(std::uintptr_t{0x005A4EC0});
    inline uint8_t& _done_player_reports = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A4EB8});
    inline uint32_t (&_something_interesting_happened)[2] = *reinterpret_cast<uint32_t(*)[2]>(std::uintptr_t{0x005A4EB0});
    inline uint8_t (&_done_colonize_check)[9] = *reinterpret_cast<uint8_t(*)[9]>(std::uintptr_t{0x005A4EA0});
    inline uint8_t& _displayed_msgs = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A5276});
    inline uint8_t& _displayed_gnn = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A4EAC});

    inline void __cdecl Initialize_Reports_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x004aa050})();
    }

    inline void __cdecl Load_Report_Strings_(char* data) {
        using Fn = void(__cdecl*)(char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004A9D80});
        fn(data);
    }

    s_colony* __cdecl Player_Colony_Base_At_Star_(short player_idx, short star_idx);
    s_ship_data* __cdecl Player_Ship_Type_At_Star_(short owner, short ship_type, short location, char include_just_built);
    bool __cdecl Has_Msg_(int16_t player_idx, int8_t show_summary);
    inline int8_t __cdecl Display_Report_Aux_(short player_num, int8_t type) {
        using Fn = int8_t(__cdecl*)(short, int8_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004AA270});
        return fn(player_num, type);
    }

    inline void __cdecl Redirect_Newly_Built_Ships_(short player_num) {
        using Fn = void(__cdecl*)(short);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x004AB2F0});
        fn(player_num);
    }

    int8_t __cdecl Must_Display_Reports_(short player_num);
}