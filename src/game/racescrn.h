#pragma once

namespace RACESCRN {
    using render_fn = void(*)(s_racescrn_mouse_group*);

    extern const char s_billtext_lbx_0055135c[13];
    extern const char s_billtex2_lbx_00556038[13];
    extern const char s_races_lbx_005574e8[10];

    extern s_racescrn_icon_layout _mouse_group_data;
    extern int16_t _g_spy_xfer_count;
    extern s_racescrn_mouse_group* _g_agent_group_p;
    extern s_racescrn_mouse_group* _g_mouse_group_p;
    extern int32_t _net_state;
    extern s_racescrn_race_display* _g_display_p;
    extern uint8_t _g_in_who_mode;
    extern s_animation_header* _g_mouse_seg;
    extern int16_t _g_agent_field;
    extern uint8_t _g_in_spy_mode;
    extern s_animation_header* _arrow_seg;

    extern char _trade_treaty_buffer[30];
    extern char _ignoring_str[20];
    extern s_animation_header* _netbox_seg;
    extern char _agent_bonus_str[20];
    extern char _no_contact_str[20];
    extern char _declare_war_buffer[120];
    extern char _spy_bonus_str[20];
    extern char _giving_treaty_buffer[80];
    extern char _research_treaty_buffer[30];
    extern char _relation_text[17][15];
    extern uint8_t _slider_remap_list[26];
    extern uint8_t _relation_name_table[17];
    extern s_racescrn_icon_layout _spy_group_data[7];
    extern int16_t _race_bar[14];
    extern int16_t _race_spies[14];
    extern int16_t _race_spy_btns[14];
    extern int16_t _race_slider_x[7];
    extern s_racescrn_icon_layout _agent_group_data;
    extern int16_t _race_picture[14];
    extern int16_t _race_text[14];
    extern s_colors _race_normal_color;
    extern s_colors _race_high_color;
    extern s_colors _banner_color_high;
    extern s_colors _banner_color_low;
    extern s_colors _banner_color_edge;
    extern char _receiving_treaty_buffer[80];

    extern const char s_empty_string[1];
    extern const char s_carriage_return[2];
    extern const char s_spy_bonus_fmt[8];
    extern const char s_agent_bonus_fmt[8];
    extern const char s_trade_bc_fmt[8];
    extern const char s_trade_mc_fmt[8];
    extern const char s_research_fmt[8];
    extern const char s_pr_str[3];
    extern const char s_mc_str[3];
    extern const char s_fp_str[4];
    extern const char s_bc_str[3];
    extern const char s_rp_str[3];

    extern const char s_space[2];
    extern const char s_I[2];
    extern const char s_R[2];
    extern const char s_A[2];
    extern const char s_D[2];

    extern char _diplomacy_net_wait_buffer[100];
    extern char _diplomacy_net_requeting_buffer[100];

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