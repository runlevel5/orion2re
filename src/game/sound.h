#pragma once

extern "C" HRESULT WINAPI DirectSoundCreate(LPCGUID pcGuidDevice, struct IDirectSound** ppDS, IUnknown* pUnkOuter);

namespace sound {
    inline int16_t (&_ds_volume_table)[101] =
        *reinterpret_cast<int16_t (*)[101]>(std::uintptr_t{0x0055B3B8});

    inline int16_t* _stream_volume_table_end =
        reinterpret_cast<int16_t*>(std::uintptr_t{0x0055B482});

    inline uint32_t& _audioInitialized =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055B484});

    inline uint8_t& _soundOn =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055B488});

    inline uint8_t& _soundErrorOccurred =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055B48C});

    inline MMRESULT& _soundTimerId =
        *reinterpret_cast<MMRESULT*>(std::uintptr_t{0x0055B490});

    inline uint8_t& _soundTimerActive =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055B494});

    inline uint32_t& _soundTimerRunning =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055B498});

    inline uint32_t& _soundTimerStopFlag =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055B49C});

    inline const char* _default_sound_file_name =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055B4A8}); // "sound.lbx"

    inline const char* _default_music_file_name =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055B4D0}); // "music.lbx"

    inline const char* _stream_file_name =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055B4F8}); // "stream.lbx"

    inline void*& _soundBuffer =
        *reinterpret_cast<void**>(std::uintptr_t{ 0x0055B520 });

    inline uint32_t& _soundBufferSize =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{ 0x0055B524 });

    inline uint32_t& _soundTimerLock =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0055B534});

    inline void*& _streamBuffer1 =
        *reinterpret_cast<void**>(std::uintptr_t{0x0055B4A0});

    inline void*& _streamBuffer2 =
        *reinterpret_cast<void**>(std::uintptr_t{0x0055B4A4});

    inline uint8_t& _soundFxActive =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055B8F8});

    inline int32_t& _global_stream_volume =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x0055B8FC});

    inline uint8_t& _loopStream =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055B900});

    inline int32_t& _doubleBufferLoops =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x0055B904});

    inline uint8_t& _command =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055B908});

    inline uint8_t& _streamIncVal =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0055B90C});

    inline const char* s_STREAM_ERROR_0x_x_DSERR_PRIOLEV_0055b910 =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055B910}); // "STREAM ERROR:0x%x (DSERR_PRIOLEVELNEEDED) Unable to Unlock buffer. "

    inline const char* s_STREAM_ERROR_0x_x_DSERR_INVALID_0055b954 =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055B954}); // "STREAM ERROR:0x%x (DSERR_INVALIDPARAM) Unable to Unlock buffer. "

    inline const char* s_STREAM_ERROR_0x_x_DSERR_INVALID_0055b998 =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055B998}); // "STREAM ERROR:0x%x (DSERR_INVALIDCALL) Unable to Unlock buffer. "

    inline const char* s_STREAM_ERROR_Play_Stream_Unable_to_lock_buffer =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055B9D8}); // "STREAM ERROR: (Play_Stream) Unable to lock buffer."

    inline const char* s_Cant_Read = reinterpret_cast<const char*>(std::uintptr_t{0x0055BA0C});

    inline const char* s_data_0055c20c = reinterpret_cast<const char*>(std::uintptr_t{0x0055C20C}); // "data"
    inline const char* s_fmt_0055c214 = reinterpret_cast<const char*>(std::uintptr_t{0x0055C214});  // "fmt "
    inline const char* s_WAVE_0055c21c = reinterpret_cast<const char*>(std::uintptr_t{0x0055C21C}); // "WAVE"
    inline const char* s_RIFF_0055c224 = reinterpret_cast<const char*>(std::uintptr_t{0x0055C224}); // "RIFF"

    void __cdecl Copy_FourCC_(void* dest, const void* src);

    inline MMRESULT __stdcall mmioGetInfo(HMMIO hmmio, MMIOINFO* lpmmioinfo, uint32_t wFlags) {
        using Fn = MMRESULT(__stdcall*)(HMMIO, MMIOINFO*, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005AA5A8});
        return fn(hmmio, lpmmioinfo, wFlags);
    }

    inline uint32_t __stdcall mmioAdvance(HMMIO hmmio, MMIOINFO* lpmmioinfo, uint32_t wFlags) {
        using Fn = uint32_t(__stdcall*)(HMMIO, MMIOINFO*, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005AA5B4});
        return fn(hmmio, lpmmioinfo, wFlags);
    }

    inline uint32_t __stdcall mmioSetInfo(HMMIO hmmio, MMIOINFO* lpmmioinfo, uint32_t wFlags) {
        using Fn = uint32_t(__stdcall*)(HMMIO, MMIOINFO*, uint32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x005AA5B8});
        return fn(hmmio, lpmmioinfo, wFlags);
    }

    inline uint32_t& _streamAvailable2 =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0057B700});

    inline s_lbx_stream_header& _musicLbxHandle =
        *reinterpret_cast<s_lbx_stream_header*>(std::uintptr_t{0x0057B708});

    inline IDirectSound*& _dsound =
        *reinterpret_cast<IDirectSound**>(std::uintptr_t{0x0057C754});

    inline HRESULT& _dsound_hr =
        *reinterpret_cast<HRESULT*>(std::uintptr_t{0x0057C758});

    inline uint32_t& _streamAvailable1 =
        *reinterpret_cast<uint32_t*>(std::uintptr_t{0x0057C75C});

    inline int32_t& _dsMasterVolume =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x0057C760});

    inline int32_t& _dsMasterPan =
        *reinterpret_cast<int32_t*>(std::uintptr_t{0x0057C764});

    inline s_sound_index (&_soundIndices)[33] =
        *reinterpret_cast<s_sound_index (*)[33]>(std::uintptr_t{0x0057C768});

    inline LPDIRECTSOUNDBUFFER& _dsound_buffer =
        *reinterpret_cast<LPDIRECTSOUNDBUFFER*>(std::uintptr_t{0x0057CC90});

    inline DSBUFFERDESC1& _dsound_buffer_desc =
        *reinterpret_cast<DSBUFFERDESC1*>(std::uintptr_t{0x0057CC98});

    inline s_sound_channel (&_sound_channels)[16] =
        *reinterpret_cast<s_sound_channel (*)[16]>(std::uintptr_t{0x0057CCB0});

    inline const char* s_SOUND_ERROR_0x_x__DSERR_PRIOLEVE_0055b538 =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055B538}); // "SOUND ERROR:0x%x (DSERR_PRIOLEVELNEEDED) Unable to Unlock buffer. "
    inline const char* s_SOUND_ERROR_0x_x__DSERR_INVALIDP_0055b57c =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055B57C}); // "SOUND ERROR:0x%x (DSERR_INVALIDPARAM) Unable to Unlock buffer. "
    inline const char* s_SOUND_ERROR_0x_x__DSERR_INVALIDC_0055b5bc =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055B5BC}); // "SOUND ERROR:0x%x (DSERR_INVALIDCALL) Unable to Unlock buffer. "
    inline const char* s_SOUND_ERROR___Play_Sound_File__U_0055b5fc =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055B5FC}); // "SOUND ERROR: (Play_Sound_File) Unable to lock buffer."
    inline const char* s_SOUND_ERROR__Reading_Wave_0055b634 =
        reinterpret_cast<const char*>(std::uintptr_t{0x0055B634}); // "SOUND ERROR: Reading Wave"

    void __cdecl Stop_Sound_Timer_And_Wait2_();

    void __cdecl Merge_Free_Blocks_();

    int32_t __cdecl Get_Free_Sound_Index_();

    bool __cdecl Init_Wave_Header_From_Memory_(const void* src_data, s_sound_wav_header* header);

    void __cdecl Reset_Sound_Channel_(int8_t channel_idx);

    void __cdecl Resume_Timer2_();

    inline s_lbx_stream_header& _soundLbxHandle =
        *reinterpret_cast<s_lbx_stream_header*>(std::uintptr_t{0x0057CEB0});

    inline s_wave_stream_header& _streamLbxHandle =
        *reinterpret_cast<s_wave_stream_header*>(std::uintptr_t{0x00589A20});

    inline uint8_t& _musicActive =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0058AA6C});

    inline s_sound_stream& _stream_data =
        *reinterpret_cast<s_sound_stream*>(std::uintptr_t{0x0057E130});

    void __stdcall Sound_Timer_Callback_(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5);

    void __cdecl Update_Sound_Channels_();

    void __cdecl Refill_Sound_Channel_Buffer_(int channel_idx);

    void __cdecl Update_Sound_Channel_Status_(int channel_idx);

    void __cdecl Update_Stream_Status_();

    void __cdecl Adjust_Settings_();

    void __cdecl Audio_Clear_Cache_();

    void __cdecl Fade_Stream_Down_(uint8_t fade_rate);

    int32_t __cdecl Get_Stream_Volume_();

    void __cdecl Stop_Sound_Timer_();

    void __cdecl Stop_Stream_Loop_();

    void __cdecl Enable_Sound_Timer_();

    int8_t __cdecl Load_Stream_Wave_Header_(int entry_index, s_sound_stream* stream);

    uint8_t __cdecl Read_Stream_Data_(void* buffer, uint32_t bytes_to_read, uint32_t* bytes_read_ptr);

    void __cdecl Stop_Stream_();

    void __cdecl Stream_On_();

    void __cdecl Set_Stream_Max_Volume_(int32_t volume);

    void __cdecl Set_Stream_Filename_(const char* filename);

    void __cdecl Play_Stream_(int32_t stream_id);

    void __cdecl Loop_Stream_(int32_t count);

    void __cdecl Set_Stream_Volume_(int32_t volume_percent);

    void __cdecl Fade_Stream_Up_(uint8_t fade_increment);

    inline int32_t __cdecl Is_Stream_Playing_() {
        using Fn = int32_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00532050});
        return fn();
    }

    void __cdecl Set_Audio_Buffer_(void* buffer);

    void __cdecl Sound_On_();

    void __cdecl Stop_All_Sounds_();

    void __cdecl Resume_Sound_(int32_t channel);

    void __cdecl Set_Sound_Max_Volume_(int32_t volume);

    void __cdecl Set_Channel_Volume_(int32_t channel, int32_t volume);

    void __cdecl Set_Master_Volume_(int32_t volume);

    void __cdecl Remove_Sound_Instance_(int32_t instance_id);

    void __cdecl Stop_Sound_(int32_t channel_idx);

    void __cdecl Sound_Update_();

    int __cdecl Get_Channel_Volume_Level_(int channel_idx);

    int8_t __cdecl Find_Free_Channel_();

    void __cdecl Merge_Free_Sound_Blocks_();

    int32_t __cdecl Play_Sound_File_(int32_t channel_idx, void* wave_data, uint32_t size);

    void* __cdecl Allocate_Sound_Block_(int32_t channel, int32_t resource_id, int32_t size);

    int32_t __cdecl Play_Sound_(int32_t resource_id);

    void __cdecl Fadeout_Sound_(int16_t channel_idx, int8_t volume_idx);

    void __cdecl Pitch_Bend_Sound_(int32_t handle, int32_t pitch);

    void __cdecl Loop_Sound_(int channel_idx, int8_t loop_count);

    void __cdecl Init_Sound_();
}