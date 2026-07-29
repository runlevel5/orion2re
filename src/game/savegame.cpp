#include "pch.h"

#include "game/savegame.h"

#include <cstddef>

namespace SAVEGAME {
    namespace {
        constexpr uint16_t LEGACY_VERSION = 0;
        constexpr size_t SETTINGS_RESERVED_OFFSET = 0xE4;
        const uint8_t ZEROES[512] = {};

        static_assert(MAX_STARS == 72, "save version 0 requires 72 stars");
        static_assert(MAX_PLANETS == 360, "save version 0 requires 360 planets");
        static_assert(MAX_COLONIES == 250, "save version 0 requires 250 colonies");
        static_assert(ORIGINAL_MAX_SHIPS == 500, "save version 0 requires 500 ships");
        static_assert(MAX_SHIPS >= ORIGINAL_MAX_SHIPS, "ship capacity cannot load version 0 saves");
        static_assert(offsetof(s_settings, version) == 0xE2, "settings version offset mismatch");

        void Write_Zeroes_(SERIAL::Writer_* writer, size_t size) {
            while (size != 0) {
                size_t chunk = size < sizeof(ZEROES) ? size : sizeof(ZEROES);
                writer->Write_Bytes_(ZEROES, chunk);
                size -= chunk;
            }
        }

        void Write_Int16_Array_(SERIAL::Writer_* writer, const int16_t* values, size_t count) {
            for (size_t i = 0; i < count; ++i) writer->Write_Int16_(values[i]);
        }

        void Read_Int16_Array_(SERIAL::Reader_* reader, int16_t* values, size_t count) {
            for (size_t i = 0; i < count; ++i) values[i] = reader->Read_Int16_();
        }

        void Write_Uint16_Array_(SERIAL::Writer_* writer, const uint16_t* values, size_t count) {
            for (size_t i = 0; i < count; ++i) writer->Write_Uint16_(values[i]);
        }

        void Read_Uint16_Array_(SERIAL::Reader_* reader, uint16_t* values, size_t count) {
            for (size_t i = 0; i < count; ++i) values[i] = reader->Read_Uint16_();
        }

        void Write_Int32_Array_(SERIAL::Writer_* writer, const int32_t* values, size_t count) {
            for (size_t i = 0; i < count; ++i) writer->Write_Int32_(values[i]);
        }

        void Read_Int32_Array_(SERIAL::Reader_* reader, int32_t* values, size_t count) {
            for (size_t i = 0; i < count; ++i) values[i] = reader->Read_Int32_();
        }

        void Write_Settings_(SERIAL::Writer_* writer, const s_settings* settings) {
            writer->Write_Uint8_(settings->end_of_turn_summary);
            writer->Write_Uint8_(settings->end_of_turn_wait);
            writer->Write_Uint8_(settings->random_events);
            writer->Write_Uint8_(settings->enemy_moves);
            writer->Write_Uint8_(settings->expanding_help);
            writer->Write_Uint8_(settings->auto_select_ships);
            writer->Write_Uint8_(settings->animations_on);
            writer->Write_Uint8_(settings->auto_select_colony);
            writer->Write_Uint8_(settings->show_relocation_lines);
            writer->Write_Uint8_(settings->show_gnn_report);
            writer->Write_Uint8_(settings->auto_delete_tg_housing);
            writer->Write_Uint8_(settings->auto_saves);
            writer->Write_Uint8_(settings->only_show_serious_turn_summary_reports);
            writer->Write_Uint8_(settings->ship_initiative);
            writer->Write_Int8_(settings->tactical_combat_speed);
            writer->Write_Int8_(settings->message_delay);
            writer->Write_Uint8_(settings->field16_0x10);
            writer->Write_Uint8_(settings->sound_fx_on);
            writer->Write_Int8_(settings->sound_fx_level);
            writer->Write_Uint8_(settings->music_on);
            writer->Write_Int8_(settings->music_level);
            writer->Write_Int8_(settings->active_save_slot);
            writer->Write_Uint8_(settings->multi_player_game_type);
            writer->Write_Bytes_(settings->net_game_name, sizeof(settings->net_game_name));
            writer->Write_Bytes_(settings->online_settings.reserved_0x00, sizeof(settings->online_settings.reserved_0x00));
            writer->Write_Bytes_(settings->online_settings.endpoint, sizeof(settings->online_settings.endpoint));
            writer->Write_Bytes_(settings->online_settings.reserved_0x2f, sizeof(settings->online_settings.reserved_0x2f));
            writer->Write_Bytes_(settings->online_init_string, sizeof(settings->online_init_string));
            writer->Write_Bytes_(reinterpret_cast<const uint8_t*>(settings) + 0xAE, 5);
            writer->Write_Int8_(settings->online_initialized);
            writer->Write_Int8_(settings->combat_speedx2_flag);
            writer->Write_Int8_(settings->combat_legal_moves_flag);
            writer->Write_Int8_(settings->combat_missile_impact_flag);
            writer->Write_Int8_(settings->combat_shields_arc_flag);
            writer->Write_Int8_(settings->combat_grid_flag);
            writer->Write_Bytes_(reinterpret_cast<const uint8_t*>(settings) + 0xB9, 0x19);
            writer->Write_Uint8_(settings->language);
            writer->Write_Uint8_(settings->xenons_exist);
            writer->Write_Int8_(settings->game_difficulty);
            writer->Write_Int8_(settings->number_of_players);
            writer->Write_Int8_(settings->galaxy_size);
            writer->Write_Int8_(settings->galaxy_age);
            writer->Write_Uint8_(settings->strategic_combat_flag);
            writer->Write_Int8_(settings->starting_civilization_level);
            writer->Write_Uint32_(settings->start_of_game_seed);
            writer->Write_Int16_(settings->sound_driver_id);
            writer->Write_Int16_(settings->music_driver_id);
            writer->Write_Uint16_(settings->version);
            writer->Write_Bytes_(reinterpret_cast<const uint8_t*>(settings) + SETTINGS_RESERVED_OFFSET,
                                 sizeof(*settings) - SETTINGS_RESERVED_OFFSET);
        }

        void Read_Settings_(SERIAL::Reader_* reader, s_settings* settings) {
            settings->end_of_turn_summary = reader->Read_Uint8_();
            settings->end_of_turn_wait = reader->Read_Uint8_();
            settings->random_events = reader->Read_Uint8_();
            settings->enemy_moves = reader->Read_Uint8_();
            settings->expanding_help = reader->Read_Uint8_();
            settings->auto_select_ships = reader->Read_Uint8_();
            settings->animations_on = reader->Read_Uint8_();
            settings->auto_select_colony = reader->Read_Uint8_();
            settings->show_relocation_lines = reader->Read_Uint8_();
            settings->show_gnn_report = reader->Read_Uint8_();
            settings->auto_delete_tg_housing = reader->Read_Uint8_();
            settings->auto_saves = reader->Read_Uint8_();
            settings->only_show_serious_turn_summary_reports = reader->Read_Uint8_();
            settings->ship_initiative = reader->Read_Uint8_();
            settings->tactical_combat_speed = reader->Read_Int8_();
            settings->message_delay = reader->Read_Int8_();
            settings->field16_0x10 = reader->Read_Uint8_();
            settings->sound_fx_on = reader->Read_Uint8_();
            settings->sound_fx_level = reader->Read_Int8_();
            settings->music_on = reader->Read_Uint8_();
            settings->music_level = reader->Read_Int8_();
            settings->active_save_slot = reader->Read_Int8_();
            settings->multi_player_game_type = reader->Read_Uint8_();
            reader->Read_Bytes_(settings->net_game_name, sizeof(settings->net_game_name));
            reader->Read_Bytes_(settings->online_settings.reserved_0x00, sizeof(settings->online_settings.reserved_0x00));
            reader->Read_Bytes_(settings->online_settings.endpoint, sizeof(settings->online_settings.endpoint));
            reader->Read_Bytes_(settings->online_settings.reserved_0x2f, sizeof(settings->online_settings.reserved_0x2f));
            reader->Read_Bytes_(settings->online_init_string, sizeof(settings->online_init_string));
            reader->Read_Bytes_(reinterpret_cast<uint8_t*>(settings) + 0xAE, 5);
            settings->online_initialized = reader->Read_Int8_();
            settings->combat_speedx2_flag = reader->Read_Int8_();
            settings->combat_legal_moves_flag = reader->Read_Int8_();
            settings->combat_missile_impact_flag = reader->Read_Int8_();
            settings->combat_shields_arc_flag = reader->Read_Int8_();
            settings->combat_grid_flag = reader->Read_Int8_();
            reader->Read_Bytes_(reinterpret_cast<uint8_t*>(settings) + 0xB9, 0x19);
            settings->language = reader->Read_Uint8_();
            settings->xenons_exist = reader->Read_Uint8_();
            settings->game_difficulty = reader->Read_Int8_();
            settings->number_of_players = reader->Read_Int8_();
            settings->galaxy_size = reader->Read_Int8_();
            settings->galaxy_age = reader->Read_Int8_();
            settings->strategic_combat_flag = reader->Read_Uint8_();
            settings->starting_civilization_level = reader->Read_Int8_();
            settings->start_of_game_seed = reader->Read_Uint32_();
            settings->sound_driver_id = reader->Read_Int16_();
            settings->music_driver_id = reader->Read_Int16_();
            settings->version = reader->Read_Uint16_();
            reader->Read_Bytes_(reinterpret_cast<uint8_t*>(settings) + SETTINGS_RESERVED_OFFSET,
                                sizeof(*settings) - SETTINGS_RESERVED_OFFSET);
        }

        void Write_Ship_Weapons_(SERIAL::Writer_* writer, const s_ship_weapons* weapons) {
            writer->Write_Int16_(weapons->type);
            writer->Write_Int8_(weapons->count);
            writer->Write_Int8_(weapons->current_count);
            writer->Write_Int8_(weapons->firing_arc);
            writer->Write_Uint16_(weapons->specials);
            writer->Write_Int8_(weapons->ammo);
        }

        void Read_Ship_Weapons_(SERIAL::Reader_* reader, s_ship_weapons* weapons) {
            weapons->type = reader->Read_Int16_();
            weapons->count = reader->Read_Int8_();
            weapons->current_count = reader->Read_Int8_();
            weapons->firing_arc = reader->Read_Int8_();
            weapons->specials = reader->Read_Uint16_();
            weapons->ammo = reader->Read_Int8_();
        }

        void Write_Ship_Design_(SERIAL::Writer_* writer, const s_ship_design* design) {
            writer->Write_Bytes_(design->name, sizeof(design->name));
            writer->Write_Int8_(design->size);
            writer->Write_Int8_(design->ship_type);
            writer->Write_Int8_(design->shield_type);
            writer->Write_Int8_(design->ftl_type);
            writer->Write_Int8_(design->speed);
            writer->Write_Int8_(design->computer_type);
            writer->Write_Int8_(design->armor_type);
            writer->Write_Bytes_(design->special_device_flags, sizeof(design->special_device_flags));
            for (int i = 0; i < 8; ++i) Write_Ship_Weapons_(writer, &design->ship_weapon[i]);
            writer->Write_Int8_(design->picture_num);
            writer->Write_Uint8_(design->previous_owner);
            writer->Write_Int16_(design->cost);
            writer->Write_Uint8_(design->combat_speed);
            writer->Write_Uint16_(design->date_of_design);
        }

