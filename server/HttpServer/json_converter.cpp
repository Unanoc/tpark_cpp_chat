#include <iostream>
#include <jansson.h>

#include "json_converter.h"


MessageSend JsonConverter::fromJsonToMessage(json_t *requestJSON) {
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

	MessageSend msg(json_string_value(username_json), json_string_value(chat_json), json_string_value(text_json));
  	return msg;
}