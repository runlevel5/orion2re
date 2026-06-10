#include "pch.h"

namespace FILEDEF {
    const char s_Auto_Save_00556938[12] = "(Auto Save)";

    void __cdecl Load_Game_Settings_() {
        RUSS::Assert_Settings_();

        FILE* file = fopen("MOX.SET", "rb");
        if (file != nullptr) {
            fread(&MOX::_settings, sizeof(MOX::_settings), 1, file);
            fclose(file);
            RUSS::Assert_Settings_();
        }

        HAROLD::Set_Sound_Volume_To_Settings_();
    }

    bool Save_Game_Settings_(void) {
        RUSS::Assert_Settings_();

        FILE* file = fopen("MOX.SET", "wb");
        if (file == nullptr) {
            return false;
        }

        fwrite(&MOX::_settings, sizeof(MOX::_settings), 1, file);
        fclose(file);

        RUSS::Assert_Settings_();
        fclose(file);

        return true;
    }


    void __cdecl Save_Game_(int16_t slot_index) {
        uint32_t random_seed = game_random::Get_Random_Seed_();
        uint32_t save_padding[250];

        for (int i = 0; i < 250; ++i) {
            save_padding[i] = 0;
        }

        HAROLD::Save_Mouse_List_();

        char slot_str[20];
        itoa(slot_index + 1, slot_str, 10);

        char filename[260];
        strcpy(filename, "SAVE");
        strcat(filename, slot_str);
        strcat(filename, ".GAM");

        RUSS::Assert_Settings_();

        FILE* fp = fopen(filename, "wb");

        if (fp == nullptr) {
            char error_message[] = "Error saving game.\n";
            GENDRAW::Message_Box_(error_message);
            return;
        }

        uint32_t save_header = 0xe0;
        MISC::Increment_Mouse_Clock_Image_();
        fwrite(&save_header, sizeof(save_header), 1, fp);

        int16_t description_slot_index = (slot_index > 8) ? 9 : slot_index;
        fwrite(MOX::_save_game_description[description_slot_index].description, sizeof(MOX::_save_game_description[description_slot_index].description), 1, fp);
        fwrite(&MOX::_stardate, sizeof(MOX::_stardate), 1, fp);
        fwrite(&MOX::_game_type, sizeof(MOX::_game_type), 1, fp);
        fwrite(&MOX::_settings, sizeof(MOX::_settings), 1, fp);

        RUSS::Mox_Update_();
        fwrite(&random_seed, sizeof(random_seed), 1, fp);
        fwrite(&MOX::_NUM_COLONIES, sizeof(MOX::_NUM_COLONIES), 1, fp);
        fwrite(MOX::_colony, sizeof(s_colony), MAX_COLONIES, fp);

        RUSS::Mox_Update_();
        fwrite(&MOX::_planet_count, sizeof(MOX::_planet_count), 1, fp);
        fwrite(MOX::_planet, sizeof(s_planet_data), MAX_PLANETS, fp);

        RUSS::Mox_Update_();
        fwrite(&MOX::_NUM_STARS, sizeof(MOX::_NUM_STARS), 1, fp);
        fwrite(MOX::_star, sizeof(s_star_data), MAX_STARS, fp);

        RUSS::Mox_Update_();
        fwrite(MOX::_leaders, sizeof(s_leader_data), MAX_LEADERS, fp);

        RUSS::Mox_Update_();
        fwrite(&MOX::_NUM_PLAYERS, sizeof(MOX::_NUM_PLAYERS), 1, fp);
        fwrite(MOX::_player, sizeof(s_player), MAX_PLAYERS, fp);

        RUSS::Mox_Update_();
        fwrite(&MOX::_NUM_SHIPS, sizeof(MOX::_NUM_SHIPS), 1, fp);
        fwrite(MOX::_ship, sizeof(s_ship_data), MAX_SHIPS, fp);

        RUSS::Mox_Update_();
        fwrite(&MOX::_moveable_box, sizeof(MOX::_moveable_box[0]), 5, fp);
        fwrite(&MOX::_moveable_box_order, sizeof(MOX::_moveable_box_order[0]), 5, fp);
        fwrite(&MOX::_PLAYER_NUM, sizeof(MOX::_PLAYER_NUM), 1, fp);
        fwrite(&MOX::_max_map_scale, sizeof(MOX::_max_map_scale), 1, fp);
        fwrite(&MOX::_max_zoom_count, sizeof(MOX::_max_zoom_count), 1, fp);

        RUSS::Mox_Update_();
        fwrite(&MOX::_star_bg_index, sizeof(MOX::_star_bg_index), 1, fp);
        fwrite(&MOX::_MAP_MAX_X, sizeof(MOX::_MAP_MAX_X), 1, fp);
        fwrite(&MOX::_MAP_MAX_Y, sizeof(MOX::_MAP_MAX_Y), 1, fp);
        fwrite(&MOX::_fleet_icon_button_count, sizeof(MOX::_fleet_icon_button_count), 1, fp);
        fwrite(MOX::_nebula, sizeof(*MOX::_nebula), MAX_NEBULAS, fp);
        fwrite(&MOX::_NUM_NEBULAS, sizeof(MOX::_NUM_NEBULAS), 1, fp);
        fwrite(&MOX::_bill_savegame, sizeof(MOX::_bill_savegame), 1, fp);

        RUSS::Mox_Update_();
        fwrite(&REPORT::_n_reports, sizeof(REPORT::_n_reports), 1, fp);
        fwrite(&REPORT::_reports, sizeof(REPORT::_reports), 1, fp);
        fwrite(&REPORT::_done_player_reports, sizeof(REPORT::_done_player_reports), 1, fp);
        fwrite(&REPORT::_something_interesting_happened, sizeof(REPORT::_something_interesting_happened), 1, fp);
        fwrite(&REPORT::_done_colonize_check, sizeof(REPORT::_done_colonize_check), 1, fp);
        fwrite(&EVENTS::_event_data, sizeof(EVENTS::_event_data[0]), 36, fp);

        RUSS::Mox_Update_();
        fwrite(&MSG_::_msgs, sizeof(MSG_::_msgs[0]), 100, fp);
        fwrite(&MSG_::_first_msg, sizeof(MSG_::_first_msg), 1, fp);
        fwrite(&MSG_::_max_msgs, sizeof(MSG_::_max_msgs), 1, fp);
        fwrite(&MSG_::_n_msgs, sizeof(MSG_::_n_msgs), 1, fp);
        fwrite(&REPORT::_displayed_msgs, sizeof(REPORT::_displayed_msgs), 1, fp);
        fwrite(&REPORT::_displayed_gnn, sizeof(REPORT::_displayed_gnn), 1, fp);

        RUSS::Mox_Update_();
        fwrite(&DIPLODEF::_COUNCIL_FLAG, sizeof(DIPLODEF::_COUNCIL_FLAG), 1, fp);
        fwrite(&DIPLODEF::_next_council_meeting_turn, sizeof(DIPLODEF::_next_council_meeting_turn), 1, fp);
        fwrite(&MOX::_antaran_resource_level, sizeof(MOX::_antaran_resource_level), 1, fp);
        fwrite(&MOX::_chance_for_antaran_invasion, sizeof(MOX::_chance_for_antaran_invasion), 1, fp);
        fwrite(&MOX::_disallow_saves_code, sizeof(MOX::_disallow_saves_code), 1, fp);
        fwrite(&EVENTS::_last_event_year, sizeof(EVENTS::_last_event_year), 1, fp);
        fwrite(&EVENTS::_event_delay_count, sizeof(EVENTS::_event_delay_count), 1, fp);
        fwrite(&MOX::_antarans, sizeof(MOX::_antarans), 1, fp);

        RUSS::Mox_Update_();
        fwrite(&MOX::_cur_map_scale, sizeof(MOX::_cur_map_scale), 1, fp);
        fwrite(&MOX::_cur_map_x, sizeof(MOX::_cur_map_x), 1, fp);
        fwrite(&MOX::_cur_map_y, sizeof(MOX::_cur_map_y), 1, fp);
        fwrite(&save_padding, sizeof(save_padding), 1, fp);

        fclose(fp);

        HAROLD::Restore_Mouse_List_();
        RUSS::Assert_Settings_();
        FILEDEF::Save_Game_Settings_();
    }

