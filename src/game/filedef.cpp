#include "pch.h"

#include "game/savegame.h"

namespace FILEDEF {
    const char s_Auto_Save_00556938[12] = "(Auto Save)";

    void __cdecl Load_Game_Settings_() {
        RUSS::Assert_Settings_();

        FILE* file = fopen_case("MOX.SET", "rb");
        if (file != nullptr) {
            fread(&MOX::_settings, sizeof(MOX::_settings), 1, file);
            fclose(file);
            RUSS::Assert_Settings_();
        }

        HAROLD::Set_Sound_Volume_To_Settings_();
    }

    bool Save_Game_Settings_(void) {
        RUSS::Assert_Settings_();

        FILE* file = fopen_case("MOX.SET", "wb");
        if (file == nullptr) {
            return false;
        }

        fwrite(&MOX::_settings, sizeof(MOX::_settings), 1, file);
        fclose(file);

        RUSS::Assert_Settings_();

        return true;
    }


    void __cdecl Save_Game_(int16_t slot_index) {
        uint32_t random_seed = game_random::Get_Random_Seed_();

        HAROLD::Save_Mouse_List_();

        char slot_str[20];
        snprintf(slot_str, sizeof(slot_str), "%i", slot_index + 1);

        char filename[260];
        strlcpy(filename, "SAVE", sizeof(filename));
        strlcat(filename, slot_str, sizeof(filename));
        strlcat(filename, ".GAM", sizeof(filename));

        RUSS::Assert_Settings_();

        uint8_t* save_buffer = static_cast<uint8_t*>(malloc(SAVEGAME::BUFFER_CAPACITY));
        if (save_buffer == nullptr) {
            char error_message[] = "Error saving game.\n";
            GENDRAW::Message_Box_(error_message);
            HAROLD::Restore_Mouse_List_();
            return;
        }

        int16_t description_slot_index = (slot_index > 8) ? 9 : slot_index;
        SERIAL::Writer_ writer(save_buffer, SAVEGAME::BUFFER_CAPACITY, "Save game buffer overflow");
        SAVEGAME::Write_Game_(&writer, MOX::_save_game_description[description_slot_index].description, random_seed);

        FILE* fp = fopen_case(filename, "wb");

        if (fp == nullptr) {
            char error_message[] = "Error saving game.\n";
            GENDRAW::Message_Box_(error_message);
            free(save_buffer);
            HAROLD::Restore_Mouse_List_();
            return;
        }

        MISC::Increment_Mouse_Clock_Image_();
        fwrite(save_buffer, writer.Length_(), 1, fp);
        fclose(fp);
        free(save_buffer);

        HAROLD::Restore_Mouse_List_();
        RUSS::Assert_Settings_();
        FILEDEF::Save_Game_Settings_();
    }

