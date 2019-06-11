/*
** EPITECH PROJECT, 2019
** OOP_indie_studio_2018
** File description:
** world WorldSettings.hpp
*/

#pragma once

#include <irrlicht.h>
#include <string>
#include "indiestudio/common/ISerializable.hpp"
#include "indiestudio/player/IControlProvider.hpp"

namespace IndieStudio {

    class WorldSettings : public ISerializable {
    public:
        struct Player {
            std::string controlProvider = "AI";
            IControlProvider *controlProviderPtr;
            IControlProvider::Mappings mappings;
        };

        WorldSettings()
        {
            this->players[0].controlProvider = "Keyboard";
        }
    
        std::string name = "My Awesome World";
        unsigned short width = 17;
        unsigned short height = 11;
        std::string worldGenerator = "Basic";
        Player players[4];
        unsigned long elapsedSeconds = 0;

        /* ISerializable implementation */
        void pack(ByteBuffer &buffer) override;
        void unpack(ByteBuffer &buffer) override;

    protected:
    private:
    };

}
