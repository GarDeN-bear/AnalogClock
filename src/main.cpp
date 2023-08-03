#include "AnalogClock.h"

int main()
{

    std::vector<timeZone> vTZ{timeZone::London,
                              timeZone::Paris,
                              timeZone::Moscow,
                              timeZone::Tokyo,
                              timeZone::Sydney,
                              timeZone::NewYork};
    std::vector<float> cXPositions{-1200, 0, 1200, -1200, 0, 1200};
    std::vector<float> cYPositions{550, 550, 550, -550, -550, -550};

    AnalogClock w(SDL_WINDOWPOS_CENTERED_MASK,
                  SDL_WINDOWPOS_CENTERED_MASK,
                  1920, 1080, "All Analog Clocks");
    w.setTimeZone(vTZ);
    w.setPositions(cXPositions, cYPositions);
    w.setCloclAttributes(500, 100, 200, 300);

    w.startLoop();
    return 0;
}
