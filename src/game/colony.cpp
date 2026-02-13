#include "deps.h"

namespace COLONY {
    char* __cdecl Real_Building_Name_(int16_t building_id) {
        return TECHDATA::_buildings[building_id].name;
    }

    s_animation_header* __cdecl Buffer_Reload_Planets_(int16_t entry_num) {
        return reinterpret_cast<s_animation_header*>(Buffer_Reload_(
            COLONY::_PLANETS_LBX,
            static_cast<int>(entry_num),
            MOX::_global_cache_seg
        ));
    }

    s_animation_header* __cdecl C_Anims_(int16_t index) {
        MOX::Mox_Update_();

        if (index > 0x23) {
            return nullptr;
        }

        switch (index) {
        case 0: {
            auto& colony = MOX::_colony[COLONY::_colony_handle];
            auto& planet = MOX::_planet[colony.planet];
            int16_t reload_arg = (int16_t)planet.climate_bg_type + (int16_t)colony.climate * 3;
            return Buffer_Reload_Planets_(reload_arg);
        }

        case 1:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x31, MOX::_global_cache_seg));

        case 2:
        case 11:
        case 12:
        case 16: case 17: case 18: case 19: case 20:
        case 21: case 22: case 23: case 24: case 25:
        case 26: case 27: case 28:
        case 30: case 31: case 32:
            return _c_anims[index];

        case 3:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLPUPS_LBX, 5, MOX::_global_cache_seg));

        case 4:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLPUPS_LBX, 6, MOX::_global_cache_seg));

        case 5:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x10, MOX::_global_cache_seg));

        case 6:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x11, MOX::_global_cache_seg));

        case 7:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x12, MOX::_global_cache_seg));

        case 8:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x2b, MOX::_global_cache_seg));

        case 9:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x2c, MOX::_global_cache_seg));

        case 10:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x2d, MOX::_global_cache_seg));

        case 13:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x32, MOX::_global_cache_seg));

        case 14:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x33, MOX::_global_cache_seg));

        case 15:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY2_LBX, 0x34, MOX::_global_cache_seg));

        case 33:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLPUPS_LBX, 0x16, MOX::_global_cache_seg));

        case 34:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY_LBX, 0x11, MOX::_global_cache_seg));

        case 35:
            return reinterpret_cast<s_animation_header*>(Buffer_Reload_(_COLONY_LBX, 0x12, MOX::_global_cache_seg));

        default:
            return nullptr;
        }
    }

    void __cdecl Update_Colony_Palette_() {
        fonts::Load_Palette_(1, 0, 255);
        s_animation_header* colony_anims = C_Anims_(0);
        animate::Draw_Palette_(colony_anims);
        remap::Update_Glass_Remap_Colors_();
    }

    /**
     * Checks if a defensive satellite building (Star Base or Battlestation) should be replaced 
     * because a superior version already exists in the colony.
     */
    bool __cdecl Sat_Should_Be_Replaced_(int16_t building_id) {
        // Size of s_colony is 361 bytes (0x169)
        const s_colony& colony = MOX::_colony[COLONY::_colony_handle];

        if (building_id == BUILDING_STAR_BASE) {
            // A Star Base is redundant if either a Battlestation or a Star Fortress exists.
            if (colony.buildings[BUILDING_BATTLESTATION] != 0 || 
                colony.buildings[BUILDING_STAR_FORTRESS] != 0) {
                return true;
            }
        } 
        else if (building_id == BUILDING_BATTLESTATION) {
            // A Battlestation is redundant if a Star Fortress exists.
            if (colony.buildings[BUILDING_STAR_FORTRESS] != 0) {
                return true;
            }
        }

        return false;
    }

    void __cdecl Insert_Satellite_Into_Array_(int16_t satellite_id, int16_t* count) {
        if (!Sat_Should_Be_Replaced_(satellite_id)) {
            _colony_satellites[*count] = satellite_id;
            (*count)++;
        }
    }
}