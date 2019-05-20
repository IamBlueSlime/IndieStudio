/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common Logger.hpp
*/

#pragma once

#include "indiestudio/common/ILogger.hpp"

namespace IndieStudio {

    class Logger : public ILogger {
    public:
        Logger(const std::string &name);

        void log(LogLevel targetLevel, const std::string &message) override;
        void setLevel(LogLevel level) override;

    protected:
    private:
        static std::size_t maxLength_;
        const std::string name_;
        LogLevel level_ = INFO;

        static std::string getFormattedLevel(LogLevel level);
    };

}
