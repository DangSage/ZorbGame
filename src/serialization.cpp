#include "Zorb.hpp"
#include "gameplayManager.hpp"
#include <fstream>


// Serialize functions

void Zorb::serialize(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&power), sizeof(power));
    out.write(reinterpret_cast<const char*>(&team_id), sizeof(team_id));

    size_t nameSize = name.size();
    out.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    out.write(name.c_str(), nameSize);

    appearance.serialize(out);

    int buffCount = buffs.size();
    out.write(reinterpret_cast<const char*>(&buffCount), sizeof(buffCount));
    for (auto& buff : buffs) {
        // Don't serialize a BuffDodge.
        if (dynamic_cast<BuffDodge*>(buff.get()) == nullptr)
            continue;
        buff->serialize(out);
    }
}

void ZorbAppearance::serialize(std::ostream& out) const {
    out.write(reinterpret_cast<const char*>(&currentAppearance), sizeof(currentAppearance));

    std::string colorStr = std::string(color);
    size_t colorSize = colorStr.size();
    out.write(reinterpret_cast<const char*>(&colorSize), sizeof(colorSize));
    out.write(colorStr.c_str(), colorSize);

    int enumInt = static_cast<int>(currentEnum);
    out.write(reinterpret_cast<const char*>(&enumInt), sizeof(enumInt));
}

void TempBuff::serialize(std::ostream& out) const {
    size_t buffTypeSize = sizeof("TempBuff");
    out.write(reinterpret_cast<const char*>(&buffTypeSize), sizeof(buffTypeSize));
    out.write("TempBuff", buffTypeSize);

    out.write(reinterpret_cast<const char*>(&duration), sizeof(duration));
}

void BuffPowerI::serialize(std::ostream& out) const {
    size_t buffTypeSize = sizeof("BuffPowerI");
    out.write(reinterpret_cast<const char*>(&buffTypeSize), sizeof(buffTypeSize));
    out.write("BuffPowerI", buffTypeSize);

    out.write(reinterpret_cast<const char*>(&duration), sizeof(duration));
    out.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
}

void BuffPowerD::serialize(std::ostream& out) const {
    size_t buffTypeSize = sizeof("BuffPowerD");
    out.write(reinterpret_cast<const char*>(&buffTypeSize), sizeof(buffTypeSize));
    out.write("BuffPowerD", buffTypeSize);

    out.write(reinterpret_cast<const char*>(&duration), sizeof(duration));
    out.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
}

void BuffDodge::serialize(std::ostream& out) const {
    // Don't serialize a BuffDodge.
}

// Deserialize functions

void Zorb::deserialize(std::istream& in) {
    in.read(reinterpret_cast<char*>(&power), sizeof(power));
    in.read(reinterpret_cast<char*>(&team_id), sizeof(team_id));

    size_t nameSize;
    in.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
    name.resize(nameSize);
    in.read(&name[0], nameSize);

    appearance.deserialize(in);

    int buffCount;
    in.read(reinterpret_cast<char*>(&buffCount), sizeof(buffCount));
    // Deserialize buffs using iterator
    for (int i = 0; i < buffCount; i++) {
        std::string buffType;
        size_t buffTypeSize;
        in.read(reinterpret_cast<char*>(&buffTypeSize), sizeof(buffTypeSize));
        buffType.resize(buffTypeSize);
        in.read(&buffType[0], buffTypeSize);

        if (buffType == "BuffPowerI") {
            int duration, amount;
            in.read(reinterpret_cast<char*>(&duration), sizeof(duration));
            in.read(reinterpret_cast<char*>(&amount), sizeof(amount));
            buffs.push_back(std::make_shared<BuffPowerI>(duration, amount));
        } else if (buffType == "BuffPowerD") {
            int duration, amount;
            in.read(reinterpret_cast<char*>(&duration), sizeof(duration));
            in.read(reinterpret_cast<char*>(&amount), sizeof(amount));
            buffs.push_back(std::make_shared<BuffPowerD>(duration, amount));
        } else {
            std::cout << "EXCEPTION: unknown buff type: " << buffType << std::endl;
            exit(0);
        }
    }
}

void ZorbAppearance::deserialize(std::istream& in) {
    in.read(reinterpret_cast<char*>(&currentAppearance), sizeof(currentAppearance));

    size_t colorSize;
    in.read(reinterpret_cast<char*>(&colorSize), sizeof(colorSize));
    std::string _color;
    _color.resize(colorSize);
    in.read(&_color[0], colorSize);
    color = _color;

    int enumInt;
    in.read(reinterpret_cast<char*>(&enumInt), sizeof(enumInt));
    currentEnum = static_cast<appearanceEnum>(enumInt);
}
