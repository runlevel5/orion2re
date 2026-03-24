#pragma once
#include <cstdint>

struct s_animation_header;
struct s_colors;
struct s_drop_area;
struct s_list_item;
struct s_list_state;
struct s_tech_review_achievement;

namespace INFO {
    inline s_tech_review_achievement** _review_list =
        reinterpret_cast<s_tech_review_achievement**>(std::uintptr_t{0x00555B78});

    inline const char* s_billtex2_lbx =
        reinterpret_cast<const char*>(std::uintptr_t{0x00556038}); // "billtex2.lbx"

    inline s_colors& _info_normal_color =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x00555B88});

    inline int16_t (&_tech_rev_field)[8] =
        *reinterpret_cast<int16_t(*)[8]>(std::uintptr_t{0x00555CB0});

    inline s_drop_area (&_tech_review_drops)[4] =
        *reinterpret_cast<s_drop_area(*)[4]>(std::uintptr_t{0x00555CE8});

    inline uint16_t (&_list_sizes)[4] =
        *reinterpret_cast<uint16_t(*)[4]>(std::uintptr_t{0x00555EC8});

    inline s_list_state& _tech_review_list_header =
        *reinterpret_cast<s_list_state*>(std::uintptr_t{0x00555ED0});

    inline int16_t& _tech_review_tab_index =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x00555F10});

    inline void*& _g_tech_image_seg =
        *reinterpret_cast<void**>(std::uintptr_t{0x005A7B80});

    inline int16_t& _g_last_list_field =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A7B84});

    inline uint16_t& _g_selected_app_id =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A7B86});

    inline s_list_state*& _g_header_p =
        *reinterpret_cast<s_list_state**>(std::uintptr_t{0x005A7B88});

    inline void*& _g_bitmap1_p =
        *reinterpret_cast<void**>(std::uintptr_t{0x005A7B8C});

    inline void*& _g_bitmap2_p =
        *reinterpret_cast<void**>(std::uintptr_t{0x005A7B90});

    inline int16_t& _g_prior_choice =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A7B94});

    inline s_list_item*& _g_selected_list_p =
        *reinterpret_cast<s_list_item**>(std::uintptr_t{0x005A7B98});

    inline uint8_t& _g_list_update_needed =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A7B9C});

    inline int16_t& _g_first_list_field =
        *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A7D94});

    inline s_colors& _info_red_color =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x00555B98});

    inline uint8_t (&_sub_scr_field)[10] =
        *reinterpret_cast<uint8_t(*)[10]>(std::uintptr_t{0x00555CA0});

    inline uint8_t& _g_reference_screen_type =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00564D78});

    inline uint8_t& _g_turn_message_ix =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00564D7C});

    inline uint8_t& _race_statistics_page =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A7B9D});

    inline uint16_t& _g_exit_field =
        *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A7B9E});

    int16_t __cdecl Get_Plyr_Info_Btns_();

    int16_t __cdecl Initialize_Maintenance_Values_(int32_t* values);

    void __cdecl Build_Maint_Chart_Bitmaps_(s_animation_header** sprites, s_animation_header** bars, s_animation_header** labels, int32_t* values);

    inline uint8_t& _info_dark_color = 
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00555BA0});

    inline s_colors* (&_maintenance_bar_colors)[7] = 
        *reinterpret_cast<s_colors*(*)[7]>(std::uintptr_t{0x00555BE8});

    inline s_colors* (&PTR__orange_text_colors)[8] = 
        *reinterpret_cast<s_colors*(*)[8]>(std::uintptr_t{0x00555C40});

    inline s_colors (&_hist_palettes)[8] = 
        *reinterpret_cast<s_colors(*)[8]>(std::uintptr_t{0x00555C60});

    inline s_colors* (&_hist_graph_palettes)[8] = 
        *reinterpret_cast<s_colors*(*)[8]>(std::uintptr_t{0x00564D80});

    void __cdecl Colorize_Bitmap_(s_animation_header* anim, s_colors* color_table, uint8_t range_start, uint8_t range_end);

    inline const char* s_billtext_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x0055135C}); // "billtext.lbx"
    inline const char* lbx_billtext = reinterpret_cast<const char*>(std::uintptr_t{0x0055135C});
    inline const char* empty_string = reinterpret_cast<const char*>(std::uintptr_t{0x00551100});
    inline s_drop_area* _history_graph_drops = reinterpret_cast<s_drop_area*>(std::uintptr_t{0x00555CC0});
    inline int16_t* _graph_button_x = reinterpret_cast<int16_t*>(std::uintptr_t{0x00555E48});

    inline const char* s_pct_fmt = reinterpret_cast<const char*>(std::uintptr_t{0x0055600C}); // "%3i%%"
    inline const char* s_int_fmt = reinterpret_cast<const char*>(std::uintptr_t{0x00556014}); // "%i"

    inline int8_t (&_graph_offsets)[10] = *reinterpret_cast<int8_t(*)[10]>(std::uintptr_t{0x00555E38});
    inline const char* s_fmt_comma = reinterpret_cast<const char*>(std::uintptr_t{0x00555FF0}); // "%i,%i"
    inline const char* s_fmt_dot = reinterpret_cast<const char*>(std::uintptr_t{0x00555FF8});   // "%i.%i"

    void __cdecl Draw_Maint_Income_Chart_(s_animation_header** chart_bars, s_animation_header** chart_labels, int16_t val);

    int16_t __cdecl History_Subscreen_(s_animation_header** lbx_images, int16_t exit_threshold);

    void __cdecl Set_Up_Player_Colors_(s_animation_header** anim_ptrs);

    int32_t __cdecl Smooth_History_Data_(int32_t* data, int16_t start_idx, int16_t end_idx);

    void __cdecl Get_Plyr_Hist_Btns_(int16_t* btns);

    void __cdecl Set_Plyr_Hist_Btns_(int16_t* btns_state);

    void __cdecl Draw_History_Subscreen_();

    void __cdecl Draw_Drop_Area_(s_drop_area* area);

    void __cdecl Draw_Multiple_Drop_Areas_(s_drop_area* areas, uint32_t count);

    void __cdecl Display_Graphed_Players_Names_();

    int16_t __cdecl Draw_The_History_Graph_();

    void __cdecl Draw_Histories_(int16_t offset_x);

    void __cdecl Introduce_Subscreen_();

    void __cdecl Draw_Tech_Review_Subscreen_();

    void __cdecl Init_Tech_Review_List_(s_list_state* state, int16_t review_idx);

    int16_t __cdecl _Tech_Review_Subscreen_(s_animation_header** param_1, int16_t param_2, int16_t param_3);

    int16_t __cdecl Tech_Review_Subscreen_(s_animation_header** headers, int16_t param_2);

    inline int16_t __cdecl Race_Stats_Subscreen_(s_animation_header** info_lbx, int16_t counter) {
        using Fn = int16_t(__cdecl*)(s_animation_header**, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00427C60 });
        return fn(info_lbx, counter);
    }

    inline int16_t __cdecl Turn_Summary_Subscreen_(s_animation_header** info_lbx, int16_t counter) {
        using Fn = int16_t(__cdecl*)(s_animation_header**, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00428600 });
        return fn(info_lbx, counter);
    }

    inline int16_t __cdecl Reference_Subscreen_(s_animation_header** info_lbx, int16_t counter) {
        using Fn = int16_t(__cdecl*)(s_animation_header**, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00428A30 });
        return fn(info_lbx, counter);
    }

    void __cdecl Info_Screen_();
}