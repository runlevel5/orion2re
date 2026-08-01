#include "generate_cpp.h"

#include <cstdio>
#include <filesystem>

namespace config::tool {
namespace {
    const char* const GENERATED_FILES[] = {
        "config.cpp",
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
        case PRIMITIVE_RATIO: return "CONFIG_VALUE_RATIO";
        }
        return "CONFIG_VALUE_BOOL";
    }

    const char* Native_Type_(e_primitive_type type) {
        switch (type) {
        case PRIMITIVE_BOOL: return "uint8_t";
        case PRIMITIVE_INT8: return "int8_t";
        case PRIMITIVE_UINT8: return "uint8_t";
        case PRIMITIVE_INT16: return "int16_t";
        case PRIMITIVE_UINT16: return "uint16_t";
        case PRIMITIVE_INT32: return "int32_t";
        case PRIMITIVE_UINT32: return "uint32_t";
        case PRIMITIVE_STRING: return "const char*";
        case PRIMITIVE_RATIO: return "void";
        }
        return "uint8_t";
    }

    std::string Default_Value_(const s_parameter& parameter) {
        if (parameter.type == PRIMITIVE_BOOL) {
            return parameter.default_bool ? "true" : "false";
        }
        if (parameter.type == PRIMITIVE_STRING) {
            return Cpp_String_(parameter.default_string);
        }
        if (parameter.type == PRIMITIVE_RATIO) {
            return std::to_string(parameter.default_ratio_numerator) + "/" +
                   std::to_string(parameter.default_ratio_denominator);
        }
        return std::to_string(parameter.default_integer);
    }

    const char* Scalar_Kind_(e_scalar_kind kind) {
        switch (kind) {
        case SCALAR_PRIMITIVE: return "CONFIG_SCALAR_PRIMITIVE";
        case SCALAR_ENUM: return "CONFIG_SCALAR_ENUM";
        case SCALAR_MASK: return "CONFIG_SCALAR_MASK";
        case SCALAR_RATIO: return "CONFIG_SCALAR_RATIO";
        }
        return "CONFIG_SCALAR_PRIMITIVE";
    }

    const char* Node_Kind_(e_node_kind kind) {
        switch (kind) {
        case NODE_SCALAR: return "CONFIG_NODE_SCALAR";
        case NODE_RECORD: return "CONFIG_NODE_RECORD";
        case NODE_ARRAY: return "CONFIG_NODE_ARRAY";
        }
        return "CONFIG_NODE_SCALAR";
    }

    std::string Native_Target_(const s_parameter& parameter, size_t index) {
        if (index < parameter.native_targets.size()) {
            return parameter.native_targets[index];
        }
        return parameter.binding.symbol + "." + parameter.binding.members[index];
    }

