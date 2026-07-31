#include "generate_cpp.h"

#include <cctype>
#include <climits>
#include <cstdio>
#include <filesystem>

namespace config::tool {
namespace {
    const char* const GENERATED_FILES[] = {
        "config_bindings.cpp",
        "config_bindings.h",
        "config_schema.cpp",
        "config_schema.h",
    };

    std::string Join_Path_(const char* directory, const char* name) {
        return (std::filesystem::path(directory) / name).string();
    }

    std::string Cpp_String_(const std::string& value) {
        std::string output = "\"";
        for (uint8_t byte : value) {
            switch (byte) {
            case '\\': output += "\\\\"; break;
            case '"': output += "\\\""; break;
            case '\n': output += "\\n"; break;
            case '\r': output += "\\r"; break;
            case '\t': output += "\\t"; break;
            default:
                if (byte < 0x20 || byte >= 0x7F) {
                    char escaped[8];
                    std::snprintf(escaped, sizeof(escaped), "\\%03o", byte);
                    output += escaped;
                } else {
                    output += static_cast<char>(byte);
                }
                break;
            }
        }
        output += '"';
        return output;
    }

    std::string State_Member_(const std::string& path) {
        std::string output;
        for (uint8_t byte : path) {
            if (std::isalnum(byte) != 0 || byte == '_') {
                output += static_cast<char>(std::tolower(byte));
            } else {
                output += '_';
            }
        }
        return output;
    }

    std::string Identifier_(const std::string& path) {
        std::string output = "CONFIG_PARAMETER_";
        for (uint8_t byte : path) {
            if (std::isalnum(byte) != 0) {
                output += static_cast<char>(std::toupper(byte));
            } else {
                output += '_';
            }
        }
        return output;
    }

    bool Read_File_(const std::string& path, std::string* content) {
        FILE* file = std::fopen(path.c_str(), "rb");
        if (file == nullptr) {
            return false;
        }
        char buffer[4096];
        while (true) {
            const size_t count = std::fread(buffer, 1, sizeof(buffer), file);
            content->append(buffer, count);
            if (count != sizeof(buffer)) {
                const bool success = std::ferror(file) == 0;
                std::fclose(file);
                return success;
            }
        }
    }

    bool Write_If_Different_(const std::string& path, const std::string& content,
                             std::string* diagnostic) {
        std::string existing;
        if (Read_File_(path, &existing) && existing == content) {
            return true;
        }
        FILE* file = std::fopen(path.c_str(), "wb");
        if (file == nullptr) {
            *diagnostic = path + ": unable to open generated output";
            return false;
        }
        const bool success = std::fwrite(content.data(), 1, content.size(), file) == content.size() &&
                             std::fclose(file) == 0;
        if (!success) {
            *diagnostic = path + ": unable to write generated output";
        }
        return success;
    }

    const char* Cpp_Type_(e_primitive_type type) {
        switch (type) {
        case PRIMITIVE_BOOL: return "CONFIG_VALUE_BOOL";
        case PRIMITIVE_INT8: return "CONFIG_VALUE_INT8";
        case PRIMITIVE_UINT8: return "CONFIG_VALUE_UINT8";
        case PRIMITIVE_INT16: return "CONFIG_VALUE_INT16";
        case PRIMITIVE_UINT16: return "CONFIG_VALUE_UINT16";
        case PRIMITIVE_INT32: return "CONFIG_VALUE_INT32";
        case PRIMITIVE_UINT32: return "CONFIG_VALUE_UINT32";
        case PRIMITIVE_STRING: return "CONFIG_VALUE_STRING";
        }
        return "CONFIG_VALUE_BOOL";
    }

    const char* Storage_Type_(e_primitive_type type) {
        switch (type) {
        case PRIMITIVE_BOOL: return "bool";
        case PRIMITIVE_INT8: return "int8_t";
        case PRIMITIVE_UINT8: return "uint8_t";
        case PRIMITIVE_INT16: return "int16_t";
        case PRIMITIVE_UINT16: return "uint16_t";
        case PRIMITIVE_INT32: return "int32_t";
        case PRIMITIVE_UINT32: return "uint32_t";
        case PRIMITIVE_STRING: return "std::string";
        }
        return "bool";
    }

