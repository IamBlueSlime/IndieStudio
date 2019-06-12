/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems PickUpPowerUpSystems.cpp
*/

#include "indiestudio/ecs/systems/PickUpPowerUpSystem.hpp"
#include "indiestudio/world/World.hpp"

namespace IndieStudio::ECS::System {

    template<typename ManagerType>
    void PickUpPowerUpSystem<ManagerType>::process(ManagerType &manager, World *world)
    {
        manager.template forEntitiesWith<IsPlayer, Position, Stat, Speed>(
        [&manager, world, this](auto &data, [[gnu::unused]] auto id) {
            auto &playerPos = manager.template getComponent<Position>(data);
            auto &speed = manager.template getComponent<Speed>(data);
            auto &stat = manager.template getComponent<Stat>(data);
            MapPattern *tilemap = world->getPattern();

            manager.template forEntitiesWith<IsPowerUp, Position>(
                [&manager, &playerPos, &stat, &speed, tilemap, this](auto &entity, [[gnu::unused]] auto id) {
                    auto &powerUpPos = manager.template getComponent<Position>(entity);
                    auto tilePlayerPos = MapPattern::positionToTile(playerPos.x, playerPos.z);
                    auto &powerUp =  manager.template getComponent<IsPowerUp>(entity);
                    auto tilePowerUpPos = MapPattern::positionToTile(powerUpPos.x, powerUpPos.z);

                    if (this->isOnPowerUp(tilePlayerPos, tilePowerUpPos)) {
                        auto &node = manager.template getComponent<Node>(entity);
                        node.node->setVisible(false);
                        tilemap->set(tilePlayerPos.first, 1, tilePlayerPos.second, MapPattern::TileType::EMPTY);
                        manager.delEntity(entity);
                        applyPowerUp(speed, stat, powerUp);
                    }
            });
        });
    }

    template<typename ManagerType>
    bool PickUpPowerUpSystem<ManagerType>::isOnPowerUp(
        const std::pair<std::size_t, std::size_t> &player,
        const std::pair<std::size_t, std::size_t> &powerUp)
    {
        if (player.first == powerUp.first && player.second == powerUp.second)
            return true;
        return false;
    }

    template<typename ManagerType>
    void PickUpPowerUpSystem<ManagerType>::applyPowerUp(Speed &speed, Stat &stat, IsPowerUp &powerUp)
    {
        if (powerUp.type == IsPowerUp::SPEED_POWERUP) {
            speed.x += 0.1;
            speed.y += 0.1;
            speed.z += 0.1;
        } else if (powerUp.type == IsPowerUp::STOCK_POWERUP)
            stat.bomb += 1;
        else
            stat.range += 1;
    }

    template class PickUpPowerUpSystem<WorldECS>;

}