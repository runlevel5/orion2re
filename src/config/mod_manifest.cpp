#include "mod_manifest.h"

#include <charconv>
#include <cctype>
#include <cstring>
#include <string_view>
#include <tuple>

#include "game/vfs.h"
#include "version.h"
#include "yyjson.h"

namespace config {
namespace {
    constexpr size_t MOD_MANIFEST_MAX_SIZE = 1024 * 1024;

    bool Is_Identifier_(std::string_view value) {
        if (value.empty() || value.front() < 'a' || value.front() > 'z') {
            return false;
        }
        for (const char character : value) {
            if (!((character >= 'a' && character <= 'z') ||
                  (character >= '0' && character <= '9') ||
                  character == '_' || character == '-')) {
                return false;
            }
        }
        return true;
    }

    bool Read_Vfs_File_(const std::string& path, std::string* source,
                        s_cfg_diagnostic* diagnostic) {
        s_vfs_file* file = vfs::fopen(path.c_str(), "rb");
        if (file == nullptr) {
            diagnostic->location.source = path;
            diagnostic->message = "mod manifest not found";
            return false;
        }
        const int32_t size = vfs::fsize(file);
        if (size < 0 || static_cast<size_t>(size) > MOD_MANIFEST_MAX_SIZE) {
            vfs::fclose(file);
            diagnostic->location.source = path;
            diagnostic->message = "mod manifest has an invalid size";
            return false;
        }
        source->resize(static_cast<size_t>(size));
        const size_t read = source->empty() ? 0 :
                            vfs::fread(source->data(), 1, source->size(), file);
        const int32_t close_result = vfs::fclose(file);
        if (read != source->size() || close_result != 0) {
            diagnostic->location.source = path;
            diagnostic->message = "unable to read mod manifest";
            return false;
        }
        return true;
    }

    bool Fail_(const std::string& path, const std::string& message,
               s_cfg_diagnostic* diagnostic) {
        diagnostic->location.source = path;
        diagnostic->location.line = 1;
        diagnostic->location.column = 1;
        diagnostic->message = message;
        return false;
    }

    bool Validate_Unique_Keys_(yyjson_val* value, const std::string& path,
                               const std::string& value_path,
                               s_cfg_diagnostic* diagnostic, size_t depth) {
        if (depth > 64) {
            return Fail_(path, "mod manifest nesting exceeds 64 levels", diagnostic);
        }
        if (yyjson_is_obj(value)) {
            std::vector<std::string> keys;
            size_t index = 0;
            size_t count = 0;
            yyjson_val* key = nullptr;
            yyjson_val* child = nullptr;
            yyjson_obj_foreach(value, index, count, key, child) {
                const std::string name(yyjson_get_str(key), yyjson_get_len(key));
                for (const std::string& prior : keys) {
                    if (prior == name) {
                        return Fail_(path, "duplicate manifest property '" +
                                           value_path + name + "'", diagnostic);
                    }
                }
                keys.push_back(name);
                if (!Validate_Unique_Keys_(child, path, value_path + name + ".",
                                           diagnostic, depth + 1)) {
                    return false;
                }
            }
        } else if (yyjson_is_arr(value)) {
            size_t index = 0;
            size_t count = 0;
            yyjson_val* child = nullptr;
            yyjson_arr_foreach(value, index, count, child) {
                if (!Validate_Unique_Keys_(child, path, value_path, diagnostic,
                                           depth + 1)) {
                    return false;
                }
            }
        }
        return true;
    }

    bool Get_Required_String_(yyjson_val* object, const char* name,
                              const std::string& path, std::string* output,
                              s_cfg_diagnostic* diagnostic) {
        yyjson_val* value = yyjson_obj_get(object, name);
        if (!yyjson_is_str(value) || yyjson_get_len(value) == 0) {
            return Fail_(path, std::string("manifest property '") + name +
                               "' must be a non-empty string", diagnostic);
        }
        output->assign(yyjson_get_str(value), yyjson_get_len(value));
        return true;
    }

