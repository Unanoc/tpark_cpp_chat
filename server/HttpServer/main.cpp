#include <iostream>
#include <errno.h>
#include <event.h>
#include <evhttp.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <jansson.h>

#include "server.hpp"

void brokenPipe(int signum) {
    fprintf(stderr, "Broken Pipe (%d)\n", signum);
}

int main(int argc, char **argv) {
    short http_port = 8000;
    const char *http_addr = "127.0.0.1";
    struct evhttp *http_server = NULL;

    if (argc > 1) {
        http_addr = argv[1];
        
        if (argc > 2) {
            http_port = atol(argv[2]);
        }
    }
    /* Don't exit on broken pipe (just fail with message). It's just recomended... */
    signal(SIGPIPE, brokenPipe);

    event_init();
    http_server = evhttp_start(http_addr, http_port);

    evhttp_set_cb(http_server, "/send_message", send_message_handler, NULL);
    evhttp_set_cb(http_server, "/registration", registration_handler, NULL);
    evhttp_set_cb(http_server, "/create_chat", create_chat_handler, NULL);
    evhttp_set_cb(http_server, "/invite_user_to_chat", invite_user_to_chat_handler, NULL);
    evhttp_set_cb(http_server, "/get_messages", get_messages_handler, NULL);
    evhttp_set_gencb(http_server, generic_handler, NULL);

    fprintf(stderr, "Server started on %s port %d\n", http_addr, http_port);

    event_dispatch();
    evhttp_free(http_server);
    return 0;
}