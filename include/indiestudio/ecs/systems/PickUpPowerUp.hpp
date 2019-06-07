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
#include "indiestudio/world/IWorld.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    class PickUpPowerUp : public BaseSystem<ManagerType> {
        public:

            struct Coord {
                std::size_t x;
                std::size_t y;
            };

            bool isOnPowerUp(Coord player, MapPattern *map) {
                if (map->get(player.x, 1, player.y) == MapPattern::TileType::POWER_UP)
                    return true;
                return false;
            }

            Coord convertPosition(Position p) {
                std::pair<short, short> tileScalePos = MapPattern::positionToTile(p.x, p.z);
                PickUpPowerUp::Coord coord = {static_cast<std::size_t>(tileScalePos.first), static_cast<std::size_t>(tileScalePos.second)};
                return coord;
            }

            void applyPowerUp(Speed &speed, Stat &stat) {
                std::cout << "POWER UP APPLIED !" << std::endl;
                speed.x += 1;
                speed.y += 1;
                speed.z += 1;
                stat.bomb += 1;
                stat.range += 1;
            }

            IWorld *getWorld(IWorld *world) { return world; }

            void process(ManagerType &manager, World *world) override {
                manager.template forEntitiesWith<IsPlayer, Position, Stat, Speed>(
                [&manager, world, this](auto &data, [[gnu::unused]] auto id) {
                    auto &playerPos = manager.template getComponent<Position>(data);
                    auto &speed = manager.template getComponent<Speed>(data);
                    auto &stat = manager.template getComponent<Stat>(data);
                    MapPattern *tilemap = getWorld(world)->getPattern();
                    
                    manager.template forEntitiesWith<IsPowerUp>(
                        [&manager, &playerPos, &stat, &speed, tilemap, this](auto &entity, [[gnu::unused]] auto id) {
                            PickUpPowerUp::Coord tilePlayerPos = this->convertPosition(playerPos);

                            if (this->isOnPowerUp(tilePlayerPos, tilemap)) {
                                auto &node = manager.template getComponent<Node>(entity);
                                node.node->setVisible(false);
                                tilemap->set(tilePlayerPos.x, 1, tilePlayerPos.y, MapPattern::TileType::EMPTY);
                                manager.delEntity(entity);
                                applyPowerUp(speed, stat);
                            }
                    });
                });
            }

        protected:
        private:
    };
}