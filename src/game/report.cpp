#include "pch.h"

namespace REPORT {

    s_colony* __cdecl Player_Colony_Base_At_Star_(short player_idx, short star_idx) {
        for (int i = 0; i < 5; ++i) {
            short planet_idx = MOX::_star[star_idx].planet_index[i];
            
            if (planet_idx != -1) {
                short colony_idx = MOX::_planet[planet_idx].colony_index;
                
                if (colony_idx != -1) {
                    s_colony* colony = &MOX::_colony[colony_idx];
                    
                    if (colony->owner == player_idx && colony->buildings[11] > 0) {
                        return colony;
                    }
                }
            }
        }

        return nullptr;
    }

    s_ship_data* __cdecl Player_Ship_Type_At_Star_(short owner, short ship_type, short location, char include_just_built) {
        s_ship_data* ship = MOX::_ship;
        int num_ships = (int)MOX::_NUM_SHIPS;
        s_ship_data* end = &ship[num_ships];

        while (ship < end) {
            if ((short)(int8_t)ship->d.ship_type == ship_type &&
                (short)ship->owner == owner &&
                ship->location == location &&
                ship->status < 5) {

                if (include_just_built != 0 || ship->just_built == 0) {
                    return ship;
                }
            }
            ship++;
        }

        return nullptr;
    }

    int8_t __cdecl Has_Msg_(int16_t player_idx, int8_t show_summary) {
        int8_t result = 0;

        // Check if the summary for this player hasn't been displayed yet
        if (!((REPORT::_displayed_msgs >> (player_idx & 0x1F)) & 1)) {
            int16_t num_msgs = MSG_::N_Msgs_For_Player_(player_idx);

            if (num_msgs > 0 && MOX::_settings.end_of_turn_summary != 0) {
                int16_t first_idx = MSG_::First_Msg_For_Player_(player_idx);
                bool should_show = false;

                // Scan messages backwards from newest to oldest
                for (int i = num_msgs - 1; i >= 0; --i) {
                    s_MSG* msg = &MSG_::_msgs[first_idx + i];

                    switch (msg->msg_type) {
                    case 0:
                        switch (msg->u.colony.subtype) {
                        case 4:
                        case 9:
                            should_show = true;
                            break;
                        case 6:
                        case 7:
                        case 8:
                            should_show = false;
                            break;
                        default:
                            // Serious reports check (caseD_1)
                            should_show = (MOX::_settings.only_show_serious_turn_summary_reports == 0);
                            break;
                        }
                        break;

                    case 4:
                    case 5:
                    case 7:
                        should_show = true;
                        break;

                    case 8:
                    case 16:
                        should_show = false;
                        break;

                    case 9:
                        // Check specific criteria for message type 9
                        if (msg->u.hyperspace_beast.ship_idx != -1 && msg->u.hyperspace_beast.fate != 0) {
                            should_show = true;
                        }
                        else {
                            should_show = false;
                        }
                        break;

                    case 17:
                        if ((uint8_t)msg->u.diplomacy.plr_a == (uint8_t)MOX::_PLAYER_NUM) {
                            should_show = false;
                        }
                        else {
                            // Serious reports check (caseD_1)
                            should_show = (MOX::_settings.only_show_serious_turn_summary_reports == 0);
                        }
                        break;

                    default:
                        // Serious reports check (caseD_1)
                        should_show = (MOX::_settings.only_show_serious_turn_summary_reports == 0);
                        break;
                    }

                    if (should_show) {
                        break;
                    }
                }

                if (should_show) {
                    result = 4;
                    if (show_summary != 0) {
                        MOX::_current_screen = 0x28 /* SCREEN_TURN_SUMMARY */;
                        MOX::_return_screen = 0x27 /* SCREEN_REPORTS */;
                        MOX::_global_esc = 1;
                        REPORT::_displayed_msgs |= (1 << (player_idx & 0x1F));
                    }
                }
            }
        }

        return result;
    }

