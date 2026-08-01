#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "config/cfg_parser.h"
#include "config/config_runtime.h"

namespace config {

struct s_config_write {
    t_variable_id variable_id = 0;
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