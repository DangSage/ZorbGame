#include "ZorbAppearance.hpp"

int ZorbAppearance::count = 0; // Initialize the the variable

appearanceEnum GetRandomAppearance() {
    //randomly pick an appearance from the map and return the enum
    int randomAppearance = z_util::random::value<int>(0, appearanceMap.size()-1);
    if(_DEBUGMODE)
        std::cout << "DEBUG: GetRandomAppearance() - rolled: " << randomAppearance << ": " << appearanceNames.at(static_cast<appearanceEnum>(randomAppearance)) << std::endl;
    return static_cast<appearanceEnum>(randomAppearance);
}

std::string ZorbAppearance::SetColor(std::string _color) { 
    color = _color; 
    currentAppearance = SetAppearance(currentEnum, color); 
    return currentAppearance; 
}

int ZorbAppearance::GetCount() const { 
    return count; 
}

std::string ZorbAppearance::SetAppearance(appearanceEnum _enum, std::string_view COLOR)
{
    // Check if the specified appearanceEnum exists in the map
    auto it = appearanceMap.find(_enum);
    if (it != appearanceMap.end()) {
        std::string before = it->second; // Use the mapped string
        std::string _appearance;

        // Split the multi-line string into individual lines
        std::istringstream iss(before);
        std::string line;

        while (std::getline(iss, line)) {
            _appearance += COLOR;
            _appearance += line;
            if(COLOR != "")
                _appearance += ansi::RESET;
            _appearance += "\n";
        }
        return _appearance;
    } else {
        std::cout << "EXCEPTION: appearance not set, invalid enum. (ZorbAppearance::SetAppearance)" << std::endl
            << "No appearance set for enum " << static_cast<int>(_enum) << std::endl;
        exit(0);
    }
}
std::string ZorbAppearance::GetAppearance() const
{
    if(currentAppearance == "")
    {   
        std::cout << "EXCEPTION: no appearance, initalization error." << std::endl
            << "Check that the appearance is set before calling GetAppearance()" << std::endl;
        exit(0);
    }
    return currentAppearance;
}
std::string_view ZorbAppearance::GetColor() const { 
    return color; 
}

// overload == operator to compare two ZorbAppearance objects
bool operator==(const ZorbAppearance& lhs, const ZorbAppearance& rhs) {
    return lhs.currentAppearance == rhs.currentAppearance;
}

namespace z_debug {
    void PrintZorbAppearances(int a, bool printNames, std::string_view color) {
        if(appearanceMap.size() < 1) {
            std::cout << "No ZorbAppearances in memory" << std::endl;
            return;
        }
        else if(a > appearanceMap.size() || a < 0) {
            std::cout << "Invalid number of appearances to print" << std::endl
                << "current number of appearances: " << appearanceMap.size() << std::endl;
            return;
        }
        std::vector<std::string> charLines;
        std::vector<std::vector<std::string>> rowBuffers;
        
        // Calculate margin
        size_t margin = (CONSOLESIZE%ZORBWIDTH) + ZORBWIDTH/2;

        for(int _enumNUM=0; _enumNUM<a; _enumNUM++) {
            //define debug appearance object that is the current appearance
            ZorbAppearance debugAppearance(static_cast<appearanceEnum>(_enumNUM), color);
            std::string appearanceText = debugAppearance.GetAppearance();
            std::vector<std::string> appearanceLines = z_util::SplitMultilineString(appearanceText);

            std::string nameText = appearanceNames.at(static_cast<appearanceEnum>(_enumNUM));

            if(_DEBUGMODE)
                std::cout << "DEBUG: PrintZorbAppearances() - appearance (" << _enumNUM+1 << "): " << nameText << std::endl;

            if(printNames) {
                if (charLines.size() < appearanceLines.size()) {
                    charLines.resize(appearanceLines.size()+1);
                }
            } else {
                if (charLines.size() < appearanceLines.size()) {
                    charLines.resize(appearanceLines.size());
                }
            }

            if(nameText.size() < ZORBWIDTH){
                nameText.resize(ZORBWIDTH, ' ');
            }

            if ((charLines.back().length() + z_util::GetLengthWithoutEscapeCodes(appearanceLines.back()) + ZORBWIDTH) > CONSOLESIZE-margin) {
                rowBuffers.push_back(charLines);
                charLines.clear();
                _enumNUM--;
            } else {
                for (size_t i = 0; i < appearanceLines.size(); ++i) {
                    charLines[i] += z_util::SpaceToPrint(margin);
                    charLines[i] += appearanceLines[i];
                }
                if(printNames)
                    charLines[appearanceLines.size()] += z_util::SpaceToPrint(margin) + nameText;
            }
        }
        rowBuffers.push_back(charLines);
        for (const auto& rowBuffer : rowBuffers) {
            for (const std::string& charLine : rowBuffer) {
                std::cout << std::right << std::setw(margin) << charLine << std::endl;
            }
        }
    }
}