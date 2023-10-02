#ifndef ZORB_H
#define ZORB_H

#include <iostream>
#include <sstream>
#include "ZorbAppearance.hpp"

class Zorb {
public:
    // Default constructor (Empty Space)
    Zorb() : power(0), team_id(-1), name(""), appearance() {}
    // Team ID constructor (Creates Default Zorb)
    Zorb(appearanceEnum _enum, std::string _color) : power(0), team_id(-1), name(""), appearance() { SetAppearance(_enum, _color); }
    // Power, ID, name constructor
    Zorb(int power, int team_id, const std::string& name) : power(power), team_id(team_id), name(name), appearance() {}
    // Fully parameterized constructor
    Zorb(int power, int team_id, const std::string& name, ZorbAppearance _appearance) : power(power), team_id(team_id), name(name), appearance(std::move(_appearance)) {}
    // Copy constructor
    Zorb(const Zorb& other)
        : power(other.power),
        team_id(other.team_id),
        name(other.name),
        appearance(other.appearance) {}

    // Move constructor
    Zorb(Zorb&& other) noexcept
        : power(std::exchange(other.power, 0)),
        team_id(std::exchange(other.team_id, -1)),
        name(std::move(other.name)),
        appearance(std::move(other.appearance)) {}

    // Accessor functions
    int GetPower() const { return power; }
    int GetTeamId() const { return team_id; }
    std::string GetName() const { return name; }
    std::string GetAppearance() const { return appearance.GetAppearance(); }

    // Mutator Functions
    void SetAppearance(appearanceEnum _enum, std::string color="") { appearance.SetAppearance(_enum, color); }
    void SetPower(int _power) { this->power = _power; }
    void SetName(std::string _name) { this->name = _name; }

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
    os << "T" << zorb.team_id << ":" << zorb.power << ' ';
    os << zorb.name;
    os << zorb.GetAppearance(); // Include the appearance when printing
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

#endif // ZORB_H
