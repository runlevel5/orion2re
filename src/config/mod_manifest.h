#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "config/cfg_parser.h"

namespace config {

struct s_semantic_version {
    uint32_t major = 0;
    uint32_t minor = 0;
    uint32_t patch = 0;
};

struct s_mod_dependency {
    std::string id;
    std::string version;
};

struct s_mod_manifest {
    std::string id;
    std::string name;
    s_semantic_version version;
    std::string version_text;
    std::string engine_range;
    std::vector<s_mod_dependency> dependencies;
    std::vector<s_mod_dependency> optional_dependencies;
    std::vector<std::string> conflicts;
    std::vector<std::string> load_after;
    std::string config;
};

bool Parse_Semantic_Version(const std::string& text, s_semantic_version* version);
bool Validate_Semantic_Version_Range(const std::string& range);
bool Semantic_Version_Matches(const s_semantic_version& version,
                              const std::string& range);
bool Load_Mod_Manifest(const std::string& package_id, s_mod_manifest* manifest,
                       s_cfg_diagnostic* diagnostic);

}