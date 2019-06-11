/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** player ControlProviderManager.cpp
*/

#include "indiestudio/Game.hpp"
#include "indiestudio/player/ControlProviderManager.hpp"

namespace IndieStudio {

    ControlProviderManager::ControlProviderManager() : logger("controlprovidermanager"),
        keyboardControlProvider(std::make_unique<KeyboardControlProvider>()),
        aiControlProvider(std::make_unique<AIControlProvider>())
    {}

    void ControlProviderManager::init()
    {
        this->registerControlProvider("Keyboard", this->keyboardControlProvider.get());
        this->registerControlProvider("AI", this->aiControlProvider.get());
    }

    void ControlProviderManager::registerControlProvider(std::string id,
        IControlProvider *controlProvider)
    {
        this->registry.add(id, controlProvider);
        this->logger.debug("Registered control provider '" + id + "'.");

        Game::INSTANCE->getSceneManager().textureManager.registerTexture(
            controlProvider->getIconPath());
    }

    IControlProvider *ControlProviderManager::getControlProvider(const std::string &id)
    {
        if (!this->registry.has(id))
            throw ProviderError("Failed to find control provider with id " + id);

        return this->registry.get(id);
    }

}