#pragma once

#include "zorb.hpp"

class GameManager; // Forward declaration of GameManager class

//declare a class called GameplayManager that will be a child of GameManager and will handle all gameplay related functions
//this class will be a friend of the GameManager class so that it can access all of the private variables and functions of the GameManager class
//This class will also only be initialized by the GameManager class
//all functions and variables will need to only be directly accessed by the GameManager class

class GameplayManager {
public:
    GameplayManager() = default; //default constructor
    GameplayManager(GameManager& gameManager) : m_gameManager(gameManager) {} //constructor that takes in a GameManager object
    ~GameplayManager() = default; //default destructor

    //region Gameplay Functions
    void startGame();
    void endGame();
    //endregion
private:
    //region Gameplay Variables
    GameManager& m_gameManager; //reference to the GameManager object
    //endregion
    
};

