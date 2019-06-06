/*
** EPITECH PROJECT, 2019
** indie studio
** File description:
** pickup powerup
*/

#pragma once


#include "indiestudio/world/MapPattern.hpp"
#include "indiestudio/ecs/Components.hpp"
#include "indiestudio/ecs/BaseSystem.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    class PickUpPowerUp : public BaseSystem<ManagerType> {
    public:

        struct Coord {
            std::size_t x;
            std::size_t y;
        };

        bool isOnPowerUp(Coord player, Coord powerup, MapPattern *map) {
            if (map->get(player.x, 1, player.y) == map->get(powerup.x, 1, powerup.y))
                return true;
            return false;
        }

        Coord convertPosition(Position p) {
            Coord coord = {static_cast<std::size_t>(p.x / 20), static_cast<std::size_t>(p.z / 20)};
            return coord;
        }

        void process(ManagerType &manager, World *world) override {
            manager.template forEntitiesWith<IsPlayer, Position>(
                [&manager, world, this](auto &data, [[gnu::unused]] auto id) {
                    auto &playerPos = manager.template getComponent<Position>(data);
                    MapPattern *tilemap = (static_cast<IWorld*>(world))->getPattern();

                    manager.template forEntitiesWith<IsPowerUp, Position>(
                        [&manager, &playerPos, tilemap, this](auto &data, [[gnu::unused]] auto id) {
                            auto &powerupPos = manager.template getComponent<Position>(data);
                            Coord tilePlayerPos = this->convertPosition(playerPos);
  
                            if (this->isOnPowerUp(tilePlayerPos, this->convertPosition(powerupPos), tilemap)) {
                                // update tilemap
                                tilemap->set(tilePlayerPos.x, 1, tilePlayerPos.y, MapPattern::TileType::PLAYER);
                                // apply powerup
                                std::cout << "POWER UP APPLIED !" << std::endl;
                                // delete entity
                                manager.template delEntity(data);
                            }
                        });
            });
        }
    protected:
    private:
    };
}