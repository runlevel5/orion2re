#include "deps.h"

namespace GEO {

    /**
     * Initializes the black_hole_blocks bit fields for all stars.
     * This field tracks which other stars are blocked from view by a black hole
     * relative to a given star, based on a proximity radius 'distance'.
     */
    void __cdecl Initialize_Black_Hole_Blocks_(int16_t distance) {
        int16_t black_hole_indices[72];
        int16_t num_black_holes = Collect_Black_Holes_(black_hole_indices);

        // Clear black_hole_blocks bitfield for all stars
        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            for (int k = 0; k < 9; ++k) {
                MOX::_star[i].black_hole_blocks[k] = 0;
            }
        }

        // For every pair of stars (i, j), check if any black hole lies on the path between them
        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            if (MOX::_star[i].spectral_class == STAR_CLASS_BLACK_HOLE) {
                continue;
            }

            int16_t x1 = MOX::_star[i].x;
            int16_t y1 = MOX::_star[i].y;

            for (int16_t j = 0; j < i; ++j) {
                if (MOX::_star[j].spectral_class == STAR_CLASS_BLACK_HOLE) {
                    continue;
                }

                int16_t x2 = MOX::_star[j].x;
                int16_t y2 = MOX::_star[j].y;

                for (int16_t k = 0; k < num_black_holes; ++k) {
                    int16_t bh_idx = black_hole_indices[k];
                    
                    if (Point_Within_D_Of_Seg_(MOX::_star[bh_idx].x, MOX::_star[bh_idx].y, distance, x1, y1, x2, y2)) {
                        // Mark path as blocked in both stars' bitfields
                        struct_::Set_Bit_Field_(MOX::_star[i].black_hole_blocks, (uint16_t)j);
                        struct_::Set_Bit_Field_(MOX::_star[j].black_hole_blocks, (uint16_t)i);
                        break;
                    }
                }
            }
        }
    }

    /**
     * @brief Iterates through all stars and updates their nebula status.
     * 
     * Semantically equivalent to GEO::Initialize_Star_In_Nebula_Info_ at 0x00412ba0.
     */
    void __cdecl Initialize_Star_In_Nebula_Info_() {
        if (MOX::_NUM_STARS > 0) {
            for (int32_t i = 0; i < static_cast<int32_t>(MOX::_NUM_STARS); ++i) {
                // The status is stored in the 'in_nebula' field (offset 0x6F)
                MOX::_star[i].in_nebula = Star_Is_In_Some_Nebula_(i);
            }
        }
    }

    /**
     * Calculates the distance in parsecs between two stars in the galaxy map.
     */
    int16_t __cdecl Parsecs_Between_Stars_(int16_t star_idx_1, int16_t star_idx_2) {
        return GEO::Parsecs_Between_Points_(
            MOX::_star[star_idx_1].x,
            MOX::_star[star_idx_1].y,
            MOX::_star[star_idx_2].x,
            MOX::_star[star_idx_2].y
        );
    }

    /**
     * @brief Calculates the distance in parsecs between two points (x1, y1) and (x2, y2).
     * 
     * The distance is calculated as ceil(sqrt((dx^2 + dy^2) / 900)).
     * Note: 900 corresponds to 30^2, implying 1 parsec = 30 coordinate units.
     */
    int16_t __cdecl Parsecs_Between_Points_(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
        int32_t dx = (int32_t)x1 - (int32_t)x2;
        int32_t dy = (int32_t)y1 - (int32_t)y2;
        uint32_t dist_sq = (uint32_t)(dx * dx + dy * dy);

        // Initial estimate for distance in parsecs
        uint32_t parsecs = isqrt_(dist_sq / 900);

        // Correction loop to ensure we return the ceiling of the square root
        // effectively calculating: ceil(sqrt(dist_sq / 900.0))
        while (parsecs * parsecs * 900 < dist_sq) {
            parsecs++;
        }

        return (int16_t)parsecs;
    }

    /**
     * Checks if a specific star is located within a nebula.
     * @param star_index The index into the global star array.
     */
    uint8_t __cdecl Star_Is_In_Some_Nebula_(int32_t star_index) {
        // Star data structures are 113 (0x71) bytes in size.
        // x is at offset 0xF (15), y is at offset 0x11 (17).
        const auto& star = MOX::_star[star_index];
        return Point_Is_In_Some_Nebula_(star.x, star.y);
    }

    bool __cdecl Point_Is_In_Some_Nebula_(int16_t x, int16_t y) {
        bool in_any_nebula = false;
        int16_t i = 0;

        while (true) {
            // Check if current index is within the number of active nebulas
            if (static_cast<int16_t>(MOX::_NUM_NEBULAS) <= i) {
                return in_any_nebula;
            }

            if (Point_Is_In_Nebula_N_(x, y, i)) {
                in_any_nebula = true;
            }

            i++;

            // If we found the point is in a nebula, break and return
            if (in_any_nebula) {
                break;
            }
        }

        return in_any_nebula;
    }

    /**
     * Scans all stars in the galaxy and collects the indices of those that are classified
     * as black holes into the provided buffer.
     * 
     * @param black_hole_indices Output buffer to store indices of black hole stars.
     */
    int16_t __cdecl Collect_Black_Holes_(int16_t* black_hole_indices) {
        int16_t black_hole_count = 0;

        for (int16_t i = 0; i < MOX::_NUM_STARS; ++i) {
            // STAR_CLASS_BLACK_HOLE is represented by value 6
            if (MOX::_star[i].spectral_class == STAR_CLASS_BLACK_HOLE) {
                black_hole_indices[black_hole_count] = i;
                black_hole_count++;
            }
        }

        return black_hole_count;
    }

} // namespace GEO