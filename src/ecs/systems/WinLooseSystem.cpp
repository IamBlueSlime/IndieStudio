/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** WinLooseSystem
*/

#include <irrlicht/irrlicht.h>
#include "indiestudio/ecs/systems/WinLooseSystem.hpp"
#include "indiestudio/world/World.hpp"
#include "indiestudio/common/Scheduler.hpp"
#include "indiestudio/Game.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template <typename ManagerType>
    void WinLooseSystem<ManagerType>::process(ManagerType &manager, [[gnu::unused]] IndieStudio::World *world) {
        int deadPlayer = 0;
        int winnerPlayerID = 0;
        static int already = 0;

        if (already == 1)
            return;

        manager.template forEntitiesWith<RealPlayer>(
        [&] (auto &data, [[gnu::unused]] auto id) {
            try {
                manager.template getComponent<Alive>(data);
                winnerPlayerID = id;
            } catch (...) {
                deadPlayer++;
            }
        });

        manager.template forEntitiesWith<IA>(
        [&] (auto &data, [[gnu::unused]] auto id) {
            try {
                manager.template getComponent<Alive>(data);
                winnerPlayerID = id;
            } catch (...) {
                deadPlayer++;
            }
        });

        if (deadPlayer == 4) {
            std::cout << "Withdraw" << std::endl;
        } else if (deadPlayer == 3) {
            manager.template unsetComponent<Alive>(winnerPlayerID);
            auto &stat = manager.template getComponent<Stat>(winnerPlayerID);
            auto position = manager.template getComponent<Position>(winnerPlayerID);
            auto W = static_cast<World *>(Game::INSTANCE->getWorldManager().getLoadedWorld());
            auto animCircle = W->getScene().scene->createFlyCircleAnimator(
                {position.x, position.y + 100, position.z},
                100.0f,
                0.0005f,
                {0, 1, 0});
            W->getScene().scene->getActiveCamera()->addAnimator(animCircle);
            W->getScene().scene->getActiveCamera()->setTarget({position.x, position.y, position.z});
            animCircle->drop();
            stat.winner = true;
            already = 1;
        }
    }
    template class WinLooseSystem<WorldECS>;
}