        void Read_Ship_Design_(SERIAL::Reader_* reader, s_ship_design* design) {
            reader->Read_Bytes_(design->name, sizeof(design->name));
            design->size = reader->Read_Int8_();
            design->ship_type = reader->Read_Int8_();
            design->shield_type = reader->Read_Int8_();
            design->ftl_type = reader->Read_Int8_();
            design->speed = reader->Read_Int8_();
            design->computer_type = reader->Read_Int8_();
            design->armor_type = reader->Read_Int8_();
            reader->Read_Bytes_(design->special_device_flags, sizeof(design->special_device_flags));
            for (int i = 0; i < 8; ++i) Read_Ship_Weapons_(reader, &design->ship_weapon[i]);
            design->picture_num = reader->Read_Int8_();
            design->previous_owner = reader->Read_Uint8_();
            design->cost = reader->Read_Int16_();
            design->combat_speed = reader->Read_Uint8_();
            design->date_of_design = reader->Read_Uint16_();
        }

        void Write_Colony_(SERIAL::Writer_* writer, const s_colony* colony) {
            writer->Write_Int8_(colony->owner);
            writer->Write_Int8_(colony->allocated_to);
            writer->Write_Int16_(colony->planet);
            writer->Write_Int16_(colony->officer);
            writer->Write_Uint8_(colony->outpost_flag);
            writer->Write_Int8_(colony->morale);
            writer->Write_Int16_(colony->pollution);
            writer->Write_Uint8_(colony->n_pops);
            writer->Write_Int8_(colony->specialty);
            for (int i = 0; i < 42; ++i) writer->Write_Uint32_(colony->pop[i]);
            Write_Int16_Array_(writer, colony->pop_roundoff, 10);
            Write_Int16_Array_(writer, colony->pop_growth, 10);
            writer->Write_Int8_(colony->n_turns_existed);
            writer->Write_Int8_(colony->food2_per_farmer);
            writer->Write_Int8_(colony->industry_per_worker);
            writer->Write_Int8_(colony->research_per_scientist);
            writer->Write_Uint8_(colony->max_farms);
            writer->Write_Int8_(colony->max_population);
            writer->Write_Uint8_(colony->climate);
            writer->Write_Int16_(colony->ground_strength);
            writer->Write_Int16_(colony->space_strength);
            Write_Int16_Array_(writer, colony->production, 4);
            writer->Write_Bytes_(colony->maintenance, sizeof(colony->maintenance));
            Write_Int16_Array_(writer, colony->imports, 4);
            writer->Write_Int8_(colony->n_industry_recyclers);
            writer->Write_Uint8_(colony->food2_needed_for_our_empire);
            writer->Write_Uint8_(colony->food2_needed_for_assimilated);
            writer->Write_Uint8_(colony->food2_needed_for_conquered);
            writer->Write_Uint8_(colony->food2_needed_for_natives);
            writer->Write_Uint8_(colony->industry2_needed_for_our_empire);
            writer->Write_Uint8_(colony->industry2_needed_for_androids);
            writer->Write_Uint8_(colony->industry2_needed_for_assimilated);
            writer->Write_Uint8_(colony->industry2_needed_for_conquered);
            writer->Write_Bytes_(colony->food2_needed_for_empire, sizeof(colony->food2_needed_for_empire));
            writer->Write_Bytes_(colony->industry2_needed_for_empire, sizeof(colony->industry2_needed_for_empire));
            writer->Write_Int8_(colony->n_food_replicated);
            Write_Int16_Array_(writer, colony->producing, 7);
            writer->Write_Int16_(colony->just_produced);
            writer->Write_Int16_(colony->production_spent);
            writer->Write_Int16_(colony->n_industry_taxed);
            writer->Write_Uint8_(colony->auto_building);
            writer->Write_Int16_(colony->production_surplus);
            writer->Write_Int16_(colony->bought_outright);
            writer->Write_Int8_(colony->occupation_points);
            writer->Write_Int8_(colony->occupation_policy);
            Write_Int16_Array_(writer, colony->military, 2);
            writer->Write_Int8_(colony->tank_roundoff);
            writer->Write_Int8_(colony->infantry_roundoff);
            writer->Write_Bytes_(colony->buildings, sizeof(colony->buildings));
            writer->Write_Uint16_(colony->last_turn_building_destroyed);
        }

        void Read_Colony_(SERIAL::Reader_* reader, s_colony* colony) {
            colony->owner = reader->Read_Int8_();
            colony->allocated_to = reader->Read_Int8_();
            colony->planet = reader->Read_Int16_();
            colony->officer = reader->Read_Int16_();
            colony->outpost_flag = reader->Read_Uint8_();
            colony->morale = reader->Read_Int8_();
            colony->pollution = reader->Read_Int16_();
            colony->n_pops = reader->Read_Uint8_();
            colony->specialty = reader->Read_Int8_();
            for (int i = 0; i < 42; ++i) colony->pop[i] = reader->Read_Uint32_();
            Read_Int16_Array_(reader, colony->pop_roundoff, 10);
            Read_Int16_Array_(reader, colony->pop_growth, 10);
            colony->n_turns_existed = reader->Read_Int8_();
            colony->food2_per_farmer = reader->Read_Int8_();
            colony->industry_per_worker = reader->Read_Int8_();
            colony->research_per_scientist = reader->Read_Int8_();
            colony->max_farms = reader->Read_Uint8_();
            colony->max_population = reader->Read_Int8_();
            colony->climate = reader->Read_Uint8_();
            colony->ground_strength = reader->Read_Int16_();
            colony->space_strength = reader->Read_Int16_();
            Read_Int16_Array_(reader, colony->production, 4);
            reader->Read_Bytes_(colony->maintenance, sizeof(colony->maintenance));
            Read_Int16_Array_(reader, colony->imports, 4);
            colony->n_industry_recyclers = reader->Read_Int8_();
            colony->food2_needed_for_our_empire = reader->Read_Uint8_();
            colony->food2_needed_for_assimilated = reader->Read_Uint8_();
            colony->food2_needed_for_conquered = reader->Read_Uint8_();
            colony->food2_needed_for_natives = reader->Read_Uint8_();
            colony->industry2_needed_for_our_empire = reader->Read_Uint8_();
            colony->industry2_needed_for_androids = reader->Read_Uint8_();
            colony->industry2_needed_for_assimilated = reader->Read_Uint8_();
            colony->industry2_needed_for_conquered = reader->Read_Uint8_();
            reader->Read_Bytes_(colony->food2_needed_for_empire, sizeof(colony->food2_needed_for_empire));
            reader->Read_Bytes_(colony->industry2_needed_for_empire, sizeof(colony->industry2_needed_for_empire));
            colony->n_food_replicated = reader->Read_Int8_();
            Read_Int16_Array_(reader, colony->producing, 7);
            colony->just_produced = reader->Read_Int16_();
            colony->production_spent = reader->Read_Int16_();
            colony->n_industry_taxed = reader->Read_Int16_();
            colony->auto_building = reader->Read_Uint8_();
            colony->production_surplus = reader->Read_Int16_();
            colony->bought_outright = reader->Read_Int16_();
            colony->occupation_points = reader->Read_Int8_();
            colony->occupation_policy = reader->Read_Int8_();
            Read_Int16_Array_(reader, colony->military, 2);
            colony->tank_roundoff = reader->Read_Int8_();
            colony->infantry_roundoff = reader->Read_Int8_();
            reader->Read_Bytes_(colony->buildings, sizeof(colony->buildings));
            colony->last_turn_building_destroyed = reader->Read_Uint16_();
        }

        void Write_Planet_(SERIAL::Writer_* writer, const s_planet_data* planet) {
            writer->Write_Int16_(planet->colony_index);
            writer->Write_Int16_(planet->star_index);
            writer->Write_Int8_(planet->orbit);
            writer->Write_Int8_(planet->planet_type);
            writer->Write_Int8_(planet->size);
            writer->Write_Int8_(planet->gravity_class);
            writer->Write_Int8_(planet->group);
            writer->Write_Int8_(planet->climate);
            writer->Write_Int8_(planet->climate_bg_type);
            writer->Write_Int8_(planet->mineral_class);
            writer->Write_Int8_(planet->food_per_farmer);
            writer->Write_Int8_(planet->n_terraforms);
            writer->Write_Uint8_(planet->max_farms);
            writer->Write_Int8_(planet->max_population);
            writer->Write_Int8_(planet->planet_special);
            writer->Write_Int8_(planet->environmental_alterations);
        }

        void Read_Planet_(SERIAL::Reader_* reader, s_planet_data* planet, uint16_t version) {
            planet->colony_index = reader->Read_Int16_();
            planet->star_index = version == LEGACY_VERSION ? reader->Read_Int8_() : reader->Read_Int16_();
            planet->orbit = reader->Read_Int8_();
            planet->planet_type = reader->Read_Int8_();
            planet->size = reader->Read_Int8_();
            planet->gravity_class = reader->Read_Int8_();
            planet->group = reader->Read_Int8_();
            planet->climate = reader->Read_Int8_();
            planet->climate_bg_type = reader->Read_Int8_();
            planet->mineral_class = reader->Read_Int8_();
            planet->food_per_farmer = reader->Read_Int8_();
            planet->n_terraforms = reader->Read_Int8_();
            planet->max_farms = reader->Read_Uint8_();
            planet->max_population = reader->Read_Int8_();
            planet->planet_special = reader->Read_Int8_();
            planet->environmental_alterations = reader->Read_Int8_();
        }

        void Write_Star_(SERIAL::Writer_* writer, const s_star_data* star) {
            writer->Write_Bytes_(star->name, sizeof(star->name));
            writer->Write_Int16_(star->x);
            writer->Write_Int16_(star->y);
            writer->Write_Int8_(star->size);
            writer->Write_Int8_(star->owner);
            writer->Write_Int8_(star->pict_type);
            writer->Write_Int8_(star->spectral_class);
            writer->Write_Bytes_(star->last_planet_selected, sizeof(star->last_planet_selected));
            writer->Write_Bytes_(star->black_hole_blocks, sizeof(star->black_hole_blocks));
            writer->Write_Int8_(star->system_special);
            writer->Write_Int16_(star->wormhole_star_id);
            writer->Write_Uint8_(star->blockaded);
            writer->Write_Bytes_(star->blockaded_by, sizeof(star->blockaded_by));
            writer->Write_Uint8_(star->visited);
            writer->Write_Uint8_(star->just_visited);
            writer->Write_Uint8_(star->ignore_colony_ships);
            writer->Write_Uint8_(star->ignore_combat_ships);
            writer->Write_Int8_(star->colonize_player);
            writer->Write_Uint8_(star->has_colony);
            writer->Write_Uint8_(star->has_warp_field_interdictor);
            writer->Write_Int16_(star->next_wfi_in_list);
            writer->Write_Uint8_(star->has_tachyon);
            writer->Write_Uint8_(star->has_subspace);
            writer->Write_Uint8_(star->has_stargate);
            writer->Write_Uint8_(star->has_jumpgate);
            writer->Write_Uint8_(star->has_artemis_net);
            writer->Write_Uint8_(star->has_dimensional_portal);
            writer->Write_Uint8_(star->is_stagepoint);
            writer->Write_Bytes_(star->officer_index, sizeof(star->officer_index));
            Write_Int16_Array_(writer, star->planet_index, 5);
            Write_Int16_Array_(writer, star->relocate_ship_to, MAX_PLAYERS);
            writer->Write_Int8_(star->twinkling1);
            writer->Write_Int8_(star->twinkling2);
            writer->Write_Uint8_(star->not_used3);
            writer->Write_Bytes_(star->surrender_to, sizeof(star->surrender_to));
            writer->Write_Uint8_(star->in_nebula);
            writer->Write_Uint8_(star->artifacts_gave_app);
        }

