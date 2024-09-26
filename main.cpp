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
    int intensity = 0; //
    LD19_Round roundi = LD19_Round::LLD_19_NONE;
    std::string valTmp;

    while( (opt = getopt(argc, argv, "f:i:r:")) != -1)
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
                valTmp = cvalue;
                try
                {
                    intensity = std::stoi(valTmp);
                }
                catch(const std::exception& e)
                {
                    std::cerr << "Bad value of -i argument: " << e.what() << '\n';
                    return EXIT_FAILURE;
                }
                
                std::cout << "Option " << static_cast<char>(opt) << " value " << cvalue << "\n";
                break;
            case 'r':
                cvalue = optarg;
                valTmp = cvalue;
                try
                {
                    if(cvalue[0] == '2')
                    {
                        roundi = LD19_Round::LD19_cm;
                    }
                    else if(cvalue[0] == '3')
                    {
                        roundi = LD19_Round::LD19_mm;
                    }
                    else
                    {
                        valTmp = "Bad argument of r option: [" + std::string(cvalue) + "]";
                        std::cerr << valTmp << "\n";
                        throw std::invalid_argument(valTmp);
                    }
                }
                catch(const std::exception& e)
                {
                    std::cerr << "Bad value of -r argument: " << e.what() << '\n';
                    return EXIT_FAILURE;
                }
                
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

    //return 0;

    try
    {
        LD19 lidar(static_cast<uint8_t>(intensity), roundi);
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