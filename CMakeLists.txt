cmake_minimum_required(VERSION 3.20)

project(AnalogClock)
add_executable(AnalogClock src/main.cpp src/AnalogClock.cpp)
target_link_libraries(AnalogClock SDL2 SDL2_ttf)