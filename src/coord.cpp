#include "coord.h"
#include <iomanip>
#include <cmath>

#include <iostream>

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
    os << std::dec << crd.angle << ":" << std::dec << std::fixed << std::setprecision(crd._precision) << Coord::round(crd.point.x, crd._precision) << ":"
    << std::dec << std::fixed << std::setprecision(crd._precision) << Coord::round(crd.point.y, crd._precision) << ":" << std::dec << std::fixed << std::setprecision(3) << crd.distance << "\n";
    return os;
}

bool Coord::operator<(const Coord& c)
{
    return point.x < c.point.x || point.x == c.point.x && point.y < c.point.y;
}