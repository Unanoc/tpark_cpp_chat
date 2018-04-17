#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H


void generic_handler(struct evhttp_request *request, void *arg);
void send_message_response(struct evhttp_request *request, json_t *requestJSON, struct event_base *base);
void send_message_handler(struct evhttp_request *request, void *arg)




#endif SERVER_SERVER_H