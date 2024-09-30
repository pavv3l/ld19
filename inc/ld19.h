#ifndef __LD19_H__
#define __LD19_H__

#include <string>
#include "coord.h"
#include "averageCoord.h"
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>

enum class LD19_Round
{
    LLD_19_NONE = 0,
    LD19_cm = 2,
    LD19_mm = 3,
};

class LD19
{
public:
    explicit LD19(uint8_t intens ,LD19_Round round);
    virtual ~LD19();
    virtual void open(const std::string&);
    virtual void parse();
    virtual void saveFile(const std::string&);
    virtual void saveFile();
protected:
    std::string loadFil;
    std::string saveFilLidar;
    std::string saveFilCord;

    std::ifstream fileLoad;

    int _intensity;
    int _round;

    bool CRC8(uint8_t* data, int len);

    bool analyzeChunk(std::vector<uint8_t>& bit);

    bool analyzeFrame(uint8_t* frame, int len);

    Ld19Status status;
    LiDARFrameTypeDef frame_tmp;
    std::vector<LiDARFrameTypeDef> frameVec;
    std::vector<Coord> coordVec;
    std::unordered_map<float, AverageCoord> averagecoordMap;

    std::set<float> temp_0;
    std::set<float> temp_1;
    std::set<float> temp_2;
    std::set<float> temp_3;
};

#endif // __LD19_H__