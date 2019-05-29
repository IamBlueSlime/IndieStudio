/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** createNodeSystem
*/

#pragma once

#include "../Components.hpp"
#include "../BaseSystem.hpp"
#include "indiestudio/Game.hpp"

namespace Ecs::System {

    using namespace Ecs::Component;

    template<typename ManagerType>
    struct drawEntities : public BaseSystem<ManagerType> {

        void process(ManagerType &manager) override {
            manager.template forEntitiesWith<NodeCreate, MeshPath>(
            [&manager](auto &data, [[gnu::unused]] auto id) {
                //TODO: create the node and possibly set attributes
            });
        }
    };
}