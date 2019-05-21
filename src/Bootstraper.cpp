/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Bootstraper.cpp
*/

#include <iostream>
#include "indiestudio/Bootstraper.hpp"
#include "indiestudio/ecs/Signature.hpp"
#include "indiestudio/mod/ModManager.hpp"
#include "indiestudio/world/World.hpp"
#include "indiestudio/world/WorldManager.hpp"

namespace IndieStudio {

    int Bootstraper::start(int ac, char **av)
    {
        (void) ac;
        (void) av;

        std::cout << std::endl << "== TEST WORLD ==" << std::endl;
        WorldManager worldManager;
        World w = World();
        w.getSettings().name = "my_world";
        worldManager.save("my_world.bmm", w);
        World w2 = worldManager.load("my_world.bmm");
        std::cout << w2.getSettings().name << std::endl;

        std::cout << std::endl << "== TEST MOD ==" << std::endl;
        ModManager modManager;
        modManager.reload("mods");

        test();

        return 0;
    }

}