    uint32_t __cdecl Load_Game_(int16_t slot_index) {
        char filename[256];
        char error_message[128] = {};
        uint32_t header_val;
        uint32_t random_seed;
        s_settings saved_settings;
        uint8_t save_padding[1000];

        HAROLD::Save_Mouse_List_();

        sprintf(filename, "SAVE%d.GAM", slot_index + 1);

        RUSS::Mox_Update_();
        RUSS::Assert_Settings_();

        FILE* file = fopen(filename, "rb");
        if (file == nullptr) {
            RUSS::PFMT_(0, 0, "File read error");
            assert::hgetch_();
            return 0;
        }

        fread(&header_val, sizeof(header_val), 1, file);
        if (header_val != 0xe0) {
            const char* fmt = nullptr;
            switch (MOX::_settings.language) {
            case 0: fmt = "SAVE%d.GAM is invalid! Try another save slot or start a NEW game"; break;
            case 1: fmt = "SAVE%d.GAM ist ung\x9Fltig! W\x8Ahlen Sie ein anderes Spiel, oder starten Sie ein NEUES Spiel."; break;
            case 2: fmt = "SAUVEGARDER%d.GAM n'est pas valide! Essayez un autre emplacement de sauvegarde ou commencez une NOUVELLE partie"; break;
            case 3: fmt = "#SAVE%d.GAM no es v<lido! Prueba otro hueco para guardar or comienza una partida NUEVA"; break;
            case 4: fmt = "Il file SAVE%d.GAM non > valido! Provate a caricarne un altro o iniziate una nuova partita"; break;
            }
            if (fmt) {
                sprintf(error_message, fmt, slot_index + 1);
            }
            GENDRAW::Warning_Box_(error_message);
            video::Set_Page_Off_();
            graphics::Fill_(0, 0, 639, 479, 0);
            fclose(file);
            return 0;
        }

        MISC::Increment_Mouse_Clock_Image_();

        int16_t description_slot_index = (slot_index >= 9) ? 9 : slot_index;
        fread(MOX::_save_game_description[description_slot_index].description, sizeof(MOX::_save_game_description[description_slot_index].description), 1, file);
        fread(&MOX::_stardate, sizeof(MOX::_stardate), 1, file);
        fread(&MOX::_game_type, sizeof(MOX::_game_type), 1, file);
        fread(&saved_settings, sizeof(saved_settings), 1, file);
        fread(&random_seed, sizeof(random_seed), 1, file);

        RUSS::Mox_Update_();
        fread(&MOX::_NUM_COLONIES, sizeof(MOX::_NUM_COLONIES), 1, file);
        for (int i = 0; i < MAX_COLONIES; ++i) {
            fread(&MOX::_colony[i], sizeof(s_colony), 1, file);
        }

        RUSS::Mox_Update_();
        fread(&MOX::_planet_count, sizeof(MOX::_planet_count), 1, file);
        for (int i = 0; i < MAX_PLANETS; ++i) {
            fread(&MOX::_planet[i], sizeof(s_planet_data), 1, file);
        }

        RUSS::Mox_Update_();
        fread(&MOX::_NUM_STARS, sizeof(MOX::_NUM_STARS), 1, file);
        for (int i = 0; i < MAX_STARS; ++i) {
            fread(&MOX::_star[i], sizeof(s_star_data), 1, file);
        }

        RUSS::Mox_Update_();
        for (int i = 0; i < MAX_LEADERS; ++i) {
            fread(&MOX::_leaders[i], sizeof(s_leader_data), 1, file);
        }

        RUSS::Mox_Update_();
        fread(&MOX::_NUM_PLAYERS, sizeof(MOX::_NUM_PLAYERS), 1, file);
        for (int i = 0; i < MAX_PLAYERS; ++i) {
            fread(&MOX::_player[i], sizeof(s_player), 1, file);
        }

        RUSS::Mox_Update_();
        fread(&MOX::_NUM_SHIPS, sizeof(MOX::_NUM_SHIPS), 1, file);
        for (int i = 0; i < MAX_SHIPS; ++i) {
            fread(&MOX::_ship[i], sizeof(s_ship_data), 1, file);
        }

        fread(&MOX::_moveable_box, sizeof(MOX::_moveable_box[0]), 5, file);
        fread(&MOX::_moveable_box_order, sizeof(MOX::_moveable_box_order[0]), 5, file);
        fread(&MOX::_PLAYER_NUM, sizeof(MOX::_PLAYER_NUM), 1, file);
        fread(&MOX::_max_map_scale, sizeof(MOX::_max_map_scale), 1, file);
        fread(&MOX::_max_zoom_count, sizeof(MOX::_max_zoom_count), 1, file);
        fread(&MOX::_star_bg_index, sizeof(MOX::_star_bg_index), 1, file);
        fread(&MOX::_MAP_MAX_X, sizeof(MOX::_MAP_MAX_X), 1, file);
        fread(&MOX::_MAP_MAX_Y, sizeof(MOX::_MAP_MAX_Y), 1, file);

        RUSS::Mox_Update_();
        fread(&MOX::_fleet_icon_button_count, sizeof(MOX::_fleet_icon_button_count), 1, file);
        fread(MOX::_nebula, sizeof(*MOX::_nebula), MAX_NEBULAS, file);
        fread(&MOX::_NUM_NEBULAS, sizeof(MOX::_NUM_NEBULAS), 1, file);

        if (MOX::_nebula_screen_seg == 0) {
            MOX::_NUM_NEBULAS = 0;
        }

        fread(&MOX::_bill_savegame, sizeof(MOX::_bill_savegame), 1, file);
        RUSS::Mox_Update_();

        fread(&REPORT::_n_reports, sizeof(REPORT::_n_reports), 1, file);
        fread(&REPORT::_reports, sizeof(REPORT::_reports), 1, file);
        fread(&REPORT::_done_player_reports, sizeof(REPORT::_done_player_reports), 1, file);
        fread(&REPORT::_something_interesting_happened, sizeof(REPORT::_something_interesting_happened), 1, file);
        fread(&REPORT::_done_colonize_check, sizeof(REPORT::_done_colonize_check), 1, file);
        fread(&EVENTS::_event_data, sizeof(EVENTS::_event_data[0]), 36, file);

        RUSS::Mox_Update_();
        fread(&MSG_::_msgs, sizeof(MSG_::_msgs[0]), 100, file);
        fread(&MSG_::_first_msg, sizeof(MSG_::_first_msg), 1, file);
        fread(&MSG_::_max_msgs, sizeof(MSG_::_max_msgs), 1, file);
        fread(&MSG_::_n_msgs, sizeof(MSG_::_n_msgs), 1, file);
        fread(&REPORT::_displayed_msgs, sizeof(REPORT::_displayed_msgs), 1, file);
        fread(&REPORT::_displayed_gnn, sizeof(REPORT::_displayed_gnn), 1, file);
        fread(&diplodef::_COUNCIL_FLAG, sizeof(diplodef::_COUNCIL_FLAG), 1, file);
        fread(&diplodef::_next_council_meeting_turn, sizeof(diplodef::_next_council_meeting_turn), 1, file);

        RUSS::Mox_Update_();
        fread(&MOX::_antaran_resource_level, sizeof(MOX::_antaran_resource_level), 1, file);
        fread(&MOX::_chance_for_antaran_invasion, sizeof(MOX::_chance_for_antaran_invasion), 1, file);
        fread(&MOX::_disallow_saves_code, sizeof(MOX::_disallow_saves_code), 1, file);
        fread(&EVENTS::_last_event_year, sizeof(EVENTS::_last_event_year), 1, file);
        fread(&EVENTS::_event_delay_count, sizeof(EVENTS::_event_delay_count), 1, file);
        fread(&MOX::_antarans, sizeof(MOX::_antarans), 1, file);

        RUSS::Mox_Update_();
        fread(&MOX::_cur_map_scale, sizeof(MOX::_cur_map_scale), 1, file);
        fread(&MOX::_cur_map_x, sizeof(MOX::_cur_map_x), 1, file);
        fread(&MOX::_cur_map_y, sizeof(MOX::_cur_map_y), 1, file);
        fread(&save_padding, sizeof(save_padding), 1, file);
        fclose(file);

        MOX2::Init_Map_Defaults_();
        game_random::Set_Random_Seed_(random_seed);
        RUSS::Mox_Update_();
        BILL::Cache_Player_Scientist_Anim_();
        RUSS::Mox_Update_();

        if (MOX::_NUM_NEBULAS != 0) {
            MAPGEN::Load_Nebula_Pictures_();
        }

        MAINSCR::Update_Ship_Icons_In_Between_Turns_();
        if (MOVEBOX::Moveable_Box_Selected_(2)) {
            FLEETPOP::Reload_Fleet_Box_Data_();
        }

        COLCALC::No_Op1_();
        MOX::Apply_Save_Settings(&saved_settings);
        HAROLD::Confirm_Planet_And_Star_Index_Link_("loading a game");
        RUSS::Assert_Settings_();
        MAINSCR::Update_Star_Owners_();
        RUSS::Assert_Settings_();
        HAROLD::Load_Leader_Titles_();

        for (int i = 0; i < MAX_PLAYERS; ++i) {
            if (MOX::_player[i].dead_field != 0) {
                MOX::_player[i].research_accumulated = (uint16_t)MOX::_player[i].dead_field;
                MOX::_player[i].dead_field = 0;
            }
        }

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            for (int j = 0; j < MOX::_NUM_PLAYERS; ++j) {
                MOX::_player[i].delayed_diplomacy_orders[j] = -1;
            }
        }

