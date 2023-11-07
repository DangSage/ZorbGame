#pragma once

/*
This is the precompiled header file for the Zorb Game. This header file contains all the constants that are used throughout the game.
Some of those constant functions and variables include the following:
- Game States
- Zorb Names
- Zorb Appearances
- ANSI Escape Sequences
- Game Art
*/

//#include "gameDefs.hpp"
#include "zUtility.hpp" //for random name generation
#include "zArt.hpp"   //for game art

namespace zorb {
    std::string RandomName() {
        std::string name1 = NAMES[z_util::random::value(static_cast<size_t>(0), NAMES.size()-1)];
        std::string name2 = NAMES[z_util::random::value(static_cast<size_t>(0), NAMES.size()-1)];

        //return name1 + " " + name2 if name 2 exists, otherwise just return name1 without the space
        return name2.size() > 0 ? name1 + " " + name2 : name1;
    }
} // namespace zorb