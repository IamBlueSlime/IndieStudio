/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** player KeyboardControlProvider.hpp
*/

#pragma once

#include "indiestudio/player/IControlProvider.hpp"

namespace IndieStudio {

    class KeyboardControlProvider : public IControlProvider {
	public:
        void initConfigurationArea(irr::gui::IGUIEnvironment *guiEnv,
            irr::gui::IGUIElement *area, int playerIdx) override;
        bool onConfigurationEvent(const irr::SEvent &event,
            irr::gui::IGUIElement *area, int playerIdx) override;

        Mappings getPlayerMappings(int playerIdx) const override;
        std::deque<ECS::Event::EventData> pollEvents() override
            { return std::deque<ECS::Event::EventData>(); }

        const std::string &getIconPath() const override { return ICON_PATH; }

		void pack(ByteBuffer &buffer, int playerIdx) const override;
        void unpack(ByteBuffer &buffer, int playerIdx) override;

	protected:
	private:
        static const std::string ICON_PATH;
        static const irr::EKEY_CODE DEFAULT_KEYS[4][5];
        Mappings playerMappings[4];

        void updateButtons(irr::gui::IGUIElement *area, int playerIdx) const;
        bool isValidKeyboardKey(irr::EKEY_CODE code) const;
    };

}