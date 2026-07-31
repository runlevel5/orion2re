#include "config_decode.h"

#include <climits>
#include <utility>

namespace config {
namespace {
    bool Fail_(const s_source_location& location, const std::string& message,
               s_config_write_set* write_set, s_cfg_diagnostic* diagnostic) {
        write_set->writes.clear();
        diagnostic->location = location;
        diagnostic->message = message;
        return false;
    }

    const char* Type_Name_(generated::e_value_type type) {
        switch (type) {
        case generated::CONFIG_VALUE_BOOL: return "bool";
        case generated::CONFIG_VALUE_INT8: return "int8";
        case generated::CONFIG_VALUE_UINT8: return "uint8";
        case generated::CONFIG_VALUE_INT16: return "int16";
        case generated::CONFIG_VALUE_UINT16: return "uint16";
        case generated::CONFIG_VALUE_INT32: return "int32";
        case generated::CONFIG_VALUE_UINT32: return "uint32";
        case generated::CONFIG_VALUE_STRING: return "string";
        }
        return "unknown";
    }

    void Primitive_Range_(generated::e_value_type type, int64_t* minimum, int64_t* maximum) {
        switch (type) {
        case generated::CONFIG_VALUE_INT8: *minimum = INT8_MIN; *maximum = INT8_MAX; break;
        case generated::CONFIG_VALUE_UINT8: *minimum = 0; *maximum = UINT8_MAX; break;
        case generated::CONFIG_VALUE_INT16: *minimum = INT16_MIN; *maximum = INT16_MAX; break;
        case generated::CONFIG_VALUE_UINT16: *minimum = 0; *maximum = UINT16_MAX; break;
        case generated::CONFIG_VALUE_INT32: *minimum = INT32_MIN; *maximum = INT32_MAX; break;
        case generated::CONFIG_VALUE_UINT32: *minimum = 0; *maximum = UINT32_MAX; break;
        default: *minimum = 0; *maximum = 0; break;
        }
    }

    int Digit_(char character, uint32_t base) {
        int value = -1;
        if (character >= '0' && character <= '9') {
            value = character - '0';
        } else if (character >= 'a' && character <= 'f') {
            value = character - 'a' + 10;
        } else if (character >= 'A' && character <= 'F') {
            value = character - 'A' + 10;
        }
        return value >= 0 && static_cast<uint32_t>(value) < base ? value : -1;
    }

    bool Parse_Integer_(const std::string& text, int64_t* value) {
        size_t offset = 0;
        bool negative = false;
        if (text[offset] == '+' || text[offset] == '-') {
            negative = text[offset] == '-';
            ++offset;
        }

        uint32_t base = 10;
        if (offset + 2 <= text.size() && text[offset] == '0' &&
            (text[offset + 1] == 'x' || text[offset + 1] == 'X')) {
            base = 16;
            offset += 2;
        } else if (offset + 2 <= text.size() && text[offset] == '0' &&
                   (text[offset + 1] == 'b' || text[offset + 1] == 'B')) {
            base = 2;
            offset += 2;
        }

        const uint64_t limit = negative ? UINT64_C(9223372036854775808) :
                                          UINT64_C(9223372036854775807);
        uint64_t magnitude = 0;
        for (; offset < text.size(); ++offset) {
            const int digit = Digit_(text[offset], base);
            if (digit < 0 || magnitude > (limit - static_cast<uint32_t>(digit)) / base) {
                return false;
            }
            magnitude = magnitude * base + static_cast<uint32_t>(digit);
        }
        if (negative && magnitude == UINT64_C(9223372036854775808)) {
            *value = INT64_MIN;
        } else if (negative) {
            *value = -static_cast<int64_t>(magnitude);
        } else {
            *value = static_cast<int64_t>(magnitude);
        }
        return true;
    }

    bool Decode_Bool_(const s_cfg_token& token, s_typed_value* value) {
        if (token.kind != CFG_TOKEN_IDENTIFIER) {
            return false;
        }
        if (token.value == "true") {
            value->bool_value = true;
            return true;
        }
        if (token.value == "false") {
            value->bool_value = false;
            return true;
        }
        return false;
    }

    bool Decode_Value_(const generated::s_parameter_descriptor* parameter,
                       const s_cfg_token& token, s_typed_value* value,
                       std::string* expected) {
        value->type = parameter->type;
        *expected = Type_Name_(parameter->type);
        if (parameter->type == generated::CONFIG_VALUE_BOOL) {
            return Decode_Bool_(token, value);
        }
        if (parameter->type == generated::CONFIG_VALUE_STRING) {
            if (token.kind != CFG_TOKEN_STRING) {
                return false;
            }
            value->string_value = token.value;
            return true;
        }
        if (token.kind != CFG_TOKEN_INTEGER || !Parse_Integer_(token.value, &value->integer_value)) {
            return false;
        }

        int64_t primitive_minimum = 0;
        int64_t primitive_maximum = 0;
        Primitive_Range_(parameter->type, &primitive_minimum, &primitive_maximum);
        const int64_t minimum = parameter->has_min ? parameter->min : primitive_minimum;
        const int64_t maximum = parameter->has_max ? parameter->max : primitive_maximum;
        if (value->integer_value < minimum || value->integer_value > maximum) {
            *expected += " in range " + std::to_string(minimum) + ".." + std::to_string(maximum);
            return false;
        }
        return true;
    }
}

bool Decode_Assignment(const s_cfg_statement* statement, s_config_write_set* write_set,
                       s_cfg_diagnostic* diagnostic) {
    if (statement == nullptr || write_set == nullptr || diagnostic == nullptr) {
        return false;
    }
    write_set->writes.clear();
    *diagnostic = {};
    if (statement->kind != CFG_STATEMENT_ASSIGNMENT) {
        return Fail_(statement->location, "expected an assignment statement", write_set, diagnostic);
    }
    if (statement->path.empty()) {
        return Fail_(statement->location, "assignment path is empty", write_set, diagnostic);
    }

    const s_cfg_token& root = statement->path.front();
    const generated::s_parameter_descriptor* parameter =
        generated::Find_Parameter(root.value.data(), root.value.size());
    if (parameter == nullptr) {
        return Fail_(root.location, "unknown configuration parameter '" + root.value + "'",
                     write_set, diagnostic);
    }
    if (statement->path.size() > 1) {
        return Fail_(statement->path[1].location,
                     "parameter '" + root.value + "' is a scalar and cannot be traversed",
                     write_set, diagnostic);
    }
    if (statement->values.empty()) {
        return Fail_(statement->location, "assignment requires a value", write_set, diagnostic);
    }
    if (statement->values.size() > 1) {
        return Fail_(statement->values[1].location,
                     "value 2 exceeds scalar parameter '" + root.value + "'",
                     write_set, diagnostic);
    }

    const s_cfg_token& token = statement->values.front();
    if (token.kind == CFG_TOKEN_SKIP) {
        return Fail_(token.location, "an assignment containing only skips has no effect",
                     write_set, diagnostic);
    }

    s_config_write write;
    write.parameter_id = parameter->id;
    write.canonical_path = parameter->path;
    write.provenance = token.location;
    std::string expected;
    if (!Decode_Value_(parameter, token, &write.value, &expected)) {
        return Fail_(token.location, "invalid value '" + token.raw + "' for '" +
                     root.value + "'; expected " + expected, write_set, diagnostic);
    }
    write_set->writes.push_back(std::move(write));
    return true;
}

}