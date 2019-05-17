/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Main.cpp
*/

#include "ecs/Ecs.hpp"
#include "indiestudio/Bootstraper.hpp"

struct Component1;
struct Component2;
struct Component3;

int main(int ac, char **av)
{
    auto manager = Ecs::Manager<Component1, Component2, Component3>();
    manager.forEntitiesWith<Component1, Component3>(
        [](auto &data, auto id) {
            std::cout << "He! :D I found " << id << std::endl;
        });
    return IndieStudio::Bootstraper::start(ac, av);
}