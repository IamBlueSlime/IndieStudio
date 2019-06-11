/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** DeathSystem
*/

#pragma once

#include "indiestudio/ecs/BaseSystem.hpp"

namespace IndieStudio::ECS::System {

    template<typename ManagerType>
    class DeathSystem : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager, World *world) override;

    protected:
    private:
    };
}