#include "generate_cpp.h"

#include <cctype>
#include <cstdio>
#include <filesystem>

namespace config::tool {
namespace {
    const char* const GENERATED_FILES[] = {
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
            "}\n";
        return output;
    }

    std::string Source_(const s_schema* schema) {
        std::string output =
            "// Generated from specs/config/main.json5. Do not edit.\n"
            "#include \"config_schema.h\"\n\n"
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
           Write_If_Different_(Join_Path_(output_directory, "config_schema.cpp"), Source_(schema), diagnostic);
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