        HAROLD::Restore_Mouse_List_();
        RUSS::Release_All_Blocks_(MOX::_screen_seg);

        return 1;
    }

    void __cdecl Save_Moo_Custom_Player_Data_(s_player* player) {
    }

    bool __cdecl Read_Moo_Custom_Player_Data_(s_player* player) {
        return false;
    }

    void __cdecl Get_Saved_Game_Descriptions_(int32_t* save_times) {
        for (int16_t i = 0; i < 10; ++i) {
            char num_str[8];
            char filename[16];

            itoa(i + 1, num_str, 10);

            strcpy(filename, "SAVE");
            strcat(filename, num_str);
            strcat(filename, ".GAM");

            if (File_Exists_(filename)) {
                FILE* fp = fopen(filename, "rb");
                if (fp != nullptr) {
                    int32_t type;
                    fread(&type, sizeof(type), 1, fp);
                    fread(MOX::_save_game_description[i].description, sizeof(MOX::_save_game_description[i].description), 1, fp);
                    fread(&save_times[i], sizeof(save_times[i]), 1, fp);

                    if (LOADSAVE::Is_Valid_Game_Type_(type)) {
                        if (i == 9) {
                            strcpy(MOX::_save_game_description[i].description, "(Auto Save)");
                        } else if (MOX::_save_game_description[i].description[0] == '\0') {
                            strcpy(MOX::_save_game_description[i].description, "<< no description >>");
                        }
                    } else {
                        save_times[i] = -1;
                        strcpy(MOX::_save_game_description[i].description, HAROLD::H_Message_(0x184));
                    }
                    fclose(fp);
                }
            } else {
                strcpy(MOX::_save_game_description[i].description, HAROLD::H_Message_(0x184));
                save_times[i] = -1;
            }
        }
    }

    int8_t __cdecl Get_Game_Type_From_Saved_Game_(int16_t save_slot) {
        int8_t game_type = -1;
        char filename[16];

        sprintf(filename, "SAVE%d.GAM", save_slot + 1);
        FILE* file = fopen(filename, "rb");
        if (file != nullptr) {
            if (fseek(file, 0x2D, SEEK_SET) == 0) {
                fread(&game_type, sizeof(game_type), 1, file);
            }
            fclose(file);
        }
        return game_type;
    }

    void Update_Game_Settings_Version_Number_() {
        bool needs_reset = false;
        FILE* file = fopen("MOX.SET", "rb");

        if (file != nullptr) {
            fclose(file);

            Load_Game_Settings_();

            // Set version to 0x82 (130)
            MOX::_settings.version = 0x82;

            // If saving fails, we reset settings
            if (!Save_Game_Settings_()) {
                needs_reset = true;
            }
        }
        else {
            // If settings file doesn't exist, we must reset
            needs_reset = true;
        }

        if (needs_reset) {
            INITGAME::Set_Default_Game_Settings_();
        }
    }
}