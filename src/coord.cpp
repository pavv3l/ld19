#include "coord.h"
#include <iomanip>
#include <cmath>

uint32_t Coord::_precision = 3;

float round(float arg, int prec)
{
    ++prec;
    float mult = std::pow(10, prec);
    if(arg > 0)
    {
        return (arg + 5.0 / mult);
    }
    else if(arg < 0)
    {
        return (arg - 5.0 / mult);
    }
    else
    {
        return arg;
    }
    
}

std::ostream& operator<<(std::ostream& os, const Coord& crd)
{
    os << std::dec << (int)crd.timestamp << ":" << std::dec << std::fixed <<std::setprecision(crd._precision) << round(crd.x, crd._precision) << ":"
    << std::dec << std::fixed << std::setprecision(crd._precision) << round(crd.y, crd._precision) << ":" << std::dec << (int)crd.intensity << "\n";
    return os;
}