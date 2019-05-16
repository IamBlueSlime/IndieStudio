/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common Logger.hpp
*/

#pragma once

#include <string>

namespace IndieStudio {

    class Logger {
    public:
        Logger(const std::string &name);

        enum LogLevel {
            DEBUG, INFO, WARNING, ERROR, CRITICAL
        };

        void log(LogLevel targetLevel, const std::string &message);
        void debug(const std::string &message) { log(DEBUG, message); }
        void info(const std::string &message) { log(INFO, message); }
        void warning(const std::string &message) { log(WARNING, message); }
        void error(const std::string &message) { log(ERROR, message); }
        void critical(const std::string &message) { log(CRITICAL, message); }

        void setLevel(LogLevel level);

    protected:
    private:
        static std::size_t maxLength_;
        const std::string name_;
        LogLevel level_ = INFO;

        static std::string getFormattedLevel(LogLevel level);
    };

}
