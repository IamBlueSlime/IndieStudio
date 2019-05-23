/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Main.cpp
*/

#include "ecs/Ecs.hpp"
#include "indiestudio/Bootstraper.hpp"

#include <thread>


using namespace Ecs::Component;

// Components registers in the manager
using EcsManager = Ecs::Manager<
Component1,
Component2,
Component3,
EventCallbacks
>;

using namespace Ecs::System;

// Systems used to run the manager
using EcsSystems = Systems<
// System1<EcsManager>,
// System2<EcsManager>,
// System3<EcsManager>,
EventSystem<EcsManager>
>;

int main(int ac, char **av)
{
    // Creating a new manager
    auto manager = EcsManager();

    // Creating entity
    auto &entity1 = manager.addEntity();
    // Donc forget to get the REFERENCE, so you can modify the entity with it

    // Create component
    Component1 comp1 = {10, 12};

    // Set this component on the entity
    manager.setComponent(entity1, comp1);
    // The component type is automatically deduced from the parameter

    // Add more entity / components
    auto &entity2 = manager.addEntity();
    manager.setComponent(entity2, Component1());
    manager.setComponent(entity2, Component2("Bonjour"));
    auto &entity3 = manager.addEntity();
    manager.setComponent(entity3, Component1());
    manager.setComponent(entity3, Component2());
    manager.setComponent(entity3, Component3());

    // Get un component
    std::cout << manager.getComponent<Component2>(entity3).str << std::endl;
    // Get will raise an error if the component is missing, so should be called only from systems

    // Get return a reference, so you can modify the component
    manager.getComponent<Component2>(entity3).str = "I'm a reference!";
    std::cout << manager.getComponent<Component2>(entity3).str << std::endl;


    auto &test_entity = manager.addEntity();
    EventCallbacks event_listener;

    EventData event_left;
    event_left.type = EventType::KEYBOARD_EVENT;
    event_left.keyInput.Key = irr::KEY_LEFT;

    EventData event_right;
    event_right.type = EventType::KEYBOARD_EVENT;
    event_right.keyInput.Key = irr::KEY_RIGHT;

    event_listener.addCallback(event_left, [](const EventData &event) {
        std::cout << "left pressed" << std::endl;
    });

    event_listener.addCallback(event_right, [](const EventData &event) {
        std::cout << "right pressed" << std::endl;
    });

    manager.setComponent(test_entity, event_listener);


    // Run le manager avec les systems spécifiés
    manager.run(EcsSystems());

    return IndieStudio::Bootstraper::start(ac, av);
}