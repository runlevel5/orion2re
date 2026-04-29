#include "pch.h"

namespace ESTRINGS {

void __cdecl Load_E_Strings_() {
    char* loaded_data;
    int8_t lang = MOX::_settings.language;

    if (lang == 0) {
        loaded_data = (char*)farload::Farload_Data_Static_(s_estrings_lbx, 0, _string_data, 0, 1, 21000);
    }
    else if (lang == 1) {
        loaded_data = (char*)farload::Farload_Data_Static_(s_estrGERM_lbx, 0, _string_data, 0, 1, 21000);
    }
    else if (lang == 2) {
        loaded_data = (char*)farload::Farload_Data_Static_(s_estrFREN_lbx, 0, _string_data, 0, 1, 21000);
    }
    else if (lang == 3) {
        loaded_data = (char*)farload::Farload_Data_Static_(s_estrSPAN_lbx, 0, _string_data, 0, 1, 21000);
    }
    else if (lang == 4) {
        loaded_data = (char*)farload::Farload_Data_Static_(s_estrITAL_lbx, 0, _string_data, 0, 1, 21000);
    }
    else if (lang == 5) {
        loaded_data = (char*)farload::Farload_Data_Static_(s_estrPOLI_lbx, 0, _string_data, 0, 1, 21000);
    }
    else {
        volatile char* dummy;
        loaded_data = const_cast<char*>(dummy);
    }

    char* current_ptr = loaded_data;
    for (int i = 0; i < 812; ++i) {
        ESTRINGS::_strings[i] = current_ptr;
        current_ptr += strlen(current_ptr) + 1;
    }

    if ((uintptr_t)current_ptr > (uintptr_t)ESTRINGS::_strings) {
        exitm::Exit_With_Message(s_you_blew_the_array_eric);
    }

    Load_H_And_Pro_File_Strings_();

    char* target_str = ESTRINGS::_strings[368];

    if (lang == 1) {
        memcpy(target_str, s_maingerm_lbx, 13);
        return;
    }

    if (lang == 0) {
        memcpy(target_str, s_maintext_lbx, 13);
        return;
    }

    if (lang == 2) {
        memcpy(target_str, s_mainfren_lbx, 13);
        return;
    }

    if (lang == 3) {
        memcpy(target_str, s_mainspan_lbx, 13);
        return;
    }

    if (lang == 4) {
        memcpy(target_str, s_mainital_lbx, 13);
        return;
    }

    if (lang == 5) {
        memcpy(target_str, s_mainpoli_lbx, 13);
    }
}

void __cdecl Load_H_And_Pro_File_Strings_(void) {
    MOX::_personality_labels[0] = ESTRINGS::E_Strings_(0x268);
    MOX::_personality_labels[1] = ESTRINGS::E_Strings_(0x269);
    MOX::_personality_labels[2] = ESTRINGS::E_Strings_(0x26A);
    MOX::_personality_labels[3] = ESTRINGS::E_Strings_(0x26B);
    MOX::_personality_labels[4] = ESTRINGS::E_Strings_(0x26C);
    MOX::_personality_labels[5] = ESTRINGS::E_Strings_(0x26D);
    MOX::_personality_labels[6] = ESTRINGS::E_Strings_(0x26E);

    MOX::_objective_labels[0] = ESTRINGS::E_Strings_(0x26F);
    MOX::_objective_labels[1] = ESTRINGS::E_Strings_(0x270);
    MOX::_objective_labels[2] = ESTRINGS::E_Strings_(0x271);
    MOX::_objective_labels[3] = ESTRINGS::E_Strings_(0x272);
    MOX::_objective_labels[4] = ESTRINGS::E_Strings_(0x273);
    MOX::_objective_labels[5] = ESTRINGS::E_Strings_(0x274);

    MOX::_treaty_labels[0] = ESTRINGS::E_Strings_(0x275);
    MOX::_treaty_labels[1] = ESTRINGS::E_Strings_(0x276);
    MOX::_treaty_labels[2] = ESTRINGS::E_Strings_(0x277);
    MOX::_treaty_labels[3] = ESTRINGS::E_Strings_(0x278);
    MOX::_treaty_labels[4] = ESTRINGS::E_Strings_(0x279);
    MOX::_treaty_labels[5] = ESTRINGS::E_Strings_(0x27A);
    MOX::_treaty_labels[6] = ESTRINGS::E_Strings_(0x27B);

    MOX::_government_labels[0] = ESTRINGS::E_Strings_(0x27C);
    MOX::_government_labels[1] = ESTRINGS::E_Strings_(0x27D);
    MOX::_government_labels[2] = ESTRINGS::E_Strings_(0x27E);
    MOX::_government_labels[3] = ESTRINGS::E_Strings_(0x27F);
    MOX::_government_labels[4] = ESTRINGS::E_Strings_(0x280);
    MOX::_government_labels[5] = ESTRINGS::E_Strings_(0x281);
    MOX::_government_labels[6] = ESTRINGS::E_Strings_(0x282);
    MOX::_government_labels[7] = ESTRINGS::E_Strings_(0x283);

    MOX::_race_names[0] = ESTRINGS::E_Strings_(0x285);
    MOX::_race_names[1] = ESTRINGS::E_Strings_(0x286);
    MOX::_race_names[2] = ESTRINGS::E_Strings_(0x287);
    MOX::_race_names[3] = ESTRINGS::E_Strings_(0x288);
    MOX::_race_names[4] = ESTRINGS::E_Strings_(0x289);
    MOX::_race_names[5] = ESTRINGS::E_Strings_(0x28A);
    MOX::_race_names[6] = ESTRINGS::E_Strings_(0x28B);
    MOX::_race_names[7] = ESTRINGS::E_Strings_(0x28C);
    MOX::_race_names[8] = ESTRINGS::E_Strings_(0x28D);
    MOX::_race_names[9] = ESTRINGS::E_Strings_(0x28E);
    MOX::_race_names[10] = ESTRINGS::E_Strings_(0x28F);
    MOX::_race_names[11] = ESTRINGS::E_Strings_(0x290);
    MOX::_race_names[12] = ESTRINGS::E_Strings_(0x291);
    MOX::_race_names[13] = ESTRINGS::E_Strings_(0x292);
    MOX::_race_names[14] = ESTRINGS::E_Strings_(0x293);
    MOX::_race_names[15] = ESTRINGS::E_Strings_(0x0AD);
    MOX::_race_names[16] = ESTRINGS::E_Strings_(0x0F6);
    MOX::_race_names[17] = ESTRINGS::E_Strings_(0x110);
    MOX::_race_names[18] = ESTRINGS::E_Strings_(0x119);
    MOX::_race_names[19] = ESTRINGS::E_Strings_(0x144);

    MOX::_help_category_labels[0] = ESTRINGS::E_Strings_(0x08E);
    MOX::_help_category_labels[1] = ESTRINGS::E_Strings_(0x294);
    MOX::_help_category_labels[2] = ESTRINGS::E_Strings_(0x295);
    MOX::_help_category_labels[3] = ESTRINGS::E_Strings_(0x296);
    MOX::_help_category_labels[4] = ESTRINGS::E_Strings_(0x297);
    MOX::_help_category_labels[5] = ESTRINGS::E_Strings_(0x298);
    MOX::_help_category_labels[6] = ESTRINGS::E_Strings_(0x299);
    MOX::_help_category_labels[7] = ESTRINGS::E_Strings_(0x29A);
    MOX::_help_category_labels[8] = ESTRINGS::E_Strings_(0x29B);
    MOX::_help_category_labels[9] = ESTRINGS::E_Strings_(0x29C);
    MOX::_help_category_labels[10] = ESTRINGS::E_Strings_(0x29D);
    MOX::_help_category_labels[11] = ESTRINGS::E_Strings_(0x29E);
    MOX::_help_category_labels[12] = ESTRINGS::E_Strings_(0x29F);
    MOX::_help_category_labels[13] = ESTRINGS::E_Strings_(0x2A0);
    MOX::_help_category_labels[14] = ESTRINGS::E_Strings_(0x2A1);
    MOX::_help_category_labels[15] = ESTRINGS::E_Strings_(0x2A2);
    MOX::_help_category_labels[16] = ESTRINGS::E_Strings_(0x2A3);

    MOX::_spectral_class_string[0] = ESTRINGS::E_Strings_(0x2A4);
    MOX::_spectral_class_string[1] = ESTRINGS::E_Strings_(0x2A5);
    MOX::_spectral_class_string[2] = ESTRINGS::E_Strings_(0x2A6);
    MOX::_spectral_class_string[3] = ESTRINGS::E_Strings_(0x2A7);
    MOX::_spectral_class_string[4] = ESTRINGS::E_Strings_(0x2A8);
    MOX::_spectral_class_string[5] = ESTRINGS::E_Strings_(0x2A9);
    MOX::_spectral_class_string[6] = ESTRINGS::E_Strings_(0x2AA);

    MOX::_planet_size_string[0] = ESTRINGS::E_Strings_(0x2AB);
    MOX::_planet_size_string[1] = ESTRINGS::E_Strings_(0x1E0);
    MOX::_planet_size_string[2] = ESTRINGS::E_Strings_(0x173);
    MOX::_planet_size_string[3] = ESTRINGS::E_Strings_(0x168);
    MOX::_planet_size_string[4] = ESTRINGS::E_Strings_(0x143);

    MOX::_mineral_class_string[0] = ESTRINGS::E_Strings_(0x2AC);
    MOX::_mineral_class_string[1] = ESTRINGS::E_Strings_(0x1A7);
    MOX::_mineral_class_string[2] = ESTRINGS::E_Strings_(0x2AD);
    MOX::_mineral_class_string[3] = ESTRINGS::E_Strings_(0x1C2);
    MOX::_mineral_class_string[4] = ESTRINGS::E_Strings_(0x2AE);

    MOX::_planet_gravity_string[0] = ESTRINGS::E_Strings_(0x2AF);
    MOX::_planet_gravity_string[1] = ESTRINGS::E_Strings_(0x2B0);
    MOX::_planet_gravity_string[2] = ESTRINGS::E_Strings_(0x2B1);

    MOX::_planet_atmosphere_string[0] = ESTRINGS::E_Strings_(0x2B2);
    MOX::_planet_atmosphere_string[1] = ESTRINGS::E_Strings_(0x2B3);
    MOX::_planet_atmosphere_string[2] = ESTRINGS::E_Strings_(0x2B4);

    MOX::_planet_group_string[0] = ESTRINGS::E_Strings_(0x2B5);
    MOX::_planet_group_string[1] = ESTRINGS::E_Strings_(0x2B6);
    MOX::_planet_group_string[2] = ESTRINGS::E_Strings_(0x2B7);
    MOX::_planet_group_string[3] = ESTRINGS::E_Strings_(0x2B8);

    MOX::_planet_special_string[0] = ESTRINGS::E_Strings_(0x2B9);
    MOX::_planet_special_string[1] = ESTRINGS::E_Strings_(0x2BA);
    MOX::_planet_special_string[2] = ESTRINGS::E_Strings_(0x2BB);
    MOX::_planet_special_string[3] = ESTRINGS::E_Strings_(0x2BC);
    MOX::_planet_special_string[4] = ESTRINGS::E_Strings_(0x2BD);
    MOX::_planet_special_string[5] = ESTRINGS::E_Strings_(0x2BE);
    MOX::_planet_special_string[6] = ESTRINGS::E_Strings_(0x2BF);
    MOX::_planet_special_string[7] = ESTRINGS::E_Strings_(0x2C0);
    MOX::_planet_special_string[8] = ESTRINGS::E_Strings_(0x2C1);
    MOX::_planet_special_string[9] = ESTRINGS::E_Strings_(0x2C2);
    MOX::_planet_special_string[10] = ESTRINGS::E_Strings_(0x2C3);
    MOX::_planet_special_string[11] = ESTRINGS::E_Strings_(0x2C4);

    MOX::_planet_type_string[0] = ESTRINGS::E_Strings_(0x2C5);
    MOX::_planet_type_string[1] = ESTRINGS::E_Strings_(0x2C6);
    MOX::_planet_type_string[2] = ESTRINGS::E_Strings_(0x2C7);
    MOX::_planet_type_string[3] = ESTRINGS::E_Strings_(0x2C8);
    MOX::_planet_type_string[4] = ESTRINGS::E_Strings_(0x2C9);
    MOX::_planet_type_string[5] = ESTRINGS::E_Strings_(0x2CA);
    MOX::_planet_type_string[6] = ESTRINGS::E_Strings_(0x2CB);
    MOX::_planet_type_string[7] = ESTRINGS::E_Strings_(0x2CC);
    MOX::_planet_type_string[8] = ESTRINGS::E_Strings_(0x2CD);
    MOX::_planet_type_string[9] = ESTRINGS::E_Strings_(0x2CE);

    MOX::_planet_climate_string[0] = ESTRINGS::E_Strings_(0x21B);
    MOX::_planet_climate_string[1] = ESTRINGS::E_Strings_(0x2CF);
    MOX::_planet_climate_string[2] = ESTRINGS::E_Strings_(0x2D0);
    MOX::_planet_climate_string[3] = ESTRINGS::E_Strings_(0x2D1);
    MOX::_planet_climate_string[4] = ESTRINGS::E_Strings_(0x2D2);
    MOX::_planet_climate_string[5] = ESTRINGS::E_Strings_(0x18F);
    MOX::_planet_climate_string[6] = ESTRINGS::E_Strings_(0x1F5);
    MOX::_planet_climate_string[7] = ESTRINGS::E_Strings_(0x0B8);
    MOX::_planet_climate_string[8] = ESTRINGS::E_Strings_(0x2D3);
    MOX::_planet_climate_string[9] = ESTRINGS::E_Strings_(0x12F);

    MOX::_skill_data[0].name = ESTRINGS::E_Strings_(0x2D4);
    MOX::_skill_data[1].name = ESTRINGS::E_Strings_(0x2D5);
    MOX::_skill_data[2].name = ESTRINGS::E_Strings_(0x2D6);
    MOX::_skill_data[3].name = ESTRINGS::E_Strings_(0x2D7);
    MOX::_skill_data[4].name = ESTRINGS::E_Strings_(0x26F);
    MOX::_skill_data[5].name = ESTRINGS::E_Strings_(0x2D8);
    MOX::_skill_data[6].name = ESTRINGS::E_Strings_(0x2D9);
    MOX::_skill_data[7].name = ESTRINGS::E_Strings_(0x2DA);
    MOX::_skill_data[8].name = ESTRINGS::E_Strings_(0x2DB);
    MOX::_skill_data[9].name = ESTRINGS::E_Strings_(0x2DC);
    MOX::_skill_data[10].name = ESTRINGS::E_Strings_(0x2DD);
    MOX::_skill_data[11].name = ESTRINGS::E_Strings_(0x2DE);
    MOX::_skill_data[12].name = ESTRINGS::E_Strings_(0x2DF);
    MOX::_skill_data[13].name = ESTRINGS::E_Strings_(0x2E0);
    MOX::_skill_data[14].name = ESTRINGS::E_Strings_(0x2E1);
    MOX::_skill_data[15].name = ESTRINGS::E_Strings_(0x2E2);
    MOX::_skill_data[16].name = ESTRINGS::E_Strings_(0x2E3);
    MOX::_skill_data[17].name = ESTRINGS::E_Strings_(0x2E4);
    MOX::_skill_data[18].name = ESTRINGS::E_Strings_(0x2E5);
    MOX::_skill_data[19].name = ESTRINGS::E_Strings_(0x2E6);
    MOX::_skill_data[20].name = ESTRINGS::E_Strings_(0x2E7);
    MOX::_skill_data[21].name = ESTRINGS::E_Strings_(0x2E8);
    MOX::_skill_data[22].name = ESTRINGS::E_Strings_(0x2E9);
    MOX::_skill_data[23].name = ESTRINGS::E_Strings_(0x2EA);
    MOX::_skill_data[24].name = ESTRINGS::E_Strings_(0x2EB);
    MOX::_skill_data[25].name = ESTRINGS::E_Strings_(0x2EC);
    MOX::_skill_data[26].name = ESTRINGS::E_Strings_(0x2ED);
    MOX::_skill_data[27].name = ESTRINGS::E_Strings_(0x2EE);
    MOX::_skill_data[28].name = ESTRINGS::E_Strings_(0x2EF);
    MOX::_skill_data[29].name = ESTRINGS::E_Strings_(0x2F0);
    MOX::_skill_data[30].name = ESTRINGS::E_Strings_(0x2F1);
    MOX::_skill_data[31].name = ESTRINGS::E_Strings_(0x2F2);
    MOX::_skill_data[32].name = ESTRINGS::E_Strings_(0x2F3);
    MOX::_skill_data[33].name = ESTRINGS::E_Strings_(0x2F4);
    MOX::_skill_data[34].name = ESTRINGS::E_Strings_(0x2F5);
    MOX::_skill_data[35].name = ESTRINGS::E_Strings_(0x2F6);
    MOX::_skill_data[36].name = ESTRINGS::E_Strings_(0x2F7);
    MOX::_skill_data[37].name = ESTRINGS::E_Strings_(0x2F8);
    MOX::_skill_data[38].name = ESTRINGS::E_Strings_(0x2F9);
    MOX::_skill_data[39].name = ESTRINGS::E_Strings_(0x2FA);
    MOX::_skill_data[40].name = ESTRINGS::E_Strings_(0x2A0);
    MOX::_skill_data[41].name = ESTRINGS::E_Strings_(0x2FB);
    MOX::_skill_data[42].name = ESTRINGS::E_Strings_(0x2FC);
    MOX::_skill_data[43].name = ESTRINGS::E_Strings_(0x2FD);
    MOX::_skill_data[44].name = ESTRINGS::E_Strings_(0x2FE);
    MOX::_skill_data[45].name = ESTRINGS::E_Strings_(0x2FF);
    MOX::_skill_data[46].name = ESTRINGS::E_Strings_(0x300);
    MOX::_skill_data[47].name = ESTRINGS::E_Strings_(0x301);
    MOX::_skill_data[48].name = ESTRINGS::E_Strings_(0x302);
    MOX::_skill_data[49].name = ESTRINGS::E_Strings_(0x303);
    MOX::_skill_data[50].name = ESTRINGS::E_Strings_(0x304);
    MOX::_skill_data[51].name = ESTRINGS::E_Strings_(0x305);
    MOX::_skill_data[52].name = ESTRINGS::E_Strings_(0x306);
    MOX::_skill_data[53].name = ESTRINGS::E_Strings_(0x307);

    MOX::_admin_skill_names[0] = ESTRINGS::E_Strings_(0x2DB);
    MOX::_admin_skill_names[1] = ESTRINGS::E_Strings_(0x2DF);
    MOX::_admin_skill_names[2] = ESTRINGS::E_Strings_(0x2E3);
    MOX::_admin_skill_names[3] = ESTRINGS::E_Strings_(0x2E9);
    MOX::_admin_skill_names[4] = ESTRINGS::E_Strings_(0x2EB);
    MOX::_admin_skill_names[5] = ESTRINGS::E_Strings_(0x2ED);
    MOX::_admin_skill_names[6] = ESTRINGS::E_Strings_(0x2F9);
    MOX::_admin_skill_names[7] = ESTRINGS::E_Strings_(0x2FC);
    MOX::_admin_skill_names[8] = ESTRINGS::E_Strings_(0x300);

    MOX::_officer_level_names[0] = ESTRINGS::E_Strings_(0x308);
    MOX::_officer_level_names[1] = ESTRINGS::E_Strings_(0x309);
    MOX::_officer_level_names[2] = ESTRINGS::E_Strings_(0x30A);
    MOX::_officer_level_names[3] = ESTRINGS::E_Strings_(0x30B);
    MOX::_officer_level_names[4] = ESTRINGS::E_Strings_(0x30C);
    MOX::_officer_level_names[5] = ESTRINGS::E_Strings_(0x30D);

    MOX::_star_officer_level_names[0] = ESTRINGS::E_Strings_(0x30E);
    MOX::_star_officer_level_names[1] = ESTRINGS::E_Strings_(0x30F);
    MOX::_star_officer_level_names[2] = ESTRINGS::E_Strings_(0x310);
    MOX::_star_officer_level_names[3] = ESTRINGS::E_Strings_(0x311);
    MOX::_star_officer_level_names[4] = ESTRINGS::E_Strings_(0x312);
    MOX::_star_officer_level_names[5] = ESTRINGS::E_Strings_(0x313);

    MOX::_hyper_field_title = ESTRINGS::E_Strings_(0x284);
}

char* __cdecl E_Strings_(int16_t index) {
    return _strings[index];
}

}