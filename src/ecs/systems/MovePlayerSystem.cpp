/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems MovePlayerSystem.hpp
*/

#include "indiestudio/ecs/systems/MovePlayerSystem.hpp"
#include "indiestudio/world/World.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    void MovePlayerSystem<ManagerType>::process(ManagerType &manager, World *world)
    {
        (void) world;

        manager.template forEntitiesWith<Position, Speed, Movable>(
            [&manager](auto &data, [[gnu::unused]] auto id) {
                auto &pos = manager.template getComponent<Position>(data);
                auto &speed = manager.template getComponent<Speed>(data);
                pos.x += speed.x;
                pos.y += speed.y;
                pos.z += speed.z;
        });
    }

    template class MovePlayerSystem<WorldECS>;

}