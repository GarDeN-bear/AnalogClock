#pragma once

#include <iostream>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

using namespace std::chrono_literals;

class AnalogClock
{
public:
    AnalogClock(int _x, int _y, int _w, int _h);
    ~AnalogClock();

private:
    SDL_Window *win = nullptr;
    SDL_Surface *screen = nullptr;
    const int x;
    const int y;
    const int w;
    const int h;
};