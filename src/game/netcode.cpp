#include "pch.h"

namespace netcode {
    uint32_t _netInitialized;
    int8_t _tpSending;
    uint8_t _inComms;
    recv_fn _recv_fn;
    int32_t _connection_type;
    int32_t _in_send_message;

    int32_t __cdecl Do_Net_Init_(uint32_t mode, GUID guid, recv_fn recv_fn_ptr, s_network* network) {
        RUSS::Mox_Update_();
        int32_t result = Net_Init_(mode, guid, recv_fn_ptr, network);
        RUSS::Mox_Update_();
        return result;
    }

    int32_t __cdecl Net_Init_(uint32_t mode, GUID guid, recv_fn recv_fn_ptr, s_network* network) {
        int32_t result = 0;
        _connection_type = mode;

        if (mode == 0) {
            time_::_time = time_::time_;
            time_::_time2 = time_::time_;

            // TCP - original wants IPX
            result = WINNET::Init_Connection_(2);
            WINNET::Refresh_Game_List_(guid);

            _recv_fn = recv_fn_ptr;
        }

        if (mode == 1 || mode == 2) {
            result = simmodem::Init_Modem_(network, recv_fn_ptr);
        }

        if (mode == 3) {
            _connection_type = -1;
            return 0;
        }

        return result;
    }

    int32_t __cdecl Net_Get_Game_Count_() {
        return WINNET::_session_count;
    }

    int32_t __cdecl Get_Player_Count_() {
        return WINNET::_player_count;
    }

    char* __cdecl Net_Get_Game_Name_(uint32_t session_idx) {
        return WINNET::Get_Game_Name_(session_idx);
    }

    void __cdecl Net_Uninit_()
    {
        if (_connection_type == 0) {
            WINNET::Shutdown_();
        }

        if (_connection_type == 1 || _connection_type == 2) {
            simmodem::Uninit_Modem_();
        }
    }

    int32_t __cdecl Net_Is_Sending_() {
        return (int32_t)netcode::_tpSending;
    }

    int32_t __cdecl Net_Join_Game_(const char* game_name, const char* password, const char* player_name) {
        if (!WINNET::Join_Game_(game_name, password, player_name)) {
            return 0;
        }

        comcore::ComInit_(0x1c2, const_cast<char*>(game_name), _recv_fn);

        _netInitialized = 1;
        return 1;
    }

    void __cdecl Set_Null_Modem_Slave_() {
        net::_thisHost = 2;
        simmodem::_connected_to_id = 1;
    }

    void __cdecl Set_Null_Modem_Host_() {
        net::_thisHost = 1;
        simmodem::_connected_to_id = 2;
    }

    int32_t __cdecl Net_Get_Users_(uint8_t* users) {
        if (_connection_type == 3) {
            return ten::TEN_Get_Users_(users);
        }

        if (_connection_type == 1 || _connection_type == 2) {
            if (users != nullptr) {
                users[0] = (uint8_t)net::_thisHost;
                users[1] = simmodem::_connected_to_id;
            }
            return 2;
        }

        if (_connection_type == 0) {
            int32_t player_count = Get_Player_Count_();
            if (users != nullptr && player_count > 0) {
                for (int32_t i = 0; i < player_count; ++i) {
                    users[i] = WINNET::Get_Player_Network_Id_(i);
                }
            }
            return player_count;
        }

        return 0;
    }

    void __cdecl Net_Disable_New_Players_() {
        WINNET::Net_Disable_New_Players_();
    }

    uint32_t __cdecl Net_Host_ID_() {
        if (_connection_type == 3) {
            return ten::TEN_Get_Host_Id_();
        }
        return net::_thisHost;
    }

    void __cdecl Net_Update_() {
        if (_connection_type == 3) {
            ten::Ten_Update_();
        }

        if (_connection_type == 0 && _inComms == 0) {
            _inComms = 1;
            if (_netInitialized != 0) {
                comcore::ComDoComms_();
            }
            _inComms = 0;
        }

        if (_connection_type == 1 || _connection_type == 2) {
            motrans::Modem_Update_();
        }
    }

    uint32_t __cdecl Net_Send_Message_(int32_t target_id, int16_t msg_type, void* packet, uint32_t size) {
        uint8_t* data = (uint8_t*)packet;

        if (_in_send_message == 0) {
            _in_send_message = 1;
        } else {
            ::OutputDebugStringA("REENTERING (Net_Send_Message)!!!!");
        }

        if (_connection_type == 3) {
            ten::Send_Message_(target_id, msg_type, data, size);
        }

        if (_connection_type == 0) {
            while (transprt::_tpSending != 0) {
                Net_Update_();
            }

            comcore::ComSendUserMessage_(data, size, target_id, 0x65, 2, msg_type);

            clock_t start_time = clock();
            while (transprt::_tpSending != 0) {
                if ((uint32_t)(clock() - start_time) >= 120000) {
                    exitm::Exit_With_Message("Lost Player Connection...");
                }
                Net_Update_();
            }
        }

        if (_connection_type == 1 || _connection_type == 2) {
            motrans::Modem_Send_Message_(target_id, msg_type, data, size);
        }

        _in_send_message = 0;
        return Net_Status_() & 4;
    }

    uint32_t __cdecl Net_Status_() {
        uint32_t status = 0;

        if (_connection_type == 0) {
            if (transprt::_tpSending != 0) {
                status = 1;
            }
            if (transprt::_tpRecNum != 0 && transprt::_tpRecSrc != 0) {
                status |= 2;
            }
            if (transprt::_tpLastSendSuccessful != 0) {
                status |= 4;
            }
        }

        if (_connection_type == 3) {
            status |= 4;
        }

        if (_connection_type == 1 || _connection_type == 2) {
            return motrans::Modem_Status_();
        }

        return status;
    }

    int32_t __cdecl Net_Create_Game_(GUID guid, const char* name, int32_t max_players, const char* password, const char* player_name) {
        if (!WINNET::Create_Game_(guid, name, max_players, password, player_name)) {
            return 0;
        }

        comcore::ComInit_(0x1c2, const_cast<char*>(name), _recv_fn);

        _netInitialized = 1;
        return 1;
    }
}