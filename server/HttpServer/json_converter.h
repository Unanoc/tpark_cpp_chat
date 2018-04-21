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
};


class JsonConverter {
public:
	Message fromJsonToMessage(json_t *requestJSON);
};


#endif