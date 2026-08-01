#include "config_runtime.h"

#include <climits>
#include <cstring>
#include <utility>

#include "config/config_decode.h"

namespace config {
namespace {
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
        const s_variable_descriptor* variable = Get_Variable(write->variable_id);
        if (variable == nullptr) {
            return Fail_(write, "unknown typed write variable", diagnostic);
        }
        if (write->value.type != variable->type) {
            diagnostic->location = write->provenance;
            diagnostic->message = "typed write mismatch for " + std::string(variable->name);
            return false;
        }
        if (variable->type == CONFIG_VALUE_BOOL || variable->type == CONFIG_VALUE_STRING) {
            return true;
        }

        int64_t primitive_minimum = 0;
        int64_t primitive_maximum = 0;
        Primitive_Range_(variable->type, &primitive_minimum, &primitive_maximum);
        const int64_t minimum = variable->has_min ? variable->min : primitive_minimum;
        const int64_t maximum = variable->has_max ? variable->max : primitive_maximum;
        if (write->value.integer_value < minimum || write->value.integer_value > maximum) {
            diagnostic->location = write->provenance;
            diagnostic->message = "typed write range error for " + std::string(variable->name);
            return false;
        }
        return true;
    }
}

const s_variable_descriptor* Get_Variable(t_variable_id variable_id) {
    return variable_id < variable_count ? &variables[variable_id] : nullptr;
}

bool Find_Variable(const char* name, size_t name_length, t_variable_id* variable_id) {
    if (name == nullptr || variable_id == nullptr) {
        return false;
    }
    for (size_t index = 0; index < variable_count; ++index) {
        if (std::strlen(variables[index].name) == name_length &&
            std::memcmp(variables[index].name, name, name_length) == 0) {
            *variable_id = (t_variable_id)index;
            return true;
        }
    }
    return false;
}

const char* Get_Variable_Name(t_variable_id variable_id) {
    const s_variable_descriptor* variable = Get_Variable(variable_id);
    return variable != nullptr ? variable->name : nullptr;
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

void Initialize_Defaults(s_config_state* state) {
    state->values.clear();
    state->values.resize(variable_count);
    for (t_variable_id variable_id = 0; variable_id < variable_count; ++variable_id) {
        if (!generated::Get_Default_Value(variable_id, &state->values[variable_id])) {
            state->values.clear();
            return;
        }
    }
}

void Read_Current_State(s_config_state* state) {
    state->values.clear();
    state->values.resize(variable_count);
    for (t_variable_id variable_id = 0; variable_id < variable_count; ++variable_id) {
        if (!generated::Get_Current_Value(variable_id, &state->values[variable_id])) {
            state->values.clear();
            return;
        }
    }
}

bool Read_State_Value(const s_config_state* state, t_variable_id variable_id,
                      s_typed_value* value) {
    if (state == nullptr || value == nullptr || variable_id >= state->values.size()) {
        return false;
    }
    *value = state->values[variable_id];
    return true;
}

bool Apply_Write_Set(const s_config_write_set* write_set,
                     const s_config_state* base_state,
                     s_config_state* candidate_state,
                     s_cfg_diagnostic* diagnostic) {
    if (write_set == nullptr || base_state == nullptr || candidate_state == nullptr ||
        diagnostic == nullptr || base_state->values.size() != variable_count) {
        return false;
    }
    *diagnostic = {};
    s_config_state temporary = *base_state;
    for (const s_config_write& write : write_set->writes) {
        if (!Validate_Write_(&write, diagnostic)) {
            return false;
        }
        temporary.values[write.variable_id] = write.value;
    }
    *candidate_state = std::move(temporary);
    return true;
}

bool Apply_Transaction(const s_config_write_set* write_set,
                       s_cfg_diagnostic* diagnostic) {
    s_config_state base_state;
    s_config_state candidate_state;
    Read_Current_State(&base_state);
    if (!Apply_Write_Set(write_set, &base_state, &candidate_state, diagnostic)) {
        return false;
    }
    return Commit_State(&candidate_state);
}

bool Commit_State(const s_config_state* state) {
    if (state == nullptr || state->values.size() != variable_count) {
        return false;
    }
    for (t_variable_id variable_id = 0; variable_id < variable_count; ++variable_id) {
        if (state->values[variable_id].type != variables[variable_id].type) {
            return false;
        }
    }
    for (t_variable_id variable_id = 0; variable_id < variable_count; ++variable_id) {
        if (!generated::Set_Value(variable_id, &state->values[variable_id])) {
            return false;
        }
    }
    return true;
}

}