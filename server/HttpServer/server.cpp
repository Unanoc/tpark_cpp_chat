#include <iostream>
#include <errno.h>
#include <event.h>
#include <evhttp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <jansson.h>
#include <pqxx/pqxx>

#include "server.hpp"
#include "json_converter.hpp"
#include "database_interaction/database_interaction.hpp"

#define HTTP_NOTACCEPTABLE 406
#define HTTP_FORBIDDEN 403

void logger(struct evhttp_request *request) {
    printf("URL '%s' from %s\n", evhttp_request_uri(request), evhttp_request_get_host(request)); 
}

void generic_handler(struct evhttp_request *request, void *arg) {
    logger(request);

    struct evbuffer *buf;
    buf = evbuffer_new();

    if (buf == NULL) {
        std::cout << "Failed to create response buffer " << strerror(errno) << std::endl;
    }

    evbuffer_add_printf(buf, "URL %s doesn't exist.", evhttp_request_uri(request));
    evhttp_send_reply(request, HTTP_BADMETHOD, "Bad Method", buf);
}

void registration_handler(struct evhttp_request *request, void *arg) {
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
    /* Making a response buffer */
    struct evbuffer *responseBuffer = evbuffer_new();

    if (requestJSON != NULL) {
        /* Getting perfect JSON string */
        requestDataString = json_dumps(requestJSON, JSON_INDENT(4));
        printf("%s\n", requestDataString);
        /* Put the message in DB */
        JsonConverter jsonConv;
        UserRegisterStruct user = jsonConv.fromJsonToUserRegister(requestJSON);
        /* Making a connection to PostgreSQL */
        pqxx::connection c;
        int user_id = add_user(c, user.username, user.password);

        if (user_id != 0) { /* if user does not exist */
            evhttp_send_reply(request, HTTP_OK, "OK", responseBuffer);
        } else {
            evhttp_send_reply(request, HTTP_NOTACCEPTABLE, "Not Acceptable", responseBuffer);
        }
        
        evbuffer_free(responseBuffer);
        free(requestDataString);
        json_decref(requestJSON);
    } else {
        snprintf(errorText, 1024, "Input error: on line %d: %s\n", error.line, error.text);
    }
}

void create_chat_handler(struct evhttp_request *request, void *arg) {
    logger(request);

    struct event_base *base = (struct event_base *)arg;
    struct evbuffer *requestBuffer = evhttp_request_get_input_buffer(request); 
    size_t requestLen = evbuffer_get_length(requestBuffer);
    char *requestDataString = (char *)malloc(sizeof(char) * requestLen);
    memset(requestDataString, 0, requestLen);
    evbuffer_copyout(requestBuffer, requestDataString, requestLen); //evbuffer_copyout automaticly frees requestBuffer
    char errorText[1024];
    json_error_t error;
    json_t *requestJSON = json_loadb(requestDataString, requestLen, 0, &error);

    struct evbuffer *responseBuffer = evbuffer_new();

    if (requestJSON != NULL) {
        requestDataString = json_dumps(requestJSON, JSON_INDENT(4));
        printf("%s\n", requestDataString);

        JsonConverter jsonConv;
        ChatCreateStruct chat = jsonConv.fromJsonToChatCreate(requestJSON);

        pqxx::connection c;
        int user_id = get_user_id_by_login_pasword(c, chat.username, chat.password);

        if (user_id != 0) {
            int chat_id = add_chat(c, user_id, chat.chat);
            if (chat_id != 0) {
                evhttp_send_reply(request, HTTP_OK, "OK", responseBuffer);
            } else {
                evhttp_send_reply(request, HTTP_NOTACCEPTABLE, "Not Acceptable", responseBuffer);
            }
        } else {
            evhttp_send_reply(request, HTTP_FORBIDDEN, "FORBIDDEN", responseBuffer);
        }
        
        evbuffer_free(responseBuffer);
        free(requestDataString);
        json_decref(requestJSON);
    } else {
        snprintf(errorText, 1024, "Input error: on line %d: %s\n", error.line, error.text);
    }
}

