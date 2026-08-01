#include "mod_manager.h"

#include <unordered_map>
#include <utility>
#include <vector>

#include "config/mod_manifest.h"
#include "game/vfs.h"

namespace config {
namespace {
    struct s_enabled_package {
        s_mod_manifest manifest;
        s_cfg_document document;
    };

    struct s_package_resolver {
        s_vfs_mount_transaction* mounts = nullptr;
        std::vector<s_enabled_package> packages;
        std::unordered_map<std::string, size_t> package_indexes;
        std::vector<std::string> stack;
        std::vector<std::string> requested_packages;
        s_cfg_diagnostic* diagnostic = nullptr;
        t_config_document_loader load_document = nullptr;
    };

    void Record_Package_Request_(s_package_resolver* resolver,
                                 const std::string& package_id) {
        for (const std::string& requested : resolver->requested_packages) {
            if (requested == package_id) {
                return;
            }
        }
        resolver->requested_packages.push_back(package_id);
    }

    void Normalize_Package_Priority_(s_package_resolver* resolver) {
        std::vector<s_enabled_package> ordered;
        ordered.reserve(resolver->packages.size());
        std::vector<bool> selected(resolver->packages.size(), false);
        for (const std::string& requested : resolver->requested_packages) {
            const auto found = resolver->package_indexes.find(requested);
            if (found != resolver->package_indexes.end()) {
                selected[found->second] = true;
                ordered.push_back(std::move(resolver->packages[found->second]));
            }
        }
        for (size_t index = 0; index < resolver->packages.size(); ++index) {
            if (!selected[index]) {
                ordered.push_back(std::move(resolver->packages[index]));
            }
        }
        resolver->packages = std::move(ordered);
        resolver->package_indexes.clear();
        for (size_t index = 0; index < resolver->packages.size(); ++index) {
            resolver->package_indexes.emplace(resolver->packages[index].manifest.id, index);
        }
    }

    std::string Package_Chain_(const s_package_resolver* resolver,
                               size_t cycle_start, const std::string& repeated_id) {
        std::string chain;
        for (size_t index = cycle_start; index < resolver->stack.size(); ++index) {
            if (!chain.empty()) {
                chain += " -> ";
            }
            chain += resolver->stack[index];
        }
        chain += " -> " + repeated_id;
        return chain;
    }

    bool Resolve_Package_(s_package_resolver* resolver, const std::string& package_id,
                          bool optional, const s_source_location& location);

    bool Resolve_Dependency_(s_package_resolver* resolver,
                             const s_mod_dependency& dependency, bool optional,
                             const s_source_location& location) {
        if (!Resolve_Package_(resolver, dependency.id, optional, location)) {
            return false;
        }
        const auto found = resolver->package_indexes.find(dependency.id);
        if (found == resolver->package_indexes.end()) {
            return true;
        }
        const s_mod_manifest& manifest = resolver->packages[found->second].manifest;
        if (!Semantic_Version_Matches(manifest.version, dependency.version)) {
            resolver->diagnostic->location = location;
            resolver->diagnostic->message = "package '" + dependency.id + "' version " +
                manifest.version_text + " does not satisfy '" + dependency.version + "'";
            return false;
        }
        return true;
    }

    bool Resolve_Package_(s_package_resolver* resolver, const std::string& package_id,
                          bool optional, const s_source_location& location) {
        for (size_t index = 0; index < resolver->stack.size(); ++index) {
            if (resolver->stack[index] == package_id) {
                resolver->diagnostic->location = location;
                resolver->diagnostic->message = "mod dependency cycle: " +
                    Package_Chain_(resolver, index, package_id);
                return false;
            }
        }
        if (resolver->package_indexes.contains(package_id)) {
            return true;
        }

        std::string mount_diagnostic;
        if (vfs::Stage_Package_Mount(resolver->mounts, package_id.c_str(),
                                     &mount_diagnostic) == 0) {
            if (optional && mount_diagnostic == "package not found: '" + package_id + "'") {
                return true;
            }
            resolver->diagnostic->location = location;
            resolver->diagnostic->message = mount_diagnostic;
            return false;
        }

        s_mod_manifest manifest;
        if (!Load_Mod_Manifest(package_id, &manifest, resolver->diagnostic)) {
            return false;
        }

        const size_t package_index = resolver->packages.size();
        resolver->package_indexes.emplace(package_id, package_index);
        resolver->packages.emplace_back();
        resolver->packages.back().manifest = std::move(manifest);
        resolver->stack.push_back(package_id);

        const std::vector<s_mod_dependency> dependencies =
            resolver->packages[package_index].manifest.dependencies;
        const std::vector<s_mod_dependency> optional_dependencies =
            resolver->packages[package_index].manifest.optional_dependencies;
        for (const s_mod_dependency& dependency : dependencies) {
            if (!Resolve_Dependency_(resolver, dependency, false, location)) {
                resolver->stack.pop_back();
                return false;
            }
        }
        for (const s_mod_dependency& dependency : optional_dependencies) {
            if (!Resolve_Dependency_(resolver, dependency, true, location)) {
                resolver->stack.pop_back();
                return false;
            }
        }
        resolver->stack.pop_back();
        return true;
    }

