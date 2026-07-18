#include "pch.h"

#include "game/allocate.h"
#include "game/chat.h"
#include "game/colcalc.h"
#include "game/combfind.h"
#include "game/eric.h"
#include "game/ericnet.h"
#include "game/fields.h"
#include "game/mainscr.h"
#include "game/mox.h"
#include "game/multplay.h"
#include "game/netcode.h"
#include "game/netmox.h"
#include "game/netstart.h"
#include "game/nextturn.h"
#include "game/report.h"
#include "game/russ.h"
#include "game/sound.h"

namespace CHAT {
    s_net_chat* _chat_info = nullptr;
}

namespace MOX {
    int16_t _return_screen = 0;
    int8_t _previous_screen = 0;
    int16_t _current_screen = 0;
    s_animation_header* _screen_seg = nullptr;
    int8_t _game_type = 0;
    char _temp_string[512] = {};
    int16_t _PLAYER_NUM = 0;
    int16_t _combat_turn = 0;
    s_player* _player = nullptr;
    int16_t _NUM_PLAYERS = 0;
    int16_t _NUM_STARS = 0;
    s_star_data* _star = nullptr;
    int32_t _stardate = 0;
    s_settings _settings = {};
    s_antaran _antarans = {};
    s_nebula* _nebula = nullptr;
    uint8_t _NUM_NEBULAS = 0;
    int16_t _NUM_COLONIES = 0;
    int16_t _planet_count = 0;
    s_planet_data* _planet = nullptr;
    int16_t _NUM_SHIPS = 0;
    s_ship_data* _ship = nullptr;
    s_leader_data* _leaders = nullptr;
    s_colony* _colony = nullptr;
    int16_t _bill_savegame[6] = {};
}

namespace NETSTART {
    void __cdecl Mox_Net_Uninit_() {}
}

namespace MULTPLAY {
    void (*g_chat_box_input_loop_hook)() = nullptr;

    void __cdecl Load_Net_Next_Turn_Screen_(int8_t) {}
    void __cdecl Draw_Net_Next_Turn_Screen_() {}
    void __cdecl Add_Net_Next_Turn_Fields_() {}
    void __cdecl Chat_Box_Input_Loop_() {
        if (g_chat_box_input_loop_hook != nullptr) {
            g_chat_box_input_loop_hook();
        }
    }
}

namespace fields {
    void __cdecl Assign_Auto_Function_(MOX2::action_func_t, int16_t) {}
}

namespace ERICNET {
    int __cdecl Mox_Play_Sound_(int16_t) { return 0; }
}

namespace ERIC {
    void __cdecl Play_Loose_Flic_() {}
}

namespace RUSS {
    void __cdecl Mox_Update_() {
        sound::Sound_Update_();
        NETMOX::Mox_Net_Update_();
    }
    void __cdecl Make_Player_Into_AI_(int) {}
}

namespace MAINSCR {
    void __cdecl Main_Receive_Message_(uint32_t, int16_t, uint16_t, uint32_t, void*) {}
    void __cdecl Update_Ship_Icons_In_Between_Turns_() {}
}

namespace COLCALC {
    void __cdecl No_Op1_() {}
}

namespace NEXTTURN {
    void __cdecl Next_Turn_() {}
}

namespace COMBFIND {
    s_combfind_star_has_player* _current_star_has_player = nullptr;
    uint8_t* _current_star_has_nonplayer = nullptr;
    s_combfind_star_has_player (*_star_has_player)[MAX_STARS + 1][MAX_PLAYERS] = nullptr;
    uint8_t (*_star_has_nonplayer)[MAX_STARS + 1] = nullptr;

    void __cdecl Client_Combat_Loop_() {}
    void __cdecl Do_Net_Combat_Update_(bool) {}
    void __cdecl Copy_Target_Info_(s_combfind_target*) {}
    int16_t __cdecl Target_Colony_To_Planet_(int32_t colony_idx) { return (int16_t)colony_idx; }
    void __cdecl Decode_Star_Player_Presence_(NETPROTO::Packet_Reader_*, int16_t) {}
    void __cdecl Send_Star_Player_Presence_(NETPROTO::Packet_Writer_*, int16_t) {}
}

namespace EVENTS {
    s_event_data _event_data[36] = {};
    uint32_t _last_event_year = 0;
    int16_t _event_delay_count = 0;
    void __cdecl Event_Monster_Update_(int16_t, int16_t, int16_t) {}
}

namespace REPORT {
    int16_t _n_reports = 0;
    s_report _reports[MAX_REPORTS] = {};
    uint8_t _something_interesting_happened[MAX_PLAYERS] = {};

    void __cdecl Client_Initialize_Reports_() {}
}

namespace MSG_ {
    uint8_t _first_msg[MAX_PLAYERS] = {};
    uint8_t _n_msgs[MAX_PLAYERS] = {};
    s_MSG _msgs[MAX_MESSAGES] = {};
    uint8_t _max_msgs[MAX_PLAYERS] = {};
}

namespace diplodef {
    int16_t _COUNCIL_FLAG = 0;
    int16_t _next_council_meeting_turn = 0;
}

namespace diplomac {
    void __cdecl Change_Relations_(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t) {}
}

namespace GENDRAW {
    void __cdecl Message_Box_(char const*) {}
}

namespace DIP_SCRN {
    void __cdecl Diplomacy_Receive_Message_(uint32_t, uint16_t, uint32_t, s_dipscrn_net_message*) {}
    void __cdecl Diplomacy_Screen_(int32_t) {}
}

namespace CMBTMOV1 {
    void __cdecl Tactical_Combat_Receive_Message_(int, uint16_t, int, s_net_tactical_combat_message*) {}
}

namespace netcode {
    void (*g_net_update_hook)() = nullptr;

    uint32_t __cdecl Net_Send_Message_(int32_t, int16_t, void*, uint32_t) { return 1; }
    void __cdecl Net_Update_() {
        if (g_net_update_hook != nullptr) {
            g_net_update_hook();
        }
    }
    int32_t __cdecl Net_Is_Sending_() { return 0; }
}

namespace sound {
    void __cdecl Sound_Update_() {}
}