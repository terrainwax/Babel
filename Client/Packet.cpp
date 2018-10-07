//
// Created by entropy on 9/18/18.
//

#include "Packet.h"

Packet::Packet() : _body_length(0)
{
}

const char *Packet::data() const
{
	return (char *)_data;
}

char *Packet::data()
{
	return (char *)_data;
}

std::size_t Packet::length() const
{
	return header_length + _body_length;
}

const char *Packet::body() const
{
	return (char *)_data + header_length;
}

char *Packet::body()
{
	return (char *)_data + header_length;
}

std::size_t Packet::bodyLength() const
{
	return _body_length;
}

void Packet::bodyLength(std::size_t new_length)
{
	_body_length = new_length;
	if (_body_length > max_body_length)
		_body_length = max_body_length;
}

bool Packet::decodeHeader()
{
	_body_length = ((unsigned short *)_data)[0];
	if (_body_length > max_body_length) {
		_body_length = 0;
		return false;
	}
	return true;
}

void Packet::encodeHeader()
{
	((unsigned short *)_data)[0] = _body_length;
}

BabelString Packet::str()
{
	return BabelString((char *)_data + header_length, bodyLength());
}