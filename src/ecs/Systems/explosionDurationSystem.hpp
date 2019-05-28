/*
** EPITECH PROJECT, 2019
** explosionDurationSystem
** File description:
** Systems
*/

#pragma once

#include "../Components.hpp"
#include "../BaseSystem.hpp"

namespace Ecs::System {

    using namespace Ecs::Component;

    template<typename ManagerType>
    struct explosionDuration : public BaseSystem<ManagerType> {

        void process(ManagerType &manager) override {
            manager.template forEntitiesWith<IsBomb, IsExploding, ExplosionLifeTime>(
                [&manager](auto &data, [[gnu::unused]] auto id) {
                    auto &explosionTime = manager.template getComponent<ExplosionLifeTime>(data);

                    if ((std::time(nullptr) - explosionTime.explosionLifeTime) >= 1)
                        manager.template delEntity(data);
            });
        }
    };
}