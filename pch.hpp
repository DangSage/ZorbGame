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

#include "bin/gameDefs.hpp" // Include gameDefs.hpp for game definitions
#include "bin/zUtility.hpp" // Include gameDefs.hpp for game definitions
#include "bin/zArt.hpp" // Include gameDefs.hpp for game definitions

namespace zorb {
    std::string RandomName() {
        std::string name = NAMES[z_debug::RandomValue(static_cast<size_t>(1), NAMES.size()-1)] + " " + NAMES[z_debug::RandomValue(static_cast<size_t>(1), NAMES.size()-1)];
        return name;
    }
} // namespace zorb