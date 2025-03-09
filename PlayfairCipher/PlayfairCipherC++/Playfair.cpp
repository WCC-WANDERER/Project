#include <iostream>
#include "Engine.h"

int main()
{
    Engine engine;
    std::string encoded = engine.encode("Playfair", "Wheatson", 'X');
    std::cout << encoded << std::endl;
    std::string decoded = engine.decode(encoded, "Wheatson", 'X');
    std::string decoded2 = engine.decode("QMBAIHKQ", "Wheatson", 'X');
    std::cout << decoded2 << std::endl;
}


