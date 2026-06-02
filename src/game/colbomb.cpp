#include "pch.h"

namespace COLBOMB {
    s_combat_bomb_result* _results_ptr;
    s_combat_bomb_result* _backup_results_ptr;

    void __cdecl Colony_Bombing_Screen_(s_combat_bomb_result* results) {
        s_combat_bomb_result backup_results;

        _results_ptr = results;
        struct_::Copy_Structure_(&backup_results, results, sizeof(backup_results));
        _backup_results_ptr = &backup_results;

        Do_Summary_String_Popup_();

        COLONY::Set_Colony_Globals_(_results_ptr->colony_idx);

        MOX::_current_screen = MOX::_return_screen;
    }

    void __cdecl Do_Summary_String_Popup_() {
        char attacker_name[500];
        char message_buffer[4000];
        char* message_cursor;
        int32_t line_count;
        int32_t len;

        COLONY::Set_Colony_Font_To_(4);

        HACCESS::Race_Name_(_backup_results_ptr->attacker_idx, attacker_name);

        line_count = 1;

        len = sprintf(
            message_buffer,
            ESTRINGS::E_Strings_(0x33),
            HACCESS::Get_Planet_Name_(MOX::_colony[_backup_results_ptr->colony_idx].planet),
            attacker_name
        );

        message_cursor = message_buffer + len;

        if (_backup_results_ptr->population_killed != 0) {
            len = sprintf(message_cursor, ESTRINGS::E_Strings_(0x1D), _backup_results_ptr->population_killed);
            message_cursor += len;
            line_count = 2;
        }

        if (_backup_results_ptr->infantry_killed != 0) {
            len = sprintf(message_cursor, ESTRINGS::E_Strings_(0x18), _backup_results_ptr->infantry_killed);
            message_cursor += len;
            line_count++;
        }

        if (_backup_results_ptr->tanks_killed != 0) {
            len = sprintf(message_cursor, ESTRINGS::E_Strings_(0x11), _backup_results_ptr->tanks_killed);
            message_cursor += len;
            line_count++;
        }

        if (_backup_results_ptr->is_bio_weapon != 0) {
            len = sprintf(message_cursor, ESTRINGS::E_Strings_(0x1B), _backup_results_ptr->bio_pop_killed);
            message_cursor += len;
            line_count++;
        }

        for (int32_t building_idx = 0; building_idx < 49; building_idx++) {
            if (line_count >= 15) {
                break;
            }
            if (_backup_results_ptr->buildings_destroyed[building_idx] != 0) {
                const char* building_format;
                if (MOX::_settings.language == 4) {
                    building_format = "Edificio distrutto: %s";
                } else {
                    building_format = ESTRINGS::E_Strings_(0x37);
                }
                len = sprintf(message_cursor, building_format, TECHDATA::_buildings[building_idx].name);
                message_cursor += len;
                line_count++;
            }
        }

        if (line_count > 15) {
            len = sprintf(message_cursor, ESTRINGS::E_Strings_(0x17A));
            message_cursor += len;
        }

        if (_backup_results_ptr->colony_wiped_out != 0) {
            const char* wiped_out_format;
            if (MOX::_settings.language == 4) {
                wiped_out_format = "Perdita di:%s";
            } else {
                wiped_out_format = ESTRINGS::E_Strings_(0x37);
            }
            sprintf(message_cursor, wiped_out_format, HACCESS::Get_Planet_Name_(MOX::_colony[_backup_results_ptr->colony_idx].planet));
        }

        TEXTBOX::Text_Box_(ESTRINGS::E_Strings_(0x1F2), message_buffer);
    }
}