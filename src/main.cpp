#include "AnalogClock.h"


int main()
{

std::vector<timeZone> vTZ{timeZone::London,
                          timeZone::Paris,
                          timeZone::Moscow,
                          timeZone::Tokyo,
                          timeZone::Sydney,
                          timeZone::NewYork
                          };
    AnalogClock w(SDL_WINDOWPOS_CENTERED_MASK,
                  SDL_WINDOWPOS_CENTERED_MASK,
                  1920, 1080, "All Analog Clocks");
    // w.setTimeZone(timeZone::NewYork);
    w.startLoop(vTZ);
    return 0;
}
