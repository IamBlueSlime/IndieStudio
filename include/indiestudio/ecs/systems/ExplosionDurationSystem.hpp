/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems ExplosionDurationSystem.hpp
*/

#pragma once

#include "indiestudio/ecs/Components.hpp"
#include "indiestudio/ecs/BaseSystem.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    class ExplosionDuration : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager, IndieStudio::World *world) {
            manager.template forEntitiesWith<IsBomb, IsExploding, ExplosionLifeTime, ExplosionRange, Position>(
                [this, &manager, &world](auto &data, [[gnu::unused]] auto id) {
                    auto &explosionTime = manager.template getComponent<ExplosionLifeTime>(data);
                    auto &range = manager.template getComponent<ExplosionRange>(data);
                    auto &position = manager.template getComponent<Position>(data);
                    auto pattern = this->getWorld(world)->getPattern();
                    std::pair<short, short> posInTile;

                     if ((std::time(nullptr) - explosionTime.explosionLifeTime) >= 1) {
                        manager.template unsetComponent<IsExploding>(data);
                        manager.template unsetComponent<ExplosionLifeTime>(data);
                        for (float i = 0; i < range.explosionRangeUp; i++) {
                            posInTile = pattern->positionToTile(position.x, position.z + (i * 20));
                            pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::EMPTY);
                        }
                        for (float i = 0; i < range.explosionRangeDown; i++) {
                            posInTile = pattern->positionToTile(position.x, position.z - (i * 20));
                            pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::EMPTY);
                        }
                        for (float i = 0; i < range.explosionRangeLeft; i++) {
                            posInTile = pattern->positionToTile(position.x - (i * 20), position.z);
                            pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::EMPTY);
                        }
                        for (float i = 0; i < range.explosionRangeRight; i++) {
                            posInTile = pattern->positionToTile(position.x + (i * 20), position.z);
                            pattern->set(posInTile.first, 1, posInTile.second, IndieStudio::MapPattern::TileType::EMPTY);
                        }
                        manager.delEntity(data);
                     }
            });
        }
    protected:
    private:
        IWorld *getWorld(IWorld *world) { return world; }
    };
}