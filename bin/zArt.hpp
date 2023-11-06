#ifndef Z_ART_HPP
#define Z_ART_HPP

#include "gameDefs.hpp"
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

//define constexpr string for dodged zorb
constexpr char dodgedZorb[] = R"(
   o   
~/\~/\~
(     )
~>   <~)";

//region appearance enums and maps
enum class appearanceEnum : int {
    A_DEFAULT = 0,
}; //dynamic enum for appearance

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
        std::cout << "ERROR: initAppearanceMaps() - appearances.json not found" << std::endl
        << "DEFAULT appearance will be used. Press any key to continue" << std::endl;
        std::cin.get();

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
        return;
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

    zorb::BARBERNAME = randomChoice({"Jean-Baptiste", "Rupert", "Gribert", "Rogier", "Emmanuel"});
}
//endregion appearance enums and maps

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
    std::string const barber = R"(
   _.---------._            
  (  "SQUEEK?"  )           
   )           (            
   `'-,   ,---'`            
       \/              .___.
   (,\.-./,)           |  ||
    (_o.o_)/         <_l__<|
    b> 3 <%            _I_  
)";

    std::string const sample = R"(
sample
)";
    
    std::string const implode = R"(
                             ____                       
                     __,-~~/~    `---.                  
                   _/_,---(      ,    )                 
               __ /        <    /   )  \___             
- ------===;;;'====------------------===;;;===----- -  -
                  \/  ~"~"~"~"~"~\~"~)~"/               
                  (_ (   \  (     >    \)               
                   \_( _ <         >_>'                 
                      ~ `-i' ::>|--"                    
                          I;|.|.|                       
                         <|i::|i|`.                     
                        (` ^'"`-' ")                    
)";
}

//define vector of string arrays for the barber, these string arrays will be a hair style for zorbs that takes up 3 lines
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


//endregion

#endif // Z_ART_HPP
