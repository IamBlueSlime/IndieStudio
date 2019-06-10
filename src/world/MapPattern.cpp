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

        this->forEach([&](unsigned short x, unsigned short y, unsigned short z, TileType tileType) {
            (void) x;
            (void) y;
            (void) z;

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
        for (short x = 0; x < this->width; x += 1) {
            for (short z = 0; z < this->height; z += 1) {
                this->set(x, 0, z, TileType::EMPTY);
                this->set(x, 1, z, TileType::EMPTY);
            }
        }

        unsigned int tileCount;
        buffer >> tileCount;

        for (unsigned int i = 0; i < tileCount; i += 1) {
            short x;
            short y;
            short z;
            TileType tileType;

            buffer >> x;
            buffer >> y;
            buffer >> z;
            buffer >> tileType;
            this->set(x, y, z, tileType);
        }
    }
}
