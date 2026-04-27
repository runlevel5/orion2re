#pragma once

namespace BEAMS {
    inline uint8_t& _g_pierced_shield_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552D08});

    inline int16_t (&_old_color)[2] =
        *reinterpret_cast<int16_t(*)[2]>(std::uintptr_t{0x00552D0C});

    inline s_animation_header*& _mauler_ball_seg =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00564D64});

    inline s_animation_header*& _l_anims =
        *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00564D68});

    inline uint8_t& _shield_hit_now_flag =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552D10});

    inline uint8_t& _reflection_field_hit_now_flag = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00552D14});

    inline uint8_t (&_ship_hit_now_flag)[2] = *reinterpret_cast<uint8_t(*)[2]>(std::uintptr_t{0x00552D18});

    inline uint16_t& _ship_firing_now_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x00552D1C});

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

    inline void __cdecl Fire_Weapon_(int16_t x1, int16_t y1, int16_t x2, int16_t y2, 
                                    int16_t dir1, int16_t dir2, int16_t weapon_type, int16_t frame,
                                    int p9, int p10, int p11, int p12, int p13, int p14, int p15, int p16, int p17, bool is_mod3) {
        using Fn = void(__cdecl*)(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t,
                                  int, int, int, int, int, int, int, int, int, bool);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00421cd0});
        fn(x1, y1, x2, y2, dir1, dir2, weapon_type, frame, p9, p10, p11, p12, p13, p14, p15, p16, p17, is_mod3);
    }

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