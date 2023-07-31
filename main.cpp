#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <chrono>

using namespace std::chrono_literals;

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * win = SDL_CreateWindow("title", 0, 0, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Surface * screen = SDL_GetWindowSurface(win);
    SDL_Delay(2000);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}