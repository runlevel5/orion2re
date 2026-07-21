#pragma once

#include <cstdint>

namespace oracle {
    // Parses oracle replay options and enables replay mode.
    bool Configure_(int argc, char** argv);

    // Loads the requested turn and prepares replay after normal game initialization.
    bool Start_();

    // Injects the next due logical input event without compressing overdue event spacing.
    void Pump_Input_();

    // Saves and verifies the state produced by the completed turn.
    void Complete_Turn_();

    // Physical keyboard and mouse input is ignored while oracle mode is enabled.
    bool Physical_Input_Enabled_();
}