/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world IWorld.hpp
*/

#pragma once

#include <irrlicht.h>
#include "indiestudio/world/MapPattern.hpp"

namespace IndieStudio {

    class IWorld {
    public:
        virtual ~IWorld() = default;
        virtual MapPattern *getPattern() = 0;
        virtual std::size_t getBlockEntityIdByPos(short x, short z) = 0;
        virtual irr::scene::IMetaTriangleSelector *getMeta() = 0;
    };

}