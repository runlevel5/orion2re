#include "pch.h"

#include "game/savegame.h"

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
            uint32_t random_call_count;
            uint32_t random_seed_count;
            uint32_t tactical_state_count;
            uint32_t strategic_state_count;
            uint32_t presentation_complete_count;
        };

        struct s_random_record {
            uint32_t type;
            uint32_t size;
            uint32_t oracle_turn;
            int32_t stardate;
            uint32_t call_index;
            uint32_t max_value;
            uint32_t value;
            uint32_t random_seed;
            uint32_t return_address_count;
            uint32_t return_addresses[10];
        };

        struct s_random_seed_record {
            uint32_t type;
            uint32_t size;
            uint32_t oracle_turn;
            int32_t stardate;
            uint32_t call_index;
            uint32_t random_seed;
            uint32_t return_address_count;
            uint32_t return_addresses[10];
        };

        struct s_tactical_scalar_state {
            int16_t combat_attacker_count;
            int16_t combat_defender_count;
            int16_t combat_attacker_player;
            int16_t combat_defender_player;
            int16_t combat_turn;
            int16_t current_ship;
            int16_t current_pointer;
            int16_t current_missile_count;
            uint16_t combat_init;
            uint16_t apply_damage_to_planet;
            int16_t in_nebula_flag;
            int16_t auto_combat;
            int16_t take_control_flag;
            uint8_t colony_defense_flags;
            uint8_t real_attack_flag;
            int16_t combat_colony_id;
            uint16_t bio_weapon_flag;
            int16_t ai_retreat_flag;
            uint16_t start_of_turn;
            int16_t fleet_target;
        };

        struct s_historical_combat_bomb_result {
            int8_t attacker_idx;
            uint8_t colony_idx;
            int8_t is_planetary_hit;
            int16_t bomb_hits_remaining;
            int8_t is_bio_weapon;
            int16_t bio_hits_remaining;
            int16_t infantry_killed;
            int16_t tanks_killed;
            int16_t population_killed;
            int8_t buildings_destroyed[BUILDING_COUNT];
            int16_t bio_pop_killed;
            int8_t damage_occurred;
            int8_t colony_wiped_out;
            int16_t production_lost;
            int32_t total_hits_processed;
        };

        struct s_tactical_state_record {
            uint32_t type;
            uint32_t size;
            uint32_t oracle_turn;
            int32_t stardate;
            uint32_t combat_instance;
            uint32_t checkpoint_index;
            uint32_t checkpoint_kind;
            int16_t combat_turn;
            int16_t combat_total_ship_count;
            int16_t active_player_id;
            int16_t time_warp_only;
            uint32_t random_call_count;
            uint32_t random_seed_count;
            uint32_t random_seed;
            uint32_t scalar_size;
            uint32_t combat_data_size;
            uint32_t missile_data_size;
            uint32_t turn_order_size;
            uint32_t combat_grid_size;
            uint32_t global_combat_data_size;
            uint32_t combat_bomb_result_size;
            uint32_t tactical_arrays_size;
            uint32_t colony_size;
        };

        struct s_strategic_state_record {
            uint32_t type;
            uint32_t size;
            uint32_t oracle_turn;
            int32_t stardate;
            uint32_t combat_instance;
            uint32_t checkpoint_index;
            int16_t combat_turn;
            int16_t combat_total_ship_count;
            int16_t combat_attacker_count;
            int16_t combat_defender_count;
            int16_t combat_attacker_player;
            int16_t combat_defender_player;
            int16_t combat_colony_id;
            uint8_t colony_defense_flags;
            uint8_t reserved;
            uint32_t random_call_count;
            uint32_t random_seed_count;
            uint32_t random_seed;
            uint32_t quick_combat_data_size;
            uint32_t colony_size;
        };

        struct s_presentation_complete_record {
            uint32_t type;
            uint32_t size;
            uint32_t oracle_turn;
            int32_t stardate;
            uint32_t sequence;
            uint32_t presentation_kind;
            uint32_t input_record_count;
            uint32_t random_call_count;
            uint32_t random_seed_count;
        };
