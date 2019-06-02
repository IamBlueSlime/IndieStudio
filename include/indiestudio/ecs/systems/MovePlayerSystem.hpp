/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems MovePlayerSystem.hpp
*/

#pragma once

#include "indiestudio/ecs/Components.hpp"
#include "indiestudio/ecs/BaseSystem.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    class MovePlayer : public BaseSystem<ManagerType> {
    public:
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
    
    protected:
    private:
    };
}