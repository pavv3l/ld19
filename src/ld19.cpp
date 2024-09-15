#include "ld19.h"

#include <iostream>
#include <iomanip>
#include <exception>
#include <cstring>
#include <cmath>

static int bad_readings = 0;
static int good_readings = 0;
static int all_readings = 0;

#define INTENSITY 30

std::ostream& operator<<(std::ostream& os, LidarPointStructDef* point)
{
    for(int i = 0; i < POINT_PER_PACK; ++i)
    {
        os << ":" << std::dec <<(int)point->distance << ":" << std::dec << (int)point->intensity;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const LiDARFrameTypeDef& lid)
{
    os << std::dec << (int)lid.header << ":" << std::dec << (int)lid.ver_len << ":" << std::dec << (int)lid.speed << ":"
    << std::dec << (int)lid.start_angle << (LidarPointStructDef*)&lid.point[0] << ":"
    << std::dec << (int)lid.end_angle << ":" << std::dec << (int)lid.timestamp << "\n";
    return os;
}

#define BytesChunk 33554432

static const uint8_t CrcTable[256] =
{
    0x00, 0x4d, 0x9a, 0xd7, 0x79, 0x34, 0xe3, 0xae, 0xf2, 0xbf, 0x68, 0x25,
    0x8b, 0xc6, 0x11, 0x5c, 0xa9, 0xe4, 0x33, 0x7e, 0xd0, 0x9d, 0x4a, 0x07,
    0x5b, 0x16, 0xc1, 0x8c, 0x22, 0x6f, 0xb8, 0xf5, 0x1f, 0x52, 0x85, 0xc8,
    0x66, 0x2b, 0xfc, 0xb1, 0xed, 0xa0, 0x77, 0x3a, 0x94, 0xd9, 0x0e, 0x43,
    0xb6, 0xfb, 0x2c, 0x61, 0xcf, 0x82, 0x55, 0x18, 0x44, 0x09, 0xde, 0x93,
    0x3d, 0x70, 0xa7, 0xea, 0x3e, 0x73, 0xa4, 0xe9, 0x47, 0x0a, 0xdd, 0x90,
    0xcc, 0x81, 0x56, 0x1b, 0xb5, 0xf8, 0x2f, 0x62, 0x97, 0xda, 0x0d, 0x40,
    0xee, 0xa3, 0x74, 0x39, 0x65, 0x28, 0xff, 0xb2, 0x1c, 0x51, 0x86, 0xcb,
    0x21, 0x6c, 0xbb, 0xf6, 0x58, 0x15, 0xc2, 0x8f, 0xd3, 0x9e, 0x49, 0x04,
    0xaa, 0xe7, 0x30, 0x7d, 0x88, 0xc5, 0x12, 0x5f, 0xf1, 0xbc, 0x6b, 0x26,
    0x7a, 0x37, 0xe0, 0xad, 0x03, 0x4e, 0x99, 0xd4, 0x7c, 0x31, 0xe6, 0xab,
    0x05, 0x48, 0x9f, 0xd2, 0x8e, 0xc3, 0x14, 0x59, 0xf7, 0xba, 0x6d, 0x20,
    0xd5, 0x98, 0x4f, 0x02, 0xac, 0xe1, 0x36, 0x7b, 0x27, 0x6a, 0xbd, 0xf0,
    0x5e, 0x13, 0xc4, 0x89, 0x63, 0x2e, 0xf9, 0xb4, 0x1a, 0x57, 0x80, 0xcd,
    0x91, 0xdc, 0x0b, 0x46, 0xe8, 0xa5, 0x72, 0x3f, 0xca, 0x87, 0x50, 0x1d,
    0xb3, 0xfe, 0x29, 0x64, 0x38, 0x75, 0xa2, 0xef, 0x41, 0x0c, 0xdb, 0x96,
    0x42, 0x0f, 0xd8, 0x95, 0x3b, 0x76, 0xa1, 0xec, 0xb0, 0xfd, 0x2a, 0x67,
    0xc9, 0x84, 0x53, 0x1e, 0xeb, 0xa6, 0x71, 0x3c, 0x92, 0xdf, 0x08, 0x45,
    0x19, 0x54, 0x83, 0xce, 0x60, 0x2d, 0xfa, 0xb7, 0x5d, 0x10, 0xc7, 0x8a,
    0x24, 0x69, 0xbe, 0xf3, 0xaf, 0xe2, 0x35, 0x78, 0xd6, 0x9b, 0x4c, 0x01,
    0xf4, 0xb9, 0x6e, 0x23, 0x8d, 0xc0, 0x17, 0x5a, 0x06, 0x4b, 0x9c, 0xd1,
    0x7f, 0x32, 0xe5, 0xa8
};

bool LD19::CRC8(uint8_t* data, int len)
{

    std::cerr << "Check CRC: size: " << sizeof(LiDARFrameTypeDef) << " len " << len << " last byte: " << std::hex << (int)data[len] << "\n";
    uint checkCRC = data[len];
    /*
    for(int i = 0; i < sizeof(LiDARFrameTypeDef); ++i)
    {
        std::cerr << std::hex << (int)data[i] << ":";
    }
    std::cerr << "\n";
    */

    uint8_t crc = 0;
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        crc = CrcTable[(crc ^ *data++) & 0xff];
    }

    return crc == checkCRC;
}

