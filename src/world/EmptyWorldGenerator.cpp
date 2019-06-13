/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world EmptyWorldGenerator.cpp
*/

#include <ctime>
#include "indiestudio/world/EmptyWorldGenerator.hpp"

namespace IndieStudio {

    void EmptyWorldGenerator::generate(MapPattern *pattern)
    {
        // We fill the map with only brekable blocks.
        for (int x = 1; x < pattern->getWidth() - 1; x += 1)
            for (int z = 1; z < pattern->getHeight() - 1; z += 1)
                pattern->set(x, 1, z, MapPattern::TileType::BREAKABLE_BLOCK);
    }

}