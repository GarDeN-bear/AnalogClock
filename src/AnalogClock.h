#pragma once

#include <iostream>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std::chrono_literals;

class AnalogClock
{
public:
    AnalogClock(int _x, int _y, int _w, int _h);
    ~AnalogClock();

private:
    SDL_Window *win = nullptr;
    SDL_Surface *winSurface = nullptr;
    const int x;
    const int y;
    const int w;
    const int h;

    SDL_Texture *text = nullptr;
    TTF_Font * font = nullptr;
    SDL_Renderer* renderTarget = nullptr;
    SDL_Surface *textSurface = nullptr;
    
};