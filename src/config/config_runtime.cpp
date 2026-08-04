#include "config_runtime.h"

#include <algorithm>
#include <charconv>
#include <climits>
#include <cstring>
#include <string_view>
#include <utility>

#include "config/config_decode.h"

namespace config {
namespace {
    bool Read_Integer_(const void* address, e_value_type type, int64_t* value) {
        switch (type) {
        case CONFIG_VALUE_BOOL: *value = *(const uint8_t*)address; return true;
        case CONFIG_VALUE_INT8: *value = *(const int8_t*)address; return true;
        case CONFIG_VALUE_UINT8: *value = *(const uint8_t*)address; return true;
        case CONFIG_VALUE_INT16: *value = *(const int16_t*)address; return true;
        case CONFIG_VALUE_UINT16: *value = *(const uint16_t*)address; return true;
        case CONFIG_VALUE_INT32: *value = *(const int32_t*)address; return true;
        case CONFIG_VALUE_UINT32: *value = *(const uint32_t*)address; return true;
        default: return false;
        }
    }

    bool Write_Integer_(void* address, size_t size, e_value_type type, int64_t value) {
        switch (type) {
        case CONFIG_VALUE_BOOL:
            if (size != sizeof(uint8_t)) return false;
            *(uint8_t*)address = (uint8_t)(value != 0);
            return true;
        case CONFIG_VALUE_INT8:
            if (size != sizeof(int8_t)) return false;
            *(int8_t*)address = (int8_t)value;
            return true;
        case CONFIG_VALUE_UINT8:
            if (size != sizeof(uint8_t)) return false;
            *(uint8_t*)address = (uint8_t)value;
            return true;
        case CONFIG_VALUE_INT16:
            if (size != sizeof(int16_t)) return false;
            *(int16_t*)address = (int16_t)value;
            return true;
        case CONFIG_VALUE_UINT16:
            if (size != sizeof(uint16_t)) return false;
            *(uint16_t*)address = (uint16_t)value;
            return true;
        case CONFIG_VALUE_INT32:
            if (size != sizeof(int32_t)) return false;
            *(int32_t*)address = (int32_t)value;
            return true;
        case CONFIG_VALUE_UINT32:
            if (size != sizeof(uint32_t)) return false;
            *(uint32_t*)address = (uint32_t)value;
            return true;
        default: return false;
        }
    }

    bool Fail_(const s_config_write* write, const char* message,
               s_cfg_diagnostic* diagnostic) {
        diagnostic->location = write->provenance;
        diagnostic->message = message;
        return false;
    }

    void Primitive_Range_(e_value_type type, int64_t* minimum, int64_t* maximum) {
        switch (type) {
        case CONFIG_VALUE_INT8: *minimum = INT8_MIN; *maximum = INT8_MAX; break;
        case CONFIG_VALUE_UINT8: *minimum = 0; *maximum = UINT8_MAX; break;
        case CONFIG_VALUE_INT16: *minimum = INT16_MIN; *maximum = INT16_MAX; break;
        case CONFIG_VALUE_UINT16: *minimum = 0; *maximum = UINT16_MAX; break;
        case CONFIG_VALUE_INT32: *minimum = INT32_MIN; *maximum = INT32_MAX; break;
        case CONFIG_VALUE_UINT32: *minimum = 0; *maximum = UINT32_MAX; break;
        default: *minimum = 0; *maximum = 0; break;
        }
    }

