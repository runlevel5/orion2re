#include "pch.h"

namespace netcode {
    uint32_t _netInitialized;
    int8_t _tpSending;
    uint8_t _inComms;
    recv_fn _recv_fn;
    int32_t _connection_type;
    int32_t _in_send_message;

    namespace {
        netadapter::s_adapter _router_adapter;
        int32_t _router_last_send_successful;

        void __cdecl Pump_Router_Packets_() {
            netadapter::Pump(&_router_adapter);

            netadapter::s_game_packet packet;
            while (netadapter::Poll_Game_Packet(&_router_adapter, &packet)) {
                if (_recv_fn != nullptr) {
                    _recv_fn(
                        (int16_t)packet.sender_network_id,
                        packet.message_type,
                        (uint32_t)packet.payload.size(),
                        packet.payload.empty() ? nullptr : packet.payload.data());
                }
            }

            netadapter::s_event event;
            while (netadapter::Poll_Event(&_router_adapter, &event)) {
                if (event.kind == netadapter::EVENT_MEMBER_LEFT) {
                    SDL_Log("Router member left: %u", (unsigned)event.network_id);
                } else if (event.kind == netadapter::EVENT_SESSION_CLOSED) {
                    SDL_Log("Router session closed");
                } else if (event.kind == netadapter::EVENT_DISCONNECTED) {
                    SDL_Log("Router disconnected");
                } else if (event.kind == netadapter::EVENT_ERROR) {
                    SDL_Log("Router error: %s", event.error.message);
                }
            }
        }

        void __cdecl Pump_Router_Until_(bool (*predicate)()) {
            clock_t start_time = clock();
            while (!predicate()) {
                Pump_Router_Packets_();
                if ((uint32_t)(clock() - start_time) >= 120000) {
                    break;
                }
                SDL_Delay(1);
            }
        }

        bool __cdecl Router_Local_Id_Ready_() {
            return netadapter::Local_Network_Id(&_router_adapter) != 0;
        }

        bool __cdecl Router_Session_List_Ready_() {
            return netadapter::Game_Count(&_router_adapter) != 0 || !netadapter::Has_Pending_Client_Events(&_router_adapter);
        }

        bool __cdecl Router_Session_Ready_() {
            return netadapter::Session_Id(&_router_adapter) != 0;
        }

        int32_t __cdecl Find_Router_Game_By_Name_(const char* game_name) {
            int32_t game_count = netadapter::Game_Count(&_router_adapter);
            for (int32_t i = 0; i < game_count; ++i) {
                const char* candidate = netadapter::Game_Name(&_router_adapter, (uint32_t)i);
                if (candidate != nullptr && game_name != nullptr && strcmp(candidate, game_name) == 0) {
                    return i;
                }
            }
            return -1;
        }

        void __cdecl Init_Router_Config_(netadapter::s_config* config, const char* player_name, s_network* network) {
            *config = netadapter::s_config{};
            if (network != nullptr && network->modem_settings.cmd_reset[0] != '\0') {
                network::router::Router_Copy_String(config->host, sizeof(config->host), network->modem_settings.cmd_reset);
            }
            if (player_name != nullptr && player_name[0] != '\0') {
                network::router::Router_Copy_String(config->client_name, sizeof(config->client_name), player_name);
            }
            network::router::Router_Copy_String(config->build_string, sizeof(config->build_string), GAME_BUILD_DATE);
        }
    }

    int32_t __cdecl Do_Net_Init_(uint32_t mode, recv_fn recv_fn_ptr, s_network* network) {
        RUSS::Mox_Update_();
        int32_t result = Net_Init_(mode, recv_fn_ptr, network);
        RUSS::Mox_Update_();
        return result;
    }

