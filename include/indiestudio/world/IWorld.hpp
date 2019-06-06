/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** IWorld
*/

#pragma once

#include "indiestudio/world/MapPattern.hpp"

namespace IndieStudio {

    class IWorld {
    public:
        virtual MapPattern *getPattern() = 0;
        virtual std::size_t getBlockEntityIdByPos(short x, short z) = 0;
        virtual irr::scene::IMetaTriangleSelector *getMeta() = 0;
   };

}