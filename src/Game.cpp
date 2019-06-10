/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Game.cpp
*/

#include "indiestudio/Game.hpp"
#include "indiestudio/Singleton.hpp"

namespace IndieStudio {

    Game *Game::INSTANCE = nullptr;

    Game::Game() : logger("game"), achievementManager(*this), modManager(*this)
    {}

    void Game::start()
    {
        this->logger.info("Starting game...");
        this->statisticManager.load();
        this->soundManager.init();
        this->fontManager.init();
        this->controlProviderManager.init();
        this->worldManager.init();
        this->modManager.init();
        this->sceneManager.init();

        irr::IrrlichtDevice *device = Singleton::getDevice();
        irr::video::IVideoDriver *driver = device->getVideoDriver();
        bool stop = false;

        irr::core::array<irr::SJoystickInfo> joystickInfo;
        device->activateJoysticks(joystickInfo);

        this->logger.info("Starting main loop...");

        while (device->run() && !stop) {
            if (!device->isWindowActive()) {
                device->yield();
                continue;
            }

            driver->beginScene(true, true, irr::video::SColor(255, 255, 255, 255));
            this->sceneManager.draw();
            driver->endScene();
        }
    }

}