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
        uint32_t _router_wait_session_list_revision;
        constexpr uint32_t ROUTER_CONNECT_ATTEMPTS = 3;
        constexpr uint32_t ROUTER_CONNECT_RETRY_DELAY_MS = 250;

        bool __cdecl Is_Router_Mode_(uint32_t mode) {
            return mode == 0 || mode == 1;
        }

        bool __cdecl Is_Router_Host_Char_(char value) {
            return (value >= '0' && value <= '9') ||
                (value >= 'A' && value <= 'Z') ||
                (value >= 'a' && value <= 'z') ||
                value == '.' || value == '-';
        }

        bool __cdecl Parse_Router_Endpoint_(const char* text, netadapter::s_config* config) {
            if (text == nullptr || config == nullptr || text[0] == '\0') {
                return false;
            }

            char host[ROUTER_ADDRESS_SIZE] = {};
            uint32_t host_len = 0;
            const char* cursor = text;
            while (*cursor != '\0' && *cursor != ':' && host_len + 1 < sizeof(host)) {
                if (!Is_Router_Host_Char_(*cursor)) {
                    return false;
                }
                host[host_len++] = *cursor++;
            }
            if (host_len == 0) {
                return false;
            }

            uint16_t port = config->tcp_port;
            if (*cursor == ':') {
                ++cursor;
                uint32_t parsed_port = 0;
                if (*cursor == '\0') {
                    return false;
                }
                while (*cursor != '\0') {
                    if (*cursor < '0' || *cursor > '9') {
                        return false;
                    }
                    parsed_port = parsed_port * 10 + (uint32_t)(*cursor - '0');
                    if (parsed_port > 65535) {
                        return false;
                    }
                    ++cursor;
                }
                if (parsed_port == 0) {
                    return false;
                }
                port = (uint16_t)parsed_port;
            } else if (*cursor != '\0') {
                return false;
            }

            network::router::Router_Copy_String(config->host, sizeof(config->host), host);
            config->tcp_port = port;
            return true;
        }

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
                    int32_t player_idx = NETMOX::Net_Id_To_Player_Id_((int32_t)event.network_id);
                    if (player_idx >= 0) {
                        NETMOX::Handle_Network_Player_Disconnected_(player_idx);
                    }
                } else if (event.kind == netadapter::EVENT_SESSION_CLOSED) {
                    SDL_Log("Router session closed");
                } else if (event.kind == netadapter::EVENT_DISCONNECTED) {
                    SDL_Log("Router disconnected");
                } else if (event.kind == netadapter::EVENT_ERROR) {
                    SDL_Log("Router error: %s", event.error.message);
                }
            }
        }

        void __cdecl Pump_Router_Until_(bool (*predicate)(), uint32_t timeout_ms) {
            uint64_t start_time = SDL_GetTicks();
            while (!predicate()) {
                Pump_Router_Packets_();
                if ((uint32_t)(SDL_GetTicks() - start_time) >= timeout_ms) {
                    break;
                }
                SDL_Delay(1);
            }
        }

        bool __cdecl Router_Local_Id_Ready_() {
            return netadapter::Local_Network_Id(&_router_adapter) != 0;
        }

        bool __cdecl Router_Session_List_Ready_() {
            return netadapter::Game_List_Revision(&_router_adapter) != _router_wait_session_list_revision;
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

        int32_t __cdecl Refresh_Router_Game_List_(uint32_t timeout_ms) {
            if (!Is_Router_Mode_((uint32_t)_connection_type)) {
                return 0;
            }

            _router_wait_session_list_revision = netadapter::Game_List_Revision(&_router_adapter);
            if (!netadapter::Refresh_Game_List(&_router_adapter)) {
                return netadapter::Game_Count(&_router_adapter);
            }
            Pump_Router_Until_(Router_Session_List_Ready_, timeout_ms);
            return netadapter::Game_Count(&_router_adapter);
        }

        bool __cdecl Init_Router_Config_(netadapter::s_config* config, const char* player_name, s_network* network, bool parse_endpoint) {
            *config = netadapter::s_config{};
            bool endpoint_configured = false;
            if (parse_endpoint && network != nullptr && network->modem_settings.cmd_reset[0] != '\0') {
                if (Parse_Router_Endpoint_(network->modem_settings.cmd_reset, config)) {
                    endpoint_configured = true;
                    SDL_Log("Router endpoint from clear string: %s:%u", config->host, (unsigned)config->tcp_port);
                } else {
                    SDL_Log("Ignoring invalid router endpoint in clear string '%s'; using %s:%u", network->modem_settings.cmd_reset, config->host, (unsigned)config->tcp_port);
                }
            }
            if (player_name != nullptr && player_name[0] != '\0') {
                network::router::Router_Copy_String(config->client_name, sizeof(config->client_name), player_name);
            }
            network::router::Router_Copy_String(config->build_string, sizeof(config->build_string), GAME_BUILD_DATE);
            return endpoint_configured;
        }

        bool __cdecl Is_Router_Host_Startup_(uint32_t mode, const s_network* network) {
            return mode == 0 && network != nullptr && network->sync_state == 1;
        }

        int32_t __cdecl Connect_Router_With_Retry_(const netadapter::s_config* config) {
            for (uint32_t attempt = 1; attempt <= ROUTER_CONNECT_ATTEMPTS; ++attempt) {
                if (netadapter::Connect(&_router_adapter)) {
                    return 1;
                }

                const char* failure_phase = "startup";
                switch (netadapter::Connect_Failure_Phase(&_router_adapter)) {
                case netadapter::CONNECT_FAILURE_TCP_CONNECT:
                    failure_phase = "TCP connect";
                    break;
                case netadapter::CONNECT_FAILURE_HELLO_SEND:
                    failure_phase = "HELLO send";
                    break;
                default:
                    break;
                }

                SDL_Log(
                    "Router %s attempt %u/%u failed for %s:%u, error=%d (%s)",
                    failure_phase,
                    (unsigned)attempt,
                    (unsigned)ROUTER_CONNECT_ATTEMPTS,
                    config->host,
                    (unsigned)config->tcp_port,
                    netadapter::Last_Error(&_router_adapter),
                    netadapter::Last_Error_Message(&_router_adapter));

                netadapter::Shutdown(&_router_adapter);
                if (attempt != ROUTER_CONNECT_ATTEMPTS) {
                    SDL_Delay(ROUTER_CONNECT_RETRY_DELAY_MS);
                }
            }
            return 0;
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
        SDL_Log("Net_Init mode=%u", (unsigned)mode);

        if (Is_Router_Mode_(mode)) {
            time_::_time = time_::time_;
            time_::_time2 = time_::time_;

            netadapter::s_config config;
            Init_Router_Config_(&config, nullptr, network, mode == 1);
            if (mode == 0 && Is_Router_Host_Startup_(mode, network)) {
                config.bootstrap_mode = netadapter::BOOTSTRAP_EMBEDDED_LAN_HOST;
            } else if (mode == 0) {
                config.bootstrap_mode = netadapter::BOOTSTRAP_LAN_DISCOVERY;
            } else {
                config.bootstrap_mode = netadapter::BOOTSTRAP_DIRECT_ENDPOINT;
            }
            SDL_Log("Router connecting to %s:%u", config.host, (unsigned)config.tcp_port);
            netadapter::Init(&_router_adapter, &config);
            result = Connect_Router_With_Retry_(&config);
            if (result != 0 && netadapter::Is_Connected(&_router_adapter)) {
                Pump_Router_Until_(Router_Local_Id_Ready_, 5000);
                result = Router_Local_Id_Ready_() ? 1 : 0;
                if (result == 0) {
                    SDL_Log("Router connected but WELCOME was not received within timeout");
                } else {
                    SDL_Log("Router connected as network id %u", (unsigned)netadapter::Local_Network_Id(&_router_adapter));
                }
            }
            if (result != 0) {
                Refresh_Router_Game_List_(1000);
            }
            if (result == 0) {
                netadapter::Shutdown(&_router_adapter);
            }

            _recv_fn = recv_fn_ptr;
        }

        if (mode == 2) {
            result = simmodem::Init_Modem_(network, recv_fn_ptr);
        }

        if (mode == 3) {
            _connection_type = -1;
            return 0;
        }

        return result;
    }

    int32_t __cdecl Net_Get_Game_Count_() {
        if (Is_Router_Mode_((uint32_t)_connection_type)) {
            Pump_Router_Packets_();
            return netadapter::Game_Count(&_router_adapter);
        }
        return 0;
    }

    int32_t __cdecl Net_Refresh_Game_List_() {
        return Refresh_Router_Game_List_(1000);
    }

    int32_t __cdecl Get_Player_Count_() {
        if (Is_Router_Mode_((uint32_t)_connection_type)) {
            Pump_Router_Packets_();
            return netadapter::Get_Users(&_router_adapter, nullptr, 0);
        }
        return 0;
    }

    char* __cdecl Net_Get_Game_Name_(uint32_t session_idx) {
        if (Is_Router_Mode_((uint32_t)_connection_type)) {
            Pump_Router_Packets_();
            return const_cast<char*>(netadapter::Game_Name(&_router_adapter, session_idx));
        }
        return nullptr;
    }

    void __cdecl Net_Uninit_()
    {
        if (Is_Router_Mode_((uint32_t)_connection_type)) {
            netadapter::Shutdown(&_router_adapter);
        }

        if (_connection_type == 2) {
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
            Refresh_Router_Game_List_(1000);
            session_index = Find_Router_Game_By_Name_(game_name);
        }
        if (session_index < 0 || !netadapter::Join_Game(&_router_adapter, (uint32_t)session_index, player_name)) {
            return 0;
        }

        Pump_Router_Until_(Router_Session_Ready_, 5000);
        if (!Router_Session_Ready_()) {
            return 0;
        }

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

        if (_connection_type == 2) {
            if (users != nullptr) {
                users[0] = (int16_t)net::_thisHost;
                users[1] = simmodem::_connected_to_id;
            }
            return 2;
        }

        if (Is_Router_Mode_((uint32_t)_connection_type)) {
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
        if (Is_Router_Mode_((uint32_t)_connection_type)) {
            netadapter::Set_New_Players_Enabled(&_router_adapter, false);
            Pump_Router_Packets_();
            return;
        }

    }

    uint32_t __cdecl Net_Host_ID_() {
        if (_connection_type == 3) {
            return ten::TEN_Get_Host_Id_();
        }
        if (Is_Router_Mode_((uint32_t)_connection_type)) {
            return netadapter::Local_Network_Id(&_router_adapter);
        }
        return net::_thisHost;
    }

    void __cdecl Net_Update_() {
        if (_connection_type == 3) {
            ten::Ten_Update_();
        }

        if (Is_Router_Mode_((uint32_t)_connection_type) && _inComms == 0) {
            _inComms = 1;
            if (_netInitialized != 0) {
                Pump_Router_Packets_();
            }
            _inComms = 0;
        }

        if (_connection_type == 2) {
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

        if (Is_Router_Mode_((uint32_t)_connection_type)) {
            _router_last_send_successful = netadapter::Send_Game_Message(&_router_adapter, (uint32_t)target_id, (uint16_t)msg_type, data, size) ? 1 : 0;
            Pump_Router_Packets_();
        }

        if (_connection_type == 2) {
            motrans::Modem_Send_Message_(target_id, msg_type, data, size);
        }

        _in_send_message = 0;
        return Net_Status_() & 4;
    }

    uint32_t __cdecl Net_Status_() {
        uint32_t status = 0;

        if (Is_Router_Mode_((uint32_t)_connection_type)) {
            if (_router_last_send_successful != 0) {
                status |= 4;
            }
        }

        if (_connection_type == 3) {
            status |= 4;
        }

        if (_connection_type == 2) {
            return motrans::Modem_Status_();
        }

        return status;
    }

    int32_t __cdecl Net_Create_Game_(const char* name, int32_t max_players, const char* password, const char* player_name) {
        (void)password;
        if (!netadapter::Create_Game(&_router_adapter, name, max_players, player_name)) {
            return 0;
        }

        Pump_Router_Until_(Router_Session_Ready_, 5000);
        if (!Router_Session_Ready_()) {
            return 0;
        }

        net::_thisHost = netadapter::Local_Network_Id(&_router_adapter);

        _netInitialized = 1;
        return 1;
    }
}