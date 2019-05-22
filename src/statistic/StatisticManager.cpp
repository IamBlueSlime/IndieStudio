/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** statistic StatisticManager.cpp
*/

#include "indiestudio/statistic/StatisticManager.hpp"

namespace IndieStudio {

    const std::string StatisticManager::FILE_PATH = "statistics.ini";

    StatisticManager::StatisticManager()
        : logger("statisticmanager"), statistics(FILE_PATH)
    {}

    StatisticManager::~StatisticManager()
    {
        this->save();
    }

    void StatisticManager::load()
    {
        this->logger.info("Loading statistics from disk...");
        this->statistics.load();
    }

    void StatisticManager::save()
    {
        this->logger.info("Writing statistics on disk...");
        this->statistics.save();
    }

    int StatisticManager::setStatistic(const std::string &id, int value)
    {
        this->statistics.setInteger(id, "value", value);
        return value;
    }

    int StatisticManager::incrementStatistic(const std::string &id, int by)
    {
        int newValue = by;
        newValue += this->statistics.getInteger(id, "value", 0);

        return this->setStatistic(id, newValue);
    }

    int StatisticManager::getStatistic(const std::string &id, int defaultValue)
    {
        return this->statistics.getInteger(id, "value", defaultValue);
    }

}