LD19::LD19()
{
    status = Ld19Status::PKG_HEADER;
}

LD19::~LD19()
{
    if(fileLoad.is_open())
    {
        fileLoad.close();
    }
}

void LD19::open(const std::string& str)
{
    fileLoad.open(str, std::ios::binary | std::ios::in);
    if(fileLoad.is_open())
    {
        loadFil = str;
        saveFilLidar = str + "_lidar";
        saveFilCord = str + "_cord";
        std::cout << "Plik " << loadFil << " otworzono do odczytu\n";
    }
    else
    {
        std::cerr << "Plik " << loadFil << " blad odczytu\n";
        throw std::invalid_argument("Plik do odczytu nie istnieje");
    }
}

void LD19::parse()
{
    if(!fileLoad.is_open())
    {
        std::cerr << "Plik nie otwarty do odczytu\n";
        throw std::runtime_error("Plik nie jest otwarty do odczytu\n");
    }
    uint8_t* tmp_buffer = reinterpret_cast<uint8_t*>( new(char[BytesChunk]) );
    while(!fileLoad.eof())
    {
        memset(tmp_buffer, 0, sizeof(tmp_buffer));
        fileLoad.read(reinterpret_cast<char*>(&tmp_buffer[0]), BytesChunk);
        int len = fileLoad.gcount();

        if(BytesChunk == len)
        {
            std::cout << "Parse - readed " << BytesChunk << " bytes of data\n";
        }
        else
        {
            std::cout << "Parse - readed " << len << " bytes of data\n";
        }
    
        analyzeChunk(tmp_buffer, len);
    }
    std::cout << "Good readings: " << good_readings << " bad readings " << bad_readings << " Percentage of good "
        << (float)good_readings / (float)(bad_readings + good_readings) << "\n";
    delete tmp_buffer;
}

void LD19::saveFile(const std::string& str)
{
    saveFilLidar = str + "_lidar";
    saveFilCord = str + "_cord";
    saveFile();
}

void LD19::saveFile()
{
    std::fstream writeFile;
    writeFile.open(saveFilLidar, std::ios::binary | std::ios::out);

    if(!writeFile.is_open())
    {
        std::cerr << "file lidar to save is not open\n";
        throw std::invalid_argument("Plik nie istnieje");
        return;
    }
    for(auto& x: frameVec)
    {
        writeFile << x;
    }
    std::cout << "File lidar was saved\n";
    writeFile.close();


    std::string fileCord2 = saveFilCord + "_2";
    writeFile.open(fileCord2, std::ios::binary | std::ios::out);

    if(!writeFile.is_open())
    {
        std::cerr << "file coord_2 to save is not open\n";
        return;
    }
    Coord::_precision = 2;
    for(auto& x: coordVec)
    {
        if(x.intensity > INTENSITY)
        writeFile << x;
    }
    std::cout << "File coord_2 was saved points " << coordVec.size() << " unikalne start angle: " << temp.size() << "\n";
    writeFile.close();

    std::string fileCord3 = saveFilCord + "_3";
    writeFile.open(fileCord3, std::ios::binary | std::ios::out);

    if(!writeFile.is_open())
    {
        std::cerr << "file coord_3 to save is not open\n";
        return;
    }
    Coord::_precision = 3;
    for(auto& x: coordVec)
    {
        if(x.intensity > INTENSITY)
        writeFile << x;
    }
    std::cout << "File coord_3 was saved\n";
    writeFile.close();
}

