// Generated from specs/config/main.json5. Do not edit.
#include "config_schema.h"

#include <cstring>

namespace config::generated {

const s_group_descriptor groups[] = {
    { "interface", "Interface", "Local presentation, audio, and control settings.", false, false },
};

const size_t group_count = sizeof(groups) / sizeof(groups[0]);

const s_parameter_descriptor parameters[] = {
    { CONFIG_PARAMETER_MUSIC_ENABLED, "music_enabled", "Music enabled", "Controls whether game music is played.", "", "", "interface", CONFIG_VALUE_BOOL, false, false, 0, 0, true, 0, nullptr, "game/mox.h", "MOX::_settings", "s_settings", "music_on" },
    { CONFIG_PARAMETER_MUSIC_VOLUME, "music_volume", "Music volume", "Sets music playback volume as a percentage.", "", "percent", "interface", CONFIG_VALUE_INT8, true, true, 0, 100, false, 50, nullptr, "game/mox.h", "MOX::_settings", "s_settings", "music_level" },
    { CONFIG_PARAMETER_SOUND_EFFECTS_ENABLED, "sound_effects_enabled", "Sound effects enabled", "Controls whether game sound effects are played.", "", "", "interface", CONFIG_VALUE_BOOL, false, false, 0, 0, true, 0, nullptr, "game/mox.h", "MOX::_settings", "s_settings", "sound_fx_on" },
    { CONFIG_PARAMETER_SOUND_EFFECTS_VOLUME, "sound_effects_volume", "Sound effects volume", "Sets sound-effect playback volume as a percentage.", "", "percent", "interface", CONFIG_VALUE_INT8, true, true, 0, 100, false, 50, nullptr, "game/mox.h", "MOX::_settings", "s_settings", "sound_fx_level" },
};

const size_t parameter_count = sizeof(parameters) / sizeof(parameters[0]);

const s_parameter_descriptor* Find_Parameter(const char* path, size_t path_length) {
    if (path == nullptr) {
        return nullptr;
    }
    for (size_t index = 0; index < parameter_count; ++index) {
        if (std::strlen(parameters[index].path) == path_length &&
            std::memcmp(parameters[index].path, path, path_length) == 0) {
            return &parameters[index];
        }
    }
    return nullptr;
}

bool Find_Parameter_Index(e_parameter_id parameter_id, size_t* index) {
    if (index == nullptr) {
        return false;
    }
    for (size_t current = 0; current < parameter_count; ++current) {
        if (parameters[current].id == parameter_id) {
            *index = current;
            return true;
        }
    }
    return false;
}

}
