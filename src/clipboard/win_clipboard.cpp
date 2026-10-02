#include "clipboard.h"
#include "clipboard_guard.h"
#include <windows.h>
#include <cstring>

std::string Clipboard::getText() const{
    ClipboardGuard guard; 
    if(!IsClipboardFormatAvailable(CF_UNICODETEXT)){
        return "";
    }
    HANDLE handle = GetClipboardData(CF_UNICODETEXT);
    if(handle == nullptr){
        return "";
    }
    wchar_t* text = static_cast<wchar_t*>(GlobalLock(handle)); 
    if(text == nullptr){
        return "";
    }
    int size = WideCharToMultiByte(
        CP_UTF8,
        0,
        text,
        -1,
        nullptr,
        0,
        nullptr,
        nullptr
    );
    std::string res(size, '\0');
    WideCharToMultiByte(
        CP_UTF8,
        0,
        text,
        -1,
        res.data(),
        size,
        nullptr,
        nullptr
    );
    GlobalUnlock(handle);
    return res;
}

void Clipboard::setText(const std::string& text){
    ClipboardGuard guard;
    EmptyClipboard();
    int size = MultiByteToWideChar(
        CP_UTF8,
        0,
        text.c_str(),
        -1,
        nullptr,
        0
    );
    std::wstring wideText(size, L'\0');
    MultiByteToWideChar(
        CP_UTF8,
        0,
        text.c_str(),
        -1,
        wideText.data(),
        size);
    HGLOBAL handle = GlobalAlloc(
        GMEM_MOVEABLE,
        wideText.size() * sizeof(wchar_t)
    );
    wchar_t* buffer = static_cast<wchar_t*>(GlobalLock(handle));
    std::memcpy(buffer, wideText.data(), wideText.size() * sizeof(wchar_t));
    GlobalUnlock(handle);
    SetClipboardData(CF_UNICODETEXT, handle);
}