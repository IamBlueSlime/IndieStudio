/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** MazeWorldGenerator MazeWorldGenerator.cpp
*/

#include <ctime>
#include <deque>
#include "indiestudio/mazeworldgenerator/MazeWorldGenerator.hpp"

namespace IndieStudio::MazeWorldGenerator {

    MazeWorldGenerator::MazeWorldGenerator()
    {}

    void MazeWorldGenerator::onEnable(IGame &game)
    {
        game.getWorldManager().registerGenerator("Maze", this);
    }

    void MazeWorldGenerator::onDisable(IGame &game)
    {
        (void) game;
    }

    void MazeWorldGenerator::generate(MapPattern *pattern)
    {
        std::srand(std::time(NULL));

        std::deque<std::pair<short, short>> points;

        // We place a wall each two tiles, one line of two.
        for (int x = 2; x < pattern->getWidth() - 2; x += 2) {
            for (int z = 2; z < pattern->getHeight() - 2; z += 2) {
                pattern->set(x, 1, z, MapPattern::TileType::INNER_WALL_BLOCK);
                points.push_back(std::make_pair(x, z));
            }
        }

        while (!points.empty()) {
            std::pair<short, short> point = points.front();
            int randDirection = std::rand() % 4;
            std::pair<short, short> computedCoordinates;

            switch (randDirection) {
                case 0:
                    computedCoordinates = std::make_pair(point.first, point.second + 1);
                    break;
                case 1:
                    computedCoordinates = std::make_pair(point.first + 1, point.second);
                    break;
                case 2:
                    computedCoordinates = std::make_pair(point.first, point.second - 1);
                    break;
                case 3:
                    computedCoordinates = std::make_pair(point.first - 1, point.second);
                    break;
            }

            pattern->set(computedCoordinates.first, 1, computedCoordinates.second,
                MapPattern::TileType::INNER_WALL_BLOCK);

            points.pop_front();
        }

        // We remove some breakable blocks.
        for (int x = 1; x < pattern->getWidth() - 1; x += 1)
            for (int z = 1; z < pattern->getHeight() - 1; z += 1)
                if (pattern->get(x, 1, z) == MapPattern::TileType::BREAKABLE_BLOCK
                && std::rand() % HOLE_RAND_MAX < HOLE_RATE)
                    pattern->set(x, 1, z, MapPattern::TileType::EMPTY);
    }

}

EXPORT_MOD(
    "mazeworldgenerator",
    "MazeWorldGenerator",
    "Jérémy Levilain <jeremy.levilain@epitech.eu>",
    "1.0.0",
    IndieStudio::MazeWorldGenerator::MazeWorldGenerator
)
