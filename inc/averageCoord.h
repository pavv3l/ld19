#include "coord.h"


class AverageCoord: public Coord
{
public:
    void addPoint(const Point& p)
    {
        addPoint(p.x, p.y);
    }
    void addPoint(float x, float y)
    {
        float tmp_x = point.x * cntPoint;
        float tmp_y = point.y * cntPoint;

        ++cntPoint;

        tmp_x += x;
        tmp_y += y;

        tmp_x /= cntPoint;
        tmp_y /= cntPoint;

        point.x = tmp_x;
        point.y = tmp_y;
    }
    void addIntensity(uint8_t i)
    {
        uint32_t tmpInt = intensity * cntIntensity;

        ++cntIntensity;

        tmpInt += i;
        tmpInt /= cntIntensity;

        intensity = static_cast<uint8_t>(tmpInt);
    }
protected:
    uint32_t cntPoint = 0;
    uint32_t cntIntensity = 0;
};