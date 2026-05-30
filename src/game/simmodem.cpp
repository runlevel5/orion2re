#include "pch.h"

#include "simmodem.h"

namespace simmodem {

    uint8_t* _open_game_port;
    uint8_t _modemInitialized;
    int8_t _connected_to_id;
    recv_fn _modemUserRecFunc;

}