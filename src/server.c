#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <poll.h>
#include <errno.h>

#include "libserver_internal.h"

static const unsigned int SERVER_DEFAULT_BUFSIZ = 1024;

#define GEN_SET_CALLBACK_FUNCTION(event)                              \
void                                                                  \
server_set_##event##_callback(server_t *server,                       \
                              server_##event##_callback_t callback) { \
   server->on_##event = callback;                                     \
}                                                                     \

GEN_SET_CALLBACK_FUNCTION(accept)
GEN_SET_CALLBACK_FUNCTION(data)
GEN_SET_CALLBACK_FUNCTION(close)

#define DISPATCH_METHOD_TO_SERVER_ARGS(method, server, ...) \
    (server->type == SERVER_TYPE_TCP) ?                     \
        tcp_server_##method(server, __VA_ARGS__) :          \
        udp_server_##method(server, __VA_ARGS__)            \

#define DISPATCH_METHOD_TO_SERVER_NOARGS(method, server) \
    (server->type == SERVER_TYPE_TCP) ?                  \
        tcp_server_##method(server) :                    \
        udp_server_##method(server)                      \

server_t *
server_new(server_type_t type, int port) {
    server_t *server = calloc(1, sizeof(server_t));
    server->type = type;
    server->bufsiz = SERVER_DEFAULT_BUFSIZ;

    DISPATCH_METHOD_TO_SERVER_ARGS(setup_socket, server, port);
    return server;
}

void
server_destroy(server_t *server) {
    close(server->listen_fd);
    free(server);
}

void
server_loop(server_t *server) {
    DISPATCH_METHOD_TO_SERVER_NOARGS(loop, server);
    return;
}

void
server_mux(server_t **servers, int nservers) {
    struct pollfd spoll[nservers];
    memset(spoll, 0, nservers * sizeof(struct pollfd));

    for (int i = 0; i < nservers; i++) {
        spoll[i].fd = servers[i]->listen_fd;
        spoll[i].events = POLLIN;
    }

    while (true) {
        if (poll(spoll, nservers, -1) < 0 && errno != EINTR) return;

        for (int i = 0; i < nservers; i++) {
            if (spoll[i].revents & POLLIN) {
                // new connection on this socket!
                DISPATCH_METHOD_TO_SERVER_NOARGS(accept, servers[i]);
            }
        }
    }
}
