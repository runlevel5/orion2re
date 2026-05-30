#include "pch.h"

namespace NEWGAME {

int16_t _g_x = 15;
int16_t _g_y = 5;
s_colors _color_ken;
int16_t _opp_button_variable;
int16_t _diff_button_variable;
int16_t _xenon_attacks_button_variable;
int16_t _civ_button_variable;
uint16_t _combat_button_variable;
int16_t _gal_size_button_variable;
int16_t _random_events_button_variable;
int16_t _gal_age_button_variable;
int16_t _fields[10];
s_animation_header* _anims[7];
s_animation_header* _tech_level_anims[3];
s_animation_header* _difficulty_anims[5];
s_animation_header* _num_players_anims[7];
s_animation_header* _gal_age_anims[3];
s_animation_header* _gal_size_anims[4];
s_animation_header* _background_seg;
char* _gal_size_button_strings[4];
char* _civ_button_strings[3];
char* _diff_button_strings[5];
char* _gal_age_button_strings[3];
char* _combat_button_strings[2];
char _hack_msg[20];
char* _opp_button_strings[7];

const char s_Media_00557db4[6] = "Media";
const char s_sen_00557dbc[5] = "%sen";
const char s_Piccola_00557dc4[8] = "Piccola";

short __cdecl Newgame_Screen_(void) {
    memcpy(NEWGAME::_color_ken.colors, "y{z|||||", 8);

    struct_::Clear_Structure_(MOX::_star, 0x1fc8);
    struct_::Clear_Structure_(MOX::_planet, 0x17e8);
    struct_::Clear_Structure_(MOX::_colony, 0x1608a);
    struct_::Clear_Structure_(MOX::_ship, 0xfbf4);
    struct_::Clear_Structure_(MOX::_player, 0x7548);
    struct_::Clear_Structure_(MOX::_leaders, 0xf71);

    int16_t result = 0;
    int16_t current_civ_btn = NEWGAME::_civ_button_variable;

    mouse::Set_Mouse_List_(&COLGCBT::_mouse_list_blank, 1);
    ERIC::Fast_Fade_Out_();
    fonts::Load_Palette_(9, 0, 0xff);

    if (MOX::_game_type == 0 || MOX::_game_type == 1) {
        memset(RACESEL::_color_flag, 0, 8);
        memset(RACESEL::_race_flag, 0, 14);
    }

    NEWGAME::Reload_Newgame_Screen_();
    NEWGAME::Globals_To_Temps_();
    NEWGAME::Update_Fields_();

    COLONY::_full_draw = 1;
    video::Set_Refresh_Stencil_();
    ERICHELP::Set_New_Game_Screen_Help_List_(NEWGAME::_civ_button_variable);
    fields::Assign_Auto_Function_(NEWGAME::Draw_Newgame_Screen_, 2);
    video::Set_Page_Off_();
    NEWGAME::Draw_Newgame_Screen_();
    palstore::Darken_Palette_(100);
    palette::Clear_Palette_Changes_(0, 0xff);

    bool exit_screen = false;
    ERIC::Fast_Fade_In_();
    mouse::Set_Mouse_List_(&MOX::_mouse_list_arrow, 1);

    while (!exit_screen) {
        int16_t input = fields::Get_Input_();
        FAKEBUTS::Evaluate_Click_Thru_Fields_(input);

        if (input == NEWGAME::_fields[6] || input == NEWGAME::_fields[7]) {
            MOX::_current_screen = MOX::_return_screen;
            exit_screen = true;
        }

        if (input == NEWGAME::_fields[8]) {
            NEWGAME::Temps_To_Globals_();
            if (MOX::_game_type == 1) {
                MOX::_current_screen = 16 /* 0x10 */;
                result = 1;
                exit_screen = true;
            } else if (MOX::_game_type == 2 || MOX::_game_type == 3) {
                MOX::_current_screen = 21 /* 0x15 */;
                result = 1;
                exit_screen = true;
            } else {
                result = RACESEL::Race_Selection_Screen_(MOX::_player);
                if (result == 1) {
                    MOX::_player->objectives = 100 /* 0x64 */;
                    exit_screen = true;
                } else {
                    NEWGAME::Reload_Newgame_Screen_();
                    video::Set_Refresh_Stencil_();
                    COLONY::_full_draw = 1;
                    NEWGAME::Globals_To_Temps_();
                    NEWGAME::Update_Fields_();
                    fields::Assign_Auto_Function_(NEWGAME::Draw_Newgame_Screen_, 2);
                    ERICHELP::Set_New_Game_Screen_Help_List_(NEWGAME::_civ_button_variable);
                }
            }
        }

        if (current_civ_btn != NEWGAME::_civ_button_variable) {
            current_civ_btn = NEWGAME::_civ_button_variable;
            ERICHELP::Set_New_Game_Screen_Help_List_(current_civ_btn);
        }

        if (!exit_screen) {
            fonts::Set_Font_Style_(3, &NEWGAME::_color_ken);
            NEWGAME::Draw_Newgame_Screen_();
            fonts::Set_Font_Style_(3, &NEWGAME::_color_ken);
            RUSS::Mox_Sync_Update_();
        }
    }

    if (result == 1) {
        NEWGAME::Temps_To_Globals_();
    }

    fields::Clear_Fields_();
    FAKEBUTS::Clear_Click_Thru_Fields_();
    fields::Deactivate_Help_List_();
    fields::Deactivate_Auto_Function_();

    if (MOX::_game_type == 2 || MOX::_game_type == 3) {
        strcpy(MOX::_settings.net_game_name, NETMOX::_net.cur_game_name);
    }

    return result;
}

void __cdecl Globals_To_Temps_() {
    _diff_button_variable = (int16_t)(int8_t)MOX::_settings.game_difficulty;
    _gal_size_button_variable = (int16_t)(int8_t)MOX::_settings.galaxy_size;
    _gal_age_button_variable = (int16_t)(int8_t)MOX::_settings.galaxy_age;
    _opp_button_variable = (int16_t)((int8_t)MOX::_settings.number_of_players - 2);
    _combat_button_variable = (uint16_t)(MOX::_settings.strategic_combat_flag == 0);
    _civ_button_variable = (int16_t)(int8_t)MOX::_settings.starting_civilization_level;
    _random_events_button_variable = (uint8_t)MOX::_settings.random_events;
    _xenon_attacks_button_variable = MOX::_settings.xenons_exist;
}

void __cdecl Reload_Newgame_Screen_() {
    _background_seg = (s_animation_header*)farload::Far_Reload_(s_RACEOPT_LBX, 0, MOX::_screen_seg);
    RUSS::Mox_Update_();

    int16_t index = 0;
    for (int i = 0; i < 3; ++i) {
        _gal_age_anims[i] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_NEWGAME_LBX, i + 1, MOX::_screen_seg);
        index = i + 1;
    }

