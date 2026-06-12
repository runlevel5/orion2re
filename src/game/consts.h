#pragma once

#define MAX_PLAYERS 8
#define MAX_SHIPS 500
#define MAX_COLONIES 250
#define MAX_PLANETS 360
#define MAX_STARS 72
#define MAX_LEADERS 67
#define MAX_NEBULAS 4

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