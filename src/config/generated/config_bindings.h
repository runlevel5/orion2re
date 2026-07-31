// Generated from specs/config/main.json5. Do not edit.
#pragma once

#include <cstdint>
#include <string>

#include "config/config_decode.h"

namespace config::generated {

struct s_config_state {
    bool music_enabled = true;
    int8_t music_volume = 50;
    bool sound_effects_enabled = true;
    int8_t sound_effects_volume = 50;
};

void Initialize_Defaults(s_config_state* state);
void Read_Native_State(s_config_state* state);
bool Apply_Write_Set(const s_config_write_set* write_set,
                     const s_config_state* base_state,
                     s_config_state* candidate_state,
                     s_cfg_diagnostic* diagnostic);
bool Apply_Transaction(const s_config_write_set* write_set,
                       s_cfg_diagnostic* diagnostic);
void Commit_State(const s_config_state* state);

}