    int8_t __cdecl Colonization_(int16_t player_idx, char show_dialog) {
        for (int16_t star_idx = 0; star_idx < MOX::_NUM_STARS; ++star_idx) {
            if (MOX::_star[star_idx].colonize_player == player_idx) {
                if (struct_::Test_Bit_Field_(_done_colonize_check, star_idx) == 0) {
                    s_colony* colony_base = Player_Colony_Base_At_Star_(player_idx, star_idx);
                    s_ship_data* colony_ship = Player_Ship_Type_At_Star_(player_idx, 1, star_idx, 0);
                    s_ship_data* outpost_ship = Player_Ship_Type_At_Star_(player_idx, 4, star_idx, 0);

                    bool has_ships = false;
                    if (colony_ship != nullptr || outpost_ship != nullptr) {
                        has_ships = true;
                    }

                    if (colony_base != nullptr || (has_ships && ((MOX::_star[star_idx].ignore_colony_ships >> player_idx) & 1) == 0)) {
                        if (show_dialog != 0) {
                            do {
                                bool is_colony = false;
                                MOVEBOX::Remove_Box_Check_();

                                int16_t planet_idx;
                                if (colony_base == nullptr && (colony_ship == nullptr || outpost_ship != nullptr)) {
                                    planet_idx = PLNTSUM::Colony_Ship_Auto_Colonization_(outpost_ship);
                                    if (planet_idx == -1) {
                                        planet_idx = MAINPUPS::New_Outpost_Selection_Popup_(star_idx);
                                    } else if (MOX::_planet[planet_idx].colony_index == -1) {
                                        char msg[200];
                                        sprintf(msg, _outpost_preselected_planet_fmt, HACCESS::Get_Planet_Name_(planet_idx));
                                        if (GENDRAW::Do_Confirmation_Box_(msg) == 0) {
                                            planet_idx = -1;
                                        } else {
                                            if (namestar::Can_Change_Star_Name_(star_idx) != 0 && MOX::_settings.animations_on == 0) {
                                                int16_t y = namestar::Star_Name_Popup_Screen_Center_Y_();
                                                int16_t x = namestar::Star_Name_Popup_Screen_Center_X_();
                                                namestar::Change_Star_Name_Popup_(x, y, star_idx, 0);
                                            }
                                        }
                                    } else {
                                        planet_idx = MAINPUPS::New_Outpost_Selection_Popup_(star_idx);
                                    }
                                } else {
                                    is_colony = true;
                                    bool auto_colonized = false;

                                    if (colony_base == nullptr && PLNTSUM::Colony_Ship_Auto_Colonization_(colony_ship) != -1) {
                                        planet_idx = PLNTSUM::Colony_Ship_Auto_Colonization_(colony_ship);
                                        if (MOX::_planet[planet_idx].colony_index == -1) {
                                            auto_colonized = true;
                                            char msg[200];
                                            sprintf(msg, _colonize_preselected_planet_fmt, HACCESS::Get_Planet_Name_(planet_idx));
                                            if (GENDRAW::Do_Confirmation_Box_(msg) == 0) {
                                                planet_idx = -1;
                                            } else {
                                                if (namestar::Can_Change_Star_Name_(star_idx) != 0 && MOX::_settings.animations_on == 0) {
                                                    int16_t y = namestar::Star_Name_Popup_Screen_Center_Y_();
                                                    int16_t x = namestar::Star_Name_Popup_Screen_Center_X_();
                                                    namestar::Change_Star_Name_Popup_(x, y, star_idx, 0);
                                                }
                                            }
                                        }
                                    }

                                    if (!auto_colonized) {
                                        planet_idx = MAINPUPS::New_Colony_Selection_Popup_(star_idx, colony_base == nullptr);
                                    }

                                    while (colony_base != nullptr && planet_idx < 0) {
                                        int refund = TECHDATA::_buildings[11].cost / 2;
                                        char msg[200];
                                        sprintf(msg, _really_trash_colony_base_fmt, refund);
                                        if (GENDRAW::Do_Confirmation_Box_(msg) == 0) {
                                            planet_idx = MAINPUPS::New_Colony_Selection_Popup_(star_idx, colony_base == nullptr);
                                        } else {
                                            do {
                                                MOX::_player[player_idx].bc += refund;
                                                colony_base->buildings[11] = 0;
                                                colony_base = Player_Colony_Base_At_Star_(player_idx, star_idx);
                                            } while (colony_base != nullptr);
                                            break;
                                        }
                                    }
                                }

                                if (colony_ship != nullptr) {
                                    SHIPS::Clear_Auto_Colonization_Flag_(colony_ship);
                                }
                                if (outpost_ship != nullptr) {
                                    SHIPS::Clear_Auto_Colonization_Flag_(outpost_ship);
                                }

                                if (planet_idx < 0) {
                                    struct_::Set_Bit_Field_(_done_colonize_check, star_idx);
                                    MOX::_star[star_idx].ignore_colony_ships |= (1 << player_idx);
                                } else {
                                    if (is_colony) {
                                        COLONIZE::Make_New_Colony_(planet_idx, player_idx);
                                        if (colony_base == nullptr) {
                                            SHIPS::Kill_Ship_(static_cast<int16_t>(colony_ship - MOX::_ship));
                                            MAINSCR::Update_Ship_Icons_In_Between_Turns_();
                                            colony_base = nullptr;
                                        } else {
                                            ERICNET::Remove_Building_(static_cast<int16_t>(colony_base - MOX::_colony), 11);
                                            colony_base = Player_Colony_Base_At_Star_(player_idx, star_idx);
                                        }

                                        if (COLONIZE::Star_N_Colonizable_Planets_For_Player_(&MOX::_star[star_idx], player_idx) == 0) {
                                            struct_::Set_Bit_Field_(_done_colonize_check, star_idx);
                                            if (colony_base != nullptr) {
                                                do {
                                                    MOX::_player[player_idx].bc += TECHDATA::_buildings[11].cost / 2;
                                                    colony_base->buildings[11] = 0;
                                                    colony_base = Player_Colony_Base_At_Star_(player_idx, star_idx);
                                                } while (colony_base != nullptr);
                                            }
                                        }

                                        if (MOX::_settings.animations_on != 0) {
                                            MOX::_current_screen = 0x21; /* SCR_COLONIZE */
                                            MOX::_return_screen = 0x27; /* SCR_TURN_SUMMARY */
                                            MOX::_screen_data = MOX::_planet[planet_idx].star_index;
                                            MOX::_global_esc = 1;
                                            COLONY::_orbit_temp = MOX::_planet[planet_idx].orbit;
                                            return 3;
                                        }
                                        return 1;
                                    } else {
                                        COLONIZE::Make_New_Outpost_(planet_idx, player_idx);
                                        SHIPS::Kill_Ship_(static_cast<int16_t>(outpost_ship - MOX::_ship));
                                        MAINSCR::Update_Ship_Icons_In_Between_Turns_();
                                    }
                                }
                            } while (colony_base != nullptr);
                        }
                        return 1;
                    } else {
                        struct_::Set_Bit_Field_(_done_colonize_check, star_idx);
                    }
                }
            }
        }
        return 0;
    }

