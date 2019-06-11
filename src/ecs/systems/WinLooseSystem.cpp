/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** WinLooseSystem
*/

#include "indiestudio/ecs/systems/WinLooseSystem.hpp"
#include "indiestudio/world/World.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template <typename ManagerType>
    void WinLooseSystem<ManagerType>::process(ManagerType &manager, [[gnu::unused]] IndieStudio::World *world) {
        int deadPlayer = 0;
        int winnerPlayerID = 0;

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

        // TODO: do traveling to the winner player
        // if (deadPlayer == 4) {
        //     std::cout << "Equality" << std::endl;
        // } else if (deadPlayer == 3) {
        //     std::cout << "Win du player " << winnerPlayerID << std::endl;
        // }
    }
    template class WinLooseSystem<WorldECS>;
}