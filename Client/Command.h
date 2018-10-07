/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Command.h
*/

#pragma once

const unsigned int COMMAND_MAGIC = 0xC0FFEE;
const unsigned int MAX_COMMAND_SIZE = 1024;

typedef enum {
	LOGIN = 0,
	ONLINE = 1,
	OFFLINE = 2,
	HOST = 3,
	CALL = 4,
	HANG = 5,
	ALIVE = 6,
	LIST = 7,
	JOIN = 8
} CommandIdentifier;

typedef union {
	char data[MAX_COMMAND_SIZE];
	CommandIdentifier id;
} CommandData;

typedef struct {
	unsigned int magic;
	CommandData data;
} Command;