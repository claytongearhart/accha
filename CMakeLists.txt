cmake_minimum_required(VERSION 3.24)
project(droidControl)

set(CMAKE_CXX_STANDARD 20)
add_definitions(-w)

include_directories(src)

add_executable(droidControl
        src/ocr.cpp
        src/ocr.h src/phyControl.cxx src/phyControl.hxx src/processImg.cxx src/processImg.hxx)
target_link_directories(droidControl PUBLIC  /opt/homebrew/Cellar/opencv/4.7.0_7/lib/ /opt/homebrew/Cellar/boost/1.82.0_1/lib )

target_include_directories(droidControl PUBLIC  /opt/homebrew/Cellar/opencv/4.7.0_7/include/opencv4 /opt/homebrew/Cellar/boost/1.82.0_1/include)
target_include_directories(droidControl PUBLIC  ./pInclude/eigen include/)

TARGET_LINK_LIBRARIES(droidControl opencv_core opencv_imgproc opencv_video opencv_imgcodecs opencv_highgui)