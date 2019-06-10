/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** SolidBombSystem
*/

#pragma once

#include "indiestudio/world/World.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template <typename ManagerType>
    class SolidBomb : public BaseSystem<ManagerType> {
        public:
            void process(ManagerType &manager, IndieStudio::World *world) {
                auto pattern = getWorld(world)->getPattern();
                manager.template forEntitiesWith<IsBomb, Node, NotSolid, Position, PosedBy>(
                [&](auto &data, [[gnu::unused]] auto id) {
                    const auto &position = manager.template getComponent<Position>(data);
                    const auto BombInTile = pattern->positionToTile(position.x, position.z);

                    const auto &PosedByID = manager.template getComponent<PosedBy>(data);
                    const auto &player_pos = manager.template getComponent<Position>(PosedByID.id);
                    const auto playerInTile = pattern->positionToTile(player_pos.x, player_pos.z);

                    if (BombInTile.first == playerInTile.first &&
                        BombInTile.second == playerInTile.second) {
                        return;
                    }
                    auto node = manager.template getComponent<Node>(data).node;
                    auto selector = this->getWorld(world)->getScene().scene->createTriangleSelectorFromBoundingBox(node);
                    node->setTriangleSelector(selector);
                    this->getWorld(world)->getMeta()->addTriangleSelector(selector);
                    manager.template unsetComponent<NotSolid>(data);
                });
            }
        private:
            IWorld *getWorld(IWorld *world) { return world; }
    };
}