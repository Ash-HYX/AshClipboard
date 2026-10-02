#pragma once
#include <string>
class Clipboard{
public:
    std::string getText() const;
    void setText(const std::string& text);
};