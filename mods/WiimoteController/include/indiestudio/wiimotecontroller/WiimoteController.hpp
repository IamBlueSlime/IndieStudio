/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** WiimoteController WiimoteController.hpp
*/

#pragma once

#include <cwiid.h>
#include "indiestudio/mod/AMod.hpp"

namespace IndieStudio::WiimoteController {

    class WiimoteController : public AMod {
    public:
        WiimoteController();

        void onEnable(IGame &game) override;
        void onDisable(IGame &game) override;

        void onWorldUpdate(IGame &game, WorldSettings &settings);

    protected:
    private:
        static const WorldSettings::Player::ControlType WIIMOTE_CONTROL_TYPE;
        cwiid_wiimote_t *wiimotes[4];

        void initializeWiimote(int idx);
    };

}
