/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common SoundManager.hpp
*/

#pragma once

#include "indiestudio/common/ISoundManager.hpp"
#include "indiestudio/common/Logger.hpp"
#include "indiestudio/common/Registry.hpp"

namespace IndieStudio {

    class SoundManager : public ISoundManager {
	public:
		SoundManager();

        void init();
        void registerSound(std::string id) override;
        void registerSound(std::string id, Sound &sound,
            bool overrideExisting = false) override;
        Sound &getSound(const std::string &id);

	protected:
	private:
        Logger logger;
        Registry<std::string, Sound> registry;
    };

}