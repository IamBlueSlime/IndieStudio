/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** wiimotecontroller WiimoteControlProvider.hpp
*/

#pragma once

#include <cwiid.h>
#include "indiestudio/common/ILogger.hpp"
#include "indiestudio/player/IControlProvider.hpp"

namespace IndieStudio::WiimoteController {

    class WiimoteController;

    class WiimoteControlProvider : public IControlProvider {
    public:
        WiimoteControlProvider(WiimoteController &mod);
        ~WiimoteControlProvider();

        void initConfigurationArea(irr::gui::IGUIEnvironment *guiEnv,
			irr::gui::IGUIElement *area, int playerIdx) override;
		bool onConfigurationEvent(const irr::SEvent &event,
			irr::gui::IGUIElement *area, int playerIdx) override;
		Mappings getPlayerMappings(int playerIdx) const override;
		const std::string &getIconPath() const override { return ICON_PATH; };

    protected:
    private:
        static const std::string ICON_PATH;
        WiimoteController &mod;
        cwiid_wiimote_t *wiimotes[4];

        void initializeWiimote(irr::gui::IGUIElement *area, int idx);
};

}