    int8_t __cdecl Display_Report_Aux_(int16_t player_idx, uint8_t param2) {
        int8_t status;

        if (((_done_player_reports >> (uint8_t)player_idx) & 1) != 0) {
            return 0;
        }

        if (MOX::_stardate == 35000) {
            return 0;
        }

        if ((status = Antarans_(player_idx, param2)) != 0) return status;
        if ((status = Has_GNN_((uint8_t)player_idx, (bool)param2)) != 0) return status;
        if ((status = Set_Initial_Tech_(player_idx, (char)param2)) != 0) return status;
        if ((status = Has_Research_Breakthrough_(player_idx, (int8_t)param2)) != 0) return status;
        if ((status = Has_Evolutionary_Upgrade_(player_idx, (int8_t)param2)) != 0) return status;
        if ((status = Has_Officer_For_Hire_(player_idx, (int8_t)param2)) != 0) return status;
        if ((status = Colonization_(player_idx, param2)) != 0) return status;
        if ((status = Explored_New_Star_(player_idx, (char)param2)) != 0) return status;
        if ((status = OFFICER::Officer_Made_Level_Report_(player_idx, (int8_t)param2)) != 0) return status;
        if ((status = Has_Report_(player_idx, param2)) != 0) return status;
        if ((status = Has_Diplomacy_Messages_(player_idx, (bool)param2)) != 0) return status;
        if ((status = Has_Msg_(player_idx, (int8_t)param2)) != 0) return status;
        if ((status = Empty_Colony_Queue_(player_idx, (int8_t)param2)) != 0) return status;
        if ((status = Need_To_Set_Occupation_Policy_(player_idx, (int8_t)param2)) != 0) return status;

        _done_player_reports |= (1 << (uint8_t)player_idx);

        return 0;
    }

