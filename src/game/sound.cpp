#include "pch.h"

namespace sound {
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

    int16_t _volume_table[101] = {
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
    SDL_TimerID _soundTimerId;
    uint8_t _soundTimerActive;
    volatile uint32_t _soundTimerRunning = 1;
    volatile uint32_t _soundTimerStopFlag;
    void* _soundBuffer;
    uint32_t _soundBufferSize;
    volatile uint32_t _soundTimerLock;
    void* _streamBuffer1;
    void* _streamBuffer2;
    uint8_t _audioPlaybackActive;
    int32_t _global_stream_volume = 100;
    uint8_t _loopStream;
    int32_t _doubleBufferLoops;
    uint8_t _command;
    uint8_t _streamIncVal;
    uint32_t _streamAvailable2;
    s_lbx_stream_header _musicLbxHandle;
    uint32_t _streamAvailable1;
    int32_t _masterVolume;
    s_sound_index _soundIndices[33];
    s_sound_channel _soundChannels[16];
    s_lbx_stream_header _soundLbxHandle;
    s_wave_stream_header _streamLbxHandle;
    uint8_t _close_stream = 1;
    uint8_t _musicActive;
    s_sound_stream _stream_data;
    char _currentStreamFilename[k_stream_filename_capacity] = "stream.lbx";
    int32_t _currentStreamId;
    int32_t _currentStreamVolume = 100;
    uint8_t _streamReadErrorLogged;

    namespace {
        constexpr Uint32 k_sound_timer_interval_ms = 160;
        constexpr int32_t k_stream_prefill_blocks = 20;

        SDL_AudioStream* As_SDL_Audio_Stream_(void* stream) {
            return (SDL_AudioStream*)stream;
        }

        SDL_AudioFormat Get_SDL_Audio_Format_(uint16_t format_tag, uint16_t bits_per_sample) {
            if (format_tag != 1) {
                return SDL_AUDIO_UNKNOWN;
            }

            if (bits_per_sample == 8) {
                return SDL_AUDIO_U8;
            }

            if (bits_per_sample == 16) {
                return SDL_AUDIO_S16LE;
            }

            return SDL_AUDIO_UNKNOWN;
        }

        bool Channel_Loop_Enabled_(const s_sound_channel& channel) {
            return (channel.flags & 0x0C) != 0;
        }

        void Destroy_SDL_Stream_(void*& stream) {
            if (stream != nullptr) {
                SDL_DestroyAudioStream(As_SDL_Audio_Stream_(stream));
                stream = nullptr;
            }
        }

        bool Set_SDL_Stream_Gain_(void* stream, int32_t volume_percent) {
            if (stream == nullptr) {
                return false;
            }

            return SDL_SetAudioStreamGain(As_SDL_Audio_Stream_(stream), (float)volume_percent / 100.0f);
        }

        bool Queue_Channel_Data_(s_sound_channel& channel, uint32_t bytes_to_queue) {
            if (channel.buffer == nullptr || bytes_to_queue == 0) {
                return false;
            }

            if (!SDL_PutAudioStreamData(As_SDL_Audio_Stream_(channel.buffer), &channel.sample_data[channel.sample_offset], (int)bytes_to_queue)) {
                return false;
            }

            channel.sample_offset += bytes_to_queue;
            channel.ds_buffer_size = bytes_to_queue;
            return true;
        }

        bool Restart_Channel_Loop_(s_sound_channel& channel) {
            if (!Channel_Loop_Enabled_(channel)) {
                return false;
            }

            channel.sample_offset = 0;
            if (channel.loop_count > 0) {
                channel.loop_count--;
                if (channel.loop_count == 0) {
                    channel.flags &= ~0x0C;
                }
            }

            return true;
        }

        bool Create_SDL_Channel_Stream_(s_sound_channel& channel, const s_sound_wav_header& info) {
            SDL_AudioSpec spec = {};
            spec.format = Get_SDL_Audio_Format_(info.audio_format, info.bits_per_sample);
            spec.channels = info.num_channels;
            spec.freq = (int)info.sample_rate;

            if (spec.format == SDL_AUDIO_UNKNOWN) {
                return false;
            }

            channel.buffer = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
            if (channel.buffer == nullptr) {
                return false;
            }

            channel.sample_rate = info.sample_rate;
            channel.current_volume = _masterVolume;
            Set_SDL_Stream_Gain_(channel.buffer, channel.current_volume);
            return true;
        }

        bool Rewind_Stream_Source_() {
            clearerr(_streamLbxHandle.file);
            return fseek(_streamLbxHandle.file, _stream_data.data_start_offset, SEEK_SET) == 0;
        }

        bool Queue_Stream_Block_(uint32_t bytes_to_queue) {
            if (_stream_data.ds_buffer == nullptr || bytes_to_queue == 0) {
                return false;
            }

            uint8_t* block = (uint8_t*)malloc(bytes_to_queue);
            if (block == nullptr) {
                return false;
            }

            uint32_t bytes_read = 0;
            uint8_t read_status = Read_Stream_Data_(block, bytes_to_queue, &bytes_read);
            bool queued = false;

            if (bytes_read != 0) {
                queued = SDL_PutAudioStreamData(As_SDL_Audio_Stream_(_stream_data.ds_buffer), block, (int)bytes_read);
                if (queued) {
                    _stream_data.bytes_written += bytes_read;
                }
            }

            free(block);

            if (!queued) {
                return false;
            }

            return (read_status == 0 || read_status == 4);
        }

        bool Queue_Stream_Until_Target_(uint32_t target_queued_bytes) {
            if (_stream_data.ds_buffer == nullptr) {
                return false;
            }

            while (SDL_GetAudioStreamQueued(As_SDL_Audio_Stream_(_stream_data.ds_buffer)) < (int)target_queued_bytes) {
                uint32_t bytes_remaining = _stream_data.data_remaining - _stream_data.bytes_written;

                if (bytes_remaining == 0) {
                    if (_loopStream == 0) {
                        _stream_data.status &= ~0x02;
                        SDL_FlushAudioStream(As_SDL_Audio_Stream_(_stream_data.ds_buffer));
                        return true;
                    }

                    _stream_data.bytes_written = 0;
                    if (!Rewind_Stream_Source_()) {
                        return false;
                    }

                    _doubleBufferLoops--;
                    if (_doubleBufferLoops == 0) {
                        _loopStream = 0;
                    }

                    bytes_remaining = _stream_data.data_remaining;
                    if (bytes_remaining == 0) {
                        _stream_data.status &= ~0x02;
                        SDL_FlushAudioStream(As_SDL_Audio_Stream_(_stream_data.ds_buffer));
                        return true;
                    }
                }

                uint32_t bytes_to_queue = bytes_remaining;
                if (bytes_to_queue > _stream_data.block_size) {
                    bytes_to_queue = _stream_data.block_size;
                }

                if (!Queue_Stream_Block_(bytes_to_queue)) {
                    return false;
                }
            }

            return true;
        }

        Uint32 SDLCALL Sound_Timer_Thunk_(void* userdata, SDL_TimerID timer_id, Uint32 interval) {
            (void)userdata;
            sound::Sound_Timer_Callback_(timer_id, 0, 0, 0, 0);
            return interval;
        }

        void Debug_Log_Stream_Message_(const char* format, ...) {
            char message[512];
            va_list args;
            va_start(args, format);
            vsnprintf(message, sizeof(message), format, args);
            va_end(args);

            SDL_Log("%s", message);
        }
    }

    /**
     * @brief Iterates through active sound channels and updates their SDL stream gain.
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
                    channel.current_volume = diff;
                    Set_SDL_Stream_Gain_(channel.buffer, channel.current_volume);
                } else {
                    // Out of range: stop and cleanup
                    channel.flags ^= 1;
                    Remove_Sound_Instance_(i);
                    Destroy_SDL_Stream_(channel.buffer);
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
        _soundChannels[channel_idx].sample_rate = 0;
        _soundChannels[channel_idx].sample_offset = 0;
        _soundChannels[channel_idx].ds_buffer_size = 0;
        _soundChannels[channel_idx].field7_0x13 = 0;
        _soundChannels[channel_idx].current_volume = 0;
        _soundChannels[channel_idx].write_cursor = 0;
        _soundChannels[channel_idx].flags = 0;
        _soundChannels[channel_idx].volume_index = 0;
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
            sound::_masterVolume = volume;
            sound::Set_Channel_Volume_(-1, volume);
        }
    }

    void __cdecl Set_Stream_Filename_(const char* filename) {
        if (sound::_audioInitialized != 0) {
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
     * Releases SDL audio stream resources and stops the periodic sound timer.
     */
    void __cdecl Destroy_Sound_() {
        Stop_Sound_Timer_And_Wait2_();
        if (_soundTimerId != 0) {
            SDL_RemoveTimer(_soundTimerId);
            _soundTimerId = 0;
        }

        for (int i = 0; i < 16; ++i) {
            Destroy_SDL_Stream_(_soundChannels[i].buffer);
        }

        Destroy_SDL_Stream_(_stream_data.ds_buffer);

        _audioInitialized = 0;
    }

    void __cdecl Init_Sound_() {
        if ((SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO) == 0) {
            if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
                _soundErrorOccurred = 1;
                return;
            }
        }

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

        for (int i = 0; i < 16; ++i) {
            _soundChannels[i].buffer = nullptr;
            _soundChannels[i].sample_data = nullptr;
            _soundChannels[i].sample_size = 0;
            _soundChannels[i].sample_rate = 0;
            _soundChannels[i].sample_offset = 0;
            _soundChannels[i].ds_buffer_size = 0;
            _soundChannels[i].field7_0x13 = 0;
            _soundChannels[i].current_volume = 0;
            _soundChannels[i].write_cursor = 0;
            _soundChannels[i].flags = 0;
            _soundChannels[i].volume_index = 0;
        }

        _soundIndices[1].buffer_size = _soundBufferSize;
        _soundIndices[1].prev_id = 0;
        _soundIndices[0].next_id = 1;
        _soundIndices[0].resource_id = -1;
        _soundIndices[0].num_active_channels = 1000000 /* 0xf4240 */;

        _soundTimerId = SDL_AddTimer(k_sound_timer_interval_ms, Sound_Timer_Thunk_, nullptr);
        if (_soundTimerId != 0) {
            _soundTimerActive = 1;
            _masterVolume = 100;
            _soundOn = 1;
            _audioPlaybackActive = 1;
            _audioInitialized = 1;
            _musicActive = 1;
            _soundErrorOccurred = 0;
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

        if (sound::_audioPlaybackActive == 1 && sound::_musicActive == 1) {
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
        if (sound::_audioPlaybackActive != 0 && _stream_data.ds_buffer != nullptr) {
            // Validate that the volume percentage is within the expected range
            if (volume_percent >= 0 && volume_percent <= 100) {
                // Cap the requested volume to the global stream volume limit
                if (volume_percent > sound::_global_stream_volume) {
                    volume_percent = sound::_global_stream_volume;
                }

                _currentStreamVolume = volume_percent;
                SDL_SetAudioStreamGain(As_SDL_Audio_Stream_(_stream_data.ds_buffer), (float)volume_percent / 100.0f);
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
            if (sound::_masterVolume < volume) {
                volume = sound::_masterVolume;
            }

            if (channel_idx >= 0 && channel_idx < 16) {
                _soundChannels[channel_idx].current_volume = volume;
                Set_SDL_Stream_Gain_(_soundChannels[channel_idx].buffer, volume);
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
                channel->flags ^= 1;
                sound::Remove_Sound_Instance_(channel_idx);
                Destroy_SDL_Stream_(channel->buffer);
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
        if (sound::_audioPlaybackActive != 0 && _stream_data.ds_buffer != nullptr) {
            return _currentStreamVolume;
        }

        return 0;
    }

    void __cdecl Stop_Stream_() {
        if (_audioPlaybackActive != 0 && _stream_data.ds_buffer != nullptr) {
            Stop_Sound_Timer_();

            SDL_DestroyAudioStream(As_SDL_Audio_Stream_(_stream_data.ds_buffer));

            _stream_data.ds_buffer = nullptr;
            _loopStream = 0;
            _audioPlaybackActive = 0;

            Stop_Stream_Loop_();
            Enable_Sound_Timer_();
        }
    }

    void __cdecl Pitch_Bend_Sound_(int32_t handle, int32_t pitch) {
        if (handle >= 1 && handle < 16) {
            s_sound_channel& channel = _soundChannels[handle];
            if (channel.buffer == nullptr || channel.sample_rate == 0) {
                return;
            }

            SDL_AudioStream* stream = As_SDL_Audio_Stream_(channel.buffer);
            float ratio = SDL_GetAudioStreamFrequencyRatio(stream);
            if (ratio <= 0.0f) {
                ratio = 1.0f;
            }

            float frequency = (float)channel.sample_rate * ratio;
            frequency += (float)pitch;
            if (frequency < 1.0f) {
                frequency = 1.0f;
            }

            SDL_SetAudioStreamFrequencyRatio(stream, frequency / (float)channel.sample_rate);
        }
    }

    int8_t __cdecl Load_Stream_Wave_Header_(int32_t entry_index, s_sound_stream* stream) {
        if (entry_index == 0) {
            Debug_Log_Stream_Message_("stream: rejected empty entry file=%s entry=%d", _currentStreamFilename, entry_index);
            return 0;
        }

        uint32_t entry_offset = _streamLbxHandle.entries[entry_index - 1].offset;

        fseek(_streamLbxHandle.file, entry_offset, SEEK_SET);

        char tag_buffer[5];
        tag_buffer[4] = '\0';

        fread(stream->riff_tag, 0x30, 1, _streamLbxHandle.file);
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

        fseek(_streamLbxHandle.file, stream->data_start_offset, SEEK_SET);

        return 1;
    }

    void __cdecl Play_Stream_(int32_t stream_id) {
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
            SDL_DestroyAudioStream(As_SDL_Audio_Stream_(_stream_data.ds_buffer));
            _stream_data.ds_buffer = nullptr;
        }

        if (Load_Stream_Wave_Header_(stream_id, &_stream_data) == 0) {
            Enable_Sound_Timer_();
            return;
        }

        _stream_data.block_size = ((_stream_data.avg_bytes_per_sec * 320) / 100) / 20;
        _stream_data.data_remaining = _stream_data.total_data_size;
        _stream_data.ds_buffer_size = _stream_data.block_size * 20;
        _stream_data.bytes_written = 0;
        _stream_data.last_lock_size = _stream_data.block_size;
        _stream_data.write_cursor = 0;
        _stream_data.status = 0;

        SDL_AudioSpec stream_spec = {};
        stream_spec.format = Get_SDL_Audio_Format_(_stream_data.format_tag, _stream_data.bits_per_sample);
        stream_spec.channels = _stream_data.channels;
        stream_spec.freq = (int)_stream_data.samples_per_sec;

        if (stream_spec.format == SDL_AUDIO_UNKNOWN) {
            Debug_Log_Stream_Message_("stream: unsupported audio format file=%s entry=%d format=%u bits=%u", _currentStreamFilename, _currentStreamId, _stream_data.format_tag, _stream_data.bits_per_sample);
            Enable_Sound_Timer_();
            return;
        }

        _stream_data.ds_buffer = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &stream_spec, nullptr, nullptr);
        if (_stream_data.ds_buffer == nullptr) {
            Debug_Log_Stream_Message_("stream: SDL_OpenAudioDeviceStream failed file=%s entry=%d error=%s", _currentStreamFilename, _currentStreamId, SDL_GetError());
            Enable_Sound_Timer_();
            return;
        }

        _stream_data.status |= 0x01;
        if (_stream_data.data_remaining > 0) {
            _stream_data.status |= 0x02;
        }

        if (!Queue_Stream_Until_Target_(_stream_data.block_size * k_stream_prefill_blocks)) {
            SDL_DestroyAudioStream(As_SDL_Audio_Stream_(_stream_data.ds_buffer));
            _stream_data.ds_buffer = nullptr;
            Enable_Sound_Timer_();
            return;
        }

        _currentStreamVolume = volume_idx;
        SDL_SetAudioStreamGain(As_SDL_Audio_Stream_(_stream_data.ds_buffer), (float)volume_idx / 100.0f);
        SDL_ResumeAudioStreamDevice(As_SDL_Audio_Stream_(_stream_data.ds_buffer));

        _audioPlaybackActive = 1;
        Enable_Sound_Timer_();
    }

    uint8_t __cdecl Read_Stream_Data_(void* buffer, uint32_t bytes_to_read, uint32_t* bytes_read_ptr) {
        size_t bytes_read = fread(buffer, 1, bytes_to_read, sound::_streamLbxHandle.file);
        if (bytes_read == bytes_to_read) {
            *bytes_read_ptr = (uint32_t)bytes_read;
            return 0;
        }

        uint32_t status = ferror(sound::_streamLbxHandle.file) ? 1u : 4u;

        if (_streamReadErrorLogged == 0) {
            _streamReadErrorLogged = 1;
            Debug_Log_Stream_Message_(
                "stream: read failed file=%s entry=%d status=%u requested=%u copied=%u bytes_written=%u total_data=%u data_offset=%u loop=%u",
                _currentStreamFilename,
                _currentStreamId,
                status,
                bytes_to_read,
                (uint32_t)bytes_read,
                _stream_data.bytes_written,
                _stream_data.total_data_size,
                _stream_data.data_start_offset,
                _loopStream
            );
        }

        fonts::Disp_PSTR_(320 /* 0x140 */, 0, "Cant Read........");
        *bytes_read_ptr = (uint32_t)bytes_read;
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
        if (sound::_soundOn == 0) {
            return 0;
        }

        if (channel_idx >= 0 && channel_idx < 16) {
            return _soundChannels[channel_idx].current_volume;
        }

        return 0;
    }

    void __cdecl Copy_FourCC_(char* dest, const char* src) {
        memcpy(dest, src, 4);
    }

    void __cdecl Refill_Sound_Channel_Buffer_(int32_t channel_idx) {
        s_sound_channel* channel = &_soundChannels[channel_idx];

        if (channel->buffer == nullptr) {
            return;
        }

        int queued = SDL_GetAudioStreamQueued(As_SDL_Audio_Stream_(channel->buffer));
        if (queued < 0 || queued >= 0x1555) {
            return;
        }

        while (queued >= 0 && queued < 0x1555) {
            uint32_t remaining = channel->sample_size - channel->sample_offset;
            if (remaining == 0) {
                if (!Restart_Channel_Loop_(*channel)) {
                    channel->flags &= ~0x02;
                    SDL_FlushAudioStream(As_SDL_Audio_Stream_(channel->buffer));
                    break;
                }
                remaining = channel->sample_size;
            }

            uint32_t bytes_to_queue = remaining;
            if (bytes_to_queue > 0x1555) {
                bytes_to_queue = 0x1555;
            }

            if (!Queue_Channel_Data_(*channel, bytes_to_queue)) {
                break;
            }

            if (channel->sample_offset >= channel->sample_size && !Channel_Loop_Enabled_(*channel)) {
                channel->flags &= ~0x02;
                SDL_FlushAudioStream(As_SDL_Audio_Stream_(channel->buffer));
            }

            queued = SDL_GetAudioStreamQueued(As_SDL_Audio_Stream_(channel->buffer));
        }

        if (channel->sample_size <= channel->sample_offset) {
            channel->flags &= ~0x02;
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
            if (volume > sound::_masterVolume) {
                volume = sound::_masterVolume;
            }

            for (int i = 0; i < 16; ++i) {
                sound::_soundChannels[i].current_volume = volume;
                Set_SDL_Stream_Gain_(sound::_soundChannels[i].buffer, volume);
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
        if (sound::_audioPlaybackActive != 1 || !(sound::_stream_data.status & 0x01)) {
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

            if (!Queue_Stream_Until_Target_(sound::_stream_data.block_size * k_stream_prefill_blocks)) {
                return;
            }
        } else if (!(sound::_stream_data.status & 0x08)) {
            sound::Update_Stream_Status_();
        }
    }

    int32_t __cdecl Play_Sound_File_(int32_t channel_idx, void* wave_data, uint32_t wave_data_size) {
        s_sound_wav_header info;
        Stop_Sound_Timer_And_Wait2_();

        (void)wave_data_size;

        if (!Init_Wave_Header_From_Memory_(wave_data, &info)) {
            exitm::Exit_With_Message("SOUND ERROR: Reading Wave");
        }

        Destroy_SDL_Stream_(_soundChannels[channel_idx].buffer);
        Reset_Sound_Channel_(channel_idx);

        if (!Create_SDL_Channel_Stream_(_soundChannels[channel_idx], info)) {
            exitm::Exit_With_Message("SOUND ERROR: Unable to open SDL audio stream.");
        }

        _soundChannels[channel_idx].sample_size = info.data_size;
        _soundChannels[channel_idx].sample_data = (uint8_t*)info.data_ptr;
        _soundChannels[channel_idx].sample_rate = info.sample_rate;
        _soundChannels[channel_idx].volume_index = 0;
        _soundChannels[channel_idx].current_volume = _masterVolume;

        uint32_t dwBytes = _soundChannels[channel_idx].sample_size;
        if (dwBytes > 0x4000) {
            _soundChannels[channel_idx].flags |= 0x2;
            dwBytes = 0x4000;
        }

        if (!Queue_Channel_Data_(_soundChannels[channel_idx], dwBytes)) {
            exitm::Exit_With_Message("SOUND ERROR: Unable to queue SDL channel audio.");
        }

        _soundChannels[channel_idx].flags |= 0x1;

        if (_soundChannels[channel_idx].sample_offset >= _soundChannels[channel_idx].sample_size && !Channel_Loop_Enabled_(_soundChannels[channel_idx])) {
            SDL_FlushAudioStream(As_SDL_Audio_Stream_(_soundChannels[channel_idx].buffer));
        }

        SDL_SetAudioStreamFrequencyRatio(As_SDL_Audio_Stream_(_soundChannels[channel_idx].buffer), 1.0f);
        SDL_ResumeAudioStreamDevice(As_SDL_Audio_Stream_(_soundChannels[channel_idx].buffer));

        Resume_Timer2_();

        return channel_idx;
    }

    /**
     * @brief Searches for an available sound channel.
     * 
     * A channel is considered free if its SDL audio stream is null,
     * or if no audio is queued and the internal flags indicate the channel
     * is not busy.
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

            if (SDL_GetAudioStreamQueued(As_SDL_Audio_Stream_(channel.buffer)) == 0) {
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
        s_sound_channel& channel = _soundChannels[channel_idx];

        if (channel.buffer == nullptr) {
            return;
        }

        int queued = SDL_GetAudioStreamQueued(As_SDL_Audio_Stream_(channel.buffer));
        if (queued > 0) {
            return;
        }

        if (channel.sample_size > 0x4000) {
            if (channel.sample_offset >= channel.sample_size) {
                channel.flags ^= 0x1; // Deactivate active flag
                Remove_Sound_Instance_(channel_idx);
                Destroy_SDL_Stream_(channel.buffer);
                return;
            }
        } else {
            if (Restart_Channel_Loop_(channel)) {
                SDL_ClearAudioStream(As_SDL_Audio_Stream_(channel.buffer));
                if (Queue_Channel_Data_(channel, channel.sample_size)) {
                    SDL_FlushAudioStream(As_SDL_Audio_Stream_(channel.buffer));
                    SDL_ResumeAudioStreamDevice(As_SDL_Audio_Stream_(channel.buffer));
                }
            } else {
                channel.flags ^= 0x1; // Deactivate active flag
                Remove_Sound_Instance_(channel_idx);
                Destroy_SDL_Stream_(channel.buffer);
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

        if (SDL_GetAudioStreamQueued(As_SDL_Audio_Stream_(_stream_data.ds_buffer)) == 0) {
            _stream_data.status ^= 1;
            SDL_PauseAudioStreamDevice(As_SDL_Audio_Stream_(_stream_data.ds_buffer));
            _audioPlaybackActive = 0;
        }
    }

    void __cdecl Stop_Stream_Loop_() {
        if (_stream_data.ds_buffer != nullptr) {
            SDL_ClearAudioStream(As_SDL_Audio_Stream_(_stream_data.ds_buffer));
            SDL_PauseAudioStreamDevice(As_SDL_Audio_Stream_(_stream_data.ds_buffer));
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
        return _audioPlaybackActive;
    }
}