    uint32_t __cdecl Load_Game_(int16_t slot_index) {
        char filename[256];
        char error_message[128] = {};
        uint32_t random_seed;
        s_settings saved_settings;

        HAROLD::Save_Mouse_List_();

        snprintf(filename, sizeof(filename), "SAVE%d.GAM", slot_index + 1);

        RUSS::Mox_Update_();
        RUSS::Assert_Settings_();

        FILE* file = fopen_case(filename, "rb");
        if (file == nullptr) {
            RUSS::PFMT_(0, 0, "File read error");
            key::Wait_For_Key_();
            return 0;
        }

        if (fseek(file, 0, SEEK_END) != 0) {
            fclose(file);
            return 0;
        }
        long file_size_long = ftell(file);
        if (file_size_long <= 0 || file_size_long > static_cast<long>(SAVEGAME::BUFFER_CAPACITY) ||
            fseek(file, 0, SEEK_SET) != 0) {
            fclose(file);
            return 0;
        }

        uint32_t file_size = static_cast<uint32_t>(file_size_long);
        uint8_t* save_buffer = static_cast<uint8_t*>(malloc(file_size));
        if (save_buffer == nullptr || fread(save_buffer, file_size, 1, file) != 1) {
            free(save_buffer);
            fclose(file);
            return 0;
        }
        fclose(file);

        SERIAL::Reader_ reader(save_buffer, file_size, "Invalid or truncated save game");
        SAVEGAME::s_metadata metadata = {};
        if (!SAVEGAME::Read_Game_(&reader, &metadata, &saved_settings, &random_seed)) {
            const char* fmt = nullptr;
            switch (MOX::_settings.language) {
            case 0: fmt = "SAVE%d.GAM is invalid! Try another save slot or start a NEW game"; break;
            case 1: fmt = "SAVE%d.GAM ist ung\x9Fltig! W\x8Ahlen Sie ein anderes Spiel, oder starten Sie ein NEUES Spiel."; break;
            case 2: fmt = "SAUVEGARDER%d.GAM n'est pas valide! Essayez un autre emplacement de sauvegarde ou commencez une NOUVELLE partie"; break;
            case 3: fmt = "#SAVE%d.GAM no es v<lido! Prueba otro hueco para guardar or comienza una partida NUEVA"; break;
            case 4: fmt = "Il file SAVE%d.GAM non > valido! Provate a caricarne un altro o iniziate una nuova partita"; break;
            }
            if (fmt) {
                snprintf(error_message, sizeof(error_message), fmt, slot_index + 1);
            }
            GENDRAW::Warning_Box_(error_message);
            video::Set_Page_Off_();
            graphics::Fill_(0, 0, 639, 479, 0);
            free(save_buffer);
            return 0;
        }
        free(save_buffer);

        MISC::Increment_Mouse_Clock_Image_();

        int16_t description_slot_index = (slot_index >= 9) ? 9 : slot_index;
        memcpy(MOX::_save_game_description[description_slot_index].description, metadata.description, sizeof(metadata.description));
        MOX::_stardate = metadata.stardate;
        MOX::_game_type = metadata.game_type;

        if (MOX::_nebula_screen_seg == 0) {
            MOX::_NUM_NEBULAS = 0;
        }

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

            snprintf(num_str, sizeof(num_str), "%i", i + 1);

            strlcpy(filename, "SAVE", sizeof(filename));
            strlcat(filename, num_str, sizeof(filename));
            strlcat(filename, ".GAM", sizeof(filename));

            if (File_Exists_(filename)) {
                FILE* fp = fopen_case(filename, "rb");
                if (fp != nullptr) {
                    int32_t type;
                    fread(&type, sizeof(type), 1, fp);
                    fread(MOX::_save_game_description[i].description, sizeof(MOX::_save_game_description[i].description), 1, fp);
                    fread(&save_times[i], sizeof(save_times[i]), 1, fp);

                    if (LOADSAVE::Is_Valid_Game_Type_(type)) {
                        if (i == 9) {
                            strlcpy(MOX::_save_game_description[i].description, "(Auto Save)", sizeof(MOX::_save_game_description[i].description));
                        } else if (MOX::_save_game_description[i].description[0] == '\0') {
                            strlcpy(MOX::_save_game_description[i].description, "<< no description >>", sizeof(MOX::_save_game_description[i].description));
                        }
                    } else {
                        save_times[i] = -1;
                        strlcpy(MOX::_save_game_description[i].description, HAROLD::H_Message_(0x184), sizeof(MOX::_save_game_description[i].description));
                    }
                    fclose(fp);
                }
            } else {
                strlcpy(MOX::_save_game_description[i].description, HAROLD::H_Message_(0x184), sizeof(MOX::_save_game_description[i].description));
                save_times[i] = -1;
            }
        }
    }

    int8_t __cdecl Get_Game_Type_From_Saved_Game_(int16_t save_slot) {
        int8_t game_type = -1;
        char filename[16];

        snprintf(filename, sizeof(filename), "SAVE%d.GAM", save_slot + 1);
        FILE* file = fopen_case(filename, "rb");
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
        FILE* file = fopen_case("MOX.SET", "rb");

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