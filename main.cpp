#include "ld19.h"
#include <iostream>


int main(int argc, char** argv)
{
    std::cout << "Hello world\n";
    if(argc != 2)
    {
        std::cerr << "Bledna liczba argumentow\n";
        return 0;
    }
    const char* file = argv[1];
    std::cout << "Plik do odczytu: " << file << "\n";

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