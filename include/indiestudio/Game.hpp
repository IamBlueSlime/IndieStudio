/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Game.hpp
*/

#pragma once

#include <irrlicht/irrlicht.h>
#include "indiestudio/common/Logger.hpp"
#include "indiestudio/mod/ModManager.hpp"
#include "indiestudio/statistic/AchievementManager.hpp"
#include "indiestudio/statistic/StatisticManager.hpp"
#include "indiestudio/world/WorldManager.hpp"

namespace IndieStudio {

    class Game {
	public:
		Game();

        void start();

        static irr::IrrlichtDevice *getDevice(irr::video::E_DRIVER_TYPE type = irr::video::EDT_OPENGL,int width = 1280, int height = 960);
        ModManager &getModManager() { return this->modManager; }
        StatisticManager &getStatisticManager() { return this->statisticManager; }
        AchievementManager &getAchievementManager() { return this->achievementManager; }
        WorldManager &getWorldManager() { return this->worldManager; }

	protected:
	private:
        Logger logger;
        ModManager modManager;
        StatisticManager statisticManager;
        AchievementManager achievementManager;
        WorldManager worldManager;
    };

}