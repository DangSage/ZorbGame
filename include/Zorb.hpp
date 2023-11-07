#ifndef ZORB_HPP
#define ZORB_HPP

#include "ZorbAppearance.hpp" // Include ZorbAppearance.hpp for Zorb class usage
#include <memory>

class Zorb {
private:
    int power;
    int team_id;
    std::string name;
    ZorbAppearance appearance;
    static int count; // Static variable to keep track of the number of Zorbs in memory
public:
    Zorb(ZorbAppearance _appearance, int power = 0, int team_id = -1, const std::string& name = zorb::RandomName()) 
    : power(power), 
    team_id(team_id), 
    name(name), 
    appearance(_appearance) {
        count++;
    }

    // Empty zorb constructor (used for dodging, and imploded zorbs)
    Zorb(int team_id) 
    : team_id(team_id), name("~dodged"), appearance(0), power(-1){
        count++;
    }

    // Copy constructor
    Zorb(const Zorb& other)
        : power(other.power),
        team_id(other.team_id),
        name(other.name),
        appearance(other.appearance) { 
            count++;
        }

    // Default destructor
    ~Zorb() { 
        count--;
    }

    // Accessor functions
    int GetPower() const;
    const int GetTeamId() const;
    static int GetCount() { return count; }
    std::string GetName() const;
    std::string GetAppearance() const;

    // Mutator Functions
    void SetPower(int _power);
    void SetName(std::string _name);
    void SetAppearance(appearanceEnum _enum, std::string_view _color);

    friend std::ostream& operator<<(std::ostream& os, const Zorb& zorb); // Overload the insertion operator to print a Zorb
    friend bool operator<(const Zorb& left, const Zorb& right); // Overload the < operator to compare Zorbs
    friend Zorb operator+(const Zorb& zorb1, const Zorb& zorb2); // Overload the + operator to combine two Zorbs
    
    //define operator== to check Zorb objects to shared pointers of Zorb objects
    friend bool operator==(const Zorb& lhs, const std::shared_ptr<Zorb>& rhs);
    friend bool operator==(const Zorb& lhs, const Zorb& rhs);
};

#endif // ZORB_HPP