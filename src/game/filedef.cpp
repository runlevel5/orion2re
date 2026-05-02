#include "pch.h"

namespace FILEDEF {
    void __cdecl Load_Game_Settings_() {
        RUSS::Assert_Settings_();

        FILE* file = FID_conflict_wfopen("MOX.SET", "rb");
        if (file != nullptr) {
            // Read 553 bytes (size of s_settings up to version/seed block)
            fread(&MOX::_settings, 0x229, 1, file);
            fclose(file);
            RUSS::Assert_Settings_();
        }

        HAROLD::Set_Sound_Volume_To_Settings_();
    }

    bool Save_Game_Settings_(void) {
        RUSS::Assert_Settings_();

        // Open settings file in binary write mode
        FILE* file = FID_conflict_wfopen("MOX.SET", "wb");
        if (file == nullptr) {
            return false;
        }

        // Write the settings structure (size 0x229 = 553 bytes)
        fwrite(&MOX::_settings, 0x229, 1, file);
        fclose(file);

        // Re-assert state after saving
        RUSS::Assert_Settings_();

        return true;
    }


    void __cdecl Save_Game_(short slot) {
        uint32_t random_seed = random::Get_Random_Seed_();
        uint32_t padding[250];

        for (int i = 0; i < 250; ++i) {
            padding[i] = 0;
        }

        HAROLD::Save_Mouse_List_();

        char slot_str[20];
        itoa(slot + 1, slot_str, 10);

        char filename[260];
        const char* prefix = reinterpret_cast<const char*>(0x005568FC); // "SAVE"
        const char* suffix = reinterpret_cast<const char*>(0x005568F4); // ".GAM"

        char* dst = filename;
        const char* src = prefix;
        while ((*dst++ = *src++) != '\0');
        dst--;
        src = slot_str;
        while ((*dst++ = *src++) != '\0');
        dst--;
        src = suffix;
        while ((*dst++ = *src++) != '\0');

        RUSS::Assert_Settings_();

        FILE* fp = fopen(filename, s_wb_00551178); // "wb"

        if (fp == nullptr) {
            char error_msg[20];
            const char* err_src = reinterpret_cast<const char*>(0x00556904); // "Error saving game."
            for (int i = 0; i < 5; ++i) {
                ((uint32_t*)error_msg)[i] = ((const uint32_t*)err_src)[i];
            }
            GENDRAW::Message_Box_(error_msg);
            return;
        }

        uint32_t save_header = 0xe0;
        MISC::Increment_Mouse_Clock_Image_();
        fwrite(&save_header, 4, 1, fp);

        int desc_slot = (slot > 8) ? 9 : (int)slot;
        fwrite(&MOX::_save_game_description[desc_slot], 37, 1, fp);
        fwrite(&MOX::_stardate, 4, 1, fp);
        fwrite(&MOX::_game_type, 1, 1, fp);
        fwrite(&MOX::_settings, 553, 1, fp); // 0x229 bytes

        RUSS::Mox_Update_();
        fwrite(&random_seed, 4, 1, fp);
        fwrite(&MOX::_NUM_COLONIES, 2, 1, fp);
        fwrite(MOX::_colony, sizeof(s_colony), MAX_COLONIES, fp); // 0x169 per item

        RUSS::Mox_Update_();
        fwrite(&MOX::_planet_count, 2, 1, fp);
        fwrite(MOX::_planet, sizeof(s_planet_data), MAX_PLANETS, fp); // 0x11 per item

        RUSS::Mox_Update_();
        fwrite(&MOX::_NUM_STARS, 2, 1, fp);
        fwrite(MOX::_star, sizeof(s_star_data), MAX_STARS, fp); // 0x71 per item

        RUSS::Mox_Update_();
        fwrite(MOX::_leaders, sizeof(s_leader_data), MAX_LEADERS, fp); // 0x3b per item

        RUSS::Mox_Update_();
        fwrite(&MOX::_NUM_PLAYERS, 2, 1, fp);
        fwrite(MOX::_player, sizeof(s_player), MAX_PLAYERS, fp); // 0xea9 per item

        RUSS::Mox_Update_();
        fwrite(&MOX::_NUM_SHIPS, 2, 1, fp);
        fwrite(MOX::_ship, sizeof(s_ship_data), MAX_SHIPS, fp); // 0x81 per item

        RUSS::Mox_Update_();
        fwrite(&MOX::_moveable_box, 28, 5, fp); // 0x1c per item
        fwrite(&MOX::_moveable_box_order, 2, 5, fp);
        fwrite(&MOX::_PLAYER_NUM, 2, 1, fp);
        fwrite(&MOX::_max_map_scale, 2, 1, fp);
        fwrite(&MOX::_max_zoom_count, 2, 1, fp);

        RUSS::Mox_Update_();
        fwrite(&MOX::_star_bg_index, 1, 1, fp);
        fwrite(&MOX::_MAP_MAX_X, 2, 1, fp);
        fwrite(&MOX::_MAP_MAX_Y, 2, 1, fp);
        fwrite(&MOX::_fleet_icon_button_count, 2, 1, fp);
        fwrite(MOX::_nebula, 20, 1, fp); // 0x14 size
        fwrite(&MOX::_NUM_NEBULAS, 1, 1, fp);
        fwrite(&MOX::_bill_savegame, 12, 1, fp); // 0xc size

        RUSS::Mox_Update_();
        fwrite(&REPORT::_n_reports, 2, 1, fp);
        fwrite(&REPORT::_reports, 950, 1, fp); // 0x3b6 size
        fwrite(&REPORT::_done_player_reports, 1, 1, fp);
        fwrite(&REPORT::_something_interesting_happened, 8, 1, fp);
        fwrite(&REPORT::_done_colonize_check, 9, 1, fp);
        fwrite(&EVENTS::_event_data, 9, 36, fp);

        RUSS::Mox_Update_();
        fwrite(&MSG_::_msgs, 18, 100, fp); // 0x12 per item
        fwrite(&MSG_::_first_msg, 8, 1, fp);
        fwrite(&MSG_::_max_msgs, 8, 1, fp);
        fwrite(&MSG_::_n_msgs, 8, 1, fp);
        fwrite(&REPORT::_displayed_msgs, 1, 1, fp);
        fwrite(&REPORT::_displayed_gnn, 1, 1, fp);

        RUSS::Mox_Update_();
        fwrite(&diplodef::_COUNCIL_FLAG, 2, 1, fp);
        fwrite(&diplodef::_next_council_meeting_turn, 2, 1, fp);
        fwrite(&MOX::_antaran_resource_level, 2, 1, fp);
        fwrite(&MOX::_chance_for_antaran_invasion, 2, 1, fp);
        fwrite(&MOX::_disallow_saves_code, 2, 1, fp);
        fwrite(&EVENTS::_last_event_year, 4, 1, fp);
        fwrite(&EVENTS::_event_delay_count, 2, 1, fp);
        fwrite(&MOX::_antarans, 66, 1, fp);

        RUSS::Mox_Update_();
        fwrite(&MOX::_cur_map_scale, 2, 1, fp);
        fwrite(&MOX::_cur_map_x, 2, 1, fp);
        fwrite(&MOX::_cur_map_y, 2, 1, fp);
        fwrite(&padding, 1000, 1, fp);

        fclose(fp);

        HAROLD::Restore_Mouse_List_();
        RUSS::Assert_Settings_();
        FILEDEF::Save_Game_Settings_();
    }