    bool Read_String_Array_(yyjson_val* object, const char* name,
                            const std::string& path, std::vector<std::string>* output,
                            s_cfg_diagnostic* diagnostic) {
        yyjson_val* array = yyjson_obj_get(object, name);
        if (array == nullptr) {
            return true;
        }
        if (!yyjson_is_arr(array)) {
            return Fail_(path, std::string("manifest property '") + name +
                               "' must be an array", diagnostic);
        }
        size_t index = 0;
        size_t count = 0;
        yyjson_val* value = nullptr;
        yyjson_arr_foreach(array, index, count, value) {
            if (!yyjson_is_str(value) || yyjson_get_len(value) == 0) {
                return Fail_(path, std::string("manifest property '") + name +
                                   "' contains a non-string value", diagnostic);
            }
            std::string item(yyjson_get_str(value), yyjson_get_len(value));
            if (!Is_Identifier_(item)) {
                return Fail_(path, "invalid package ID '" + item + "'", diagnostic);
            }
            for (const std::string& prior : *output) {
                if (prior == item) {
                    return Fail_(path, std::string("duplicate '") + name +
                                       "' entry '" + item + "'", diagnostic);
                }
            }
            output->push_back(std::move(item));
        }
        return true;
    }

    bool Read_Dependencies_(yyjson_val* object, const char* name,
                            const std::string& path,
                            std::vector<s_mod_dependency>* output,
                            s_cfg_diagnostic* diagnostic) {
        yyjson_val* array = yyjson_obj_get(object, name);
        if (array == nullptr) {
            return true;
        }
        if (!yyjson_is_arr(array)) {
            return Fail_(path, std::string("manifest property '") + name +
                               "' must be an array", diagnostic);
        }
        size_t index = 0;
        size_t count = 0;
        yyjson_val* value = nullptr;
        yyjson_arr_foreach(array, index, count, value) {
            if (!yyjson_is_obj(value)) {
                return Fail_(path, std::string("manifest property '") + name +
                                   "' contains a non-object value", diagnostic);
            }
            if (yyjson_obj_size(value) != 2) {
                return Fail_(path, std::string("manifest property '") + name +
                                   "' entries must contain only 'id' and 'version'",
                             diagnostic);
            }
            s_mod_dependency dependency;
            if (!Get_Required_String_(value, "id", path, &dependency.id, diagnostic) ||
                !Get_Required_String_(value, "version", path, &dependency.version,
                                      diagnostic)) {
                return false;
            }
            if (!Is_Identifier_(dependency.id)) {
                return Fail_(path, "invalid package ID '" + dependency.id + "'", diagnostic);
            }
            if (!Validate_Semantic_Version_Range(dependency.version)) {
                return Fail_(path, "invalid package version range '" +
                                   dependency.version + "'", diagnostic);
            }
            for (const s_mod_dependency& prior : *output) {
                if (prior.id == dependency.id) {
                    return Fail_(path, std::string("duplicate '") + name +
                                       "' entry '" + dependency.id + "'", diagnostic);
                }
            }
            output->push_back(std::move(dependency));
        }
        return true;
    }

