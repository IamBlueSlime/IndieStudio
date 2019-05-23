/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** statistic AchievementManager.hpp
*/

#pragma once

#include <string>
#include <unordered_map>
#include "indiestudio/common/Logger.hpp"
#include "indiestudio/statistic/IAchievementManager.hpp"

namespace IndieStudio {

    class Game;

    class AchievementManager : public IAchievementManager {
    public:
        AchievementManager(Game &game);

        void registerAchievement(const std::string &id, Achievement achievement) override;
        void unlockAchievement(const std::string &id) override;
        bool hasAchievement(const std::string &id) override;

    protected:
    private:
        Game &game;
        Logger logger;
        std::unordered_map<std::string, Achievement> achievements;

        static std::string getStatisticId(const std::string &id);
    };
}
