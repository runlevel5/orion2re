#include "pch.h"

namespace TECHINIT {

void __cdecl Init_Player_Tech_(int16_t player_idx) {
  s_player *player = &MOX::_player[player_idx];
  int16_t starting_tech_count;

  player->tech_fields[0x12] = 2;
  player->tech_fields[0x37] = 2;
  player->tech_fields[0x39] = 2;
  player->tech_fields[0x1D] = 2;
  player->tech_fields[0x07] = 2;
  player->tech_fields[0x16] = 2;
  player->tech_fields[0x1C] = 2;
  player->tech_fields[0x0A] = 2;

  switch (MOX::_settings.starting_civilization_level) {
  case 0:
    starting_tech_count = 1;
    break;
  case 1:
    starting_tech_count = 6;
    break;
  case 2:
    starting_tech_count = 25;
    break;
  }

  player->tech_fields[0] = 3;

  for (int app_idx = 1; app_idx < 0xD4; ++app_idx) {
    int16_t tech_field_id = TECHDATA::_technology_applications[app_idx].tech_field_id;

    if (tech_field_id != 6) {
      if (tech_field_id == 0) {
        player->tech_applications[app_idx] = 3;
      } else if ((player->traits[0x15] == 0 || tech_field_id == -1 ||
                  TECHDATA::_technology_fields[tech_field_id].cost == 0x32 ||
                  tech_field_id == 0x17) &&
                 TECHINIT::Tech_Is_Legal_For_Player_(player_idx, static_cast<int16_t>(app_idx))) {
        player->tech_applications[app_idx] = 1;
      } else {
        player->tech_applications[app_idx] = 0;
      }

      if (app_idx == 0x34 && MOX::_settings.xenons_exist == 0) {
        player->tech_applications[app_idx] = 0;
      }
    }
  }

  switch (player->traits[0]) {
  case 0:
    player->tech_applications[0x2A] = 1;
    break;
  case 2:
    player->tech_applications[0x5C] = 1;
    break;
  case 4:
    player->tech_applications[0x41] = 1;
    break;
  case 6:
    player->tech_applications[0x4D] = 1;
    player->tech_applications[0x56] = 0;
    player->tech_applications[0x8D] = 0;
    player->tech_applications[0xC3] = 0;
    break;
  }

  player->tech_applications[0] = 0;

  for (int field_idx = 1; field_idx < 0x4A; ++field_idx) {
    const uint16_t *field_tech_ids = &TECHDATA::_technology_fields[field_idx].tech1;
    int missing_count = 0;
    bool has_app = false;

    if (player->tech_fields[field_idx] == 0) {
      player->tech_fields[field_idx] = 1;
    }

    for (int slot = 0; slot < 4; ++slot) {
      int16_t tech_app_id = static_cast<int16_t>(field_tech_ids[slot]);
      if (tech_app_id == 0) {
        break;
      }

      if (player->tech_applications[tech_app_id] == 0) {
        ++missing_count;
      } else {
        has_app = true;
      }
    }

    if (!has_app) {
      int16_t tech_app_id;
      do {
        int random_slot = random::Random_(missing_count) - 1;
        tech_app_id = static_cast<int16_t>(field_tech_ids[random_slot]);
      } while (!TECHINIT::Tech_Is_Legal_For_Player_(player_idx, tech_app_id));

      player->tech_applications[tech_app_id] = 1;
    }
  }

  player->tech_fields[0x4A] = 0;

  for (int field_idx = 0x4B; field_idx < 0x53; ++field_idx) {
    if (player->tech_fields[field_idx] == 0) {
      player->tech_fields[field_idx] = 1;
    }
  }

  const bool restore_objectives = player->objectives == 'd';
  if (restore_objectives) {
    player->objectives = 1;
  }

  for (int tier = 0; tier < starting_tech_count; ++tier) {
    if (tier < 6) {
      int16_t field_id = TECHDATA::_starting_tech_field_ids[tier];
      const uint16_t *field_tech_ids = &TECHDATA::_technology_fields[field_id].tech1;

      player->tech_fields[field_id] = 3;
      player->tech_fields[TECHDATA::_technology_fields[field_id].next_field_id] = 2;

      for (int slot = 0; slot < 4; ++slot) {
        int16_t tech_app_id = static_cast<int16_t>(field_tech_ids[slot]);
        if (tech_app_id == 0) {
          break;
        }

        if (TECHINIT::Tech_Is_Legal_For_Player_(player_idx, tech_app_id)) {
          player->tech_applications[tech_app_id] = 3;
        }
      }
    } else {
      int16_t selected_app = static_cast<int16_t>(newtech::Choose_Tech_Application_(player_idx));
      int16_t field_id = TECHDATA::_technology_applications[selected_app].tech_field_id;
      const uint16_t *field_tech_ids = &TECHDATA::_technology_fields[field_id].tech1;

      player->tech_fields[field_id] = 3;
      player->tech_fields[TECHDATA::_technology_fields[field_id].next_field_id] = 2;

      if (player->traits[0x16] == 0) {
        player->tech_applications[selected_app] = 3;
      } else {
        for (int slot = 0; slot < 4; ++slot) {
          int16_t tech_app_id = static_cast<int16_t>(field_tech_ids[slot]);
          if (tech_app_id == 0) {
            break;
          }

          if (TECHINIT::Tech_Is_Legal_For_Player_(player_idx, tech_app_id)) {
            player->tech_applications[tech_app_id] = 3;
          }
        }
      }

      if (field_id == 6) {
        ++player->traits[0];
      }
    }
  }

  if (restore_objectives) {
    player->objectives = 'd';
  }

  player->best_ftl_drive = static_cast<char>(INITSHIP::Best_Warp_Drive_(player_idx));
  player->ship_speed = static_cast<char>(INITSHIP::Calc_Player_FTL_Speed_(player_idx));
}

}