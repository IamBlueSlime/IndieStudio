/*
** EPITECH PROJECT, 2019
** CCP_plazza_2018
** File description:
** Logger.cpp
*/

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <ctime>
#include "indiestudio/common/Logger.hpp"

namespace IndieStudio {
    
    std::size_t Logger::maxLength_ = 0;

    Logger::Logger(const std::string &name) : name_(name)
    {
        std::string envKey = "LOGGER_" + name;

        std::transform(
            envKey.begin(), envKey.end(), envKey.begin(), [](unsigned char c) {
                return toupper(c);
            }
        );

        if (name.size() > maxLength_)
            maxLength_ = name.size() + 3;

        const char *envValue = std::getenv(envKey.c_str());

        if (envValue == nullptr)
            envValue = std::getenv("LOGGER_ALL");

        if (envValue != nullptr) {
            std::string envValueStr(envValue);

            if (envValueStr == "DEBUG")
                setLevel(DEBUG);
            else if (envValueStr == "INFO")
                setLevel(INFO);
            else if (envValueStr == "WARNING")
                setLevel(WARNING);
            else if (envValueStr == "ERROR")
                setLevel(ERROR);
            else if (envValueStr == "CRITICAL")
                setLevel(CRITICAL);
        }
    }

    void Logger::log(LogLevel targetLevel, const std::string &message)
    {
        if (targetLevel < level_)
            return;
        
        std::time_t currentTime = std::time(nullptr);
        char buffer[100];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S",
            std::localtime(&currentTime));

        std::cerr <<
            "[" << buffer << "] " <<
            std::right << std::setw(maxLength_) << std::setfill(' ') << "[" + name_ + "] " <<
            std::left << std::setw(24) << std::setfill(' ') <<
            getFormattedLevel(targetLevel) << ": " <<
            message <<
        std::endl;
        std::cerr.flush();
    }

    void Logger::setLevel(LogLevel level)
    {
        level_ = level;
    }

    std::string Logger::getFormattedLevel(LogLevel level)
    {
        if (level == DEBUG)
            return "\e[34mdebug\e[39m";
        else if (level == INFO)
            return "\e[32minfo\e[39m";
        else if (level == WARNING)
            return "\e[93mwarning\e[39m";
        else if (level == ERROR)
            return "\e[31merror\e[39m";
        else if (level == CRITICAL)
            return "\e[91mcritical\e[39m";

        return "<unknown>";
    }

}
