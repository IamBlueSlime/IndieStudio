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
    struct MovePlayer : public BaseSystem<ManagerType> {

        void process(ManagerType &manager) override {
            manager.template forEntitiesWith<Position, Speed, Movable>(
                [&manager](auto &data, [[gnu::unused]] auto id) {
                    auto &pos = manager.template getComponent<Position>(data);
                    auto &speed = manager.template getComponent<Speed>(data);
                    pos.x += speed.x;
                    pos.y += speed.y;
                    pos.z += speed.z;       
            });
        }
    };
}