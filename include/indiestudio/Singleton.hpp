/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** Singleton.hpp
*/

#pragma once

#include <irrlicht.h>

namespace IndieStudio {

    class Singleton {
	public:
		static irr::IrrlichtDevice *getDevice(
            irr::video::E_DRIVER_TYPE type = irr::video::EDT_OPENGL,
            int width = 1280, int height = 720)
        {
            static irr::IrrlichtDevice *device = nullptr;

            if (!device) {
                device = irr::createDevice(type, irr::core::dimension2d<irr::u32>(width, height), 16, false, true);
                device->setWindowCaption(L"Indie Studio");
                device->getLogger()->setLogLevel(irr::ELL_NONE);
            }
            return device;
        }
	protected:
	private:
    };

}