void invite_user_to_chat_handler(struct evhttp_request *request, void *arg) {
    logger(request);

    struct event_base *base = (struct event_base *)arg;
    struct evbuffer *requestBuffer = evhttp_request_get_input_buffer(request); 
    size_t requestLen = evbuffer_get_length(requestBuffer);
    char *requestDataString = (char *)malloc(sizeof(char) * requestLen);
    memset(requestDataString, 0, requestLen);
    evbuffer_copyout(requestBuffer, requestDataString, requestLen);
    char errorText[1024];
    json_error_t error;
    json_t *requestJSON = json_loadb(requestDataString, requestLen, 0, &error);

    struct evbuffer *responseBuffer = evbuffer_new();

    if (requestJSON != NULL) {
        requestDataString = json_dumps(requestJSON, JSON_INDENT(4));
        printf("%s\n", requestDataString);

        JsonConverter jsonConv;
        InviteToChatStruct invite = jsonConv.fromJsonToInviteChat(requestJSON);

        pqxx::connection c;
        int user_id = get_user_id_by_login_pasword(c, invite.username, invite.password);

        if (user_id != 0) {
            int chat_id = get_chat_id_by_user_id_chat_title(c, user_id, invite.chat);
            if (chat_id != 0) {
                int added_user_id = get_user_id_by_login(c, invite.new_person);
                if (added_user_id != 0) {
                    int adding_user = add_user_to_chat(c, added_user_id, chat_id);
                    if (adding_user != 0)
                        evhttp_send_reply(request, HTTP_OK, "OK", responseBuffer);
                    else
                        evhttp_send_reply(request, HTTP_NOTACCEPTABLE, "Not Acceptable", responseBuffer);
                } else {
                    evhttp_send_reply(request, HTTP_NOTACCEPTABLE, "Not Acceptable", responseBuffer);
                }
            } else {
                evhttp_send_reply(request, HTTP_FORBIDDEN, "FORBIDDEN", responseBuffer);
            }
        } else {
            evhttp_send_reply(request, HTTP_FORBIDDEN, "FORBIDDEN", responseBuffer);
        }

        evbuffer_free(responseBuffer);
        free(requestDataString);
        json_decref(requestJSON);
    } else {
        snprintf(errorText, 1024, "Input error: on line %d: %s\n", error.line, error.text);
    }
}

void send_message_handler(struct evhttp_request *request, void *arg) {
    logger(request);

    struct event_base *base = (struct event_base *)arg;
    struct evbuffer *requestBuffer = evhttp_request_get_input_buffer(request); 
    size_t requestLen = evbuffer_get_length(requestBuffer);
    char *requestDataString = (char *)malloc(sizeof(char) * requestLen);
    memset(requestDataString, 0, requestLen);
    evbuffer_copyout(requestBuffer, requestDataString, requestLen);
    char errorText[1024];
    json_error_t error;
    json_t *requestJSON = json_loadb(requestDataString, requestLen, 0, &error);

    struct evbuffer *responseBuffer = evbuffer_new();

    if (requestJSON != NULL) {
        requestDataString = json_dumps(requestJSON, JSON_INDENT(4));
        printf("%s\n", requestDataString);

        JsonConverter jsonConv;
        MessageSendStruct msg = jsonConv.fromJsonToMessageSend(requestJSON);

        pqxx::connection c;
        int user_id = get_user_id_by_login_pasword(c, msg.username, msg.password);

        if (user_id != 0) {
            int chat_id = get_chat_id_by_user_id_chat_title(c, user_id, msg.chat);
            if (chat_id != 0) {
                set_message_by_user_id_chat_id(c, user_id, chat_id, msg.text);
                evhttp_send_reply(request, HTTP_OK, "OK", responseBuffer);
            } else {
                evhttp_send_reply(request, HTTP_FORBIDDEN, "FORBIDDEN", responseBuffer);
            }
        } else {
            evhttp_send_reply(request, HTTP_FORBIDDEN, "FORBIDDEN", responseBuffer);
        }

        evbuffer_free(responseBuffer);
        free(requestDataString);
        json_decref(requestJSON);
    } else {
        snprintf(errorText, 1024, "Input error: on line %d: %s\n", error.line, error.text);
    }
}

void get_messages_handler(struct evhttp_request *request, void *arg) {
    logger(request);

    struct event_base *base = (struct event_base *)arg;
    struct evbuffer *requestBuffer = evhttp_request_get_input_buffer(request); 
    size_t requestLen = evbuffer_get_length(requestBuffer);
    char *requestDataString = (char *)malloc(sizeof(char) * requestLen);
    memset(requestDataString, 0, requestLen);
    evbuffer_copyout(requestBuffer, requestDataString, requestLen); //evbuffer_copyout automaticly frees requestBuffer
    char errorText[1024];
    json_error_t error;
    json_t *requestJSON = json_loadb(requestDataString, requestLen, 0, &error);

    struct evbuffer *responseBuffer = evbuffer_new();

    if (requestJSON != NULL) {
        requestDataString = json_dumps(requestJSON, JSON_INDENT(4));
        printf("%s\n", requestDataString);

        JsonConverter jsonConv;
        MessageGetReqStruct user = jsonConv.fromJsonToMessageGetReqStruct(requestJSON);

        pqxx::connection c;
        int user_id = get_user_id_by_login_pasword(c, user.username, user.password);

        if (user_id != 0) {
            std::vector<MessageGetStruct> messages = get_from_chats_by_user_id_time(c, user_id, user.last_update);
            for (int i = 0; i < messages.size(); i++) {
                std::cout << messages[i].username << " " << messages[i].text << std::endl;
                // TODO
            }
            evhttp_send_reply(request, HTTP_OK, "OK", responseBuffer);
        } else {
            evhttp_send_reply(request, HTTP_FORBIDDEN, "FORBIDDEN", responseBuffer);
        }
        
        evbuffer_free(responseBuffer);
        free(requestDataString);
        json_decref(requestJSON);
    } else {
        snprintf(errorText, 1024, "Input error: on line %d: %s\n", error.line, error.text);
    }
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
