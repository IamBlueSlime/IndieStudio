/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** player ControlProviderManager.hpp
*/

#pragma once

#include <memory>
#include "indiestudio/common/Logger.hpp"
#include "indiestudio/common/Registry.hpp"
#include "indiestudio/player/AIControlProvider.hpp"
#include "indiestudio/player/IControlProviderManager.hpp"
#include "indiestudio/player/KeyboardControlProvider.hpp"

namespace IndieStudio {

    class ControlProviderManager : public IControlProviderManager {
    public:
        ControlProviderManager();

        void init();
        void registerControlProvider(std::string id,
            IControlProvider *controlProvider) override;
        IControlProvider *getControlProvider(const std::string &id) override;
        Registry<std::string, IControlProvider *> &getControlProviders() { return this->registry; }

    protected:
    private:
        Logger logger;
        Registry<std::string, IControlProvider *> registry;

        std::unique_ptr<KeyboardControlProvider> keyboardControlProvider;
        std::unique_ptr<AIControlProvider> aiControlProvider;
    };

}