    std::string Source_(const s_schema* schema) {
        std::string output =
            "// Generated from specs/config/main.json5. Do not edit.\n"
            "#include \"config/config_runtime.h\"\n\n";
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
        output +=
            "\nnamespace config {\n\n"
            "const s_group_descriptor groups[] = {\n";
        for (const s_group& group : schema->groups) {
            output += "    { " + Cpp_String_(group.id) + ", " + Cpp_String_(group.metadata.title) +
                      ", " + Cpp_String_(group.metadata.description) + ", " +
                      (group.save ? "true" : "false") +
                      ", " + (group.synchronize ? "true" : "false") + " },\n";
        }
        output +=
            "};\n\n"
            "const size_t group_count = sizeof(groups) / sizeof(groups[0]);\n\n";
        for (size_t index = 0; index < schema->parameters.size(); ++index) {
            const s_parameter& parameter = schema->parameters[index];
            if (!parameter.values.empty()) {
                output += "static const s_symbol_descriptor variable_" +
                          std::to_string(index) + "_values[] = {\n";
                for (const s_symbol& symbol : parameter.values) {
                    output += "    { " + Cpp_String_(symbol.name) + ", " +
                              std::to_string(symbol.value) + " },\n";
                }
                output += "};\n\n";
            }
            if (!parameter.flags.empty()) {
                output += "static const s_symbol_descriptor variable_" +
                          std::to_string(index) + "_flags[] = {\n";
                for (const s_symbol& symbol : parameter.flags) {
                    output += "    { " + Cpp_String_(symbol.name) + ", " +
                              std::to_string(symbol.value) + " },\n";
                }
                output += "};\n\n";
            }
        }
        output += "const s_variable_descriptor variables[] = {\n";
        for (size_t index = 0; index < schema->parameters.size(); ++index) {
            const s_parameter& parameter = schema->parameters[index];
            const std::string values = parameter.values.empty() ? "nullptr" :
                                       "variable_" + std::to_string(index) + "_values";
            const std::string flags = parameter.flags.empty() ? "nullptr" :
                                      "variable_" + std::to_string(index) + "_flags";
            output += "    { " + Cpp_String_(parameter.path) + ", " +
                      Cpp_String_(parameter.metadata.title) + ", " +
                      Cpp_String_(parameter.metadata.description) + ", " +
                      Cpp_String_(parameter.metadata.remarks) + ", " +
                      Cpp_String_(parameter.metadata.units) + ", " + Cpp_String_(parameter.group) +
                      ", " + Cpp_Type_(parameter.type) + ", " +
                      (parameter.constraints.has_min ? "true" : "false") + ", " +
                      (parameter.constraints.has_max ? "true" : "false") + ", " +
                      std::to_string(parameter.constraints.min) + ", " +
                      std::to_string(parameter.constraints.max) +
                      ", " + Scalar_Kind_(parameter.scalar_kind) + ", " + values +
                      ", " + std::to_string(parameter.values.size()) + ", " + flags +
                      ", " + std::to_string(parameter.flags.size()) +
                      ", " + (parameter.allow_numeric ? "true" : "false") +
                      ", " + (parameter.reject_unknown_numeric ? "true" : "false") +
                      ", " + std::to_string(parameter.known_bits) +
                      ", " + (parameter.constraints.has_min_length ? "true" : "false") +
                      ", " + (parameter.constraints.has_max_length ? "true" : "false") +
                      ", " + std::to_string(parameter.constraints.min_length) +
                      ", " + std::to_string(parameter.constraints.max_length) +
                      ", " + Cpp_Type_(parameter.ratio_component_type) + " },\n";
        }
        output +=
            "};\n\n"
            "const size_t variable_count = sizeof(variables) / sizeof(variables[0]);\n\n"
            "const s_schema_node_descriptor schema_nodes[] = {\n";
        if (schema->nodes.empty()) {
            for (size_t index = 0; index < schema->parameters.size(); ++index) {
                const s_parameter& parameter = schema->parameters[index];
                output += "    { " + Cpp_String_(parameter.path) +
                          ", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, " +
                          std::to_string(index) + ", " + std::to_string(index) +
                          ", " + std::to_string(index) + " },\n";
            }
        } else {
            for (const s_node& node : schema->nodes) {
                const std::string parent = node.parent == UINT32_MAX ? "CONFIG_NO_NODE" :
                                           std::to_string(node.parent);
                const std::string first = node.first_parameter == UINT32_MAX ?
                                          "CONFIG_NO_VARIABLE" :
                                          std::to_string(node.first_parameter);
                const std::string last = node.last_parameter == UINT32_MAX ?
                                         "CONFIG_NO_VARIABLE" :
                                         std::to_string(node.last_parameter);
                const std::string parameter = node.parameter == UINT32_MAX ?
                                              "CONFIG_NO_VARIABLE" :
                                              std::to_string(node.parameter);
                output += "    { " + Cpp_String_(node.id) + ", " +
                          Node_Kind_(node.kind) + ", " + parent + ", " +
                          std::to_string(node.external_index) + ", " + first +
                          ", " + last + ", " + parameter + " },\n";
            }
        }
        output +=
            "};\n\n"
            "const size_t schema_node_count = sizeof(schema_nodes) / sizeof(schema_nodes[0]);\n\n"
            "}\n"
            "\nnamespace config::generated {\n\n"
            "bool Get_Default_Value(t_variable_id variable_id, s_typed_value* value) {\n"
            "    if (value == nullptr) {\n"
            "        return false;\n"
            "    }\n"
            "    *value = {};\n"
            "    switch (variable_id) {\n";
        for (size_t index = 0; index < schema->parameters.size(); ++index) {
            const s_parameter& parameter = schema->parameters[index];
            output += "    case " + std::to_string(index) + ":\n"
                      "        value->type = " + Cpp_Type_(parameter.type) + ";\n";
            if (parameter.type == PRIMITIVE_BOOL) {
                output += "        value->bool_value = " + Default_Value_(parameter) + ";\n";
            } else if (parameter.type == PRIMITIVE_STRING) {
                output += "        value->string_value = " +
                          Cpp_String_(Encode_Game_String(parameter.default_string)) + ";\n";
            } else if (parameter.type == PRIMITIVE_RATIO) {
                output += "        value->ratio_numerator = " +
                          std::to_string(parameter.default_ratio_numerator) + ";\n"
                          "        value->ratio_denominator = " +
                          std::to_string(parameter.default_ratio_denominator) + ";\n";
            } else {
                output += "        value->integer_value = " + Default_Value_(parameter) + ";\n";
            }
            output += "        return true;\n";
        }
        output +=
            "    }\n"
            "    return false;\n"
            "}\n\n"
            "bool Set_Value(t_variable_id variable_id, const s_typed_value* value) {\n"
            "    if (value == nullptr) {\n"
            "        return false;\n"
            "    }\n"
            "    switch (variable_id) {\n";
        for (size_t index = 0; index < schema->parameters.size(); ++index) {
            const s_parameter& parameter = schema->parameters[index];
            const std::string native = Native_Target_(parameter, 0);
            const char* value = parameter.type == PRIMITIVE_BOOL ? "value->bool_value" :
                                parameter.type == PRIMITIVE_STRING ? "value->string_value.c_str()" :
                                "value->integer_value";
            output += "    case " + std::to_string(index) + ":\n";
            if (parameter.type == PRIMITIVE_STRING) {
                const size_t target_count = parameter.native_targets.empty() ? 1 :
                                            parameter.native_targets.size();
                for (size_t target = 0; target < target_count; ++target) {
                    const std::string destination = Native_Target_(parameter, target);
                    output += "        strlcpy(" + destination +
                              ", value->string_value.c_str(), sizeof(" + destination + "));\n";
                }
            } else if (parameter.type == PRIMITIVE_RATIO) {
                output += "        " + native + " = (" +
                          Native_Type_(parameter.ratio_component_type) +
                          ")value->ratio_numerator;\n"
                          "        " + Native_Target_(parameter, 1) + " = (" +
                          Native_Type_(parameter.ratio_component_type) +
                          ")value->ratio_denominator;\n";
            } else {
                for (size_t target = 0; target < parameter.native_targets.size(); ++target) {
                    output += "        " + Native_Target_(parameter, target) + " = (" +
                              Native_Type_(parameter.type) + ")" + value + ";\n";
                }
                if (parameter.native_targets.empty()) {
                    output += "        " + native + " = (" + Native_Type_(parameter.type) +
                              ")" + value + ";\n";
                }
            }
            output += "        return true;\n";
        }
        output +=
            "    }\n"
            "    return false;\n"
            "}\n\n"
            "bool Get_Current_Value(t_variable_id variable_id, s_typed_value* value) {\n"
            "    if (value == nullptr) {\n"
            "        return false;\n"
            "    }\n"
            "    *value = {};\n"
            "    switch (variable_id) {\n";
        for (size_t index = 0; index < schema->parameters.size(); ++index) {
            const s_parameter& parameter = schema->parameters[index];
            const std::string native = Native_Target_(parameter, 0);
            output += "    case " + std::to_string(index) + ":\n"
                      "        value->type = " + Cpp_Type_(parameter.type) + ";\n";
            if (parameter.type == PRIMITIVE_BOOL) {
                output += "        value->bool_value = (bool)" + native + ";\n";
            } else if (parameter.type == PRIMITIVE_STRING) {
                output += "        value->string_value = (const char*)" + native + ";\n";
            } else if (parameter.type == PRIMITIVE_RATIO) {
                output += "        value->ratio_numerator = (int64_t)" + native + ";\n"
                          "        value->ratio_denominator = (int64_t)" +
                          Native_Target_(parameter, 1) + ";\n";
            } else {
                output += "        value->integer_value = (int64_t)" + native + ";\n";
            }
            output += "        return true;\n";
        }
        output +=
            "    }\n"
            "    return false;\n"
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
    return Write_If_Different_(Join_Path_(output_directory, "config.cpp"),
                               Source_(schema), diagnostic);
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