        void Read_Star_(SERIAL::Reader_* reader, s_star_data* star, uint16_t version) {
            reader->Read_Bytes_(star->name, sizeof(star->name));
            star->x = reader->Read_Int16_();
            star->y = reader->Read_Int16_();
            star->size = reader->Read_Int8_();
            star->owner = reader->Read_Int8_();
            star->pict_type = reader->Read_Int8_();
            star->spectral_class = reader->Read_Int8_();
            reader->Read_Bytes_(star->last_planet_selected, sizeof(star->last_planet_selected));
            reader->Read_Bytes_(star->black_hole_blocks, sizeof(star->black_hole_blocks));
            star->system_special = reader->Read_Int8_();
            star->wormhole_star_id = version == LEGACY_VERSION ? reader->Read_Int8_() : reader->Read_Int16_();
            star->blockaded = reader->Read_Uint8_();
            reader->Read_Bytes_(star->blockaded_by, sizeof(star->blockaded_by));
            star->visited = reader->Read_Uint8_();
            star->just_visited = reader->Read_Uint8_();
            star->ignore_colony_ships = reader->Read_Uint8_();
            star->ignore_combat_ships = reader->Read_Uint8_();
            star->colonize_player = reader->Read_Int8_();
            star->has_colony = reader->Read_Uint8_();
            star->has_warp_field_interdictor = reader->Read_Uint8_();
            star->next_wfi_in_list = version == LEGACY_VERSION ? reader->Read_Uint8_() : reader->Read_Int16_();
            star->has_tachyon = reader->Read_Uint8_();
            star->has_subspace = reader->Read_Uint8_();
            star->has_stargate = reader->Read_Uint8_();
            star->has_jumpgate = reader->Read_Uint8_();
            star->has_artemis_net = reader->Read_Uint8_();
            star->has_dimensional_portal = reader->Read_Uint8_();
            star->is_stagepoint = reader->Read_Uint8_();
            reader->Read_Bytes_(star->officer_index, sizeof(star->officer_index));
            Read_Int16_Array_(reader, star->planet_index, 5);
            Read_Int16_Array_(reader, star->relocate_ship_to, MAX_PLAYERS);
            star->twinkling1 = reader->Read_Int8_();
            star->twinkling2 = reader->Read_Int8_();
            star->not_used3 = reader->Read_Uint8_();
            reader->Read_Bytes_(star->surrender_to, sizeof(star->surrender_to));
            star->in_nebula = reader->Read_Uint8_();
            star->artifacts_gave_app = reader->Read_Uint8_();
        }

        void Write_Leader_(SERIAL::Writer_* writer, const s_leader_data* leader) {
            writer->Write_Bytes_(leader->name, sizeof(leader->name));
            writer->Write_Bytes_(leader->title, sizeof(leader->title));
            writer->Write_Uint8_(leader->type);
            writer->Write_Int16_(leader->xp);
            writer->Write_Uint32_(leader->general_skills);
            writer->Write_Uint32_(leader->special_skills);
            writer->Write_Bytes_(leader->tech_application, sizeof(leader->tech_application));
            writer->Write_Uint8_(leader->pict_num);
            writer->Write_Int16_(leader->skill_value);
            writer->Write_Uint8_(leader->level);
            writer->Write_Int16_(leader->location);
            writer->Write_Int8_(leader->eta);
            writer->Write_Char_(leader->display_level_popup);
            writer->Write_Int8_(leader->status);
            writer->Write_Int8_(leader->player_index);
        }

        void Read_Leader_(SERIAL::Reader_* reader, s_leader_data* leader) {
            reader->Read_Bytes_(leader->name, sizeof(leader->name));
            reader->Read_Bytes_(leader->title, sizeof(leader->title));
            leader->type = reader->Read_Uint8_();
            leader->xp = reader->Read_Int16_();
            leader->general_skills = reader->Read_Uint32_();
            leader->special_skills = reader->Read_Uint32_();
            reader->Read_Bytes_(leader->tech_application, sizeof(leader->tech_application));
            leader->pict_num = reader->Read_Uint8_();
            leader->skill_value = reader->Read_Int16_();
            leader->level = reader->Read_Uint8_();
            leader->location = reader->Read_Int16_();
            leader->eta = reader->Read_Int8_();
            leader->display_level_popup = reader->Read_Char_();
            leader->status = reader->Read_Int8_();
            leader->player_index = reader->Read_Int8_();
        }

        void Write_Player_(SERIAL::Writer_* writer, const s_player* player) {
            writer->Write_Int8_(player->picture);
            writer->Write_Bytes_(player->name, sizeof(player->name));
            writer->Write_Bytes_(player->race_name, sizeof(player->race_name));
            writer->Write_Uint8_(player->eliminated);
            writer->Write_Uint8_(player->race);
            writer->Write_Uint8_(player->color);
            writer->Write_Uint8_(player->personality);
            writer->Write_Uint8_(player->objectives);
            writer->Write_Int16_(player->home_planet_id);
            writer->Write_Int16_(player->network_player_id);
            writer->Write_Int8_(player->player_done_flags);
            writer->Write_Int16_(player->dead_field);
            writer->Write_Int8_(player->research_breakthrough);
            writer->Write_Uint8_(player->tax_rate);
            writer->Write_Int32_(player->bc);
            writer->Write_Int16_(player->n_freighters);
            writer->Write_Int16_(player->surplus_freighters);
            writer->Write_Int16_(player->command_points);
            writer->Write_Int16_(player->command_points_used);
            writer->Write_Int16_(player->food_freighted);
            writer->Write_Int16_(player->settlers_freighted);
            for (int i = 0; i < 25; ++i) writer->Write_Uint64_(player->settlers[i].val);
            writer->Write_Int16_(player->total_pop);
            writer->Write_Int16_(player->food_produced);
            writer->Write_Int16_(player->industry_produced);
            writer->Write_Int16_(player->research_produced);
            writer->Write_Int16_(player->bc_produced);
            writer->Write_Int16_(player->surplus_food);
            writer->Write_Int16_(player->surplus_bc);
            writer->Write_Int32_(player->total_maintenance);
            Write_Int16_Array_(writer, player->maintenance, 6);
            writer->Write_Bytes_(player->tech_fields, sizeof(player->tech_fields));
            writer->Write_Bytes_(player->tech_applications, sizeof(player->tech_applications));
            writer->Write_Int32_(player->research_accumulated);
            writer->Write_Int8_(player->captured_orion);
            writer->Write_Int8_(player->captured_antares);
            writer->Write_Int8_(player->won_council_vote);
            Write_Uint16_Array_(writer, player->player_defeated, MAX_PLAYERS);
            writer->Write_Int16_(player->captured_population);
            writer->Write_Int8_(player->last_attacker);
            writer->Write_Uint8_(player->ship_design_theme);
            writer->Write_Int8_(player->ship_special_theme);
            writer->Write_Bytes_(player->tech_apps_learned, sizeof(player->tech_apps_learned));
            writer->Write_Bytes_(player->tech_modifiers, sizeof(player->tech_modifiers));
            writer->Write_Bytes_(player->hyper_advanced_tech, sizeof(player->hyper_advanced_tech));
            writer->Write_Int32_(player->total_known_tech_cost);
            writer->Write_Int8_(player->knows_racial_stats);
            Write_Int32_Array_(writer, player->last_tech_report_date, MAX_PLAYERS);
            writer->Write_Bytes_(player->player_tech_applications, sizeof(player->player_tech_applications));
            writer->Write_Int8_(player->current_research_field);
            writer->Write_Int8_(player->current_research_application);
            writer->Write_Int8_(player->got_new_app);
            writer->Write_Int16_(player->ship_range);
            for (int i = 0; i < 6; ++i) Write_Ship_Design_(writer, &player->ship_designs[i]);
            Write_Int16_Array_(writer, player->ships_built, 6);
            writer->Write_Bytes_(player->contact, sizeof(player->contact));
            writer->Write_Int8_(player->just_established_contact);
            writer->Write_Bytes_(player->n_times_established_contact, sizeof(player->n_times_established_contact));
            writer->Write_Bytes_(player->within_range, sizeof(player->within_range));
            writer->Write_Int8_(player->do_not_research);
            writer->Write_Int16_(player->offensive_stagepoint);
            writer->Write_Int8_(player->best_ftl_drive);
            writer->Write_Uint8_(player->ship_speed);
            writer->Write_Int8_(player->officer_for_hire);
            writer->Write_Int16_(player->n_trade_pops);
            Write_Int16_Array_(writer, player->current_trade_agreement_level, MAX_PLAYERS);
            Write_Int16_Array_(writer, player->trade_agreement_goal, MAX_PLAYERS);
            writer->Write_Int16_(player->n_research_pops);
            Write_Int16_Array_(writer, player->current_research_agreement_level, MAX_PLAYERS);
            Write_Int16_Array_(writer, player->research_agreement_goal, MAX_PLAYERS);
            writer->Write_Int8_(player->fighter_beam_type);
            writer->Write_Int8_(player->fighter_bomb_type);
            writer->Write_Int16_(player->total_research);
            writer->Write_Int16_(player->total_ships);
            Write_Int32_Array_(writer, player->total_ship_strength, MAX_PLAYERS);
            writer->Write_Int16_(player->total_colonies);
            writer->Write_Int8_(player->failed_to_expand);
            writer->Write_Bytes_(player->first_contact, sizeof(player->first_contact));
            writer->Write_Bytes_(player->relations, sizeof(player->relations));
            writer->Write_Bytes_(player->base_relations, sizeof(player->base_relations));
            writer->Write_Bytes_(player->treaty, sizeof(player->treaty));
            writer->Write_Bytes_(player->trade_treaty, sizeof(player->trade_treaty));
            writer->Write_Bytes_(player->research_treaty, sizeof(player->research_treaty));
            Write_Int16_Array_(writer, player->tribute_treaty, MAX_PLAYERS);
            writer->Write_Bytes_(player->diplomacy_incident, sizeof(player->diplomacy_incident));
            writer->Write_Bytes_(player->diplomacy_message, sizeof(player->diplomacy_message));
            Write_Int16_Array_(writer, player->diplomacy_severity, MAX_PLAYERS);
            Write_Int16_Array_(writer, player->diplomacy_value, MAX_PLAYERS);
            Write_Int16_Array_(writer, player->diplomacy_system, MAX_PLAYERS);
            Write_Int16_Array_(writer, player->treaty_modifier, MAX_PLAYERS);
            Write_Int16_Array_(writer, player->trade_modifier, MAX_PLAYERS);
            Write_Int16_Array_(writer, player->tech_exchange_modifier, MAX_PLAYERS);
            Write_Int16_Array_(writer, player->peace_modifier, MAX_PLAYERS);
            writer->Write_Bytes_(player->last_bad_diplomatic_incident, sizeof(player->last_bad_diplomatic_incident));
            writer->Write_Bytes_(player->broken_treaties_modifier, sizeof(player->broken_treaties_modifier));
            writer->Write_Bytes_(player->last_broken_treaty, sizeof(player->last_broken_treaty));
            writer->Write_Bytes_(player->last_gift, sizeof(player->last_gift));
            Write_Int16_Array_(writer, player->ship_war_losses, MAX_PLAYERS);
            Write_Int16_Array_(writer, player->colony_war_losses, MAX_PLAYERS);
            writer->Write_Bytes_(player->biological_weapons_flag, sizeof(player->biological_weapons_flag));
            writer->Write_Bytes_(player->time_since_last_attack, sizeof(player->time_since_last_attack));
            writer->Write_Bytes_(player->threats, sizeof(player->threats));
            writer->Write_Bytes_(player->dishonored_flag, sizeof(player->dishonored_flag));
            writer->Write_Bytes_(player->peace_duration, sizeof(player->peace_duration));
            Write_Int16_Array_(writer, player->last_turn_diplomacy_severity, MAX_PLAYERS);
            writer->Write_Bytes_(player->last_broken_treaty_type, sizeof(player->last_broken_treaty_type));
            writer->Write_Bytes_(player->last_turn_treaty, sizeof(player->last_turn_treaty));
            writer->Write_Bytes_(player->diplomacy_proposal_request, sizeof(player->diplomacy_proposal_request));
            writer->Write_Bytes_(player->diplomacy_proposal_tribute_bribe, sizeof(player->diplomacy_proposal_tribute_bribe));
            writer->Write_Bytes_(player->diplomacy_proposal_tech_bribe1, sizeof(player->diplomacy_proposal_tech_bribe1));
            writer->Write_Bytes_(player->diplomacy_proposal_tech_bribe2, sizeof(player->diplomacy_proposal_tech_bribe2));
            Write_Int16_Array_(writer, player->diplomacy_proposal_gold_bribe, MAX_PLAYERS);
            writer->Write_Bytes_(player->diplomacy_proposal_war_player, sizeof(player->diplomacy_proposal_war_player));
            Write_Int32_Array_(writer, player->diplomacy_proposal_exchange_max_value, MAX_PLAYERS);
            writer->Write_Bytes_(player->diplomacy_proposal_exchange_tech, sizeof(player->diplomacy_proposal_exchange_tech));
            Write_Int16_Array_(writer, player->diplomacy_proposal_system_bribe, MAX_PLAYERS);
            writer->Write_Int16_(player->sneak_attack_planet);
            writer->Write_Int8_(player->sneak_attack_message);
            writer->Write_Int8_(player->sneak_attack_player);
            writer->Write_Int8_(player->council_threat);
            Write_Int16_Array_(writer, player->reward_amount, MAX_PLAYERS);
            writer->Write_Bytes_(player->reward_tech, sizeof(player->reward_tech));
            writer->Write_Bytes_(player->reward_attack_player, sizeof(player->reward_attack_player));
            writer->Write_Int16_(player->food_shortage_duration);
            writer->Write_Bytes_(player->trust_worthiness, sizeof(player->trust_worthiness));
            writer->Write_Bytes_(player->trust_breaker_player, sizeof(player->trust_breaker_player));
            writer->Write_Bytes_(player->trust_breaker_treaty, sizeof(player->trust_breaker_treaty));
            writer->Write_Bytes_(player->stop_spying_duration, sizeof(player->stop_spying_duration));
            writer->Write_Bytes_(player->stop_blockading_duration, sizeof(player->stop_blockading_duration));
            writer->Write_Int8_(player->war_buildup_duration);
            Write_Int16_Array_(writer, player->last_diplomacy_proposal_turn, MAX_PLAYERS);
            Write_Int16_Array_(writer, player->diplomacy_proposal_rejection, MAX_PLAYERS);
            Write_Int16_Array_(writer, player->opportunity_planet_num, MAX_PLAYERS);
            Write_Int16_Array_(writer, player->opportunity_planet_value, MAX_PLAYERS);
            Write_Int32_Array_(writer, player->opportunity_actual_attacker_ratio, MAX_PLAYERS);
            Write_Int16_Array_(writer, player->temp_diplomacy_dummy1, MAX_PLAYERS);
            writer->Write_Bytes_(player->diplomacy_demand_rejection_message, sizeof(player->diplomacy_demand_rejection_message));
            writer->Write_Bytes_(player->last_diplomacy_threat_turn, sizeof(player->last_diplomacy_threat_turn));
            writer->Write_Bytes_(player->delayed_diplomacy_orders, sizeof(player->delayed_diplomacy_orders));
            writer->Write_Bytes_(player->traits, sizeof(player->traits));
            writer->Write_Bytes_(player->evolutionary_upgrade_traits, sizeof(player->evolutionary_upgrade_traits));
            writer->Write_Int8_(player->picked_evolutionary_upgrade);
            writer->Write_Int8_(player->applied_evolutionary_upgrade);
            writer->Write_Bytes_(player->fleet_history, sizeof(player->fleet_history));
            writer->Write_Bytes_(player->tech_history, sizeof(player->tech_history));
            writer->Write_Bytes_(player->population_history, sizeof(player->population_history));
            writer->Write_Bytes_(player->production_history, sizeof(player->production_history));
            writer->Write_Bytes_(player->spies, sizeof(player->spies));
            writer->Write_Int8_(player->history_btns);
            writer->Write_Uint8_(player->ignoring);
            writer->Write_Bytes_(player->peace_flags, sizeof(player->peace_flags));
            writer->Write_Int8_(player->cheated);
            writer->Write_Uint16_(player->stardate_last_offered_hero);
            writer->Write_Int8_(player->current_zoom_level);
            writer->Write_Int16_(player->cur_map_x);
            writer->Write_Int16_(player->cur_map_y);
            writer->Write_Int8_(player->council_voted_for);
            writer->Write_Int8_(player->surrender_to);
            writer->Write_Int16_(player->time_since_last_lucky_event);
            writer->Write_Int8_(player->has_galactic_lore);
            writer->Write_Int8_(player->warned_about_cheating_player);
            writer->Write_Int8_(player->cheated_this_turn);
            writer->Write_Int32_(player->tribute_income);
            writer->Write_Bytes_(player->filler, sizeof(player->filler));
        }

