#ifndef Z_ART_HPP
#define Z_ART_HPP

#include "zDefs.hpp"
#include "json.hpp"

using json = nlohmann::json;

//region appearance enums and maps
enum class appearanceEnum : int {
    EMPTY = -1,
    A_DEFAULT = 0,
}; //dynamic enum for appearance

//map of appearance enums to appearance strings
extern std::map<appearanceEnum, std::string> appearanceMap;
//map of appearance enums to names
extern std::map<appearanceEnum, std::string> appearanceNames;
//map of weights (double) to appearance enums
extern std::map<appearanceEnum, double> appearanceWeights;

//initialize appearance maps
void initAppearanceMaps();
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
extern std::vector<std::array<std::string, 3>> barberHair;


//endregion

#endif // Z_ART_HPP
