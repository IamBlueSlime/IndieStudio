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
#include "./Events.hpp"

namespace Ecs::System {

    using namespace Ecs::Component;
    using namespace Ecs::Event;

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

    template<typename ManagerType>
    struct EventSystem : public BaseSystem<ManagerType> {

        void process(ManagerType &manager) override {
            manager.template forEntitiesWith<EventCallbacks>(
                [&manager](auto &data, [[gnu::unused]] auto id) {
                    auto &event_manager = manager.getEventManager();
                    const auto &callbacks = manager.template getComponent<EventCallbacks>(data).getCallbacks();

                    for (const auto &event : event_manager.getEventQueue()) {
                        const auto &callback = callbacks.find(event);
                        if (callback != callbacks.end()) {
                            auto funcs = callback->second;
                            for (const auto &func : funcs) {
                                func(event);
                            }
                        }
                    }
                }
            );
        }

    };


}

#endif /* !SYSTEMS_HPP */