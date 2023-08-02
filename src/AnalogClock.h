#pragma once

#include <iostream>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

using namespace std::chrono_literals;

class Window
{
public:
    Window(int _x, int _y, int _w, int _h, bool _hideWindow = true);
protected:
    const int x;
    const int y;
    const int w;
    const int h;
    SDL_Window *win = nullptr;
    SDL_Surface *winSurface = nullptr;
    bool hideWindow;
    virtual void startLoop();
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

class AnalogClock : public Window
{
public:
    AnalogClock(int _x, int _y, int _w, int _h, bool _hideWindow = false);

private:
    float clockRadious = 800.f;
    float hourHandLength = 600.f; 
    float minuteHandLength = 400.f; 
    float secondHandLength = 300.f; 
    SDL_GLContext mainContext = nullptr;
    void startLoop() override;
    void drawCircle();
    void drawPoint();
    void drawClock();
    void drawHourHand(const int& hours);
    void drawMinuteHand(const int& minutes);
    void drawSecondHand(const int& seconds);
    
};