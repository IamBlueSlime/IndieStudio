/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common ILogger.hpp
*/

#pragma once

#include <string>

namespace IndieStudio {

    class ILogger {
	public:
		virtual ~ILogger() {};

        enum LogLevel {
            DEBUG, INFO, WARNING, ERROR, CRITICAL
        };

        virtual void log(LogLevel targetLevel, const std::string &message) = 0;
        void debug(const std::string &message) { log(DEBUG, message); }
        void info(const std::string &message) { log(INFO, message); }
        void warning(const std::string &message) { log(WARNING, message); }
        void error(const std::string &message) { log(ERROR, message); }
        void critical(const std::string &message) { log(CRITICAL, message); }

        virtual void setLevel(LogLevel level) = 0;

	protected:
	private:
    };

}
