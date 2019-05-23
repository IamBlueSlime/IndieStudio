/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** BaseSystem
*/

#ifndef BASESYSTEM_HPP_
#define BASESYSTEM_HPP_

#include <functional>
#include <map>
#include <memory>
#include <iostream>
#include <variant>

#include "./TypeList.hpp"
#include "./Entity.hpp"

namespace Ecs {

    template<typename... ComponentTypes>
    class Manager;

    namespace System {

        using namespace Ecs::Component;

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

            SystemsImpl(ManagerType &manager_)
            :
            manager(manager_) {
                TypeList<SystemTypes...>::forEach([this](auto system, std::size_t idx) {
                    this->systems[idx] = system;
                });
            }

            void process() final {
                for (auto system : this->systems) {
                    // TODO:    Getting type without the loop
                    TypeList<SystemTypes...>::forEach([this, system](auto ref, [[gnu::unused]] std::size_t idx) {
                        try {
                            auto sys = std::get<typeof(ref)>(system.second);
                            sys.process(this->manager);
                        } catch (...) {}
                    });
                }
            }

            ManagerType &manager;
            std::map<std::size_t, std::variant<SystemTypes...>> systems;

        };
    }
}

#endif /* !BASESYSTEM_HPP_ */
