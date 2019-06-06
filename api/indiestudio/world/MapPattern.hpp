/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world MapPattern.hpp
*/

#pragma once

#include <functional>
#include <memory>
#include <utility>
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
            FLOOR_SECOND
        };

        MapPattern(unsigned short width, unsigned short height) : width(width), height(height)
        {
            this->ground = std::make_unique<TileType[]>(width * height);
            this->walls = std::make_unique<TileType[]>(width * height);

            for (unsigned short x = 1; x < this->width - 1; x += 1)
                for (unsigned short z = 1; z < this->height - 1; z += 1)
                    this->set(x, 0, z, TileType::FLOOR_FIRST);

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

        std::pair<short, short> positionstoTile(short x, short z) {
            std::pair<short, short> values;
            short factor = 20;

            if (x % factor > factor / 2) {
                values.first = x + factor - (x % factor);
            } else {
                values.first = x - (x % factor);
            }
            if (z % factor > factor / 2) {
                values.second = z + factor - (z % factor);
            } else {
                values.second = z - (z % factor);
            }
            values.first /= 20;
            values.second /= 20;
            return values;
        }

        unsigned short getWidth() const { return this->width; }
        unsigned short getHeight() const { return this->height; }

        /* ISerializable implementation */
        void pack(ByteBuffer &buffer) const override;
        void unpack(ByteBuffer &buffer) override;

    protected:
    private:
        const unsigned short width;
        const unsigned short height;
        std::unique_ptr<TileType[]> ground;
        std::unique_ptr<TileType[]> walls;
    };

}
