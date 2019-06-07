/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** player AIControlProvider.hpp
*/

#pragma once

#include "indiestudio/player/IControlProvider.hpp"

namespace IndieStudio {

    class AIControlProvider : public IControlProvider {
	public:
        void initConfigurationArea(irr::gui::IGUIEnvironment *guiEnv,
            irr::gui::IGUIElement *area, int playerIdx) override
        {
            (void) guiEnv;
            (void) area;
            (void) playerIdx;
        }

        bool onConfigurationEvent(const irr::SEvent &event,
            irr::gui::IGUIElement *area, int playerIdx) override
        {
            (void) event;
            (void) area;
            (void) playerIdx;
            return false;
        }

        Mappings getPlayerMappings(int playerIdx) const override
        {
            (void) playerIdx;

            return {
                irr::EKEY_CODE::KEY_F1,
                irr::EKEY_CODE::KEY_F1,
                irr::EKEY_CODE::KEY_F1,
                irr::EKEY_CODE::KEY_F1,
                irr::EKEY_CODE::KEY_F1
            };
        }

        const std::string &getIconPath() const override { return ICON_PATH; }

	protected:
	private:
        static const std::string ICON_PATH;
    };

}