#pragma once

#include <windows.h>
#include <stdexcept>

class ClipboardGuard{
public:
    ClipboardGuard(){
        if(!OpenClipboard(nullptr)){
            throw std::runtime_error("OpenClipboard failed");
        }
    }
    ~ClipboardGuard(){
        CloseClipboard();
    }
};