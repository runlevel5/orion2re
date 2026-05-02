#include "pch.h"

namespace sound {
    /**
     * @brief Iterates through active sound channels and updates their volume levels via DirectSound.
     * Stops and removes channels if the volume delta is out of bounds.
     */
    void __cdecl Sound_Update_() {
        for (int i = 0; i < 17; ++i) {
            s_sound_channel& channel = _sound_channels[i];

            // Check if bit 0 is set (active flag) and volume is non-zero
            if ((channel.flags & 1) && channel.volume_index != 0) {
                int vol_level = Get_Channel_Volume_Level_(i);
                int diff = vol_level - static_cast<int>(channel.volume_index);

                // Valid delta range is [1, 100]
                if (diff >= 1 && diff <= 100) {
                    int16_t ds_vol = _ds_volume_table[diff];
                    channel.buffer->SetVolume(static_cast<int32_t>(ds_vol));
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
        _sound_channels[channel_idx].sample_data = nullptr;
        _sound_channels[channel_idx].sample_size = 0;
        _sound_channels[channel_idx].sample_offset = 0;
        _sound_channels[channel_idx].ds_buffer_size = 0;
        _sound_channels[channel_idx].field7_0x13 = 0;
        _sound_channels[channel_idx].write_cursor = 0;
        _sound_channels[channel_idx].flags = 0;
        _sound_channels[channel_idx].volume_index = 0;
        _sound_channels[channel_idx].buffer->Stop();
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
        if (_strcmpi(tag_buf, s_RIFF_0055c224) != 0) {
            return false;
        }

        sound::Copy_FourCC_(tag_buf, header->wave_tag);
        if (_strcmpi(tag_buf, s_WAVE_0055c21c) != 0) {
            return false;
        }

        sound::Copy_FourCC_(tag_buf, header->fmt_tag);
        if (_strcmpi(tag_buf, s_fmt_0055c214) != 0) {
            return false;
        }

        sound::Copy_FourCC_(tag_buf, header->data_tag);
        return _strcmpi(tag_buf, s_data_0055c20c) == 0;
    }

    /**
     * @address 0052DC80
     */
    void __cdecl Destroy_Sound_() {
        Stop_Sound_Timer_And_Wait2_();
        PTR_timeKillEvent_005aa5cc(_soundTimerId);
        PTR_timeEndPeriod_005aa5c4(0xA0);

        if (_soundOn == 1) {
            _dsound->Release();
        }

        _audioInitialized = 0;
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
                        memset(&_sound_channels[i], 0, sizeof(s_sound_channel));

                        _dsound_hr = _dsound->CreateSoundBuffer((LPCDSBUFFERDESC)&_dsound_buffer_desc, &_sound_channels[i].buffer, nullptr);
                        if (_dsound_hr != DS_OK) {
                            _dsound->Release();
                            return;
                        }
                        
                        _sound_channels[i].buffer->Stop();
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

    void __stdcall Sound_Timer_Callback_(uint32_t p1, uint32_t p2, uint32_t p3, uint32_t p4, uint32_t p5) {
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
     * @address 00532440
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
     * @address 0052e7a0
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
                LPDIRECTSOUNDBUFFER ds_buffer = _sound_channels[channel_idx].buffer;

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
            s_sound_channel* channel = &sound::_sound_channels[channel_idx];
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

    void __cdecl Resume_Sound_(int sound) {
        return;
    }

    int32_t __cdecl Get_Stream_Volume_() {
        if (sound::_soundFxActive != 0 && _stream_data.ds_buffer != nullptr) {
            LONG current_volume = 0;
            _stream_data.ds_buffer->GetVolume(&current_volume);

            int32_t index = 0;
            for (int16_t* entry = sound::_ds_volume_table; entry < sound::_stream_volume_table_end; ++entry) {
                if (static_cast<int32_t>(*entry) == current_volume) {
                    return index;
                }
                index++;
            }

            return static_cast<int32_t>(current_volume);
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
            LPDIRECTSOUNDBUFFER buffer = _sound_channels[handle].buffer;

            DWORD frequency;
            buffer->GetFrequency(&frequency);

            frequency += (DWORD)pitch;

            buffer->SetFrequency(frequency);
        }
    }

    int8_t __cdecl Load_Stream_Wave_Header_(int entry_index, s_sound_stream* stream) {
        if (entry_index == 0) {
            return 0;
        }

        uint32_t entry_offset = _streamLbxHandle.entries[entry_index - 1].offset;

        mmioSeek(_streamLbxHandle.file, entry_offset, SEEK_SET);

        char tag_buffer[5];
        tag_buffer[4] = '\0';

        mmioRead(_streamLbxHandle.file, stream, 0x30);
        stream->data_start_offset = entry_offset + 0x2c;

        sound::Copy_FourCC_(tag_buffer, stream->riff_tag);
        if (_strcmpi(tag_buffer, sound::s_RIFF_0055c224) != 0) {
            return 0;
        }

        sound::Copy_FourCC_(tag_buffer, stream->wave_tag);
        if (_strcmpi(tag_buffer, sound::s_WAVE_0055c21c) != 0) {
            return 0;
        }

        sound::Copy_FourCC_(tag_buffer, stream->fmt_tag);
        if (_strcmpi(tag_buffer, sound::s_fmt_0055c214) != 0) {
            return 0;
        }

        sound::Copy_FourCC_(tag_buffer, stream->data_tag);
        if (_strcmpi(tag_buffer, sound::s_data_0055c20c) != 0) {
            return 0;
        }

        mmioSeek(_streamLbxHandle.file, stream->data_start_offset, SEEK_SET);

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
            strcpy(&msg_buf[0], s_STREAM_ERROR_Play_Stream_Unable_to_lock_buffer);
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
                _sprintf(msg_buf, s_STREAM_ERROR_0x_x_DSERR_INVALID_0055b954, _dsound_hr);
            } else if (_dsound_hr == DSERR_INVALIDCALL) {
                _sprintf(msg_buf, s_STREAM_ERROR_0x_x_DSERR_INVALID_0055b998, _dsound_hr);
            } else if (_dsound_hr == DSERR_PRIOLEVELNEEDED) {
                _sprintf(msg_buf, s_STREAM_ERROR_0x_x_DSERR_PRIOLEV_0055b910, _dsound_hr);
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
        MMRESULT res = sound::mmioGetInfo(sound::_streamLbxHandle.file, &mmio_info, 0);

        uint32_t status = (res == 0) ? 0 : 1;

        if (status == 0) {
            uint32_t total_copied = 0;
            bool error_occurred = false;

            if (bytes_to_read != 0) {
                while (total_copied < bytes_to_read) {
                    if (mmio_info.pchNext == mmio_info.pchEndRead) {
                        status = sound::mmioAdvance(sound::_streamLbxHandle.file, &mmio_info, 0);
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

                    memcpy(static_cast<uint8_t*>(buffer) + total_copied, mmio_info.pchNext, chunk_size);

                    mmio_info.pchNext += chunk_size;
                    total_copied += chunk_size;
                }
            }

            if (!error_occurred) {
                status = sound::mmioSetInfo(sound::_streamLbxHandle.file, &mmio_info, 0);
                if (status == 0) {
                    *bytes_read_ptr = bytes_to_read;
                    return 0;
                }
            }
        }

        fonts::Disp_PSTR_(320 /* 0x140 */, 0, sound::s_Cant_Read);
        *bytes_read_ptr = 0;
        return static_cast<uint8_t>(status);
    }

    /**
     * @address 0052e850
     */
    void __cdecl Fadeout_Sound_(int16_t channel_idx, int8_t volume_idx) {
        if (channel_idx >= 0 && channel_idx < 16) {
            _sound_channels[channel_idx].volume_index = volume_idx;
        }
    }

    void __cdecl Loop_Sound_(int channel_idx, int8_t loop_count) {
        if (channel_idx >= 1) {
            _sound_channels[channel_idx].flags |= 0x08;
            _sound_channels[channel_idx].loop_count = loop_count;
        }
    }

    int __cdecl Get_Channel_Volume_Level_(int channel_idx) {
        LONG volume = 0;

        if (sound::_soundOn == 0) {
            return 0;
        }

        if (channel_idx >= 0 && channel_idx < 16) {
            _sound_channels[channel_idx].buffer->GetVolume(&volume);

            for (int i = 0; i < 101; ++i) {
                if (sound::_ds_volume_table[i] == volume) {
                    return i;
                }
            }
        }

        return (int)volume;
    }

    void __cdecl Copy_FourCC_(void* dest, const void* src) {
        *static_cast<uint32_t*>(dest) = *static_cast<const uint32_t*>(src);
    }

    void __cdecl Refill_Sound_Channel_Buffer_(int channel_idx) {
        DWORD play_cursor;
        DWORD write_cursor;
        DWORD status;
        DWORD pos1;
        DWORD pos2;

        s_sound_channel* channel = &_sound_channels[channel_idx];

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
                if (sound::_sound_channels[i].flags & 1) {
                    if (sound::_sound_channels[i].flags & 2) {
                        sound::Refill_Sound_Channel_Buffer_(i);
                    } else {
                        sound::Update_Sound_Channel_Status_(i);
                    }
                }
            }
        }
    }

    /**
     * @address 0052e800
     */
    void __cdecl Set_Master_Volume_(int32_t volume) {
        if (sound::_soundOn != 0 && volume >= 0 && volume <= 100) {
            // Cap volume to master volume limit
            if (volume > sound::_dsMasterVolume) {
                volume = sound::_dsMasterVolume;
            }

            int16_t ds_volume = sound::_ds_volume_table[volume];

            for (int i = 0; i < 16; ++i) {
                LPDIRECTSOUNDBUFFER ds_buffer = sound::_sound_channels[i].buffer;
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

    int32_t __cdecl Play_Sound_File_(int32_t channel_idx, void* wave_data, uint32_t size) {
        s_sound_wav_header info;
        WAVEFORMATEX wfx;
        char error_msg[256];
        uint32_t dwBytes;
        Stop_Sound_Timer_And_Wait2_();

        if (!Init_Wave_Header_From_Memory_(wave_data, &info)) {
            exitm::Exit_With_Message(s_SOUND_ERROR__Reading_Wave_0055b634);
        }

        _sound_channels[channel_idx].buffer->Stop();
        Reset_Sound_Channel_(channel_idx);
        _dsound_hr = _sound_channels[channel_idx].buffer->Release();

        memset(&_dsound_buffer_desc, 0, sizeof(DSBUFFERDESC1));
        _dsound_buffer_desc.dwSize = 0x14;
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

        _dsound_hr = _dsound->CreateSoundBuffer((LPCDSBUFFERDESC)&_dsound_buffer_desc, &_sound_channels[channel_idx].buffer, NULL);

        _sound_channels[channel_idx].sample_size = info.data_size;
        _sound_channels[channel_idx].sample_data = (uint8_t*)info.data_ptr;
        _sound_channels[channel_idx].volume_index = 0;

        dwBytes = _sound_channels[channel_idx].sample_size;
        if (dwBytes > 0x4000) {
            _sound_channels[channel_idx].flags |= 0x2;
            dwBytes = 0x4000;
        }

        _dsound_hr = _sound_channels[channel_idx].buffer->Lock(0, dwBytes, &_streamBuffer1, (LPDWORD)&_streamAvailable1, &_streamBuffer2, (LPDWORD)&_streamAvailable2, 0);

        if (_dsound_hr != 0) {
            strcpy(error_msg, s_SOUND_ERROR___Play_Sound_File__U_0055b5fc);
            exitm::Exit_With_Message(error_msg);
        }

        if (_streamAvailable1 != 0) {
            memcpy(_streamBuffer1, _sound_channels[channel_idx].sample_data + _sound_channels[channel_idx].sample_offset, _streamAvailable1);
            _sound_channels[channel_idx].sample_offset += _streamAvailable1;
        }

        if (_streamAvailable2 != 0) {
            memcpy(_streamBuffer2, _sound_channels[channel_idx].sample_data + _sound_channels[channel_idx].sample_offset, _streamAvailable2);
            _sound_channels[channel_idx].sample_offset += _streamAvailable2;
        }

        _dsound_hr = _sound_channels[channel_idx].buffer->Unlock(_streamBuffer1, _streamAvailable1, _streamBuffer2, _streamAvailable2);

        if (_dsound_hr != 0) {
            const char* fmt = nullptr;
            if (_dsound_hr == 0x80070057 /* DSERR_INVALIDPARAM */) {
                fmt = s_SOUND_ERROR_0x_x__DSERR_INVALIDP_0055b57c;
            } else if (_dsound_hr == 0x88780032 /* DSERR_INVALIDCALL */) {
                fmt = s_SOUND_ERROR_0x_x__DSERR_INVALIDC_0055b5bc;
            } else if (_dsound_hr == 0x88780046 /* DSERR_PRIOLEVELNEEDED */) {
                fmt = s_SOUND_ERROR_0x_x__DSERR_PRIOLEVE_0055b538;
            }

            if (fmt) {
                _sprintf(error_msg, fmt, _dsound_hr);
            }
            exitm::Exit_With_Message(error_msg);
        }

        _sound_channels[channel_idx].ds_buffer_size = dwBytes;
        _sound_channels[channel_idx].flags |= 0x1;

        int32_t master_vol = _dsMasterVolume;
        _sound_channels[channel_idx].buffer->SetVolume((LONG)_ds_volume_table[master_vol]);
        _sound_channels[channel_idx].buffer->Play(0, 0, (dwBytes >= 0x4000) ? 1 /* DSBPLAY_LOOPING */ : 0);

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
            s_sound_channel& channel = _sound_channels[i];

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

                void* buffer = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(_soundBuffer) + index.buffer_offset);
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
    void __cdecl Update_Sound_Channel_Status_(int channel_idx) {
        DWORD play_cursor;
        DWORD ds_write_cursor;
        DWORD status;
        uint32_t distance;

        s_sound_channel& channel = _sound_channels[channel_idx];

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
     * @address 0052e5a0
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
     * @brief Traverses the sound buffer index list and merges adjacent free blocks.
     */
    /**
     * @address 0052e2b0
     */
    void* __cdecl Allocate_Sound_Block_(int32_t channel, int32_t resource_id, int32_t size) {
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
                        current->active_channel_ids[0] = (uint8_t)channel;
                        current->resource_id = resource_id;
                        current->buffer_size = size;
                        current->num_active_channels = 1;

                        return reinterpret_cast<uint8_t*>(_soundBuffer) + current->buffer_offset;
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