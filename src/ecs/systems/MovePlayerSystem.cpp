/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** MovePlayerSystem
*/

#include "indiestudio/ecs/systems/MovePlayerSystem.hpp"
#include "indiestudio/world/World.hpp"

namespace IndieStudio::ECS::System {
    template<typename ManagerType>
    bool MovePlayer<ManagerType>::isColliding(short playerX, short playerZ, short directionX, short directionZ, World *world) {
        return world->getPattern()->isWalkable(playerX + directionX, playerZ + directionZ);
    }
}