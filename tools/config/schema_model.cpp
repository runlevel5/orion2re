#include "schema_model.h"

#include <climits>

#include "json5_reader.h"

namespace config::tool {
namespace {
    const s_json_value* Member_(const s_json_value* object, const char* name) {
        if (object->kind != JSON_OBJECT) {
            return nullptr;
        }
        for (size_t index = 0; index < object->member_names.size(); ++index) {
            if (object->member_names[index] == name) {
                return &object->values[index];
            }
        }
        return nullptr;
    }

    bool Has_Only_Members_(const s_json_value* object, const std::vector<std::string>& allowed,
                           const std::string& path, std::string* diagnostic) {
        if (object->kind != JSON_OBJECT) {
            *diagnostic = path + ": expected an object";
            return false;
        }
        for (const std::string& name : object->member_names) {
            bool found = false;
            for (const std::string& allowed_name : allowed) {
                if (name == allowed_name) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                *diagnostic = path + ": unknown property '" + name + "'";
                return false;
            }
        }
        return true;
    }

    bool Required_String_(const s_json_value* object, const char* name, const std::string& path,
                          std::string* output, std::string* diagnostic) {
        const s_json_value* value = Member_(object, name);
        if (value == nullptr || value->kind != JSON_STRING || value->string_value.empty()) {
            *diagnostic = path + "." + name + ": expected a non-empty string";
            return false;
        }
        *output = value->string_value;
        return true;
    }

    bool Optional_String_(const s_json_value* object, const char* name, const std::string& path,
                          std::string* output, std::string* diagnostic) {
        const s_json_value* value = Member_(object, name);
        if (value == nullptr) {
            return true;
        }
        if (value->kind != JSON_STRING) {
            *diagnostic = path + "." + name + ": expected a string";
            return false;
        }
        *output = value->string_value;
        return true;
    }

    bool Required_Bool_(const s_json_value* object, const char* name, const std::string& path,
                        bool* output, std::string* diagnostic) {
        const s_json_value* value = Member_(object, name);
        if (value == nullptr || value->kind != JSON_BOOL) {
            *diagnostic = path + "." + name + ": expected a boolean";
            return false;
        }
        *output = value->bool_value;
        return true;
    }

    bool Integer_(const s_json_value* value, int64_t* output) {
        if (value->kind == JSON_SIGNED) {
            *output = value->signed_value;
            return true;
        }
        if (value->kind == JSON_UNSIGNED && value->unsigned_value <= INT64_MAX) {
            *output = static_cast<int64_t>(value->unsigned_value);
            return true;
        }
        return false;
    }

    bool Decode_Utf8_(const std::string& text, size_t* offset, uint32_t* codepoint) {
        const uint8_t first = static_cast<uint8_t>(text[*offset]);
        if (first < 0x80) {
            *codepoint = first;
            ++*offset;
            return true;
        }

        size_t count = 0;
        uint32_t value = 0;
        if ((first & 0xE0) == 0xC0) {
            count = 2;
            value = first & 0x1F;
        } else if ((first & 0xF0) == 0xE0) {
            count = 3;
            value = first & 0x0F;
        } else if ((first & 0xF8) == 0xF0) {
            count = 4;
            value = first & 0x07;
        } else {
            return false;
        }
        if (*offset + count > text.size()) {
            return false;
        }
        for (size_t index = 1; index < count; ++index) {
            const uint8_t next = static_cast<uint8_t>(text[*offset + index]);
            if ((next & 0xC0) != 0x80) {
                return false;
            }
            value = value << 6 | (next & 0x3F);
        }
        *offset += count;
        *codepoint = value;
        return true;
    }

