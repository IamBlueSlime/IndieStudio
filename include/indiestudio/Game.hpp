/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Game.hpp
*/

#pragma once

#include <irrlicht.h>
#include "indiestudio/IGame.hpp"
#include "indiestudio/common/Logger.hpp"
#include "indiestudio/common/SoundManager.hpp"
#include "indiestudio/common/FontManager.hpp"
#include "indiestudio/mod/ModManager.hpp"
#include "indiestudio/scene/SceneManager.hpp"
#include "indiestudio/statistic/AchievementManager.hpp"
#include "indiestudio/statistic/StatisticManager.hpp"
#include "indiestudio/world/WorldManager.hpp"

namespace IndieStudio {

    class Game : public IGame {
	public:
        static Game *INSTANCE;

		Game();

        void start();

        IStatisticManager &getStatisticManager() override { return this->statisticManager; }
        IAchievementManager &getAchievementManager() override { return this->achievementManager; }
        ISoundManager &getSoundManager() override { return this->soundManager; }
        IFontManager &getFontManager() override { return this->fontManager; }
        IWorldManager &getWorldManager() override { return this->worldManager; }
        ModManager &getModManager() { return this->modManager; }
        SceneManager &getSceneManager() { return this->sceneManager; }

	protected:
	private:
        Logger logger;
        StatisticManager statisticManager;
        AchievementManager achievementManager;
        SoundManager soundManager;
        FontManager fontManager;
        WorldManager worldManager;
        ModManager modManager;
        SceneManager sceneManager;
    };

}
