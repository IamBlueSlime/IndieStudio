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

        void onWorldQuit(int playerIdx) override
        {
            (void) playerIdx;
        }

        Mappings getPlayerMappings(int playerIdx) const override
        {
            (void) playerIdx;
            return {};
        }

        std::deque<ECS::Event::EventData> pollEvents() override
            { return std::deque<ECS::Event::EventData>(); }

        const std::string &getIconPath() const override { return ICON_PATH; }

		void pack(ByteBuffer &buffer, int playerIdx) const override
        {
            (void) buffer;
            (void) playerIdx;
        }

        void unpack(ByteBuffer &buffer, int playerIdx) override
        {
            (void) buffer;
            (void) playerIdx;
        }

	protected:
	private:
        static const std::string ICON_PATH;
    };

}