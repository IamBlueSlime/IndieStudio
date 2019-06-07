/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common ISoundManager.hpp
*/

#pragma once

#include <string>
#include "SFML/Audio.hpp"

namespace IndieStudio {

    class ISoundManager {
	public:
        struct Sound {
            sf::SoundBuffer buffer;
        };

		virtual ~ISoundManager() = default;
        virtual void registerSound(std::string id) = 0;
        virtual void registerSound(std::string id, Sound &sound,
            bool overrideExisting = false) = 0;
        virtual Sound &getSound(const std::string &id) = 0;
    };

}
