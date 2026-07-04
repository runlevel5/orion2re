#pragma once

namespace RACESEL {
    extern int16_t _custom_flag;
    extern int16_t _displayed_race;
    extern int16_t _old_race;
    extern int16_t _race_button_status[14];

    extern uint8_t* _race_specials[14];

    extern s_animation_header* _race_button[14];

    extern char _last_race_name[15];

    extern s_animation_header* _last_race_seg;

    extern uint8_t _color_flag[MAX_PLAYERS];
    extern uint8_t _race_flag[14];

    /**
     * Converts saved custom-race bonus values into the 1..3 option indices used by the selection UI.
     * @param custom_race_flags 31-byte custom-race data buffer to normalize in place.
     */
    void __cdecl Convert_Custom_Race_Flags_(uint8_t* custom_race_flags);

    /**
     * Loads race-selection graphics, race data, and the saved custom-race preview into screen memory.
     */
    void __cdecl Reload_Race_Selection_Screen_();
    /**
     * Shows the animated naming popup and writes the chosen or fallback ruler name to the caller buffer.
     * @param output_name Destination buffer for the resulting ruler name.
     * @param output_name_size Size of output_name in bytes.
     * @param race_idx Selected race index used to choose default names.
     */
    int8_t __cdecl Naming_Popup_(char* output_name, uint32_t output_name_size, int16_t race_idx);
    /**
     * Shows the banner-color selection popup and stores the accepted color index.
     * @param selected_color Destination for the chosen banner color.
     */
    int16_t __cdecl Flag_Screen_(uint8_t* selected_color);
    /**
     * Runs the custom-race option screen and writes accepted trait choices back to the player record.
     * @param player Player data being edited.
     */
    int16_t __cdecl Racial_Option_Screen_(s_player* player);
    /**
     * Runs the race-selection flow for the player and returns 1 when a race is accepted.
     * @param player Player data receiving the chosen race, name, traits, and color.
     */
    int16_t Race_Selection_Screen_(s_player *player);

    /**
     * Configures help hotspots for the main race-selection screen.
     */
    void __cdecl Set_Race_Select_Help_();
    /**
     * Clears a previously requested race and notifies the network host.
     * @param race_idx Race index to release.
     */
    void __cdecl Unrequest_Race_(uint8_t race_idx);
    /**
     * Draws the flag-selection popup and skips colors that are already unavailable.
     */
    void __cdecl Draw_Flag_Screen_();
    /**
     * Configures help hotspots for the flag-selection popup.
     */
    void __cdecl Set_Flag_Screen_Help_();

    extern char* _temp_ruler_name;

    /**
     * Redraws the naming popup background when a full-screen refresh is requested.
     */
    void __cdecl Draw_Naming_Popup_();
    /**
     * Appends list separators while building the displayed race-ability paragraph.
     * @param message_count Running number of entries already appended.
     * @param split_at_count Entry count where the final separator should be inserted.
     * @param output_text Destination string being built in place.
     * @param output_text_size Size of output_text in bytes.
     */
    void __cdecl Message_Build_(int16_t* message_count, int16_t split_at_count, char* output_text, uint32_t output_text_size);
    /**
     * Draws the currently highlighted race portrait, availability state, and summary text.
     */
    void __cdecl Draw_Race_Selection_Screen_();
}