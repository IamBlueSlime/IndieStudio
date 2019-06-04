/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs BaseSystem.hpp
*/

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <iostream>
#include <variant>

#include "indiestudio/ecs/TypeList.hpp"
#include "indiestudio/ecs/Entity.hpp"

namespace IndieStudio::ECS {

    template<typename... ComponentTypes>
    class Manager;

    class World;

    namespace System {

        template<typename ManagerType>
        struct BaseSystem {
            BaseSystem() {}
            virtual ~BaseSystem() = default;

            virtual void process([[gnu::unused]] ManagerType &manager) {}
        };

        struct ISyst {
            virtual ~ISyst() = default;
            virtual void process() = 0;
        };

        template<typename... SystemTypes>
        struct Systems : public ISyst {
            void process() override {}
        };

        template<typename ManagerType, typename... SystemTypes>
        struct SystemsImpl : public Systems<SystemTypes...> {

            SystemsImpl(ManagerType &manager, World *world)
                : manager(manager), world(world)
            {}

            void process() final {
                TypeList<SystemTypes...>::forEach([this](auto ref, [[gnu::unused]] std::size_t idx) {
                    std::get<typeof(ref)>(this->systems).process(this->manager, world);
                });
            }

            ManagerType &manager;
            World *world;
            std::tuple<SystemTypes...> systems;

        };
    }
}
