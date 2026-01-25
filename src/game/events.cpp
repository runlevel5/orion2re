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
}