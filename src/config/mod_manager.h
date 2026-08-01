#pragma once

#include <string>

#include "config/config_lifecycle.h"

namespace config {

using t_config_document_loader = bool (*)(const std::string& entry_path,
                                          s_cfg_document* document,
                                          s_cfg_diagnostic* diagnostic);

bool Apply_Config_With_Mods(const s_cfg_document& base_document,
                            const char* package_id,
                            s_effective_config* effective,
                            s_cfg_diagnostic* diagnostic,
                            t_config_document_loader load_document);

}