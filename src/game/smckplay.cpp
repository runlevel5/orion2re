#include "pch.h"

namespace smckplay {
    smk _smk;
    uint32_t _lastkey;

    struct s_smacker_state {
        unsigned long width = 0;
        unsigned long height = 0;
        unsigned long frame_count = 0;
        unsigned long current_frame = 0;
        double microseconds_per_frame = 0.0;
        Uint64 next_frame_tick_ns = 0;
        unsigned char audio_track_mask = 0;
        unsigned char audio_channels[7] = {};
        unsigned char audio_bitdepth[7] = {};
        unsigned long audio_rate[7] = {};
        SDL_AudioStream* audio_streams[7] = {};
        bool last_frame = false;
    };

    s_smacker_state _smacker_state;

    namespace {
        smk Open_Smacker_File_(const char* file_name) {
            char resolved_path[512];
            const char* open_path = file_name;

            if (Resolve_File_Path_Case_(file_name, resolved_path, sizeof(resolved_path))) {
                open_path = resolved_path;
            }

            return smk_open_file(open_path, SMK_MODE_MEMORY);
        }
    }

    void Reset_Smacker_State_() {
        memset(&_smacker_state, 0, sizeof(_smacker_state));
    }

    void Close_Smacker_Audio_Streams_() {
        for (SDL_AudioStream*& stream : _smacker_state.audio_streams) {
            if (stream != nullptr) {
                SDL_DestroyAudioStream(stream);
                stream = nullptr;
            }
        }
    }

    SDL_AudioFormat Get_Smacker_Audio_Format_(unsigned char bitdepth) {
        if (bitdepth == 8) {
            return SDL_AUDIO_U8;
        }

        if (bitdepth == 16) {
            return SDL_AUDIO_S16LE;
        }

        return SDL_AUDIO_UNKNOWN;
    }

    void Queue_Smacker_Audio_Frame_() {
        for (int track = 0; track < 7; ++track) {
            SDL_AudioStream* stream = _smacker_state.audio_streams[track];
            if (stream == nullptr) {
                continue;
            }

            const unsigned char* audio_data = smk_get_audio(_smk, (unsigned char)track);
            unsigned long audio_size = smk_get_audio_size(_smk, (unsigned char)track);
            if (audio_data == nullptr || audio_size == 0) {
                continue;
            }

            if (!SDL_PutAudioStreamData(stream, audio_data, (int)audio_size)) {
                SDL_Log("smacker: SDL_PutAudioStreamData failed for track %d: %s", track, SDL_GetError());
            }
        }
    }

    bool Init_Smacker_Audio_Streams_() {
        if ((MOX::_settings.music_on == 0) || (_smacker_state.audio_track_mask == 0)) {
            return true;
        }

        for (int track = 0; track < 7; ++track) {
            if ((_smacker_state.audio_track_mask & (1u << track)) == 0) {
                continue;
            }

            if (Get_Smacker_Audio_Format_(_smacker_state.audio_bitdepth[track]) == SDL_AUDIO_UNKNOWN ||
                _smacker_state.audio_channels[track] == 0 ||
                _smacker_state.audio_rate[track] == 0) {
                continue;
            }

            SDL_AudioStream* stream = sound::Open_Playback_Stream_(
                _smacker_state.audio_bitdepth[track],
                (uint16_t)_smacker_state.audio_channels[track],
                (uint32_t)_smacker_state.audio_rate[track],
                MOX::_settings.music_level);
            if (stream == nullptr) {
                continue;
            }

            smk_enable_audio(_smk, (unsigned char)track, 1);

            _smacker_state.audio_streams[track] = stream;
        }

        return true;
    }

    void Blit_Smacker_Frame_(int32_t left, int32_t top) {
        const unsigned char* frame = smk_get_video(_smk);
        if (frame == nullptr || video::g_page_off.pixels == nullptr) {
            return;
        }

        uint8_t* dst = (uint8_t*)video::g_page_off.pixels;
        const int32_t pitch = video::g_screen_res_x;

        for (unsigned long y = 0; y < _smacker_state.height; ++y) {
            uint8_t* dst_row = dst + ((top + (int32_t)y) * pitch) + left;
            const unsigned char* src_row = frame + (y * _smacker_state.width);

            for (unsigned long x = 0; x < _smacker_state.width; ++x) {
                // libsmacker's decoded frame indices are already aligned to the
                // game's active Smacker palette window. The old smackw32.dll path
                // effectively used palette entries 10..245 directly, so applying
                // an extra +10 index remap here corrupts the popup background.
                dst_row[x] = src_row[x];
            }
        }
    }

    void Update_Smacker_Frame_Metadata_() {
        smk_info_all(_smk, &_smacker_state.current_frame, &_smacker_state.frame_count, &_smacker_state.microseconds_per_frame);
        _smacker_state.last_frame = (_smacker_state.current_frame + 1 >= _smacker_state.frame_count);
    }

