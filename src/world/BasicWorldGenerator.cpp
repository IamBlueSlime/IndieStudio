/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world BasicWorldGenerator.cpp
*/

#include <ctime>
#include "indiestudio/world/BasicWorldGenerator.hpp"

namespace IndieStudio {

    void BasicWorldGenerator::generate(MapPattern &pattern)
    {
        std::srand(std::time(NULL));

        // We place a wall each two tiles, one line of two.
        for (int y = 2; y < pattern.getHeight() - 2; y += 2)
            for (int x = 2; x < pattern.getWidth() - 2; x += 2)
                pattern.set(y, x, MapPattern::INNER_WALL_TILE);
        
        // We remove some breakable blocks.
        for (int y = 1; y < pattern.getHeight() - 1; y += 1)
            for (int x = 1; x < pattern.getWidth() - 1; x += 1)
                if (pattern.get(y, x) == MapPattern::BREAKABLE_BLOCK_TILE
                && std::rand() % HOLE_RAND_MAX < HOLE_RATE)
                    pattern.set(y, x, MapPattern::EMPTY_TILE);
    
        // Because we fill the empty space randomly, we ensure that
        // the spawn areas are usable.
        pattern.set(1, 1, MapPattern::EMPTY_TILE);
        pattern.set(1, 2, MapPattern::EMPTY_TILE);
        pattern.set(2, 1, MapPattern::EMPTY_TILE);

        pattern.set(1, pattern.getWidth() - 2, MapPattern::EMPTY_TILE);
        pattern.set(1, pattern.getWidth() - 3, MapPattern::EMPTY_TILE);
        pattern.set(2, pattern.getWidth() - 2, MapPattern::EMPTY_TILE);

        pattern.set(pattern.getHeight() - 2, 1, MapPattern::EMPTY_TILE);
        pattern.set(pattern.getHeight() - 2, 2, MapPattern::EMPTY_TILE);
        pattern.set(pattern.getHeight() - 3, 1, MapPattern::EMPTY_TILE);

        pattern.set(pattern.getHeight() - 2, pattern.getWidth() - 2, MapPattern::EMPTY_TILE);
        pattern.set(pattern.getHeight() - 2, pattern.getWidth() - 3, MapPattern::EMPTY_TILE);
        pattern.set(pattern.getHeight() - 3, pattern.getWidth() - 2, MapPattern::EMPTY_TILE);
    }

}