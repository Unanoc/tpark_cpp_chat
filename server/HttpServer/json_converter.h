#ifndef JSON_CONVERTER
#define JSON_CONVERTER

#include <iostream>
#include <errno.h>
#include <event.h>
#include <evhttp.h>
#include <jansson.h>


class Message {
public:
	std::string sender;
	std::string chat;
	std::string text;
	Message(std::string username, std::string chatName, std::string msgText) : sender(username), chat(chatName), text(msgText) {}
};

class JsonConverter {
public:
	Message fromJsonToMessage(json_t *requestJSON);
};





#endif