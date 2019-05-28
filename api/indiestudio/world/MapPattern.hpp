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
        static const char EMPTY_TILE = ' ';

		MapPattern(int width, int height) : width(width), height(height)
        {
            this->pattern = std::make_unique<unsigned char []>(width * height);
            
            for (int y = 0; y < this->height; y += 1)
                for (int x = 0; x < this->width; x += 1)
                    this->set(y, x, BREAKABLE_BLOCK_TILE);
            
            // This code generate the horizontal walls
            for (int x = 0; x < this->width; x += 1) {
                this->set(0, x, BORDER_WALL_TILE);
                this->set(height - 1, x, BORDER_WALL_TILE);
            }

            // This code generate the vertical walls
            for (int y = 0; y < this->height; y += 1) {
                this->set(y, 0, BORDER_WALL_TILE);
                this->set(y, width - 1, BORDER_WALL_TILE);
            }
        }

        void forEach(std::function<void(int, int, unsigned char)> callback) const
        {
            for (int y = 0; y < this->height; y += 1)
                for (int x = 0; x < this->width; x += 1)
                    callback(y, x, this->get(y, x));
        }

        void set(int y, int x, unsigned char value)
        {
            this->pattern.get()[y * this->width + x] = value;
        }

        unsigned char get(int y, int x) const
        {
            return this->pattern.get()[y * this->width + x];
        }

        int getWidth() const { return this->width; }
        int getHeight() const { return this->height; }

	protected:
	private:
        const int width;
        const int height;
        std::unique_ptr<unsigned char []> pattern;
    };

    std::ostream &operator <<(std::ostream &stream, const MapPattern &pattern);

}
