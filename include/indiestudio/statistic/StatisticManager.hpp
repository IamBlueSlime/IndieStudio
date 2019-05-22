/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** statistic StatisticManager.hpp
*/

#pragma once

#include <string>
#include <unordered_map>
#include "indiestudio/common/Configuration.hpp"
#include "indiestudio/common/Logger.hpp"
#include "indiestudio/statistic/IStatisticManager.hpp"

namespace IndieStudio {

    class StatisticManager : public IStatisticManager {
	public:
        StatisticManager();
		~StatisticManager();

        void load();
        void save();

        int setStatistic(const std::string &id, int value) override;
        int incrementStatistic(const std::string &id, int by) override;
        int getStatistic(const std::string &id, int defaultValue = 0) override;

	protected:
	private:
        static const std::string FILE_PATH;

        Logger logger;
        Configuration statistics;
    };

}
