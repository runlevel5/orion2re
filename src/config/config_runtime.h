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
    CONFIG_VALUE_BITMAP,
    CONFIG_VALUE_RATIO,
};

enum e_scalar_kind {
    CONFIG_SCALAR_PRIMITIVE,
    CONFIG_SCALAR_ENUM,
    CONFIG_SCALAR_MASK,
    CONFIG_SCALAR_BITMAP,
    CONFIG_SCALAR_RATIO,
};

enum e_schema_node_kind {
    CONFIG_NODE_SCALAR,
    CONFIG_NODE_RECORD,
    CONFIG_NODE_ARRAY,
    CONFIG_NODE_NAMESPACE,
};

using t_variable_id = uint32_t;

struct s_typed_value {
    e_value_type type = CONFIG_VALUE_BOOL;
    bool bool_value = false;
    int64_t integer_value = 0;
    std::string string_value;
    std::vector<uint8_t> bitmap_value;
    int64_t ratio_numerator = 0;
    int64_t ratio_denominator = 1;
};

struct s_symbol_descriptor {
    const char* name;
    int64_t value;
};

struct s_bitmap_symbol_descriptor {
    const char* name;
    const uint8_t* value;
};

struct s_type_descriptor {
    e_value_type value_type;
    bool has_min;
    bool has_max;
    int64_t min;
    int64_t max;
    e_scalar_kind scalar_kind;
    const s_symbol_descriptor* values;
    size_t value_count;
    const s_symbol_descriptor* flags;
    size_t flag_count;
    const s_bitmap_symbol_descriptor* bitmap_values;
    size_t bitmap_value_count;
    const s_bitmap_symbol_descriptor* bitmap_flags;
    size_t bitmap_flag_count;
    const uint8_t* bitmap_known_bits;
    size_t bitmap_bit_count;
    bool allow_numeric;
    bool reject_unknown_numeric;
    uint64_t known_bits;
    bool has_min_length;
    bool has_max_length;
    size_t min_length;
    size_t max_length;
    e_value_type ratio_component_type;
};

struct s_ratio_value {
    int64_t numerator;
    int64_t denominator;
};

struct s_variable_target {
    void* address;
    size_t size;
};

struct s_variable_descriptor {
    const char* name;
    const s_type_descriptor* type;
};

constexpr size_t CONFIG_MAX_VARIABLE_TARGETS = 2;

struct s_resolved_variable {
    const s_type_descriptor* type;
    s_variable_target targets[CONFIG_MAX_VARIABLE_TARGETS];
    size_t target_count;
};

struct s_schema_node_descriptor {
    const char* id;
    e_schema_node_kind kind;
    uint32_t parent;
    int32_t external_index;
    t_variable_id first_variable;
    t_variable_id last_variable;
    t_variable_id variable_id;
    bool has_terminator;
    int64_t terminator;
    uint32_t first_child;
    uint32_t next_sibling;
    const s_type_descriptor* type;
};

struct s_array_index_alias_descriptor {
    uint32_t array_node;
    const char* name;
    int32_t external_index;
};

struct s_runtime_schema {
    const s_schema_node_descriptor* nodes;
    size_t node_count;
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

extern const size_t variable_count;
extern const s_schema_node_descriptor schema_nodes[];
extern const size_t schema_node_count;
extern const s_array_index_alias_descriptor* const array_index_aliases;
extern const size_t array_index_alias_count;

constexpr uint32_t CONFIG_NO_NODE = UINT32_MAX;
constexpr t_variable_id CONFIG_NO_VARIABLE = UINT32_MAX;

const s_runtime_schema* Get_Runtime_Schema();
const s_schema_node_descriptor* Get_Variable_Node(const s_runtime_schema* schema,
                                                  t_variable_id variable_id);
const s_type_descriptor* Get_Variable_Type(t_variable_id variable_id);
bool Find_Variable(const char* name, size_t name_length, t_variable_id* variable_id);
bool Get_Variable_Name(const s_runtime_schema* schema, t_variable_id variable_id,
                       std::string* name);
bool Get_Variable_Name(t_variable_id variable_id, std::string* name);
bool Resolve_Write_Path(s_config_write* write, s_cfg_diagnostic* diagnostic);

bool Initialize_Defaults(s_config_state* state);
bool Read_Current_State(s_config_state* state);
bool Read_State_Value(const s_config_state* state, t_variable_id variable_id,
                      s_typed_value* value);
bool Apply_Write_Set(const s_config_write_set* write_set, s_config_state* state,
                     s_cfg_diagnostic* diagnostic);
bool Commit_State(const s_config_state* state);

namespace generated {

bool Get_Default_Value(t_variable_id variable_id, s_typed_value* value);
bool Initialize_Scalar_Array_Defaults(uint32_t node_index, s_config_state* state);
bool Resolve_Variable(t_variable_id variable_id, s_resolved_variable* variable);
void Set_Value(t_variable_id variable_id, const s_typed_value* value);
bool Get_Current_Value(t_variable_id variable_id, s_typed_value* value);

}

}