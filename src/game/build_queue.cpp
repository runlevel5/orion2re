#include "pch.h"

#include "build_queue.h"

namespace BUILD_QUEUE {
namespace {
    constexpr int QUEUE_CAPACITY = 7;

    enum e_mode {
        MODE_APPEND,
        MODE_PREPEND,
        MODE_MERGE,
    };

    bool Is_Mode_(uint8_t instruction) {
        return instruction >= INSTRUCTION_APPEND && instruction <= INSTRUCTION_MERGE;
    }

    bool Is_Product_(uint8_t instruction) {
        return instruction >= INSTRUCTION_HOUSING;
    }

    bool Is_Building_(uint8_t instruction) {
        return instruction > INSTRUCTION_BUILDING_BASE &&
               instruction <= INSTRUCTION_BUILDING_BASE + BUILDING_COUNT - 1;
    }

    int16_t Building_Product_(uint8_t instruction) {
        return static_cast<int16_t>(instruction - INSTRUCTION_BUILDING_BASE);
    }

    int Queue_Length_(const int16_t* queue) {
        for (int index = 0; index < QUEUE_CAPACITY; ++index) {
            if (queue[index] == COLONY_PRODUCTION_NONE) {
                return index;
            }
        }
        return QUEUE_CAPACITY;
    }

    void Defragment_(int16_t* queue) {
        int destination = 0;
        for (int source = 0; source < QUEUE_CAPACITY; ++source) {
            if (queue[source] != COLONY_PRODUCTION_NONE) {
                queue[destination++] = queue[source];
            }
        }
        while (destination < QUEUE_CAPACITY) {
            queue[destination++] = COLONY_PRODUCTION_NONE;
        }
    }

    bool Queue_Contains_(const int16_t* queue, int16_t product) {
        for (int index = 0; index < QUEUE_CAPACITY; ++index) {
            if (queue[index] == product) {
                return true;
            }
        }
        return false;
    }

    void Delete_Ship_Product_(int16_t product) {
        if (COLBLDG::Colony_Production_Is_Queued_Ship_(product)) {
            COLBLDG::Delete_Ship_(COLBLDG::Colony_Production_Ship_Index_(product));
        }
    }

    void Delete_At_(int16_t* queue, int position, bool refits_only) {
        if (position < 0 || position >= QUEUE_CAPACITY) {
            return;
        }
        const int16_t product = queue[position];
        if (product == COLONY_PRODUCTION_NONE) {
            return;
        }
        int end = position + 1;
        if (end < QUEUE_CAPACITY && queue[end] == COLONY_PRODUCTION_REPEAT) {
            ++end;
        }
        if (COLBLDG::Colony_Production_Is_Queued_Ship_(product)) {
            const int ship_idx = COLBLDG::Colony_Production_Ship_Index_(product);
            const bool is_refit = MOX::_ship[ship_idx].status == 4;
            if (is_refit != refits_only) {
                return;
            }
            Delete_Ship_Product_(product);
        } else if (refits_only) {
            return;
        }
        for (int index = position; index < end; ++index) {
            queue[index] = COLONY_PRODUCTION_NONE;
        }
    }

    int First_Unbought_Slot_(const s_colony& colony, const int16_t* queue) {
        if (colony.bought_outright == 0) {
            return 0;
        }
        return queue[1] == COLONY_PRODUCTION_REPEAT ? 2 : 1;
    }

    bool Colony_Base_Available_(int16_t colony_idx, const int16_t* queue) {
        const int16_t star_idx = MOX::_planet[MOX::_colony[colony_idx].planet].star_index;
        int available = 0;
        int queued = 0;
        for (int orbit = 0; orbit < 5; ++orbit) {
            const int16_t planet_idx = MOX::_star[star_idx].planet_index[orbit];
            if (planet_idx == -1 || MOX::_planet[planet_idx].planet_type != PLANET_TYPE_PLANET) {
                continue;
            }
            const int16_t other_idx = MOX::_planet[planet_idx].colony_index;
            if (other_idx == -1 ||
                (MOX::_colony[other_idx].outpost_flag != 0 &&
                 MOX::_colony[other_idx].owner == MOX::_colony[colony_idx].owner)) {
                ++available;
            }
            const int16_t* other_queue = other_idx == colony_idx ? queue :
                (other_idx >= 0 ? MOX::_colony[other_idx].producing : nullptr);
            if (other_queue != nullptr) {
                for (int slot = 0; slot < QUEUE_CAPACITY; ++slot) {
                    queued += other_queue[slot] == BUILDING_COLONY_BASE ? 1 : 0;
                }
            }
        }
        return queued < available;
    }

