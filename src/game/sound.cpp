#include "pch.h"

namespace sound {
    /**
     * @brief Sets the internal audio buffer and updates its available size.
     *
     * @param buffer Pointer to the memory block to be used as the audio buffer.
     */
    void __cdecl Set_Audio_Buffer_(void* buffer) {
        if (buffer == nullptr) {
            _soundBufferSize = 0;
            _soundBuffer = nullptr;
        }
        else {
            uint32_t remaining_space = allocate::Get_Remaining_Block_Space_(buffer);
            _soundBufferSize = remaining_space;

            if (remaining_space != 0) {
                _soundBuffer = buffer;
            }
            else {
                _soundBuffer = nullptr;
            }
        }

        Audio_Clear_Cache_();
    }

    void __cdecl Audio_Clear_Cache_() {
        sound::Stop_All_Sounds_();

        uint32_t buffer_size = sound::_soundBufferSize;
        if (sound::_soundBuffer != nullptr) {
            memset(sound::_soundBuffer, 0, buffer_size);
        }

        memset(sound::_soundIndices, 0, sizeof(sound::_soundIndices));

        for (int i = 0; i < 33; ++i) {
            sound::_soundIndices[i].next_id = -1;
            sound::_soundIndices[i].prev_id = -1;
        }

        sound::_soundIndices[0].next_id = 1;
        sound::_soundIndices[1].buffer_size = buffer_size;
        sound::_soundIndices[0].resource_id = -1;
        sound::_soundIndices[0].priority = 1000000;
        sound::_soundIndices[1].prev_id = 0;
    }

    void __cdecl Fade_Stream_Down_(uint8_t fade_rate) {
        _command = 2;
        _streamIncVal = fade_rate;
    }

    void __cdecl Stream_On_() {
        return;
    }

    void __cdecl Set_Stream_Max_Volume_(int32_t volume) {
        if (volume >= 0 && volume <= 100) {
            _global_stream_volume = volume;
            Set_Stream_Volume_(volume);
        }
    }

    void __cdecl Set_Stream_Filename_(const char* filename) {
        if (sound::_dsound != nullptr) {
            lbx::LBX_init_stream_(&sound::_streamLbxHandle, filename);
        }
    }

    void __cdecl Init_Sound_() {
        _dsound = nullptr;
        _dsound_hr = DirectSoundCreate(nullptr, &_dsound, nullptr);

        if (_dsound != nullptr) {
            memset(&_soundLbxHandle, 0, sizeof(s_lbx_stream_header));
            memset(&_musicLbxHandle, 0, sizeof(s_lbx_stream_header));

            lbx::LBX_init_(&_soundLbxHandle, _default_sound_file_name);
            lbx::LBX_init_(&_musicLbxHandle, _default_music_file_name);
            lbx::LBX_init_stream_(&_streamLbxHandle, _stream_file_name);

            if (_soundBuffer != nullptr) {
                _soundBufferSize = allocate::Get_Remaining_Block_Space_(_soundBuffer);
                memset(_soundBuffer, 0, _soundBufferSize);
            }

            memset(_soundIndices, 0, sizeof(_soundIndices));

            for (int i = 0; i < 33; ++i) {
                _soundIndices[i].next_id = -1;
                _soundIndices[i].prev_id = -1;
            }

            _soundIndices[1].buffer_size = _soundBufferSize;
            _soundIndices[1].prev_id = 0;
            _soundIndices[0].next_id = 1;
            _soundIndices[0].resource_id = -1;
            _soundIndices[0].priority = 1000000 /* 0xf4240 */;

            if (timeBeginPeriod(160 /* 0xa0 */) == TIMERR_NOERROR) {
                _soundTimerId = timeSetEvent(160 /* 0xa0 */, 10, (LPTIMECALLBACK)Sound_Timer_Callback_, 0, TIME_PERIODIC /* 1 */);
                if (_soundTimerId != 0) {
                    _soundTimerActive = 1;
                    _dsound_hr = _dsound->SetCooperativeLevel(platform::g_main_window, DSSCL_PRIORITY /* 2 */);

                    memset(&_dsound_buffer_desc, 0, sizeof(_dsound_buffer_desc));
                    _dsound_buffer_desc.dwSize = sizeof(_DSBUFFERDESC1);
                    _dsound_buffer_desc.dwFlags = DSBCAPS_PRIMARYBUFFER /* 1 */;
                    
                    _dsound->CreateSoundBuffer((LPCDSBUFFERDESC)&_dsound_buffer_desc, &_dsound_buffer, nullptr);
                    _dsound_buffer->Play(0, 0, DSBPLAY_LOOPING /* 1 */);

                    WAVEFORMATEX wfx;
                    memset(&wfx, 0, sizeof(WAVEFORMATEX));
                    wfx.wFormatTag = WAVE_FORMAT_PCM /* 1 */;
                    wfx.nChannels = 2;
                    wfx.nSamplesPerSec = 22050 /* 0x5622 */;
                    wfx.nAvgBytesPerSec = 88200 /* 0x15888 */;
                    wfx.nBlockAlign = 4;
                    wfx.wBitsPerSample = 16;

                    memset(&_dsound_buffer_desc, 0, sizeof(_dsound_buffer_desc));
                    _dsound_buffer_desc.dwSize = sizeof(_DSBUFFERDESC1);
                    _dsound_buffer_desc.lpwfxFormat = &wfx;
                    _dsound_buffer_desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLPAN | DSBCAPS_GETCURRENTPOSITION2 /* 0xe0 */;
                    _dsound_buffer_desc.dwBufferBytes = 16384 /* 0x4000 */;

                    for (int i = 0; i < 16; ++i) {
                        _dsound_hr = _dsound->CreateSoundBuffer((LPCDSBUFFERDESC)&_dsound_buffer_desc, &_soundChannels[i].buffer, nullptr);
                        if (_dsound_hr != DS_OK) {
                            _dsound->Release();
                            return;
                        }
                        
                        // Zeros the resource_id and fields array in s_sound_channel (first 28 bytes)
                        memset(&_soundChannels[i], 0, offsetof(s_sound_channel, buffer));
                        _soundChannels[i].buffer->Stop();
                    }

                    _dsound_buffer->SetFormat(&wfx);
                    _dsound_buffer->GetVolume((LPLONG)&_dsMasterVolume);
                    _dsound_buffer->GetPan((LPLONG)&_dsMasterPan);

                    if (_dsound != nullptr) {
                        _soundOn = 1;
                        _soundFxActive = 1;
                        _audioInitialized = 1;
                        _musicActive = 1;
                    }
                    _soundErrorOccurred = 0;
                }
            }
        }
    }

    void __cdecl Loop_Stream_(int32_t count) {
        if (count == -1) {
            count = 0xFFFF;
        }

        _loopStream = 1;
        _doubleBufferLoops = count;
    }

    void __cdecl Fade_Stream_Up_(uint8_t fade_increment) {
        _command = 1;
        _streamIncVal = fade_increment;
    }

    void __cdecl Sound_On_() {
        if (_audioInitialized == 1) {
            _soundOn = 1;
        }
    }

    void __cdecl Stop_All_Sounds_() {
        if (sound::_soundOn != 0) {
            for (int32_t i = 0; i <= 16; ++i) {
                sound::Stop_Sound_(i);
            }
        }
    }

    void __cdecl Resume_Sound_(int32_t channel) {
        return;
    }
}