    uint8_t __cdecl Display_Report_(int16_t report_idx) {
        return (uint8_t)REPORT::Display_Report_Aux_(report_idx, 1);
    }

    /**
     * @address 004a9e40
     */
    void __cdecl sprintf_report_(char* buffer, s_report* report) {
        if (report->type == 1 /* MSG_TYPE_REPORT_YOUR_SPY */) {
            if (report->action == 0 /* SPY_ACTION_STEAL */) {
                buffer += sprintf(buffer, REPORT::_your_spy_steals_fmt,
                                  TECHDATA::_technology_applications[report->d.steal.tech_app_id].name,
                                  MOX::_player[report->d.steal.victim_player_idx].race_name);
            } else if (report->action == 1 /* SPY_ACTION_SABOTAGE */) {
                char* planet_name = HACCESS::Get_Planet_Name_(report->d.sabotage.planet_idx);
                buffer += sprintf(buffer, REPORT::_your_spy_destroys_fmt,
                                  MOX::_player[report->d.sabotage.victim_player_idx].race_name,
                                  TECHDATA::_buildings[report->d.sabotage.building_id].name,
                                  planet_name);
            }

            if (report->d.steal.framed_player_idx != -1) {
                sprintf(buffer, REPORT::_you_framed_fmt,
                        MOX::_player[report->d.steal.framed_player_idx].race_name);
            }
        } else if (report->type == 2 /* MSG_TYPE_REPORT_ENEMY_SPY */) {
            if (report->action == 0 /* SPY_ACTION_STEAL */) {
                sprintf(buffer, REPORT::_enemy_spy_steals_fmt,
                        MOX::_player[report->d.enemy_steal.enemy_player_idx].race_name,
                        TECHDATA::_technology_applications[report->d.enemy_steal.tech_app_id].name);
            } else if (report->action == 1 /* SPY_ACTION_SABOTAGE */) {
                char* planet_name = HACCESS::Get_Planet_Name_(report->d.enemy_sabotage.planet_idx);
                sprintf(buffer, REPORT::_enemy_spy_destroys_fmt,
                        MOX::_player[report->d.enemy_sabotage.enemy_player_idx].race_name,
                        TECHDATA::_buildings[report->d.enemy_sabotage.building_id].name,
                        planet_name);
            }
        }
    }

    char* __cdecl Load_Report_Strings_(char* data) {
        _your_spy_steals_fmt = data;
        data += strlen(data) + 1;

        _your_spy_destroys_fmt = data;
        data += strlen(data) + 1;

        _you_framed_fmt = data;
        data += strlen(data) + 1;

        _enemy_spy_steals_fmt = data;
        data += strlen(data) + 1;

        _enemy_spy_destroys_fmt = data;
        data += strlen(data) + 1;

        _really_trash_colony_base_fmt = data;
        data += strlen(data) + 1;

        _colonize_preselected_planet_fmt = data;
        data += strlen(data) + 1;

        _outpost_preselected_planet_fmt = data;
        data += strlen(data) + 1;

        return data;
    }

    /**
     * @address 004aa020
     */
    void __cdecl Client_Initialize_Reports_() {
        _done_player_reports = 0;
        _displayed_msgs = 0;
        _displayed_gnn = 0;

        // Clearing _done_colonize_check array (9 bytes)
        memset(_done_colonize_check, 0, sizeof(_done_colonize_check));

        MOX::_antarans.attack_notified_mask = 0;
    }

