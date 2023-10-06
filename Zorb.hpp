#ifndef ZORB_H
#define ZORB_H

#include "zUtility.hpp" // Include zUtility.hpp for z_debug namespace
#include "ZorbAppearance.hpp" // Include ZorbAppearance.hpp for Zorb class usage

class Zorb {
public:
    // Default constructor (Empty Space)
    Zorb() : power(0), team_id(-1), name(""), appearance(ZorbAppearance()) { z_debug::zorbCount++; }
    // Team ID constructor (Creates Default Zorb)
    Zorb(int team_id) : power(0), team_id(team_id), name(""), appearance(ZorbAppearance()) { z_debug::zorbCount++; }
    // Appearance constructor
    Zorb(appearanceEnum _enum, std::string _color) : power(0), team_id(0), name("Gleep"), appearance(ZorbAppearance(_enum, _color)) {
        z_debug::zorbCount++;
    }
    // Power, ID, name constructor
    Zorb(int power, int team_id, const std::string& name) : power(power), team_id(team_id), name(name), appearance(ZorbAppearance()) { z_debug::zorbCount++; }
    // Fully parameterized constructor
    Zorb(int power, int team_id, const std::string& name, ZorbAppearance _appearance) : power(power), team_id(team_id), name(name), appearance(std::move(_appearance)) { z_debug::zorbCount++; }
    // Copy constructor
    Zorb(const Zorb& other)
        : power(other.power),
        team_id(other.team_id),
        name(other.name),
        appearance(other.appearance) { z_debug::zorbCount++; }

    // Move constructor
    Zorb(Zorb&& other) noexcept
        : power(std::exchange(other.power, 0)),
        team_id(std::exchange(other.team_id, -1)),
        name(std::move(other.name)),
        appearance(std::move(other.appearance)) {}

    // Default destructor
    ~Zorb() { z_debug::zorbCount--; }

    // Accessor functions
    int GetPower() const { return power; }
    int GetTeamId() const { return team_id; }
    std::string GetName() const { return name; }
    std::string GetAppearance() const { return appearance.GetAppearance(); }
    ZorbAppearance ReturnAppearanceObject() const { return appearance; }

    // Mutator Functions
    void SetPower(int _power) { this->power = _power; }
    void SetName(std::string _name) { this->name = _name; }
    void SetAppearance(appearanceEnum _enum, std::string _color = "") { appearance.SetAppearance(_enum, _color); }

    friend std::ostream& operator<<(std::ostream& os, const Zorb& zorb); // Overload the insertion operator to print a Zorb
    friend bool operator<(const Zorb& left, const Zorb& right); // Overload the < operator to compare Zorbs
    friend Zorb operator+(const Zorb& zorb1, const Zorb& zorb2); // Overload the + operator to combine two Zorbs

private:
    int power;
    int team_id;
    std::string name;
    ZorbAppearance appearance;
};

// IMPLEMENTATIONS AFTER THIS POINT //

// Overload the insertion operator
std::ostream& operator<<(std::ostream& os, const Zorb& zorb) {
    std::string zorbInfo = zorb.appearance.GetAppearance() + " " + zorb.name + "\nT" + std::to_string(zorb.team_id) + ":" + std::to_string(zorb.power) + '\n';

    std::istringstream iss(zorbInfo);
    std::string line;

    while (std::getline(iss, line)) {
        // Center-align each line and output to the stream
        os << z_debug::CenterAlignString(line, ZORBWIDTH) << '\n';
    }

    return os;
}

// Overload the < operator
bool operator<(const Zorb& left, const Zorb& right) {
    return (left.team_id != right.team_id) && (left.power < right.power);
}

// Overload the + operator
Zorb operator+(const Zorb& zorb1, const Zorb& zorb2) {
    int combined_power = zorb1.power + zorb2.power;
    int new_team_id = (zorb1.power >= zorb2.power) ? zorb1.team_id : zorb2.team_id;
    std::string new_name = (zorb1.power >= zorb2.power) ? zorb1.name : zorb2.name;
    ZorbAppearance new_appearance = (zorb1.power >= zorb2.power) ? zorb1.appearance : zorb2.appearance;
    
    Zorb new_zorb(combined_power, new_team_id, new_name, new_appearance);
    return new_zorb;
}

namespace z_debug {
    void PrintAllZorbAppearances() {
        ZorbAppearance _debugappearance;
        std::vector<std::string> charLines;
        std::vector<std::vector<std::string>> rowBuffers;
        int _enumNUM = APPEARANCE_DEFAULT;
        
        // Calculate margin
        size_t margin = (CONSOLESIZE%ZORBWIDTH) + ZORBWIDTH/2;

        while (_enumNUM < NUM_APPEARANCES) {
            _debugappearance.SetAppearance(static_cast<appearanceEnum>(_enumNUM), ANSI_YELLOW);
            
            std::string enumName = appearanceNames[static_cast<appearanceEnum>(_enumNUM)];
            std::string appearanceText = _debugappearance.GetAppearance() + enumName;
            std::vector<std::string> appearanceLines = SplitMultilineString(appearanceText);

            if (charLines.size() < appearanceLines.size()) {
                charLines.resize(appearanceLines.size());
            }

            if ((charLines.back().length() + GetLengthWithoutEscapeCodes(appearanceLines[0]) + ZORBWIDTH) >= CONSOLESIZE) {
                rowBuffers.push_back(charLines);
                charLines.clear();
            } else {
                for (size_t i = 0; i < appearanceLines.size(); ++i) {
                    charLines[i] += z_debug::SpaceToPrint(margin);
                    charLines[i] += appearanceLines[i];
                }
                _enumNUM++;
            }
        }

        rowBuffers.push_back(charLines);

        for (const auto& rowBuffer : rowBuffers) {
            for (const std::string& charLine : rowBuffer) {
                std::cout << charLine << std::endl;
            }
        }
    }

}

#endif // ZORB_H
