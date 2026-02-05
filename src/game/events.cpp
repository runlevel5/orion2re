#include "deps.h"
#include "events.h"

namespace EVENTS {
    /**
     * @address 00473df0
     */
    void Init_Events_() {
        _event_delay_count = 0;
        _last_event_year = MOX::_stardate;

        for (int i = 0; i < 36; ++i) {
            _event_data[i].field0_0x0 = -1;
            _event_data[i].field1_0x1 = 0;
            _event_data[i].field2_0x2 = 0;
            _event_data[i].event_type = -1;
            _event_data[i].field4_0x5 = -1;
            _event_data[i].field5_0x7 = -1;
        }
    }

    /**
     * @address 00477630
     */
    int16_t __cdecl Event_Check_Space_Anomaly_(int16_t colony_idx) {
        auto& event = _event_data[25];

        if (event.field1_0x1 == 2 || event.field1_0x1 == 4 || event.field1_0x1 == 6) {
            int16_t planet_idx = MOX::_colony[colony_idx].planet;
            if ((int16_t)MOX::_planet[planet_idx].star_index == event.event_type) {
                return 1;
            }
        }

        return 0;
    }

    /**
     * @address 004784b0
     */
    int16_t __cdecl Event_Check_Space_Anomaly_At_Star_(int16_t star_idx) {
        auto& event = _event_data[25];

        if ((event.field1_0x1 == 2 || event.field1_0x1 == 4 || event.field1_0x1 == 6) &&
            (event.event_type == star_idx)) {
            return 1;
        }

        return 0;
    }

    /**
     * @address 00477690
     */
    int16_t __cdecl Event_Check_Pirate_Activity_(uint8_t player_idx) {
        uint8_t event_status = _event_data[14].field1_0x1;

        if (event_status == 2 || event_status == 4 || event_status == 6) {
            uint16_t star_idx = (uint16_t)_event_data[14].event_type;
            s_star_data* star = &MOX::_star[star_idx];

            if ((star->has_colony >> (player_idx & 0x1F)) & 1) {
                return 1;
            }
        }

        return 0;
    }

    /**
     * @address 00478110
     */
    int16_t __cdecl Event_Check_Colony_Researching_(int16_t colony_idx) {
        if ((_event_data[16].field1_0x1 == 2 || _event_data[16].field1_0x1 == 6) &&
            (_event_data[16].event_type == colony_idx)) {
            return 1;
        }

        if ((_event_data[24].field1_0x1 == 2 || _event_data[24].field1_0x1 == 6)) {
            int16_t planet_idx = MOX::_colony[colony_idx].planet;
            if (MOX::_planet[planet_idx].star_index == _event_data[24].event_type) {
                return 1;
            }
        }

        return 0;
    }

    /**
     * @address 00477720
     */
    int16_t __cdecl Event_Check_Population_Boom_(int16_t colony_idx) {
        int8_t status = _event_data[17].field1_0x1;

        if ((status == 2 || status == 4 || status == 6) && (_event_data[17].event_type == colony_idx)) {
            return 1;
        }

        return 0;
    }

    /**
     * @address 004776d0
     */
    int16_t __cdecl Event_Check_Plague_(int16_t colony_idx) {
        // _event_data[16] is specifically checked for plague-related flags
        const auto& event = _event_data[16];

        if ((event.field1_0x1 == 2 || event.field1_0x1 == 4 || event.field1_0x1 == 6) &&
            event.event_type == colony_idx) {
            return 1;
        }

        return 0;
    }

    /**
     * @address 00477610
     */
    int16_t __cdecl Event_Check_Hyperspace_Flux_() {
        int8_t state = _event_data[9].field1_0x1;

        if (state == 2 || state == 4 || state == 6) {
            return 1;
        }

        return 0;
    }
}