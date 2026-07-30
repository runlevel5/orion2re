#pragma once

#include <cstdint>

namespace MAP_SCALE {
    inline bool Uses_Extended_Scaling_(int16_t star_count, int16_t original_max_stars) {
        return star_count > original_max_stars;
    }

    inline int16_t Extended_Scale_For_Zoom_Level_(int16_t max_map_scale, int16_t max_zoom_level, int16_t zoom_level) {
        if (zoom_level < 0) {
            zoom_level = 0;
        }
        if (zoom_level >= max_zoom_level) {
            return max_map_scale;
        }

        int32_t scale = max_map_scale;
        for (int16_t level = max_zoom_level; level > zoom_level; --level) {
            scale = (scale + 1) / 2;
        }
        return static_cast<int16_t>(scale);
    }

    inline int16_t Extended_Zoom_Level_For_Scale_(int16_t map_scale, int16_t max_map_scale, int16_t max_zoom_level) {
        for (int16_t zoom_level = max_zoom_level; zoom_level > 0; --zoom_level) {
            if (map_scale >= Extended_Scale_For_Zoom_Level_(max_map_scale, max_zoom_level, zoom_level)) {
                return zoom_level;
            }
        }
        return 0;
    }

    inline int16_t Star_Scale_Percent_(int16_t star_count, int16_t original_max_stars, int16_t map_scale) {
        if (!Uses_Extended_Scaling_(star_count, original_max_stars) || map_scale <= 30) {
            return 100;
        }
        return static_cast<int16_t>(3000 / map_scale);
    }

    inline int16_t Scale_Star_Dimension_(int16_t dimension, int16_t scale_percent) {
        if (scale_percent >= 100) {
            return dimension;
        }

        dimension = static_cast<int16_t>((static_cast<int32_t>(dimension) * scale_percent) / 100);
        return dimension < 3 ? 3 : dimension;
    }

    inline bool Is_Extended_Max_Map_View_(
        int16_t star_count,
        int16_t original_max_stars,
        int16_t map_scale,
        int16_t max_map_scale
    ) {
        return Uses_Extended_Scaling_(star_count, original_max_stars) && map_scale == max_map_scale;
    }

    inline bool Advance_Black_Hole_Animation_(int16_t star_scale_percent) {
        return star_scale_percent >= 100;
    }
}