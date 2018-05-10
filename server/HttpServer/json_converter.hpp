#ifndef JSON_CONVERTER
#define JSON_CONVERTER

#include <iostream>
#include <errno.h>
#include <event.h>
#include <evhttp.h>
#include <jansson.h>


class UserRegisterStruct {
public:
	std::string username;
	std::string password;
	UserRegisterStruct(std::string username, std::string password) : username(username), password(password) {}
};

class ChatCreateStruct {
public:
	std::string username;
	std::string password;
	std::string chat;
	ChatCreateStruct(std::string username, std::string password, std::string chat) : username(username), password(password), chat(chat) {}
};

class MessageSendStruct {
public:
	std::string username;
	std::string chat;
	std::string text;
	std::string password;
	MessageSendStruct(std::string username, std::string chatName, std::string msgText, std::string password) : username(username), chat(chatName), text(msgText), password(password) {}
};

class MessageGetStruct {
public:
    std::string username;
    long int send_timestamp;
    std::string text;
    std::string chat;
    MessageGetStruct() :send_timestamp(0) {}
};



class JsonConverter {
public:
	MessageSendStruct fromJsonToMessageSend(json_t *requestJSON);
	UserRegisterStruct fromJsonToUserRegister(json_t *requestJSON);
	ChatCreateStruct fromJsonToChatCreate(json_t *requestJSON);
};





#endif
