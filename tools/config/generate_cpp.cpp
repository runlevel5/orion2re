#include "generate_cpp.h"

#include <algorithm>
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

    std::string Byte_Array_(const std::vector<uint8_t>& value);

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

    struct s_prefix_fill_range {
        size_t node_index;
        size_t first_parameter;
        size_t count;
        size_t explicit_default_count;
    };

    std::vector<s_prefix_fill_range> Prefix_Fill_Ranges_(const s_schema* schema) {
        std::vector<s_prefix_fill_range> ranges;
        for (size_t node_index = 0; node_index < schema->nodes.size(); ++node_index) {
            const s_node& node = schema->nodes[node_index];
            if (node.kind != NODE_ARRAY || node.array_count <= 0 ||
                node.first_parameter == UINT32_MAX ||
                node.last_parameter - node.first_parameter + 1 !=
                    static_cast<uint32_t>(node.array_count) ||
                node.first_parameter >= schema->parameters.size()) {
                continue;
            }
            size_t child_count = 0;
            bool scalar_elements = true;
            for (const s_node& child : schema->nodes) {
                if (child.parent == node_index) {
                    ++child_count;
                    scalar_elements = scalar_elements && child.kind == NODE_SCALAR;
                }
            }
            if (!scalar_elements || child_count != static_cast<size_t>(node.array_count)) {
                continue;
            }
            const s_parameter& first = schema->parameters[node.first_parameter];
            if (first.type == PRIMITIVE_STRING || first.type == PRIMITIVE_BITMAP ||
                first.type == PRIMITIVE_RATIO ||
                first.native_targets.size() != node.native_targets.size()) {
                continue;
            }
            bool compatible = true;
            for (size_t offset = 0; offset < static_cast<size_t>(node.array_count); ++offset) {
                const s_parameter& parameter = schema->parameters[node.first_parameter + offset];
                compatible = compatible && Same_Runtime_Type_(first, parameter) &&
                    parameter.native_targets.size() == node.native_targets.size();
                for (size_t target = 0;
                     compatible && target < node.native_targets.size(); ++target) {
                    compatible = parameter.native_targets[target] ==
                        node.native_targets[target] + "[" + std::to_string(offset) + "]";
                }
            }
            if (compatible) {
                ranges.push_back({node_index, node.first_parameter,
                                  static_cast<size_t>(node.array_count),
                                  node.explicit_default_count});
            }
        }
        return ranges;
    }

    void Append_Typed_Default_(const s_parameter& parameter,
                               const std::string& value, std::string* output) {
        *output += "        *value = {};\n";
        *output += std::string("        value->type = ") +
                   Cpp_Type_(parameter.type) + ";\n";
        if (parameter.type == PRIMITIVE_BOOL) {
            *output += "        value->bool_value = " + value + ";\n";
        } else {
            *output += "        value->integer_value = " + value + ";\n";
        }
        *output += "        return true;\n";
    }

    bool Same_Target_Expression_(const s_native_target_expression& left,
                                 const s_native_target_expression& right) {
        if (left.root != right.root || left.components.size() != right.components.size()) {
            return false;
        }
        for (size_t index = 0; index < left.components.size(); ++index) {
            if (left.components[index].array_index != right.components[index].array_index ||
                left.components[index].member != right.components[index].member) {
                return false;
            }
        }
        return true;
    }

    bool Same_Target_Expressions_(const s_parameter& left, const s_parameter& right) {
        if (left.native_target_expressions.size() != right.native_target_expressions.size()) {
            return false;
        }
        for (size_t index = 0; index < left.native_target_expressions.size(); ++index) {
            if (!Same_Target_Expression_(left.native_target_expressions[index],
                                         right.native_target_expressions[index])) {
                return false;
            }
        }
        return true;
    }

    void Append_Direct_Default_(const s_parameter& parameter, std::string* output) {
        *output += "        *value = {};\n";
        *output += std::string("        value->type = ") +
                   Cpp_Type_(parameter.type) + ";\n";
        if (parameter.type == PRIMITIVE_BOOL) {
            *output += std::string("        value->bool_value = ") +
                       (parameter.default_bool ? "true" : "false") + ";\n";
        } else if (parameter.type == PRIMITIVE_STRING) {
            *output += "        value->string_value = " +
                       Cpp_String_(Encode_Game_String(parameter.default_string)) + ";\n";
        } else if (parameter.type == PRIMITIVE_BITMAP) {
            *output += "        value->bitmap_value = " +
                       Byte_Array_(parameter.default_bitmap) + ";\n";
        } else if (parameter.type == PRIMITIVE_RATIO) {
            *output += "        value->ratio_numerator = " +
                       std::to_string(parameter.default_ratio_numerator) + ";\n";
            *output += "        value->ratio_denominator = " +
                       std::to_string(parameter.default_ratio_denominator) + ";\n";
        } else {
            *output += "        value->integer_value = " +
                       std::to_string(parameter.default_integer) + ";\n";
        }
        *output += "        return true;\n";
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

    bool Same_Layout_(const s_schema* schema,
                      const std::vector<std::vector<uint32_t>>& children,
                      uint32_t left_index, uint32_t right_index) {
        const s_node& left = schema->nodes[left_index];
        const s_node& right = schema->nodes[right_index];
        if (left.id != right.id || left.kind != right.kind ||
            left.last_parameter - left.first_parameter !=
                right.last_parameter - right.first_parameter ||
            left.array_count != right.array_count ||
            left.array_index_base != right.array_index_base ||
            left.has_terminator != right.has_terminator ||
            left.terminator != right.terminator ||
            !Same_Symbols_(left.index_aliases, right.index_aliases) ||
            children[left_index].size() != children[right_index].size()) {
            return false;
        }
        if (left.kind == NODE_SCALAR) {
            return Same_Runtime_Type_(schema->parameters[left.parameter],
                                      schema->parameters[right.parameter]) &&
                   Same_Target_Expressions_(schema->parameters[left.parameter],
                                            schema->parameters[right.parameter]);
        }
        for (size_t child = 0; child < children[left_index].size(); ++child) {
            const uint32_t left_child = children[left_index][child];
            const uint32_t right_child = children[right_index][child];
            if (schema->nodes[left_child].external_index !=
                    schema->nodes[right_child].external_index ||
                !Same_Layout_(schema, children, left_child, right_child)) {
                return false;
            }
        }
        return true;
    }

    void Map_Subtree_(const std::vector<std::vector<uint32_t>>& children,
                      uint32_t representative, uint32_t duplicate,
                      std::vector<bool>* omitted_nodes,
                      std::vector<uint32_t>* node_representatives) {
        (*omitted_nodes)[duplicate] = true;
        (*node_representatives)[duplicate] = representative;
        for (size_t child = 0; child < children[representative].size(); ++child) {
            Map_Subtree_(children, children[representative][child],
                         children[duplicate][child], omitted_nodes,
                         node_representatives);
        }
    }

    uint32_t Compact_Leaf_Count_(const s_schema* schema,
                                 const std::vector<std::vector<uint32_t>>& children,
                                 const std::vector<bool>& repeated_layout_arrays,
                                 uint32_t node_index) {
        uint32_t layout = children[node_index].front();
        while (schema->nodes[layout].kind == NODE_ARRAY &&
               repeated_layout_arrays[layout]) {
            layout = children[layout].front();
        }
        return schema->nodes[layout].last_parameter -
               schema->nodes[layout].first_parameter + 1;
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
        const std::vector<s_prefix_fill_range> prefix_fill_ranges =
            Prefix_Fill_Ranges_(schema);
        std::vector<bool> prefix_fill_parameters(schema->parameters.size(), false);
        for (size_t range_index = 0; range_index < prefix_fill_ranges.size(); ++range_index) {
            const s_prefix_fill_range& range = prefix_fill_ranges[range_index];
            for (size_t offset = 0; offset < range.count; ++offset) {
                prefix_fill_parameters[range.first_parameter + offset] = true;
            }
            if (range.explicit_default_count == 0) {
                continue;
            }
            const s_parameter& parameter = schema->parameters[range.first_parameter];
            output += std::string("static const ") + Native_Type_(parameter.type) +
                      " generated_array_defaults_" + std::to_string(range_index) + "[] = { ";
            for (size_t offset = 0; offset < range.explicit_default_count; ++offset) {
                output += offset == 0 ? "" : ", ";
                output += Default_Value_(schema->parameters[range.first_parameter + offset]);
            }
            output += " };\n";
        }
        output += "\nconst size_t variable_count = " +
                  std::to_string(schema->parameters.size()) + ";\n\n";
        size_t maximum_target_count = 0;
        for (const s_parameter& parameter : schema->parameters) {
            maximum_target_count = std::max(maximum_target_count,
                                            parameter.native_targets.size());
        }
        output += "static_assert(CONFIG_MAX_VARIABLE_TARGETS >= " +
                  std::to_string(maximum_target_count) + ");\n\n";
        std::vector<std::vector<uint32_t>> model_children(schema->nodes.size());
        for (uint32_t node_index = 0; node_index < schema->nodes.size(); ++node_index) {
            const uint32_t parent = schema->nodes[node_index].parent;
            if (parent != UINT32_MAX) model_children[parent].push_back(node_index);
        }
        std::vector<bool> omitted_nodes(schema->nodes.size(), false);
        std::vector<uint32_t> node_representatives(schema->nodes.size());
        for (uint32_t node_index = 0; node_index < schema->nodes.size(); ++node_index) {
            node_representatives[node_index] = node_index;
        }
        std::vector<bool> repeated_layout_arrays(schema->nodes.size(), false);
        for (uint32_t node_index = 0; node_index < schema->nodes.size(); ++node_index) {
            const s_node& node = schema->nodes[node_index];
            if (omitted_nodes[node_index] || node.kind != NODE_ARRAY ||
                model_children[node_index].size() != static_cast<size_t>(node.array_count) ||
                model_children[node_index].empty()) {
                continue;
            }
            const uint32_t representative = model_children[node_index].front();
            bool equivalent = true;
            for (size_t element = 1; element < model_children[node_index].size(); ++element) {
                equivalent = equivalent &&
                    Same_Layout_(schema, model_children, representative,
                                 model_children[node_index][element]);
            }
            if (!equivalent) continue;
            repeated_layout_arrays[node_index] = true;
            for (size_t element = 1; element < model_children[node_index].size(); ++element) {
                Map_Subtree_(model_children, representative,
                             model_children[node_index][element], &omitted_nodes,
                             &node_representatives);
            }
        }
        for (uint32_t node_index = 0; node_index < schema->nodes.size(); ++node_index) {
            while (node_representatives[node_index] !=
                   node_representatives[node_representatives[node_index]]) {
                node_representatives[node_index] =
                    node_representatives[node_representatives[node_index]];
            }
        }
        std::vector<uint32_t> runtime_node_ids(schema->nodes.size(), UINT32_MAX);
        std::vector<uint32_t> runtime_nodes;
        for (uint32_t node_index = 0; node_index < schema->nodes.size(); ++node_index) {
            if (!omitted_nodes[node_index]) {
                runtime_node_ids[node_index] = static_cast<uint32_t>(runtime_nodes.size());
                runtime_nodes.push_back(node_index);
            }
        }
        std::vector<uint32_t> compact_ranges;
        std::vector<uint32_t> compact_default_ranges;
        for (uint32_t node_index = 0; node_index < schema->nodes.size(); ++node_index) {
            if (!repeated_layout_arrays[node_index]) continue;
            bool nested = false;
            for (uint32_t parent = schema->nodes[node_index].parent;
                 parent != UINT32_MAX; parent = schema->nodes[parent].parent) {
                nested = nested || repeated_layout_arrays[parent];
            }
            bool entirely_prefix_fill = true;
            for (uint32_t parameter = schema->nodes[node_index].first_parameter;
                 parameter <= schema->nodes[node_index].last_parameter; ++parameter) {
                entirely_prefix_fill = entirely_prefix_fill &&
                    prefix_fill_parameters[parameter];
            }
            if (!nested) {
                compact_ranges.push_back(node_index);
                if (!entirely_prefix_fill) compact_default_ranges.push_back(node_index);
            }
        }
        std::vector<bool> compact_parameters(schema->parameters.size(), false);
        for (uint32_t node_index : compact_ranges) {
            const s_node& node = schema->nodes[node_index];
            for (uint32_t parameter = node.first_parameter;
                 parameter <= node.last_parameter; ++parameter) {
                compact_parameters[parameter] = true;
            }
        }
        std::vector<bool> compact_default_parameters(schema->parameters.size(), false);
        for (uint32_t node_index : compact_default_ranges) {
            const s_node& node = schema->nodes[node_index];
            for (uint32_t parameter = node.first_parameter;
                 parameter <= node.last_parameter; ++parameter) {
                compact_default_parameters[parameter] = true;
            }
        }
        for (size_t range_index = 0; range_index < compact_default_ranges.size(); ++range_index) {
            const s_node& range = schema->nodes[compact_default_ranges[range_index]];
            const uint32_t leaf_count = Compact_Leaf_Count_(
                schema, model_children, repeated_layout_arrays,
                compact_default_ranges[range_index]);
            const uint32_t instance_count =
                (range.last_parameter - range.first_parameter + 1) / leaf_count;
            for (uint32_t leaf = 0; leaf < leaf_count; ++leaf) {
                const s_parameter& parameter = schema->parameters[range.first_parameter + leaf];
                const std::string name = "generated_compact_defaults_" +
                                         std::to_string(range_index) + "_" +
                                         std::to_string(leaf);
                if (parameter.type == PRIMITIVE_BITMAP) {
                    output += "static const uint8_t " + name + "[][" +
                              std::to_string((parameter.bitmap_bit_count + 7) / 8) + "] = {\n";
                } else if (parameter.type == PRIMITIVE_RATIO) {
                    output += "static const s_ratio_value " + name + "[] = {\n";
                } else if (parameter.type == PRIMITIVE_STRING) {
                    output += "static const char* const " + name + "[] = {\n";
                } else {
                    output += std::string("static const ") + Native_Type_(parameter.type) +
                              " " + name + "[] = {\n";
                }
                for (uint32_t instance = 0; instance < instance_count; ++instance) {
                    const s_parameter& value = schema->parameters[
                        range.first_parameter + instance * leaf_count + leaf];
                    output += "    ";
                    if (value.type == PRIMITIVE_BITMAP) {
                        output += Byte_Array_(value.default_bitmap);
                    } else if (value.type == PRIMITIVE_RATIO) {
                        output += "{ " + std::to_string(value.default_ratio_numerator) +
                                  ", " + std::to_string(value.default_ratio_denominator) + " }";
                    } else if (value.type == PRIMITIVE_STRING) {
                        output += Cpp_String_(Encode_Game_String(value.default_string));
                    } else {
                        output += Default_Value_(value);
                    }
                    output += ",\n";
                }
                output += "};\n";
            }
        }
        output += "\n";
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
        for (size_t node_index = 0; node_index < schema->nodes.size(); ++node_index) {
            if (!omitted_nodes[node_index]) {
                array_index_alias_count += schema->nodes[node_index].index_aliases.size();
            }
        }
        if (array_index_alias_count != 0) {
            output += "static const s_array_index_alias_descriptor generated_array_index_aliases[] = {\n";
            for (size_t node_index = 0; node_index < schema->nodes.size(); ++node_index) {
                for (const s_symbol& alias : schema->nodes[node_index].index_aliases) {
                    if (omitted_nodes[node_index]) continue;
                    output += "    { " + std::to_string(runtime_node_ids[node_index]) + ", " +
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
        std::vector<uint32_t> first_children(runtime_nodes.size(), UINT32_MAX);
        std::vector<uint32_t> next_siblings(runtime_nodes.size(), UINT32_MAX);
        std::vector<uint32_t> last_children(runtime_nodes.size(), UINT32_MAX);
        for (uint32_t runtime_index = 0; runtime_index < runtime_nodes.size(); ++runtime_index) {
            const uint32_t node_index = runtime_nodes[runtime_index];
            const uint32_t parent = schema->nodes[node_index].parent;
            if (parent == UINT32_MAX) {
                continue;
            }
            const uint32_t runtime_parent = runtime_node_ids[parent];
            if (first_children[runtime_parent] == UINT32_MAX) {
                first_children[runtime_parent] = runtime_index;
            } else {
                next_siblings[last_children[runtime_parent]] = runtime_index;
            }
            last_children[runtime_parent] = runtime_index;
        }
        if (schema->nodes.empty()) {
            for (size_t index = 0; index < schema->parameters.size(); ++index) {
                const s_parameter& parameter = schema->parameters[index];
                output += "    { " + Cpp_String_(parameter.path) +
                          ", CONFIG_NODE_SCALAR, CONFIG_NO_NODE, 0, " +
                          std::to_string(index) + ", " + std::to_string(index) +
                          ", " + std::to_string(index) + ", false, 0, CONFIG_NO_NODE, " +
                          "CONFIG_NO_NODE, &generated_type_" +
                          std::to_string(type_ids[index]) + ", 0, 0, 0 },\n";
            }
        } else {
            for (size_t runtime_index = 0; runtime_index < runtime_nodes.size(); ++runtime_index) {
                const size_t node_index = runtime_nodes[runtime_index];
                const s_node& node = schema->nodes[node_index];
                const std::string parent = node.parent == UINT32_MAX ? "CONFIG_NO_NODE" :
                                           std::to_string(runtime_node_ids[node.parent]);
                const std::string first = node.first_parameter == UINT32_MAX ?
                                          "CONFIG_NO_VARIABLE" :
                                          std::to_string(node.first_parameter);
                const std::string last = node.last_parameter == UINT32_MAX ?
                                         "CONFIG_NO_VARIABLE" :
                                         std::to_string(node.last_parameter);
                const std::string parameter = node.parameter == UINT32_MAX ?
                                              "CONFIG_NO_VARIABLE" :
                                              std::to_string(node.parameter);
                const std::string first_child = first_children[runtime_index] == UINT32_MAX ?
                                                "CONFIG_NO_NODE" :
                                                std::to_string(first_children[runtime_index]);
                const std::string next_sibling = next_siblings[runtime_index] == UINT32_MAX ?
                                                 "CONFIG_NO_NODE" :
                                                 std::to_string(next_siblings[runtime_index]);
                const uint32_t element_variable_count = repeated_layout_arrays[node_index] ?
                    static_cast<uint32_t>((node.last_parameter - node.first_parameter + 1) /
                                          node.array_count) : 0;
                const std::string runtime_type = node.parameter != UINT32_MAX ?
                    "&generated_type_" + std::to_string(type_ids[node.parameter]) :
                    "nullptr";
                output += "    { " + Cpp_String_(node.id) + ", " +
                          Node_Kind_(node.kind) + ", " + parent + ", " +
                          std::to_string(node.external_index) + ", " + first +
                          ", " + last + ", " + parameter + ", " +
                          (node.has_terminator ? "true" : "false") + ", " +
                          std::to_string(node.terminator) + ", " + first_child + ", " +
                          next_sibling + ", " + runtime_type + ", " +
                          std::to_string(repeated_layout_arrays[node_index] ?
                                         node.array_index_base : 0) +
                          ", " +
                          std::to_string(repeated_layout_arrays[node_index] ?
                                         node.array_count : 0) + ", " +
                          std::to_string(element_variable_count) + " },\n";
            }
        }
        output +=
            "};\n\n"
            "const size_t schema_node_count = sizeof(schema_nodes) / sizeof(schema_nodes[0]);\n\n"
            "namespace generated {\n\n";
        output += "bool Get_Default_Value(t_variable_id variable_id, s_typed_value* value) {\n"
                  "    if (value == nullptr) return false;\n";
        for (size_t range_index = 0; range_index < prefix_fill_ranges.size(); ++range_index) {
            const s_prefix_fill_range& range = prefix_fill_ranges[range_index];
            const s_parameter& first = schema->parameters[range.first_parameter];
            output += "    if (variable_id >= " + std::to_string(range.first_parameter) +
                      " && variable_id < " +
                      std::to_string(range.first_parameter + range.count) + ") {\n";
            output += "        const size_t offset = variable_id - " +
                      std::to_string(range.first_parameter) + ";\n";
            if (range.explicit_default_count == range.count) {
                Append_Typed_Default_(first,
                    "generated_array_defaults_" + std::to_string(range_index) + "[offset]",
                    &output);
            } else {
                output += "        if (offset < " +
                          std::to_string(range.explicit_default_count) + ") {\n";
                if (range.explicit_default_count != 0) {
                    Append_Typed_Default_(first,
                        "generated_array_defaults_" + std::to_string(range_index) + "[offset]",
                        &output);
                } else {
                    output += "            return false;\n";
                }
                output += "        }\n";
                Append_Direct_Default_(
                    schema->parameters[range.first_parameter + range.explicit_default_count],
                    &output);
            }
            output += "    }\n";
        }
        for (size_t range_index = 0; range_index < compact_default_ranges.size(); ++range_index) {
            const s_node& range = schema->nodes[compact_default_ranges[range_index]];
            const uint32_t leaf_count = Compact_Leaf_Count_(
                schema, model_children, repeated_layout_arrays,
                compact_default_ranges[range_index]);
            output += "    if (variable_id >= " + std::to_string(range.first_parameter) +
                      " && variable_id <= " + std::to_string(range.last_parameter) + ") {\n"
                      "        const size_t instance = (variable_id - " +
                      std::to_string(range.first_parameter) + ") / " +
                      std::to_string(leaf_count) + ";\n"
                      "        const size_t leaf = (variable_id - " +
                      std::to_string(range.first_parameter) + ") % " +
                      std::to_string(leaf_count) + ";\n"
                      "        *value = {};\n"
                      "        switch (leaf) {\n";
            for (uint32_t leaf = 0; leaf < leaf_count; ++leaf) {
                const s_parameter& parameter = schema->parameters[range.first_parameter + leaf];
                const std::string defaults = "generated_compact_defaults_" +
                                             std::to_string(range_index) + "_" +
                                             std::to_string(leaf);
                output += "        case " + std::to_string(leaf) + ":\n"
                          "            value->type = " + Cpp_Type_(parameter.type) + ";\n";
                if (parameter.type == PRIMITIVE_BOOL) {
                    output += "            value->bool_value = " + defaults + "[instance];\n";
                } else if (parameter.type == PRIMITIVE_STRING) {
                    output += "            value->string_value = " + defaults + "[instance];\n";
                } else if (parameter.type == PRIMITIVE_BITMAP) {
                    output += "            value->bitmap_value.assign(" + defaults +
                              "[instance], " + defaults + "[instance] + sizeof(" +
                              defaults + "[instance]));\n";
                } else if (parameter.type == PRIMITIVE_RATIO) {
                    output += "            value->ratio_numerator = " + defaults +
                              "[instance].numerator;\n"
                              "            value->ratio_denominator = " + defaults +
                              "[instance].denominator;\n";
                } else {
                    output += "            value->integer_value = " + defaults +
                              "[instance];\n";
                }
                output += "            return true;\n";
            }
            output += "        default: return false;\n"
                      "        }\n"
                      "    }\n";
        }
        output += "    switch (variable_id) {\n";
        for (size_t index = 0; index < schema->parameters.size(); ++index) {
            if (prefix_fill_parameters[index] || compact_default_parameters[index]) continue;
            output += "    case " + std::to_string(index) + ":\n";
            Append_Direct_Default_(schema->parameters[index], &output);
        }
        output += "    default: return false;\n    }\n}\n\n";
        output += "bool Resolve_Variable(t_variable_id variable_id, "
                  "s_resolved_variable* variable) {\n"
                  "    if (variable == nullptr) return false;\n"
                  "    *variable = {};\n";
        for (uint32_t node_index : compact_ranges) {
            const s_node& range_node = schema->nodes[node_index];
            const uint32_t element_variable_count = Compact_Leaf_Count_(
                schema, model_children, repeated_layout_arrays, node_index);
            output += "    if (variable_id >= " +
                      std::to_string(range_node.first_parameter) +
                      " && variable_id <= " +
                      std::to_string(range_node.last_parameter) + ") {\n"
                      "        const t_variable_id leaf = (variable_id - " +
                      std::to_string(range_node.first_parameter) + ") % " +
                      std::to_string(element_variable_count) + ";\n"
                      "        switch (leaf) {\n";
            const uint32_t first = range_node.first_parameter;
            const uint32_t last = first + element_variable_count;
            for (uint32_t parameter_index = first; parameter_index < last;
                 ++parameter_index) {
                const s_parameter& parameter = schema->parameters[parameter_index];
                output += "        case " + std::to_string(parameter_index - first) + ": {\n"
                          "            variable->type = &generated_type_" +
                          std::to_string(type_ids[parameter_index]) + ";\n";
                std::set<uint32_t> emitted_indices;
                for (size_t target_index = 0;
                     target_index < parameter.native_target_expressions.size(); ++target_index) {
                    const s_native_target_expression& target =
                        parameter.native_target_expressions[target_index];
                    std::string expression = target.root;
                    for (const s_native_target_component& component : target.components) {
                        if (!component.array_index) {
                            expression += "." + component.member;
                            continue;
                        }
                        const uint32_t representative =
                            node_representatives[component.array_node];
                        const uint32_t runtime_node = runtime_node_ids[representative];
                        const std::string index_name =
                            "generated_index_" + std::to_string(runtime_node);
                        if (emitted_indices.insert(runtime_node).second) {
                            output += "            uint32_t " + index_name + " = 0;\n"
                                      "            if (!Get_Array_Element_Offset("
                                      "Get_Runtime_Schema(), variable_id, " +
                                      std::to_string(runtime_node) + ", &" + index_name +
                                      ")) return false;\n";
                        }
                        expression += "[" + index_name + "]";
                    }
                    output += "            variable->targets[" +
                              std::to_string(target_index) + "] = { (void*)&" +
                              expression + ", sizeof(" + expression + ") };\n";
                }
                output += "            variable->target_count = " +
                          std::to_string(parameter.native_targets.size()) + ";\n"
                          "            return true;\n"
                          "        }\n";
            }
            output += "        default: return false;\n"
                      "        }\n"
                      "    }\n";
        }
        output += "    switch (variable_id) {\n";
        for (size_t index = 0; index < schema->parameters.size(); ++index) {
            if (compact_parameters[index]) continue;
            const s_parameter& parameter = schema->parameters[index];
            output += "    case " + std::to_string(index) + ":\n"
                      "        variable->type = &generated_type_" +
                      std::to_string(type_ids[index]) + ";\n";
            for (size_t target = 0; target < parameter.native_targets.size(); ++target) {
                const std::string& expression = parameter.native_targets[target];
                output += "        variable->targets[" + std::to_string(target) +
                          "] = { (void*)&" + expression + ", sizeof(" + expression + ") };\n";
            }
            output += "        variable->target_count = " +
                      std::to_string(parameter.native_targets.size()) + ";\n"
                      "        return true;\n";
        }
        output += "    default: return false;\n    }\n}\n\n"
                  "}\n\n"
                  "}\n";
        return output;
    }
}

bool Generate_Cpp(const s_schema* schema, const char* output_directory,
                  std::string* diagnostic) {
    if (!Validate_Runtime_Layout(schema, diagnostic)) {
        return false;
    }
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