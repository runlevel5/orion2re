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
    s_constraints constraints;
    bool default_bool = false;
    int64_t default_integer = 0;
    std::string default_string;
    s_binding binding;
};

struct s_schema {
    std::vector<s_group> groups;
    std::vector<s_parameter> parameters;
};

bool Load_And_Validate_Schema(const char* path, s_schema* schema, std::string* diagnostic);
const char* Primitive_Name(e_primitive_type type);
std::string Encode_Game_String(const std::string& value);

}