/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Main.cpp
*/

#include "ecs/Ecs.hpp"
#include "indiestudio/Bootstraper.hpp"

//using Components = Ecs::TypeList<int, float, std::string>;

int main(int ac, char **av)
{
    auto manager = Ecs::Manager<int, float, std::string>();
    manager.forEntitiesWith<int, std::string, bool>(
        [](auto &data, auto id) {
            std::cout << "He! :D" << std::endl;
        });
    return IndieStudio::Bootstraper::start(ac, av);
}