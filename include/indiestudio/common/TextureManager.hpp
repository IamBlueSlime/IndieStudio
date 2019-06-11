/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** common TextureManager.hpp
*/

#pragma once

#include "indiestudio/common/ITextureManager.hpp"
#include "indiestudio/common/Logger.hpp"
#include "indiestudio/common/Registry.hpp"

namespace IndieStudio {

    class TextureManager : public ITextureManager {
	public:
		TextureManager();

        void init();
        void registerTexture(std::string id) override;
        void registerTexture(std::string id, Texture &Texture,
            bool overrideExisting = false) override;
        Texture &getTexture(const std::string &id) override;

	protected:
	private:
        Logger logger;
        Registry<std::string, Texture> registry;
    };

}