/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world EmptyWorldGenerator.hpp
*/

#pragma once

#include "indiestudio/world/IWorldGenerator.hpp"

namespace IndieStudio {

    class EmptyWorldGenerator : public IWorldGenerator {
	public:
        void generate(MapPattern *pattern) override;

	protected:
	private:
    };

}