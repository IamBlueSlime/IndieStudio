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
        IndieStudio::Logger log("IndieStudio");
        Game game;
        Game::INSTANCE = &game;
        try {
            game.start();
        } catch (const IndieError &e) {
            log.error(e.getComponent() + ": " + e.what());
            return 84;
        }
        return 0;
    }

}