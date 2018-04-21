#include <iostream>
#include <errno.h>
#include <event.h>
#include <evhttp.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <jansson.h>

#include "server.h"


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


    /* MOVE IT IN CEPARATE CLASS IN THE FUTURE */
    /* Set callbacks for different requests. */
    evhttp_set_cb(http_server, "/send_message", send_message_handler, NULL);

    /*
    	Other handlers will place here
    */

    /* Set a callback for all other requests. */
    evhttp_set_gencb(http_server, generic_handler, NULL);



    fprintf(stderr, "Server started on %s port %d\n", http_addr, http_port);

    event_dispatch();    /* Not reached in this code as it is now. */
    evhttp_free(http_server);
    return 0;
}