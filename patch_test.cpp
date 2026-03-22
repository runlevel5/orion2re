#include <cstdint>

struct s_animation_header;

struct s_racescrn_mouse_group {
    int16_t x;             // 0x0
    int16_t y;             // 0x2
    int16_t width;         // 0x4
    int16_t height;        // 0x6
    int16_t spacing_x;     // 0x8
    int16_t field_A;       // 0xA
    int16_t count;         // 0xC
    int16_t field_E;       // 0xE
    int16_t sprite_width;  // 0x10
    int16_t field_12;      // 0x12
    struct s_animation_header* sprite1; // 0x14
    struct s_animation_header* sprite2; // 0x18
    int16_t spacing_y;     // 0x1C
    int16_t max_icons_per_row; // 0x1E
};
static_assert(sizeof(s_racescrn_mouse_group) == 32, "Size is not 32!");
