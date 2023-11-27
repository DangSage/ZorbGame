#ifndef ZORB_HPP
#define ZORB_HPP

#include "ZorbAppearance.hpp" // Include ZorbAppearance.hpp for Zorb class usage
#include <memory>
#include <list>

// Forward declarations
class TempBuff;

// Main Zorb class that is used in the game as the main object
class Zorb {
private:
    int power;
    int team_id;
    std::string name;
    ZorbAppearance appearance;
    static int count; // Static variable to keep track of the number of Zorbs in memory

    std::list<std::shared_ptr<TempBuff>> buffs;   //vector of Buffs

    friend class BuffPowerI;    //power increase buff
    friend class BuffPowerD;    //power decrease buff
    friend class BuffDodge;     //dodge buff
public:
    Zorb();                     // Default constructor (returns nothing)
    Zorb(ZorbAppearance _appearance, int power = 0, int team_id = -1, const std::string& name = zorb::RandomName(), std::list<std::shared_ptr<TempBuff>> buffs = {});
    Zorb(int team_id);          // Empty zorb constructor (used for dodging, and imploded zorbs)
    Zorb(const Zorb& other);    // Copy constructor
    ~Zorb();                    // Default destructor

    // Accessor functions
    int GetPower() const;
    int GetTeamId() const;
    static int GetCount() { return count; }
    std::string GetName() const;
    std::string GetAppearance() const;

    // Mutator Functions
    void UpdateBuffs();
    void AddBuff(std::shared_ptr<TempBuff> buff);
    void SetPower(int _power);
    void SetName(std::string _name);
    void SetAppearance(appearanceEnum _enum, std::string_view _color);
    void SetColor(std::string_view _color);

    friend std::ostream& operator<<(std::ostream& os, const Zorb& zorb); // Overload the insertion operator to print a Zorb
    friend bool operator<(const Zorb& left, const Zorb& right); // Overload the < operator to compare Zorbs
    friend bool operator>(const Zorb& left, const Zorb& right);

    friend Zorb operator+(const Zorb& zorb1, const Zorb& zorb2); // Overload the + operator to combine two Zorbs
    
    //define operator== to check Zorb objects to shared pointers of Zorb objects
    friend bool operator==(const Zorb& lhs, const std::shared_ptr<Zorb>& rhs);
    friend bool operator==(const Zorb& lhs, const Zorb& rhs);
};

// Abstract class for temporary buffs
class TempBuff {
public:
    int duration; // Number of turns or seconds, depending on your game
    TempBuff(int _duration) : duration(_duration+1) {} // Constructor
    
    virtual void Apply(Zorb& zorb) = 0; // Pure virtual function
    virtual void Remove(Zorb& zorb) = 0; // Pure virtual function
};

// Power increase buff
class BuffPowerI : public TempBuff {
public:
    int amount;
    BuffPowerI(int _duration, int _amount);

    void Apply(Zorb& zorb) override;
    void Remove(Zorb& zorb) override;
};

// Power decrease debuff
class BuffPowerD : public TempBuff {
public:
    int amount;
    BuffPowerD(int _duration, int _amount);

    void Apply(Zorb& zorb) override;
    void Remove(Zorb& zorb) override;
};

// Dodge buff, inherits from BuffPowerI
class BuffDodge : public BuffPowerI {
public:
    BuffDodge(int teamID);

    void Apply(Zorb& zorb) override;
    void Remove(Zorb& zorb) override;

    // returns a shared pointer to the dodge zorb that represents the dodge
    std::shared_ptr<Zorb> GetDodged() const;
private:
    std::shared_ptr<Zorb> dodgedZorb;
};


#endif // ZORB_HPP