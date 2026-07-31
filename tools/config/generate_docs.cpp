#include "generate_docs.h"

#include <cstdio>
#include <filesystem>

namespace config::tool {
namespace {
    std::string Default_(const s_parameter& parameter) {
        if (parameter.type == PRIMITIVE_BOOL) {
            return parameter.default_bool ? "true" : "false";
        }
        if (parameter.type == PRIMITIVE_STRING) {
            return "`\"" + parameter.default_string + "\"`";
        }
        return std::to_string(parameter.default_integer);
    }

    bool Write_If_Different_(const char* path, const std::string& content,
                             std::string* diagnostic) {
        std::string existing;
        FILE* input = std::fopen(path, "rb");
        if (input != nullptr) {
            char buffer[4096];
            while (true) {
                const size_t count = std::fread(buffer, 1, sizeof(buffer), input);
                existing.append(buffer, count);
                if (count != sizeof(buffer)) {
                    break;
                }
            }
            std::fclose(input);
            if (existing == content) {
                return true;
            }
        }
        FILE* output = std::fopen(path, "wb");
        if (output == nullptr) {
            *diagnostic = std::string(path) + ": unable to write documentation";
            return false;
        }
        const bool wrote = std::fwrite(content.data(), 1, content.size(), output) == content.size();
        const bool closed = std::fclose(output) == 0;
        if (!wrote || !closed) {
            *diagnostic = std::string(path) + ": unable to write documentation";
            return false;
        }
        return true;
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
                  "- Mutable: `" + group.mutable_policy + "`\n" +
                  "- Saved: `" + (group.save ? std::string("yes") : std::string("no")) + "`\n" +
                  "- Multiplayer synchronized: `" +
                  (group.synchronize ? std::string("yes") : std::string("no")) + "`\n\n";
    }
    output += "## Parameters\n\n";
    for (const s_parameter& parameter : schema->parameters) {
        output += "### " + parameter.metadata.title + " (`" + parameter.path + "`)\n\n" +
                  parameter.metadata.description + "\n\n" +
                  "- Type: `" + Primitive_Name(parameter.type) + "`\n" +
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
        output += "- Native binding: `" + parameter.binding.symbol + "." +
                  parameter.binding.members.front() + "` from `" + parameter.binding.header + "`\n\n";
        if (!parameter.metadata.remarks.empty()) {
            output += parameter.metadata.remarks + "\n\n";
        }
        for (const std::string& example : parameter.metadata.examples) {
            output += "```text\n" + example + "\n```\n\n";
        }
    }
    return Write_If_Different_(output_path, output, diagnostic);
}

}