    bool Cp850_Byte_(uint32_t codepoint, uint8_t* byte) {
        static const uint16_t extended[] = {
            0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7,
            0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
            0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9,
            0x00FF, 0x00D6, 0x00DC, 0x00F8, 0x00A3, 0x00D8, 0x00D7, 0x0192,
            0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA,
            0x00BF, 0x00AE, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
            0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x00C1, 0x00C2, 0x00C0,
            0x00A9, 0x2563, 0x2551, 0x2557, 0x255D, 0x00A2, 0x00A5, 0x2510,
            0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x00E3, 0x00C3,
            0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x00A4,
            0x00F0, 0x00D0, 0x00CA, 0x00CB, 0x00C8, 0x0131, 0x00CD, 0x00CE,
            0x00CF, 0x2518, 0x250C, 0x2588, 0x2584, 0x00A6, 0x00CC, 0x2580,
            0x00D3, 0x00DF, 0x00D4, 0x00D2, 0x00F5, 0x00D5, 0x00B5, 0x00FE,
            0x00DE, 0x00DA, 0x00DB, 0x00D9, 0x00FD, 0x00DD, 0x00AF, 0x00B4,
            0x00AD, 0x00B1, 0x2017, 0x00BE, 0x00B6, 0x00A7, 0x00F7, 0x00B8,
            0x00B0, 0x00A8, 0x00B7, 0x00B9, 0x00B3, 0x00B2, 0x25A0, 0x00A0,
        };
        if (codepoint < 0x80) {
            *byte = static_cast<uint8_t>(codepoint);
            return true;
        }
        for (size_t index = 0; index < sizeof(extended) / sizeof(extended[0]); ++index) {
            if (extended[index] == codepoint) {
                *byte = static_cast<uint8_t>(0x80 + index);
                return true;
            }
        }
        return false;
    }

    bool Canonical_Name_(const std::string& name) {
        if (name.empty() || name.front() < 'a' || name.front() > 'z' || name.back() == '_') {
            return false;
        }
        bool underscore = false;
        for (char character : name) {
            if (character == '_') {
                if (underscore) {
                    return false;
                }
                underscore = true;
            } else if ((character < 'a' || character > 'z') &&
                       (character < '0' || character > '9')) {
                return false;
            } else {
                underscore = false;
            }
        }
        return true;
    }

    size_t Encoded_Length_(const std::string& value) {
        const std::string encoded = Encode_Game_String(value);
        if (!value.empty() && encoded.empty()) {
            return SIZE_MAX;
        }
        return encoded.size();
    }

    bool Metadata_(const s_json_value* object, const std::string& path, s_metadata* metadata,
                   std::string* diagnostic) {
        if (!Required_String_(object, "title", path, &metadata->title, diagnostic) ||
            !Required_String_(object, "description", path, &metadata->description, diagnostic) ||
            !Optional_String_(object, "remarks", path, &metadata->remarks, diagnostic) ||
            !Optional_String_(object, "units", path, &metadata->units, diagnostic)) {
            return false;
        }
        const s_json_value* examples = Member_(object, "examples");
        if (examples != nullptr) {
            if (examples->kind != JSON_ARRAY) {
                *diagnostic = path + ".examples: expected an array";
                return false;
            }
            for (const s_json_value& example : examples->values) {
                if (example.kind != JSON_STRING) {
                    *diagnostic = path + ".examples: expected strings";
                    return false;
                }
                metadata->examples.push_back(example.string_value);
            }
        }
        return true;
    }

    bool Primitive_(const std::string& name, e_primitive_type* type) {
        const char* names[] = {"bool", "int8", "uint8", "int16", "uint16", "int32", "uint32", "string"};
        for (size_t index = 0; index < sizeof(names) / sizeof(names[0]); ++index) {
            if (name == names[index]) {
                *type = static_cast<e_primitive_type>(index);
                return true;
            }
        }
        return false;
    }

    void Primitive_Range_(e_primitive_type type, int64_t* minimum, int64_t* maximum) {
        switch (type) {
        case PRIMITIVE_INT8: *minimum = INT8_MIN; *maximum = INT8_MAX; break;
        case PRIMITIVE_UINT8: *minimum = 0; *maximum = UINT8_MAX; break;
        case PRIMITIVE_INT16: *minimum = INT16_MIN; *maximum = INT16_MAX; break;
        case PRIMITIVE_UINT16: *minimum = 0; *maximum = UINT16_MAX; break;
        case PRIMITIVE_INT32: *minimum = INT32_MIN; *maximum = INT32_MAX; break;
        case PRIMITIVE_UINT32: *minimum = 0; *maximum = UINT32_MAX; break;
        default: *minimum = 0; *maximum = 0; break;
        }
    }

