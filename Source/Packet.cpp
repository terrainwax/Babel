//
// Created by entropy on 9/18/18.
//

#include "Packet.h"

Packet::Packet()
        : body_length_(0) {
}

const char *Packet::data() const {
    return data_;
}

char *Packet::data() {
    return data_;
}

std::size_t Packet::length() const {
    return header_length + body_length_;
}

const char *Packet::body() const {
    return data_ + header_length;
}

char *Packet::body() {
    return data_ + header_length;
}

std::size_t Packet::bodyLength() const {
    return body_length_;
}

void Packet::bodyLength(std::size_t new_length) {
    body_length_ = new_length;
    if (body_length_ > max_body_length)
        body_length_ = max_body_length;
}

bool Packet::decodeHeader() {
    char header[header_length + 1] = "";
    std::strncat(header, data_, header_length);
    body_length_ = std::atoi(header);
    if (body_length_ > max_body_length) {
        body_length_ = 0;
        return false;
    }
    return true;
}

void Packet::encodeHeader() {
    char header[header_length + 1] = "";
    std::sprintf(header, "%4d", body_length_);
    std::memcpy(data_, header, header_length);
}

std::string Packet::str()
{
    return std::string(data_ + header_length, bodyLength());
}