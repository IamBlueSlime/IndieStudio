/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common SoundManager.cpp
*/

#include "indiestudio/common/SoundManager.hpp"

namespace IndieStudio {

    SoundManager::SoundManager() : logger("soundmanager")
    {}

    void SoundManager::init()
    {
        this->registerSound("assets/sounds/shooting_stars.ogg");
        this->registerSound("assets/sounds/countdown.ogg");
        this->registerSound("assets/sounds/bg_music_1.wav");
    }

    void SoundManager::registerSound(std::string id)
    {
        Sound sound;
        sound.buffer = sf::SoundBuffer();

        if (!sound.buffer.loadFromFile(id))
            throw RessourceError("Failed to load the sound " + id);

        this->registerSound(id, sound);
    }

    void SoundManager::registerSound(std::string id, Sound &sound,
        bool overrideExisting)
    {
        this->registry.add(id, std::move(sound), overrideExisting);
        this->logger.debug("Registered sound '" + id + "'.");
    }

    SoundManager::Sound &SoundManager::getSound(const std::string &id)
    {
        if (!this->registry.has(id))
            throw RessourceError("Failed to find sound with id " + id);

        return this->registry.get(id);
    }

}