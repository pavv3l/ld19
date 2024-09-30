#include "coord.h"


class AverageCoord: public Coord
{
public:
    void add(const Point& p)
    {
        add(p.x, p.y);
    }
    void add(float x, float y)
    {
        float tmp_x = point.x * cnt;
        float tmp_y = point.y * cnt;

        ++cnt;

        tmp_x += x;
        tmp_y += y;

        tmp_x /= cnt;
        tmp_y /= cnt;

        point.x = tmp_x;
        point.y = tmp_y;
    }
protected:
    uint32_t cnt = 0;
};