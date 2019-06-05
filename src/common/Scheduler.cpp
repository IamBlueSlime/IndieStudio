/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common Scheduler.cpp
*/

#include "indiestudio/common/Scheduler.hpp"

namespace IndieStudio {

    std::vector<Scheduler::Task> Scheduler::tasks = std::vector<Scheduler::Task>();
    std::deque<Scheduler::Task> Scheduler::waitingPush = std::deque<Scheduler::Task>();

    void Scheduler::tick()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

        std::vector<Task> toSchedule;

        for (auto it = tasks.begin(); it != tasks.end();) {
            if (nowTime >= it->expirationTime) {
                if (it->callback())
                    toSchedule.push_back(*it);
                
                it = tasks.erase(it);
            } else {
                ++it;
            }
        }

        for (Task &task : toSchedule)
            schedule(task.millis, task.callback);

        while (!waitingPush.empty()) {
            tasks.push_back(waitingPush.front());
            waitingPush.pop_front();
        }
    }

    void Scheduler::schedule(unsigned long millis, std::function<bool()> &&callback)
    {
        schedule(millis, callback);
    }

    void Scheduler::schedule(unsigned long millis, std::function<bool()> &callback)
    {
        auto delta = std::chrono::system_clock::now() + (std::chrono::milliseconds(millis));
        Task task = { 0, millis, std::move(callback) };
        task.expirationTime = std::chrono::system_clock::to_time_t(delta);

        waitingPush.push_back(task);
    }

}