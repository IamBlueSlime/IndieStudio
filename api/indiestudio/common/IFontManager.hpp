/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common IFontManager.hpp
*/

#pragma once

#include <string>
#include <irrlicht.h>

namespace IndieStudio {

    class IFontManager {
	public:
        struct Font {
            irr::gui::IGUIFont *content;
        };

		virtual ~IFontManager() = default;
        virtual void registerFont(std::string id) = 0;
        virtual void registerFont(std::string id, Font &font,
            bool overrideExisting = false) = 0;
    };

}