bool LD19::analyzeChunk(uint8_t* bit, int len)
{
    LiDARFrameTypeDef tmp;
    status = Ld19Status::PKG_HEADER;


    for(int i = 0; i < len; ++i)
    {
        switch (status)
        {
        case Ld19Status::PKG_HEADER:
            
            if(bit[i] == PKG_HEADER)
            {
            //tmp.header = bit[i];
            status = Ld19Status::PKG_VER_LEN;
            }
            break;

        case Ld19Status::PKG_VER_LEN:
            
            if(bit[i] == PKG_VER_LEN)
            {
                if( (i + sizeof(LiDARFrameTypeDef) - 1 < len) && CRC8(&bit[i-1], sizeof(LiDARFrameTypeDef) - 1))
                {
                    if( analyzeFrame(&bit[i-1], sizeof(LiDARFrameTypeDef)) )
                    {
                        i += sizeof(LiDARFrameTypeDef) - 2;
                        ++good_readings;
                    }
                    else
                    {
                        ++bad_readings;
                    }
                    status = Ld19Status::PKG_HEADER;
                }
                else
                {
                    //std::cout << "Analyze frame bad CRC8\n";
                    ++bad_readings;
                    status = Ld19Status::PKG_HEADER;
                }
            }
            else
            {
                status = Ld19Status::PKG_HEADER;
            }
            break;

        default:

            break;

        }
    }
}

bool LD19::analyzeFrame(uint8_t* frame, int len)
{
    if(CRC8(frame, sizeof(LiDARFrameTypeDef) - 1))
    {
        std::cout << "Analyze frame CRC OK\n";
    }
    else
    {
        std::cerr << "Analyze frame CRC NOT OK\n";
        return false;
    }

    //for(int i = 0; i < sizeof(LiDARFrameTypeDef); ++i)
    //{
    //    std::cerr << std::hex << (int)frame[i] << ":";
    //}
    //std::cerr << "\n";

    LiDARFrameTypeDef tmpLidar;
    memset(&tmpLidar, 0, sizeof(LiDARFrameTypeDef));

    //memcpy(&tmpLidar, frame, sizeof(LiDARFrameTypeDef));
    
    tmpLidar.header = frame[0];
    tmpLidar.ver_len = frame[1];

    tmpLidar.speed = frame[2];
    tmpLidar.speed |= frame[3] << 8;

    tmpLidar.start_angle = frame[4];
    tmpLidar.start_angle |= frame[5] << 8;
    uint8_t* tmpLoop = &frame[6];

    for(int i = 0; i < POINT_PER_PACK; ++i)
    {
        tmpLidar.point[i].distance = *tmpLoop;
        ++tmpLoop;
        tmpLidar.point[i].distance |= *tmpLoop << 8;
        ++tmpLoop;
        tmpLidar.point[i].intensity = *tmpLoop;
        ++tmpLoop;
    }

    tmpLidar.end_angle = frame[sizeof(LiDARFrameTypeDef) - 5];
    tmpLidar.end_angle |= frame[sizeof(LiDARFrameTypeDef) - 4] << 8;

    tmpLidar.timestamp = frame[sizeof(LiDARFrameTypeDef) - 3];
    tmpLidar.timestamp |= frame[sizeof(LiDARFrameTypeDef) - 2] << 8;

    tmpLidar.crc8 = frame[sizeof(LiDARFrameTypeDef) - 1];
    
    frameVec.push_back(tmpLidar);

    Coord tmpCord[POINT_PER_PACK];

    int angleDiff = (tmpLidar.end_angle - tmpLidar.start_angle + 360) % 360;

    float angleStep = static_cast<float>(angleDiff / 100.) / static_cast<float>(POINT_PER_PACK - 1);
    float startAngle = static_cast<float>(tmpLidar.start_angle)/100.;
    for(int i = 0; i < POINT_PER_PACK; ++i)
    {
        tmpCord[i].intensity = tmpLidar.point[i].intensity;
        tmpCord[i].timestamp = tmpLidar.timestamp;

        float angl = startAngle + i * angleStep;
        if(angl > 360.)
        {
            angl -= 360.;
        }

        //
        temp.insert(angl);
        //
        tmpCord[i].x = static_cast<float>(tmpLidar.point[i].distance)/1000. * std::sin(DegreesToRadians(angl));
        tmpCord[i].y =static_cast<float>(tmpLidar.point[i].distance)/1000. * std::cos(DegreesToRadians(angl));
    }
    coordVec.insert(coordVec.end(), tmpCord, tmpCord + POINT_PER_PACK);

    return true;
}