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
		static std::size_t schedule(unsigned long millis, std::function<bool()> &&callback);
        static std::size_t schedule(unsigned long millis, std::function<bool()> &callback);
        static void stopTask(std::size_t id);

	protected:
	private:
        static std::size_t ID_POOL;

        struct Task {
            std::size_t id;
            unsigned long millis;
            std::function<bool()> callback;

            // Internal usage only
            time_t expirationTime;
        };

        static std::vector<Task> tasks;
        static std::deque<Task> waitingPush;

        static std::size_t schedule(Task &task);
    };

}
