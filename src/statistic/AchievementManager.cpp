/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** statistic AchievementManager.cpp
*/

#include "indiestudio/Game.hpp"
#include "indiestudio/statistic/AchievementManager.hpp"

namespace IndieStudio {

    AchievementManager::AchievementManager(Game &game)
        : game(game), logger("achievementmanager")
    {}

    void AchievementManager::registerAchievement(const std::string &id, Achievement achievement)
    {
        this->achievements.insert(std::make_pair(id, achievement));
    }

    void AchievementManager::unlockAchievement(const std::string &id)
    {
        std::string statId = AchievementManager::getStatisticId(id);

        if (!this->hasAchievement(id)) {
            this->game.getStatisticManager().setStatistic(statId, 1);
            // TODO: Trigger achievement rendering?
        }
    }

    bool AchievementManager::hasAchievement(const std::string &id)
    {
        std::string statId = AchievementManager::getStatisticId(id);
        return this->game.getStatisticManager().getStatistic(statId, 0) == 1;
    }

    std::string AchievementManager::getStatisticId(const std::string &id)
    {
        return "achievement_" + id;
    }

}