/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Game.hpp
*/

#pragma once

#include "indiestudio/common/Logger.hpp"
#include "indiestudio/mod/ModManager.hpp"
#include "indiestudio/world/WorldManager.hpp"

namespace IndieStudio {

    class Game {
	public:
		Game();

        void start();

	protected:
	private:
        Logger logger;
        WorldManager worldManager;
        ModManager modManager;
    };

}