    std::string Default_Value_(const s_parameter& parameter) {
        if (parameter.type == PRIMITIVE_BOOL) {
            return parameter.default_bool ? "true" : "false";
        }
        if (parameter.type == PRIMITIVE_STRING) {
            return Cpp_String_(parameter.default_string);
        }
        return std::to_string(parameter.default_integer);
    }

    std::string Binding_Header_(const s_schema* schema) {
        std::string output =
            "// Generated from specs/config/main.json5. Do not edit.\n"
            "#pragma once\n\n"
            "#include <cstdint>\n"
            "#include <string>\n\n"
            "#include \"config/config_decode.h\"\n\n"
            "namespace config::generated {\n\n"
            "struct s_config_state {\n";
        for (const s_parameter& parameter : schema->parameters) {
            output += "    " + std::string(Storage_Type_(parameter.type)) + " " +
                      State_Member_(parameter.path) + " = " + Default_Value_(parameter) + ";\n";
        }
        output +=
            "};\n\n"
            "void Initialize_Defaults(s_config_state* state);\n"
            "void Read_Native_State(s_config_state* state);\n"
            "bool Apply_Write_Set(const s_config_write_set* write_set,\n"
            "                     const s_config_state* base_state,\n"
            "                     s_config_state* candidate_state,\n"
            "                     s_cfg_diagnostic* diagnostic);\n"
            "bool Apply_Transaction(const s_config_write_set* write_set,\n"
            "                       s_cfg_diagnostic* diagnostic);\n"
            "void Commit_State(const s_config_state* state);\n\n"
            "}\n";
        return output;
    }

    std::string Native_Compatibility_(const s_parameter& parameter) {
        const std::string native = parameter.binding.symbol + "." +
                                   parameter.binding.members.front();
        std::string condition = "std::is_integral_v<std::remove_cvref_t<decltype(" + native + ")>>";
        switch (parameter.type) {
        case PRIMITIVE_BOOL:
            condition += " && sizeof(" + native + ") == 1";
            break;
        case PRIMITIVE_INT8:
            condition += " && sizeof(" + native + ") == 1 && "
                         "std::is_signed_v<std::remove_cvref_t<decltype(" + native + ")>>";
            break;
        case PRIMITIVE_UINT8:
            condition += " && sizeof(" + native + ") == 1 && "
                         "std::is_unsigned_v<std::remove_cvref_t<decltype(" + native + ")>>";
            break;
        case PRIMITIVE_INT16:
            condition += " && sizeof(" + native + ") == 2 && "
                         "std::is_signed_v<std::remove_cvref_t<decltype(" + native + ")>>";
            break;
        case PRIMITIVE_UINT16:
            condition += " && sizeof(" + native + ") == 2 && "
                         "std::is_unsigned_v<std::remove_cvref_t<decltype(" + native + ")>>";
            break;
        case PRIMITIVE_INT32:
            condition += " && sizeof(" + native + ") == 4 && "
                         "std::is_signed_v<std::remove_cvref_t<decltype(" + native + ")>>";
            break;
        case PRIMITIVE_UINT32:
            condition += " && sizeof(" + native + ") == 4 && "
                         "std::is_unsigned_v<std::remove_cvref_t<decltype(" + native + ")>>";
            break;
        case PRIMITIVE_STRING:
            condition = "false";
            break;
        }
        return condition;
    }

