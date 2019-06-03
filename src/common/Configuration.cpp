/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common Configuration.cpp
*/

#include <fstream>
#include <sstream>
#include "indiestudio/common/Configuration.hpp"

static const std::string DEFAULT = "___INTERNAL_DEFAULTVALUE___";

namespace IndieStudio {

    Configuration::Configuration(const std::string &filename)
        : filename(filename)
    {}

    Configuration::~Configuration()
    {
        if (this->dirty)
            this->save();
    }

    void Configuration::load()
    {
        std::ifstream stream(this->filename);

        if (!stream.good())
            return;
        std::string currentSection = "";
        std::string line;
        while (std::getline(stream, line)) {
            lskip(rstrip(line));
            if (line[0] == ';' || line[0] == '#') {
                continue;
            } else if (line[0] == '[') {
                size_t end = line.find("]");
                if (end == line.npos)
                    throw std::runtime_error("Bad formatted section");
                currentSection = line.substr(1, end - 1);
                continue;
            } else if (line[0]) {
                size_t equal = line.find("=");
                if (equal == 0)
                    throw std::runtime_error("Bad formatted line");
                std::string key = line.substr(0, equal);
                lskip(rstrip(key));
                std::string value = "";
                if (equal != line.npos)
                    value = line.substr(equal + 1, line.size());
                value = lskip(rstrip(value));
                this->set(currentSection, key, value);
            }
        }
        stream.close();
        this->dirty = false;
    }

    void Configuration::save()
    {
        if (!this->dirty)
            return;
        std::ofstream stream(this->filename);
        if (!stream.good())
            throw std::runtime_error("Failed to open the configuration file");
        for (auto sectionIt = this->sections.begin(); sectionIt != this->sections.end(); ++sectionIt) {
            std::map<std::string, std::string> section = sectionIt->second;
            if (sectionIt != this->sections.begin())
                stream << std::endl;
            stream << "[" + sectionIt->first + "]" << std::endl;
            for (auto entryIt = section.begin(); entryIt != section.end(); ++entryIt)
                stream << entryIt->first + "=" + entryIt->second << std::endl;
        }
    }
    
    void Configuration::clear()
    {
        sections.clear();
    }

    void Configuration::set(const std::string &section, const std::string &name,
        const std::string &value)
    {
        auto sectionIt = this->sections.find(section);

        if (sectionIt != this->sections.end()) {
            std::map<std::string, std::string> &section = sectionIt->second;
            if (section.find(name) != section.end())
                section.erase(name);
            section.insert(std::make_pair(name, value));
        } else {
            std::map<std::string, std::string> newSection;
            newSection.insert(std::make_pair(name, value));
            this->sections.insert(std::make_pair(section, newSection));
        }
        this->dirty = true;
    }

    void Configuration::setInteger(const std::string &section, const std::string &name,
        int value)
    {
        std::stringstream stream;
        stream << value;
        this->set(section, name, stream.str());
    }

    void Configuration::setBoolean(const std::string &section, const std::string &name,
        bool value)
    {
        std::stringstream stream;
        stream << value;
        this->set(section, name, stream.str());
    }

    std::string Configuration::get(const std::string &section, const std::string &name,
        const std::string &default_value) const
    {
        auto sectionIt = this->sections.find(section);

        if (sectionIt == this->sections.end())
            return default_value;
        auto valueIt = sectionIt->second.find(name);
        if (valueIt == sectionIt->second.end())
            return default_value;
        return valueIt->second;
    }

    int Configuration::getInteger(const std::string &section, const std::string &name,
        int default_value) const
    {
        std::string value = this->get(section, name, DEFAULT);
        if (value == DEFAULT)
            return default_value;
        return std::stoi(value);
    }

    bool Configuration::getBoolean(const std::string &section, const std::string &name,
        bool default_value) const
    {
        std::string value = this->get(section, name, DEFAULT);
        if (value == DEFAULT)
            return default_value;
        if (value == "true" || value == "yes" || value == "on" || value == "1")
            return true;
        else if (value == "false" || value == "no" || value == "off" || value == "0")
            return false;
        return default_value;
    }

    const Configuration::Container &Configuration::getSections() const
    {
        return this->sections;
    }

    bool Configuration::has(const std::string &section, const std::string &name) const
    {
        auto sectionIt = this->sections.find(section);

        if (sectionIt == this->sections.end())
            return false;
        auto valueIt = sectionIt->second.find(name);
        if (valueIt == sectionIt->second.end())
            return false;
        return true;
    }

    std::string &Configuration::lskip(std::string &str)
    {
        int i = 0;

        for (auto it = str.begin(); it != str.end(); ++it) {
            if (!isspace(*it))
                break;
            i += 1;
        }
        str = str.erase(0, i);
        return str;
    }

    std::string &Configuration::rstrip(std::string &str)
    {
        int i = 0;
        for (auto it = str.end() - 1; it >= str.begin(); --it) {
            if (!isspace(*it))
                break;
            i += 1;
        }
        str = str.erase(str.size() - i, str.size());
        return str;
    }

}