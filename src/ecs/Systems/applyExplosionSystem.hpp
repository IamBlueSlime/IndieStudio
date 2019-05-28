/*
** EPITECH PROJECT, 2019
** applyexplosion
** File description:
** System
*/

#pragma once

#include "../Components.hpp"
#include "../BaseSystem.hpp"

namespace Ecs::System {

    using namespace Ecs::Component;

    template<typename ManagerType>
    struct applyExplosion : public BaseSystem<ManagerType> {

        void process(ManagerType &manager) override {
            manager.template forEntitiesWith<IsBomb, Position, IsExploding, ExplosionRange>(
                [&manager](auto &data, [[gnu::unused]] auto id) {

                    auto &bombPosition = manager.template getComponent<Position>(data);
                    auto &bombRange = manager.template getComponent<ExplosionRange>(data);

                    manager.template forEntitiesWith<Position, Destructible>(
                        [&manager, &bombPosition, &bombRange](auto &data, [[gnu::unused]] auto id) {

                            auto &position = manager.template getComponent<Position>(data);

                            for (float i = 0; i < bombRange.explosionRangeUp; i++) {
                                if (static_cast<int>(bombPosition.z + i) == static_cast<int>(position.z)) {
                                    bombRange.explosionRangeUp = i;
                                    break;
                                }
                            }
                            for (float i = 0; i < bombRange.explosionRangeDown; i++) {
                                if (static_cast<int>(bombPosition.z - i) == static_cast<int>(position.z)) {
                                    bombRange.explosionRangeDown = i;
                                    break;
                                }
                            }

                            for (float i = 0; i < bombRange.explosionRangeLeft; i++) {
                                if (static_cast<int>(bombPosition.x + i) == static_cast<int>(position.x)) {
                                    std::cout << "explosion at range " << i << std::endl;
                                    bombRange.explosionRangeLeft = i;
                                    //break;
                                }
                            }

                            for (float i = 0; i < bombRange.explosionRangeRight; i++) {
                                if (static_cast<int>(bombPosition.x - i) == static_cast<int>(position.x)) {
                                    bombRange.explosionRangeRight = i;
                                    break;
                                }
                            }
                        }
                    );
                }
            );
        }

    };
}