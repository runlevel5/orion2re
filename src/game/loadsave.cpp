#include "deps.h"

namespace LOADSAVE {
    /**
     * @address 0050fa10
     */
    void __cdecl Load_Month_Names_() {
        auto copy_message = [](int32_t msg_id, char* dest) {
            const char* src = HAROLD::H_Message_((int16_t)msg_id);
            if (src) {
                const char* s = src;
                char* d = dest;
                while ((*d++ = *s++) != '\0');
            }
        };

        copy_message(0xBE, _months[0]);  // January
        copy_message(0xBF, _months[1]);  // February
        copy_message(0xC0, _months[2]);  // March
        copy_message(0xC1, _months[3]);  // April
        copy_message(0xC2, _months[4]);  // May
        copy_message(0xC3, _months[5]);  // June
        copy_message(0xC4, _months[6]);  // July
        copy_message(0xC5, _months[7]);  // August
        copy_message(0xC6, _months[8]);  // September
        copy_message(0xC7, _months[9]);  // October
        copy_message(0xC8, _months[10]); // November
        copy_message(0xC9, _months[11]); // December
    }
}