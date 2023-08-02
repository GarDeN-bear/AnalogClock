#include "AnalogClock.h"

int main()
{
    std::thread t1([]()
                   {
                       AnalogClock w1(SDL_WINDOWPOS_CENTERED_MASK,
                                      SDL_WINDOWPOS_CENTERED_MASK,
                                      1920, 1080);
                       w1.setTimeZone(timeZone::Sydney);
                       w1.startLoop();
                   });
    // std::thread t2([]()
    //                {
    //                    AnalogClock w2(SDL_WINDOWPOS_CENTERED_MASK,
    //                                   SDL_WINDOWPOS_CENTERED_MASK,
    //                                   1920, 1080);
    //                    w2.startLoop();
    //                });
    t1.join();
    // t2.join();

    return 0;
}
