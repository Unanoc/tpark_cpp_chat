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

class InviteToChatStruct {
public:
	std::string username;
	std::string password;
	std::string chat;
	std::string new_person;
	InviteToChatStruct(std::string username, std::string password, std::string chat, std::string new_person) : username(username), password(password), chat(chat), new_person(new_person) {}
};

class MessageSendStruct {
public:
	std::string username;
	std::string chat;
	std::string text;
	std::string password;
	MessageSendStruct(std::string username, std::string chatName, std::string msgText, std::string password) : username(username), chat(chatName), text(msgText), password(password) {}
};
// request
class MessageGetReqStruct {
public:
    std::string username;
    std::string password;
    long int last_update;
    MessageGetReqStruct(std::string username, std::string password, long int last_update) : username(username), password(password), last_update(last_update) {}
};
// response
class MessageGetStruct {
public:
    std::string username;
    std::string chat;
    std::string text;
    long int send_timestamp;
    MessageGetStruct(std::string username, std::string chat, std::string text, long int send_timestamp) :
		username(username),
		chat(chat),
		text(text),
		send_timestamp(send_timestamp) {}
};

class JsonConverter {
public:
	MessageSendStruct fromJsonToMessageSend(json_t *requestJSON);
	UserRegisterStruct fromJsonToUserRegister(json_t *requestJSON);
	ChatCreateStruct fromJsonToChatCreate(json_t *requestJSON);
	InviteToChatStruct fromJsonToInviteChat(json_t *requestJSON);
	MessageGetReqStruct fromJsonToMessageGetReqStruct(json_t *requestJSON);
};

#endif // JSON_CONVERTER