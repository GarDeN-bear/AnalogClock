#pragma once

#include <iostream>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>

using namespace std::chrono_literals;

class Window
{
public:
    Window(int _x, int _y, int _w, int _h, bool _hideWindow = true);

private:
    const int x;
    const int y;
    const int w;
    const int h;

protected:
    SDL_Window *win = nullptr;
    SDL_Surface *winSurface = nullptr;
    bool hideWindow;
    virtual void startLoop();
    SDL_Window *getWindow()
    {
        return win;
    }
    void destroyWindow()
    {
        SDL_FreeSurface(winSurface);
        SDL_DestroyWindow(win);
        SDL_Quit();
    }
};

class DigitalClock : public Window
{
public:
    DigitalClock(int _x, int _y, int _w, int _h, bool _hideWindow = false);

private:
    SDL_Renderer *rendererTarget = nullptr;
    SDL_Surface *textSurface = nullptr;
    SDL_Texture *text = nullptr;
    TTF_Font *font = nullptr;
    void startLoop() override;
};

class AnalogClock : Window
{
public:
    AnalogClock(int _x, int _y, int _w, int _h, bool _hideWindow = false);

private:
    SDL_Renderer *rendererTarget = nullptr;
    SDL_Surface *textSurface = nullptr;
    SDL_Texture *text = nullptr;
    void startLoop() override;
};