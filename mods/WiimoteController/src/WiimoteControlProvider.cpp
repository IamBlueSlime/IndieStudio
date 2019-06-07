/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** wiimotecontroller WiimoteControlProvider.cpp
*/

#include <unistd.h>
#include <sstream>
#include "indiestudio/wiimotecontroller/WiimoteController.hpp"
#include "indiestudio/wiimotecontroller/WiimoteControlProvider.hpp"

namespace IndieStudio::WiimoteController {

    const std::string WiimoteControlProvider::ICON_PATH =
        "mods/WiimoteController/assets/textures/icons/wiimote.png";

    WiimoteControlProvider::WiimoteControlProvider(WiimoteController
        &mod) : mod(mod)
    {
        for (int i = 0; i < 4; i += 1)
            this->wiimotes[i] = nullptr;
    }

    WiimoteControlProvider::~WiimoteControlProvider()
    {
        this->mod.getLogger()->info("Disconnecting connected Wiimotes...");
        for (int i = 0; i < 4; i += 1)
            if (this->wiimotes[i] != nullptr)
                cwiid_disconnect(this->wiimotes[i]);
    }

    void WiimoteControlProvider::initConfigurationArea(irr::gui::IGUIEnvironment *guiEnv,
        irr::gui::IGUIElement *area, int playerIdx)
    {
        irr::gui::IGUIButton *configureButton = guiEnv->addButton(irr::core::recti(
            {0, 0}, {225, 30}
        ), area, 4343 + playerIdx, L"Connect Wiimote");
    }

    bool WiimoteControlProvider::onConfigurationEvent(const irr::SEvent &event,
        irr::gui::IGUIElement *area, int playerIdx)
    {
        if (event.EventType == irr::EEVENT_TYPE::EET_GUI_EVENT
        && event.GUIEvent.EventType == irr::gui::EGUI_EVENT_TYPE::EGET_BUTTON_CLICKED
        && event.GUIEvent.Caller->getID() - playerIdx == 4343) {
            uint8_t ledBindings[4] = {
                CWIID_LED1_ON, CWIID_LED2_ON, CWIID_LED3_ON, CWIID_LED4_ON
            };

            this->initializeWiimote(area, playerIdx);
            cwiid_set_led(this->wiimotes[playerIdx], ledBindings[playerIdx]);   

            return true;
        }

        return false;
    }

    IControlProvider::Mappings WiimoteControlProvider::getPlayerMappings(int playerIdx) const
    {
        return {};
    }

    void WiimoteControlProvider::initializeWiimote(irr::gui::IGUIElement *area,
        int idx)
    {
        bdaddr_t wiimoteAddr;
        bdaddr_t broadcastAddr = {{0, 0, 0, 0, 0, 0}};

        std::cout << this->mod.getLogger() << std::endl;

        std::stringstream ss;
        ss << "Player " << idx + 1 << ", put your Wiimote in discoverable mode now (press 1+2)...";
        this->mod.getLogger()->warning(ss.str());

        bacpy(&wiimoteAddr, &broadcastAddr);

        if (!bacmp(&wiimoteAddr, &broadcastAddr))
            if (cwiid_find_wiimote(&wiimoteAddr, -1))
                throw std::runtime_error("Failed to find the Wiimote");

        cwiid_set_err([](wiimote *wiimote, const char *str, va_list ap) {
            if (errno != EHOSTDOWN) {
                vfprintf(stderr, str, ap);
                fprintf(stderr, "\n");
            }
        });
    
        while (this->wiimotes[idx] == nullptr)
            this->wiimotes[idx] = cwiid_open(&wiimoteAddr, CWIID_FLAG_MESG_IFC);
        cwiid_set_err(cwiid_err_default);

        this->mod.getLogger()->info("Connected.");
        cwiid_set_rumble(this->wiimotes[idx], 1);
        usleep(1000 * 250);
        cwiid_set_rumble(this->wiimotes[idx], 0);

        area->getElementFromId(4343 + idx)->setEnabled(false);
    }

}