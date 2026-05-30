#pragma once

namespace RACESCRN {
    using render_fn = void(*)(s_racescrn_mouse_group*);

    inline char* s_billtext_lbx_0055135c = reinterpret_cast<char*>(std::uintptr_t{0x0055135C});
    inline char* s_billtex2_lbx_00556038 = reinterpret_cast<char*>(std::uintptr_t{0x00556038});
    inline char* s_races_lbx = reinterpret_cast<char*>(std::uintptr_t{0x005574E8});
    inline char* s_races_lbx_005574e8 = s_races_lbx;

    inline s_racescrn_icon_layout& _mouse_group_data = *reinterpret_cast<s_racescrn_icon_layout*>(std::uintptr_t{0x005581E8});
    inline int16_t& _g_spy_xfer_count = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00566550});
    inline s_racescrn_mouse_group*& _g_agent_group_p = *reinterpret_cast<s_racescrn_mouse_group**>(std::uintptr_t{0x00566554});
    inline s_racescrn_mouse_group*& _g_mouse_group_p = *reinterpret_cast<s_racescrn_mouse_group**>(std::uintptr_t{0x00566558});
    inline int32_t& _net_state = *reinterpret_cast<int32_t*>(std::uintptr_t{0x0056655C});
    inline s_racescrn_race_display*& _g_display_p = *reinterpret_cast<s_racescrn_race_display**>(std::uintptr_t{0x00566560});
    inline uint8_t& _g_in_who_mode = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00566564});
    inline s_animation_header*& _g_mouse_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00566568});
    inline int16_t& _g_agent_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x0056656C});
    inline uint8_t& _g_in_spy_mode = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00566570});
    inline s_animation_header*& _arrow_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00566574});

    inline char* _trade_treaty_buffer = reinterpret_cast<char*>(std::uintptr_t{0x005A5330});
    inline char* _ignoring_str = reinterpret_cast<char*>(std::uintptr_t{0x005A5350});
    inline void*& _netbox_seg = *reinterpret_cast<void**>(std::uintptr_t{0x005A5364});
    inline char* _agent_bonus_str = reinterpret_cast<char*>(std::uintptr_t{0x005A5370});
    inline char* _no_contact_str = reinterpret_cast<char*>(std::uintptr_t{0x005A5390});
    inline char* _declare_war_buffer = reinterpret_cast<char*>(std::uintptr_t{0x005A53B0});
    inline char* _spy_bonus_str = reinterpret_cast<char*>(std::uintptr_t{0x005A5430});
    inline char* _giving_treaty_buffer = reinterpret_cast<char*>(std::uintptr_t{0x005A5450});
    inline char* _research_treaty_buffer = reinterpret_cast<char*>(std::uintptr_t{0x005A54A0});
    inline char (*_relation_text)[15] = reinterpret_cast<char(*)[15]>(std::uintptr_t{0x005A54C0});
    inline uint8_t* _slider_remap_list = reinterpret_cast<uint8_t*>(std::uintptr_t{0x00558088});
    inline uint8_t* _relation_name_table = reinterpret_cast<uint8_t*>(std::uintptr_t{0x005580A8});
    inline s_racescrn_icon_layout* _spy_group_data = reinterpret_cast<s_racescrn_icon_layout*>(std::uintptr_t{0x005580C0});
    inline int16_t (&_race_bar)[14] = *reinterpret_cast<int16_t(*)[14]>(std::uintptr_t{0x00558148});
    inline int16_t (&_race_spies)[16] = *reinterpret_cast<int16_t(*)[16]>(std::uintptr_t{0x00558198});
    inline int16_t (&_race_spy_btns)[16] = *reinterpret_cast<int16_t(*)[16]>(std::uintptr_t{0x005581B8});
    inline int16_t (&_race_slider_x)[7] = *reinterpret_cast<int16_t(*)[7]>(std::uintptr_t{0x00558168});
    inline int16_t* _race_bar_y = reinterpret_cast<int16_t*>(std::uintptr_t{0x0055814A});
    inline s_racescrn_icon_layout& _agent_group_data = *reinterpret_cast<s_racescrn_icon_layout*>(std::uintptr_t{0x005581D8});
    inline int16_t (&_race_picture)[14] = *reinterpret_cast<int16_t(*)[14]>(std::uintptr_t{0x00558128});
    inline int16_t (&_race_text)[14] = *reinterpret_cast<int16_t(*)[14]>(std::uintptr_t{0x00558178});
    inline s_colors& _race_normal_color = *reinterpret_cast<s_colors*>(std::uintptr_t{0x005581F8});
    inline s_colors& _race_high_color = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00558200});
    inline s_colors& _banner_color_high = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00558208});
    inline s_colors& _banner_color_low = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00558210});
    inline s_colors& _banner_color_edge = *reinterpret_cast<s_colors*>(std::uintptr_t{0x00558218});
    inline const char* races_lbx = "races.lbx";
    inline char* _receiving_treaty_buffer = reinterpret_cast<char*>(std::uintptr_t{0x005A55C0});

    inline const char* s_empty_string = reinterpret_cast<const char*>(std::uintptr_t{0x00551100});
    inline const char* s_carriage_return = reinterpret_cast<const char*>(std::uintptr_t{0x0055136C});
    inline const char* s_spy_bonus_fmt = reinterpret_cast<const char*>(std::uintptr_t{0x00558078});
    inline const char* s_agent_bonus_fmt = reinterpret_cast<const char*>(std::uintptr_t{0x00558080});
    inline const char* s_trade_bc_fmt = reinterpret_cast<const char*>(std::uintptr_t{0x00558220});
    inline const char* s_trade_mc_fmt = reinterpret_cast<const char*>(std::uintptr_t{0x00558228});
    inline const char* s_research_fmt = reinterpret_cast<const char*>(std::uintptr_t{0x00558230});
    inline const char* s_pr_str = reinterpret_cast<const char*>(std::uintptr_t{0x00558238});
    inline const char* s_mc_str = reinterpret_cast<const char*>(std::uintptr_t{0x0055823C});
    inline const char* s_fp_str = reinterpret_cast<const char*>(std::uintptr_t{0x00558240});
    inline const char* s_bc_str = reinterpret_cast<const char*>(std::uintptr_t{0x00558244});
    inline const char* s_rp_str = reinterpret_cast<const char*>(std::uintptr_t{0x00558248});

    inline const char* s_space = reinterpret_cast<const char*>(std::uintptr_t{0x00551120});
    inline const char* s_I = reinterpret_cast<const char*>(std::uintptr_t{0x0055801C});
    inline const char* s_R = reinterpret_cast<const char*>(std::uintptr_t{0x0055824C});
    inline const char* s_A = reinterpret_cast<const char*>(std::uintptr_t{0x00557ED4});
    inline const char* s_D = reinterpret_cast<const char*>(std::uintptr_t{0x005569DC});

    inline char* _diplomacy_net_wait_buffer = reinterpret_cast<char*>(std::uintptr_t{0x005A5610});
    inline char* _diplomacy_net_requeting_buffer = reinterpret_cast<char*>(std::uintptr_t{0x005A5680});

    void __cdecl Init_Race_Display_Data_(s_racescrn_race_display* race_display, s_racescrn_mouse_group* agent_group);
    void __cdecl Calculate_Icon_Group_Data_(s_racescrn_mouse_group* group);
    void __cdecl Setup_Icon_Group_Data_(s_racescrn_mouse_group* group, s_racescrn_icon_layout* layout, int16_t count, s_animation_header* sprite1, s_animation_header* sprite2);
    void __cdecl Draw_Race_Photos_(s_racescrn_race_display* race_displays);
    void __cdecl Draw_Race_Text_(s_racescrn_race_display* display);
    void __cdecl Draw_Relations_Sliders_(s_racescrn_race_display* displays);
    int16_t __cdecl Setup_Main_Fields_(int16_t* ok_btn_id, int16_t* agriculture_btn_id, int16_t* agriculture_field_id,
                               int16_t* research_btn_id, int16_t* research_field_id, int16_t* industry_btn_id,
                               int16_t* industry_field_id, int16_t* diplomacy_btn_id, int16_t* diplomacy_field_id);
    int8_t __cdecl Setup_Race_Display_Fields_(s_racescrn_race_display* race_displays, int16_t base_field_id);
    void __cdecl Draw_Race_Screen_();

    void __cdecl Draw_Icon_Group_(s_racescrn_mouse_group* group);

    int16_t __cdecl Draw_Icon_Group_Cursor_(s_racescrn_mouse_group* group, int16_t x_pos, uint8_t color, bool show_count);

    void __cdecl Move_Spy_Group_From_Mouse_(s_racescrn_mouse_group* target_group);
    void __cdecl Move_Icon_Group_Icons_(s_racescrn_mouse_group* src, s_racescrn_mouse_group* dst, uint16_t count);
    void __cdecl Redraw_Spy_Mouse_(s_animation_header* spy_icon);
    int8_t __cdecl Setup_Race_Who_Fields_(s_racescrn_race_display* race_displays, int16_t field_id_base);
    s_racescrn_mouse_group* __cdecl Get_Spy_Group_For_Field_(int16_t field_id, int16_t* out_index);
    void __cdecl Move_Spies_Group_(s_racescrn_mouse_group* mouse_group, s_animation_header* mouse_sprite);
    void __cdecl Adjust_Spy_Mission_Data_();
    void __cdecl Update_Spy_Stuff_(s_racescrn_race_display* race_displays);

    void __cdecl Race_Screen_();
}