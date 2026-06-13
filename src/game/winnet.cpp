#include "pch.h"

namespace WINNET {
    void __cdecl Register_Player_(uint32_t player_id, const char* player_name, const char* event_str);
    void __cdecl Drop_Player_(uint32_t player_id);

    int32_t _provider_count;
    s_winnet_provider* _providers[9];
    clock_t _net_refresh_timer;
    uint32_t _session_count;
    uint32_t _player_count;
    uint32_t _local_player_id;
    int32_t _provider_mode;

    namespace {
        s_net_player* _player_list[20];
        const GUID k_game_guid = {0xEBD6DA60, 0x7CCD, 0x11CF, {0xB4, 0xC3, 0x00, 0xAA, 0x00, 0x3F, 0xCD, 0x2E}};

#if ORION2RE_ENABLE_NETWORKING
        IDirectPlay* _dplay;
        int8_t _is_searching_games;
        DPSESSIONDESC* _sessions[100];

        int8_t __cdecl Find_Provider_(const char* name) {
            int8_t found_index = -1;

            for (int32_t i = 0; i < _provider_count; ++i) {
                if (strstr(_providers[i]->name, name) != nullptr) {
                    found_index = (int8_t)i;
                }
            }

            return found_index;
        }

        int __stdcall Refresh_Game_List_Callback_(LPDPSESSIONDESC session_desc, void* context, uint32_t* timeout, uint32_t flags) {
            if ((flags & 1) != 0 || _session_count == 100) {
                _is_searching_games = 0;
                return 0;
            }

            uint32_t insert_idx = _session_count;

            for (uint32_t i = 0; i < _session_count; ++i) {
                if (strcasecmp(session_desc->szSessionName, _sessions[i]->szSessionName) < 0) {
                    insert_idx = i;
                    _sessions[_session_count] = (LPDPSESSIONDESC)malloc(sizeof(*_sessions[_session_count]));

                    for (uint32_t j = _session_count; j > i; --j) {
                        memcpy(_sessions[j], _sessions[j - 1], sizeof(*_sessions[j]));
                    }

                    memcpy(_sessions[i], session_desc, sizeof(*_sessions[i]));
                    break;
                }
            }

            if (insert_idx == _session_count) {
                _sessions[_session_count] = (LPDPSESSIONDESC)malloc(sizeof(*_sessions[_session_count]));
                memcpy(_sessions[_session_count], session_desc, sizeof(*_sessions[_session_count]));
            }

            _session_count++;
            return 1;
        }

        BOOL __stdcall Enumerate_Providers_(GUID* guid, char* name, DWORD major_version, DWORD minor_version, void* context) {
            if (_provider_count < 9) {
                s_winnet_provider* provider = (s_winnet_provider*)malloc(sizeof(*provider));
                _providers[_provider_count] = provider;

                provider->guid = *guid;
                memcpy(provider->name, name, sizeof(provider->name));
                provider->major_version = major_version;
                provider->minor_version = minor_version;

                strlwr(provider->name);

                _provider_count++;
                return 1;
            }
            return 0;
        }

        BOOL __stdcall Player_Enumerate_Callback_(DPID dp_id, char* short_name, char* long_name, DWORD flags, void* context) {
            Register_Player_(dp_id, short_name, long_name);
            return 1;
        }

        void __cdecl Exit_With_DPlay_Error_(int32_t error_code) {
            char message[200];
            const char* error_string;

            strcpy(message, "DirectPlay ERR: ");

            switch (error_code) {
            case 0x80070057:
                error_string = "Invalid Params";
                break;
            case 0x80004005:
                error_string = "Generic";
                break;
            case 0x8877001E:
                error_string = "Buffer To Small";
                break;
            case 0x88770082:
                error_string = "Invalid Object";
                break;
            case 0x887700BE:
                error_string = "No Messages";
                break;
            default:
                error_string = "Unknown Err";
                break;
            }

            strcat(message, error_string);
            exitm::Exit_With_Message(message);
        }