    index++;
    RUSS::Mox_Update_();

    for (int i = 0; i < 5; ++i) {
        _difficulty_anims[i] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_NEWGAME_LBX, index + i, MOX::_screen_seg);
    }
    index += 5;
    RUSS::Mox_Update_();

    for (int i = 0; i < 4; ++i) {
        _gal_size_anims[i] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_NEWGAME_LBX, index + i, MOX::_screen_seg);
    }
    index += 4;
    RUSS::Mox_Update_();

    for (int i = 0; i < 7; ++i) {
        _num_players_anims[i] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_NEWGAME_LBX, index + i, MOX::_screen_seg);
    }
    index += 7;
    RUSS::Mox_Update_();

    for (int i = 0; i < 3; ++i) {
        _tech_level_anims[i] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_NEWGAME_LBX, index + i, MOX::_screen_seg);
    }
    index += 3;
    RUSS::Mox_Update_();

    _anims[1] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_NEWGAME_LBX, (int32_t)index, MOX::_screen_seg);
    _anims[0] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_NEWGAME_LBX, (int32_t)index + 1, MOX::_screen_seg);
    _anims[2] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_NEWGAME_LBX, (int32_t)index + 2, MOX::_screen_seg);
    _anims[3] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_NEWGAME_LBX, (int32_t)index + 3, MOX::_screen_seg);
    _anims[4] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_NEWGAME_LBX, (int32_t)index + 4, MOX::_screen_seg);
    _anims[6] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_NEWGAME_LBX, (int32_t)index + 5, MOX::_screen_seg);
    _anims[5] = (s_animation_header*)farload::Far_Reload_Next_(COLONY::_NEWGAME_LBX, (int32_t)index + 6, MOX::_screen_seg);

    RUSS::Mox_Update_();
    Load_New_Game_Strings_();
}

void __cdecl Draw_Pictures_() {
    int16_t col = 0;
    int16_t x_offset = 0;
    do {
        int16_t row = 0;
        int16_t y_offset = 0;
        do {
            if ((row != 1) || (col < 2)) {
                s_animation_header* anim = nullptr;

                if ((col == 0) && (row == 0)) {
                    anim = _difficulty_anims[_diff_button_variable];
                } else if ((col == 1) && (row == 0)) {
                    anim = _gal_size_anims[_gal_size_button_variable];
                } else if ((col == 2) && (row == 0)) {
                    anim = _gal_age_anims[_gal_age_button_variable];
                } else if ((col == 0) && (row == 1)) {
                    anim = _num_players_anims[_opp_button_variable];
                } else if ((col == 1) && (row == 1)) {
                    anim = _tech_level_anims[_civ_button_variable];
                }

                animate::Draw_(
                    x_offset + _g_x + 121,
                    _g_y + y_offset + 119,
                    anim
                );
            }
            y_offset += 140;
            row++;
        } while (row < 2);

        x_offset += 155;
        col++;
    } while (col < 3);
}

