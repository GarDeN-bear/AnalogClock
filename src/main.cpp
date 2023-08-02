#include "AnalogClock.h"
#include <thread>
#include <mutex>

int main()
{
    AnalogClock w(SDL_WINDOWPOS_CENTERED_MASK,
                                    SDL_WINDOWPOS_CENTERED_MASK,
                                    1920, 1080);

    return 0;
}
