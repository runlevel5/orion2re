#pragma once
#include <cstdint>

namespace CMBTSPEC {
    extern int32_t _g_engine_sfx_handle;

    void __cdecl Draw_Cloak_(int16_t ship_idx, int16_t cloak_type);

    void __cdecl Draw_Continuing_Tractor_(int16_t ship_idx);

    void __cdecl Start_Ship_Engine_SFX_();

    void __cdecl End_Ship_Engine_SFX_();

    void __cdecl Draw_Moving_Tractor_(int16_t ship_idx, int16_t center_x, int16_t center_y);

    void __cdecl Lightning_Field_(int16_t ship_idx, int16_t missile_idx, int16_t target_x, int16_t target_y);

    void __cdecl Destroy_Ship_FX_(int16_t ship_idx, int16_t x, int16_t y, int16_t explosion_type);

    void __cdecl Draw_Stasis_Ball_(int16_t ship_idx, int16_t scale_percent, int16_t off_x, int16_t off_y);

    void __cdecl Draw_Plasma_Web_(int16_t ship_idx, int16_t x_off, int16_t y_off);

    void __cdecl Do_Spherical_SFX_(int16_t ship_idx, int16_t x, int16_t y, int16_t subtype, int16_t shots_left);

    void __cdecl Draw_Stasis_Beam_(int16_t source_ship_idx, int16_t target_ship_idx);

    void __cdecl Draw_Anti_Missile_Rockets_(int16_t ship_idx, int16_t target_x, int16_t target_y, int16_t missile_idx, int16_t tech_level);

    void __cdecl Draw_Anti_Missile_Rocket_Picture_(int16_t x, int16_t y);

    void __cdecl Anti_Missile_Rockets_(int16_t ship_idx, int16_t ship_x, int16_t ship_y, int16_t missile_idx, int16_t weapon_slot_idx, int16_t tech_level);

    void __cdecl Gyro_Destablizer_(int16_t source_idx, int16_t target_idx, int16_t attack_count);

    void __cdecl Draw_Gyro_Special_Effect_(int16_t source_idx, int16_t target_idx);

    void __cdecl Plasma_Web_(int16_t param_1, int16_t param_2, int16_t param_3);

    void __cdecl Stellar_Converter_(int16_t attacker_idx, int16_t target_idx, int16_t num_shots);

    void __cdecl Tractor_Beam_(int16_t ship_idx, int16_t target_idx);

    void __cdecl Draw_Transporter_Bomb_Beam_(int16_t ship_idx, int16_t target_x, int16_t target_y);

    void __cdecl Missile_Launch_FX_(int16_t missile_idx, int16_t tx, int16_t ty, int16_t is_targeting_missile);

    void __cdecl Play_Ship_Explosion_SFX_(int16_t param_1);

    void __cdecl Draw_Ship_Collapsing_(int16_t ship_idx, int16_t x, int16_t y, int16_t scale, int8_t rotation_offset);
}