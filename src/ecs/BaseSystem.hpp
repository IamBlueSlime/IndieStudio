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
#include "./Components.hpp"
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

        template<typename ManagerType>
        struct System1 : public BaseSystem<ManagerType> {

            void process(ManagerType &manager) override {
                manager.template forEntitiesWith<Component1>(
                    [&manager](auto &data, [[gnu::unused]] auto id) {
                        auto &comp1 = manager.template getComponent<Component1>(data);

                        std::cout << comp1.x << "x " << comp1.y << "y" << std::endl;
                    }
                );
            }

        };

        template<typename ManagerType>
        struct System2 : public BaseSystem<ManagerType> {

            void process(ManagerType &manager) override {
                manager.template forEntitiesWith<Component1, Component3>(
                    [&manager](auto &data, [[gnu::unused]] auto id) {
                        auto &comp1 = manager.template getComponent<Component1>(data);

                        std::cout << comp1.x << "x " << comp1.y << "y" << std::endl;
                    }
                );
            }

        };

        template<typename ManagerType>
        struct System3 : public BaseSystem<ManagerType> {

            void process(ManagerType &manager) override {
                manager.template forEntitiesWith<Component1, Component2, Component3>(
                    [&manager](auto &data, [[gnu::unused]] auto id) {
                        auto &comp1 = manager.template getComponent<Component1>(data);
                        auto &comp2 = manager.template getComponent<Component2>(data);

                        std::cout << comp1.x << "x " << comp1.y << "y" << std::endl;
                        std::cout << comp2.str << std::endl;
                    }
                );
            }

        };

        struct ISyst {
            virtual ~ISyst() = default;
            virtual void process() = 0;
        };

        template<typename... SystemTypes>
        struct Systems : public ISyst {
            virtual void process() override {}
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
