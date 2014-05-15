#pragma once

#include "libserver.h"

struct server {
    server_type_t type;
    int listen_fd;
    size_t bufsiz;
    server_accept_callback_t on_accept;
    server_data_callback_t on_data;
    server_accept_callback_t on_close;
};

void udp_server_setup_socket(server_t *server, int port);
void udp_server_loop(server_t *server);
void udp_server_accept(server_t *server);

void tcp_server_setup_socket(server_t *server, int port);
void tcp_server_loop(server_t *server);
void tcp_server_accept(server_t *server);
