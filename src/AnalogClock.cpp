#include "AnalogClock.h"

Window::Window(int _x, int _y, int _w, int _h, bool _hideWindow) : x(_x), y(_y), w(_w), h(_h)
{
    hideWindow = _hideWindow;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL_Init error: " << SDL_GetError() << std::endl;
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
    if (hideWindow == false)
    {
        startLoop();
    }
}

void Window::startLoop()
{

    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                destroyWindow();
            }
        }
    }
}

DigitalClock::DigitalClock(int _x, int _y, int _w, int _h, bool _hideWindow) : Window(_x, _y, _w, _h)
{
    hideWindow = _hideWindow;
    if (TTF_Init() < 0)
    {
        std::cout << "TTF_Init error: " << TTF_GetError() << std::endl;
    }
    if (hideWindow == false)
    {
        startLoop();
    }
}

void DigitalClock::startLoop()
{
    rendererTarget = SDL_CreateRenderer(getWindow(), -1, SDL_RENDERER_ACCELERATED);

    font = TTF_OpenFont("TimeRoman.ttf", 50);
    if (font == nullptr)
    {
        std::cout << "TTF_OpenFont error: " << TTF_GetError() << std::endl;
    }
    SDL_Color color = {0, 0, 0};

    char buf[9];
    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        SDL_SetRenderDrawColor(rendererTarget, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(rendererTarget);
        auto now = std::chrono::system_clock::now();
        std::time_t currectTime = std::chrono::system_clock::to_time_t(now);
        std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&currectTime));
        textSurface = TTF_RenderText_Solid(font, buf, color);
        text = SDL_CreateTextureFromSurface(rendererTarget, textSurface);
        SDL_FreeSurface(textSurface);
        SDL_Rect textRect = {0, 0, textSurface->w, textSurface->h};
        SDL_RenderCopy(rendererTarget, text, NULL, &textRect);
        SDL_RenderPresent(rendererTarget);
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                SDL_DestroyTexture(text);
                TTF_CloseFont(font);
                SDL_DestroyRenderer(rendererTarget);
                TTF_Quit();
                destroyWindow();
            }
        }
    }
}

AnalogClock::AnalogClock(int _x, int _y, int _w, int _h, bool _hideWindow) : Window(_x, _y, _w, _h)
{
    hideWindow = _hideWindow;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    mainContext = SDL_GL_CreateContext(getWindow());
    if (hideWindow == false)
    {
        startLoop();
    }

}

void AnalogClock::startLoop()
{


    SDL_Color color = {0, 0, 0};

    char buf[9];
    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        glClearColor(255.0f, 255.0f, 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        SDL_GL_SwapWindow(getWindow());
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                destroyWindow();
            }
        }
    }
}