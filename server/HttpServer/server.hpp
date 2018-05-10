#ifndef SERVER_H
#define SERVER_H

#include <event.h>
#include <evhttp.h>
#include <jansson.h>

/* Handler for other requests */
void generic_handler(struct evhttp_request *request, void *arg);

/* Registration */
void registration_handler(struct evhttp_request *request, void *arg);

/* Creating a new chat */
void create_chat_handler(struct evhttp_request *request, void *arg);

/* Sending a message*/
void send_message_handler(struct evhttp_request *request, void *arg);


#endif 