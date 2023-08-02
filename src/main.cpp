#include "AnalogClock.h"
std::mutex mS;
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
std::vector<timeZone> vTZ{timeZone::London,
                          timeZone::Paris,
                          timeZone::Moscow,
                          timeZone::Tokyo,
                          timeZone::Sydney,
                          timeZone::NewYork};

std::vector<AnalogClock> vAnalogClocks;
void startThread(int i)
{
    std::unique_lock ul(mS);
    AnalogClock w(SDL_WINDOWPOS_CENTERED_MASK,
                  SDL_WINDOWPOS_CENTERED_MASK,
                  850, 850, chooseTimeZone(vTZ.at(i)));
    w.setTimeZone(vTZ.at(i));
    vAnalogClocks.push_back(w);
    mS.unlock();
    w.startLoop();
}
int main()
{
    std::vector<std::thread> vThreads;
    for (int i = 0; i < vTZ.size(); ++i)
    {
        vThreads.push_back(std::thread(startThread, i));
    }
    for (int i = 0; i < vTZ.size(); ++i)
    {
        vThreads.at(i).join();
    }
    return 0;
}
