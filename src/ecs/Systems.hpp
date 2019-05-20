/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** System
*/

#ifndef SYSTEMS_HPP_
#define SYSTEMS_HPP_

#include "./Components.hpp"
#include "./BaseSystem.hpp"

namespace Ecs {

    template<typename... ComponentTypes>
    class Manager;

    namespace System {

        using namespace Component;

        template<typename... ComponentTypes>
        using ISystem = BaseSystem<ComponentTypes...>

        struct System1 : public ISystem {
            void process() override {
                this->manager.forEntitiesWith<Component1, Component3>(
                [](auto &entity, auto id) {
                    std::cout << "System1 found " << id << std::endl;
                });
            }
        };

        struct System2 : public ISystem {
            void process() override {
                this->manager.forEntitiesWith<Component1, Component2, Component3>(
                [](auto &entity, auto id) {
                    std::cout << "System2 found " << id << std::endl;
                });
            }

        };

        struct System3 : public ISystem {
            void process() override {
                this->manager.forEntitiesWith<Component1>(
                [](auto &entity, auto id) {
                    std::cout << "System3 found " << id << std::endl;
                });
            }
        };
    }
}

#endif /* !SYSTEMS_HPP_ */

