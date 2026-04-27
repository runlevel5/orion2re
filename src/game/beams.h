#pragma once

namespace BEAMS {
    inline uint8_t& _g_pierced_shield_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552D08});

    inline int16_t (&_old_color)[2] =
        *reinterpret_cast<int16_t(*)[2]>(std::uintptr_t{0x00552D0C});

    inline int16_t& _g_weapon_n = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00564D6C });
    inline int16_t& _g_sfx_n = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00564D70 });

    inline bool __cdecl Is_Steve_Stuff_Weapon_(int16_t weapon_id) {
        using Fn = bool(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00424c20 });
        return fn(weapon_id);
    }

    inline void __cdecl Add_Weapon_Palette_Fragment_To_Current_Palette_(int16_t color, int offset, int size) {
        using Fn = void(__cdecl*)(int16_t, int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00420ce0 });
        fn(color, offset, size);
    }

    inline void __cdecl Build_Steve_Stuff_Struct_For_Weapon_(int16_t weapon_id, int16_t specials, s_weapon_visual_fx* out) {
        using Fn = void(__cdecl*)(int16_t, int16_t, s_weapon_visual_fx*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00424250 });
        fn(weapon_id, specials, out);
    }

    inline void __cdecl Draw_Generic_Beam_(
        int16_t attacker_x, int16_t attacker_y, int16_t target_x, int16_t target_y,
        int16_t attacker_facing, int16_t target_facing, int16_t duration, int palette_offset,
        int16_t v_local_3e, int16_t v_local_42_2, int16_t v_sfx_sprite_id, int16_t v_local_42,
        int16_t v_local_46_2, int16_t v_local_46, int16_t v_local_38, int16_t v_local_3e_2,
        int16_t v_local_4a_2, int16_t v_local_4a, int16_t v_local_4c, int16_t v_ebx,
        int16_t mount_scale_mod, int16_t v_local_54_6, int16_t v_local_54_4, int16_t v_local_54_2,
        int16_t v_local_54, int16_t frame_index, int16_t target_size_class,
        int16_t hit_result_flags, int16_t special_flag, int16_t is_non_ship_target,
        int16_t v_local_58, int16_t reflection_state, int16_t attacker_size_class,
        s_weapon_visual_fx* pfx) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, s_weapon_visual_fx*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00420110 });
        fn(attacker_x, attacker_y, target_x, target_y, attacker_facing, target_facing, duration, palette_offset, v_local_3e, v_local_42_2, v_sfx_sprite_id, v_local_42, v_local_46_2, v_local_46, v_local_38, v_local_3e_2, v_local_4a_2, v_local_4a, v_local_4c, v_ebx, mount_scale_mod, v_local_54_6, v_local_54_4, v_local_54_2, v_local_54, frame_index, target_size_class, hit_result_flags, special_flag, is_non_ship_target, v_local_58, reflection_state, attacker_size_class, pfx);
    }

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

    inline uint32_t& _g_shield_sfx_handle =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x00552D48});

    inline int16_t& _max_frames =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00552D50});

    inline uint8_t (&_shield_diameters)[8] =
        *reinterpret_cast<uint8_t (*)[8]>(std::uintptr_t{0x00552D58});

    inline uint16_t& _shield_hit_draw_facing = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00552D60});

    inline uint16_t& _reflection_field_hit_draw_facing = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00552D64});

    inline uint16_t& _shield_display_status =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00552D68});

    inline uint8_t (&_reflection_field_diameters)[8] = *reinterpret_cast<uint8_t(*)[8]>(std::uintptr_t{0x00552D6C});

    inline uint16_t& _reflection_field_display_status = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00552D70});

    inline uint16_t& _g_reflection_state_flag =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00552D74});

    inline uint8_t& _g_firing_reflected_beam_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552D78});

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

    inline int16_t __cdecl Get_Ship_Range_(int16_t attacker_x, int16_t attacker_y, int16_t target_x, int16_t target_y) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00420A50});
        return fn(attacker_x, attacker_y, target_x, target_y);
    }

    inline void __cdecl Adjust_Reflection_Field_Display_Status_(uint16_t* draw_facing) {
        using Fn = void(__cdecl*)(uint16_t*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00423AA0});
        fn(draw_facing);
    }

    inline int16_t __cdecl Get_SFX_Draw_Shield_Facing2_(int16_t attacker_id, int16_t target_id) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00422ED0});
        return fn(attacker_id, target_id);
    }

    inline int16_t __cdecl Do_Get_SFX_Draw_Shield_Facing_(int16_t attacker_id, int16_t target_id) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00422E40});
        return fn(attacker_id, target_id);
    }

    inline void __cdecl Draw_Reflection_Field_Hit_(int16_t x, int16_t y, int16_t target_size_class, int16_t hit_facing, int16_t draw_facing) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00423C00});
        fn(x, y, target_size_class, hit_facing, draw_facing);
    }

    inline void __cdecl Reset_Beam_SFX_Handles_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00421CA0});
        fn();
    }

    void __cdecl Fire_Weapon_(int16_t attacker_x, int16_t attacker_y, int16_t target_x, int16_t target_y,
                              int16_t attacker_facing, int16_t target_facing, int16_t weapon_id, int16_t current_frame,
                              int16_t fx_slot_idx, int16_t target_size_class, int16_t hit_result_flags,
                              int16_t special_flag, int16_t is_non_ship_target, int16_t fire_point_idx,
                              int16_t reflection_state, int16_t attacker_size_class, uint16_t weapon_specials, bool is_mod3);

    int8_t __cdecl Draw_Fighter_Beam_(int16_t* fighter_coords, int16_t count, int16_t target_x, int16_t target_y, int16_t weapon_type, int16_t frame);

    void __cdecl Add_Damage_Indicator_For_Missile_(int16_t x, int16_t y, uint8_t direction, int16_t damage);

    void __cdecl Add_Damage_Indicator_For_Biological_(int16_t x, int16_t y, uint8_t side_flag, int16_t damage);

    void __cdecl Beam_SFX_(int16_t attacker_ship_id, int16_t attacker_x, int16_t attacker_y, int16_t target_id, int16_t target_x, int16_t target_y, s_beam_fx_entry* param_7, int16_t is_non_ship_target);

    inline void __cdecl Rob_Interpolate_Line_(int16_t *x, int16_t *y, int16_t target_x, int16_t target_y, int16_t len) {
        using Fn = void(__cdecl *)(int16_t *, int16_t *, int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041ffa0});
        fn(x, y, target_x, target_y, len);
    }
}