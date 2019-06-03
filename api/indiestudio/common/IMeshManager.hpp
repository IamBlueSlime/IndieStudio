/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common IMeshManager.hpp
*/

#pragma once

#include <string>
#include <irrlicht/irrlicht.h>

namespace IndieStudio {

    class IMeshManager {
	public:
        struct Mesh {
            irr::scene::IAnimatedMesh *content;
        };

		virtual ~IMeshManager() = default;
        virtual void registerMesh(std::string id) = 0;
        virtual void registerMesh(std::string id, Mesh &Mesh,
            bool overrideExisting = false) = 0;
    };

}
