/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** DeathSystem
*/

#pragma once

#include "indiestudio/world/World.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template <typename ManagerType>
    class DeathSystem {
    	public:
            void process(ManagerType &manager, IndieStudio::World *world) {
                auto pattern = getWorld(world)->getPattern();
                manager.template forEntitiesWith<IsPlayer, Position, Node>(
                [&] (auto &data, [[gnu::unused]] auto id) {
                    const auto &position = manager.template getComponent<Position>(data);
                    auto posInTile = pattern->positionToTile(position.x, position.z);
                    const auto &tile = pattern->get(posInTile.first, 1, posInTile.second);
                    auto &node = manager.template getComponent<Node>(data);
                    irr::core::vector3df explosionCenter(position.x, 61, position.z);

                    if (tile == MapPattern::TileType::BOMB_EXPLOSION) {
                        manager.template unsetComponent<Alive>(data);
                        getWorld(world)->eject(node.node, explosionCenter);
                    }
                });
            }
    	private:
            IWorld *getWorld(IWorld *world) { return world; }
    };
}