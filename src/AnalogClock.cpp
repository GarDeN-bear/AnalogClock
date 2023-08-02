#include "AnalogClock.h"
#include <mutex>
#include <thread>
std::mutex m;
std::mutex m1;
Window::Window(int _x, int _y, int _w, int _h, bool _hideWindow) : x(_x), y(_y), w(_w), h(_h)
{
    hideWindow = _hideWindow;
    std::unique_lock ul(m);
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
    ul.unlock();
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
    rendererTarget = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    mainContext = SDL_GL_CreateContext(win);
    if (mainContext == nullptr)
    {
        std::cout << "SDL_GL_CreateContext error: " << glGetError() << std::endl;
    }
    glViewport(0, 0, w, h);
    if (hideWindow == false)
    {
        startLoop();
    }
}

void AnalogClock::startLoop()
{
    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        drawClock();
        SDL_GL_SwapWindow(win);
        // std::cout << hours << ":" << minutes << ":" << seconds << std::endl;
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
                SDL_GL_DeleteContext(mainContext);
                destroyWindow();
            }
        }
    }
}

void AnalogClock::drawClock()
{
    char buf[9];
    auto now = std::chrono::system_clock::now();
    std::time_t currectTime = std::chrono::system_clock::to_time_t(now);
    std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&currectTime));
    int hours = static_cast<int>(buf[0] - 48) * 10 + static_cast<int>(buf[1] - 48);
    int minutes = static_cast<int>(buf[3] - 48) * 10 + static_cast<int>(buf[4] - 48);
    int seconds = static_cast<int>(buf[6] - 48) * 10 + static_cast<int>(buf[7] - 48);
    drawCircle();
    drawPoint();
    drawHourHand(hours);
    drawMinuteHand(minutes);
    drawSecondHand(seconds);
}

void AnalogClock::drawPoint()
{
    glEnable(GL_POINT_SMOOTH);
    glPointSize(50);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    glEnd();
}

void AnalogClock::drawCircle()
{
    float cx = 0.0f, cy = 0.0f;
    int numSegments = 100;
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(cx, cy);
    for (int i = 0; i <= numSegments; i++)
    {
        float theta = 2.0f * M_PI * float(i) / float(numSegments); 
        float x = clockRadious * cosf(theta);                      
        float y = clockRadious * sinf(theta);                      
        glVertex2f(1.0f / w * (cx + x), 1.0f / h * (cy + y));      
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(cx, cy);
    for (int i = 0; i <= numSegments; i++)
    {
        float theta = 2.0f * M_PI * float(i) / float(numSegments); 
        float x = (clockRadious - 20) * cosf(theta);               
        float y = (clockRadious - 20) * sinf(theta);               
        glVertex2f(1.0f / w * (cx + x), 1.0f / h * (cy + y));      
    }
    glEnd();
}

void AnalogClock::drawHourHand(const int &hours)
{
    float cx = 0.0f, cy = 0.0f;
    float endX, endY;
    int oneDivision = 360 / 12;
    glLineWidth(10);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(cx, cy);
    float theta = (oneDivision * hours) * M_PI / 180;
    if (hours > 12)
    {
        float theta = (oneDivision * (hours - 12)) * M_PI / 180;
    }
    endX = hourHandLength * cos(theta - M_PI / 2) + cx;
    endY = hourHandLength * sin(theta + M_PI / 2) + cy;
    glVertex2f(1.0f / w * endX, 1.0f / h * endY);
    glEnd();
}

void AnalogClock::drawMinuteHand(const int &minutes)
{
    float cx = 0.0f, cy = 0.0f;
    float endX, endY;
    int oneDivision = 360 / 60;
    glLineWidth(10);
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(cx, cy);
    float theta = (oneDivision * minutes) * M_PI / 180;
    endX = minuteHandLength * cos(theta - M_PI / 2) + cx;
    endY = minuteHandLength * sin(theta + M_PI / 2) + cy;
    glVertex2f(1.0f / w * endX, 1.0f / h * endY);
    glEnd();
}

void AnalogClock::drawSecondHand(const int &seconds)
{
    float cx = 0.0f, cy = 0.0f;
    float endX, endY;
    glLineWidth(10);
    int oneDivision = 360 / 60;
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(cx, cy);
    float theta = (oneDivision * seconds) * M_PI / 180;
    endX = secondHandLength * cos(theta - M_PI / 2) + cx;
    endY = secondHandLength * sin(theta + M_PI / 2) + cy;
    glVertex2f(1.0f / w * endX, 1.0f / h * endY);
    glEnd();
}