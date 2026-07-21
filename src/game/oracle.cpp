#include "pch.h"

#include <algorithm>
#include <limits>
#include <string>
#include <vector>

#if defined(_WIN32)
#include <direct.h>
#else
#include <unistd.h>
#endif

namespace oracle {
    namespace {
#pragma pack(push, 1)
        struct s_input_state {
            int16_t mouse_x;
            int16_t mouse_y;
            int16_t mouse_buttons;
            int16_t mouse_buffer_button;
            int16_t mouse_buffer_x;
            int16_t mouse_buffer_y;
            int16_t mouse_buffer_flag;
            uint16_t mouse_buffer_flag2;
            uint8_t key_pressed;
            uint8_t key_write;
            int8_t key_read;
            uint8_t reserved;
            uint32_t last_scan_code;
            int32_t key_buffer[10];
        };

        struct s_log_header {
            uint32_t magic;
            uint32_t version;
            uint32_t size;
            uint32_t oracle_turn;
            int32_t stardate;
            int64_t qpc_frequency;
            int64_t start_qpc;
            s_input_state input_state;
        };

        struct s_input_record {
            uint32_t type;
            uint32_t size;
            int64_t delta_qpc;
            uint32_t oracle_turn;
            int32_t stardate;
            uint32_t input_type;
            int16_t mouse_x;
            int16_t mouse_y;
            uint16_t mouse_buttons;
            uint16_t reserved;
            uint32_t key_code;
            uint32_t scan_code;
        };

        struct s_end_record {
            uint32_t type;
            uint32_t size;
            int64_t delta_qpc;
            uint32_t oracle_turn;
            int32_t stardate;
            uint32_t completed;
        };
#pragma pack(pop)

        static_assert(sizeof(s_input_state) == 64, "oracle input state layout mismatch");
        static_assert(sizeof(s_log_header) == 100, "oracle log header layout mismatch");
        static_assert(sizeof(s_input_record) == 44, "oracle input record layout mismatch");
        static_assert(sizeof(s_end_record) == 28, "oracle end record layout mismatch");

        constexpr uint32_t k_log_magic = 0x4C43524F;
        constexpr uint32_t k_log_version = 3;
        constexpr uint32_t k_input_record_type = 1;
        constexpr uint32_t k_end_record_type = 2;
        constexpr uint32_t k_mouse_input_type = 1;
        constexpr uint32_t k_key_input_type = 2;

        enum e_prepare_result {
            PREPARE_READY,
            PREPARE_DONE,
            PREPARE_ERROR,
        };

        struct s_save_block {
            const char* name;
            size_t size;
        };

        struct s_field_range {
            const char* name;
            size_t offset;
            size_t size;
            size_t element_size;
        };

        bool g_enabled;
        bool g_replay_active;
        uint32_t g_turn;
        char g_oracle_directory[1024];
        s_log_header g_header;
        std::vector<s_input_record> g_records;
        size_t g_record_index;
        uint64_t g_last_consumption_counter;
        uint32_t g_speed_percent;
        uint32_t g_delay_ms;

        constexpr char Native_Separator_() {
#if defined(_WIN32)
            return '\\';
#else
            return '/';
#endif
        }

        bool Build_Path_(char* output, size_t output_size, const char* format, ...) {
            va_list args;
            va_start(args, format);
            const int length = vsnprintf(output, output_size, format, args);
            va_end(args);
            return length >= 0 && (size_t)length < output_size;
        }

        bool Parse_Turn_(const char* value, uint32_t* turn) {
            if (value == nullptr || value[0] == '\0' || value[0] == '-') {
                return false;
            }

            char* end = nullptr;
            errno = 0;
            unsigned long parsed = strtoul(value, &end, 10);
            if (errno != 0 || end == value || *end != '\0' || parsed == 0 ||
                parsed > std::numeric_limits<uint32_t>::max()) {
                return false;
            }

            *turn = (uint32_t)parsed;
            return true;
        }

        bool Parse_Uint32_(const char* value, uint32_t minimum, uint32_t maximum, uint32_t* result) {
            if (value == nullptr || value[0] == '\0' || value[0] == '-') {
                return false;
            }

            char* end = nullptr;
            errno = 0;
            const unsigned long parsed = strtoul(value, &end, 10);
            if (errno != 0 || end == value || *end != '\0' ||
                parsed < minimum || parsed > maximum) {
                return false;
            }

            *result = (uint32_t)parsed;
            return true;
        }

        uint64_t Replay_Delta_Counters_(int64_t source_qpc) {
            const uint64_t local_frequency = SDL_GetPerformanceFrequency();
            long double seconds =
                ((long double)source_qpc / (long double)g_header.qpc_frequency) *
                (100.0L / (long double)g_speed_percent);
            seconds += (long double)g_delay_ms / 1000.0L;
            return (uint64_t)(seconds * (long double)local_frequency);
        }

        bool Path_Is_Directory_(const char* path) {
            SDL_PathInfo info;
            return SDL_GetPathInfo(path, &info) && info.type == SDL_PATHTYPE_DIRECTORY;
        }

