#include "pch.h"

#include <dsound.h>
#include <mmsystem.h>

namespace sound {
    extern "C" HRESULT WINAPI DirectSoundCreate(LPCGUID pcGuidDevice, struct IDirectSound** ppDS, IUnknown* pUnkOuter);

    constexpr size_t k_stream_filename_capacity = 260;

    void __cdecl Copy_FourCC_(char* dest, const char* src);
    void __cdecl Stop_Sound_Timer_And_Wait2_();
    void __cdecl Merge_Free_Blocks_();
    int32_t __cdecl Get_Free_Sound_Index_();
    bool __cdecl Init_Wave_Header_From_Memory_(const void* src_data, s_sound_wav_header* header);
    void __cdecl Reset_Sound_Channel_(int8_t channel_idx);
    void __cdecl Resume_Timer2_();
    void __cdecl Update_Sound_Channels_();
    void __cdecl Refill_Sound_Channel_Buffer_(int32_t channel_idx);
    void __cdecl Update_Sound_Channel_Status_(int32_t channel_idx);
    void __cdecl Update_Stream_Status_();
    void __cdecl Adjust_Settings_();
    void __cdecl Audio_Clear_Cache_();
    void __cdecl Stop_Sound_Timer_();
    void __cdecl Enable_Sound_Timer_();
    int8_t __cdecl Load_Stream_Wave_Header_(int32_t entry_index, s_sound_stream* stream);
    uint8_t __cdecl Read_Stream_Data_(void* buffer, uint32_t bytes_to_read, uint32_t* bytes_read_ptr);
    void __cdecl Set_Channel_Volume_(int32_t channel, int32_t volume);
    void __cdecl Set_Master_Volume_(int32_t volume);
    void __cdecl Remove_Sound_Instance_(int32_t instance_id);
    int32_t __cdecl Get_Channel_Volume_Level_(int32_t channel_idx);
    int8_t __cdecl Find_Free_Channel_();
    void __cdecl Merge_Free_Sound_Blocks_();
    int32_t __cdecl Play_Sound_File_(int32_t channel_idx, void* wave_data, uint32_t wave_data_size);
    void* __cdecl Allocate_Sound_Block_(int8_t channel_idx, int32_t resource_id, int32_t size);
    void __cdecl Stop_Stream_Loop_();

    int16_t _ds_volume_table[101] = {
        -10000, -10000, -10000, -10000, -9287, -8643, -8117, -7673, -7287, -6947,
        -6643, -6368, -6117, -5886, -5673, -5473, -5287, -5112, -4947, -4791,
        -4643, -4503, -4368, -4240, -4117, -4000, -3886, -3777, -3673, -3571,
        -3473, -3379, -3287, -3198, -3112, -3029, -2947, -2868, -2791, -2716,
        -2643, -2572, -2503, -2435, -2368, -2304, -2240, -2178, -2117, -2058,
        -2000, -1942, -1886, -1831, -1777, -1724, -1673, -1621, -1571, -1522,
        -1473, -1426, -1379, -1333, -1287, -1242, -1198, -1155, -1112, -1070,
        -1029, -988, -947, -908, -868, -830, -791, -754, -716, -680,
        -643, -608, -572, -537, -503, -468, -435, -401, -368, -336,
        -304, -272, -240, -209, -178, -148, -117, -87, -58, -28,
        0
    };

    uint32_t _audioInitialized;
    uint8_t _soundOn;
    uint8_t _soundErrorOccurred;
    MMRESULT _soundTimerId;
    uint8_t _soundTimerActive;
    volatile uint32_t _soundTimerRunning = 1;
    volatile uint32_t _soundTimerStopFlag;
    void* _soundBuffer;
    uint32_t _soundBufferSize;
    volatile uint32_t _soundTimerLock;
    void* _streamBuffer1;
    void* _streamBuffer2;
    uint8_t _soundFxActive;
    int32_t _global_stream_volume = 100;
    uint8_t _loopStream;
    int32_t _doubleBufferLoops;
    uint8_t _command;
    uint8_t _streamIncVal;
    uint32_t _streamAvailable2;
    s_lbx_stream_header _musicLbxHandle;
    IDirectSound* _dsound;
    HRESULT _dsound_hr;
    uint32_t _streamAvailable1;
    int32_t _dsMasterVolume;
    int32_t _dsMasterPan;
    s_sound_index _soundIndices[33];
    LPDIRECTSOUNDBUFFER _dsound_buffer;
    DSBUFFERDESC1 _dsound_buffer_desc;
    s_sound_channel _soundChannels[16];
    s_lbx_stream_header _soundLbxHandle;
    s_wave_stream_header _streamLbxHandle;
    uint8_t _close_stream = 1;
    uint8_t _musicActive;
    s_sound_stream _stream_data;
    char _currentStreamFilename[k_stream_filename_capacity] = "stream.lbx";
    int32_t _currentStreamId;
    uint8_t _streamReadErrorLogged;

    namespace {
        void Debug_Log_Stream_Message_(const char* format, ...) {
            char message[512];
            va_list args;
            va_start(args, format);
            vsnprintf(message, sizeof(message), format, args);
            va_end(args);

            SDL_Log("%s", message);
        }
    }

    void* __cdecl Get_Native_Sound_Device_() {
        return _dsound;
    }

    /**
     * @brief Iterates through active sound channels and updates their volume levels via DirectSound.
     * Stops and removes channels if the volume delta is out of bounds.
     */
    void __cdecl Sound_Update_() {
        for (int i = 0; i < 16; ++i) {
            s_sound_channel& channel = _soundChannels[i];

            // Check if bit 0 is set (active flag) and volume is non-zero
            if ((channel.flags & 1) && channel.volume_index != 0) {
                int32_t vol_level = Get_Channel_Volume_Level_(i);
                int32_t diff = vol_level - (int32_t)channel.volume_index;

                // Valid delta range is [1, 100]
                if (diff >= 1 && diff <= 100) {
                    int16_t ds_vol = _ds_volume_table[diff];
                    channel.buffer->SetVolume((int32_t)ds_vol);
                } else {
                    // Out of range: stop and cleanup
                    channel.flags ^= 1;
                    Remove_Sound_Instance_(i);
                    channel.buffer->Stop();
                    channel.volume_index = 0;
                    return; // Function exits immediately after processing one cleanup case
                }
            }
        }
    }

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