void __cdecl Update_Fields_() {
    FAKEBUTS::Clear_Click_Thru_Fields_();
    fields::Clear_Fields_();
    fonts::Set_Font_Style_(3, &_color_ken);

    const char* common_str = ESTRINGS::E_Strings_(0x0C);
    int16_t gx = _g_x;
    int16_t gy = _g_y;

    // Difficulty
    int16_t h0 = fields::Add_Hidden_Field_(gx + 0x79, gy + 0x77, gx + 0xBC, gy + 0xB8, common_str, 0);
    _fields[0] = FAKEBUTS::Add_Click_Thru_Field_(gx + 0x69, gy + 0xCC, gx + 0xCD, gy + 0xE0, _diff_button_strings, 5, &_diff_button_variable, 0, common_str, 0, h0);

    // Galaxy Size
    int16_t h1 = fields::Add_Hidden_Field_(gx + 0x114, gy + 0x77, gx + 0x157, gy + 0xB8, common_str, 0);
    _fields[1] = FAKEBUTS::Add_Click_Thru_Field_(gx + 0x105, gy + 0xCC, gx + 0x169, gy + 0xE0, _gal_size_button_strings, 4, &_gal_size_button_variable, 0, common_str, 0, h1);

    // Galaxy Age
    int16_t h2 = fields::Add_Hidden_Field_(gx + 0x1AF, gy + 0x77, gx + 0x1F2, gy + 0xB8, common_str, 0);
    _fields[2] = FAKEBUTS::Add_Click_Thru_Field_(gx + 0x1A1, gy + 0xCC, gx + 0x205, gy + 0xE0, _gal_age_button_strings, 3, &_gal_age_button_variable, 0, common_str, 0, h2);

    // Opponents
    int16_t h3 = fields::Add_Hidden_Field_(gx + 0x79, gy + 0x103, gx + 0xBC, gy + 0x144, common_str, 0);
    _fields[3] = FAKEBUTS::Add_Click_Thru_Field_(gx + 0x69, gy + 0x15D, gx + 0xCD, gy + 0x171, _opp_button_strings, 7, &_opp_button_variable, 0, common_str, 0, h3);

    // Starting Civilization Level
    int16_t h4 = fields::Add_Hidden_Field_(gx + 0x114, gy + 0x103, gx + 0x157, gy + 0x144, common_str, 0);
    _fields[5] = FAKEBUTS::Add_Click_Thru_Field_(gx + 0x105, gy + 0x15D, gx + 0x169, gy + 0x171, _civ_button_strings, 3, &_civ_button_variable, 0, common_str, 0, h4);

    // Radio Buttons: Random Events, Xenon Attacks, Strategic Combat
    fields::Add_Radio_Button_Field_(gx + 0x17C, gy + 0x127, ESTRINGS::E_Strings_(0x0C), _anims[0], &_random_events_button_variable, ESTRINGS::E_Strings_(0x0C), 0x29);
    fields::Add_Radio_Button_Field_(gx + 0x17C, gy + 0x14A, ESTRINGS::E_Strings_(0x0C), _anims[2], &_xenon_attacks_button_variable, ESTRINGS::E_Strings_(0x0C), 0x29);
    fields::Add_Radio_Button_Field_(gx + 0x17C, gy + 0x103, ESTRINGS::E_Strings_(0x0C), _anims[1], (int16_t*)&_combat_button_variable, ESTRINGS::E_Strings_(0x0C), 0x29);

    if (MOX::_game_type == 2 /* NET_GAME */) {
        strcpy(NETMOX::_net.cur_game_name, MOX::_settings.net_game_name);
        fields::Add_Continuous_String_Input_Field_(gx + 0xE1, gy + 0x18A, 0xA8, 0x12, NETMOX::_net.cur_game_name, 8, 0, 0, 0, 0);
    }

    // Accept and Cancel buttons
    _fields[6] = fields::Add_Button_Field_(gx + 0x64, gy + 0x182, ESTRINGS::E_Strings_(0x0C), _anims[3], ESTRINGS::E_Strings_(0x0C), 0x28);
    _fields[8] = fields::Add_Button_Field_(gx + 0x1A2, gy + 0x183, ESTRINGS::E_Strings_(0x0C), _anims[4], ESTRINGS::E_Strings_(0x0C), 0x28);
    _fields[7] = fields::Add_Hot_Key_(ESTRINGS::E_Strings_(0x87));
}

