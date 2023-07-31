#include "AnalogClock.h"



int main()
{
    AnalogClock w(
        SDL_WINDOWPOS_CENTERED_MASK,
        SDL_WINDOWPOS_CENTERED_MASK,
        1920, 1080);
    auto now = std::chrono::system_clock::now();
    std::time_t currectTime = std::chrono::system_clock::to_time_t(now);
    char buf[9];
    std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&currectTime));
    std::cout << "Hours: " << buf << std::endl;
    std::cout << "Hours: " << (static_cast<int>(buf[0])-48) * 10 + static_cast<int>(buf[1]) - 48 << std::endl;
    std::cout << "Minutes: " << (static_cast<int>(buf[3])-48) * 10 + static_cast<int>(buf[4]) - 48 << std::endl;
    std::cout << "Seconds: " << (static_cast<int>(buf[6])-48) * 10 + static_cast<int>(buf[7]) - 48 << std::endl;
    return 0;
}