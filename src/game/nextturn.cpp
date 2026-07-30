#include "pch.h"

namespace NEXTTURN {

int16_t Get_Winner_() {
    if (diplodef::_council_winner != -1) {
        return (int16_t)diplodef::_council_winner;
    }

    int16_t winner_idx = -1;
    int16_t num_players = (int16_t)MOX::_NUM_PLAYERS;

    for (int16_t i = 0; i < num_players; ++i) {
        if (MOX::_player[i].captured_antares != 0 || MOX::_player[i].eliminated == 0) {
            winner_idx = i;
        }
    }

    return winner_idx;
}

void Do_Autosave_() {
    // Original has 4
    int32_t autosave_interval = 1;

    if (MOX::_game_type == 2 || MOX::_game_type == 3) {
        autosave_interval = 1;
    }

    if (MOX::_settings.auto_saves != 0) {
        if (((int32_t)MOX::_stardate % autosave_interval) == 0) {
            strlcpy(MOX::_save_game_description[9].description, "(Auto Save)", sizeof(MOX::_save_game_description[9].description));
            FILEDEF::Save_Game_(9);
        }
    }
}

void Next_Turn_() {
    if (MOX::_game_type == 0 || MOX::_game_type == 1) {
        Do_Autosave_();
    }

    HAROLD::Confirm_Planet_And_Star_Index_Link_(HAROLD::s_in_next_turn);
    Next_Turn_Calc_();

    MOX::_current_screen = MOX::_return_screen;

    if (MOX::_game_type != 2 && MOX::_game_type != 3 && RUSS::_all_ai_game == 0) {
        if (COLCALC::N_Living_Human_Players_() < 1) {
            ERIC::Fast_Fade_Out_();
            video::Set_Page_Off_();
            graphics::Fill_(0, 0, 0x27f, 0x1df, 0);
            RUSS::Mox_Sync_Update_();
            video::Copy_Off_To_Back_();
            video::Set_Refresh_Stencil_();
            ERIC::Play_Loose_Flic_();
            SCORE::End_Of_Game_Hi_Score_(MOX::_PLAYER_NUM);
            MOX::_current_screen = SCREEN_MAIN_MENU;
        } else {
            if (COLCALC::N_Living_Players_() < 2) {
                int16_t winner = Get_Winner_();
                ERIC::Fast_Fade_Out_();
                ERIC::Play_Normal_Win_Flic_();
                SCORE::End_Of_Game_Hi_Score_(winner);
                MOX::_current_screen = SCREEN_MAIN_MENU;
            } else if (diplodef::_council_winner != -1) {
                if (MOX::_PLAYER_NUM != diplodef::_council_winner) {
                    ERIC::Play_Loose_Flic_();
                }
                SCORE::End_Of_Game_Hi_Score_(MOX::_PLAYER_NUM);
                MOX::_current_screen = SCREEN_MAIN_MENU;
            } else {
                for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
                    if (MOX::_player[i].captured_antares != 0) {
                        fields::Deactivate_Auto_Function_();
                        ERIC::Play_All_Flics_();
                        SCORE::End_Of_Game_Hi_Score_((int16_t)i);
                        MOX::_current_screen = SCREEN_MAIN_MENU;
                    }
                }
            }
        }
    }

    for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
        MOX::_player[i].player_done_flags = 0;
    }

    MOX::_old_system_index = -1;
    oracle::Complete_Turn_();
}

void __cdecl Next_Turn_Calc_() {
    if (MOX::_game_type >= 0 && MOX::_game_type <= 1) {
        fields::Assign_Auto_Function_(SCORE::nullsub, 1);
    }

    MSG_::Allocate_Msg_Slots_();

    if (MOX::_settings.xenons_exist != 0 && MOX::_antarans.field6_0x3c == 0) {
        antarans::Antaran_Invasion_Check_();
    }

    aitech::Apply_Evolutionary_Upgrades_();
    AIDATA::Compute_AI_Data_();
    DIP_SCRN::Resolve_Delayed_Diplomacy_Orders_();
    DIP_SCRN::Set_Opportunity_Attacks_();
    diplomac::Clear_Diplomacy_Messages_();
    npcdiplo::NPC_Diplomacy_();
    diplomac::Diplomacy_Growth_();
    aileader::Do_AI_Leaders_();
    AIBUILD::Compute_Empire_Building_Needs_();
    aidudes::All_Colony_AI_();
    AIMOVE::Move_All_AI_();
    COLONIZE::All_AI_Colonize_();
    aitech::All_AI_Tech_Select_();
    maintain::Make_Scrap_Ships_Dead_();
    REPORT::Initialize_Reports_();
    TRADE::Process_Trade_And_Research_Agreements_();
    SHIPMOVE::Move_All_Ships_Toward_Stars_();
    spy::Resolve_Spies_();
    COLCALC::Apply_All_Colony_Changes_();
    COLCALC::Do_Surrenders_();
    AIDATA::Deallocate_AI_Data_();
    COMBFIND::Search_For_Battles_();
    EVENTS::Determine_Event_();
    ERICNET::Do_All_Ships_XP_Check_();
    EVENTS::Event_Twiddle_();
    lostship::Lose_Out_Of_Range_Ships_();
    COLCALC::All_Colony_Calculations_();
    COLCALC::Apply_All_Player_Changes_();
    COLCALC::Compute_Blockades_();
    SETTLER::Move_Settlers_();
    COLCALC::All_Colony_Calculations_();
    SHIPMOVE::Allocate_New_Ship_Slots_();
    OFFICER::Check_For_Officer_Level_();
    OFFICER::Decrement_Officer_ETA_();
    invasion::Check_All_Rebellions_();
    INITSHIP::Repair_Ships_At_Colonies_();
    COLONIZE::Allocate_New_Colony_Slots_();
    CONTACT::Compute_Contacts_();
    diplomac::Determine_First_Contacts_();
    SHIPMOVE::Reset_Needless_Ignore_Combat_Ships_Flags_();
    diplomac::Determine_Diplomacy_Messages_();
    diplomac::End_Of_Turn_Diplomacy_Adjustments_();

    if (MOX::_game_type == 0 && RUSS::_all_ai_game == 0) {
        council::Check_For_Council_Meeting_();
    }

    for (int16_t i = 0; i < MOX::_NUM_PLAYERS; ++i) {
        OFFICER::Random_Officer_Check_(i);
    }

    MSG_::Add_Ongoing_Event_Msgs_();

    if (capture::Check_Release_Version_() == 0) {
        OFFICER::Assert_Marooned_Leaders_();
    }

    BILL::Record_History_();
    MOX::_stardate++;

    if (MAINSCR::_in_auto_next_turn_mode == 0) {
        mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);
    }
}

} // namespace NEXTTURN