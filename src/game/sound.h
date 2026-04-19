#pragma once

extern "C" HRESULT WINAPI DirectSoundCreate(LPCGUID pcGuidDevice, struct IDirectSound** ppDS, IUnknown* pUnkOuter);

namespace sound {
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

    inline s_lbx_stream_header& _musicLbxHandle =
        *reinterpret_cast<s_lbx_stream_header*>(std::uintptr_t{0x0057B708});

    inline IDirectSound*& _dsound =
        *reinterpret_cast<IDirectSound**>(std::uintptr_t{0x0057C754});

    inline HRESULT& _dsound_hr =
        *reinterpret_cast<HRESULT*>(std::uintptr_t{0x0057C758});

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

    inline s_sound_channel (&_soundChannels)[16] =
        *reinterpret_cast<s_sound_channel (*)[16]>(std::uintptr_t{0x0057CCB0});

    inline s_lbx_stream_header& _soundLbxHandle =
        *reinterpret_cast<s_lbx_stream_header*>(std::uintptr_t{0x0057CEB0});

    inline s_lbx_stream_header& _streamLbxHandle =
        *reinterpret_cast<s_lbx_stream_header*>(std::uintptr_t{0x00589A20});

    inline uint8_t& _musicActive =
        *reinterpret_cast<uint8_t*>(std::uintptr_t{0x0058AA6C});

    inline void __stdcall Sound_Timer_Callback_(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2) {
        using Fn = void(__stdcall*)(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052D8C0});
        fn(uTimerID, uMsg, dwUser, dw1, dw2);
    }

    void __cdecl Audio_Clear_Cache_();

    void __cdecl Fade_Stream_Down_(uint8_t fade_rate);

    inline int16_t __cdecl Get_Stream_Volume_() {
        using Fn = int16_t(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00532080 });
        return fn();
    }

    inline void __cdecl Stop_Stream_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00531FE0 });
        fn();
    }

    inline void __cdecl thunk_Stop_Stream_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x00532030});
        fn();
    }

    void __cdecl Stream_On_();

    void __cdecl Set_Stream_Max_Volume_(int32_t volume);

    void __cdecl Set_Stream_Filename_(const char* filename);

    inline void __cdecl Play_Stream_(int32_t stream_id) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x00532130 });
        fn(stream_id);
    }

    void __cdecl Loop_Stream_(int32_t count);

    inline void __cdecl Set_Stream_Volume_(int32_t volume) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x005320E0 });
        fn(volume);
    }

    void __cdecl Fade_Stream_Up_(uint8_t fade_increment);

    void __cdecl Set_Audio_Buffer_(void* buffer);

    void __cdecl Sound_On_();

    void __cdecl Stop_All_Sounds_();

    void __cdecl Resume_Sound_(int32_t channel);

    inline void __cdecl Set_Sound_Max_Volume_(int32_t volume) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{ 0x0052D500 });
        fn(volume);
    }

    inline void __cdecl Stop_Sound_(int32_t handle) {
        using Fn = void(__cdecl*)(int32_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052e6b0});
        fn(handle);
    }

    inline void __cdecl Sound_Update_() {
        using Fn = void(__cdecl*)();
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052E630});
        fn();
    }

    inline int __cdecl Play_Sound_(int16_t sound_id) {
        using Fn = int(__cdecl*)(int16_t);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052de10});
        return fn(sound_id);
    }

    inline void __cdecl Fadeout_Sound_(int32_t handle, int32_t duration) {
        using Fn = void(__cdecl*)(int32_t, int32_t);
        reinterpret_cast<Fn>(0x0052e850)(handle, duration);
    }

    inline void __cdecl Pitch_Bend_Sound_(int32_t handle, int32_t pitch) {
        using Fn = void(__cdecl*)(int32_t, int32_t);
        reinterpret_cast<Fn>(std::uintptr_t{0x0052D520})(handle, pitch);
    }

    inline void __cdecl Loop_Sound_(int handle, int loop_count) {
        using Fn = void(__cdecl*)(int, int);
        auto fn = reinterpret_cast<Fn>(std::uintptr_t{0x0052d560});
        fn(handle, loop_count);
    }

    void __cdecl Init_Sound_();
}