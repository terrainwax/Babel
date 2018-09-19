//
// Created by entropy on 9/18/18.
//

#ifndef CPP_BABEL_2018_CHAT_H
#define CPP_BABEL_2018_CHAT_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <string>

class Packet {
public:
    enum {
        header_length = 4
    };
    enum {
        max_body_length = 512
    };

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
    char data_[header_length + max_body_length];
    std::size_t body_length_;
};

typedef std::deque<Packet*> PacketQueue;

#endif //CPP_BABEL_2018_CHAT_H
