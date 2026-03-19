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

char* __cdecl E_Strings_(int16_t index) {
    return _strings[index];
}

}