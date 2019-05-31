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

    irr::IrrlichtDevice *Game::getDevice(irr::video::E_DRIVER_TYPE type, int width, int height)
    {
        static irr::IrrlichtDevice *device = nullptr;

        if (!device) {
            device = irr::createDevice(type, irr::core::dimension2d<irr::u32>(width, height), 16, false, true);
            device->setWindowCaption(L"Indie Studio");
        }
        return device;
    }

}