        void Read_Player_(SERIAL::Reader_* reader, s_player* player, uint16_t version) {
            player->picture = reader->Read_Int8_();
            reader->Read_Bytes_(player->name, sizeof(player->name));
            reader->Read_Bytes_(player->race_name, sizeof(player->race_name));
            player->eliminated = reader->Read_Uint8_();
            player->race = reader->Read_Uint8_();
            player->color = reader->Read_Uint8_();
            player->personality = reader->Read_Uint8_();
            player->objectives = reader->Read_Uint8_();
            player->home_planet_id = reader->Read_Int16_();
            player->network_player_id = reader->Read_Int16_();
            player->player_done_flags = reader->Read_Int8_();
            player->dead_field = reader->Read_Int16_();
            player->research_breakthrough = reader->Read_Int8_();
            player->tax_rate = reader->Read_Uint8_();
            player->bc = reader->Read_Int32_();
            player->n_freighters = reader->Read_Int16_();
            player->surplus_freighters = reader->Read_Int16_();
            player->command_points = reader->Read_Int16_();
            player->command_points_used = reader->Read_Int16_();
            player->food_freighted = reader->Read_Int16_();
            player->settlers_freighted = reader->Read_Int16_();
            for (int i = 0; i < 25; ++i) {
                player->settlers[i].val = version == LEGACY_VERSION
                        ? SETTLER::From_Legacy(reader->Read_Uint32_())
                        : reader->Read_Uint64_();
            }
            player->total_pop = reader->Read_Int16_();
            player->food_produced = reader->Read_Int16_();
            player->industry_produced = reader->Read_Int16_();
            player->research_produced = reader->Read_Int16_();
            player->bc_produced = reader->Read_Int16_();
            player->surplus_food = reader->Read_Int16_();
            player->surplus_bc = reader->Read_Int16_();
            player->total_maintenance = reader->Read_Int32_();
            Read_Int16_Array_(reader, player->maintenance, 6);
            reader->Read_Bytes_(player->tech_fields, sizeof(player->tech_fields));
            reader->Read_Bytes_(player->tech_applications, sizeof(player->tech_applications));
            player->research_accumulated = reader->Read_Int32_();
            player->captured_orion = reader->Read_Int8_();
            player->captured_antares = reader->Read_Int8_();
            player->won_council_vote = reader->Read_Int8_();
            Read_Uint16_Array_(reader, player->player_defeated, MAX_PLAYERS);
            player->captured_population = reader->Read_Int16_();
            player->last_attacker = reader->Read_Int8_();
            player->ship_design_theme = reader->Read_Uint8_();
            player->ship_special_theme = reader->Read_Int8_();
            reader->Read_Bytes_(player->tech_apps_learned, sizeof(player->tech_apps_learned));
            reader->Read_Bytes_(player->tech_modifiers, sizeof(player->tech_modifiers));
            reader->Read_Bytes_(player->hyper_advanced_tech, sizeof(player->hyper_advanced_tech));
            player->total_known_tech_cost = reader->Read_Int32_();
            player->knows_racial_stats = reader->Read_Int8_();
            Read_Int32_Array_(reader, player->last_tech_report_date, MAX_PLAYERS);
            reader->Read_Bytes_(player->player_tech_applications, sizeof(player->player_tech_applications));
            player->current_research_field = reader->Read_Int8_();
            player->current_research_application = reader->Read_Int8_();
            player->got_new_app = reader->Read_Int8_();
            player->ship_range = reader->Read_Int16_();
            for (int i = 0; i < 6; ++i) Read_Ship_Design_(reader, &player->ship_designs[i]);
            Read_Int16_Array_(reader, player->ships_built, 6);
            reader->Read_Bytes_(player->contact, sizeof(player->contact));
            player->just_established_contact = reader->Read_Int8_();
            reader->Read_Bytes_(player->n_times_established_contact, sizeof(player->n_times_established_contact));
            reader->Read_Bytes_(player->within_range, sizeof(player->within_range));
            player->do_not_research = reader->Read_Int8_();
            player->offensive_stagepoint = version == LEGACY_VERSION ? reader->Read_Int8_() : reader->Read_Int16_();
            player->best_ftl_drive = reader->Read_Int8_();
            player->ship_speed = reader->Read_Uint8_();
            player->officer_for_hire = reader->Read_Int8_();
            player->n_trade_pops = reader->Read_Int16_();
            Read_Int16_Array_(reader, player->current_trade_agreement_level, MAX_PLAYERS);
            Read_Int16_Array_(reader, player->trade_agreement_goal, MAX_PLAYERS);
            player->n_research_pops = reader->Read_Int16_();
            Read_Int16_Array_(reader, player->current_research_agreement_level, MAX_PLAYERS);
            Read_Int16_Array_(reader, player->research_agreement_goal, MAX_PLAYERS);
            player->fighter_beam_type = reader->Read_Int8_();
            player->fighter_bomb_type = reader->Read_Int8_();
            player->total_research = reader->Read_Int16_();
            player->total_ships = reader->Read_Int16_();
            Read_Int32_Array_(reader, player->total_ship_strength, MAX_PLAYERS);
            player->total_colonies = reader->Read_Int16_();
            player->failed_to_expand = reader->Read_Int8_();
            reader->Read_Bytes_(player->first_contact, sizeof(player->first_contact));
            reader->Read_Bytes_(player->relations, sizeof(player->relations));
            reader->Read_Bytes_(player->base_relations, sizeof(player->base_relations));
            reader->Read_Bytes_(player->treaty, sizeof(player->treaty));
            reader->Read_Bytes_(player->trade_treaty, sizeof(player->trade_treaty));
            reader->Read_Bytes_(player->research_treaty, sizeof(player->research_treaty));
            Read_Int16_Array_(reader, player->tribute_treaty, MAX_PLAYERS);
            reader->Read_Bytes_(player->diplomacy_incident, sizeof(player->diplomacy_incident));
            reader->Read_Bytes_(player->diplomacy_message, sizeof(player->diplomacy_message));
            Read_Int16_Array_(reader, player->diplomacy_severity, MAX_PLAYERS);
            Read_Int16_Array_(reader, player->diplomacy_value, MAX_PLAYERS);
            Read_Int16_Array_(reader, player->diplomacy_system, MAX_PLAYERS);
            Read_Int16_Array_(reader, player->treaty_modifier, MAX_PLAYERS);
            Read_Int16_Array_(reader, player->trade_modifier, MAX_PLAYERS);
            Read_Int16_Array_(reader, player->tech_exchange_modifier, MAX_PLAYERS);
            Read_Int16_Array_(reader, player->peace_modifier, MAX_PLAYERS);
            reader->Read_Bytes_(player->last_bad_diplomatic_incident, sizeof(player->last_bad_diplomatic_incident));
            reader->Read_Bytes_(player->broken_treaties_modifier, sizeof(player->broken_treaties_modifier));
            reader->Read_Bytes_(player->last_broken_treaty, sizeof(player->last_broken_treaty));
            reader->Read_Bytes_(player->last_gift, sizeof(player->last_gift));
            Read_Int16_Array_(reader, player->ship_war_losses, MAX_PLAYERS);
            Read_Int16_Array_(reader, player->colony_war_losses, MAX_PLAYERS);
            reader->Read_Bytes_(player->biological_weapons_flag, sizeof(player->biological_weapons_flag));
            reader->Read_Bytes_(player->time_since_last_attack, sizeof(player->time_since_last_attack));
            reader->Read_Bytes_(player->threats, sizeof(player->threats));
            reader->Read_Bytes_(player->dishonored_flag, sizeof(player->dishonored_flag));
            reader->Read_Bytes_(player->peace_duration, sizeof(player->peace_duration));
            Read_Int16_Array_(reader, player->last_turn_diplomacy_severity, MAX_PLAYERS);
            reader->Read_Bytes_(player->last_broken_treaty_type, sizeof(player->last_broken_treaty_type));
            reader->Read_Bytes_(player->last_turn_treaty, sizeof(player->last_turn_treaty));
            reader->Read_Bytes_(player->diplomacy_proposal_request, sizeof(player->diplomacy_proposal_request));
            reader->Read_Bytes_(player->diplomacy_proposal_tribute_bribe, sizeof(player->diplomacy_proposal_tribute_bribe));
            reader->Read_Bytes_(player->diplomacy_proposal_tech_bribe1, sizeof(player->diplomacy_proposal_tech_bribe1));
            reader->Read_Bytes_(player->diplomacy_proposal_tech_bribe2, sizeof(player->diplomacy_proposal_tech_bribe2));
            Read_Int16_Array_(reader, player->diplomacy_proposal_gold_bribe, MAX_PLAYERS);
            reader->Read_Bytes_(player->diplomacy_proposal_war_player, sizeof(player->diplomacy_proposal_war_player));
            Read_Int32_Array_(reader, player->diplomacy_proposal_exchange_max_value, MAX_PLAYERS);
            reader->Read_Bytes_(player->diplomacy_proposal_exchange_tech, sizeof(player->diplomacy_proposal_exchange_tech));
            Read_Int16_Array_(reader, player->diplomacy_proposal_system_bribe, MAX_PLAYERS);
            player->sneak_attack_planet = reader->Read_Int16_();
            player->sneak_attack_message = reader->Read_Int8_();
            player->sneak_attack_player = reader->Read_Int8_();
            player->council_threat = reader->Read_Int8_();
            Read_Int16_Array_(reader, player->reward_amount, MAX_PLAYERS);
            reader->Read_Bytes_(player->reward_tech, sizeof(player->reward_tech));
            reader->Read_Bytes_(player->reward_attack_player, sizeof(player->reward_attack_player));
            player->food_shortage_duration = reader->Read_Int16_();
            reader->Read_Bytes_(player->trust_worthiness, sizeof(player->trust_worthiness));
            reader->Read_Bytes_(player->trust_breaker_player, sizeof(player->trust_breaker_player));
            reader->Read_Bytes_(player->trust_breaker_treaty, sizeof(player->trust_breaker_treaty));
            reader->Read_Bytes_(player->stop_spying_duration, sizeof(player->stop_spying_duration));
            reader->Read_Bytes_(player->stop_blockading_duration, sizeof(player->stop_blockading_duration));
            player->war_buildup_duration = reader->Read_Int8_();
            Read_Int16_Array_(reader, player->last_diplomacy_proposal_turn, MAX_PLAYERS);
            Read_Int16_Array_(reader, player->diplomacy_proposal_rejection, MAX_PLAYERS);
            Read_Int16_Array_(reader, player->opportunity_planet_num, MAX_PLAYERS);
            Read_Int16_Array_(reader, player->opportunity_planet_value, MAX_PLAYERS);
            Read_Int32_Array_(reader, player->opportunity_actual_attacker_ratio, MAX_PLAYERS);
            Read_Int16_Array_(reader, player->temp_diplomacy_dummy1, MAX_PLAYERS);
            reader->Read_Bytes_(player->diplomacy_demand_rejection_message, sizeof(player->diplomacy_demand_rejection_message));
            reader->Read_Bytes_(player->last_diplomacy_threat_turn, sizeof(player->last_diplomacy_threat_turn));
            reader->Read_Bytes_(player->delayed_diplomacy_orders, sizeof(player->delayed_diplomacy_orders));
            reader->Read_Bytes_(player->traits, sizeof(player->traits));
            reader->Read_Bytes_(player->evolutionary_upgrade_traits, sizeof(player->evolutionary_upgrade_traits));
            player->picked_evolutionary_upgrade = reader->Read_Int8_();
            player->applied_evolutionary_upgrade = reader->Read_Int8_();
            reader->Read_Bytes_(player->fleet_history, sizeof(player->fleet_history));
            reader->Read_Bytes_(player->tech_history, sizeof(player->tech_history));
            reader->Read_Bytes_(player->population_history, sizeof(player->population_history));
            reader->Read_Bytes_(player->production_history, sizeof(player->production_history));
            reader->Read_Bytes_(player->spies, sizeof(player->spies));
            player->history_btns = reader->Read_Int8_();
            player->ignoring = reader->Read_Uint8_();
            reader->Read_Bytes_(player->peace_flags, sizeof(player->peace_flags));
            player->cheated = reader->Read_Int8_();
            player->stardate_last_offered_hero = reader->Read_Uint16_();
            player->current_zoom_level = reader->Read_Int8_();
            player->cur_map_x = reader->Read_Int16_();
            player->cur_map_y = reader->Read_Int16_();
            player->council_voted_for = reader->Read_Int8_();
            player->surrender_to = reader->Read_Int8_();
            player->time_since_last_lucky_event = reader->Read_Int16_();
            player->has_galactic_lore = reader->Read_Int8_();
            player->warned_about_cheating_player = reader->Read_Int8_();
            player->cheated_this_turn = reader->Read_Int8_();
            player->tribute_income = reader->Read_Int32_();
            reader->Read_Bytes_(player->filler, sizeof(player->filler));
        }

