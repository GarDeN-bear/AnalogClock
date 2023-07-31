#include "AnalogClock.h"

AnalogClock::AnalogClock(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h)
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
    AnalogClock::~AnalogClock()
    {
        SDL_DestroyWindow(win);
        SDL_Quit();
    }