void __cdecl Draw_Newgame_Screen_() {
    fonts::Set_Font_Style_Shadow_Down_(4, &_color_ken);
    fonts::Set_Outline_Color_(0, 0x40);
    animate::Draw_(_g_x, _g_y, _anims[6]);
    fonts::Set_Font_Style_(2, &_color_ken);
    video::Set_Page_Off_();

    if (COLONY::_full_draw != 0) {
        graphics::Fill_(0, 0, 0x27F, 0x1DF, 0);
        animate::Draw_(0, 0, _background_seg);
        fonts::Set_Font_Style_(3, &_color_ken);
        video::Copy_Off_To_Back_();
        COLONY::_full_draw = 0;
    }

    FAKEBUTS::Draw_Click_Thru_Fields_();
    Draw_Pictures_();

    if (MOX::_game_type == 2 /* ECON_INDUSTRY */) {
        animate::Draw_(_g_x + 0xD9, _g_y + 0x179, _anims[5]);
    }

    fonts::Set_Font_Style_(3, &_color_ken);
    fonts::Set_Alias_Color_(0x9A);
}

void Load_New_Game_Strings_() {
    _diff_button_strings[0] = ESTRINGS::E_Strings_(0x221 /* ESTR_TUTOR */);
    _diff_button_strings[1] = ESTRINGS::E_Strings_(0x118 /* ESTR_EASY */);
    _diff_button_strings[2] = ESTRINGS::E_Strings_(0xbe /* ESTR_AVERAGE */);
    _diff_button_strings[3] = ESTRINGS::E_Strings_(0x13e /* ESTR_HARD */);
    _diff_button_strings[4] = ESTRINGS::E_Strings_(0x148 /* ESTR_IMPOSSIBLE */);

    _gal_size_button_strings[0] = ESTRINGS::E_Strings_(0x1e0 /* ESTR_SMALL */);
    // Special case for Italian language
    if (MOX::_settings.language == 4) {
        sprintf(_hack_msg, s_Piccola_00557dc4);
        _gal_size_button_strings[0] = _hack_msg;
    }

    _gal_size_button_strings[1] = ESTRINGS::E_Strings_(0x173 /* ESTR_MEDIUM */);

    if (MOX::_settings.language == 2) {
        char* medium_str = ESTRINGS::E_Strings_(0x173 /* ESTR_MEDIUM */);
        sprintf(_hack_msg, s_sen_00557dbc, medium_str);
        _gal_size_button_strings[2] = _hack_msg;
    } else if (MOX::_settings.language == 4) {
        sprintf(_hack_msg, s_Media_00557db4);
        _gal_size_button_strings[2] = _hack_msg;
    }

    // Original behavior: this unconditional assignment overwrites any temporary localized hack string.
    _gal_size_button_strings[2] = ESTRINGS::E_Strings_(0x168);
    _gal_size_button_strings[3] = ESTRINGS::E_Strings_(0x143);

    _gal_age_button_strings[0] = ESTRINGS::E_Strings_(0x179);
    _gal_age_button_strings[1] = ESTRINGS::E_Strings_(0xbe /* ESTR_AVERAGE */);
    _gal_age_button_strings[2] = ESTRINGS::E_Strings_(0x192);

    _combat_button_strings[0] = ESTRINGS::E_Strings_(0x1f7);
    _combat_button_strings[1] = ESTRINGS::E_Strings_(0x1f0);

    _opp_button_strings[0] = ESTRINGS::E_Strings_(0x94);
    _opp_button_strings[1] = ESTRINGS::E_Strings_(0x96);
    _opp_button_strings[2] = ESTRINGS::E_Strings_(0x98);
    _opp_button_strings[3] = ESTRINGS::E_Strings_(0x9a);
    _opp_button_strings[4] = ESTRINGS::E_Strings_(0x9c);
    _opp_button_strings[5] = ESTRINGS::E_Strings_(0x9d);
    _opp_button_strings[6] = ESTRINGS::E_Strings_(0x9e);

    _civ_button_strings[0] = ESTRINGS::E_Strings_(0x1ad);
    _civ_button_strings[1] = ESTRINGS::E_Strings_(0xbe /* ESTR_AVERAGE */);
    _civ_button_strings[2] = ESTRINGS::E_Strings_(0xa9);
}

void Temps_To_Globals_() {
    MOX::_settings.game_difficulty = (uint8_t)_diff_button_variable;
    MOX::_settings.galaxy_size = (uint8_t)_gal_size_button_variable;
    MOX::_settings.galaxy_age = (uint8_t)_gal_age_button_variable;
    MOX::_settings.number_of_players = (uint8_t)_opp_button_variable + 2;
    MOX::_settings.starting_civilization_level = (uint8_t)_civ_button_variable;
    MOX::_settings.strategic_combat_flag = (_combat_button_variable == 0);
    MOX::_settings.random_events = (int8_t)_random_events_button_variable;
    MOX::_settings.xenons_exist = (byte)_xenon_attacks_button_variable;
}

} // namespace NEWGAME