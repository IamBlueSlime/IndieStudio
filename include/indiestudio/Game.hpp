/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Game.hpp
*/

#pragma once

#include "indiestudio/IGame.hpp"
#include "indiestudio/common/Logger.hpp"
#include "indiestudio/mod/ModManager.hpp"
#include "indiestudio/statistic/AchievementManager.hpp"
#include "indiestudio/statistic/StatisticManager.hpp"
#include "indiestudio/world/WorldManager.hpp"

namespace IndieStudio {

    class Game : public IGame {
	public:
		Game();

        void start();

        IStatisticManager &getStatisticManager() override { return this->statisticManager; }
        IAchievementManager &getAchievementManager() override { return this->achievementManager; }
        IWorldManager &getWorldManager() override { return this->worldManager; }
        ModManager &getModManager() { return this->modManager; }

	protected:
	private:
        Logger logger;
        StatisticManager statisticManager;
        AchievementManager achievementManager;
        WorldManager worldManager;
        ModManager modManager;
    };

}