        void __cdecl Exit_With_Error_(HRESULT hr) {
            char message[100];
            const char* error_string;

            switch (hr) {
            case (int32_t)0x80004005:
                error_string = "Generic";
                break;
            case (int32_t)0x80004001:
                error_string = "Unsupported";
                break;
            case (int32_t)0x88770014:
                error_string = "Active Players";
                break;
            case (int32_t)0x80070057:
                error_string = "Invalid Params";
                break;
            case (int32_t)0x88770082:
                error_string = "Invalid Object";
                break;
            case (int32_t)0x88770096:
                error_string = "Invalid Player";
                break;
            case (int32_t)0x887700FA:
                error_string = "Unavailable";
                break;
            case (int32_t)0x8877010E:
                error_string = "Busy";
                break;
            case (int32_t)0x88770118:
                error_string = "User Cancel";
                break;
            default:
                error_string = "Error Unknown";
                break;
            }

            strcpy(message, error_string);
            exitm::Exit_With_Message(message);
        }

        void __cdecl Create_Player_(DPID* pid, const char* friendly_name, const char* formal_name) {
            HRESULT hr = _dplay->CreatePlayer(
                pid,
                const_cast<char*>(friendly_name),
                const_cast<char*>(formal_name),
                nullptr
            );

            if (hr != 0) {
                *pid = 0;
                Exit_With_DPlay_Error_(hr);
                return;
            }

            Register_Player_(*pid, friendly_name, formal_name);
        }
#endif
    }

    int32_t __cdecl Init_Connection_(int32_t mode) {
#if !ORION2RE_ENABLE_NETWORKING
        (void)mode;
        _provider_mode = -1;
        return 0;
#else
        int32_t provider_idx = -1;
        const char* provider_name = nullptr;

        if (mode == 0) {
            provider_name = "modem";
        } else if (mode == 1) {
            provider_name = "ipx";
        } else if (mode == 2) {
            provider_name = "tcp";
        }

        if (provider_name != nullptr) {
            provider_idx = Find_Provider_(provider_name);
        }

        if (provider_idx == -1) {
            return 0;
        }

        if (_dplay != nullptr) {
            _dplay->Release();
            _dplay = nullptr;
        }

        HRESULT hr = DirectPlayCreate(&_providers[provider_idx]->guid, &_dplay, nullptr);
        if (hr == 0) {
            WINNET::_provider_mode = mode;
            return 1;
        }

        return 0;
#endif
    }

    uint32_t __cdecl Refresh_Game_List_() {
#if !ORION2RE_ENABLE_NETWORKING
        _session_count = 0;
        return 0;
#else

        if (_session_count != 0) {
            for (uint32_t i = 0; i < _session_count; ++i) {
                if (_sessions[i] != nullptr) {
                    ::free(_sessions[i]);
                }
            }
        }

        _session_count = 0;

        DPSESSIONDESC session_desc;
        memset(&session_desc, 0, sizeof(session_desc));
        session_desc.dwSize = sizeof(DPSESSIONDESC);
        session_desc.guidSession = k_game_guid;

        if (netcode::_connection_type == 0) {
            _is_searching_games = 1;

            HRESULT hr = _dplay->EnumSessions(
                &session_desc,
                1000,
                (LPDPENUMSESSIONSCALLBACK)Refresh_Game_List_Callback_,
                platform::g_main_window,
                1 /* DPENUMSESSIONS_AVAILABLE */
            );

            if (hr != 0) {
                if (hr == (HRESULT)0x88770082) { // DPERR_INVALIDOBJECT
                    exitm::Exit_With_Message("Error Enum Current Games Invalid Object");
                }
                if (hr == (HRESULT)0x80070057) { // E_INVALIDARG / DPERR_INVALIDPARAMS
                    exitm::Exit_With_Message("Error Enum Current Games Invalid Params");
                }
            }
        }

        return _session_count;
#endif
    }

    int32_t __cdecl Init_Providers_() {
#if !ORION2RE_ENABLE_NETWORKING
        _provider_count = 0;
        return 0;
#else
        DirectPlayEnumerateA(Enumerate_Providers_, (LPVOID)platform::g_main_window);
        return 0;
#endif
    }

