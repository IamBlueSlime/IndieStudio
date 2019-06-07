/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** player IControlProviderManager.hpp
*/

#pragma once

#include <string>
#include "indiestudio/player/IControlProvider.hpp"

namespace IndieStudio {

    class IControlProviderManager {
	public:
		virtual ~IControlProviderManager() = default;
        virtual void registerControlProvider(std::string id,
            IControlProvider *controlProvider) = 0;
        virtual IControlProvider *getControlProvider(const std::string &id) = 0;
    };

}
