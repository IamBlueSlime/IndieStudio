/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world MapPattern.hpp
*/

#pragma once

#include <functional>
#include <memory>

namespace IndieStudio {

    class MapPattern {
	public:
        static const char BORDER_WALL_TILE = '#';
        static const char INNER_WALL_TILE = '*';
        static const char BREAKABLE_BLOCK_TILE = 'X';
        static const char GROUND_FIRST_TILE = '1';
        static const char GROUND_SECOND_TILE = '2';
        static const char EMPTY_TILE = '0';

		MapPattern(int width, int height) : width(width), height(height)
        {
            this->ground = std::make_unique<unsigned char []>(width * height);
            this->walls = std::make_unique<unsigned char []>(width * height);

            for (int x = 1; x < this->width - 1; x += 1)
                for (int z = 1; z < this->height - 1; z += 1)
                    this->set(x, 0, z, GROUND_FIRST_TILE);
            
            for (int x = 1; x < this->width - 1; x += 1)
                for (int z = 1; z < this->height - 1; z += 1)
                    this->set(x, 1, z, BREAKABLE_BLOCK_TILE);
            
            // This code generate the horizontal walls
            for (int x = 0; x < this->width; x += 1) {
                this->set(x, 1, 0, BORDER_WALL_TILE);
                this->set(x, 1, height - 1, BORDER_WALL_TILE);
            }

            // This code generate the vertical walls
            for (int z = 0; z < this->height; z += 1) {
                this->set(0, 1, z, BORDER_WALL_TILE);
                this->set(width - 1, 1, z, BORDER_WALL_TILE);
            }
        }

        void forEach(std::function<void(int, int, int, unsigned char)> callback) const
        {
            for (int x = 0; x < this->width; x += 1) {
                for (int z = 0; z < this->height; z += 1) {
                    callback(x, 0, z, this->get(x, 0, z));
                    callback(x, 1, z, this->get(x, 1, z));
                }
            }
        }

        void set(int x, int y, int z, unsigned char value)
        {
            unsigned char *ptr = (y == 0 ? this->ground.get() : this->walls.get());
            ptr[z * this->width + x] = value;
        }

        unsigned char get(int x, int y, int z) const
        {
            unsigned char *ptr = (y == 0 ? this->ground.get() : this->walls.get());
            return ptr[z * this->width + x];
        }

        int getWidth() const { return this->width; }
        int getHeight() const { return this->height; }

	protected:
	private:
        const int width;
        const int height;
        std::unique_ptr<unsigned char []> ground;
        std::unique_ptr<unsigned char []> walls;
    };

}
