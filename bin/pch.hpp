#pragma once // Include this file only once per compilation unit
/*
This is the precompiled header file for the Zorb Game. This header file contains all the constants that are used throughout the game.
Some of those constant functions and variables include the following:
- Game States
- Zorb Names
- Zorb Appearances
- ANSI Escape Sequences
- Game Art
*/

// Standard library headers
#include <iomanip>
#include <cstdlib>
#include <initializer_list>
#include <memory>

// defined headers
#include "zDefs.hpp"
#include "zUtility.hpp"
#include "zArt.hpp"

const std::string url = "https://github.com/DangSage/ZorbGame";
const std::string command = "start " + url; // "start" is a Windows command to open a URL in the default browser

