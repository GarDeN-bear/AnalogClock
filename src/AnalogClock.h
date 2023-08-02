#pragma once

#include <iostream>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

#include <mutex>
#include <thread>

enum class timeZone
{
    London,
    Paris,
    Moscow,
    Tokyo = 8,
    Sydney,
    NewYork = 19
};

using namespace std::chrono_literals;

class Window
{
public:
    Window(int _x, int _y, int _w, int _h);
    virtual void startLoop();

protected:
    const int x;
    const int y;
    const int w;
    const int h;
    SDL_Window *win = nullptr;
    SDL_Surface *winSurface = nullptr;
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
    DigitalClock(int _x, int _y, int _w, int _h);
    void startLoop() override;

private:
    SDL_Renderer *rendererTarget = nullptr;
    SDL_Surface *textSurface = nullptr;
    SDL_Texture *text = nullptr;
    TTF_Font *font = nullptr;
};

class AnalogClock : public Window
{
public:
    AnalogClock(int _x, int _y, int _w, int _h, timeZone _currentTZ = timeZone::Moscow);
    void startLoop() override;
    void setTimeZone(timeZone _tZ);

private:
    float clockRadious = 800.f;
    float hourHandLength = 300.f;
    float minuteHandLength = 400.f;
    float secondHandLength = 600.f;
    timeZone tZ = timeZone::Moscow;
    timeZone currentTZ;
    SDL_GLContext mainContext = nullptr;
    void drawCircle(const float &cx, const float &cy);
    void drawPoint(const float &cx, const float &cy);
    void drawClock();
    void drawHourHand(const float &cx, const float &cy, const int &hours);
    void drawMinuteHand(const float &cx, const float &cy, const int &minutes);
    void drawSecondHand(const float &cx, const float &cy, const int &seconds);
    float convertGLfX(float valX);
    float convertGLfY(float valY);
};