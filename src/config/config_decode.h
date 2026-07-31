#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "config/cfg_parser.h"
#include "config/generated/config_schema.h"

namespace config {

struct s_typed_value {
    generated::e_value_type type = generated::CONFIG_VALUE_BOOL;
    bool bool_value = false;
    int64_t integer_value = 0;
    std::string string_value;
};

struct s_config_write {
    generated::e_parameter_id parameter_id = generated::CONFIG_PARAMETER_MUSIC_ENABLED;
    std::string canonical_path;
    s_typed_value value;
    s_source_location provenance;
};

struct s_config_write_set {
    std::vector<s_config_write> writes;
};

bool Decode_Assignment(const s_cfg_statement* statement, s_config_write_set* write_set,
                       s_cfg_diagnostic* diagnostic);

}