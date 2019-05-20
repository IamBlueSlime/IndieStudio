/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Main.cpp
*/

#include "ecs/Ecs.hpp"
#include "indiestudio/Bootstraper.hpp"

// Pour "include" les Composants (ne pas avoir à spécifier Ecs::Component à chaque fois
using namespace Ecs::Component;

// Components registers in the manager
using EcsManager = Ecs::Manager<
Component1,
Component2,
Component3
>;

int main(int ac, char **av)
{
    // Creating a new manager
    auto manager = EcsManager();

    // Creating entity
    auto &entity = manager.addEntity();
    // Donc forget to get the REFERENCE, so you can modify the entity with it

    // Create component
    Component1 comp1 = {10, 12};

    // Set this component on the entity
    manager.setComponent(entity, comp1);
    // The component type is automatically deduced from the parameter

    // Add more entity / components
    entity = manager.addEntity();
    manager.setComponent(entity, Component1());
    manager.setComponent(entity, Component2("Bonjour"));
    entity = manager.addEntity();
    manager.setComponent(entity, Component1());
    manager.setComponent(entity, Component2());
    manager.setComponent(entity, Component3());

    // Get un component
    std::cout << manager.getComponent<Component2>(entity).str << std::endl;
    // Get will raise an error if the component is missing, so should be called only from systems

    // Get return a reference, so you can modify the component
    manager.getComponent<Component2>(entity).str = "I'm a reference!";
    std::cout << manager.getComponent<Component2>(entity).str << std::endl;

    // Iterate only on the entities with a Component1 and a Component3
    manager.forEntitiesWith<Component1, Component3>(
        [](auto &data, auto id) {
            std::cout << "He! :D I found " << id << std::endl;
        });
    return IndieStudio::Bootstraper::start(ac, av);
}