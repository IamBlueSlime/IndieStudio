/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** statistic IStatisticManager.hpp
*/

#pragma once

#include <string>

namespace IndieStudio {

    class IStatisticManager {
	public:
		virtual ~IStatisticManager() = default;

        virtual int setStatistic(const std::string &id, int value) = 0;
        virtual int incrementStatistic(const std::string &id, int by) = 0;
        virtual int getStatistic(const std::string &id, int defaultValue) = 0;

        int decrementStatistic(const std::string &id, int by)
        {
            return this->incrementStatistic(id, -by);
        }
    };

}