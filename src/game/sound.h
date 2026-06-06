#pragma once

extern "C" HRESULT WINAPI DirectSoundCreate(LPCGUID pcGuidDevice, struct IDirectSound** ppDS, IUnknown* pUnkOuter);

namespace sound {
    extern int16_t _ds_volume_table[101];

    extern uint32_t _audioInitialized;

    extern uint8_t _soundOn;

    extern uint8_t _soundErrorOccurred;

    extern MMRESULT _soundTimerId;

    extern uint8_t _soundTimerActive;

    extern volatile uint32_t _soundTimerRunning;

    extern volatile uint32_t _soundTimerStopFlag;

    extern void* _soundBuffer;

    extern uint32_t _soundBufferSize;

    extern volatile uint32_t _soundTimerLock;

    extern void* _streamBuffer1;

    extern void* _streamBuffer2;

    extern uint8_t _soundFxActive;

    extern int32_t _global_stream_volume;

    extern uint8_t _loopStream;

    extern int32_t _doubleBufferLoops;

    extern uint8_t _command;

    extern uint8_t _streamIncVal;

    void __cdecl Copy_FourCC_(char* dest, const char* src);

    extern uint32_t _streamAvailable2;

    extern s_lbx_stream_header _musicLbxHandle;

    extern IDirectSound* _dsound;

    extern HRESULT _dsound_hr;

    extern uint32_t _streamAvailable1;

    extern int32_t _dsMasterVolume;

    extern int32_t _dsMasterPan;

    extern s_sound_index _soundIndices[33];

    extern LPDIRECTSOUNDBUFFER _dsound_buffer;

    extern DSBUFFERDESC1 _dsound_buffer_desc;

    // Sound_Update_ walks one extra 0x20-byte slot in the binary while helper
    // functions still treat 0..15 as the valid channel index range.
    extern s_sound_channel _soundChannels[16];

    void __cdecl Stop_Sound_Timer_And_Wait2_();

    void __cdecl Merge_Free_Blocks_();

    int32_t __cdecl Get_Free_Sound_Index_();

    bool __cdecl Init_Wave_Header_From_Memory_(const void* src_data, s_sound_wav_header* header);

    void __cdecl Reset_Sound_Channel_(int8_t channel_idx);

    void __cdecl Resume_Timer2_();

    extern s_lbx_stream_header _soundLbxHandle;

    extern s_wave_stream_header _streamLbxHandle;

    extern uint8_t _close_stream;

    extern uint8_t _musicActive;

    extern s_sound_stream _stream_data;

    /**
     * Drives periodic sound and stream maintenance from the multimedia timer.
     * @param timer_id Multimedia timer identifier.
     * @param message Timer callback message code.
     * @param user_data Caller-supplied timer user data.
     * @param param1 Reserved timer callback parameter.
     * @param param2 Reserved timer callback parameter.
     */
    void __stdcall Sound_Timer_Callback_(uint32_t timer_id, uint32_t message, uint32_t user_data, uint32_t param1, uint32_t param2);

    void __cdecl Update_Sound_Channels_();

    void __cdecl Refill_Sound_Channel_Buffer_(int32_t channel_idx);

    void __cdecl Update_Sound_Channel_Status_(int32_t channel_idx);

    void __cdecl Update_Stream_Status_();

    void __cdecl Adjust_Settings_();

    void __cdecl Audio_Clear_Cache_();

    void __cdecl Fade_Stream_Down_(uint8_t fade_rate);

    int32_t __cdecl Get_Stream_Volume_();

    void __cdecl Stop_Sound_Timer_();

    void __cdecl Stop_Stream_Loop_();

    void __cdecl Enable_Sound_Timer_();

    int8_t __cdecl Load_Stream_Wave_Header_(int32_t entry_index, s_sound_stream* stream);

    uint8_t __cdecl Read_Stream_Data_(void* buffer, uint32_t bytes_to_read, uint32_t* bytes_read_ptr);

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

    void __cdecl Set_Channel_Volume_(int32_t channel, int32_t volume);

    void __cdecl Set_Master_Volume_(int32_t volume);

    void __cdecl Remove_Sound_Instance_(int32_t instance_id);

    void __cdecl Stop_Sound_(int32_t channel_idx);

    void __cdecl Sound_Update_();

    int32_t __cdecl Get_Channel_Volume_Level_(int32_t channel_idx);

    int8_t __cdecl Find_Free_Channel_();

    void __cdecl Merge_Free_Sound_Blocks_();

    int32_t __cdecl Play_Sound_File_(int32_t channel_idx, void* wave_data, uint32_t wave_data_size);

    void* __cdecl Allocate_Sound_Block_(int8_t channel_idx, int32_t resource_id, int32_t size);

    int32_t __cdecl Play_Sound_(int32_t resource_id);

    void __cdecl Fadeout_Sound_(int16_t channel_idx, int8_t volume_idx);

    void __cdecl Pitch_Bend_Sound_(int32_t handle, int32_t pitch);

    void __cdecl Loop_Sound_(int32_t channel_idx, int8_t loop_count);

    void __cdecl Destroy_Sound_();

    void __cdecl Init_Sound_();
}