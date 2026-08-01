#pragma once

#include "config/config_lifecycle.h"

namespace config {

bool Load_Config_File(const char* entry_path, const char* package_id,
                      s_effective_config* effective, s_cfg_diagnostic* diagnostic);
bool Load_Startup_Config(const char* entry_path, const char* package_id,
                         s_effective_config* effective, bool* loaded,
                         s_cfg_diagnostic* diagnostic);

}