    bool Repeat_Is_Legal_(const int16_t* queue, int position, int previous_position) {
        if (position == 0 || previous_position < 0) {
            return false;
        }
        const int16_t previous = queue[previous_position];
        if ((COLBLDG::Colony_Production_Is_Building_(previous) &&
             previous != BUILDING_TERRAFORMING && previous != BUILDING_COLONY_BASE) ||
            previous == COLONY_PRODUCTION_HOUSING ||
            previous == COLONY_PRODUCTION_TRADE_GOODS ||
            previous == COLONY_PRODUCTION_NONE ||
            previous == COLONY_PRODUCTION_REPEAT) {
            return false;
        }
        return true;
    }

    int16_t Product_For_Instruction_(int16_t colony_idx, uint8_t instruction,
                                     const int16_t* queue) {
        s_colony& colony = MOX::_colony[colony_idx];
        int16_t product = COLONY_PRODUCTION_NONE;
        if (Is_Building_(instruction)) {
            product = Building_Product_(instruction);
        } else {
            switch (instruction) {
            case INSTRUCTION_HOUSING: product = COLONY_PRODUCTION_HOUSING; break;
            case INSTRUCTION_TRADE_GOODS: product = COLONY_PRODUCTION_TRADE_GOODS; break;
            case INSTRUCTION_FREIGHTER_FLEET: product = COLONY_PRODUCTION_FREIGHTERS; break;
            case INSTRUCTION_SPY: product = COLONY_PRODUCTION_SPY; break;
            case INSTRUCTION_ANDROID_FARMER: product = COLONY_PRODUCTION_FARMER; break;
            case INSTRUCTION_ANDROID_WORKER: product = COLONY_PRODUCTION_WORKER; break;
            case INSTRUCTION_ANDROID_SCIENTIST: product = COLONY_PRODUCTION_SCIENTIST; break;
            case INSTRUCTION_REPEAT: return COLONY_PRODUCTION_REPEAT;
            default: break;
            }
        }
        if (product != COLONY_PRODUCTION_NONE) {
            if (COLCALC::Colony_Can_Build_Product_(&colony, product) != 1) {
                return COLONY_PRODUCTION_NONE;
            }
            if (Colony_Production_Is_Continuous_Project_(product) &&
                Queue_Contains_(queue, product)) {
                return COLONY_PRODUCTION_NONE;
            }
            if (Is_Building_(instruction) && product != BUILDING_COLONY_BASE &&
                Queue_Contains_(queue, product)) {
                return COLONY_PRODUCTION_NONE;
            }
            if (product == BUILDING_COLONY_BASE &&
                !Colony_Base_Available_(colony_idx, queue)) {
                return COLONY_PRODUCTION_NONE;
            }
            return product;
        }

        int design_idx = -1;
        if (instruction >= INSTRUCTION_DESIGN1 && instruction <= INSTRUCTION_DESIGN6) {
            design_idx = instruction - INSTRUCTION_DESIGN1;
        } else if (instruction >= INSTRUCTION_COLONY_SHIP &&
                   instruction <= INSTRUCTION_TRANSPORT) {
            design_idx = 5;
            const int16_t owner = colony.owner;
            if (instruction == INSTRUCTION_COLONY_SHIP) {
                INITSHIP::Load_Colony_Ship_Design_(owner, &MOX::_player[owner].ship_designs[5]);
            } else if (instruction == INSTRUCTION_OUTPOST_SHIP) {
                INITSHIP::Load_Outpost_Ship_Design_(owner, &MOX::_player[owner].ship_designs[5]);
            } else {
                INITSHIP::Load_Transport_Ship_Design_(owner, &MOX::_player[owner].ship_designs[5]);
            }
        }
        if (design_idx < 0) {
            return COLONY_PRODUCTION_NONE;
        }
        const int16_t design_product =
            COLBLDG::Colony_Production_From_Ship_Design_Index_(design_idx);
        if (COLCALC::Colony_Can_Build_Product_(&colony, design_product) != 1) {
            return COLONY_PRODUCTION_NONE;
        }
        const int16_t ship_idx = COLBLDG::Create_Ship_(colony.owner, design_idx, colony_idx);
        if (ship_idx == -1) {
            return COLONY_PRODUCTION_NONE;
        }
        MOX::_ship[ship_idx].status = 6;
        return COLBLDG::Colony_Production_From_Ship_Index_(ship_idx);
    }

