/*
** EPITECH PROJECT, 2019
** drawEntities
** File description:
** System
*/

#pragma once

#include "../Components.hpp"
#include "../BaseSystem.hpp"
#include "indiestudio/Game.hpp"

namespace Ecs::System {

    using namespace Ecs::Component;

    template<typename ManagerType>
    struct drawEntities : public BaseSystem<ManagerType> {

        void process(ManagerType &manager) override {
            auto videoDrive = IndieStudio::Game::getDevice()->getVideoDriver();
            auto sceneManager = IndieStudio::Game::getDevice()->getSceneManager();

            videoDrive->beginScene(true, true, irr::video::SColor{255, 200, 200, 200});
            sceneManager->drawAll();
            videoDrive->endScene();
        }
    };
}