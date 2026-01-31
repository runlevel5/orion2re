#include "deps.h"

namespace RUSS {
    int16_t __cdecl Get_Weighted_Choice_Char_(int8_t* weights, int16_t count) {
        uint32_t total_weight = 0;
        for (int16_t i = 0; i < count; ++i) {
            total_weight += weights[i];
        }

        if (total_weight == 0) {
            return -1;
        }

        uint16_t r = static_cast<uint16_t>(random::Random_(total_weight));
        int16_t index = 0;

        // Cumulative sum check to determine which index the random value falls into.
        if (weights[index] < r) {
            do {
                r -= static_cast<uint16_t>(weights[index]);
                index++;
            } while (weights[index] < r);
        }

        return index;
    }

    int16_t __cdecl Get_Weighted_Choice_Short_(const int16_t* weights, int16_t count) {
        uint32_t total_weight = 0;

        // Calculate the sum of all weights
        if (count > 0) {
            for (int16_t i = 0; i < count; ++i) {
                total_weight += static_cast<uint16_t>(weights[i]);
            }
        }

        if (total_weight == 0) {
            return -1;
        }

        // Obtain a random target value from [0, total_weight - 1] 
        // (Assuming Random_ behaves as such, or [1, total_weight])
        uint32_t random_val = static_cast<uint32_t>(random::Random_(static_cast<int32_t>(total_weight)));
        int16_t index = 0;

        // Perform weighted selection
        // Logic: Return the first index such that the sum of weights up to that index 
        // satisfies the random_val threshold.
        if (static_cast<uint16_t>(weights[index]) < random_val) {
            do {
                uint32_t current_weight = static_cast<uint16_t>(weights[index]);
                index++;
                random_val -= current_weight;
            } while (static_cast<uint16_t>(weights[index]) < random_val);
        }

        return index;
    }

    /**
     * @brief Releases all allocated blocks within a memory segment by resetting it.
     * 
     * This function effectively resets the memory segment provided, allowing it 
     * to be reused from the beginning. It calls the underlying allocation system 
     * to reset the segment.
     * 
     * @param seg Pointer to the memory segment to be released/reset.
     */
    void __cdecl Release_All_Blocks_(void* seg) {
        allocate::Allocate_First_Block_(seg, 1);
    }
}