    bool Equivalent_(uint8_t instruction, int16_t product) {
        if (Is_Building_(instruction)) {
            return product == Building_Product_(instruction);
        }
        switch (instruction) {
        case INSTRUCTION_HOUSING: return product == COLONY_PRODUCTION_HOUSING;
        case INSTRUCTION_TRADE_GOODS: return product == COLONY_PRODUCTION_TRADE_GOODS;
        case INSTRUCTION_FREIGHTER_FLEET: return product == COLONY_PRODUCTION_FREIGHTERS;
        case INSTRUCTION_SPY: return product == COLONY_PRODUCTION_SPY;
        case INSTRUCTION_ANDROID_FARMER: return product == COLONY_PRODUCTION_FARMER;
        case INSTRUCTION_ANDROID_WORKER: return product == COLONY_PRODUCTION_WORKER;
        case INSTRUCTION_ANDROID_SCIENTIST: return product == COLONY_PRODUCTION_SCIENTIST;
        case INSTRUCTION_REPEAT: return product == COLONY_PRODUCTION_REPEAT;
        default: break;
        }
        if (!COLBLDG::Colony_Production_Is_Queued_Ship_(product)) {
            return false;
        }
        const s_ship_design& design =
            MOX::_ship[COLBLDG::Colony_Production_Ship_Index_(product)].d;
        if (instruction == INSTRUCTION_COLONY_SHIP) return design.ship_type == SHIP_TYPE_COLONY;
        if (instruction == INSTRUCTION_OUTPOST_SHIP) return design.ship_type == SHIP_TYPE_OUTPOST;
        if (instruction == INSTRUCTION_TRANSPORT) return design.ship_type == SHIP_TYPE_TRANSPORT;
        return instruction >= INSTRUCTION_DESIGN1 && instruction <= INSTRUCTION_DESIGN6 &&
               design.ship_type == SHIP_TYPE_COMBAT &&
               MOX::_ship[COLBLDG::Colony_Production_Ship_Index_(product)].status == 6;
    }

    bool Insert_At_(int16_t colony_idx, int16_t* queue, uint8_t instruction,
                    int position, int previous_position) {
        if (position < 0 || position >= QUEUE_CAPACITY ||
            queue[position] != COLONY_PRODUCTION_NONE) {
            return false;
        }
        if (instruction == INSTRUCTION_REPEAT) {
            if (!Repeat_Is_Legal_(queue, position, previous_position)) {
                return false;
            }
            queue[position] = COLONY_PRODUCTION_REPEAT;
            return true;
        }
        const int16_t product = Product_For_Instruction_(colony_idx, instruction, queue);
        if (product == COLONY_PRODUCTION_NONE) {
            return false;
        }
        queue[position] = product;
        return true;
    }

    void Rotate_To_(int16_t* queue, int first, int last) {
        const int16_t product = queue[last];
        for (int index = last; index > first; --index) {
            queue[index] = queue[index - 1];
        }
        queue[first] = product;
    }

    bool Future_List_Contains_(const uint8_t* list, int start, int16_t product) {
        for (int index = start; index < INSTRUCTION_CAPACITY; ++index) {
            const uint8_t instruction = list[index];
            if (instruction == INSTRUCTION_END || instruction == INSTRUCTION_STOP ||
                Is_Mode_(instruction)) {
                return false;
            }
            if (Is_Product_(instruction) && Equivalent_(instruction, product)) {
                return true;
            }
        }
        return false;
    }

    void Start_Merge_(const s_colony& colony, int16_t* queue, const uint8_t* list,
                      int next_instruction, bool* merge_cells) {
        int cells = 0;
        int empty = 0;
        const int first = First_Unbought_Slot_(colony, queue);
        for (int position = first; position < QUEUE_CAPACITY; ++position) {
            const int16_t product = queue[position];
            if (product == COLONY_PRODUCTION_NONE) {
                ++empty;
            } else if (!Future_List_Contains_(list, next_instruction, product)) {
                continue;
            }
            merge_cells[position] = true;
            ++cells;
            if (position + 1 < QUEUE_CAPACITY &&
                queue[position + 1] == COLONY_PRODUCTION_REPEAT) {
                merge_cells[++position] = true;
                ++cells;
            }
        }
        if (cells != empty) {
            return;
        }
        const int products = QUEUE_CAPACITY - empty;
        if (products <= first) {
            return;
        }
        memset(merge_cells, 0, sizeof(bool) * QUEUE_CAPACITY);
        for (int position = products - 1; position >= first; --position) {
            queue[position + empty] = queue[position];
        }
        for (int position = first; position < first + empty; ++position) {
            queue[position] = COLONY_PRODUCTION_NONE;
            merge_cells[position] = true;
        }
    }

