#include "deps.h"

namespace COLDRAW {

void __cdecl Draw_Colony_Prod_(int16_t colony_handle, int32_t selection, int32_t p3, int16_t y_offset, int32_t p5, int32_t p6) {
    Draw_Colony_Prod_Both_(colony_handle, (int16_t)selection, (int16_t)p3, y_offset, (int16_t)p5, (int16_t)p6, 0);
}

}