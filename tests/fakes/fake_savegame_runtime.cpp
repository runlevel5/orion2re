#include "pch.h"

namespace MOX {
    int16_t _NUM_STARS = 0;
    int16_t _cur_map_scale = 0;
    int16_t _cur_map_x = 0;
    int16_t _cur_map_y = 0;
    int8_t _game_type = 0;
    s_antaran _antarans = {};
    int16_t _PLAYER_NUM = 0;
    int32_t _stardate = 0;
    s_settings _settings = {};
    uint8_t _NUM_NEBULAS = 0;
    int16_t _NUM_COLONIES = 0;
    int16_t _planet_count = 0;
    s_planet_data* _planet = nullptr;
    int16_t _NUM_SHIPS = 0;
    int16_t _max_zoom_count = 0;
    uint8_t _star_bg_index = 0;
    uint16_t _fleet_icon_button_count = 0;
    int16_t _NUM_PLAYERS = 0;
    s_player* _player = nullptr;
    s_ship_data* _ship = nullptr;
    s_leader_data* _leaders = nullptr;
    s_colony* _colony = nullptr;
    s_star_data* _star = nullptr;
    int16_t _bill_savegame[6] = {};
    uint16_t _antaran_resource_level = 0;
    uint16_t _chance_for_antaran_invasion = 0;
    uint16_t _disallow_saves_code = 0;
    int16_t _max_map_scale = 0;
    int16_t _MAP_MAX_X = 0;
    int16_t _MAP_MAX_Y = 0;
    s_nebula* _nebula = nullptr;
    s_movable_box _moveable_box[5] = {};
    int16_t _moveable_box_order[5] = {};
}

namespace REPORT {
    int16_t _n_reports = 0;
    s_report _reports[50] = {};
    uint8_t _done_player_reports = 0;
    uint8_t _something_interesting_happened[MAX_PLAYERS] = {};
    uint8_t _done_colonize_check[BITMAP(MAX_STARS)] = {};
    uint8_t _displayed_msgs = 0;
    uint8_t _displayed_gnn = 0;
}

namespace EVENTS {
    s_event_data _event_data[36] = {};
    uint32_t _last_event_year = 0;
    int16_t _event_delay_count = 0;
}

namespace MSG_ {
    uint8_t _first_msg[MAX_PLAYERS] = {};
    uint8_t _n_msgs[MAX_PLAYERS] = {};
    s_MSG _msgs[100] = {};
    uint8_t _max_msgs[MAX_PLAYERS] = {};
}

namespace diplodef {
    int16_t _next_council_meeting_turn = 0;
    int16_t _COUNCIL_FLAG = 0;
}

namespace exitm {
    [[noreturn]] void __cdecl Exit_With_Message(const char*) {
        std::abort();
    }
}