#include "pch.h"

#include "game/ship_config.h"

#define SHIP_WEAPON(type_, arc_, mods_, ammo_, amount_) \
    { (type_), (amount_), (amount_), (arc_), (mods_), (ammo_) }

#define ANTARAN_SMALL(s0_, s1_, s2_, s3_, s4_) \
    { .size = SHIP_SIZE_SMALL, .shield_type = SHIELD_NO_SHIELD, \
      .ftl_type = DRIVE_HYPER_DRIVE, .computer_type = COMPUTER_MOLECULARTRONIC_COMPUTER, \
      .armor_type = ARMOR_XENTRONIUM_ARMOR, .special_device_flags = {s0_, s1_, s2_, s3_, s4_}, \
      .ship_weapon = {SHIP_WEAPON(4, 1, 0, 0, 2), SHIP_WEAPON(24, 15, 0, 0, 1)}, \
      .picture_num = 0 }

#define ANTARAN_MEDIUM(s0_, s1_, s2_, s3_, s4_) \
    { .size = SHIP_SIZE_MEDIUM, .shield_type = SHIELD_NO_SHIELD, \
      .ftl_type = DRIVE_HYPER_DRIVE, .computer_type = COMPUTER_MOLECULARTRONIC_COMPUTER, \
      .armor_type = ARMOR_XENTRONIUM_ARMOR, .special_device_flags = {s0_, s1_, s2_, s3_, s4_}, \
      .ship_weapon = {SHIP_WEAPON(4, 1, 0, 0, 3), SHIP_WEAPON(4, 1, 2, 0, 1), \
                      SHIP_WEAPON(24, 15, 0, 10, 1)}, .picture_num = 1 }

#define ANTARAN_LARGE(s0_, s1_, s2_, s3_, s4_) \
    { .size = SHIP_SIZE_LARGE, .shield_type = SHIELD_NO_SHIELD, \
      .ftl_type = DRIVE_HYPER_DRIVE, .computer_type = COMPUTER_MOLECULARTRONIC_COMPUTER, \
      .armor_type = ARMOR_XENTRONIUM_ARMOR, .special_device_flags = {s0_, s1_, s2_, s3_, s4_}, \
      .ship_weapon = {SHIP_WEAPON(4, 1, 0, 0, 4), SHIP_WEAPON(4, 1, 2, 0, 2), \
                      SHIP_WEAPON(24, 15, 0, 10, 5), SHIP_WEAPON(13, 15, 0, 0, 2), \
                      SHIP_WEAPON(31, 15, 0, 0, 3)}, .picture_num = 2 }

#define ANTARAN_HUGE(s0_, s1_, s2_, s3_, s4_) \
    { .size = SHIP_SIZE_HUGE, .shield_type = SHIELD_NO_SHIELD, \
      .ftl_type = DRIVE_HYPER_DRIVE, .computer_type = COMPUTER_MOLECULARTRONIC_COMPUTER, \
      .armor_type = ARMOR_XENTRONIUM_ARMOR, .special_device_flags = {s0_, s1_, s2_, s3_, s4_}, \
      .ship_weapon = {SHIP_WEAPON(4, 1, 0, 0, 10), SHIP_WEAPON(4, 1, 2, 0, 4), \
                      SHIP_WEAPON(24, 1, 0, 10, 15), SHIP_WEAPON(13, 15, 0, 0, 2), \
                      SHIP_WEAPON(4, 1, 4, 0, 10), SHIP_WEAPON(11, 1, 0, 0, 4)}, \
      .picture_num = 3 }

#define ANTARAN_TITAN(s0_, s1_, s2_, s3_, s4_) \
    { .size = SHIP_SIZE_TITAN, .shield_type = SHIELD_NO_SHIELD, \
      .ftl_type = DRIVE_HYPER_DRIVE, .computer_type = COMPUTER_MOLECULARTRONIC_COMPUTER, \
      .armor_type = ARMOR_XENTRONIUM_ARMOR, .special_device_flags = {s0_, s1_, s2_, s3_, s4_}, \
      .ship_weapon = {SHIP_WEAPON(4, 1, 0, 0, 20), SHIP_WEAPON(4, 1, 2, 0, 5), \
                      SHIP_WEAPON(24, 1, 0, 10, 30), SHIP_WEAPON(13, 15, 0, 0, 3), \
                      SHIP_WEAPON(4, 1, 4, 0, 20), SHIP_WEAPON(11, 1, 2, 0, 5), \
                      SHIP_WEAPON(37, 15, 0, 0, 1), SHIP_WEAPON(37, 15, 0, 0, 12)}, \
      .picture_num = 4 }

