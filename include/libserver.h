#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct server server_t;

typedef enum {
    SERVER_TYPE_TCP,
    SERVER_TYPE_UDP
} server_type_t;

typedef void (*server_accept_callback_t)(int fd, const struct sockaddr *peer);
typedef int (*server_data_callback_t)(int fd, const char *data, size_t len);
typedef void (*server_close_callback_t)(int fd, const struct sockaddr *peer);

server_t *server_new(server_type_t type, int port);
void server_destroy(server_t *server);
void server_set_accept_callback(server_t *server, server_accept_callback_t callback);
void server_set_data_callback(server_t *server, server_data_callback_t callback);
void server_set_close_callback(server_t *server, server_close_callback_t callback);
void server_loop(server_t *server);
void server_mux(server_t **servers, int nservers);
