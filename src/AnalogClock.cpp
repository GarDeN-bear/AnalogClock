#include "AnalogClock.h"

AnalogClock::AnalogClock(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL_Init error: " << SDL_GetError() << std::endl;
    }
    if (TTF_Init() < 0)
    {
        std::cout << "TTF_Init error: " << TTF_GetError() << std::endl;
    }

    win = SDL_CreateWindow("Analog Clock",
                           x, y,
                           w, h,
                           SDL_WINDOW_OPENGL);
    if (win == nullptr)
    {
        std::cout << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
    }
    winSurface = SDL_GetWindowSurface(win);

    renderTarget = SDL_CreateRenderer(win, -1, 0);

    font = TTF_OpenFont("TimeRoman.ttf", 50);
    if (font == nullptr)
    {
        std::cout << "TTF_OpenFont error: " << TTF_GetError() << std::endl;
    }

    SDL_Color color = {0, 0, 0};

    bool quit = false;
    SDL_Event e;
    char buf[9];
    while (!quit)
    {
        SDL_SetRenderDrawColor(renderTarget, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderTarget);
        auto now = std::chrono::system_clock::now();
        std::time_t currectTime = std::chrono::system_clock::to_time_t(now);
        std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&currectTime));
        textSurface = TTF_RenderText_Solid(font, buf, color);
        text = SDL_CreateTextureFromSurface(renderTarget, textSurface);
        SDL_Rect textRect = {0, 0, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderTarget, text, NULL, &textRect);
        SDL_RenderPresent(renderTarget);
        // std::cout << "Hours: " << buf << std::endl;
        // std::cout << "Hours: " << (static_cast<int>(buf[0]) - 48) * 10 + static_cast<int>(buf[1]) - 48 << std::endl;
        // std::cout << "Minutes: " << (static_cast<int>(buf[3]) - 48) * 10 + static_cast<int>(buf[4]) - 48 << std::endl;
        // std::cout << "Seconds: " << (static_cast<int>(buf[6]) - 48) * 10 + static_cast<int>(buf[7]) - 48 << std::endl;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                SDL_FreeSurface(winSurface);
                SDL_DestroyWindow(win);
                SDL_DestroyTexture(text);
                TTF_CloseFont(font);
                SDL_FreeSurface(textSurface);
                SDL_DestroyRenderer(renderTarget);
                SDL_Quit();
                TTF_Quit();
            }
        }
    }
}
AnalogClock::~AnalogClock()
{

}