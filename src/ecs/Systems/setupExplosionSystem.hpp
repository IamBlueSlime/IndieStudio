/*
** EPITECH PROJECT, 2019
** setupExplosion
** File description:
** system
*/

#pragma once

#include "../Components.hpp"
#include "../BaseSystem.hpp"

namespace Ecs::System {

    using namespace Ecs::Component;

    template<typename ManagerType>
    struct setupExplosion : public BaseSystem<ManagerType> {

        void process(ManagerType &manager) override {
            manager.template forEntitiesWith<IsBomb, LifeTime>(
                [&manager](auto &data, [[gnu::unused]] auto id) {
                    auto &Lifetime = manager.template getComponent<LifeTime>(data);

                    if (std::time(nullptr) - Lifetime.lifeTime >= 3) {
                        manager.template setComponent(data, IsExploding());
                        manager.template setComponent(data, ExplosionLifeTime());
                    }
            });
        }
    };
}