    void Unbuy_(s_colony& colony) {
        if (colony.bought_outright == 0) {
            return;
        }
        const int32_t refund = COLCALC::Colony_Cost_To_Buy_Given_Product_(
            &colony, colony.producing[0], colony.production_spent);
        int32_t& bc = MOX::_player[colony.owner].bc;
        if (refund >= 0 && bc <= INT32_MAX - refund) {
            bc += refund;
            colony.bought_outright = 0;
        }
    }

    void Buy_(int16_t colony_idx, uint8_t instruction) {
        s_colony& colony = MOX::_colony[colony_idx];
        if (colony.bought_outright != 0 || COLCALC::Colony_Is_Blockaded(&colony)) {
            return;
        }
        const int16_t product = colony.producing[0];
        if (product == COLONY_PRODUCTION_NONE || product == COLONY_PRODUCTION_HOUSING ||
            product == COLONY_PRODUCTION_TRADE_GOODS || product == COLONY_PRODUCTION_REPEAT) {
            return;
        }
        const int64_t total = COLCALC::Colony_Product_Cost_(&colony, product);
        const int64_t spent = colony.production_spent;
        if (total <= spent ||
            (instruction == INSTRUCTION_BUY10 && spent * 10 < total) ||
            (instruction == INSTRUCTION_BUY50 && spent * 2 < total) ||
            (instruction == INSTRUCTION_BUY75 && spent * 4 < total * 3)) {
            return;
        }
        const int32_t cost = COLCALC::Colony_Cost_To_Buy_Producing_Product_(&colony);
        if (cost >= 0 && cost <= MOX::_player[colony.owner].bc) {
            COLCALC::Colony_Buy_Outright_(colony_idx);
        }
    }

    bool Apply_(int16_t colony_idx, const uint8_t* list) {
        if (colony_idx < 0 || colony_idx >= MOX::_NUM_COLONIES || list == nullptr) {
            return false;
        }
        s_colony& colony = MOX::_colony[colony_idx];
        int16_t queue[QUEUE_CAPACITY];
        memcpy(queue, colony.producing, sizeof(queue));
        e_mode mode = MODE_APPEND;
        bool merge_cells[QUEUE_CAPACITY] = {};
        int insertion_position = 0;

        for (int index = 0; index < INSTRUCTION_CAPACITY; ++index) {
            const uint8_t instruction = list[index];
            if (instruction == INSTRUCTION_END || instruction == INSTRUCTION_STOP) {
                break;
            }
            if (Is_Mode_(instruction)) {
                Defragment_(queue);
                memset(merge_cells, 0, sizeof(merge_cells));
                insertion_position = 0;
                mode = instruction == INSTRUCTION_APPEND ? MODE_APPEND :
                       instruction == INSTRUCTION_PREPEND ? MODE_PREPEND : MODE_MERGE;
                if (mode == MODE_MERGE) {
                    Start_Merge_(colony, queue, list, index + 1, merge_cells);
                }
                continue;
            }
            if (instruction == INSTRUCTION_CLEAR || instruction == INSTRUCTION_CLEAR_REFITS) {
                const bool refits = instruction == INSTRUCTION_CLEAR_REFITS;
                const int first = First_Unbought_Slot_(colony, queue);
                for (int position = first; position < QUEUE_CAPACITY; ++position) {
                    if (mode != MODE_MERGE || merge_cells[position]) {
                        Delete_At_(queue, position, refits);
                    }
                }
                if (instruction == INSTRUCTION_CLEAR && mode != MODE_MERGE) {
                    Defragment_(queue);
                }
                insertion_position = 0;
                continue;
            }
            if (instruction == INSTRUCTION_CLEAR_CONTINUOUS) {
                const int first = First_Unbought_Slot_(colony, queue);
                for (int position = first; position < QUEUE_CAPACITY; ++position) {
                    if (Colony_Production_Is_Continuous_Project_(queue[position])) {
                        Delete_At_(queue, position, false);
                    }
                }
                Defragment_(queue);
                memset(merge_cells, 0, sizeof(merge_cells));
                insertion_position = 0;
                if (mode == MODE_MERGE) {
                    Start_Merge_(colony, queue, list, index + 1, merge_cells);
                }
                continue;
            }
            if (instruction >= INSTRUCTION_BUY && instruction <= INSTRUCTION_BUY75) {
                if (mode == MODE_MERGE && !merge_cells[0]) {
                    continue;
                }
                memcpy(colony.producing, queue, sizeof(queue));
                Buy_(colony_idx, instruction);
                if (colony.bought_outright != 0) {
                    merge_cells[0] = false;
                }
                continue;
            }
            if (instruction == INSTRUCTION_UNBUY) {
                memcpy(colony.producing, queue, sizeof(queue));
                Unbuy_(colony);
                continue;
            }
            if (!Is_Product_(instruction)) {
                continue;
            }

            if (mode == MODE_APPEND) {
                const int position = Queue_Length_(queue);
                if (position < QUEUE_CAPACITY) {
                    Insert_At_(colony_idx, queue, instruction, position, position - 1);
                }
            } else if (mode == MODE_PREPEND) {
                const int empty = Queue_Length_(queue);
                if (empty >= QUEUE_CAPACITY) {
                    continue;
                }
                if (insertion_position == 0 && colony.bought_outright != 0) {
                    insertion_position = First_Unbought_Slot_(colony, queue);
                }
                if (Insert_At_(colony_idx, queue, instruction, empty,
                               insertion_position - 1)) {
                    Rotate_To_(queue, insertion_position, empty);
                    ++insertion_position;
                }
            } else {
                if ((Is_Building_(instruction) &&
                     Building_Product_(instruction) != BUILDING_COLONY_BASE) ||
                    instruction == INSTRUCTION_HOUSING ||
                    instruction == INSTRUCTION_TRADE_GOODS) {
                    for (int position = insertion_position;
                         position < QUEUE_CAPACITY; ++position) {
                        if (merge_cells[position] && Equivalent_(instruction, queue[position])) {
                            Delete_At_(queue, position, false);
                        }
                    }
                }
                while (insertion_position < QUEUE_CAPACITY &&
                       !merge_cells[insertion_position]) {
                    ++insertion_position;
                }
                if (insertion_position < QUEUE_CAPACITY) {
                    Delete_At_(queue, insertion_position, false);
                }
                if (insertion_position < QUEUE_CAPACITY &&
                    Insert_At_(colony_idx, queue, instruction, insertion_position,
                               insertion_position - 1)) {
                    ++insertion_position;
                }
            }
        }

        Defragment_(queue);
        const int16_t original_front = colony.producing[0];
        memcpy(colony.producing, queue, sizeof(queue));
        if (original_front != colony.producing[0]) {
            colony.bought_outright = 0;
        }
        COLCALC::Colony_Calculation_(colony_idx);
        return true;
    }
}

