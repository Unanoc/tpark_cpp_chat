#ifndef JSON_CONVERTER
#define JSON_CONVERTER

#include <iostream>
#include <errno.h>
#include <event.h>
#include <evhttp.h>
#include <jansson.h>


class MessageSend {
public:
	std::string username;
	std::string chat;
	std::string text;
	MessageSend(std::string username, std::string chatName, std::string msgText) : username(username), chat(chatName), text(msgText) {}
};

class MessageGet {
public:
    std::string username;
    long int send_timestamp;
    std::string text;

    MessageGet() :send_timestamp(0) {}
};



class JsonConverter {
public:
	MessageSend fromJsonToMessage(json_t *requestJSON);
};





#endif