    int8_t __cdecl Open_Smacker_Animation_(const char* file_name, char mode, uint32_t options) {
        (void)mode;
        (void)options;

        char path_buffer[512];
        Reset_Smacker_State_();

        _smk = Open_Smacker_File_(file_name);
        if (_smk == nullptr) {
            if (farload::_secondary_drive_path[0] == '\0') {
                return 0;
            }

            if (snprintf(path_buffer, sizeof(path_buffer), "%s%s", farload::_secondary_drive_path, file_name) >= (int)sizeof(path_buffer)) {
                return 0;
            }

            _smk = Open_Smacker_File_(path_buffer);
        }

        if (_smk == nullptr) {
            return 0;
        }

        palette::Save_Palette_();

        if (smk_enable_video(_smk, 1) < 0 ||
            smk_info_video(_smk, &_smacker_state.width, &_smacker_state.height, nullptr) < 0 ||
            smk_info_all(_smk, &_smacker_state.current_frame, &_smacker_state.frame_count, &_smacker_state.microseconds_per_frame) < 0 ||
            smk_info_audio(_smk,
            &_smacker_state.audio_track_mask,
            _smacker_state.audio_channels,
            _smacker_state.audio_bitdepth,
            _smacker_state.audio_rate) < 0) {
            smk_close(_smk);
            _smk = nullptr;
            Reset_Smacker_State_();
            return 0;
        }

        if (!Init_Smacker_Audio_Streams_()) {
            Close_Smacker_Audio_Streams_();
        }

        if (smk_first(_smk) == SMK_ERROR) {
            Close_Smacker_Audio_Streams_();
            smk_close(_smk);
            _smk = nullptr;
            Reset_Smacker_State_();
            return 0;
        }

        Update_Smacker_Frame_Metadata_();
        Queue_Smacker_Audio_Frame_();

        _smacker_state.next_frame_tick_ns = SDL_GetTicksNS();

        return (int8_t)(_smk != nullptr);
    }

    int32_t __cdecl Get_Smacker_Animation_Width_() {
        if (_smk == nullptr) {
            return 0;
        }
        return (int32_t)_smacker_state.width;
    }

    int32_t __cdecl Get_Smacker_Animation_Height_() {
        if (_smk == nullptr) {
            return 0;
        }
        return (int32_t)_smacker_state.height;
    }

    void __cdecl Draw_Smacker_Animation_() {
        Do_Draw_Smacker_Animation_(SMACK::_smack_x, SMACK::_smack_y);
    }

    void __cdecl Do_Draw_Smacker_Animation_(int32_t left, int32_t top) {
        if (smckplay::_smk != nullptr) {
            const Uint64 now_ns = SDL_GetTicksNS();
            if (_smacker_state.next_frame_tick_ns > now_ns) {
                SDL_DelayNS(_smacker_state.next_frame_tick_ns - now_ns);
            }

            smckplay::Set_Smacker_Palette_();
            Blit_Smacker_Frame_(left, top);

            setvesa::Add_Square_To_Scan_List_(left,
                top,
                left + (int32_t)_smacker_state.width,
                top + (int32_t)_smacker_state.height);

            _lastkey = _smacker_state.current_frame + 1;

            if (!_smacker_state.last_frame) {
                if (smk_next(_smk) != SMK_ERROR) {
                    Update_Smacker_Frame_Metadata_();
                    Queue_Smacker_Audio_Frame_();
                }
            }

            const Uint64 frame_duration_ns = (_smacker_state.microseconds_per_frame > 0.0)
                ? (Uint64)(_smacker_state.microseconds_per_frame * 1000.0)
                : 0;
            // Advance from the prior presentation target so occasional slow
            // frames do not permanently rebase the schedule and drain audio
            // lead over time.
            _smacker_state.next_frame_tick_ns += frame_duration_ns;
        }
    }

    void __cdecl Set_Smacker_Palette_() {
        const unsigned char* palette = smk_get_palette(_smk);
        if (palette == nullptr) {
            return;
        }

        // The cinematic palette lives in the game's 10..245 window. The legacy
        // smackw32.dll interface exposed a 236-color view that corresponds to
        // libsmacker palette entries 10..245, not 0..235.
        palette += 10 * 3;

        for (s_palette_entry* current_entry = video::_current_palette + 10;
            current_entry < video::_current_palette + 246;
            ++current_entry, palette += 3) {
            current_entry->changed = 1;
            current_entry->r = (uint8_t)(palette[0] >> 2);
            current_entry->g = (uint8_t)(palette[1] >> 2);
            current_entry->b = (uint8_t)(palette[2] >> 2);
        }
    }

    uint32_t __cdecl Get_Smacker_Animation_Total_Frames_() {
        if (_smk == nullptr) {
            return 0;
        }
        return (uint32_t)_smacker_state.frame_count;
    }

    uint32_t __cdecl Get_Smacker_Animation_Frame_() {
        if (smckplay::_smk == nullptr) {
            return 0;
        }
        return (uint32_t)_smacker_state.current_frame;
    }

    void __cdecl Close_Smacker_Animation_(void) {
        if (_smk != nullptr) {
            Close_Smacker_Audio_Streams_();
            smk_close(_smk);
            _smk = nullptr;
            Reset_Smacker_State_();
            palette::Restore_Palette_();
            palette::Refresh_Palette_();
        }
    }
}