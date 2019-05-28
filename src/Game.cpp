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

    irr::IrrlichtDevice *Game::getDevice(irr::video::E_DRIVER_TYPE type, int width, int height)
    {
        static irr::IrrlichtDevice *device = 0;

        if (!device) {
            device = irr::createDevice(type, irr::core::dimension2d<irr::u32>(width, height));
            device->setWindowCaption(L"Indie Studio");
        }
        return device;
    }

}