#include <errno.h>
#include <event.h>
#include <evhttp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>


void brokenPipe(int signum) {
   fprintf(stderr, "Broken Pipe (%d)\n", signum);
}



void generic_handler(struct evhttp_request *request, void *arg) {
    struct evbuffer *buf;
    buf = evbuffer_new();

    if (buf == NULL) {
    	std::cout << "Failed to create response buffer " << strerror(errno) << std::endl;
    }

    evbuffer_add_printf(buf, "Requested: %s \n", evhttp_request_uri(request)); //сюда можно запихивать созданный json-ответ
    evhttp_send_reply(request, HTTP_OK, "OK", buf);
}


void send_message_handler(struct evhttp_request *request, void *arg)
{
    struct evbuffer *buf;
    buf = evbuffer_new();

    if (buf == NULL) {
    	std::cout << "Failed to create response buffer " << strerror(errno) << std::endl;
    }

    evbuffer_add_printf(buf, "%s", "hello"); //сюда можно запихивать созданный json-ответ
    evhttp_send_reply(request, HTTP_OK, "OK", buf);
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

   	// don't exit on broken pipe (just fail with message)
   	signal(SIGPIPE, brokenPipe);

    event_init();
    http_server = evhttp_start(http_addr, http_port);

    std::cout << "Server is running ..." << std::endl << "http://" << http_addr << ":" << http_port << std::endl;


    /* Set a callback for different requests. */
    evhttp_set_cb(http_server, "/send_message", send_message_handler, NULL);
    /*
    	other handlers will place here
    */


    /* Set a callback for all other requests. */
    evhttp_set_gencb(http_server, generic_handler, NULL);

    event_dispatch();    /* Not reached in this code as it is now. */
    evhttp_free(http_server);
    
    return 0;
}