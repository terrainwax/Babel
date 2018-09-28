//
// Created by entropy on 9/28/18.
//

#ifndef CPP_BABEL_2018_BABEL_STRING_H
#define CPP_BABEL_2018_BABEL_STRING_H

#include <cstdlib>
#include <cctype>
#include <cstring>
#include <ostream>

class BabelString {
public:
    BabelString();
    BabelString(const char *data);
    BabelString(const char *data, std::size_t size);
    BabelString(const char character, std::size_t size);
    BabelString(const BabelString &string);
    ~BabelString();

    size_t getSize() const;
    char *getData() const;
    BabelString substr(size_t start, size_t size) const;
    void append(const BabelString &string);
    bool empty() const;

    bool operator==(const BabelString &rhs) const;
    bool operator==(const char *rhs) const;
    BabelString operator+(const BabelString &rhs) const;
    BabelString operator+(const char *rhs) const;
    BabelString &operator=(const BabelString & rhs);

    char* begin() const;
    char* end() const;

private:
    char *_data;
    std::size_t _size;
};

std::ostream& operator<<(std::ostream& os, const BabelString &string);

#endif //CPP_BABEL_2018_BABEL_STRING_H
