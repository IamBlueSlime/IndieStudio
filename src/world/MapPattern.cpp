/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world World.cpp
*/

#include "indiestudio/world/MapPattern.hpp"

namespace IndieStudio {

    void MapPattern::pack(ByteBuffer &buffer)
    {
        unsigned int tileCount = 0;

        this->forEach([&](auto, auto, auto, TileType tileType) {
            if (tileType == TileType::EMPTY)
                return;
            
            tileCount += 1;
        });

        buffer << tileCount;

        this->forEach([&](unsigned short x, unsigned short y, unsigned short z, TileType tileType) {
            if (tileType == TileType::EMPTY)
                return;
            
            buffer << x;
            buffer << y;
            buffer << z;
            buffer << tileType;
        });
    }

    void MapPattern::unpack(ByteBuffer &buffer)
    {
        unsigned int tileCount;
        buffer >> tileCount;

        for (unsigned int i = 0; i < tileCount; i += 1) {
            unsigned short x;
            unsigned short y;
            unsigned short z;
            TileType tileType;

            buffer >> x;
            buffer >> y;
            buffer >> z;
            buffer >> tileType;
            this->set(x, y, z, tileType);
        }
    }
}
