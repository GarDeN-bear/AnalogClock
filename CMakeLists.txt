cmake_minimum_required(VERSION 3.7)

project(AnalogClock)
find_package(SDL2 REQUIRED)
include_directories(AnalogClock ${SDL2_INCLUDE_DIRS})
add_executable(AnalogClock main.cpp)
target_link_libraries(AnalogClock ${SDL2_LIBRARIES})