    bool Validate_Write_(const s_config_write* write, s_cfg_diagnostic* diagnostic) {
        const s_type_descriptor* type = Get_Variable_Type(write->variable_id);
        if (type == nullptr) {
            return Fail_(write, "unknown typed write variable", diagnostic);
        }
        if (write->value.type != type->value_type) {
            diagnostic->location = write->provenance;
            std::string name;
            Get_Variable_Name(write->variable_id, &name);
            diagnostic->message = "typed write mismatch for " + name;
            return false;
        }
        if (type->value_type == CONFIG_VALUE_BOOL) {
            return true;
        }
        if (type->value_type == CONFIG_VALUE_STRING) {
            const size_t length = write->value.string_value.size();
            if ((type->has_min_length && length < type->min_length) ||
                (type->has_max_length && length > type->max_length)) {
                return Fail_(write, "typed write string length error", diagnostic);
            }
            return true;
        }
        if (type->value_type == CONFIG_VALUE_BITMAP) {
            const size_t byte_count = (type->bitmap_bit_count + 7) / 8;
            if (write->value.bitmap_value.size() != byte_count) {
                return Fail_(write, "typed write bitmap size error", diagnostic);
            }
            for (size_t index = 0; index < byte_count; ++index) {
                if ((write->value.bitmap_value[index] &
                     static_cast<uint8_t>(~type->bitmap_known_bits[index])) != 0) {
                    return Fail_(write, "typed write unknown bitmap bits", diagnostic);
                }
            }
            if ((type->bitmap_bit_count & 7) != 0 && !write->value.bitmap_value.empty()) {
                const uint8_t final_mask = static_cast<uint8_t>(
                    (1U << (type->bitmap_bit_count & 7)) - 1U);
                if ((write->value.bitmap_value.back() & static_cast<uint8_t>(~final_mask)) != 0) {
                    return Fail_(write, "typed write bitmap padding bits", diagnostic);
                }
            }
            return true;
        }
        if (type->value_type == CONFIG_VALUE_RATIO) {
            int64_t minimum = 0;
            int64_t maximum = 0;
            Primitive_Range_(type->ratio_component_type, &minimum, &maximum);
            if (write->value.ratio_denominator <= 0 ||
                write->value.ratio_numerator < minimum ||
                write->value.ratio_numerator > maximum ||
                write->value.ratio_denominator > maximum) {
                return Fail_(write, "typed write ratio component error", diagnostic);
            }
            return true;
        }

        int64_t primitive_minimum = 0;
        int64_t primitive_maximum = 0;
        Primitive_Range_(type->value_type, &primitive_minimum, &primitive_maximum);
        const int64_t minimum = type->has_min ? type->min : primitive_minimum;
        const int64_t maximum = type->has_max ? type->max : primitive_maximum;
        if (write->value.integer_value < minimum || write->value.integer_value > maximum) {
            diagnostic->location = write->provenance;
            std::string name;
            Get_Variable_Name(write->variable_id, &name);
            diagnostic->message = "typed write range error for " + name;
            return false;
        }
        if (type->scalar_kind == CONFIG_SCALAR_ENUM &&
            type->reject_unknown_numeric) {
            bool found = false;
            for (size_t index = 0; index < type->value_count; ++index) {
                found = found || type->values[index].value == write->value.integer_value;
            }
            if (!found) {
                return Fail_(write, "typed write unknown enum value", diagnostic);
            }
        }
        if (type->scalar_kind == CONFIG_SCALAR_MASK &&
            type->reject_unknown_numeric &&
            (static_cast<uint64_t>(write->value.integer_value) &
             ~type->known_bits) != 0) {
            return Fail_(write, "typed write unknown mask bits", diagnostic);
        }
        return true;
    }

