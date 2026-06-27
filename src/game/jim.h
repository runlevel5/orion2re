#pragma once

struct s_animation_header;
struct s_mouse_list;

namespace JIM {
    extern int16_t _jim_current_stream;

    extern int16_t _jim_old_music_id;

    extern int16_t _jim_fade_music_flag;
    extern int16_t _jim_old_transition_id;

    extern s_mouse_list _logo_mouse_list;
    extern int16_t _darken_palette_value;

    // Returns true when the Simtex logo asset exists on either configured data path.
    bool __cdecl Check_For_Simtex_Logo_();

    // Darkens the current palette to black and resets the logo display pages.
    void __cdecl Logo_Fadeout_();

    // Plays the logo sequence and optionally the intro cinematic.
    void __cdecl Draw_Logos_(void);

    // Starts a random looping background stream if one is not already active.
    void Play_Background_Music_();

    // Stops the active background stream and clears JIM music state.
    void __cdecl Stop_Background_Music_();

    // Starts a random looping combat stream when music is enabled.
    void __cdecl Play_Combat_Music_();

    /* Starts a streamed music track and optional transition track.
     * @param music_id Primary stream identifier.
     * @param transition_id Follow-up stream identifier, or a sentinel for background fallback.
     * @param loop_flag Nonzero to loop the resulting stream.
     */
    void __cdecl Play_Streaming_Music_(int16_t music_id, int16_t transition_id, int16_t loop_flag);

    // Expands name placeholders in buffer in place.
    void __cdecl Decode_Text_Name_And_Name_(char* buffer, int16_t name_idx1, int16_t name_idx2, int16_t max_len);

    /* Loads a localized text entry into dest.
     * @param filename LBX filename containing the message table.
     * @param message_index Message-table entry index.
     * @param dest Destination buffer that receives the loaded message.
     * @param max_len Capacity of dest, including the terminator.
     */
    void __cdecl Get_Text_Message_(const char* filename, int16_t message_index, char* dest, int16_t max_len);

    void __cdecl Decode_Text_Name_And_Tech_(char* buffer, int16_t name_idx, int16_t tech_idx, int16_t max_len);

    void __cdecl Decode_Text_Name_And_Other_Tech_(char* buffer, int16_t name_idx, int16_t other_name_idx, int16_t tech_idx, int16_t max_len);

    void __cdecl Decode_Text_System_(char* buffer, int16_t system_idx, int16_t max_len);

    void __cdecl Decode_Text_Name_And_System_(char* buffer, int16_t name_idx, int16_t system_idx, int16_t max_len);

    void __cdecl Decode_Text_Name_Tech_And_Other_Tech_(char* buffer, int16_t name_idx, int16_t other_name_idx, int16_t first_tech_idx, int16_t second_tech_idx, int16_t max_len);

    void __cdecl Bill_Picture_To_Mouse_(s_animation_header* anim);

    void __cdecl Decode_Text_Name_And_Stat_(char* buffer, int16_t name_idx, int16_t numeric_value, int16_t max_len);

    void __cdecl Decode_Text_Tech_And_Other_Name_(char* buffer, int16_t tech_id, int16_t other_name_1, int16_t other_name_2, int16_t max_len);

    void __cdecl Decode_Text_Race2_(char* buffer, int16_t race_idx, int16_t max_len);

    void __cdecl Decode_Text_Race_(char* buffer, int16_t race_idx, int16_t max_len);

    void __cdecl Decode_Text_Stat_(char* buffer, int16_t numeric_value, int16_t max_len);

    /* Expands placeholder tokens in message_buffer in place.
     * @param message_buffer Source and destination tokenized message buffer.
     * @param player1_id Index used for first-player name or race substitutions.
     * @param star_id Index used for star or planet-name substitutions.
     * @param tech_app1_id Index used for the first technology-name substitution.
     * @param number Numeric value used for number substitution.
     * @param player2_id Index used for second-player name or race substitutions.
     * @param tech_app2_id Index used for the second technology-name substitution.
     * @param tech_field_id Index used for technology-field substitutions.
     * @param max_len Capacity of message_buffer, including the terminator.
     */
    void __cdecl Decode_Text_Message_(char* message_buffer, int16_t player1_id, int16_t star_id, int16_t tech_app1_id, int16_t number, int16_t player2_id, int16_t tech_app2_id, int16_t tech_field_id, int16_t max_len);

    void __cdecl Decode_Text_Year_(char* buffer, int16_t max_len);

    void __cdecl Decode_Text_Field_(char* buffer, int16_t field_id, int16_t max_len);
}