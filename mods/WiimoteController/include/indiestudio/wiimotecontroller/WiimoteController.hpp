/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** WiimoteController WiimoteController.hpp
*/

#pragma once

#include "indiestudio/mod/AMod.hpp"
#include "indiestudio/wiimotecontroller/WiimoteControlProvider.hpp"

namespace IndieStudio::WiimoteController {

    class WiimoteController : public AMod {
    public:
        WiimoteController();

        void onEnable(IGame &game) override;
        void onDisable(IGame &game) override;

    protected:
    private:
        std::unique_ptr<WiimoteControlProvider> wiimoteControlProvider;
    };

}
