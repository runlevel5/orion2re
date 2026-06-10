#pragma once

namespace sound {
    /**
     * Drives periodic sound and stream maintenance from the multimedia timer.
     * @param timer_id Multimedia timer identifier.
     * @param message Timer callback message code.
     * @param user_data Caller-supplied timer user data.
     * @param param1 Reserved timer callback parameter.
     * @param param2 Reserved timer callback parameter.
     */
    void __stdcall Sound_Timer_Callback_(uint32_t timer_id, uint32_t message, uint32_t user_data, uint32_t param1, uint32_t param2);

    void __cdecl Fade_Stream_Down_(uint8_t fade_rate);

    int32_t __cdecl Get_Stream_Volume_();

    void __cdecl Stop_Stream_();

    void __cdecl Stream_On_();

    void __cdecl Set_Stream_Max_Volume_(int32_t volume);

    void __cdecl Set_Stream_Filename_(const char* filename);

    void __cdecl Play_Stream_(int32_t stream_id);

    void __cdecl Loop_Stream_(int32_t count);

    void __cdecl Set_Stream_Volume_(int32_t volume_percent);

    void __cdecl Fade_Stream_Up_(uint8_t fade_increment);

    uint8_t __cdecl Is_Stream_Playing_();

    void __cdecl Set_Audio_Buffer_(void* buffer);

    void __cdecl Sound_On_();

    void __cdecl Stop_All_Sounds_();

    void __cdecl Resume_Sound_(int32_t channel_idx);

    void __cdecl Set_Sound_Max_Volume_(int32_t volume);

    void __cdecl Stop_Sound_(int32_t channel_idx);

    void __cdecl Sound_Update_();

    int32_t __cdecl Play_Sound_(int32_t resource_id);

    void __cdecl Fadeout_Sound_(int16_t channel_idx, int8_t volume_idx);

    void __cdecl Pitch_Bend_Sound_(int32_t handle, int32_t pitch);

    void __cdecl Loop_Sound_(int32_t channel_idx, int8_t loop_count);

    void __cdecl Destroy_Sound_();

    void __cdecl Init_Sound_();
}