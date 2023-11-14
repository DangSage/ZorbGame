#include "zArt.hpp"
#include "zUtility.hpp"
#include <fstream>

//map of appearance enums to appearance strings
std::map<appearanceEnum, std::string> appearanceMap = {
{
appearanceEnum::A_DEFAULT, 
R"(       
   o   
./\|/\.
( o.o )
 > ^ < )"}
};

//map of appearance enums to names
std::map<appearanceEnum, std::string> appearanceNames = {{appearanceEnum::A_DEFAULT, "normal"}}; 
//map of weights (double) to appearance enums
std::map<appearanceEnum, double> appearanceWeights = {{appearanceEnum::A_DEFAULT, 5.0}};

//initialize appearance maps
void initAppearanceMaps() {
    std::ifstream file("appearances.json");
    if(!file) {
        //write default appearance to a new appearances.json file
        json j = {
            {
                {"appearance", {
                    "       ",
                    "   o   ",
                    "./\\|/\\.",
                    "( o.o )",
                    " > ^ < "
                }},
                {"enum", "A_EXAMPLE"},
                {"name", "example"}
            }
        };
        std::ofstream outfile("appearances.json");
        outfile << std::setw(4) << j << std::endl;
        outfile.close();

        NoJSONException exc = NoJSONException("initAppearanceMaps()", "appearances.json");
        z_debug::PrintError(exc);
    }

    json j;
    file >> j;
    int appNum = 1;
    // Loop through the JSON array and add each appearance to the appearance map
    for (auto& appearance : j) {
        std::string appearanceString;
        int lineCount = 0;
        //if the appearance does not have a weight, set it to 3
        if(appearance.find("weight") == appearance.end())
            appearance["weight"] = 3;
        /* Example JSON entry:
            "       ",
            "   o   ",
            "./\\|/\\.",
            "( o.o )",
            " > ^ < "
        */
        for (auto& line : appearance["appearance"]) {
            if (static_cast<std::string>(line).length() != ZORBWIDTH) { //exception handling for invalid appearance width
                std::cout << ansi::YELLOW << "EXCEPTION: appearance width != to ZORBWIDTH, initalization error." << std::endl
                << "\tLine: " << lineCount << " of enum " << appearance["enum"] << std::endl
                << "\tAppearance width: " << static_cast<std::string>(line).length() << ansi::RESET << std::endl;
                exit(0);
            }
            appearanceString += line;
            appearanceString += "\n";
            lineCount++;
        }
        appearanceMap[static_cast<appearanceEnum>(appNum)] = appearanceString;
        appearanceNames[static_cast<appearanceEnum>(appNum)] = appearance["name"];
        appearanceWeights[static_cast<appearanceEnum>(appNum)] = appearance["weight"];
        appNum++;
        if(_DEBUGMODE)
            std::cout << "appearance " << appNum << '-' << appearance["name"] << std::endl;
    }
    if(_DEBUGMODE) {
            std::cout << "DEBUG: initAppearanceMaps() - total appearances: " << appNum << std::endl;
            std::cout << "DEBUG: initAppearanceMaps() - appearanceMap size: " << appearanceMap.size() << std::endl;
            std::cout << "DEBUG: initAppearanceMaps() - appearanceNames size: " << appearanceNames.size() << std::endl;
    }
    file.close();
}

std::string BARBERNAME = z_util::random::choice({"Jean-Baptiste", "Rupert", "Gribert", "Rogier", "Emmanuel"});

std::vector<std::array<std::string, 3>> barberHair = {
    {
        "       ",
        "    o  ",
        "./\\|/\\.",
    },
    {
        "       ",
        "   o   ",
        ".]\\|/[.",
    },
    {
        "       ",
        "   o   ",
        ".-,|,-.",
    },
    {
        "       ",
        "   o   ",
        ".^,|,^.",
    }
};
