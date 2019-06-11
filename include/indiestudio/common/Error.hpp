/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Error
*/

#pragma once

#include <exception>
#include <string>

class IndieError : public std::exception
{
    public:
        IndieError(std::string const &message, std::string const &component = "Unknown")
            : std::exception(),  _message(message), _component(component) {};

        const char *what() const noexcept override { return _message.c_str(); };
        const std::string &getComponent() const { return _component; };
    protected:
        std::string _message;
        std::string _component;
};

class RessourceError : public IndieError
{
    public:
        RessourceError(std::string const &message, std::string const &component = "RessourceError")
            : IndieError(message, component) {};
};

class SaveError : public IndieError
{
    public:
        SaveError(std::string const &message, std::string const &component = "SaveError")
            : IndieError(message, component) {};
};

class ProviderError : public IndieError
{
    public:
        ProviderError(std::string const &message, std::string const &component = "ProviderError")
            : IndieError(message, component) {};
};

class RegistryError : public IndieError
{
    public:
        RegistryError(std::string const &message, std::string const &component = "RegistryError")
            : IndieError(message, component) {};
};