/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common Configuration.hpp
*/

#pragma once

#include <string>
#include <map>
#include <vector>

namespace IndieStudio {

    class Configuration
    {
    public:
        Configuration(const std::string &filename);
        ~Configuration();

        using Container = std::map<std::string, std::map<std::string, std::string>>;

        void load();
        void save();

        void clear();

        void set(const std::string &section, const std::string &name,
            const std::string &value);
        void setInteger(const std::string &section, const std::string &name,
            int value);
        void setBoolean(const std::string &section, const std::string &name,
            bool value);
        std::string get(const std::string &section, const std::string &name,
            const std::string &default_value) const;
        int getInteger(const std::string &section, const std::string &name,
            int default_value) const;
        bool getBoolean(const std::string &section, const std::string &name,
            bool default_value) const;
        const Container &getSections() const;
        bool has(const std::string &section, const std::string &name) const;

    protected:
    private:
        const std::string filename;
        Container sections;
        bool dirty = false;

        static std::string &lskip(std::string &str);
        static std::string &rstrip(std::string &str);
    };

}
