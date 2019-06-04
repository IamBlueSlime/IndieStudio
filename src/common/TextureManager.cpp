/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common TextureManager.cpp
*/

#include "indiestudio/common/TextureManager.hpp"
#include "indiestudio/Game.hpp"
#include "indiestudio/Singleton.hpp"

namespace IndieStudio {

    TextureManager::TextureManager() : logger("texturemanager")
    {}

    void TextureManager::registerTexture(std::string id)
    {
        Texture texture;
        texture.content = Singleton::getDevice()->getVideoDriver()->getTexture(id.c_str());

        if (!texture.content)
            throw std::runtime_error("Failed to load the texture " + id);

        this->registerTexture(id, texture);
    }

    void TextureManager::registerTexture(std::string id, Texture &texture,
        bool overrideExisting)
    {
        this->registry.add(id, std::move(texture), overrideExisting);
        this->logger.debug("Registered texture '" + id + "'.");
    }

    TextureManager::Texture &TextureManager::getTexture(const std::string &id)
    {
        if (!this->registry.has(id))
            throw std::runtime_error("Failed to find texture with id " + id);

        return this->registry.get(id);
    }

}