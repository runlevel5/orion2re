#pragma once

#define BITMAP(bit_count) (((bit_count) + 7) / 8)

#define MAX_PLAYERS 8
// Can't go higher than 127 due to int8_t. Change will make game binary-incompatible with previous version.
#define MAX_STARS 72
// Limited by int16_t ship indices and encoded ship locations. Change will make game binary-incompatible with previous version.
#define MAX_SHIPS 500
// Can't go higher than 255 due to uint8_t. Change will make game binary-incompatible with previous version.
#define MAX_COLONIES 250
#define MAX_PLANETS (MAX_STARS * 5)
#define MAX_LEADERS 67
#define MAX_NEBULAS 4
#define MAX_REPORTS 50
#define MAX_MESSAGES 100

#define ORIGINAL_MAX_STARS 72
#define ORIGINAL_MAX_SHIPS 500
#define SHIP_LOCATION_STEP 5000
#define SHIP_LOCATION_MOVING_OFFSET SHIP_LOCATION_STEP
#define SHIP_LOCATION_WORMHOLE_OFFSET (SHIP_LOCATION_STEP * 2)
#define SHIP_LOCATION_LIMIT (SHIP_LOCATION_STEP * 3)
#define SHIP_ICON_COUNT (MAX_SHIPS + MAX_STARS)
#define SHIP_LIST_CAPACITY (MAX_SHIPS + 1)

static_assert(MAX_STARS < SHIP_LOCATION_STEP, "encoded ship locations must reserve one destination for Antares");
static_assert(SHIP_LOCATION_LIMIT - 1 <= INT16_MAX, "encoded ship locations do not fit in int16_t");

#define GALAXY_SIZE_MAXIMUM 4
#if MAX_STARS > ORIGINAL_MAX_STARS
#define GALAXY_SIZE_COUNT 5
#else
#define GALAXY_SIZE_COUNT 4
#endif

#define COMBAT_GRID_WIDTH 81
#define COMBAT_GRID_HEIGHT 68
#define COMBAT_GRID_VIEW_WIDTH 32
#define COMBAT_GRID_VIEW_HEIGHT 18
#define COMBAT_GRID_VIEW_MAX_X (COMBAT_GRID_VIEW_WIDTH - 1)
#define COMBAT_GRID_VIEW_MAX_Y (COMBAT_GRID_VIEW_HEIGHT - 1)

inline constexpr char GAME_VERSION[] = "1.31-re";
inline constexpr char GAME_VERSION_LABEL[] = "Version 1.31-re";
inline constexpr char GAME_BUILD_DATE[] = "May 31 2026";

//#define FULLSCREEN
