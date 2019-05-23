/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Game.cpp
*/

#include "indiestudio/Game.hpp"

namespace IndieStudio {

    Game::Game() : logger("game"), achievementManager(*this)
    {}

    void Game::start()
    {
        this->logger.info("Starting game...");
        this->modManager.reload();
        this->statisticManager.load();
        // TODO: Fire achievement register event
    }

}