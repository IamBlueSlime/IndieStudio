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
#include "indiestudio/common/Error.hpp"

namespace IndieStudio::WiimoteController {

    const std::string WiimoteControlProvider::ICON_PATH =
        "mods/WiimoteController/assets/textures/icons/wiimote.png";

    std::unordered_map<cwiid_wiimote_t *, int> WiimoteControlProvider::WIIMOTE_TO_IDX;
    std::unordered_map<cwiid_wiimote_t *, uint16_t> WiimoteControlProvider::LAST_WIIMOTE_DATA;
    std::deque<ECS::Event::EventData> WiimoteControlProvider::EVENTS_TO_PUSH;
    IGame *WiimoteControlProvider::GAME = nullptr;

    WiimoteControlProvider::WiimoteControlProvider(WiimoteController
        &mod, IGame *game) : mod(mod)
    {
        GAME = game;
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
        guiEnv->addButton(irr::core::recti(
            {0, 0}, {225, 30}
        ), area, 4343 + playerIdx, L"Connect Wiimote");
    }

    bool WiimoteControlProvider::onConfigurationEvent(const irr::SEvent &event,
        irr::gui::IGUIElement *area, int playerIdx)
    {
        if (event.EventType == irr::EEVENT_TYPE::EET_GUI_EVENT
        && event.GUIEvent.EventType == irr::gui::EGUI_EVENT_TYPE::EGET_BUTTON_CLICKED
        && event.GUIEvent.Caller->getID() - playerIdx == 4343) {
            this->initializeWiimote(playerIdx);
            area->getElementFromId(4343 + playerIdx)->setEnabled(false);
            return true;
        }

        return false;
    }

    IControlProvider::Mappings WiimoteControlProvider::getPlayerMappings(int playerIdx) const
    {
        ECS::Event::EventData eventData;
        eventData.type = ECS::Event::EventType::INDIE_KEYBOARD_EVENT;

        IControlProvider::Mappings mappings;

        eventData.keyInput.Key = static_cast<irr::EKEY_CODE>(4343 + (playerIdx * 10));
        mappings.up = eventData;
        eventData.keyInput.Key = static_cast<irr::EKEY_CODE>(4343 + (playerIdx * 10) + 1);
        mappings.left = eventData;
        eventData.keyInput.Key = static_cast<irr::EKEY_CODE>(4343 + (playerIdx * 10) + 2);
        mappings.down = eventData;
        eventData.keyInput.Key = static_cast<irr::EKEY_CODE>(4343 + (playerIdx * 10) + 3);
        mappings.right = eventData;
        eventData.keyInput.Key = static_cast<irr::EKEY_CODE>(4343 + (playerIdx * 10) + 4);
        mappings.drop = eventData;

        return mappings;
    }

    std::deque<ECS::Event::EventData> WiimoteControlProvider::pollEvents()
    {
        std::deque<ECS::Event::EventData> cpy = EVENTS_TO_PUSH;
        EVENTS_TO_PUSH.clear();
        return cpy;
    }