    std::string Binding_Source_(const s_schema* schema) {
        std::string output =
            "// Generated from specs/config/main.json5. Do not edit.\n"
            "#include \"config_bindings.h\"\n\n"
            "#include <type_traits>\n\n";
        std::vector<std::string> headers;
        for (const s_parameter& parameter : schema->parameters) {
            bool found = false;
            for (const std::string& header : headers) {
                if (header == parameter.binding.header) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                headers.push_back(parameter.binding.header);
                output += "#include \"" + parameter.binding.header + "\"\n";
            }
        }
        output += "\nnamespace config::generated {\n\n";
        for (const s_parameter& parameter : schema->parameters) {
            output += "static_assert(std::is_same_v<std::remove_cvref_t<decltype(" +
                      parameter.binding.symbol + ")>, " + parameter.binding.cpp_type +
                      ">, \"native binding root type mismatch for " + parameter.path + "\");\n";
            output += "static_assert(" + Native_Compatibility_(parameter) +
                      ", \"native binding type mismatch for " + parameter.path + "\");\n";
        }
        output +=
            "\nnamespace {\n"
            "    bool Fail_(const s_config_write* write, const char* message,\n"
            "               s_cfg_diagnostic* diagnostic) {\n"
            "        diagnostic->location = write->provenance;\n"
            "        diagnostic->message = message;\n"
            "        return false;\n"
            "    }\n\n"
            "    bool Apply_Write_(const s_config_write* write, s_config_state* state,\n"
            "                      s_cfg_diagnostic* diagnostic) {\n"
            "        switch (write->parameter_id) {\n";
        for (const s_parameter& parameter : schema->parameters) {
            output += "        case " + Identifier_(parameter.path) + ":\n";
            output += "            if (write->value.type != " +
                      std::string(Cpp_Type_(parameter.type)) + ") {\n";
            output += "                return Fail_(write, \"typed write mismatch for " +
                      parameter.path + "\", diagnostic);\n"
                      "            }\n";
            if (parameter.type != PRIMITIVE_BOOL && parameter.type != PRIMITIVE_STRING) {
                int64_t primitive_minimum = 0;
                int64_t primitive_maximum = 0;
                switch (parameter.type) {
                case PRIMITIVE_INT8: primitive_minimum = INT8_MIN; primitive_maximum = INT8_MAX; break;
                case PRIMITIVE_UINT8: primitive_minimum = 0; primitive_maximum = UINT8_MAX; break;
                case PRIMITIVE_INT16: primitive_minimum = INT16_MIN; primitive_maximum = INT16_MAX; break;
                case PRIMITIVE_UINT16: primitive_minimum = 0; primitive_maximum = UINT16_MAX; break;
                case PRIMITIVE_INT32: primitive_minimum = INT32_MIN; primitive_maximum = INT32_MAX; break;
                case PRIMITIVE_UINT32: primitive_minimum = 0; primitive_maximum = UINT32_MAX; break;
                default: break;
                }
                const int64_t minimum = parameter.constraints.has_min ? parameter.constraints.min : primitive_minimum;
                const int64_t maximum = parameter.constraints.has_max ? parameter.constraints.max : primitive_maximum;
                output += "            if (write->value.integer_value < " + std::to_string(minimum) +
                          " || write->value.integer_value > " + std::to_string(maximum) + ") {\n"
                          "                return Fail_(write, \"typed write range error for " +
                          parameter.path + "\", diagnostic);\n"
                          "            }\n";
            }
            const std::string member = State_Member_(parameter.path);
            if (parameter.type == PRIMITIVE_BOOL) {
                output += "            state->" + member + " = write->value.bool_value;\n";
            } else if (parameter.type == PRIMITIVE_STRING) {
                output += "            state->" + member + " = write->value.string_value;\n";
            } else {
                output += "            state->" + member + " = static_cast<" +
                          Storage_Type_(parameter.type) + ">(write->value.integer_value);\n";
            }
            output += "            return true;\n";
        }
        output +=
            "        }\n"
            "        return Fail_(write, \"unknown typed write parameter\", diagnostic);\n"
            "    }\n"
            "}\n\n"
            "void Initialize_Defaults(s_config_state* state) {\n"
            "    *state = {};\n"
            "}\n\n"
            "void Read_Native_State(s_config_state* state) {\n";
        for (const s_parameter& parameter : schema->parameters) {
            const std::string native = parameter.binding.symbol + "." +
                                       parameter.binding.members.front();
            const std::string member = State_Member_(parameter.path);
            if (parameter.type == PRIMITIVE_BOOL) {
                output += "    state->" + member + " = " + native + " != 0;\n";
            } else {
                output += "    state->" + member + " = static_cast<" +
                          Storage_Type_(parameter.type) + ">(" + native + ");\n";
            }
        }
        output +=
            "}\n\n"
            "bool Apply_Write_Set(const s_config_write_set* write_set,\n"
            "                     const s_config_state* base_state,\n"
            "                     s_config_state* candidate_state,\n"
            "                     s_cfg_diagnostic* diagnostic) {\n"
            "    if (write_set == nullptr || base_state == nullptr ||\n"
            "        candidate_state == nullptr || diagnostic == nullptr) {\n"
            "        return false;\n"
            "    }\n"
            "    *diagnostic = {};\n"
            "    s_config_state temporary = *base_state;\n"
            "    for (const s_config_write& write : write_set->writes) {\n"
            "        if (!Apply_Write_(&write, &temporary, diagnostic)) {\n"
            "            return false;\n"
            "        }\n"
            "    }\n"
            "    *candidate_state = temporary;\n"
            "    return true;\n"
            "}\n\n"
            "bool Apply_Transaction(const s_config_write_set* write_set,\n"
            "                       s_cfg_diagnostic* diagnostic) {\n"
            "    s_config_state base_state;\n"
            "    s_config_state candidate_state;\n"
            "    Read_Native_State(&base_state);\n"
            "    if (!Apply_Write_Set(write_set, &base_state, &candidate_state, diagnostic)) {\n"
            "        return false;\n"
            "    }\n"
            "    Commit_State(&candidate_state);\n"
            "    return true;\n"
            "}\n\n"
            "void Commit_State(const s_config_state* state) {\n";
        for (const s_parameter& parameter : schema->parameters) {
            const std::string native = parameter.binding.symbol + "." +
                                       parameter.binding.members.front();
            const std::string member = State_Member_(parameter.path);
            output += "    " + native + " = static_cast<decltype(" + native + ")>(state->" +
                      member + ");\n";
        }
        output += "}\n\n}\n";
        return output;
    }

