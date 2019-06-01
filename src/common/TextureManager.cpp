/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common TextureManager.cpp
*/

#include "indiestudio/common/TextureManager.hpp"
#include "indiestudio/Game.hpp"

namespace IndieStudio {

    TextureManager::TextureManager() : logger("Texturemanager")
    {}

    void TextureManager::registerTexture(std::string id)
    {
        Texture Texture;
        Texture.content = Game::getDevice()->getVideoDriver()->getTexture(id.c_str());

        if (!Texture.content)
            throw std::runtime_error("Failed to load the Texture " + id);

        this->registerTexture(id, Texture);
    }

    void TextureManager::registerTexture(std::string id, Texture &Texture,
        bool overrideExisting)
    {
        this->registry.add(id, std::move(Texture), overrideExisting);
        this->logger.info("Registered Texture '" + id + "'.");
    }

    TextureManager::Texture &TextureManager::getTexture(const std::string &id)
    {
        if (!this->registry.has(id))
            throw std::runtime_error("Failed to find Texture with id " + id);

        return this->registry.get(id);
    }

}