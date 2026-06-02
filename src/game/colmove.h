#pragma once
#include <cstdint>

namespace COLMOVE {
    extern int16_t _cluster_colony_n;
    extern uint8_t _g_hacky_dont_draw_cluster_flag;

    // Draws the currently selected population cluster near the cursor.
    // @param x Screen-space X position for the cluster widget.
    // @param y Screen-space Y position for the cluster widget.
    void __cdecl Draw_Cluster_(int16_t x, int16_t y);

    // Counts how many pops in the colony are still selected for reassignment.
    // @param colony_idx Colony index to scan.
    int16_t __cdecl Get_Num_Unemployed_(int16_t colony_idx);

    // Finds the first pop in the colony that is still selected for reassignment.
    // @param colony_idx Colony index to scan.
    int16_t __cdecl Get_First_Unemployed_(int16_t colony_idx);

    // Clears the current cluster selection and restores normal colony input state.
    void __cdecl Clear_Cluster_();

    // Selects all pops matching the starting pop for clustered reassignment.
    // @param colony_idx Colony index containing the selected pop.
    // @param start_pop_idx Pop index used as the cluster template.
    void __cdecl Get_Cluster_(int16_t colony_idx, int16_t start_pop_idx);

    // Checks whether two colony pops can be treated as the same reassignment group.
    // @param source_colony_idx Colony index for the first pop.
    // @param source_pop_idx Pop index for the first pop.
    // @param dest_colony_idx Colony index for the second pop.
    // @param dest_pop_idx Pop index for the second pop.
    bool __cdecl Pops_Identical_(int16_t source_colony_idx, int16_t source_pop_idx, int16_t dest_colony_idx, int16_t dest_pop_idx);

    // Applies the current cluster selection to a job change or transport destination.
    // @param colony_idx Destination colony index, or the source colony for job reassignment.
    // @param requested_job Target job id, or -1 to preserve the current job.
    void __cdecl Send_Cluster_(int16_t colony_idx, int16_t requested_job);

    // Builds a localized description string for a colony pop.
    // @param colony_idx Colony index containing the pop.
    // @param pop_idx Pop index to describe.
    // @param output_buffer Destination buffer for the formatted text.
    void __cdecl Colony_Pop_String_(int16_t colony_idx, int16_t pop_idx, char* output_buffer);

    // Changes a pop to a new job if colony rules allow it.
    // @param colony_idx Colony index containing the pop.
    // @param pop_idx Pop index to update.
    // @param new_job Target job id.
    uint8_t __cdecl Give_Colonist_New_Job_(int16_t colony_idx, int16_t pop_idx, int16_t new_job);
}