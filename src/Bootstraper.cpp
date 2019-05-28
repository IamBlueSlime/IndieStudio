/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Bootstraper.cpp
*/

#include "indiestudio/Bootstraper.hpp"
#include "indiestudio/Game.hpp"

namespace IndieStudio {

    int Bootstraper::start(int ac, char **av)
    {
        (void) ac;
        (void) av;
        test();
        //  intro();
        // Game game;
        // game.start();
        return 0;
    }

}