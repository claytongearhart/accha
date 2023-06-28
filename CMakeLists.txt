cmake_minimum_required(VERSION 3.24)
project(droidControl)

set(CMAKE_CXX_STANDARD 14)


include_directories(src)

add_executable(droidControl
        src/ocr.cpp
        src/ocr.h)

target_include_directories(droidControl PUBLIC  /opt/homebrew/Cellar/opencv/4.7.0_4/include/opencv4 )
