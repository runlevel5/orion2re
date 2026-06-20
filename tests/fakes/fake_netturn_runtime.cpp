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
    s_player* _player = nullptr;
    int16_t _NUM_PLAYERS = 0;
    int16_t _NUM_STARS = 0;
    s_star_data* _star = nullptr;
}

namespace NETSTART {
    void __cdecl Mox_Net_Uninit_() {}
}

namespace MULTPLAY {
    void __cdecl Load_Net_Next_Turn_Screen_(int8_t) {}
    void __cdecl Draw_Net_Next_Turn_Screen_() {}
    void __cdecl Add_Net_Next_Turn_Fields_() {}
    void __cdecl Chat_Box_Input_Loop_() {}
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
    void __cdecl Mox_Update_() {}
    void __cdecl Make_Player_Into_AI_(int) {}
}

namespace allocate {
    void __cdecl Pop_Block_(void*) {}
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

    void __cdecl Client_Combat_Loop_() {}
}

namespace REPORT {
    uint8_t _something_interesting_happened[MAX_PLAYERS] = {};
}

namespace DIP_SCRN {
    void __cdecl Diplomacy_Receive_Message_(uint32_t, uint16_t, uint32_t, s_dipscrn_net_message*) {}
}

namespace CMBTMOV1 {
    void __cdecl Tactical_Combat_Receive_Message_(int, uint16_t, int, s_net_tactical_combat_message*) {}
}

namespace netcode {
    uint32_t __cdecl Net_Send_Message_(int32_t, int16_t, void*, uint32_t) { return 1; }
    void __cdecl Net_Update_() {}
    int32_t __cdecl Net_Is_Sending_() { return 0; }
}

namespace sound {
    void __cdecl Sound_Update_() {}
}