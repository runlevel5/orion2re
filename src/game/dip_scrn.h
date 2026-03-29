#pragma once
#include <cstdint>

namespace DIP_SCRN {
    inline s_mouse_list& _diplomacy_hand_mouse = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x005569A8});
    inline s_mouse_list& _diplomacy_blank_mouse = *reinterpret_cast<s_mouse_list*>(std::uintptr_t{0x00552318});
    inline uint8_t& _ambassador_option = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A81BC});
    inline int16_t& _main_diplomacy_help_entry = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x00552280 });
    inline int16_t& _response_message = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A81CC});
    inline uint16_t& _synch_up_established_flag = *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A81CE});
    inline int16_t& _old_response_message = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A81D0});
    inline int16_t* _propose_treaty_help_entry = reinterpret_cast<int16_t*>(std::uintptr_t{0x005522D0});
    inline char*& _main_diplomacy_title = *reinterpret_cast<char**>(std::uintptr_t{ 0x005A82E8 });
    inline char*& _propose_treaty_title = *reinterpret_cast<char**>(std::uintptr_t{0x005A85DC});
    inline int8_t& _talker_ambassador = *reinterpret_cast<int8_t*>(std::uintptr_t{0x005A8378});
    inline int16_t& _diplomacy_current_music = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A89EC});
    inline int16_t& _net_diplomacy_esc_pressed = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A89FC});
    inline uint8_t (&_target_tech_list)[212] = *reinterpret_cast<uint8_t(*)[212]>(std::uintptr_t{ 0x005A85E0 });
    inline uint8_t& _listen_ambassador = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A8A06});
    inline int16_t& _exchange_tech_count = *reinterpret_cast<int16_t*>(std::uintptr_t{ 0x005A89D8 });
    inline char* (&_main_diplomacy_list)[9] = *reinterpret_cast<char* (*)[9]>(std::uintptr_t{ 0x005A8A10 });
    inline char* (&_propose_treaty_diplomacy_list)[7] = *reinterpret_cast<char*(*)[7]>(std::uintptr_t{0x005A8A50});
    inline int16_t (&_exchange_tech_list)[100] = *reinterpret_cast<int16_t(*)[100]>(std::uintptr_t{ 0x005A8A70 });
    inline char (&_chat_message)[100] = *reinterpret_cast<char (*)[100]>(std::uintptr_t{0x005A82F0});
    inline int16_t& _diplomacy_quit_chat_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A8A00});
    inline int16_t& _diplomacy_chat_field = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A8A40});
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
    inline const char* s__00551100 = (const char*)std::uintptr_t{0x00551100}; // ""
    inline const char* s_jimtext2_lbx_00552340 = (const char*)std::uintptr_t{0x00552340}; // "jimtext2.lbx"
    inline const char* s_jimtext2_lbx = reinterpret_cast<const char*>(std::uintptr_t{0x00552340});
    inline const char* s_jimtext_lbx_00552360 = (const char*)std::uintptr_t{0x00552360}; // "jimtext.lbx"
    inline const char* s__d_BC_00552384 = (const char*)std::uintptr_t{0x00552384}; // "  %d BC"

    inline uint16_t& _chat_width =
            *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A85D8});

    inline uint16_t& _chat_base_y =
            *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A89DA});

    inline uint16_t& _chat_base_x =
            *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A89DC});

    inline uint16_t& _chat_height =
            *reinterpret_cast<uint16_t*>(std::uintptr_t{0x005A89FE});

    inline uint8_t& _chat_mode_flag =
            *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A8A02});

    inline s_colors& _diplomacy_normal_colors =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x00552268});

    inline s_colors& _diplomacy_highlight_colors =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x00552270});

    inline s_colors& _diplomacy_special_colors =
        *reinterpret_cast<s_colors*>(std::uintptr_t{0x00552278});

    inline int16_t (&_break_treaty_help_entry)[7] =
        *reinterpret_cast<int16_t(*)[7]>(std::uintptr_t{0x005522E0});

    inline char*& _break_treaty_title = *reinterpret_cast<char**>(std::uintptr_t{0x005A8354});

    inline char* (&_break_treaty_diplomacy_list)[7] =
        *reinterpret_cast<char*(*)[7]>(std::uintptr_t{0x005A86C0});

    inline char (&_diplomacy_list_choice)[15][50] =
        *reinterpret_cast<char (*)[15][50]>(std::uintptr_t{0x005A86E0});

    inline s_dipscrn_help_item (&_jim_help_list)[15] =
        *reinterpret_cast<s_dipscrn_help_item(*)[15]>(std::uintptr_t{0x005A8250});

    inline uint8_t& DAT_005a8712 = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A8712});
    inline uint8_t& DAT_005a8744 = *reinterpret_cast<uint8_t*>(std::uintptr_t{0x005A8744});

    inline bool __cdecl Check_Better_Offer_Available_(int16_t player_a, int16_t player_b) {
        using Fn = bool(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00418490});
        return fn(player_a, player_b);
    }

    void __cdecl Get_Human_Propose_Treaty_Message_(int16_t treaty_type);

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
    void __cdecl Get_Main_Repulsive_Diplomacy_Choices_(int16_t param_1, int16_t param_2, int8_t param_3);
    void __cdecl Diplomacy_Display_Response_();

    void __cdecl Draw_Diplomacy_Synch_Mode_();

    void __cdecl Diplomacy_Set_Main_Options_(int16_t player_idx, int16_t target_idx, uint8_t* options);

    void __cdecl Diplomacy_Propose_Treaty_(int16_t player_idx, int16_t target_idx);

    int16_t __cdecl Diplomacy_Determine_Treaty_Proposal_(int16_t proposer_idx, int16_t target_idx, int16_t treaty_type);

    void __cdecl Diplomacy_Break_Treaty_(int16_t param_1, int16_t param_2);

    void __cdecl Diplomacy_Break_Treaty_Resolution_(int16_t sender_id, int16_t target_id, int16_t treaty_type);

    inline void __cdecl Diplomacy_Demand_(int16_t player_num, int16_t ambassador_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x004156f0 });
        fn(player_num, ambassador_idx);
    }

    void __cdecl Diplomacy_Offer_Gift_(int16_t player_num, int16_t ambassador_idx);

    inline void __cdecl Diplomacy_Exchange_Technology_(int16_t player_num, int16_t ambassador_idx) {
        using Fn = void(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00419f70 });
        fn(player_num, ambassador_idx);
    }

    int16_t __cdecl Diplomacy_Declare_War_Resolution_(int16_t sender_id, int16_t target_id);

    int16_t __cdecl Diplomacy_Surrender_(int16_t sender_player_id, int16_t target_player_id);

    void __cdecl Get_Human_Surrender_Message_(int16_t race_idx);

    void __cdecl Add_Diplomacy_Turn_Summary_Messages_();

    void __cdecl Diplomacy_Mouse_Init_(int16_t param_1);

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

    inline int16_t* _main_repulsive_diplomacy_help_entry = reinterpret_cast<int16_t*>(std::uintptr_t{0x005522B0});
    inline int16_t* _main_net_repulsive_diplomacy_help_entry = reinterpret_cast<int16_t*>(std::uintptr_t{0x005522C0});
    inline char* (&_main_repulsive_diplomacy_list)[5] = *reinterpret_cast<char* (*)[5]>(std::uintptr_t{0x005A8230});
    inline char* (&_main_net_repulsive_diplomacy_list)[6] = *reinterpret_cast<char* (*)[6]>(std::uintptr_t{0x005A85B0});
    inline s_dipscrn_net_message& _net_diplomacy_message = *reinterpret_cast<s_dipscrn_net_message*>(std::uintptr_t{0x005A8370});

    void __cdecl Diplomacy_Main_Net_Chat_(int16_t param_1, int16_t param_2);

    void __cdecl Draw_Diplomacy_Listen_Mode_();

    void __cdecl Diplomacy_Chat_(int16_t player_idx, uint8_t target_player_id);

    int16_t __cdecl Check_Human_Treaty_Proposal_(int16_t player_id, int16_t target_player_id, int16_t treaty_type);

    int16_t __cdecl Get_Human_Treaty_Choice_(int16_t player_a, int16_t player_b, int16_t treaty_type);

    void __cdecl Get_Listen_Mode_Message_(int16_t race_idx);

    void __cdecl Diplomacy_Listen_Mode_(int16_t player_idx, uint8_t target_player_id);

    int16_t __cdecl Diplomacy_Need_Better_Offer_(int16_t type, int16_t tech_app, int16_t bc, int16_t star_idx, int16_t tribute);

    inline int16_t __cdecl Diplomacy_Human_Need_Better_Offer_(int16_t proposer_idx, int16_t target_idx) {
        using Fn = int16_t(__cdecl*)(int16_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00419020});
        return fn(proposer_idx, target_idx);
    }

    void __cdecl Get_Human_Form_Treaty_Message_(int16_t proposer_idx, int16_t target_idx, int16_t treaty_type, int16_t msg_type);

    void __cdecl Get_Human_Declare_War_Message_(int16_t race_idx);

    void __cdecl Get_Diplomacy_Demand_Treaty_List_(int16_t player1, int16_t player2, int16_t treaty_type, int16_t* out_count, uint8_t* out_list);

    void __cdecl Get_Diplomacy_Tech_List_(int16_t player_a_idx, int16_t player_b_idx, int16_t* tech_count_ptr, uint8_t* tech_list_buffer);

    int16_t __cdecl Diplomacy_Generic_List_(int16_t item_count, char* list_strings, int8_t add_cancel, char* title);

    inline int16_t& _offer_help_entry = *reinterpret_cast<int16_t*>(std::uintptr_t{0x00552308});
    inline char* (&_offer_diplomacy_list)[7] = *reinterpret_cast<char*(*)[7]>(std::uintptr_t{0x005A81E0});
    inline char*& _offer_title = *reinterpret_cast<char**>(std::uintptr_t{0x005A86B4});

    inline char* (&_demand_diplomacy_list)[11] = *reinterpret_cast<char*(*)[11]>(std::uintptr_t{0x005A8190});
    inline char*& _net_listen_mode_option_buffer = *reinterpret_cast<char**>(std::uintptr_t{0x005A81C0});
    inline char*& _system_unsurrenderable_buffer = *reinterpret_cast<char**>(std::uintptr_t{0x005A81C4});
    inline char* (&_main_net_diplomacy_list)[11] = *reinterpret_cast<char*(*)[11]>(std::uintptr_t{0x005A8200});
    inline char*& _demand_title = *reinterpret_cast<char**>(std::uintptr_t{0x005A82EC});
    inline char* (&_agree_diplomacy_list)[4] = *reinterpret_cast<char*(*)[4]>(std::uintptr_t{0x005A8360});
    inline char*& _system_cancel_buffer = *reinterpret_cast<char**>(std::uintptr_t{0x005A837C});
    inline uint8_t*& _diplomacy_system_star_background_seg = *reinterpret_cast<uint8_t**>(std::uintptr_t{0x005A84FC});
    inline uint8_t*& _diplomacy_mouse_bitmap = *reinterpret_cast<uint8_t**>(std::uintptr_t{0x005A8500});
    inline char*& _system_homeworld_buffer = *reinterpret_cast<char**>(std::uintptr_t{0x005A85A8});
    inline int16_t& _diplomacy_good_music = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A85CA});
    inline char*& _statement_research_treaty_buffer = *reinterpret_cast<char**>(std::uintptr_t{0x005A85CC});
    inline int16_t& _diplomacy_bad_music = *reinterpret_cast<int16_t*>(std::uintptr_t{0x005A85D2});
    inline char*& _choose_a_system_buffer = *reinterpret_cast<char**>(std::uintptr_t{0x005A85D4});
    inline char*& _enter_your_message_buffer = *reinterpret_cast<char**>(std::uintptr_t{0x005A89D0});
    inline char*& _statement_trade_treaty_buffer = *reinterpret_cast<char**>(std::uintptr_t{0x005A89E0});
    inline char*& _system_million_colonists_buffer = *reinterpret_cast<char**>(std::uintptr_t{0x005A89E8});
    inline char*& _net_listen_mode_good_bye_buffer = *reinterpret_cast<char**>(std::uintptr_t{0x005A89F4});
    inline char*& _system_x_star_system_buffer = *reinterpret_cast<char**>(std::uintptr_t{0x005A89F8});
    inline char*& _percent_tribute_treaty_buffer = *reinterpret_cast<char**>(std::uintptr_t{0x005A8A34});
    inline char*& _you_were_framed_buffer = *reinterpret_cast<char**>(std::uintptr_t{0x005A8A3C});

    inline void __cdecl Load_Diplomacy_Text_Messages_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0041cc00});
        fn();
    }

    bool __cdecl Check_Surrender_System_(int16_t player_a, int16_t player_b);

    void __cdecl Get_Human_Offer_Gift_Message_(int16_t player_idx, int16_t offer_type, int16_t value);

    void __cdecl Diplomacy_Load_New_Ambassador_(int16_t player_idx);

    void __cdecl Get_Human_Break_Treaty_Message_(int16_t race_idx, int16_t treaty_type);

    void __cdecl Get_Human_Gift_Response_(int16_t sender_id, int16_t target_id, int16_t gift_type, int16_t gift_arg);

    inline int16_t __cdecl Diplomacy_System_Display_(int16_t param_1, uint8_t ambassador, int16_t flag) {
        using Fn = int16_t(__cdecl*)(int16_t, uint8_t, int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00419800});
        return fn(param_1, ambassador, flag);
    }
    int16_t __cdecl Diplomacy_Offer_Money_(int16_t player_idx, int16_t target_idx, int8_t do_transaction);

    void __cdecl Start_Diplomacy_Music_(int16_t music_type);

    int16_t __cdecl Diplomacy_System_Scan_(int16_t star_idx, int16_t player_idx, int16_t* artemis_planet_idx, int16_t* owned_count, int8_t* planet_indices);
}