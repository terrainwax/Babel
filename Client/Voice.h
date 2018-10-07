/*
** EPITECH PROJECT, 2018
** CPP_babel_2018
** File description:
** Command.h
*/

#pragma once

const unsigned int VOICE_MAGIC = 0xC0DECAD;
const unsigned int MAX_VOICE_SIZE = 1024;

typedef struct {
	unsigned char data[MAX_VOICE_SIZE];
	unsigned int N_bytes;
} VoiceData;

typedef struct {
	unsigned int magic;
	VoiceData data;
} Voice;