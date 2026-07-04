#pragma once

namespace MAINMENU {
 #pragma pack(push, 1)
    struct s_credit_y_slot {
        union {
            int32_t dword;
            struct {
                int16_t y;
                int16_t upper_word;
            };
        };
    };
 #pragma pack(pop)

    extern s_main_menu* _mainmenu;

    extern s_animation_header* _main_menu_background;

    extern s_animation_header* _mainmenu_first_seg;
    extern s_animation_header* _mainmenu_last_seg;
    extern void* __gametype;
    extern int16_t _max_credits;
    extern int32_t _credit_count;
    extern int16_t _credits_font_style;
    extern uint8_t _credit_num;
    extern uint8_t _credits_font_spacing;
    extern s_animation_header* _bitmap;
    extern s_credit_y_slot _credit_y_slot;
    extern int32_t _credit_x;
    extern int16_t _delay_count;
    extern int16_t _fade_in_count;
    extern int16_t _fade_out_count;
    extern char (*_credits)[2][127];

    /**
     * Draws the main menu background during fade-in.
     */
    void __cdecl Main_Menu_Fade_Function_();

    /**
     * Draws the opening main menu segment during fade-in.
     */
    void __cdecl Main_Menu_Fade_Function2_();

    /**
     * Creates the active main-menu button fields for the current save-state.
     */
    void __cdecl Add_Main_Menu_Fields_();

    /**
     * Reloads the main menu background and button animations from LBX data.
     */
    void __cdecl Reload_Main_Menu_Screen_();

    /**
     * Plays the opening file animation and returns the selected menu hotkey code.
     */
    int16_t __cdecl Do_Main_Menu_File_Animation_();

    /**
     * Applies a fade step to the scrolling credit palette entries.
     * @param amount Additive fade amount.
     * @param colors Palette buffer to adjust.
     */
    void __cdecl Fade_Credit_Colors_(int8_t amount, s_colors* colors);

    extern uint8_t _credits_column_one;

    /**
     * Renders one credit entry into the scratch bitmap.
     * @param index Credit row index.
     * @param bitmap Destination bitmap.
     */
    void __cdecl Copy_Credit_String_To_Bitmap_(int16_t index, s_animation_header* bitmap);

    /**
     * Draws the scrolling credits overlay.
     */
    void __cdecl Draw_Credits_();

    /**
     * Draws the current main menu screen state.
     */
    void __cdecl Draw_Main_Menu_Screen_();

    /**
     * Replaces the active help list for the main menu.
     * @param help_data Help entry array.
     * @param count Number of help entries.
     */
    void __cdecl Set_Main_Menu_Screen_Help_List_Aux_(s_help_box* help_data, int16_t count);

    /**
     * Installs the standard main menu help list.
     */
    void __cdecl Set_Main_Menu_Screen_Help_List_();

    /**
     * Runs the load-game screen until the popup finishes.
     */
    int8_t __cdecl Do_Mainmenu_Load_Screen_();

    /**
     * Loads the artwork used by the main menu load-game popup.
     */
    void __cdecl Load_Mainmenu_Load_Game_Popup_();

    /**
     * Displays the main menu load-game popup and returns its result.
     * @param save_game_list Storage used by the popup load/save code.
     */
    int8_t __cdecl Mainmenu_Load_Game_Popup_(int16_t* save_game_list);

    /**
     * Runs the main menu screen loop until the user leaves it.
     */
    void __cdecl Main_Menu_Screen_();

    /**
     * Returns the scratch-buffer size required for the current file-animation block.
     */
    int32_t __cdecl Get_File_Animation_Block_Space_();

    /**
     * Selects the localized credits resource and font settings.
     * @param filename Output buffer for the chosen credits LBX filename.
     * @param filename_size Size of the output buffer in bytes.
     */
    void __cdecl Init_Credits_(char* filename, uint32_t filename_size);

    /**
     * Loads the scrolling credits text and scratch bitmap.
     */
    void __cdecl Load_Credits_();
}