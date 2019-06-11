/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** IGame.hpp
*/

#pragma once

#include "indiestudio/common/ISoundManager.hpp"
#include "indiestudio/common/ITextureManager.hpp"
#include "indiestudio/common/IFontManager.hpp"
#include "indiestudio/player/IControlProviderManager.hpp"
#include "indiestudio/statistic/IAchievementManager.hpp"
#include "indiestudio/statistic/IStatisticManager.hpp"
#include "indiestudio/world/IWorldManager.hpp"

namespace IndieStudio {

    class IGame {
	public:
		virtual ~IGame() = default;
        virtual IStatisticManager &getStatisticManager() = 0;
        virtual IAchievementManager &getAchievementManager() = 0;
        virtual ISoundManager &getSoundManager() = 0;
        virtual IFontManager &getFontManager() = 0;
        virtual IControlProviderManager &getControlProviderManager() = 0;
        virtual IWorldManager &getWorldManager() = 0;
        virtual ITextureManager &getTextureManager() = 0;

    };

}