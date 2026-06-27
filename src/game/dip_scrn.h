#pragma once
#include <cstdint>

struct s_dipscrn_net_message;

namespace DIP_SCRN {
    extern s_mouse_list _diplomacy_hand_mouse;
    extern s_mouse_list _diplomacy_blank_mouse;
    extern uint8_t _ambassador_option;
    extern int16_t _main_diplomacy_help_entry[8];
    extern int16_t _response_message;
    extern uint16_t _synch_up_established_flag;
    extern int16_t _old_response_message;
    extern int16_t _propose_treaty_help_entry[7];
    extern char* _main_diplomacy_title;
    extern char* _propose_treaty_title;
    extern uint8_t _talker_ambassador;
    extern int16_t _diplomacy_current_music;
    extern int16_t _net_diplomacy_esc_pressed;
    extern uint8_t _target_tech_list[TECH_APP_COUNT];
    extern uint8_t _listen_ambassador;
    extern int16_t _exchange_tech_count;
    extern char* _main_diplomacy_list[9];
    extern char* _propose_treaty_diplomacy_list[7];
    extern uint8_t _exchange_tech_list[TECH_APP_COUNT];
    extern char _chat_message[100];
    extern int16_t _diplomacy_quit_chat_field;
    extern int16_t _net_diplomacy_treaty_type;
    extern int16_t _diplomacy_chat_field;
    extern char _net_diplomacy_chat_message[100];
    extern char* _player_number_name_buffer;
    extern char* _player_name_ambassador_buffer;
    extern char _receive_chat_message[110];
    extern uint8_t _current_ambassador;
    extern char _diplomacy_message[250];
    extern int16_t _main_net_diplomacy_help_entry[10];
    extern int16_t _net_diplomacy_tech_offered;
    extern const char s_empty_string_00551100[];
    extern const char s_empty_string[];
    extern const char s_empty[];
    extern const char s_jimtext_lbx[];
    extern const char s__00551100[];
    extern const char s_jimtext2_lbx_00552340[];
    extern const char s_jimtext2_lbx[];
    extern const char s_jimtext_lbx_00552360[];
    extern const char s__d_BC_00552384[];
    extern const char s_fmt_s_0055236c[];
    extern const char s__s_0055236c[];
    extern const char s_fmt_d_bc_00552384[];
    extern const char s_fmt_BC[];
    extern const char s_fmt_comma[];
    extern const char s_fmt_dot[];
    extern const char s_FWEE[];
    extern const char s_DIPLOMSP_LBX[];
    extern const char s_DIPLOMSI_LBX[];
    extern const char s_DIPLOMSS_LBX[];
    extern const char s_DIPLOMSF_LBX[];
    extern const char s_DIPLOMSG_LBX[];
    extern const char s_DIPLOMSE_LBX[];

    const char* __cdecl An_(char* text);

    int8_t __cdecl Diplomacy_Message_Is_Interactive_(int16_t msg_id);

    extern uint16_t _chat_width;

    extern uint16_t _chat_base_y;

    extern uint16_t _chat_base_x;

    extern uint16_t _chat_height;

    extern uint8_t _chat_mode_flag;

    extern s_colors _diplomacy_normal_colors;

    extern s_colors _diplomacy_highlight_colors;

    extern s_colors _diplomacy_special_colors;

    extern int16_t _break_treaty_help_entry[7];

    extern char* _break_treaty_title;

    extern char* _break_treaty_diplomacy_list[7];

    extern char _diplomacy_list_choice[15][50];

    extern s_help_box _jim_help_list[15];

    int16_t __cdecl Check_Better_Offer_Available_(int16_t player_idx, int16_t other_player_idx);

    void __cdecl Get_Human_Propose_Treaty_Message_(int16_t treaty_type);

    void Resolve_Delayed_Diplomacy_Orders_();

    void __cdecl Set_Opportunity_Attacks_();

    void __cdecl Diplomacy_Screen_(int player_idx);

    void __cdecl Diplomacy_Fade_In_();

    void __cdecl Get_Net_Diplomacy_Choices_();
    void __cdecl Get_Main_Diplomacy_Choices_();
    void __cdecl Get_Main_Repulsive_Diplomacy_Choices_(int16_t speaker_player_id, int16_t listener_player_id, int8_t net_mode);
    void __cdecl Diplomacy_Display_Response_();

    void __cdecl Draw_Diplomacy_Synch_Mode_();

    void __cdecl Diplomacy_Set_Main_Options_(int16_t player_idx, int16_t target_idx, uint8_t* options);

    void __cdecl Diplomacy_Propose_Treaty_(int16_t player_idx, int16_t target_idx);

    int16_t __cdecl Diplomacy_Determine_Treaty_Proposal_(int16_t proposer_idx, int16_t target_idx, int16_t treaty_type);

