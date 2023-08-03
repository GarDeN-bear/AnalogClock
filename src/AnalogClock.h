#pragma once

#include <iostream>
#include <chrono>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

#include <mutex>
#include <thread>
#include <vector>

using namespace std::chrono_literals;

enum class timeZone
{
    London,
    Paris,
    Moscow,
    Tokyo = 8,
    Sydney,
    NewYork = 19
};


std::string chooseTimeZone(timeZone tZ);

class Window
{
public:
    Window(int _x, int _y, int _w, int _h, std::string _text = "Window");
    virtual void startLoop();

protected:
    const int x;
    const int y;
    const int w;
    const int h;
    std::string text;
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
    DigitalClock(int _x, int _y, int _w, int _h, std::string _text);
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
    AnalogClock(int _x, int _y, int _w, int _h, std::string _text, timeZone _currentTZ = timeZone::Moscow);
    void startLoop() override;
    void startLoop(const std::vector<timeZone>& timeZones);
    void setTimeZone(timeZone _tZ);

private:
    float clockRadious = 400.f;
    float hourHandLength = 100.f;
    float minuteHandLength = 200.f;
    float secondHandLength = 300.f;
    std::vector<float> cXPositions {-900, 0, 900, -900, 0, 900};
    std::vector<float> cYPositions {600, 600, 600, -600, -600, -600};
    timeZone tZ;
    timeZone currentTZ;
    SDL_GLContext mainContext = nullptr;
    void drawCircle(const float &cx, const float &cy);
    void drawPoint(const float &cx, const float &cy);
    void drawClock();
    void drawClock(std::vector<timeZone> timeZones);
    void drawHourHand(const float &cx, const float &cy, const int &hours);
    void drawMinuteHand(const float &cx, const float &cy, const int &minutes);
    void drawSecondHand(const float &cx, const float &cy, const int &seconds);
    float convertGLfX(float valX);
    float convertGLfY(float valY);
};