    void WiimoteControlProvider::initializeWiimote(int idx)
    {
        uint8_t ledBindings[4] = {
            CWIID_LED1_ON, CWIID_LED2_ON, CWIID_LED3_ON, CWIID_LED4_ON
        };

        bdaddr_t wiimoteAddr;
        bdaddr_t broadcastAddr = {{0, 0, 0, 0, 0, 0}};

        std::stringstream ss;
        ss << "Player " << idx + 1 << ", put your Wiimote in discoverable mode now (press 1+2)...";
        this->mod.getLogger()->warning(ss.str());

        bacpy(&wiimoteAddr, &broadcastAddr);

        if (!bacmp(&wiimoteAddr, &broadcastAddr))
            if (cwiid_find_wiimote(&wiimoteAddr, -1))
                throw ProviderError("Failed to find the Wiimote");

        cwiid_set_err([](wiimote *wiimote, const char *str, va_list ap) {
            (void) wiimote;
            (void) str;
            (void) ap;
        });

        while (this->wiimotes[idx] == nullptr)
            this->wiimotes[idx] = cwiid_open(&wiimoteAddr, CWIID_FLAG_MESG_IFC);

        cwiid_set_err(cwiid_err_default);

        this->mod.getLogger()->info("Connected.");
        WIIMOTE_TO_IDX.insert(std::make_pair(this->wiimotes[idx], idx));
        cwiid_set_mesg_callback(this->wiimotes[idx], onWiimoteMessage);
        cwiid_set_rpt_mode(this->wiimotes[idx], CWIID_RPT_BTN);
        cwiid_set_rumble(this->wiimotes[idx], 1);
        usleep(1000 * 250);
        cwiid_set_rumble(this->wiimotes[idx], 0);

        cwiid_set_led(this->wiimotes[idx], ledBindings[idx]);
    }

    void WiimoteControlProvider::onWiimoteMessage(cwiid_wiimote_t *wiimote, int msgsCount,
            union cwiid_mesg msgs[], struct timespec *timestamp)
    {
        (void) timestamp;

        IWorld *world = GAME->getWorldManager().getLoadedWorld();

        if (world == nullptr)
            return;

        ECS::Event::EventData eventData;
        eventData.type = ECS::Event::EventType::INDIE_KEYBOARD_EVENT;

        for (int i = 0; i < msgsCount; i += 1) {
            if (msgs[i].type == CWIID_MESG_ERROR) {
                // TODO: Log/Abort?
                return;
            } else if (msgs[i].type != CWIID_MESG_BTN) {
                continue;
            }

            uint16_t pressed = msgs[i].btn_mesg.buttons & ~LAST_WIIMOTE_DATA[wiimote];
            uint16_t released = ~msgs[i].btn_mesg.buttons & LAST_WIIMOTE_DATA[wiimote];
            LAST_WIIMOTE_DATA[wiimote] = msgs[i].btn_mesg.buttons;

            int playerIdx = WIIMOTE_TO_IDX[wiimote];
            long uniqueTag = -1;
            bool press = true;

            if (pressed & CWIID_BTN_RIGHT) {
                uniqueTag = 4343 + (playerIdx * 10);
            } else if (pressed & CWIID_BTN_UP) {
                uniqueTag = 4343 + (playerIdx * 10) + 1;
            } else if (pressed & CWIID_BTN_LEFT) {
                uniqueTag = 4343 + (playerIdx * 10) + 2;
            } else if (pressed & CWIID_BTN_DOWN) {
                uniqueTag = 4343 + (playerIdx * 10) + 3;
            } else if (pressed & CWIID_BTN_1) {
                uniqueTag = 4343 + (playerIdx * 10) + 4;
            } else if (released & CWIID_BTN_RIGHT) {
                uniqueTag = 4343 + (playerIdx * 10);
                press = false;
            } else if (released & CWIID_BTN_UP) {
                uniqueTag = 4343 + (playerIdx * 10) + 1;
                press = false;
            } else if (released & CWIID_BTN_LEFT) {
                uniqueTag = 4343 + (playerIdx * 10) + 2;
                press = false;
            } else if (released & CWIID_BTN_DOWN) {
                uniqueTag = 4343 + (playerIdx * 10) + 3;
                press = false;
            } else if (released & CWIID_BTN_1) {
                uniqueTag = 4343 + (playerIdx * 10) + 4;
                press = false;
            }

            if (uniqueTag != -1) {
                eventData.keyInput.Key = static_cast<irr::EKEY_CODE>(uniqueTag);
                eventData.keyInput.PressedDown = press;
                EVENTS_TO_PUSH.push_back(eventData);
            }
        }
    }

    void WiimoteControlProvider::unpack(ByteBuffer &buffer, int playerIdx)
    {
        (void) buffer;
        this->initializeWiimote(playerIdx);
    }

}