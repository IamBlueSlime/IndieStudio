/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common FontManager.hpp
*/

#pragma once

#include "indiestudio/common/IFontManager.hpp"
#include "indiestudio/common/Logger.hpp"
#include "indiestudio/common/Registry.hpp"

namespace IndieStudio {

    class FontManager : public IFontManager {
	public:
		FontManager();

        void init();
        void registerFont(std::string id) override;
        void registerFont(std::string id, Font &font,
            bool overrideExisting = false) override;
        Font &getFont(const std::string &id) override;

	protected:
	private:
        Logger logger;
        Registry<std::string, Font> registry;
    };

}