    bool Constraints_(const s_json_value* object, const std::string& path,
                      e_primitive_type type, s_constraints* constraints, std::string* diagnostic) {
        if (object == nullptr) {
            return true;
        }
        if (!Has_Only_Members_(object, {"min", "max", "min_length", "max_length", "encoding"}, path, diagnostic)) {
            return false;
        }
        const s_json_value* min = Member_(object, "min");
        const s_json_value* max = Member_(object, "max");
        const s_json_value* min_length = Member_(object, "min_length");
        const s_json_value* max_length = Member_(object, "max_length");
        if (type == PRIMITIVE_STRING) {
            int64_t value = 0;
            if (min != nullptr || max != nullptr) {
                *diagnostic = path + ": numeric constraints cannot be used with string";
                return false;
            }
            if (min_length != nullptr) {
                if (!Integer_(min_length, &value) || value < 0) {
                    *diagnostic = path + ".min_length: expected a non-negative integer";
                    return false;
                }
                constraints->has_min_length = true;
                constraints->min_length = static_cast<size_t>(value);
            }
            if (max_length != nullptr) {
                if (!Integer_(max_length, &value) || value < 0) {
                    *diagnostic = path + ".max_length: expected a non-negative integer";
                    return false;
                }
                constraints->has_max_length = true;
                constraints->max_length = static_cast<size_t>(value);
            }
            if (!Optional_String_(object, "encoding", path, &constraints->encoding, diagnostic)) {
                return false;
            }
            if (constraints->encoding.empty()) {
                constraints->encoding = "cp850";
            } else if (constraints->encoding != "cp850") {
                *diagnostic = path + ".encoding: only 'cp850' is supported";
                return false;
            }
            if (constraints->has_min_length && constraints->has_max_length &&
                constraints->min_length > constraints->max_length) {
                *diagnostic = path + ": min_length exceeds max_length";
                return false;
            }
            return true;
        }

        if (min_length != nullptr || max_length != nullptr || Member_(object, "encoding") != nullptr) {
            *diagnostic = path + ": string constraints require type 'string'";
            return false;
        }
        if (type == PRIMITIVE_BOOL && (min != nullptr || max != nullptr)) {
            *diagnostic = path + ": bool does not accept numeric constraints";
            return false;
        }
        int64_t primitive_min = 0;
        int64_t primitive_max = 0;
        Primitive_Range_(type, &primitive_min, &primitive_max);
        if (min != nullptr) {
            if (!Integer_(min, &constraints->min) || constraints->min < primitive_min || constraints->min > primitive_max) {
                *diagnostic = path + ".min: value is outside the primitive range";
                return false;
            }
            constraints->has_min = true;
        }
        if (max != nullptr) {
            if (!Integer_(max, &constraints->max) || constraints->max < primitive_min || constraints->max > primitive_max) {
                *diagnostic = path + ".max: value is outside the primitive range";
                return false;
            }
            constraints->has_max = true;
        }
        if (constraints->has_min && constraints->has_max && constraints->min > constraints->max) {
            *diagnostic = path + ": min exceeds max";
            return false;
        }
        return true;
    }