    namespace {
        int32_t __cdecl Refresh_Player_List_(uint32_t session_id) {
#if !ORION2RE_ENABLE_NETWORKING
            (void)session_id;
            _player_count = 0;
            return 0;
#else
        for (uint32_t i = 0; i < _player_count; ++i) {
            if (_player_list[i] != nullptr) {
                ::free(_player_list[i]);
            }
        }

        _player_count = 0;

        _dplay->EnumPlayers(
            session_id,
            Player_Enumerate_Callback_,
            (LPVOID)platform::g_main_window,
            0x80 /* DPENUMPLAYERS_SESSION */
        );


        return 1;
#endif
        }
    }

    uint32_t __cdecl Get_Player_Network_Id_(int32_t player_idx) {
        return _player_list[player_idx]->player_id;
    }

    void __cdecl Register_Player_(uint32_t player_id, const char* player_name, const char* event_str) {
        s_net_player* player = (s_net_player*)malloc(sizeof(*player));

        _player_list[_player_count] = player;
        player->player_id = player_id;

        strcpy(player->event_string, event_str);
        strcpy(player->player_name, player_name);

        player->join_time = time_::_time();

        _player_count++;
    }

    void __cdecl Net_Enable_New_Players() {
#if ORION2RE_ENABLE_NETWORKING
        if (_dplay != nullptr) {
            _dplay->EnableNewPlayers(1);
        }
#endif
    }

    void __cdecl Net_Disable_New_Players_() {
#if ORION2RE_ENABLE_NETWORKING
        if (_dplay != nullptr) {
            _dplay->EnableNewPlayers(0);
        }
#endif
    }

    void __cdecl Handle_Create_Player_(s_net_pack_create_player* pack) {
        if (pack->player_type == 1) {
            Register_Player_(pack->player_id, pack->player_name, pack->event_str);
        }
    }

    void __cdecl Handle_Destroy_Player_(s_net_pack_destroy_player* packet) {
        Drop_Player_(packet->player_id);
    }

    void __cdecl Drop_Player_(uint32_t player_id) {
        uint32_t index = 0xFFFFFFFF;
        bool found = false;

        if (_player_count > 0) {
            for (uint32_t i = 0; i < _player_count; i++) {
                if (_player_list[i]->player_id == player_id) {
                    index = i;
                    found = true;
                    break;
                }
            }
        }

        if (found) {
            if (index < _player_count - 1) {
                for (uint32_t j = index; j < _player_count - 1; j++) {
                    _player_list[j]->player_id = _player_list[j + 1]->player_id;
                    _player_list[j]->join_time = _player_list[j + 1]->join_time;
                    strcpy(_player_list[j]->event_string, _player_list[j + 1]->event_string);
                    strcpy(_player_list[j]->player_name, _player_list[j + 1]->player_name);
                }
            }

            ::free(_player_list[_player_count - 1]);
            _player_count--;
        }
    }

    char* __cdecl Get_Game_Name_(uint32_t session_idx) {
#if !ORION2RE_ENABLE_NETWORKING
        (void)session_idx;
        return nullptr;
#else
        if (session_idx >= _session_count) {
            return nullptr;
        }

        if (_dplay == nullptr) {
            return nullptr;
        }

        return _sessions[session_idx]->szSessionName;
#endif
    }

    int32_t __cdecl Join_Game_(const char* game_name, const char* password, const char* player_name) {
#if !ORION2RE_ENABLE_NETWORKING
        (void)game_name;
        (void)password;
        (void)player_name;
        return 0;
#else
        uint32_t found_idx = 0xffffffff;

        if (_session_count != 0) {
            for (uint32_t i = 0; i < _session_count; ++i) {
                const char* name = Get_Game_Name_(i);
                if (name != nullptr && strcmp(game_name, name) == 0) {
                    found_idx = i;
                    break;
                }
            }
        }

        if (found_idx == 0xffffffff || found_idx >= _session_count) {
            return 0;
        }

        DPSESSIONDESC* session = _sessions[found_idx];
        Refresh_Player_List_(session->dwSession);

        DPSESSIONDESC session_desc;
        memset(&session_desc, 0, sizeof(session_desc));

        session_desc.dwSize = sizeof(DPSESSIONDESC);
        session_desc.guidSession = session->guidSession;
        session_desc.dwFlags = 1;
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
        Create_Player_(&player_id, player_name, "");
        _local_player_id = player_id;

        return 1;
#endif
    }

