#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace config {

enum e_value_type {
    CONFIG_VALUE_BOOL,
    CONFIG_VALUE_INT8,
    CONFIG_VALUE_UINT8,
    CONFIG_VALUE_INT16,
    CONFIG_VALUE_UINT16,
    CONFIG_VALUE_INT32,
    CONFIG_VALUE_UINT32,
    CONFIG_VALUE_STRING,
    CONFIG_VALUE_RATIO,
};

enum e_scalar_kind {
    CONFIG_SCALAR_PRIMITIVE,
    CONFIG_SCALAR_ENUM,
    CONFIG_SCALAR_MASK,
    CONFIG_SCALAR_RATIO,
};

enum e_schema_node_kind {
    CONFIG_NODE_SCALAR,
    CONFIG_NODE_RECORD,
    CONFIG_NODE_ARRAY,
};

using t_variable_id = uint32_t;

struct s_typed_value {
    e_value_type type = CONFIG_VALUE_BOOL;
    bool bool_value = false;
    int64_t integer_value = 0;
    std::string string_value;
    int64_t ratio_numerator = 0;
    int64_t ratio_denominator = 1;
};

struct s_symbol_descriptor {
    const char* name;
    int64_t value;
};

struct s_group_descriptor {
    const char* id;
    const char* title;
    const char* description;
    bool save;
    bool synchronize;
};

struct s_variable_descriptor {
    const char* name;
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
    e_scalar_kind scalar_kind;
    const s_symbol_descriptor* values;
    size_t value_count;
    const s_symbol_descriptor* flags;
    size_t flag_count;
    bool allow_numeric;
    bool reject_unknown_numeric;
    uint64_t known_bits;
    bool has_min_length;
    bool has_max_length;
    size_t min_length;
    size_t max_length;
    e_value_type ratio_component_type;
};

struct s_schema_node_descriptor {
    const char* id;
    e_schema_node_kind kind;
    uint32_t parent;
    int32_t external_index;
    t_variable_id first_variable;
    t_variable_id last_variable;
    t_variable_id variable_id;
};

struct s_array_index_alias_descriptor {
    uint32_t array_node;
    const char* name;
    int32_t external_index;
};

struct s_runtime_schema {
    const s_schema_node_descriptor* nodes;
    size_t node_count;
    const s_variable_descriptor* variables;
    size_t variable_count;
    const s_array_index_alias_descriptor* array_index_aliases;
    size_t array_index_alias_count;
};

struct s_config_state {
    std::vector<s_typed_value> values;
};

struct s_config_write;
struct s_config_write_set;
struct s_cfg_diagnostic;

extern const s_group_descriptor groups[];
extern const size_t group_count;
extern const s_variable_descriptor variables[];
extern const size_t variable_count;
extern const s_schema_node_descriptor schema_nodes[];
extern const size_t schema_node_count;
extern const s_array_index_alias_descriptor* const array_index_aliases;
extern const size_t array_index_alias_count;

constexpr uint32_t CONFIG_NO_NODE = UINT32_MAX;
constexpr t_variable_id CONFIG_NO_VARIABLE = UINT32_MAX;

const s_variable_descriptor* Get_Variable(t_variable_id variable_id);
const s_runtime_schema* Get_Runtime_Schema();
bool Find_Variable(const char* name, size_t name_length, t_variable_id* variable_id);
const char* Get_Variable_Name(t_variable_id variable_id);
bool Resolve_Write_Path(s_config_write* write, s_cfg_diagnostic* diagnostic);

bool Initialize_Defaults(s_config_state* state);
bool Read_Current_State(s_config_state* state);
bool Read_State_Value(const s_config_state* state, t_variable_id variable_id,
                      s_typed_value* value);
bool Apply_Write_Set(const s_config_write_set* write_set,
                     const s_config_state* base_state,
                     s_config_state* candidate_state,
                     s_cfg_diagnostic* diagnostic);
bool Apply_Transaction(const s_config_write_set* write_set,
                       s_cfg_diagnostic* diagnostic);
bool Commit_State(const s_config_state* state);

namespace generated {

bool Get_Default_Value(t_variable_id variable_id, s_typed_value* value);
bool Set_Value(t_variable_id variable_id, const s_typed_value* value);
bool Get_Current_Value(t_variable_id variable_id, s_typed_value* value);

}

}