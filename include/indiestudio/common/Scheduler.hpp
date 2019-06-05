/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common Scheduler.hpp
*/

#pragma once

#include <chrono>
#include <functional>
#include <vector>
#include <deque>

namespace IndieStudio {

    class Scheduler {
	public:
        static void tick();
		static void schedule(unsigned long millis, std::function<bool()> &&callback);
        static void schedule(unsigned long millis, std::function<bool()> &callback);

	protected:
	private:
        struct Task {
            std::time_t expirationTime;
            unsigned long millis;
            std::function<bool()> callback;
        };

        static std::vector<Task> tasks;
        static std::deque<Task> waitingPush;
    };

}