    bool Validate_Node_Terminators_(uint32_t node_index, t_variable_id variable_offset,
                                    const s_config_state* state) {
        const s_schema_node_descriptor& node = schema_nodes[node_index];
        if (node.has_terminator) {
            bool found = false;
            for (t_variable_id variable_id = node.first_variable + variable_offset;
                 variable_id <= node.last_variable + variable_offset; ++variable_id) {
                found = found || state->values[variable_id].integer_value == node.terminator;
            }
            if (!found) return false;
        }
        if (node.kind == CONFIG_NODE_ARRAY && node.array_element_variable_count != 0) {
            for (uint32_t element = 0; element < node.array_element_count; ++element) {
                if (!Validate_Node_Terminators_(
                        node.first_child,
                        variable_offset + element * node.array_element_variable_count,
                        state)) {
                    return false;
                }
            }
            return true;
        }
        for (uint32_t child = node.first_child; child != CONFIG_NO_NODE;
             child = schema_nodes[child].next_sibling) {
            if (!Validate_Node_Terminators_(child, variable_offset, state)) return false;
        }
        return true;
    }
}

const s_runtime_schema* Get_Runtime_Schema() {
    static const s_runtime_schema schema = {
        schema_nodes,
        schema_node_count,
        variable_count,
        array_index_aliases,
        array_index_alias_count,
    };
    return &schema;
}

bool Get_Variable_Location(const s_runtime_schema* schema, t_variable_id variable_id,
                           s_variable_location* location) {
    if (schema == nullptr || location == nullptr || variable_id >= schema->variable_count) {
        return false;
    }
    uint32_t node_index = CONFIG_NO_NODE;
    for (uint32_t index = 0; index < schema->node_count; ++index) {
        const s_schema_node_descriptor& node = schema->nodes[index];
        if (node.parent == CONFIG_NO_NODE && variable_id >= node.first_variable &&
            variable_id <= node.last_variable) {
            node_index = index;
            break;
        }
    }
    t_variable_id variable_offset = 0;
    while (node_index != CONFIG_NO_NODE) {
        const s_schema_node_descriptor& node = schema->nodes[node_index];
        if (node.kind == CONFIG_NODE_SCALAR) {
            if (node.variable_id + variable_offset != variable_id) return false;
            *location = {&node, variable_offset};
            return true;
        }
        if (node.array_element_count != 0 && node.array_element_variable_count == 0) {
            if (variable_id < node.first_variable + variable_offset ||
                variable_id > node.last_variable + variable_offset) return false;
            *location = {&node, variable_offset};
            return true;
        }
        if (node.kind == CONFIG_NODE_ARRAY && node.array_element_variable_count != 0) {
            const t_variable_id first = node.first_variable + variable_offset;
            if (variable_id < first || variable_id > node.last_variable + variable_offset) {
                return false;
            }
            const t_variable_id element =
                (variable_id - first) / node.array_element_variable_count;
            variable_offset += element * node.array_element_variable_count;
            node_index = node.first_child;
            continue;
        }
        uint32_t selected = CONFIG_NO_NODE;
        for (uint32_t child = node.first_child; child != CONFIG_NO_NODE;
             child = schema->nodes[child].next_sibling) {
            const s_schema_node_descriptor& candidate = schema->nodes[child];
            if (variable_id >= candidate.first_variable + variable_offset &&
                variable_id <= candidate.last_variable + variable_offset) {
                selected = child;
                break;
            }
        }
        node_index = selected;
    }
    return false;
}

const s_schema_node_descriptor* Get_Variable_Node(const s_runtime_schema* schema,
                                                  t_variable_id variable_id) {
    s_variable_location location = {};
    return Get_Variable_Location(schema, variable_id, &location) ? location.node : nullptr;
}

bool Get_Array_Element_Offset(const s_runtime_schema* schema, t_variable_id variable_id,
                              uint32_t array_node, uint32_t* element_offset) {
    if (schema == nullptr || element_offset == nullptr || array_node >= schema->node_count) {
        return false;
    }
    uint32_t node_index = CONFIG_NO_NODE;
    for (uint32_t index = 0; index < schema->node_count; ++index) {
        const s_schema_node_descriptor& node = schema->nodes[index];
        if (node.parent == CONFIG_NO_NODE && variable_id >= node.first_variable &&
            variable_id <= node.last_variable) {
            node_index = index;
            break;
        }
    }
    t_variable_id variable_offset = 0;
    while (node_index != CONFIG_NO_NODE) {
        const s_schema_node_descriptor& node = schema->nodes[node_index];
        if (node.kind == CONFIG_NODE_ARRAY) {
            const t_variable_id first = node.first_variable + variable_offset;
            const uint32_t stride = node.array_element_variable_count == 0 ? 1 :
                                    node.array_element_variable_count;
            if (stride == 0 || variable_id < first ||
                variable_id > node.last_variable + variable_offset) return false;
            const uint32_t element = (variable_id - first) / stride;
            if (node_index == array_node) {
                *element_offset = element;
                return true;
            }
            if (node.array_element_variable_count == 0) return false;
            variable_offset += element * stride;
            node_index = node.first_child;
            continue;
        }
        uint32_t selected = CONFIG_NO_NODE;
        for (uint32_t child = node.first_child; child != CONFIG_NO_NODE;
             child = schema->nodes[child].next_sibling) {
            const s_schema_node_descriptor& candidate = schema->nodes[child];
            if (variable_id >= candidate.first_variable + variable_offset &&
                variable_id <= candidate.last_variable + variable_offset) {
                selected = child;
                break;
            }
        }
        node_index = selected;
    }
    return false;
}

const s_type_descriptor* Get_Variable_Type(t_variable_id variable_id) {
    const s_schema_node_descriptor* node =
        Get_Variable_Node(Get_Runtime_Schema(), variable_id);
    return node != nullptr ? node->type : nullptr;
}

bool Find_Variable(const char* name, size_t name_length, t_variable_id* variable_id) {
    if (name == nullptr || variable_id == nullptr) {
        return false;
    }
    const std::string_view requested(name, name_length);
    size_t offset = 0;
    uint32_t selected = CONFIG_NO_NODE;
    t_variable_id variable_offset = 0;
    while (offset < requested.size()) {
        const size_t separator = requested.find('.', offset);
        const size_t end = separator == std::string_view::npos ? requested.size() : separator;
        const std::string_view component = requested.substr(offset, end - offset);
        if (component.empty() ||
            (separator != std::string_view::npos && separator + 1 == requested.size())) {
            return false;
        }
        uint32_t match = CONFIG_NO_NODE;
        if (selected == CONFIG_NO_NODE) {
            for (uint32_t index = 0; index < schema_node_count; ++index) {
                const s_schema_node_descriptor& node = schema_nodes[index];
                if (node.parent == CONFIG_NO_NODE && component == node.id) {
                    match = index;
                    break;
                }
            }
        } else {
            const s_schema_node_descriptor& parent = schema_nodes[selected];
            int32_t external_index = 0;
            if (parent.kind == CONFIG_NODE_ARRAY) {
                const char* begin = component.data();
                const char* component_end = begin + component.size();
                const std::from_chars_result parsed =
                    std::from_chars(begin, component_end, external_index);
                if (parsed.ec != std::errc() || parsed.ptr != component_end ||
                    std::to_string(external_index) != component) {
                    return false;
                }
                if (parent.array_element_count != 0 &&
                    parent.array_element_variable_count == 0) {
                    const int64_t offset = static_cast<int64_t>(external_index) -
                                           parent.array_index_base;
                    if (offset < 0 || offset >= parent.array_element_count ||
                        separator != std::string_view::npos) {
                        return false;
                    }
                    *variable_id = parent.first_variable + variable_offset +
                        static_cast<t_variable_id>(offset);
                    return true;
                }
                if (parent.array_element_variable_count != 0) {
                    const int64_t offset = static_cast<int64_t>(external_index) -
                                           parent.array_index_base;
                    if (offset < 0 || offset >= parent.array_element_count) return false;
                    variable_offset += static_cast<t_variable_id>(offset) *
                                       parent.array_element_variable_count;
                    match = parent.first_child;
                }
            }
            if (match != CONFIG_NO_NODE) {
                selected = match;
                offset = end + 1;
                continue;
            }
            for (uint32_t child = parent.first_child; child != CONFIG_NO_NODE;
                 child = schema_nodes[child].next_sibling) {
                const s_schema_node_descriptor& node = schema_nodes[child];
                if ((parent.kind == CONFIG_NODE_ARRAY &&
                     node.external_index == external_index) ||
                    (parent.kind != CONFIG_NODE_ARRAY && component == node.id)) {
                    match = child;
                    break;
                }
            }
        }
        if (match == CONFIG_NO_NODE) {
            return false;
        }
        selected = match;
        offset = end + 1;
    }
    if (selected == CONFIG_NO_NODE ||
        schema_nodes[selected].kind != CONFIG_NODE_SCALAR) {
        return false;
    }
    *variable_id = schema_nodes[selected].variable_id + variable_offset;
    return true;
}

bool Get_Variable_Name(const s_runtime_schema* schema, t_variable_id variable_id,
                       std::string* name) {
    if (name == nullptr) {
        return false;
    }
    name->clear();
    uint32_t node_index = CONFIG_NO_NODE;
    for (uint32_t index = 0; index < schema->node_count; ++index) {
        const s_schema_node_descriptor& node = schema->nodes[index];
        if (node.parent == CONFIG_NO_NODE && variable_id >= node.first_variable &&
            variable_id <= node.last_variable) {
            node_index = index;
            break;
        }
    }
    t_variable_id variable_offset = 0;
    while (node_index != CONFIG_NO_NODE) {
        const s_schema_node_descriptor& node = schema->nodes[node_index];
        if (node.id[0] != '\0') {
            if (!name->empty()) name->push_back('.');
            *name += node.id;
        }
        if (node.kind == CONFIG_NODE_SCALAR) {
            return node.variable_id + variable_offset == variable_id;
        }
        if (node.kind == CONFIG_NODE_ARRAY) {
            if (node.array_element_count != 0) {
                const uint32_t stride = node.array_element_variable_count == 0 ? 1 :
                                        node.array_element_variable_count;
                const uint32_t element =
                    (variable_id - node.first_variable - variable_offset) / stride;
                if (!name->empty()) name->push_back('.');
                *name += std::to_string(node.array_index_base + element);
                if (node.array_element_variable_count == 0) return true;
                variable_offset += element * stride;
                node_index = node.first_child;
                continue;
            }
            uint32_t element = CONFIG_NO_NODE;
            for (uint32_t child = node.first_child; child != CONFIG_NO_NODE;
                 child = schema->nodes[child].next_sibling) {
                const s_schema_node_descriptor& candidate = schema->nodes[child];
                if (variable_id >= candidate.first_variable + variable_offset &&
                    variable_id <= candidate.last_variable + variable_offset) {
                    element = child;
                    break;
                }
            }
            if (element == CONFIG_NO_NODE) return false;
            if (!name->empty()) name->push_back('.');
            *name += std::to_string(schema->nodes[element].external_index);
            node_index = element;
            continue;
        }
        uint32_t selected = CONFIG_NO_NODE;
        for (uint32_t child = node.first_child; child != CONFIG_NO_NODE;
             child = schema->nodes[child].next_sibling) {
            const s_schema_node_descriptor& candidate = schema->nodes[child];
            if (variable_id >= candidate.first_variable + variable_offset &&
                variable_id <= candidate.last_variable + variable_offset) {
                selected = child;
                break;
            }
        }
        node_index = selected;
    }
    return false;
}

bool Get_Variable_Name(t_variable_id variable_id, std::string* name) {
    return Get_Variable_Name(Get_Runtime_Schema(), variable_id, name);
}

bool Resolve_Write_Path(s_config_write* write, s_cfg_diagnostic* diagnostic) {
    if (write == nullptr || diagnostic == nullptr) {
        return false;
    }
    t_variable_id variable_id = 0;
    if (!Find_Variable(write->canonical_path.data(), write->canonical_path.size(), &variable_id)) {
        diagnostic->location = write->provenance;
        diagnostic->message = "unknown configuration variable '" + write->canonical_path + "'";
        return false;
    }
    write->variable_id = variable_id;
    return true;
}

bool Initialize_Defaults(s_config_state* state) {
    if (state == nullptr) {
        return false;
    }
    state->values.clear();
    state->values.resize(variable_count);
    for (t_variable_id variable_id = 0; variable_id < variable_count; ++variable_id) {
        if (!generated::Get_Default_Value(variable_id, &state->values[variable_id])) {
            state->values.clear();
            return false;
        }
    }
    return true;
}

bool Read_Current_State(s_config_state* state) {
    if (state == nullptr) {
        return false;
    }
    state->values.clear();
    state->values.resize(variable_count);
    for (t_variable_id variable_id = 0; variable_id < variable_count; ++variable_id) {
        if (!generated::Get_Current_Value(variable_id, &state->values[variable_id])) {
            state->values.clear();
            return false;
        }
    }
    return true;
}

bool Read_State_Value(const s_config_state* state, t_variable_id variable_id,
                      s_typed_value* value) {
    if (state == nullptr || value == nullptr || variable_id >= state->values.size()) {
        return false;
    }
    *value = state->values[variable_id];
    return true;
}

bool Apply_Write_Set(const s_config_write_set* write_set, s_config_state* state,
                     s_cfg_diagnostic* diagnostic) {
    if (write_set == nullptr || state == nullptr || diagnostic == nullptr ||
        state->values.size() != variable_count) {
        return false;
    }
    *diagnostic = {};
    for (const s_config_write& write : write_set->writes) {
        if (!Validate_Write_(&write, diagnostic)) {
            return false;
        }
    }
    for (const s_config_write& write : write_set->writes) {
        state->values[write.variable_id] = write.value;
    }
    return true;
}

bool Commit_State(const s_config_state* state) {
    if (state == nullptr || state->values.size() != variable_count) {
        return false;
    }
    for (t_variable_id variable_id = 0; variable_id < variable_count; ++variable_id) {
        const s_type_descriptor* type = Get_Variable_Type(variable_id);
        if (type == nullptr) {
            return false;
        }
        if (state->values[variable_id].type != type->value_type) {
            return false;
        }
        if (type->value_type == CONFIG_VALUE_BITMAP &&
            state->values[variable_id].bitmap_value.size() !=
                (type->bitmap_bit_count + 7) / 8) {
            return false;
        }
    }
    for (uint32_t node_index = 0; node_index < schema_node_count; ++node_index) {
        if (schema_nodes[node_index].parent == CONFIG_NO_NODE &&
            !Validate_Node_Terminators_(node_index, 0, state)) {
            return false;
        }
    }
    for (t_variable_id variable_id = 0; variable_id < variable_count; ++variable_id) {
        generated::Set_Value(variable_id, &state->values[variable_id]);
    }
    return true;
}

namespace generated {

void Set_Value(t_variable_id variable_id, const s_typed_value* value) {
    s_resolved_variable variable = {};
    if (value == nullptr || !Resolve_Variable(variable_id, &variable) ||
        value->type != variable.type->value_type) {
        return;
    }
    if (value->type == CONFIG_VALUE_STRING) {
        for (size_t index = 0; index < variable.target_count; ++index) {
            const s_variable_target& target = variable.targets[index];
            if (target.size == 0) continue;
            const size_t copy_size = std::min(value->string_value.size(), target.size - 1);
            std::memcpy(target.address, value->string_value.data(), copy_size);
            ((char*)target.address)[copy_size] = '\0';
        }
        return;
    }
    if (value->type == CONFIG_VALUE_BITMAP) {
        for (size_t index = 0; index < variable.target_count; ++index) {
            const s_variable_target& target = variable.targets[index];
            if (target.size == value->bitmap_value.size()) {
                std::memcpy(target.address, value->bitmap_value.data(), target.size);
            }
        }
        return;
    }
    if (value->type == CONFIG_VALUE_RATIO) {
        if (variable.target_count == 2) {
            Write_Integer_(variable.targets[0].address, variable.targets[0].size,
                           variable.type->ratio_component_type, value->ratio_numerator);
            Write_Integer_(variable.targets[1].address, variable.targets[1].size,
                           variable.type->ratio_component_type, value->ratio_denominator);
        }
        return;
    }
    const int64_t integer = value->type == CONFIG_VALUE_BOOL ?
        (value->bool_value ? 1 : 0) : value->integer_value;
    for (size_t index = 0; index < variable.target_count; ++index) {
        Write_Integer_(variable.targets[index].address, variable.targets[index].size,
                       value->type, integer);
    }
}

bool Get_Current_Value(t_variable_id variable_id, s_typed_value* value) {
    s_resolved_variable variable = {};
    if (value == nullptr || !Resolve_Variable(variable_id, &variable) ||
        variable.target_count == 0) {
        return false;
    }
    *value = {};
    value->type = variable.type->value_type;
    const s_variable_target& target = variable.targets[0];
    switch (value->type) {
    case CONFIG_VALUE_BOOL: {
        int64_t integer = 0;
        if (!Read_Integer_(target.address, value->type, &integer)) return false;
        value->bool_value = integer != 0;
        return true;
    }
    case CONFIG_VALUE_INT8:
    case CONFIG_VALUE_UINT8:
    case CONFIG_VALUE_INT16:
    case CONFIG_VALUE_UINT16:
    case CONFIG_VALUE_INT32:
    case CONFIG_VALUE_UINT32:
        return Read_Integer_(target.address, value->type, &value->integer_value);
    case CONFIG_VALUE_STRING:
        value->string_value = (const char*)target.address;
        return true;
    case CONFIG_VALUE_BITMAP: {
        const uint8_t* bytes = (const uint8_t*)target.address;
        value->bitmap_value.assign(bytes, bytes + target.size);
        return true;
    }
    case CONFIG_VALUE_RATIO:
        if (variable.target_count != 2) return false;
        return Read_Integer_(variable.targets[0].address,
                             variable.type->ratio_component_type,
                             &value->ratio_numerator) &&
               Read_Integer_(variable.targets[1].address,
                             variable.type->ratio_component_type,
                             &value->ratio_denominator);
    }
    return false;
}

}

}