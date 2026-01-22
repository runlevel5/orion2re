#include "deps.h"

namespace FILEDEF {

    /**
     * @brief Loads game settings from "MOX.SET".
     * Reads exactly 0x229 bytes (553 bytes) into the global settings structure.
     */
    void __cdecl Load_Game_Settings_() {
        RUSS::Assert_Settings_();

        FILE* file = FID_conflict_wfopen("MOX.SET", "rb");
        if (file != nullptr) {
            // Read 553 bytes (size of s_settings up to version/seed block)
            _fread(&MOX::_settings, 0x229, 1, file);
            _fclose(file);
            RUSS::Assert_Settings_();
        }

        HAROLD::Set_Sound_Volume_To_Settings_();
    }

    bool Save_Game_Settings_(void) {
        RUSS::Assert_Settings_();

        // Open settings file in binary write mode
        FILE* file = FID_conflict_wfopen("MOX.SET", "wb");
        if (file == nullptr) {
            return false;
        }

        // Write the settings structure (size 0x229 = 553 bytes)
        _fwrite(&MOX::_settings, 0x229, 1, file);
        _fclose(file);

        // Re-assert state after saving
        RUSS::Assert_Settings_();

        // Redundant file closure per original binary assembly logic
        _fclose(file);

        return true;
    }


    void Update_Game_Settings_Version_Number_() {
        bool needs_reset = false;
        FILE* file = FID_conflict_wfopen("MOX.SET", "rb");

        if (file != nullptr) {
            _fclose(file);

            Load_Game_Settings_();

            // Set version to 0x82 (130)
            MOX::_settings.version = 0x82;

            // If saving fails, we reset settings
            if (Save_Game_Settings_() == 0) {
                needs_reset = true;
            }
        }
        else {
            // If settings file doesn't exist, we must reset
            needs_reset = true;
        }

        if (needs_reset) {
            INITGAME::Set_Default_Game_Settings_();
        }
    }
}