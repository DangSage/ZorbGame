#pragma once

#include "gameManager.hpp"

class GameplayManager : public GameManager {
public:
    static GameplayManager& getInstance() {
        static GameplayManager instance;
        return instance;
    }

    void updateGame() {
        // Add your game logic here
    }

private:
    GameplayManager();
    GameplayManager(const GameplayManager&) = delete;
    void operator=(const GameplayManager&) = delete;
};
