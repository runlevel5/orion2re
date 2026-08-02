#pragma once

#include "version.h"

#define BITMAP(bit_count) (((bit_count) + 7) / 8)

#define MAX_PLAYERS 8
#define MAX_STARS 1024
#define MAX_SHIPS 9000
#define MAX_PLANETS (MAX_STARS * 5)
#define MAX_COLONIES MAX_PLANETS
#define MAX_LEADERS 67
#define MAX_NEBULAS 4
#define MAX_REPORTS 50
#define MAX_MESSAGES 100

#define ORIGINAL_MAX_STARS 72
#define ORIGINAL_MAX_SHIPS 500
#define ORIGINAL_MAX_PLANETS 360
#define ORIGINAL_MAX_COLONIES 250
#define SHIP_LOCATION_STEP 10000
#define SHIP_LOCATION_MOVING_OFFSET SHIP_LOCATION_STEP
#define SHIP_LOCATION_WORMHOLE_OFFSET (SHIP_LOCATION_STEP * 2)
#define SHIP_LOCATION_LIMIT (SHIP_LOCATION_STEP * 3)
#define SHIP_ICON_COUNT (MAX_SHIPS + MAX_STARS)
#define SHIP_LIST_CAPACITY (MAX_SHIPS + 1)
#define SHIP_NODE_UI_TAG_BASE 10000

static_assert(MAX_STARS < SHIP_LOCATION_STEP, "encoded ship locations must reserve one destination for Antares");
static_assert(SHIP_LOCATION_LIMIT - 1 <= INT16_MAX, "encoded ship locations do not fit in int16_t");
static_assert(MAX_SHIPS <= SHIP_NODE_UI_TAG_BASE, "encoded UI ship nodes overlap real ship-node indices");
static_assert(SHIP_NODE_UI_TAG_BASE + MAX_SHIPS - 1 <= INT16_MAX, "encoded UI ship nodes do not fit in int16_t");

#define COMBAT_GRID_WIDTH 81
#define COMBAT_GRID_HEIGHT 68
#define COMBAT_GRID_VIEW_WIDTH 32
#define COMBAT_GRID_VIEW_HEIGHT 18
#define COMBAT_GRID_VIEW_MAX_X (COMBAT_GRID_VIEW_WIDTH - 1)
#define COMBAT_GRID_VIEW_MAX_Y (COMBAT_GRID_VIEW_HEIGHT - 1)

inline constexpr const char* GAME_VERSION = orion2re::ENGINE_VERSION;
inline constexpr char GAME_VERSION_LABEL[] = "Version 1.60.0";
inline constexpr char GAME_BUILD_DATE[] = "May 31 2026";

//#define FULLSCREEN