    bool Binding_(const s_json_value* parameter, const std::string& path, s_binding* binding,
                  std::string* diagnostic) {
        const s_json_value* root = Member_(parameter, "bind_root");
        const s_json_value* members = Member_(parameter, "bind");
        if (root == nullptr || !Has_Only_Members_(root, {"header", "symbol", "cpp_type"}, path + ".bind_root", diagnostic) ||
            !Required_String_(root, "header", path + ".bind_root", &binding->header, diagnostic) ||
            !Required_String_(root, "symbol", path + ".bind_root", &binding->symbol, diagnostic) ||
            !Required_String_(root, "cpp_type", path + ".bind_root", &binding->cpp_type, diagnostic)) {
            return false;
        }
        if (members == nullptr || members->kind != JSON_ARRAY || members->values.empty()) {
            *diagnostic = path + ".bind: expected a non-empty member-path array";
            return false;
        }
        for (const s_json_value& member : members->values) {
            if (member.kind != JSON_STRING || member.string_value.empty()) {
                *diagnostic = path + ".bind: expected non-empty member names";
                return false;
            }
            binding->members.push_back(member.string_value);
        }
        if (binding->members.size() != 1) {
            *diagnostic = path + ".bind: the primitive bootstrap supports one direct native member";
            return false;
        }
        return true;
    }

    uint64_t Schema_Id_(const std::string& path) {
        uint64_t hash = UINT64_C(14695981039346656037);
        for (uint8_t byte : path) {
            hash ^= byte;
            hash *= UINT64_C(1099511628211);
        }
        return hash;
    }

    bool Group_(const std::string& id, const s_json_value* value, const std::string& root,
                s_group* group, std::string* diagnostic) {
        const std::string path = root + ".groups." + id;
        if (!Canonical_Name_(id)) {
            *diagnostic = path + ": group names must use lower_snake_case";
            return false;
        }
        if (!Has_Only_Members_(value, {"title", "description", "remarks", "units", "examples", "save", "synchronize"}, path, diagnostic) ||
            !Metadata_(value, path, &group->metadata, diagnostic) ||
            !Required_Bool_(value, "save", path, &group->save, diagnostic) ||
            !Required_Bool_(value, "synchronize", path, &group->synchronize, diagnostic)) {
            return false;
        }
        group->id = id;
        return true;
    }

