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
    inline uint8_t* _food_per_farmer_table = reinterpret_cast<uint8_t*>(std::uintptr_t{0x00559690});
    inline uint8_t* _star_class_table = reinterpret_cast<uint8_t*>(std::uintptr_t{0x005596C0});
    inline int16_t& num_stars = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059740A });
    inline int16_t& _NUM_STARS = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059740A });
    inline int16_t& _cur_map_scale = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059D604 });
    inline uint8_t& _previous_screen = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00597AD0 });
    inline int16_t& _max_debug_saves = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059C458 });
    inline int16_t& _first_debug_save = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005A1830 });
    inline int16_t& _debug_saves_on = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059D6C2 });
    inline uint8_t& _turns_between_debug_saves = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005A49F0 });
    inline void*& _sound_buffer = *reinterpret_cast<void**>(std::uintptr_t{ 0x005A1B34 });
    inline SCREEN& _current_screen = *reinterpret_cast<SCREEN*>(std::uintptr_t{ 0x00597302 });
    inline void*& _screen_seg = *reinterpret_cast<void**>(std::uintptr_t{ 0x005973E4 });
    inline uint8_t& _game_type = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x005974DD });
    inline uint8_t& _maps_to_generate = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0059791C });
    inline uint8_t& _refresh_mode = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0059B5A4 });
    inline uint8_t& _mox_font_colors_offset = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x0059BA6E });
    inline char& _temp_string = *reinterpret_cast<char*>(std::uintptr_t{ 0x00596230 });
    inline int16_t& _PLAYER_NUM = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005A18F2 });
    inline s_player*& _player = *reinterpret_cast<s_player**>(std::uintptr_t{ 0x0059B3D0 });
    inline uint32_t* _antarans = reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0059BA20 });
    inline uint8_t (&_antarans_blob)[66] = *reinterpret_cast<uint8_t(*)[66]>(std::uintptr_t{ 0x0059BA20 });
    inline uint32_t& _stardate = *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0059BA64 });
    inline s_settings& _settings = *reinterpret_cast<s_settings*>(std::uintptr_t{ 0x00597080 });
    inline uint8_t& _settings_language = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00597152 });
    inline uint8_t& _settings_galaxy_age = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00597157 });

    inline uint16_t* _h_string_ix = reinterpret_cast<uint16_t*>(std::uintptr_t{0x0059B6E0});
    inline char* _h_strings = reinterpret_cast<char*>(std::uintptr_t{0x0059D6E0});

    // Suggested name: MOX::_class_to_mineral
    inline uint8_t (&_class_to_mineral)[60] = *reinterpret_cast<uint8_t(*)[60]>(std::uintptr_t{0x00559540});

    // Suggested name: MOX::_gravity_table
    inline uint8_t (&_gravity_table)[30] = *reinterpret_cast<uint8_t(*)[30]>(std::uintptr_t{0x00559580});

    inline uint8_t& _mouse_list_arrow = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00558D10 });
    
    inline void*& _special_names_seg =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x00595390 });

    inline void*& _global_data_seg =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x00597410 });

    inline void*& _nebula =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x00597770 });

    inline uint16_t& _NUM_COLONIES =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x00597856 });

    inline uint16_t& _planet_count = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0059700E });

    inline s_planet_data*& _planet =
        *reinterpret_cast<s_planet_data**>(std::uintptr_t{ 0x005978F8 });

    inline uint16_t& _NUM_SHIPS = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x0059B4BC });

    inline void*& _techname_seg =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x0059B31C });

    inline void*& _global_screen_seg =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x0059B5A8 });

    inline uint32_t& _global_cache_seg =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0059B5B8 });

    inline uint32_t& _global_cache_size =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0059BA10 });

    inline uint16_t& _NUM_PLAYERS = *reinterpret_cast<uint16_t*>(std::uintptr_t{ 0x005A2C1C });
    inline char* (&_race_names)[20] =
        *reinterpret_cast<char* (*)[20]>(std::uintptr_t{ 0x0059D660 });
    inline uint8_t (&_special_bonus)[10][3] = *reinterpret_cast<uint8_t (*)[10][3]>(std::uintptr_t{ 0x0055903A });
    inline uint16_t (&_star_fields_dim)[6] = *reinterpret_cast<uint16_t(*)[6]>(std::uintptr_t{ 0x005A47B0 });

    inline s_ship_data*& _ship =
        *reinterpret_cast<s_ship_data**>(std::uintptr_t{ 0x0059BD3C });

    inline s_leader_data*& _leaders =
        *reinterpret_cast<s_leader_data**>(std::uintptr_t{ 0x0059BF40 });

    inline s_colony*& _colony =
        *reinterpret_cast<s_colony**>(std::uintptr_t{ 0x005A1828 });

    inline s_star_data*& _star =
        *reinterpret_cast<s_star_data**>(std::uintptr_t{ 0x005A18F4 });

    inline int16_t& _max_map_scale = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00597768 });
    inline int16_t& _MAP_MAX_X = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00597998 });
    inline int16_t& _MAP_MAX_Y = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x0059799A });
    inline uint8_t& _g_galaxy_size = *reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00597A88 });

    // Suggested name: g_extra_data_seg
    inline uint32_t& DAT_005a2bec =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x005A2BEC });

    // Suggested name: _planet_max_farms
    inline uint8_t* _planet_max_farms = reinterpret_cast<uint8_t*>(std::uintptr_t{ 0x00559660 });

    // 5 groups per spectral class
    inline uint8_t (*_class_to_group)[5] = reinterpret_cast<uint8_t(*)[5]>(std::uintptr_t{0x005595C0});

    // 4 groups per climate
    inline uint8_t (*_climate_roll_table)[4] = reinterpret_cast<uint8_t(*)[4]>(std::uintptr_t{0x005595E0});

    #pragma pack(push, 1)
    struct s_movable_box {
        uint16_t index;
        int16_t off_flag;
        uint16_t field2_0x4;
        uint16_t field3_0x6;
        int16_t field4_0x8;
        int16_t field5_0xa;
        uint8_t field6_0xc;
        uint8_t field7_0xd;
        uint16_t field8_0xe;
        uint16_t field9_0x10;
        uint8_t field10_0x12;
        uint8_t field11_0x13;
        uint8_t field12_0x14;
        uint8_t field13_0x15;
        uint8_t field14_0x16;
        uint8_t field15_0x17;
        uint16_t field16_0x18;
        uint16_t field17_0x1a;
    };
    #pragma pack(pop)

    // Array of movable box structures (size 0x1C each, total 0x8C/140 bytes)
    inline s_movable_box (&_moveable_box)[5] =
        *reinterpret_cast<s_movable_box(*)[5]>(std::uintptr_t{0x00597860});

    // Suggested name: MOX::moveable_box_display_order
    inline uint16_t (&_moveable_box_order)[5] =
        *reinterpret_cast<uint16_t(*)[5]>(std::uintptr_t{0x0059BD20});

    // Suggested name: MOX::box_visibility_flags
    inline uint16_t (&_box_off_flag)[5] =
        *reinterpret_cast<uint16_t(*)[5]>(std::uintptr_t{0x0059C420});

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