/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world IWorld.hpp
*/

#pragma once

#include "indiestudio/world/MapPattern.hpp"

namespace IndieStudio {

    class IWorld {
    public:
        virtual ~IWorld() = default;

        virtual MapPattern *getPattern() = 0;
        virtual void dropBomb(float bombPosX, float bombPosZ, std::size_t playerID, float range) = 0;
    };
}