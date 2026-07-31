// Generated from specs/config/main.json5. Do not edit.
#include "config_bindings.h"

#include <type_traits>

#include "game/mox.h"

namespace config::generated {

static_assert(std::is_same_v<std::remove_cvref_t<decltype(MOX::_settings)>, s_settings>, "native binding root type mismatch for music_enabled");
static_assert(std::is_integral_v<std::remove_cvref_t<decltype(MOX::_settings.music_on)>> && sizeof(MOX::_settings.music_on) == 1, "native binding type mismatch for music_enabled");
static_assert(std::is_same_v<std::remove_cvref_t<decltype(MOX::_settings)>, s_settings>, "native binding root type mismatch for music_volume");
static_assert(std::is_integral_v<std::remove_cvref_t<decltype(MOX::_settings.music_level)>> && sizeof(MOX::_settings.music_level) == 1 && std::is_signed_v<std::remove_cvref_t<decltype(MOX::_settings.music_level)>>, "native binding type mismatch for music_volume");
static_assert(std::is_same_v<std::remove_cvref_t<decltype(MOX::_settings)>, s_settings>, "native binding root type mismatch for sound_effects_enabled");
static_assert(std::is_integral_v<std::remove_cvref_t<decltype(MOX::_settings.sound_fx_on)>> && sizeof(MOX::_settings.sound_fx_on) == 1, "native binding type mismatch for sound_effects_enabled");
static_assert(std::is_same_v<std::remove_cvref_t<decltype(MOX::_settings)>, s_settings>, "native binding root type mismatch for sound_effects_volume");
static_assert(std::is_integral_v<std::remove_cvref_t<decltype(MOX::_settings.sound_fx_level)>> && sizeof(MOX::_settings.sound_fx_level) == 1 && std::is_signed_v<std::remove_cvref_t<decltype(MOX::_settings.sound_fx_level)>>, "native binding type mismatch for sound_effects_volume");

namespace {
    bool Fail_(const s_config_write* write, const char* message,
               s_cfg_diagnostic* diagnostic) {
        diagnostic->location = write->provenance;
        diagnostic->message = message;
        return false;
    }

    bool Apply_Write_(const s_config_write* write, s_config_state* state,
                      s_cfg_diagnostic* diagnostic) {
        switch (write->parameter_id) {
        case CONFIG_PARAMETER_MUSIC_ENABLED:
            if (write->value.type != CONFIG_VALUE_BOOL) {
                return Fail_(write, "typed write mismatch for music_enabled", diagnostic);
            }
            state->music_enabled = write->value.bool_value;
            return true;
        case CONFIG_PARAMETER_MUSIC_VOLUME:
            if (write->value.type != CONFIG_VALUE_INT8) {
                return Fail_(write, "typed write mismatch for music_volume", diagnostic);
            }
            if (write->value.integer_value < 0 || write->value.integer_value > 100) {
                return Fail_(write, "typed write range error for music_volume", diagnostic);
            }
            state->music_volume = static_cast<int8_t>(write->value.integer_value);
            return true;
        case CONFIG_PARAMETER_SOUND_EFFECTS_ENABLED:
            if (write->value.type != CONFIG_VALUE_BOOL) {
                return Fail_(write, "typed write mismatch for sound_effects_enabled", diagnostic);
            }
            state->sound_effects_enabled = write->value.bool_value;
            return true;
        case CONFIG_PARAMETER_SOUND_EFFECTS_VOLUME:
            if (write->value.type != CONFIG_VALUE_INT8) {
                return Fail_(write, "typed write mismatch for sound_effects_volume", diagnostic);
            }
            if (write->value.integer_value < 0 || write->value.integer_value > 100) {
                return Fail_(write, "typed write range error for sound_effects_volume", diagnostic);
            }
            state->sound_effects_volume = static_cast<int8_t>(write->value.integer_value);
            return true;
        }
        return Fail_(write, "unknown typed write parameter", diagnostic);
    }
}

void Initialize_Defaults(s_config_state* state) {
    *state = {};
}

void Read_Native_State(s_config_state* state) {
    state->music_enabled = MOX::_settings.music_on != 0;
    state->music_volume = static_cast<int8_t>(MOX::_settings.music_level);
    state->sound_effects_enabled = MOX::_settings.sound_fx_on != 0;
    state->sound_effects_volume = static_cast<int8_t>(MOX::_settings.sound_fx_level);
}

bool Read_State_Value(const s_config_state* state, e_parameter_id parameter_id,
                      s_typed_value* value) {
    if (state == nullptr || value == nullptr) {
        return false;
    }
    *value = {};
    switch (parameter_id) {
    case CONFIG_PARAMETER_MUSIC_ENABLED:
        value->type = CONFIG_VALUE_BOOL;
        value->bool_value = state->music_enabled;
        return true;
    case CONFIG_PARAMETER_MUSIC_VOLUME:
        value->type = CONFIG_VALUE_INT8;
        value->integer_value = state->music_volume;
        return true;
    case CONFIG_PARAMETER_SOUND_EFFECTS_ENABLED:
        value->type = CONFIG_VALUE_BOOL;
        value->bool_value = state->sound_effects_enabled;
        return true;
    case CONFIG_PARAMETER_SOUND_EFFECTS_VOLUME:
        value->type = CONFIG_VALUE_INT8;
        value->integer_value = state->sound_effects_volume;
        return true;
    }
    return false;
}

bool Apply_Write_Set(const s_config_write_set* write_set,
                     const s_config_state* base_state,
                     s_config_state* candidate_state,
                     s_cfg_diagnostic* diagnostic) {
    if (write_set == nullptr || base_state == nullptr ||
        candidate_state == nullptr || diagnostic == nullptr) {
        return false;
    }
    *diagnostic = {};
    s_config_state temporary = *base_state;
    for (const s_config_write& write : write_set->writes) {
        if (!Apply_Write_(&write, &temporary, diagnostic)) {
            return false;
        }
    }
    *candidate_state = temporary;
    return true;
}

bool Apply_Transaction(const s_config_write_set* write_set,
                       s_cfg_diagnostic* diagnostic) {
    s_config_state base_state;
    s_config_state candidate_state;
    Read_Native_State(&base_state);
    if (!Apply_Write_Set(write_set, &base_state, &candidate_state, diagnostic)) {
        return false;
    }
    Commit_State(&candidate_state);
    return true;
}

void Commit_State(const s_config_state* state) {
    MOX::_settings.music_on = static_cast<decltype(MOX::_settings.music_on)>(state->music_enabled);
    MOX::_settings.music_level = static_cast<decltype(MOX::_settings.music_level)>(state->music_volume);
    MOX::_settings.sound_fx_on = static_cast<decltype(MOX::_settings.sound_fx_on)>(state->sound_effects_enabled);
    MOX::_settings.sound_fx_level = static_cast<decltype(MOX::_settings.sound_fx_level)>(state->sound_effects_volume);
}

}
