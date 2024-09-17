#include "gtest/gtest.h"
#include "coord.h"

TEST(round, COORD)
{
    float a = 0.1523425435345f;
    float a1 = Coord::round(a, 3);
    float a_result = 0.152f;
    ASSERT_EQ(a1, a_result);

    float b = 0.1525425435345f;
    float b1 = Coord::round(b, 3);
    float b_result = 0.153f;
    ASSERT_EQ(b1, b_result);

    float c = -0.1523425435345f;
    float c1 = Coord::round(c, 3);
    float c_result = -0.152f;
    ASSERT_EQ(c1, c_result);

    float d = -0.1525425435345f;
    float d1 = Coord::round(d, 3);
    float d_result = -0.153f;
    ASSERT_EQ(d1, d_result);
}