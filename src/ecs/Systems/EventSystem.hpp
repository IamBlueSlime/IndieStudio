/*
** EPITECH PROJECT, 2019
** EventSystem
** File description:
** Systems
*/

#pragma once

#include "../Events.hpp"

namespace Ecs::System {

    using namespace Ecs::Event;

    //TODO: thread safety omg
    template<typename ManagerType>
    struct EventSystem : public BaseSystem<ManagerType> {

        void process(ManagerType &manager) override {
            manager.template forEntitiesWith<EventCallbacks<ManagerType>>(
                [&manager](auto &data, auto id) {
                    auto &event_manager = manager.getEventManager();
                    const auto &callbacks = manager.template getComponent<EventCallbacks<ManagerType>>(data).getCallbacks();

                    for (const auto &event : event_manager.getEventQueue()) {
                        const auto &callback = callbacks.find(event.first);
                        if (callback != callbacks.end()) {
                            auto funcs = callback->second;
                            for (const auto &func : funcs) {
                                func(event.first, id, manager);
                            }
                        }
                    }
                }
            );
        }

    };
}