    int32_t __cdecl Create_Game_(const char* name, int32_t max_players, const char* password, const char* player_name) {
#if !ORION2RE_ENABLE_NETWORKING
        (void)name;
        (void)max_players;
        (void)password;
        (void)player_name;
        return 0;
#else
        DPSESSIONDESC session_desc;
        DPID player_id;

        memset(&session_desc, 0, sizeof(DPSESSIONDESC));
        session_desc.dwSize = sizeof(DPSESSIONDESC);
        session_desc.guidSession = k_game_guid;
        session_desc.dwFlags = 2;
        session_desc.dwMaxPlayers = max_players;

        strcpy(session_desc.szSessionName, name);

        if (password != nullptr) {
            strcpy(session_desc.szPassword, password);
        }

        HRESULT hr = _dplay->Open(&session_desc);
        if (hr != 0) {
            _dplay->Release();
            _dplay = nullptr;
            return 0;
        }

        DPSESSIONDESC* new_session = (DPSESSIONDESC*)::malloc(sizeof(DPSESSIONDESC));
        _sessions[_session_count] = new_session;
        memcpy(new_session, &session_desc, sizeof(DPSESSIONDESC));
        _session_count++;

        Net_Enable_New_Players();

        Create_Player_(&player_id, player_name, "");
        _local_player_id = player_id;

        return 1;
#endif
    }

    void __cdecl Shutdown_() {
#if ORION2RE_ENABLE_NETWORKING
        if (_dplay != nullptr) {
            _dplay->Close();
            _dplay->Release();
            _dplay = nullptr;
        }

        if (_session_count != 0) {
            for (uint32_t i = 0; i < _session_count; ++i) {
                if (_sessions[i] != nullptr) {
                    ::free(_sessions[i]);
                    _sessions[i] = nullptr;
                }
            }
        }
#endif
        _session_count = 0;
    }

    int32_t __cdecl Send_Message_(uint32_t from_player_id, uint32_t to_player_id, void* data, uint32_t size) {
#if !ORION2RE_ENABLE_NETWORKING
        (void)from_player_id;
        (void)to_player_id;
        (void)data;
        (void)size;
        return 0;
#else
        HRESULT hr = _dplay->Send(from_player_id, to_player_id, 0, data, size);
        if (hr != 0) {
            Exit_With_Error_(hr);
            return 0;
        }

        return 1;
#endif
    }

    int32_t __cdecl Poll_Message_(s_comcore_poll_buf* poll_buf) {
#if !ORION2RE_ENABLE_NETWORKING
        (void)poll_buf;
        return 0;
#else
        HRESULT receive_result;
        uint32_t id_from;
        uint32_t id_to;
        uint32_t buffer_length = comcore::_recBufferLength;

        receive_result = _dplay->Receive(
            (LPDPID)&id_from,
            (LPDPID)&id_to,
            9,
            poll_buf->send_buf,
            (LPDWORD)&buffer_length);

        if (receive_result != 0) {
            if (receive_result != (HRESULT)0x887700BEu) {
                Exit_With_DPlay_Error_(receive_result);
            }
            return 0;
        }

        SDL_Log("Receiving Message....");

        receive_result = _dplay->Receive(
            (LPDPID)&id_from,
            (LPDPID)&id_to,
            1,
            poll_buf->send_buf,
            (LPDWORD)&buffer_length);

        if (receive_result == 0) {
            poll_buf->id_from = id_from;
            poll_buf->id_to = id_to;
            poll_buf->len = buffer_length;
            return 3;
        }

        Exit_With_DPlay_Error_(receive_result);
        return 0;
#endif
    }

    void __cdecl SetPlayerLastKeepAlive_(s_comcore_poll_buf* poll_buf) {
        if (poll_buf->id_from == _local_player_id) {
            return;
        }

        for (uint32_t i = 0; i < _player_count; i++) {
            s_net_player* player = _player_list[i];
            if (player->player_id == poll_buf->id_from) {
                player->join_time = time_::_time();
            }
        }
    }
}