        void Write_Ship_(SERIAL::Writer_* writer, const s_ship_data* ship) {
            Write_Ship_Design_(writer, &ship->d);
            writer->Write_Int8_(ship->owner);
            writer->Write_Int8_(ship->status);
            writer->Write_Int16_(ship->location);
            writer->Write_Int16_(ship->x);
            writer->Write_Int16_(ship->y);
            writer->Write_Int8_(ship->group_has_navigator);
            writer->Write_Int8_(ship->travelling_speed);
            writer->Write_Uint8_(ship->turns_left);
            writer->Write_Int8_(ship->shield_damage_percent);
            writer->Write_Int8_(ship->drive_damage_percent);
            writer->Write_Int8_(ship->computer_damage);
            writer->Write_Int8_(ship->crew_quality);
            writer->Write_Int16_(ship->crew_experience);
            writer->Write_Int16_(ship->officer_index);
            writer->Write_Bytes_(ship->special_device_damage_flags, sizeof(ship->special_device_damage_flags));
            writer->Write_Int16_(ship->armor_damage);
            writer->Write_Int16_(ship->structural_damage);
            writer->Write_Int8_(ship->mission);
            writer->Write_Int8_(ship->just_built);
        }

        void Read_Ship_(SERIAL::Reader_* reader, s_ship_data* ship, uint16_t version) {
            Read_Ship_Design_(reader, &ship->d);
            ship->owner = reader->Read_Int8_();
            ship->status = reader->Read_Int8_();
            ship->location = reader->Read_Int16_();
            if (version == LEGACY_VERSION) {
                ship->location = Convert_Legacy_Ship_Location_(ship->location);
            }
            ship->x = reader->Read_Int16_();
            ship->y = reader->Read_Int16_();
            ship->group_has_navigator = reader->Read_Int8_();
            ship->travelling_speed = reader->Read_Int8_();
            ship->turns_left = reader->Read_Uint8_();
            ship->shield_damage_percent = reader->Read_Int8_();
            ship->drive_damage_percent = reader->Read_Int8_();
            ship->computer_damage = reader->Read_Int8_();
            ship->crew_quality = reader->Read_Int8_();
            ship->crew_experience = reader->Read_Int16_();
            ship->officer_index = reader->Read_Int16_();
            reader->Read_Bytes_(ship->special_device_damage_flags, sizeof(ship->special_device_damage_flags));
            ship->armor_damage = reader->Read_Int16_();
            ship->structural_damage = reader->Read_Int16_();
            ship->mission = reader->Read_Int8_();
            ship->just_built = reader->Read_Int8_();
        }

        void Write_Movable_Box_(SERIAL::Writer_* writer, const s_movable_box* box) {
            writer->Write_Int16_(box->index);
            writer->Write_Int16_(box->id);
            writer->Write_Int16_(box->height);
            writer->Write_Int16_(box->width);
            writer->Write_Int16_(box->x);
            writer->Write_Int16_(box->y);
            writer->Write_Int16_(box->active);
            writer->Write_Int16_(box->drag_field);
            writer->Write_Int16_(box->close_btn);
            writer->Write_Int16_(box->window_field_id);
            writer->Write_Int16_(box->unselected_width);
            writer->Write_Int16_(box->unselected_height);
            writer->Write_Int16_(box->content_offset_x);
            writer->Write_Int16_(box->content_offset_y);
        }

        void Read_Movable_Box_(SERIAL::Reader_* reader, s_movable_box* box) {
            box->index = reader->Read_Int16_();
            box->id = reader->Read_Int16_();
            box->height = reader->Read_Int16_();
            box->width = reader->Read_Int16_();
            box->x = reader->Read_Int16_();
            box->y = reader->Read_Int16_();
            box->active = reader->Read_Int16_();
            box->drag_field = reader->Read_Int16_();
            box->close_btn = reader->Read_Int16_();
            box->window_field_id = reader->Read_Int16_();
            box->unselected_width = reader->Read_Int16_();
            box->unselected_height = reader->Read_Int16_();
            box->content_offset_x = reader->Read_Int16_();
            box->content_offset_y = reader->Read_Int16_();
        }

