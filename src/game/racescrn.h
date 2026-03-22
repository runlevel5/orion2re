#pragma once
#include <cstdint>

struct s_racescrn_race_display;
struct s_racescrn_mouse_group;
struct s_animation_header;

namespace RACESCRN {

    inline const char* s__005510f0 = reinterpret_cast<const char*>(std::uintptr_t{0x005510F0});
    inline char* s_billtext_lbx_0055135c = reinterpret_cast<char*>(std::uintptr_t{0x0055135C});
    inline char* s_billtex2_lbx_00556038 = reinterpret_cast<char*>(std::uintptr_t{0x00556038});
    inline char* s_races_lbx_005574e8 = reinterpret_cast<char*>(std::uintptr_t{0x005574E8});

    inline s_racescrn_icon_layout& _mouse_group_data = *reinterpret_cast<s_racescrn_icon_layout*>(std::uintptr_t{0x005581E8});
    inline s_racescrn_mouse_group*& _g_agent_group_p = *reinterpret_cast<s_racescrn_mouse_group**>(std::uintptr_t{0x00566554});
    inline s_racescrn_mouse_group*& _g_mouse_group_p = *reinterpret_cast<s_racescrn_mouse_group**>(std::uintptr_t{0x00566558});
    inline int32_t& _net_state = *reinterpret_cast<int32_t*>(std::uintptr_t{0x0056655C});
    inline s_racescrn_race_display*& _g_display_p = *reinterpret_cast<s_racescrn_race_display**>(std::uintptr_t{0x00566560});
    inline uint8_t& _g_in_who_mode = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00566564});
    inline s_animation_header*& _g_mouse_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00566568});
    inline uint8_t& _g_in_spy_mode = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x00566570});
    inline s_animation_header*& _arrow_seg = *reinterpret_cast<s_animation_header**>(std::uintptr_t{0x00566574});

    inline char* _trade_treaty_buffer = reinterpret_cast<char*>(std::uintptr_t{0x005A5330});
    inline char* _ignoring_str = reinterpret_cast<char*>(std::uintptr_t{0x005A5350});
    inline uint32_t& _netbox_seg = *reinterpret_cast<uint32_t*>(std::uintptr_t{0x005A5364});
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
    inline int16_t* _race_bar_y = reinterpret_cast<int16_t*>(std::uintptr_t{0x0055814A});
    inline s_racescrn_icon_layout& _agent_group_data = *reinterpret_cast<s_racescrn_icon_layout*>(std::uintptr_t{0x005581D8});
    inline const char* races_lbx = "races.lbx";
    inline char* _receiving_treaty_buffer = reinterpret_cast<char*>(std::uintptr_t{0x005A55C0});
    inline char* _diplomacy_net_wait_buffer = reinterpret_cast<char*>(std::uintptr_t{0x005A5610});
    inline char* _diplomacy_net_requeting_buffer = reinterpret_cast<char*>(std::uintptr_t{0x005A5680});

    void __cdecl Init_Race_Display_Data_(s_racescrn_race_display* race_display, s_racescrn_mouse_group* agent_group);

    inline void __cdecl Setup_Icon_Group_Data_(s_racescrn_mouse_group* dest, s_racescrn_icon_layout* src, int16_t count, s_animation_header* sprite, s_animation_header* unused) {
        using Fn = void(__cdecl*)(s_racescrn_mouse_group*, s_racescrn_icon_layout*, int16_t, s_animation_header*, s_animation_header*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0049DB80});
        fn(dest, src, count, sprite, unused);
    }
    inline void __cdecl Draw_Race_Photos_(s_racescrn_race_display* display) {
        using Fn = void(__cdecl*)(s_racescrn_race_display*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0049C900})(display);
    }
    inline void __cdecl Draw_Race_Text_(s_racescrn_race_display* display) {
        using Fn = void(__cdecl*)(s_racescrn_race_display*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0049CB50})(display);
    }
    inline void __cdecl Draw_Relations_Sliders_(s_racescrn_race_display* display) {
        using Fn = void(__cdecl*)(s_racescrn_race_display*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0049C840})(display);
    }
    inline int16_t __cdecl Setup_Main_Fields_(int16_t* exit_field, int16_t* btn_diplomacy, int16_t* state_diplomacy, int16_t* btn_report, int16_t* state_report, int16_t* btn_ignoring, int16_t* state_ignoring, int16_t* btn_war, int16_t* state_war) {
        using Fn = int16_t(__cdecl*)(int16_t*, int16_t*, int16_t*, int16_t*, int16_t*, int16_t*, int16_t*, int16_t*, int16_t*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0049D160})(exit_field, btn_diplomacy, state_diplomacy, btn_report, state_report, btn_ignoring, state_ignoring, btn_war, state_war);
    }
    inline void __cdecl Setup_Race_Display_Fields_(s_racescrn_race_display* display, int16_t base_field) {
        using Fn = void(__cdecl*)(s_racescrn_race_display*, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0049D300})(display, base_field);
    }
    inline void __cdecl Draw_Race_Screen_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0049D5B0})();
    }
    inline void __cdecl Move_Spy_Group_From_Mouse_(s_racescrn_mouse_group* grp) {
        using Fn = void(__cdecl*)(s_racescrn_mouse_group*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0049D990})(grp);
    }
    inline void __cdecl Setup_Race_Who_Fields_(s_racescrn_race_display* display, int16_t base_field) {
        using Fn = void(__cdecl*)(s_racescrn_race_display*, int16_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0049D4A0})(display, base_field);
    }
    inline s_racescrn_mouse_group* __cdecl Get_Spy_Group_For_Field_(int16_t field, int16_t* race_idx) {
        using Fn = s_racescrn_mouse_group*(__cdecl*)(int16_t, int16_t*);
        return reinterpret_cast<Fn>(std::uintptr_t{0x0049D900})(field, race_idx);
    }
    inline void __cdecl Move_Spies_Group_(s_racescrn_mouse_group* grp, s_animation_header* sprite) {
        using Fn = void(__cdecl*)(s_racescrn_mouse_group*, s_animation_header*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0049DA90})(grp, sprite);
    }
    inline void __cdecl Adjust_Spy_Mission_Data_() {
        using Fn = void(__cdecl*)();
        reinterpret_cast<Fn>(std::uintptr_t{0x0049D960})();
    }
    inline void __cdecl Update_Spy_Stuff_(s_racescrn_race_display* display) {
        using Fn = void(__cdecl*)(s_racescrn_race_display*);
        reinterpret_cast<Fn>(std::uintptr_t{0x0049D540})(display);
    }

    void __cdecl Race_Screen_();
}