/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world WorldGenerator.hpp
*/

#pragma once

#include "indiestudio/world/MapPattern.hpp"

namespace IndieStudio {

    class IWorldGenerator {
	public:
		virtual ~IWorldGenerator() = default;
		virtual void generate(MapPattern *pattern) = 0;
    };

}