    int32_t __cdecl Net_Init_(uint32_t mode, recv_fn recv_fn_ptr, s_network* network) {
        int32_t result = 0;
        _connection_type = mode;

        if (mode == 0) {
            time_::_time = time_::time_;
            time_::_time2 = time_::time_;

            netadapter::s_config config;
            Init_Router_Config_(&config, nullptr, network);
            netadapter::Init(&_router_adapter, &config);
            result = netadapter::Connect(&_router_adapter) ? 1 : 0;
            if (result != 0) {
                Pump_Router_Until_(Router_Local_Id_Ready_);
                result = Router_Local_Id_Ready_() ? 1 : 0;
            }
            if (result != 0) {
                netadapter::Refresh_Game_List(&_router_adapter);
                Pump_Router_Packets_();
            }

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
        if (_connection_type == 0) {
            Pump_Router_Packets_();
            return netadapter::Game_Count(&_router_adapter);
        }
        return 0;
    }

    int32_t __cdecl Get_Player_Count_() {
        if (_connection_type == 0) {
            Pump_Router_Packets_();
            return netadapter::Get_Users(&_router_adapter, nullptr, 0);
        }
        return WINNET::_player_count;
    }

    char* __cdecl Net_Get_Game_Name_(uint32_t session_idx) {
        if (_connection_type == 0) {
            Pump_Router_Packets_();
            return const_cast<char*>(netadapter::Game_Name(&_router_adapter, session_idx));
        }
        return WINNET::Get_Game_Name_(session_idx);
    }

    void __cdecl Net_Uninit_()
    {
        if (_connection_type == 0) {
            netadapter::Shutdown(&_router_adapter);
        }

        if (_connection_type == 1 || _connection_type == 2) {
            simmodem::Uninit_Modem_();
        }
    }

    int32_t __cdecl Net_Is_Sending_() {
        return (int32_t)netcode::_tpSending;
    }

    int32_t __cdecl Net_Join_Game_(const char* game_name, const char* password, const char* player_name) {
        (void)password;
        int32_t session_index = Find_Router_Game_By_Name_(game_name);
        if (session_index < 0) {
            netadapter::Refresh_Game_List(&_router_adapter);
            Pump_Router_Until_(Router_Session_List_Ready_);
            session_index = Find_Router_Game_By_Name_(game_name);
        }
        if (session_index < 0 || !netadapter::Join_Game(&_router_adapter, (uint32_t)session_index, player_name)) {
            return 0;
        }

        Pump_Router_Until_(Router_Session_Ready_);
        if (!Router_Session_Ready_()) {
            return 0;
        }

        comcore::ComInit_(0x1c2, const_cast<char*>(game_name), _recv_fn);
        net::_thisHost = netadapter::Local_Network_Id(&_router_adapter);

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

    int32_t __cdecl Net_Get_Users_(int16_t* users) {
        if (_connection_type == 3) {
            return ten::TEN_Get_Users_(users);
        }

        if (_connection_type == 1 || _connection_type == 2) {
            if (users != nullptr) {
                users[0] = (int16_t)net::_thisHost;
                users[1] = simmodem::_connected_to_id;
            }
            return 2;
        }

        if (_connection_type == 0) {
            Pump_Router_Packets_();
            uint32_t router_users[256]{};
            int32_t player_count = netadapter::Get_Users(&_router_adapter, router_users, 256);
            if (users != nullptr && player_count > 0) {
                for (int32_t i = 0; i < player_count; ++i) {
                    users[i] = (int16_t)router_users[i];
                }
            }
            return player_count;
        }

        return 0;
    }

    void __cdecl Net_Disable_New_Players_() {
        if (_connection_type == 0) {
            netadapter::Set_New_Players_Enabled(&_router_adapter, false);
            Pump_Router_Packets_();
            return;
        }

        WINNET::Net_Disable_New_Players_();
    }

    uint32_t __cdecl Net_Host_ID_() {
        if (_connection_type == 3) {
            return ten::TEN_Get_Host_Id_();
        }
        if (_connection_type == 0) {
            return netadapter::Local_Network_Id(&_router_adapter);
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
                Pump_Router_Packets_();
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
            SDL_Log("REENTERING (Net_Send_Message)!!!!");
        }

        if (_connection_type == 3) {
            ten::Send_Message_(target_id, msg_type, data, size);
        }

        if (_connection_type == 0) {
            _router_last_send_successful = netadapter::Send_Game_Message(&_router_adapter, (uint32_t)target_id, (uint16_t)msg_type, data, size) ? 1 : 0;
            Pump_Router_Packets_();
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
            if (_router_last_send_successful != 0) {
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

    int32_t __cdecl Net_Create_Game_(const char* name, int32_t max_players, const char* password, const char* player_name) {
        (void)password;
        if (!netadapter::Create_Game(&_router_adapter, name, max_players, player_name)) {
            return 0;
        }

        Pump_Router_Until_(Router_Session_Ready_);
        if (!Router_Session_Ready_()) {
            return 0;
        }

        comcore::ComInit_(0x1c2, const_cast<char*>(name), _recv_fn);
        net::_thisHost = netadapter::Local_Network_Id(&_router_adapter);

        _netInitialized = 1;
        return 1;
    }
}