#pragma pack(pop)

        static_assert(sizeof(s_input_state) == 64, "oracle input state layout mismatch");
        static_assert(sizeof(s_log_header) == 100, "oracle log header layout mismatch");
        static_assert(sizeof(s_input_record) == 44, "oracle input record layout mismatch");
        static_assert(sizeof(s_end_record) == 48, "oracle end record layout mismatch");
        static_assert(sizeof(s_random_record) == 76, "oracle random record layout mismatch");
        static_assert(sizeof(s_random_seed_record) == 68, "oracle random seed record layout mismatch");
        static_assert(sizeof(s_tactical_scalar_state) == 38, "oracle tactical scalar layout mismatch");
        static_assert(sizeof(s_historical_combat_bomb_result) == 73,
            "historical oracle combat bomb result layout mismatch");
        static_assert(sizeof(s_tactical_state_record) == 84, "oracle tactical record layout mismatch");
        static_assert(sizeof(s_strategic_state_record) == 60,
            "oracle strategic record layout mismatch");
        static_assert(sizeof(s_presentation_complete_record) == 36,
            "oracle presentation completion layout mismatch");

        constexpr uint32_t k_log_magic = 0x4C43524F;
        constexpr uint32_t k_log_version = 9;
        constexpr uint32_t k_input_record_type = 1;
        constexpr uint32_t k_end_record_type = 2;
        constexpr uint32_t k_random_record_type = 3;
        constexpr uint32_t k_random_seed_record_type = 4;
        constexpr uint32_t k_tactical_state_record_type = 5;
        constexpr uint32_t k_presentation_complete_record_type = 6;
        constexpr uint32_t k_strategic_state_record_type = 7;
        constexpr uint32_t k_tactical_turn_begin = 1;
        constexpr uint32_t k_tactical_combat_end = 2;
        constexpr uint32_t k_ground_combat_presentation = 1;
        constexpr uint32_t k_bombing_presentation = 2;
        constexpr uint32_t k_mouse_input_type = 1;
        constexpr uint32_t k_key_input_type = 2;
        constexpr uint32_t k_return_address_capacity = 10;
        constexpr uintptr_t k_original_image_base = 0x00400000;
        constexpr uint32_t k_original_combat_data_size = 313;
        constexpr uint32_t k_original_missile_size = 26;
        constexpr uint32_t k_original_quick_combat_data_size = 33;
        constexpr uint32_t k_original_quick_combat_capacity = 501;
        constexpr uint32_t k_combat_ship_capacity = 210;
        constexpr uint32_t k_missile_capacity = 300;
        constexpr uint32_t k_ship_turn_order_size = 210 * sizeof(int16_t);
        constexpr uint32_t k_combat_grid_size = 81 * 68 * sizeof(int16_t);
        constexpr uint32_t k_global_combat_data_size = 313;
        constexpr uint32_t k_historical_combat_bomb_result_size =
            sizeof(s_historical_combat_bomb_result);
        constexpr uint32_t k_combat_bomb_result_size = sizeof(s_combat_bomb_result);
        constexpr uint32_t k_colony_size = 361;
        constexpr uint32_t k_tactical_arrays_size =
            15 * sizeof(int16_t) + 15 * sizeof(int8_t) + 2 * sizeof(int16_t) +
            15 * sizeof(int16_t) + 9 * sizeof(int16_t) + 9 * sizeof(int16_t);

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

        struct s_tactical_state {
            s_tactical_state_record record;
            std::vector<uint8_t> payload;
        };

        struct s_strategic_state {
            s_strategic_state_record record;
            std::vector<uint8_t> payload;
        };

        struct s_input_position {
            uint32_t random_call_count;
            uint32_t random_seed_count;
        };

        struct s_presentation_complete {
            s_presentation_complete_record record;
            size_t terminal_input_index;
        };

        bool g_enabled;
        bool g_replay_active;
        uint32_t g_turn;
        char g_oracle_directory[1024];
        s_log_header g_header;
        std::vector<s_input_record> g_records;
        std::vector<s_input_position> g_record_positions;
        std::vector<s_random_record> g_random_records;
        std::vector<s_random_seed_record> g_random_seed_records;
        std::vector<s_tactical_state> g_tactical_states;
        std::vector<s_strategic_state> g_strategic_states;
        std::vector<s_presentation_complete> g_presentation_completions;
        size_t g_record_index;
        size_t g_random_record_index;
        size_t g_random_seed_record_index;
        size_t g_tactical_state_index;
        size_t g_strategic_state_index;
        size_t g_presentation_completion_index;
        uint32_t g_active_presentation_kind;
        bool g_active_presentation_ready;
        bool g_presentation_rng_desynced;
        uint32_t g_active_combat_instance;
        uint32_t g_active_strategic_combat_instance;
        int16_t g_last_verified_combat_turn;
        bool g_tactical_combat_active;
        uint64_t g_last_consumption_counter;
        uint64_t g_click_pause_until_counter;
        uint32_t g_speed_percent;
        uint32_t g_delay_ms;
        uint32_t g_click_delay_ms;
        uint16_t g_replay_mouse_buttons;
        bool g_click_pause_active;
        bool g_replay_clock_armed;

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

        uint64_t Milliseconds_To_Counters_(uint32_t milliseconds) {
            return ((uint64_t)milliseconds * SDL_GetPerformanceFrequency()) / 1000;
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
                FILEDEF::Load_Game_Settings_();
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
            char settings_path[1024];
            const char separator = Native_Separator_();
            if (!Build_Path_(directory, sizeof(directory), "%s%c%u", g_oracle_directory, separator, turn) ||
                !Build_Path_(save_path, sizeof(save_path), "%s%c%u%cSAVE10.GAM", g_oracle_directory, separator, turn, separator) ||
                !Build_Path_(settings_path, sizeof(settings_path), "%s%c%u%cMOX.SET", g_oracle_directory, separator, turn, separator)) {
                return false;
            }

            if (!File_Exists_(save_path)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "oracle: missing turn %u save '%s'", turn, save_path);
                return false;
            }
            if (!File_Exists_(settings_path)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "oracle: missing turn %u settings '%s'", turn, settings_path);
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

        bool Find_Presentation_Terminal_Input_(
            const std::vector<s_input_record>& records,
            size_t range_begin, size_t range_end,
            uint16_t initial_mouse_buttons,
            size_t* terminal_input_index) {
            if (range_begin >= range_end || range_end > records.size()) {
                return false;
            }

            uint16_t mouse_buttons = initial_mouse_buttons;
            for (size_t i = 0; i < range_begin; ++i) {
                if (records[i].input_type == k_mouse_input_type) {
                    mouse_buttons = records[i].mouse_buttons;
                }
            }

            bool found = false;
            size_t candidate = range_begin;
            for (size_t i = range_begin; i < range_end; ++i) {
                const s_input_record& record = records[i];
                if (record.input_type == k_key_input_type) {
                    candidate = i;
                    found = true;
                    continue;
                }

                const uint16_t pressed =
                    record.mouse_buttons &
                    static_cast<uint16_t>(~mouse_buttons);
                mouse_buttons = record.mouse_buttons;
                if (pressed != 0) {
                    candidate = i;
                    found = true;
                }
            }

            if (!found) {
                return false;
            }

            *terminal_input_index = candidate;
            return true;
        }

        bool Normalize_Historical_Tactical_State_(s_tactical_state* state) {
            s_tactical_state_record& record = state->record;
            if (record.combat_bomb_result_size == k_combat_bomb_result_size) {
                return true;
            }
            if (record.combat_bomb_result_size !=
                    k_historical_combat_bomb_result_size) {
                return false;
            }

            const size_t bomb_result_offset =
                record.scalar_size +
                record.combat_data_size +
                record.missile_data_size +
                record.turn_order_size +
                record.combat_grid_size +
                record.global_combat_data_size;
            if (bomb_result_offset + k_historical_combat_bomb_result_size >
                    state->payload.size()) {
                return false;
            }

            s_historical_combat_bomb_result historical;
            memcpy(&historical, state->payload.data() + bomb_result_offset,
                sizeof(historical));
            s_combat_bomb_result normalized = {};
            normalized.attacker_idx = historical.attacker_idx;
            normalized.colony_idx = historical.colony_idx;
            normalized.is_planetary_hit = historical.is_planetary_hit;
            normalized.bomb_hits_remaining = historical.bomb_hits_remaining;
            normalized.is_bio_weapon = historical.is_bio_weapon;
            normalized.bio_hits_remaining = historical.bio_hits_remaining;
            normalized.infantry_killed = historical.infantry_killed;
            normalized.tanks_killed = historical.tanks_killed;
            normalized.population_killed = historical.population_killed;
            memcpy(normalized.buildings_destroyed,
                historical.buildings_destroyed,
                sizeof(normalized.buildings_destroyed));
            normalized.bio_pop_killed = historical.bio_pop_killed;
            normalized.damage_occurred = historical.damage_occurred;
            normalized.colony_wiped_out = historical.colony_wiped_out;
            normalized.production_lost = historical.production_lost;
            normalized.total_hits_processed = historical.total_hits_processed;

            const size_t historical_tail_offset = bomb_result_offset +
                k_historical_combat_bomb_result_size;
            const size_t normalized_tail_offset = bomb_result_offset +
                k_combat_bomb_result_size;
            std::vector<uint8_t> normalized_payload(
                state->payload.size() + k_combat_bomb_result_size -
                    k_historical_combat_bomb_result_size);
            memcpy(normalized_payload.data(), state->payload.data(),
                bomb_result_offset);
            memcpy(normalized_payload.data() + bomb_result_offset,
                &normalized, sizeof(normalized));
            memcpy(normalized_payload.data() + normalized_tail_offset,
                state->payload.data() + historical_tail_offset,
                state->payload.size() - historical_tail_offset);
            state->payload.swap(normalized_payload);
            record.combat_bomb_result_size = k_combat_bomb_result_size;
            record.size += k_combat_bomb_result_size -
                k_historical_combat_bomb_result_size;
            return true;
        }

        bool Read_Completed_Log_(uint32_t turn, s_log_header* header,
            std::vector<s_input_record>* records,
            std::vector<s_input_position>* record_positions,
            std::vector<s_random_record>* random_records,
            std::vector<s_random_seed_record>* random_seed_records,
            std::vector<s_tactical_state>* tactical_states,
            std::vector<s_strategic_state>* strategic_states,
            std::vector<s_presentation_complete>* presentation_completions,
            bool* exists, bool* completed) {
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
            record_positions->clear();
            random_records->clear();
            random_seed_records->clear();
            tactical_states->clear();
            strategic_states->clear();
            presentation_completions->clear();
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
                    record_positions->push_back({
                        static_cast<uint32_t>(random_records->size()),
                        static_cast<uint32_t>(random_seed_records->size())
                    });
                } else if (type == k_random_record_type && size == sizeof(s_random_record)) {
                    s_random_record record;
                    memcpy(&record, data.data() + offset, sizeof(record));
                    if (record.oracle_turn != turn || record.stardate != header->stardate ||
                        record.call_index != random_records->size() ||
                        record.return_address_count > k_return_address_capacity) {
                        return false;
                    }
                    random_records->push_back(record);
                } else if (type == k_random_seed_record_type && size == sizeof(s_random_seed_record)) {
                    s_random_seed_record record;
                    memcpy(&record, data.data() + offset, sizeof(record));
                    if (record.oracle_turn != turn || record.stardate != header->stardate ||
                        record.call_index != random_seed_records->size() ||
                        record.return_address_count > k_return_address_capacity) {
                        return false;
                    }
                    random_seed_records->push_back(record);
                } else if (type == k_tactical_state_record_type &&
                    size >= sizeof(s_tactical_state_record)) {
                    s_tactical_state state;
                    memcpy(&state.record, data.data() + offset, sizeof(state.record));

                    const s_tactical_state_record& record = state.record;
                    const bool valid_checkpoint_kind =
                        record.checkpoint_kind == k_tactical_turn_begin ||
                        record.checkpoint_kind == k_tactical_combat_end;
                    const bool valid_colony_size =
                        record.colony_size == 0 || record.colony_size == k_colony_size;
                    const bool valid_combat_bomb_result_size =
                        record.combat_bomb_result_size ==
                            k_historical_combat_bomb_result_size ||
                        record.combat_bomb_result_size ==
                            k_combat_bomb_result_size;

                    uint64_t expected_size = sizeof(s_tactical_state_record);
                    expected_size += record.scalar_size;
                    expected_size += record.combat_data_size;
                    expected_size += record.missile_data_size;
                    expected_size += record.turn_order_size;
                    expected_size += record.combat_grid_size;
                    expected_size += record.global_combat_data_size;
                    expected_size += record.combat_bomb_result_size;
                    expected_size += record.tactical_arrays_size;
                    expected_size += record.colony_size;

                    if (record.oracle_turn != turn ||
                        record.stardate != header->stardate ||
                        record.combat_instance == 0 ||
                        record.combat_total_ship_count < 0 ||
                        record.combat_total_ship_count >
                            static_cast<int16_t>(k_combat_ship_capacity) ||
                        !valid_checkpoint_kind ||
                        !valid_colony_size ||
                        record.scalar_size != sizeof(s_tactical_scalar_state) ||
                        record.combat_data_size !=
                            static_cast<uint32_t>(record.combat_total_ship_count) *
                                k_original_combat_data_size ||
                        record.missile_data_size !=
                            k_missile_capacity * k_original_missile_size ||
                        record.turn_order_size != k_ship_turn_order_size ||
                        record.combat_grid_size != k_combat_grid_size ||
                        record.global_combat_data_size !=
                            k_global_combat_data_size ||
                        !valid_combat_bomb_result_size ||
                        record.tactical_arrays_size != k_tactical_arrays_size ||
                        expected_size != size ||
                        record.random_call_count != random_records->size() ||
                        record.random_seed_count != random_seed_records->size()) {
                        return false;
                    }

                    if (tactical_states->empty()) {
                        if (record.combat_instance != 1 ||
                            record.checkpoint_index != 0) {
                            return false;
                        }
                    } else {
                        const s_tactical_state_record& previous =
                            tactical_states->back().record;
                        if (record.combat_instance == previous.combat_instance) {
                            if (previous.checkpoint_kind ==
                                    k_tactical_combat_end ||
                                record.checkpoint_index !=
                                    previous.checkpoint_index + 1) {
                                return false;
                            }
                        } else if (record.combat_instance !=
                                previous.combat_instance + 1 ||
                            record.checkpoint_index != 0 ||
                            previous.checkpoint_kind !=
                                k_tactical_combat_end) {
                            return false;
                        }
                    }

                    const size_t payload_offset =
                        offset + sizeof(s_tactical_state_record);
                    state.payload.resize(
                        size - sizeof(s_tactical_state_record));
                    if (!state.payload.empty()) {
                        memcpy(state.payload.data(),
                            data.data() + payload_offset,
                            state.payload.size());
                    }
                    if (!Normalize_Historical_Tactical_State_(&state)) {
                        return false;
                    }
                    tactical_states->push_back(std::move(state));
                } else if (type == k_strategic_state_record_type &&
                    size >= sizeof(s_strategic_state_record)) {
                    s_strategic_state state;
                    memcpy(&state.record, data.data() + offset,
                        sizeof(state.record));

                    const s_strategic_state_record& record = state.record;
                    const bool valid_colony_size = record.colony_size == 0 ||
                        record.colony_size == k_colony_size;
                    uint64_t expected_size = sizeof(s_strategic_state_record);
                    expected_size += record.quick_combat_data_size;
                    expected_size += record.colony_size;

                    if (record.oracle_turn != turn ||
                        record.stardate != header->stardate ||
                        record.combat_instance == 0 ||
                        record.combat_turn < 0 || record.combat_turn > 50 ||
                        record.combat_total_ship_count < 0 ||
                        record.combat_total_ship_count >
                            static_cast<int16_t>(
                                k_original_quick_combat_capacity) ||
                        record.reserved != 0 || !valid_colony_size ||
                        record.quick_combat_data_size !=
                            static_cast<uint32_t>(
                                record.combat_total_ship_count) *
                                k_original_quick_combat_data_size ||
                        expected_size != size ||
                        record.random_call_count != random_records->size() ||
                        record.random_seed_count !=
                            random_seed_records->size()) {
                        return false;
                    }

                    if (strategic_states->empty()) {
                        if (record.combat_instance != 1 ||
                            record.checkpoint_index != 0 ||
                            record.combat_turn != 0) {
                            return false;
                        }
                    } else {
                        const s_strategic_state_record& previous =
                            strategic_states->back().record;
                        if (record.combat_instance ==
                            previous.combat_instance) {
                            if (record.checkpoint_index !=
                                    previous.checkpoint_index + 1 ||
                                record.combat_turn !=
                                    previous.combat_turn + 1) {
                                return false;
                            }
                        } else if (record.combat_instance !=
                                previous.combat_instance + 1 ||
                            record.checkpoint_index != 0 ||
                            record.combat_turn != 0) {
                            return false;
                        }
                    }

                    const size_t payload_offset =
                        offset + sizeof(s_strategic_state_record);
                    state.payload.resize(
                        size - sizeof(s_strategic_state_record));
                    if (!state.payload.empty()) {
                        memcpy(state.payload.data(),
                            data.data() + payload_offset,
                            state.payload.size());
                    }
                    strategic_states->push_back(std::move(state));
                } else if (type == k_presentation_complete_record_type &&
                    size == sizeof(s_presentation_complete_record)) {
                    s_presentation_complete presentation = {};
                    memcpy(&presentation.record, data.data() + offset,
                        sizeof(presentation.record));

                    const s_presentation_complete_record& record =
                        presentation.record;
                    if (record.oracle_turn != turn ||
                        record.stardate != header->stardate ||
                        record.sequence != presentation_completions->size() ||
                        (record.presentation_kind !=
                                k_ground_combat_presentation &&
                            record.presentation_kind !=
                                k_bombing_presentation) ||
                        record.input_record_count != records->size() ||
                        record.random_call_count != random_records->size() ||
                        record.random_seed_count !=
                            random_seed_records->size()) {
                        return false;
                    }

                    const size_t range_begin =
                        presentation_completions->empty()
                        ? 0
                        : presentation_completions->back()
                              .record.input_record_count;
                    if (!Find_Presentation_Terminal_Input_(
                            *records,
                            range_begin,
                            record.input_record_count,
                            static_cast<uint16_t>(
                                header->input_state.mouse_buttons),
                            &presentation.terminal_input_index) ||
                        presentation.terminal_input_index >=
                            record.input_record_count) {
                        return false;
                    }

                    presentation_completions->push_back(presentation);
                } else if (header->version == k_log_version && type == k_end_record_type &&
                    size == sizeof(s_end_record)) {
                    s_end_record end_record;
                    memcpy(&end_record, data.data() + offset, sizeof(end_record));
                    if (end_record.oracle_turn != turn ||
                        end_record.random_call_count != random_records->size() ||
                        end_record.random_seed_count != random_seed_records->size() ||
                        end_record.tactical_state_count != tactical_states->size() ||
                        end_record.strategic_state_count !=
                            strategic_states->size() ||
                        end_record.presentation_complete_count !=
                            presentation_completions->size() ||
                        (!tactical_states->empty() &&
                            tactical_states->back().record.checkpoint_kind !=
                                k_tactical_combat_end)) {
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

        e_prepare_result Prepare_Turn_(uint32_t turn, bool defer_clock_until_first_poll) {
            bool log_exists;
            bool completed;
            s_log_header header;
            std::vector<s_input_record> records;
            std::vector<s_input_position> record_positions;
            std::vector<s_random_record> random_records;
            std::vector<s_random_seed_record> random_seed_records;
            std::vector<s_tactical_state> tactical_states;
            std::vector<s_strategic_state> strategic_states;
            std::vector<s_presentation_complete> presentation_completions;
            if (!Read_Completed_Log_(turn, &header, &records,
                    &record_positions, &random_records, &random_seed_records,
                    &tactical_states, &strategic_states,
                    &presentation_completions, &log_exists, &completed)) {
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
            g_record_positions.swap(record_positions);
            g_random_records.swap(random_records);
            g_random_seed_records.swap(random_seed_records);
            g_tactical_states.swap(tactical_states);
            g_strategic_states.swap(strategic_states);
            g_presentation_completions.swap(presentation_completions);
            g_record_index = 0;
            g_random_record_index = 0;
            g_random_seed_record_index = 0;
            g_tactical_state_index = 0;
            g_strategic_state_index = 0;
            g_presentation_completion_index = 0;
            g_active_presentation_kind = 0;
            g_active_presentation_ready = false;
            g_presentation_rng_desynced = false;
            g_active_combat_instance = 0;
            g_active_strategic_combat_instance = 0;
            g_last_verified_combat_turn =
                std::numeric_limits<int16_t>::min();
            g_tactical_combat_active = false;
            g_replay_active = true;
            Restore_Input_State_();
            g_last_consumption_counter = defer_clock_until_first_poll ? 0 : SDL_GetPerformanceCounter();
            g_click_pause_until_counter = 0;
            g_click_pause_active = false;
            g_replay_clock_armed = !defer_clock_until_first_poll;
            g_replay_mouse_buttons = (uint16_t)header.input_state.mouse_buttons;

            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: replaying turn %u, stardate %d, %zu input events, %zu random calls, %zu random seed calls, %zu tactical states, %zu strategic states, and %zu presentation completions at %u%% speed with %u ms added per event and %u ms after clicks",
                turn, header.stardate, g_records.size(), g_random_records.size(),
                g_random_seed_records.size(), g_tactical_states.size(),
                g_strategic_states.size(),
                g_presentation_completions.size(),
                g_speed_percent, g_delay_ms, g_click_delay_ms);
            return PREPARE_READY;
        }

        bool Read_Save_Count_(const std::vector<uint8_t>& save, size_t offset,
            int16_t maximum, int16_t* count) {
            if (offset + sizeof(*count) > save.size()) {
                return false;
            }
            memcpy(count, save.data() + offset, sizeof(*count));
            return *count >= 0 && *count <= maximum;
        }

        const char* Describe_Save_Offset_(const std::vector<uint8_t>& save,
            size_t difference_offset, size_t* block_offset) {
            std::vector<s_save_block> blocks = {
                {"save header", sizeof(uint32_t)},
                {"save description", sizeof(MOX::_save_game_description[0].description)},
                {"stardate", sizeof(MOX::_stardate)},
                {"game type", sizeof(MOX::_game_type)},
                {"save version", sizeof(uint16_t)},
                {"settings", sizeof(MOX::_settings)},
                {"random seed", sizeof(uint32_t)},
                {"colony count", sizeof(MOX::_NUM_COLONIES)},
            };

            size_t count_offset = 0;
            for (const s_save_block& block : blocks) {
                count_offset += block.size;
            }
            count_offset -= sizeof(MOX::_NUM_COLONIES);

            int16_t colony_count;
            if (!Read_Save_Count_(save, count_offset, MAX_COLONIES,
                    &colony_count)) {
                *block_offset = difference_offset;
                return "invalid save layout";
            }
            blocks.push_back({"colonies",
                sizeof(s_colony) * static_cast<size_t>(colony_count)});
            count_offset += sizeof(MOX::_NUM_COLONIES) +
                sizeof(s_colony) * static_cast<size_t>(colony_count);
            blocks.push_back({"planet count", sizeof(MOX::_planet_count)});

            int16_t planet_count;
            if (!Read_Save_Count_(save, count_offset, MAX_PLANETS,
                    &planet_count)) {
                *block_offset = difference_offset;
                return "invalid save layout";
            }
            blocks.push_back({"planets",
                sizeof(s_planet_data) * static_cast<size_t>(planet_count)});
            count_offset += sizeof(MOX::_planet_count) +
                sizeof(s_planet_data) * static_cast<size_t>(planet_count);
            blocks.push_back({"star count", sizeof(MOX::_NUM_STARS)});

            int16_t star_count;
            if (!Read_Save_Count_(save, count_offset, MAX_STARS,
                    &star_count)) {
                *block_offset = difference_offset;
                return "invalid save layout";
            }
            blocks.push_back({"stars",
                sizeof(s_star_data) * static_cast<size_t>(star_count)});
            blocks.push_back({"leaders", sizeof(s_leader_data) * MAX_LEADERS});
            blocks.push_back({"player count", sizeof(MOX::_NUM_PLAYERS)});
            blocks.push_back({"players", sizeof(s_player) * MAX_PLAYERS});

            count_offset += sizeof(MOX::_NUM_STARS) +
                sizeof(s_star_data) * static_cast<size_t>(star_count) +
                sizeof(s_leader_data) * MAX_LEADERS +
                sizeof(MOX::_NUM_PLAYERS) + sizeof(s_player) * MAX_PLAYERS;
            blocks.push_back({"ship count", sizeof(MOX::_NUM_SHIPS)});

            int16_t ship_count;
            if (!Read_Save_Count_(save, count_offset, MAX_SHIPS,
                    &ship_count)) {
                *block_offset = difference_offset;
                return "invalid save layout";
            }
            blocks.push_back({"ships",
                sizeof(s_ship_data) * static_cast<size_t>(ship_count)});
            const s_save_block trailing_blocks[] = {
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
            blocks.insert(blocks.end(), std::begin(trailing_blocks),
                std::end(trailing_blocks));

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

        bool Canonicalize_Save_Comparison_(std::vector<uint8_t>* save, const char* path) {
            constexpr size_t version_offset =
                sizeof(uint32_t) + sizeof(MOX::_save_game_description[0].description) +
                sizeof(MOX::_stardate) + sizeof(MOX::_game_type);
            constexpr size_t settler_count =
                sizeof(s_player::settlers) / sizeof(s_player::settlers[0]);
            constexpr size_t legacy_settlers_size =
                settler_count * sizeof(SETTLER::legacy_value_type);
            constexpr size_t legacy_player_size =
                sizeof(s_player) - sizeof(s_player::settlers) + legacy_settlers_size -
                sizeof(s_player::offensive_stagepoint) + sizeof(int8_t);
            constexpr size_t legacy_planet_size = sizeof(s_planet_data) - sizeof(int8_t);
            constexpr size_t expanded_star_bitmap_size =
                BITMAP(MAX_STARS) - BITMAP(ORIGINAL_MAX_STARS);
            constexpr size_t legacy_star_size =
                sizeof(s_star_data) - expanded_star_bitmap_size - 2 * sizeof(int8_t);
            constexpr size_t legacy_report_size = sizeof(s_report) - sizeof(int8_t);
            constexpr size_t legacy_message_size = sizeof(s_MSG) - sizeof(int8_t);
            if (save->size() < sizeof(uint32_t)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: save '%s' is too small", path);
                return false;
            }

            uint32_t header;
            memcpy(&header, save->data(), sizeof(header));
            if (header == SAVEGAME::LEGACY_HEADER) {
                if (save->size() < version_offset) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: legacy save '%s' is too small", path);
                    return false;
                }
                const uint16_t version = 0;
                save->insert(save->begin() + version_offset,
                    reinterpret_cast<const uint8_t*>(&version),
                    reinterpret_cast<const uint8_t*>(&version) + sizeof(version));
                header = SAVEGAME::VERSIONED_HEADER;
                memcpy(save->data(), &header, sizeof(header));
            }
            if (header != SAVEGAME::VERSIONED_HEADER ||
                save->size() < version_offset + sizeof(uint16_t)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: save '%s' has an unsupported header", path);
                return false;
            }

            uint16_t version;
            memcpy(&version, save->data() + version_offset, sizeof(version));
            if (version == SAVEGAME::CURRENT_VERSION) {
                return true;
            }
            if (version != 0) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: save '%s' uses unsupported comparison version %u",
                    path, static_cast<unsigned>(version));
                return false;
            }

            std::vector<uint8_t> canonical;
            canonical.reserve(save->size() +
                MAX_PLAYERS * (sizeof(s_player) - legacy_player_size));
            const size_t state_offset = version_offset + sizeof(uint16_t) +
                sizeof(MOX::_settings) + sizeof(uint32_t);
            if (state_offset > save->size()) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: version 0 save '%s' is truncated before game state", path);
                return false;
            }
            canonical.insert(canonical.end(), save->begin(),
                save->begin() + state_offset);

            size_t source_offset = state_offset;
            auto append_counted_records = [&](int16_t maximum,
                size_t element_size, const char* block_name) {
                int16_t count;
                if (!Read_Save_Count_(*save, source_offset, maximum, &count)) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: version 0 save '%s' has an invalid %s count", path,
                        block_name);
                    return false;
                }
                const size_t records_offset = source_offset + sizeof(count);
                const size_t legacy_size = element_size *
                    static_cast<size_t>(maximum);
                if (records_offset + legacy_size > save->size()) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: version 0 save '%s' has a truncated %s block", path,
                        block_name);
                    return false;
                }
                canonical.insert(canonical.end(), save->begin() + source_offset,
                    save->begin() + records_offset +
                        element_size * static_cast<size_t>(count));
                source_offset = records_offset + legacy_size;
                return true;
            };

            if (!append_counted_records(ORIGINAL_MAX_COLONIES, sizeof(s_colony),
                    "colony")) {
                return false;
            }

            int16_t planet_count;
            if (!Read_Save_Count_(*save, source_offset, ORIGINAL_MAX_PLANETS,
                    &planet_count)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: version 0 save '%s' has an invalid planet count", path);
                return false;
            }
            canonical.insert(canonical.end(), save->begin() + source_offset,
                save->begin() + source_offset + sizeof(planet_count));
            source_offset += sizeof(planet_count);
            if (source_offset + legacy_planet_size * ORIGINAL_MAX_PLANETS > save->size()) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: version 0 save '%s' has a truncated planet block", path);
                return false;
            }
            for (int16_t planet_idx = 0; planet_idx < planet_count; ++planet_idx) {
                const size_t record_offset = source_offset +
                    static_cast<size_t>(planet_idx) * legacy_planet_size;
                const size_t star_offset = offsetof(s_planet_data, star_index);
                canonical.insert(canonical.end(), save->begin() + record_offset,
                    save->begin() + record_offset + star_offset);
                const int16_t star_index = static_cast<int8_t>((*save)[record_offset + star_offset]);
                const uint8_t* star_bytes = reinterpret_cast<const uint8_t*>(&star_index);
                canonical.insert(canonical.end(), star_bytes,
                    star_bytes + sizeof(star_index));
                canonical.insert(canonical.end(),
                    save->begin() + record_offset + star_offset + sizeof(int8_t),
                    save->begin() + record_offset + legacy_planet_size);
            }
            source_offset += legacy_planet_size * ORIGINAL_MAX_PLANETS;

            int16_t star_count;
            if (!Read_Save_Count_(*save, source_offset, ORIGINAL_MAX_STARS, &star_count)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: version 0 save '%s' has an invalid star count", path);
                return false;
            }
            canonical.insert(canonical.end(), save->begin() + source_offset,
                save->begin() + source_offset + sizeof(star_count));
            source_offset += sizeof(star_count);
            if (source_offset + legacy_star_size * ORIGINAL_MAX_STARS > save->size()) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: version 0 save '%s' has a truncated star block", path);
                return false;
            }
            const size_t black_hole_blocks_offset = offsetof(s_star_data, black_hole_blocks);
            const size_t legacy_black_hole_blocks_size = BITMAP(ORIGINAL_MAX_STARS);
            const size_t legacy_wormhole_offset =
                offsetof(s_star_data, wormhole_star_id) - expanded_star_bitmap_size;
            const size_t legacy_next_wfi_offset =
                offsetof(s_star_data, next_wfi_in_list) - expanded_star_bitmap_size - sizeof(int8_t);
            for (int16_t star_idx = 0; star_idx < star_count; ++star_idx) {
                const size_t record_offset = source_offset +
                    static_cast<size_t>(star_idx) * legacy_star_size;
                canonical.insert(canonical.end(), save->begin() + record_offset,
                    save->begin() + record_offset + black_hole_blocks_offset +
                        legacy_black_hole_blocks_size);
                canonical.insert(canonical.end(), expanded_star_bitmap_size, 0);
                canonical.insert(canonical.end(),
                    save->begin() + record_offset + black_hole_blocks_offset +
                        legacy_black_hole_blocks_size,
                    save->begin() + record_offset + legacy_wormhole_offset);
                const int16_t wormhole_star =
                    static_cast<int8_t>((*save)[record_offset + legacy_wormhole_offset]);
                const uint8_t* wormhole_bytes =
                    reinterpret_cast<const uint8_t*>(&wormhole_star);
                canonical.insert(canonical.end(), wormhole_bytes,
                    wormhole_bytes + sizeof(wormhole_star));
                canonical.insert(canonical.end(),
                    save->begin() + record_offset + legacy_wormhole_offset + sizeof(int8_t),
                    save->begin() + record_offset + legacy_next_wfi_offset);
                const int16_t next_wfi = (*save)[record_offset + legacy_next_wfi_offset];
                const uint8_t* next_wfi_bytes =
                    reinterpret_cast<const uint8_t*>(&next_wfi);
                canonical.insert(canonical.end(), next_wfi_bytes,
                    next_wfi_bytes + sizeof(next_wfi));
                canonical.insert(canonical.end(),
                    save->begin() + record_offset + legacy_next_wfi_offset + sizeof(uint8_t),
                    save->begin() + record_offset + legacy_star_size);
            }
            source_offset += legacy_star_size * ORIGINAL_MAX_STARS;

            const size_t leaders_and_player_count_size =
                sizeof(s_leader_data) * MAX_LEADERS + sizeof(MOX::_NUM_PLAYERS);
            if (source_offset + leaders_and_player_count_size > save->size()) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: version 0 save '%s' has a truncated leader block", path);
                return false;
            }
            canonical.insert(canonical.end(), save->begin() + source_offset,
                save->begin() + source_offset + leaders_and_player_count_size);
            source_offset += leaders_and_player_count_size;

            for (size_t player_idx = 0; player_idx < MAX_PLAYERS; ++player_idx) {
                const size_t legacy_player_offset = source_offset;
                const size_t legacy_settlers_offset = legacy_player_offset +
                    offsetof(s_player, settlers);
                const size_t legacy_player_end =
                    legacy_player_offset + legacy_player_size;
                if (legacy_player_end > save->size()) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: version 0 save '%s' has a truncated player block", path);
                    return false;
                }
                canonical.insert(canonical.end(),
                    save->begin() + legacy_player_offset,
                    save->begin() + legacy_settlers_offset);

                for (size_t settler_idx = 0; settler_idx < settler_count;
                    ++settler_idx) {
                    SETTLER::legacy_value_type legacy_settler;
                    memcpy(&legacy_settler,
                        save->data() + legacy_settlers_offset +
                            settler_idx * sizeof(legacy_settler),
                        sizeof(legacy_settler));
                    const SETTLER::value_type settler =
                        SETTLER::From_Legacy(legacy_settler);
                    const uint8_t* settler_bytes =
                        reinterpret_cast<const uint8_t*>(&settler);
                    canonical.insert(canonical.end(), settler_bytes,
                        settler_bytes + sizeof(settler));
                }

                canonical.insert(canonical.end(),
                    save->begin() + legacy_settlers_offset +
                        legacy_settlers_size,
                    save->begin() + legacy_player_offset +
                        offsetof(s_player, offensive_stagepoint) -
                        (sizeof(s_player::settlers) - legacy_settlers_size));
                const size_t legacy_offensive_stagepoint_offset =
                    legacy_player_offset + offsetof(s_player, offensive_stagepoint) -
                    (sizeof(s_player::settlers) - legacy_settlers_size);
                const int16_t offensive_stagepoint = static_cast<int8_t>(
                    (*save)[legacy_offensive_stagepoint_offset]);
                const uint8_t* offensive_stagepoint_bytes =
                    reinterpret_cast<const uint8_t*>(&offensive_stagepoint);
                canonical.insert(canonical.end(), offensive_stagepoint_bytes,
                    offensive_stagepoint_bytes + sizeof(offensive_stagepoint));
                canonical.insert(canonical.end(),
                    save->begin() + legacy_offensive_stagepoint_offset + sizeof(int8_t),
                    save->begin() + legacy_player_end);
                source_offset = legacy_player_end;
            }

            int16_t ship_count;
            if (!Read_Save_Count_(*save, source_offset, ORIGINAL_MAX_SHIPS,
                    &ship_count)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: version 0 save '%s' has an invalid ship count", path);
                return false;
            }
            canonical.insert(canonical.end(), save->begin() + source_offset,
                save->begin() + source_offset + sizeof(ship_count));
            source_offset += sizeof(ship_count);
            const size_t legacy_ships_size =
                sizeof(s_ship_data) * ORIGINAL_MAX_SHIPS;
            if (source_offset + legacy_ships_size > save->size()) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: version 0 save '%s' has a truncated ship block", path);
                return false;
            }
            const size_t ships_offset = canonical.size();
            canonical.insert(canonical.end(), save->begin() + source_offset,
                save->begin() + source_offset +
                    sizeof(s_ship_data) * static_cast<size_t>(ship_count));
            source_offset += legacy_ships_size;

            for (size_t ship_idx = 0;
                ship_idx < static_cast<size_t>(ship_count); ++ship_idx) {
                const size_t location_offset = ships_offset +
                    ship_idx * sizeof(s_ship_data) + offsetof(s_ship_data, location);
                int16_t location;
                memcpy(&location, canonical.data() + location_offset, sizeof(location));
                location = SAVEGAME::Convert_Legacy_Ship_Location_(location);
                memcpy(canonical.data() + location_offset, &location, sizeof(location));
            }

            const size_t trailing_before_reports_size =
                sizeof(MOX::_moveable_box[0]) * 5 +
                sizeof(MOX::_moveable_box_order[0]) * 5 +
                sizeof(MOX::_PLAYER_NUM) + sizeof(MOX::_max_map_scale) +
                sizeof(MOX::_max_zoom_count) + sizeof(MOX::_star_bg_index) +
                sizeof(MOX::_MAP_MAX_X) + sizeof(MOX::_MAP_MAX_Y) +
                sizeof(MOX::_fleet_icon_button_count) +
                sizeof(*MOX::_nebula) * MAX_NEBULAS +
                sizeof(MOX::_NUM_NEBULAS) + sizeof(MOX::_bill_savegame) +
                sizeof(REPORT::_n_reports);
            if (source_offset + trailing_before_reports_size +
                    legacy_report_size * MAX_REPORTS > save->size()) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: version 0 save '%s' has a truncated report block", path);
                return false;
            }
            canonical.insert(canonical.end(), save->begin() + source_offset,
                save->begin() + source_offset + trailing_before_reports_size);
            source_offset += trailing_before_reports_size;
            for (size_t report_idx = 0; report_idx < MAX_REPORTS; ++report_idx) {
                const size_t report_offset = source_offset +
                    report_idx * legacy_report_size;
                const uint8_t type = (*save)[report_offset];
                const uint8_t action = (*save)[report_offset + offsetof(s_report, action)];
                canonical.insert(canonical.end(), save->begin() + report_offset,
                    save->begin() + report_offset + offsetof(s_report, d));
                const size_t data_offset = report_offset + offsetof(s_report, d);
                if (type == 1 && action == 1) {
                    canonical.insert(canonical.end(), save->begin() + data_offset,
                        save->begin() + data_offset + 3);
                    canonical.push_back(0);
                    canonical.push_back((*save)[data_offset + 3]);
                } else if (type == 2 && action == 1) {
                    canonical.insert(canonical.end(), save->begin() + data_offset,
                        save->begin() + data_offset + 3);
                    canonical.push_back(0);
                    canonical.push_back((*save)[data_offset + 3]);
                } else {
                    canonical.insert(canonical.end(), save->begin() + data_offset,
                        save->begin() + data_offset + 4);
                    canonical.push_back(0);
                }
                canonical.insert(canonical.end(), save->begin() + data_offset + 4,
                    save->begin() + report_offset + legacy_report_size);
            }
            source_offset += legacy_report_size * MAX_REPORTS;

            const size_t trailing_before_colonize_check_size =
                sizeof(REPORT::_done_player_reports) +
                sizeof(REPORT::_something_interesting_happened);
            const size_t legacy_colonize_check_size = BITMAP(ORIGINAL_MAX_STARS);
            const size_t trailing_after_colonize_check_size =
                sizeof(EVENTS::_event_data[0]) * 36;
            const size_t legacy_trailing_before_messages_size =
                trailing_before_colonize_check_size + legacy_colonize_check_size +
                trailing_after_colonize_check_size;
            if (source_offset + legacy_trailing_before_messages_size +
                    legacy_message_size * MAX_MESSAGES > save->size()) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: version 0 save '%s' has a truncated message block", path);
                return false;
            }
            canonical.insert(canonical.end(), save->begin() + source_offset,
                save->begin() + source_offset + trailing_before_colonize_check_size +
                    legacy_colonize_check_size);
            canonical.insert(canonical.end(),
                sizeof(REPORT::_done_colonize_check) - legacy_colonize_check_size, 0);
            canonical.insert(canonical.end(),
                save->begin() + source_offset + trailing_before_colonize_check_size +
                    legacy_colonize_check_size,
                save->begin() + source_offset + legacy_trailing_before_messages_size);
            source_offset += legacy_trailing_before_messages_size;
            const size_t messages_offset = canonical.size();
            for (size_t message_idx = 0; message_idx < MAX_MESSAGES; ++message_idx) {
                const size_t source_message_offset = source_offset +
                    message_idx * legacy_message_size;
                const int8_t type = static_cast<int8_t>((*save)[source_message_offset]);
                canonical.insert(canonical.end(), save->begin() + source_message_offset,
                    save->begin() + source_message_offset + offsetof(s_MSG, u));
                const size_t payload_offset = source_message_offset + offsetof(s_MSG, u);
                const bool signed_star =
                    type == MSG_TYPE_HYPERSPACE_BEAST ||
                    type == MSG_TYPE_ARTEMIS_LEADER ||
                    type == MSG_TYPE_FAILED_RETREAT;
                const bool unsigned_star =
                    type == MSG_TYPE_ANOMALY || type == MSG_TYPE_EEL_SPAWNED ||
                    type == MSG_TYPE_EEL_VANISHED || type == MSG_TYPE_ARTIFACTS;
                if (type == MSG_TYPE_COLONY || signed_star || unsigned_star) {
                    size_t index_offset = 0;
                    if (type == MSG_TYPE_HYPERSPACE_BEAST) index_offset = sizeof(int16_t);
                    canonical.insert(canonical.end(), save->begin() + payload_offset,
                        save->begin() + payload_offset + index_offset);
                    const int16_t index = signed_star
                        ? static_cast<int8_t>((*save)[payload_offset + index_offset])
                        : (*save)[payload_offset + index_offset];
                    const uint8_t* index_bytes = reinterpret_cast<const uint8_t*>(&index);
                    canonical.insert(canonical.end(), index_bytes,
                        index_bytes + sizeof(index));
                    canonical.insert(canonical.end(),
                        save->begin() + payload_offset + index_offset + sizeof(int8_t),
                        save->begin() + payload_offset + 8);
                } else {
                    canonical.insert(canonical.end(), save->begin() + payload_offset,
                        save->begin() + payload_offset + 8);
                    canonical.push_back(0);
                }
                canonical.insert(canonical.end(),
                    save->begin() + payload_offset + 8,
                    save->begin() + source_message_offset + legacy_message_size);
            }
            source_offset += legacy_message_size * MAX_MESSAGES;
            canonical.insert(canonical.end(), save->begin() + source_offset,
                save->end());

            for (size_t message_idx = 0; message_idx < MAX_MESSAGES; ++message_idx) {
                const size_t message_offset = messages_offset +
                    message_idx * sizeof(s_MSG);
                if (static_cast<int8_t>(canonical[message_offset]) ==
                        MSG_TYPE_SCRAP_SHIP) {
                    const size_t location_offset = message_offset +
                        offsetof(s_MSG, u) + offsetof(s_MSG_scrap_ship, where);
                    int16_t location;
                    memcpy(&location, canonical.data() + location_offset,
                        sizeof(location));
                    location = SAVEGAME::Convert_Legacy_Ship_Location_(location);
                    memcpy(canonical.data() + location_offset, &location,
                        sizeof(location));
                }
            }

            version = SAVEGAME::CURRENT_VERSION;
            memcpy(canonical.data() + version_offset, &version, sizeof(version));
            save->swap(canonical);
            return true;
        }

        size_t Find_First_Save_Difference_(
            const std::vector<uint8_t>& actual,
            const std::vector<uint8_t>& expected) {
            const size_t common_size = std::min(actual.size(), expected.size());
            size_t difference = 0;
            while (difference < common_size && actual[difference] == expected[difference]) {
                ++difference;
            }
            return difference;
        }

        bool Cleanup_Save_Comparison_(
            std::vector<uint8_t>* actual,
            std::vector<uint8_t>* expected,
            size_t difference,
            const char** rule_name) {
            size_t block_offset = 0;
            const char* block_name = Describe_Save_Offset_(*actual, difference,
                &block_offset);
            if (strcmp(block_name, "save description") == 0) {
                constexpr size_t auto_save_length = sizeof(FILEDEF::s_Auto_Save_00556938) - 1;
                const size_t description_offset = difference - block_offset;
                if (description_offset + auto_save_length + 2 > actual->size() ||
                    description_offset + auto_save_length + 1 > expected->size() ||
                    memcmp(actual->data() + description_offset,
                        FILEDEF::s_Auto_Save_00556938, auto_save_length) != 0 ||
                    memcmp(expected->data() + description_offset,
                        FILEDEF::s_Auto_Save_00556938, auto_save_length) != 0 ||
                    (*actual)[description_offset + auto_save_length] != '_' ||
                    (*actual)[description_offset + auto_save_length + 1] != 0 ||
                    (*expected)[description_offset + auto_save_length] != 0) {
                    return false;
                }

                (*actual)[description_offset + auto_save_length] = 0;
                *rule_name = "normalize autosave description cursor marker";
                return true;
            }

            if (strcmp(block_name, "moveable boxes") == 0) {
                const size_t member_offset = block_offset % sizeof(s_movable_box);
                const bool is_position =
                    member_offset >= offsetof(s_movable_box, x) &&
                    member_offset < offsetof(s_movable_box, y) + sizeof(s_movable_box::y);
                if (!is_position) {
                    return false;
                }

                const size_t boxes_offset = difference - block_offset;
                bool changed = false;
                for (size_t box_idx = 0; box_idx < 5; ++box_idx) {
                    const size_t position_offset = boxes_offset +
                        box_idx * sizeof(s_movable_box) + offsetof(s_movable_box, x);
                    const size_t position_size =
                        sizeof(s_movable_box::x) + sizeof(s_movable_box::y);
                    if (position_offset + position_size > actual->size() ||
                        position_offset + position_size > expected->size()) {
                        return false;
                    }

                    for (size_t i = 0; i < position_size; ++i) {
                        if ((*actual)[position_offset + i] != 0 ||
                            (*expected)[position_offset + i] != 0) {
                            (*actual)[position_offset + i] = 0;
                            (*expected)[position_offset + i] = 0;
                            changed = true;
                        }
                    }
                }

                if (changed) {
                    *rule_name = "normalize presentation-only moveable-box positions";
                }
                return changed;
            }

            if (strcmp(block_name, "players") == 0) {
                const size_t players_offset = difference - block_offset;
                const size_t player_offset = block_offset % sizeof(s_player);
                const size_t settlers_offset = offsetof(s_player, settlers);
                if (player_offset >= settlers_offset &&
                    player_offset < settlers_offset + sizeof(s_player::settlers)) {
                    constexpr SETTLER::value_type meaningful_settler_bits =
                        SETTLER::MASK_SOURCE_COLONY |
                        SETTLER::MASK_DEST_PLANET |
                        SETTLER::MASK_RACE |
                        SETTLER::MASK_ETA |
                        SETTLER::MASK_PROF;
                    const size_t settler_capacity =
                        sizeof(s_player::settlers) / sizeof(s_player::settlers[0]);
                    bool changed = false;

                    for (size_t player_idx = 0; player_idx < MAX_PLAYERS; ++player_idx) {
                        const size_t current_player_offset =
                            players_offset + player_idx * sizeof(s_player);
                        const size_t count_offset = current_player_offset +
                            offsetof(s_player, settlers_freighted);
                        const size_t current_settlers_offset =
                            current_player_offset + settlers_offset;
                        if (current_settlers_offset + sizeof(s_player::settlers) > actual->size() ||
                            current_settlers_offset + sizeof(s_player::settlers) > expected->size()) {
                            return false;
                        }

                        int16_t actual_count;
                        int16_t expected_count;
                        memcpy(&actual_count, actual->data() + count_offset, sizeof(actual_count));
                        memcpy(&expected_count, expected->data() + count_offset, sizeof(expected_count));
                        if (actual_count != expected_count || actual_count < 0 ||
                            actual_count > static_cast<int16_t>(settler_capacity)) {
                            return false;
                        }

                        for (size_t settler_idx = 0; settler_idx < settler_capacity; ++settler_idx) {
                            const size_t offset = current_settlers_offset +
                                settler_idx * sizeof(s_settler_info);
                            SETTLER::value_type actual_value;
                            SETTLER::value_type expected_value;
                            memcpy(&actual_value, actual->data() + offset, sizeof(actual_value));
                            memcpy(&expected_value, expected->data() + offset, sizeof(expected_value));

                            if (settler_idx < static_cast<size_t>(actual_count)) {
                                actual_value &= meaningful_settler_bits;
                                expected_value &= meaningful_settler_bits;
                            } else {
                                actual_value = 0;
                                expected_value = 0;
                            }

                            SETTLER::value_type original_actual;
                            SETTLER::value_type original_expected;
                            memcpy(&original_actual, actual->data() + offset, sizeof(original_actual));
                            memcpy(&original_expected, expected->data() + offset, sizeof(original_expected));
                            if (actual_value != original_actual) {
                                memcpy(actual->data() + offset, &actual_value, sizeof(actual_value));
                                changed = true;
                            }
                            if (expected_value != original_expected) {
                                memcpy(expected->data() + offset, &expected_value, sizeof(expected_value));
                                changed = true;
                            }
                        }
                    }

                    if (changed) {
                        *rule_name = "normalize undefined settler bits and inactive settler slots";
                    }
                    return changed;
                }
            }

            if (strcmp(block_name, "messages") == 0) {
                const size_t messages_offset = difference - block_offset;
                const size_t differing_message_offset = messages_offset +
                    (block_offset / sizeof(s_MSG)) * sizeof(s_MSG);
                if (differing_message_offset + sizeof(s_MSG) > actual->size() ||
                    differing_message_offset + sizeof(s_MSG) > expected->size()) {
                    return false;
                }

                s_MSG differing_actual;
                s_MSG differing_expected;
                memcpy(&differing_actual,
                    actual->data() + differing_message_offset,
                    sizeof(differing_actual));
                memcpy(&differing_expected,
                    expected->data() + differing_message_offset,
                    sizeof(differing_expected));
                const bool difference_is_eel =
                    differing_actual.msg_type == differing_expected.msg_type &&
                    (differing_actual.msg_type == MSG_TYPE_EEL_SPAWNED ||
                        differing_actual.msg_type == MSG_TYPE_EEL_VANISHED);
                if (difference_is_eel) {
                    bool changed = false;
                    for (size_t message_idx = 0; message_idx < 100; ++message_idx) {
                        const size_t message_offset =
                            messages_offset + message_idx * sizeof(s_MSG);
                        if (message_offset + sizeof(s_MSG) > actual->size() ||
                            message_offset + sizeof(s_MSG) > expected->size()) {
                            return false;
                        }

                        s_MSG actual_message;
                        s_MSG expected_message;
                        memcpy(&actual_message, actual->data() + message_offset,
                            sizeof(actual_message));
                        memcpy(&expected_message, expected->data() + message_offset,
                            sizeof(expected_message));
                        if (actual_message.msg_type != expected_message.msg_type ||
                            (actual_message.msg_type != MSG_TYPE_EEL_SPAWNED &&
                                actual_message.msg_type != MSG_TYPE_EEL_VANISHED)) {
                            continue;
                        }

                        const size_t priority_offset = message_offset +
                            offsetof(s_MSG, priority);
                        if ((*actual)[priority_offset] != 0 ||
                            (*expected)[priority_offset] != 0) {
                            (*actual)[priority_offset] = 0;
                            (*expected)[priority_offset] = 0;
                            changed = true;
                        }

                        const size_t unused_offset = message_offset +
                            offsetof(s_MSG, u) + sizeof(s_MSG_eel);
                        const size_t unused_size = sizeof(s_MSG) -
                            offsetof(s_MSG, u) - sizeof(s_MSG_eel);
                        for (size_t i = 0; i < unused_size; ++i) {
                            if ((*actual)[unused_offset + i] != 0 ||
                                (*expected)[unused_offset + i] != 0) {
                                (*actual)[unused_offset + i] = 0;
                                (*expected)[unused_offset + i] = 0;
                                changed = true;
                            }
                        }
                    }

                    if (changed) {
                        *rule_name = "normalize uninitialized eel message bytes";
                    }
                    return changed;
                }

                const bool difference_is_failed_retreat =
                    differing_actual.msg_type == MSG_TYPE_FAILED_RETREAT &&
                    differing_expected.msg_type == MSG_TYPE_FAILED_RETREAT;
                if (difference_is_failed_retreat) {
                    bool changed = false;
                    for (size_t message_idx = 0; message_idx < 100; ++message_idx) {
                        const size_t message_offset =
                            messages_offset + message_idx * sizeof(s_MSG);
                        if (message_offset + sizeof(s_MSG) > actual->size() ||
                            message_offset + sizeof(s_MSG) > expected->size()) {
                            return false;
                        }

                        s_MSG actual_message;
                        s_MSG expected_message;
                        memcpy(&actual_message, actual->data() + message_offset,
                            sizeof(actual_message));
                        memcpy(&expected_message, expected->data() + message_offset,
                            sizeof(expected_message));
                        if (actual_message.msg_type != MSG_TYPE_FAILED_RETREAT ||
                            expected_message.msg_type != MSG_TYPE_FAILED_RETREAT) {
                            continue;
                        }

                        const size_t priority_offset = message_offset +
                            offsetof(s_MSG, priority);
                        if ((*actual)[priority_offset] != 0 ||
                            (*expected)[priority_offset] != 0) {
                            (*actual)[priority_offset] = 0;
                            (*expected)[priority_offset] = 0;
                            changed = true;
                        }

                        const size_t unused_offset = message_offset +
                            offsetof(s_MSG, u) + sizeof(s_MSG_failed_retreat);
                        const size_t unused_size = sizeof(s_MSG) -
                            offsetof(s_MSG, u) - sizeof(s_MSG_failed_retreat);
                        for (size_t i = 0; i < unused_size; ++i) {
                            if ((*actual)[unused_offset + i] != 0 ||
                                (*expected)[unused_offset + i] != 0) {
                                (*actual)[unused_offset + i] = 0;
                                (*expected)[unused_offset + i] = 0;
                                changed = true;
                            }
                        }
                    }

                    if (changed) {
                        *rule_name = "normalize uninitialized failed-retreat message bytes";
                    }
                    return changed;
                }

                const bool difference_is_hyperspace_beast =
                    differing_actual.msg_type == MSG_TYPE_HYPERSPACE_BEAST &&
                    differing_expected.msg_type == MSG_TYPE_HYPERSPACE_BEAST;
                bool changed = false;
                for (size_t message_idx = 0;
                    difference_is_hyperspace_beast && message_idx < 100;
                    ++message_idx) {
                    const size_t message_offset =
                        messages_offset + message_idx * sizeof(s_MSG);
                    if (message_offset + sizeof(s_MSG) > actual->size() ||
                        message_offset + sizeof(s_MSG) > expected->size()) {
                        return false;
                    }

                    s_MSG actual_message;
                    s_MSG expected_message;
                    memcpy(&actual_message, actual->data() + message_offset,
                        sizeof(actual_message));
                    memcpy(&expected_message, expected->data() + message_offset,
                        sizeof(expected_message));
                    if (actual_message.msg_type != MSG_TYPE_HYPERSPACE_BEAST ||
                        expected_message.msg_type != MSG_TYPE_HYPERSPACE_BEAST) {
                        continue;
                    }

                    const size_t priority_offset = message_offset +
                        offsetof(s_MSG, priority);
                    if ((*actual)[priority_offset] != 0 ||
                        (*expected)[priority_offset] != 0) {
                        (*actual)[priority_offset] = 0;
                        (*expected)[priority_offset] = 0;
                        changed = true;
                    }

                    const s_MSG_hyperspace_beast& actual_beast =
                        actual_message.u.hyperspace_beast;
                    const s_MSG_hyperspace_beast& expected_beast =
                        expected_message.u.hyperspace_beast;
                    const bool actual_own_ship =
                        actual_beast.ship_idx != -1 && actual_beast.fate != 0;
                    const bool expected_own_ship =
                        expected_beast.ship_idx != -1 && expected_beast.fate != 0;
                    bool meaningful_fields[sizeof(s_MSG_hyperspace_beast)] = {};
                    meaningful_fields[offsetof(s_MSG_hyperspace_beast, ship_idx)] = true;
                    meaningful_fields[offsetof(s_MSG_hyperspace_beast, ship_idx) + 1] = true;
                    const bool both_have_ship =
                        actual_beast.ship_idx != -1 &&
                        expected_beast.ship_idx != -1;
                    meaningful_fields[offsetof(s_MSG_hyperspace_beast, fate)] =
                        both_have_ship;
                    if (actual_own_ship && expected_own_ship) {
                        meaningful_fields[offsetof(s_MSG_hyperspace_beast, star_idx)] = true;
                        meaningful_fields[offsetof(s_MSG_hyperspace_beast, star_idx) + 1] = true;
                        meaningful_fields[offsetof(s_MSG_hyperspace_beast, officer_idx)] = true;
                        meaningful_fields[offsetof(s_MSG_hyperspace_beast, leader_status)] =
                            actual_beast.officer_idx != -1 &&
                            expected_beast.officer_idx != -1;
                    }
                    const size_t beast_offset = message_offset +
                        offsetof(s_MSG, u);
                    for (size_t i = 0; i < sizeof(meaningful_fields); ++i) {
                        if (!meaningful_fields[i] &&
                            ((*actual)[beast_offset + i] != 0 ||
                                (*expected)[beast_offset + i] != 0)) {
                            (*actual)[beast_offset + i] = 0;
                            (*expected)[beast_offset + i] = 0;
                            changed = true;
                        }
                    }

                    const size_t tail_offset = beast_offset +
                        sizeof(s_MSG_hyperspace_beast);
                    const size_t tail_size = sizeof(s_MSG) -
                        offsetof(s_MSG, u) -
                        sizeof(s_MSG_hyperspace_beast);
                    for (size_t i = 0; i < tail_size; ++i) {
                        if ((*actual)[tail_offset + i] != 0 ||
                            (*expected)[tail_offset + i] != 0) {
                            (*actual)[tail_offset + i] = 0;
                            (*expected)[tail_offset + i] = 0;
                            changed = true;
                        }
                    }
                }

                if (changed) {
                    *rule_name = "normalize branch-unused hyperspace-beast message fields";
                    return true;
                }
            }

            if (strcmp(block_name, "messages") == 0 &&
                block_offset % sizeof(s_MSG) >=
                    offsetof(s_MSG, u) + sizeof(s_MSG_spy_assasination) &&
                block_offset % sizeof(s_MSG) < offsetof(s_MSG, reserved)) {
                const size_t messages_offset = difference - block_offset;
                bool changed = false;
                for (size_t message_idx = 0; message_idx < 100; ++message_idx) {
                    const size_t message_offset =
                        messages_offset + message_idx * sizeof(s_MSG);
                    if (message_offset + sizeof(s_MSG) > expected->size()) {
                        return false;
                    }

                    if (static_cast<int8_t>((*expected)[message_offset +
                            offsetof(s_MSG, msg_type)]) !=
                        MSG_TYPE_SPY_ASSASSINATION) {
                        continue;
                    }

                    const size_t tail_offset = message_offset +
                        offsetof(s_MSG, u) +
                        sizeof(s_MSG_spy_assasination);
                    const size_t tail_size = offsetof(s_MSG, reserved) -
                        offsetof(s_MSG, u) -
                        sizeof(s_MSG_spy_assasination);
                    for (size_t i = 0; i < tail_size; ++i) {
                        if ((*expected)[tail_offset + i] != 0) {
                            (*expected)[tail_offset + i] = 0;
                            changed = true;
                        }
                    }
                }

                if (changed) {
                    *rule_name = "zero original uninitialized spy-assassination payload tail";
                }
                return changed;
            }

            if (strcmp(block_name, "messages") == 0 &&
                block_offset % sizeof(s_MSG) >= offsetof(s_MSG, reserved)) {
                const size_t messages_offset = difference - block_offset;
                bool changed = false;
                for (size_t message_idx = 0; message_idx < 100; ++message_idx) {
                    const size_t reserved_offset =
                        messages_offset + message_idx * sizeof(s_MSG) + offsetof(s_MSG, reserved);
                    if (reserved_offset + sizeof(s_MSG::reserved) > expected->size()) {
                        return false;
                    }

                    for (size_t i = 0; i < sizeof(s_MSG::reserved); ++i) {
                        if ((*expected)[reserved_offset + i] != 0) {
                            (*expected)[reserved_offset + i] = 0;
                            changed = true;
                        }
                    }
                }

                if (changed) {
                    *rule_name = "zero original uninitialized message reserved bytes";
                }
                return changed;
            }

            if (strcmp(block_name, "reports") != 0) {
                return false;
            }

            const size_t reports_offset = difference - block_offset;
            if (reports_offset < sizeof(REPORT::_n_reports)) {
                return false;
            }

            int16_t actual_report_count;
            int16_t expected_report_count;
            memcpy(&actual_report_count,
                actual->data() + reports_offset - sizeof(actual_report_count),
                sizeof(actual_report_count));
            memcpy(&expected_report_count,
                expected->data() + reports_offset - sizeof(expected_report_count),
                sizeof(expected_report_count));
            if (actual_report_count != expected_report_count ||
                expected_report_count < 0 || expected_report_count > 50) {
                return false;
            }

            const size_t difference_report_idx =
                block_offset / sizeof(s_report);
            const size_t difference_report_offset =
                block_offset % sizeof(s_report);
            if (difference_report_idx <
                    static_cast<size_t>(expected_report_count) &&
                difference_report_offset < offsetof(s_report, reserved)) {
                return false;
            }

            bool changed = false;
            for (size_t report_idx = 0; report_idx < 50; ++report_idx) {
                const bool active = report_idx <
                    static_cast<size_t>(expected_report_count);
                const size_t cleanup_offset = reports_offset +
                    report_idx * sizeof(s_report) +
                    (active ? offsetof(s_report, reserved) : 0);
                const size_t cleanup_size = active
                    ? sizeof(s_report::reserved) : sizeof(s_report);
                if (cleanup_offset + cleanup_size > actual->size() ||
                    cleanup_offset + cleanup_size > expected->size()) {
                    return false;
                }

                for (size_t i = 0; i < cleanup_size; ++i) {
                    if ((*actual)[cleanup_offset + i] != 0) {
                        (*actual)[cleanup_offset + i] = 0;
                        changed = true;
                    }
                    if ((*expected)[cleanup_offset + i] != 0) {
                        (*expected)[cleanup_offset + i] = 0;
                        changed = true;
                    }
                }
            }

            if (changed) {
                *rule_name = "normalize active report reserved bytes and inactive report slots";
            }
            return changed;
        }

        const char* Message_Type_Name_(int8_t msg_type) {
            switch (msg_type) {
                case MSG_TYPE_COLONY: return "colony";
                case MSG_TYPE_DECLARE_ALLIANCE: return "declare alliance";
                case MSG_TYPE_DECLARE_WAR: return "declare war";
                case MSG_TYPE_UNKNOWN_0x3: return "unknown 3";
                case MSG_TYPE_FINANCIAL: return "financial";
                case MSG_TYPE_LOST_SHIP: return "lost ship";
                case MSG_TYPE_SCRAP_SHIP: return "scrap ship";
                case MSG_TYPE_LOST_COLONY: return "lost colony";
                case MSG_TYPE_HYPERSPACE_FLUX: return "hyperspace flux";
                case MSG_TYPE_HYPERSPACE_BEAST: return "hyperspace beast";
                case MSG_TYPE_LOST_ATTACKING_SPY: return "lost attacking spy";
                case MSG_TYPE_LOST_DEFENDING_SPY: return "lost defending spy";
                case MSG_TYPE_ARTEMIS: return "Artemis";
                case MSG_TYPE_ARTEMIS_LEADER: return "Artemis leader";
                case MSG_TYPE_FAILED_RETREAT: return "failed retreat";
                case MSG_TYPE_SPY_ASSASSINATION: return "spy assassination";
                case MSG_TYPE_ANOMALY: return "anomaly";
                case MSG_TYPE_DIPLOMACY: return "diplomacy";
                case MSG_TYPE_EEL_SPAWNED: return "eel spawned";
                case MSG_TYPE_EEL_VANISHED: return "eel vanished";
                case MSG_TYPE_ARTIFACTS: return "artifacts";
                case MSG_TYPE_APRIL1: return "April 1";
                case MSG_TYPE_SCRAP_LEADER: return "scrap leader";
                default: return "unknown";
            }
        }

        void Log_Original_Return_Addresses_(const s_random_seed_record& record) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: original image base=0x%08X, %u probable return addresses",
                (uint32_t)k_original_image_base, record.return_address_count);
            for (uint32_t i = 0; i < record.return_address_count; ++i) {
                const uint32_t address = record.return_addresses[i];
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: original caller[%u]=0x%08X RVA=0x%08X",
                    i, address, address - (uint32_t)k_original_image_base);
            }
        }

        void Describe_Message_Offset_(size_t block_offset, char* description, size_t description_size) {
            const size_t message_index = block_offset / sizeof(s_MSG);
            const size_t message_offset = block_offset % sizeof(s_MSG);
            if (message_offset == offsetof(s_MSG, msg_type)) {
                snprintf(description, description_size, "MSG_::_msgs[%zu].msg_type", message_index);
            } else if (message_offset == offsetof(s_MSG, priority)) {
                snprintf(description, description_size, "MSG_::_msgs[%zu].priority", message_index);
            } else if (message_offset >= offsetof(s_MSG, reserved)) {
                snprintf(description, description_size, "MSG_::_msgs[%zu].reserved[%zu]",
                    message_index, message_offset - offsetof(s_MSG, reserved));
            } else {
                snprintf(description, description_size, "MSG_::_msgs[%zu].u +%zu",
                    message_index, message_offset - offsetof(s_MSG, u));
            }
        }

        void Log_Message_Difference_(const std::vector<uint8_t>& actual,
            const std::vector<uint8_t>& expected, size_t difference, size_t block_offset) {
            const size_t message_index = block_offset / sizeof(s_MSG);
            const size_t messages_offset = difference - block_offset;
            const size_t message_offset = messages_offset + message_index * sizeof(s_MSG);
            if (message_offset + sizeof(s_MSG) > actual.size() ||
                message_offset + sizeof(s_MSG) > expected.size()) {
                return;
            }

            s_MSG actual_message;
            s_MSG expected_message;
            memcpy(&actual_message, actual.data() + message_offset, sizeof(actual_message));
            memcpy(&expected_message, expected.data() + message_offset, sizeof(expected_message));
            const uint8_t* actual_payload = reinterpret_cast<const uint8_t*>(&actual_message.u);
            const uint8_t* expected_payload = reinterpret_cast<const uint8_t*>(&expected_message.u);
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: message slot %zu actual type=%d (%s) priority=%u payload=%02X %02X %02X %02X %02X %02X %02X %02X",
                message_index, (int)actual_message.msg_type, Message_Type_Name_(actual_message.msg_type),
                (unsigned int)actual_message.priority,
                actual_payload[0], actual_payload[1], actual_payload[2], actual_payload[3],
                actual_payload[4], actual_payload[5], actual_payload[6], actual_payload[7]);
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: message slot %zu expected type=%d (%s) priority=%u payload=%02X %02X %02X %02X %02X %02X %02X %02X",
                message_index, (int)expected_message.msg_type, Message_Type_Name_(expected_message.msg_type),
                (unsigned int)expected_message.priority,
                expected_payload[0], expected_payload[1], expected_payload[2], expected_payload[3],
                expected_payload[4], expected_payload[5], expected_payload[6], expected_payload[7]);
        }

        void Describe_Colony_Offset_(size_t block_offset, char* description, size_t description_size) {
            const int16_t colony_index = (int16_t)(block_offset / sizeof(s_colony));
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
                    snprintf(description, description_size, "MOX::_colony[%d].%s +%zu",
                        colony_index, field.name, colony_offset - field.offset);
                    return;
                }
            }

            snprintf(description, description_size, "MOX::_colony[%d] +%zu",
                colony_index, colony_offset);
        }

        void Describe_Ship_Offset_(size_t block_offset, char* description, size_t description_size) {
            const int16_t ship_index = (int16_t)(block_offset / sizeof(s_ship_data));
            const size_t ship_offset = block_offset % sizeof(s_ship_data);
#define SHIP_FIELD(name) {#name, offsetof(s_ship_data, name), sizeof(s_ship_data::name), 0}
#define SHIP_ARRAY(name) {#name, offsetof(s_ship_data, name), sizeof(s_ship_data::name), sizeof(((s_ship_data*)nullptr)->name[0])}
            const s_field_range fields[] = {
                SHIP_FIELD(owner),
                SHIP_FIELD(status),
                SHIP_FIELD(location),
                SHIP_FIELD(x),
                SHIP_FIELD(y),
                SHIP_FIELD(group_has_navigator),
                SHIP_FIELD(travelling_speed),
                SHIP_FIELD(turns_left),
                SHIP_FIELD(shield_damage_percent),
                SHIP_FIELD(drive_damage_percent),
                SHIP_FIELD(computer_damage),
                SHIP_FIELD(crew_quality),
                SHIP_FIELD(crew_experience),
                SHIP_FIELD(officer_index),
                SHIP_ARRAY(special_device_damage_flags),
                SHIP_FIELD(armor_damage),
                SHIP_FIELD(structural_damage),
                SHIP_FIELD(mission),
                SHIP_FIELD(just_built),
            };
#undef SHIP_ARRAY
#undef SHIP_FIELD

            if (ship_offset < sizeof(s_ship_design)) {
                const size_t design_offset = ship_offset;
#define DESIGN_FIELD(name) {#name, offsetof(s_ship_design, name), sizeof(s_ship_design::name), 0}
#define DESIGN_ARRAY(name) {#name, offsetof(s_ship_design, name), sizeof(s_ship_design::name), sizeof(((s_ship_design*)nullptr)->name[0])}
                const s_field_range design_fields[] = {
                    DESIGN_ARRAY(name),
                    DESIGN_FIELD(size),
                    DESIGN_FIELD(ship_type),
                    DESIGN_FIELD(shield_type),
                    DESIGN_FIELD(ftl_type),
                    DESIGN_FIELD(speed),
                    DESIGN_FIELD(computer_type),
                    DESIGN_FIELD(armor_type),
                    DESIGN_ARRAY(special_device_flags),
                    DESIGN_ARRAY(ship_weapon),
                    DESIGN_FIELD(picture_num),
                    DESIGN_FIELD(previous_owner),
                    DESIGN_FIELD(cost),
                    DESIGN_FIELD(combat_speed),
                    DESIGN_FIELD(date_of_design),
                };
#undef DESIGN_ARRAY
#undef DESIGN_FIELD

                for (const s_field_range& field : design_fields) {
                    if (design_offset < field.offset ||
                        design_offset >= field.offset + field.size) {
                        continue;
                    }

                    const size_t field_offset = design_offset - field.offset;
                    if (field.element_size != 0) {
                        snprintf(description, description_size,
                            "MOX::_ship[%d].d.%s[%zu] +%zu",
                            ship_index, field.name,
                            field_offset / field.element_size,
                            field_offset % field.element_size);
                    } else {
                        snprintf(description, description_size,
                            "MOX::_ship[%d].d.%s +%zu",
                            ship_index, field.name, field_offset);
                    }
                    return;
                }
            }

            for (const s_field_range& field : fields) {
                if (ship_offset < field.offset ||
                    ship_offset >= field.offset + field.size) {
                    continue;
                }

                const size_t field_offset = ship_offset - field.offset;
                if (field.element_size != 0) {
                    snprintf(description, description_size,
                        "MOX::_ship[%d].%s[%zu] +%zu",
                        ship_index, field.name,
                        field_offset / field.element_size,
                        field_offset % field.element_size);
                } else {
                    snprintf(description, description_size,
                        "MOX::_ship[%d].%s +%zu",
                        ship_index, field.name, field_offset);
                }
                return;
            }

            snprintf(description, description_size, "MOX::_ship[%d] +%zu",
                ship_index, ship_offset);
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
            if (!Canonicalize_Save_Comparison_(&actual, actual_path) ||
                !Canonicalize_Save_Comparison_(&expected, expected_path)) {
                return false;
            }

            size_t applied_cleanup_count = 0;
            size_t difference = 0;
            while (true) {
                difference = Find_First_Save_Difference_(actual, expected);
                const size_t common_size = std::min(actual.size(), expected.size());
                if (difference == common_size) {
                    break;
                }

                const char* rule_name = nullptr;
                if (!Cleanup_Save_Comparison_(&actual, &expected, difference, &rule_name)) {
                    break;
                }

                ++applied_cleanup_count;
                size_t cleanup_block_offset;
                const char* cleanup_block_name = Describe_Save_Offset_(actual,
                    difference, &cleanup_block_offset);
                char cleanup_field_description[128] = {};
                if (strcmp(cleanup_block_name, "messages") == 0) {
                    Describe_Message_Offset_(cleanup_block_offset,
                        cleanup_field_description, sizeof(cleanup_field_description));
                } else if (strcmp(cleanup_block_name, "players") == 0) {
                    Describe_Player_Offset_(cleanup_block_offset,
                        cleanup_field_description, sizeof(cleanup_field_description));
                } else if (strcmp(cleanup_block_name, "ships") == 0) {
                    Describe_Ship_Offset_(cleanup_block_offset,
                        cleanup_field_description, sizeof(cleanup_field_description));
                }
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: turn %u cleanup #%zu at save offset %zu (%s +%zu%s%s): %s; re-verifying",
                    next_turn, applied_cleanup_count, difference,
                    cleanup_block_name, cleanup_block_offset,
                    cleanup_field_description[0] != '\0' ? ", " : "", cleanup_field_description,
                    rule_name);
            }

            const size_t common_size = std::min(actual.size(), expected.size());
            if (difference == common_size && actual.size() == expected.size()) {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: turn %u matches '%s' (%zu bytes, %zu cleanup rule%s applied)",
                    next_turn, expected_path, expected.size(), applied_cleanup_count,
                    applied_cleanup_count == 1 ? "" : "s");
                return true;
            }

            if (!Load_Turn_State_(next_turn)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: mismatch found but expected turn %u could not be restored", next_turn);
                return false;
            }

            size_t block_offset;
            const char* block_name = Describe_Save_Offset_(actual, difference,
                &block_offset);
            char field_description[128] = {};
            if (strcmp(block_name, "colonies") == 0) {
                Describe_Colony_Offset_(block_offset, field_description, sizeof(field_description));
            } else if (strcmp(block_name, "players") == 0) {
                Describe_Player_Offset_(block_offset, field_description, sizeof(field_description));
            } else if (strcmp(block_name, "ships") == 0) {
                Describe_Ship_Offset_(block_offset, field_description, sizeof(field_description));
            } else if (strcmp(block_name, "messages") == 0) {
                Describe_Message_Offset_(block_offset, field_description, sizeof(field_description));
            }
            const int actual_byte = difference < actual.size() ? actual[difference] : -1;
            const int expected_byte = difference < expected.size() ? expected[difference] : -1;
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: turn %u differs at save offset %zu (%s +%zu%s%s): actual=%d expected=%d; sizes actual=%zu expected=%zu",
                next_turn, difference, block_name, block_offset,
                field_description[0] != '\0' ? ", " : "", field_description,
                actual_byte, expected_byte,
                actual.size(), expected.size());
            if (strcmp(block_name, "messages") == 0) {
                Log_Message_Difference_(actual, expected, difference, block_offset);
            }
            return false;
        }

        template <typename T>
        void Append_Tactical_Value_(std::vector<uint8_t>* output,
            const T& value) {
            const size_t offset = output->size();
            output->resize(offset + sizeof(value));
            memcpy(output->data() + offset, &value, sizeof(value));
        }

        void Append_Tactical_Bytes_(std::vector<uint8_t>* output,
            const void* data, size_t size) {
            const size_t offset = output->size();
            output->resize(offset + size);
            if (size != 0) {
                memcpy(output->data() + offset, data, size);
            }
        }

        bool Capture_Tactical_State_(std::vector<uint8_t>* output,
            bool allow_missing_missiles) {
#if UINTPTR_MAX != UINT32_MAX
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: tactical state replay requires a 32-bit build");
            return false;
#else
            static_assert(sizeof(s_combat_data) ==
                k_original_combat_data_size);
            static_assert(sizeof(s_missile) == k_original_missile_size);
            static_assert(sizeof(s_colony) == k_colony_size);
            static_assert(sizeof(s_combat_bomb_result) ==
                k_combat_bomb_result_size);
            static_assert(offsetof(s_combat_data, ship_image_seg) == 198);

            if (MOX::_combat_total_ship_count < 0 ||
                MOX::_combat_total_ship_count >
                    static_cast<int16_t>(k_combat_ship_capacity) ||
                MOX::_combat_data == nullptr ||
                (MOX::_missile == nullptr && !allow_missing_missiles)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: invalid local tactical state, ships=%d combat=%p missiles=%p",
                    static_cast<int>(MOX::_combat_total_ship_count),
                    static_cast<void*>(MOX::_combat_data),
                    static_cast<void*>(MOX::_missile));
                return false;
            }

            const s_tactical_scalar_state scalars = {
                MOX::_combat_attacker_count,
                MOX::_combat_defender_count,
                MOX::_combat_attacker_player,
                MOX::_combat_defender_player,
                MOX::_combat_turn,
                MOX::_cur_ship,
                MOX::_cur_ptr,
                MOX::_cur_missile_count,
                MOX::_combat_init,
                MOX::_apply_damage_to_planet,
                MOX::_in_nebula_flag,
                MOX::_auto_combat,
                MOX::_take_control_flag,
                MOX::_colony_defense_flags,
                COMBINIT::_real_attack_flag,
                MOX::_cmbt_colony_id,
                static_cast<uint16_t>(MOX::_bio_weapon_flag),
                MOX::_ai_retreat_flag,
                COMBAT1::_start_of_turn,
                CMBTAI::_fleet_target
            };

            output->clear();
            Append_Tactical_Value_(output, scalars);
            Append_Tactical_Bytes_(output, MOX::_combat_data,
                static_cast<size_t>(MOX::_combat_total_ship_count) *
                    sizeof(s_combat_data));
            if (MOX::_missile != nullptr) {
                Append_Tactical_Bytes_(output, MOX::_missile,
                    k_missile_capacity * sizeof(s_missile));
            } else {
                output->resize(output->size() +
                    k_missile_capacity * sizeof(s_missile), 0);
            }
            Append_Tactical_Bytes_(output, MOX::_ship_turn_order,
                sizeof(MOX::_ship_turn_order));
            Append_Tactical_Bytes_(output, MOX::_combat_grid,
                sizeof(MOX::_combat_grid));
            Append_Tactical_Value_(output, MOX::_global_combat_data);
            Append_Tactical_Value_(output, MOX::_combat_bomb_result);
            Append_Tactical_Bytes_(output, MOX::_side_end_of_turn,
                sizeof(MOX::_side_end_of_turn));
            Append_Tactical_Bytes_(output, MOX::_player_mode,
                sizeof(MOX::_player_mode));
            Append_Tactical_Bytes_(output, MOX::_auto_combat_flag,
                sizeof(MOX::_auto_combat_flag));
            Append_Tactical_Bytes_(output, MOX::_ordnance_bonus,
                sizeof(MOX::_ordnance_bonus));
            Append_Tactical_Bytes_(output, MOX::_ranged_damage_penalty,
                sizeof(MOX::_ranged_damage_penalty));
            Append_Tactical_Bytes_(output, MOX::_ranged_to_hit_penalty,
                sizeof(MOX::_ranged_to_hit_penalty));

            if (MOX::_cmbt_colony_id >= 0) {
                if (MOX::_colony == nullptr ||
                    MOX::_cmbt_colony_id >= MAX_COLONIES) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: invalid tactical colony index %d",
                        static_cast<int>(MOX::_cmbt_colony_id));
                    return false;
                }
                Append_Tactical_Value_(output,
                    MOX::_colony[MOX::_cmbt_colony_id]);
            }
            return true;
