#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

class AnalogClock
{
public:
    AnalogClock(const int _x, const int _y, const int _w, const int _h) : x(_x), y(_y), w(_w), h(_h)
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            std::cout << SDL_GetError() << std::endl;
        }
        else
        {
            win = SDL_CreateWindow("Analog Clock",
                                   x, y,
                                   w, h,
                                   SDL_WINDOW_OPENGL);
            if (win == nullptr)
            {
                std::cout << SDL_GetError() << std::endl;
            }
            else
            {
                screen = SDL_GetWindowSurface(win);
                SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF));
                SDL_UpdateWindowSurface(win);
                bool quit = false;
                SDL_Event e;
                while (!quit)
                {
                    while (SDL_PollEvent(&e) != 0)
                    {
                        if (e.type == SDL_QUIT)
                        {
                            quit = true;
                        }
                    }
                }
            }
        }
    }
    ~AnalogClock()
    {
        SDL_DestroyWindow(win);
        SDL_Quit();
    }

private:
    SDL_Window *win = nullptr;
    SDL_Surface *screen = nullptr;
    const int x;
    const int y;
    const int w;
    const int h;
};

int main()
{
    AnalogClock w(
        SDL_WINDOWPOS_CENTERED_MASK,
        SDL_WINDOWPOS_CENTERED_MASK,
        1920, 1080);
    return 0;
}