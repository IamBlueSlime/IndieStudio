/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world VerticalWorldGenerator.hpp
*/

#pragma once

#include "indiestudio/world/IWorldGenerator.hpp"

namespace IndieStudio {

    class VerticalWorldGenerator : public IWorldGenerator {
	public:
        void generate(MapPattern *pattern) override;

	protected:
	private:
        static const int HOLE_RAND_MAX = 1000;
        static const int HOLE_RATE = 150;
    };

}