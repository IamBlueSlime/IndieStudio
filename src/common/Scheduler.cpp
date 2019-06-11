/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common Scheduler.cpp
*/

#include <iostream>
#include "indiestudio/common/Scheduler.hpp"

namespace IndieStudio {

    std::size_t Scheduler::ID_POOL = 0;
    std::vector<Scheduler::Task> Scheduler::tasks = std::vector<Scheduler::Task>();
    std::deque<Scheduler::Task> Scheduler::waitingPush = std::deque<Scheduler::Task>();

    void Scheduler::tick()
    {
        auto now = std::chrono::system_clock::now();
        time_t nowTime = std::chrono::system_clock::to_time_t(now);

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
            schedule(task);

        while (!waitingPush.empty()) {
            tasks.push_back(waitingPush.front());
            waitingPush.pop_front();
        }
    }

    std::size_t Scheduler::schedule(unsigned long millis, std::function<bool()> &&callback)
    {
        return schedule(millis, callback);
    }

    std::size_t Scheduler::schedule(unsigned long millis, std::function<bool()> &callback)
    {
        Task task = { ID_POOL++, millis, std::move(callback), 0 };
        return schedule(task);
    }

    std::size_t Scheduler::schedule(Task &task)
    {
        auto delta = std::chrono::system_clock::now() + (std::chrono::milliseconds(task.millis));
        task.expirationTime = std::chrono::system_clock::to_time_t(delta);
        waitingPush.push_back(std::move(task));

        return task.id;
    }

    void Scheduler::stopTask(std::size_t id)
    {
        tick();

        for (auto it = tasks.begin(); it != tasks.end();) {
            if (it->id == id)
                it = tasks.erase(it);
            else
                ++it;
        }
    }

}