    uint32_t __cdecl Load_Game_(int16_t save_idx) {
        char filename[256];
        char err_msg[128];
        uint32_t header_val;
        uint32_t random_seed;
        s_settings saved_settings;
        uint8_t buffer_1000[1000];

        HAROLD::Save_Mouse_List_();

        _sprintf(filename, "SAVE%d.GAM", save_idx + 1);

        RUSS::Mox_Update_();
        RUSS::Assert_Settings_();

        FILE* file = fopen(filename, "rb");
        if (file == nullptr) {
            RUSS::PFMT_(0, 0, "File read error");
            hgetch_();
            return 0;
        }

        fread(&header_val, 4, 1, file);
        if (header_val != 0xe0) {
            const char* fmt = nullptr;
            // TODO: Fix me
            switch (MOX::_settings.language) {
            case 0: fmt = "SAVE%d.GAM is invalid! Try another."; break;
            case 1: fmt = "SAVE%d.GAM ist ungültig! Wählen Sie..."; break;
            case 2: fmt = "SAUVEGARDER%d.GAM n'est pas valide..."; break;
            case 3: fmt = "#SAVE%d.GAM no es válido! Prueba..."; break;
            case 4: fmt = "Il file SAVE%d.GAM non è valido!"; break;
            }
            if (fmt) {
                _sprintf(err_msg, fmt, save_idx + 1);
                GENDRAW::Warning_Box_(err_msg);
            }
            video::Set_Page_Off_();
            graphics::Fill_(0, 0, 639, 479, 0);
            fclose(file);
            return 0;
        }

        MISC::Increment_Mouse_Clock_Image_();

        int desc_idx = (save_idx > 9) ? 9 : (int)save_idx;
        fread(&MOX::_save_game_description[desc_idx], 37, 1, file);
        fread(&MOX::_stardate, 4, 1, file);
        fread(&MOX::_game_type, 1, 1, file);
        fread(&saved_settings, 0x229, 1, file);
        fread(&random_seed, 4, 1, file);

        RUSS::Mox_Update_();
        fread(&MOX::_NUM_COLONIES, 2, 1, file);
        for (int i = 0; i < MAX_COLONIES; ++i) {
            fread(&MOX::_colony[i], sizeof(s_colony), 1, file);
        }

        RUSS::Mox_Update_();
        fread(&MOX::_planet_count, 2, 1, file);
        for (int i = 0; i < MAX_PLANETS; ++i) {
            fread(&MOX::_planet[i], sizeof(s_planet_data), 1, file);
        }

        RUSS::Mox_Update_();
        fread(&MOX::_NUM_STARS, 2, 1, file);
        for (int i = 0; i < MAX_STARS; ++i) {
            fread(&MOX::_star[i], sizeof(s_star_data), 1, file);
        }

        RUSS::Mox_Update_();
        for (int i = 0; i < MAX_LEADERS; ++i) {
            fread(&MOX::_leaders[i], sizeof(s_leader_data), 1, file);
        }

        RUSS::Mox_Update_();
        fread(&MOX::_NUM_PLAYERS, 2, 1, file);
        for (int i = 0; i < MAX_PLAYERS; ++i) {
            fread(&MOX::_player[i], sizeof(s_player), 1, file);
        }

        fread(&MOX::_NUM_SHIPS, 2, 1, file);
        for (int i = 0; i < MAX_SHIPS; ++i) {
            fread(&MOX::_ship[i], sizeof(s_ship_data), 1, file);
        }

        fread(&MOX::_moveable_box, 0x1c, 5, file);
        fread(&MOX::_moveable_box_order, 2, 5, file);
        fread(&MOX::_PLAYER_NUM, 2, 1, file);
        fread(&MOX::_max_map_scale, 2, 1, file);
        fread(&MOX::_max_zoom_count, 2, 1, file);
        fread(&MOX::_star_bg_index, 1, 1, file);
        fread(&MOX::_MAP_MAX_X, 2, 1, file);
        fread(&MOX::_MAP_MAX_Y, 2, 1, file);

        RUSS::Mox_Update_();
        fread(&MOX::_fleet_icon_button_count, 2, 1, file);
        fread(MOX::_nebula, 0x14, 1, file);
        fread(&MOX::_NUM_NEBULAS, 1, 1, file);

        if (MOX::_nebula_screen_seg == 0) {
            MOX::_NUM_NEBULAS = 0;
        }

        fread(&MOX::_bill_savegame, 0xc, 1, file);
        RUSS::Mox_Update_();

        fread(&REPORT::_n_reports, 2, 1, file);
        fread(&REPORT::_reports, 0x3b6, 1, file);
        fread(&REPORT::_done_player_reports, 1, 1, file);
        fread(&REPORT::_something_interesting_happened, 8, 1, file);
        fread(&REPORT::_done_colonize_check, 9, 1, file);
        fread(&EVENTS::_event_data, 9, 36, file);

        RUSS::Mox_Update_();
        fread(&MSG_::_msgs, 0x12, 100, file);
        fread(&MSG_::_first_msg, 8, 1, file);
        fread(&MSG_::_max_msgs, 8, 1, file);
        fread(&MSG_::_n_msgs, 8, 1, file);
        fread(&REPORT::_displayed_msgs, 1, 1, file);
        fread(&REPORT::_displayed_gnn, 1, 1, file);
        fread(&diplodef::_COUNCIL_FLAG, 2, 1, file);
        fread(&diplodef::_next_council_meeting_turn, 2, 1, file);

        RUSS::Mox_Update_();
        fread(&MOX::_antaran_resource_level, 2, 1, file);
        fread(&MOX::_chance_for_antaran_invasion, 2, 1, file);
        fread(&MOX::_disallow_saves_code, 2, 1, file);
        fread(&EVENTS::_last_event_year, 4, 1, file);
        fread(&EVENTS::_event_delay_count, 2, 1, file);
        fread(&MOX::_antarans, 66, 1, file);
        fread(&MOX::_cur_map_scale, 2, 1, file);
        fread(&MOX::_cur_map_x, 2, 1, file);
        fread(&MOX::_cur_map_y, 2, 1, file);
        fread(&buffer_1000, 1000, 1, file);
        fclose(file);

        MOX2::Init_Map_Defaults_();
        random::Set_Random_Seed_(random_seed);
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
        HAROLD::Load_Leader_Titles();

        for (int i = 0; i < 8; ++i) {
            if (MOX::_player[i].dead_field != 0) {
                MOX::_player[i].research_accumulated = (int32_t)MOX::_player[i].dead_field;
                MOX::_player[i].dead_field = 0;
            }
        }

        for (int i = 0; i < MOX::_NUM_PLAYERS; ++i) {
            for (int j = 0; j < 8; ++j) {
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

            strcpy(filename, s_SAVE_005568fc);
            strcat(filename, num_str);
            strcat(filename, s_GAM_005568f4);

            if (accss::_access_(filename, 0) == 0) {
                FILE* fp = fopen(filename, s_rb_0055117c);
                if (fp != nullptr) {
                    int32_t type;
                    fread(&type, 4, 1, fp);
                    fread(MOX::_save_game_description[i].description, 37, 1, fp);
                    fread(&save_times[i], 4, 1, fp);

                    if (LOADSAVE::Is_Valid_Game_Type_(type)) {
                        if (i == 9) {
                            strcpy(MOX::_save_game_description[i].description, s_Auto_Save_00556938);
                        } else if (MOX::_save_game_description[i].description[0] == '\0') {
                            strcpy(MOX::_save_game_description[i].description, s_no_description_00556920);
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

        _sprintf(filename, s_SAVE_d_GAM_00556944, save_slot + 1);
        FILE* file = fopen(filename, s_rb_0055117c);
        if (file != nullptr) {
            if (fseek(file, 0x2D, SEEK_SET) == 0) {
                fread(&game_type, 1, 1, file);
            }
            fclose(file);
        }
        return game_type;
    }

    void Update_Game_Settings_Version_Number_() {
        bool needs_reset = false;
        FILE* file = FID_conflict_wfopen("MOX.SET", "rb");

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