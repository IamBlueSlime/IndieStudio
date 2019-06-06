/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems ApplyExplosionSystem.hpp
*/

#pragma once

#include "indiestudio/Constants.hpp"
#include "indiestudio/ecs/Components.hpp"
#include "indiestudio/ecs/BaseSystem.hpp"
<<<<<<< Updated upstream
=======
#include "indiestudio/world/MapPattern.hpp"
#include "indiestudio/world/IWorld.hpp"
>>>>>>> Stashed changes

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    class ApplyExplosion : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager, IndieStudio::IWorld *world) {
            IndieStudio::MapPattern *pattern = world->getPattern();

            manager.template forEntitiesWith<IsBomb, Position, IsExploding, ExplosionRange>(
            [&manager, &pattern](auto &data, [[gnu::unused]] auto id) {

                auto &bombPosition = manager.template getComponent<Position>(data);
                auto &bombRange = manager.template getComponent<ExplosionRange>(data);
                manager.template forEntitiesWith<Node, Position>(
                [&manager, &bombPosition, &bombRange, &pattern](auto &data, [[gnu::unused]] auto id) {
                    auto &position = manager.template getComponent<Position>(data);
                    auto &node = manager.template getComponent<Node>(data);
                    std::pair<short, short> posInTile;

                    for (float i = 0; i < bombRange.explosionRangeUp; i += 1.0f) {
                        if (static_cast<int>(bombPosition.z + (i * 20)) == static_cast<int>(position.z) &&
                            static_cast<int>(bombPosition.x) == static_cast<int>(position.x) &&
                            static_cast<int>(bombPosition.y) == static_cast<int>(position.y)) {
                            posInTile = pattern->positionstoTile(position.x, position.z);
                            IndieStudio::MapPattern::TileType tile = pattern->get(posInTile.first, 1, posInTile.second);
                            if (tile == IndieStudio::MapPattern::TileType::INNER_WALL_BLOCK ||
                                tile == IndieStudio::MapPattern::TileType::BORDER_WALL_BLOCK) {
                                bombRange.explosionRangeUp = i;
                                break;
                            } else if (tile == IndieStudio::MapPattern::TileType::BREAKABLE_BLOCK) {
                                node.node->setVisible(false);
                                bombRange.explosionRangeUp = i;
                                break;
                            }
                        }
                    }
                    for (float i = 0; i < bombRange.explosionRangeDown; i += 1.0f) {
                        if (static_cast<int>(bombPosition.z - (i * 20)) == static_cast<int>(position.z) &&
                            static_cast<int>(bombPosition.x) == static_cast<int>(position.x) &&
                            static_cast<int>(bombPosition.y) == static_cast<int>(position.y)) {
                            posInTile = pattern->positionstoTile(position.x, position.z);
                            IndieStudio::MapPattern::TileType tile = pattern->get(posInTile.first, 1, posInTile.second);
                            if (tile == IndieStudio::MapPattern::TileType::INNER_WALL_BLOCK ||
                                tile == IndieStudio::MapPattern::TileType::BORDER_WALL_BLOCK) {
                                bombRange.explosionRangeDown = i;
                                break;
                            } else if (tile == IndieStudio::MapPattern::TileType::BREAKABLE_BLOCK) {
                                std::cout << "i = " << i << std::endl;
                                std::cout << "Something to destroy !" << std::endl;
                                node.node->setVisible(false);
                                pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::EMPTY);
                                bombRange.explosionRangeDown = i;
                                break;
                            }
                        }
                    }
                    for (float i = 0; i < bombRange.explosionRangeLeft; i += 1.0f) {
                        if (static_cast<int>(bombPosition.x - (i * 20)) == static_cast<int>(position.x) &&
                            static_cast<int>(bombPosition.z) == static_cast<int>(position.z) &&
                            static_cast<int>(bombPosition.y) == static_cast<int>(position.y)) {
                            posInTile = pattern->positionstoTile(position.x, position.z);
                            IndieStudio::MapPattern::TileType tile = pattern->get(posInTile.first, 1, posInTile.second);
                            if (tile == IndieStudio::MapPattern::TileType::INNER_WALL_BLOCK ||
                                tile == IndieStudio::MapPattern::TileType::BORDER_WALL_BLOCK) {
                                bombRange.explosionRangeLeft = i;
                                break;
                            } else if (tile == IndieStudio::MapPattern::TileType::BREAKABLE_BLOCK) {
                                node.node->setVisible(false);
                                bombRange.explosionRangeLeft = i;
                                break;
                            }
                        }
                    }
                    for (float i = 0; i < bombRange.explosionRangeRight; i += 1.0f) {
                        if (static_cast<int>(bombPosition.x + (i * 20)) == static_cast<int>(position.x) &&
                            static_cast<int>(bombPosition.z) == static_cast<int>(position.z) &&
                            static_cast<int>(bombPosition.y) == static_cast<int>(position.y)) {
                            posInTile = pattern->positionstoTile(position.x, position.z);
                            IndieStudio::MapPattern::TileType tile = pattern->get(posInTile.first, 1, posInTile.second);
                            if (tile == IndieStudio::MapPattern::TileType::INNER_WALL_BLOCK ||
                                tile == IndieStudio::MapPattern::TileType::BORDER_WALL_BLOCK) {
                                bombRange.explosionRangeRight = i;
                                break;
                            } else if (tile == IndieStudio::MapPattern::TileType::BREAKABLE_BLOCK) {
                                node.node->setVisible(false);
                                bombRange.explosionRangeRight = i;
                                break;
                            }
                        }
                    }
                });
            });
        }
    };
}