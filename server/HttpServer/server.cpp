#include <errno.h>
#include <event.h>
#include <evhttp.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <jansson.h>

#include <iostream>
#include "server.h"


#define HEADER_BUFFER_SIZE 1024
#define ERROR_RESPONSE_SIZE 1024


void generic_handler(struct evhttp_request *request, void *arg) {
    struct evbuffer *buf;
    buf = evbuffer_new();

    if (buf == NULL) {
      std::cout << "Failed to create response buffer " << strerror(errno) << std::endl;
    }

    evbuffer_add_printf(buf, "Requested: %s \n", evhttp_request_uri(request)); //сюда можно запихивать созданный json-ответ
    evhttp_send_reply(request, HTTP_OK, "OK", buf);
}


void send_message_response(struct evhttp_request *request, json_t *requestJSON, struct event_base *base) {
   // Reponse
   json_t *responseJSON;
   json_t *message;

   char responseHeader[HEADER_BUFFER_SIZE];

   char *responseData;
   int responseLen;
   struct evbuffer *responseBuffer;

   // Create JSON response data
   responseJSON = json_object();

   message = json_string("Hello World");
   json_object_set_new(responseJSON, "message", message);

   // dump JSON to buffer and store response length as string
   responseData = json_dumps(responseJSON, JSON_INDENT(3));
   responseLen = strlen(responseData);
   snprintf(responseHeader, HEADER_BUFFER_SIZE, "%d", (int)responseLen);
   json_decref(responseJSON);

   // create a response buffer to send reply
   responseBuffer = evbuffer_new();

   // add appropriate headers
   evhttp_add_header(request->output_headers, "Content-Type", "application/json");
   evhttp_add_header(request->output_headers, "Content-Length", responseHeader);

   evbuffer_add(responseBuffer, responseData, responseLen);

   // send the reply
   evhttp_send_reply(request, 200, "OK", responseBuffer); 

   evbuffer_free(responseBuffer);
}



void send_message_handler(struct evhttp_request *request, void *arg) {
   struct event_base *base = (struct event_base *)arg;

   // Request
   struct evbuffer *requestBuffer;
   size_t requestLen;
   char *requestDataBuffer;

   json_t *requestJSON;
   json_error_t error;

   // Error buffer
   char errorText[ERROR_RESPONSE_SIZE];

   // Process Request
   requestBuffer = evhttp_request_get_input_buffer(request);
   requestLen = evbuffer_get_length(requestBuffer);

   requestDataBuffer = (char *)malloc(sizeof(char) * requestLen);
   memset(requestDataBuffer, 0, requestLen);
   evbuffer_copyout(requestBuffer, requestDataBuffer, requestLen);

   printf("%s\n", evhttp_request_uri(request));

   requestJSON = json_loadb(requestDataBuffer, requestLen, 0, &error);
   free(requestDataBuffer);

   if (requestJSON == NULL) {
      snprintf(errorText, ERROR_RESPONSE_SIZE, "Input error: on line %d: %s\n", error.line, error.text);
   } 
   else {
      // Debug out
      requestDataBuffer = json_dumps(requestJSON, JSON_INDENT(3));
      printf("%s\n", requestDataBuffer);
      free(requestDataBuffer);

      send_message_response(request, requestJSON, base);
      json_decref(requestJSON);
   }
   return;
}