#endif
        }

        bool Normalize_Tactical_State_(std::vector<uint8_t>* payload,
            const s_tactical_state_record& record,
            bool incomplete_tactical_setup) {
            constexpr size_t pointer_offset = 198;
            constexpr size_t pointer_size = sizeof(uint32_t);
            const bool no_tactical_turn =
                record.checkpoint_kind == k_tactical_combat_end &&
                record.checkpoint_index == 0;

            // Tactical combat neither initializes nor consumes these strategic-combat counters.
            memset(payload->data() +
                offsetof(s_tactical_scalar_state, combat_attacker_count), 0,
                sizeof(s_tactical_scalar_state::combat_attacker_count));
            memset(payload->data() +
                offsetof(s_tactical_scalar_state, combat_defender_count), 0,
                sizeof(s_tactical_scalar_state::combat_defender_count));

            if (record.checkpoint_kind == k_tactical_turn_begin ||
                no_tactical_turn) {
                // Tactical setup leaves these stale; Do_Combat_Turn_ replaces them before use.
                memset(payload->data() +
                    offsetof(s_tactical_scalar_state, current_ship), 0,
                    sizeof(s_tactical_scalar_state::current_ship));
                memset(payload->data() +
                    offsetof(s_tactical_scalar_state, current_pointer), 0,
                    sizeof(s_tactical_scalar_state::current_pointer));
                memset(payload->data() +
                    offsetof(s_tactical_scalar_state, auto_combat), 0,
                    sizeof(s_tactical_scalar_state::auto_combat));
                memset(payload->data() +
                    offsetof(s_tactical_scalar_state,
                        current_missile_count), 0,
                    sizeof(s_tactical_scalar_state::current_missile_count));
                memset(payload->data() +
                    offsetof(s_tactical_scalar_state, start_of_turn), 0,
                    sizeof(s_tactical_scalar_state::start_of_turn));
            }

            if (no_tactical_turn) {
                // The combat loop sets this only immediately before Do_Combat_Turn_.
                memset(payload->data() +
                    offsetof(s_tactical_scalar_state, fleet_target), 0,
                    sizeof(s_tactical_scalar_state::fleet_target));
            }

            if (incomplete_tactical_setup) {
                memset(payload->data() +
                    offsetof(s_tactical_scalar_state,
                        current_missile_count), 0,
                    sizeof(s_tactical_scalar_state::current_missile_count));
                memset(payload->data() +
                    offsetof(s_tactical_scalar_state, auto_combat), 0,
                    sizeof(s_tactical_scalar_state::auto_combat));
                memset(payload->data() +
                    offsetof(s_tactical_scalar_state,
                        colony_defense_flags), 0,
                    sizeof(s_tactical_scalar_state::colony_defense_flags));
            }

            size_t combat_offset = record.scalar_size;
            size_t turn_order_offset = combat_offset +
                record.combat_data_size + record.missile_data_size;
            size_t combat_grid_offset = turn_order_offset +
                record.turn_order_size;
            size_t global_offset = combat_offset +
                record.combat_data_size +
                record.missile_data_size +
                record.turn_order_size +
                record.combat_grid_size;
            size_t tactical_arrays_offset = global_offset +
                record.global_combat_data_size +
                record.combat_bomb_result_size;

            if (payload->size() < global_offset +
                    record.global_combat_data_size +
                    record.combat_bomb_result_size +
                    record.tactical_arrays_size ||
                pointer_offset + pointer_size >
                    k_original_combat_data_size) {
                return false;
            }

            if (incomplete_tactical_setup) {
                memset(payload->data() + combat_offset +
                    record.combat_data_size, 0,
                    record.missile_data_size);
                memset(payload->data() + turn_order_offset, 0,
                    record.turn_order_size);
                memset(payload->data() + combat_grid_offset, 0,
                    record.combat_grid_size);
                memset(payload->data() + tactical_arrays_offset, 0,
                    record.tactical_arrays_size);
            }

            // This is a ship-detail presentation scratch record. It is not initialized or
            // consumed by tactical simulation and can persist across saves and combats.
            memset(payload->data() + global_offset, 0,
                record.global_combat_data_size);

            // This block belongs to the separate bombardment combat path and is stale here.
            memset(payload->data() + global_offset +
                record.global_combat_data_size, 0,
                record.combat_bomb_result_size);

            if (record.checkpoint_kind == k_tactical_turn_begin ||
                no_tactical_turn) {
                memset(payload->data() + turn_order_offset, 0,
                    record.turn_order_size);
                memset(payload->data() + combat_grid_offset, 0,
                    record.combat_grid_size);
                memset(payload->data() + tactical_arrays_offset, 0,
                    sizeof(MOX::_side_end_of_turn));
            }

            s_tactical_scalar_state scalars;
            memcpy(&scalars, payload->data(), sizeof(scalars));
            const size_t player_mode_offset = tactical_arrays_offset +
                sizeof(MOX::_side_end_of_turn);
            for (size_t i = 0;
                i < sizeof(MOX::_player_mode) / sizeof(MOX::_player_mode[0]);
                ++i) {
                if (i != static_cast<size_t>(
                        scalars.combat_attacker_player) &&
                    i != static_cast<size_t>(
                        scalars.combat_defender_player)) {
                    memset(payload->data() + player_mode_offset +
                        i * sizeof(MOX::_player_mode[0]), 0,
                        sizeof(MOX::_player_mode[0]));
                }
            }
            const size_t ordnance_offset = tactical_arrays_offset +
                sizeof(MOX::_side_end_of_turn) +
                sizeof(MOX::_player_mode) +
                sizeof(MOX::_auto_combat_flag);
            for (size_t i = 0;
                i < sizeof(MOX::_ordnance_bonus) /
                    sizeof(MOX::_ordnance_bonus[0]);
                ++i) {
                if (i != static_cast<size_t>(
                        scalars.combat_attacker_player) &&
                    i != static_cast<size_t>(
                        scalars.combat_defender_player)) {
                    memset(payload->data() + ordnance_offset +
                        i * sizeof(MOX::_ordnance_bonus[0]), 0,
                        sizeof(MOX::_ordnance_bonus[0]));
                }
            }

            for (int16_t i = 0;
                i < record.combat_total_ship_count; ++i) {
                const size_t offset = combat_offset +
                    static_cast<size_t>(i) *
                        k_original_combat_data_size +
                    pointer_offset;
                memset(payload->data() + offset, 0, pointer_size);
            }

            memset(payload->data() + global_offset + pointer_offset,
                0, pointer_size);
            return true;
        }

        const char* Describe_Tactical_Offset_(
            const s_tactical_state_record& record, size_t difference,
            size_t* section_offset, size_t* element_index,
            size_t* element_offset) {
            struct s_section {
                const char* name;
                size_t size;
                size_t element_size;
            };

            const s_section sections[] = {
                {"scalars", record.scalar_size, record.scalar_size},
                {"combat ships", record.combat_data_size,
                    k_original_combat_data_size},
                {"missiles", record.missile_data_size,
                    k_original_missile_size},
                {"turn order", record.turn_order_size, sizeof(int16_t)},
                {"combat grid", record.combat_grid_size, sizeof(int16_t)},
                {"global combat data", record.global_combat_data_size,
                    k_original_combat_data_size},
                {"combat bomb result", record.combat_bomb_result_size,
                    record.combat_bomb_result_size},
                {"tactical arrays", record.tactical_arrays_size, 1},
                {"colony", record.colony_size,
                    record.colony_size == 0 ? 1 : record.colony_size},
            };

            size_t start = 0;
            for (const s_section& section : sections) {
                if (difference < start + section.size) {
                    *section_offset = difference - start;
                    *element_index =
                        *section_offset / section.element_size;
                    *element_offset =
                        *section_offset % section.element_size;
                    return section.name;
                }
                start += section.size;
            }

            *section_offset = difference;
            *element_index = 0;
            *element_offset = difference;
            return "unknown";
        }

        bool Verify_Tactical_State_(uint32_t checkpoint_kind,
            int16_t active_player_id, int16_t time_warp_only) {
            if (g_tactical_state_index >= g_tactical_states.size()) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: unexpected tactical checkpoint in strategic turn %u, combat %u, tactical turn %d",
                    g_turn, g_active_combat_instance,
                    static_cast<int>(MOX::_combat_turn));
                return false;
            }

            const s_tactical_state& expected_state =
                g_tactical_states[g_tactical_state_index];
            const s_tactical_state_record& expected =
                expected_state.record;

            if (expected.checkpoint_kind != checkpoint_kind ||
                expected.combat_instance != g_active_combat_instance ||
                expected.combat_turn != MOX::_combat_turn ||
                expected.combat_total_ship_count !=
                    MOX::_combat_total_ship_count ||
                expected.random_call_count != g_random_record_index ||
                expected.random_seed_count !=
                    g_random_seed_record_index ||
                expected.random_seed !=
                    game_random::Get_Random_Seed_() ||
                (checkpoint_kind == k_tactical_turn_begin &&
                    (expected.active_player_id != active_player_id ||
                     expected.time_warp_only != time_warp_only))) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: tactical checkpoint #%zu metadata mismatch: oracle kind=%u combat=%u turn=%d ships=%d phase=%d/%d rng=%u/%u seed=%u; local kind=%u combat=%u turn=%d ships=%d phase=%d/%d rng=%zu/%zu seed=%u",
                    g_tactical_state_index,
                    expected.checkpoint_kind,
                    expected.combat_instance,
                    static_cast<int>(expected.combat_turn),
                    static_cast<int>(
                        expected.combat_total_ship_count),
                    static_cast<int>(expected.active_player_id),
                    static_cast<int>(expected.time_warp_only),
                    expected.random_call_count,
                    expected.random_seed_count,
                    expected.random_seed,
                    checkpoint_kind,
                    g_active_combat_instance,
                    static_cast<int>(MOX::_combat_turn),
                    static_cast<int>(MOX::_combat_total_ship_count),
                    static_cast<int>(active_player_id),
                    static_cast<int>(time_warp_only),
                    g_random_record_index,
                    g_random_seed_record_index,
                    game_random::Get_Random_Seed_());
                return false;
            }

            if (checkpoint_kind == k_tactical_turn_begin) {
                const size_t tactical_arrays_offset =
                    expected.scalar_size +
                    expected.combat_data_size +
                    expected.missile_data_size +
                    expected.turn_order_size +
                    expected.combat_grid_size +
                    expected.global_combat_data_size +
                    expected.combat_bomb_result_size;
                const size_t player_mode_offset = tactical_arrays_offset +
                    sizeof(MOX::_side_end_of_turn);
                if (expected_state.payload.size() < player_mode_offset +
                        sizeof(MOX::_player_mode) ||
                    MOX::_combat_attacker_player < 0 ||
                    MOX::_combat_attacker_player >= 15 ||
                    MOX::_combat_defender_player < 0 ||
                    MOX::_combat_defender_player >= 15) {
                    return false;
                }

                const int8_t* expected_player_modes =
                    reinterpret_cast<const int8_t*>(
                        expected_state.payload.data() + player_mode_offset);
                MOX::_player_mode[MOX::_combat_attacker_player] =
                    expected_player_modes[MOX::_combat_attacker_player];
                MOX::_player_mode[MOX::_combat_defender_player] =
                    expected_player_modes[MOX::_combat_defender_player];
            }

            std::vector<uint8_t> actual;
            const bool incomplete_tactical_setup =
                checkpoint_kind == k_tactical_combat_end &&
                MOX::_combat_turn == 0 && MOX::_missile == nullptr;
            if (!Capture_Tactical_State_(&actual,
                    incomplete_tactical_setup)) {
                return false;
            }

            std::vector<uint8_t> expected_payload =
                expected_state.payload;
            if (actual.size() != expected_payload.size()) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: tactical checkpoint #%zu payload size mismatch: oracle=%zu local=%zu",
                    g_tactical_state_index, expected_payload.size(),
                    actual.size());
                return false;
            }

            if (!Normalize_Tactical_State_(&expected_payload,
                    expected, incomplete_tactical_setup) ||
                !Normalize_Tactical_State_(&actual, expected,
                    incomplete_tactical_setup)) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: tactical checkpoint #%zu could not be normalized",
                    g_tactical_state_index);
                return false;
            }

            size_t difference = 0;
            while (difference < actual.size() &&
                actual[difference] == expected_payload[difference]) {
                ++difference;
            }
            if (difference != actual.size()) {
                if (difference < sizeof(s_tactical_scalar_state)) {
                    s_tactical_scalar_state expected_scalars;
                    s_tactical_scalar_state actual_scalars;
                    memcpy(&expected_scalars, expected_payload.data(),
                        sizeof(expected_scalars));
                    memcpy(&actual_scalars, actual.data(),
                        sizeof(actual_scalars));
                    const int16_t* expected_values =
                        reinterpret_cast<const int16_t*>(&expected_scalars);
                    const int16_t* actual_values =
                        reinterpret_cast<const int16_t*>(&actual_scalars);
                    for (size_t i = 0;
                        i < sizeof(s_tactical_scalar_state) / sizeof(int16_t);
                        ++i) {
                        if (expected_values[i] != actual_values[i]) {
                            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                                "oracle: tactical scalar word %zu differs: oracle=%d local=%d",
                                i, (int)expected_values[i],
                                (int)actual_values[i]);
                        }
                    }
                }
                size_t section_offset;
                size_t element_index;
                size_t element_offset;
                const char* section = Describe_Tactical_Offset_(
                    expected, difference, &section_offset,
                    &element_index, &element_offset);
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: tactical checkpoint #%zu differs at payload offset %zu (%s +%zu, element %zu +%zu): oracle=%u local=%u; combat=%u turn=%d",
                    g_tactical_state_index, difference, section,
                    section_offset, element_index, element_offset,
                    static_cast<unsigned>(
                        expected_payload[difference]),
                    static_cast<unsigned>(actual[difference]),
                    expected.combat_instance,
                    static_cast<int>(expected.combat_turn));
                return false;
            }

            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: tactical checkpoint #%zu matches, combat=%u turn=%d kind=%u, %zu bytes",
                g_tactical_state_index, expected.combat_instance,
                static_cast<int>(expected.combat_turn),
                expected.checkpoint_kind, actual.size());
            ++g_tactical_state_index;
            return true;
        }

        bool Capture_Strategic_State_(std::vector<uint8_t>* output) {
            static_assert(sizeof(s_qship) ==
                k_original_quick_combat_data_size);
            static_assert(sizeof(s_colony) == k_colony_size);

            if (MOX::_combat_total_ship_count < 0 ||
                MOX::_combat_total_ship_count >
                    static_cast<int16_t>(
                        k_original_quick_combat_capacity) ||
                MOX::_qcombat_data == nullptr) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: invalid local strategic state, ships=%d combat=%p",
                    static_cast<int>(MOX::_combat_total_ship_count),
                    static_cast<void*>(MOX::_qcombat_data));
                return false;
            }

            output->clear();
            Append_Tactical_Bytes_(output, MOX::_qcombat_data,
                static_cast<size_t>(MOX::_combat_total_ship_count) *
                    sizeof(s_qship));
            if (MOX::_cmbt_colony_id >= 0) {
                if (MOX::_colony == nullptr ||
                    MOX::_cmbt_colony_id >= MAX_COLONIES) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: invalid strategic colony index %d",
                        static_cast<int>(MOX::_cmbt_colony_id));
                    return false;
                }
                Append_Tactical_Value_(output,
                    MOX::_colony[MOX::_cmbt_colony_id]);
            }
            return true;
        }

        bool Verify_Strategic_State_() {
            if (g_strategic_state_index >= g_strategic_states.size()) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: unexpected strategic checkpoint in turn %u, combat %u round %d",
                    g_turn, g_active_strategic_combat_instance,
                    static_cast<int>(MOX::_combat_turn));
                return false;
            }

            const s_strategic_state& expected_state =
                g_strategic_states[g_strategic_state_index];
            const s_strategic_state_record& expected =
                expected_state.record;
            const uint32_t colony_size = MOX::_cmbt_colony_id >= 0
                ? k_colony_size : 0;

            if (expected.combat_instance !=
                    g_active_strategic_combat_instance ||
                expected.combat_turn != MOX::_combat_turn ||
                expected.combat_total_ship_count !=
                    MOX::_combat_total_ship_count ||
                expected.combat_attacker_count !=
                    MOX::_combat_attacker_count ||
                expected.combat_defender_count !=
                    MOX::_combat_defender_count ||
                expected.combat_attacker_player !=
                    MOX::_combat_attacker_player ||
                expected.combat_defender_player !=
                    MOX::_combat_defender_player ||
                expected.combat_colony_id != MOX::_cmbt_colony_id ||
                expected.colony_defense_flags !=
                    MOX::_colony_defense_flags ||
                expected.random_call_count != g_random_record_index ||
                expected.random_seed_count !=
                    g_random_seed_record_index ||
                expected.random_seed != game_random::Get_Random_Seed_() ||
                expected.colony_size != colony_size) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: strategic checkpoint #%zu metadata mismatch: oracle combat=%u round=%d ships=%d sides=%d/%d players=%d/%d colony=%d defenses=%u rng=%u/%u seed=%u; local combat=%u round=%d ships=%d sides=%d/%d players=%d/%d colony=%d defenses=%u rng=%zu/%zu seed=%u",
                    g_strategic_state_index, expected.combat_instance,
                    static_cast<int>(expected.combat_turn),
                    static_cast<int>(expected.combat_total_ship_count),
                    static_cast<int>(expected.combat_attacker_count),
                    static_cast<int>(expected.combat_defender_count),
                    static_cast<int>(expected.combat_attacker_player),
                    static_cast<int>(expected.combat_defender_player),
                    static_cast<int>(expected.combat_colony_id),
                    static_cast<unsigned>(expected.colony_defense_flags),
                    expected.random_call_count, expected.random_seed_count,
                    expected.random_seed,
                    g_active_strategic_combat_instance,
                    static_cast<int>(MOX::_combat_turn),
                    static_cast<int>(MOX::_combat_total_ship_count),
                    static_cast<int>(MOX::_combat_attacker_count),
                    static_cast<int>(MOX::_combat_defender_count),
                    static_cast<int>(MOX::_combat_attacker_player),
                    static_cast<int>(MOX::_combat_defender_player),
                    static_cast<int>(MOX::_cmbt_colony_id),
                    static_cast<unsigned>(MOX::_colony_defense_flags),
                    g_random_record_index, g_random_seed_record_index,
                    game_random::Get_Random_Seed_());
                return false;
            }

            std::vector<uint8_t> actual;
            if (!Capture_Strategic_State_(&actual) ||
                actual.size() != expected_state.payload.size()) {
                return false;
            }

            size_t difference = 0;
            while (difference < actual.size() &&
                actual[difference] == expected_state.payload[difference]) {
                ++difference;
            }
            if (difference != actual.size()) {
                const bool in_quick_combat =
                    difference < expected.quick_combat_data_size;
                const size_t section_offset = in_quick_combat
                    ? difference
                    : difference - expected.quick_combat_data_size;
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: strategic checkpoint #%zu differs at payload offset %zu (%s +%zu, element %zu +%zu): oracle=%u local=%u; combat=%u round=%d",
                    g_strategic_state_index, difference,
                    in_quick_combat ? "quick combat ships" : "colony",
                    section_offset,
                    in_quick_combat
                        ? section_offset / k_original_quick_combat_data_size
                        : 0,
                    in_quick_combat
                        ? section_offset % k_original_quick_combat_data_size
                        : section_offset,
                    static_cast<unsigned>(expected_state.payload[difference]),
                    static_cast<unsigned>(actual[difference]),
                    expected.combat_instance,
                    static_cast<int>(expected.combat_turn));
                return false;
            }

            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: strategic checkpoint #%zu matches, combat=%u round=%d, %zu bytes",
                g_strategic_state_index, expected.combat_instance,
                static_cast<int>(expected.combat_turn), actual.size());
            ++g_strategic_state_index;
            return true;
        }

        bool Fail_(const char* message) {
            g_replay_active = false;
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "oracle: %s", message);
            platform::Request_Exit_(message);
            return false;
        }

        void Begin_Presentation_(uint32_t presentation_kind) {
            if (!g_replay_active) {
                return;
            }

            if (g_active_presentation_kind != 0) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: presentation %u started while presentation %u is active",
                    presentation_kind, g_active_presentation_kind);
                Fail_("Nested oracle presentation encountered.");
                throw platform::process_exit_requested{};
            }

            if (g_presentation_completion_index >=
                    g_presentation_completions.size() ||
                g_presentation_completions[
                    g_presentation_completion_index]
                        .record.presentation_kind != presentation_kind) {
                const uint32_t expected_kind =
                    g_presentation_completion_index <
                            g_presentation_completions.size()
                        ? g_presentation_completions[
                              g_presentation_completion_index]
                              .record.presentation_kind
                        : 0;
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: unexpected presentation %u in turn %u; next expected kind=%u",
                    presentation_kind, g_turn, expected_kind);
                Fail_("Oracle presentation sequence mismatch.");
                throw platform::process_exit_requested{};
            }

            g_active_presentation_kind = presentation_kind;
            g_active_presentation_ready = false;
            g_presentation_rng_desynced = false;
        }

        void Allow_Presentation_Completion_(uint32_t presentation_kind) {
            if (g_replay_active &&
                g_active_presentation_kind == presentation_kind) {
                g_active_presentation_ready = true;
            }
        }

        void End_Presentation_(uint32_t presentation_kind) {
            if (!g_replay_active) {
                return;
            }

            if (g_active_presentation_kind != presentation_kind ||
                g_presentation_completion_index >=
                    g_presentation_completions.size()) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: presentation %u ended while active kind=%u",
                    presentation_kind, g_active_presentation_kind);
                Fail_("Oracle presentation end mismatch.");
                throw platform::process_exit_requested{};
            }

            const s_presentation_complete_record& expected =
                g_presentation_completions[
                    g_presentation_completion_index]
                    .record;

            size_t passive_input_count = 0;
            while (g_record_index < expected.input_record_count) {
                const s_input_record& record =
                    g_records[g_record_index];
                const s_input_position& position =
                    g_record_positions[g_record_index];
                if (record.input_type != k_mouse_input_type ||
                    record.mouse_buttons != 0 ||
                    position.random_call_count >
                        expected.random_call_count ||
                    position.random_seed_count >
                        expected.random_seed_count) {
                    break;
                }

                mouse::User_Mouse_Handler_(record.mouse_x, record.mouse_y,
                    record.mouse_buttons);
                g_replay_mouse_buttons = record.mouse_buttons;
                ++g_record_index;
                ++passive_input_count;
            }

            size_t ignored_mouse_input_count = 0;
            if (g_record_index < expected.input_record_count) {
                bool balanced_mouse_tail = true;
                uint16_t final_mouse_buttons = g_replay_mouse_buttons;
                for (size_t i = g_record_index;
                    i < expected.input_record_count; ++i) {
                    const s_input_record& record = g_records[i];
                    const s_input_position& position =
                        g_record_positions[i];
                    if (record.input_type != k_mouse_input_type ||
                        position.random_call_count >
                            expected.random_call_count ||
                        position.random_seed_count >
                            expected.random_seed_count) {
                        balanced_mouse_tail = false;
                        break;
                    }
                    final_mouse_buttons = record.mouse_buttons;
                }

                if (balanced_mouse_tail && final_mouse_buttons == 0) {
                    const s_input_record& final_record =
                        g_records[expected.input_record_count - 1];
                    ignored_mouse_input_count =
                        expected.input_record_count - g_record_index;
                    mouse::User_Mouse_Handler_(final_record.mouse_x,
                        final_record.mouse_y, final_record.mouse_buttons);
                    g_replay_mouse_buttons = final_record.mouse_buttons;
                    g_record_index = expected.input_record_count;
                    SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: ignored %zu trailing balanced mouse event%s at presentation %u completion",
                        ignored_mouse_input_count,
                        ignored_mouse_input_count == 1 ? "" : "s",
                        presentation_kind);
                }
            }

            const bool rng_boundary_mismatch =
                g_random_record_index != expected.random_call_count ||
                g_random_seed_record_index != expected.random_seed_count;
            if (rng_boundary_mismatch && !g_presentation_rng_desynced) {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: presentation %u RNG boundary differs: local random=%zu seeds=%zu; oracle random=%u seeds=%u; tolerating until completion marker",
                    presentation_kind,
                    g_random_record_index, g_random_seed_record_index,
                    expected.random_call_count, expected.random_seed_count);
                g_presentation_rng_desynced = true;
            }

            if (!g_active_presentation_ready ||
                g_record_index != expected.input_record_count) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: presentation %u ended at input=%zu/%u random=%zu/%u seeds=%zu/%u ready=%d",
                    presentation_kind,
                    g_record_index, expected.input_record_count,
                    g_random_record_index, expected.random_call_count,
                    g_random_seed_record_index,
                    expected.random_seed_count,
                    g_active_presentation_ready ? 1 : 0);
                Fail_("Oracle presentation boundary mismatch.");
                throw platform::process_exit_requested{};
            }

            if (g_presentation_rng_desynced) {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: resynchronized presentation %u RNG counters at completion: random %zu->%u seeds %zu->%u",
                    presentation_kind,
                    g_random_record_index, expected.random_call_count,
                    g_random_seed_record_index, expected.random_seed_count);
                g_random_record_index = expected.random_call_count;
                g_random_seed_record_index = expected.random_seed_count;
            }

            if (passive_input_count != 0) {
                SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: consumed %zu trailing buttonless mouse event%s at presentation completion",
                    passive_input_count,
                    passive_input_count == 1 ? "" : "s");
            }

            SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: completed presentation #%zu kind=%u at input=%zu random=%zu seeds=%zu",
                g_presentation_completion_index,
                presentation_kind,
                g_record_index,
                g_random_record_index,
                g_random_seed_record_index);

            ++g_presentation_completion_index;
            g_active_presentation_kind = 0;
            g_active_presentation_ready = false;
            g_presentation_rng_desynced = false;
            g_click_pause_active = false;
            g_last_consumption_counter = SDL_GetPerformanceCounter();
        }

        void Log_Original_Return_Addresses_(const s_random_record& record) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: original image base=0x%08X, %u probable return addresses",
                (uint32_t)k_original_image_base, record.return_address_count);
            for (uint32_t i = 0; i < record.return_address_count; ++i) {
                const uint32_t address = record.return_addresses[i];
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: original caller[%u]=0x%08X RVA=0x%08X",
                    i, address, address - (uint32_t)k_original_image_base);
            }
        }

        void Log_Local_Return_Addresses_(const uintptr_t* return_addresses,
            uint32_t return_address_count, uintptr_t image_base) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: local image base=%p, %u captured return addresses",
                reinterpret_cast<void*>(image_base), return_address_count);
            for (uint32_t i = 0; i < return_address_count; ++i) {
                const uintptr_t address = return_addresses[i];
                if (image_base != 0 && address >= image_base) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: local caller[%u]=%p RVA=0x%llX",
                        i, reinterpret_cast<void*>(address),
                        (unsigned long long)(address - image_base));
                } else {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: local caller[%u]=%p RVA=unavailable",
                        i, reinterpret_cast<void*>(address));
                }
            }
        }
    }

    void Begin_Tactical_Combat_() {
        if (!g_replay_active) {
            return;
        }

        if (g_tactical_combat_active) {
            Fail_("Nested tactical combat encountered during oracle replay.");
            throw platform::process_exit_requested{};
        }

        ++g_active_combat_instance;
        g_last_verified_combat_turn =
            std::numeric_limits<int16_t>::min();
        g_tactical_combat_active = true;

        if (g_tactical_state_index >= g_tactical_states.size() ||
            g_tactical_states[g_tactical_state_index]
                    .record.combat_instance !=
                g_active_combat_instance) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: unexpected tactical combat %u in strategic turn %u",
                g_active_combat_instance, g_turn);
            Fail_("Oracle tactical combat sequence mismatch.");
            throw platform::process_exit_requested{};
        }
    }

    void Verify_Tactical_Turn_(int16_t active_player_id,
        int16_t time_warp_only) {
        if (!g_replay_active) {
            return;
        }
        if (!g_tactical_combat_active) {
            Fail_("Tactical turn encountered outside tactical combat.");
            throw platform::process_exit_requested{};
        }
        if (MOX::_combat_turn == g_last_verified_combat_turn) {
            return;
        }

        if (!Verify_Tactical_State_(k_tactical_turn_begin,
                active_player_id, time_warp_only)) {
            Fail_("Oracle tactical turn state mismatch.");
            throw platform::process_exit_requested{};
        }
        g_last_verified_combat_turn = MOX::_combat_turn;
    }

    void Verify_Tactical_Combat_End_() {
        if (!g_replay_active) {
            return;
        }
        if (!g_tactical_combat_active ||
            !Verify_Tactical_State_(k_tactical_combat_end, 0, 0)) {
            Fail_("Oracle tactical combat final state mismatch.");
            throw platform::process_exit_requested{};
        }
        g_tactical_combat_active = false;
    }

    void Verify_Strategic_Combat_State_() {
        if (!g_replay_active) {
            return;
        }

        if (MOX::_combat_turn == 0) {
            ++g_active_strategic_combat_instance;
        }
        if (!Verify_Strategic_State_()) {
            Fail_("Oracle strategic combat state mismatch.");
            throw platform::process_exit_requested{};
        }
    }

    void Begin_Ground_Combat_Presentation_() {
        Begin_Presentation_(k_ground_combat_presentation);
    }

    void Allow_Ground_Combat_Completion_() {
        Allow_Presentation_Completion_(
            k_ground_combat_presentation);
    }

    void End_Ground_Combat_Presentation_() {
        End_Presentation_(k_ground_combat_presentation);
    }

    void Begin_Bombing_Presentation_() {
        Begin_Presentation_(k_bombing_presentation);
    }

    void Allow_Bombing_Completion_() {
        Allow_Presentation_Completion_(
            k_bombing_presentation);
    }

    void End_Bombing_Presentation_() {
        End_Presentation_(k_bombing_presentation);
    }

    bool Configure_(int argc, char** argv) {
        g_enabled = false;
        g_replay_active = false;
        g_turn = 0;
        g_speed_percent = 100;
        g_delay_ms = 0;
        g_click_delay_ms = 100;

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
            } else if (strcmp(argv[i], "--oracle-click-delay-ms") == 0) {
                if (++i >= argc || !Parse_Uint32_(argv[i], 0, 60000, &g_click_delay_ms)) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: --oracle-click-delay-ms requires a value from 0 to 60000");
                    return false;
                }
                continue;
            } else if (strncmp(argv[i], "--oracle-click-delay-ms=", 24) == 0) {
                if (!Parse_Uint32_(argv[i] + 24, 0, 60000, &g_click_delay_ms)) {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                        "oracle: invalid click delay '%s'", argv[i] + 24);
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

#if UINTPTR_MAX != UINT32_MAX
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
            "oracle: oracle verification requires a 32-bit build because captured structures contain 32-bit pointers");
        return false;