    /**
     * @address 004aa050
     */
    void __cdecl Initialize_Reports_() {
        _n_reports = 0;

        // Zeroing _something_interesting_happened (8 bytes)
        memset(_something_interesting_happened, 0, sizeof(_something_interesting_happened));

        Client_Initialize_Reports_();
    }

    /**
     * @brief Checks if a GNN (Galactic News Network) event is available.
     * 
     * @param index The bit index in the displayed GNN mask to check.
     * @param mark_as_displayed If true, marks the event as displayed and triggers event processing.
     * @return int8_t Returns 7 if a GNN event message is available, 0 otherwise.
     */
    int8_t __cdecl Has_GNN_(uint8_t index, bool mark_as_displayed) {
        int8_t status = 0;

        // Check if the GNN event at the specified index has already been displayed
        if (!((REPORT::_displayed_gnn >> index) & 0x1)) {
            if (EVENTS::Check_For_Event_Message_() != 0) {
                status = 7;
                if (mark_as_displayed) {
                    EVENTS::Check_For_Event_();
                    REPORT::_displayed_gnn |= (1 << index);
                }
            }
        }

        return status;
    }

    uint8_t __cdecl Must_Display_Reports_(int16_t player_num) {
        int8_t displayed = 0;

        // Check if player objectives are complete (flag 100)
        if (MOX::_player[player_num].objectives == 100) {
            displayed = REPORT::Display_Report_Aux_(player_num, 0);
        }

        if (displayed == 0) {
            REPORT::Redirect_Newly_Built_Ships_(player_num);

            // Re-checking displayed (BL) as per disassembly 0x004ab2c6
            if (displayed == 0) {
                // If game type is 2 (Hotseat) or 3 (Network), perform autosave
                if (MOX::_game_type >= 2 && MOX::_game_type <= 3) {
                    NEXTTURN::Do_Autosave_();
                }
            }
        }

        return displayed;
    }

    int8_t __cdecl Set_Initial_Tech_(short player_idx, char select_tech) {
        if (MOX::_stardate > 35000) {
            uint8_t field_idx = (uint8_t)MOX::_player[player_idx].current_research_field;

            // Check if current research field progress is at state 3 and field is 0
            if (MOX::_player[player_idx].tech_fields[field_idx] == 3 && field_idx == 0) {
                if (select_tech != 0) {
                    TECH::Tech_Select_();
                }
                return 2;
            }
        }
        return 0;
    }

    int8_t __cdecl Has_Officer_For_Hire_(int16_t player_idx, int8_t hire_flag) {
        int8_t has_officer = 0;
        int8_t officer_id = MOX::_player[player_idx].officer_for_hire;

        if (officer_id != -1) {
            has_officer = 1;

            if (hire_flag != 0) {
                bool accepted = MAINPUPS::Random_New_Officer_Popup_(player_idx);
                int16_t cost = OFFICER::Calculate_Officer_Cost_(player_idx, officer_id);

                OFFICER::Set_Officer_To_Player_(officer_id, player_idx, (uint8_t)accepted);

                if (accepted) {
                    MOX::_player[player_idx].bc -= (int32_t)cost;
                    COLCALC::Do_Player_Colony_Calculations_(player_idx);
                }

                MOX::_player[player_idx].officer_for_hire = -1;
            }
        }

        return has_officer;
    }

    /**
     * @address 004aae90
     */
    int8_t __cdecl Has_Research_Breakthrough_(int16_t player_idx, int8_t do_select) {
        int8_t status = 0;

        if (MOX::_player[player_idx].research_breakthrough != 0) {
            status = 1;
            if (do_select != 0) {
                TECH::Tech_Select_();
                MOX::_player[player_idx].research_breakthrough = 0;
                status = 2;
            }
        }

        return status;
    }

