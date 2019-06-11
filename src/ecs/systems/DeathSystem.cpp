/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems ApplyExplosionSystem.hpp
*/

#include "indiestudio/ecs/systems/DeathSystem.hpp"
#include "indiestudio/world/World.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    void DeathSystem<ManagerType>::process(ManagerType &manager, World *world)
    {
        auto pattern = world->getPattern();

        manager.template forEntitiesWith<IsPlayer, Position, Node, Stat>(
        [&] (auto &data, [[gnu::unused]] auto id) {
            const auto &position = manager.template getComponent<Position>(data);
            auto posInTile = pattern->positionToTile(position.x, position.z);
            const auto &tile = pattern->get(posInTile.first, 1, posInTile.second);
            auto &node = manager.template getComponent<Node>(data);
            auto &stat = manager.template getComponent<Stat>(data);
            irr::core::vector3df explosionCenter(position.x, 61, position.z);

            if (tile == MapPattern::TileType::BOMB_EXPLOSION) {
                manager.template unsetComponent<Alive>(data);
                stat.alive = false;
                world->eject(node.node, explosionCenter);
            }
        });
    }

    template class DeathSystem<WorldECS>;

}