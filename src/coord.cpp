#include "coord.h"
#include <iomanip>
#include <cmath>

uint32_t Coord::_precision = 3;

float Coord::round(float arg, int prec)
{
    float mult = std::pow(10, prec);
    if(arg > 0)
    {
        arg *= mult;
        arg += 0.5;
    }
    else if(arg < 0)
    {
        arg *= mult;
        arg -= 0.5;
    }
    else
    {
        return arg;
    }
    return static_cast<int>(arg)/mult;
}

std::ostream& operator<<(std::ostream& os, const Coord& crd)
{
    os << std::dec << (int)crd.timestamp << ":" << std::dec << std::fixed <<std::setprecision(crd._precision) << Coord::round(crd.x, crd._precision) << ":"
    << std::dec << std::fixed << std::setprecision(crd._precision) << Coord::round(crd.y, crd._precision) << ":" << std::dec << (int)crd.intensity << "\n";
    return os;
}