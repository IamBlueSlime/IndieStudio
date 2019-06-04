/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems SetupExplosionSystem.hpp
*/

#pragma once

#include "indiestudio/ecs/Components.hpp"
#include "indiestudio/ecs/BaseSystem.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    class SetupExplosion : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager, World *world) override {
            (void) world;

            std::cout << "BOMB ?" << std::endl;
            manager.template forEntitiesWith<IsBomb, LifeTime>(
                [&manager](auto &data, [[gnu::unused]] auto id) {
                    auto &Lifetime = manager.template getComponent<LifeTime>(data);

                    if (std::time(nullptr) - Lifetime.lifeTime >= 3) {
                        std::cout << "Explose !" << std::endl;
                        manager.template setComponent(data, IsExploding());
                        manager.template setComponent(data, ExplosionLifeTime());
                    }
            });
        }
    protected:
    private:
    };
}