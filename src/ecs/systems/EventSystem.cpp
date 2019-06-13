/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** ecs systems ApplyExplosionSystem.hpp
*/

#include "indiestudio/ecs/systems/EventSystem.hpp"
#include "indiestudio/world/World.hpp"

namespace IndieStudio::ECS::System {

    template<typename ManagerType>
    void EventSystem<ManagerType>::process(ManagerType &manager, World *world)
    {
        (void) world;

        manager.template forEntitiesWith<EventCallbacks<ManagerType>>(
            [&manager](auto &data, auto id) {
                auto &event_manager = manager.getEventManager();
                const auto &callbacks = manager.template getComponent<EventCallbacks<ManagerType>>(data).getCallbacks();

                for (const auto &event : event_manager.getEventQueue()) {
                    const auto &callback = callbacks.find(event.second);
                    if (callback != callbacks.end()) {
                        auto &funcs = callback->second;
                        for (const auto &func : funcs) {
                            func(event.first, id, manager);
                        }
                    }
                }
            }
        );
    }

    template class EventSystem<WorldECS>;

}