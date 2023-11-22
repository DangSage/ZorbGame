#include "pch.hpp"
#include "Zorb.hpp"

int Zorb::count = 0; // Initialize the static variable

Zorb::Zorb() 
    : power(), team_id(), name(), appearance() {
        count++;
    }

Zorb::Zorb(ZorbAppearance _appearance, int power, int team_id, const std::string& _name, std::list<std::shared_ptr<TempBuff>> _buffs) 
    : power(power), 
    team_id(team_id), 
    name(_name), 
    appearance(_appearance),
    buffs(_buffs) {
        count++;
    }

Zorb::Zorb(int team_id) 
    : team_id(team_id), name(), appearance(team_id), power(0){
        count++;
    }

Zorb::Zorb(const Zorb& other)
    : power(other.power),
    team_id(other.team_id),
    name(other.name),
    appearance(other.appearance),
    buffs(other.buffs) { 
        count++;
    }

Zorb::~Zorb() {
    count--;
}

int Zorb::GetPower() const { return power; }
int Zorb::GetTeamId() const { return team_id; }
std::string Zorb::GetName() const { return name; }
std::string Zorb::GetAppearance() const { return appearance.GetAppearance(); }

void Zorb::SetPower(int _power) { this->power = _power; }
void Zorb::SetName(std::string _name) { this->name = _name; }
void Zorb::SetAppearance(appearanceEnum _enum, std::string_view _color = "") { appearance.SetAppearance(_enum, _color); }
void Zorb::SetColor(std::string_view _color) { appearance.SetColor(_color); }


// Overload the insertion operator
std::ostream& operator<<(std::ostream& os, const Zorb& zorb) {
    // Display the Zorb as ASCII art
    std::string appearanceText = zorb.GetAppearance();
    std::string infoText;
    
    infoText = z_util::FormattedText((std::to_string(zorb.GetPower()) + " power"), ansi::YELLOW);

    std::vector<std::string> appearanceLines = z_util::SplitMultilineString(appearanceText);

    // Calculate margin
    size_t longestLineLength = 0;
    for (const auto& line : appearanceLines) {
        longestLineLength = std::max(longestLineLength, line.length());
    }
    size_t margin = (CONSOLESIZE - longestLineLength) / 2;

    for (const auto& line : appearanceLines) {
        os << z_util::SpaceToPrint(margin+ZORBWIDTH) << line << std::endl;
    }

    os << z_util::CenterAlignStrings(zorb.GetName(), CONSOLESIZE+(ZORBWIDTH/2))
    << z_util::CenterAlignString(infoText, CONSOLESIZE) << std::endl;
    return os;
}

// Overload the < operator
bool operator<(const Zorb& left, const Zorb& right) {
    return left.power < right.power;
}

// Overload the > operator
bool operator>(const Zorb& left, const Zorb& right) {
    return left.power > right.power;
}

// Overload the + operator
Zorb operator+(const Zorb& zorb1, const Zorb& zorb2) {
    // Combine the power of the two Zorbs, the weaker Zorb is destroyed and the stronger Zorb is returned with the combined power and same team id
    int combined_power = zorb1.power + zorb2.power;
    if(zorb1.power == -1 || zorb2.power == -1) {
        return Zorb();
    }

    if(zorb1 > zorb2) {
        return Zorb(zorb1.appearance, combined_power, zorb1.team_id, zorb1.name, zorb1.buffs);
    }
    else if(zorb1 < zorb2) {
        return Zorb(zorb2.appearance, combined_power, zorb2.team_id, zorb2.name, zorb2.buffs);
    }
    else {
        // If the Zorbs have the same power, the team id is turned to negative and the Zorb implodes
        if(z_util::random::value() == 1) {
            ZorbAppearance new_appearance = ZorbAppearance(static_cast<appearanceEnum>(0), ansi::RED);
            return Zorb(new_appearance, -1, -1, zorb::N_IMPLODE);
        } else {
            return Zorb(zorb1.appearance, combined_power, zorb1.team_id, zorb1.name, zorb1.buffs);
        }        
    }
}

//define operator== to check Zorb objects to shared pointers of Zorb objects
bool operator==(const Zorb& lhs, const std::shared_ptr<Zorb>& rhs) {
    return lhs.power == rhs->power && lhs.team_id == rhs->team_id && lhs.name == rhs->name && lhs.appearance == rhs->appearance;
}
bool operator==(const Zorb& lhs, const Zorb& rhs) {
    return lhs.team_id == rhs.team_id && lhs.name == rhs.name && lhs.appearance == rhs.appearance;
}