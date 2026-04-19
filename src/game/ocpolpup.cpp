#include "pch.h"

namespace OCPOLPUP {
    void __cdecl Occupation_Policy_Popup_(void) {
        uint8_t popup_colors[4] = {0x9F, 0xA0, 0xA1, 0xA2};
        int16_t planet_index;
        int16_t colony_index;
        int8_t owner_idx;
        int8_t player_trait;

        planet_index = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);
        colony_index = MOX::_planet[planet_index].colony_index;
        _value = static_cast<uint16_t>(static_cast<uint8_t>(MOX::_colony[colony_index].occupation_policy));

        fonts::Set_Font_Style_Shadow_Down_(1, reinterpret_cast<s_colors*>(popup_colors));
        video::Copy_Off_To_Back_();
        ERICHELP::Set_Occupation_Policy_Popup_Help_List_();
        MOX2::Get_Auto_Function_(&DAT_0056475c);
        fields::Deactivate_Auto_Function_();

        planet_index = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);
        colony_index = MOX::_planet[planet_index].colony_index;
        owner_idx = static_cast<int8_t>(MOX::_colony[colony_index].owner);
        player_trait = MOX::_player[owner_idx].traits[0];
        if (player_trait != 4 && player_trait != 5) {
            if (GENDRAW::Warning_Box2_(ESTRINGS::E_Strings_(0xB4)) != 0) {
                _value = 0;
            } else {
                _value = 3;
            }
        } else {
            _value = 3;
        }

        planet_index = HAROLD::Planet_Index_(MOX::_screen_data, COLONY::_orbit_temp);
        colony_index = MOX::_planet[planet_index].colony_index;
        MOX::_colony[colony_index].occupation_policy = static_cast<char>(_value);

        fields::Deactivate_Help_List_();
        MOX2::Put_Auto_Function_(&DAT_0056475c);
        video::Copy_Back_To_Off_();
    }
}