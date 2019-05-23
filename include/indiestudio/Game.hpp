/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Game.hpp
*/

#pragma once

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