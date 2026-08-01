#include "config_loader.h"

#include <string>
#include <utility>
#include <vector>

#include "config/cfg_parser.h"
#include "game/vfs.h"

namespace config {
namespace {
    constexpr size_t CONFIG_MAX_INCLUDE_DEPTH = 64;
    constexpr size_t CONFIG_MAX_FILE_SIZE = 16 * 1024 * 1024;
    constexpr size_t CONFIG_MAX_PATH = 1024;

    struct s_include_frame {
        std::string path;
    };

    struct s_loader {
        std::vector<s_include_frame> stack;
        s_cfg_document flattened;
        s_cfg_diagnostic* diagnostic = nullptr;
    };

    bool Fail_(s_loader* loader, const s_source_location& location,
               const std::string& message) {
        loader->diagnostic->location = location;
        loader->diagnostic->message = message;
        return false;
    }

    bool Canonicalize_Path_(const std::string& path, std::string* canonical) {
        char buffer[CONFIG_MAX_PATH];
        if (vfs::Canonicalize_Asset_Path(path.c_str(), buffer, sizeof(buffer)) == 0) {
            return false;
        }
        *canonical = buffer;
        return true;
    }

    std::string Directory_(const std::string& path) {
        const size_t slash = path.rfind('/');
        return slash == std::string::npos ? std::string() : path.substr(0, slash);
    }

    bool Resolve_Include_(const std::string& current_path, const s_cfg_token& token,
                          std::string* resolved) {
        const std::string directory = Directory_(current_path);
        const std::string combined = directory.empty() ? token.value :
                                     directory + "/" + token.value;
        return Canonicalize_Path_(combined, resolved);
    }

    bool Read_File_(s_loader* loader, const std::string& path,
                    const s_source_location& location, std::string* source) {
        s_vfs_file* file = vfs::fopen(path.c_str(), "rb");
        if (file == nullptr) {
            return Fail_(loader, location, "configuration file not found: '" + path + "'");
        }
        const int32_t size = vfs::fsize(file);
        if (size < 0 || static_cast<size_t>(size) > CONFIG_MAX_FILE_SIZE) {
            vfs::fclose(file);
            return Fail_(loader, location, "configuration file has an invalid size: '" + path + "'");
        }
        source->resize(static_cast<size_t>(size));
        const size_t count = source->empty() ? 0 :
                             vfs::fread(source->data(), 1, source->size(), file);
        const int32_t close_result = vfs::fclose(file);
        if (count != source->size() || close_result != 0) {
            return Fail_(loader, location, "unable to read configuration file: '" + path + "'");
        }
        return true;
    }

    std::string Include_Chain_(const s_loader* loader, size_t cycle_start,
                               const std::string& repeated_path) {
        std::string chain;
        for (size_t index = cycle_start; index < loader->stack.size(); ++index) {
            if (!chain.empty()) {
                chain += " -> ";
            }
            chain += loader->stack[index].path;
        }
        chain += " -> " + repeated_path;
        return chain;
    }

    bool Load_Recursive_(s_loader* loader, const std::string& path,
                         const s_source_location& include_location) {
        if (loader->stack.size() >= CONFIG_MAX_INCLUDE_DEPTH) {
            return Fail_(loader, include_location, "include depth exceeds 64 files");
        }
        for (size_t index = 0; index < loader->stack.size(); ++index) {
            if (loader->stack[index].path == path) {
                return Fail_(loader, include_location, "include cycle: " +
                             Include_Chain_(loader, index, path));
            }
        }

        std::string source;
        if (!Read_File_(loader, path, include_location, &source)) {
            return false;
        }
        s_cfg_document document;
        if (!Parse_Cfg(path.c_str(), source.data(), source.size(),
                       &document, loader->diagnostic)) {
            return false;
        }

        s_include_frame frame;
        frame.path = path;
        loader->stack.push_back(std::move(frame));
        for (s_cfg_statement& statement : document.statements) {
            if (statement.kind == CFG_STATEMENT_STOP) {
                break;
            }
            if (statement.kind == CFG_STATEMENT_INCLUDE) {
                std::string include_path;
                if (!Resolve_Include_(path, statement.argument, &include_path)) {
                    loader->stack.pop_back();
                    return Fail_(loader, statement.argument.location,
                                 "invalid include path '" + statement.argument.value + "'");
                }
                if (!Load_Recursive_(loader, include_path, statement.argument.location)) {
                    loader->stack.pop_back();
                    return false;
                }
            } else {
                loader->flattened.statements.push_back(std::move(statement));
            }
        }
        loader->stack.pop_back();
        return true;
    }
}

bool Load_Config_File(const char* entry_path, const char* package_id,
                      s_effective_config* effective, s_cfg_diagnostic* diagnostic) {
    if (entry_path == nullptr || package_id == nullptr ||
        effective == nullptr || diagnostic == nullptr) {
        return false;
    }
    *diagnostic = {};
    std::string normalized_entry;
    if (!Canonicalize_Path_(entry_path, &normalized_entry)) {
        diagnostic->location.source = entry_path;
        diagnostic->message = "invalid configuration entry path";
        return false;
    }

    s_loader loader;
    loader.diagnostic = diagnostic;
    s_source_location entry_location;
    entry_location.source = normalized_entry;
    if (!Load_Recursive_(&loader, normalized_entry, entry_location)) {
        return false;
    }
    return Apply_Config_Document(&loader.flattened, package_id, effective, diagnostic);
}

bool Load_Startup_Config(const char* entry_path, const char* package_id,
                         s_effective_config* effective, bool* loaded,
                         s_cfg_diagnostic* diagnostic) {
    if (entry_path == nullptr || package_id == nullptr || effective == nullptr ||
        loaded == nullptr || diagnostic == nullptr) {
        return false;
    }
    *loaded = false;
    *diagnostic = {};
    if (!Initialize_Effective_Config(effective)) {
        diagnostic->message = "unable to initialize configuration defaults";
        return false;
    }
    if (vfs::File_Exists(entry_path) == 0) {
        if (!Commit_State(&effective->state)) {
            diagnostic->message = "unable to commit configuration defaults";
            return false;
        }
        return true;
    }
    if (!Load_Config_File(entry_path, package_id, effective, diagnostic)) {
        return false;
    }
    *loaded = true;
    return true;
}

}