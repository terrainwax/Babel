//
// Created by entropy on 9/28/18.
//

#include "BabelString.h"

BabelString::BabelString()
{
    _size = 0;
    _data = (char *)std::malloc(1);

    _data[_size] = '\0';
}

BabelString::BabelString(const char *data)
{
    _size = strlen(data);
    _data = (char *)std::malloc(std::strlen(data) + 1);

    std::memcpy(_data, data, _size);
    _data[_size] = '\0';
}

BabelString::BabelString(const char *data, std::size_t size)
{
    _size = size;
    _data = (char *)std::malloc(size + 1);

    std::memcpy(_data, data, size);
    _data[size] = '\0';
}

BabelString::BabelString(const char character, std::size_t size)
{
    _size = size;
    _data = (char *)std::malloc(size + 1);

    std::memset(_data, character, size);
    _data[size] = '\0';
}

BabelString::~BabelString()
{
    std::free(_data);
    _size = 0;
}

BabelString &BabelString::operator=(const BabelString &rhs)
{
    _size = rhs.getSize();
    _data = (char *)std::malloc(_size + 1);

    std::memcpy(_data, rhs.getData(), _size);

    return *this;
}

size_t BabelString::getSize() const
{
    return _size;
}

char* BabelString::getData() const
{
    return _data;
}

BabelString BabelString::substr(size_t start, size_t size) const
{
    return BabelString(getData() + start, size);
}

void BabelString::append(const BabelString &string)
{
    char *oldData = _data;
    size_t oldSize = _size;

    _size = oldSize + string.getSize();
    _data = (char *)std::malloc(oldSize + string.getSize() + 1);

    std::memcpy(_data, oldData, oldSize);
    std::memcpy(_data + oldSize, string.getData(), string.getSize());
    _data[_size] = '\0';

    std::free(oldData);
}

bool BabelString::empty() const
{
    return _size == 0;
}

bool BabelString::operator==(const BabelString &rhs) const
{
    if (getSize() != rhs.getSize())
        return false;

    return std::memcmp(getData(), rhs.getData(), getSize()) == 0;
}

bool BabelString::operator==(const char *rhs) const
{
    if (strlen(rhs) != getSize())
        return false;

    return std::memcmp(getData(), rhs, getSize()) == 0;
}

BabelString BabelString::operator+(const BabelString &rhs) const
{
    BabelString result = BabelString();

    result.append(*this);
    result.append(rhs);

    return result;
}

BabelString BabelString::operator+(const char *rhs) const
{
    BabelString result = BabelString();

    result.append(*this);
    result.append(rhs);

    return result;
}

char* BabelString::begin() const
{
    return &_data[0];
}

char* BabelString::end() const
{
    return &_data[_size - 1];
}

BabelString::BabelString(const BabelString &babelString)
{
    _data = static_cast<char *>(std::malloc(babelString.getSize() + 1));
    _size = babelString.getSize();

    std::memcpy(_data, babelString.getData(), _size);
    _data[_size] = '\0';
}

std::ostream& operator<<(std::ostream& os, const BabelString &string)
{
    os << string.getData();

    return os;
}
