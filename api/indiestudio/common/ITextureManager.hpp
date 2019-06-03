/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common ITextureManager.hpp
*/

#pragma once

#include <string>
#include <irrlicht.h>

namespace IndieStudio {

    class ITextureManager {
	public:
        struct Texture {
            irr::video::ITexture *content;
        };

		virtual ~ITextureManager() = default;
        virtual void registerTexture(std::string id) = 0;
        virtual void registerTexture(std::string id, Texture &Texture,
            bool overrideExisting = false) = 0;
    };

}