    std::string Header_(const s_schema* schema) {
        std::string output =
            "// Generated from specs/config/main.json5. Do not edit.\n"
            "#pragma once\n\n"
            "#include <cstddef>\n"
            "#include <cstdint>\n\n"
            "namespace config::generated {\n\n"
            "enum e_value_type {\n"
            "    CONFIG_VALUE_BOOL,\n"
            "    CONFIG_VALUE_INT8,\n"
            "    CONFIG_VALUE_UINT8,\n"
            "    CONFIG_VALUE_INT16,\n"
            "    CONFIG_VALUE_UINT16,\n"
            "    CONFIG_VALUE_INT32,\n"
            "    CONFIG_VALUE_UINT32,\n"
            "    CONFIG_VALUE_STRING,\n"
            "};\n\n"
            "enum e_parameter_id : uint64_t {\n";
        for (const s_parameter& parameter : schema->parameters) {
            char id[32];
            std::snprintf(id, sizeof(id), "0x%016llX", static_cast<unsigned long long>(parameter.schema_id));
            output += "    " + Identifier_(parameter.path) + " = UINT64_C(" + id + "),\n";
        }
        output +=
            "};\n\n"
            "struct s_group_descriptor {\n"
            "    const char* id;\n"
            "    const char* title;\n"
            "    const char* description;\n"
            "    const char* mutable_policy;\n"
            "    bool save;\n"
            "    bool synchronize;\n"
            "};\n\n"
            "struct s_parameter_descriptor {\n"
            "    e_parameter_id id;\n"
            "    const char* path;\n"
            "    const char* title;\n"
            "    const char* description;\n"
            "    const char* remarks;\n"
            "    const char* units;\n"
            "    const char* group;\n"
            "    e_value_type type;\n"
            "    bool has_min;\n"
            "    bool has_max;\n"
            "    int64_t min;\n"
            "    int64_t max;\n"
            "    bool default_bool;\n"
            "    int64_t default_integer;\n"
            "    const char* default_string;\n"
            "    const char* binding_header;\n"
            "    const char* binding_symbol;\n"
            "    const char* binding_cpp_type;\n"
            "    const char* binding_member;\n"
            "};\n\n"
            "extern const s_group_descriptor groups[];\n"
            "extern const size_t group_count;\n"
            "extern const s_parameter_descriptor parameters[];\n"
            "extern const size_t parameter_count;\n\n"
            "const s_parameter_descriptor* Find_Parameter(const char* path, size_t path_length);\n\n"
            "}\n";
        return output;
    }