    bool Validate_Properties_(yyjson_val* object, const std::string& path,
                              s_cfg_diagnostic* diagnostic) {
        static constexpr const char* known[] = {
            "id", "name", "version", "requires", "dependencies",
            "optional_dependencies", "conflicts", "load_after", "config",
        };
        size_t index = 0;
        size_t count = 0;
        yyjson_val* key = nullptr;
        yyjson_val* value = nullptr;
        yyjson_obj_foreach(object, index, count, key, value) {
            const std::string name(yyjson_get_str(key), yyjson_get_len(key));
            bool found = false;
            for (const char* candidate : known) {
                found = found || name == candidate;
            }
            if (!found) {
                return Fail_(path, "unknown manifest property '" + name + "'", diagnostic);
            }
        }
        return true;
    }
}

bool Parse_Semantic_Version(const std::string& text, s_semantic_version* version) {
    if (version == nullptr || text.empty()) {
        return false;
    }
    uint32_t parts[3] = {};
    size_t start = 0;
    for (size_t part = 0; part < 3; ++part) {
        const size_t end = part == 2 ? text.size() : text.find('.', start);
        if (end == std::string::npos || end == start ||
            (end - start > 1 && text[start] == '0')) {
            return false;
        }
        const char* first = text.data() + start;
        const char* last = text.data() + end;
        const auto result = std::from_chars(first, last, parts[part]);
        if (result.ec != std::errc{} || result.ptr != last) {
            return false;
        }
        start = end + 1;
    }
    version->major = parts[0];
    version->minor = parts[1];
    version->patch = parts[2];
    return true;
}

bool Semantic_Version_Matches(const s_semantic_version& version,
                              const std::string& range) {
    if (range.empty()) {
        return false;
    }
    size_t cursor = 0;
    bool matched_token = false;
    while (cursor < range.size()) {
        while (cursor < range.size() &&
               std::isspace(static_cast<unsigned char>(range[cursor]))) {
            ++cursor;
        }
        if (cursor == range.size()) {
            break;
        }
        size_t end = cursor;
        while (end < range.size() &&
               !std::isspace(static_cast<unsigned char>(range[end]))) {
            ++end;
        }
        std::string_view token(range.data() + cursor, end - cursor);
        std::string_view operation = "=";
        if (token.starts_with(">=") || token.starts_with("<=")) {
            operation = token.substr(0, 2);
            token.remove_prefix(2);
        } else if (token.starts_with('>') || token.starts_with('<') ||
                   token.starts_with('=')) {
            operation = token.substr(0, 1);
            token.remove_prefix(1);
        }
        s_semantic_version required;
        if (!Parse_Semantic_Version(std::string(token), &required)) {
            return false;
        }
        const auto tuple = [](const s_semantic_version& value) {
            return std::tuple(value.major, value.minor, value.patch);
        };
        const auto actual = tuple(version);
        const auto expected = tuple(required);
        const bool matches = operation == ">=" ? actual >= expected :
                             operation == "<=" ? actual <= expected :
                             operation == ">" ? actual > expected :
                             operation == "<" ? actual < expected :
                             actual == expected;
        if (!matches) {
            return false;
        }
        matched_token = true;
        cursor = end;
    }
    return matched_token;
}

bool Validate_Semantic_Version_Range(const std::string& range) {
    if (range.empty()) {
        return false;
    }
    size_t cursor = 0;
    bool found = false;
    while (cursor < range.size()) {
        while (cursor < range.size() &&
               std::isspace(static_cast<unsigned char>(range[cursor]))) {
            ++cursor;
        }
        if (cursor == range.size()) {
            break;
        }
        size_t end = cursor;
        while (end < range.size() &&
               !std::isspace(static_cast<unsigned char>(range[end]))) {
            ++end;
        }
        std::string_view token(range.data() + cursor, end - cursor);
        if (token.starts_with(">=") || token.starts_with("<=")) {
            token.remove_prefix(2);
        } else if (token.starts_with('>') || token.starts_with('<') ||
                   token.starts_with('=')) {
            token.remove_prefix(1);
        }
        s_semantic_version ignored;
        if (!Parse_Semantic_Version(std::string(token), &ignored)) {
            return false;
        }
        found = true;
        cursor = end;
    }
    return found;
}

bool Load_Mod_Manifest(const std::string& package_id, s_mod_manifest* manifest,
                       s_cfg_diagnostic* diagnostic) {
    if (manifest == nullptr || diagnostic == nullptr || !Is_Identifier_(package_id)) {
        return false;
    }
    *diagnostic = {};
    const std::string path = "mods/" + package_id + "/mod.json5";
    std::string source;
    if (!Read_Vfs_File_(path, &source, diagnostic)) {
        return false;
    }

    const yyjson_read_flag flags = YYJSON_READ_ALLOW_TRAILING_COMMAS |
                                   YYJSON_READ_ALLOW_COMMENTS |
                                   YYJSON_READ_ALLOW_EXT_NUMBER |
                                   YYJSON_READ_ALLOW_SINGLE_QUOTED_STR |
                                   YYJSON_READ_ALLOW_UNQUOTED_KEY;
    yyjson_read_err error{};
    yyjson_doc* document = yyjson_read_opts(source.data(), source.size(), flags,
                                            nullptr, &error);
    if (document == nullptr) {
        size_t line = 1;
        size_t column = 1;
        size_t character = 0;
        yyjson_locate_pos(source.data(), source.size(), error.pos, &line, &column,
                          &character);
        diagnostic->location.source = path;
        diagnostic->location.offset = error.pos;
        diagnostic->location.line = static_cast<uint32_t>(line);
        diagnostic->location.column = static_cast<uint32_t>(column);
        diagnostic->message = std::string("invalid mod manifest: ") + error.msg;
        return false;
    }

    yyjson_val* root = yyjson_doc_get_root(document);
    bool valid = yyjson_is_obj(root) &&
                 Validate_Unique_Keys_(root, path, "", diagnostic, 0) &&
                 Validate_Properties_(root, path, diagnostic);
    if (!yyjson_is_obj(root)) {
        valid = Fail_(path, "mod manifest root must be an object", diagnostic);
    }

    s_mod_manifest candidate;
    if (valid) {
        valid = Get_Required_String_(root, "id", path, &candidate.id, diagnostic) &&
                Get_Required_String_(root, "name", path, &candidate.name, diagnostic) &&
                Get_Required_String_(root, "version", path, &candidate.version_text,
                                     diagnostic);
    }
    if (valid && (!Is_Identifier_(candidate.id) || candidate.id != package_id)) {
        valid = Fail_(path, "manifest ID '" + candidate.id +
                            "' does not match package '" + package_id + "'", diagnostic);
    }
    if (valid && !Parse_Semantic_Version(candidate.version_text, &candidate.version)) {
        valid = Fail_(path, "invalid semantic version '" + candidate.version_text + "'",
                      diagnostic);
    }

    yyjson_val* requirements = valid ? yyjson_obj_get(root, "requires") : nullptr;
    if (valid && !yyjson_is_obj(requirements)) {
        valid = Fail_(path, "manifest property 'requires' must be an object", diagnostic);
    }
    if (valid) {
        size_t requires_size = yyjson_obj_size(requirements);
        yyjson_val* engine = yyjson_obj_get(requirements, "engine");
        if (requires_size != 1 || !yyjson_is_str(engine) || yyjson_get_len(engine) == 0) {
            valid = Fail_(path, "manifest property 'requires.engine' must be the only "
                                "non-empty requirement", diagnostic);
        } else {
            candidate.engine_range.assign(yyjson_get_str(engine), yyjson_get_len(engine));
            const s_semantic_version engine_version = {
                orion2re::ENGINE_VERSION_MAJOR,
                orion2re::ENGINE_VERSION_MINOR,
                orion2re::ENGINE_VERSION_PATCH,
            };
            if (!Validate_Semantic_Version_Range(candidate.engine_range)) {
                valid = Fail_(path, "invalid engine version range '" +
                                    candidate.engine_range + "'", diagnostic);
            } else if (!Semantic_Version_Matches(engine_version, candidate.engine_range)) {
                valid = Fail_(path, "package requires engine '" + candidate.engine_range +
                                    "' but this engine is " + orion2re::ENGINE_VERSION,
                              diagnostic);
            }
        }
    }

    if (valid) {
        valid = Read_Dependencies_(root, "dependencies", path, &candidate.dependencies,
                                   diagnostic) &&
                Read_Dependencies_(root, "optional_dependencies", path,
                                   &candidate.optional_dependencies, diagnostic) &&
                Read_String_Array_(root, "conflicts", path, &candidate.conflicts,
                                   diagnostic) &&
                Read_String_Array_(root, "load_after", path, &candidate.load_after,
                                   diagnostic);
    }
    yyjson_val* config = valid ? yyjson_obj_get(root, "config") : nullptr;
    if (valid) {
        if (!yyjson_is_str(config) || yyjson_get_len(config) == 0) {
            valid = Fail_(path, "manifest property 'config' must be a non-empty string",
                          diagnostic);
        } else {
            candidate.config.assign(yyjson_get_str(config), yyjson_get_len(config));
            char canonical[1024];
            if (vfs::Canonicalize_Asset_Path(candidate.config.c_str(), canonical,
                                             sizeof(canonical)) == 0) {
                valid = Fail_(path, "invalid package configuration path '" +
                                    candidate.config + "'", diagnostic);
            } else {
                candidate.config = canonical;
            }
        }
    }
    yyjson_doc_free(document);
    if (valid) {
        *manifest = std::move(candidate);
    }
    return valid;
}

}