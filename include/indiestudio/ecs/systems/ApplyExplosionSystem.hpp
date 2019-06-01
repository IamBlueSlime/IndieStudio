/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems ApplyExplosionSystem.hpp
*/

#pragma once

#include "indiestudio/ecs/Components.hpp"
#include "indiestudio/ecs/BaseSystem.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    class ApplyExplosion : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager) override {
            manager.template forEntitiesWith<IsBomb, Position, IsExploding, ExplosionRange>(
            [&manager](auto &data, [[gnu::unused]] auto id) {

                auto &bombPosition = manager.template getComponent<Position>(data);
                auto &bombRange = manager.template getComponent<ExplosionRange>(data);

                manager.template forEntitiesWith<Position, Alive>(
                [&manager, &bombPosition, &bombRange](auto &data, [[gnu::unused]] auto id) {

                    auto &position = manager.template getComponent<Position>(data);

                    for (float i = 0; i <= bombRange.explosionRangeUp; i++) {
                        if (static_cast<int>(bombPosition.z + i) == static_cast<int>(position.z)) {
                            manager.template unsetComponent<Alive>(data);
                            bombRange.explosionRangeUp = i;
                            break;
                        }
                    }
                    for (float i = 0; i <= bombRange.explosionRangeDown; i++) {
                        if (static_cast<int>(bombPosition.z - i) == static_cast<int>(position.z)) {
                            manager.template unsetComponent<Alive>(data);
                            bombRange.explosionRangeDown = i;
                            break;
                        }
                    }
                    for (float i = 0; i <= bombRange.explosionRangeLeft; i++) {
                        if (static_cast<int>(bombPosition.x + i) == static_cast<int>(position.x)) {
                            manager.template unsetComponent<Alive>(data);
                            bombRange.explosionRangeLeft = i;
                            break;
                        }
                    }
                    for (float i = 0; i <= bombRange.explosionRangeRight; i++) {
                        if (static_cast<int>(bombPosition.x - i) == static_cast<int>(position.x)) {
                            manager.template unsetComponent<Alive>(data);
                            bombRange.explosionRangeRight = i;
                            break;
                        }
                    }
                });
            });
        }
    
    protected:
    private:
    };
}