    int8_t __cdecl Has_Evolutionary_Upgrade_(int16_t player_idx, int8_t do_setup) {
        int8_t result = 0;
        s_player* plr = &MOX::_player[player_idx];

        // Check if player has researched Evolutionary Mutation (Tech App 62) 
        // and hasn't picked the upgrade yet.
        if (plr->tech_applications[62 /* TECH_APP_EVOLUTIONARY_MUTATION */] == 3 &&
            plr->picked_evolutionary_upgrade == 0) {
            
            result = 1;
            
            if (do_setup != 0) {
                raceopt::Setup_Evolutionary_Upgrade_(player_idx);
            }
        }
        
        return result;
    }

    /**
     * @address 004aa550
     */
    /**
     * @address 004aa0d0
     */
    int8_t __cdecl Draw_Special_(s_star_data* star, int16_t param_2) {
        int8_t special = star->system_special;

        if (special < 50) {
            switch (special) {
                case 0:
                case 4:
                case 5:
                case 6:
                case 10:
                case 11:
                    return special;
                default:
                    return 0;
            }
        } else {
            int8_t result;
            if (special >= 90) {
                result = 10;
            } else if (special >= 80) {
                result = 8;
            } else if (special >= 70) {
                result = 7;
            } else {
                result = (special >= 60) + 2;
            }

            int16_t remainder = special % 10;
            if (remainder == param_2 || result == 10) {
                return result;
            }

            return 0;
        }
    }

    int8_t __cdecl Star_Has_Special_For_Player_(s_star_data* star, int player_idx) {
        int8_t has_special = 0;

        if (star->system_special >= 50) {
            if ((star->system_special % 10) == player_idx) {
                has_special = 1;
            }
        }

        return has_special;
    }

    int8_t __cdecl Explored_New_Star_(short player_idx, char show_popup) {
        for (int i = 0; i < (int)MOX::_NUM_STARS; ++i) {
            s_star_data* star = &MOX::_star[i];
            int8_t has_special = REPORT::Star_Has_Special_For_Player_(star, (int)player_idx);

            // Check if player just visited this star or it contains a relevant special
            if (((star->just_visited >> (uint8_t)player_idx) & 1) != 0 || has_special != 0) {
                // If telepathic and the star discovery is not a special, clear discovery flag and skip report
                if (MOX::_player[player_idx].traits[27] != 0 && has_special == 0) {
                    star->just_visited &= ~(1 << (uint8_t)player_idx);
                    return 0;
                }

                if (show_popup != 0) {
                    MOVEBOX::Remove_Box_Check_();
                    char special_type = REPORT::Draw_Special_(star, player_idx);
                    short star_idx = (short)(star - MOX::_star);
                    MAINPUPS::New_System_Discovery_Popup_(star_idx, (int16_t)special_type);

                    // Mark as no longer "just visited" after reporting
                    star->just_visited &= ~(1 << (uint8_t)player_idx);
                    
                    if (has_special != 0) {
                        star->system_special = 0;
                    }
                }
                return 1;
            }
        }
        return 0;
    }

    /**
     * @address 004ab010
     */
    int8_t __cdecl Has_Diplomacy_Messages_(int16_t receiver_idx, bool show_messages) {
        int8_t has_messages = 0;

        for (int16_t player_idx = 0; player_idx < MOX::_NUM_PLAYERS; ++player_idx) {
            // A player doesn't send messages to themselves
            if (player_idx == receiver_idx) {
                continue;
            }

            // Check if the receiver is ignoring the sender
            if (!((MOX::_player[receiver_idx].ignoring >> (player_idx & 0x1F)) & 1)) {
                // Check if player_idx has a message for receiver_idx
                uint8_t msg_id = MOX::_player[player_idx].diplomacy_message[receiver_idx];
                if (msg_id != 0) {
                    has_messages = 5;

                    if (show_messages) {
                        // Check for multiplayer/interactive diplomacy requirements
                        if ((MOX::_game_type == 2 || MOX::_game_type == 3) &&
                            DIP_SCRN::Diplomacy_Message_Is_Interactive_((uint16_t)msg_id)) {
                            
                            NETMOX::Net_Request_AI_Diplomacy_((uint8_t)player_idx);
                            
                            while (!NETMOX::Net_May_Begin_AI_Diplomacy_()) {
                                RUSS::Mox_Update_();
                            }
                            
                            DIP_SCRN::Npc_Diplomacy_Screen_(player_idx, 0);
                            MOX::_player[player_idx].diplomacy_message[receiver_idx] = 0;
                            NETMOX::Net_End_AI_Diplomacy_();
                        } else {
                            // Standard single-player or non-interactive message display
                            DIP_SCRN::Npc_Diplomacy_Screen_(player_idx, 0);
                            MOX::_player[player_idx].diplomacy_message[receiver_idx] = 0;
                        }
                    }
                }
            }
        }

        return has_messages;
    }

