#ifndef ZUI_HPP
#define ZUI_HPP

#include "Zorb.hpp" // Include Zorb.hpp for Zorb class usage

namespace z_debug {
    void DisplayDebugColors();
}

enum DisplayFormat {
        TABLE,
        ASCII_ART,
        SIMPLE,
        COMPACT
    };

class UI {
public:
    // Constructor to initialize the default display format
    UI(DisplayFormat defaultFormat = ASCII_ART) : currentFormat(defaultFormat) {}

    // display driver for vector of zorbs
    void DisplayZorbs(const std::vector<Zorb>& zorbs, char displaySide = 'L') const;
    // display driver for zorb objects
    void DisplayZorb(const Zorb& zorb, char displaySide = 'L') const;
    // accessor for DisplayFormat (string)
    std::string GetDisplayFormatAsString() const;   

    // mutator for DisplayFormat
    void SetDisplayFormat(DisplayFormat format);

    void screenMainMenu() const;                            // title screen display
    void screenDebugOptions() const;                        // setting screen display
    void screenInfo() const;                                // info screen display
    void screenGameOver(std::vector<std::string>& names) const; // gameover screen display
    void screenDebugColors() const;                         // debug color screen display
    void screenDebugZorbs() const;                          // debug zorb screen display

    void screenRecruitment(const Zorb& zorb, const std::vector<Zorb>& playerZorbs) const; // recruitment screen display
    void screenFightOutcome(Zorb& winZorb, Zorb& lossZorb, const std::string& attack) const; //screen for battle outcomes
    void screenBattle(const std::vector<Zorb>& team1, const std::vector<Zorb>& team2, const std::string& team1Name, const std::string& team2Name); //main battle screen that takes 2 arrays of zorbs as parameters
    void screenBarber(const std::vector<Zorb>& pZorbs) const; // barber shop screen display

    friend void _clearScreen();     // wipes output stream in the console
    friend void _pauseSystem();    // pauses output stream in console until user input    
    friend void _createStyledTextBox(const std::string& text); // stylized text box sent to output stream
    friend void _createDivider(char borderChar); // puts out a line halfway across display to output stream
private:
    DisplayFormat currentFormat; // Member variable to store the current display format

    // Functions for displaying Zorbs in different formats
    void ZorbDisplayTable(const std::vector<Zorb>& zorbs, char displaySide) const;
    void ZorbDisplayAscii(const std::vector<Zorb>& zorbs, char displaySide) const;
    void ZorbDisplaySimple(const std::vector<Zorb>& zorbs, char displaySide) const;
    void ZorbDisplayCompact(const std::vector<Zorb>& zorbs, char displaySide) const;

    static void _createHorizontalLine(char borderChar);
    static int _countTextLines(const std::string& text);
    
    // Singleton destructors, only one UI object allowed to be declared
    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;
};

// Friend function definitions
void _clearScreen();
void _createStyledTextBox(const std::string& text);
void _createDivider(char borderChar);
#endif // ZUI_HPP