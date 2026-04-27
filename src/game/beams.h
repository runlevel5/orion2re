#pragma once

namespace BEAMS {
    struct LocalColor {
        int32_t g;
        int32_t b;
        int32_t r;
    };

    inline uint8_t& _g_pierced_shield_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552D08});

    inline int16_t (&_old_color)[2] =
        *reinterpret_cast<int16_t(*)[2]>(std::uintptr_t{0x00552D0C});

    inline int16_t& _g_weapon_n = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00564D6C });
    inline int16_t& _g_sfx_n = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00564D70 });

    int8_t __cdecl Is_Steve_Stuff_Weapon_(int16_t weapon_id);

    void __cdecl Add_Weapon_Palette_Fragment_To_Current_Palette_(int16_t source_fragment, int16_t dest_start, int16_t count);

    void __cdecl Build_Steve_Stuff_Struct_For_Weapon_(int16_t weapon_id, uint16_t specials, s_weapon_visual_fx* out);

    void __cdecl Draw_Generic_Beam_(
        int16_t attacker_x, int16_t attacker_y, int16_t target_x, int16_t target_y, int16_t attacker_facing,
        int16_t target_facing, int16_t duration_frames, int16_t palette_base_index, int16_t beam_length,
        int16_t beam_width, int16_t sprite_id, int16_t draw_core_flag, int16_t draw_glow_flag,
        int16_t draw_particles_flag, int16_t is_shield_piercing, int16_t is_autofiring, int16_t param_17,
        int16_t param_18, int16_t param_19, int16_t param_20, int16_t mount_scale_mod, int16_t fx_param_1,
        int16_t fx_param_2, int16_t fx_param_3, int16_t fx_param_4, int16_t frame_index,
        int16_t target_size_class, uint16_t hit_result_flags, uint16_t special_flag,
        int16_t is_non_ship_target, int16_t base_damage_class, int16_t reflection_state,
        int16_t attacker_size_class, s_weapon_visual_fx* param_34
    );

    inline s_animation_header*& _mauler_ball_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00564D64});

    inline s_animation_header*& _l_anims =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00564D68});

    inline uint8_t& _shield_hit_now_flag =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552D10});

    inline uint8_t& _reflection_field_hit_now_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552D14});

    inline uint8_t (&_ship_hit_now_flag)[2] = *reinterpret_cast<uint8_t(*)[2]>(std::uintptr_t{0x00552D18});

    inline uint8_t (&_ship_firing_now_flag)[2] = *reinterpret_cast<uint8_t (*)[2]>(std::uintptr_t{ 0x00552D1C });

    inline int32_t (&_g_beam_fire_sfx_handle)[10] = *reinterpret_cast<int32_t(*)[10]>(std::uintptr_t{0x00552D20});

    inline int32_t& _g_shield_sfx_handle =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x00552D48});

    inline uint32_t& _g_reflection_field_sfx_handle =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00552D4C});

    inline int16_t& _max_frames =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00552D50});

    inline int16_t& _beam_angle = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00552D54});

    inline uint8_t (&_shield_diameters)[8] =
        *reinterpret_cast<uint8_t (*)[8]>(std::uintptr_t{0x00552D58});

    inline uint16_t& _shield_hit_draw_facing = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00552D60});

    inline int16_t& _reflection_field_hit_draw_facing = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00552D64});

    inline uint16_t& _shield_display_status =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00552D68});

    inline uint8_t (&_reflection_field_diameters)[8] = *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x00552D6C});

    inline int16_t& _reflection_field_display_status = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00552D70});

    inline int16_t& _g_reflection_state_flag =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00552D74});

    inline uint8_t& _g_firing_reflected_beam_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552D78});

    inline int32_t (&_low_ramp_values)[20] =
        *reinterpret_cast<int32_t(*)[20]>(std::uintptr_t{0x00552D80});

    inline int32_t (&_low_primary_cross_values)[20] =
        *reinterpret_cast<int32_t(*)[20]>(std::uintptr_t{0x00552DD0});

    inline int32_t (&_low_saturated_values)[20] =
        *reinterpret_cast<int32_t(*)[20]>(std::uintptr_t{0x00552E20});

    inline int32_t (&_moderate_primary_cross_values)[20] =
        *reinterpret_cast<int32_t(*)[20]>(std::uintptr_t{0x00552EC0});

    inline int32_t (&_moderate_saturated_values)[20] =
        *reinterpret_cast<int32_t(*)[20]>(std::uintptr_t{0x00552F10});

    inline int32_t (&_high_primary_cross_values)[20] =
        *reinterpret_cast<int32_t(*)[20]>(std::uintptr_t{0x00552FB0});

    inline int32_t (&_high_saturated_values)[20] =
        *reinterpret_cast<int32_t(*)[20]>(std::uintptr_t{0x00553000});

    inline int16_t& _n_damage_msgs =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00552D7C});

    inline s_beam_dmg_msg (&_msg_queue)[35] =
        *reinterpret_cast<s_beam_dmg_msg(*)[35]>(std::uintptr_t{0x005A7FC0});

    void __cdecl Reset_Beam_Swapping_Colors_();

    void __cdecl Init_Beams_();

    inline s_animation_header* (&_laser_burst_anim)[2][2][16] = 
        *reinterpret_cast<s_animation_header* (*)[2][2][16]>(std::uintptr_t{0x005A7EC0});

    void __cdecl Load_Ship_Burst_();

    inline s_animation_header* (&_explosion_anim)[2][16] =
        *reinterpret_cast<s_animation_header* (*)[2][16]>(std::uintptr_t{0x005A7DA0});

    void __cdecl Load_Ship_Explosion_();

    inline s_animation_header* (&_shield_anims)[5][4][2] =
        *reinterpret_cast<s_animation_header* (*)[5][4][2]>(std::uintptr_t{0x005A7E20});

    void __cdecl Load_Shield_Hit_();

    s_animation_header* __cdecl Shield_Anims_(int16_t beam_idx, int16_t size_idx, int16_t is_special, int16_t is_heavy);

    int16_t __cdecl Ship_Size_To_Shield_Size_(int16_t ship_size);

    int16_t __cdecl Rot_To_Base_Rot_(int16_t rot);

    int16_t __cdecl Rot_To_Rot_Method_(int16_t rot);

    int16_t __cdecl Planet_Size_To_Shield_Size_(int16_t planet_size);

    void __cdecl Adjust_Shield_Display_Status_(uint16_t* shield_status);

    void __cdecl Draw_Shield_Hit_(int16_t x, int16_t y, int16_t ship_idx, int16_t param_4, int16_t param_5, int16_t param_6, int16_t param_7);

    s_animation_header* __cdecl Reflection_Field_Anims_(int32_t p1, int32_t p2);

    void __cdecl Load_Reflection_Field_Hit_(int8_t should_load);

    void __cdecl Load_Combat_Beam_Stuff_(int8_t reflection_hit_param);

    void __cdecl Add_Damage_Message_To_Queue_(int16_t x, int16_t y, int16_t total_dmg, int16_t struct_dmg, int16_t shield_dmg, uint8_t side);

    void __cdecl Add_Damage_Indicator_For_Ship_(int32_t dx, int32_t dy, int16_t ship_idx, int32_t flag, int16_t total_damage, int16_t struct_damage);

    void __cdecl Draw_Shield_Hit_Once_Through_(int16_t ship_idx, int16_t param_2, int16_t draw_x, int16_t draw_y);

    void __cdecl Squish_Out_Msg_Queue_Slot_(int16_t index);

    void __cdecl Draw_Damage_Indicator_(int16_t x, int16_t y, int16_t total_dmg, int16_t hull_dmg, int16_t shield_dmg, uint8_t side, int16_t anim_timer);

    int8_t __cdecl Done_Drawing_Damage_Message_Queue_();

    void __cdecl Draw_Damage_Message_Queue_();

    void __cdecl Draw_Damage_Message_Queue_Until_Done_();

    int16_t __cdecl Get_Max_Frames_();

    int16_t __cdecl Get_Ship_Range_(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

    void __cdecl Adjust_Reflection_Field_Display_Status_(int16_t* param_1);

    int16_t __cdecl Get_SFX_Draw_Shield_Facing2_(int16_t ship1_idx, int16_t ship2_idx);

    int8_t __cdecl Do_Get_SFX_Draw_Shield_Facing_(int16_t attacker_id, int16_t target_id);

    void __cdecl Draw_Reflection_Field_Hit_(int16_t x, int16_t y, int16_t ship_size, int16_t rotation, int16_t frame);

    void __cdecl Reset_Beam_SFX_Handles_();

    void __cdecl Fire_Weapon_(int16_t attacker_x, int16_t attacker_y, int16_t target_x, int16_t target_y,
                              int16_t attacker_facing, int16_t target_facing, int16_t weapon_id, int16_t current_frame,
                              int16_t fx_slot_idx, int16_t target_size_class, int16_t hit_result_flags,
                              int16_t special_flag, int16_t is_non_ship_target, int16_t fire_point_idx,
                              int16_t reflection_state, int16_t attacker_size_class, uint16_t weapon_specials);

    int8_t __cdecl Draw_Fighter_Beam_(int16_t* fighter_coords, int16_t count, int16_t target_x, int16_t target_y, int16_t weapon_type, int16_t frame);

    void __cdecl Add_Damage_Indicator_For_Missile_(int16_t x, int16_t y, uint8_t direction, int16_t damage);

    void __cdecl Add_Damage_Indicator_For_Biological_(int16_t x, int16_t y, uint8_t side_flag, int16_t damage);

    void __cdecl Beam_SFX_(int16_t attacker_ship_id, int16_t attacker_x, int16_t attacker_y, int16_t target_id, int16_t target_x, int16_t target_y, s_beam_fx_entry* param_7, int16_t is_non_ship_target);

    void __cdecl Rob_Interpolate_Line_(int16_t* p_x, int16_t* p_y, int16_t target_x, int16_t target_y, int16_t step);

    void __cdecl Draw_Ship_Burst_(int16_t x, int16_t y, int16_t dir, int16_t base_damage_class, int16_t frame, int16_t is_palette_not_90);

    void __cdecl Draw_Beam2_(int16_t start_x, int16_t start_y, int16_t end_x, int16_t end_y, int32_t x1, int32_t y1,
                             int32_t x2, int32_t y2, int32_t unused_9, uint32_t thickness, int32_t color_idx, int32_t beam_color_1,
                             int32_t beam_color_2, int32_t beam_color_3, int32_t beam_color_4, int32_t beam_color_5,
                             int32_t beam_color_6, int32_t beam_color_7, uint32_t beam_type, int32_t condition_var,
                             int32_t unused_21, int32_t size_multiplier);

    void __cdecl Set_Beam_Colors_(
        int32_t palette_idx,
        int32_t r1, int32_t g1, int32_t b1,
        int32_t r2, int32_t g2, int32_t b2,
        int32_t intensity
    );

    void __cdecl Draw_Ship_Explosion_(int16_t x, int16_t y, int16_t type_idx, int16_t frame_idx, int16_t size_idx);

    bool __cdecl XY_In_Shield_Radius_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t ship_size);

    bool __cdecl XY_In_Reflection_Field_Radius_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t ship_size);
}