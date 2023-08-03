#include "AnalogClock.h"

std::string chooseTimeZone(timeZone tZ)
{
    switch (tZ)
    {
    case (timeZone::London):
        return "London";
        break;
    case (timeZone::Paris):
        return "Paris";
        break;
    case (timeZone::Moscow):
        return "Moscow";
        break;
    case (timeZone::Tokyo):
        return "Tokyo";
        break;
    case (timeZone::Sydney):
        return "Sydney";
        break;
    case (timeZone::NewYork):
        return "NewYork";
        break;
    default:
        return "0";
    }
}

Window::Window(int _x, int _y, int _w, int _h, std::string _text) : x(_x), y(_y), w(_w), h(_h)
{
    text = _text;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "SDL_Init error: " << SDL_GetError() << std::endl;
    }

    win = SDL_CreateWindow(text.c_str(),
                           x, y,
                           w, h,
                           SDL_WINDOW_OPENGL);
    if (win == nullptr)
    {
        std::cout << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
    }
    winSurface = SDL_GetWindowSurface(win);
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

DigitalClock::DigitalClock(int _x, int _y, int _w, int _h, std::string _text) : Window(_x, _y, _w, _h, _text)
{
    if (TTF_Init() < 0)
    {
        std::cout << "TTF_Init error: " << TTF_GetError() << std::endl;
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

AnalogClock::AnalogClock(int _x, int _y, int _w, int _h, std::string _text, timeZone _currentTZ) : Window(_x, _y, _w, _h, _text)
{
    currentTZ = _currentTZ;
    tZ = _currentTZ;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    mainContext = SDL_GL_CreateContext(win);
    if (mainContext == nullptr)
    {
        std::cout << "SDL_GL_CreateContext error: " << glGetError() << std::endl;
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

void AnalogClock::startLoop(const std::vector<timeZone> &timeZones)
{
    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        drawClock(timeZones);
        SDL_GL_SwapWindow(win);
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
    int hours = static_cast<int>(buf[0] - 48) * 10 + static_cast<int>(buf[1] - 48) - static_cast<int>(currentTZ) + static_cast<int>(tZ);
    if (hours > 24)
    {
        hours -= 24;
    }
    int minutes = static_cast<int>(buf[3] - 48) * 10 + static_cast<int>(buf[4] - 48);
    int seconds = static_cast<int>(buf[6] - 48) * 10 + static_cast<int>(buf[7] - 48);
    float cx = 0.0f, cy = 0.0f;
    drawCircle(cx, cy);
    drawPoint(cx, cy);
    drawHourHand(cx, cy, hours);
    drawMinuteHand(cx, cy, minutes);
    drawSecondHand(cx, cy, seconds);
}

void AnalogClock::drawClock(std::vector<timeZone> timeZones)
{
    char buf[9];
    auto now = std::chrono::system_clock::now();
    std::time_t currectTime = std::chrono::system_clock::to_time_t(now);
    std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&currectTime));
    for (int i = 0; i < timeZones.size(); ++i)
    {
        tZ = timeZones.at(i);
        int hours = static_cast<int>(buf[0] - 48) * 10 + static_cast<int>(buf[1] - 48) - static_cast<int>(currentTZ) + static_cast<int>(tZ);
        if (hours > 24)
        {
            hours -= 24;
        }
        int minutes = static_cast<int>(buf[3] - 48) * 10 + static_cast<int>(buf[4] - 48);
        int seconds = static_cast<int>(buf[6] - 48) * 10 + static_cast<int>(buf[7] - 48);
        float cx = cXPositions.at(i);
        float cy = cYPositions.at(i);
        drawCircle(cx, cy);
        drawPoint(cx, cy);
        drawHourHand(cx, cy, hours);
        drawMinuteHand(cx, cy, minutes);
        drawSecondHand(cx, cy, seconds);
    }
}

void AnalogClock::drawPoint(const float &cx, const float &cy)
{
    glEnable(GL_POINT_SMOOTH);
    glPointSize(50);
    glBegin(GL_POINTS);
    glColor3f(0.0f, 0.0f, 0.0f);
    glVertex2f(convertGLfX(cx), convertGLfY(cy));
    glEnd();
}

void AnalogClock::drawCircle(const float &cx, const float &cy)
{
    int numSegments = 60;
    glLineWidth(20);
    glEnable(GL_LINE_SMOOTH);
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 0.0f, 0.0f);
    for (int i = 0; i <= numSegments; i++)
    {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        float x = clockRadious * cosf(theta) + cx;
        float y = clockRadious * sinf(theta) + cy;
        glVertex2f(convertGLfX(x), convertGLfY(y));
    }
    glEnd();
    for (int i = 0; i <= numSegments; i++)
    {
        float theta = 2.0f * M_PI * float(i) / float(numSegments);
        glLineWidth(20);
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        float x1 = clockRadious * cosf(theta);
        float y1 = clockRadious * sinf(theta);
        glVertex2f(convertGLfX(cx + x1), convertGLfY(cy + y1));
        float x2, y2;
        if (i % 5 == 0)
        {
            x2 = (clockRadious - 100) * cosf(theta);
            y2 = (clockRadious - 100) * sinf(theta);
        }
        else
        {
            x2 = (clockRadious - 40) * cosf(theta);
            y2 = (clockRadious - 40) * sinf(theta);
        }
        glVertex2f(convertGLfX(cx + x2), convertGLfY(cy + y2));
        glEnd();
    }
}

void AnalogClock::drawHourHand(const float &cx, const float &cy, const int &hours)
{
    float x, y;
    int oneDivision = 360 / 12;
    glLineWidth(10);
    glEnable(GL_LINE_SMOOTH);
    glBegin(GL_LINES);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(convertGLfX(cx), convertGLfY(cy));
    float theta = (oneDivision * hours) * M_PI / 180;
    if (hours > 12)
    {
        float theta = (oneDivision * (hours - 12)) * M_PI / 180;
    }
    x = hourHandLength * cos(theta - M_PI / 2) + cx;
    y = hourHandLength * sin(theta + M_PI / 2) + cy;
    glVertex2f(convertGLfX(x), convertGLfY(y));
    glEnd();
}

void AnalogClock::drawMinuteHand(const float &cx, const float &cy, const int &minutes)
{
    float x, y;
    int oneDivision = 360 / 60;
    glLineWidth(10);
    glEnable(GL_LINE_SMOOTH);
    glBegin(GL_LINES);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(convertGLfX(cx), convertGLfY(cy));
    float theta = (oneDivision * minutes) * M_PI / 180;
    x = minuteHandLength * cos(theta - M_PI / 2) + cx;
    y = minuteHandLength * sin(theta + M_PI / 2) + cy;
    glVertex2f(convertGLfX(x), convertGLfY(y));
    glEnd();
}

void AnalogClock::drawSecondHand(const float &cx, const float &cy, const int &seconds)
{
    float x, y;
    glLineWidth(10);
    int oneDivision = 360 / 60;
    glEnable(GL_LINE_SMOOTH);
    glBegin(GL_LINES);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(convertGLfX(cx), convertGLfY(cy));
    float theta = (oneDivision * seconds) * M_PI / 180;
    x = secondHandLength * cos(theta - M_PI / 2) + cx;
    y = secondHandLength * sin(theta + M_PI / 2) + cy;
    glVertex2f(convertGLfX(x), convertGLfY(y));
    glEnd();
}

float AnalogClock::convertGLfX(float valX)
{
    return 1.0f / w * valX;
}

float AnalogClock::convertGLfY(float valY)
{
    return 1.0f / h * valY;
}

void AnalogClock::setTimeZone(timeZone _tZ)
{
    tZ = _tZ;
}
