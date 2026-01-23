#pragma once

namespace MOX {
    enum SCREEN : uint16_t {
        SCREEN_MAIN = 0,
        SCREEN_COLONY = 1,
        SCREEN_DESIGN = 3,
        SCREEN_FLEET = 4,
        SCREEN_RACE = 6,
        SCREEN_EXIT = 7,
        SCREEN_GAME = 8,
        SCREEN_INFO = 9,
        SCREEN_MAIN_MENU = 10,
        SCREEN_MAIN_NEXT_TURN = 12,
        SCREEN_NEW_GAME = 13,
        SCREEN_HALL_OF_FAME = 14,
        SCREEN_MULTI_PLAYER = 15,
        SCREEN_HOTSEAT = 16,
        SCREEN_HOTSEAT_SELECT_PLAYER = 17,
        SCREEN_PLANET_DATA = 18,
        SCREEN_COLONY_SUMMARY = 20,
        SCREEN_START_NET = 21,
        SCREEN_JOIN_NET = 22,
        SCREEN_QUEUE_POPUP = 25,
        SCREEN_DISTANCE_BETWEEN_STARS = 28,
        SCREEN_OFFICERS = 29,
        SCREEN_COLONIZATION_IN_MAIN = 30,
        SCREEN_BUILDING_OUTPOSTS_IN_MAIN = 31,
        SCREEN_PLANET_SUMMARY = 32,
        SCREEN_COLONY_LANDING = 33,
        SCREEN_UNLOADING_TRANSPORTS_IN_MAIN = 34,
        SCREEN_SCRAP_FREIGHTERS = 35,
        SCREEN_TECH_CHANGE = 36,
        SCREEN_NET_NEXT_TURN = 37,
        SCREEN_REPORTS = 39,
        SCREEN_TURN_SUMMARY = 40,
        SCREEN_LOAD_NET = 41,
        SCREEN_SHOW_COMMAND_POINTS = 43
    };

    // References
    inline uint8_t& _previous_screen = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00597AD0 });
    inline int16_t& _max_debug_saves = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059C458 });
    inline int16_t& _first_debug_save = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005A1830 });
    inline int16_t& _debug_saves_on = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059D6C2 });
    inline uint8_t& _turns_between_debug_saves = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005A49F0 });
    inline void*& _sound_buffer = *reinterpret_cast<void**>(std::uintptr_t{ 0x005A1B34 });
    inline SCREEN& _current_screen = *reinterpret_cast<SCREEN*>(std::uintptr_t{ 0x00597302 });
    inline uint8_t& _game_type = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005974DD });
    inline uint8_t& _refresh_mode = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0059B5A4 });
    inline uint8_t& _mox_font_colors_offset = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0059BA6E });
    inline char& _temp_string = *reinterpret_cast<char*>(std::uintptr_t{ 0x00596230 });
    inline int16_t& _PLAYER_NUM = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005A18F2 });
    inline s_player*& _player = *reinterpret_cast<s_player**>(std::uintptr_t{ 0x0059B3D0 });
    inline s_settings& _settings = *reinterpret_cast<s_settings*>(std::uintptr_t{ 0x00597080 });
    inline uint8_t& _settings_language = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00597152 });
    inline uint8_t& _mouse_list_arrow = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00558D10 });
    
    inline uint32_t& _special_names_seg =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x00595390 });

    inline uint32_t& _global_data_seg =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x00597410 });

    inline uint32_t& _nebula =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x00597770 });

    inline s_planet_data*& _planet =
        *reinterpret_cast<s_planet_data**>(std::uintptr_t{ 0x005978F8 });

    inline uint32_t& _techname_seg =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0059B31C });

    inline uint32_t& _global_screen_seg =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0059B5A8 });

    inline uint32_t& _global_cache_seg =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0059B5B8 });

    inline uint32_t& _global_cache_size =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0059BA10 });

    inline s_ship_data*& _ship =
        *reinterpret_cast<s_ship_data**>(std::uintptr_t{ 0x0059BD3C });

    inline s_leader_data*& _leaders =
        *reinterpret_cast<s_leader_data**>(std::uintptr_t{ 0x0059BF40 });

    inline s_colony*& _colony =
        *reinterpret_cast<s_colony**>(std::uintptr_t{ 0x005A1828 });

    inline s_star_data*& _star =
        *reinterpret_cast<s_star_data**>(std::uintptr_t{ 0x005A18F4 });

    // Suggested name: g_extra_data_seg
    inline uint32_t& DAT_005a2bec =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x005A2BEC });

    // Funcs
    inline void Mox_Update_() {
        using Fn = void(*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004D7820 });
        fn();
    }

    inline bool Read_Moo_Custom_Player_Data_(s_player* player) {
        using Fn = bool(*)(s_player*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0043D1C0 });
        return fn(player);
    }
}