    bool Parameter_(const std::string& id, const s_json_value* value, const std::string& root,
                    s_parameter* parameter, std::string* diagnostic) {
        const std::string path = root + ".parameters." + id;
        if (!Canonical_Name_(id)) {
            *diagnostic = path + ": parameter names must use lower_snake_case";
            return false;
        }
        if (!Has_Only_Members_(value, {"title", "description", "remarks", "units", "examples", "group", "type", "constraints", "default", "bind_root", "bind"}, path, diagnostic) ||
            !Metadata_(value, path, &parameter->metadata, diagnostic) ||
            !Required_String_(value, "group", path, &parameter->group, diagnostic)) {
            return false;
        }
        std::string type_name;
        if (!Required_String_(value, "type", path, &type_name, diagnostic) ||
            !Primitive_(type_name, &parameter->type)) {
            *diagnostic = path + ".type: expected a built-in explicit-width primitive";
            return false;
        }
        if (!Constraints_(Member_(value, "constraints"), path + ".constraints", parameter->type,
                          &parameter->constraints, diagnostic) ||
            !Binding_(value, path, &parameter->binding, diagnostic)) {
            return false;
        }

        const s_json_value* default_value = Member_(value, "default");
        if (default_value == nullptr) {
            *diagnostic = path + ".default: every writable parameter requires a default";
            return false;
        }
        if (parameter->type == PRIMITIVE_BOOL) {
            if (default_value->kind != JSON_BOOL) {
                *diagnostic = path + ".default: expected a boolean";
                return false;
            }
            parameter->default_bool = default_value->bool_value;
        } else if (parameter->type == PRIMITIVE_STRING) {
            if (default_value->kind != JSON_STRING) {
                *diagnostic = path + ".default: expected a string";
                return false;
            }
            const size_t length = Encoded_Length_(default_value->string_value);
            if (length == SIZE_MAX) {
                *diagnostic = path + ".default: string contains text not representable in CP850";
                return false;
            }
            if ((parameter->constraints.has_min_length && length < parameter->constraints.min_length) ||
                (parameter->constraints.has_max_length && length > parameter->constraints.max_length)) {
                *diagnostic = path + ".default: CP850 byte length violates constraints";
                return false;
            }
            parameter->default_string = default_value->string_value;
        } else {
            int64_t primitive_min = 0;
            int64_t primitive_max = 0;
            if (!Integer_(default_value, &parameter->default_integer)) {
                *diagnostic = path + ".default: expected an integer";
                return false;
            }
            Primitive_Range_(parameter->type, &primitive_min, &primitive_max);
            const int64_t minimum = parameter->constraints.has_min ? parameter->constraints.min : primitive_min;
            const int64_t maximum = parameter->constraints.has_max ? parameter->constraints.max : primitive_max;
            if (parameter->default_integer < minimum || parameter->default_integer > maximum) {
                *diagnostic = path + ".default: integer violates type or field constraints";
                return false;
            }
        }

        parameter->path = id;
        parameter->schema_id = Schema_Id_(id);
        return true;
    }
}

std::string Encode_Game_String(const std::string& text) {
    std::string encoded;
    size_t offset = 0;
    while (offset < text.size()) {
        uint32_t codepoint = 0;
        uint8_t byte = 0;
        if (!Decode_Utf8_(text, &offset, &codepoint) || !Cp850_Byte_(codepoint, &byte)) {
            return {};
        }
        encoded.push_back(static_cast<char>(byte));
    }
    return encoded;
}

const char* Primitive_Name(e_primitive_type type) {
    static const char* names[] = {"bool", "int8", "uint8", "int16", "uint16", "int32", "uint32", "string"};
    return names[type];
}

bool Load_And_Validate_Schema(const char* path, s_schema* schema, std::string* diagnostic) {
    s_json_value root;
    if (!Read_Json5_File(path, &root, diagnostic)) {
        return false;
    }
    const std::string schema_path(path);
    if (!Has_Only_Members_(&root, {"groups", "types", "records", "parameters"}, schema_path, diagnostic)) {
        return false;
    }
    const s_json_value* groups = Member_(&root, "groups");
    const s_json_value* types = Member_(&root, "types");
    const s_json_value* records = Member_(&root, "records");
    const s_json_value* parameters = Member_(&root, "parameters");
    if (groups == nullptr || groups->kind != JSON_OBJECT || groups->values.empty()) {
        *diagnostic = schema_path + ".groups: expected a non-empty object";
        return false;
    }
    if (types == nullptr || types->kind != JSON_OBJECT || !types->values.empty()) {
        *diagnostic = schema_path + ".types: phase-one bootstrap requires an empty object";
        return false;
    }
    if (records == nullptr || records->kind != JSON_OBJECT || !records->values.empty()) {
        *diagnostic = schema_path + ".records: phase-one bootstrap requires an empty object";
        return false;
    }
    if (parameters == nullptr || parameters->kind != JSON_OBJECT || parameters->values.empty()) {
        *diagnostic = schema_path + ".parameters: expected a non-empty object";
        return false;
    }

    for (size_t index = 0; index < groups->member_names.size(); ++index) {
        schema->groups.emplace_back();
        if (!Group_(groups->member_names[index], &groups->values[index], schema_path,
                    &schema->groups.back(), diagnostic)) {
            return false;
        }
    }
    for (size_t index = 0; index < parameters->member_names.size(); ++index) {
        schema->parameters.emplace_back();
        if (!Parameter_(parameters->member_names[index], &parameters->values[index], schema_path,
                        &schema->parameters.back(), diagnostic)) {
            return false;
        }
        bool group_found = false;
        for (const s_group& group : schema->groups) {
            if (group.id == schema->parameters.back().group) {
                group_found = true;
                break;
            }
        }
        if (!group_found) {
            *diagnostic = schema_path + ".parameters." + parameters->member_names[index] +
                          ".group: unknown group '" + schema->parameters.back().group + "'";
            return false;
        }
        for (size_t prior = 0; prior + 1 < schema->parameters.size(); ++prior) {
            if (schema->parameters[prior].schema_id == schema->parameters.back().schema_id) {
                *diagnostic = schema_path + ".parameters: canonical-path schema ID collision";
                return false;
            }
        }
    }
    return true;
}

}