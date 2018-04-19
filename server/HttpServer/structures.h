#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <iostream>
#include <vector>


struct message {
	int sender;
	int chat;
	const char * text;
	message(int sender, int chat, const char *text) : sender(sender), chat(chat), text(text) {}
};

#endif