    void __cdecl Diplomacy_Break_Treaty_(int16_t player_idx, int16_t target_idx);

    void __cdecl Diplomacy_Break_Treaty_Resolution_(int16_t sender_id, int16_t target_id, int16_t treaty_type);

    void __cdecl Diplomacy_Demand_(int16_t player_num, int16_t ambassador_idx);

    extern int16_t _demand_help_entry[11];

    void __cdecl Get_Human_Demand_Response_(int16_t player_id, int16_t target_player_id, int16_t demand_type, int16_t demand_arg, int16_t* out_choice);

    void __cdecl Diplomacy_Give_In_To_Demand_(int16_t asking_player_idx, int16_t giving_player_idx, int16_t demand_type, int16_t demand_data);

    void __cdecl Diplomacy_Offer_Gift_(int16_t player_num, int16_t ambassador_idx);

    void __cdecl Diplomacy_Exchange_Technology_(int16_t player_1, int16_t player_2);

    int16_t __cdecl Diplomacy_Declare_War_Resolution_(int16_t sender_id, int16_t target_id);

    int16_t __cdecl Diplomacy_Surrender_(int16_t sender_player_id, int16_t target_player_id);

    void __cdecl Get_Human_Surrender_Message_(int16_t race_idx);

    void __cdecl Add_Diplomacy_Turn_Summary_Messages_();

    void __cdecl Diplomacy_Mouse_Init_(int16_t ambassador_player_id);

    void __cdecl Get_Diplomacy_Statement_(int16_t statement_id, int16_t speaker_player_id, int16_t listener_player_id, char* out_text);

    void __cdecl Setup_Back_Page_();

    void __cdecl Draw_Diplomacy_Fade_In_Screen_();

    void __cdecl Setup_Ambassador_Pic_(int16_t is_animated);

    void __cdecl Npc_Diplomacy_Screen_(int16_t player_idx, int8_t is_sneak_attack);

    int16_t __cdecl Diplomacy_Generic_Exchange_Tech_List_(int16_t tech_app_player, int16_t unused_2, char* list_title, uint8_t* tech_ids, int16_t num_techs, int8_t mode);

    int16_t __cdecl Get_Non_Player_Proposals_();

    void __cdecl Accepted_Npc_Demand_Resolution_(int16_t player_id, int16_t ambassador_id);
    void __cdecl Npc_Give_Gift_(int16_t player_idx, int16_t target_idx);

    void __cdecl Show_Sneak_Attack_Message_(int16_t player_idx, int16_t defender_idx);

    void __cdecl Setup_Diplomacy_Help_(int16_t count, int16_t* help_ids);

    void __cdecl Send_Diplomacy_Handshake_(int16_t sender_player_id, int16_t target_player_id);

    void __cdecl Diplomacy_Synch_Mode_(int16_t p1, int16_t p2);

    void __cdecl Diplomacy_Net_Switcheroo_();

    void __cdecl Get_Human_Demand_Message_(int16_t player_idx, int16_t unused_target_idx, int16_t demand_type, int16_t item_idx);

    int16_t __cdecl Get_Human_Demand_Choice_(int16_t player_idx, int16_t other_player_idx, int16_t demand_type, int16_t demand_item_id);

    void __cdecl Setup_Full_Screen_Diplomacy_Help_(int16_t help_id);

    extern int16_t _main_repulsive_diplomacy_help_entry[7];
    extern int16_t _main_net_repulsive_diplomacy_help_entry[7];
    extern char* _main_repulsive_diplomacy_list[7];
    extern char* _main_net_repulsive_diplomacy_list[6];
    extern s_dipscrn_net_message _net_diplomacy_message;

    void __cdecl Diplomacy_Receive_Message_(uint32_t from_player_id, uint16_t msg_code, uint32_t payload_len_or_unused, s_dipscrn_net_message* message);

    void __cdecl Diplomacy_Main_Net_Chat_(int16_t player_idx, int16_t target_player_idx);

    void __cdecl Start_Net_Treaty_();

    void __cdecl Draw_Diplomacy_Listen_Mode_();

    void __cdecl Diplomacy_Chat_(int16_t player_idx, uint8_t target_player_id);

    int16_t __cdecl Check_Human_Treaty_Proposal_(int16_t player_id, int16_t target_player_id, int16_t treaty_type);

    int16_t __cdecl Get_Human_Treaty_Choice_(int16_t player_a, int16_t player_b, int16_t treaty_type);

    void __cdecl Get_Listen_Mode_Message_(int16_t race_idx);

    void __cdecl Diplomacy_Listen_Mode_(int16_t player_idx, uint8_t target_player_id);

