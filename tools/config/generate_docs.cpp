#include "generate_docs.h"

#include <algorithm>
#include <filesystem>

#include "file_util.h"

namespace config::tool {
namespace {
    std::string Default_(const s_parameter& parameter) {
        if (parameter.type == PRIMITIVE_BOOL) {
            return parameter.default_bool ? "true" : "false";
        }
        if (parameter.type == PRIMITIVE_STRING) {
            return "`\"" + parameter.default_string + "\"`";
        }
        if (parameter.type == PRIMITIVE_RATIO) {
            return "`" + std::to_string(parameter.default_ratio_numerator) + "/" +
                   std::to_string(parameter.default_ratio_denominator) + "`";
        }
        return std::to_string(parameter.default_integer);
    }

    const char* Type_(const s_parameter& parameter) {
        switch (parameter.scalar_kind) {
        case SCALAR_ENUM: return "enum";
        case SCALAR_MASK: return "mask";
        case SCALAR_RATIO: return "ratio";
        case SCALAR_PRIMITIVE: return Primitive_Name(parameter.type);
        }
        return Primitive_Name(parameter.type);
    }

    std::string Node_Path_(const s_schema* schema, size_t node_index) {
        std::vector<std::string> components;
        while (node_index != UINT32_MAX) {
            const s_node& node = schema->nodes[node_index];
            if (!node.id.empty()) {
                components.push_back(node.id);
            } else if (node.parent != UINT32_MAX &&
                       schema->nodes[node.parent].kind == NODE_ARRAY) {
                components.push_back(std::to_string(node.external_index));
            }
            node_index = node.parent;
        }
        std::reverse(components.begin(), components.end());
        std::string path;
        for (const std::string& component : components) {
            if (!path.empty()) {
                path += ".";
            }
            path += component;
        }
        return path;
    }

}

bool Generate_Documentation(const s_schema* schema, const char* output_path,
                            std::string* diagnostic) {
    const std::filesystem::path parent = std::filesystem::path(output_path).parent_path();
    std::error_code error;
    if (!parent.empty()) {
        std::filesystem::create_directories(parent, error);
    }
    if (error) {
        *diagnostic = parent.string() + ": unable to create documentation directory";
        return false;
    }

    std::string output =
        "<!-- Generated from specs/config/main.json5. Do not edit. -->\n\n"
        "# Configuration reference\n\n"
        "## Groups\n\n";
    for (const s_group& group : schema->groups) {
        output += "### " + group.metadata.title + " (`" + group.id + "`)\n\n" +
                  group.metadata.description + "\n\n" +
                  "- Saved: `" + (group.save ? std::string("yes") : std::string("no")) + "`\n" +
                  "- Multiplayer synchronized: `" +
                  (group.synchronize ? std::string("yes") : std::string("no")) + "`\n\n";
    }
    output += "## Parameters\n\n";
    bool has_index_aliases = false;
    for (const s_node& node : schema->nodes) {
        has_index_aliases = has_index_aliases || !node.index_aliases.empty();
    }
    if (has_index_aliases) {
        output += "### Array index aliases\n\n";
        for (size_t node_index = 0; node_index < schema->nodes.size(); ++node_index) {
            const s_node& node = schema->nodes[node_index];
            if (node.index_aliases.empty()) {
                continue;
            }
            output += "- `" + Node_Path_(schema, node_index) + "`: ";
            for (size_t index = 0; index < node.index_aliases.size(); ++index) {
                if (index != 0) {
                    output += ", ";
                }
                output += "`" + node.index_aliases[index].name + "` = `" +
                          std::to_string(node.index_aliases[index].value) + "`";
            }
            output += "\n";
        }
        output += "\n";
    }
    for (const s_parameter& parameter : schema->parameters) {
        output += "### " + parameter.metadata.title + " (`" + parameter.path + "`)\n\n" +
                  parameter.metadata.description + "\n\n" +
                  "- Type: `" + Type_(parameter) + "`\n" +
                  "- Group: `" + parameter.group + "`\n" +
                  "- Default: " + Default_(parameter) + "\n";
        if (parameter.constraints.has_min || parameter.constraints.has_max) {
            output += "- Range: ";
            output += parameter.constraints.has_min ? std::to_string(parameter.constraints.min) : "primitive minimum";
            output += " through ";
            output += parameter.constraints.has_max ? std::to_string(parameter.constraints.max) : "primitive maximum";
            output += "\n";
        }
        if (!parameter.metadata.units.empty()) {
            output += "- Units: " + parameter.metadata.units + "\n";
        }
        output += "- Native binding: `";
        for (size_t index = 0; index < parameter.native_targets.size(); ++index) {
            if (index != 0) {
                output += "`, `";
            }
            output += parameter.native_targets[index];
        }
        output += "` from `" + parameter.binding.header + "`\n\n";
        if (!parameter.metadata.remarks.empty()) {
            output += parameter.metadata.remarks + "\n\n";
        }
        for (const std::string& example : parameter.metadata.examples) {
            output += "```text\n" + example + "\n```\n\n";
        }
    }
    return Write_File_If_Different(output_path, output, diagnostic);
}

}