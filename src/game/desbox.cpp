#include "pch.h"

namespace DESBOX {

uint32_t __cdecl Application_Type_(int16_t app_id) {
    // Accesses the 'type' field (offset 12) of the app_id-th element.
    return TECHDATA::_technology_applications[app_id].type;
}

}