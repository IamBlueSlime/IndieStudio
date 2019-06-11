/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world MapPattern.hpp
*/

#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <utility>
#include "indiestudio/Constants.hpp"
#include "indiestudio/common/ISerializable.hpp"

namespace IndieStudio {

    class MapPattern : public ISerializable {
	public:
        enum class TileType {
            // On the player level
            EMPTY,
            PLAYER,
            BOMB,
            BOMB_EXPLOSION,
            POWER_UP,
            BREAKABLE_BLOCK,
            BORDER_WALL_BLOCK,
            INNER_WALL_BLOCK,

            // On the floor
            FLOOR_FIRST,
            FLOOR_SECOND,

            // Only in the AI
            SOLID_POTENTIAL_EXPLOSION,
            POTENTIAL_EXPLOSION
        };

        MapPattern(unsigned short width, unsigned short height) : width(width), height(height)
        {
            this->ground = std::make_unique<TileType[]>(width * height);
            this->walls = std::make_unique<TileType[]>(width * height);

            for (unsigned short x = 1; x < this->width - 1; x += 1)
                for (unsigned short z = 1; z < this->height - 1; z += 1)
                    this->set(x, 0, z, TileType::FLOOR_FIRST);
            
            // This code generate the alternate floor texture
            for (unsigned short x = 2; x < this->width - 2; x += 1) {
                this->set(x, 0, 2, TileType::FLOOR_SECOND);
                this->set(x, 0, this->height - 3, TileType::FLOOR_SECOND);
            }
            for (unsigned short x = 2; x < (this->width / 2 - 1); x += 2) {
                if (x % 4 != 0) {
                    for (unsigned short z = 2; z < this->height - 2; z += 1) {
                        this->set(x, 0, z, TileType::FLOOR_SECOND);
                        this->set(x + 1, 0, z, TileType::FLOOR_SECOND);

                        int oppositeX = this->width - x - 2;
                        this->set(oppositeX, 0, z, TileType::FLOOR_SECOND);
                        this->set(oppositeX + 1, 0, z, TileType::FLOOR_SECOND);
                    }
                }
            }

            for (unsigned short x = 1; x < this->width - 1; x += 1)
                for (unsigned short z = 1; z < this->height - 1; z += 1)
                    this->set(x, 1, z, TileType::BREAKABLE_BLOCK);

            // This code generate the horizontal walls
            for (unsigned short x = 0; x < this->width; x += 1) {
                this->set(x, 1, 0, TileType::BORDER_WALL_BLOCK);
                this->set(x, 1, height - 1, TileType::BORDER_WALL_BLOCK);
            }

            // This code generate the vertical walls
            for (unsigned short z = 0; z < this->height; z += 1) {
                this->set(0, 1, z, TileType::BORDER_WALL_BLOCK);
                this->set(width - 1, 1, z, TileType::BORDER_WALL_BLOCK);
            }
        }

        void forEach(std::function<void(unsigned short, unsigned short, unsigned short, TileType)> callback) const
        {
            for (unsigned short x = 0; x < this->width; x += 1) {
                for (unsigned short z = 0; z < this->height; z += 1) {
                    callback(x, 0, z, this->get(x, 0, z));
                    callback(x, 1, z, this->get(x, 1, z));
                }
            }
        }

        void set(unsigned short x, unsigned short y, unsigned short z, TileType value)
        {
            TileType *ptr = (y == 0 ? this->ground.get() : this->walls.get());
            ptr[z * this->width + x] = value;
        }

        TileType get(unsigned short x, unsigned short y, unsigned short z) const
        {
            TileType *ptr = (y == 0 ? this->ground.get() : this->walls.get());
            return ptr[z * this->width + x];
        }

        bool isWalkable(unsigned short x, unsigned short z) const
        {
            TileType tileType = this->get(x, 1, z);

            return tileType == TileType::EMPTY || tileType == TileType::BOMB_EXPLOSION
                || tileType == TileType::PLAYER || tileType == TileType::POWER_UP;
        }

        static std::pair<short, short> positionToTile(short x, short z) {
            std::pair<short, short> values;
            int factor = Constants::TILE_SIZE_FACTOR;

            if (x % factor > factor / 2)
                values.first = x + factor - (x % factor);
            else
                values.first = x - (x % factor);

            if (z % factor > factor / 2)
                values.second = z + factor - (z % factor);
            else
                values.second = z - (z % factor);
            values.first /= factor;
            values.second /= factor;
            return values;
        }

        unsigned short getWidth() const { return this->width; }
        unsigned short getHeight() const { return this->height; }

        /* ISerializable implementation */
        void pack(ByteBuffer &buffer) override;
        void unpack(ByteBuffer &buffer) override;

    protected:
    private:
        const unsigned short width;
        const unsigned short height;
        std::unique_ptr<TileType[]> ground;
        std::unique_ptr<TileType[]> walls;
    };
}
