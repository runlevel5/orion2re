#include "pch.h"

namespace TECHINIT {

namespace {
static char* Advance_To_Next_String_(char* string_block, uint32_t& string_offset) {
    while (string_block[string_offset] != '\0') {
        ++string_offset;
    }

    do {
        ++string_offset;
    } while (string_block[string_offset] == '\0');

    return string_block + string_offset;
}
}

void __cdecl Load_Tech_Names_() {
    LOADER::Load_Player_Special_Names_();
    RUSS::Mox_Update_();

    uint32_t string_offset = 0;
    char* string_block = static_cast<char*>(farload::Far_Reload_(
        s_techname_lbx_0055a96c,
        static_cast<uint32_t>(MOX::_settings_language),
        MOX::_techname_seg));

    RUSS::Mox_Update_();

    TECHDATA::_technology_fields[0].name = string_block;
    for (uint32_t i = 1; i < 83; ++i) {
        TECHDATA::_technology_fields[i].name =
            Advance_To_Next_String_(string_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < 212; ++i) {
        TECHDATA::_technology_applications[i].name =
            Advance_To_Next_String_(string_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < 49; ++i) {
        TECHDATA::_buildings[i].name =
            Advance_To_Next_String_(string_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < 40; ++i) {
        TECHDATA::_specials[i].name =
            Advance_To_Next_String_(string_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < 7; ++i) {
        TECHDATA::_armor[i].name =
            Advance_To_Next_String_(string_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < 6; ++i) {
        TECHDATA::_shields[i].name =
            Advance_To_Next_String_(string_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < 46; ++i) {
        TECHDATA::_weapons[i].name =
            Advance_To_Next_String_(string_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < 6; ++i) {
        TECHDATA::_computers[i].name =
            Advance_To_Next_String_(string_block, string_offset);
    }

    for (uint32_t i = 0; i < 7; ++i) {
        TECHDATA::_drives[i].name =
            Advance_To_Next_String_(string_block, string_offset);
    }

    for (uint32_t i = 0; i < 4; ++i) {
        TECHDATA::_units[i].name =
            Advance_To_Next_String_(string_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < 6; ++i) {
        TECHDATA::_fuel[i].name =
            Advance_To_Next_String_(string_block, string_offset);
    }

    for (uint32_t i = 0; i < 15; ++i) {
        TECHDATA::_weapon_modifications[i].name =
            Advance_To_Next_String_(string_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < 15; ++i) {
        TECHDATA::_weapon_modifications[i].name_plural =
            Advance_To_Next_String_(string_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < 46; ++i) {
        TECHDATA::_weapons[i].name_plural =
            Advance_To_Next_String_(string_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < 9; ++i) {
        TECHDATA::_hull_data[i].name =
            Advance_To_Next_String_(string_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < 9; ++i) {
        TECHDATA::_hull_data[i].size_name =
            Advance_To_Next_String_(string_block, string_offset);
    }
}

int16_t __cdecl Tech_Is_Legal_For_Player_(int16_t player_idx, int16_t tech_id) {
    int16_t is_legal = 1;

    if (MOX::_player[player_idx].traits[0] == 6) {
        if (tech_id == 0x56 || tech_id == 0xC3 || tech_id == 0x8D) {
            is_legal = 0;
        }
    }

    if (MOX::_player[player_idx].traits[23] != 0) {
        if (tech_id == 0x8E || tech_id == 0x13 || tech_id == 0x32 || tech_id == 0x71) {
            is_legal = 0;
        }
    }

    if (MOX::_player[player_idx].traits[18] != 0) {
        if (tech_id == 0x06 || tech_id == 0x1D || tech_id == 0x44 || tech_id == 0x57 || tech_id == 0xA2 || tech_id == 0xB2) {
            is_legal = 0;
        }
    }

    if (MOX::_settings.strategic_combat_flag == 1) {
        if (TECHDATA::_technology_applications[tech_id].in_strategic == 0) {
            is_legal = 0;
        }
    }

    if (tech_id == 0x34) {
        if (MOX::_settings.xenons_exist == 0) {
            is_legal = 0;
        }
    }

    return is_legal;
}

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
    const uint16_t *field_tech_ids = TECHDATA::_technology_fields[field_idx].tech;
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
      const uint16_t *field_tech_ids = TECHDATA::_technology_fields[field_id].tech;

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
      const uint16_t *field_tech_ids = TECHDATA::_technology_fields[field_id].tech;

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

void __cdecl Init_Tech_() {
    char message[40];

    RUSS::Mox_Update_();
    RUSS::Mox_Update_();

    for (int16_t building_index = 1; building_index < 49; ++building_index) {
        TECHDATA::_technology_applications[TECHDATA::_buildings[building_index].tech_app].type = 1;
    }

    RUSS::Mox_Update_();

    for (int16_t special_index = 1; special_index < 40; ++special_index) {
        TECHDATA::_technology_applications[TECHDATA::_specials[special_index].tech_app_id].type = 9;
    }

    RUSS::Mox_Update_();

    for (int16_t armor_index = 1; armor_index < 7; ++armor_index) {
        TECHDATA::_technology_applications[TECHDATA::_armor[armor_index].tech_app_id].type = 7;
    }

    RUSS::Mox_Update_();

    for (int16_t shield_index = 1; shield_index < 6; ++shield_index) {
        TECHDATA::_technology_applications[TECHDATA::_shields[shield_index].tech_app_id].type = 2;
    }

    RUSS::Mox_Update_();

    for (int16_t weapon_index = 1; weapon_index < 46; ++weapon_index) {
        TECHDATA::_technology_applications[TECHDATA::_weapons[weapon_index].tech_app_id].type = 3;
    }

    RUSS::Mox_Update_();

    for (int16_t computer_index = 1; computer_index < 6; ++computer_index) {
        TECHDATA::_technology_applications[TECHDATA::_computers[computer_index].tech_app_id].type = 8;
    }

    for (int16_t drive_index = 1; drive_index < 7; ++drive_index) {
        TECHDATA::_technology_applications[TECHDATA::_drives[drive_index].tech_app_id].type = 5;
    }

    for (int16_t fuel_index = 1; fuel_index < 6; ++fuel_index) {
        TECHDATA::_technology_applications[TECHDATA::_fuel[fuel_index].tech_app_id].type = 4;
    }

    RUSS::Mox_Update_();

    for (int16_t tech_app_index = 0; tech_app_index < 212; ++tech_app_index) {
        int16_t tech_field_id = TECHDATA::_technology_applications[tech_app_index].tech_field_id;

        if (tech_field_id != 0 && tech_field_id != -1 && tech_field_id != 0x4A) {
            s_tech_field_data& tech_field = TECHDATA::_technology_fields[tech_field_id];
            uint16_t* tech_slots[4] = {
                &tech_field.tech[0],
                &tech_field.tech[1],
                &tech_field.tech[2],
                &tech_field.tech[3],
            };
            int16_t free_slot = -1;

            for (int16_t slot_index = 0; slot_index < 4; ++slot_index) {
                if (*tech_slots[slot_index] == 0) {
                    free_slot = slot_index;
                    break;
                }
            }

            if (free_slot == -1) {
                __itoa(tech_app_index, message, 10);
                strcat(message, s_tech_has_too_many_apps_0055a97c);
                exitm::Exit_With_Message(message);
            } else {
                *tech_slots[free_slot] = static_cast<uint16_t>(tech_app_index);
            }
        }

        RUSS::Mox_Update_();
    }

    RUSS::Mox_Update_();
    TECHINIT::Load_Tech_Names_();
    RUSS::Mox_Update_();
}

}