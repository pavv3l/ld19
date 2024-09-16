#include "ld19.h"

#include <unistd.h>
#include <ctype.h>
#include <iostream>
#include <string>


int main(int argc, char** argv)
{
    std::cout << "Hello world\n";

    int opt = -1;
    const char* cvalue = NULL;
    std::string file;

    while( (opt = getopt(argc, argv, "fir")) != -1)
    {
        switch(opt)
        {
            case 'f':
                cvalue = optarg;
                std::cout << "Option " << static_cast<char>(opt) << " value " << cvalue << "\n";
                file = cvalue;
                break;
            case 'i':
                cvalue = optarg;
                std::cout << "Option " << static_cast<char>(opt) << " value " << cvalue << "\n";
                break;
            case 'r':
                cvalue = optarg;
                std::cout << "Option " << static_cast<char>(opt) << " value " << cvalue << "\n";
                break;
            case '?':
                if(optopt == 'f' || optopt == 'i' || optopt == 'r')
                {
                    std::cerr << "Bad value of " << static_cast<char>(optopt) << "\n";
                }
                else if(isprint(optopt))
                {
                    std::cerr << "Unknown option " << static_cast<char>(optopt) << "\n";
                }
                else
                {
                    std::cerr << "Unknown option character\n";
                }
                break;
            default:
                std::cerr << "Error options\n";
                break;
        }
    }

    return 0;

    try
    {
        LD19 lidar;
        lidar.open(file);
        lidar.parse();
        lidar.saveFile();
    }
    catch(std::exception& e)
    {
        std::cerr << "Exception caught: " << e.what() << "\n";
    }
    
    std::cout << "skonczono\n";


    return 0;
}