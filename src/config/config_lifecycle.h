#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "config/cfg_parser.h"
#include "config/config_runtime.h"

namespace config {

struct s_value_provenance {
    bool has_override = false;
    std::string package_id;
    s_source_location location;
    uint32_t statement = 0;
};

struct s_prior_value {
    s_typed_value value;
    s_value_provenance provenance;
};

struct s_effective_config {
    s_config_state state;
    std::vector<s_value_provenance> provenance;
    std::vector<std::vector<s_prior_value>> prior_values;
};

bool Initialize_Effective_Config(s_effective_config* effective);
bool Read_Effective_Native_Config(s_effective_config* effective);
bool Apply_Config_Document(const s_cfg_document* document, const char* package_id,
                           s_effective_config* effective, s_cfg_diagnostic* diagnostic);

}