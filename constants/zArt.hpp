#ifndef Z_ART_HPP
#define Z_ART_HPP

#include "gameDefs.hpp"
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

//region appearance enums and maps
enum class appearanceEnum : int {}; //dynamic enum for appearance
std::map<appearanceEnum, std::string> appearanceMap; //map of appearance enums to appearance strings
std::map<appearanceEnum, std::string> appearanceNames; //map of appearance enums to names

//initialize appearance maps
void initAppearanceMaps() {
    std::ifstream file("appearances.json");
    json j;
    file >> j;
    int appNum = 0;
    // Loop through the JSON array and add each appearance to the appearance map
    for (auto& appearance : j) {
        std::string appearanceString;
        int lineCount = 0;
        /* Example JSON entry:
            "       ",
            "   o   ",
            "./\\|/\\.",
            "( o.o )",
            " > ^ < "
        ]
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
        appNum++;
        if(_DEBUGMODE)
            std::cout << "appearance " << appNum << '-' << appearance["name"] << std::endl;
    }
    if(_DEBUGMODE) {
            std::cout << "DEBUG: initAppearanceMaps() - total appearances: " << appNum << std::endl;
            std::cout << "DEBUG: initAppearanceMaps() - appearanceMap size: " << appearanceMap.size() << std::endl;
            std::cout << "DEBUG: initAppearanceMaps() - appearanceNames size: " << appearanceNames.size() << std::endl;
    }
}

//region ASCII ART DEFINITIONS
namespace z_art {
    std::string const introSpace = R"(
        .              +   .                .   . .     .  .
                   .                    .       .     *     
  .       *                        . . . .  .   .  + .      
            "You Are Here"            .   .  +  . . .       
.                 |             .  .   .    .    . .        
                  |           .     .     . +.    +  .      
                 \|/            .       .   . .             
        . .       V          .    * . . .  .  +   .         
           +      .           .   .      +                  
                            .       . +  .+. .              
  .                      .     . + .  . .     .      .      
           .      .    .     . .   . . .        ! /         
      *             .    . .  +    .  .       - O -         
          .     .    .  +   . .  *  .       . / |           
               . + .  .  .  .. +  .                         
.      .  .  .  *   .  *  . +..  .            *             
 .      .   . .   .   .   . .  +   .    .            +   .  )";

    std::string const planetZorb = R"(
.         _  .          .          .    +     .          .          .      .
        .(_)          .            .            .            .       :      
        .   .      .    .     .     .    .      .   .      . .  .  -+-      
          .           .   .        .           .          /         :  .    
    . .        .  .      /.   .      .    .     .     .  / .      . ' .     
        .  +       .    /     .          .          .   /      .            
       .            .  /         .            .        *   .         .     .
      .   .      .    *     .     .    .      .   .       .  .              
          .           .           .           .           .         +  .    
  . .        .  .       .   .      .    .     .     .    .      .   .       
                                                                            
 .   +      .         ,___/\_._/~~\_...__/\__.._._/~\-,      .         .   .
 ,_    ',\_._/~~\_.-/'                                 `--./\_    .   .     
/~~\_.-/'                                                     `--._/~\/\-.__)";

    std::string const optionsScreen = R"(
  ` : | | | |:  ||  :     `  :  |  |+|: | : : :|   .        `              .  
      ` : | :|  ||  |:  :    `  |  | :| : | : |:   |  .                    :  
         .' ':  ||  |:  |  '       ` || | : | |: : |   .  `           .   :.  
                `'  ||  |  ' |   *    ` : | | :| |*|  :   :               :|  
        *    *       `  |  : :  |  .      ` ' :| | :| . : :         *   :.||  
             .`            | |  |  : .:|       ` | || | : |: |          | ||  
      '          .         + `  |  :  .: .         '| | : :| :    .   |:| ||  
         .                 .    ` *|  || :       `    | | :| | :      |:| |   
 .                .          .        || |.: *          | || : :     :|||     
        .            .   . *    .   .  ` |||.  +        + '| |||  .  ||`      
     .             *              .     +:`|!             . ||||  :.||`       
 +                      .                ..!|*          . | :`||+ |||`        
     .                         +      : |||`        .| :| | | |.| ||`     .   
       *     +   '               +  :|| |`     :.+. || || | |:`|| `           
                            .      .||` .    ..|| | |: '` `| | |`  +          
  .       +++                      ||        !|!: `       :| |                
              +         .      .    | .      `|||.:      .||    .      .    ` 
          '         ___   pick your    `|.   .  `:|||   + ||'     `           
  __    +      *  _[|=|]_  < settings!    `'       `'|.    `:                 
"'  `---"""----...( ^.^ ).._--`^``----.,.___           `.    `.  .    ____,.,-
    ___,--'""`---"o> ^ <n  ^        ^       """'---,..___ __,..---""'         
--"'                        ^                         ``--..,__  ^`           
)";

    std::string const gameTitle = R"(
_/\____/\_             ___.      __________  O            .__   __   .__      
\____    / ____ _______\_ |__    \____    / _|__    ____  |__|_/  |_ |  |__   
  /     / /  _ \\_  __ \| __ \     /     /_/ __ \  /    \ |  |\   __\|  |  \.,
 /     /_(  <_> )|  | \/| \_\ \   /     /_\  ___/ |   |  \|  | |  |  |   Y  \.
/_______ \\____/ |__|   |___  /  /_______ \\___  >|___|  /|__| |__|  |___|  / 
        \/                  \/           \/     \/     \/                 \/  
)";

    std::string const endCard = R"(
/\/\      o      /\/\
\  /   ./\|/\.   \  /
 \/    ( ^.^ )    \/ 
       b> ^ <d       
)";
}

//endregion

#endif // Z_ART_HPP
