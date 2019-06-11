/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** player ControlProviderManager.cpp
*/

#include "indiestudio/Game.hpp"
#include "indiestudio/player/ControlProviderManager.hpp"

namespace IndieStudio {

    ControlProviderManager::ControlProviderManager() : logger("controlprovidermanager")
    {}

    void ControlProviderManager::init()
    {
        this->registerControlProvider("Keyboard", &this->keyboardControlProvider);
        this->registerControlProvider("AI", &this->aiControlProvider);
    }

    void ControlProviderManager::registerControlProvider(std::string id,
        IControlProvider *controlProvider)
    {
        this->registry.add(id, controlProvider);
        this->logger.debug("Registered control provider '" + id + "'.");

        std::cout << "textureManager" << Game::INSTANCE->getSceneManager().textureManager << std::endl;
        Game::INSTANCE->getSceneManager().textureManager->registerTexture(
            controlProvider->getIconPath());
    }

    IControlProvider *ControlProviderManager::getControlProvider(const std::string &id)
    {
        if (!this->registry.has(id))
            throw ProviderError("Failed to find control provider with id " + id);

        return this->registry.get(id);
    }

}