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

    void TextureManager::init()
    {
        registerTexture("assets/textures/skydome.jpg");
        registerTexture("assets/textures/title.png");
        registerTexture("assets/textures/water_stones.jpg");
        registerTexture("assets/textures/water.jpg");
        registerTexture("assets/textures/particlewhite.bmp");
        registerTexture("assets/textures/jesus.png");

        for (const std::string &color : Constants::PLAYER_COLORS) {
            registerTexture("assets/textures/player_" + color + ".png");
            registerTexture("assets/textures/player_" + color + "_icon.png");
        }

        registerTexture("assets/textures/player_banner.png");
        registerTexture("assets/textures/player_banner_inverse.png");
        registerTexture("assets/textures/timer.png");
        registerTexture("assets/textures/icons/bomb.png");
        registerTexture("assets/textures/icons/bomberman.png");
    }


    void TextureManager::registerTexture(std::string id)
    {
        Texture texture;
        texture.content = Singleton::getDevice()->getVideoDriver()->getTexture(id.c_str());

        if (!texture.content)
            throw RessourceError("Failed to load the texture " + id);

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
            throw RessourceError("Failed to find texture with id " + id);

        return this->registry.get(id);
    }

}