#include <iostream>
#include <jansson.h>

#include "json_converter.hpp"

UserRegisterStruct JsonConverter::fromJsonToUserRegister(json_t *requestJSON) {
	json_t *root= json_object_get(requestJSON, "user");

	json_t *username_json = json_object_get(root, "username");
	if(!json_is_string(username_json))
		fprintf(stderr, "error: sender is not a string\n");

	json_t *password_json = json_object_get(root, "password");
	if(!json_is_string(password_json))
		fprintf(stderr, "error: password is not a string\n");

	UserRegisterStruct user(json_string_value(username_json), json_string_value(password_json));
	return user;
}

ChatCreateStruct JsonConverter::fromJsonToChatCreate(json_t *requestJSON) {
	json_t *root= json_object_get(requestJSON, "new_chat");

	json_t *username_json = json_object_get(root, "username");
	if(!json_is_string(username_json))
		fprintf(stderr, "error: sender is not a string\n");

	json_t *password_json = json_object_get(root, "password");
	if(!json_is_string(password_json))
		fprintf(stderr, "error: password is not a string\n");

	json_t *chat_json = json_object_get(root, "chat");
	if(!json_is_string(chat_json))
		fprintf(stderr, "error: chat is not a string\n");

	ChatCreateStruct chat(json_string_value(username_json), json_string_value(password_json), json_string_value(chat_json));
	return chat;
}

InviteToChatStruct JsonConverter::fromJsonToInviteChat(json_t *requestJSON) {
	json_t *root= json_object_get(requestJSON, "invite");

	json_t *username_json = json_object_get(root, "username");
	if(!json_is_string(username_json))
		fprintf(stderr, "error: sender is not a string\n");

	json_t *password_json = json_object_get(root, "password");
	if(!json_is_string(password_json))
		fprintf(stderr, "error: password is not a string\n");

	json_t *chat_json = json_object_get(root, "chat");
	if(!json_is_string(chat_json))
		fprintf(stderr, "error: chat is not a string\n");

	json_t *new_person_json = json_object_get(root, "new_person");
	if(!json_is_string(chat_json))
		fprintf(stderr, "error: new_person is not a string\n");

	InviteToChatStruct invite(json_string_value(username_json), json_string_value(password_json), json_string_value(chat_json), json_string_value(new_person_json));
	return invite;
}

MessageSendStruct JsonConverter::fromJsonToMessageSend(json_t *requestJSON) {
	json_t *root= json_object_get(requestJSON, "message");

	json_t *username_json = json_object_get(root, "username");
	if(!json_is_string(username_json))
		fprintf(stderr, "error: sender is not a string\n");

	json_t *chat_json = json_object_get(root, "chat");
	if(!json_is_string(chat_json))
		fprintf(stderr, "error: chat is not a string\n");

	json_t *text_json = json_object_get(root, "text");
	if(!json_is_string(text_json))
		fprintf(stderr, "error: text is not a string\n");

	json_t *password_json = json_object_get(root, "password");
	if(!json_is_string(password_json))
		fprintf(stderr, "error: password is not a string\n");

	MessageSendStruct msg(json_string_value(username_json), json_string_value(chat_json), json_string_value(text_json), json_string_value(password_json));
	return msg;
}

MessageGetReqStruct JsonConverter::fromJsonToMessageGetReqStruct(json_t *requestJSON) {
	json_t *root= json_object_get(requestJSON, "user");

	json_t *username_json = json_object_get(root, "username");
	if(!json_is_string(username_json))
		fprintf(stderr, "error: sender is not a string\n");

	json_t *password_json = json_object_get(root, "password");
	if(!json_is_string(password_json))
		fprintf(stderr, "error: password is not a string\n");

	json_t *last_update_json = json_object_get(root, "last_update");
	if(!json_is_integer(last_update_json))
		fprintf(stderr, "error: last_update is not a integer\n");

	MessageGetReqStruct msg(json_string_value(username_json), json_string_value(password_json), json_integer_value(last_update_json));
	return msg;
}