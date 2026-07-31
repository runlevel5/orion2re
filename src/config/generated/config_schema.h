// Generated from specs/config/main.json5. Do not edit.
#pragma once

#include <cstddef>
#include <cstdint>

namespace config::generated {

enum e_value_type {
    CONFIG_VALUE_BOOL,
    CONFIG_VALUE_INT8,
    CONFIG_VALUE_UINT8,
    CONFIG_VALUE_INT16,
    CONFIG_VALUE_UINT16,
    CONFIG_VALUE_INT32,
    CONFIG_VALUE_UINT32,
    CONFIG_VALUE_STRING,
};

enum e_parameter_id : uint64_t {
    CONFIG_PARAMETER_MUSIC_ENABLED = UINT64_C(0x8534D4211ACB65FA),
    CONFIG_PARAMETER_MUSIC_VOLUME = UINT64_C(0xAF330E3168FC345B),
    CONFIG_PARAMETER_SOUND_EFFECTS_ENABLED = UINT64_C(0xADA66B6AA3B175B7),
    CONFIG_PARAMETER_SOUND_EFFECTS_VOLUME = UINT64_C(0xC9A768FD94C4726C),
};

struct s_group_descriptor {
    const char* id;
    const char* title;
    const char* description;
    const char* mutable_policy;
    bool save;
    bool synchronize;
};

struct s_parameter_descriptor {
    e_parameter_id id;
    const char* path;
    const char* title;
    const char* description;
    const char* remarks;
    const char* units;
    const char* group;
    e_value_type type;
    bool has_min;
    bool has_max;
    int64_t min;
    int64_t max;
    bool default_bool;
    int64_t default_integer;
    const char* default_string;
    const char* binding_header;
    const char* binding_symbol;
    const char* binding_cpp_type;
    const char* binding_member;
};

extern const s_group_descriptor groups[];
extern const size_t group_count;
extern const s_parameter_descriptor parameters[];
extern const size_t parameter_count;

}
