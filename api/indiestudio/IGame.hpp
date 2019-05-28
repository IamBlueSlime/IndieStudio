/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** IGame.hpp
*/

#pragma once

#include "indiestudio/statistic/IAchievementManager.hpp"
#include "indiestudio/statistic/IStatisticManager.hpp"
#include "indiestudio/world/IWorldManager.hpp"

namespace IndieStudio {

    class IGame {
	public:
		virtual ~IGame() = default;
        virtual IStatisticManager &getStatisticManager() = 0;
        virtual IAchievementManager &getAchievementManager() = 0;
        virtual IWorldManager &getWorldManager() = 0;
    };

}