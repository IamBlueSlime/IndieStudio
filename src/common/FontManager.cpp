/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common FontManager.cpp
*/

#include "indiestudio/Singleton.hpp"
#include "indiestudio/common/FontManager.hpp"

namespace IndieStudio {

    FontManager::FontManager() : logger("fontmanager")
    {}

    void FontManager::init()
    {
        this->registerFont("assets/fonts/roboto_regular.png");
        this->registerFont("assets/fonts/roboto_regular_72.png");
    }

    void FontManager::registerFont(std::string id)
    {
        Font font;
        font.content = Singleton::getDevice()->getSceneManager()->
            getGUIEnvironment()->getFont(id.c_str());
        
        if (!font.content)
            throw std::runtime_error("Failed to load the font " + id);
        
        this->registerFont(id, font);
    }

    void FontManager::registerFont(std::string id, Font &font,
        bool overrideExisting)
    {
        this->registry.add(id, std::move(font), overrideExisting);
        this->logger.debug("Registered font '" + id + "'.");
    }

    FontManager::Font &FontManager::getFont(const std::string &id)
    {
        if (!this->registry.has(id))
            throw std::runtime_error("Failed to find font with id " + id);
        
        return this->registry.get(id);
    }

}