    std::string Source_(const s_schema* schema) {
        std::string output =
            "// Generated from specs/config/main.json5. Do not edit.\n"
            "#include \"config_schema.h\"\n\n"
            "#include <cstring>\n\n"
            "namespace config::generated {\n\n"
            "const s_group_descriptor groups[] = {\n";
        for (const s_group& group : schema->groups) {
            output += "    { " + Cpp_String_(group.id) + ", " + Cpp_String_(group.metadata.title) +
                      ", " + Cpp_String_(group.metadata.description) + ", " +
                      Cpp_String_(group.mutable_policy) + ", " + (group.save ? "true" : "false") +
                      ", " + (group.synchronize ? "true" : "false") + " },\n";
        }
        output +=
            "};\n\n"
            "const size_t group_count = sizeof(groups) / sizeof(groups[0]);\n\n"
            "const s_parameter_descriptor parameters[] = {\n";
        for (const s_parameter& parameter : schema->parameters) {
            const std::string encoded_default = Encode_Game_String(parameter.default_string);
            const std::string default_string = parameter.type == PRIMITIVE_STRING ?
                                               Cpp_String_(encoded_default) : "nullptr";
            output += "    { " + Identifier_(parameter.path) + ", " + Cpp_String_(parameter.path) +
                      ", " + Cpp_String_(parameter.metadata.title) + ", " +
                      Cpp_String_(parameter.metadata.description) + ", " +
                      Cpp_String_(parameter.metadata.remarks) + ", " +
                      Cpp_String_(parameter.metadata.units) + ", " + Cpp_String_(parameter.group) +
                      ", " + Cpp_Type_(parameter.type) + ", " +
                      (parameter.constraints.has_min ? "true" : "false") + ", " +
                      (parameter.constraints.has_max ? "true" : "false") + ", " +
                      std::to_string(parameter.constraints.min) + ", " +
                      std::to_string(parameter.constraints.max) + ", " +
                      (parameter.default_bool ? "true" : "false") + ", " +
                      std::to_string(parameter.default_integer) + ", " + default_string + ", " +
                      Cpp_String_(parameter.binding.header) + ", " +
                      Cpp_String_(parameter.binding.symbol) + ", " +
                      Cpp_String_(parameter.binding.cpp_type) + ", " +
                      Cpp_String_(parameter.binding.members.front()) + " },\n";
        }
        output +=
            "};\n\n"
            "const size_t parameter_count = sizeof(parameters) / sizeof(parameters[0]);\n\n"
            "const s_parameter_descriptor* Find_Parameter(const char* path, size_t path_length) {\n"
            "    if (path == nullptr) {\n"
            "        return nullptr;\n"
            "    }\n"
            "    for (size_t index = 0; index < parameter_count; ++index) {\n"
            "        if (std::strlen(parameters[index].path) == path_length &&\n"
            "            std::memcmp(parameters[index].path, path, path_length) == 0) {\n"
            "            return &parameters[index];\n"
            "        }\n"
            "    }\n"
            "    return nullptr;\n"
            "}\n\n"
            "}\n";
        return output;
    }
}

bool Generate_Cpp(const s_schema* schema, const char* output_directory,
                  std::string* diagnostic) {
    std::error_code error;
    std::filesystem::create_directories(output_directory, error);
    if (error) {
        *diagnostic = std::string(output_directory) + ": unable to create output directory";
        return false;
    }
    return Write_If_Different_(Join_Path_(output_directory, "config_schema.h"), Header_(schema), diagnostic) &&
           Write_If_Different_(Join_Path_(output_directory, "config_schema.cpp"), Source_(schema), diagnostic) &&
           Write_If_Different_(Join_Path_(output_directory, "config_bindings.h"), Binding_Header_(schema), diagnostic) &&
           Write_If_Different_(Join_Path_(output_directory, "config_bindings.cpp"), Binding_Source_(schema), diagnostic);
}

bool Check_Generated_Cpp(const s_schema* schema, const char* committed_directory,
                         const char* temporary_directory, std::string* diagnostic) {
    if (!Generate_Cpp(schema, temporary_directory, diagnostic)) {
        return false;
    }
    for (const char* name : GENERATED_FILES) {
        std::string expected;
        std::string actual;
        const std::string committed_path = Join_Path_(committed_directory, name);
        const std::string temporary_path = Join_Path_(temporary_directory, name);
        if (!Read_File_(committed_path, &expected)) {
            *diagnostic = committed_path + ": generated file is missing";
            return false;
        }
        if (!Read_File_(temporary_path, &actual) || expected != actual) {
            *diagnostic = committed_path + ": generated file is stale";
            return false;
        }
    }
    for (const std::filesystem::directory_entry& entry :
         std::filesystem::directory_iterator(committed_directory)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        bool known = false;
        for (const char* name : GENERATED_FILES) {
            if (entry.path().filename() == name) {
                known = true;
                break;
            }
        }
        if (!known) {
            *diagnostic = entry.path().string() + ": unexpected generated file";
            return false;
        }
    }
    return true;
}

}