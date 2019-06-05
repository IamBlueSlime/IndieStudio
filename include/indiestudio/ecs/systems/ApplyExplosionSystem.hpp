/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems ApplyExplosionSystem.hpp
*/

#pragma once

#include "indiestudio/ecs/Components.hpp"
#include "indiestudio/ecs/BaseSystem.hpp"
#include "unistd.h"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    class ApplyExplosion : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager, World *world) override {
            (void) world;
            manager.template forEntitiesWith<IsBomb, Position, IsExploding, ExplosionRange>(
            [&manager](auto &data, [[gnu::unused]] auto id) {

                auto &bombPosition = manager.template getComponent<Position>(data);
                auto &bombRange = manager.template getComponent<ExplosionRange>(data);

                manager.template forEntitiesWith<Node, Position, Alive>(
                [&manager, &bombPosition, &bombRange](auto &data, [[gnu::unused]] auto id) {

                    auto &position = manager.template getComponent<Position>(data);
                    auto &node = manager.template getComponent<Node>(data);

                    for (float i = 0; i <= bombRange.explosionRangeUp; i += 1.0) {
                        if (static_cast<int>(bombPosition.z + (i * 20)) == static_cast<int>(position.z) &&
                            static_cast<int>(bombPosition.x) == static_cast<int>(position.x)) {
                            node.node->setVisible(false);
                            bombRange.explosionRangeUp = i;
                            break;
                        }
                    }
                    for (float i = 0; i <= bombRange.explosionRangeDown; i += 1.0) {
                        if (static_cast<int>(bombPosition.z - (i * 20)) == static_cast<int>(position.z) &&
                            static_cast<int>(bombPosition.x) == static_cast<int>(position.x)) {
                            node.node->setVisible(false);
                            bombRange.explosionRangeDown = i;
                            break;
                        }
                    }
                    for (float i = 0; i <= bombRange.explosionRangeLeft; i += 1.0) {
                        if (static_cast<int>(bombPosition.x - (i * 20)) == static_cast<int>(position.x) &&
                            static_cast<int>(bombPosition.z) == static_cast<int>(position.z)) {
                            node.node->setVisible(false);
                            bombRange.explosionRangeLeft = i;
                            break;
                        }
                    }
                    for (float i = 0; i <= bombRange.explosionRangeRight; i += 1.0) {
                        if (static_cast<int>(bombPosition.x + (i * 20)) == static_cast<int>(position.x) &&
                            static_cast<int>(bombPosition.z) == static_cast<int>(position.z)) {
                            node.node->setVisible(false);
                            bombRange.explosionRangeRight = i;
                            break;
                        }
                    }
                });
            });
        }
    };
}