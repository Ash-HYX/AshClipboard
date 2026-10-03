#pragma once
#include <string>
class Message{
public:
    virtual ~Message() = default;
    virtual std::size_t size() const = 0;
    virtual std::string encode() const = 0;
};