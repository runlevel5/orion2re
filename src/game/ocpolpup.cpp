#include "pch.h"

namespace OCPOLPUP {
    uint16_t _occupation_policy_value;

    MOX2::action_func_t _saved_autofunction;

    void __cdecl Occupation_Policy_Popup_() {
        s_colors font_colors = {{0x9f, 0xa0, 0xa1, 0xa2, 0, 0, 0, 0}};
        int16_t planet_index;
        int16_t colony_index;
        int16_t owner_index;

        planet_index = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);
        colony_index = MOX::_planet[planet_index].colony_index;
        _occupation_policy_value = (uint16_t)(uint8_t)MOX::_colony[colony_index].occupation_policy;

        fonts::Set_Font_Style_Shadow_Down_(1, &font_colors);
        video::Copy_Off_To_Back_();
        ERICHELP::Set_Occupation_Policy_Popup_Help_List_();
        MOX2::Get_Auto_Function_(&_saved_autofunction);
        fields::Deactivate_Auto_Function_();

        planet_index = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);
        colony_index = MOX::_planet[planet_index].colony_index;
        owner_index = MOX::_colony[colony_index].owner;

        if (MOX::_player[owner_index].traits[TRAIT_CURRENT_GOVERNMENT] != GOVERNMENT_DEMOCRACY && MOX::_player[owner_index].traits[TRAIT_CURRENT_GOVERNMENT] != GOVERNMENT_FEDERATION) {
            if (GENDRAW::Confirmation_Box_(ESTRINGS::E_Strings_(0xB4)) != 0) {
                _occupation_policy_value = 0;
            } else {
                _occupation_policy_value = 3;
            }
        } else {
            _occupation_policy_value = 3;
        }

        planet_index = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);
        colony_index = MOX::_planet[planet_index].colony_index;
        MOX::_colony[colony_index].occupation_policy = (char)_occupation_policy_value;

        fields::Deactivate_Help_List_();
        MOX2::Put_Auto_Function_(&_saved_autofunction);
        video::Copy_Back_To_Off_();
    }
}