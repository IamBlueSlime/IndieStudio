/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** WinLooseSystem
*/

#pragma once

#include "indiestudio/ecs/BaseSystem.hpp"

namespace IndieStudio::ECS::System {

    template<typename ManagerType>
    class WinLooseSystem {
    public:
        void process(ManagerType &manager, World *world);
    protected:
    private:
    };
}