    bool Colony_Production_Is_Continuous_Project_(int16_t product) {
        return product == COLONY_PRODUCTION_HOUSING ||
               product == COLONY_PRODUCTION_TRADE_GOODS;
    }

    int Parse_Autobuild_Preset_(const char* text) {
        if (text == nullptr || text[0] == '\0') {
            return -1;
        }
        int preset = 0;
        for (const char* cursor = text; *cursor != '\0'; ++cursor) {
            if (*cursor < '0' || *cursor > '9') {
                return -2;
            }
            preset = preset * 10 + (*cursor - '0');
            if (preset >= PRESET_COUNT) {
                return -2;
            }
        }
        return preset;
    }

    bool Apply_Preset_To_Colony_(int16_t colony_idx, int preset_idx) {
        return preset_idx >= 0 && preset_idx < PRESET_COUNT &&
               Apply_(colony_idx, presets[preset_idx]);
    }

    void Apply_Preset_To_Empire_(int16_t player_idx, int preset_idx) {
        if (preset_idx < 0 || preset_idx >= PRESET_COUNT) {
            return;
        }
        for (int16_t colony_idx = 0; colony_idx < MOX::_NUM_COLONIES; ++colony_idx) {
            s_colony& colony = MOX::_colony[colony_idx];
            if (colony.owner == player_idx && colony.outpost_flag == 0) {
                Apply_(colony_idx, presets[preset_idx]);
            }
        }
        COLCALC::Do_Player_Colony_Calculations_(player_idx);
    }

    bool Apply_Custom_Autobuild_(int16_t colony_idx) {
        if (autobuild_settings.enabled == 0 ||
            colony_idx < 0 || colony_idx >= MOX::_NUM_COLONIES) {
            return false;
        }
        const int preset_idx = Decode_Autobuild_Preset_(
            MOX::_colony[colony_idx].auto_building);
        return preset_idx >= 0 && Apply_Preset_To_Colony_(colony_idx, preset_idx);
    }
}