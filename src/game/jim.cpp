#include "deps.h"

namespace JIM {
    void Play_Background_Music_() {
        // Only proceed if music is enabled in settings
        if (MOX::_settings.music_on != 1) {
            return;
        }

        _jim_old_music_id = 0xFFFF;

        // If one of the background tracks (1, 2, or 3) is already playing, do nothing
        if (_jim_current_stream == 1 || _jim_current_stream == 2 || _jim_current_stream == 3) {
            return;
        }

        // Fade out existing audio
        sound::Fade_Stream_Down_(25);

        // Wait for volume to reach zero while updating engine state
        while (sound::Get_Stream_Volume_() > 0) {
            MOX::Mox_Update_();
        }

        sound::Stop_Stream_();

        // Randomly select next track from "STREAM.LBX" (indices 1, 2, or 3)
        _jim_current_stream = static_cast<uint16_t>((_clock() % 3) + 1);

        sound::Set_Stream_Filename_(s_STREAM_LBX);
        sound::Play_Stream_(static_cast<int32_t>(_jim_current_stream));
        sound::Loop_Stream_(-1);

        _jim_old_music_id = 0xFFFF;
        sound::Set_Stream_Volume_(0);
        sound::Fade_Stream_Up_(30);
    }
}
