/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world IWorld.hpp
*/

#pragma once

#include <irrlicht.h>
#include "indiestudio/world/MapPattern.hpp"
#include "indiestudio/scene/SceneManager.hpp"

namespace IndieStudio {

    class IWorld {
    public:
        virtual ~IWorld() = default;

        virtual bool createBlast(const irr::core::vector3df &position, unsigned int timeMs) = 0;
        virtual void dropBomb(float bombPosX, float bombPosZ, std::size_t playerID) = 0;
        virtual void eject(irr::scene::ISceneNode *node, irr::core::vector3df &bombPos) = 0;

        virtual MapPattern *getPattern() = 0;
        virtual std::size_t getBlockEntityIdByPos(short x, short z) = 0;
        virtual irr::scene::IMetaTriangleSelector *getMeta() = 0;
        virtual SceneManager::Scene &getScene() = 0;
    };
}