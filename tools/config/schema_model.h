#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace config::tool {

enum e_primitive_type {
    PRIMITIVE_BOOL,
    PRIMITIVE_INT8,
    PRIMITIVE_UINT8,
    PRIMITIVE_INT16,
    PRIMITIVE_UINT16,
    PRIMITIVE_INT32,
    PRIMITIVE_UINT32,
    PRIMITIVE_STRING,
    PRIMITIVE_RATIO,
};

enum e_scalar_kind {
    SCALAR_PRIMITIVE,
    SCALAR_ENUM,
    SCALAR_MASK,
    SCALAR_RATIO,
};

enum e_node_kind {
    NODE_SCALAR,
    NODE_RECORD,
    NODE_ARRAY,
};

struct s_symbol {
    std::string name;
    int64_t value = 0;
};

struct s_metadata {
    std::string title;
    std::string description;
    std::string remarks;
    std::string units;
    std::vector<std::string> examples;
};

struct s_constraints {
    bool has_min = false;
    bool has_max = false;
    int64_t min = 0;
    int64_t max = 0;
    bool has_min_length = false;
    bool has_max_length = false;
    size_t min_length = 0;
    size_t max_length = 0;
    std::string encoding;
};

struct s_binding {
    std::string header;
    std::string symbol;
    std::string cpp_type;
    std::vector<std::string> members;
};

struct s_group {
    std::string id;
    s_metadata metadata;
    bool save = false;
    bool synchronize = false;
};

struct s_parameter {
    std::string path;
    s_metadata metadata;
    std::string group;
    e_primitive_type type = PRIMITIVE_BOOL;
    e_scalar_kind scalar_kind = SCALAR_PRIMITIVE;
    s_constraints constraints;
    bool default_bool = false;
    int64_t default_integer = 0;
    std::string default_string;
    int64_t default_ratio_numerator = 0;
    int64_t default_ratio_denominator = 1;
    e_primitive_type ratio_component_type = PRIMITIVE_INT16;
    std::vector<s_symbol> values;
    std::vector<s_symbol> flags;
    bool allow_numeric = false;
    bool reject_unknown_numeric = false;
    uint64_t known_bits = 0;
    s_binding binding;
    std::vector<std::string> native_targets;
};

struct s_node {
    std::string id;
    e_node_kind kind = NODE_SCALAR;
    uint32_t parent = UINT32_MAX;
    int32_t external_index = 0;
    std::vector<s_symbol> index_aliases;
    uint32_t first_parameter = UINT32_MAX;
    uint32_t last_parameter = UINT32_MAX;
    uint32_t parameter = UINT32_MAX;
};

struct s_schema {
    std::vector<s_group> groups;
    std::vector<s_parameter> parameters;
    std::vector<s_node> nodes;
};

bool Load_And_Validate_Schema(const char* path, s_schema* schema, std::string* diagnostic);
const char* Primitive_Name(e_primitive_type type);
std::string Encode_Game_String(const std::string& value);

}