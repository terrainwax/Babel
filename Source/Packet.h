//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_PACKET_H
#define CPP_BABEL_2018_PACKET_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <string>
class Packet {
public:
	static const unsigned char header_length = 2;
	static const unsigned short max_body_length = 1024;

	Packet();

	const char *data() const;
	char *data();
	std::size_t length() const;
	const char *body() const;
	char *body();
	std::size_t bodyLength() const;
	void bodyLength(std::size_t new_length);
	bool decodeHeader();
	void encodeHeader();
	std::string str();

private:
	unsigned char _data[header_length + max_body_length];
	std::size_t _body_length;
};

typedef std::deque<Packet> PacketQueue;

#endif //CPP_BABEL_2018_PACKET_H
