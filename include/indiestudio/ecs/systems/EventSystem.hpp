/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems EventSystem.hpp
*/

#pragma once

#include "indiestudio/ecs/BaseSystem.hpp"

namespace IndieStudio::ECS::System {

    template<typename ManagerType>
    class EventSystem : public BaseSystem<ManagerType> {
    public:
        void process(ManagerType &manager, World *world) override;

    protected:
    private:
    };

}