        void Write_Nebula_(SERIAL::Writer_* writer, const s_nebula* nebula) {
            writer->Write_Int16_(nebula->x);
            writer->Write_Int16_(nebula->y);
            writer->Write_Int8_(nebula->type);
        }

        void Read_Nebula_(SERIAL::Reader_* reader, s_nebula* nebula) {
            nebula->x = reader->Read_Int16_();
            nebula->y = reader->Read_Int16_();
            nebula->type = reader->Read_Int8_();
        }

        void Write_Report_(SERIAL::Writer_* writer, const s_report* report) {
            writer->Write_Uint8_(report->type);
            writer->Write_Int16_(report->player_idx);
            writer->Write_Uint8_(report->action);
            if (report->type == 1 && report->action == 0) {
                writer->Write_Int8_(report->d.steal.framed_player_idx);
                writer->Write_Uint8_(report->d.steal.victim_player_idx);
                writer->Write_Int16_(report->d.steal.tech_app_id);
                Write_Zeroes_(writer, 1);
            } else if (report->type == 1 && report->action == 1) {
                writer->Write_Int8_(report->d.sabotage.framed_player_idx);
                writer->Write_Uint8_(report->d.sabotage.victim_player_idx);
                writer->Write_Int16_(report->d.sabotage.planet_idx);
                writer->Write_Uint8_(report->d.sabotage.building_id);
            } else if (report->type == 2 && report->action == 0) {
                writer->Write_Int16_(report->d.enemy_steal.enemy_player_idx);
                writer->Write_Int16_(report->d.enemy_steal.tech_app_id);
                Write_Zeroes_(writer, 1);
            } else if (report->type == 2 && report->action == 1) {
                writer->Write_Int16_(report->d.enemy_sabotage.enemy_player_idx);
                writer->Write_Int16_(report->d.enemy_sabotage.planet_idx);
                writer->Write_Uint8_(report->d.enemy_sabotage.building_id);
            } else {
                writer->Write_Bytes_(&report->d, sizeof(report->d));
            }
            writer->Write_Bytes_(report->reserved, sizeof(report->reserved));
        }

        void Read_Report_(SERIAL::Reader_* reader, s_report* report, uint16_t version) {
            report->type = reader->Read_Uint8_();
            report->player_idx = reader->Read_Int16_();
            report->action = reader->Read_Uint8_();
            memset(&report->d, 0, sizeof(report->d));
            if (report->type == 1 && report->action == 0) {
                report->d.steal.framed_player_idx = reader->Read_Int8_();
                report->d.steal.victim_player_idx = reader->Read_Uint8_();
                report->d.steal.tech_app_id = reader->Read_Int16_();
                if (version != LEGACY_VERSION) reader->Skip_(1);
            } else if (report->type == 1 && report->action == 1) {
                report->d.sabotage.framed_player_idx = reader->Read_Int8_();
                report->d.sabotage.victim_player_idx = reader->Read_Uint8_();
                report->d.sabotage.planet_idx = version == LEGACY_VERSION ? reader->Read_Uint8_() : reader->Read_Int16_();
                report->d.sabotage.building_id = reader->Read_Uint8_();
            } else if (report->type == 2 && report->action == 0) {
                report->d.enemy_steal.enemy_player_idx = reader->Read_Int16_();
                report->d.enemy_steal.tech_app_id = reader->Read_Int16_();
                if (version != LEGACY_VERSION) reader->Skip_(1);
            } else if (report->type == 2 && report->action == 1) {
                report->d.enemy_sabotage.enemy_player_idx = reader->Read_Int16_();
                report->d.enemy_sabotage.planet_idx = version == LEGACY_VERSION ? reader->Read_Uint8_() : reader->Read_Int16_();
                report->d.enemy_sabotage.building_id = reader->Read_Uint8_();
            } else {
                reader->Read_Bytes_(&report->d, version == LEGACY_VERSION ? 4 : sizeof(report->d));
            }
            reader->Read_Bytes_(report->reserved, sizeof(report->reserved));
        }

        void Write_Event_(SERIAL::Writer_* writer, const s_event_data* event) {
            writer->Write_Int8_(event->player_idx);
            writer->Write_Int8_(event->status);
            writer->Write_Int8_(event->turn_count);
            writer->Write_Int16_(event->target_id);
            writer->Write_Int16_(event->amount);
            writer->Write_Int16_(event->max_amount);
        }

        void Read_Event_(SERIAL::Reader_* reader, s_event_data* event) {
            event->player_idx = reader->Read_Int8_();
            event->status = reader->Read_Int8_();
            event->turn_count = reader->Read_Int8_();
            event->target_id = reader->Read_Int16_();
            event->amount = reader->Read_Int16_();
            event->max_amount = reader->Read_Int16_();
        }

        void Write_Colony_Message_(SERIAL::Writer_* writer, const s_MSG_colony* colony) {
            writer->Write_Int16_(colony->planet_idx);
            writer->Write_Int8_(colony->subtype);
            switch (colony->subtype) {
            case 0:
                writer->Write_Int16_(colony->c.scrap.building_id);
                writer->Write_Int16_(colony->c.scrap.bc);
                Write_Zeroes_(writer, 2);
                break;
            case 1:
                writer->Write_Int16_(colony->c.build.build_id);
                writer->Write_Int16_(colony->c.build.product_id);
                writer->Write_Uint8_(colony->c.build.is_refit ? 1 : 0);
                writer->Write_Int8_(colony->c.build.append_fmt);
                break;
            case 2:
            case 3:
                writer->Write_Uint8_(colony->c.assimilation.val);
                Write_Zeroes_(writer, 5);
                break;
            case 4:
                writer->Write_Int8_(colony->c.starvation.a);
                writer->Write_Int8_(colony->c.starvation.b);
                Write_Zeroes_(writer, 4);
                break;
            case 5:
                writer->Write_Int8_(colony->c.settlers.settlers);
                writer->Write_Int8_(colony->c.settlers.over_limit);
                writer->Write_Uint8_(colony->c.settlers.blockaded ? 1 : 0);
                Write_Zeroes_(writer, 3);
                break;
            case 9:
                writer->Write_Int8_(colony->c.rebels.owner_player);
                writer->Write_Int8_(colony->c.rebels.rebels);
                writer->Write_Int8_(colony->c.rebels.lost);
                Write_Zeroes_(writer, 3);
                break;
            case 11:
                writer->Write_Int8_(colony->c.android_killed.race_idx);
                Write_Zeroes_(writer, 5);
                break;
            default:
                Write_Zeroes_(writer, 6);
                break;
            }
        }

        void Read_Colony_Message_(SERIAL::Reader_* reader, s_MSG_colony* colony, uint16_t version) {
            colony->planet_idx = version == LEGACY_VERSION ? reader->Read_Uint8_() : reader->Read_Int16_();
            colony->subtype = reader->Read_Int8_();
            switch (colony->subtype) {
            case 0:
                colony->c.scrap.building_id = reader->Read_Int16_();
                colony->c.scrap.bc = reader->Read_Int16_();
                reader->Skip_(2);
                break;
            case 1:
                colony->c.build.build_id = reader->Read_Int16_();
                colony->c.build.product_id = reader->Read_Int16_();
                colony->c.build.is_refit = reader->Read_Uint8_() != 0;
                colony->c.build.append_fmt = reader->Read_Int8_();
                break;
            case 2:
            case 3:
                colony->c.assimilation.val = reader->Read_Uint8_();
                reader->Skip_(5);
                break;
            case 4:
                colony->c.starvation.a = reader->Read_Int8_();
                colony->c.starvation.b = reader->Read_Int8_();
                reader->Skip_(4);
                break;
            case 5:
                colony->c.settlers.settlers = reader->Read_Int8_();
                colony->c.settlers.over_limit = reader->Read_Int8_();
                colony->c.settlers.blockaded = reader->Read_Uint8_() != 0;
                reader->Skip_(3);
                break;
            case 9:
                colony->c.rebels.owner_player = reader->Read_Int8_();
                colony->c.rebels.rebels = reader->Read_Int8_();
                colony->c.rebels.lost = reader->Read_Int8_();
                reader->Skip_(3);
                break;
            case 11:
                colony->c.android_killed.race_idx = reader->Read_Int8_();
                reader->Skip_(5);
                break;
            default:
                reader->Skip_(6);
                break;
            }
        }

