/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** IControlProvider.hpp
*/

#pragma once

#include <string>
#include "irrlicht.h"

namespace IndieStudio {

	class IControlProvider {
	public:
		struct Mappings {
			irr::EKEY_CODE keyUp;
			irr::EKEY_CODE keyLeft;
			irr::EKEY_CODE keyDown;
			irr::EKEY_CODE keyRight;
			irr::EKEY_CODE keyDrop;
		};

		virtual ~IControlProvider() = default;
		virtual void initConfigurationArea(irr::gui::IGUIEnvironment *guiEnv,
			irr::gui::IGUIElement *area, int playerIdx) = 0;
		virtual bool onConfigurationEvent(const irr::SEvent &event,
			irr::gui::IGUIElement *area, int playerIdx) = 0;
		virtual Mappings getPlayerMappings(int playerIdx) const = 0;
		virtual const std::string &getIconPath() const = 0;
	};

}
