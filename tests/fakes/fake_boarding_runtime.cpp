#include "pch.h"

#include "game/boarding.h"
#include "game/techdata.h"

namespace {
    s_combat_data combat_data[2];
    s_ship_data ships[2];
    s_player players[2];
    s_leader_data leaders[1];
    int16_t killed_officer;
    int16_t pooled_officer;
    int16_t raid_choice_idx;
    int16_t combat_shield_max;
    int16_t boarding_attacker_bonus;
    int16_t boarding_defender_bonus;
    char last_raid_message[3000];
    char weapon_names[8][80] = {
        "Weapon zero with a deliberately long raid report name",
        "Weapon one with a deliberately long raid report name",
        "Weapon two with a deliberately long raid report name",
        "Weapon three with a deliberately long raid report name",
        "Weapon four with a deliberately long raid report name",
        "Weapon five with a deliberately long raid report name",
        "Weapon six with a deliberately long raid report name",
        "Weapon seven with a deliberately long raid report name"
    };
    char weapon_plural_names[8][80] = {
        "Weapon zeroes with a deliberately long raid report name",
        "Weapon ones with a deliberately long raid report name",
        "Weapon twos with a deliberately long raid report name",
        "Weapon threes with a deliberately long raid report name",
        "Weapon fours with a deliberately long raid report name",
        "Weapon fives with a deliberately long raid report name",
        "Weapon sixes with a deliberately long raid report name",
        "Weapon sevens with a deliberately long raid report name"
    };
    char special_names[8][80] = {
        "Special device zero with a deliberately long raid report name",
        "Special device one with a deliberately long raid report name",
        "Special device two with a deliberately long raid report name",
        "Special device three with a deliberately long raid report name",
        "Special device four with a deliberately long raid report name",
        "Special device five with a deliberately long raid report name",
        "Special device six with a deliberately long raid report name",
        "Special device seven final marker"
    };
}

namespace TEST_BOARDING {
    void Reset_() {
        memset(combat_data, 0, sizeof(combat_data));
        memset(ships, 0, sizeof(ships));
        memset(players, 0, sizeof(players));
        memset(leaders, 0, sizeof(leaders));
        memset(MOX::_crew_data, 0, sizeof(MOX::_crew_data));
        memset(TECHDATA::_hull_data, 0, sizeof(TECHDATA::_hull_data));
        memset(TECHDATA::_specials, 0, sizeof(TECHDATA::_specials));
        memset(TECHDATA::_weapons, 0, sizeof(TECHDATA::_weapons));
        for (s_combat_data& ship : combat_data) {
            ship.officer_idx = -1;
            ship.ship_idx = -1;
            ship.stasis_source_idx = UINT8_MAX;
            ship.black_hole_source_idx = UINT8_MAX;
        }
        for (s_ship_data& ship : ships) {
            ship.officer_index = -1;
        }
        memset(MOX::_ship_turn_order, -1, sizeof(MOX::_ship_turn_order));
        MOX::_ship_turn_order[0] = -10;
        MOX::_combat_attacker_player = 0;
        MOX::_combat_defender_player = 1;
        MOX::_combat_total_ship_count = 2;
        MOX::_auto_combat = 0;
        MOX::_auto_combat_flag[0] = 0;
        MOX::_auto_combat_flag[1] = 0;
        MOX::_super_fast_flag = 0;
        MOX::_in_nebula_flag = 0;
        MOX::_ken_trace = 0;
        killed_officer = -1;
        pooled_officer = -1;
        raid_choice_idx = 0;
        combat_shield_max = 0;
        boarding_attacker_bonus = 0;
        boarding_defender_bonus = 0;
        last_raid_message[0] = '\0';
        BOARDING::_telepathic_multiple_capture = 0;
        BOARDING::_kill_excess_marines_after_capture = 0;
        BOARDING::_board_in_black_hole = 1;
    }

    int16_t Killed_Officer_() {
        return killed_officer;
    }

    int16_t Pooled_Officer_() {
        return pooled_officer;
    }

    void Set_Combat_Shield_Max_(int16_t value) {
        combat_shield_max = value;
    }

    void Prepare_Long_Raid_() {
        combat_data[0].owner = 0;
        combat_data[1].owner = 1;
        strlcpy(combat_data[1].name, "Target", sizeof(combat_data[1].name));
        combat_data[1].drive_current_hits = 100;
        combat_data[1].computer_current_hits = 100;
        combat_data[1].shield_current_hits = 100;

        players[0].objectives = PLAYER_OBJECTIVE_HUMAN;
        players[1].objectives = PLAYER_OBJECTIVE_HUMAN;

        for (int i = 0; i < 8; ++i) {
            combat_data[1].weapons[i].weapon_id = (uint16_t)(i + 1);
            combat_data[1].weapons[i].count = 1;
            combat_data[1].weapon_hits[i] = 100;
            combat_data[1].weapon_hits_per_unit[i] = 100;
            combat_data[1].weapon_total_hits[i] = 100;
            combat_data[1].special_hits[i] = i == 0 ? 200 : 10;
            combat_data[1].special_device_hits[i] = combat_data[1].special_hits[i];
            struct_::Set_Bit_Field_(combat_data[1].special_device_flags, i + 1);

            TECHDATA::_weapons[i + 1].name = weapon_names[i];
            TECHDATA::_weapons[i + 1].name_plural = weapon_plural_names[i];
            TECHDATA::_specials[i + 1].name = special_names[i];
        }
    }

