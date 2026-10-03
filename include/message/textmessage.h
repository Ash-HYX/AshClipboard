#pragma once
#include "message/message.h"
#include <string>
class TextMessage : public Message{
private:
    std::string __text;
public:
    explicit TextMessage() = default;
    explicit TextMessage(const std::string& text) : __text(text){ }
    TextMessage(const TextMessage& other) : TextMessage(other.__text){ }
    TextMessage(TextMessage&& other) : TextMessage(std::move(other.__text)){ }
    std::string encode() const override{
        return std::to_string(this->__text.size()) + " " + this->__text + "[ASHSTOPFLAG]";
    }
    static TextMessage parseFrom(const std::string& s){
        auto breakPos = s.find(' ');
        return TextMessage(s.substr(breakPos + 1));
    }
    void setText(const std::string& newText){
        this->__text = newText;
    }
    size_t size() const override{
        return this->__text.size();
    }
};