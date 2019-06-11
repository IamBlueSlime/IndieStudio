/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** MazeWorldGenerator MazeWorldGenerator.hpp
*/

#pragma once

#include "indiestudio/mod/AMod.hpp"
#include "indiestudio/world/IWorldGenerator.hpp"

namespace IndieStudio::MazeWorldGenerator {

    class MazeWorldGenerator : public AMod, public IWorldGenerator {
    public:
        MazeWorldGenerator();

        void onEnable(IGame &game) override;
        void onDisable(IGame &game) override;

        void generate(MapPattern *pattern) override;

    protected:
    private:
        static const int HOLE_RAND_MAX = 1000;
        static const int HOLE_RATE = 150;
    };

}
