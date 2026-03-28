#pragma once
#include <cstdint>

namespace DIP_SCRN {
    inline s_mouse_list& _diplomacy_hand_mouse = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x005569A8});
    inline s_mouse_list& _diplomacy_blank_mouse = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x00552318});
    inline uint8_t& _ambassador_option = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A81BC});
    inline int16_t& _main_diplomacy_help_entry = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00552280 });
    inline uint16_t& _response_message = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A81CC});
    inline uint16_t& _synch_up_established_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A81CE});
    inline int16_t& _old_response_message = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A81D0});
    inline int16_t* _propose_treaty_help_entry = reinterpret_cast<int16_t*>(std::uintptr_t{0x005522D0});
    inline char*& _main_diplomacy_title = *reinterpret_cast<char**>(std::uintptr_t{ 0x005A82E8 });
    inline char*& _propose_treaty_title = *reinterpret_cast<char**>(std::uintptr_t{0x005A85DC});
    inline int8_t& _talker_ambassador = *reinterpret_cast<int8_t*>(std::uintptr_t{0x005A8378});
    inline int16_t& _diplomacy_current_music = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A89EC});
    inline uint16_t& _net_diplomacy_esc_pressed = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A89FC});
    inline uint8_t (&_target_tech_list)[212] = *reinterpret_cast<uint8_t(*)[212]>(std::uintptr_t{ 0x005A85E0 });
    inline uint8_t& _listen_ambassador = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A8A06});
    inline int16_t& _exchange_tech_count = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005A89D8 });
    inline char* (&_main_diplomacy_list)[15] = *reinterpret_cast<char* (*)[15]>(std::uintptr_t{ 0x005A8A10 });
    inline char* (&_propose_treaty_diplomacy_list)[7] = *reinterpret_cast<char*(*)[7]>(std::uintptr_t{0x005A8A50});
    inline int16_t (&_exchange_tech_list)[100] = *reinterpret_cast<int16_t(*)[100]>(std::uintptr_t{ 0x005A8A70 });
    inline char (&_net_diplomacy_chat_message)[100] = *reinterpret_cast<char(*)[100]>(std::uintptr_t{0x005A8B50});
    inline char*& _player_number_name_buffer = *reinterpret_cast<char**>(std::uintptr_t{0x005A89F0});
    inline char*& _player_name_ambassador_buffer = *reinterpret_cast<char**>(std::uintptr_t{0x005A81C8});
    inline char (&_receive_chat_message)[110] = *reinterpret_cast<char (*)[110]>(std::uintptr_t{0x005A8390});
    inline uint8_t& _current_ambassador = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A8BB4});
    inline char* _diplomacy_message = reinterpret_cast<char*>(std::uintptr_t{0x005A8400});
    inline const char* s_empty_string_00551100 = reinterpret_cast<const char*>(std::uintptr_t{0x00551100});
    inline const char* s_empty_string = reinterpret_cast<const char*>(std::uintptr_t{ 0x00551100 });
    inline const char* s_empty = reinterpret_cast<const char*>(0x00551100);
    inline const char* s_jimtext_lbx = reinterpret_cast<const char*>(0x00552360);

    inline s_colors& _diplomacy_normal_colors =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x00552268});

    inline s_colors& _diplomacy_highlight_colors =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x00552270});

    inline s_colors& _diplomacy_special_colors =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x00552278});

    inline char (&_diplomacy_list_choice)[15][50] =
        *reinterpret_cast<char (*)[15][50]>(std::uintptr_t{0x005A86E0});

    inline s_dipscrn_help_item (&_jim_help_list)[15] =
        *reinterpret_cast<s_dipscrn_help_item(*)[15]>(std::uintptr_t{0x005A8250});

    inline uint8_t& DAT_005a8712 = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A8712});
    inline uint8_t& DAT_005a8744 = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A8744});

    void Resolve_Delayed_Diplomacy_Orders_();

    void __cdecl Set_Opportunity_Attacks_();

    void __cdecl Diplomacy_Screen_(uint8_t current_ambassador);

    void __cdecl Diplomacy_Fade_In_();
    inline void __cdecl Get_Net_Diplomacy_Choices_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041B2E0});
        fn();
    }
    void __cdecl Get_Main_Diplomacy_Choices_();
    inline void __cdecl Get_Main_Repulsive_Diplomacy_Choices_(int16_t player_num, uint32_t ambassador, int32_t param_3) {
        using Fn = void(__cdecl*)(int16_t, uint32_t, int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00413A30});
        fn(player_num, ambassador, param_3);
    }
    void __cdecl Diplomacy_Display_Response_();

    void __cdecl Draw_Diplomacy_Synch_Mode_();

    void __cdecl Diplomacy_Set_Main_Options_(int16_t player_idx, int16_t target_idx, uint8_t* options);

    void __cdecl Diplomacy_Propose_Treaty_(int16_t player_idx, int16_t target_idx);

    inline int16_t __cdecl Diplomacy_Determine_Treaty_Proposal_(int16_t player_idx, int16_t target_idx, int16_t treaty_type) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00416530});
        return fn(player_idx, target_idx, treaty_type);
    }

    inline void __cdecl Diplomacy_Break_Treaty_(int16_t player_num, int16_t ambassador_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00414a30 });
        fn(player_num, ambassador_idx);
    }

    inline void __cdecl Diplomacy_Demand_(int16_t player_num, int16_t ambassador_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004156f0 });
        fn(player_num, ambassador_idx);
    }

    inline void __cdecl Diplomacy_Offer_Gift_(int16_t player_num, int16_t ambassador_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00415ff0 });
        fn(player_num, ambassador_idx);
    }

    inline void __cdecl Diplomacy_Exchange_Technology_(int16_t player_num, int16_t ambassador_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00419f70 });
        fn(player_num, ambassador_idx);
    }

    inline int16_t __cdecl Diplomacy_Declare_War_Resolution_(int16_t player_num, int16_t ambassador_idx) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00414300 });
        return fn(player_num, ambassador_idx);
    }

    inline int16_t __cdecl Diplomacy_Surrender_(int16_t player_num, int16_t ambassador_idx) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0041a4b0 });
        return fn(player_num, ambassador_idx);
    }

    void __cdecl Add_Diplomacy_Turn_Summary_Messages_();

    inline void __cdecl Diplomacy_Mouse_Init_(uint16_t ambassador_idx) {
        using Fn = void(__cdecl*)(uint16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00418B90});
        fn(ambassador_idx);
    }

    inline void __cdecl Get_Diplomacy_Statement_(uint16_t response, int16_t player_num, uint16_t ambassador, char* message_buffer) {
        using Fn = void(__cdecl*)(uint16_t, int16_t, uint16_t, char*);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041A8E0});
        fn(response, player_num, ambassador, message_buffer);
    }

    void __cdecl Setup_Back_Page_();

    void __cdecl Draw_Diplomacy_Fade_In_Screen_();

    void __cdecl Setup_Ambassador_Pic_(int16_t is_animated);

    void __cdecl Npc_Diplomacy_Screen_(int16_t player_idx, int8_t is_sneak_attack);

    int16_t __cdecl Diplomacy_Generic_Exchange_Tech_List_(int16_t tech_app_player, int16_t unused_2, char* list_title, int8_t* tech_ids, int16_t num_techs, int8_t mode);

    int16_t __cdecl Get_Non_Player_Proposals_();

    void __cdecl Accepted_Npc_Demand_Resolution_(int16_t player_id, int16_t ambassador_id);
    void __cdecl Npc_Give_Gift_(int16_t player_idx, int16_t target_idx);

    void __cdecl Show_Sneak_Attack_Message_(int16_t player_idx, int16_t defender_idx);

    void __cdecl Setup_Diplomacy_Help_(int16_t count, int16_t* help_ids);

    void __cdecl Setup_Full_Screen_Diplomacy_Help_(int16_t help_id);
}