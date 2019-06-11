/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems SetupExplosionSystem.hpp
*/

#pragma once

#include "indiestudio/ecs/BaseSystem.hpp"

namespace IndieStudio::ECS::System {

    using namespace ECS::Component;

    template<typename ManagerType>
    class SetupExplosionSystem : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager, World *world) override;

    protected:
    private:
    };
}