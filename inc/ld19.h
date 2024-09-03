#ifndef __LD19_H__
#define __LD19_H__

#include <string>
#include "coord.h"
#include <fstream>
#include <vector>
#include <set>

class LD19
{
public:
    LD19();
    virtual ~LD19();
    virtual void open(const std::string&);
    virtual void parse();
    virtual void saveFile(const std::string&);
    virtual void saveFile();
protected:
    std::string loadFil;
    std::string saveFilLidar;
    std::string saveFilCord;

    std::fstream fileLoad;

    bool CRC8(uint8_t* data, int len);

    bool analyzeChunk(uint8_t* bit, int len);

    bool analyzeFrame(uint8_t* frame, int len);

    Ld19Status status;
    LiDARFrameTypeDef frame_tmp;
    std::vector<LiDARFrameTypeDef> frameVec;
    std::vector<Coord> coordVec;

    std::set<float> temp;
};

#endif // __LD19_H__