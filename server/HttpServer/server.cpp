#include <iostream>
#include <errno.h>
#include <event.h>
#include <evhttp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <jansson.h>
#include <pqxx/pqxx>

#include "server.h"
#include "json_converter.h"
#include "database_interaction/database_interaction.hpp"

void logger(struct evhttp_request *request) {
   printf("URL '%s' from %s\n", evhttp_request_uri(request), evhttp_request_get_host(request)); 
}


void generic_handler(struct evhttp_request *request, void *arg) {
    struct evbuffer *buf;
    buf = evbuffer_new();

    if (buf == NULL) {
      std::cout << "Failed to create response buffer " << strerror(errno) << std::endl;
    }

    logger(request);
    evbuffer_add_printf(buf, "URL %s doesn't exist.", evhttp_request_uri(request)); //сюда можно запихивать созданный json-ответ
    evhttp_send_reply(request, HTTP_BADMETHOD, "Bad Method", buf);
}


void send_message_handler(struct evhttp_request *request, void *arg) {
   logger(request);

   struct event_base *base = (struct event_base *)arg;
   /* Getting data body from POST request */
   struct evbuffer *requestBuffer = evhttp_request_get_input_buffer(request); 
   /* Getting length of body from POST request */
   size_t requestLen = evbuffer_get_length(requestBuffer);


   /* Getting data body from structure to string, but there is not only json in this string */
   char *requestDataString = (char *)malloc(sizeof(char) * requestLen);
   memset(requestDataString, 0, requestLen);
   evbuffer_copyout(requestBuffer, requestDataString, requestLen); //evbuffer_copyout automaticly frees requestBuffer

   char errorText[1024];
   json_error_t error;
   /* Getting JSON instance */
   json_t *requestJSON = json_loadb(requestDataString, requestLen, 0, &error);

   if (requestJSON != NULL) {
      /* May be in future this printing of JSON will be usefull, but now it's temporary for debug */
      /* Getting perfect JSON string */
      requestDataString = json_dumps(requestJSON, JSON_INDENT(3));
      /* Print JSON for debug*/
      printf("%s\n", requestDataString);

      /* Put the message in DB */
      JsonConverter jsonConv;
      MessageSend msg = jsonConv.fromJsonToMessage(requestJSON);

      struct evbuffer *responseBuffer = evbuffer_new();
      evbuffer_add(responseBuffer, requestDataString, strlen(requestDataString));

      // pqxx::connection connection_db;
      // int user_id = get_user_id_by_login_pasword(connection_db, std::string &username, std::string &password_hash);

      // if (user_id == 0) {

      // }


      evhttp_send_reply(request, HTTP_OK, "OK", responseBuffer); //delete it in the future

      evbuffer_free(responseBuffer);
      free(requestDataString);
      json_decref(requestJSON);
   } else 
      snprintf(errorText, 1024, "Input error: on line %d: %s\n", error.line, error.text);
   
   return;
}


// void send_message_response(struct evhttp_request *request, json_t *requestJSON, struct event_base *base) {
//    // Reponse
//    json_t *responseJSON = requestJSON;
//    json_t *message;

//    char responseHeader[1024];

//    char *responseData;
//    int responseLen;
//    struct evbuffer *responseBuffer;

//    // Create JSON response data
//    // responseJSON = json_object();

//    message = json_string("date");
//    json_object_set_new(responseJSON, "18.04.18", message);

//    // dump JSON to buffer and store response length as string
//    responseData = json_dumps(responseJSON, JSON_INDENT(3));
//    responseLen = strlen(responseData);
//    snprintf(responseHeader, HEADER_BUFFER_SIZE, "%d", (int)responseLen);
//    // json_decref(responseJSON);

//    // create a response buffer to send reply
//    responseBuffer = evbuffer_new();

//    // add appropriate headers
//    evhttp_add_header(request->output_headers, "Content-Type", "application/json");
//    evhttp_add_header(request->output_headers, "Content-Length", responseHeader);

//    evbuffer_add(responseBuffer, responseData, responseLen);

//    // send the reply
//    evhttp_send_reply(request, 200, "OK", responseBuffer); 

//    evbuffer_free(responseBuffer);
// }
