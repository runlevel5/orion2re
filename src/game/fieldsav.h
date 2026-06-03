#pragma once

namespace FIELDSAV {
    /** Saves the current field state into the provided save buffer.
     * @param data Destination structure that receives the current field state.
     */
    void __cdecl Save_Field_Stats_(s_fieldsav_data* data);
    /** Restores the current field state from the provided save buffer.
     * @param data Source structure that provides the field state to restore.
     */
    void __cdecl Restore_Field_Stats_(s_fieldsav_data* data);
}