#include <pBasic/pBasic.hpp>

#include <string>
#include <iostream>

int main()
{
    const std::string verStr = std::to_string(pBasic::Version::major) + "."
                                + std::to_string(pBasic::Version::minor) + "."
                                + std::to_string(pBasic::Version::patch);

    std::cout << "[INFO]: Check version string...\n";
    if (std::string(pBasic::Version::versionString) != verStr)
    {
        std::cerr << "[ERROR]: Check version string...FAILED!!!\n";
        return -1;
    }

    return 0;
}
