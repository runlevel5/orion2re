#include "network/router_service.h"

#include "network/router_core.h"

#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unordered_map>
#include <utility>
#include <vector>

#include <uv.h>

namespace network::router {

struct s_router_client_connection;

namespace {

struct s_router_write_request {
    uv_write_t request;
    uv_buf_t buffer;
    Bit_Stream data;
    s_router_client_connection* connection = nullptr;
    bool close_after_write = false;
    uint32_t byte_count = 0;
};

struct s_router_udp_send_request {
    uv_udp_send_t request;
    uv_buf_t buffer;
    Bit_Stream data;
};

}

struct s_router_service_state {
    s_router_config config;
    s_router_core_state core;
    uv_loop_t loop{};
    uv_tcp_t tcp_server{};
    uv_udp_t udp_socket{};
    uv_async_t stop_async{};
    uv_signal_t sigint{};
    uv_signal_t sigterm{};
    bool loop_initialized = false;
    bool tcp_server_initialized = false;
    bool udp_socket_initialized = false;
    bool stop_async_initialized = false;
    bool sigint_initialized = false;
    bool sigterm_initialized = false;
    bool shutting_down = false;
    t_router_connection_id next_connection_id = 1;
    std::unordered_map<t_router_connection_id, s_router_client_connection*> clients;
    std::atomic_bool running{false};
    std::atomic_bool stop_requested{false};
    std::atomic<uint16_t> bound_tcp_port{0};
    std::atomic<uint16_t> bound_discovery_port{0};
};

struct s_router_client_connection {
    s_router_service_state* service = nullptr;
    t_router_connection_id connection_id = 0;
    uv_tcp_t tcp;
    std::vector<uint8_t> read_buffer;
    uint32_t pending_write_bytes = 0;
    bool closing = false;
};

namespace {

void Router_Service_Alloc_Callback_(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buffer);
void Router_Service_Tcp_Connection_Callback_(uv_stream_t* server, int status);
void Router_Service_Read_Callback_(uv_stream_t* stream, ssize_t bytes_read, const uv_buf_t* buffer);
void Router_Service_Write_Callback_(uv_write_t* request, int status);
void Router_Service_Client_Close_Callback_(uv_handle_t* handle);
void Router_Service_Stop_Async_Callback_(uv_async_t* handle);
void Router_Service_Signal_Callback_(uv_signal_t* handle, int signum);
void Router_Service_Udp_Recv_Callback_(uv_udp_t* handle, ssize_t bytes_read, const uv_buf_t* buffer, const sockaddr* address, unsigned flags);
void Router_Service_Udp_Send_Callback_(uv_udp_send_t* request, int status);

void Router_Service_Log_(s_router_service_state* state, const char* message) {
    if (state == nullptr || message == nullptr) {
        return;
    }
    if (state->config.log_callback != nullptr) {
        state->config.log_callback(message, state->config.log_user_data);
        return;
    }
    std::fprintf(stderr, "orion2-router: %s\n", message);
}

void Router_Service_Logf_(s_router_service_state* state, const char* format, uint32_t value_a, uint32_t value_b) {
    char message[256]{};
    std::snprintf(message, sizeof(message), format, value_a, value_b);
    Router_Service_Log_(state, message);
}

void Router_Service_Close_Handle_(uv_handle_t* handle) {
    if (handle != nullptr && !uv_is_closing(handle)) {
        uv_close(handle, nullptr);
    }
}

int Router_Service_Start_Stop_Async_(s_router_service_state* state) {
    const int result = uv_async_init(&state->loop, &state->stop_async, Router_Service_Stop_Async_Callback_);
    if (result != 0) {
        return result;
    }
    state->stop_async.data = state;
    state->stop_async_initialized = true;
    return 0;
}

int Router_Service_Start_Signals_(s_router_service_state* state) {
    int result = uv_signal_init(&state->loop, &state->sigint);
    if (result != 0) {
        return result;
    }
    state->sigint.data = state;
    state->sigint_initialized = true;
    result = uv_signal_start(&state->sigint, Router_Service_Signal_Callback_, SIGINT);
    if (result != 0) {
        return result;
    }

    result = uv_signal_init(&state->loop, &state->sigterm);
    if (result != 0) {
        return result;
    }
    state->sigterm.data = state;
    state->sigterm_initialized = true;
    return uv_signal_start(&state->sigterm, Router_Service_Signal_Callback_, SIGTERM);
}

int Router_Service_Start_Tcp_Listener_(s_router_service_state* state) {
    int result = uv_tcp_init(&state->loop, &state->tcp_server);
    if (result != 0) {
        return result;
    }
    state->tcp_server.data = state;
    state->tcp_server_initialized = true;

    sockaddr_in address{};
    result = uv_ip4_addr(state->config.listen_host, state->config.tcp_port, &address);
    if (result != 0) {
        return result;
    }

    result = uv_tcp_bind(&state->tcp_server, (const sockaddr*)&address, 0);
    if (result != 0) {
        return result;
    }

    result = uv_listen((uv_stream_t*)&state->tcp_server, 128, Router_Service_Tcp_Connection_Callback_);
    if (result != 0) {
        return result;
    }

    sockaddr_in bound_address{};
    int bound_address_size = sizeof(bound_address);
    result = uv_tcp_getsockname(&state->tcp_server, (sockaddr*)&bound_address, &bound_address_size);
    if (result != 0) {
        return result;
    }
    state->bound_tcp_port.store((uint16_t)ntohs(bound_address.sin_port));
    return 0;
}

int Router_Service_Start_Udp_Discovery_(s_router_service_state* state) {
    int result = uv_udp_init(&state->loop, &state->udp_socket);
    if (result != 0) {
        return result;
    }
    state->udp_socket.data = state;
    state->udp_socket_initialized = true;

    sockaddr_in address{};
    result = uv_ip4_addr("0.0.0.0", state->config.discovery_port, &address);
    if (result != 0) {
        return result;
    }

    result = uv_udp_bind(&state->udp_socket, (const sockaddr*)&address, UV_UDP_REUSEADDR);
    if (result != 0) {
        return result;
    }

    sockaddr_in bound_address{};
    int bound_address_size = sizeof(bound_address);
    result = uv_udp_getsockname(&state->udp_socket, (sockaddr*)&bound_address, &bound_address_size);
    if (result != 0) {
        return result;
    }
    state->bound_discovery_port.store((uint16_t)ntohs(bound_address.sin_port));

    return uv_udp_recv_start(&state->udp_socket, Router_Service_Alloc_Callback_, Router_Service_Udp_Recv_Callback_);
}

void Router_Service_Send_Output_(s_router_service_state* state, const s_router_outbound_message* output);
void Router_Service_Send_Protocol_Error_And_Close_(s_router_service_state* state, s_router_client_connection* connection, const char* message);

void Router_Service_Send_Outputs_(s_router_service_state* state, const std::vector<s_router_outbound_message>* output) {
    for (const s_router_outbound_message& message : *output) {
        Router_Service_Send_Output_(state, &message);
    }
}

void Router_Service_Close_Client_(s_router_service_state* state, s_router_client_connection* connection, bool notify_core) {
    if (connection == nullptr || connection->closing) {
        return;
    }

    connection->closing = true;
    uv_read_stop((uv_stream_t*)&connection->tcp);
    std::vector<s_router_outbound_message> output;
    Router_Core_Close_Connection(&state->core, connection->connection_id, &output);
    if (notify_core) {
        Router_Service_Send_Outputs_(state, &output);
    }
    uv_close((uv_handle_t*)&connection->tcp, Router_Service_Client_Close_Callback_);
}

void Router_Service_Begin_Shutdown_(s_router_service_state* state) {
    if (state->shutting_down) {
        return;
    }
    state->shutting_down = true;

    std::vector<s_router_client_connection*> clients;
    clients.reserve(state->clients.size());
    for (auto& entry : state->clients) {
        clients.push_back(entry.second);
    }

    for (s_router_client_connection* connection : clients) {
        Router_Service_Close_Client_(state, connection, false);
    }

    if (state->tcp_server_initialized) {
        Router_Service_Close_Handle_((uv_handle_t*)&state->tcp_server);
    }
    if (state->udp_socket_initialized) {
        Router_Service_Close_Handle_((uv_handle_t*)&state->udp_socket);
    }
    if (state->sigint_initialized) {
        uv_signal_stop(&state->sigint);
        Router_Service_Close_Handle_((uv_handle_t*)&state->sigint);
    }
    if (state->sigterm_initialized) {
        uv_signal_stop(&state->sigterm);
        Router_Service_Close_Handle_((uv_handle_t*)&state->sigterm);
    }
    if (state->stop_async_initialized) {
        Router_Service_Close_Handle_((uv_handle_t*)&state->stop_async);
    }
}

void Router_Service_Send_Output_(s_router_service_state* state, const s_router_outbound_message* output) {
    auto connection_it = state->clients.find(output->connection_id);
    if (connection_it == state->clients.end() || connection_it->second->closing) {
        return;
    }

    s_router_write_request* write_request = new s_router_write_request{};
    Encode_Frame(&output->message, &write_request->data);
    write_request->byte_count = write_request->data.Size();

    s_router_client_connection* connection = connection_it->second;
    if (write_request->byte_count > state->config.max_pending_write_bytes ||
        connection->pending_write_bytes > state->config.max_pending_write_bytes - write_request->byte_count) {
        Router_Service_Logf_(
            state,
            "dropping outbound router message for connection %u because pending write bytes would exceed %u",
            connection->connection_id,
            state->config.max_pending_write_bytes);
        delete write_request;
        return;
    }

    connection->pending_write_bytes += write_request->byte_count;

    write_request->buffer = uv_buf_init((char*)write_request->data.Data(), write_request->data.Size());
    write_request->request.data = write_request;
    write_request->connection = connection;

    const int result = uv_write(&write_request->request, (uv_stream_t*)&connection->tcp, &write_request->buffer, 1, Router_Service_Write_Callback_);
    if (result != 0) {
        connection->pending_write_bytes -= write_request->byte_count;
        Router_Service_Logf_(state, "closing connection %u after uv_write failed with %u", connection->connection_id, (uint32_t)(-result));
        delete write_request;
        Router_Service_Close_Client_(state, connection, true);
    }
}

void Router_Service_Send_Protocol_Error_And_Close_(s_router_service_state* state, s_router_client_connection* connection, const char* message) {
    if (connection == nullptr || connection->closing) {
        return;
    }

    s_router_message error_message;
    error_message.header.kind = ROUTER_MESSAGE_ERROR;
    error_message.header.target_network_id = Router_Core_Client_Network_Id(&state->core, connection->connection_id);
    Encode_Error_Payload(ROUTER_ERROR_INVALID_MESSAGE, message != nullptr ? message : "protocol error", &error_message.payload);

    Router_Service_Log_(state, message != nullptr ? message : "protocol error; closing connection");

    connection->closing = true;
    uv_read_stop((uv_stream_t*)&connection->tcp);

    std::vector<s_router_outbound_message> output;
    Router_Core_Close_Connection(&state->core, connection->connection_id, &output);
    Router_Service_Send_Outputs_(state, &output);

    s_router_write_request* write_request = new s_router_write_request{};
    Encode_Frame(&error_message, &write_request->data);
    write_request->byte_count = write_request->data.Size();
    write_request->buffer = uv_buf_init((char*)write_request->data.Data(), write_request->data.Size());
    write_request->request.data = write_request;
    write_request->connection = connection;
    write_request->close_after_write = true;

    const int result = uv_write(&write_request->request, (uv_stream_t*)&connection->tcp, &write_request->buffer, 1, Router_Service_Write_Callback_);
    if (result != 0) {
        delete write_request;
        uv_close((uv_handle_t*)&connection->tcp, Router_Service_Client_Close_Callback_);
    }
}

void Router_Service_Send_Discovery_Reply_(s_router_service_state* state, const sockaddr* address) {
    if (address == nullptr || !state->udp_socket_initialized || uv_is_closing((uv_handle_t*)&state->udp_socket)) {
        return;
    }

    s_router_udp_send_request* send_request = new s_router_udp_send_request{};
    Build_Discovery_Reply(&state->config, state->bound_tcp_port.load(), &send_request->data);
    send_request->buffer = uv_buf_init((char*)send_request->data.Data(), send_request->data.Size());
    send_request->request.data = send_request;

    const int result = uv_udp_send(&send_request->request, &state->udp_socket, &send_request->buffer, 1, address, Router_Service_Udp_Send_Callback_);
    if (result != 0) {
        delete send_request;
    }
}

void Router_Service_Alloc_Callback_(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buffer) {
    (void)handle;
    const size_t allocation_size = suggested_size == 0 ? 4096 : suggested_size;
    buffer->base = (char*)std::malloc(allocation_size);
    buffer->len = (unsigned int)allocation_size;
}

void Router_Service_Tcp_Connection_Callback_(uv_stream_t* server, int status) {
    s_router_service_state* state = (s_router_service_state*)server->data;
    if (status != 0 || state == nullptr || state->shutting_down) {
        return;
    }

    s_router_client_connection* connection = new s_router_client_connection{};
    connection->service = state;
    connection->connection_id = state->next_connection_id++;

    int result = uv_tcp_init(&state->loop, &connection->tcp);
    if (result != 0) {
        delete connection;
        return;
    }
    connection->tcp.data = connection;

    result = uv_accept(server, (uv_stream_t*)&connection->tcp);
    if (result != 0) {
        uv_close((uv_handle_t*)&connection->tcp, Router_Service_Client_Close_Callback_);
        return;
    }

    uv_tcp_nodelay(&connection->tcp, 1);
    state->clients[connection->connection_id] = connection;
    Router_Core_Open_Connection(&state->core, connection->connection_id);

    result = uv_read_start((uv_stream_t*)&connection->tcp, Router_Service_Alloc_Callback_, Router_Service_Read_Callback_);
    if (result != 0) {
        Router_Service_Close_Client_(state, connection, true);
    }
}

void Router_Service_Read_Callback_(uv_stream_t* stream, ssize_t bytes_read, const uv_buf_t* buffer) {
    s_router_client_connection* connection = (s_router_client_connection*)stream->data;
    if (connection == nullptr || connection->service == nullptr) {
        if (buffer != nullptr) {
            std::free(buffer->base);
        }
        return;
    }

    s_router_service_state* state = connection->service;
    if (bytes_read > 0) {
        const uint8_t* bytes = (const uint8_t*)buffer->base;
        connection->read_buffer.insert(connection->read_buffer.end(), bytes, bytes + bytes_read);

        for (;;) {
            s_router_message message;
            std::string error_message;
            const e_frame_read_status status = Try_Read_Frame(&connection->read_buffer, state->config.max_frame_size, &message, &error_message);
            if (status == FRAME_READ_NEED_MORE_DATA) {
                break;
            }
            if (status == FRAME_READ_PROTOCOL_ERROR) {
                Router_Service_Send_Protocol_Error_And_Close_(state, connection, error_message.c_str());
                break;
            }

            std::vector<s_router_outbound_message> output;
            Router_Core_Handle_Message(&state->core, connection->connection_id, &message, &output);
            Router_Service_Send_Outputs_(state, &output);
            if (connection->closing) {
                break;
            }
        }
    } else if (bytes_read < 0) {
        Router_Service_Close_Client_(state, connection, true);
    }

    if (buffer != nullptr) {
        std::free(buffer->base);
    }
}

void Router_Service_Write_Callback_(uv_write_t* request, int status) {
    s_router_write_request* write_request = (s_router_write_request*)request->data;
    if (write_request != nullptr && write_request->connection != nullptr && write_request->byte_count <= write_request->connection->pending_write_bytes) {
        write_request->connection->pending_write_bytes -= write_request->byte_count;
    }
    if (write_request != nullptr && write_request->connection != nullptr && write_request->close_after_write) {
        if (!uv_is_closing((uv_handle_t*)&write_request->connection->tcp)) {
            uv_close((uv_handle_t*)&write_request->connection->tcp, Router_Service_Client_Close_Callback_);
        }
    } else if (status != 0 && write_request != nullptr && write_request->connection != nullptr) {
        s_router_client_connection* connection = write_request->connection;
        if (connection->service != nullptr) {
            Router_Service_Close_Client_(connection->service, connection, true);
        }
    }
    delete write_request;
}

void Router_Service_Client_Close_Callback_(uv_handle_t* handle) {
    s_router_client_connection* connection = (s_router_client_connection*)handle->data;
    if (connection == nullptr) {
        return;
    }

    s_router_service_state* state = connection->service;
    if (state != nullptr) {
        state->clients.erase(connection->connection_id);
    }
    delete connection;
}

void Router_Service_Stop_Async_Callback_(uv_async_t* handle) {
    s_router_service_state* state = (s_router_service_state*)handle->data;
    if (state != nullptr) {
        Router_Service_Begin_Shutdown_(state);
    }
}

void Router_Service_Signal_Callback_(uv_signal_t* handle, int signum) {
    (void)signum;
    s_router_service_state* state = (s_router_service_state*)handle->data;
    if (state != nullptr) {
        Router_Service_Begin_Shutdown_(state);
    }
}

void Router_Service_Udp_Recv_Callback_(uv_udp_t* handle, ssize_t bytes_read, const uv_buf_t* buffer, const sockaddr* address, unsigned flags) {
    (void)flags;
    s_router_service_state* state = (s_router_service_state*)handle->data;
    if (state != nullptr && bytes_read > 0 && address != nullptr) {
        if (Is_Discovery_Probe((const uint8_t*)buffer->base, (uint32_t)bytes_read)) {
            Router_Service_Send_Discovery_Reply_(state, address);
        }
    }

    if (buffer != nullptr) {
        std::free(buffer->base);
    }
}

void Router_Service_Udp_Send_Callback_(uv_udp_send_t* request, int status) {
    (void)status;
    s_router_udp_send_request* send_request = (s_router_udp_send_request*)request->data;
    delete send_request;
}

}

void Build_Discovery_Reply(const s_router_config* config, uint16_t bound_tcp_port, Bit_Stream* output) {
    Encode_Discovery_Reply(
        bound_tcp_port,
        config != nullptr ? config->router_id : "",
        config != nullptr ? config->router_name : "",
        output);
}

s_router_service::s_router_service()
    : state(std::make_unique<s_router_service_state>()) {
}

s_router_service::~s_router_service() = default;

void Router_Service_Init(s_router_service* service, const s_router_config* config) {
    if (service == nullptr) {
        return;
    }

    if (service->state == nullptr) {
        service->state = std::make_unique<s_router_service_state>();
    }

    service->state->config = config != nullptr ? *config : s_router_config{};
    Router_Core_Init(&service->state->core, service->state->config.router_id);
}

int Router_Service_Run(s_router_service* service) {
    if (service == nullptr || service->state == nullptr) {
        return UV_EINVAL;
    }

    s_router_service_state* state = service->state.get();
    bool expected = false;
    if (!state->running.compare_exchange_strong(expected, true)) {
        return UV_EBUSY;
    }

    state->stop_requested.store(false);
    state->bound_tcp_port.store(0);
    state->shutting_down = false;

    int result = uv_loop_init(&state->loop);
    if (result != 0) {
        state->running.store(false);
        return result;
    }
    state->loop_initialized = true;

    result = Router_Service_Start_Stop_Async_(state);
    if (result == 0 && state->config.enable_signal_handlers) {
        result = Router_Service_Start_Signals_(state);
    }
    if (result == 0) {
        result = Router_Service_Start_Tcp_Listener_(state);
    }
    if (result == 0 && state->config.enable_discovery) {
        result = Router_Service_Start_Udp_Discovery_(state);
    }

    if (result != 0 || state->stop_requested.load()) {
        Router_Service_Begin_Shutdown_(state);
    }

    uv_run(&state->loop, UV_RUN_DEFAULT);

    while (uv_loop_close(&state->loop) == UV_EBUSY) {
        uv_run(&state->loop, UV_RUN_NOWAIT);
    }

    state->loop_initialized = false;
    state->tcp_server_initialized = false;
    state->udp_socket_initialized = false;
    state->stop_async_initialized = false;
    state->sigint_initialized = false;
    state->sigterm_initialized = false;
    state->clients.clear();
    state->bound_tcp_port.store(0);
    state->bound_discovery_port.store(0);
    state->running.store(false);
    return result;
}

void Router_Service_Request_Stop(s_router_service* service) {
    if (service == nullptr || service->state == nullptr) {
        return;
    }

    service->state->stop_requested.store(true);
    if (service->state->stop_async_initialized) {
        uv_async_send(&service->state->stop_async);
    }
}

bool Router_Service_Is_Running(const s_router_service* service) {
    if (service == nullptr || service->state == nullptr) {
        return false;
    }
    return service->state->running.load();
}

uint16_t Router_Service_Bound_Tcp_Port(const s_router_service* service) {
    if (service == nullptr || service->state == nullptr) {
        return 0;
    }
    return service->state->bound_tcp_port.load();
}

uint16_t Router_Service_Bound_Discovery_Port(const s_router_service* service) {
    if (service == nullptr || service->state == nullptr) {
        return 0;
    }
    return service->state->bound_discovery_port.load();
}

const s_router_config* Router_Service_Config(const s_router_service* service) {
    if (service == nullptr || service->state == nullptr) {
        return nullptr;
    }
    return &service->state->config;
}

}