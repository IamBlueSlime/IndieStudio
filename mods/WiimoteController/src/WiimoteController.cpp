/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** WiimoteController WiimoteController.cpp
*/

#include <sstream>
#include "indiestudio/wiimotecontroller/WiimoteController.hpp"

namespace IndieStudio::WiimoteController {

    WiimoteController::WiimoteController()
    {}

    void WiimoteController::onEnable(IGame &game)
    {
        this->wiimoteControlProvider = std::make_unique<WiimoteControlProvider>(*this);
        game.getControlProviderManager().registerControlProvider("Wiimote",
            this->wiimoteControlProvider.get());
    }

    void WiimoteController::onDisable(IGame &game)
    {
        (void) game;
    }

}

EXPORT_MOD(
    "wiimotecontroller",
    "WiimoteController",
    "Jérémy Levilain <jeremy.levilain@epitech.eu>",
    "1.0.0",
    IndieStudio::WiimoteController::WiimoteController
)
