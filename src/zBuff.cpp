#include "pch.hpp"
#include "Zorb.hpp"
#include "gameplayManager.hpp"

void Zorb::AddBuff(std::shared_ptr<TempBuff> buff) {
    // Apply the buff
    buff->Apply(*this);
    // Add the buff to the list
    buffs.push_back(buff);
}

//Power increase buff constructor definition
BuffPowerI::BuffPowerI(int _duration, int _amount) : TempBuff(_duration), amount(_amount) {}

void BuffPowerI::Apply(Zorb& zorb) { 
    zorb.power += amount; 
}
void BuffPowerI::Remove(Zorb& zorb) { 
    zorb.power -= amount; 
}
void BuffPowerI::serialize(std::ostream& out) const {
    out << "BuffPowerI" << std::endl;
    out << duration << std::endl;
    out << amount << std::endl;
}

//Power decrease debuff constructor definition
BuffPowerD::BuffPowerD(int _duration, int _amount) : TempBuff(_duration), amount(_amount) {}

void BuffPowerD::Apply(Zorb& zorb) { 
    zorb.power -= amount; 
}
void BuffPowerD::Remove(Zorb& zorb) { 
    zorb.power += amount;
}
void BuffPowerD::serialize(std::ostream& out) const {
    out << "BuffPowerD" << std::endl;
    out << duration << std::endl;
    out << amount << std::endl;
}

//Dodge buff

BuffDodge::BuffDodge(int teamID) : BuffPowerI(1, 1), dodgedZorb(std::make_shared<Zorb>(teamID)) {}

void BuffDodge::Apply(Zorb& zorb) {
    zorb.name.insert(0, "*");
    if(zorb.power >= 4)
        amount = zorb.power / 3;
    zorb.power += amount;
}
void BuffDodge::Remove(Zorb& zorb) {
    zorb.power -= amount;
    zorb.name.erase(0, 1); // remove the * from the name
    dodgedZorb->SetPower(-2);
    dodgedZorb = nullptr;
}
void BuffDodge::serialize(std::ostream& out) const {
    out << "BuffDodge" << std::endl;
    out << duration << std::endl;
    out << amount << std::endl;
}

std::shared_ptr<Zorb> BuffDodge::GetDodged() const {
    if (dodgedZorb == nullptr) {
        // The pointer is null, so return immediately
        return nullptr;
    }
    // The pointer is not null, so return it
    return dodgedZorb;
}


void Zorb::UpdateBuffs() {
    // Loop through all buffs and update their duration
    for (auto it = buffs.begin(); it != buffs.end(); ) {
        (*it)->duration--;
        // If the buff has expired, remove it
        if ((*it)->duration <= 0) {
            (*it)->Remove(*this);
            it = buffs.erase(it);
        } else {
            it++; // Increment the iterator
        }
    }
}
