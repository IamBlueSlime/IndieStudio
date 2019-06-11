/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** wiimotecontroller WiimoteControlProvider.hpp
*/

#pragma once

#include <deque>
#include <unordered_map>
#include <cwiid.h>
#include "indiestudio/IGame.hpp"
#include "indiestudio/common/ILogger.hpp"
#include "indiestudio/player/IControlProvider.hpp"

namespace IndieStudio::WiimoteController {

    class WiimoteController;

    class WiimoteControlProvider : public IControlProvider {
    public:
        WiimoteControlProvider(WiimoteController &mod, IGame *game);
        ~WiimoteControlProvider();

        void initConfigurationArea(irr::gui::IGUIEnvironment *guiEnv,
			irr::gui::IGUIElement *area, int playerIdx) override;
		bool onConfigurationEvent(const irr::SEvent &event,
			irr::gui::IGUIElement *area, int playerIdx) override;

		Mappings getPlayerMappings(int playerIdx) const override;
        std::deque<ECS::Event::EventData> pollEvents() override;

		const std::string &getIconPath() const override { return ICON_PATH; };

        void pack(ByteBuffer &buffer, int playerIdx) const override
        {
            (void) buffer;
            (void) playerIdx;
        }

        void unpack(ByteBuffer &buffer, int playerIdx) override;

    protected:
    private:
        static const std::string ICON_PATH;
        static std::unordered_map<cwiid_wiimote_t *, int> WIIMOTE_TO_IDX;
        static std::unordered_map<cwiid_wiimote_t *, uint16_t> LAST_WIIMOTE_DATA;
        static std::deque<ECS::Event::EventData> EVENTS_TO_PUSH;
        static IGame *GAME;

        WiimoteController &mod;
        cwiid_wiimote_t *wiimotes[4];

        void initializeWiimote(int idx);
        
        static void onWiimoteMessage(cwiid_wiimote_t *wiimote, int msgsCount,
            union cwiid_mesg msgs[], struct timespec *timestamp);
};

}