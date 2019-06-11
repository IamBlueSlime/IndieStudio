/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems ApplyExplosionSystem.hpp
*/

#include "indiestudio/ecs/systems/ApplyExplosionSystem.hpp"
#include "indiestudio/world/World.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template <typename ManagerType>
    void ApplyExplosionSystem<ManagerType>::chainBomb(ManagerType &manager, int entityID, int bombID)
    {
        if (!manager.template hasComponent<IsExploding>(entityID)) {
            auto &node = manager.template getComponent<Node>(entityID);
            auto &playerID = manager.template getComponent<PosedBy>(bombID);
            auto &stat = manager.template getComponent<Stat>(playerID.id);
            manager.setComponent(entityID, IsExploding());
            manager.setComponent(entityID, ExplosionLifeTime());
            manager.template unsetComponent<LifeTime>(entityID);
            node.node->setVisible(false);
            stat.bomb++;
        }
    }

    template <typename ManagerType>
    void ApplyExplosionSystem<ManagerType>::destroyBlock(ManagerType &manager, World *world, std::pair<short, short> posInTile) {
        auto pattern = world->getPattern();
        auto entity = world->getBlockEntityIdByPos(posInTile.first, posInTile.second);
        auto &node = manager.template getComponent<Node>(entity);
        irr::scene::IMetaTriangleSelector *meta = world->getMeta();
        meta->removeTriangleSelector(node.node->getTriangleSelector());
        node.node->setVisible(false);
        manager.delEntity(entity);
        world->createBlast(irr::core::vector3df(posInTile.first * 20 + 0.5, 70, posInTile.second * 20 + 0.5), 1100);
        pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::EMPTY);
        this->dropPowerUp(world, posInTile);
    }

    template <typename ManagerType>
    int ApplyExplosionSystem<ManagerType>::findBombByID(ManagerType &manager, MapPattern *pattern, std::pair<short, short> posInTile)
    {
        int entityID = 0;

        manager.template forEntitiesWith<IsBomb, Position, Node>(
            [&]([[gnu::unused]] auto &data, auto id) {
                auto pos = manager.template getComponent<Position>(id);
                auto BombInTile = pattern->positionToTile(pos.x, pos.z);
                if (BombInTile.first == posInTile.first &&
                    BombInTile.second == posInTile.second)
                    entityID = id;
            }
        );
        return entityID;
    }

    template<typename ManagerType>
    void ApplyExplosionSystem<ManagerType>::process(ManagerType &manager, World *world)
    {
        IndieStudio::MapPattern *pattern = world->getPattern();

        manager.template forEntitiesWith<IsBomb, Position, IsExploding, ExplosionRange, ExplosionLifeTime>(
        [this, &manager, &pattern, &world](auto &data, auto id) {
            auto &bombPosition = manager.template getComponent<Position>(data);
            auto &bombRange = manager.template getComponent<ExplosionRange>(data);
            auto &explosionTime = manager.template getComponent<ExplosionLifeTime>(data);
            std::pair<short, short> posInTile;

            if (std::time(nullptr) - explosionTime.explosionLifeTime >= 1) {
                return;
            }
            auto &node = manager.template getComponent<Node>(data);
            irr::scene::IMetaTriangleSelector *meta = world->getMeta();
            meta->removeTriangleSelector(node.node->getTriangleSelector());

            for (float i = 0; i < bombRange.explosionRangeUp; i += 1.0f) {
                posInTile = pattern->positionToTile(bombPosition.x, bombPosition.z + (i * 20));
                IndieStudio::MapPattern::TileType tile = pattern->get(posInTile.first, 1, posInTile.second);
                if (tile == IndieStudio::MapPattern::TileType::INNER_WALL_BLOCK ||
                    tile == IndieStudio::MapPattern::TileType::BORDER_WALL_BLOCK) {
                    bombRange.explosionRangeUp = i;
                    break;
                } else if (tile == IndieStudio::MapPattern::TileType::BREAKABLE_BLOCK) {
                    destroyBlock(manager, world, posInTile);
                    bombRange.explosionRangeUp = i;
                    world->createBlast(irr::core::vector3df(posInTile.first * 20 + 0.5, 70, posInTile.second * 20 + 0.5), 1100);
                    pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::EMPTY);
                    this->dropPowerUp(world, posInTile);
                    break;
                }  else if (tile == IndieStudio::MapPattern::TileType::BOMB) {
                    std::size_t entityID = findBombByID(manager, pattern, posInTile);
                    chainBomb(manager, entityID, id);
                    pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
                    world->createBlast(irr::core::vector3df(posInTile.first * 20 + 0.5, 70, posInTile.second * 20 + 0.5), 1100);
                } else {
                    if (pattern->get(posInTile.first, 1, posInTile.second) != IndieStudio::MapPattern::TileType::BOMB_EXPLOSION) {
                        world->createBlast(irr::core::vector3df(posInTile.first * 20 + 0.5, 70, posInTile.second * 20 + 0.5), 1100);
                        pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
                    }
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
                    destroyBlock(manager, world, posInTile);
                    bombRange.explosionRangeDown = i;
                    world->createBlast(irr::core::vector3df(posInTile.first * 20 + 0.5, 70, posInTile.second * 20 + 0.5), 1100);
                    pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::EMPTY);
                    this->dropPowerUp(world, posInTile);
                    break;
                } else if (tile == IndieStudio::MapPattern::TileType::BOMB) {
                    std::size_t entityID = findBombByID(manager, pattern, posInTile);
                    chainBomb(manager, entityID, id);
                    pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
                    world->createBlast(irr::core::vector3df(posInTile.first * 20 + 0.5, 70, posInTile.second * 20 + 0.5), 1100);
                } else {
                    if (pattern->get(posInTile.first, 1, posInTile.second) != IndieStudio::MapPattern::TileType::BOMB_EXPLOSION) {
                        pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
                        world->createBlast(irr::core::vector3df(posInTile.first * 20 + 0.5, 70, posInTile.second * 20 + 0.5), 1100);
                    }
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
                    destroyBlock(manager, world, posInTile);
                    bombRange.explosionRangeLeft = i;
                    world->createBlast(irr::core::vector3df(posInTile.first * 20 + 0.5, 70, posInTile.second * 20 + 0.5), 1100);
                    pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::EMPTY);
                    this->dropPowerUp(world, posInTile);
                    break;
                } else if (tile == IndieStudio::MapPattern::TileType::BOMB) {
                    std::size_t entityID = findBombByID(manager, pattern, posInTile);
                    chainBomb(manager, entityID, id);
                    pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
                    world->createBlast(irr::core::vector3df(posInTile.first * 20 + 0.5, 70, posInTile.second * 20 + 0.5), 1100);
                } else {
                    if (pattern->get(posInTile.first, 1, posInTile.second) != IndieStudio::MapPattern::TileType::BOMB_EXPLOSION) {
                        world->createBlast(irr::core::vector3df(posInTile.first * 20 + 0.5, 70, posInTile.second * 20 + 0.5), 1100);
                        pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
                    }
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
                    destroyBlock(manager, world, posInTile);
                    bombRange.explosionRangeRight = i;
                    world->createBlast(irr::core::vector3df(posInTile.first * 20 + 0.5, 70, posInTile.second * 20 + 0.5), 1100);
                    pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::EMPTY);
                    this->dropPowerUp(world, posInTile);
                    break;
                }  else if (tile == IndieStudio::MapPattern::TileType::BOMB) {
                    std::size_t entityID = findBombByID(manager, pattern, posInTile);
                    chainBomb(manager, entityID, id);
                    pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
                    world->createBlast(irr::core::vector3df(posInTile.first * 20 + 0.5, 70, posInTile.second * 20 + 0.5), 1100);
                } else {
                    if (pattern->get(posInTile.first, 1, posInTile.second) != IndieStudio::MapPattern::TileType::BOMB_EXPLOSION) {
                        world->createBlast(irr::core::vector3df(posInTile.first * 20 + 0.5, 70, posInTile.second * 20 + 0.5), 1100);
                        pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::BOMB_EXPLOSION);
                    }
                }
            }
        });
    }

    template<typename ManagerType>
    void ApplyExplosionSystem<ManagerType>::dropPowerUp(World *world, std::pair<short, short> posInTile) {
        int spawnRand = rand() % 3;
        int typeRand;
        IndieStudio::MapPattern *pattern = world->getPattern();
        if (spawnRand != 1)
            return;
        typeRand = rand() % 3;
        world->createPowerUp(typeRand, Position(posInTile.first * 20 + 0.5, 80, posInTile.second * 20 + 0.5));
        pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::POWER_UP);
    }

    template class ApplyExplosionSystem<WorldECS>;

}