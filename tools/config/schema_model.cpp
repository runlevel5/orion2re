#include "schema_model.h"

#include <climits>
#include <numeric>

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
        return true;
    }

    struct s_resolved_type {
        e_node_kind node_kind = NODE_SCALAR;
        e_primitive_type primitive = PRIMITIVE_BOOL;
        e_scalar_kind scalar_kind = SCALAR_PRIMITIVE;
        s_constraints constraints;
        std::vector<s_symbol> values;
        std::vector<s_symbol> flags;
        bool allow_numeric = false;
        bool reject_unknown_numeric = false;
        uint64_t known_bits = 0;
        e_primitive_type ratio_component_type = PRIMITIVE_INT16;
        const s_json_value* aggregate = nullptr;
    };

    bool Optional_Bool_(const s_json_value* object, const char* name, bool default_value,
                        const std::string& path, bool* output, std::string* diagnostic) {
        const s_json_value* value = Member_(object, name);
        if (value == nullptr) {
            *output = default_value;
            return true;
        }
        if (value->kind != JSON_BOOL) {
            *diagnostic = path + "." + name + ": expected a boolean";
            return false;
        }
        *output = value->bool_value;
        return true;
    }

    bool Find_Named_(const s_json_value* root, const char* collection,
                     const std::string& name, const s_json_value** value) {
        const s_json_value* object = Member_(root, collection);
        if (object == nullptr || object->kind != JSON_OBJECT) {
            return false;
        }
        *value = Member_(object, name.c_str());
        return *value != nullptr;
    }

    bool Symbols_(const s_json_value* object, const std::string& path,
                  std::vector<s_symbol>* symbols, uint64_t* known_bits,
                  std::string* diagnostic) {
        if (object == nullptr) {
            return true;
        }
        if (object->kind != JSON_OBJECT) {
            *diagnostic = path + ": expected an object";
            return false;
        }
        for (size_t index = 0; index < object->member_names.size(); ++index) {
            const std::string& name = object->member_names[index];
            if (!Canonical_Name_(name)) {
                *diagnostic = path + "." + name + ": names must use lower_snake_case";
                return false;
            }
            const s_json_value* value = &object->values[index];
            if (value->kind == JSON_OBJECT) {
                value = Member_(value, "value");
            }
            s_symbol symbol;
            symbol.name = name;
            if (value == nullptr || !Integer_(value, &symbol.value)) {
                *diagnostic = path + "." + name + ": expected an integer value";
                return false;
            }
            for (const s_symbol& existing : *symbols) {
                if (existing.value == symbol.value) {
                    *diagnostic = path + "." + name + ": duplicate numeric value";
                    return false;
                }
            }
            symbols->push_back(std::move(symbol));
            if (known_bits != nullptr) {
                *known_bits |= static_cast<uint64_t>(symbols->back().value);
            }
        }
        return true;
    }

    void Merge_Constraints_(const s_constraints& additional, s_constraints* constraints) {
        if (additional.has_min) {
            constraints->has_min = true;
            constraints->min = additional.min;
        }
        if (additional.has_max) {
            constraints->has_max = true;
            constraints->max = additional.max;
        }
        if (additional.has_min_length) {
            constraints->has_min_length = true;
            constraints->min_length = additional.min_length;
        }
        if (additional.has_max_length) {
            constraints->has_max_length = true;
            constraints->max_length = additional.max_length;
        }
        if (!additional.encoding.empty()) {
            constraints->encoding = additional.encoding;
        }
    }

    bool Narrow_Constraints_(const s_constraints& base, const s_constraints& field,
                             const std::string& path, s_constraints* result,
                             std::string* diagnostic) {
        *result = base;
        if ((field.has_min && base.has_min && field.min < base.min) ||
            (field.has_max && base.has_max && field.max > base.max) ||
            (field.has_min_length && base.has_min_length &&
             field.min_length < base.min_length) ||
            (field.has_max_length && base.has_max_length &&
             field.max_length > base.max_length)) {
            *diagnostic = path + ": field constraints cannot broaden the referenced type";
            return false;
        }
        Merge_Constraints_(field, result);
        if ((result->has_min && result->has_max && result->min > result->max) ||
            (result->has_min_length && result->has_max_length &&
             result->min_length > result->max_length)) {
            *diagnostic = path + ": field constraints define an empty range";
            return false;
        }
        return true;
    }

    bool Resolve_Type_(const s_json_value* root, const s_json_value* declaration,
                       const std::string& path, std::vector<std::string>* stack,
                       s_resolved_type* resolved, std::string* diagnostic) {
        if (declaration == nullptr) {
            *diagnostic = path + ": missing type";
            return false;
        }
        if (declaration->kind == JSON_STRING) {
            if (Primitive_(declaration->string_value, &resolved->primitive)) {
                resolved->node_kind = NODE_SCALAR;
                resolved->scalar_kind = SCALAR_PRIMITIVE;
                return true;
            }
            const s_json_value* named = nullptr;
            const char* collection = nullptr;
            if (Find_Named_(root, "types", declaration->string_value, &named)) {
                collection = "types";
            } else if (Find_Named_(root, "records", declaration->string_value, &named)) {
                collection = "records";
            } else {
                *diagnostic = path + ": unknown type '" + declaration->string_value + "'";
                return false;
            }
            for (const std::string& active : *stack) {
                if (active == declaration->string_value) {
                    *diagnostic = path + ": recursive value type '" + declaration->string_value + "'";
                    return false;
                }
            }
            stack->push_back(declaration->string_value);
            bool success = false;
            if (std::string(collection) == "records") {
                resolved->node_kind = NODE_RECORD;
                resolved->aggregate = named;
                success = true;
            } else {
                success = Resolve_Type_(root, named, path, stack, resolved, diagnostic);
            }
            stack->pop_back();
            return success;
        }
        if (declaration->kind != JSON_OBJECT) {
            *diagnostic = path + ": expected a type name or object";
            return false;
        }

        const s_json_value* kind_value = Member_(declaration, "kind");
        if (kind_value == nullptr) {
            return Resolve_Type_(root, Member_(declaration, "type"), path + ".type",
                                 stack, resolved, diagnostic);
        }
        if (kind_value->kind != JSON_STRING) {
            *diagnostic = path + ".kind: expected a string";
            return false;
        }
        const std::string& kind = kind_value->string_value;
        if (kind == "record") {
            resolved->node_kind = NODE_RECORD;
            resolved->aggregate = declaration;
            return true;
        }
        if (kind == "array") {
            resolved->node_kind = NODE_ARRAY;
            resolved->aggregate = declaration;
            return true;
        }
        if (kind == "alias") {
            if (!Resolve_Type_(root, Member_(declaration, "type"), path + ".type",
                               stack, resolved, diagnostic)) {
                return false;
            }
            s_constraints constraints;
            if (!Constraints_(Member_(declaration, "constraints"), path + ".constraints",
                              resolved->primitive, &constraints, diagnostic)) {
                return false;
            }
            s_constraints narrowed;
            if (!Narrow_Constraints_(resolved->constraints, constraints,
                                     path + ".constraints", &narrowed, diagnostic)) {
                return false;
            }
            resolved->constraints = narrowed;
            return true;
        }
        if (kind == "enum" || kind == "mask") {
            const s_json_value* base = Member_(declaration, "base_type");
            s_json_value default_base;
            if (base == nullptr && kind == "mask") {
                default_base.kind = JSON_STRING;
                default_base.string_value = "uint32";
                base = &default_base;
            }
            s_resolved_type base_type;
            if (!Resolve_Type_(root, base, path + ".base_type", stack,
                               &base_type, diagnostic) ||
                base_type.node_kind != NODE_SCALAR ||
                base_type.primitive == PRIMITIVE_BOOL ||
                base_type.primitive == PRIMITIVE_STRING ||
                base_type.primitive == PRIMITIVE_RATIO) {
                *diagnostic = path + ".base_type: expected an integer primitive or alias";
                return false;
            }
            resolved->primitive = base_type.primitive;
            resolved->constraints = base_type.constraints;
            resolved->scalar_kind = kind == "enum" ? SCALAR_ENUM : SCALAR_MASK;
            if (!Optional_Bool_(declaration, "allow_numeric", false, path,
                                &resolved->allow_numeric, diagnostic)) {
                return false;
            }
            const char* reject_name = kind == "enum" ? "reject_unknown_numeric" :
                                                       "reject_unknown_bits";
            if (!Optional_Bool_(declaration, reject_name, false, path,
                                &resolved->reject_unknown_numeric, diagnostic) ||
                !Symbols_(Member_(declaration, "values"), path + ".values",
                          &resolved->values, nullptr, diagnostic)) {
                return false;
            }
            if (kind == "mask" &&
                !Symbols_(Member_(declaration, "flags"), path + ".flags",
                          &resolved->flags, &resolved->known_bits, diagnostic)) {
                return false;
            }
            if (kind == "enum" && resolved->values.empty()) {
                *diagnostic = path + ".values: enum requires at least one value";
                return false;
            }
            int64_t minimum = 0;
            int64_t maximum = 0;
            Primitive_Range_(resolved->primitive, &minimum, &maximum);
            for (const s_symbol& symbol : resolved->values) {
                if (symbol.value < minimum || symbol.value > maximum) {
                    *diagnostic = path + ".values." + symbol.name +
                                  ": value is outside the base type range";
                    return false;
                }
                if ((resolved->constraints.has_min &&
                     symbol.value < resolved->constraints.min) ||
                    (resolved->constraints.has_max &&
                     symbol.value > resolved->constraints.max)) {
                    *diagnostic = path + ".values." + symbol.name +
                                  ": value violates base alias constraints";
                    return false;
                }
            }
            for (const s_symbol& flag : resolved->flags) {
                const uint64_t bits = static_cast<uint64_t>(flag.value);
                if (flag.value <= 0 || (bits & (bits - 1)) != 0 ||
                    flag.value < minimum || flag.value > maximum) {
                    *diagnostic = path + ".flags." + flag.name +
                                  ": flags must be single positive bits in the base type range";
                    return false;
                }
            }
            if (kind == "mask") {
                for (const s_symbol& symbol : resolved->values) {
                    if ((static_cast<uint64_t>(symbol.value) &
                         ~resolved->known_bits) != 0) {
                        *diagnostic = path + ".values." + symbol.name +
                                      ": composite contains unknown bits";
                        return false;
                    }
                }
            }
            return true;
        }
        if (kind == "ratio") {
            const s_json_value* component = Member_(declaration, "component_type");
            s_resolved_type component_type;
            if (!Resolve_Type_(root, component, path + ".component_type", stack,
                               &component_type, diagnostic) ||
                component_type.node_kind != NODE_SCALAR ||
                component_type.primitive == PRIMITIVE_BOOL ||
                component_type.primitive == PRIMITIVE_STRING ||
                component_type.primitive == PRIMITIVE_RATIO) {
                *diagnostic = path + ".component_type: expected an integer primitive";
                return false;
            }
            resolved->primitive = PRIMITIVE_RATIO;
            resolved->scalar_kind = SCALAR_RATIO;
            resolved->ratio_component_type = component_type.primitive;
            return true;
        }
        *diagnostic = path + ".kind: unsupported type kind '" + kind + "'";
        return false;
    }

    const s_json_value* Object_Member_(const s_json_value* object, const std::string& name) {
        return object != nullptr && object->kind == JSON_OBJECT ?
               Member_(object, name.c_str()) : nullptr;
    }

    bool Parse_Decimal_(const std::string& text, int64_t* value) {
        if (text.empty()) {
            return false;
        }
        size_t offset = text.front() == '-' || text.front() == '+' ? 1 : 0;
        if (offset == text.size()) {
            return false;
        }
        uint64_t magnitude = 0;
        for (; offset < text.size(); ++offset) {
            if (text[offset] < '0' || text[offset] > '9') {
                return false;
            }
            const uint32_t digit = static_cast<uint32_t>(text[offset] - '0');
            if (magnitude > (UINT64_C(9223372036854775807) - digit) / 10) {
                return false;
            }
            magnitude = magnitude * 10 + digit;
        }
        *value = text.front() == '-' ? -static_cast<int64_t>(magnitude) :
                                      static_cast<int64_t>(magnitude);
        return true;
    }

    const s_symbol* Find_Symbol_(const std::vector<s_symbol>& symbols,
                                 const std::string& name) {
        for (const s_symbol& symbol : symbols) {
            if (symbol.name == name) {
                return &symbol;
            }
        }
        return nullptr;
    }

    bool Decode_Default_(const s_resolved_type& type, const s_json_value* value,
                         const std::string& path, s_parameter* parameter,
                         std::string* diagnostic) {
        if (value == nullptr) {
            *diagnostic = path + ": every writable leaf requires a default";
            return false;
        }
        if (type.scalar_kind == SCALAR_ENUM) {
            if (value->kind == JSON_STRING) {
                const s_symbol* symbol = Find_Symbol_(type.values, value->string_value);
                if (symbol == nullptr) {
                    *diagnostic = path + ": unknown enum value '" + value->string_value + "'";
                    return false;
                }
                parameter->default_integer = symbol->value;
                return true;
            }
            if (!type.allow_numeric) {
                *diagnostic = path + ": numeric enum defaults are not allowed";
                return false;
            }
        } else if (type.scalar_kind == SCALAR_MASK) {
            if (value->kind == JSON_STRING) {
                const s_symbol* composite = Find_Symbol_(type.values, value->string_value);
                if (composite != nullptr) {
                    parameter->default_integer = composite->value;
                    return true;
                }
                size_t start = 0;
                while (start < value->string_value.size()) {
                    const size_t comma = value->string_value.find(',', start);
                    const size_t end = comma == std::string::npos ?
                                       value->string_value.size() : comma;
                    const s_symbol* flag = Find_Symbol_(
                        type.flags, value->string_value.substr(start, end - start));
                    if (flag == nullptr) {
                        *diagnostic = path + ": unknown mask flag";
                        return false;
                    }
                    parameter->default_integer |= flag->value;
                    start = end + 1;
                }
                return true;
            }
            if (!type.allow_numeric) {
                *diagnostic = path + ": numeric mask defaults are not allowed";
                return false;
            }
        } else if (type.scalar_kind == SCALAR_RATIO) {
            if (value->kind == JSON_STRING) {
                const size_t slash = value->string_value.find('/');
                if (slash == std::string::npos ||
                    !Parse_Decimal_(value->string_value.substr(0, slash),
                                    &parameter->default_ratio_numerator) ||
                    !Parse_Decimal_(value->string_value.substr(slash + 1),
                                    &parameter->default_ratio_denominator) ||
                    parameter->default_ratio_denominator == 0) {
                    *diagnostic = path + ": invalid ratio default";
                    return false;
                }
            } else if (Integer_(value, &parameter->default_ratio_numerator)) {
                parameter->default_ratio_denominator = 1;
            } else {
                *diagnostic = path + ": expected a ratio string or integer";
                return false;
            }
            if (parameter->default_ratio_denominator < 0) {
                parameter->default_ratio_numerator = -parameter->default_ratio_numerator;
                parameter->default_ratio_denominator = -parameter->default_ratio_denominator;
            }
            const int64_t divisor = std::gcd(parameter->default_ratio_numerator,
                                             parameter->default_ratio_denominator);
            parameter->default_ratio_numerator /= divisor;
            parameter->default_ratio_denominator /= divisor;
            int64_t minimum = 0;
            int64_t maximum = 0;
            Primitive_Range_(type.ratio_component_type, &minimum, &maximum);
            if (parameter->default_ratio_numerator < minimum ||
                parameter->default_ratio_numerator > maximum ||
                parameter->default_ratio_denominator < minimum ||
                parameter->default_ratio_denominator > maximum) {
                *diagnostic = path + ": normalized ratio components exceed their type";
                return false;
            }
            return true;
        }

        if (type.primitive == PRIMITIVE_BOOL) {
            if (value->kind != JSON_BOOL) {
                *diagnostic = path + ": expected a boolean";
                return false;
            }
            parameter->default_bool = value->bool_value;
            return true;
        }
        if (type.primitive == PRIMITIVE_STRING) {
            if (value->kind != JSON_STRING) {
                *diagnostic = path + ": expected a string";
                return false;
            }
            const size_t length = Encoded_Length_(value->string_value);
            if (length == SIZE_MAX ||
                (type.constraints.has_min_length && length < type.constraints.min_length) ||
                (type.constraints.has_max_length && length > type.constraints.max_length)) {
                *diagnostic = path + ": string violates encoding or length constraints";
                return false;
            }
            parameter->default_string = value->string_value;
            return true;
        }
        if (!Integer_(value, &parameter->default_integer)) {
            *diagnostic = path + ": expected an integer";
            return false;
        }
        int64_t minimum = 0;
        int64_t maximum = 0;
        Primitive_Range_(type.primitive, &minimum, &maximum);
        if ((type.constraints.has_min && parameter->default_integer < type.constraints.min) ||
            (type.constraints.has_max && parameter->default_integer > type.constraints.max) ||
            parameter->default_integer < minimum || parameter->default_integer > maximum) {
            *diagnostic = path + ": integer violates type or field constraints";
            return false;
        }
        if ((type.scalar_kind == SCALAR_ENUM && type.reject_unknown_numeric &&
             Find_Symbol_(type.values, std::to_string(parameter->default_integer)) == nullptr)) {
            bool found = false;
            for (const s_symbol& symbol : type.values) {
                found = found || symbol.value == parameter->default_integer;
            }
            if (!found) {
                *diagnostic = path + ": unknown numeric enum default";
                return false;
            }
        }
        if (type.scalar_kind == SCALAR_MASK && type.reject_unknown_numeric &&
            (static_cast<uint64_t>(parameter->default_integer) & ~type.known_bits) != 0) {
            *diagnostic = path + ": mask default contains unknown bits";
            return false;
        }
        return true;
    }

    bool Binding_Members_(const s_json_value* declaration, const std::string& path,
                          std::vector<std::string>* members, std::string* diagnostic) {
        const s_json_value* binding = Member_(declaration, "bind");
        if (binding == nullptr) {
            return true;
        }
        if (binding->kind != JSON_ARRAY || binding->values.empty()) {
            *diagnostic = path + ".bind: expected a non-empty member-path array";
            return false;
        }
        for (const s_json_value& member : binding->values) {
            if (member.kind != JSON_STRING || member.string_value.empty()) {
                *diagnostic = path + ".bind: expected non-empty member paths";
                return false;
            }
            members->push_back(member.string_value);
        }
        return true;
    }

    std::vector<std::string> Append_Bindings_(const std::vector<std::string>& bases,
                                              const std::vector<std::string>& members) {
        if (members.empty()) {
            return bases;
        }
        std::vector<std::string> targets;
        for (const std::string& base : bases) {
            for (const std::string& member : members) {
                targets.push_back(base + "." + member);
            }
        }
        return targets;
    }

    bool Build_Node_(const s_json_value* root, const std::string& id,
                     const s_json_value* declaration, const s_json_value* inherited_default,
                     const std::string& canonical_path, const std::string& schema_path,
                     const std::string& group, const s_metadata& inherited_metadata,
                     const s_binding& root_binding, const std::vector<std::string>& bases,
                     uint32_t parent, int32_t external_index,
                     std::vector<const s_json_value*>* aggregate_stack, s_schema* schema,
                     std::string* diagnostic) {
        std::vector<std::string> stack;
        s_resolved_type type;
        const s_json_value* type_declaration = Member_(declaration, "type");
        if (!Resolve_Type_(root, type_declaration, schema_path + ".type", &stack,
                           &type, diagnostic)) {
            return false;
        }
        if (type.node_kind != NODE_SCALAR) {
            for (const s_json_value* active : *aggregate_stack) {
                if (active == type.aggregate) {
                    *diagnostic = schema_path + ": recursive aggregate value type";
                    return false;
                }
            }
            if (aggregate_stack->size() >= 64) {
                *diagnostic = schema_path + ": aggregate nesting exceeds 64 levels";
                return false;
            }
            aggregate_stack->push_back(type.aggregate);
        }
        s_constraints field_constraints;
        if (type.node_kind == NODE_SCALAR &&
            !Constraints_(Member_(declaration, "constraints"), schema_path + ".constraints",
                          type.primitive, &field_constraints, diagnostic)) {
            return false;
        }
        s_constraints effective_constraints;
        if (!Narrow_Constraints_(type.constraints, field_constraints,
                                 schema_path + ".constraints", &effective_constraints,
                                 diagnostic)) {
            return false;
        }
        type.constraints = effective_constraints;

        std::vector<std::string> members;
        if (!Binding_Members_(declaration, schema_path, &members, diagnostic)) {
            return false;
        }
        const std::vector<std::string> targets = Append_Bindings_(bases, members);
        const uint32_t node_index = static_cast<uint32_t>(schema->nodes.size());
        s_node node;
        node.id = id;
        node.kind = type.node_kind;
        node.parent = parent;
        node.external_index = external_index;
        schema->nodes.push_back(std::move(node));

        const s_json_value* declared_default = Member_(declaration, "default");
        if (inherited_default != nullptr && declared_default != nullptr) {
            *diagnostic = schema_path + ".default: aggregate and descendant defaults overlap";
            return false;
        }
        const s_json_value* local_default = inherited_default != nullptr ? inherited_default :
                                            declared_default;
        if (type.node_kind == NODE_SCALAR) {
            if (targets.empty()) {
                *diagnostic = schema_path + ".bind: writable leaf has no native binding";
                return false;
            }
            s_parameter parameter;
            parameter.path = canonical_path;
            parameter.metadata = inherited_metadata;
            if (!Optional_String_(declaration, "title", schema_path,
                                  &parameter.metadata.title, diagnostic) ||
                !Optional_String_(declaration, "description", schema_path,
                                  &parameter.metadata.description, diagnostic) ||
                !Optional_String_(declaration, "remarks", schema_path,
                                  &parameter.metadata.remarks, diagnostic) ||
                !Optional_String_(declaration, "units", schema_path,
                                  &parameter.metadata.units, diagnostic)) {
                return false;
            }
            parameter.group = group;
            parameter.type = type.primitive;
            parameter.scalar_kind = type.scalar_kind;
            parameter.constraints = type.constraints;
            parameter.values = type.values;
            parameter.flags = type.flags;
            parameter.allow_numeric = type.allow_numeric;
            parameter.reject_unknown_numeric = type.reject_unknown_numeric;
            parameter.known_bits = type.known_bits;
            parameter.ratio_component_type = type.ratio_component_type;
            parameter.binding = root_binding;
            parameter.native_targets = targets;
            if (type.scalar_kind == SCALAR_RATIO && targets.size() != 2) {
                *diagnostic = schema_path +
                              ".bind: ratio leaves require numerator and denominator targets";
                return false;
            }
            if (!Decode_Default_(type, local_default, schema_path + ".default",
                                 &parameter, diagnostic)) {
                return false;
            }
            const uint32_t parameter_index = static_cast<uint32_t>(schema->parameters.size());
            schema->parameters.push_back(std::move(parameter));
            schema->nodes[node_index].first_parameter = parameter_index;
            schema->nodes[node_index].last_parameter = parameter_index;
            schema->nodes[node_index].parameter = parameter_index;
            return true;
        }

        if (type.node_kind == NODE_RECORD) {
            const s_json_value* fields = Member_(type.aggregate, "fields");
            if (fields == nullptr || fields->kind != JSON_ARRAY || fields->values.empty()) {
                *diagnostic = schema_path + ".fields: record requires a non-empty field array";
                return false;
            }
            if (local_default != nullptr && local_default->kind != JSON_OBJECT) {
                *diagnostic = schema_path + ".default: record default must be an object";
                return false;
            }
            std::vector<std::string> field_ids;
            for (const s_json_value& field : fields->values) {
                std::string field_id;
                if (field.kind != JSON_OBJECT ||
                    !Required_String_(&field, "id", schema_path + ".fields", &field_id, diagnostic) ||
                    !Canonical_Name_(field_id)) {
                    *diagnostic = schema_path + ".fields: invalid field declaration";
                    return false;
                }
                for (const std::string& existing : field_ids) {
                    if (existing == field_id) {
                        *diagnostic = schema_path + ".fields." + field_id +
                                      ": duplicate field ID";
                        return false;
                    }
                }
                field_ids.push_back(field_id);
                const s_json_value* field_default = Object_Member_(local_default, field_id);
                if (local_default != nullptr && field_default == nullptr) {
                    *diagnostic = schema_path + ".default: record default is missing field '" +
                                  field_id + "'";
                    return false;
                }
                if (!Build_Node_(root, field_id, &field, field_default,
                                 canonical_path + "." + field_id,
                                 schema_path + ".fields." + field_id, group,
                                 inherited_metadata, root_binding, targets, node_index, 0,
                                 aggregate_stack, schema, diagnostic)) {
                    return false;
                }
            }
            if (local_default != nullptr) {
                for (const std::string& default_name : local_default->member_names) {
                    bool found = false;
                    for (const std::string& field_id : field_ids) {
                        found = found || default_name == field_id;
                    }
                    if (!found) {
                        *diagnostic = schema_path + ".default: unknown record field '" +
                                      default_name + "'";
                        return false;
                    }
                }
            }
        } else {
            int64_t count = 0;
            int64_t index_base = 1;
            const s_json_value* count_value = Member_(type.aggregate, "count");
            const s_json_value* index_value = Member_(type.aggregate, "index_base");
            const s_json_value* element = Member_(type.aggregate, "element");
            if (count_value == nullptr || !Integer_(count_value, &count) || count <= 0 ||
                count > INT32_MAX || element == nullptr || element->kind != JSON_OBJECT ||
                (index_value != nullptr && !Integer_(index_value, &index_base)) ||
                index_base < INT32_MIN || index_base > INT32_MAX ||
                index_base + count - 1 > INT32_MAX) {
                *diagnostic = schema_path + ": invalid array count, index base, or element";
                return false;
            }
            if (local_default != nullptr &&
                (local_default->kind != JSON_ARRAY ||
                 local_default->values.size() != static_cast<size_t>(count))) {
                *diagnostic = schema_path + ".default: exact array default has the wrong length";
                return false;
            }
            for (int64_t index = 0; index < count; ++index) {
                const int64_t external = index_base + index;
                std::vector<std::string> element_targets;
                for (const std::string& target : targets) {
                    element_targets.push_back(target + "[" + std::to_string(index) + "]");
                }
                const s_json_value* element_default = local_default != nullptr ?
                    &local_default->values[static_cast<size_t>(index)] : nullptr;
                if (!Build_Node_(root, "", element, element_default,
                                 canonical_path + "." + std::to_string(external),
                                 schema_path + ".element", group, inherited_metadata,
                                 root_binding, element_targets, node_index,
                                 static_cast<int32_t>(external), aggregate_stack,
                                 schema, diagnostic)) {
                    return false;
                }
            }
        }
        aggregate_stack->pop_back();
        if (schema->parameters.empty()) {
            *diagnostic = schema_path + ": aggregate has no writable leaves";
            return false;
        }
        schema->nodes[node_index].first_parameter =
            schema->nodes[node_index + 1].first_parameter;
        schema->nodes[node_index].last_parameter =
            static_cast<uint32_t>(schema->parameters.size() - 1);
        return true;
    }

    bool Rich_Parameter_(const s_json_value* root, const std::string& id,
                         const s_json_value* value, const std::string& root_path,
                         s_schema* schema, std::string* diagnostic) {
        const std::string path = root_path + ".parameters." + id;
        s_metadata metadata;
        std::string group;
        if (!Canonical_Name_(id) || !Metadata_(value, path, &metadata, diagnostic) ||
            !Required_String_(value, "group", path, &group, diagnostic)) {
            return false;
        }
        s_binding binding;
        const s_json_value* bind_root = Member_(value, "bind_root");
        if (bind_root == nullptr ||
            !Required_String_(bind_root, "header", path + ".bind_root",
                              &binding.header, diagnostic) ||
            !Required_String_(bind_root, "symbol", path + ".bind_root",
                              &binding.symbol, diagnostic) ||
            !Required_String_(bind_root, "cpp_type", path + ".bind_root",
                              &binding.cpp_type, diagnostic)) {
            return false;
        }
        std::vector<std::string> bases = {binding.symbol};
        std::vector<const s_json_value*> aggregate_stack;
        return Build_Node_(root, id, value, nullptr, id, path, group, metadata, binding,
                           bases, UINT32_MAX, 0, &aggregate_stack, schema, diagnostic);
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
    static const char* names[] = {"bool", "int8", "uint8", "int16", "uint16", "int32", "uint32", "string", "ratio"};
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
    if (types == nullptr || types->kind != JSON_OBJECT) {
        *diagnostic = schema_path + ".types: expected an object";
        return false;
    }
    if (records == nullptr || records->kind != JSON_OBJECT) {
        *diagnostic = schema_path + ".records: expected an object";
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
        const s_json_value& parameter_value = parameters->values[index];
        const s_json_value* type_value = Member_(&parameter_value, "type");
        e_primitive_type primitive = PRIMITIVE_BOOL;
        const bool primitive_bootstrap = type_value != nullptr &&
            type_value->kind == JSON_STRING && Primitive_(type_value->string_value, &primitive) &&
            types->values.empty() && records->values.empty();
        if (primitive_bootstrap) {
            schema->parameters.emplace_back();
            if (!Parameter_(parameters->member_names[index], &parameter_value, schema_path,
                            &schema->parameters.back(), diagnostic)) {
                return false;
            }
        } else if (!Rich_Parameter_(&root, parameters->member_names[index],
                                    &parameter_value, schema_path, schema, diagnostic)) {
            return false;
        }
        bool group_found = false;
        for (const s_group& group : schema->groups) {
            const std::string parameter_group = primitive_bootstrap ?
                schema->parameters.back().group :
                Member_(&parameter_value, "group")->string_value;
            if (group.id == parameter_group) {
                group_found = true;
                break;
            }
        }
        if (!group_found) {
            const std::string parameter_group = Member_(&parameter_value, "group") != nullptr ?
                Member_(&parameter_value, "group")->string_value : std::string();
            *diagnostic = schema_path + ".parameters." + parameters->member_names[index] +
                          ".group: unknown group '" + parameter_group + "'";
            return false;
        }
    }
    return true;
}

}