    /**
     * @address 004aab80
     */
    /**
     * @brief Evaluates and triggers Antaran attack notifications.
     * 
     * @param param_1 Unused.
     * @param param_2 Flag indicating if notification logic should proceed beyond basic state check.
     * @return int8_t 0 if no notification, 6 if stardate matches but logic skipped, 1 if attack movie played.
     */
    int8_t __cdecl Antarans_(int16_t param_1, int8_t param_2) {
        int8_t result = 0;

        // Check if stardate is exactly one day past invasion date
        if (((int32_t)_antarans::invasion_stardate - (int32_t)MOX::_stardate) == -1) {
            // Check if current player was already notified
            if (!((_antarans::attack_notified_mask >> (uint8_t)MOX::_PLAYER_NUM) & 1)) {
                result = 6;
                if (param_2 != 0) {
                    int32_t target_star_idx = -1;
                    uint8_t max_turns_left = 0;

                    // Locate the Antaran ship with the longest remaining travel time to determine target
                    for (int32_t i = 0; i < MOX::_NUM_SHIPS; ++i) {
                        s_ship_data* ship = &MOX::_ship[i];
                        if (ship->owner == 8 /* Antaran */ && ship->status == 1 /* Active */) {
                            if (max_turns_left < (uint8_t)ship->turns_left) {
                                target_star_idx = ship->location - 500;
                                max_turns_left = ship->turns_left;
                            }
                        }
                    }

                    bool room_screen_shown = false;
                    if (target_star_idx != -1) {
                        // Check if player has a colony at the target system
                        if ((MOX::_star[target_star_idx].has_colony >> (uint8_t)MOX::_PLAYER_NUM) & 1) {
                            if (random::Random_(10) <= 4) {
                                ANTAROOM::Main_Antaran_Room_Screen_(_antarans::invasion_stardate < 35000 /* 0x88B8 */);
                                room_screen_shown = true;
                            }
                        }
                    }

                    if (!room_screen_shown) {
                        result = 1;
                        ERIC::Play_Antarans_Attack_Flic_();
                    }

                    // Mark player as notified
                    _antarans::attack_notified_mask |= (1 << (uint8_t)MOX::_PLAYER_NUM);
                }
            }
        }

        return result;
    }

    int8_t __cdecl Need_To_Set_Occupation_Policy_(int16_t player_idx, int8_t show_screen) {
        int16_t num_colonies = MOX::_NUM_COLONIES;
        s_colony* colonies = MOX::_colony;

        for (int i = 0; i < num_colonies; ++i) {
            s_colony* colony = &colonies[i];

            if (colony->owner == (int8_t)player_idx && colony->occupation_policy == 4) {
                uint8_t num_pops = colony->n_pops;
                for (int j = 0; j < (uint8_t)num_pops; ++j) {
                    if (colony->pop[j] & 0x400) {
                        if (show_screen != 0) {
                            MOVEBOX::Remove_Box_Check_();
                            MOX::_current_screen = 1;
                            MOX::_return_screen = 39 /* SCREEN_REPORTS */;
                            
                            int16_t planet_idx = colony->planet;
                            s_planet_data* planet = &MOX::_planet[planet_idx];
                            
                            MOX::_screen_data = (int16_t)planet->star_index;
                            COLONY::_orbit_temp = (int16_t)planet->orbit;
                            
                            COLONY::Colony_Screen_();
                        }
                        return 1;
                    }
                }
            }
        }

        return 0;
    }

