// Generated from specs/config/main.json5. Do not edit.
#include "config/config_runtime.h"

namespace config {

const s_group_descriptor groups[] = {
    { "interface", "Interface", "Local presentation, audio, and control settings.", false, false },
};

const size_t group_count = sizeof(groups) / sizeof(groups[0]);

const s_variable_descriptor variables[] = {
    { "music_enabled", "Music enabled", "Controls whether game music is played.", "", "", "interface", CONFIG_VALUE_BOOL, false, false, 0, 0, CONFIG_SCALAR_PRIMITIVE, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 },
    { "music_volume", "Music volume", "Sets music playback volume as a percentage.", "", "percent", "interface", CONFIG_VALUE_INT8, true, true, 0, 100, CONFIG_SCALAR_PRIMITIVE, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 },
    { "sound_effects_enabled", "Sound effects enabled", "Controls whether game sound effects are played.", "", "", "interface", CONFIG_VALUE_BOOL, false, false, 0, 0, CONFIG_SCALAR_PRIMITIVE, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 },
    { "sound_effects_volume", "Sound effects volume", "Sets sound-effect playback volume as a percentage.", "", "percent", "interface", CONFIG_VALUE_INT8, true, true, 0, 100, CONFIG_SCALAR_PRIMITIVE, nullptr, 0, nullptr, 0, false, false, 0, false, false, 0, 0, CONFIG_VALUE_INT16 },
};

const size_t variable_count = sizeof(variables) / sizeof(variables[0]);

const s_array_index_alias_descriptor* const array_index_aliases = nullptr;

const size_t array_index_alias_count = 0;

const s_schema_node_descriptor schema_nodes[] = {
    { "music_enabled", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, 0, 0, 0 },
    { "music_volume", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, 1, 1, 1 },
    { "sound_effects_enabled", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, 2, 2, 2 },
    { "sound_effects_volume", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, 3, 3, 3 },
};

const size_t schema_node_count = sizeof(schema_nodes) / sizeof(schema_nodes[0]);

}

namespace config::generated {

bool Get_Default_Value(t_variable_id variable_id, s_typed_value* value) {
    if (value == nullptr) {
        return false;
    }
    *value = {};
    switch (variable_id) {
    case 0: // music_enabled
        value->type = CONFIG_VALUE_BOOL;
        value->bool_value = true;
        return true;
    case 1: // music_volume
        value->type = CONFIG_VALUE_INT8;
        value->integer_value = 50;
        return true;
    case 2: // sound_effects_enabled
        value->type = CONFIG_VALUE_BOOL;
        value->bool_value = true;
        return true;
    case 3: // sound_effects_volume
        value->type = CONFIG_VALUE_INT8;
        value->integer_value = 50;
        return true;
    }
    return false;
}

void Set_Value(t_variable_id variable_id, const s_typed_value* value) {
    switch (variable_id) {
    case 0: // music_enabled
        MOX::_settings.music_on = (uint8_t)value->bool_value;
        return;
    case 1: // music_volume
        MOX::_settings.music_level = (int8_t)value->integer_value;
        return;
    case 2: // sound_effects_enabled
        MOX::_settings.sound_fx_on = (uint8_t)value->bool_value;
        return;
    case 3: // sound_effects_volume
        MOX::_settings.sound_fx_level = (int8_t)value->integer_value;
        return;
    }
}

bool Get_Current_Value(t_variable_id variable_id, s_typed_value* value) {
    if (value == nullptr) {
        return false;
    }
    *value = {};
    switch (variable_id) {
    case 0: // music_enabled
        value->type = CONFIG_VALUE_BOOL;
        value->bool_value = (bool)MOX::_settings.music_on;
        return true;
    case 1: // music_volume
        value->type = CONFIG_VALUE_INT8;
        value->integer_value = (int64_t)MOX::_settings.music_level;
        return true;
    case 2: // sound_effects_enabled
        value->type = CONFIG_VALUE_BOOL;
        value->bool_value = (bool)MOX::_settings.sound_fx_on;
        return true;
    case 3: // sound_effects_volume
        value->type = CONFIG_VALUE_INT8;
        value->integer_value = (int64_t)MOX::_settings.sound_fx_level;
        return true;
    }
    return false;
}

}
