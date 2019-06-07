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
#include "indiestudio/world/MapPattern.hpp"
#include "indiestudio/world/IWorld.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    class ApplyExplosion : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager, IndieStudio::IWorld *world) {
            IndieStudio::MapPattern *pattern = world->getPattern();

            manager.template forEntitiesWith<IsBomb, Position, IsExploding, ExplosionRange>(
            [&manager, &pattern, &world](auto &data, [[gnu::unused]] auto id) {

                auto &bombPosition = manager.template getComponent<Position>(data);
                auto &bombRange = manager.template getComponent<ExplosionRange>(data);
                std::pair<short, short> posInTile;

                for (float i = 0; i < bombRange.explosionRangeUp; i += 1.0f) {
                    posInTile = pattern->positionToTile(bombPosition.x, bombPosition.z + (i * 20));
                    IndieStudio::MapPattern::TileType tile = pattern->get(posInTile.first, 1, posInTile.second);
                    if (tile == IndieStudio::MapPattern::TileType::INNER_WALL_BLOCK ||
                        tile == IndieStudio::MapPattern::TileType::BORDER_WALL_BLOCK) {
                        bombRange.explosionRangeUp = i;
                        break;
                    } else if (tile == IndieStudio::MapPattern::TileType::BREAKABLE_BLOCK) {
                        auto entity = world->getBlockEntityIdByPos(posInTile.first, posInTile.second);
                        auto &node = manager.template getComponent<Node>(entity);
                        irr::scene::IMetaTriangleSelector *meta = world->getMeta();
                        meta->removeTriangleSelector(node.node->getTriangleSelector());
                        node.node->setVisible(false);
                        manager.delEntity(entity);
                        bombRange.explosionRangeUp = i;
                        //TODO: add power up if it drops
                        pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::EMPTY);
                        break;
                    } else {
                        pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
                    }
                }
                for (float i = 0; i < bombRange.explosionRangeDown; i += 1.0f) {
                    posInTile = pattern->positionToTile(bombPosition.x, bombPosition.z - (i * 20));
                    IndieStudio::MapPattern::TileType tile = pattern->get(posInTile.first, 1, posInTile.second);
                    if (tile == IndieStudio::MapPattern::TileType::INNER_WALL_BLOCK ||
                        tile == IndieStudio::MapPattern::TileType::BORDER_WALL_BLOCK) {
                        bombRange.explosionRangeDown = i;
                        break;
                    } else if (tile == IndieStudio::MapPattern::TileType::BREAKABLE_BLOCK) {
                        auto entity = world->getBlockEntityIdByPos(posInTile.first, posInTile.second);
                        auto &node = manager.template getComponent<Node>(entity);
                        irr::scene::IMetaTriangleSelector *meta = world->getMeta();
                        meta->removeTriangleSelector(node.node->getTriangleSelector());
                        node.node->setVisible(false);
                        manager.delEntity(entity);
                        bombRange.explosionRangeDown = i;
                        //TODO: add power up if it drops
                        pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::EMPTY);
                        break;
                    } else {
                        pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
                    }
                }
                for (float i = 0; i < bombRange.explosionRangeLeft; i += 1.0f) {
                    posInTile = pattern->positionToTile(bombPosition.x - (i * 20), bombPosition.z);
                    IndieStudio::MapPattern::TileType tile = pattern->get(posInTile.first, 1, posInTile.second);
                    if (tile == IndieStudio::MapPattern::TileType::INNER_WALL_BLOCK ||
                        tile == IndieStudio::MapPattern::TileType::BORDER_WALL_BLOCK) {
                        bombRange.explosionRangeLeft = i;
                        break;
                    } else if (tile == IndieStudio::MapPattern::TileType::BREAKABLE_BLOCK) {
                        auto entity = world->getBlockEntityIdByPos(posInTile.first, posInTile.second);
                        auto &node = manager.template getComponent<Node>(entity);
                        irr::scene::IMetaTriangleSelector *meta = world->getMeta();
                        meta->removeTriangleSelector(node.node->getTriangleSelector());
                        node.node->setVisible(false);
                        manager.delEntity(entity);
                        bombRange.explosionRangeLeft = i;
                        //TODO: add power up if it drops
                        pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::EMPTY);
                        break;
                    } else {
                        pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
                    }
                }
                for (float i = 0; i < bombRange.explosionRangeRight; i += 1.0f) {
                    posInTile = pattern->positionToTile(bombPosition.x + (i * 20), bombPosition.z);
                    IndieStudio::MapPattern::TileType tile = pattern->get(posInTile.first, 1, posInTile.second);
                    if (tile == IndieStudio::MapPattern::TileType::INNER_WALL_BLOCK ||
                        tile == IndieStudio::MapPattern::TileType::BORDER_WALL_BLOCK) {
                        bombRange.explosionRangeRight = i;
                        break;
                    } else if (tile == IndieStudio::MapPattern::TileType::BREAKABLE_BLOCK) {
                        auto entity = world->getBlockEntityIdByPos(posInTile.first, posInTile.second);
                        auto &node = manager.template getComponent<Node>(entity);
                        irr::scene::IMetaTriangleSelector *meta = world->getMeta();
                        meta->removeTriangleSelector(node.node->getTriangleSelector());
                        node.node->setVisible(false);
                        manager.delEntity(entity);
                        bombRange.explosionRangeRight = i;
                        //TODO: add power up if it drops
                        pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::EMPTY);
                        break;
                    } else {
                        pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
                    }
                }
            });
        }
    };
}