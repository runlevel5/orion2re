#include "pch.h"

namespace DESBOX {

uint32_t __cdecl Application_Type_(int16_t app_id) {
    // Accesses the 'type' field (offset 12) of the app_id-th element.
    return TECHDATA::_technology_applications[app_id].type;
}

void __cdecl Update_Weapon_Modification_Data_() {
    uint16_t mods = 0;
    for (int16_t i = 1; i < 15; ++i) {
        if (MOX::_weapon_mod_field_status[i] == 1) {
            mods |= (1 << i);
        }
        DESBOX::_weapon_replacement_mods = mods;
    }
}

}