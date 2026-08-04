#include "generate_cpp.h"

#include <cstdio>
#include <filesystem>
#include <set>

#include "file_util.h"

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
        case PRIMITIVE_BITMAP: return "CONFIG_VALUE_BITMAP";
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
        case PRIMITIVE_BITMAP: return "void";
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
        case SCALAR_BITMAP: return "CONFIG_SCALAR_BITMAP";
        case SCALAR_RATIO: return "CONFIG_SCALAR_RATIO";
        }
        return "CONFIG_SCALAR_PRIMITIVE";
    }

    const char* Node_Kind_(e_node_kind kind) {
        switch (kind) {
        case NODE_SCALAR: return "CONFIG_NODE_SCALAR";
        case NODE_RECORD: return "CONFIG_NODE_RECORD";
        case NODE_ARRAY: return "CONFIG_NODE_ARRAY";
        case NODE_NAMESPACE: return "CONFIG_NODE_NAMESPACE";
        }
        return "CONFIG_NODE_SCALAR";
    }

    std::string Native_Target_(const s_parameter& parameter, size_t index) {
        return parameter.native_targets[index];
    }

    std::string Default_Name_(size_t index) {
        return "generated_default_" + std::to_string(index);
    }

    std::string Targets_Name_(size_t index) {
        return "generated_targets_" + std::to_string(index);
    }

    std::string Byte_Array_(const std::vector<uint8_t>& value);

    void Append_Default_(const s_parameter& parameter, size_t index,
                         std::string* output) {
        const std::string name = Default_Name_(index);
        if (parameter.type == PRIMITIVE_STRING) {
            *output += "static const char " + name + "[] = " +
                       Cpp_String_(Encode_Game_String(parameter.default_string)) + ";\n";
        } else if (parameter.type == PRIMITIVE_BITMAP) {
            *output += "static const uint8_t " + name + "[] = " +
                       Byte_Array_(parameter.default_bitmap) + ";\n";
        } else if (parameter.type == PRIMITIVE_RATIO) {
            *output += "static const s_ratio_value " + name + " = { " +
                       std::to_string(parameter.default_ratio_numerator) + ", " +
                       std::to_string(parameter.default_ratio_denominator) + " };\n";
        } else {
            *output += std::string("static const ") + Native_Type_(parameter.type) + " " + name +
                       " = " + Default_Value_(parameter) + ";\n";
        }
    }

    void Append_Targets_(const s_parameter& parameter, size_t index,
                         std::string* output) {
        *output += "static const s_variable_target " + Targets_Name_(index) + "[] = {\n";
        for (const std::string& target : parameter.native_targets) {
            *output += "    { (void*)&" + target + ", sizeof(" + target + ") },\n";
        }
        *output += "};\n\n";
    }

    std::string Byte_Array_(const std::vector<uint8_t>& value) {
        std::string output = "{ ";
        for (size_t index = 0; index < value.size(); ++index) {
            output += index == 0 ? "" : ", ";
            output += std::to_string(value[index]);
        }
        return output + " }";
    }

    bool Same_Symbols_(const std::vector<s_symbol>& left,
                       const std::vector<s_symbol>& right) {
        if (left.size() != right.size()) {
            return false;
        }
        for (size_t index = 0; index < left.size(); ++index) {
            if (left[index].name != right[index].name ||
                left[index].value != right[index].value) {
                return false;
            }
        }
        return true;
    }

    bool Same_Bitmap_Symbols_(const std::vector<s_bitmap_symbol>& left,
                              const std::vector<s_bitmap_symbol>& right) {
        if (left.size() != right.size()) {
            return false;
        }
        for (size_t index = 0; index < left.size(); ++index) {
            if (left[index].name != right[index].name ||
                left[index].value != right[index].value) {
                return false;
            }
        }
        return true;
    }

    bool Same_Runtime_Type_(const s_parameter& left, const s_parameter& right) {
        return left.type == right.type &&
               left.constraints.has_min == right.constraints.has_min &&
               left.constraints.has_max == right.constraints.has_max &&
               left.constraints.min == right.constraints.min &&
               left.constraints.max == right.constraints.max &&
               left.scalar_kind == right.scalar_kind &&
               Same_Symbols_(left.values, right.values) &&
               Same_Symbols_(left.flags, right.flags) &&
               Same_Bitmap_Symbols_(left.bitmap_values, right.bitmap_values) &&
               Same_Bitmap_Symbols_(left.bitmap_flags, right.bitmap_flags) &&
               left.bitmap_known_bits == right.bitmap_known_bits &&
               left.bitmap_bit_count == right.bitmap_bit_count &&
               left.allow_numeric == right.allow_numeric &&
               left.reject_unknown_numeric == right.reject_unknown_numeric &&
               left.known_bits == right.known_bits &&
               left.constraints.has_min_length == right.constraints.has_min_length &&
               left.constraints.has_max_length == right.constraints.has_max_length &&
               left.constraints.min_length == right.constraints.min_length &&
               left.constraints.max_length == right.constraints.max_length &&
               left.ratio_component_type == right.ratio_component_type;
    }

    template<typename T, typename Equal>
    size_t Intern_Table_(const T& table, const std::vector<T>& tables, Equal equal) {
        for (size_t index = 0; index < tables.size(); ++index) {
            if (equal(table, tables[index])) {
                return index;
            }
        }
        return tables.size();
    }

    std::string Source_(const s_schema* schema) {
        std::string output =
            "// Generated from specs/config/main.json5. Do not edit.\n"
            "#include \"config/config_runtime.h\"\n"
            "#include <cstring>\n";
        std::set<std::string> headers;
        for (const s_parameter& parameter : schema->parameters) {
            if (!parameter.binding.header.empty()) {
                headers.insert(parameter.binding.header);
            }
        }
        for (const std::string& header : headers) {
            output += "#include " + Cpp_String_(header) + "\n";
        }
        output += "\n";
        output +=
            "namespace config {\n\n";
        std::vector<std::vector<s_symbol>> value_tables;
        std::vector<std::vector<s_symbol>> flag_tables;
        std::vector<std::vector<s_bitmap_symbol>> bitmap_value_tables;
        std::vector<std::vector<s_bitmap_symbol>> bitmap_flag_tables;
        std::vector<std::vector<uint8_t>> bitmap_known_bits_tables;
        std::vector<size_t> value_table_ids(schema->parameters.size(), SIZE_MAX);
        std::vector<size_t> flag_table_ids(schema->parameters.size(), SIZE_MAX);
        std::vector<size_t> bitmap_value_table_ids(schema->parameters.size(), SIZE_MAX);
        std::vector<size_t> bitmap_flag_table_ids(schema->parameters.size(), SIZE_MAX);
        std::vector<size_t> bitmap_known_bits_table_ids(schema->parameters.size(), SIZE_MAX);
        for (size_t index = 0; index < schema->parameters.size(); ++index) {
            const s_parameter& parameter = schema->parameters[index];
            if (!parameter.values.empty()) {
                value_table_ids[index] = Intern_Table_(
                    parameter.values, value_tables, Same_Symbols_);
                if (value_table_ids[index] == value_tables.size()) {
                    value_tables.push_back(parameter.values);
                }
            }
            if (!parameter.flags.empty()) {
                flag_table_ids[index] = Intern_Table_(
                    parameter.flags, flag_tables, Same_Symbols_);
                if (flag_table_ids[index] == flag_tables.size()) {
                    flag_tables.push_back(parameter.flags);
                }
            }
            if (!parameter.bitmap_values.empty()) {
                bitmap_value_table_ids[index] = Intern_Table_(
                    parameter.bitmap_values, bitmap_value_tables, Same_Bitmap_Symbols_);
                if (bitmap_value_table_ids[index] == bitmap_value_tables.size()) {
                    bitmap_value_tables.push_back(parameter.bitmap_values);
                }
            }
            if (!parameter.bitmap_flags.empty()) {
                bitmap_flag_table_ids[index] = Intern_Table_(
                    parameter.bitmap_flags, bitmap_flag_tables, Same_Bitmap_Symbols_);
                if (bitmap_flag_table_ids[index] == bitmap_flag_tables.size()) {
                    bitmap_flag_tables.push_back(parameter.bitmap_flags);
                }
            }
            if (!parameter.bitmap_known_bits.empty()) {
                bitmap_known_bits_table_ids[index] = Intern_Table_(
                    parameter.bitmap_known_bits, bitmap_known_bits_tables,
                    [](const std::vector<uint8_t>& left, const std::vector<uint8_t>& right) {
                        return left == right;
                    });
                if (bitmap_known_bits_table_ids[index] == bitmap_known_bits_tables.size()) {
                    bitmap_known_bits_tables.push_back(parameter.bitmap_known_bits);
                }
            }
        }
        for (size_t index = 0; index < value_tables.size(); ++index) {
            output += "static const s_symbol_descriptor generated_values_" +
                      std::to_string(index) + "[] = {\n";
            for (const s_symbol& symbol : value_tables[index]) {
                output += "    { " + Cpp_String_(symbol.name) + ", " +
                          std::to_string(symbol.value) + " },\n";
            }
            output += "};\n\n";
        }
        for (size_t index = 0; index < flag_tables.size(); ++index) {
            output += "static const s_symbol_descriptor generated_flags_" +
                      std::to_string(index) + "[] = {\n";
            for (const s_symbol& symbol : flag_tables[index]) {
                output += "    { " + Cpp_String_(symbol.name) + ", " +
                          std::to_string(symbol.value) + " },\n";
            }
            output += "};\n\n";
        }
        for (size_t index = 0; index < bitmap_value_tables.size(); ++index) {
            for (size_t symbol = 0; symbol < bitmap_value_tables[index].size(); ++symbol) {
                output += "static const uint8_t generated_bitmap_value_" +
                          std::to_string(index) + "_symbol_" + std::to_string(symbol) +
                          "[] = " + Byte_Array_(bitmap_value_tables[index][symbol].value) +
                          ";\n";
            }
            output += "static const s_bitmap_symbol_descriptor generated_bitmap_values_" +
                      std::to_string(index) + "[] = {\n";
            for (size_t symbol = 0; symbol < bitmap_value_tables[index].size(); ++symbol) {
                output += "    { " + Cpp_String_(bitmap_value_tables[index][symbol].name) +
                          ", generated_bitmap_value_" + std::to_string(index) + "_symbol_" +
                          std::to_string(symbol) + " },\n";
            }
            output += "};\n\n";
        }
        for (size_t index = 0; index < bitmap_flag_tables.size(); ++index) {
            for (size_t symbol = 0; symbol < bitmap_flag_tables[index].size(); ++symbol) {
                output += "static const uint8_t generated_bitmap_flag_" +
                          std::to_string(index) + "_symbol_" + std::to_string(symbol) +
                          "[] = " + Byte_Array_(bitmap_flag_tables[index][symbol].value) +
                          ";\n";
            }
            output += "static const s_bitmap_symbol_descriptor generated_bitmap_flags_" +
                      std::to_string(index) + "[] = {\n";
            for (size_t symbol = 0; symbol < bitmap_flag_tables[index].size(); ++symbol) {
                output += "    { " + Cpp_String_(bitmap_flag_tables[index][symbol].name) +
                          ", generated_bitmap_flag_" + std::to_string(index) + "_symbol_" +
                          std::to_string(symbol) + " },\n";
            }
            output += "};\n\n";
        }
        for (size_t index = 0; index < bitmap_known_bits_tables.size(); ++index) {
            output += "static const uint8_t generated_bitmap_known_bits_" +
                      std::to_string(index) + "[] = " +
                      Byte_Array_(bitmap_known_bits_tables[index]) + ";\n\n";
        }
        std::vector<size_t> type_representatives;
        std::vector<size_t> type_ids(schema->parameters.size(), SIZE_MAX);
        for (size_t index = 0; index < schema->parameters.size(); ++index) {
            for (size_t type_id = 0; type_id < type_representatives.size(); ++type_id) {
                if (Same_Runtime_Type_(schema->parameters[index],
                                       schema->parameters[type_representatives[type_id]])) {
                    type_ids[index] = type_id;
                    break;
                }
            }
            if (type_ids[index] == SIZE_MAX) {
                type_ids[index] = type_representatives.size();
                type_representatives.push_back(index);
            }
        }
        for (size_t type_id = 0; type_id < type_representatives.size(); ++type_id) {
            const size_t index = type_representatives[type_id];
            const s_parameter& parameter = schema->parameters[index];
            const std::string values = parameter.values.empty() ? "nullptr" :
                "generated_values_" + std::to_string(value_table_ids[index]);
            const std::string flags = parameter.flags.empty() ? "nullptr" :
                "generated_flags_" + std::to_string(flag_table_ids[index]);
            const std::string bitmap_values = parameter.bitmap_values.empty() ? "nullptr" :
                "generated_bitmap_values_" + std::to_string(bitmap_value_table_ids[index]);
            const std::string bitmap_flags = parameter.bitmap_flags.empty() ? "nullptr" :
                "generated_bitmap_flags_" + std::to_string(bitmap_flag_table_ids[index]);
            const std::string bitmap_known_bits = parameter.bitmap_known_bits.empty() ?
                "nullptr" : "generated_bitmap_known_bits_" +
                std::to_string(bitmap_known_bits_table_ids[index]);
            output += "static const s_type_descriptor generated_type_" +
                      std::to_string(type_id) + " = { " + Cpp_Type_(parameter.type) + ", " +
                      (parameter.constraints.has_min ? "true" : "false") + ", " +
                      (parameter.constraints.has_max ? "true" : "false") + ", " +
                      std::to_string(parameter.constraints.min) + ", " +
                      std::to_string(parameter.constraints.max) + ", " +
                      Scalar_Kind_(parameter.scalar_kind) + ", " + values + ", " +
                      std::to_string(parameter.values.size()) + ", " + flags + ", " +
                      std::to_string(parameter.flags.size()) + ", " + bitmap_values + ", " +
                      std::to_string(parameter.bitmap_values.size()) + ", " + bitmap_flags + ", " +
                      std::to_string(parameter.bitmap_flags.size()) + ", " + bitmap_known_bits + ", " +
                      std::to_string(parameter.bitmap_bit_count) + ", " +
                      (parameter.allow_numeric ? "true" : "false") + ", " +
                      (parameter.reject_unknown_numeric ? "true" : "false") + ", " +
                      std::to_string(parameter.known_bits) + ", " +
                      (parameter.constraints.has_min_length ? "true" : "false") + ", " +
                      (parameter.constraints.has_max_length ? "true" : "false") + ", " +
                      std::to_string(parameter.constraints.min_length) + ", " +
                      std::to_string(parameter.constraints.max_length) + ", " +
                      Cpp_Type_(parameter.ratio_component_type) + " };\n";
        }
        output += "\n";
        for (size_t index = 0; index < schema->parameters.size(); ++index) {
            const s_parameter& parameter = schema->parameters[index];
            Append_Default_(parameter, index, &output);
            Append_Targets_(parameter, index, &output);
        }
        output += "const s_variable_descriptor variables[] = {\n";
        for (size_t index = 0; index < schema->parameters.size(); ++index) {
            const s_parameter& parameter = schema->parameters[index];
            output += "    { " + Cpp_String_(parameter.path) + ", " +
                      "&generated_type_" + std::to_string(type_ids[index]) + ", " +
                      "&" + Default_Name_(index) + ", " + Targets_Name_(index) + ", " +
                      std::to_string(parameter.native_targets.size()) + " },\n";
        }
        output +=
            "};\n\n"
            "const size_t variable_count = sizeof(variables) / sizeof(variables[0]);\n\n"
            "";
        for (const s_parameter& parameter : schema->parameters) {
            if (parameter.scalar_kind != SCALAR_BITMAP) {
                continue;
            }
            const size_t byte_count = (parameter.bitmap_bit_count + 7) / 8;
            for (const std::string& target : parameter.native_targets) {
                output += "static_assert(sizeof(" + target + ") == " +
                          std::to_string(byte_count) + ");\n";
            }
        }
        output += "\n";
        size_t array_index_alias_count = 0;
        for (const s_node& node : schema->nodes) {
            array_index_alias_count += node.index_aliases.size();
        }
        if (array_index_alias_count != 0) {
            output += "static const s_array_index_alias_descriptor generated_array_index_aliases[] = {\n";
            for (size_t node_index = 0; node_index < schema->nodes.size(); ++node_index) {
                for (const s_symbol& alias : schema->nodes[node_index].index_aliases) {
                    output += "    { " + std::to_string(node_index) + ", " +
                              Cpp_String_(alias.name) + ", " +
                              std::to_string(alias.value) + " },\n";
                }
            }
            output +=
                "};\n\n"
                "const s_array_index_alias_descriptor* const array_index_aliases = "
                "generated_array_index_aliases;\n\n";
        } else {
            output += "const s_array_index_alias_descriptor* const array_index_aliases = nullptr;\n\n";
        }
        output += "const size_t array_index_alias_count = " +
                  std::to_string(array_index_alias_count) + ";\n\n";
        output += "const s_schema_node_descriptor schema_nodes[] = {\n";
        if (schema->nodes.empty()) {
            for (size_t index = 0; index < schema->parameters.size(); ++index) {
                const s_parameter& parameter = schema->parameters[index];
                output += "    { " + Cpp_String_(parameter.path) +
                          ", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, " +
                          std::to_string(index) + ", " + std::to_string(index) +
                          ", " + std::to_string(index) + ", false, 0 },\n";
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
                          ", " + last + ", " + parameter + ", " +
                          (node.has_terminator ? "true" : "false") + ", " +
                          std::to_string(node.terminator) + " },\n";
            }
        }
        output +=
            "};\n\n"
            "const size_t schema_node_count = sizeof(schema_nodes) / sizeof(schema_nodes[0]);\n\n"
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
    const std::string output_path = Join_Path_(output_directory, "config.cpp");
    return Write_File_If_Different(output_path.c_str(), Source_(schema), diagnostic);
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
        if (!Read_File(committed_path.c_str(), &expected)) {
            *diagnostic = committed_path + ": generated file is missing";
            return false;
        }
        if (!Read_File(temporary_path.c_str(), &actual) || expected != actual) {
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