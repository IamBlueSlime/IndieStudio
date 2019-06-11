/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems ApplyExplosionSystem.hpp
*/

#include "indiestudio/ecs/systems/ExplosionDurationSystem.hpp"
#include "indiestudio/world/World.hpp"

namespace IndieStudio::ECS::System {

    template<typename ManagerType>
    void ExplosionDurationSystem<ManagerType>::process(ManagerType &manager, World *world)
    {
        manager.template forEntitiesWith<IsBomb, IsExploding, ExplosionLifeTime, ExplosionRange, Position>(
            [&manager, &world](auto &data, [[gnu::unused]] auto id) {
                auto &explosionTime = manager.template getComponent<ExplosionLifeTime>(data);
                auto &range = manager.template getComponent<ExplosionRange>(data);
                auto &position = manager.template getComponent<Position>(data);
                auto pattern = world->getPattern();
                std::pair<short, short> posInTile;

                if (std::time(nullptr) - explosionTime.explosionLifeTime >= 1) {
                    manager.delEntity(data);
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
                }
        });
    }

    template class ExplosionDurationSystem<WorldECS>;

}