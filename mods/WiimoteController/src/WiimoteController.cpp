/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** WiimoteController WiimoteController.cpp
*/

#include <sstream>
#include "indiestudio/wiimotecontroller/WiimoteController.hpp"

namespace IndieStudio::WiimoteController {

    const WorldSettings::Player::ControlType WiimoteController::WIIMOTE_CONTROL_TYPE =
        static_cast<WorldSettings::Player::ControlType>(42);

    WiimoteController::WiimoteController()
    {
        for (int i = 0; i < 4; i += 1)
            this->wiimotes[i] = nullptr;
    }

    void WiimoteController::onEnable(IGame &game)
    {
        (void) game;
        this->logger->info("Hello!");
    }

    void WiimoteController::onDisable(IGame &game)
    {
        (void) game;
        
        this->logger->info("Disconnecting connected Wiimotes...");
        for (int i = 0; i < 4; i += 1)
            if (this->wiimotes[i] != nullptr)
                cwiid_disconnect(this->wiimotes[i]);
    }

    void WiimoteController::onWorldUpdate(IGame &game, WorldSettings &settings)
    {
        (void) game;

        uint8_t ledBindings[4] = {
            CWIID_LED1_ON, CWIID_LED2_ON, CWIID_LED3_ON, CWIID_LED4_ON
        };

        for (int i = 0; i < 4; i += 1) {
            if (settings.players[i].controlType != WIIMOTE_CONTROL_TYPE)
                continue;
            
            if (this->wiimotes[i] == nullptr)
                initializeWiimote(i);
            
            cwiid_set_led(this->wiimotes[i], ledBindings[i]);
        }
    }

    void WiimoteController::initializeWiimote(int idx)
    {
        bdaddr_t wiimoteAddr;
        bdaddr_t broadcastAddr = {{0, 0, 0, 0, 0, 0}};

        std::stringstream ss;
        ss << "Player " << idx + 1 << ", put your Wiimote in discoverable mode now (press 1+2)...";
        this->logger->warning(ss.str());

        if (!bacmp(&wiimoteAddr, &broadcastAddr))
            if (cwiid_find_wiimote(&wiimoteAddr, -1))
                throw std::runtime_error("Failed to find the Wiimote");

        while (this->wiimotes[idx] == nullptr)
            this->wiimotes[idx] = cwiid_open(&wiimoteAddr, CWIID_FLAG_MESG_IFC);
    }

}

EXPORT_MOD(
    "wiimotecontroller",
    "WiimoteController",
    "Jérémy Levilain <jeremy.levilain@epitech.eu>",
    "1.0.0",
    IndieStudio::WiimoteController::WiimoteController
)