    int16_t __cdecl Diplomacy_Need_Better_Offer_(int16_t type, int16_t tech_app, int16_t bc, int16_t star_idx, int16_t tribute);

    int16_t __cdecl Diplomacy_Human_Need_Better_Offer_(int16_t proposer_idx, int16_t target_idx);

    void __cdecl Get_Human_Form_Treaty_Message_(int16_t proposer_idx, int16_t target_idx, int16_t treaty_type, int16_t msg_type);

    void __cdecl Get_Human_Declare_War_Message_(int16_t race_idx);

    void __cdecl Get_Diplomacy_Demand_Treaty_List_(int16_t player1, int16_t player2, int16_t treaty_type, int16_t* out_count, uint8_t* out_list);

    void __cdecl Get_Diplomacy_Tech_List_(int16_t player_a_idx, int16_t player_b_idx, int16_t* tech_count_ptr, uint8_t* tech_list_buffer);

    int16_t __cdecl Diplomacy_Generic_List_(int16_t item_count, char* list_strings, int8_t add_cancel, char* title);

    int16_t __cdecl Diplomacy_Generic_System_List_(int16_t star_idx, int16_t giver_player_idx, int16_t receiver_player_idx, int16_t mode_flag);

    extern int16_t _offer_help_entry[7];
    extern char* _offer_diplomacy_list[7];
    extern char* _offer_title;

    extern uint8_t _diplomacy_system_planet_list[5];
    extern int16_t _diplomacy_system_population;
    extern uint8_t _diplomacy_system_receiver;
    extern int16_t _diplomacy_system_planet_count;
    extern int16_t _diplomacy_scan_input_flag;
    extern uint8_t _diplomacy_system_giver;

    extern char* _demand_diplomacy_list[11];
    extern char* _net_listen_mode_option_buffer;
    extern char* _system_unsurrenderable_buffer;
    extern char* _main_net_diplomacy_list[11];
    extern char* _demand_title;
    extern char* _agree_diplomacy_list[4];
    extern char* _system_cancel_buffer;
    extern int16_t _diplomacy_system_homeworld_flag;
    extern s_animation_header* _diplomacy_system_star_background_seg;
    extern int16_t _diplomacy_system_fields[MAX_STARS];
    extern uint8_t* _diplomacy_mouse_bitmap;
    extern char* _system_homeworld_buffer;
    extern int16_t _diplomacy_good_music;
    extern char* _statement_research_treaty_buffer;
    extern int16_t _diplomacy_bad_music;
    extern char* _choose_a_system_buffer;
    extern char* _enter_your_message_buffer;
    extern char* _statement_trade_treaty_buffer;
    extern char* _system_million_colonists_buffer;
    extern char* _net_listen_mode_good_bye_buffer;
    extern char* _system_x_star_system_buffer;
    extern char* _percent_tribute_treaty_buffer;
    extern char* _you_were_framed_buffer;

    void __cdecl Load_Diplomacy_Text_Messages_();

    bool __cdecl Check_Surrender_System_(int16_t player_a, int16_t player_b);

    void __cdecl Get_Human_Offer_Gift_Message_(int16_t player_idx, int16_t offer_type, int16_t value);

    void __cdecl Diplomacy_Load_New_Ambassador_(int16_t player_idx);

    void __cdecl Get_Human_Tech_Exchange_First_Message_(int16_t player_1, int16_t player_2, int16_t tech_id);

    void __cdecl Get_Human_Tech_Exchange_Message_(int16_t player_2, int16_t player_1, int16_t tech_1, int16_t tech_2);

    void __cdecl Get_Human_Tech_Exchange_Accept_Message_();

    void __cdecl Get_Human_Tech_Exchange_Decline_Message_();

    void __cdecl Get_Human_Break_Treaty_Message_(int16_t race_idx, int16_t treaty_type);

    void __cdecl Get_Human_Gift_Response_(int16_t sender_id, int16_t target_id, int16_t gift_type, int16_t gift_arg);

    int16_t __cdecl Diplomacy_System_Display_(int16_t giver_idx, int16_t receiver_idx, int16_t display_mode);

    void __cdecl Draw_Diplomacy_System_Display_();

    void __cdecl Draw_Diplomacy_System_();

    void __cdecl Draw_Diplomacy_System_Text_(int16_t star_idx, int16_t selected_planet_idx);

    int16_t __cdecl Diplomacy_Offer_Money_(int16_t player_idx, int16_t target_idx, int8_t do_transaction);

    void __cdecl Start_Diplomacy_Music_(int16_t music_type);

    int16_t __cdecl Diplomacy_System_Scan_(int16_t star_idx, int16_t player_idx, int16_t* artemis_planet_idx, int16_t* owned_count, uint8_t* planet_indices);

    void __cdecl Diplomacy_Generic_System_List_Draw_();
}