cmake_minimum_required(VERSION 3.20)
project(AshClipboard LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(AshClipboard
    src/main.cpp
    src/clipboard/win_clipboard.cpp
)
target_include_directories(AshClipboard PRIVATE
include
)