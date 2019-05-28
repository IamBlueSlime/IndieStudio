/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Game.cpp
*/

#include "indiestudio/Game.hpp"

namespace IndieStudio {

    Game::Game() : logger("game"), achievementManager(*this), modManager(*this)
    {}

    void Game::start()
    {
        this->logger.info("Starting game...");
        this->statisticManager.load();
        this->worldManager.init();
        this->modManager.reload();
        this->logger.info("Done initialization.");
    }

}