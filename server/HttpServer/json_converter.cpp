#include <iostream>
#include <jansson.h>

#include "json_converter.h"


Message JsonConverter::fromJsonToMessage(json_t *requestJSON) {
	json_t *root= json_object_get(requestJSON, "message");

	json_t *sender_json = json_object_get(root, "sender");
	if(!json_is_string(sender_json))
        fprintf(stderr, "error: sender is not a string\n");

	json_t *chat_json = json_object_get(root, "chat");
	if(!json_is_string(chat_json))
        fprintf(stderr, "error: chat is not a string\n");
 
	json_t *text_json = json_object_get(root, "text");
	if(!json_is_string(text_json))
        fprintf(stderr, "error: text is not a string\n");

	Message msg(json_string_value(sender_json), json_string_value(chat_json), json_string_value(text_json));
  	return msg;
}