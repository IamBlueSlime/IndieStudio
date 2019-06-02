/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world IWorldManager.hpp
*/

#pragma once

#include <string>
#include "indiestudio/world/IWorldGenerator.hpp"

namespace IndieStudio {

    class IWorldManager {
	public:
		virtual ~IWorldManager() = default;
        virtual void registerGenerator(const std::string &name,
            IWorldGenerator *generator) = 0;
    };

}