    void __cdecl Reset_Sound_Channel_(int8_t channel_idx) {
        _soundChannels[channel_idx].sample_data = nullptr;
        _soundChannels[channel_idx].sample_size = 0;
        _soundChannels[channel_idx].sample_offset = 0;
        _soundChannels[channel_idx].ds_buffer_size = 0;
        _soundChannels[channel_idx].field7_0x13 = 0;
        _soundChannels[channel_idx].write_cursor = 0;
        _soundChannels[channel_idx].flags = 0;
        _soundChannels[channel_idx].volume_index = 0;
        _soundChannels[channel_idx].buffer->Stop();
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
        sound::_soundIndices[0].num_active_channels = 1000000;
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

    void __cdecl Set_Sound_Max_Volume_(int32_t volume) {
        if (volume >= 0 && volume <= 100) {
            sound::_dsMasterVolume = volume;
            sound::Set_Channel_Volume_(-1, volume);
        }
    }

    void __cdecl Set_Stream_Filename_(const char* filename) {
        if (sound::_dsound != nullptr) {
            strncpy(_currentStreamFilename, filename, sizeof(_currentStreamFilename) - 1);
            _currentStreamFilename[sizeof(_currentStreamFilename) - 1] = '\0';
            lbx::LBX_init_stream_(&sound::_streamLbxHandle, filename);
        }
    }

    bool __cdecl Init_Wave_Header_From_Memory_(const void* src_data, s_sound_wav_header* header) {
        char tag_buf[5];
        tag_buf[4] = '\0';

        // Copy standard RIFF/WAVE header fields from source buffer
        memcpy(header, src_data, sizeof(s_sound_wav_header));

        // Set data pointer to the start of raw audio data in the source buffer
        header->data_ptr = (uint8_t*)src_data + 0x2c;

        sound::Copy_FourCC_(tag_buf, header->riff_tag);
        if (cstrcmpi(tag_buf, "RIFF") != 0) {
            return false;
        }

        sound::Copy_FourCC_(tag_buf, header->wave_tag);
        if (cstrcmpi(tag_buf, "WAVE") != 0) {
            return false;
        }

        sound::Copy_FourCC_(tag_buf, header->fmt_tag);
        if (cstrcmpi(tag_buf, "fmt ") != 0) {
            return false;
        }

        sound::Copy_FourCC_(tag_buf, header->data_tag);
        return cstrcmpi(tag_buf, "data") == 0;
    }

    /**
     * Releases DirectSound resources and stops the periodic sound timer.
     */
    void __cdecl Destroy_Sound_() {
        Stop_Sound_Timer_And_Wait2_();
        timeKillEvent(_soundTimerId);
        timeEndPeriod(0xA0);

        if (_soundOn == 1 && _dsound != NULL) {
            _dsound->Release();
            _dsound = NULL;
        }

        _audioInitialized = 0;
    }

    void __cdecl Init_Sound_() {
        _dsound = nullptr;
        _dsound_hr = DirectSoundCreate(nullptr, &_dsound, nullptr);

        if (_dsound != nullptr) {
            memset(&_soundLbxHandle, 0, sizeof(s_lbx_stream_header));
            memset(&_musicLbxHandle, 0, sizeof(s_lbx_stream_header));

            lbx::LBX_init_(&_soundLbxHandle, "sound.lbx");
            lbx::LBX_init_(&_musicLbxHandle, "music.lbx");
            lbx::LBX_init_stream_(&_streamLbxHandle, "stream.lbx");

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
            _soundIndices[0].num_active_channels = 1000000 /* 0xf4240 */;

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
                        
                        _soundChannels[i].sample_data = nullptr;
                        _soundChannels[i].sample_size = 0;
                        _soundChannels[i].sample_offset = 0;
                        _soundChannels[i].ds_buffer_size = 0;
                        _soundChannels[i].field7_0x13 = 0;
                        _soundChannels[i].write_cursor = 0;
                        _soundChannels[i].flags = 0;

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

    void __stdcall Sound_Timer_Callback_(uint32_t timer_id, uint32_t message, uint32_t user_data, uint32_t param1, uint32_t param2) {
        if (sound::_soundTimerLock == 1) {
            return;
        }

        sound::_soundTimerLock = 1;

        if (sound::_soundTimerRunning == 0) {
            if (sound::_soundTimerStopFlag != 0) {
                sound::_soundTimerLock = 0;
                return;
            }
            sound::_soundTimerRunning = 1;
        }

        if (sound::_soundOn == 1 && sound::_audioInitialized == 1) {
            sound::Update_Sound_Channels_();
        }

        if (sound::_soundFxActive == 1 && sound::_musicActive == 1) {
            sound::Adjust_Settings_();
        }

        if (sound::_soundTimerStopFlag != 0) {
            sound::_soundTimerRunning = 0;
        }

        sound::_soundTimerLock = 0;
    }

    /**
     * Requests the periodic sound timer to stop and drains in-flight work.
     */
    void __cdecl Stop_Sound_Timer_() {
        sound::_soundTimerStopFlag = 1;

        while (sound::_soundTimerRunning != 0) {
            sound::Sound_Timer_Callback_(0, 0, 0, 0, 0);
        }
    }

    void __cdecl Stop_Sound_Timer_And_Wait2_() {
        sound::_soundTimerStopFlag = 1;

        while (sound::_soundTimerRunning != 0) {
            sound::Sound_Timer_Callback_(0, 0, 0, 0, 0);
        }
    }

    /**
     * Enables the sound timer by clearing the stop flag.
     */
    void __cdecl Enable_Sound_Timer_() {
        _soundTimerStopFlag = 0;
    }

    void __cdecl Resume_Timer2_() {
        _soundTimerStopFlag = 0;
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

    /**
     * Sets the volume of the sound stream buffer, capped by the global stream volume limit.
     * @param volume_percent The desired volume as a percentage (0-100).
     */
    void __cdecl Set_Stream_Volume_(int32_t volume_percent) {
        if (sound::_soundFxActive != 0 && _stream_data.ds_buffer != nullptr) {
            // Validate that the volume percentage is within the expected range
            if (volume_percent >= 0 && volume_percent <= 100) {
                // Cap the requested volume to the global stream volume limit
                if (volume_percent > sound::_global_stream_volume) {
                    volume_percent = sound::_global_stream_volume;
                }

                // Look up the DirectSound volume value (in hundredths of decibels) from the table
                int32_t ds_volume = (int32_t)sound::_ds_volume_table[volume_percent];

                // Update the DirectSound buffer volume
                _stream_data.ds_buffer->SetVolume(ds_volume);
            }
        }
    }

    void __cdecl Sound_On_() {
        if (_audioInitialized == 1) {
            _soundOn = 1;
        }
    }

    /**
     * Sets one channel volume, or updates the master volume when channel_idx is -1.
     */
    void __cdecl Set_Channel_Volume_(int32_t channel_idx, int32_t volume) {
        if (sound::_soundOn == 0) {
            return;
        }

        if (channel_idx == -1) {
            sound::Set_Master_Volume_(volume);
            return;
        }

        if (volume >= 0 && volume <= 100) {
            if (sound::_dsMasterVolume < volume) {
                volume = sound::_dsMasterVolume;
            }

            if (channel_idx >= 0 && channel_idx < 16) {
                int16_t ds_volume = sound::_ds_volume_table[volume];
                LPDIRECTSOUNDBUFFER ds_buffer = _soundChannels[channel_idx].buffer;

                ds_buffer->SetVolume((long)ds_volume);
            }
        }
    }

    void __cdecl Stop_Sound_(int32_t channel_idx) {
        if (sound::_soundOn == 0) {
            return;
        }

        if (channel_idx == -1) {
            sound::Stop_All_Sounds_();
            return;
        }

        if (channel_idx >= 0 && channel_idx < 16) {
            s_sound_channel* channel = &sound::_soundChannels[channel_idx];
            if ((channel->flags & 1) != 0) {
                channel->buffer->Stop();
                channel->flags ^= 1;
                sound::Remove_Sound_Instance_(channel_idx);
            }
        }
    }

    void __cdecl Stop_All_Sounds_() {
        if (sound::_soundOn != 0) {
            for (int32_t i = 0; i <= 16; ++i) {
                sound::Stop_Sound_(i);
            }
        }
    }

    /**
     * Placeholder stub for resuming sound playback on a channel.
     * @param channel_idx Channel index to resume.
     */
    void __cdecl Resume_Sound_(int32_t channel_idx) {
        (void)channel_idx;
        return;
    }

    int32_t __cdecl Get_Stream_Volume_() {
        if (sound::_soundFxActive != 0 && _stream_data.ds_buffer != nullptr) {
            LONG current_volume = 0;
            _stream_data.ds_buffer->GetVolume(&current_volume);

            int32_t index = 0;
            for (int16_t* entry = sound::_ds_volume_table; entry < sound::_ds_volume_table + 101; ++entry) {
                if ((int32_t)*entry == current_volume) {
                    return index;
                }
                index++;
            }

            return (int32_t)current_volume;
        }

        return 0;
    }

    void __cdecl Stop_Stream_() {
        if (_soundFxActive != 0 && _stream_data.ds_buffer != nullptr) {
            Stop_Sound_Timer_();

            _stream_data.ds_buffer->Stop();
            _stream_data.ds_buffer->Release();

            _stream_data.ds_buffer = nullptr;
            _loopStream = 0;
            _soundFxActive = 0;

            Stop_Stream_Loop_();
            Enable_Sound_Timer_();
        }
    }

    void __cdecl Pitch_Bend_Sound_(int32_t handle, int32_t pitch) {
        if (handle >= 1) {
            LPDIRECTSOUNDBUFFER buffer = _soundChannels[handle].buffer;

            DWORD frequency;
            buffer->GetFrequency(&frequency);

            frequency += (DWORD)pitch;

            buffer->SetFrequency(frequency);
        }
    }

    int8_t __cdecl Load_Stream_Wave_Header_(int32_t entry_index, s_sound_stream* stream) {
        if (entry_index == 0) {
            Debug_Log_Stream_Message_("stream: rejected empty entry file=%s entry=%d", _currentStreamFilename, entry_index);
            return 0;
        }

        uint32_t entry_offset = _streamLbxHandle.entries[entry_index - 1].offset;

        mmioSeek((HMMIO)_streamLbxHandle.file, entry_offset, 0);

        char tag_buffer[5];
        tag_buffer[4] = '\0';

        mmioRead((HMMIO)_streamLbxHandle.file, (HPSTR)stream->riff_tag, 0x30);
        stream->data_start_offset = entry_offset + 0x2c;

        sound::Copy_FourCC_(tag_buffer, stream->riff_tag);
        if (cstrcmpi(tag_buffer, "RIFF") != 0) {
            Debug_Log_Stream_Message_("stream: invalid RIFF tag file=%s entry=%d tag=%.4s", _currentStreamFilename, entry_index, tag_buffer);
            return 0;
        }

        sound::Copy_FourCC_(tag_buffer, stream->wave_tag);
        if (cstrcmpi(tag_buffer, "WAVE") != 0) {
            Debug_Log_Stream_Message_("stream: invalid WAVE tag file=%s entry=%d tag=%.4s", _currentStreamFilename, entry_index, tag_buffer);
            return 0;
        }

        sound::Copy_FourCC_(tag_buffer, stream->fmt_tag);
        if (cstrcmpi(tag_buffer, "fmt ") != 0) {
            Debug_Log_Stream_Message_("stream: invalid fmt tag file=%s entry=%d tag=%.4s", _currentStreamFilename, entry_index, tag_buffer);
            return 0;
        }

        sound::Copy_FourCC_(tag_buffer, stream->data_tag);
        if (cstrcmpi(tag_buffer, "data") != 0) {
            Debug_Log_Stream_Message_("stream: invalid data tag file=%s entry=%d tag=%.4s", _currentStreamFilename, entry_index, tag_buffer);
            return 0;
        }

        mmioSeek((HMMIO)_streamLbxHandle.file, stream->data_start_offset, 0);

        return 1;
    }

    void __cdecl Play_Stream_(int32_t stream_id) {
        char msg_buf[200];
        WAVEFORMATEX wfx;
        DSBUFFERDESC dsbd;
        uint32_t dwBytes;
        uint32_t bytes_read;

        if (stream_id == 0 || _musicActive == 0) {
            return;
        }

        _currentStreamId = stream_id;
        _streamReadErrorLogged = 0;
        Debug_Log_Stream_Message_("stream: play file=%s entry=%d", _currentStreamFilename, _currentStreamId);

        Stop_Sound_Timer_();
        int32_t volume_idx = Get_Stream_Volume_();
        Stop_Stream_Loop_();

        if (_stream_data.ds_buffer != nullptr) {
            _stream_data.ds_buffer->Stop();
            _stream_data.ds_buffer->Release();
            _stream_data.ds_buffer = nullptr;
        }

        if (Load_Stream_Wave_Header_(stream_id, &_stream_data) == 0) {
            Enable_Sound_Timer_();
            return;
        }

        _stream_data.block_size = ((_stream_data.avg_bytes_per_sec * 320) / 100) / 20;
        _stream_data.data_remaining = _stream_data.total_data_size;
        _stream_data.ds_buffer_size = _stream_data.block_size * 20;

        memset(&wfx, 0, sizeof(WAVEFORMATEX));
        wfx.wFormatTag = WAVE_FORMAT_PCM;
        wfx.nChannels = _stream_data.channels;
        wfx.nSamplesPerSec = _stream_data.samples_per_sec;
        wfx.nAvgBytesPerSec = _stream_data.avg_bytes_per_sec;
        wfx.nBlockAlign = _stream_data.block_align;
        wfx.wBitsPerSample = _stream_data.bits_per_sample;
        wfx.cbSize = 0;

        memset(&dsbd, 0, sizeof(DSBUFFERDESC));
        dsbd.dwSize = 20; /* sizeof(DSBUFFERDESC) for older DirectX */
        dsbd.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY;
        dsbd.dwBufferBytes = _stream_data.ds_buffer_size;
        dsbd.lpwfxFormat = &wfx;

        _dsound_hr = _dsound->CreateSoundBuffer(&dsbd, &_stream_data.ds_buffer, nullptr);
        if (_dsound_hr != 0) {
            return;
        }

        if (_stream_data.ds_buffer_size < _stream_data.data_remaining) {
            _stream_data.status |= 2;
            dwBytes = _stream_data.ds_buffer_size;
        } else {
            dwBytes = _stream_data.data_remaining;
        }

        _dsound_hr = _stream_data.ds_buffer->Lock(
            0,
            dwBytes,
            &_streamBuffer1,
            (LPDWORD)&_streamAvailable1,
            &_streamBuffer2,
            (LPDWORD)&_streamAvailable2,
            0
        );

        if (_dsound_hr != 0) {
            strcpy(&msg_buf[0], "STREAM ERROR: (Play_Stream) Unable to lock buffer.");
            exitm::Exit_With_Message(&msg_buf[0]);
        }

        if (_streamAvailable1 != 0) {
            Read_Stream_Data_(_streamBuffer1, _streamAvailable1, &bytes_read);
            _stream_data.bytes_written += bytes_read;
        }

        if (_streamAvailable2 != 0) {
            Read_Stream_Data_(_streamBuffer2, _streamAvailable2, &bytes_read);
            _stream_data.bytes_written += bytes_read;
        }

        _dsound_hr = _stream_data.ds_buffer->Unlock(
            _streamBuffer1,
            _streamAvailable1,
            _streamBuffer2,
            _streamAvailable2
        );

        if (_dsound_hr != 0) {
            if (_dsound_hr == DSERR_INVALIDPARAM) {
                sprintf(msg_buf, "STREAM ERROR:0x%x (DSERR_INVALIDPARAM) Unable to Unlock buffer. ", _dsound_hr);
            } else if (_dsound_hr == DSERR_INVALIDCALL) {
                sprintf(msg_buf, "STREAM ERROR:0x%x (DSERR_INVALIDCALL) Unable to Unlock buffer. ", _dsound_hr);
            } else if (_dsound_hr == DSERR_PRIOLEVELNEEDED) {
                sprintf(msg_buf, "STREAM ERROR:0x%x (DSERR_PRIOLEVELNEEDED) Unable to Unlock buffer. ", _dsound_hr);
            }
            exitm::Exit_With_Message(msg_buf);
        }

        _stream_data.status |= 1;
        _stream_data.last_lock_size = dwBytes;

        _stream_data.ds_buffer->SetVolume((long)_ds_volume_table[volume_idx]);
        _stream_data.ds_buffer->Play(0, 0, DSBPLAY_LOOPING);

        _soundFxActive = 1;
        Enable_Sound_Timer_();
    }

    uint8_t __cdecl Read_Stream_Data_(void* buffer, uint32_t bytes_to_read, uint32_t* bytes_read_ptr) {
        MMIOINFO mmio_info;
        MMRESULT res = mmioGetInfo((HMMIO)sound::_streamLbxHandle.file, &mmio_info, 0);

        uint32_t status = (res == 0) ? 0 : 1;
        uint32_t total_copied = 0;

        if (status == 0) {
            bool error_occurred = false;

            if (bytes_to_read != 0) {
                while (total_copied < bytes_to_read) {
                    if (mmio_info.pchNext == mmio_info.pchEndRead) {
                        status = mmioAdvance((HMMIO)sound::_streamLbxHandle.file, &mmio_info, 0);
                        if (status != 0) {
                            error_occurred = true;
                            break;
                        }
                        if (mmio_info.pchNext == mmio_info.pchEndRead) {
                            status = 4;
                            error_occurred = true;
                            break;
                        }
                    }

                    uint32_t available = (uint32_t)(mmio_info.pchEndRead - mmio_info.pchNext);
                    uint32_t remaining = bytes_to_read - total_copied;
                    uint32_t chunk_size = (available < remaining) ? available : remaining;

                    memcpy((uint8_t*)buffer + total_copied, mmio_info.pchNext, chunk_size);

                    mmio_info.pchNext += chunk_size;
                    total_copied += chunk_size;
                }
            }

            if (!error_occurred) {
                status = mmioSetInfo((HMMIO)sound::_streamLbxHandle.file, &mmio_info, 0);
                if (status == 0) {
                    *bytes_read_ptr = bytes_to_read;
                    return 0;
                }
            }
        }

        if (_streamReadErrorLogged == 0) {
            _streamReadErrorLogged = 1;
            Debug_Log_Stream_Message_(
                "stream: read failed file=%s entry=%d status=%u requested=%u copied=%u bytes_written=%u total_data=%u data_offset=%u loop=%u",
                _currentStreamFilename,
                _currentStreamId,
                status,
                bytes_to_read,
                total_copied,
                _stream_data.bytes_written,
                _stream_data.total_data_size,
                _stream_data.data_start_offset,
                _loopStream
            );
        }

        fonts::Disp_PSTR_(320 /* 0x140 */, 0, "Cant Read........");
        *bytes_read_ptr = 0;
        return (uint8_t)status;
    }

    /**
     * Stores the fade target volume index for a channel.
     */
    void __cdecl Fadeout_Sound_(int16_t channel_idx, int8_t volume_idx) {
        if (channel_idx >= 0 && channel_idx < 16) {
            _soundChannels[channel_idx].volume_index = volume_idx;
        }
    }

    void __cdecl Loop_Sound_(int32_t channel_idx, int8_t loop_count) {
        if (channel_idx >= 1) {
            _soundChannels[channel_idx].flags |= 0x08;
            _soundChannels[channel_idx].loop_count = loop_count;
        }
    }

    int32_t __cdecl Get_Channel_Volume_Level_(int32_t channel_idx) {
        LONG volume = 0;

        if (sound::_soundOn == 0) {
            return 0;
        }

        if (channel_idx >= 0 && channel_idx < 16) {
            _soundChannels[channel_idx].buffer->GetVolume(&volume);

            for (int i = 0; i < 101; ++i) {
                if (sound::_ds_volume_table[i] == volume) {
                    return i;
                }
            }
        }

        return (int32_t)volume;
    }

    void __cdecl Copy_FourCC_(char* dest, const char* src) {
        memcpy(dest, src, 4);
    }

    void __cdecl Refill_Sound_Channel_Buffer_(int32_t channel_idx) {
        DWORD play_cursor;
        DWORD write_cursor;
        DWORD status;
        DWORD pos1;
        DWORD pos2;

        s_sound_channel* channel = &_soundChannels[channel_idx];

        _dsound_hr = channel->buffer->GetCurrentPosition(&play_cursor, &write_cursor);
        if (_dsound_hr == 0) {
            channel->buffer->GetStatus(&status);

            if (_dsound_hr == 0) {
                uint32_t diff = 0;
                if (channel->write_cursor < (uint32_t)play_cursor) {
                    diff = (uint32_t)play_cursor - channel->write_cursor;
                } else if (play_cursor != 0 || channel->write_cursor != 0) {
                    diff = ((uint32_t)play_cursor - channel->write_cursor) + 0x4000;
                }

                if (diff == 0 || diff < 0x1555) {
                    return;
                }

                uint32_t lock_size = 0x1555;
                uint32_t remaining = channel->sample_size - channel->sample_offset;
                if (remaining <= 0x1555) {
                    lock_size = remaining;
                }

                _dsound_hr = channel->buffer->Lock(
                    channel->write_cursor,
                    lock_size,
                    &_streamBuffer1,
                    (LPDWORD)&_streamAvailable1,
                    &_streamBuffer2,
                    (LPDWORD)&_streamAvailable2,
                    0
                );

                if (_dsound_hr == 0) {
                    if (_streamAvailable1 != 0) {
                        memcpy(_streamBuffer1, &channel->sample_data[channel->sample_offset], _streamAvailable1);
                        channel->sample_offset += _streamAvailable1;
                    }

                    if (_streamAvailable2 != 0) {
                        memcpy(_streamBuffer2, &channel->sample_data[channel->sample_offset], _streamAvailable2);
                        channel->sample_offset += _streamAvailable2;
                    }

                    _dsound_hr = channel->buffer->Unlock(
                        _streamBuffer1,
                        _streamAvailable1,
                        _streamBuffer2,
                        _streamAvailable2
                    );

                    // Assembly calls GetCurrentPosition here but result is ignored
                    _dsound_hr = channel->buffer->GetCurrentPosition(&pos1, &pos2);

                    channel->ds_buffer_size = lock_size;

                    if (lock_size < 0x1555 && (channel->flags & 0x04)) {
                        channel->sample_offset = 0;
                        channel->loop_count--;
                        if (channel->loop_count == 0) {
                            channel->flags ^= 0x04;
                        }

                        _dsound_hr = channel->buffer->Lock(
                            channel->write_cursor,
                            0x1555 - lock_size,
                            &_streamBuffer1,
                            (LPDWORD)&_streamAvailable1,
                            &_streamBuffer2,
                            (LPDWORD)&_streamAvailable2,
                            0
                        );

                        if (_dsound_hr == 0) {
                            if (_streamAvailable1 != 0) {
                                memcpy(_streamBuffer1, &channel->sample_data[channel->sample_offset], _streamAvailable1);
                                channel->sample_offset += _streamAvailable1;
                            }

                            if (_streamAvailable2 != 0) {
                                memcpy(_streamBuffer2, &channel->sample_data[channel->sample_offset], _streamAvailable2);
                                channel->sample_offset += _streamAvailable2;
                            }

                            _dsound_hr = channel->buffer->Unlock(
                                _streamBuffer1,
                                _streamAvailable1,
                                _streamBuffer2,
                                _streamAvailable2
                            );
                        }
                    }
                    channel->write_cursor = (channel->write_cursor + 0x1555) & 0x3FFF;
                }
            }
        }

        if (channel->sample_size <= channel->sample_offset) {
            channel->flags ^= 0x02;
        }
    }

    void __cdecl Update_Sound_Channels_() {
        if (sound::_soundOn == 1) {
            for (int i = 0; i < 16; ++i) {
                if (sound::_soundChannels[i].flags & 1) {
                    if (sound::_soundChannels[i].flags & 2) {
                        sound::Refill_Sound_Channel_Buffer_(i);
                    } else {
                        sound::Update_Sound_Channel_Status_(i);
                    }
                }
            }
        }
    }

    /**
     * Sets the master playback volume for all active sound channels.
     */
    void __cdecl Set_Master_Volume_(int32_t volume) {
        if (sound::_soundOn != 0 && volume >= 0 && volume <= 100) {
            // Cap volume to master volume limit
            if (volume > sound::_dsMasterVolume) {
                volume = sound::_dsMasterVolume;
            }

            int16_t ds_volume = sound::_ds_volume_table[volume];

            for (int i = 0; i < 16; ++i) {
                LPDIRECTSOUNDBUFFER ds_buffer = sound::_soundChannels[i].buffer;
                ds_buffer->SetVolume((long)ds_volume);
            }
        }
    }

    /**
     * @brief Removes a sound instance ID from the active channel lists.
     *
     * Iterates through sound indices (skipping index 0) and searches for the given instance_id.
     * If found, the instance is removed by swapping it with the last active instance
     * in that sound's channel list.
     */
    void __cdecl Remove_Sound_Instance_(int32_t instance_id) {
        for (int i = 1; i < 33; ++i) {
            s_sound_index& sound_idx = _soundIndices[i];

            for (int j = 0; j < sound_idx.num_active_channels; ++j) {
                // Sign-extended comparison of the byte-sized instance ID
                if ((int8_t)sound_idx.active_channel_ids[j] == instance_id) {
                    if (sound_idx.num_active_channels > 1) {
                        sound_idx.num_active_channels--;
                        // Replace removed instance with the last one in the list
                        sound_idx.active_channel_ids[j] = sound_idx.active_channel_ids[sound_idx.num_active_channels];
                    }
                    else {
                        // If it was the last active channel, set count to -1
                        sound_idx.num_active_channels = -1;
                    }
                    return;
                }
            }
        }
    }

    void __cdecl Adjust_Settings_() {
        if (sound::_soundFxActive != 1 || !(sound::_stream_data.status & 0x01)) {
            return;
        }

        if (sound::_command == 1 /* Fade In */) {
            int32_t current_vol = sound::Get_Stream_Volume_();
            if (current_vol < sound::_global_stream_volume) {
                current_vol += (int32_t)(int8_t)sound::_streamIncVal;
                if (current_vol >= sound::_global_stream_volume) {
                    sound::_command = 0;
                    current_vol = sound::_global_stream_volume;
                }
                sound::Set_Stream_Volume_(current_vol);
            } else {
                sound::_command = 0;
            }
        }

        if (sound::_command == 2 /* Fade Out */) {
            int32_t current_vol = sound::Get_Stream_Volume_();
            if (current_vol > 0) {
                current_vol -= (int32_t)(int8_t)sound::_streamIncVal;
                if (current_vol <= 0) {
                    current_vol = 0;
                    sound::_command = 0;
                }
                sound::Set_Stream_Volume_(current_vol);
            } else {
                sound::_command = 0;
            }
        }

        if (sound::_stream_data.status & 0x02) {
            if (sound::_stream_data.ds_buffer == nullptr) {
                return;
            }

            while (true) {
                uint32_t play_pos, write_pos;
                sound::_dsound_hr = sound::_stream_data.ds_buffer->GetCurrentPosition((LPDWORD)&play_pos, (LPDWORD)&write_pos);
                if (sound::_dsound_hr == 0) {
                    DWORD status;
                    sound::_stream_data.ds_buffer->GetStatus(&status);
                    if (sound::_dsound_hr == 0) {
                        uint32_t space_available = 0;
                        if (sound::_stream_data.write_cursor < play_pos) {
                            space_available = play_pos - sound::_stream_data.write_cursor;
                        } else if (play_pos != 0 || sound::_stream_data.write_cursor != 0) {
                            space_available = (sound::_stream_data.ds_buffer_size - sound::_stream_data.write_cursor) + play_pos;
                        }

                        if (space_available == 0 || space_available < sound::_stream_data.block_size) {
                            return;
                        }

                        uint32_t bytes_to_lock = sound::_stream_data.data_remaining - sound::_stream_data.bytes_written;
                        if (bytes_to_lock > sound::_stream_data.block_size) {
                            bytes_to_lock = sound::_stream_data.block_size;
                        }

                        sound::_dsound_hr = sound::_stream_data.ds_buffer->Lock(
                            sound::_stream_data.write_cursor,
                            bytes_to_lock,
                            &sound::_streamBuffer1,
                            (LPDWORD)&sound::_streamAvailable1,
                            &sound::_streamBuffer2,
                            (LPDWORD)&sound::_streamAvailable2,
                            0
                        );

                        if (sound::_dsound_hr == 0) {
                            uint32_t bytes_read;
                            if (sound::_streamAvailable1 != 0) {
                                sound::Read_Stream_Data_(sound::_streamBuffer1, sound::_streamAvailable1, &bytes_read);
                                sound::_stream_data.bytes_written += bytes_read;
                            }
                            if (sound::_streamAvailable2 != 0) {
                                sound::Read_Stream_Data_(sound::_streamBuffer2, sound::_streamAvailable2, &bytes_read);
                                sound::_stream_data.bytes_written += bytes_read;
                            }

                            sound::_dsound_hr = sound::_stream_data.ds_buffer->Unlock(
                                sound::_streamBuffer1,
                                sound::_streamAvailable1,
                                sound::_streamBuffer2,
                                sound::_streamAvailable2
                            );

                            uint32_t unused_play, unused_write;
                            sound::_dsound_hr = sound::_stream_data.ds_buffer->GetCurrentPosition((LPDWORD)&unused_play, (LPDWORD)&unused_write);

                            sound::_stream_data.last_lock_size = bytes_to_lock;

                            if (bytes_to_lock < sound::_stream_data.block_size && sound::_loopStream) {
                                uint32_t loop_bytes = sound::_stream_data.block_size - bytes_to_lock;
                                sound::_stream_data.bytes_written = 0;
                                mmioSeek((HMMIO)sound::_streamLbxHandle.file, sound::_stream_data.data_start_offset, SEEK_SET);

                                sound::_doubleBufferLoops--;
                                if (sound::_doubleBufferLoops == 0) {
                                    sound::_loopStream = 0;
                                }

                                uint32_t loop_lock_offset = (sound::_stream_data.write_cursor - loop_bytes) + sound::_stream_data.block_size;
                                sound::_dsound_hr = sound::_stream_data.ds_buffer->Lock(
                                    loop_lock_offset,
                                    loop_bytes,
                                    &sound::_streamBuffer1,
                                    (LPDWORD)&sound::_streamAvailable1,
                                    &sound::_streamBuffer2,
                                    (LPDWORD)&sound::_streamAvailable2,
                                    0
                                );

                                if (sound::_dsound_hr == 0) {
                                    if (sound::_streamAvailable1 != 0) {
                                        sound::Read_Stream_Data_(sound::_streamBuffer1, sound::_streamAvailable1, &bytes_read);
                                        sound::_stream_data.bytes_written += bytes_read;
                                    }
                                    if (sound::_streamAvailable2 != 0) {
                                        sound::Read_Stream_Data_(sound::_streamBuffer2, sound::_streamAvailable2, &bytes_read);
                                        sound::_stream_data.bytes_written += bytes_read;
                                    }
                                    sound::_dsound_hr = sound::_stream_data.ds_buffer->Unlock(
                                        sound::_streamBuffer1,
                                        sound::_streamAvailable1,
                                        sound::_streamBuffer2,
                                        sound::_streamAvailable2
                                    );
                                }
                            }

                            sound::_stream_data.write_cursor += sound::_stream_data.block_size;
                            sound::_stream_data.write_cursor %= sound::_stream_data.ds_buffer_size;
                        }
                    }
                }

                if (sound::_stream_data.bytes_written < sound::_stream_data.data_remaining) {
                    return;
                }

                if (!sound::_loopStream) {
                    break;
                }

                sound::_stream_data.bytes_written = 0;
                mmioSeek((HMMIO)sound::_streamLbxHandle.file, sound::_stream_data.data_start_offset, SEEK_SET);
                sound::_doubleBufferLoops--;
                if (sound::_doubleBufferLoops == 0) {
                    sound::_loopStream = 0;
                }

                if (sound::_stream_data.ds_buffer == nullptr) {
                    return;
                }
            }

            sound::_stream_data.status ^= 0x02;
        } else if (!(sound::_stream_data.status & 0x08)) {
            sound::Update_Stream_Status_();
        }
    }

    int32_t __cdecl Play_Sound_File_(int32_t channel_idx, void* wave_data, uint32_t wave_data_size) {
        s_sound_wav_header info;
        WAVEFORMATEX wfx;
        char error_msg[256];
        uint32_t dwBytes;
        Stop_Sound_Timer_And_Wait2_();

        (void)wave_data_size;

        if (!Init_Wave_Header_From_Memory_(wave_data, &info)) {
            exitm::Exit_With_Message("SOUND ERROR: Reading Wave");
        }

        _soundChannels[channel_idx].buffer->Stop();
        Reset_Sound_Channel_(channel_idx);
        _dsound_hr = _soundChannels[channel_idx].buffer->Release();

        memset(&_dsound_buffer_desc, 0, sizeof(DSBUFFERDESC1));
        _dsound_buffer_desc.dwSize = sizeof(_DSBUFFERDESC1);
        _dsound_buffer_desc.dwFlags = 0xe0;

        uint32_t desc_bytes = 0x4000;
        if ((int32_t)info.data_size <= 0x4000) {
            desc_bytes = info.data_size;
        }
        _dsound_buffer_desc.dwBufferBytes = desc_bytes;

        memset(&wfx, 0, sizeof(WAVEFORMATEX));
        wfx.wFormatTag = 1;
        wfx.nChannels = info.num_channels;
        wfx.nSamplesPerSec = info.sample_rate;
        wfx.nAvgBytesPerSec = info.byte_rate;
        wfx.nBlockAlign = info.block_align;
        wfx.wBitsPerSample = info.bits_per_sample;
        wfx.cbSize = 0;

        _dsound_buffer_desc.lpwfxFormat = &wfx;

        _dsound_hr = _dsound->CreateSoundBuffer((LPCDSBUFFERDESC)&_dsound_buffer_desc, &_soundChannels[channel_idx].buffer, NULL);

        _soundChannels[channel_idx].sample_size = info.data_size;
        _soundChannels[channel_idx].sample_data = (uint8_t*)info.data_ptr;
        _soundChannels[channel_idx].volume_index = 0;

        dwBytes = _soundChannels[channel_idx].sample_size;
        if (dwBytes > 0x4000) {
            _soundChannels[channel_idx].flags |= 0x2;
            dwBytes = 0x4000;
        }

        _dsound_hr = _soundChannels[channel_idx].buffer->Lock(0, dwBytes, &_streamBuffer1, (LPDWORD)&_streamAvailable1, &_streamBuffer2, (LPDWORD)&_streamAvailable2, 0);

        if (_dsound_hr != 0) {
            strcpy(error_msg, "SOUND ERROR: (Play_Sound_File) Unable to lock buffer.");
            exitm::Exit_With_Message(error_msg);
        }

        if (_streamAvailable1 != 0) {
            memcpy(_streamBuffer1, _soundChannels[channel_idx].sample_data + _soundChannels[channel_idx].sample_offset, _streamAvailable1);
            _soundChannels[channel_idx].sample_offset += _streamAvailable1;
        }

        if (_streamAvailable2 != 0) {
            memcpy(_streamBuffer2, _soundChannels[channel_idx].sample_data + _soundChannels[channel_idx].sample_offset, _streamAvailable2);
            _soundChannels[channel_idx].sample_offset += _streamAvailable2;
        }

        _dsound_hr = _soundChannels[channel_idx].buffer->Unlock(_streamBuffer1, _streamAvailable1, _streamBuffer2, _streamAvailable2);

        if (_dsound_hr != 0) {
            const char* fmt = nullptr;
            if (_dsound_hr == 0x80070057 /* DSERR_INVALIDPARAM */) {
                fmt = "SOUND ERROR:0x%x (DSERR_INVALIDPARAM) Unable to Unlock buffer. ";
            } else if (_dsound_hr == 0x88780032 /* DSERR_INVALIDCALL */) {
                fmt = "SOUND ERROR:0x%x (DSERR_INVALIDCALL) Unable to Unlock buffer. ";
            } else if (_dsound_hr == 0x88780046 /* DSERR_PRIOLEVELNEEDED */) {
                fmt = "SOUND ERROR:0x%x (DSERR_PRIOLEVELNEEDED) Unable to Unlock buffer. ";
            }

            if (fmt) {
                sprintf(error_msg, fmt, _dsound_hr);
            }
            exitm::Exit_With_Message(error_msg);
        }

        _soundChannels[channel_idx].ds_buffer_size = dwBytes;
        _soundChannels[channel_idx].flags |= 0x1;

        int32_t master_vol = _dsMasterVolume;
        _soundChannels[channel_idx].buffer->SetVolume((LONG)_ds_volume_table[master_vol]);
        _soundChannels[channel_idx].buffer->Play(0, 0, (dwBytes >= 0x4000) ? 1 /* DSBPLAY_LOOPING */ : 0);

        Resume_Timer2_();

        return channel_idx;
    }

    /**
     * @brief Searches for an available sound channel.
     * 
     * A channel is considered free if its DirectSound buffer is null, 
     * or if the buffer is not currently playing and the internal flags 
     * indicate the channel is not busy.
     * 
     * @return The index of the free channel (0-15), or -2 if no channel is available.
     * @address 0052ddc0
     */
    int8_t __cdecl Find_Free_Channel_() {
        for (int8_t i = 0; i < 16; ++i) {
            s_sound_channel& channel = _soundChannels[i];

            if (channel.buffer == nullptr) {
                return i;
            }

            uint32_t status = 0;
            channel.buffer->GetStatus((LPDWORD)&status);

            // Check if buffer is not playing (DSBSTATUS_PLAYING = 0x1) 
            // and if internal channel flag (busy/active) is clear.
            if (!(status & 1 /* DSBSTATUS_PLAYING */)) {
                if (!(channel.flags & 1)) {
                    return i;
                }
            }
        }

        return -2;
    }

    /**
     * Attempts to play a sound resource by ID. Checks if the resource is already
     * cached in _soundIndices; otherwise, loads it from the LBX file.
     * @address 0052de10
     */
    int32_t __cdecl Play_Sound_(int32_t resource_id) {
        int32_t channel_id = (int8_t)Find_Free_Channel_();
        if (channel_id == -2) {
            return -2;
        }

        if (resource_id == 0 || _soundOn == 0) {
            return -2;
        }

        // Search cache for existing sound data
        for (int i = 0; i < 33; ++i) {
            s_sound_index& index = _soundIndices[i];
            if (index.resource_id == resource_id && index.num_active_channels != 0) {
                if (index.num_active_channels == -1) {
                    index.num_active_channels = 1;
                    index.active_channel_ids[0] = (uint8_t)channel_id;
                } else if (index.num_active_channels > 0) {
                    index.active_channel_ids[index.num_active_channels] = (uint8_t)channel_id;
                    index.num_active_channels++;
                } else {
                    continue;
                }

                void* buffer = (uint8_t*)_soundBuffer + index.buffer_offset;
                return Play_Sound_File_(channel_id, buffer, index.buffer_size);
            }
        }

        // Cache miss: Load from LBX
        uint32_t length = (uint32_t)lbx::LBX_getRecordLength_(&_soundLbxHandle, resource_id);
        if (length == 0) {
            return -2;
        }

        void* buffer = Allocate_Sound_Block_(channel_id, resource_id, length);
        if (buffer == nullptr) {
            return -2;
        }

        lbx::LBX_readRecord_(&_soundLbxHandle, resource_id, buffer);
        return Play_Sound_File_(channel_id, buffer, length);
    }

    /**
     * @brief Checks the status of a sound channel and performs updates based on its current position.
     * Handles both looping and one-shot sounds, as well as cleanup when playback is finished.
     */
    void __cdecl Update_Sound_Channel_Status_(int32_t channel_idx) {
        DWORD play_cursor;
        DWORD ds_write_cursor;
        DWORD status;
        uint32_t distance;

        s_sound_channel& channel = _soundChannels[channel_idx];

        channel.buffer->GetCurrentPosition(&play_cursor, &ds_write_cursor);

        // Calculate circular distance between write cursor and current play cursor
        if (play_cursor > channel.write_cursor) {
            distance = play_cursor - channel.write_cursor;
        } else {
            distance = (play_cursor - channel.write_cursor) + 0x4000;
        }

        if (channel.sample_size > 0x4000) {
            // Streaming/Large sample check
            if (channel.ds_buffer_size <= distance) {
                channel.flags ^= 0x1; // Deactivate active flag
                Remove_Sound_Instance_(channel_idx);
                channel.buffer->Stop();
                return;
            }
        } else {
            // Small sample check - relies on GetStatus
            channel.buffer->GetStatus(&status);

            if (!(status & 0x1 /* DSBSTATUS_PLAYING */)) {
                if (channel.flags & 0x4) { // Looping flag
                    channel.loop_count--;
                    if (channel.loop_count == 0) {
                        channel.flags ^= 0x4; // Clear looping flag
                    }
                    // Restart playback for the next loop
                    channel.buffer->Play(0, 0, 0x1 /* DSBPLAY_LOOPING */);
                } else {
                    channel.flags ^= 0x1; // Deactivate active flag
                    Remove_Sound_Instance_(channel_idx);
                }
            }
        }
    }

    /**
     * Finds an unused sound-cache index entry.
     */
    int32_t __cdecl Get_Free_Sound_Index_() {
        for (int32_t i = 1; i < 33; ++i) {
            if (_soundIndices[i].next_id == -1 && _soundIndices[i].prev_id == -1) {
                return i;
            }
        }
        return -1;
    }

    /**
     * Allocates a cached sound-data block from the shared sound buffer.
     */
    void* __cdecl Allocate_Sound_Block_(int8_t channel_idx, int32_t resource_id, int32_t size) {
        if (size > (int32_t)_soundBufferSize) {
            return nullptr;
        }

        Merge_Free_Blocks_();

        for (int32_t attempt = 0; attempt < 3; ++attempt) {
            int32_t current_idx = 0;
            s_sound_index* current = &_soundIndices[0];

            while (true) {
                bool can_allocate;
                if (attempt == 0) {
                    can_allocate = (current->num_active_channels == 0);
                } else {
                    can_allocate = (current->num_active_channels < 1);
                }

                if (can_allocate && (int32_t)current->buffer_size >= size) {
                    int32_t free_idx = Get_Free_Sound_Index_();
                    if (free_idx >= 0) {
                        s_sound_index* new_block = &_soundIndices[free_idx];

                        new_block->prev_id = current_idx;
                        new_block->next_id = current->next_id;
                        new_block->buffer_offset = current->buffer_offset + size;
                        new_block->buffer_size = current->buffer_size - size;
                        new_block->num_active_channels = 0;
                        new_block->active_channel_ids[0] = 0;
                        new_block->resource_id = 0;

                        current->next_id = free_idx;
                        current->active_channel_ids[0] = (uint8_t)channel_idx;
                        current->resource_id = resource_id;
                        current->buffer_size = size;
                        current->num_active_channels = 1;

                        return (uint8_t*)_soundBuffer + current->buffer_offset;
                    }
                    return nullptr;
                }

                current_idx = current->next_id;
                if (current_idx < 0) {
                    break;
                }
                current = &_soundIndices[current_idx];
            }

            if (attempt == 1) {
                Merge_Free_Sound_Blocks_();
            }
        }

        return nullptr;
    }

    void __cdecl Merge_Free_Sound_Blocks_() {
        int32_t current_idx = 1;

        while (current_idx != -1) {
            // If this is the last block in the linked list, we are done.
            if (_soundIndices[current_idx].next_id == -1) {
                return;
            }

            int32_t next_idx = _soundIndices[current_idx].next_id;

            // If both the current block and the next block are inactive (num_active_channels < 1), merge them.
            if (_soundIndices[current_idx].num_active_channels < 1 &&
                _soundIndices[next_idx].num_active_channels < 1) {
                
                // Link current block to the block after the next one.
                _soundIndices[current_idx].next_id = _soundIndices[next_idx].next_id;

                // Add next block's buffer size to the current block.
                _soundIndices[current_idx].buffer_size += _soundIndices[next_idx].buffer_size;
                
                // Explicitly reset status fields of the newly merged block.
                _soundIndices[current_idx].num_active_channels = 0;
                _soundIndices[current_idx].active_channel_ids[0] = 0;
                _soundIndices[current_idx].resource_id = 0;

                // Completely clear the now-merged-away next block entry.
                memset(&_soundIndices[next_idx], 0, sizeof(s_sound_index));
                _soundIndices[next_idx].next_id = -1;
                _soundIndices[next_idx].prev_id = -1;
            }

            // Move to the next block in the list.
            current_idx = _soundIndices[current_idx].next_id;
        }
    }

    /**
     * @brief Iterates through the linked list of sound blocks and merges adjacent free blocks.
     * 
     * A block is considered free if its num_active_channels is 0.
     */
    void __cdecl Merge_Free_Blocks_() {
        int32_t current_idx = 1;

        while (true) {
            // Check if there is a next block to potentially merge with
            if (_soundIndices[current_idx].next_id == -1) {
                break;
            }

            int32_t next_idx = _soundIndices[current_idx].next_id;

            // If both current block and next block are inactive, merge them
            if (_soundIndices[current_idx].num_active_channels == 0 &&
                _soundIndices[next_idx].num_active_channels == 0) {

                // Point current block to the block after next_idx
                _soundIndices[current_idx].next_id = _soundIndices[next_idx].next_id;
                
                // Add the size of the merged block to current
                _soundIndices[current_idx].buffer_size += _soundIndices[next_idx].buffer_size;

                // Reset status fields for the merged current block
                _soundIndices[current_idx].num_active_channels = 0;
                _soundIndices[current_idx].active_channel_ids[0] = 0;
                _soundIndices[current_idx].resource_id = 0;

                // Clear the vacated sound index record
                memset(&_soundIndices[next_idx], 0, sizeof(s_sound_index));
                _soundIndices[next_idx].next_id = -1;
                _soundIndices[next_idx].prev_id = -1;
            }

            // Move to the next block in the linked list
            current_idx = _soundIndices[current_idx].next_id;

            // If we've reached the end of the list, exit
            if (current_idx == -1) {
                break;
            }
        }
    }

    void __cdecl Update_Stream_Status_() {
        if (_stream_data.ds_buffer == nullptr) {
            return;
        }

        uint32_t play_cursor;
        uint32_t write_cursor_ignored;

        _stream_data.ds_buffer->GetCurrentPosition(
            (LPDWORD)&play_cursor,
            (LPDWORD)&write_cursor_ignored
        );

        uint32_t diff;
        if (play_cursor > _stream_data.write_cursor) {
            diff = play_cursor - _stream_data.write_cursor;
        } else {
            diff = _stream_data.ds_buffer_size - _stream_data.write_cursor + play_cursor;
        }

        if (diff >= _stream_data.last_lock_size) {
            _stream_data.status ^= 1;
            _stream_data.ds_buffer->Stop();
            _soundFxActive = 0;
        }
    }

    void __cdecl Stop_Stream_Loop_() {
        if (_stream_data.ds_buffer != nullptr) {
            _stream_data.ds_buffer->Stop();
        }

        _loopStream = 0;
        _stream_data.data_remaining = 0;
        _stream_data.bytes_written = 0;
        _stream_data.last_lock_size = 0;
        _stream_data.field20_0x3f = 0;
        _stream_data.write_cursor = 0;
        _stream_data.status = 0;
    }

    uint8_t __cdecl Is_Stream_Playing_() {
        return _soundFxActive;
    }
}