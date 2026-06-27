#include "pch.h"

namespace TECHINIT {

namespace {
/**
 * Advances to the next non-empty NUL-terminated string in the loaded tech-name block.
 * @param tech_name_block Pointer to the contiguous string resource.
 * @param string_offset In/out byte offset within the resource.
 */
static char* Advance_To_Next_String_(char* tech_name_block, uint32_t& string_offset) {
    while (tech_name_block[string_offset] != '\0') {
        ++string_offset;
    }

    do {
        ++string_offset;
    } while (tech_name_block[string_offset] == '\0');

    return tech_name_block + string_offset;
}
}

void __cdecl Load_Tech_Names_() {
    LOADER::Load_Player_Special_Names_();
    RUSS::Mox_Update_();

    uint32_t string_offset = 0;
    char* tech_name_block = (char*)farload::Far_Reload_(
        "techname.lbx",
        (uint32_t)MOX::_settings.language,
        MOX::_techname_seg);

    RUSS::Mox_Update_();

    TECHDATA::_technology_fields[TECH_FIELD_STARTING_TECH].name = tech_name_block;
    for (uint32_t i = 1; i < TECH_FIELD_COUNT; ++i) {
        TECHDATA::_technology_fields[i].name =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < TECH_APP_COUNT; ++i) {
        TECHDATA::_technology_applications[i].name =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < BUILDING_COUNT; ++i) {
        TECHDATA::_buildings[i].name =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < SPECIAL_COUNT; ++i) {
        TECHDATA::_specials[i].name =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < ARMOR_COUNT; ++i) {
        TECHDATA::_armor[i].name =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < SHIELD_COUNT; ++i) {
        TECHDATA::_shields[i].name =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < WEAPON_COUNT; ++i) {
        TECHDATA::_weapons[i].name =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < COMPUTER_COUNT; ++i) {
        TECHDATA::_computers[i].name =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }

    for (uint32_t i = 0; i < DRIVE_COUNT; ++i) {
        TECHDATA::_drives[i].name =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }

    for (uint32_t i = 0; i < UNIT_COUNT; ++i) {
        TECHDATA::_units[i].name =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < FUEL_COUNT; ++i) {
        TECHDATA::_fuel[i].name =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }

    for (uint32_t i = 0; i < WEAPON_MOD_COUNT; ++i) {
        TECHDATA::_weapon_modifications[i].name =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < WEAPON_MOD_COUNT; ++i) {
        TECHDATA::_weapon_modifications[i].name_plural =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < WEAPON_COUNT; ++i) {
        TECHDATA::_weapons[i].name_plural =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < HULL_CLASS_COUNT; ++i) {
        TECHDATA::_hull_data[i].name =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }

    RUSS::Mox_Update_();

    for (uint32_t i = 0; i < HULL_CLASS_COUNT; ++i) {
        TECHDATA::_hull_data[i].size_name =
            Advance_To_Next_String_(tech_name_block, string_offset);
    }
}

int16_t __cdecl Tech_Is_Legal_For_Player_(int16_t player_idx, int16_t tech_id) {
    int16_t is_legal = 1;

    if (MOX::_player[player_idx].traits[TRAIT_CURRENT_GOVERNMENT] == 6) {
        if (tech_id == TECH_APP_HOLO_SIMULATOR ||
            tech_id == TECH_APP_VIRTUAL_REALITY_NETWORK ||
            tech_id == TECH_APP_PLEASURE_DOME) {
            is_legal = 0;
        }
    }

    if (MOX::_player[player_idx].traits[TRAIT_ENVIRONMENT_IMMUNE] != 0) {
        if (tech_id == TECH_APP_POLLUTION_PROCESSOR ||
            tech_id == TECH_APP_ATMOSPHERIC_RENEWER ||
            tech_id == TECH_APP_CORE_WASTE_DUMPS ||
            tech_id == TECH_APP_NANO_DISASSEMBLERS) {
            is_legal = 0;
        }
    }

    if (MOX::_player[player_idx].traits[TRAIT_EATS_MINERALS] != 0) {
        if (tech_id == TECH_APP_ANDROID_FARMERS ||
            tech_id == TECH_APP_BIOMORPHIC_FUNGI ||
            tech_id == TECH_APP_FOOD_REPLICATORS ||
            tech_id == TECH_APP_HYDROPONIC_FARM ||
            tech_id == TECH_APP_SOIL_ENRICHMENT ||
            tech_id == TECH_APP_SUBTERRANEAN_FARMS) {
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
  s_player* player = &MOX::_player[player_idx];
  int16_t starting_tech_count = 0;

  player->tech_fields[TECH_FIELD_ASTRO_BIOLOGY] = 2;
  player->tech_fields[TECH_FIELD_NUCLEAR_FISSION] = 2;
  player->tech_fields[TECH_FIELD_PHYSICS] = 2;
  player->tech_fields[TECH_FIELD_ENGINEERING] = 2;
  player->tech_fields[TECH_FIELD_ADVANCED_MAGNETISM] = 2;
  player->tech_fields[TECH_FIELD_CHEMISTRY] = 2;
  player->tech_fields[TECH_FIELD_ELECTRONICS] = 2;
  player->tech_fields[TECH_FIELD_MILITARY_TACTICS] = 2;

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
  default:
    break;
  }

  player->tech_fields[TECH_FIELD_STARTING_TECH] = 3;

  for (int16_t tech_app_id = 1; tech_app_id < TECH_APP_COUNT; ++tech_app_id) {
    int16_t tech_field_id = TECHDATA::_technology_applications[tech_app_id].tech_field_id;

    if (tech_field_id != TECH_FIELD_ADVANCED_GOVERNMENTS) {
      if (tech_field_id == TECH_FIELD_STARTING_TECH) {
        player->tech_applications[tech_app_id] = 3;
      } else if ((player->traits[TRAIT_UNCREATIVE] == 0 || tech_field_id == -1 ||
                  TECHDATA::_technology_fields[tech_field_id].cost == 0x32 ||
                  tech_field_id == 0x17) &&
                 TECHINIT::Tech_Is_Legal_For_Player_(player_idx, tech_app_id)) {
        player->tech_applications[tech_app_id] = 1;
      } else {
        player->tech_applications[tech_app_id] = 0;
      }

      if (tech_app_id == 0x34 && MOX::_settings.xenons_exist == 0) {
        player->tech_applications[tech_app_id] = 0;
      }
    }
  }

  for (int16_t tech_slot_idx = 0; tech_slot_idx < 4; ++tech_slot_idx) {
    int16_t tech_app_id =
        TECHDATA::_technology_fields[TECH_FIELD_ADVANCED_GOVERNMENTS].tech[tech_slot_idx];
    if (tech_app_id <= 0) {
      break;
    }

    player->tech_applications[tech_app_id] = 0;
  }

  switch (player->traits[TRAIT_CURRENT_GOVERNMENT]) {
  case 0:
    player->tech_applications[TECH_APP_CONFEDERATION] = 1;
    break;
  case 2:
    player->tech_applications[TECH_APP_IMPERIUM] = 1;
    break;
  case 4:
    player->tech_applications[TECH_APP_FEDERATION] = 1;
    break;
  case 6:
    player->tech_applications[TECH_APP_GALACTIC_UNIFICATION] = 1;
    player->tech_applications[TECH_APP_HOLO_SIMULATOR] = 0;
    player->tech_applications[TECH_APP_PLEASURE_DOME] = 0;
    player->tech_applications[TECH_APP_VIRTUAL_REALITY_NETWORK] = 0;
    break;
  }

  player->tech_applications[0] = 0;

  for (int16_t tech_field_id = 1; tech_field_id < TECH_FIELD_XENON_TECHNOLOGY; ++tech_field_id) {
    const int16_t* field_tech_ids = TECHDATA::_technology_fields[tech_field_id].tech;
    int16_t missing_count = 0;
    bool has_app = false;

    if (player->tech_fields[tech_field_id] == 0) {
      player->tech_fields[tech_field_id] = 1;
    }

    for (int16_t tech_slot_idx = 0; tech_slot_idx < 4; ++tech_slot_idx) {
      int16_t tech_app_id = field_tech_ids[tech_slot_idx];
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
        int16_t random_slot_idx = game_random::Random_(missing_count) - 1;
        tech_app_id = field_tech_ids[random_slot_idx];
      } while (!TECHINIT::Tech_Is_Legal_For_Player_(player_idx, tech_app_id));

      player->tech_applications[tech_app_id] = 1;
    }
  }

  player->tech_fields[TECH_FIELD_XENON_TECHNOLOGY] = 0;

  for (int16_t tech_field_id = TECH_FIELD_BIOLOGY; tech_field_id < TECH_FIELD_COUNT; ++tech_field_id) {
    if (player->tech_fields[tech_field_id] == 0) {
      player->tech_fields[tech_field_id] = 1;
    }
  }

  const bool restore_objectives = player->objectives == 'd';
  if (restore_objectives) {
    player->objectives = 1;
  }

  for (int16_t starting_tier_idx = 0; starting_tier_idx < starting_tech_count; ++starting_tier_idx) {
    if (starting_tier_idx < 6) {
      int16_t field_id = TECHDATA::_starting_tech_field_ids[starting_tier_idx];
      const int16_t* field_tech_ids = TECHDATA::_technology_fields[field_id].tech;

      player->tech_fields[field_id] = 3;
      player->tech_fields[TECHDATA::_technology_fields[field_id].next_field_id] = 2;

      for (int16_t tech_slot_idx = 0; tech_slot_idx < 4; ++tech_slot_idx) {
        int16_t tech_app_id = field_tech_ids[tech_slot_idx];
        if (tech_app_id == 0) {
          break;
        }

        if (TECHINIT::Tech_Is_Legal_For_Player_(player_idx, tech_app_id)) {
          player->tech_applications[tech_app_id] = 3;
        }
      }
    } else {
      int16_t selected_tech_app_id = newtech::Choose_Tech_Application_(player_idx);
      int16_t field_id = TECHDATA::_technology_applications[selected_tech_app_id].tech_field_id;
      const int16_t* field_tech_ids = TECHDATA::_technology_fields[field_id].tech;

      player->tech_fields[field_id] = 3;
      player->tech_fields[TECHDATA::_technology_fields[field_id].next_field_id] = 2;

      if (player->traits[TRAIT_CREATIVE] == 0) {
        player->tech_applications[selected_tech_app_id] = 3;
      } else {
        for (int16_t tech_slot_idx = 0; tech_slot_idx < 4; ++tech_slot_idx) {
          int16_t tech_app_id = field_tech_ids[tech_slot_idx];
          if (tech_app_id == 0) {
            break;
          }

          if (TECHINIT::Tech_Is_Legal_For_Player_(player_idx, tech_app_id)) {
            player->tech_applications[tech_app_id] = 3;
          }
        }
      }

      if (field_id == TECH_FIELD_ADVANCED_GOVERNMENTS) {
        ++player->traits[TRAIT_CURRENT_GOVERNMENT];
      }
    }
  }

  if (restore_objectives) {
    player->objectives = 'd';
  }

  player->best_ftl_drive = (char)INITSHIP::Best_Warp_Drive_(player_idx);
  player->ship_speed = (uint8_t)INITSHIP::Calc_Player_FTL_Speed_(player_idx);
}

void __cdecl Init_Tech_() {
    char error_message[40];

    RUSS::Mox_Update_();
    RUSS::Mox_Update_();

    for (int16_t building_index = 1; building_index < BUILDING_COUNT; ++building_index) {
        TECHDATA::_technology_applications[TECHDATA::_buildings[building_index].tech_app].type = 1;
    }

    RUSS::Mox_Update_();

    for (int16_t special_index = 1; special_index < SPECIAL_COUNT; ++special_index) {
        TECHDATA::_technology_applications[TECHDATA::_specials[special_index].tech_app_id].type = 9;
    }

    RUSS::Mox_Update_();

    for (int16_t armor_index = 1; armor_index < ARMOR_COUNT; ++armor_index) {
        TECHDATA::_technology_applications[TECHDATA::_armor[armor_index].tech_app_id].type = 7;
    }

    RUSS::Mox_Update_();

    for (int16_t shield_index = 1; shield_index < SHIELD_COUNT; ++shield_index) {
        TECHDATA::_technology_applications[TECHDATA::_shields[shield_index].tech_app_id].type = 2;
    }

    RUSS::Mox_Update_();

    for (int16_t weapon_index = 1; weapon_index < WEAPON_COUNT; ++weapon_index) {
        TECHDATA::_technology_applications[TECHDATA::_weapons[weapon_index].tech_app_id].type = 3;
    }

    RUSS::Mox_Update_();

    for (int16_t computer_index = 1; computer_index < COMPUTER_COUNT; ++computer_index) {
        TECHDATA::_technology_applications[TECHDATA::_computers[computer_index].tech_app_id].type = 8;
    }

    for (int16_t drive_index = 1; drive_index < DRIVE_COUNT; ++drive_index) {
        TECHDATA::_technology_applications[TECHDATA::_drives[drive_index].tech_app_id].type = 5;
    }

    for (int16_t fuel_index = 1; fuel_index < FUEL_COUNT; ++fuel_index) {
        TECHDATA::_technology_applications[TECHDATA::_fuel[fuel_index].tech_app_id].type = 4;
    }

    RUSS::Mox_Update_();

    for (int16_t tech_app_index = 0; tech_app_index < TECH_APP_COUNT; ++tech_app_index) {
        int16_t tech_field_id = TECHDATA::_technology_applications[tech_app_index].tech_field_id;

        if (tech_field_id != TECH_FIELD_STARTING_TECH && tech_field_id != -1 &&
            tech_field_id != TECH_FIELD_XENON_TECHNOLOGY) {
            s_tech_field_data& tech_field = TECHDATA::_technology_fields[tech_field_id];
            int16_t* tech_slot_ptrs[4] = {
                &tech_field.tech[0],
                &tech_field.tech[1],
                &tech_field.tech[2],
                &tech_field.tech[3],
            };
            int16_t free_slot_index = -1;

            for (int16_t slot_index = 0; slot_index < 4; ++slot_index) {
                if (*tech_slot_ptrs[slot_index] == 0) {
                    free_slot_index = slot_index;
                    break;
                }
            }

            if (free_slot_index == -1) {
                itoa(tech_app_index, error_message, 10);
                strlcat(error_message, " tech has too many apps", sizeof(error_message));
                exitm::Exit_With_Message(error_message);
            } else {
                *tech_slot_ptrs[free_slot_index] = tech_app_index;
            }
        }

        RUSS::Mox_Update_();
    }

    RUSS::Mox_Update_();
    TECHINIT::Load_Tech_Names_();
    RUSS::Mox_Update_();
}

}