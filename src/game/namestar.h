#pragma once

namespace namestar {
    /* Returns the screen-space X coordinate used to center the star-name popup. */
    int16_t __cdecl Star_Name_Popup_Screen_Center_X_();
    /* Returns the screen-space Y coordinate used to center the star-name popup. */
    int16_t __cdecl Star_Name_Popup_Screen_Center_Y_();
    /* Opens the star rename popup at the given position.
     * x, y: Popup origin in screen coordinates.
     * star_idx: Star to rename.
     * use_animated_popup: Nonzero to use the animated popup variant.
     */
    void __cdecl Change_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx, int8_t use_animated_popup);
    /* Returns the map-space X coordinate used to center the star-name popup. */
    int16_t __cdecl Star_Name_Popup_Map_Center_X_();
    /* Returns the map-space Y coordinate used to center the star-name popup. */
    int16_t __cdecl Star_Name_Popup_Map_Center_Y_();
    /* Runs the star rename popup and writes the edited name back to the star record.
     * x, y: Popup origin in screen coordinates.
     * star_idx: Star to rename.
     * use_animated_popup: Nonzero to use the animated popup variant.
     * prompt_text: Prompt shown above the input field.
     */
    int8_t __cdecl Do_Change_Star_Name_(int16_t x, int16_t y, int16_t star_idx, int8_t use_animated_popup, char* prompt_text);
    /* Opens the home-star rename popup at the given position.
     * x, y: Popup origin in screen coordinates.
     * star_idx: Home star to rename.
     */
    void __cdecl Change_Home_Star_Name_Popup_(int16_t x, int16_t y, int16_t star_idx);

    /* Returns the maximum pixel width available for the specified star name.
     * star_idx: Star whose label spacing is being measured.
     */
    int16_t __cdecl Max_Pixel_Width_Star_Name_Can_Be_(int16_t star_idx);
    /* Returns whether the specified star can still be renamed.
     * star_idx: Star to test for rename eligibility.
     */
    int8_t __cdecl Can_Change_Star_Name_(int16_t star_idx);

    extern s_input_box_state _input_box;

    /* Initializes the shared popup state and draws the input box background. */
    void __cdecl Setup_Input_Box_Popup_();

    /* Redraws the active input box popup. */
    void __cdecl Draw_Input_Box_Popup_();

    /* Shows a text input popup and copies the edited text back to the caller buffer.
     * input_text: Caller buffer that provides the initial text and receives the result.
     * input_type: Input filtering mode for the field.
     * max_length: Maximum accepted pixel width or zero for the default limit.
     * prompt_text: Prompt shown above the input field.
     * x, y: Popup origin in screen coordinates.
     * show_cancel_anim: Nonzero to use the animated cancel button.
     */
    int8_t __cdecl Input_Box_Popup_(char* input_text, int16_t input_type, int16_t max_length, char* prompt_text, int16_t x, int16_t y, int8_t show_cancel_anim);

    /* Shows the animated text input popup variant. */
    int8_t __cdecl Input_Box_Popup_Animated_(char* input_text, int16_t input_type, int16_t max_length, char* prompt_text, int16_t x, int16_t y);
}