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
            manager.template forEntitiesWith<Node, IsBomb, LifeTime, PosedBy>(
                [&manager](auto &data, [[gnu::unused]] auto id) {
                    auto &Lifetime = manager.template getComponent<LifeTime>(data);
                    auto &node = manager.template getComponent<Node>(data);
                    auto &playerID = manager.template getComponent<PosedBy>(data);
                    auto &maxBomb = manager.template getComponent<MaxBomb>(playerID.id);

                    if (std::time(nullptr) - Lifetime.lifeTime >= 3) {
                        manager.setComponent(data, IsExploding());
                        manager.setComponent(data, ExplosionLifeTime());
                        manager.template unsetComponent<LifeTime>(data);
                        node.node->setVisible(false);
                        maxBomb.nb++;
                    }
            });
        }
    protected:
    private:
    };
}