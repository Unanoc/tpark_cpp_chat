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
#include "structures.h"


#define HEADER_BUFFER_SIZE 1024
#define ERROR_RESPONSE_SIZE 1024



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
    evhttp_send_reply(request, HTTP_OK, "OK", buf);
}


// void send_message_response(struct evhttp_request *request, json_t *requestJSON, struct event_base *base) {
//    // Reponse
//    json_t *responseJSON = requestJSON;
//    json_t *message;

//    char responseHeader[HEADER_BUFFER_SIZE];

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



struct message send_message_JSON_parser(json_t *requestJSON);




void send_message_handler(struct evhttp_request *request, void *arg) {
   struct event_base *base = (struct event_base *)arg;

   /* Getting data body from POST request */
   struct evbuffer *requestBuffer = evhttp_request_get_input_buffer(request); 
   /* Getting length of body from POST request */
   size_t requestLen = evbuffer_get_length(requestBuffer);


   /* Getting data body from structure to string, but there is not only json in this string */
   char *requestDataString = (char *)malloc(sizeof(char) * requestLen);
   memset(requestDataString, 0, requestLen);
   evbuffer_copyout(requestBuffer, requestDataString, requestLen); //evbuffer_copyout automaticly frees requestBuffer


   logger(request);


   char errorText[ERROR_RESPONSE_SIZE];
   json_error_t error;

   /* Getting JSON instance */
   json_t *requestJSON = json_loadb(requestDataString, requestLen, 0, &error);
   /* Empty this string */
   free(requestDataString);




   if (requestJSON != NULL) {

      /* May be in future this printing of JSON will be usefull, but now it's temporary */
      /* Getting perfect JSON string */
      requestDataString = json_dumps(requestJSON, JSON_INDENT(3));
      /* Print JSON */
      printf("%s\n", requestDataString);
      free(requestDataString);


      /* Put the message in DB */
      message msg = send_message_JSON_parser(requestJSON);
      // put_message_in_db(&msg);

      // printf("%d %d %s\n", msg.sender, msg.chat, msg.text);


      /* After all, send 200 OK to client */
      struct evbuffer *responseBuffer;
      responseBuffer = evbuffer_new();
      evhttp_send_reply(request, HTTP_OK, "OK", responseBuffer);

      evbuffer_free(responseBuffer);
      json_decref(requestJSON);

   } else 
      snprintf(errorText, ERROR_RESPONSE_SIZE, "Input error: on line %d: %s\n", error.line, error.text);
   
   return;
}


struct message send_message_JSON_parser(json_t *requestJSON) {
      json_t *sender_json, *chat_json, *text_json;

      sender_json = json_object_get(requestJSON, "sender");
      chat_json = json_object_get(requestJSON, "chat");
      text_json = json_object_get(requestJSON, "text");

      int sender = atoi(json_string_value(sender_json));
      int chat = atoi(json_string_value(chat_json));
      const char * text = json_string_value(text_json);

      message msg(sender, chat, text);
      
      printf("%d %d %s\n", msg.sender, msg.chat, msg.text);
      return msg;
}



