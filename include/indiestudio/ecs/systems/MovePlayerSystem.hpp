/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems MovePlayerSystem.hpp
*/

#pragma once

#include "indiestudio/ecs/BaseSystem.hpp"

namespace IndieStudio::ECS::System {

    template<typename ManagerType>
    class MovePlayerSystem : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager, World *world) override;

    protected:
    private:
    };

}