    const char* Last_Raid_Message_() {
        return last_raid_message;
    }

    int16_t Boarding_Attacker_Bonus_() {
        return boarding_attacker_bonus;
    }

    int16_t Boarding_Defender_Bonus_() {
        return boarding_defender_bonus;
    }
}

namespace MOX {
    s_combat_data* _combat_data = combat_data;
    s_ship_data* _ship = ships;
    s_player* _player = players;
    s_leader_data* _leaders = leaders;
    int16_t _ship_turn_order[210];
    int16_t _combat_attacker_player;
    int16_t _combat_defender_player;
    int16_t _combat_total_ship_count;
    int16_t _auto_combat;
    int16_t _auto_combat_flag[2];
    int16_t _super_fast_flag;
    int16_t _in_nebula_flag;
    uint16_t _ken_trace;
    s_crew_data _crew_data[CREW_QUALITY_COUNT];
} // namespace MOX

namespace OFFICER {
    void __cdecl Kill_Officer_(int16_t officer_idx) {
        killed_officer = officer_idx;
    }

    void __cdecl Move_Officer_To_Pool_(int16_t officer_idx) {
        pooled_officer = officer_idx;
    }

    int16_t __cdecl Owned_Officer_Level_(int16_t) {
        return 0;
    }
}

namespace COMBINIT {
    int16_t __cdecl Does_Combat_Ship_Have_Special_(int16_t, int16_t) {
        return 0;
    }

    int16_t __cdecl Calc_Combat_Shield_Max_(int16_t) {
        return combat_shield_max;
    }
}

namespace CMBTFIRE {
    void __cdecl Destroy_Ship_(int16_t, int16_t, int16_t, int16_t) {
    }

    void __cdecl Retarget_Missiles_Aimed_At_(int16_t) {
    }

    int16_t __cdecl Is_Combat_Ship_Dead_(int16_t) {
        return 0;
    }

    void __cdecl Remove_Stasis_Beam_(int16_t) {
    }
}

namespace COMBAT1 {
    void __cdecl Remove_Continuing_Effect_Specials_(int16_t) {
    }

    void Recalculate_Combat_Ship_(int16_t, bool) {
    }
}

namespace game_random {
    uint32_t __cdecl Random_(uint32_t) {
        return 0;
    }
}

namespace invasion {
    void __cdecl Get_Boarding_Info_(
        int16_t,
        int16_t,
        int16_t attacker_bonus,
        int16_t,
        int16_t,
        int16_t defender_bonus,
        s_invasion_side_info* side_a,
        s_invasion_side_info* side_b) {
        boarding_attacker_bonus = attacker_bonus;
        boarding_defender_bonus = defender_bonus;
        memset(side_a, 0, sizeof(*side_a));
        memset(side_b, 0, sizeof(*side_b));
        side_a->unit_strengths[1] = 100;
    }

    void __cdecl Ground_Combat_Round_(
        s_invasion_side_info*,
        s_invasion_side_info*) {
    }
}

namespace FMTPARA {
    void __cdecl Set_Paragraph_Sint_Pointer_(uint16_t, void*) {
    }
}

namespace RUSS {
    int16_t __cdecl Get_Weighted_Choice_Short_(const uint16_t*, int16_t) {
        static const int16_t choices[20] = {
            2, 3, 4, 5, 6, 7, 8, 9,
            10, 11, 12, 13, 14, 15, 16, 17,
            0, 1, 10, 10
        };
        return choices[raid_choice_idx++];
    }
}

namespace KEN {
    void __cdecl Ken_Get_Text_Message_(int32_t msg_index, char* buffer, int16_t message_arg) {
        const char* message = "";
        switch (msg_index) {
        case 21: message = "Computer "; break;
        case 42: message = "Raid on "; break;
        case 43: message = "Drive damaged\n"; break;
        case 44: message = "Shield "; break;
        case 45: message = "damaged\n"; break;
        case 46: message = "destroyed\n"; break;
        case 47: message = "No systems damaged\n"; break;
        case 92: message = "Attacker strength "; break;
        case 93: message = "Defender strength "; break;
        case 94: message = "attacking marine killed\n"; break;
        case 95: message = "attacking marines killed\n"; break;
        case 96: message = "defending marine killed\n"; break;
        case 97: message = "defending marines killed\n"; break;
        }
        strlcpy(buffer, message, (size_t)message_arg);
    }
}

namespace ERICNET {
    int __cdecl Mox_Play_Sound_(int16_t) {
        return 0;
    }
}

namespace sound {
    void __cdecl Fadeout_Sound_(int16_t, int8_t) {
    }
}

namespace CMBTDRW1 {
    void __cdecl Refresh_Combat_Screen_Full_() {
    }
}

namespace TEXTBOX {
    void __cdecl Text_Box_(const char*, const char* message) {
        strlcpy(last_raid_message, message, sizeof(last_raid_message));
    }

    void __cdecl Timed_Text_Box_(const char*, const char* message, int16_t) {
        strlcpy(last_raid_message, message, sizeof(last_raid_message));
    }
}

namespace string {
    const char s_Newline[] = "\n";
}

namespace TECHDATA {
    s_tech_hull_data _hull_data[HULL_CLASS_COUNT];
    s_tech_specials_data _specials[SPECIAL_COUNT];
    s_tech_weapon_data _weapons[WEAPON_COUNT];
}
