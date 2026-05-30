#include "pch.h"

namespace netcode {
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

            result = WINNET::Init_Connection_(1);
            WINNET::Refresh_Game_List_(guid);

            _recv_fn = recv_fn_ptr;
        }

        if (mode == 1 || mode == 2) {
            result = simmodem::Init_Modem_(network, recv_fn_ptr);
        }

        if (mode == 3) {
            result = 1;
            ten::Init_TEN_(mode, &guid, recv_fn_ptr, network);
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
        if (session_idx >= (uint32_t)WINNET::_session_count) {
            return nullptr;
        }

        if (_dplay == nullptr) {
            return nullptr;
        }

        if (&WINNET::_sessions[session_idx] != nullptr) {
            return WINNET::_sessions[session_idx]->szSessionName;
        }

        return nullptr;
    }

    void __cdecl Net_Uninit_()
    {
        if (_connection_type == 0) {
            if (_dplay != nullptr) {
                _dplay->Close();
                _dplay->Release();
                _dplay = nullptr;
            }

            if (WINNET::_session_count != 0) {
                for (uint32_t i = 0; i < WINNET::_session_count; ++i) {
                    if (WINNET::_sessions[i] != nullptr) {
                        ::_free(WINNET::_sessions[i]);
                        WINNET::_sessions[i] = nullptr;
                    }
                }
            }
            WINNET::_session_count = 0;
        }

        if (_connection_type == 1 || _connection_type == 2) {
            simmodem::Uninit_Modem_();
        }
    }

    int32_t __cdecl Net_Is_Sending_() {
        return (int32_t)netcode::_isSending;
    }

    int32_t __cdecl Net_Join_Game_(char* game_name, char* password, char* player_name) {
        uint32_t found_idx = 0xffffffff;

        if (WINNET::_session_count != 0) {
            for (uint32_t i = 0; i < WINNET::_session_count; ++i) {
                char* name = Net_Get_Game_Name_(i);
                if (strcmp(game_name, name) == 0) {
                    found_idx = i;
                    break;
                }
            }
        }

        if (found_idx == 0xffffffff) {
            return 0;
        }

        if (found_idx >= WINNET::_session_count) {
            return 0;
        }

        DPSESSIONDESC* session = WINNET::_sessions[found_idx];
        WINNET::Refresh_Player_List_(session->dwSession);

        DPSESSIONDESC session_desc;
        memset(&session_desc, 0, sizeof(session_desc));

        session_desc.dwSize = sizeof(DPSESSIONDESC); // 0x7c
        session_desc.guidSession = session->guidSession;
        session_desc.dwFlags = 1 /* DPOPEN_JOIN */;
        session_desc.dwSession = session->dwSession;

        if (password != nullptr) {
            strcpy(session_desc.szPassword, password);
        }

        HRESULT hr = _dplay->Open(&session_desc);
        if (hr != 0) {
            _dplay->Release();
            _dplay = nullptr;
            return 0;
        }

        DPID player_id;
        WINNET::Create_Player_(&player_id, player_name, s__00551100);
        WINNET::_local_player_id = player_id;

        comcore::ComInit_(0x1c2, session->szSessionName, _recv_fn);

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
                users[0] = static_cast<uint8_t>(net::_thisHost);
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
        if (_dplay != nullptr) {
            _dplay->EnableNewPlayers(0);
        }
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
            ::OutputDebugStringA(s_REENTERING_Net_Send_Message_0055bf58);
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
                    exitm::Exit_With_Message(s_Lost_Player_Connection_0055bf3c);
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

    int32_t __cdecl Net_Create_Game_(GUID guid, char* name, int32_t max_players, char* password, char* player_name) {
        DPSESSIONDESC local_7c;
        DPID dpid;

        memset(&local_7c, 0, sizeof(DPSESSIONDESC));
        local_7c.dwSize = sizeof(DPSESSIONDESC);
        local_7c.guidSession = guid;
        local_7c.dwFlags = 2;
        local_7c.dwMaxPlayers = max_players;

        strcpy(local_7c.szSessionName, name);

        if (password != nullptr) {
            strcpy(local_7c.szPassword, password);
        }

        HRESULT hr = _dplay->Open(&local_7c);
        if (hr != 0) {
            _dplay->Release();
            _dplay = nullptr;
            return 0;
        }

        DPSESSIONDESC* new_session = reinterpret_cast<DPSESSIONDESC*>(::_malloc(sizeof(DPSESSIONDESC)));
        WINNET::_sessions[WINNET::_session_count] = new_session;
        memcpy(new_session, &local_7c, sizeof(DPSESSIONDESC));
        WINNET::_session_count++;

        WINNET::Net_Enable_New_Players();

        WINNET::Create_Player_(&dpid, player_name, s__00551100);
        WINNET::_local_player_id = dpid;

        comcore::ComInit_(0x1c2, name, _recv_fn);

        _netInitialized = 1;
        return 1;
    }
}