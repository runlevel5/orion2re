#include "game/netadapter.h"

#include "network/router_service.h"
#include "network/router_session_browser.h"

#include <cstring>
#include <deque>
#include <utility>

namespace netadapter {
namespace {

struct s_wire_game_packet_header {
    uint16_t message_type = 0;
    uint32_t payload_size = 0;
};

void Write_Uint16_(network::router::Bit_Stream* stream, uint16_t value) {
    stream->Write_Uint16(value);
}

void Write_Uint32_(network::router::Bit_Stream* stream, uint32_t value) {
    stream->Write_Uint32(value);
}

bool Decode_Game_Packet_(const network::router::s_router_message* message, s_game_packet* packet) {
    if (message == nullptr || packet == nullptr) {
        return false;
    }

    network::router::Bit_Stream reader = message->payload;
    uint16_t message_type = 0;
    uint32_t payload_size = 0;
    if (!reader.Read_Uint16(&message_type) || !reader.Read_Uint32(&payload_size)) {
        return false;
    }
    if (reader.Size() - reader.Tell() != payload_size) {
        return false;
    }

    packet->sender_network_id = message->header.sender_network_id;
    packet->target_network_id = message->header.target_network_id;
    packet->message_type = message_type;
    packet->payload.resize(payload_size);
    if (payload_size != 0 && !reader.Read_Bytes(packet->payload.data(), payload_size)) {
        return false;
    }
    return reader.At_End();
}

void Copy_Config_To_Client_(const s_config* config, network::router::s_router_client_config* client_config) {
    network::router::Router_Copy_String(client_config->host, sizeof(client_config->host), config != nullptr ? config->host : "127.0.0.1");
    client_config->tcp_port = config != nullptr ? config->tcp_port : 47800;
}

}

struct s_state {
    s_config config;
    network::router::s_router_client client;
    network::router::s_router_session_browser browser;
    network::router::s_router_service embedded_router_service;
    SDL_Thread* embedded_router_thread = nullptr;
    SDL_AtomicInt embedded_router_result;
    std::deque<s_game_packet> packets;
    std::deque<s_event> events;
    uint32_t next_request_id = 1;
    uint32_t session_list_revision = 0;
    e_connect_failure_phase connect_failure_phase = CONNECT_FAILURE_NONE;
};

namespace {

void Reset_Browser_From_Config_(s_state* state) {
    network::router::s_router_session_browser_config browser_config;
    browser_config.mode = state->config.bootstrap_mode == BOOTSTRAP_LAN_DISCOVERY ?
        network::router::ROUTER_SESSION_BROWSER_LAN_DISCOVERY :
        network::router::ROUTER_SESSION_BROWSER_DIRECT_ENDPOINT;
    network::router::Router_Copy_String(browser_config.host, sizeof(browser_config.host), state->config.host);
    browser_config.tcp_port = state->config.tcp_port;
    browser_config.discovery_port = state->config.discovery_port;
    browser_config.discovery_timeout_ms = state->config.discovery_timeout_ms;
    network::router::Router_Copy_String(browser_config.client_name, sizeof(browser_config.client_name), state->config.client_name);
    network::router::Router_Session_Browser_Init(&state->browser, &browser_config);
}

void Reset_Client_From_Config_(s_state* state) {
    network::router::s_router_client_config client_config;
    Copy_Config_To_Client_(&state->config, &client_config);
    network::router::Router_Client_Init(&state->client, &client_config);
}

bool Connect_Client_(s_state* state) {
    if (!network::router::Router_Client_Connect(&state->client)) {
        state->connect_failure_phase = CONNECT_FAILURE_TCP_CONNECT;
        return false;
    }
    if (!network::router::Router_Client_Send_Hello(
        &state->client,
        state->config.client_name,
        ROUTER_VERSION,
        state->next_request_id++)) {
        state->connect_failure_phase = CONNECT_FAILURE_HELLO_SEND;
        return false;
    }
    return true;
}

bool Connect_Client_To_Endpoint_(s_state* state, const network::router::s_router_endpoint* endpoint) {
    if (state == nullptr || endpoint == nullptr) {
        return false;
    }

    network::router::Router_Client_Disconnect(&state->client);
    network::router::Router_Copy_String(state->config.host, sizeof(state->config.host), endpoint->host);
    state->config.tcp_port = endpoint->tcp_port;
    Reset_Client_From_Config_(state);
    return Connect_Client_(state);
}

int Embedded_Router_Thread_(void* data) {
    s_state* state = static_cast<s_state*>(data);
    int result = network::router::Router_Service_Run(&state->embedded_router_service);
    SDL_SetAtomicInt(&state->embedded_router_result, result);
    return result;
}

void Stop_Embedded_Router_(s_state* state) {
    if (state == nullptr || state->embedded_router_thread == nullptr) {
        return;
    }

    network::router::Router_Service_Request_Stop(&state->embedded_router_service);
    SDL_WaitThread(state->embedded_router_thread, nullptr);
    state->embedded_router_thread = nullptr;
    if (SDL_GetAtomicInt(&state->embedded_router_result) != 0) {
        SDL_Log("Embedded router stopped with result %d", SDL_GetAtomicInt(&state->embedded_router_result));
    }
}

bool Start_Embedded_Router_(s_state* state) {
    if (state == nullptr) {
        return false;
    }
    if (state->embedded_router_thread != nullptr) {
        return true;
    }

    network::router::s_router_config service_config;
    network::router::Router_Copy_String(service_config.listen_host, sizeof(service_config.listen_host), "0.0.0.0");
    service_config.tcp_port = state->config.tcp_port;
    service_config.discovery_port = state->config.discovery_port;
    service_config.enable_discovery = true;
    service_config.enable_signal_handlers = false;
    service_config.verbose_logging = false;
    network::router::Router_Copy_String(service_config.router_id, sizeof(service_config.router_id), "embedded-router");
    network::router::Router_Copy_String(service_config.router_name, sizeof(service_config.router_name), "Orion2 Embedded Router");

    network::router::Router_Service_Init(&state->embedded_router_service, &service_config);
    SDL_SetAtomicInt(&state->embedded_router_result, 0);
    state->embedded_router_thread = SDL_CreateThread(Embedded_Router_Thread_, "orion2-router", state);
    if (state->embedded_router_thread == nullptr) {
        SDL_Log("Embedded router thread creation failed: %s", SDL_GetError());
        return false;
    }

    uint32_t start_timeout_ms = state->config.discovery_timeout_ms;
    if (start_timeout_ms < 1000) {
        start_timeout_ms = 1000;
    }

    uint64_t start_time = SDL_GetTicks();
    while ((uint32_t)(SDL_GetTicks() - start_time) < start_timeout_ms) {
        if (network::router::Router_Service_Is_Running(&state->embedded_router_service) &&
            network::router::Router_Service_Bound_Tcp_Port(&state->embedded_router_service) != 0) {
            state->config.tcp_port = network::router::Router_Service_Bound_Tcp_Port(&state->embedded_router_service);
            network::router::Router_Copy_String(state->config.host, sizeof(state->config.host), "127.0.0.1");
            SDL_Log("Embedded router started on 127.0.0.1:%u", static_cast<unsigned>(state->config.tcp_port));
            Reset_Browser_From_Config_(state);
            return true;
        }
        if (!network::router::Router_Service_Is_Running(&state->embedded_router_service) && SDL_GetAtomicInt(&state->embedded_router_result) != 0) {
            break;
        }
        SDL_Delay(1);
    }

    SDL_Log("Embedded router failed to start, result=%d", SDL_GetAtomicInt(&state->embedded_router_result));
    Stop_Embedded_Router_(state);
    return false;
}

}

s_adapter::s_adapter()
    : state(std::make_unique<s_state>()) {
}

s_adapter::~s_adapter() {
    Shutdown(this);
}

void Init(s_adapter* adapter, const s_config* config) {
    if (adapter == nullptr) {
        return;
    }
    if (adapter->state == nullptr) {
        adapter->state = std::make_unique<s_state>();
    }
    Shutdown(adapter);
    adapter->state->config = config != nullptr ? *config : s_config{};
    adapter->state->packets.clear();
    adapter->state->events.clear();
    adapter->state->next_request_id = 1;
    adapter->state->session_list_revision = 0;
    adapter->state->connect_failure_phase = CONNECT_FAILURE_NONE;
    SDL_SetAtomicInt(&adapter->state->embedded_router_result, 0);

    Reset_Client_From_Config_(adapter->state.get());
    Reset_Browser_From_Config_(adapter->state.get());
}

bool Connect(s_adapter* adapter) {
    if (adapter == nullptr || adapter->state == nullptr) {
        return false;
    }
    adapter->state->connect_failure_phase = CONNECT_FAILURE_NONE;

    if (adapter->state->config.bootstrap_mode == BOOTSTRAP_EMBEDDED_LAN_HOST) {
        if (!Start_Embedded_Router_(adapter->state.get())) {
            adapter->state->connect_failure_phase = CONNECT_FAILURE_TCP_CONNECT;
            return false;
        }
        Reset_Client_From_Config_(adapter->state.get());
    } else if (adapter->state->config.bootstrap_mode == BOOTSTRAP_LAN_DISCOVERY) {
        return true;
    }

    return Connect_Client_(adapter->state.get());
}

void Shutdown(s_adapter* adapter) {
    if (adapter == nullptr || adapter->state == nullptr) {
        return;
    }
    network::router::Router_Client_Disconnect(&adapter->state->client);
    network::router::Router_Session_Browser_Shutdown(&adapter->state->browser);
    Stop_Embedded_Router_(adapter->state.get());
    adapter->state->packets.clear();
    adapter->state->events.clear();
}

void Pump(s_adapter* adapter) {
    if (adapter == nullptr || adapter->state == nullptr) {
        return;
    }

    network::router::Router_Client_Pump(&adapter->state->client);
    network::router::Router_Session_Browser_Pump(&adapter->state->browser);
    adapter->state->session_list_revision = network::router::Router_Session_Browser_Revision(&adapter->state->browser);

    network::router::s_router_client_event client_event;
    while (network::router::Router_Client_Pop_Event(&adapter->state->client, &client_event)) {
        switch (client_event.kind) {
        case network::router::ROUTER_CLIENT_EVENT_GAME_DATA: {
            s_game_packet packet;
            if (Decode_Game_Packet_(&client_event.message, &packet)) {
                adapter->state->packets.push_back(std::move(packet));
            } else {
                s_event event;
                event.kind = EVENT_ERROR;
                event.error.code = network::router::ROUTER_ERROR_INVALID_MESSAGE;
                network::router::Router_Copy_String(event.error.message, sizeof(event.error.message), "invalid game packet envelope");
                adapter->state->events.push_back(std::move(event));
            }
            break;
        }
        case network::router::ROUTER_CLIENT_EVENT_MEMBER_JOINED: {
            s_event event;
            event.kind = EVENT_MEMBER_JOINED;
            event.network_id = client_event.member_event.member.network_id;
            adapter->state->events.push_back(std::move(event));
            break;
        }
        case network::router::ROUTER_CLIENT_EVENT_MEMBER_LEFT: {
            s_event event;
            event.kind = EVENT_MEMBER_LEFT;
            event.network_id = client_event.member_event.member.network_id;
            adapter->state->events.push_back(std::move(event));
            break;
        }
        case network::router::ROUTER_CLIENT_EVENT_SESSION_CLOSED: {
            s_event event;
            event.kind = EVENT_SESSION_CLOSED;
            adapter->state->events.push_back(std::move(event));
            break;
        }
        case network::router::ROUTER_CLIENT_EVENT_DISCONNECTED: {
            s_event event;
            event.kind = EVENT_DISCONNECTED;
            adapter->state->events.push_back(std::move(event));
            break;
        }
        case network::router::ROUTER_CLIENT_EVENT_ERROR:
        case network::router::ROUTER_CLIENT_EVENT_PROTOCOL_ERROR: {
            s_event event;
            event.kind = EVENT_ERROR;
            event.error = client_event.error;
            adapter->state->events.push_back(std::move(event));
            break;
        }
        default:
            break;
        }
    }
}

bool Is_Connected(const s_adapter* adapter) {
    return adapter != nullptr && adapter->state != nullptr && network::router::Router_Client_Is_Connected(&adapter->state->client);
}

int Last_Error(const s_adapter* adapter) {
    if (adapter == nullptr || adapter->state == nullptr) {
        return 0;
    }
    return network::router::Router_Client_Last_Error(&adapter->state->client);
}

const char* Last_Error_Message(const s_adapter* adapter) {
    if (adapter == nullptr || adapter->state == nullptr) {
        return "no adapter";
    }
    return network::router::Router_Client_Last_Error_Message(&adapter->state->client);
}

uint32_t Local_Network_Id(const s_adapter* adapter) {
    if (adapter == nullptr || adapter->state == nullptr) {
        return 0;
    }
    return network::router::Router_Client_Local_Network_Id(&adapter->state->client);
}

uint32_t Session_Id(const s_adapter* adapter) {
    if (adapter == nullptr || adapter->state == nullptr) {
        return 0;
    }
    return network::router::Router_Client_Session_Id(&adapter->state->client);
}

bool Has_Pending_Client_Events(const s_adapter* adapter) {
    return adapter != nullptr &&
        adapter->state != nullptr &&
        network::router::Router_Client_Event_Count(&adapter->state->client) != 0;
}

e_connect_failure_phase Connect_Failure_Phase(const s_adapter* adapter) {
    if (adapter == nullptr || adapter->state == nullptr) {
        return CONNECT_FAILURE_NONE;
    }
    return adapter->state->connect_failure_phase;
}

bool Refresh_Game_List(s_adapter* adapter) {
    if (adapter == nullptr || adapter->state == nullptr) {
        return false;
    }
    return network::router::Router_Session_Browser_Refresh(&adapter->state->browser);
}

uint32_t Game_List_Revision(const s_adapter* adapter) {
    if (adapter == nullptr || adapter->state == nullptr) {
        return 0;
    }
    return adapter->state->session_list_revision;
}

int32_t Game_Count(const s_adapter* adapter) {
    if (adapter == nullptr || adapter->state == nullptr) {
        return 0;
    }
    return (int32_t)network::router::Router_Session_Browser_Sessions(&adapter->state->browser).size();
}

const char* Game_Name(const s_adapter* adapter, uint32_t session_index) {
    if (adapter == nullptr || adapter->state == nullptr) {
        return nullptr;
    }
    const std::vector<network::router::s_router_session_record>& sessions = network::router::Router_Session_Browser_Sessions(&adapter->state->browser);
    if (session_index >= sessions.size()) {
        return nullptr;
    }
    return sessions[session_index].session.session_name;
}

uint32_t Game_Session_Id(const s_adapter* adapter, uint32_t session_index) {
    if (adapter == nullptr || adapter->state == nullptr) {
        return 0;
    }
    const std::vector<network::router::s_router_session_record>& sessions = network::router::Router_Session_Browser_Sessions(&adapter->state->browser);
    if (session_index >= sessions.size()) {
        return 0;
    }
    return sessions[session_index].session.session_id;
}

bool Create_Game(s_adapter* adapter, const char* name, int32_t max_players, const char* player_name) {
    if (adapter == nullptr || adapter->state == nullptr || max_players <= 0) {
        return false;
    }

    network::router::s_router_session_registration_payload registration;
    const char* session_name = (name != nullptr && name[0] != '\0') ? name : "Game";
    network::router::Router_Copy_String(registration.session_name, sizeof(registration.session_name), session_name);
    network::router::Router_Copy_String(registration.host_name, sizeof(registration.host_name), player_name != nullptr ? player_name : adapter->state->config.client_name);
    network::router::Router_Copy_String(registration.build_string, sizeof(registration.build_string), adapter->state->config.build_string);
    registration.max_players = (uint16_t)max_players;
    return network::router::Router_Client_Register_Session(&adapter->state->client, &registration, adapter->state->next_request_id++);
}

bool Join_Game(s_adapter* adapter, uint32_t session_index, const char* player_name) {
    (void)player_name;
    if (adapter == nullptr || adapter->state == nullptr) {
        return false;
    }

    const std::vector<network::router::s_router_session_record>& sessions = network::router::Router_Session_Browser_Sessions(&adapter->state->browser);
    if (session_index >= sessions.size() || sessions[session_index].session.session_id == 0) {
        return false;
    }

    const network::router::s_router_session_record& record = sessions[session_index];
    if (!record.session.open) {
        return false;
    }

    if (!network::router::Router_Client_Is_Connected(&adapter->state->client) ||
        std::strcmp(record.router.host, adapter->state->config.host) != 0 ||
        record.router.tcp_port != adapter->state->config.tcp_port) {
        if (!Connect_Client_To_Endpoint_(adapter->state.get(), &record.router)) {
            return false;
        }
    }

    return network::router::Router_Client_Join_Session(
        &adapter->state->client,
        record.session.session_id,
        adapter->state->config.build_string,
        adapter->state->next_request_id++);
}

bool Set_New_Players_Enabled(s_adapter* adapter, bool enabled) {
    if (adapter == nullptr || adapter->state == nullptr) {
        return false;
    }
    return network::router::Router_Client_Set_Session_Open(
        &adapter->state->client,
        Session_Id(adapter),
        enabled,
        adapter->state->next_request_id++);
}

int32_t Get_Users(const s_adapter* adapter, uint32_t* users, int32_t max_users) {
    if (adapter == nullptr || adapter->state == nullptr) {
        return 0;
    }
    const std::vector<network::router::s_router_member_info>& members = network::router::Router_Client_Members(&adapter->state->client);
    if (users != nullptr && max_users > 0) {
        const int32_t count_to_copy = (int32_t)members.size() < max_users ? (int32_t)members.size() : max_users;
        for (int32_t i = 0; i < count_to_copy; ++i) {
            users[i] = members[(size_t)i].network_id;
        }
    }
    return (int32_t)members.size();
}

bool Send_Game_Message(s_adapter* adapter, uint32_t target_network_id, uint16_t message_type, const void* payload, uint32_t payload_size) {
    if (adapter == nullptr || adapter->state == nullptr || (payload == nullptr && payload_size != 0)) {
        return false;
    }

    network::router::Bit_Stream envelope;
    Write_Uint16_(&envelope, message_type);
    Write_Uint32_(&envelope, payload_size);
    envelope.Write_Bytes((const uint8_t*)payload, payload_size);

    if (target_network_id == NETADAPTER_BROADCAST_EXCLUDING_SELF || target_network_id == NETADAPTER_BROADCAST_INCLUDING_SELF) {
        const uint32_t flags = target_network_id == NETADAPTER_BROADCAST_INCLUDING_SELF ? ROUTER_FLAG_BROADCAST_INCLUDE_SENDER : 0;
        return network::router::Router_Client_Send_Broadcast(
            &adapter->state->client,
            envelope.Data(),
            envelope.Size(),
            flags,
            adapter->state->next_request_id++);
    }

    return network::router::Router_Client_Send_To(
        &adapter->state->client,
        target_network_id,
        envelope.Data(),
        envelope.Size(),
        adapter->state->next_request_id++);
}

bool Poll_Game_Packet(s_adapter* adapter, s_game_packet* packet) {
    if (adapter == nullptr || adapter->state == nullptr || packet == nullptr || adapter->state->packets.empty()) {
        return false;
    }
    *packet = std::move(adapter->state->packets.front());
    adapter->state->packets.pop_front();
    return true;
}

bool Poll_Event(s_adapter* adapter, s_event* event) {
    if (adapter == nullptr || adapter->state == nullptr || event == nullptr || adapter->state->events.empty()) {
        return false;
    }
    *event = adapter->state->events.front();
    adapter->state->events.pop_front();
    return true;
}

}