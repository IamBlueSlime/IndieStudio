/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems SetupExplosionSystem.hpp
*/

#include "indiestudio/ecs/Components.hpp"
#include "indiestudio/ecs/systems/SetupExplosionSystem.hpp"
#include "indiestudio/world/World.hpp"
#include "indiestudio/Game.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    void SetupExplosionSystem<ManagerType>::process(ManagerType &manager, World *world)
    {
        (void) world;

        manager.template forEntitiesWith<Node, IsBomb, LifeTime, PosedBy>(
            [&manager](auto &data, [[gnu::unused]] auto id) {
                auto &Lifetime = manager.template getComponent<LifeTime>(data);
                auto &node = manager.template getComponent<Node>(data);
                auto &playerID = manager.template getComponent<PosedBy>(data);
                auto &stat = manager.template getComponent<Stat>(playerID.id);

                if (std::time(nullptr) - Lifetime.lifeTime >= 3) {
                    sf::Sound soundplayer;
                    manager.setComponent(data, IsExploding());
                    manager.setComponent(data, ExplosionLifeTime());
                    soundplayer.setBuffer(Game::INSTANCE->getSoundManager().getSound("assets/sounds/bomb.wav").buffer);
                    soundplayer.play();
                    manager.template unsetComponent<LifeTime>(data);
                    node.node->setVisible(false);
                    stat.bomb++;
                }
        });
    }

    template class SetupExplosionSystem<WorldECS>;

}