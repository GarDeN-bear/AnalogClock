#include "AnalogClock.h"
std::vector<timeZone> vTZ{timeZone::London,
                          timeZone::Paris,
                          timeZone::Moscow,
                          timeZone::Tokyo,
                          timeZone::Sydney,
                          timeZone::NewYork};

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

int main()
{
    std::vector<std::thread> vThreads;
    std::vector<timeZone> vTZ{timeZone::London,
                              timeZone::Paris,
                              timeZone::Moscow,
                              timeZone::Tokyo,
                              timeZone::Sydney,
                              timeZone::NewYork};
    for (const auto &el : vTZ)
    {
        vThreads.push_back(std::thread([&el]()
                                       {
                       AnalogClock w(SDL_WINDOWPOS_CENTERED_MASK,
                                      SDL_WINDOWPOS_CENTERED_MASK,
                                      850, 850, chooseTimeZone(el));
                    w.setTimeZone(el);
                    w.startLoop(); }));
    }
    for (int i = 0; i < vTZ.size(); ++i)
    {
        vThreads.at(i).join();
    }
    return 0;
}
