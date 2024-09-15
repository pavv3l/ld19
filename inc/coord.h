#ifndef __COORD_H__
#define __COORD_H__

#include <cstdint>
#include <numbers>
#include <cmath>
#include <ostream>

enum class Ld19Status
{
  PKG_HEADER,
  PKG_VER_LEN,
  PKG_SPEED,
  PKG_START_ANGLE,
  PKG_DATA,
  PKG_END_ANGLE,
  PKG_TIMESTAMP,
  PKG_CRC,
  PKG_READY,
  PKG_ERROR,
};

enum
{
  PKG_HEADER = 0x54,
  PKG_VER_LEN = 0x2C,
  POINT_PER_PACK = 12,
};

struct __attribute__((packed)) LidarPointStructDef
{
  uint16_t distance;
  uint8_t intensity;
} ;

struct __attribute__((packed)) LiDARFrameTypeDef
{
  uint8_t header;
  uint8_t ver_len;
  uint16_t speed;
  uint16_t start_angle;
  LidarPointStructDef point[POINT_PER_PACK];
  uint16_t end_angle;
  uint16_t timestamp;
  uint8_t crc8;
};

class Coord
{
public:
    float x;
    float y;
    uint8_t intensity;
    uint16_t timestamp;
    static uint32_t _precision;

    friend std::ostream& operator<<(std::ostream& os, const Coord& crd);

    void setPrecision(int prec)
    {
      _precision = prec;
    }
};

inline float DegreesToRadians(float deg)
{
  return deg * M_PI / 180;
}

#endif // __COORD_H__