        bool Ensure_Directory_(const char* path) {
            return Path_Is_Directory_(path) || SDL_CreateDirectory(path);
        }

        bool Set_Current_Directory_(const char* path) {
#if defined(_WIN32)
            return _chdir(path) == 0;
#else
            return chdir(path) == 0;
#endif
        }

        bool Read_File_(const char* path, std::vector<uint8_t>* data, bool log_missing) {
            FILE* file = fopen_case(path, "rb");
            if (file == nullptr) {
                if (log_missing) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "oracle: cannot open '%s'", path);
                }
                return false;
            }

            if (fseek(file, 0, SEEK_END) != 0) {
                fclose(file);
                return false;
            }
            const long file_size = ftell(file);
            if (file_size < 0 || fseek(file, 0, SEEK_SET) != 0) {
                fclose(file);
                return false;
            }

            data->resize((size_t)file_size);
            const bool read = data->empty() || fread(data->data(), data->size(), 1, file) == 1;
            fclose(file);
            return read;
        }

        bool Change_Directory_And_Load_(const char* directory) {
            char* previous_directory = SDL_GetCurrentDirectory();
            if (previous_directory == nullptr) {
                return false;
            }

            bool loaded = false;
            if (Set_Current_Directory_(directory)) {
                loaded = FILEDEF::Load_Game_(9) != 0;
                if (!Set_Current_Directory_(previous_directory)) {
                    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: cannot restore working directory '%s'", previous_directory);
                    loaded = false;
                }
            }

            SDL_free(previous_directory);
            return loaded;
        }

        bool Load_Turn_State_(uint32_t turn) {
            char directory[1024];
            char save_path[1024];
            const char separator = Native_Separator_();
            if (!Build_Path_(directory, sizeof(directory), "%s%c%u", g_oracle_directory, separator, turn) ||
                !Build_Path_(save_path, sizeof(save_path), "%s%c%u%cSAVE10.GAM", g_oracle_directory, separator, turn, separator)) {
                return false;
            }

            if (!File_Exists_(save_path)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "oracle: missing turn %u save '%s'", turn, save_path);
                return false;
            }

            if (!Change_Directory_And_Load_(directory)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "oracle: failed to load turn %u", turn);
                return false;
            }
            return true;
        }

        bool Save_Actual_State_(uint32_t turn, char* save_path, size_t save_path_size) {
            char actual_root[1024];
            char turn_directory[1024];
            const char separator = Native_Separator_();
            if (!Build_Path_(actual_root, sizeof(actual_root), "%s%c_actual", g_oracle_directory, separator) ||
                !Build_Path_(turn_directory, sizeof(turn_directory), "%s%c%u", actual_root, separator, turn) ||
                !Build_Path_(save_path, save_path_size, "%s%c%u%cSAVE10.GAM", actual_root, separator, turn, separator)) {
                return false;
            }

            if (!Ensure_Directory_(actual_root) || !Ensure_Directory_(turn_directory)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "oracle: cannot create '%s'", turn_directory);
                return false;
            }

            char* previous_directory = SDL_GetCurrentDirectory();
            if (previous_directory == nullptr) {
                return false;
            }

            bool saved = false;
            if (Set_Current_Directory_(turn_directory)) {
                FILEDEF::Save_Game_(9);
                saved = File_Exists_(save_path);
                if (!Set_Current_Directory_(previous_directory)) {
                    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: cannot restore working directory '%s'", previous_directory);
                    saved = false;
                }
            }

            SDL_free(previous_directory);
            return saved;
        }

        bool Read_Completed_Log_(uint32_t turn, s_log_header* header,
            std::vector<s_input_record>* records, bool* exists, bool* completed) {
            char path[1024];
            std::vector<uint8_t> data;
            *exists = false;
            *completed = false;

            const char separator = Native_Separator_();
            if (!Build_Path_(path, sizeof(path), "%s%c%u%cinput.bin", g_oracle_directory, separator, turn, separator)) {
                return false;
            }
            if (!File_Exists_(path)) {
                return true;
            }
            *exists = true;
            if (!Read_File_(path, &data, true) || data.size() < sizeof(s_log_header)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "oracle: invalid log '%s'", path);
                return false;
            }

            memcpy(header, data.data(), sizeof(*header));
            if (header->magic != k_log_magic || header->version != k_log_version ||
                header->size != sizeof(*header) || header->oracle_turn != turn ||
                header->qpc_frequency <= 0) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "oracle: unsupported log header '%s'", path);
                return false;
            }

            records->clear();
            size_t offset = header->size;
            bool saw_end = false;
            while (offset < data.size()) {
                if (data.size() - offset < 2 * sizeof(uint32_t)) {
                    return false;
                }

                uint32_t type;
                uint32_t size;
                memcpy(&type, data.data() + offset, sizeof(type));
                memcpy(&size, data.data() + offset + sizeof(type), sizeof(size));
                if (size < 2 * sizeof(uint32_t) || size > data.size() - offset || saw_end) {
                    return false;
                }

                if (type == k_input_record_type && size == sizeof(s_input_record)) {
                    s_input_record record;
                    memcpy(&record, data.data() + offset, sizeof(record));
                    if (record.delta_qpc < 0 || record.oracle_turn != turn ||
                        (record.input_type != k_mouse_input_type && record.input_type != k_key_input_type)) {
                        return false;
                    }
                    records->push_back(record);
                } else if (type == k_end_record_type && size == sizeof(s_end_record)) {
                    s_end_record end_record;
                    memcpy(&end_record, data.data() + offset, sizeof(end_record));
                    if (end_record.oracle_turn != turn) {
                        return false;
                    }
                    saw_end = true;
                    *completed = end_record.completed != 0;
                } else {
                    return false;
                }

                offset += size;
            }

            return saw_end;
        }

        void Restore_Input_State_() {
            const s_input_state& state = g_header.input_state;
            mouse::Restore_Input_State_(state.mouse_x, state.mouse_y, state.mouse_buttons,
                state.mouse_buffer_button, state.mouse_buffer_x, state.mouse_buffer_y,
                state.mouse_buffer_flag, state.mouse_buffer_flag2);
            key::Restore_Input_State_(state.key_pressed, state.key_write, state.key_read,
                state.last_scan_code, state.key_buffer);
        }

        e_prepare_result Prepare_Turn_(uint32_t turn) {
            bool log_exists;
            bool completed;
            s_log_header header;
            std::vector<s_input_record> records;
            if (!Read_Completed_Log_(turn, &header, &records, &log_exists, &completed)) {
                return PREPARE_ERROR;
            }
            if (!log_exists || !completed) {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: no completed input remains at turn %u", turn);
                return PREPARE_DONE;
            }

            char next_save[1024];
            const char separator = Native_Separator_();
            if (!Build_Path_(next_save, sizeof(next_save), "%s%c%u%cSAVE10.GAM", g_oracle_directory, separator, turn + 1, separator) ||
                !File_Exists_(next_save)) {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: turn %u has no following save to verify", turn);
                return PREPARE_DONE;
            }

            if (header.stardate != MOX::_stardate) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: turn %u stardate mismatch, log=%d save=%d",
                    turn, header.stardate, (int)MOX::_stardate);
                return PREPARE_ERROR;
            }

            g_turn = turn;
            g_header = header;
            g_records.swap(records);
            g_record_index = 0;
            g_replay_active = true;
            Restore_Input_State_();
            g_last_consumption_counter = SDL_GetPerformanceCounter();

            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: replaying turn %u, stardate %d, %zu input events at %u%% speed with %u ms added per event",
                turn, header.stardate, g_records.size(), g_speed_percent, g_delay_ms);
            return PREPARE_READY;
        }

        const char* Describe_Save_Offset_(size_t difference_offset, size_t* block_offset) {
            const s_save_block blocks[] = {
                {"save header", sizeof(uint32_t)},
                {"save description", sizeof(MOX::_save_game_description[0].description)},
                {"stardate", sizeof(MOX::_stardate)},
                {"game type", sizeof(MOX::_game_type)},
                {"settings", sizeof(MOX::_settings)},
                {"random seed", sizeof(uint32_t)},
                {"colony count", sizeof(MOX::_NUM_COLONIES)},
                {"colonies", sizeof(s_colony) * MAX_COLONIES},
                {"planet count", sizeof(MOX::_planet_count)},
                {"planets", sizeof(s_planet_data) * MAX_PLANETS},
                {"star count", sizeof(MOX::_NUM_STARS)},
                {"stars", sizeof(s_star_data) * MAX_STARS},
                {"leaders", sizeof(s_leader_data) * MAX_LEADERS},
                {"player count", sizeof(MOX::_NUM_PLAYERS)},
                {"players", sizeof(s_player) * MAX_PLAYERS},
                {"ship count", sizeof(MOX::_NUM_SHIPS)},
                {"ships", sizeof(s_ship_data) * MAX_SHIPS},
                {"moveable boxes", sizeof(MOX::_moveable_box[0]) * 5},
                {"moveable box order", sizeof(MOX::_moveable_box_order[0]) * 5},
                {"player number", sizeof(MOX::_PLAYER_NUM)},
                {"maximum map scale", sizeof(MOX::_max_map_scale)},
                {"maximum zoom count", sizeof(MOX::_max_zoom_count)},
                {"star background index", sizeof(MOX::_star_bg_index)},
                {"map maximum x", sizeof(MOX::_MAP_MAX_X)},
                {"map maximum y", sizeof(MOX::_MAP_MAX_Y)},
                {"fleet icon button count", sizeof(MOX::_fleet_icon_button_count)},
                {"nebulae", sizeof(*MOX::_nebula) * MAX_NEBULAS},
                {"nebula count", sizeof(MOX::_NUM_NEBULAS)},
                {"bill state", sizeof(MOX::_bill_savegame)},
                {"report count", sizeof(REPORT::_n_reports)},
                {"reports", sizeof(REPORT::_reports)},
                {"completed player reports", sizeof(REPORT::_done_player_reports)},
                {"interesting report flag", sizeof(REPORT::_something_interesting_happened)},
                {"colonize report flag", sizeof(REPORT::_done_colonize_check)},
                {"event data", sizeof(EVENTS::_event_data[0]) * 36},
                {"messages", sizeof(MSG_::_msgs[0]) * 100},
                {"first message", sizeof(MSG_::_first_msg)},
                {"maximum messages", sizeof(MSG_::_max_msgs)},
                {"message count", sizeof(MSG_::_n_msgs)},
                {"displayed messages", sizeof(REPORT::_displayed_msgs)},
                {"displayed GNN", sizeof(REPORT::_displayed_gnn)},
                {"council flag", sizeof(DIPLODEF::_COUNCIL_FLAG)},
                {"next council meeting", sizeof(DIPLODEF::_next_council_meeting_turn)},
                {"antaran resource level", sizeof(MOX::_antaran_resource_level)},
                {"antaran invasion chance", sizeof(MOX::_chance_for_antaran_invasion)},
                {"disallow saves code", sizeof(MOX::_disallow_saves_code)},
                {"last event year", sizeof(EVENTS::_last_event_year)},
                {"event delay count", sizeof(EVENTS::_event_delay_count)},
                {"antaran state", sizeof(MOX::_antarans)},
                {"current map scale", sizeof(MOX::_cur_map_scale)},
                {"current map x", sizeof(MOX::_cur_map_x)},
                {"current map y", sizeof(MOX::_cur_map_y)},
                {"save padding", 1000},
            };

            size_t start = 0;
            for (const s_save_block& block : blocks) {
                if (difference_offset < start + block.size) {
                    *block_offset = difference_offset - start;
                    return block.name;
                }
                start += block.size;
            }

            *block_offset = difference_offset - start;
            return "trailing data";
        }

        void Describe_Colony_Offset_(size_t block_offset, char* description, size_t description_size) {
            const size_t colony_index = block_offset / sizeof(s_colony);
            const size_t colony_offset = block_offset % sizeof(s_colony);
            const s_field_range fields[] = {
                {"owner", offsetof(s_colony, owner), sizeof(s_colony::owner)},
                {"allocated_to", offsetof(s_colony, allocated_to), sizeof(s_colony::allocated_to)},
                {"planet", offsetof(s_colony, planet), sizeof(s_colony::planet)},
                {"officer", offsetof(s_colony, officer), sizeof(s_colony::officer)},
                {"outpost_flag", offsetof(s_colony, outpost_flag), sizeof(s_colony::outpost_flag)},
                {"morale", offsetof(s_colony, morale), sizeof(s_colony::morale)},
                {"pollution", offsetof(s_colony, pollution), sizeof(s_colony::pollution)},
                {"n_pops", offsetof(s_colony, n_pops), sizeof(s_colony::n_pops)},
                {"specialty", offsetof(s_colony, specialty), sizeof(s_colony::specialty)},
                {"pop", offsetof(s_colony, pop), sizeof(s_colony::pop)},
                {"pop_roundoff", offsetof(s_colony, pop_roundoff), sizeof(s_colony::pop_roundoff)},
                {"pop_growth", offsetof(s_colony, pop_growth), sizeof(s_colony::pop_growth)},
                {"n_turns_existed", offsetof(s_colony, n_turns_existed), sizeof(s_colony::n_turns_existed)},
                {"food2_per_farmer", offsetof(s_colony, food2_per_farmer), sizeof(s_colony::food2_per_farmer)},
                {"industry_per_worker", offsetof(s_colony, industry_per_worker), sizeof(s_colony::industry_per_worker)},
                {"research_per_scientist", offsetof(s_colony, research_per_scientist), sizeof(s_colony::research_per_scientist)},
                {"max_farms", offsetof(s_colony, max_farms), sizeof(s_colony::max_farms)},
                {"max_population", offsetof(s_colony, max_population), sizeof(s_colony::max_population)},
                {"climate", offsetof(s_colony, climate), sizeof(s_colony::climate)},
                {"ground_strength", offsetof(s_colony, ground_strength), sizeof(s_colony::ground_strength)},
                {"space_strength", offsetof(s_colony, space_strength), sizeof(s_colony::space_strength)},
                {"production", offsetof(s_colony, production), sizeof(s_colony::production)},
                {"maintenance", offsetof(s_colony, maintenance), sizeof(s_colony::maintenance)},
                {"imports", offsetof(s_colony, imports), sizeof(s_colony::imports)},
                {"n_industry_recyclers", offsetof(s_colony, n_industry_recyclers), sizeof(s_colony::n_industry_recyclers)},
                {"food2_needed_for_our_empire", offsetof(s_colony, food2_needed_for_our_empire), sizeof(s_colony::food2_needed_for_our_empire)},
                {"food2_needed_for_assimilated", offsetof(s_colony, food2_needed_for_assimilated), sizeof(s_colony::food2_needed_for_assimilated)},
                {"food2_needed_for_conquered", offsetof(s_colony, food2_needed_for_conquered), sizeof(s_colony::food2_needed_for_conquered)},
                {"food2_needed_for_natives", offsetof(s_colony, food2_needed_for_natives), sizeof(s_colony::food2_needed_for_natives)},
                {"industry2_needed_for_our_empire", offsetof(s_colony, industry2_needed_for_our_empire), sizeof(s_colony::industry2_needed_for_our_empire)},
                {"industry2_needed_for_androids", offsetof(s_colony, industry2_needed_for_androids), sizeof(s_colony::industry2_needed_for_androids)},
                {"industry2_needed_for_assimilated", offsetof(s_colony, industry2_needed_for_assimilated), sizeof(s_colony::industry2_needed_for_assimilated)},
                {"industry2_needed_for_conquered", offsetof(s_colony, industry2_needed_for_conquered), sizeof(s_colony::industry2_needed_for_conquered)},
                {"food2_needed_for_empire", offsetof(s_colony, food2_needed_for_empire), sizeof(s_colony::food2_needed_for_empire)},
                {"industry2_needed_for_empire", offsetof(s_colony, industry2_needed_for_empire), sizeof(s_colony::industry2_needed_for_empire)},
                {"n_food_replicated", offsetof(s_colony, n_food_replicated), sizeof(s_colony::n_food_replicated)},
                {"producing", offsetof(s_colony, producing), sizeof(s_colony::producing)},
                {"just_produced", offsetof(s_colony, just_produced), sizeof(s_colony::just_produced)},
                {"production_spent", offsetof(s_colony, production_spent), sizeof(s_colony::production_spent)},
                {"n_industry_taxed", offsetof(s_colony, n_industry_taxed), sizeof(s_colony::n_industry_taxed)},
                {"auto_building", offsetof(s_colony, auto_building), sizeof(s_colony::auto_building)},
                {"production_surplus", offsetof(s_colony, production_surplus), sizeof(s_colony::production_surplus)},
                {"bought_outright", offsetof(s_colony, bought_outright), sizeof(s_colony::bought_outright)},
                {"occupation_points", offsetof(s_colony, occupation_points), sizeof(s_colony::occupation_points)},
                {"occupation_policy", offsetof(s_colony, occupation_policy), sizeof(s_colony::occupation_policy)},
                {"military", offsetof(s_colony, military), sizeof(s_colony::military)},
                {"tank_roundoff", offsetof(s_colony, tank_roundoff), sizeof(s_colony::tank_roundoff)},
                {"infantry_roundoff", offsetof(s_colony, infantry_roundoff), sizeof(s_colony::infantry_roundoff)},
                {"buildings", offsetof(s_colony, buildings), sizeof(s_colony::buildings)},
                {"last_turn_building_destroyed", offsetof(s_colony, last_turn_building_destroyed), sizeof(s_colony::last_turn_building_destroyed)},
            };

            for (const s_field_range& field : fields) {
                if (colony_offset >= field.offset && colony_offset < field.offset + field.size) {
                    snprintf(description, description_size, "MOX::_colony[%zu].%s +%zu",
                        colony_index, field.name, colony_offset - field.offset);
                    return;
                }
            }

            snprintf(description, description_size, "MOX::_colony[%zu] +%zu",
                colony_index, colony_offset);
        }

        void Describe_Player_Offset_(size_t block_offset, char* description, size_t description_size) {
            const size_t player_index = block_offset / sizeof(s_player);
            const size_t player_offset = block_offset % sizeof(s_player);
#define PLAYER_FIELD(name) {#name, offsetof(s_player, name), sizeof(s_player::name), 0}
#define PLAYER_ARRAY(name) {#name, offsetof(s_player, name), sizeof(s_player::name), sizeof(((s_player*)nullptr)->name[0])}
            const s_field_range fields[] = {
                PLAYER_FIELD(picture),
                PLAYER_ARRAY(name),
                PLAYER_ARRAY(race_name),
                PLAYER_FIELD(eliminated),
                PLAYER_FIELD(race),
                PLAYER_FIELD(color),
                PLAYER_FIELD(personality),
                PLAYER_FIELD(objectives),
                PLAYER_FIELD(home_planet_id),
                PLAYER_FIELD(network_player_id),
                PLAYER_FIELD(player_done_flags),
                PLAYER_FIELD(dead_field),
                PLAYER_FIELD(research_breakthrough),
                PLAYER_FIELD(tax_rate),
                PLAYER_FIELD(bc),
                PLAYER_FIELD(n_freighters),
                PLAYER_FIELD(surplus_freighters),
                PLAYER_FIELD(command_points),
                PLAYER_FIELD(command_points_used),
                PLAYER_FIELD(food_freighted),
                PLAYER_FIELD(settlers_freighted),
                PLAYER_ARRAY(settlers),
                PLAYER_FIELD(total_pop),
                PLAYER_FIELD(food_produced),
                PLAYER_FIELD(industry_produced),
                PLAYER_FIELD(research_produced),
                PLAYER_FIELD(bc_produced),
                PLAYER_FIELD(surplus_food),
                PLAYER_FIELD(surplus_bc),
                PLAYER_FIELD(total_maintenance),
                PLAYER_ARRAY(maintenance),
                PLAYER_ARRAY(tech_fields),
                PLAYER_ARRAY(tech_applications),
                PLAYER_FIELD(research_accumulated),
                PLAYER_FIELD(captured_orion),
                PLAYER_FIELD(captured_antares),
                PLAYER_FIELD(won_council_vote),
                PLAYER_ARRAY(player_defeated),
                PLAYER_FIELD(captured_population),
                PLAYER_FIELD(last_attacker),
                PLAYER_FIELD(ship_design_theme),
                PLAYER_FIELD(ship_special_theme),
                PLAYER_ARRAY(tech_apps_learned),
                PLAYER_ARRAY(tech_modifiers),
                PLAYER_ARRAY(hyper_advanced_tech),
                PLAYER_FIELD(total_known_tech_cost),
                PLAYER_FIELD(knows_racial_stats),
                PLAYER_ARRAY(last_tech_report_date),
                PLAYER_ARRAY(player_tech_applications),
                PLAYER_FIELD(current_research_field),
                PLAYER_FIELD(current_research_application),
                PLAYER_FIELD(got_new_app),
                PLAYER_FIELD(ship_range),
                PLAYER_ARRAY(ship_designs),
                PLAYER_ARRAY(ships_built),
                PLAYER_ARRAY(contact),
                PLAYER_FIELD(just_established_contact),
                PLAYER_ARRAY(n_times_established_contact),
                PLAYER_ARRAY(within_range),
                PLAYER_FIELD(do_not_research),
                PLAYER_FIELD(offensive_stagepoint),
                PLAYER_FIELD(best_ftl_drive),
                PLAYER_FIELD(ship_speed),
                PLAYER_FIELD(officer_for_hire),
                PLAYER_FIELD(n_trade_pops),
                PLAYER_ARRAY(current_trade_agreement_level),
                PLAYER_ARRAY(trade_agreement_goal),
                PLAYER_FIELD(n_research_pops),
                PLAYER_ARRAY(current_research_agreement_level),
                PLAYER_ARRAY(research_agreement_goal),
                PLAYER_FIELD(fighter_beam_type),
                PLAYER_FIELD(fighter_bomb_type),
                PLAYER_FIELD(total_research),
                PLAYER_FIELD(total_ships),
                PLAYER_ARRAY(total_ship_strength),
                PLAYER_FIELD(total_colonies),
                PLAYER_FIELD(failed_to_expand),
                PLAYER_ARRAY(first_contact),
                PLAYER_ARRAY(relations),
                PLAYER_ARRAY(base_relations),
                PLAYER_ARRAY(treaty),
                PLAYER_ARRAY(trade_treaty),
                PLAYER_ARRAY(research_treaty),
                PLAYER_ARRAY(tribute_treaty),
                PLAYER_ARRAY(diplomacy_incident),
                PLAYER_ARRAY(diplomacy_message),
                PLAYER_ARRAY(diplomacy_severity),
                PLAYER_ARRAY(diplomacy_value),
                PLAYER_ARRAY(diplomacy_system),
                PLAYER_ARRAY(treaty_modifier),
                PLAYER_ARRAY(trade_modifier),
                PLAYER_ARRAY(tech_exchange_modifier),
                PLAYER_ARRAY(peace_modifier),
                PLAYER_ARRAY(last_bad_diplomatic_incident),
                PLAYER_ARRAY(broken_treaties_modifier),
                PLAYER_ARRAY(last_broken_treaty),
                PLAYER_ARRAY(last_gift),
                PLAYER_ARRAY(ship_war_losses),
                PLAYER_ARRAY(colony_war_losses),
                PLAYER_ARRAY(biological_weapons_flag),
                PLAYER_ARRAY(time_since_last_attack),
                PLAYER_ARRAY(threats),
                PLAYER_ARRAY(dishonored_flag),
                PLAYER_ARRAY(peace_duration),
                PLAYER_ARRAY(last_turn_diplomacy_severity),
                PLAYER_ARRAY(last_broken_treaty_type),
                PLAYER_ARRAY(last_turn_treaty),
                PLAYER_ARRAY(diplomacy_proposal_request),
                PLAYER_ARRAY(diplomacy_proposal_tribute_bribe),
                PLAYER_ARRAY(diplomacy_proposal_tech_bribe1),
                PLAYER_ARRAY(diplomacy_proposal_tech_bribe2),
                PLAYER_ARRAY(diplomacy_proposal_gold_bribe),
                PLAYER_ARRAY(diplomacy_proposal_war_player),
                PLAYER_ARRAY(diplomacy_proposal_exchange_max_value),
                PLAYER_ARRAY(diplomacy_proposal_exchange_tech),
                PLAYER_ARRAY(diplomacy_proposal_system_bribe),
                PLAYER_FIELD(sneak_attack_planet),
                PLAYER_FIELD(sneak_attack_message),
                PLAYER_FIELD(sneak_attack_player),
                PLAYER_FIELD(council_threat),
                PLAYER_ARRAY(reward_amount),
                PLAYER_ARRAY(reward_tech),
                PLAYER_ARRAY(reward_attack_player),
                PLAYER_FIELD(food_shortage_duration),
                PLAYER_ARRAY(trust_worthiness),
                PLAYER_ARRAY(trust_breaker_player),
                PLAYER_ARRAY(trust_breaker_treaty),
                PLAYER_ARRAY(stop_spying_duration),
                PLAYER_ARRAY(stop_blockading_duration),
                PLAYER_FIELD(war_buildup_duration),
                PLAYER_ARRAY(last_diplomacy_proposal_turn),
                PLAYER_ARRAY(diplomacy_proposal_rejection),
                PLAYER_ARRAY(opportunity_planet_num),
                PLAYER_ARRAY(opportunity_planet_value),
                PLAYER_ARRAY(opportunity_actual_attacker_ratio),
                PLAYER_ARRAY(temp_diplomacy_dummy1),
                PLAYER_ARRAY(diplomacy_demand_rejection_message),
                PLAYER_ARRAY(last_diplomacy_threat_turn),
                PLAYER_ARRAY(delayed_diplomacy_orders),
                PLAYER_ARRAY(traits),
                PLAYER_ARRAY(evolutionary_upgrade_traits),
                PLAYER_FIELD(picked_evolutionary_upgrade),
                PLAYER_FIELD(applied_evolutionary_upgrade),
                PLAYER_ARRAY(fleet_history),
                PLAYER_ARRAY(tech_history),
                PLAYER_ARRAY(population_history),
                PLAYER_ARRAY(production_history),
                PLAYER_ARRAY(spies),
                PLAYER_FIELD(history_btns),
                PLAYER_FIELD(ignoring),
                PLAYER_ARRAY(peace_flags),
                PLAYER_FIELD(cheated),
                PLAYER_FIELD(stardate_last_offered_hero),
                PLAYER_FIELD(current_zoom_level),
                PLAYER_FIELD(cur_map_x),
                PLAYER_FIELD(cur_map_y),
                PLAYER_FIELD(council_voted_for),
                PLAYER_FIELD(surrender_to),
                PLAYER_FIELD(time_since_last_lucky_event),
                PLAYER_FIELD(has_galactic_lore),
                PLAYER_FIELD(warned_about_cheating_player),
                PLAYER_FIELD(cheated_this_turn),
                PLAYER_FIELD(tribute_income),
                PLAYER_ARRAY(filler),
            };
#undef PLAYER_ARRAY
#undef PLAYER_FIELD

            for (const s_field_range& field : fields) {
                if (player_offset < field.offset || player_offset >= field.offset + field.size) {
                    continue;
                }

                const size_t field_offset = player_offset - field.offset;
                if (field.element_size != 0) {
                    snprintf(description, description_size, "MOX::_player[%zu].%s[%zu] +%zu",
                        player_index, field.name, field_offset / field.element_size,
                        field_offset % field.element_size);
                } else {
                    snprintf(description, description_size, "MOX::_player[%zu].%s +%zu",
                        player_index, field.name, field_offset);
                }
                return;
            }

            snprintf(description, description_size, "MOX::_player[%zu] +%zu",
                player_index, player_offset);
        }

        bool Compare_Next_State_(uint32_t next_turn) {
            char actual_path[1024];
            char expected_path[1024];
            const char separator = Native_Separator_();
            if (!Save_Actual_State_(next_turn, actual_path, sizeof(actual_path)) ||
                !Build_Path_(expected_path, sizeof(expected_path), "%s%c%u%cSAVE10.GAM", g_oracle_directory, separator, next_turn, separator)) {
                return false;
            }

            std::vector<uint8_t> actual;
            std::vector<uint8_t> expected;
            if (!Read_File_(actual_path, &actual, true) || !Read_File_(expected_path, &expected, true)) {
                return false;
            }

            const size_t common_size = std::min(actual.size(), expected.size());
            size_t difference = 0;
            while (difference < common_size && actual[difference] == expected[difference]) {
                ++difference;
            }
            if (difference == common_size && actual.size() == expected.size()) {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: turn %u matches '%s' (%zu bytes)", next_turn, expected_path, expected.size());
                return true;
            }

            if (!Load_Turn_State_(next_turn)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: mismatch found but expected turn %u could not be restored", next_turn);
                return false;
            }

            size_t block_offset;
            const char* block_name = Describe_Save_Offset_(difference, &block_offset);
            char field_description[128] = {};
            if (strcmp(block_name, "colonies") == 0) {
                Describe_Colony_Offset_(block_offset, field_description, sizeof(field_description));
            } else if (strcmp(block_name, "players") == 0) {
                Describe_Player_Offset_(block_offset, field_description, sizeof(field_description));
            }
            const int actual_byte = difference < actual.size() ? actual[difference] : -1;
            const int expected_byte = difference < expected.size() ? expected[difference] : -1;
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: turn %u differs at save offset %zu (%s +%zu%s%s): actual=%d expected=%d; sizes actual=%zu expected=%zu",
                next_turn, difference, block_name, block_offset,
                field_description[0] != '\0' ? ", " : "", field_description,
                actual_byte, expected_byte,
                actual.size(), expected.size());
            return false;
        }

        bool Fail_(const char* message) {
            g_replay_active = false;
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "oracle: %s", message);
            platform::Request_Exit_(message);
            return false;
        }
    }

    bool Configure_(int argc, char** argv) {
        g_enabled = false;
        g_replay_active = false;
        g_turn = 0;
        g_speed_percent = 100;
        g_delay_ms = 0;

        for (int i = 1; i < argc; ++i) {
            const char* value = nullptr;
            if (strcmp(argv[i], "--oracle") == 0 || strcmp(argv[i], "-oracle") == 0 ||
                strcmp(argv[i], "/oracle") == 0) {
                if (++i >= argc) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "oracle: --oracle requires a turn number");
                    return false;
                }
                value = argv[i];
            } else if (strncmp(argv[i], "--oracle=", 9) == 0) {
                value = argv[i] + 9;
            } else if (strcmp(argv[i], "--oracle-speed") == 0) {
                if (++i >= argc || !Parse_Uint32_(argv[i], 1, 1000, &g_speed_percent)) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: --oracle-speed requires a percentage from 1 to 1000");
                    return false;
                }
                continue;
            } else if (strncmp(argv[i], "--oracle-speed=", 15) == 0) {
                if (!Parse_Uint32_(argv[i] + 15, 1, 1000, &g_speed_percent)) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: invalid speed percentage '%s'", argv[i] + 15);
                    return false;
                }
                continue;
            } else if (strcmp(argv[i], "--oracle-delay-ms") == 0) {
                if (++i >= argc || !Parse_Uint32_(argv[i], 0, 60000, &g_delay_ms)) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: --oracle-delay-ms requires a value from 0 to 60000");
                    return false;
                }
                continue;
            } else if (strncmp(argv[i], "--oracle-delay-ms=", 18) == 0) {
                if (!Parse_Uint32_(argv[i] + 18, 0, 60000, &g_delay_ms)) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: invalid input delay '%s'", argv[i] + 18);
                    return false;
                }
                continue;
            } else {
                continue;
            }

            if (g_enabled || !Parse_Turn_(value, &g_turn)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: invalid or duplicate turn option '%s'", value);
                return false;
            }
            g_enabled = true;
            MOX2::_skip_intro = 1;
        }

        if (!g_enabled) {
            return true;
        }

        char* current_directory = SDL_GetCurrentDirectory();
        if (current_directory == nullptr) {
            return false;
        }
        const int length = snprintf(g_oracle_directory, sizeof(g_oracle_directory),
            "%s%coracle", current_directory, Native_Separator_());
        SDL_free(current_directory);
        if (length < 0 || (size_t)length >= sizeof(g_oracle_directory)) {
            return false;
        }

        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
            "oracle: enabled at turn %u using '%s', speed=%u%%, delay=%u ms",
            g_turn, g_oracle_directory, g_speed_percent, g_delay_ms);
        return true;
    }

    bool Start_() {
        if (!g_enabled) {
            return true;
        }

        if (!Load_Turn_State_(g_turn)) {
            return Fail_("Oracle could not load the requested turn save. See the game log.");
        }

        MOX::_return_screen = SCREEN_MAIN;
        MOX::_current_screen = SCREEN_MAIN;
        const e_prepare_result result = Prepare_Turn_(g_turn);
        if (result == PREPARE_ERROR) {
            return Fail_("Oracle could not read the requested turn input. See the game log.");
        }
        if (result == PREPARE_DONE) {
            MOX::_current_screen = SCREEN_EXIT;
        }
        return true;
    }

    void Pump_Input_() {
        if (!g_replay_active || g_record_index >= g_records.size()) {
            return;
        }

        const uint64_t elapsed_budget = SDL_GetPerformanceCounter() - g_last_consumption_counter;
        uint64_t consumed_budget = 0;
        bool consumed_input = false;

        while (g_record_index < g_records.size()) {
            const uint64_t required_delta = Replay_Delta_Counters_(
                g_records[g_record_index].delta_qpc);
            if (required_delta > elapsed_budget - consumed_budget) {
                break;
            }

            const s_input_record& record = g_records[g_record_index];
            if (record.input_type == k_mouse_input_type) {
                mouse::User_Mouse_Handler_(record.mouse_x, record.mouse_y, record.mouse_buttons);
            } else {
                key::Push_Key_Code_(record.key_code, record.scan_code);
            }

            consumed_budget += required_delta;
            consumed_input = true;
            ++g_record_index;
        }

        if (consumed_input) {
            g_last_consumption_counter = SDL_GetPerformanceCounter();
        }
    }

    void Complete_Turn_() {
        if (!g_replay_active) {
            return;
        }

        g_replay_active = false;
        if (g_record_index != g_records.size()) {
            Fail_("Oracle turn ended before all recorded input was injected. See the game log.");
            throw platform::process_exit_requested{};
        }

        const uint32_t next_turn = g_turn + 1;
        if (!Compare_Next_State_(next_turn)) {
            Fail_("Oracle game-state mismatch. See the game log for the first difference.");
            throw platform::process_exit_requested{};
        }

        const e_prepare_result result = Prepare_Turn_(next_turn);
        if (result == PREPARE_ERROR) {
            Fail_("Oracle could not read the next turn input. See the game log.");
            throw platform::process_exit_requested{};
        }
        if (result == PREPARE_DONE) {
            MOX::_current_screen = SCREEN_EXIT;
        }
    }

    bool Physical_Input_Enabled_() {
        return !g_enabled;
    }
}