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
            
            manager.template forEntitiesWith<IsPowerUp>(
                [&manager, &playerPos, &stat, &speed, tilemap, this](auto &entity, [[gnu::unused]] auto id) {
                    auto tilePlayerPos = MapPattern::positionToTile(playerPos.x, playerPos.z);

                    if (this->isOnPowerUp(tilePlayerPos, tilemap)) {
                        auto &node = manager.template getComponent<Node>(entity);
                        node.node->setVisible(false);
                        tilemap->set(tilePlayerPos.first, 1, tilePlayerPos.second, MapPattern::TileType::EMPTY);
                        manager.delEntity(entity);
                        applyPowerUp(speed, stat);
                    }
            });
        });
    }

    template<typename ManagerType>
    bool PickUpPowerUpSystem<ManagerType>::isOnPowerUp(
        const std::pair<std::size_t, std::size_t> &player, MapPattern *map)
    {
        if (map->get(player.first, 1, player.second) == MapPattern::TileType::POWER_UP)
            return true;

        return false;
    }

    template<typename ManagerType>
    void PickUpPowerUpSystem<ManagerType>::applyPowerUp(Speed &speed, Stat &stat)
    {
        std::cout << "POWER UP APPLIED !" << std::endl;
        speed.x += 1;
        speed.y += 1;
        speed.z += 1;
        stat.bomb += 1;
        stat.range += 1;
    }

    template class PickUpPowerUpSystem<WorldECS>;

}