    /**
     * @address 004aaac0
     */
    void __cdecl Redirect_Newly_Built_Ships_(int16_t player_idx) {
        bool any_ships_redirected = false;
        s_ship_data* ship = MOX::_ship;
        s_ship_data* end_ship = &MOX::_ship[MOX::_NUM_SHIPS];

        if (ship < end_ship) {
            do {
                if (ship->owner == (int8_t)player_idx && ship->just_built != 0) {
                    any_ships_redirected = true;
                    ship->just_built = 0;

                    // If ship is active (not destroyed/refitting)
                    if (ship->status == 0) {
                        int16_t star_idx = ship->location;
                        if (HACCESS::Star_Has_Relocation_(star_idx, player_idx)) {
                            int16_t target_star_idx = HACCESS::Relocation_(star_idx, player_idx);
                            
                            int16_t ship_indices[2];
                            s_ship_move_info move_info;

                            ship_indices[0] = (int16_t)(ship - MOX::_ship);
                            ship_indices[1] = -1; // Terminator

                            SHIPMOVE::Ships_Try_To_Move_To_(ship_indices, target_star_idx, &move_info);
                            if (move_info.moving != 0) {
                                SHIPMOVE::Make_Ships_Move_To_(ship_indices, target_star_idx, &move_info);
                            }
                        }
                    }
                }
                ship++;
            } while (ship < end_ship);
        }

        if (any_ships_redirected) {
            COLCALC::Do_Player_Colony_Calculations_(player_idx);
        }
    }

    /**
     * @address 004aa070
     */
    void __cdecl Add_Report_(s_report* report) {
        if (_n_reports < 50) {
            _reports[_n_reports] = *report;
            _n_reports++;
        }
    }

    int8_t __cdecl Has_Report_(int16_t player_idx, bool show_messages) {
        int8_t status = 0;
        char buf[200];

        if (REPORT::_n_reports > 0) {
            for (int16_t i = 0; i < REPORT::_n_reports; ++i) {
                if (REPORT::_reports[i].player_idx == player_idx) {
                    status = 1;
                    if (show_messages) {
                        bool report_handled = false;

                        // Check for special report type (steal tech)
                        if (REPORT::_reports[i].type == 1 && REPORT::_reports[i].action == 0) {
                            status = 2;
                            MOVEBOX::Remove_Box_Check_();
                            SCIENCE::Show_Off_Stolen_Tech_(
                                REPORT::_reports[i].d.steal.tech_app_id,
                                REPORT::_reports[i].d.steal.victim_player_idx
                            );
                            report_handled = true;
                        }

                        if (!report_handled) {
                            REPORT::sprintf_report_(buf, &REPORT::_reports[i]);
                            GENDRAW::Message_Box_(buf);
                        }

                        REPORT::_reports[i].player_idx = -1;
                    }
                }
            }
        }

        return status;
    }

    int8_t __cdecl Empty_Colony_Queue_(int16_t player_idx, int8_t do_jump) {
        s_colony* target_colony = nullptr;
        int16_t num_colonies = (int16_t)MOX::_NUM_COLONIES;

        for (int i = 0; i < num_colonies; ++i) {
            s_colony* colony = &MOX::_colony[i];
            // Check if colony belongs to player, is not an outpost, and its production queue is empty (-1)
            if (colony->owner == player_idx && colony->outpost_flag == 0 && colony->producing[0] == -1) {
                target_colony = colony;
                break;
            }
        }

        if (target_colony == nullptr) {
            return 0;
        }

        if (do_jump != 0) {
            MOVEBOX::Remove_Box_Check_();
            MOX::_return_screen = 39 /* SCREEN_REPORTS */;
            MOX::_current_screen = 1 /* SCREEN_COLONY */;
            
            // Set up planet specific data for the colony screen jump
            MOX::_screen_data = (int16_t)MOX::_planet[target_colony->planet].star_index;
            COLONY::_orbit_temp = (int16_t)MOX::_planet[target_colony->planet].orbit;
            MOX::_global_esc = 1;
        }

        return 3;
    }

}