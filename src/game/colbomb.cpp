#include "pch.h"

namespace COLBOMB {
    s_combat_bomb_result* _results_ptr;
    s_combat_bomb_result* _backup_results_ptr;
    char s_Perdita_di_s_00556b14[14] = "Perdita di:%s";
    char s_Edificio_distrutto_s_00556b24[23] = "Edificio distrutto: %s";

    void __cdecl Colony_Bombing_Screen_(s_combat_bomb_result* results) {
        s_combat_bomb_result backup;

        _results_ptr = results;
        struct_::Copy_Structure_(&backup, results, 0x49);
        _backup_results_ptr = &backup;

        Do_Summary_String_Popup_();

        COLONY::Set_Colony_Globals_(_results_ptr->colony_idx);

        MOX::_current_screen = MOX::_return_screen;
    }

    void __cdecl Do_Summary_String_Popup_() {
        char attacker_name[500];
        char message_buffer[4000];

        COLONY::Set_Colony_Font_To_(4);

        HACCESS::Race_Name_(_backup_results_ptr->attacker_idx, attacker_name);

        int line_count = 1;

        int len = sprintf(
            message_buffer,
            ESTRINGS::E_Strings_(0x33),
            HACCESS::Get_Planet_Name_(MOX::_colony[_backup_results_ptr->colony_idx].planet),
            attacker_name
        );

        char* current_msg_ptr = message_buffer + len;

        if (_backup_results_ptr->population_killed != 0) {
            len = sprintf(current_msg_ptr, ESTRINGS::E_Strings_(0x1D), _backup_results_ptr->population_killed);
            current_msg_ptr += len;
            line_count = 2;
        }

        if (_backup_results_ptr->infantry_killed != 0) {
            len = sprintf(current_msg_ptr, ESTRINGS::E_Strings_(0x18), _backup_results_ptr->infantry_killed);
            current_msg_ptr += len;
            line_count++;
        }

        if (_backup_results_ptr->tanks_killed != 0) {
            len = sprintf(current_msg_ptr, ESTRINGS::E_Strings_(0x11), _backup_results_ptr->tanks_killed);
            current_msg_ptr += len;
            line_count++;
        }

        if (_backup_results_ptr->is_bio_weapon != 0) {
            len = sprintf(current_msg_ptr, ESTRINGS::E_Strings_(0x1B), _backup_results_ptr->bio_pop_killed);
            current_msg_ptr += len;
            line_count++;
        }

        for (int i = 0; i < 49; i++) {
            if (line_count >= 15) {
                break;
            }
            if (_backup_results_ptr->buildings_destroyed[i] != 0) {
                char* format_str;
                if (MOX::_settings.language == 4) {
                    format_str = s_Edificio_distrutto_s_00556b24;
                } else {
                    format_str = ESTRINGS::E_Strings_(0x37);
                }
                len = sprintf(current_msg_ptr, format_str, TECHDATA::_buildings[i].name);
                current_msg_ptr += len;
                line_count++;
            }
        }

        if (line_count > 15) {
            len = sprintf(current_msg_ptr, ESTRINGS::E_Strings_(0x17A));
            current_msg_ptr += len;
        }

        if (_backup_results_ptr->colony_wiped_out != 0) {
            char* format_str;
            if (MOX::_settings.language == 4) {
                format_str = s_Perdita_di_s_00556b14;
            } else {
                format_str = ESTRINGS::E_Strings_(0x37);
            }
            sprintf(current_msg_ptr, format_str, HACCESS::Get_Planet_Name_(MOX::_colony[_backup_results_ptr->colony_idx].planet));
        }

        TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x1F2), message_buffer);
    }
}