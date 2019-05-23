/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** statistic IAchievementManager.hpp
*/

#pragma once

#include <string>

namespace IndieStudio {

    class IAchievementManager {
	public:
        struct Achievement {
            std::string name;
            std::string description;
        };

		virtual ~IAchievementManager() = default;

        virtual void registerAchievement(const std::string &id, Achievement achievement) = 0;
        virtual void unlockAchievement(const std::string &id) = 0;
        virtual bool hasAchievement(const std::string &id) = 0;
    };

}