        void Write_Message_(SERIAL::Writer_* writer, const s_MSG* message) {
            writer->Write_Int8_(message->msg_type);
            writer->Write_Uint8_(message->priority);
            switch (message->msg_type) {
            case MSG_TYPE_COLONY:
                Write_Colony_Message_(writer, &message->u.colony);
                break;
            case MSG_TYPE_DECLARE_ALLIANCE:
            case MSG_TYPE_DECLARE_WAR:
                writer->Write_Int16_(message->u.players.player_a_idx);
                writer->Write_Int16_(message->u.players.player_b_idx);
                Write_Zeroes_(writer, 5);
                break;
            case MSG_TYPE_FINANCIAL:
                writer->Write_Int16_(message->u.financial.a);
                writer->Write_Int16_(message->u.financial.b);
                Write_Zeroes_(writer, 5);
                break;
            case MSG_TYPE_LOST_SHIP:
                writer->Write_Int16_(message->u.lost_ship.star_idx);
                writer->Write_Int16_(message->u.lost_ship.ship_idx);
                writer->Write_Int8_(message->u.lost_ship.leader_idx);
                writer->Write_Int8_(message->u.lost_ship.fate);
                Write_Zeroes_(writer, 3);
                break;
            case MSG_TYPE_SCRAP_SHIP:
                writer->Write_Int16_(message->u.scrap_ship.where);
                writer->Write_Int16_(message->u.scrap_ship.bc);
                writer->Write_Int16_(message->u.scrap_ship.ship_idx);
                Write_Zeroes_(writer, 3);
                break;
            case MSG_TYPE_LOST_COLONY:
                writer->Write_Int16_(message->u.lost_colony.planet_idx);
                writer->Write_Int16_(message->u.lost_colony.reason);
                writer->Write_Int16_(message->u.lost_colony.leader_idx);
                writer->Write_Int8_(message->u.lost_colony.colony_or_outpost);
                Write_Zeroes_(writer, 2);
                break;
            case MSG_TYPE_HYPERSPACE_BEAST:
                writer->Write_Int16_(message->u.hyperspace_beast.ship_idx);
                writer->Write_Int16_(message->u.hyperspace_beast.star_idx);
                writer->Write_Int8_(message->u.hyperspace_beast.officer_idx);
                writer->Write_Int8_(message->u.hyperspace_beast.leader_status);
                writer->Write_Int8_(message->u.hyperspace_beast.fate);
                writer->Write_Int8_(message->u.hyperspace_beast.your_ship);
                Write_Zeroes_(writer, 1);
                break;
            case MSG_TYPE_LOST_ATTACKING_SPY:
                writer->Write_Int16_(message->u.lost_attacking_spy.player_idx);
                Write_Zeroes_(writer, 7);
                break;
            case MSG_TYPE_LOST_DEFENDING_SPY:
                writer->Write_Int16_(message->u.lost_defending_spy.count);
                Write_Zeroes_(writer, 7);
                break;
            case MSG_TYPE_ARTEMIS:
                writer->Write_Int16_(message->u.artemis.star_idx);
                Write_Zeroes_(writer, 7);
                break;
            case MSG_TYPE_ARTEMIS_LEADER:
                writer->Write_Int16_(message->u.artemis_leader.star_idx);
                writer->Write_Int8_(message->u.artemis_leader.leader_idx);
                writer->Write_Int8_(message->u.artemis_leader.fate);
                Write_Zeroes_(writer, 5);
                break;
            case MSG_TYPE_FAILED_RETREAT:
                writer->Write_Int16_(message->u.retreat.star_idx);
                writer->Write_Int8_(message->u.retreat.reason);
                writer->Write_Int16_(message->u.retreat.ships_lost);
                writer->Write_Int8_(message->u.retreat.leader_count);
                Write_Zeroes_(writer, 3);
                break;
            case MSG_TYPE_SPY_ASSASSINATION:
                writer->Write_Int8_(message->u.spy_assasination.plr_a);
                writer->Write_Int8_(message->u.spy_assasination.plr_b);
                writer->Write_Int8_(message->u.spy_assasination.leader_id);
                writer->Write_Int8_(message->u.spy_assasination.victim_id);
                Write_Zeroes_(writer, 5);
                break;
            case MSG_TYPE_ANOMALY:
                writer->Write_Int16_(message->u.anomaly.star_idx);
                Write_Zeroes_(writer, 7);
                break;
            case MSG_TYPE_DIPLOMACY:
                writer->Write_Int8_(message->u.diplomacy.diplomacy_idx);
                writer->Write_Int8_(message->u.diplomacy.plr_a);
                writer->Write_Int8_(message->u.diplomacy.plr_b);
                Write_Zeroes_(writer, 6);
                break;
            case MSG_TYPE_EEL_SPAWNED:
            case MSG_TYPE_EEL_VANISHED:
                writer->Write_Int16_(message->u.eel.star_idx);
                Write_Zeroes_(writer, 7);
                break;
            case MSG_TYPE_ARTIFACTS:
                writer->Write_Int16_(message->u.artifacts.star_idx);
                writer->Write_Int8_(message->u.artifacts.tech1);
                writer->Write_Int8_(message->u.artifacts.tech2);
                Write_Zeroes_(writer, 5);
                break;
            case MSG_TYPE_SCRAP_LEADER:
                writer->Write_Int8_(message->u.scrap_leader.leader_idx);
                Write_Zeroes_(writer, 8);
                break;
            default:
                Write_Zeroes_(writer, 9);
                break;
            }
            writer->Write_Bytes_(message->reserved, sizeof(message->reserved));
        }

        void Read_Message_(SERIAL::Reader_* reader, s_MSG* message, uint16_t version) {
            message->msg_type = reader->Read_Int8_();
            message->priority = reader->Read_Uint8_();
            memset(&message->u, 0, sizeof(message->u));
            switch (message->msg_type) {
            case MSG_TYPE_COLONY:
                Read_Colony_Message_(reader, &message->u.colony, version);
                break;
            case MSG_TYPE_DECLARE_ALLIANCE:
            case MSG_TYPE_DECLARE_WAR:
                message->u.players.player_a_idx = reader->Read_Int16_();
                message->u.players.player_b_idx = reader->Read_Int16_();
                reader->Skip_(version == LEGACY_VERSION ? 4 : 5);
                break;
            case MSG_TYPE_FINANCIAL:
                message->u.financial.a = reader->Read_Int16_();
                message->u.financial.b = reader->Read_Int16_();
                reader->Skip_(version == LEGACY_VERSION ? 4 : 5);
                break;
            case MSG_TYPE_LOST_SHIP:
                message->u.lost_ship.star_idx = reader->Read_Int16_();
                message->u.lost_ship.ship_idx = reader->Read_Int16_();
                message->u.lost_ship.leader_idx = reader->Read_Int8_();
                message->u.lost_ship.fate = reader->Read_Int8_();
                reader->Skip_(version == LEGACY_VERSION ? 2 : 3);
                break;
            case MSG_TYPE_SCRAP_SHIP:
                message->u.scrap_ship.where = reader->Read_Int16_();
                if (version == LEGACY_VERSION) {
                    message->u.scrap_ship.where = Convert_Legacy_Ship_Location_(message->u.scrap_ship.where);
                }
                message->u.scrap_ship.bc = reader->Read_Int16_();
                message->u.scrap_ship.ship_idx = reader->Read_Int16_();
                reader->Skip_(version == LEGACY_VERSION ? 2 : 3);
                break;
            case MSG_TYPE_LOST_COLONY:
                message->u.lost_colony.planet_idx = reader->Read_Int16_();
                message->u.lost_colony.reason = reader->Read_Int16_();
                message->u.lost_colony.leader_idx = reader->Read_Int16_();
                message->u.lost_colony.colony_or_outpost = reader->Read_Int8_();
                reader->Skip_(version == LEGACY_VERSION ? 1 : 2);
                break;
            case MSG_TYPE_HYPERSPACE_BEAST:
                message->u.hyperspace_beast.ship_idx = reader->Read_Int16_();
                message->u.hyperspace_beast.star_idx = version == LEGACY_VERSION ? reader->Read_Int8_() : reader->Read_Int16_();
                message->u.hyperspace_beast.officer_idx = reader->Read_Int8_();
                message->u.hyperspace_beast.leader_status = reader->Read_Int8_();
                message->u.hyperspace_beast.fate = reader->Read_Int8_();
                message->u.hyperspace_beast.your_ship = reader->Read_Int8_();
                reader->Skip_(1);
                break;
            case MSG_TYPE_LOST_ATTACKING_SPY:
                message->u.lost_attacking_spy.player_idx = reader->Read_Int16_();
                reader->Skip_(version == LEGACY_VERSION ? 6 : 7);
                break;
            case MSG_TYPE_LOST_DEFENDING_SPY:
                message->u.lost_defending_spy.count = reader->Read_Int16_();
                reader->Skip_(version == LEGACY_VERSION ? 6 : 7);
                break;
            case MSG_TYPE_ARTEMIS:
                message->u.artemis.star_idx = reader->Read_Int16_();
                reader->Skip_(version == LEGACY_VERSION ? 6 : 7);
                break;
            case MSG_TYPE_ARTEMIS_LEADER:
                message->u.artemis_leader.star_idx = version == LEGACY_VERSION ? reader->Read_Int8_() : reader->Read_Int16_();
                message->u.artemis_leader.leader_idx = reader->Read_Int8_();
                message->u.artemis_leader.fate = reader->Read_Int8_();
                reader->Skip_(5);
                break;
            case MSG_TYPE_FAILED_RETREAT:
                message->u.retreat.star_idx = version == LEGACY_VERSION ? reader->Read_Int8_() : reader->Read_Int16_();
                message->u.retreat.reason = reader->Read_Int8_();
                message->u.retreat.ships_lost = reader->Read_Int16_();
                message->u.retreat.leader_count = reader->Read_Int8_();
                reader->Skip_(3);
                break;
            case MSG_TYPE_SPY_ASSASSINATION:
                message->u.spy_assasination.plr_a = reader->Read_Int8_();
                message->u.spy_assasination.plr_b = reader->Read_Int8_();
                message->u.spy_assasination.leader_id = reader->Read_Int8_();
                message->u.spy_assasination.victim_id = reader->Read_Int8_();
                reader->Skip_(version == LEGACY_VERSION ? 4 : 5);
                break;
            case MSG_TYPE_ANOMALY:
                message->u.anomaly.star_idx = version == LEGACY_VERSION ? reader->Read_Uint8_() : reader->Read_Int16_();
                reader->Skip_(7);
                break;
            case MSG_TYPE_DIPLOMACY:
                message->u.diplomacy.diplomacy_idx = reader->Read_Int8_();
                message->u.diplomacy.plr_a = reader->Read_Int8_();
                message->u.diplomacy.plr_b = reader->Read_Int8_();
                reader->Skip_(version == LEGACY_VERSION ? 5 : 6);
                break;
            case MSG_TYPE_EEL_SPAWNED:
            case MSG_TYPE_EEL_VANISHED:
                message->u.eel.star_idx = version == LEGACY_VERSION ? reader->Read_Uint8_() : reader->Read_Int16_();
                reader->Skip_(7);
                break;
            case MSG_TYPE_ARTIFACTS:
                message->u.artifacts.star_idx = version == LEGACY_VERSION ? reader->Read_Uint8_() : reader->Read_Int16_();
                message->u.artifacts.tech1 = reader->Read_Int8_();
                message->u.artifacts.tech2 = reader->Read_Int8_();
                reader->Skip_(5);
                break;
            case MSG_TYPE_SCRAP_LEADER:
                message->u.scrap_leader.leader_idx = reader->Read_Int8_();
                reader->Skip_(version == LEGACY_VERSION ? 7 : 8);
                break;
            default:
                reader->Skip_(version == LEGACY_VERSION ? 8 : 9);
                break;
            }
            reader->Read_Bytes_(message->reserved, sizeof(message->reserved));
        }

        void Write_Antaran_(SERIAL::Writer_* writer, const s_antaran* antaran) {
            writer->Write_Int16_(antaran->attack_pressure);
            writer->Write_Int16_(antaran->resource_offensive);
            writer->Write_Int16_(antaran->resource_defensive);
            Write_Int16_Array_(writer, antaran->defensive_ships, HULL_CLASS_COUNT);
            Write_Int16_Array_(writer, antaran->offensive_ships, HULL_CLASS_COUNT);
            Write_Int16_Array_(writer, antaran->offensive_deployed, HULL_CLASS_COUNT);
            writer->Write_Uint8_(antaran->field6_0x3c);
            writer->Write_Int32_(antaran->invasion_stardate);
            writer->Write_Uint8_(antaran->attack_notified_mask);
        }

        void Read_Antaran_(SERIAL::Reader_* reader, s_antaran* antaran) {
            antaran->attack_pressure = reader->Read_Int16_();
            antaran->resource_offensive = reader->Read_Int16_();
            antaran->resource_defensive = reader->Read_Int16_();
            Read_Int16_Array_(reader, antaran->defensive_ships, HULL_CLASS_COUNT);
            Read_Int16_Array_(reader, antaran->offensive_ships, HULL_CLASS_COUNT);
            Read_Int16_Array_(reader, antaran->offensive_deployed, HULL_CLASS_COUNT);
            antaran->field6_0x3c = reader->Read_Uint8_();
            antaran->invasion_stardate = reader->Read_Int32_();
            antaran->attack_notified_mask = reader->Read_Uint8_();
        }