namespace SHIP_CONFIG {

s_ship_design guardian_template = {
    .size = SHIP_SIZE_TITAN,
    .shield_type = SHIELD_CLASS_V_SHIELD,
    .ftl_type = DRIVE_NO_DRIVE,
    .computer_type = COMPUTER_MOLECULARTRONIC_COMPUTER,
    .armor_type = ARMOR_XENTRONIUM_ARMOR,
    .special_device_flags = {0x2A, 0x20, 0x2C, 0x02, 0x00},
    .ship_weapon = {
        SHIP_WEAPON(11, 15, 2, 0, 2),
        SHIP_WEAPON(4, 15, 4, 0, 10),
        SHIP_WEAPON(20, 15, 0x4500, 0, 1),
        SHIP_WEAPON(20, 15, 0x4500, 0, 1),
        SHIP_WEAPON(13, 15, 0, 0, 2),
    },
    .picture_num = 7,
};

s_ship_design avenger_template = {
    .size = SHIP_SIZE_HUGE,
    .shield_type = SHIELD_NO_SHIELD,
    .ftl_type = DRIVE_INTERPHASED_DRIVE,
    .computer_type = COMPUTER_MOLECULARTRONIC_COMPUTER,
    .armor_type = ARMOR_XENTRONIUM_ARMOR,
    .special_device_flags = {-128, 0x00, 0x00, 0x01, 0x02},
    .ship_weapon = {
        SHIP_WEAPON(4, 1, 2, 0, 4),
        SHIP_WEAPON(11, 1, 2, 0, 2),
        SHIP_WEAPON(4, 1, 0, 0, 10),
        SHIP_WEAPON(13, 15, 0, 0, 2),
        SHIP_WEAPON(4, 1, 4, 0, 10),
        SHIP_WEAPON(24, 1, 0, 10, 15),
    },
    .picture_num = 44,
};

s_ship_design amoeba_templates[2] = {
    { .size = SHIP_SIZE_LARGE, .shield_type = SHIELD_NO_SHIELD, .ftl_type = DRIVE_NO_DRIVE,
      .computer_type = COMPUTER_OPTRONIC_COMPUTER, .armor_type = ARMOR_NO_ARMOR,
      .special_device_flags = {0, 0, 0, 0, -128},
      .ship_weapon = {SHIP_WEAPON(45, 15, 0, 0, 1)}, .picture_num = 20 },
    { .size = SHIP_SIZE_HUGE, .shield_type = SHIELD_NO_SHIELD, .ftl_type = DRIVE_FUSION_DRIVE,
      .computer_type = COMPUTER_OPTRONIC_COMPUTER, .armor_type = ARMOR_NO_ARMOR,
      .special_device_flags = {0, 0, 0, 0, -128},
      .ship_weapon = {SHIP_WEAPON(45, 15, 0, 0, 2), SHIP_WEAPON(23, 15, 0, 10, 5)},
      .picture_num = 8 },
};

s_ship_design crystal_templates[2] = {
    { .size = SHIP_SIZE_LARGE, .shield_type = SHIELD_NO_SHIELD, .ftl_type = DRIVE_NO_DRIVE,
      .computer_type = COMPUTER_MOLECULARTRONIC_COMPUTER, .armor_type = ARMOR_NO_ARMOR,
      .ship_weapon = {SHIP_WEAPON(42, 15, 0, 0, 1), SHIP_WEAPON(25, 15, 0, 10, 5)},
      .picture_num = 21 },
    { .size = SHIP_SIZE_TITAN, .shield_type = SHIELD_NO_SHIELD, .ftl_type = DRIVE_ANTI_MATTER_DRIVE,
      .computer_type = COMPUTER_MOLECULARTRONIC_COMPUTER, .armor_type = ARMOR_NO_ARMOR,
      .ship_weapon = {SHIP_WEAPON(42, 15, 2, 0, 1), SHIP_WEAPON(26, 15, 0, 10, 5)},
      .picture_num = 9 },
};

s_ship_design dragon_templates[2] = {
    { .size = SHIP_SIZE_LARGE, .shield_type = SHIELD_NO_SHIELD, .ftl_type = DRIVE_NO_DRIVE,
      .computer_type = COMPUTER_MOLECULARTRONIC_COMPUTER, .armor_type = ARMOR_NO_ARMOR,
      .ship_weapon = {SHIP_WEAPON(41, 15, 4, 0, 8), SHIP_WEAPON(40, 15, 0, 0, 1)},
      .picture_num = 22 },
    { .size = SHIP_SIZE_TITAN, .shield_type = SHIELD_NO_SHIELD, .ftl_type = DRIVE_INTERPHASED_DRIVE,
      .computer_type = COMPUTER_MOLECULARTRONIC_COMPUTER, .armor_type = ARMOR_NO_ARMOR,
      .ship_weapon = {SHIP_WEAPON(41, 15, 4, 0, 20), SHIP_WEAPON(40, 15, 0x4000, 0, 1)},
      .picture_num = 10 },
};

s_ship_design eel_templates[2] = {
    { .size = SHIP_SIZE_LARGE, .shield_type = SHIELD_NO_SHIELD, .ftl_type = DRIVE_NO_DRIVE,
      .computer_type = COMPUTER_CYBERTRONIC_COMPUTER, .armor_type = ARMOR_NO_ARMOR,
      .special_device_flags = {0, 0, 0x08, 0, 0},
      .ship_weapon = {SHIP_WEAPON(44, 15, 0, 0, 1)}, .picture_num = 23 },
    { .size = SHIP_SIZE_HUGE, .shield_type = SHIELD_NO_SHIELD, .ftl_type = DRIVE_INTERPHASED_DRIVE,
      .computer_type = COMPUTER_CYBERTRONIC_COMPUTER, .armor_type = ARMOR_NO_ARMOR,
      .special_device_flags = {0, 0, 0x08, 0, 0},
      .ship_weapon = {SHIP_WEAPON(44, 15, 0, 0, 2)}, .picture_num = 11 },
};

s_ship_design hydra_templates[2] = {
    { .size = SHIP_SIZE_LARGE, .shield_type = SHIELD_NO_SHIELD, .ftl_type = DRIVE_NO_DRIVE,
      .computer_type = COMPUTER_OPTRONIC_COMPUTER, .armor_type = ARMOR_NO_ARMOR,
      .special_device_flags = {0, 0x04, 0, 0, 0},
      .ship_weapon = {SHIP_WEAPON(43, 15, 0, 0, 3)}, .picture_num = 24 },
    { .size = SHIP_SIZE_TITAN, .shield_type = SHIELD_NO_SHIELD, .ftl_type = DRIVE_FUSION_DRIVE,
      .computer_type = COMPUTER_OPTRONIC_COMPUTER, .armor_type = ARMOR_NO_ARMOR,
      .special_device_flags = {0, 0x04, 0, 0, 0},
      .ship_weapon = {SHIP_WEAPON(43, 15, 2, 0, 5)}, .picture_num = 12 },
};

s_ship_design antaran_templates[GAME_DIFFICULTY_COUNT][5] = {
    {
        ANTARAN_SMALL(-128, 0x00, 0x00, 0x01, 0x00),
        ANTARAN_MEDIUM(-128, 0x00, 0x00, 0x01, 0x00),
        ANTARAN_LARGE(-128, 0x00, 0x00, 0x01, 0x00),
        ANTARAN_HUGE(-128, 0x00, 0x00, 0x05, 0x02),
        ANTARAN_TITAN(-128, 0x02, 0x00, 0x05, 0x02),
    },
    {
        ANTARAN_SMALL(-128, 0x00, 0x00, 0x01, 0x00),
        ANTARAN_MEDIUM(-128, 0x00, 0x00, 0x01, 0x00),
        ANTARAN_LARGE(-128, 0x00, 0x00, 0x01, 0x00),
        ANTARAN_HUGE(-128, 0x02, 0x00, 0x05, 0x02),
        ANTARAN_TITAN(-128, 0x00, 0x20, 0x05, 0x02),
    },
    {
        ANTARAN_SMALL(-128, 0x00, 0x00, 0x01, 0x00),
        ANTARAN_MEDIUM(-128, 0x00, 0x00, 0x01, 0x00),
        ANTARAN_LARGE(-128, 0x02, 0x00, 0x01, 0x00),
        ANTARAN_HUGE(-128, 0x00, 0x20, 0x05, 0x02),
        ANTARAN_TITAN(-128, 0x00, 0x00, 0x05, 0x42),
    },
    {
        ANTARAN_SMALL(-128, 0x02, 0x00, 0x01, 0x00),
        ANTARAN_MEDIUM(-128, 0x02, 0x00, 0x01, 0x00),
        ANTARAN_LARGE(-128, 0x00, 0x20, 0x01, 0x00),
        ANTARAN_HUGE(-128, 0x00, 0x00, 0x05, 0x42),
        ANTARAN_TITAN(-128, 0x00, 0x00, 0x05, 0x42),
    },
    {
        ANTARAN_SMALL(-128, 0x02, 0x00, 0x01, 0x00),
        ANTARAN_MEDIUM(-128, 0x00, 0x20, 0x01, 0x00),
        ANTARAN_LARGE(-128, 0x00, 0x00, 0x01, 0x40),
        ANTARAN_HUGE(-128, 0x00, 0x00, 0x05, 0x42),
        ANTARAN_TITAN(-128, 0x00, 0x00, 0x05, 0x42),
    },
};

}

#undef ANTARAN_TITAN
#undef ANTARAN_HUGE
#undef ANTARAN_LARGE
#undef ANTARAN_MEDIUM
#undef ANTARAN_SMALL
#undef SHIP_WEAPON