/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Game.cpp
*/

#include "indiestudio/Game.hpp"

namespace IndieStudio {

    Game::Game() : logger("game")
    {
    }

    void Game::start()
    {
        this->logger.info("Starting game...");
    }

}