    bool Validate_Package_Set_(s_package_resolver* resolver) {
        for (size_t package_index = 0; package_index < resolver->packages.size();
             ++package_index) {
            const s_mod_manifest& manifest = resolver->packages[package_index].manifest;
            for (const std::string& conflict : manifest.conflicts) {
                if (resolver->package_indexes.contains(conflict)) {
                    resolver->diagnostic->location.source =
                        "mods/" + manifest.id + "/mod.json5";
                    resolver->diagnostic->message = "package '" + manifest.id +
                        "' conflicts with active package '" + conflict + "'";
                    return false;
                }
            }
            for (const std::string& lower_priority : manifest.load_after) {
                const auto found = resolver->package_indexes.find(lower_priority);
                if (found != resolver->package_indexes.end() &&
                    package_index > found->second) {
                    resolver->diagnostic->location.source =
                        "mods/" + manifest.id + "/mod.json5";
                    resolver->diagnostic->message = "package '" + manifest.id +
                        "' must load after '" + lower_priority + "'";
                    return false;
                }
            }
        }
        return true;
    }

    bool Resolve_Enabled_Packages_(const s_cfg_document& base_document,
                                   s_package_resolver* resolver) {
        for (const s_cfg_statement& statement : base_document.statements) {
            if (statement.kind == CFG_STATEMENT_ENABLE) {
                Record_Package_Request_(resolver, statement.argument.value);
                if (!Resolve_Package_(resolver, statement.argument.value, false,
                                      statement.argument.location)) {
                    return false;
                }
            }
        }

        for (size_t package_index = 0; package_index < resolver->packages.size();
             ++package_index) {
            const std::string package_id = resolver->packages[package_index].manifest.id;
            const std::string config_path = resolver->packages[package_index].manifest.config;
            const std::string entry_path = "mods/" + package_id + "/" + config_path;
            s_cfg_document document;
            if (!resolver->load_document(entry_path, &document, resolver->diagnostic)) {
                return false;
            }
            std::vector<std::pair<std::string, s_source_location>> nested_enables;
            for (const s_cfg_statement& statement : document.statements) {
                if (statement.kind == CFG_STATEMENT_ENABLE &&
                    !statement.argument.value.empty()) {
                    nested_enables.emplace_back(statement.argument.value,
                                                statement.argument.location);
                }
            }
            resolver->packages[package_index].document = std::move(document);
            for (const auto& enable : nested_enables) {
                Record_Package_Request_(resolver, enable.first);
                if (!Resolve_Package_(resolver, enable.first, false, enable.second)) {
                    return false;
                }
            }
        }

        Normalize_Package_Priority_(resolver);
        std::vector<std::string> package_order;
        package_order.reserve(resolver->packages.size());
        for (const s_enabled_package& package : resolver->packages) {
            package_order.push_back(package.manifest.id);
        }
        std::string order_diagnostic;
        if (vfs::Order_Package_Mounts(resolver->mounts, package_order,
                                      &order_diagnostic) == 0) {
            resolver->diagnostic->message = order_diagnostic.empty() ?
                "unable to order package mounts" : order_diagnostic;
            return false;
        }
        return Validate_Package_Set_(resolver);
    }
}

bool Apply_Config_With_Mods(const s_cfg_document& base_document,
                            const char* package_id,
                            s_effective_config* effective,
                            s_cfg_diagnostic* diagnostic,
                            t_config_document_loader load_document) {
    if (package_id == nullptr || effective == nullptr || diagnostic == nullptr ||
        load_document == nullptr) {
        return false;
    }

    s_vfs_mount_transaction* mounts = vfs::Begin_Mount_Transaction();
    if (mounts == nullptr) {
        diagnostic->message = "unable to begin VFS mount transaction";
        return false;
    }

    s_package_resolver resolver;
    resolver.mounts = mounts;
    resolver.diagnostic = diagnostic;
    resolver.load_document = load_document;
    if (!Resolve_Enabled_Packages_(base_document, &resolver)) {
        vfs::Discard_Mount_Transaction(mounts);
        return false;
    }

    s_effective_config candidate;
    if (!Initialize_Effective_Config(&candidate)) {
        diagnostic->message = "unable to initialize configuration defaults";
        vfs::Discard_Mount_Transaction(mounts);
        return false;
    }
    if (!Apply_Config_Document_To_Candidate(&base_document, package_id, &candidate,
                                            diagnostic)) {
        vfs::Discard_Mount_Transaction(mounts);
        return false;
    }
    for (size_t index = resolver.packages.size(); index > 0; --index) {
        const s_enabled_package& package = resolver.packages[index - 1];
        if (!Apply_Config_Document_To_Candidate(&package.document,
                                                package.manifest.id.c_str(),
                                                &candidate, diagnostic)) {
            vfs::Discard_Mount_Transaction(mounts);
            return false;
        }
    }

    if (!Commit_State(&candidate.state)) {
        diagnostic->message = "unable to commit configuration state";
        vfs::Discard_Mount_Transaction(mounts);
        return false;
    }
    if (vfs::Commit_Mount_Transaction(mounts) == 0) {
        diagnostic->message = "unable to commit VFS mount transaction";
        return false;
    }
    *effective = std::move(candidate);
    return true;
}

}