        void Write_Game_State_(SERIAL::Writer_* writer, const s_settings* settings, uint32_t random_seed) {
            Write_Settings_(writer, settings);
            writer->Write_Uint32_(random_seed);
            writer->Write_Int16_(MOX::_NUM_COLONIES);
            for (int i = 0; i < MOX::_NUM_COLONIES; ++i) Write_Colony_(writer, &MOX::_colony[i]);
            writer->Write_Int16_(MOX::_planet_count);
            for (int i = 0; i < MOX::_planet_count; ++i) Write_Planet_(writer, &MOX::_planet[i]);
            writer->Write_Int16_(MOX::_NUM_STARS);
            for (int i = 0; i < MOX::_NUM_STARS; ++i) Write_Star_(writer, &MOX::_star[i]);
            for (int i = 0; i < MAX_LEADERS; ++i) Write_Leader_(writer, &MOX::_leaders[i]);
            writer->Write_Int16_(MOX::_NUM_PLAYERS);
            for (int i = 0; i < MAX_PLAYERS; ++i) Write_Player_(writer, &MOX::_player[i]);
            writer->Write_Int16_(MOX::_NUM_SHIPS);
            for (int i = 0; i < MOX::_NUM_SHIPS; ++i) Write_Ship_(writer, &MOX::_ship[i]);
            for (int i = 0; i < 5; ++i) Write_Movable_Box_(writer, &MOX::_moveable_box[i]);
            Write_Int16_Array_(writer, MOX::_moveable_box_order, 5);
            writer->Write_Int16_(MOX::_PLAYER_NUM);
            writer->Write_Int16_(MOX::_max_map_scale);
            writer->Write_Int16_(MOX::_max_zoom_count);
            writer->Write_Uint8_(MOX::_star_bg_index);
            writer->Write_Int16_(MOX::_MAP_MAX_X);
            writer->Write_Int16_(MOX::_MAP_MAX_Y);
            writer->Write_Uint16_(MOX::_fleet_icon_button_count);
            for (int i = 0; i < MAX_NEBULAS; ++i) Write_Nebula_(writer, &MOX::_nebula[i]);
            writer->Write_Uint8_(MOX::_NUM_NEBULAS);
            Write_Int16_Array_(writer, MOX::_bill_savegame, 6);
            writer->Write_Int16_(REPORT::_n_reports);
            for (int i = 0; i < MAX_REPORTS; ++i) Write_Report_(writer, &REPORT::_reports[i]);
            writer->Write_Uint8_(REPORT::_done_player_reports);
            writer->Write_Bytes_(REPORT::_something_interesting_happened, sizeof(REPORT::_something_interesting_happened));
            writer->Write_Bytes_(REPORT::_done_colonize_check, sizeof(REPORT::_done_colonize_check));
            for (int i = 0; i < 36; ++i) Write_Event_(writer, &EVENTS::_event_data[i]);
            for (int i = 0; i < MAX_MESSAGES; ++i) Write_Message_(writer, &MSG_::_msgs[i]);
            writer->Write_Bytes_(MSG_::_first_msg, sizeof(MSG_::_first_msg));
            writer->Write_Bytes_(MSG_::_max_msgs, sizeof(MSG_::_max_msgs));
            writer->Write_Bytes_(MSG_::_n_msgs, sizeof(MSG_::_n_msgs));
            writer->Write_Uint8_(REPORT::_displayed_msgs);
            writer->Write_Uint8_(REPORT::_displayed_gnn);
            writer->Write_Int16_(diplodef::_COUNCIL_FLAG);
            writer->Write_Int16_(diplodef::_next_council_meeting_turn);
            writer->Write_Uint16_(MOX::_antaran_resource_level);
            writer->Write_Uint16_(MOX::_chance_for_antaran_invasion);
            writer->Write_Uint16_(MOX::_disallow_saves_code);
            writer->Write_Uint32_(EVENTS::_last_event_year);
            writer->Write_Int16_(EVENTS::_event_delay_count);
            Write_Antaran_(writer, &MOX::_antarans);
            writer->Write_Int16_(MOX::_cur_map_scale);
            writer->Write_Int16_(MOX::_cur_map_x);
            writer->Write_Int16_(MOX::_cur_map_y);
            Write_Zeroes_(writer, 1000);
        }

        bool Read_Game_State_(SERIAL::Reader_* reader, uint16_t version, s_settings* settings, uint32_t* random_seed) {
            Read_Settings_(reader, settings);
            *random_seed = reader->Read_Uint32_();
            MOX::_NUM_COLONIES = reader->Read_Int16_();
            if (MOX::_NUM_COLONIES < 0 || MOX::_NUM_COLONIES > MAX_COLONIES) return false;
            const int colony_record_count = version == LEGACY_VERSION ? MAX_COLONIES : MOX::_NUM_COLONIES;
            for (int i = 0; i < colony_record_count; ++i) Read_Colony_(reader, &MOX::_colony[i]);
            memset(&MOX::_colony[colony_record_count], 0,
                   (MAX_COLONIES - colony_record_count) * sizeof(*MOX::_colony));
            MOX::_planet_count = reader->Read_Int16_();
            if (MOX::_planet_count < 0 || MOX::_planet_count > MAX_PLANETS) return false;
            const int planet_record_count = version == LEGACY_VERSION ? MAX_PLANETS : MOX::_planet_count;
            for (int i = 0; i < planet_record_count; ++i) Read_Planet_(reader, &MOX::_planet[i], version);
            memset(&MOX::_planet[planet_record_count], 0,
                   (MAX_PLANETS - planet_record_count) * sizeof(*MOX::_planet));
            MOX::_NUM_STARS = reader->Read_Int16_();
            if (MOX::_NUM_STARS < 0 || MOX::_NUM_STARS > MAX_STARS) return false;
            const int star_record_count = version == LEGACY_VERSION ? MAX_STARS : MOX::_NUM_STARS;
            for (int i = 0; i < star_record_count; ++i) Read_Star_(reader, &MOX::_star[i], version);
            memset(&MOX::_star[star_record_count], 0,
                   (MAX_STARS - star_record_count) * sizeof(*MOX::_star));
            for (int i = 0; i < MAX_LEADERS; ++i) Read_Leader_(reader, &MOX::_leaders[i]);
            MOX::_NUM_PLAYERS = reader->Read_Int16_();
            for (int i = 0; i < MAX_PLAYERS; ++i) Read_Player_(reader, &MOX::_player[i], version);
            MOX::_NUM_SHIPS = reader->Read_Int16_();
            if (MOX::_NUM_SHIPS < 0 || MOX::_NUM_SHIPS > MAX_SHIPS) return false;
            const int ship_record_count = version == LEGACY_VERSION ? ORIGINAL_MAX_SHIPS : MOX::_NUM_SHIPS;
            for (int i = 0; i < ship_record_count; ++i) Read_Ship_(reader, &MOX::_ship[i], version);
            memset(&MOX::_ship[ship_record_count], 0,
                   (MAX_SHIPS - ship_record_count) * sizeof(*MOX::_ship));
            for (int i = 0; i < 5; ++i) Read_Movable_Box_(reader, &MOX::_moveable_box[i]);
            Read_Int16_Array_(reader, MOX::_moveable_box_order, 5);
            MOX::_PLAYER_NUM = reader->Read_Int16_();
            MOX::_max_map_scale = reader->Read_Int16_();
            MOX::_max_zoom_count = reader->Read_Int16_();
            MOX::_star_bg_index = reader->Read_Uint8_();
            MOX::_MAP_MAX_X = reader->Read_Int16_();
            MOX::_MAP_MAX_Y = reader->Read_Int16_();
            MOX::_fleet_icon_button_count = reader->Read_Uint16_();
            for (int i = 0; i < MAX_NEBULAS; ++i) Read_Nebula_(reader, &MOX::_nebula[i]);
            MOX::_NUM_NEBULAS = reader->Read_Uint8_();
            Read_Int16_Array_(reader, MOX::_bill_savegame, 6);
            REPORT::_n_reports = reader->Read_Int16_();
            for (int i = 0; i < MAX_REPORTS; ++i) Read_Report_(reader, &REPORT::_reports[i], version);
            REPORT::_done_player_reports = reader->Read_Uint8_();
            reader->Read_Bytes_(REPORT::_something_interesting_happened, sizeof(REPORT::_something_interesting_happened));
            reader->Read_Bytes_(REPORT::_done_colonize_check, sizeof(REPORT::_done_colonize_check));
            for (int i = 0; i < 36; ++i) Read_Event_(reader, &EVENTS::_event_data[i]);
            for (int i = 0; i < MAX_MESSAGES; ++i) Read_Message_(reader, &MSG_::_msgs[i], version);
            reader->Read_Bytes_(MSG_::_first_msg, sizeof(MSG_::_first_msg));
            reader->Read_Bytes_(MSG_::_max_msgs, sizeof(MSG_::_max_msgs));
            reader->Read_Bytes_(MSG_::_n_msgs, sizeof(MSG_::_n_msgs));
            REPORT::_displayed_msgs = reader->Read_Uint8_();
            REPORT::_displayed_gnn = reader->Read_Uint8_();
            diplodef::_COUNCIL_FLAG = reader->Read_Int16_();
            diplodef::_next_council_meeting_turn = reader->Read_Int16_();
            MOX::_antaran_resource_level = reader->Read_Uint16_();
            MOX::_chance_for_antaran_invasion = reader->Read_Uint16_();
            MOX::_disallow_saves_code = reader->Read_Uint16_();
            EVENTS::_last_event_year = reader->Read_Uint32_();
            EVENTS::_event_delay_count = reader->Read_Int16_();
            Read_Antaran_(reader, &MOX::_antarans);
            MOX::_cur_map_scale = reader->Read_Int16_();
            MOX::_cur_map_x = reader->Read_Int16_();
            MOX::_cur_map_y = reader->Read_Int16_();
            reader->Skip_(1000);
            return true;
        }
    }

    int16_t Convert_Legacy_Ship_Location_(int16_t location) {
        constexpr int16_t legacy_moving_offset = ORIGINAL_MAX_SHIPS;
        constexpr int16_t legacy_wormhole_offset = ORIGINAL_MAX_SHIPS * 2;
        constexpr int16_t legacy_location_limit = ORIGINAL_MAX_SHIPS * 3;

        if (location >= legacy_moving_offset && location < legacy_wormhole_offset) {
            return location - legacy_moving_offset + SHIP_LOCATION_MOVING_OFFSET;
        }
        if (location >= legacy_wormhole_offset && location < legacy_location_limit) {
            return location - legacy_wormhole_offset + SHIP_LOCATION_WORMHOLE_OFFSET;
        }
        return location;
    }

    void Write_Game_(SERIAL::Writer_* writer, const char* description, uint32_t random_seed) {
        writer->Write_Uint32_(VERSIONED_HEADER);
        writer->Write_Bytes_(description, sizeof(s_metadata::description));
        writer->Write_Int32_(MOX::_stardate);
        writer->Write_Int8_(MOX::_game_type);
        writer->Write_Uint16_(CURRENT_VERSION);
        Write_Game_State_(writer, &MOX::_settings, random_seed);
    }

    bool Read_Game_(SERIAL::Reader_* reader, s_metadata* metadata, s_settings* saved_settings, uint32_t* random_seed) {
        if (reader == nullptr || metadata == nullptr || saved_settings == nullptr || random_seed == nullptr) return false;

        metadata->header = reader->Read_Uint32_();
        if (metadata->header != LEGACY_HEADER && metadata->header != VERSIONED_HEADER) return false;

        reader->Read_Bytes_(metadata->description, sizeof(metadata->description));
        metadata->stardate = reader->Read_Int32_();
        metadata->game_type = reader->Read_Int8_();
        metadata->version = metadata->header == LEGACY_HEADER ? LEGACY_VERSION : reader->Read_Uint16_();
        if (metadata->version > CURRENT_VERSION) return false;

        return Read_Game_State_(reader, metadata->version, saved_settings, random_seed);
    }
}