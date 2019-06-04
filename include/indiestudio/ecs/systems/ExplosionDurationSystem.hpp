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
        void process(ManagerType &manager, World *world) override {
            (void) world;

            std::cout << "BOMB1 ?" << std::endl;
            manager.template forEntitiesWith<IsBomb, IsExploding, ExplosionLifeTime>(
                [&manager](auto &data, [[gnu::unused]] auto id) {
                    auto &explosionTime = manager.template getComponent<ExplosionLifeTime>(data);

                    if ((std::time(nullptr) - explosionTime.explosionLifeTime) >= 1)
                        manager.delEntity(data);
            });
        }
    protected:
    private:
    };
}