#endif

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
            "oracle: enabled at turn %u using '%s', speed=%u%%, delay=%u ms, click delay=%u ms",
            g_turn, g_oracle_directory, g_speed_percent, g_delay_ms, g_click_delay_ms);
        return true;
    }

    bool Start_() {
        if (!g_enabled) {
            return true;
        }

        if (!Load_Turn_State_(g_turn)) {
            return Fail_("Oracle could not load the requested turn save. See the game log.");
        }

        const uint8_t pending_report = REPORT::Must_Display_Reports_(MOX::_PLAYER_NUM);
        if (pending_report != 0) {
            MOX::_current_screen = SCREEN_REPORTS;
            MOX::_return_screen = SCREEN_REPORTS;
        } else {
            MOX::_current_screen = SCREEN_MAIN;
            MOX::_return_screen = SCREEN_MAIN;
        }
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
            "oracle: isolated turn %u starts on screen %d with pending report %u",
            g_turn, (int)MOX::_current_screen, (unsigned int)pending_report);
        const e_prepare_result result = Prepare_Turn_(g_turn, true);
        if (result == PREPARE_ERROR) {
            return Fail_("Oracle could not read the requested turn input. See the game log.");
        }
        if (result == PREPARE_DONE) {
            MOX::_current_screen = SCREEN_EXIT;
        }
        return true;
    }

    void Pump_Input_() {
        if (!g_replay_active) {
            return;
        }

        size_t input_limit = g_records.size();
        if (g_active_presentation_kind != 0 &&
            g_presentation_completion_index <
                g_presentation_completions.size()) {
            const s_presentation_complete& presentation =
                g_presentation_completions[
                    g_presentation_completion_index];
            input_limit = g_active_presentation_ready
                ? presentation.record.input_record_count
                : presentation.terminal_input_index;
        }

        if (g_record_index >= input_limit) {
            return;
        }

        if (!g_replay_clock_armed) {
            g_replay_clock_armed = true;
            g_last_consumption_counter = SDL_GetPerformanceCounter();
            return;
        }

        if (g_click_pause_active) {
            const uint64_t now = SDL_GetPerformanceCounter();
            if (now < g_click_pause_until_counter) {
                return;
            }

            g_click_pause_active = false;
            g_last_consumption_counter = now;
            return;
        }

        const uint64_t elapsed_budget = SDL_GetPerformanceCounter() - g_last_consumption_counter;
        uint64_t consumed_budget = 0;
        bool consumed_input = false;

        while (g_record_index < input_limit) {
            const s_input_position& position =
                g_record_positions[g_record_index];
            if (!g_presentation_rng_desynced &&
                (g_random_record_index < position.random_call_count ||
                    g_random_seed_record_index < position.random_seed_count)) {
                break;
            }

            const uint64_t required_delta = Replay_Delta_Counters_(
                g_records[g_record_index].delta_qpc);
            if (required_delta > elapsed_budget - consumed_budget) {
                break;
            }

            const s_input_record& record = g_records[g_record_index];
            bool released_click = false;
            if (record.input_type == k_mouse_input_type) {
                released_click = (g_replay_mouse_buttons & ~record.mouse_buttons) != 0;
                mouse::User_Mouse_Handler_(record.mouse_x, record.mouse_y, record.mouse_buttons);
                g_replay_mouse_buttons = record.mouse_buttons;
            } else {
                key::Push_Key_Code_(record.key_code, record.scan_code);
            }

            consumed_budget += required_delta;
            consumed_input = true;
            ++g_record_index;

            if (released_click && g_click_delay_ms != 0) {
                g_click_pause_active = true;
                g_click_pause_until_counter = SDL_GetPerformanceCounter() +
                    Milliseconds_To_Counters_(g_click_delay_ms);
                break;
            }
        }

        if (consumed_input) {
            g_last_consumption_counter = SDL_GetPerformanceCounter();
        }
    }

    bool Deliver_Science_Input_Before_Jumble_() {
        if (!g_replay_active || g_record_index >= g_records.size()) {
            return false;
        }

        size_t action_index = g_record_index;
        uint16_t mouse_buttons = g_replay_mouse_buttons;
        for (; action_index < g_records.size(); ++action_index) {
            const s_input_position& position =
                g_record_positions[action_index];
            if (position.random_call_count != g_random_record_index ||
                position.random_seed_count != g_random_seed_record_index) {
                return false;
            }

            const s_input_record& record = g_records[action_index];
            if (record.input_type == k_key_input_type ||
                (record.mouse_buttons & ~mouse_buttons) != 0) {
                break;
            }
            mouse_buttons = record.mouse_buttons;
        }
        if (action_index >= g_records.size()) {
            return false;
        }

        const size_t first_input_index = g_record_index;
        while (g_record_index <= action_index) {
            const s_input_record& record = g_records[g_record_index++];
            if (record.input_type == k_mouse_input_type) {
                mouse::User_Mouse_Handler_(record.mouse_x, record.mouse_y,
                    record.mouse_buttons);
                g_replay_mouse_buttons = record.mouse_buttons;
            } else {
                key::Push_Key_Code_(record.key_code, record.scan_code);
            }
        }

        g_click_pause_active = false;
        g_last_consumption_counter = SDL_GetPerformanceCounter();
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION,
            "oracle: synchronized science input #%zu at random=%zu seeds=%zu after %zu passive event%s",
            action_index, g_random_record_index, g_random_seed_record_index,
            action_index - first_input_index,
            action_index - first_input_index == 1 ? "" : "s");
        return true;
    }

    void Complete_Turn_() {
        if (!g_replay_active) {
            return;
        }

        g_replay_active = false;
        if (g_random_record_index != g_random_records.size()) {
            const s_random_record& expected = g_random_records[g_random_record_index];
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: turn %u ended after %zu of %zu random calls; next call #%zu expects max=%u value=%u seed=%u",
                g_turn, g_random_record_index, g_random_records.size(), g_random_record_index,
                expected.max_value, expected.value, expected.random_seed);
            Log_Original_Return_Addresses_(expected);
            Fail_("Oracle random-call count mismatch. See the game log.");
            throw platform::process_exit_requested{};
        }
        if (g_random_seed_record_index != g_random_seed_records.size()) {
            const s_random_seed_record& expected =
                g_random_seed_records[g_random_seed_record_index];
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: turn %u ended after %zu of %zu random seed calls; next call #%zu expects seed=%u",
                g_turn, g_random_seed_record_index, g_random_seed_records.size(),
                g_random_seed_record_index, expected.random_seed);
            Log_Original_Return_Addresses_(expected);
            Fail_("Oracle random-seed call count mismatch. See the game log.");
            throw platform::process_exit_requested{};
        }
        if (g_tactical_combat_active ||
            g_tactical_state_index != g_tactical_states.size()) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: strategic turn %u ended after %zu of %zu tactical checkpoints; combat active=%d",
                g_turn, g_tactical_state_index,
                g_tactical_states.size(),
                g_tactical_combat_active ? 1 : 0);
            Fail_("Oracle tactical checkpoint count mismatch.");
            throw platform::process_exit_requested{};
        }
        if (g_strategic_state_index != g_strategic_states.size()) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: strategic turn %u ended after %zu of %zu strategic checkpoints",
                g_turn, g_strategic_state_index,
                g_strategic_states.size());
            Fail_("Oracle strategic checkpoint count mismatch.");
            throw platform::process_exit_requested{};
        }
        if (g_active_presentation_kind != 0 ||
            g_presentation_completion_index !=
                g_presentation_completions.size()) {
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: turn %u ended after %zu of %zu presentation completions; active kind=%u",
                g_turn, g_presentation_completion_index,
                g_presentation_completions.size(),
                g_active_presentation_kind);
            Fail_("Oracle presentation completion count mismatch.");
            throw platform::process_exit_requested{};
        }
        if (g_record_index != g_records.size()) {
            const size_t tail_start = g_record_index;
            bool balanced_mouse_tail = true;
            uint16_t final_mouse_buttons = g_replay_mouse_buttons;
            for (size_t i = tail_start; i < g_records.size(); ++i) {
                const s_input_record& record = g_records[i];
                const s_input_position& position = g_record_positions[i];
                if (record.input_type != k_mouse_input_type ||
                    position.random_call_count > g_random_record_index ||
                    position.random_seed_count > g_random_seed_record_index) {
                    balanced_mouse_tail = false;
                    break;
                }
                final_mouse_buttons = record.mouse_buttons;
            }

            if (!balanced_mouse_tail || final_mouse_buttons != 0) {
                Fail_("Oracle turn ended before all recorded input was injected. See the game log.");
                throw platform::process_exit_requested{};
            }

            const s_input_record& final_record = g_records.back();
            mouse::User_Mouse_Handler_(final_record.mouse_x,
                final_record.mouse_y, final_record.mouse_buttons);
            g_replay_mouse_buttons = final_record.mouse_buttons;
            g_record_index = g_records.size();
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: ignored %zu trailing balanced mouse event%s at turn %u completion",
                g_record_index - tail_start,
                g_record_index - tail_start == 1 ? "" : "s",
                g_turn);
        }

        const uint32_t next_turn = g_turn + 1;
        if (!Compare_Next_State_(next_turn)) {
            Fail_("Oracle game-state mismatch. See the game log for the first difference.");
            throw platform::process_exit_requested{};
        }

        const e_prepare_result result = Prepare_Turn_(next_turn, false);
        if (result == PREPARE_ERROR) {
            Fail_("Oracle could not read the next turn input. See the game log.");
            throw platform::process_exit_requested{};
        }
        if (result == PREPARE_DONE) {
            MOX::_current_screen = SCREEN_EXIT;
        }
    }

    void Verify_Random_(uint32_t max_value, uint32_t value, uint32_t random_seed,
        const uintptr_t* return_addresses, uint32_t return_address_count,
        uintptr_t image_base) {
        if (!g_replay_active) {
            return;
        }

        if (g_presentation_rng_desynced) {
            return;
        }

        if (g_random_record_index >= g_random_records.size()) {
            if (g_active_presentation_kind != 0) {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: unexpected random call #%zu during presentation %u in turn %u: max=%u value=%u seed=%u; ignoring RNG discrepancies until completion marker",
                    g_random_record_index, g_active_presentation_kind, g_turn,
                    max_value, value, random_seed);
                g_presentation_rng_desynced = true;
                return;
            }
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: unexpected random call #%zu in turn %u: max=%u value=%u seed=%u; oracle recorded %zu calls",
                g_random_record_index, g_turn, max_value, value, random_seed,
                g_random_records.size());
            Log_Local_Return_Addresses_(return_addresses, return_address_count, image_base);
            Fail_("Oracle random-call count mismatch. See the game log.");
            throw platform::process_exit_requested{};
        }

        const s_random_record& expected = g_random_records[g_random_record_index];
        if (max_value != expected.max_value || value != expected.value ||
            random_seed != expected.random_seed) {
            if (g_active_presentation_kind != 0) {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: random call #%zu differs during presentation %u in turn %u: oracle max=%u value=%u seed=%u; local max=%u value=%u seed=%u; ignoring RNG discrepancies until completion marker",
                    g_random_record_index, g_active_presentation_kind, g_turn,
                    expected.max_value, expected.value, expected.random_seed,
                    max_value, value, random_seed);
                g_presentation_rng_desynced = true;
                return;
            }
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: random call #%zu mismatch in turn %u: oracle max=%u value=%u seed=%u; local max=%u value=%u seed=%u",
                g_random_record_index, g_turn,
                expected.max_value, expected.value, expected.random_seed,
                max_value, value, random_seed);
            Log_Original_Return_Addresses_(expected);
            Log_Local_Return_Addresses_(return_addresses, return_address_count, image_base);
            Fail_("Oracle random sequence mismatch. See the game log.");
            throw platform::process_exit_requested{};
        }

        ++g_random_record_index;
    }

    uint32_t Replay_Address_Dependent_Random_Max_(uint32_t local_max_value) {
        constexpr uint32_t k_vanish_bitmap_random_return_address = 0x0053764B;
        if (!g_replay_active || g_presentation_rng_desynced ||
            g_random_record_index >= g_random_records.size()) {
            return local_max_value;
        }

        const s_random_record& expected = g_random_records[g_random_record_index];
        if (expected.return_address_count == 0 ||
            expected.return_addresses[0] != k_vanish_bitmap_random_return_address) {
            return local_max_value;
        }

        if (local_max_value != expected.max_value) {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: substituting address-dependent random max at call #%zu in turn %u: oracle max=%u; local max=%u",
                g_random_record_index, g_turn, expected.max_value, local_max_value);
        }
        return expected.max_value;
    }

    bool Defer_Strategic_Bomb_Hit_() {
        constexpr uint32_t k_pick_low_attacker_return_address = 0x004BCAF5;
        constexpr uint32_t k_pick_high_attacker_return_address = 0x004BCABA;
        if (!g_replay_active || g_random_record_index >= g_random_records.size()) {
            return false;
        }

        const s_random_record& expected = g_random_records[g_random_record_index];
        if (expected.return_address_count == 0) {
            return false;
        }

        const uint32_t caller = expected.return_addresses[0];
        return caller == k_pick_low_attacker_return_address ||
            caller == k_pick_high_attacker_return_address;
    }

    uint32_t Replay_Random_Seed_(uint32_t local_seed,
        const uintptr_t* return_addresses, uint32_t return_address_count,
        uintptr_t image_base) {
        if (!g_replay_active) {
            return local_seed;
        }

        if (g_presentation_rng_desynced) {
            return local_seed;
        }

        if (g_random_seed_record_index >= g_random_seed_records.size()) {
            if (g_active_presentation_kind != 0) {
                SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                    "oracle: unexpected random seed call #%zu during presentation %u in turn %u: local seed=%u; ignoring RNG discrepancies until completion marker",
                    g_random_seed_record_index, g_active_presentation_kind,
                    g_turn, local_seed);
                g_presentation_rng_desynced = true;
                return local_seed;
            }
            SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: unexpected random seed call #%zu in turn %u: local seed=%u; oracle recorded %zu calls",
                g_random_seed_record_index, g_turn, local_seed,
                g_random_seed_records.size());
            Log_Local_Return_Addresses_(return_addresses, return_address_count, image_base);
            Fail_("Oracle random-seed call count mismatch. See the game log.");
            throw platform::process_exit_requested{};
        }

        const s_random_seed_record& expected =
            g_random_seed_records[g_random_seed_record_index++];
        if (local_seed != expected.random_seed) {
            SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION,
                "oracle: substituting random seed call #%zu in turn %u: oracle seed=%u; local seed=%u",
                g_random_seed_record_index - 1, g_turn, expected.random_seed, local_seed);
        }
        return expected.random_seed;
    }

    bool Physical_Input_Enabled_() {
        return !g_enabled;
    }

    bool Complete_Replay_Click_(bool require_blocked_rng) {
        if (!g_replay_active || g_replay_mouse_buttons == 0 || g_record_index == 0 ||
            g_record_index >= g_records.size()) {
            return false;
        }

        const s_input_record& press = g_records[g_record_index - 1];
        if (press.input_type != k_mouse_input_type || press.mouse_buttons == 0) {
            return false;
        }

        size_t release_index = g_record_index;
        for (; release_index < g_records.size(); ++release_index) {
            const s_input_record& record = g_records[release_index];
            if (record.input_type != k_mouse_input_type ||
                record.mouse_x != press.mouse_x || record.mouse_y != press.mouse_y) {
                return false;
            }
            if (record.mouse_buttons == 0) {
                break;
            }
            if (record.mouse_buttons != press.mouse_buttons) {
                return false;
            }
        }
        if (release_index >= g_records.size()) {
            return false;
        }

        const s_input_position& release_position = g_record_positions[release_index];
        if (require_blocked_rng &&
            g_random_record_index >= release_position.random_call_count &&
            g_random_seed_record_index >= release_position.random_seed_count) {
            return false;
        }

        while (g_record_index <= release_index) {
            const s_input_record& record = g_records[g_record_index++];
            mouse::User_Mouse_Handler_(record.mouse_x, record.mouse_y, record.mouse_buttons);
            g_replay_mouse_buttons = record.mouse_buttons;
        }

        const uint64_t now = SDL_GetPerformanceCounter();
        g_last_consumption_counter = now;
        if (g_click_delay_ms != 0) {
            g_click_pause_active = true;
            g_click_pause_until_counter = now + Milliseconds_To_Counters_(g_click_delay_ms);
        }
        return true;
    }

    bool Replay_Active_() {
        return g_replay_active;
    }

    bool Defer_Replay_Click_Completion_(int16_t field_id) {
        if (!g_replay_active || MOX::_current_screen != SCREEN_MAIN ||
            !MOVEBOX::Moveable_Box_Selected_(2)) {
            return false;
        }

        const int16_t first = MOX::_first_fleet_movement_icon;
        int16_t end = first + MOX::_fleet_icon_buttons_added;
        if (end > MOX::_fleet_box.count) {
            end = MOX::_fleet_box.count;
        }
        for (int16_t i = first; i < end; ++i) {
            if (MOX::_fleet_movement_icon[i].field == field_id) {
                return true;
            }
        }
        return false;
    }
}